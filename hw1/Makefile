all: receiver polute sender receiver2 sender2 receiver3 sender3 sender4 receiver4

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

sender3: sender3.c
	gcc -O0 -g -o $@ $<

receiver3: receiver3.c
	gcc -O0 -mpreferred-stack-boundary=12 -g -o $@ $<

sender4: sender4.c
	gcc -O0 -g -o $@ $<

receiver4: receiver4.c
	gcc -O0 -mpreferred-stack-boundary=12 -g -o $@ $<

clean: 
	rm -rf receiver polute sender receiver2 sender2 receiver3 sender3 sender4 receiver4 fd_empty.tmp fd_fill.tmp fd_bit.tmp
