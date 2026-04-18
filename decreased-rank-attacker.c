#include "contiki.h"
#include "net/routing/routing.h"
#include "random.h"
#include "net/netstack.h"
#include <stdint.h>
#include <inttypes.h>
#include "net/routing/rpl-lite/rpl.h"
#include "net/packetbuf.h"
#include "net/ipv6/uipbuf.h"
#include "net/ipv6/uip-icmp6.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define WITH_SERVER_REPLY  1

/*---------------------------------------------------------------------------*/
PROCESS(decr_rank_attacker, "Decreased rank attacker");
AUTOSTART_PROCESSES(&decr_rank_attacker);

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
  .process_output = ip_output
};
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(decr_rank_attacker, ev, data)
{
  static struct etimer periodic_timer;

  PROCESS_BEGIN();

  netstack_ip_packet_processor_add(&packet_processor);

  while (1) {
    etimer_set(&periodic_timer, 0.01 * CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
