#ifndef DAGHASH_H
#define DAGHASH_H

#include "contiki.h"
#include "rpl-lite/rpl.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/netstack.h"

/*---------------------------------------------------------------------------*/
static enum netstack_ip_action
ip_input(void)
{
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

// uint16_t dag_rank_hash(uint16_t input)
// {
//     return (input * 31) % 65536;
// }

#endif /* DAGHASH_H */