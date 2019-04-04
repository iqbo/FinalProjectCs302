sdl_example: src/sdl_example.cpp
	g++ -g -std=c++11 src/sdl_example.cpp -o sdl_example -lSDL2 -lSDL2_image
clean: 
	rm sdl_example
