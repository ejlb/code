all: appender.s host.c
	as -o appender.o --defsym NDEBUG=1 appender.s
	ld -s -o appender appender.o
	gcc-4.4 host.c -o host
	gcc-4.4 detector.c -o detect/detector
	@echo -n "text size = "
	@size ./appender | tail -n 1 | awk '{print $$1}'

debug: appender.s host.c
	gcc -g appender.s -o appender
	gcc-4.4 host.c -o host
	@echo -n "text size = "
	@size ./appender | tail -n 1 | awk '{print $$1}'

clean:
	rm host appender detect/detector *.o

