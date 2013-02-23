CC=gcc
RM=rm
OBJS=convert/convert_icmp.o convert/convert_arp.o convert/convert_type.o convert/convert_port.o
CFLAGS=-O2 -Wall -s -c -I ./../include/

all: $(OBJS)
	@echo $(OBJS) >> ./../.objs

clean: 
	$(RM) $(OBJS) 
	
.c.o: 
	$(CC) $(CFLAGS) $<
