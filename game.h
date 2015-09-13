#ifndef GUARD_game
#define GUARD_game

class Game;

#include <vector>
#include "gameObject.h"
#include "renderSystem.h"
#include "planet.h"
#include "player.h"
#include "bullet.h"
#include "groundEnemy.h"
#include "skyEnemy.h"

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

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
		//gets for each object
		Planet* getPlanet() const;
		Player* getPlayer() const;
		std::vector<Bullet*> getBullets();
		std::vector<gameObject*> getGroundEnemies();
		void addBullet();

	//attributes
	private:
		//quit state, main loop stops when false
		bool quit_state;
		//renderer to pass game state to for rendering
		renderSystem* renderer;
		//time since last tick
		unsigned int d_time;
		//declare each type of mob
		Player* player;
		Planet* planet;
		//bullet array
		std::vector<Bullet*> bullets;
		std::vector<gameObject*> groundEnemies;
};

#endif