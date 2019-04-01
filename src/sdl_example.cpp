#define _USE_MATH_DEFINES

#include <SDL2/SDL.h>
#include <vector>
#include <math.h>
#include <cmath>
#include <cstdio>
using namespace std;

const float PI = M_PI;

class Player {
	public:
		bool movingLeft;
		bool movingRight;

		int xPos;
		int yPos;
};

class Shell {
	public:
		bool walls[6];
		void Draw(SDL_Renderer * gRenderer);
};

void Shell::Draw(SDL_Renderer * gRenderer){
	float x1, y1, x2, y2;
	float angle;

	float height, base;
	base = 100;
	height = 86.6025404;

	for (float i = 0; i < 6; i++) {
		if (walls[(int)i]) {
			angle = 2*PI * i/6;
			x1 = base * sin(angle + PI/2) + 200;
			y1 = height * sin(angle) + 200;
			x2 = base * sin(angle + PI/2 + PI/3) + 200;
			y2 = height * sin(angle + PI/3) + 200;
			//Draw a line corresponding to one side of the hexagon
			SDL_SetRenderDrawColor(gRenderer, 150, 200, 250, 255);
			SDL_RenderDrawLine(gRenderer, x1, y1, x2, y2);
		}
	}

	return;
}

//Super simple variables that set movement to true or false
//Usage:
//	if (keydown == left){
//		movingLeft = true
//	}
//	if (keyup == left){
//		movingLeft = false
//	}
//
//Reason for usage: more granular control of user input
//Works better than (if keydown == left) because keydown events are
//sporradic/inconsistent
//
//Doing it this way gives us more fluid movement control

vector <int> red;
vector <int> green;
vector <int> blue;

vector <vector <int> > colors;

void setColors(){
	red.push_back(255);
	red.push_back(150);
	red.push_back(150);
	green.push_back(150);
	green.push_back(255);
	green.push_back(150);
	blue.push_back(150);
	blue.push_back(150);
	blue.push_back(255);
	colors.push_back(red);
	colors.push_back(green);
	colors.push_back(blue);

	return;
}

int main(){
	Shell shell;

	//1 = visible wall
	shell.walls[0] = 0;
	shell.walls[1] = 0;
	shell.walls[2] = 0;
	shell.walls[3] = 0;
	shell.walls[4] = 0;
	shell.walls[5] = 0;

	setColors();

	printf("Opening SDL example\n");

	//BEGIN SDL INITIALIZING
	
	bool quit = false;
	SDL_Event e;

	const int SCREEN_W = 400;
	const int SCREEN_H = 400;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
		printf("SDL couldn't initialize");
	}

	SDL_Window * gWindow = SDL_CreateWindow("SDL Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED
			, SCREEN_W, SCREEN_H, SDL_WINDOW_RESIZABLE);
	SDL_Renderer * gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_PRESENTVSYNC);


	shell.Draw(gRenderer);
	//END SDL INITIALIZING

	Player player;
	player.xPos = SCREEN_W / 2;
	player.yPos = SCREEN_H / 4;

	player.movingRight = false;
	player.movingLeft = false;

	int current = 0;
	while (not quit) {

		//get events (inputs, x button, etc)
		while (SDL_PollEvent(&e) != 0){
			if (e.type == SDL_QUIT){
				quit = true;
			}
			if (e.type == SDL_KEYDOWN){
				switch (e.key.keysym.sym){
					case 97: //A
						if (not player.movingLeft) {
							player.movingLeft = true;
							printf("Starting Left\n");
						}
						break;
					case 100: //D
						if (not player.movingRight) {
							player.movingRight = true;
							printf("Starting Right\n");
						}
						break;
				}
			} else if (e.type == SDL_KEYUP){
				switch (e.key.keysym.sym){
					case 97:
						player.movingLeft = false;
						printf("Ending Left\n");
						break;
					case 100:
						player.movingRight = false;
						printf("Ending Right\n");
						break;
				}
			}
		}

		//Now handle player movement
		if (player.movingLeft){
			player.xPos -= 1;
		}
		if (player.movingRight){
			player.xPos += 1;
		}
	




		//handles player-wall collisions
		









		//Clear the screen
		SDL_SetRenderDrawColor(gRenderer, 100, 0, 100, 255);
		SDL_RenderClear(gRenderer);

		//Draw the player to the screen (broken for now)
		SDL_Rect fillRect = {player.xPos, player.yPos, SCREEN_W/20, SCREEN_H/20};
		SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
		SDL_RenderFillRect(gRenderer, &fillRect);
		shell.Draw(gRenderer);

		//Update the window when all done
		SDL_UpdateWindowSurface(gWindow);
		SDL_RenderPresent(gRenderer);
		//Waiting 16 milliseconds, i.e. 60 fps
		SDL_Delay(16);
	}

	printf("Closing...\n");
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();

	printf("Closed successfully\n");
	return 0;
}
