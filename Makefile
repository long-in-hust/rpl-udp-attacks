CONTIKI_PROJECT = udp-client udp-root udp-server dis-flooder dag-version-dio general-blackhole-node
all: $(CONTIKI_PROJECT)

CONTIKI=../..
MODULES += net/routing/rpl-lite

include $(CONTIKI)/Makefile.include
