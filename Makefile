CFLAGS= -Wall -g -std=c++11

game: main.o feature.o engine.o house.o parser.o room.o puzzle.o
	g++ -o game $^ $(CFLAGS)

dstest: dstest.o feature.o engine.o house.o parser.o room.o puzzle.o
	g++ dstest.o feature.o engine.o house.o parser.o room.o -o dstest $(CFLAGS)

test:test.o feature.o engine.o house.o parser.o room.o puzzle.o
	g++ test.o feature.o engine.o house.o parser.o room.o -o test $(CFLAGS)

%.o: %.cpp
	g++ -c $< $(CFLAGS)

%.o: %.cpp %.hpp
	g++ -c $< $(CFLAGS)

clean:
	rm -f *.o game test dstest
