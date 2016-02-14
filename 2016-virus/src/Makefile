all: appender.s host.c
	gcc -m32 -g appender.s -o appender
	gcc -m32 host.c -o host
	gcc -m32 detector.c -o detect/detector
	@echo -n "text size = "
	@size ./appender | tail -n 1 | awk '{print $$1}'

clean:
	rm host appender detect/detector *.o

