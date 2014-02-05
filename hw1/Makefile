all: receiver polute sender receiver2 sender2

receiver: receiver.c 
	gcc -O0 -g -o $@ $^

receiver2: receiver2.c lock.h
	gcc -O0 -g -o $@ $<

polute: polute.c
	gcc -O0 -g -o $@ $^

sender: sender.c
	gcc -O0 -g -o $@ $^

sender2: sender2.c lock.h
	gcc -O0 -g -o $@ $<
