#include "sys/energest.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

// Helper function to convert ticks to seconds
static unsigned long to_seconds(uint64_t time) {
  return (unsigned long)(time / ENERGEST_SECOND);
}

PROCESS(energest_monitor_process, "Energest Monitor");

PROCESS_THREAD(energest_monitor_process, ev, data) {
  static struct etimer periodic_timer;
  PROCESS_BEGIN();
  etimer_set(&periodic_timer, CLOCK_SECOND * 30); // Log every 30 seconds

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
    etimer_reset(&periodic_timer);

    energest_flush(); // Must call before reading times
    
    LOG_INFO("[Energest Data:]    ");
    LOG_INFO_(" CPU: %lu s;", to_seconds(energest_type_time(ENERGEST_TYPE_CPU)));
    LOG_INFO_(" Radio TX: %lu s;", to_seconds(energest_type_time(ENERGEST_TYPE_TRANSMIT)));
    LOG_INFO_(" Radio RX: %lu s;", to_seconds(energest_type_time(ENERGEST_TYPE_LISTEN)));
  }
  PROCESS_END();
}