CONTIKI_PROJECT = udp-client udp-root udp-server
all: $(CONTIKI_PROJECT)

CONTIKI=../../..
MODULES += net/routing/rpl-lite os/services/simple-energest
MODULES_REL += ./custom-lib

include $(CONTIKI)/Makefile.include
