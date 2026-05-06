CONTIKI_PROJECT = udp-client udp-root udp-server
all: $(CONTIKI_PROJECT)

CONTIKI=../../..
# MODULES_REL += custom-lib/rpl-lite
MODULES += net/routing/rpl-lite os/services/simple-energest # os/services/orchestra
# MAKE_MAC = MAKE_MAC_TSCH

include $(CONTIKI)/Makefile.include
