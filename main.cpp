//g++ *.cpp -o ex -std=c++0x -lSDL2 -lSDL2_gfx -lSDL2_ttf -lSDL2_image -lSDL2_mixer
//em++ *.cpp -o web/ex.html -std=c++0x -lSDL2 -lSDL2_gfx -lSDL2_ttf -lSDL2_image -lSDL2_mixer
//sudo ldconfig

#include <iostream>
#include "game.h"

#ifdef EMSCRIPTEN
#include "emscripten.h"
#endif

void loop( Game* game )
{
	game->loopIteration();
}

void run(Game* game)
{
	#ifdef EMSCRIPTEN
	emscripten_set_main_loop_arg((em_arg_callback_func)loop, game, 0, 1);
	#else
	while( !game->isQuitState() )
	{
		loop(game);
	}
	game->cleanup(0);
	#endif
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
