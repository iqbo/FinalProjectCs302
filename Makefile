superHex: superHex.o
	g++ -g -std=c++11 superHex.o -o superHex
superHex.o: src/main.cpp
	g++ -g -std=c++11 -c src/main.cpp
clean: 
	rm *.o
