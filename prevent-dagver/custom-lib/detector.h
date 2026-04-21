#ifndef DETECTOR_H
#define DETECTOR_H

#include "contiki.h"

// #ifdef LOG_LEVEL_RPL
// #undef LOG_LEVEL_RPL
// #define LOG_LEVEL_RPL LOG_LEVEL_INFO
// #endif

#include "rpl-lite/rpl.h"
#include "rpl-lite/rpl-dag.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/netstack.h"
/*---------------------------------------------------------------------------*/
static enum netstack_ip_action
ip_input(void)
{
    uint8_t proto = 0;
    uint8_t icmp6_type = 0;
    uint8_t rpl_type = 0;
    uipbuf_get_last_header(uip_buf, uip_len, &proto);

    uip_ip6addr_t root_ipaddr;
    if (!NETSTACK_ROUTING.get_root_ipaddr(&root_ipaddr)) {
        LOG_WARN("Could not get root IP address! Cannot perform DAG version verification. Letting the packet through.\n");
        return NETSTACK_IP_PROCESS;
    }

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
            uint16_t dio_rank = (uip_buf[UIP_IPH_LEN + uip_ext_len + 6] << 8) | uip_buf[UIP_IPH_LEN + uip_ext_len + 7];
            if (uip_buf[UIP_IPH_LEN + uip_ext_len + 5] != curr_instance.dag.version)
            {
                if (dag_ver_hash(uip_buf[UIP_IPH_LEN + uip_ext_len + 5])
                    != curr_instance.dag.version)
                {
                    LOG_WARN("DIO with unexpected DAG version detected! DIO DAG version: %u, hashed DAG version: %u, current DAG version: %u\n",
                        uip_buf[UIP_IPH_LEN + uip_ext_len + 5], dag_ver_hash(uip_buf[UIP_IPH_LEN + uip_ext_len + 5]), curr_instance.dag.version);
                    LOG_INFO("Dropping packet with suspectedly forged DAG version !\n");
                    if (curr_instance.dag.preferred_parent && dio_rank == curr_instance.dag.preferred_parent->rank) {
                        LOG_WARN("Current preferred parent was malformed! Removing current preferred parent from the neighbor table.\n");
                        rpl_nbr_t *nbr = curr_instance.dag.preferred_parent;
                        curr_instance.dag.preferred_parent = NULL;
                        nbr->rank = RPL_INFINITE_RANK;
                        rpl_dag_update_state();
                    }
                    return NETSTACK_IP_DROP;
                }
                else {
                    if (curr_instance.dag.preferred_parent && dio_rank != curr_instance.dag.preferred_parent->rank) {
                        LOG_WARN("DIO with unexpected rank detected! DIO rank: %u, current preferred parent rank: %u\n", dio_rank, curr_instance.dag.preferred_parent->rank);
                        LOG_INFO("Dropping packet with suspectedly forged DAG version !\n");
                        return NETSTACK_IP_DROP;
                    }
                }
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