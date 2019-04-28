#include "hexgraph.h"
#include "board.h"
#include <set>

//Point class
Point::Point() {
	x = 0;
	y = 0;

	return;
}

vector <char> color (int hex) {
	int r, g, b;
	r = (hex >> 16) & 0xff;
	g = (hex >> 8) & 0xff;
	b = (hex) & 0xff;

	vector <char> color;
	color.push_back(r);
	color.push_back(g);
	color.push_back(b);

	return color;
}

vector <char> player_color	= color(0xee5d6c);
vector <char> hex_color		= color(0xeeaf61);
vector <char> bg_color		= color(0x000000);

Point::Point(int X, int Y) {
	x = X;
	y = Y;

	return;
}

const int SCREEN_W = 500;
const int SCREEN_H = 500;

set <int> steps; //Points that the game gets harder
bool AI_enable = false;

//Adds two points
Point Point::operator +(class Point p) {
	Point rv;
	rv.x = x + p.x;
	rv.y = y + p.y;

	return rv;
}

Point Point::rotate(double angle) {
	Point rv;
	double radius;
	double curAngle;

	radius = sqrt((double)x*x + (double)y*y);
	curAngle = atan((double)y / (double)x);

	if (x >= 0 and y < 0) {
		curAngle = -curAngle;
	} else if (x < 0 and y >= 0) {
		curAngle = PI - curAngle;
	} else if (x < 0 and y < 0) {
		curAngle = PI + curAngle;
	}

	rv.x = radius*cos(angle + curAngle);
	rv.y = radius*sin(angle + curAngle);

	return rv;
}

//Draws a 1 px thick line from point p1 to point p2
void drawLine(Point p1, Point p2, SDL_Renderer * gRenderer, vector <char> & color) {
	SDL_SetRenderDrawColor(gRenderer, color[0], color[1], color[2], 255);
	SDL_RenderDrawLine(gRenderer, p1.x, p1.y, p2.x, p2.y);

	return;
}


//Player impltementation
Player::Player(){
	angle = PI/6;
	movingRight = false;
	movingLeft = false;

	pts.resize(5);
	pts[0] = Point(0, 10);
	pts[1] = Point(5, 0);
	pts[2] = Point(0, 3);
	pts[3] = Point(-5, 0);
	pts[4] = Point(0, 10);

	return;
}

void Player::Draw(SDL_Renderer * gRenderer) {
	Point p1, p2;

	int centerx, centery;
	centerx = SCREEN_W/2 + 50*cos(angle);
	centery = SCREEN_H/2 + 50*sin(angle);

	Point center(centerx, centery);

	for (int i = 0; i < pts.size() - 1; i++) {
		p1 = pts[i].rotate(angle - PI/2) + center;
		p2 = pts[i+1].rotate(angle - PI/2) + center;

		drawLine(p1, p2, gRenderer, player_color);
	}

	return;
}

void Player::interpath(int target){
		//A bit of error checking
		if (angle < 0) angle += 2*PI;
		if (angle > 2*PI) angle -= 2*PI;

		//Target angle that the AI tries to get to (a little broken at the moment)
		double t_angle = target * PI/3;

		//More error checking
		if (target == -1) {
			t_angle = angle;
		} else {
			if (t_angle > 2*PI) t_angle -= 2*PI;
		}

		//Centers t_angle in middle of sector
		t_angle += PI/6.0;	

		if (t_angle < 0)
			t_angle += 2*PI;
		else if (t_angle > 2*PI) 
			t_angle -= 2*PI;

		//The part that does the work
		if (abs(t_angle - angle) > 0.15) {
			double difference = t_angle - angle;
			if(difference >= 0 and difference <= PI) {
				angle += 0.15;
				//player.angle = t_angle;
			} else {
				angle -= 0.15;
				//player.angle = t_angle;
			}

		}

}

//Shell implementation
void Shell::genRandom(int difficulty){
	int random_i;
	int numWalls = 0;

	for (int i = 0; i < 6; i++) {
		walls[i] = 0;
	}

	while (numWalls < difficulty) {
		random_i = rand()%6;

		walls[random_i] = 1;
		numWalls += 1;
	}

	return;
}

//Draws one side of a hexagon as a trapezoid with 1px thick lines
void Shell::drawHexSide(int width, int radius, double theta, SDL_Renderer * gRenderer) {
	int centerx = SCREEN_W / 2;
	int centery = SCREEN_H / 2;
	Point p1, p2, p3, p4;

	p1.x = radius * cos(theta) + centerx;
	p1.y = radius * sin(theta) + centery;

	p2.x = radius * cos(theta + PI/3) + centerx;
	p2.y = radius * sin(theta + PI/3) + centery;

	p3.x = (radius + width) * cos(theta + PI/3) + centerx;
	p3.y = (radius + width) * sin(theta + PI/3) + centery;

	p4.x = (radius + width) * cos(theta) + centerx;
	p4.y = (radius + width) * sin(theta) + centery;

	drawLine(p1, p2, gRenderer, hex_color);
	drawLine(p2, p3, gRenderer, hex_color);
	drawLine(p3, p4, gRenderer, hex_color);
	drawLine(p4, p1, gRenderer, hex_color);

	return;
}

//Draws a hexagon shell based on the walls array
void Shell::Draw(SDL_Renderer * gRenderer){
	double angle;

	for (int i = 0; i < 6; i++) {
		if (walls[i]) {
			angle = i * PI/3;
			drawHexSide(10, size, angle, gRenderer);
		}
	}

	return;
}

//Board Implementation

//Initializes the game data SDL objects
Board::Board(){
	player = Player();
	AI_enable = false;	
	quit = false;

	difficulty = 1;
	counter = 1;
	shellcount = 0;

	shells[0].size = 200;
	shells[1].size = 325;
	shells[2].size = 450;
	shells[0].genRandom(difficulty);
	shells[1].genRandom(difficulty);
	shells[2].genRandom(difficulty);

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
		printf("SDL couldn't initialize");
	}


	gWindow = SDL_CreateWindow("SDL Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_W, SCREEN_H, SDL_WINDOW_RESIZABLE);
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_PRESENTVSYNC);
}


//Contains the main loop for the game
void Board::gameloop(){
	while (not quit) {

		//get events (inputs, x button, etc)
		processEvents(false);

		if (steps.find(counter) != steps.end()) {
			difficulty += 1;
			counter += 1;
			printf("Difficulty up to %d\n", difficulty);
		}

		//Now handle player movement
		if (player.movingLeft){
			player.angle -= 0.1;
			//cout << player.angle << endl;
		}
		if (player.movingRight){
			player.angle += 0.1;	
			//cout << player.angle << endl;
		}

		//Makes sure the player is within [0, 2PI]
		if (player.angle > 2.0*PI) {
			player.angle = player.angle - 2.0*PI;
		} else if (player.angle < 0) {
			player.angle = player.angle + 2.0*PI;
		}

		render();
	}

	close();
}


//Handles event processing
bool Board::processEvents(bool AI){

	//get events (inputs, x button, etc)
	while (SDL_PollEvent(&e) != 0){
		if (e.type == SDL_QUIT){
			quit = true;
			return false;
		}
		if (e.type == SDL_KEYDOWN){
			switch (e.key.keysym.sym){
				case 'a': //A
					if (!AI_enable) {
						if (not player.movingLeft) {
							player.movingLeft = true;
						}
					}
					break;
				case 'd': //D
					if (!AI_enable) {
						if (not player.movingRight) {
							player.movingRight = true;
						}
					}
					break;
			}
		} else if (e.type == SDL_KEYUP){
			switch (e.key.keysym.sym){
				case 'a':
					if (!AI_enable) {
						player.movingLeft = false;
					}
					break;
				case 'd':
					if (!AI_enable) {
						player.movingRight = false;
					}
					break;
				case 13:
					printf("Pressed enter\n");
					start = true;
					restart(); 
					SDL_Delay(300);
					quit = false;
					AI_enable = false;
					return false;
				case 'x':
					printf("Enabling AI...\n");
					start = true;
					restart();
					SDL_Delay(300);
					quit = false;
					AI_enable = true;
					return false;
				case 'q':
					printf("Quitting\n");
					quit = true;
					return false;
			}
		}
	}
	return true;
}

double dec(int difficulty) {
	switch (difficulty){
		case 1:
			return 1.5;
		case 2:
			return 2;
		case 3:
			return 2.5;
		case 4:
			return 3.5;
		case 5:
			return 5;
	}
}

int target;

//Draws and animates board
void Board::render(){
	int centerx = SCREEN_W / 2;
	int centery = SCREEN_H / 2;
	int maxrank = 2;

	SDL_SetRenderDrawColor(gRenderer, bg_color[0], bg_color[1], bg_color[2], 255);
	SDL_RenderClear(gRenderer);

	int p_sector = (player.angle / (PI/3));		//current sector of hexagon the player is located in

	for (int i = 0; i < 3; i++) {				//decreases size of shells to animate them moving towards the center
		shells[i].size -= dec(difficulty);
		if (shells[i].size <= 0) {				//if shell reaches center, generate a new shell and hexgraph
			shells[i].size = 400;
			shells[i].genRandom(difficulty);
			counter += 1;
			shellcount++;
			//printf("%d shells survived\n", shellcount);
		}

		if (abs(shells[i].size - 50) <= 10) {			//checks collision when the size of shell is close enough to the player's path radius
			if (shells[i].walls[p_sector]) {
				printf("You lasted %d shells\n", shellcount);
				printf("You died! \nPress enter to restart with control\nPress q to quit\nPress x to restart AI\n");
				while(processEvents(false)){
						continue;
				}
			}
		}

		shells[i].Draw(gRenderer);
	}

	//Determine maxrank, the ordering of shells
	int maxSize = shells[0].size;
	maxrank = 0;

	for(int i = 0; i < 3; i++){
		if(shells[i].size > maxSize)
			maxrank = i;
	}

	Hexgraph *hg;
	if(maxrank == 2){
		hg = new Hexgraph(shells[0].walls, shells[1].walls, shells[2].walls);
		target = hg->findPath(p_sector);
	}else if(maxrank == 1){
		hg = new Hexgraph(shells[2].walls, shells[0].walls, shells[1].walls);
		target = hg->findPath(p_sector);
	} else {
		hg = new Hexgraph(shells[1].walls, shells[2].walls, shells[0].walls);
		target = hg->findPath(p_sector);
	}
	//hg->print();
	//cout << "Passed Player Pos: " << p_sector << endl;

	if (AI_enable && target != p_sector) {
		player.interpath(target);
		//printf("target: %10d t_angle: %f\n", target, t_angle);
	}

	//Draw the player to the screen
	player.Draw(gRenderer);


	//Update the window when all done
	//SDL_UpdateWindowSurface(gWindow);
	SDL_RenderPresent(gRenderer);

	//Waiting 16 milliseconds, i.e. 60 fps
	SDL_Delay(16);
}

void Board::close(){
	printf("Closing...\n");
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();
}

void Board::restart(){
	difficulty = 1;
	counter = 1;
	shellcount = 0;

	shells[0].size = 200;
	shells[1].size = 325;
	shells[2].size = 450;

	shells[0].genRandom(difficulty);
	shells[1].genRandom(difficulty);
	shells[2].genRandom(difficulty);

	player.angle = PI/6;	

	SDL_SetRenderDrawColor(gRenderer, bg_color[0], bg_color[1], bg_color[2], 255);
	SDL_RenderClear(gRenderer);
	SDL_RenderPresent(gRenderer);

	//srand(time(NULL));
}	


//
//Main
int main(){
	printf("You are a humble triangle trapped in a world dominated by hexagons.\n");
	printf("You must find your way out. Avoid the hexagon walls, using A to move left and D to move right.\n");
	printf("Press enter to begin your journey again.\nPress X for help on your journey.\n");
	printf("Press q to give up on your journey.\n");

	steps.insert(10);
	steps.insert(15);
	steps.insert(35);
	steps.insert(65);
	steps.insert(10000);
	srand(time(NULL));

	Board myBoard = Board();
	myBoard.gameloop();

	return 0;
}
