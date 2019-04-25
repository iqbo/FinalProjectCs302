main:	src/main.cpp
	g++ -g -std=c++11 src/main.cpp src/hexgraph.cpp -o superHex2 -lSDL2
superHex: src/superHex.cpp
	g++ -g -std=c++11 src/superHex.cpp -o superHex -lSDL2 -lSDL2_image
sdl_example: src/sdl_example.cpp
	g++ -g -std=c++11 src/sdl_example.cpp -o sdl_example -lSDL2 -lSDL2_image
clean: 
	rm superHex2
