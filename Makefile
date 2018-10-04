CFLAGS= -Wall -g

test: room.o test.o
	gcc -o test test.o room.o $(CFLAGS)

room.o: room.cpp room.hpp
	gcc -c room.cpp $(CFLAGS)

test.o: test.cpp room.hpp
	gcc -c test.cpp $(CFLAGS)

clean:
	rm *.o