superHex: src/main.cpp
	g++ -g -std=c++11 src/main.cpp -o superHex
sdl_example: sdl_example.cpp
	g++ -g -std=c++11 sdl_example.cpp -o sdl_example -lSDL2
clean: 
	rm superHex
