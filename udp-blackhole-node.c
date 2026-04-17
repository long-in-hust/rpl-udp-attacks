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
PROCESS(dag_version_process, "RPL UDP Blackhole");
AUTOSTART_PROCESSES(&dag_version_process);

/*---------------------------------------------------------------------------*/
static enum netstack_ip_action
ip_input(void)
{
  uint8_t proto = 0;
  uip_ip6addr_t check_src_addr;
  // uip_ip6addr(&check_src_addr, 0xfd00, 0, 0, 0, 0x203, 0x3, 0x3, 0x3);

  uipbuf_get_last_header(uip_buf, uip_len, &proto);
  LOG_INFO("Incoming packet proto: %d, from ", proto);
  LOG_INFO_6ADDR(&UIP_IP_BUF->srcipaddr);
  LOG_INFO_("\n");
  if (proto != UIP_PROTO_UDP && proto != UIP_PROTO_HBHO) {
    return NETSTACK_IP_PROCESS;
  }
  if (proto == UIP_PROTO_HBHO && uip_buf[40] != UIP_PROTO_UDP) {
    return NETSTACK_IP_PROCESS;
  }
  LOG_INFO("UDP packet from ");
  LOG_INFO_6ADDR(&UIP_IP_BUF->srcipaddr);
  LOG_INFO_("\n");
  
  // if (uip_ip6addr_cmp(&UIP_IP_BUF->srcipaddr, &check_src_addr)) {
  LOG_INFO("Dropping packet !\n");
  return NETSTACK_IP_DROP;
  // }

  // return NETSTACK_IP_PROCESS;
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
PROCESS_THREAD(dag_version_process, ev, data)
{
  PROCESS_BEGIN();

  netstack_ip_packet_processor_add(&packet_processor);

  while (1) {
    PROCESS_YIELD();
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
