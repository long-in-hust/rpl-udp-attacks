#ifndef DETECTOR_H
#define DETECTOR_H

#include "contiki.h"
#include "custom-lib/rpl-lite/rpl.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/netstack.h"

static const float RPL_DIS_PREVENTION_THRESHOLD = 0.5; // DIS should be less than 50% of total packets
static int total_packets = 0;
static int dis_packets = 0;

/*---------------------------------------------------------------------------*/
static enum netstack_ip_action
ip_input(void)
{
    total_packets++;
    uint8_t proto = 0;
    uipbuf_get_last_header(uip_buf, uip_len, &proto);
    if (proto != UIP_PROTO_ICMP6 ||
        UIP_ICMP_BUF->type != ICMP6_RPL ||
        UIP_ICMP_BUF->icode != RPL_CODE_DIS) {
        return NETSTACK_IP_PROCESS;
    }
    dis_packets++;
    float dis_ratio = (float)(dis_packets + 1) / (float)(total_packets + 2);
    if (dis_ratio > RPL_DIS_PREVENTION_THRESHOLD) {
        return NETSTACK_IP_DROP;
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
/*---------------------------------------------------------------------------*/

PROCESS(dis_counter, "DIS Periodic Counter");

/*---------------------------------------------------------------------------*/

PROCESS_THREAD(dis_counter, ev, data)
{
    static struct etimer periodic_timer;

    PROCESS_BEGIN();

    while (1)
    {
        etimer_set(&periodic_timer, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
        total_packets = 0;
        dis_packets = 0;
        etimer_reset(&periodic_timer);
    }

    PROCESS_END();
}

#endif /* DETECTOR_H */