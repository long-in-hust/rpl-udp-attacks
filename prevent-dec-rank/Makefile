CONTIKI_PROJECT = udp-client udp-root udp-server
all: $(CONTIKI_PROJECT)

CONTIKI=../../..
MODULES += os/services/simple-energest
MODULES_REL += ./custom-lib ./custom-lib/rpl-lite

include $(CONTIKI)/Makefile.include
