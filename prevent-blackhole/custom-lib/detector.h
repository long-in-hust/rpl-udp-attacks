#ifndef DETECTOR_H
#define DETECTOR_H

#define TRUST_SECONDS 5
#define SUSPECT_SECONDS 10

#include "contiki.h"
#include "net/routing/rpl-lite/rpl.h"
#include "net/routing/rpl-lite/rpl-dag.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"

#include "packet-structs.h"

static bool trust_timer_reset = false;
static bool suspected_node_is_safe = true;
static struct simple_udp_connection verif_udp_conn;

static uip_ip6addr_t blacklisted_ipaddr; // IP address of the blacklisted node

/* ----------------------- HELPING FUNCTIONS SECTION BEGINS -------------------------*/
/*
uint16_t calculate_uip_ip6_ext_hdr_len(uint8_t *buf) {
    uint8_t* buf_ptr = buf;
    uint16_t total_ext_hdr_len = 0;
    uint8_t next_hdr_type = (*buf_ptr + 16);
    if (next_hdr_type != UIP_PROTO_HBHO && next_hdr_type != UIP_PROTO_ROUTING && next_hdr_type != UIP_PROTO_DESTO
            && next_hdr_type != UIP_PROTO_ROUTING && next_hdr_type != UIP_PROTO_NONE)
    {
        return 0;
    }

    buf_ptr += UIP_IPH_LEN;
    bool last_ext_hdr_reached = false;
    while (buf_ptr != NULL && !last_ext_hdr_reached)
    {
        next_hdr_type = *buf_ptr;
        if (next_hdr_type != UIP_PROTO_HBHO && next_hdr_type != UIP_PROTO_ROUTING && next_hdr_type != UIP_PROTO_DESTO
            && next_hdr_type != UIP_PROTO_ROUTING && next_hdr_type != UIP_PROTO_NONE)
        {
            last_ext_hdr_reached = true;
        }
        total_ext_hdr_len += *(buf_ptr + 1);
        buf_ptr += *(buf_ptr + 1);
    }
    return total_ext_hdr_len;
}
*/

/* ----------------------- HELPING FUNCTIONS SECTION ENDS -------------------------*/

/*---------------- PACKET PROCESSING PART BEGINS -----------------*/
static enum netstack_ip_action
ip_input(void)
{
    uint8_t proto = 0;
    uipbuf_get_last_header(uip_buf, uip_len, &proto);

    if (uip_ip6addr_cmp(&UIP_IP_BUF->srcipaddr, &blacklisted_ipaddr)) {
        LOG_INFO("Dropping packet from blacklisted IP address: ");
        LOG_INFO_6ADDR(&UIP_IP_BUF->srcipaddr);
        LOG_INFO_("\n");
        return NETSTACK_IP_DROP;
    }

    if (proto == UIP_PROTO_ICMP6)
    {
        return NETSTACK_IP_PROCESS;
    }

    if (curr_instance.dag.preferred_parent &&
        uip_ip6addr_cmp(&UIP_IP_BUF->srcipaddr, rpl_neighbor_get_ipaddr(curr_instance.dag.preferred_parent)))
    {
        trust_timer_reset = true;
    }
    return NETSTACK_IP_PROCESS;
}
/*---------------------------------------------------------------------------*/
static enum netstack_ip_action
ip_output(const linkaddr_t *localdest)
{
    return NETSTACK_IP_PROCESS;
}
/*---------------------------------------------------------------------------*/
struct netstack_ip_packet_processor packet_processor = {
    .process_input = ip_input,
    .process_output = ip_output};
/*---------------- PACKET PROCESSING PART ENDS -----------------*/

/*---------------- SUSPICION HANDLING -----------------*/
/*-----------------------------------------------------*/

static struct etimer trust_timer;
static struct etimer suspect_timer;

/* --------------- CALLBACK FUNCTIONS ---------------- */
static void verification_received_callback(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
    switch (UIP_BUF_BH_VERIF(data)->type) // the first byte of the payload is the packet type identifier
    {
        case VERIFICATION_REQ_TYPE:
            suspect_verification_req_t susp_verif_req;
            susp_verif_req.type = SUSPECT_VERIFICATION_REQ_TYPE;
            susp_verif_req.initiator_ip = *sender_addr;
            simple_udp_sendto(&verif_udp_conn, &susp_verif_req, sizeof(susp_verif_req), &UDP_BUF_VER_REQ(data)->suspect_ip);
            break;
        case SUSPECT_VERIFICATION_REQ_TYPE:
            suspect_clean_resp_t clean_resp;
            clean_resp.type = SUSPECT_CLEAN_RESP_TYPE;
            clean_resp.initiator_ip = UDP_BUF_SUSPECT_VER_REQ(data)->initiator_ip;
            simple_udp_sendto(&verif_udp_conn, &clean_resp, sizeof(clean_resp), sender_addr);
            break;
        case SUSPECT_CLEAN_RESP_TYPE:
            verification_conf_t verif_conf;
            verif_conf.type = VERIFICATION_CONF_TYPE;
            // the suspected node's IP address is the source IP address of the received packet
            verif_conf.suspect_ip = *sender_addr;
            simple_udp_sendto(&verif_udp_conn, &verif_conf, sizeof(verif_conf), &UDP_BUF_SUSPECT_CLEAN_RESP(data)->initiator_ip);
            break;
        case VERIFICATION_CONF_TYPE:
            LOG_INFO("Received verification confirmation for node ");
            LOG_INFO_6ADDR(&UDP_BUF_VER_CONF(data)->suspect_ip);
            LOG_INFO(". Marking it as non-blackhole.\n");
            // Here you can add code to update the routing table or take other actions to mark the node as non-blackhole
            suspected_node_is_safe = true;
            break;
        default:
            LOG_INFO("Unknown packet type received in verification_received function.\n");
    }
}

/*---------------- MAIN THREAD -----------------*/
PROCESS(detector_process, "Detector Process");

PROCESS_THREAD(detector_process, ev, data)
{
    PROCESS_BEGIN();

    NETSTACK_ROUTING.root_start();
    netstack_ip_packet_processor_add(&packet_processor);

    etimer_set(&trust_timer, TRUST_SECONDS * CLOCK_SECOND);
    while (1)
    {
        PROCESS_WAIT_EVENT_UNTIL(suspected_node_is_safe);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&trust_timer) || trust_timer_reset);
        if (etimer_expired(&trust_timer) && !trust_timer_reset &&
             NETSTACK_ROUTING.node_is_reachable() && curr_instance.dag.preferred_parent)
        {
            LOG_INFO("No packet received from preferred parent in the last %d seconds. Suspecting a blackhole.\n", TRUST_SECONDS);
            verification_req_t verif_req;
            verif_req.type = VERIFICATION_REQ_TYPE;
            verif_req.suspect_ip = *rpl_neighbor_get_ipaddr(curr_instance.dag.preferred_parent);
            simple_udp_sendto(&verif_udp_conn, &verif_req, sizeof(verif_req), &rpl_multicast_addr);
            suspected_node_is_safe = false;
        }
        else if (trust_timer_reset)
        {
            LOG_INFO("Packet received from preferred parent. Resetting trust timer.\n");
            etimer_reset(&trust_timer);
            trust_timer_reset = false;
        }
    }

    PROCESS_END();
}

/* ---------------- VERIFICATION PACKETS HANDLING ------------------ */
PROCESS(verification_udp_process, "Verification over UDP Process");
PROCESS_THREAD(verification_udp_process, ev, data)
{
    
    PROCESS_BEGIN();
    simple_udp_register(&verif_udp_conn, UDP_PORT_BHVERIF, NULL,
                      UDP_PORT_BHVERIF, verification_received_callback);
    etimer_set(&suspect_timer, SUSPECT_SECONDS * CLOCK_SECOND);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&suspect_timer) || suspected_node_is_safe);
        if (suspected_node_is_safe)
        {
            LOG_INFO("Suspected parentnode is not a blackhole.\n");
            etimer_reset(&suspect_timer);
        }
        else if (etimer_expired(&suspect_timer))
        {
            LOG_INFO("No verification confirmation received within %d seconds. Confirming that the suspected node is a blackhole.\n", SUSPECT_SECONDS);
            // Blacklist the suspected node and trigger local repair
            nbr_table_remove(rpl_neighbors, curr_instance.dag.preferred_parent);
            curr_instance.dag.preferred_parent = NULL;
            uip_ip6addr_copy(&blacklisted_ipaddr, rpl_neighbor_get_ipaddr(curr_instance.dag.preferred_parent));

            LOG_INFO("Triggering local update to find a new preferred parent.\n");
            rpl_dag_update_state();

            // reset the suspicion status for the next round of detection
            suspected_node_is_safe = true;
        }
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&trust_timer));
    }
    PROCESS_END();
}

#endif /* DETECTOR_H */