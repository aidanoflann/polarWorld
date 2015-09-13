#include <iostream>
#include <vector>
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
	(*player).init();
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
						bullets.push_back( Bullet( (*player).getR() , (*player).getTheta() , -1 ) );
					}
					//if right is pressed, shoot left
					if ( event.key.keysym.sym == SDLK_RIGHT)
					{
						bullets.push_back( Bullet( (*player).getR() , (*player).getTheta() , 1 ) );
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
		//first run player collision checks
		if ((*player).collidingWithPlanet( (*planet).getCollisionRadius() ))
			(*player).collideWithPlanet( (*planet).getCollisionRadius() );
		//tick player
		(*player).tick(d_time);
		
		//TODO:run bullet collision checks
		//tick all bullets
		for ( std::vector<Bullet>::iterator iter = bullets.begin(); iter != bullets.end(); iter++)
		{
			(*iter).tick(d_time);
		}
		
		//pass objects vector to renderer
		(*renderer).draw( (*this) );
		
	}
	
	(*this).cleanup();
}

Planet* Game::getPlanet() const
{
	return planet;
}

Player* Game::getPlayer() const
{
	return player;
}

std::vector<Bullet> Game::getBullets() const
{
	return bullets;
}

void Game::addBullet()
{
	
}