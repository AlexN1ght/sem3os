CC = g++
LD = g++
CCFLAGS = -Wall -pedantic
LDFLAGS =


main.out: main.o vector.o libvector.so
	$(LD) $(LDFLAGS) -rdynamic -o main.out main.o -ldl -L. -lvector -Wl,-rpath,.

main.o: main.cpp
	$(CC) $(CCFLAGS) -c main.cpp

libvector.so: vector.o
	$(LD) -shared -o libvector.so vector.o

vector.o: vector.cpp Vector.hpp
	$(CC) $(CCFLAGS) -c -fPIC vector.cpp
clean:
	rm *.o
	rm libvector.so
