CFLAGS= -Wall -g -std=c++11

dstest: dstest.o feature.o engine.o house.o parser.o room.o
	g++ dstest.o feature.o engine.o house.o parser.o room.o -o dstest $(CFLAGS)

test:test.o feature.o engine.o house.o parser.o room.o
	g++ test.o feature.o engine.o house.o parser.o room.o -o test $(CFLAGS)

game: main.o feature.o engine.o house.o parser.o room.o
	g++ -o game $^ $(CFLAGS)

%.o: %.cpp %.hpp
	g++ -c $< $(CFLAGS)

clean:
	rm -f *.o game test dstest
