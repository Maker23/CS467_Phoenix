CFLAGS= -Wall -g -std=c++11

test: room.o test.o house.o container.o utilities.o actions.o
	g++ -o test test.o room.o house.o container.o utilities.o actions.o $(CFLAGS)

game: room.o main.o house.o container.o utilities.o actions.o
	g++ -o game main.o room.o house.o container.o utilities.o actions.o $(CFLAGS)

%.o: %.cpp %.hpp
	g++ -c $< ${CFLAGS}

clean:
	rm -f *.o game
