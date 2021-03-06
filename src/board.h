#define _USE_MATH_DEFINES

#include <time.h>
#include <SDL2/SDL.h>
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


class Point {
	public:
		int x;
		int y;
		Point(int X, int Y);
		Point();
		Point operator + (class Point p);
		Point rotate(double angle);
};

class Player {
	public:
		Player();
		bool movingLeft;
		bool movingRight;
		void Draw(SDL_Renderer * gRenderer);
		void interpath(int target);
		double angle;
		int radius;
	private:
		vector <Point> pts;
};

class Shell {
	public:
		bool walls[6];
		void Draw(SDL_Renderer * gRenderer);
		double size;
		void genRandom(int difficulty); //gonna get harder as game goes on
//		void drawLine(Point p1, Point p2, SDL_Renderer * gRenderer);
		void drawHexSide(int width, int radius, double theta,  SDL_Renderer * gRenderer);	
};


class Board {
	public:
		Board();			//begins game by generate window
		bool processEvents(bool AI);		//game loop, if AI = true, AI plays
		void render();						//renders game
		void close();						//closes game
		void restart();						//restarts game
		void gameloop();
		//mode
		int upDifficulty();
		void Draw(SDL_Renderer * gRenderer);	
		Player player;
		Shell shells[3];

		SDL_Window * gWindow;
		SDL_Renderer * gRenderer;
		//generate graphical board using class shell
		//intreprets board into graph data structure using hexgraph
		//calls hexgraph->findpath
		//accepts CLI for user or A controller player if enter
		//upon death reset board & start game w enter or other keypress
		int difficulty;
		int counter; 	
		const int SCREEN_W = 500;
		const int SCREEN_H = 500;
		bool quit;
		bool start;
		bool AIenable;
		SDL_Event e;
		int shellcount;
};
