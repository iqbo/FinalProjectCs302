#define _USE_MATH_DEFINES

#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <math.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

const double PI = M_PI;
const int framerate = 60;

//SDL Stuff
SDL_Surface  * gScreenSurface = NULL;
SDL_Window   * gWindow = NULL;
SDL_Renderer * gRenderer = NULL;

vector <SDL_Surface*> hexSides;

class Player {
	public:
		bool movingLeft;
		bool movingRight;

		int xPos;
		int yPos;
		SDL_Surface * img;
};

class Shell {
	public:
		bool walls[6];
		void Draw(SDL_Renderer * gRenderer);
		double size;
		void genRandom(int difficulty); //gonna get harder as game goes on
										// AKA add more walls
};

//Taken from Lazyfoo.net tutorial
SDL_Surface* loadSurface(char* path)
{
    //The final optimized image
    SDL_Surface* optimizedSurface = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL) {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
    } else {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, NULL );

        if (optimizedSurface == NULL) {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return optimizedSurface;
}

void Shell::genRandom(int difficulty){
	int random_i;
	int numWalls = 0;

	for (int i = 0; i < 6; i++){
		random_i = rand()%2;
		if (random_i){
			walls[i] = 1;
			numWalls += 1;
		} else {
			walls[i] = 0;
		}
		if (numWalls == difficulty){
			for (int j = i+1; j < 6; j++){
				walls[j] = 0;
			}
			break;
		}
	}

	return;
}



void Shell::Draw(SDL_Renderer * gRenderer){
	double x1, y1, x2, y2;
	double angle;

	double height, base;
	base = 100;
	height = 86.6025404;

	/* OLD CODE using basic line geometry
	for (double i = 0; i < 6; i++) {
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
	*/

	//UPDATED using PNG image file
	vector <int> widths  = {324, 197, 196, 324, 196, 196};
	vector <int> heights = {51, 286, 289, 51, 289, 289};
	for (int i = 0; i < 6; i++) {
		if (walls[i]) {
			angle = 2*PI * i/6;
			
			SDL_Rect R;
			R.x = 100 * cos(angle) + 200;
			R.y = 100 * sin(angle) + 200;
			R.w = widths[i]/2;
			R.h = heights[i]/2;
			SDL_BlitScaled(hexSides[i], NULL, gScreenSurface, &R);
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

int main(){
	srand(time(NULL));
	int difficulty = 1;
	int counter = 1;


	printf("Opening SDL example\n");

	//BEGIN SDL INITIALIZING

	bool quit = false;
	SDL_Event e;

	const int SCREEN_W = 800;
	const int SCREEN_H = 800;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
		printf("SDL couldn't initialize");
		return -1;
	}
	
	IMG_Init(IMG_INIT_PNG);

	gWindow = SDL_CreateWindow("SDL Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED
			, SCREEN_W, SCREEN_H, SDL_WINDOW_RESIZABLE);
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_PRESENTVSYNC);
	gScreenSurface = SDL_GetWindowSurface(gWindow);

	Shell shell;
	
	//Grab the texture for the hexagon side
	char buff[100];
	hexSides.resize(6);
	for (int i = 0; i < 6; i++) {
		printf("neet %d\n", i);
		snprintf(buff, sizeof(buff), "images/hexSide%d.png", i);
		hexSides[i] = loadSurface(buff);
	}


	//END SDL INITIALIZING

	Player player;
	player.xPos = SCREEN_W / 2;
	player.yPos = SCREEN_H / 4;

	player.movingRight = false;
	player.movingLeft = false;
	
	//And for the player
	player.img = loadSurface("images/Player.png");

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
					case 'e':
						shell.genRandom(difficulty);
						counter += 1;
						printf("Pressed e\n");
				}
			}
		}
		if (counter % 10 == 0 and difficulty < 5) {
			difficulty += 1;
			counter += 1;
			printf("Difficulty up to %d\n", difficulty);
		}

		//Now handle player movement
		if (player.movingLeft){
			player.xPos -= 1;
		}
		if (player.movingRight){
			player.xPos += 1;
		}





		//handles player-wall collisions









		SDL_Rect fillRect;
		

		//Clear the screen
		//SDL_SetRenderDrawColor(gRenderer, 100, 0, 100, 255);
		//SDL_RenderClear(gRenderer);
		SDL_FillRect(gScreenSurface, 0, 0);


		//Draw the player
		SDL_Rect R;
		R.x = player.xPos;
		R.y = player.yPos;
		R.w = 20;
		R.h = 20;
		SDL_BlitScaled(player.img, NULL, gScreenSurface, &R);

		shell.Draw(gRenderer);
		SDL_UpdateWindowSurface(gWindow);

		//Update the window when all done
		//SDL_RenderPresent(gRenderer);
		
		//Waiting in milliseconds
		SDL_Delay(1000/framerate);
	}

	printf("Closing...\n");
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();

	printf("Closed successfully\n");
	return 0;
}
