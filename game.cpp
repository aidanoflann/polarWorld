#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>

#include "game.h"
#include "renderSystem.h"
#include "bullet.h"

#ifdef EMSCRIPTEN
#include "emscripten.h"
#endif


void Game::init(bool reset)
{
	srand (time(NULL));
	quit_state = 0;
	d_time = 0;
	lastTime = 0;
	fps = 0;
	fpsCheckTime = 0;
	//initialise the rendersystem
	if (!reset)
	{
		//first let's initialise SDL2, 
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
			exit(1);
		}
		
		renderer = new renderSystem();
		(*renderer).init();
		
		//initialise the sound system
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0)//2 channels, for music and sound
			std::cout << "SDL_Mixer Error: " << SDL_GetError() << std::endl;
	}
// 	sndShoot = Mix_LoadWAV("sounds/shoot.wav");
// 	if (sndShoot == NULL)
// 		std::cout << "Couldn't load shoot.wav: " << Mix_GetError() << std::endl;
	//initialise the level's planet
	planet = new Planet();
	(*planet).init();
	//initialise the level's player
	player = new Player();
	//initialise the level's cloud
	cloud = new Cloud(250, player->getTheta() + 180);
	std::vector<gameObject> enemies;
	std::vector<Bullet> bullets;
	state = Startup;
}

void Game::cleanup(bool reset)
{
	delete player;
	delete planet;
	delete cloud;
	for (int i = 0; i < enemies.size(); i++) { delete enemies[i];}
	enemies.clear();
	for (int i = 0; i < bullets.size(); i++) { delete bullets[i];}
	bullets.clear();
	//Mix_FreeMusic( mscMusic );
	//Mix_FreeChunk( sndShoot );
	if (!reset)
	{
		(*renderer).cleanup();
		delete renderer;
		Mix_Quit();
	}
}

void Game::loopIteration()
{
	SDL_Event event;
	//handle inputs, SDL_PolleEvent will return false when there are none
	while(SDL_PollEvent( &event ))
	{
		//check the type of event
		switch( event.type )
		{
			case SDL_WINDOWEVENT:
				//if the close button was pressed, turn on the quit state
				if ( event.window.event == SDL_WINDOWEVENT_CLOSE )
					(*this).quit_state = 1;
			case SDL_KEYDOWN:
				//we don't want holding down the key to repeat inputs:
				if (!event.key.repeat)
				{
					//we don't want to register most events if the game is paused
					if (state == Running)
					{
						//if left is pressed, set the player's thetaDir to -1
						if ( event.key.keysym.sym == SDLK_a)
						{
							(*player).setThetaVelDirection( player->getThetaVelDirection() - 1 );
							player->setRunningRight(0);
							player->setShootingRight(0);
						}
						//if right is pressed, set the player's thetaDir to 1
						if ( event.key.keysym.sym == SDLK_d)
						{
							(*player).setThetaVelDirection( player->getThetaVelDirection() + 1 );
							player->setRunningRight(1);
							player->setShootingRight(1);
						}
						//if up or space is pressed, jump!
						if ( event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
							(*player).jump();
						//if left is pressed, shoot left
						if ( event.key.keysym.sym == SDLK_LEFT)
						{
							bullets.push_back( new Bullet( (*player).getR() - 5 , (*player).getTheta() - 10 , -1 ) );
							player->setShootingRight(0);
// 							Mix_PlayChannel( -1, sndShoot, 0);
						}
						//if right is pressed, shoot right
						if ( event.key.keysym.sym == SDLK_RIGHT)
						{
							bullets.push_back( new Bullet( (*player).getR() - 5 , (*player).getTheta() + 10 , 1 ) );
							player->setShootingRight(1);
// 							Mix_PlayChannel( -1, sndShoot, 0);
						}
					}
					//if r is pressed, reset level
					if ( event.key.keysym.sym == SDLK_r)
					{
						restart();
					}
					//if x is pressed, pause
					if (event.key.keysym.sym == SDLK_x && state == Running)
						state = Paused;
					else if (event.key.keysym.sym == SDLK_x && state == Paused)
					{
						state = Running;
						player->setThetaVelDirection(0);
					}
				}
			case SDL_KEYUP:
				//workaround for SDL2's issue on some systems with events being both KEYUP and KEYDOWN
				//TODO: Check if this has been fixed
				if ( event.type != SDL_KEYDOWN && (event.key.keysym.sym == SDLK_a ||event.key.keysym.sym == SDLK_d))
				{
					if (state == Running)
					{
						//set up a pointer to check the states of other keys
						const Uint8 *state = SDL_GetKeyboardState(NULL);
						//if left is depressed, set the player's thetaDir to 0
						if ( event.key.keysym.sym == SDLK_a && state[SDL_SCANCODE_D])
						{
							(*player).setThetaVelDirection( 1 );
						}
						if ( event.key.keysym.sym == SDLK_a && !state[SDL_SCANCODE_D])
						{
							(*player).setThetaVelDirection( 0 );
						}
						//if right is depressed, set the player's thetaDir to 0
						if ( event.key.keysym.sym == SDLK_d && state[SDL_SCANCODE_A])
						{
							(*player).setThetaVelDirection( -1);
						}
						if ( event.key.keysym.sym == SDLK_d && !state[SDL_SCANCODE_A])
						{
							(*player).setThetaVelDirection( 0 );
						}
					}
				}
		}
	}
	
	//TODO: get current time in nanoseconds
	//calculate the time since last tick
	unsigned int currentTime = SDL_GetTicks();
	(*this).d_time = currentTime - lastTime;
	lastTime = currentTime;
	
	//update fps
	frames ++;
	if ( (currentTime - fpsCheckTime) > 1000)
	{
		fps = frames;
		fpsCheckTime += 1000;
		frames = 0;
	}
	
	//perform calculations if unpaused
	if (state == Running)
	{
		//tick player
		(*player).tick(d_time);
		
		//run player collision checks
		if ((*player).collidingWithPlanet( (*planet).getCollisionRadius() ))
			(*player).collideWithPlanet( (*planet).getCollisionRadius() );
		
		//tick cloud
		(*cloud).tick(d_time, (*player).getTheta());
		//spawn a new enemy
		
		if ( cloud->doSpawn(d_time, player->getNumKills()) )
		{
			//randomly choose between ground and sky enemies, and between left or right velocities
			int coin = rand()%4;
			if (coin == 0)
					enemies.push_back( new GroundEnemy( cloud->getR(), cloud->getTheta(), 1, std::min(player->getNumKills()*0.001+0.02, 0.08)) );
			else if (coin == 1)
					enemies.push_back( new GroundEnemy( cloud->getR(), cloud->getTheta(), -1, std::min(player->getNumKills()*0.001+0.02, 0.08)) );
			else if (coin == 2)
					enemies.push_back( new SkyEnemy( cloud->getR(), cloud->getTheta(), 1, std::min(player->getNumKills()*0.001+0.02, 0.08)) );
			else if (coin == 3)
					enemies.push_back( new SkyEnemy( cloud->getR(), cloud->getTheta(), -1, std::min(player->getNumKills()*0.001+0.02, 0.08)) );
		}
		//tick all bullets
		for (int i = 0; i < (bullets).size(); i++)
		{
			//tick all bullets
			bullets[i]->tick(d_time);
			
			//check collisions with player
			if ( bullets[i]->collidingWithGameObject( (*player).getR(), (*player).getTheta(), (*player).getCollisionRadius() ) )
				gameOver(1);
			//remove bullets that are too damn old
			else if ( bullets[i]->getTimeAlive() > bullets[i]->getLifeTime() )
					bullets[i]->MarkForDeletion();
					
			//check collisions with enemies
			for( int j = 0; j < enemies.size(); j++)
			{
				if ( (enemies[j]->isNotExploding()) && bullets[i]->collidingWithGameObject( enemies[j]->getR(), enemies[j]->getTheta(), enemies[j]->getCollisionRadius() ) )
				{
					bullets[i]->MarkForDeletion();
					enemies[j]->startExploding();
					player->addKill();
				}
			}
		}
		
		//tick all enemies
		for (int i = 0; i < (enemies).size(); i++)
		{
			enemies[i]->tick(d_time);
			//run groundEnemy collision checks
			if (enemies[i]->collidingWithPlanet( (*planet).getCollisionRadius() ))
				enemies[i]->collideWithPlanet( (*planet).getCollisionRadius() );
			//run player collision checks
			if (  (enemies[i]->isNotExploding()) &&  enemies[i]->collidingWithGameObject( player->getR(), player->getTheta(), player->getCollisionRadius() ) )
				gameOver(0);
		}
		
		//remove objects which have been marked for deletion
		//bullets
		for (int i = 0; i < bullets.size(); i++)
		{
			if (bullets[i]->getMarkedForDeletion())
			{
				//delete the object bullets[i] is pointing to
				delete bullets[i];
				//remove the pointer from the bullets array
				bullets.erase(bullets.begin() + i);
				//decrement i by 1 so as not to skip over the next element
				i--;
			}
		}
		//enemies
		for (int i = 0; i < enemies.size(); i++)
		{
			if (enemies[i]->getMarkedForDeletion())
			{
				//delete the object bullets[i] is pointing to
				delete enemies[i];
				//remove the pointer from the bullets array
				enemies.erase(enemies.begin() + i);
				//decrement i by 1 so as not to skip over the next element
				i--;
			}
		}
	} else SDL_Delay(10);//endif Running
	//pass objects vector to renderer
	(*renderer).draw( (*this) );
}

void Game::restart()
{
	cleanup(1);
	init(1);
	state = Running;
	lastTime = SDL_GetTicks();
}

void Game::gameOver(bool self)
{
	if (self) state = GameOverSelf;
	else state = GameOverEnemy;
	//restart();
}

Planet* Game::getPlanet() const{return planet;}
Player* Game::getPlayer() const{return player;}
Cloud* Game::getCloud() const{return cloud;}
std::vector<Bullet*> Game::getBullets(){return bullets;}
std::vector<gameObject*> Game::getEnemies(){return enemies;}
unsigned int Game::getFPS() { return fps; }
void Game::addBullet(){}
bool Game::isPaused(){ return (state == Paused); }
bool Game::isGameOverEnemy(){ return (state == GameOverEnemy); }
bool Game::isGameOverSelf(){ return (state == GameOverSelf); }
bool Game::isRunning(){ return (state == Running); }
bool Game::isStartup(){ return (state == Startup); }
bool Game::isQuitState(){ return (quit_state); }