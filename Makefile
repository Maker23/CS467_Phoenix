CFLAGS= -Wall -g -std=c++11

test: room.o test.o house.o parser.o
	g++ -o test test.o room.o house.o parser.o $(CFLAGS)

room.o: room.cpp room.hpp
	g++ -c room.cpp $(CFLAGS)

test.o: test.cpp room.hpp house.cpp parser.cpp
	g++ -c test.cpp $(CFLAGS)

house.o: house.cpp house.hpp
	g++ -c house.cpp $(CFLAGS)

clean:
	rm *.o
