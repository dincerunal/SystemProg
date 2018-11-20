#Sample makefile

CFLAGS=-c -g -Wall
OBJS=a.o b.o
ALLOBJS=$(wildcard *.o)

sample: a.o b.o
	$(CC) -o $@ sample.c $(OBJS)

a.o: a.c a.h
	$(CC) $(CFLAGS) $<

b.o: b.c b.h
	$(CC) $(CFLAGS) $<
    
clean:
	$(RM) $(ALLOBJS) sample
install:
	cp sample ./app

print: *.c
	lpr -p $?
	touch print
	