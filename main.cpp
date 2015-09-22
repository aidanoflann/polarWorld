//g++ *.cpp -o ex -std=c++0x -lSDL2 -lSDL2_gfx -lSDL2_ttf -lSDL2_image -lSDL2_mixer
//sudo ldconfig

#include <iostream>
#include "game.h"

int main()
{
	//instantiate the game class
	Game game;
	//initialise (0 means not resetting)
	game.init(0);
	//run (main game loop)
	game.run();
	
	return 0;
}