CFLAGS= -Wall -g -std=c++11

test: test.o feature.o engine.o house.o parser.o room.o
	g++ -o test test.o feature.o engine.o house.o parser.o room.o $(CFLAGS)

game: main.o feature.o engine.o house.o parser.o room.o
	g++ -o game $^ $(CFLAGS)

%.o: %.cpp %.hpp
	g++ -c $< $(CFLAGS)

clean:
	rm -f *.o game test
