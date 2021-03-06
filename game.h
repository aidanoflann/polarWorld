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
#include "cloud.h"

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

class Game
{
	//member functions
	public:
		//set up SDL window etc
		void init(bool);
		//close down SDL handlers
		void cleanup(bool);
		//main game loop
		void run();
		//gets for each object
		Planet* getPlanet() const;
		Player* getPlayer() const;
		Cloud* getCloud() const;
		std::vector<Bullet*> getBullets();
		std::vector<gameObject*> getEnemies();
		void addBullet();
		void restart();
		void gameOver(bool);
		unsigned int getFPS();
		bool isPaused();
		bool isGameOverEnemy();
		bool isGameOverSelf();
		bool isRunning();
		bool isStartup();
		void loopIteration();
		bool isQuitState();

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
		Cloud* cloud;
		//bullet array
		std::vector<Bullet*> bullets;
		std::vector<gameObject*> enemies;
		unsigned int lastTime;
		unsigned int fpsCheckTime;
		unsigned int fps;
		unsigned int frames;
		enum State
		{
			Paused,
			Running,
			GameOverSelf,
			GameOverEnemy,
			Startup,
		};
		State state;
		Mix_Music* mscMusic;
		Mix_Music* mscMusicDead;
		Mix_Chunk* sndShoot;
		Mix_Chunk* sndEnemyDie;
};

#endif