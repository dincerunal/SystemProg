sample: a.o b.o 
	gcc -o sample sample.c a.o b.o

a.o: a.h a.c
	gcc -c a.c

b.o: b.h b.c
	gcc -c b.c
    
clean:
	rm -f *.o sample

install:
	cp sample ./app