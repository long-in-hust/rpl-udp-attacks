CONTIKI_PROJECT = udp-client udp-root udp-server dis-flooder
all: $(CONTIKI_PROJECT)

CONTIKI=../..
MODULES += net/routing/rpl-lite
include $(CONTIKI)/Makefile.include
