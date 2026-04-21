#ifndef DETECTOR_H
#define DETECTOR_H

#include "contiki.h"

// #ifdef LOG_LEVEL_RPL
// #undef LOG_LEVEL_RPL
// #define LOG_LEVEL_RPL LOG_LEVEL_INFO
// #endif

#include "rpl-lite/rpl.h"
#include "rpl-lite/rpl-dag.h"
#include "rpl-lite/rpl-neighbor.h"
#include "net/nbr-table.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/netstack.h"

static uip_ip6addr_t blacklisted_ipaddr; // IP address of the blacklisted node
static int8_t need_version_repair = 0; // Flag to indicate whether DAG version check is needed
static bool blacklist_timer_needs_reset = false;
/*---------------------------------------------------------------------------*/
static enum netstack_ip_action
ip_input(void)
{
    uint8_t proto = 0;
    uint8_t icmp6_type = 0;
    uint8_t rpl_type = 0;
    uipbuf_get_last_header(uip_buf, uip_len, &proto);

    uip_ip6addr_t root_ipaddr;

    LOG_INFO("Received a packet from ");
    LOG_INFO_6ADDR(&UIP_IP_BUF->srcipaddr);
    LOG_INFO_("\n");

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

        if (icmp6_type == ICMP6_RPL) {
            if (rpl_type == RPL_CODE_DIO)
            {
                if (!NETSTACK_ROUTING.get_root_ipaddr(&root_ipaddr)) {
                    LOG_WARN("Could not get root IP address! Cannot perform DAG version verification. Letting the packet through.\n");
                    return NETSTACK_IP_PROCESS;
                }
                if (uip_ip6addr_cmp(&UIP_IP_BUF->srcipaddr, &blacklisted_ipaddr)) {
                    LOG_INFO("Received DIO from a blacklisted node, dropping the packet.\n");
                    return NETSTACK_IP_DROP;
                }
                if (need_version_repair) {
                    LOG_INFO("Need version repair after having received a malformed DIO.\n");
                    if (curr_instance.dag.version != uip_buf[UIP_IPH_LEN + uip_ext_len + 5]) {
                        need_version_repair = 0;
                        curr_instance.dag.version = uip_buf[UIP_IPH_LEN + uip_ext_len + 5];
                        LOG_INFO("Updated DAG version to the version in the received DIO: %u\n", curr_instance.dag.version);
                        return NETSTACK_IP_PROCESS;
                    }
                    LOG_INFO("DAG version received is still the wrong one this node is bearing!\n");
                    return NETSTACK_IP_DROP;
                }
                // Only check the DAG version if it is different from the current one, to reduce unnecessary computations and false positives
                if (uip_buf[UIP_IPH_LEN + uip_ext_len + 5] != curr_instance.dag.version)
                {
                    if (dag_ver_hash(uip_buf[UIP_IPH_LEN + uip_ext_len + 5])
                        != curr_instance.dag.version)
                    {
                        LOG_WARN("DIO with unexpected DAG version detected! DIO DAG version: %u, hashed DAG version: %u, current DAG version: %u\n",
                            uip_buf[UIP_IPH_LEN + uip_ext_len + 5], dag_ver_hash(uip_buf[UIP_IPH_LEN + uip_ext_len + 5]), curr_instance.dag.version);
                        LOG_INFO("Dropping packet with suspectedly forged DAG version !\n");
                        
                        if (!rpl_dag_root_is_root())
                        {
                            if (!curr_instance.dag.preferred_parent) {
                                LOG_INFO("However, we don't have a preferred parent yet! Letting the packet through to allow us to join the DAG and get a valid DAG version.\n");
                                return NETSTACK_IP_PROCESS;
                            }

                            if (curr_instance.dag.preferred_parent)
                            {
                                // root should be the source of truth for the correct DAG version
                                if (!uip_ip6addr_cmp(&UIP_IP_BUF->srcipaddr, &root_ipaddr)
                                    && uip_ip6addr_cmp(&UIP_IP_BUF->srcipaddr, rpl_neighbor_get_ipaddr(curr_instance.dag.preferred_parent)
                                    )
                                ) {
                                    LOG_WARN("Current preferred parent with rank %u was malformed! Removing current preferred parent from the neighbor table.\n", curr_instance.dag.preferred_parent->rank);
                                    nbr_table_remove(rpl_neighbors, curr_instance.dag.preferred_parent);
                                    /* Remove the preferred parent from the neighbor table */
                                    curr_instance.dag.preferred_parent = NULL;
                                    LOG_INFO("Triggering local repair to find a new preferred parent.\n");
                                    rpl_dag_update_state();
                                    need_version_repair = 1;
                                }
                                if (!uip_ip6addr_cmp(&UIP_IP_BUF->srcipaddr, &blacklisted_ipaddr)) {
                                    LOG_INFO("Blacklisting the source IP address of the malformed DIO: ");
                                    LOG_INFO_6ADDR(&UIP_IP_BUF->srcipaddr);
                                    LOG_INFO_("\n");
                                    uip_ip6addr_copy(&blacklisted_ipaddr, &UIP_IP_BUF->srcipaddr);
                                    blacklist_timer_needs_reset = true;
                                }
                            }
                        }
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

PROCESS(blacklist_resetter_process, "Blacklist resetter process");

PROCESS_THREAD(blacklist_resetter_process, ev, data)
{
    static struct etimer reset_timer;
    uip_ip6addr_t all_zero_addr;
    uip_ip6addr(&all_zero_addr, 0, 0, 0, 0, 0, 0, 0, 0);

    PROCESS_BEGIN();

    while (1)
    {
        etimer_set(&reset_timer, 60 * CLOCK_SECOND); // Reset blacklist every 60 seconds
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&reset_timer) || blacklist_timer_needs_reset);
        blacklist_timer_needs_reset = false;
        LOG_INFO("Resetting blacklisted IP address.\n");
        uip_ip6addr_copy(&blacklisted_ipaddr, &all_zero_addr);
    }

    PROCESS_END();
}

#endif /* DETECTOR_H */