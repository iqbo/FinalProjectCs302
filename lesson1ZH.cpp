#include "SDL/SDL.h"

int main(int argc, char * args[]){
	//The images
	SDL_Surface* hello = NULL;		//SDL_Surface is an image
	SDL_Surface* screen = NULL;		//
	
	//Start SDL
	SDL_Init( SDL_INIT_EVERYTHING );

	//Set up screen
	screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE);

	//Load image
	hello = SDL_LoadBMP( "hello.bmp" );

	//Apply image to scren
	SDL_BlitSurface( hello, NULL, screen, NULL );

	//Update screen
	SDL_Flip( screen );

	//Pause
	SDL_Delay( 2000 );

	//Free the loaded image
	SDL_FreeSurface( hello );

	//Quit SDL
	SDL_Quit();

	return 0;
}
