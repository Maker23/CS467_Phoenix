CFLAGS= -Wall -g -std=c++11

test: room.o test.o house.o
	g++ -o test test.o room.o house.o $(CFLAGS)

game: room.o main.o house.o container.o utilities.o actions.o
	g++ -o game main.o room.o house.o container.o utilities.o actions.o $(CFLAGS)

room.o: room.cpp room.hpp
	g++ -c room.cpp $(CFLAGS)

test.o: test.cpp room.hpp house.cpp
	g++ -c test.cpp $(CFLAGS)

house.o: house.cpp house.hpp
	g++ -c house.cpp $(CFLAGS)

container.o: container.cpp container.hpp
	g++ -c container.cpp $(CFLAGS)

utilities.o: utilities.cpp utilities.hpp
	g++ -c utilities.cpp $(CFLAGS)

actions.o: actions.cpp actions.hpp
	g++ -c actions.cpp $(CFLAGS)

clean:
	rm *.o game
