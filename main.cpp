//Linux: g++ *.cpp -o ex -std=c++0x -lSDL2 -lSDL2_gfx -lSDL2_ttf -lSDL2_image -lSDL2_mixer
//em++ *.cpp -o web/ex.html -std=c++0x -lSDL2 -lSDL2_gfx -lSDL2_ttf -lSDL2_image -lSDL2_mixer
//Windows: g++ *.cpp -o ex -std=c++0x -I C:\SDL2\SDL2-2.0.3\include -I C:\SDL2\SDL2_ttf-2.0.12\x86_64-w64-mingw32\include\SDL2 -I C:\SDL2\SDL2_mixer-2.0.0\x86_64-w64-mingw32\include\SDL2 -I C:\SDL2\SDL2_gfx-1.0.1 -I C:\SDL2\SDL2_image-2.0.0\x86_64-w64-mingw32\include\SDL2 -L C:\SDL2\SDL2-2.0.3\x86_64-w64-mingw32\lib -L C:\SDL2\SDL2_image-2.0.0\x86_64-w64-mingw32\lib -L C:\SDL2\SDL2_mixer-2.0.0\x86_64-w64-mingw32\lib -L C:\SDL2\SDL2_ttf-2.0.12\x86_64-w64-mingw32\lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -mwindows
//sudo ldconfig

#include <iostream>
#include "game.h"

#ifdef EMSCRIPTEN
#include "emscripten.h"
#endif
#undef main
void loop( Game* game )
{
	game->loopIteration();
}

void run(Game* game)
{
	while( !game->isQuitState() )
	{
		loop(game);
	}
	game->cleanup(0);
}

int main()
{
	//instantiate the game class
	Game* game;
	game = new Game;
	//initialise (0 means not resetting)
	game->init(0);
	//run (main game loop)
	run(game);
	return 0;
}
