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
		double size;
		void genRandom(int difficulty); //gonna get harder as game goes on
										// AKA add more walls
};

//Most of Ltexture taken from lazyfoo.net
//Texture wrapper class
/*
class LTexture
{
    public:
        //Initializes variables
        LTexture();

        //Deallocates memory
        ~LTexture();

        //Loads image at specified path
        bool loadFromFile( std::string path );

        //Deallocates texture
        void free();

        //Set color modulation
        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        //Set blending
        void setBlendMode( SDL_BlendMode blending );

        //Set alpha modulation
        void setAlpha( Uint8 alpha );
        
        //Renders texture at given point
        void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

        //Gets image dimensions
        int getWidth();
        int getHeight();

    private:
        //The actual hardware texture
        SDL_Texture* mTexture;

        //Image dimensions
        int mWidth;
        int mHeight;
};
LTexture::LTexture(){

	return;
}
LTexture::~LTexture(){

	return;
}

bool LTexture::loadFromFile(string path){
	
	return true;
}

void LTexture::free(){
	
	return;
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue){
	
	return;
}

void LTexture::setBlendMode(SDL_BlendMode blending){
	
	return;
}

void LTexture::setAlpha(Uint8 alpha){
	
	return;
}

void LTexture::render(int x, int y, class SDL_Rect* clip = NULL, double angle = 0.0,
		class SDL_Point * center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE){
	//Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    //Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
	return;
}

int LTexture::getWidth(){
	return mWidth;
}

int LTexture::getHeight(){
	return mHeight;
}
*/

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
	srand(time(NULL));
	int difficulty = 1;
	int counter = 1;

	Shell shell;

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
	
	IMG_Init(IMG_INIT_PNG);

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
