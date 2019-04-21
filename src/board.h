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
		Player();
		bool movingLeft;
		bool movingRight;

		double angle;
		int radius;
};

class Shell {
	public:
		bool walls[6];
		void Draw(SDL_Renderer * gRenderer);
		double size;
		void genRandom(int difficulty); //gonna get harder as game goes on
		void drawLine(Point p1, Point p2, SDL_Renderer * gRenderer);
		void drawHexSide(int width, int radius, double theta,  SDL_Renderer * gRenderer);
};

class Point {
	public:
		int x;
		int y;
		Point(int X, int Y);
		Point();
};

class Board {
	public:
		Board();			//begins game by generate window
		void processEvents(bool AI);		//game loop, if AI = true, AI plays
		void render();						//renders game
		void close();						//closes game
		void restart();						//restarts game
		//mode
		int upDifficulty();
		void Draw(SDL_Render * gRenderer);	
		Player player;
		Shell shells[3];

		SDL_Window * gWindow;
		SDL_Renderer * gRenderer;
		//generate graphical board using class shell
		//intreprets board into graph data structure using hexgraph
		//calls hexgraph->findpath
		//accepts CLI for user or A controller player if enter
		//upon death reset board & start game w enter or other keypress
	private:
		const int SCREEN_W;
		const int SCREEN_H;
		bool quit;
		SDL_Event e;
