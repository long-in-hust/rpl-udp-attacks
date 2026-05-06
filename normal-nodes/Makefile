CONTIKI_PROJECT = udp-client udp-root udp-server
all: $(CONTIKI_PROJECT)

CONTIKI=../../..
# MODULES_REL += custom-lib/rpl-lite
MODULES += net/routing/rpl-lite os/services/simple-energest
CFLAGS += -DLOG_CONF_LEVEL_RPL=LOG_LEVEL_INFO

include $(CONTIKI)/Makefile.include
