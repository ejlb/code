CC=gcc
RM=rm
RMDIR=rmdir
OBJS=main.o usage.o nk_process_packet.o nk_printf.o getset.o nk_resolve.o  
CFLAGS=-Wall -O2 -c -s -I ./include/  -fPIC
LINK=-s -lpcap -lnet -ldl -o  
PROG=nark
OBJF=.objs

all: check $(OBJS)
	make -C print_proto
	make -C convert
	$(CC) $(OBJS) `cat $(OBJF)` $(LINK) $(PROG)

check:
	@if [ -f "$(OBJF)" ] ; then rm "$(OBJF)" ; fi
	
clean: 
	make -i clear
	make -C print_proto clean
	make -C convert clean

clear:	
	$(RM) $(OBJS) $(PROG)

.c.o:
	$(CC) $(CFLAGS) $<
