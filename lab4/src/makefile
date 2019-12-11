CC = g++
LD = g++
CCFLAGS = -Wall -pedantic
LDFLAGS =



main.out: main.o stack.o sort.o -lrt
	$(LD) $(LDFLAGS) -o main.out main.o stack.o sort.o -pthread  -lrt
main.o: main.cpp stack.h sort.h sort.c stack.c
	$(CC) $(CCFLAGS) -c main.cpp -pthread -lrt
sort.o: sort.c sort.h
	$(CC) $(CCFLAGS) -c sort.c
stack.o: stack.c stack.h
	$(CC) $(CCFLAGS) -c stack.c
clean:
	rm *.o
