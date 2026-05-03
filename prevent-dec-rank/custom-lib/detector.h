#ifndef DETECTOR_H
#define DETECTOR_H

#include "contiki.h"
#include "custom-lib/rpl-lite/rpl.h"
#include "custom-lib/rpl-lite/rpl-dag.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/netstack.h"

/*---------------------------------------------------------------------------*/

PROCESS(network_stabilisation_checker, "DIS Periodic Counter");

/*---------------------------------------------------------------------------*/

PROCESS_THREAD(network_stabilisation_checker, ev, data)
{
    static struct etimer periodic_timer;
    clock_time_t better_parent_interval;

    PROCESS_BEGIN();

    while (1)
    {
        etimer_set(&periodic_timer, CLOCK_SECOND * 1);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
        better_parent_interval = curr_instance.dag.preferred_parent ? (clock_time() - curr_instance.dag.preferred_parent->better_parent_since) : 0;
        if (curr_instance.dag.preferred_parent && 
            better_parent_interval > 30 * CLOCK_SECOND) {
            /* If a preferred parent is stablised, add a preference for parents with a 
            valid hop count that is lower than the current preferred parent. */
            LOG_INFO("Preferred parent has been preferred for %lu seconds\n", better_parent_interval / CLOCK_SECOND);
            stabilized_parent = true;
        }
    }

    PROCESS_END();
}

#endif /* DETECTOR_H */