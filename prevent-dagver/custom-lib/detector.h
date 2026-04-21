#ifndef DETECTOR_H
#define DETECTOR_H

#include "contiki.h"
#include "rpl-lite/rpl.h"
#include "rpl-lite/rpl-dag.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/netstack.h"
/*---------------------------------------------------------------------------*/
uint8_t dag_ver_hash(uint8_t input)
{
    return (input * 31) % 256;
}
/*---------------------------------------------------------------------------*/
static enum netstack_ip_action
ip_input(void)
{
    uint8_t proto = 0;
    uint8_t icmp6_type = 0;
    uint8_t rpl_type = 0;
    uipbuf_get_last_header(uip_buf, uip_len, &proto);
    if (proto == UIP_PROTO_ICMP6)
    {
        switch (UIP_IP_BUF->proto)
        {
            case UIP_PROTO_HBHO:
                uip_ext_len = 8;
                break;
            case UIP_PROTO_ROUTING:
                uip_ext_len = 8;
                break;
            default:
                break;
        }
        icmp6_type = uip_buf[UIP_IPH_LEN + uip_ext_len];
        rpl_type = uip_buf[UIP_IPH_LEN + uip_ext_len + 1];

        if (icmp6_type == ICMP6_RPL && rpl_type == RPL_CODE_DIO)
        {
            if (uip_buf[UIP_IPH_LEN + uip_ext_len + 5] != curr_instance.dag.version &&
                dag_ver_hash(uip_buf[UIP_IPH_LEN + uip_ext_len + 5])
                != curr_instance.dag.version)
            {
                LOG_WARN("DIO with unexpected DAG version detected! DIO DAG version: %u, hashed DAG version: %u, current DAG version: %u\n",
                    uip_buf[UIP_IPH_LEN + uip_ext_len + 5], dag_ver_hash(uip_buf[UIP_IPH_LEN + uip_ext_len + 5]), curr_instance.dag.version);
                LOG_INFO("Dropping packet with suspectedly forged DAG version !\n");
                return NETSTACK_IP_DROP;
            }
        }
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

#endif /* DETECTOR_H */