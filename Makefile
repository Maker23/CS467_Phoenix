CFLAGS= -Wall -g -std=c++11

test: test.o room.o house.o parser.o container.o utilities.o actions.o
	g++ -o test test.o room.o house.o parser.o container.o utilities.o actions.o $(CFLAGS)

game: main.o room.o house.o parser.o container.o utilities.o actions.o
	g++ -o game $^ $(CFLAGS)

%.o: %.cpp %.hpp
	g++ -c $< $(CFLAGS)

clean:
	rm -f *.o game test
