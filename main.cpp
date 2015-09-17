//g++ *.cpp -o ex -std=c++0x -lSDL2 -lSDL2_gfx -lSDL2_ttf -lSDL2_image
//sudo ldconfig

#include "game.h"

int main()
{
	//instantiate the game class
	Game game;
	//initialise
	game.init();
	//run (main game loop)
	game.run();
	
	return 0;
}