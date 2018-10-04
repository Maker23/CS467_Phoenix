CFLAGS= -Wall -g

test: room.o test.o
	g++ -o test test.o room.o $(CFLAGS)

room.o: room.cpp room.hpp
	g++ -c room.cpp $(CFLAGS)

test.o: test.cpp room.hpp
	g++ -c test.cpp $(CFLAGS)

clean:
	rm *.o