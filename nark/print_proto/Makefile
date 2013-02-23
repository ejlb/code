CC=gcc
RM=rm
OBJS=print_proto/print_tcp.o print_proto/print_udp.o print_proto/print_icmp.o print_proto/print_arp.o  print_proto/print_mac.o print_proto/print_payload.o
CFLAGS=-O2 -Wall -s -c -I ./../include/

all: $(OBJS)
	@echo -n $(OBJS) >> ./../.objs    
	@echo -n " " >> ./../.objs    
    
clean: 
	$(RM) $(OBJS) 
	
.c.o: 
	$(CC) $(CFLAGS) $<
