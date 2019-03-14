all: src/main

superHex: superHex.o
	g++ -g -std=c++11 superHex.o -o superHex
superHex.o: main.cpp
	g++ -g -std=c++11 -c main.cpp
clean: 
	rm *.o superHex
