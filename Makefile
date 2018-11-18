CFLAGS= -Wall -g -std=c++11

ADDTL_FILES=utilities.hpp

game: main.o feature.o engine.o house.o parser.o room.o puzzle.o
	g++ -o game $^ $(CFLAGS)

dstest: dstest.o feature.o engine.o house.o parser.o room.o puzzle.o
	g++ dstest.o feature.o engine.o house.o parser.o room.o -o dstest $(CFLAGS)

test:test.o feature.o engine.o house.o parser.o room.o puzzle.o
	g++ test.o feature.o engine.o house.o parser.o room.o -o test $(CFLAGS)

%.o: %.cpp %.hpp $(ADDTL_FILES)
	g++ -c $< $(CFLAGS)

%.o: %.cpp $(ADDTL_FILES)
	g++ -c $< $(CFLAGS)

clean:
	rm -f *.o game test dstest
