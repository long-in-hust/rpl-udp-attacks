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
#include "sys/log.h"
#include "net/packetbuf.h"

#include "packet-structs.h"

#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

static volatile bool trust_timer_reset = false;
static volatile bool suspected_node_is_safe = false;
static bool finding_new_parent = false;
static struct simple_udp_connection verif_udp_conn;

static uip_ip6addr_t blacklisted_ipaddr; // IP address of the blacklisted node
/* --------------- PROCESS DECLARATION ---------------- */
PROCESS(detector_process, "Detector Process");
PROCESS(verification_udp_process, "Verification over UDP Process");

/*---------------- PACKET PROCESSING PART BEGINS -----------------*/
static enum netstack_ip_action
ip_input(void)
{
    uint8_t proto = 0;
    uipbuf_get_last_header(uip_buf, uip_len, &proto);

    if (uip_ip6addr_cmp(&UIP_IP_BUF->srcipaddr, &blacklisted_ipaddr)) {
        LOG_INFO("[Netstack Input] Dropping packet from blacklisted IP address: ");
        LOG_INFO_6ADDR(&UIP_IP_BUF->srcipaddr);
        LOG_INFO_("\n");
        return NETSTACK_IP_DROP;
    }

    if (proto == UIP_PROTO_ICMP6)
    {
        LOG_INFO("[Netstack Input] ICMPv6 packet will not participate in the detection mechanism. Processing it as normal.\n");
        return NETSTACK_IP_PROCESS;
    }

    LOG_INFO("[Netstack Input] Packet received through ");
    LOG_INFO_LLADDR(packetbuf_addr(PACKETBUF_ADDR_SENDER));
    LOG_INFO_("\n");

    LOG_INFO("[Netstack Input] Preferred parent: ");
    if (curr_instance.dag.preferred_parent) {
        LOG_INFO_LLADDR(rpl_neighbor_get_lladdr(curr_instance.dag.preferred_parent));
    } else {
        LOG_INFO_("None");
    }
    LOG_INFO_("\n");

    if (curr_instance.dag.preferred_parent &&
        linkaddr_cmp(packetbuf_addr(PACKETBUF_ADDR_SENDER),
                     rpl_neighbor_get_lladdr(curr_instance.dag.preferred_parent))
    )
    {
        LOG_INFO("[Netstack Input] Packet received from preferred parent. Set the trust timer reset flag to true.\n");
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
            LOG_INFO("[UDP verification] Received verification confirmation for node ");
            LOG_INFO_6ADDR(&UDP_BUF_VER_CONF(data)->suspect_ip);
            LOG_INFO(". Marking it as non-blackhole.\n");
            // Here you can add code to update the routing table or take other actions to mark the node as non-blackhole
            suspected_node_is_safe = true;
            break;
        default:
            LOG_INFO("[UDP verification] Unknown packet type received in verification_received function.\n");
    }
}

/*---------------- MAIN THREAD -----------------*/
PROCESS_THREAD(detector_process, ev, data)
{
    PROCESS_BEGIN();
    netstack_ip_packet_processor_add(&packet_processor);
    LOG_INFO("[Detector] Detector process has started.\n");

    static struct etimer startup_timer;
    etimer_set(&startup_timer, CLOCK_SECOND);

    while(!(NETSTACK_ROUTING.node_is_reachable() && curr_instance.dag.preferred_parent))
    {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&startup_timer));
        etimer_reset(&startup_timer);
    }

    // we can trust the preferred parent at the beginning since we have no evidence against it
    suspected_node_is_safe = true;
    etimer_set(&trust_timer, TRUST_SECONDS * CLOCK_SECOND);

    while (1)
    {
        PROCESS_WAIT_EVENT_UNTIL(suspected_node_is_safe);
        LOG_INFO("[Detector] Assuming the preferred parent is safe. Waiting for an event to trigger suspicion...\n");
        PROCESS_WAIT_EVENT_UNTIL(trust_timer_reset || etimer_expired(&trust_timer));
        LOG_INFO("[Detector] Event is triggered\n");
    
        if (trust_timer_reset)
        {
            LOG_INFO("[Detector] Packet received from preferred parent. Resetting trust timer.\n");
            etimer_reset(&trust_timer);
            trust_timer_reset = false;
            continue;
        }
        else if (etimer_expired(&trust_timer))
        {
            LOG_INFO("[Detector] No packet received from preferred parent in the last %d seconds. Suspecting a blackhole.\n", TRUST_SECONDS);
            verification_req_t verif_req;
            verif_req.type = VERIFICATION_REQ_TYPE;
            verif_req.suspect_ip = *rpl_neighbor_get_ipaddr(curr_instance.dag.preferred_parent);
            simple_udp_sendto(&verif_udp_conn, &verif_req, sizeof(verif_req), &rpl_multicast_addr);
            suspected_node_is_safe = false;
            continue;
        }
        LOG_INFO("[Detector] Something else happened.\n");
    }

    PROCESS_END();
}

/* ---------------- VERIFICATION PACKETS HANDLING ------------------ */
PROCESS_THREAD(verification_udp_process, ev, data)
{
    
    PROCESS_BEGIN();
    netstack_ip_packet_processor_add(&packet_processor);
    LOG_INFO("[UDP verification] Verification UDP process has started.\n");

    static struct etimer startup_timer;
    etimer_set(&startup_timer, CLOCK_SECOND);

    while(!(NETSTACK_ROUTING.node_is_reachable() && curr_instance.dag.preferred_parent)) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&startup_timer));
        etimer_reset(&startup_timer);
    }


    simple_udp_register(&verif_udp_conn, UDP_PORT_BHVERIF, NULL,
                      UDP_PORT_BHVERIF, verification_received_callback);
    
    etimer_set(&suspect_timer, SUSPECT_SECONDS * CLOCK_SECOND);
    while(1) {
        while (finding_new_parent)
        {
            LOG_INFO("[UDP verification] Finding new preferred parent. Waiting for the new preferred parent to be set...\n");
            if (curr_instance.dag.preferred_parent) {
                LOG_INFO("[UDP verification] New preferred parent is set: ");
                LOG_INFO_6ADDR(rpl_neighbor_get_ipaddr(curr_instance.dag.preferred_parent));
                LOG_INFO_("\n");
                finding_new_parent = false;
                suspected_node_is_safe = true;
                trust_timer_reset = true;
                break;
            }
            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&startup_timer));
            etimer_reset(&startup_timer);
        }

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&suspect_timer) || suspected_node_is_safe);
        LOG_INFO("[UDP verification] Event is triggered\n");

        if (curr_instance.dag.preferred_parent && curr_instance.dag.preferred_parent->rank == ROOT_RANK) {
            LOG_INFO("[UDP verification] Preferred parent is the root. Skipping verification.\n");
            continue;
        }

        if (suspected_node_is_safe)
        {
            LOG_INFO("[UDP verification] Suspected parent node ");
            LOG_INFO_6ADDR(rpl_neighbor_get_ipaddr(curr_instance.dag.preferred_parent));
            LOG_INFO_(" is not a blackhole.\n");
            etimer_reset(&suspect_timer);
        }
        else if (etimer_expired(&suspect_timer))
        {
            LOG_INFO("[UDP verification] No verification confirmation received within %d seconds. Confirming that the suspected node ", SUSPECT_SECONDS);
            LOG_INFO_6ADDR(rpl_neighbor_get_ipaddr(curr_instance.dag.preferred_parent));
            LOG_INFO_(" is a blackhole.\n");
            // Blacklist the suspected node and trigger local repair            
            uip_ip6addr_copy(&blacklisted_ipaddr, rpl_neighbor_get_ipaddr(curr_instance.dag.preferred_parent));

            nbr_table_remove(rpl_neighbors, curr_instance.dag.preferred_parent);
            curr_instance.dag.preferred_parent = NULL;

            LOG_INFO("[UDP verification] Triggering local update to find a new preferred parent.\n");
            rpl_dag_update_state();

            finding_new_parent = true;
            etimer_set(&startup_timer, CLOCK_SECOND);
        }
    }
    PROCESS_END();
}

#endif /* DETECTOR_H */