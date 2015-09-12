#ifndef GUARD_game
#define GUARD_game

#include <vector>
#include "gameObject.h"
#include "renderSystem.h"

class Game
{
	//member functions
	public:
		//set up SDL window etc
		void init();
		//close down SDL handlers
		void cleanup();
		//main game loop
		void run();
		
	//attributes
	private:
		//quit state, main loop stops when false
		bool quit_state;
		//renderer to pass game state to for rendering
		renderSystem renderer;
		//time since last tick
		double d_time;
		//keeping reference to the player (for input etc)
		gameObject player;
		//array of game objects (player, bullets, etc)
		std::vector<gameObject> objs;
};

#endif