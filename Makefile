CFLAGS= -Wall -g -std=c++11

test: room.o test.o house.o parser.o container.o utilities.o actions.o
	g++ -o test test.o room.o house.o parser.o $(CFLAGS)

game: main.o room.o house.o parser.o container.o utilities.o actions.o
	g++ -o game $^ $(CFLAGS)

%.o: %.cpp %.hpp
	g++ -c $< $(CFLAGS)

clean:
	rm *.o
