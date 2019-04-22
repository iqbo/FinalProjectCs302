#include "board.h"

//Point class
Point::Point() {
	x = 0;
	y = 0;

	return;
}

Point::Point(int X, int Y) {
	x = X;
	y = Y;

	return;
}

const int SCREEN_W = 500;
const int SCREEN_H = 500;

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
void drawLine(Point p1, Point p2, SDL_Renderer * gRenderer) {
	SDL_SetRenderDrawColor(gRenderer, 150, 200, 250, 255);
	SDL_RenderDrawLine(gRenderer, p1.x, p1.y, p2.x, p2.y);

	return;
}

//Player impltementation
Player::Player(){
	angle = 0;
	movingRight = false;
	movingLeft = false;

	pts.resize(5);
	pts[0] = Point(0, 8);
	pts[1] = Point(3, 0);
	pts[2] = Point(0, 3);
	pts[3] = Point(-4, 0);
	pts[4] = Point(0, 8);

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

		drawLine(p1, p2, gRenderer);
	}

	return;
}

//Shell implementation
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
			if (i == 5) return;
			for (int j = i+1; j < 6; j++){
				walls[j] = 0;
			}
			return;
		}
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

	drawLine(p1, p2, gRenderer);
	drawLine(p2, p3, gRenderer);
	drawLine(p3, p4, gRenderer);
	drawLine(p4, p1, gRenderer);

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
	AIenable = false;	
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

		if (counter % 11 == 0 and difficulty < 4) {
			difficulty += 1;
			counter += 1;
			printf("Difficulty up to %d\n", difficulty);
		}

		//Now handle player movement
		if (player.movingLeft){
			player.angle -= 0.1;
		}
		if (player.movingRight){
			player.angle += 0.1;
		}

		//Makes sure the player is within [0, 2PI]
		if (player.angle > 2*PI) {
			player.angle = player.angle - 2*PI;
		} else if (player.angle < 0) {
			player.angle = player.angle + 2*PI;
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
			}
			if (e.type == SDL_KEYDOWN){
				switch (e.key.keysym.sym){
					case 97: //A
						if (not player.movingLeft) {
							player.movingLeft = true;
	//						printf("Starting Left\n");
						}
						break;
					case 100: //D
						if (not player.movingRight) {
							player.movingRight = true;
	//						printf("Starting Right\n");
						}
						break;
				}
			} else if (e.type == SDL_KEYUP){
				switch (e.key.keysym.sym){
					case 97:
						player.movingLeft = false;
	//					printf("Ending Left\n");
						break;
					case 100:
						player.movingRight = false;
	//					printf("Ending Right\n");
						break;
	//				case 'e':
	//					counter += 1;
	//					printf("Pressed e\n");
	//					break;
					case 13:
						printf("Pressed enter\n");
						start = true;
						restart(); 
						SDL_Delay(300);
						quit = false;
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

//Draws and animates board
void Board::render(){
	int centerx = SCREEN_W / 2;
	int centery = SCREEN_H / 2;
	SDL_SetRenderDrawColor(gRenderer, 100, 0, 100, 255);
	SDL_RenderClear(gRenderer);

	int p_sector = (player.angle / (PI/3));		//current sector of hexagon the player is located in

	for (int i = 0; i < 3; i++) {				//decreases size of shells to animate them moving towards the center
		shells[i].size -= difficulty;
		if (shells[i].size <= 0) {				//if shell reaches center, generate a new shell
			shells[i].size = 400;
			shells[i].genRandom(difficulty);
			counter += 1;
			shellcount++;
		}

		if (abs(shells[i].size - 50) <= 10) {			//checks collision when the size of shell is close enough to the player's path radius
			if (shells[i].walls[p_sector]) {
				printf("You lasted %d shells\n", shellcount);
				printf("You ded m8 \nPress enter to restart\nPress q to quit\n");
				while(processEvents(false)){
					continue;
				}
				shells[i].genRandom(0);
			}
		}

		shells[i].Draw(gRenderer);
	}

	
	//OLD CODE to draw player
	//SDL_SetRenderDrawColor(gRenderer, 200, 50, 50, 255);
	//SDL_Rect fillRect = {50 * cos(player.angle) + centerx - 10, 50 * sin(player.angle) + centery - 10, 20, 20};
	//SDL_RenderFillRect(gRenderer, &fillRect);

	//Draw the player to the screen
	player.Draw(gRenderer);
	
	//Player score counter
	

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
	
	shells[0].genRandom(0);
	shells[1].genRandom(0);
	shells[2].genRandom(0);
	
	player.angle = 0;	
	
	SDL_SetRenderDrawColor(gRenderer, 100, 0, 100, 255);
	SDL_RenderClear(gRenderer);
	SDL_RenderPresent(gRenderer);
	
	//srand(time(NULL));
}	


//
//Main
int main(){
	srand(time(NULL));

	Board myBoard = Board();
	myBoard.gameloop();

	return 0;
}
