CC = g++
LD = g++
CCFLAGS = -Wall -pedantic
LDFLAGS =

main.out: main.cpp server.out
	$(LD) $(CCFLAGS) -o main.out main.cpp
server.out: server.o stack.o sort.o
	$(LD) $(LDFLAGS) -o server.out server.o stack.o sort.o
server.o: server.cpp stack.h sort.h sort.c stack.c
	$(CC) $(CCFLAGS) -c server.cpp
sort.o: sort.c sort.h
	$(CC) $(CCFLAGS) -c sort.c
stack.o: stack.c stack.h
	$(CC) $(CCFLAGS) -c stack.c
clean:
	rm *.o
