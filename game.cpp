#include <iostream>
#include <vector>
#include <cstdlib>
#include "game.h"
#include "renderSystem.h"
#include "bullet.h"

void Game::init()
{
	quit_state = 0;
	d_time = 0;
	//initialise the rendersystem
	renderer = new renderSystem();
	(*renderer).init();
	//initialise the level's planet
	planet = new Planet();
	(*planet).init();
	//initialise the level's player
	player = new Player();
	//initialise the level's cloud
	cloud = new Cloud(250, player->getTheta() + 180);
}

void Game::cleanup()
{
	(*renderer).cleanup();
}

void Game::run()
{
	SDL_Event event;
	unsigned int lastTime = 0;
	while( !quit_state )
	{
		//TODO: get current time in nanoseconds
		//calculate the time since last tick
		unsigned int currentTime = SDL_GetTicks();
		(*this).d_time = currentTime - lastTime;
		lastTime = currentTime;
		
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
					//if left is pressed, set the player's thetaDir to -1
					if ( event.key.keysym.sym == SDLK_a)
						(*player).setThetaVelDirection(-1);
					//if right is pressed, set the player's thetaDir to -1
					if ( event.key.keysym.sym == SDLK_d)
						(*player).setThetaVelDirection(1);
					//if up or space is pressed, jump!
					if ( event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
						(*player).jump();
					//if left is pressed, shoot left
					if ( event.key.keysym.sym == SDLK_LEFT)
					{
						bullets.push_back( new Bullet( (*player).getR() , (*player).getTheta() , -1 ) );
					}
					//if right is pressed, shoot left
					if ( event.key.keysym.sym == SDLK_RIGHT)
					{
						bullets.push_back( new Bullet( (*player).getR() , (*player).getTheta() , 1 ) );
					}
				case SDL_KEYUP:
					//workaround for SDL2's issue on some systems with events being both KEYUP and KEYDOWN
					//TODO: Check if this has been fixed
					if ( event.type != SDL_KEYDOWN )
					{
						//if left is depressed, set the player's thetaDir to 0
						if ( event.key.keysym.sym == SDLK_a)
							(*player).setThetaVelDirection(0);
						//if right is depressed, set the player's thetaDir to 0
						if ( event.key.keysym.sym == SDLK_d)
							(*player).setThetaVelDirection(0);
					}
			}
		}
		//perform calculations
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
					enemies.push_back( new GroundEnemy( cloud->getR(), cloud->getTheta(), 1) );
			else if (coin == 1)
					enemies.push_back( new GroundEnemy( cloud->getR(), cloud->getTheta(), -1) );
			else if (coin == 2)
					enemies.push_back( new SkyEnemy( cloud->getR(), cloud->getTheta(), 1) );
			else if (coin == 3)
					enemies.push_back( new SkyEnemy( cloud->getR(), cloud->getTheta(), -1) );
		}
		//tick all bullets
		for (int i = 0; i < (bullets).size(); i++)
		{
			//tick all bullets
			bullets[i]->tick(d_time);
			
			//check collisions with player
			if (SDL_GetTicks() - bullets[i]->getTimeBorn() > 500 &&
				bullets[i]->collidingWithGameObject( (*player).getR(), (*player).getTheta(), (*player).getCollisionRadius() )
				//remove bullets that are too damn old
				|| (SDL_GetTicks() - bullets[i]->getTimeBorn()) > bullets[i]->getLifeTime())
					bullets[i]->MarkForDeletion();
					
			//check collisions with enemies
			for( int j = 0; j < enemies.size(); j++)
			{
				if ( bullets[i]->collidingWithGameObject( enemies[j]->getR(), enemies[j]->getTheta(), enemies[j]->getCollisionRadius() ) )
				{
					bullets[i]->MarkForDeletion();
					enemies[j]->MarkForDeletion();
					player->addKill();
					std::cout << player->getNumKills() << std::endl;
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
		
		//pass objects vector to renderer
		(*renderer).draw( (*this) );
		
	}
	
	cleanup();
}

Planet* Game::getPlanet() const
{
	return planet;
}

Player* Game::getPlayer() const
{
	return player;
}

Cloud* Game::getCloud() const
{
	return cloud;
}

std::vector<Bullet*> Game::getBullets()
{
	return bullets;
}

std::vector<gameObject*> Game::getEnemies()
{
	return enemies;
}

void Game::addBullet()
{
	
}