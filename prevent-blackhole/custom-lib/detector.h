#ifndef DETECTOR_H
#define DETECTOR_H

#define TRUST_SECONDS 5

#include "contiki.h"
#include "net/routing/rpl-lite/rpl.h"
#include "net/routing/rpl-lite/rpl-dag.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"

#include "packet-structs.h"

static bool trust_timer_reset = false;
static struct simple_udp_connection verif_udp_conn;

/* ----------------------- HELPING FUNCTIONS SECTION BEGINS -------------------------*/

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

/* ----------------------- HELPING FUNCTIONS SECTION ENDS -------------------------*/

/*---------------- PACKET PROCESSING PART BEGINS -----------------*/
static enum netstack_ip_action
ip_input(void)
{
    uint8_t proto = 0;
    uipbuf_get_last_header(uip_buf, uip_len, &proto);

    LOG_INFO("Packet length: %u\n", uip_len);

    uip_ext_len = calculate_uip_ip6_ext_hdr_len(uip_buf);

    if (proto == UIP_PROTO_ICMP6)
    {
        if (UIP_ICMP_BUF->type == ICMP6_RPL) {
            return NETSTACK_IP_PROCESS;
        }
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

/* --------------- CALLBACK FUNCTIONS ---------------- */
static void verification_received_callback(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
    uip_ip6addr_t suspect_ip, initiator_ip;
    switch (data[0]) // the first byte of the payload is the packet type identifier
    {
        case VERIFICATION_REQ_TYPE:
            uip_ip6addr_copy(&suspect_ip, &UDP_BUF_VER_REQ(data)->suspect_ip);
            break;
        case SUSPECT_VERIFICATION_REQ_TYPE:
            uip_ip6addr_copy(&initiator_ip, &UDP_BUF_SUSPECT_VER_REQ(data)->initiator_ip);
            break;
        case SUSPECT_CLEAN_RESP_TYPE:
            uip_ip6addr_copy(&initiator_ip, &UDP_BUF_SUSPECT_CLEAN_RESP(data)->initiator_ip);
            uip_ip6addr_copy(&suspect_ip, &UDP_BUF_SUSPECT_CLEAN_RESP(data)->suspect_ip);
            break;
        case VERIFICATION_CONF_TYPE:
            trust_timer_reset = true;
            uip_ip6addr_copy(&suspect_ip, &UDP_BUF_VER_CONF(data)->suspect_ip);
            break;
        default:
            LOG_INFO("Unknown packet type received in verification_received function.\n");
    }
}

/*---------------- MAIN THREAD -----------------*/
PROCESS(detector_process, "Detector Process");

PROCESS_THREAD(detector_process, ev, data)
{

    static struct etimer trust_timer;

    PROCESS_BEGIN();

    NETSTACK_ROUTING.root_start();
    netstack_ip_packet_processor_add(&packet_processor);

    etimer_set(&trust_timer, TRUST_SECONDS * CLOCK_SECOND);
    while (1)
    {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&trust_timer) || trust_timer_reset);
        if (etimer_expired(&trust_timer) && !trust_timer_reset &&
             NETSTACK_ROUTING.node_is_reachable() && curr_instance.dag.preferred_parent)
        {
            LOG_INFO("No packet received from preferred parent in the last %d seconds. Suspecting a blackhole.\n", TRUST_SECONDS);
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
    while(1) {
        PROCESS_YIELD();
    }
    PROCESS_END();
}

#endif /* DETECTOR_H */