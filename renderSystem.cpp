#include <vector>
#include <cmath>
#include <iostream>

#include <SDL.h>
#include <math.h>
#include <SDL2_gfxPrimitives.h>

#include <typeinfo>

#include "renderSystem.h"
#include "gameObject.h"
#include "planet.h"
#include "game.h"

double radPerDeg = 0.01745329251;

int renderSystem::init()
{
	//first just give some information on the SDL version
	//SDL_version compiled;
	//SDL_version linked;

	//SDL_VERSION(&compiled);
	//SDL_GetVersion(&linked);
	
	//printf("We compiled against SDL version %d.%d.%d ...\n",
    //   compiled.major, compiled.minor, compiled.patch);
	//printf("But we are linking against SDL version %d.%d.%d.\n",
    //   linked.major, linked.minor, linked.patch);
	
	//set width and height
	width = 800;
	height = 600;
	
	//first let's initialise SDL2, 
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	
	//now create a window with title "Hello world!" at position (100, 100) with res (640x480)
	SDL_Window *win = SDL_CreateWindow("Polar World", 100, 100, (*this).width, (*this).height, SDL_WINDOW_SHOWN);
	//if the window creation fails, *win will be NULL
	if (win == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	
	//set up a renderer
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, 0);
	if (ren == nullptr)
	{
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	
	(*this).ren = ren;
	(*this).win = win;
}

void renderSystem::draw( Game& game)
{
	//First clear the renderer
	SDL_RenderClear(ren);
	
	//draw the background
	SDL_SetRenderDrawColor((*this).ren, 255, 255, 255, 255);
	SDL_Rect bg = {0,0, (int)std::round(width), (int)std::round(height)};
	SDL_RenderFillRect((*this).ren, &bg);
	
	//render the planet
	filledCircleRGBA(ren,
					 width/2 + (*(game.getPlanet())).getTheta(),
					 height/2 + (*(game.getPlanet())).getR(),
					 (*game.getPlanet()).getCollisionRadius(),
					 0, 200, 100, 200);
	
	//render the player
	filledCircleRGBA(ren,
					 width/2 + (*(game.getPlayer())).getR() * cos((*(game.getPlayer())).getTheta() * radPerDeg),
					 height/2 + (*(game.getPlayer())).getR() * sin((*(game.getPlayer())).getTheta() * radPerDeg),
					 (*game.getPlayer()).getCollisionRadius(),
					 0, 100, 200, 200);
	
	//render the cloud
	filledCircleRGBA(ren,
					 width/2 + (*(game.getCloud())).getR() * cos((*(game.getCloud())).getTheta() * radPerDeg),
					 height/2 + (*(game.getCloud())).getR() * sin((*(game.getCloud())).getTheta() * radPerDeg),
					 (*game.getCloud()).getCollisionRadius(),
					 100, 100, 100, 100);
	
	//render each groundenemy
	for (int i = 0; i != (game.getGroundEnemies()).size(); i++)
	{
		gameObject gen = *((game.getGroundEnemies())[i]);
		filledCircleRGBA(ren,
					width/2 + gen.getR() * cos(gen.getTheta() * radPerDeg),
					height/2 + gen.getR() * sin(gen.getTheta() * radPerDeg),
					gen.getCollisionRadius(),
					200, 100, 200, 200);
	}
	
	//render each bullet
	//TODO: Figure out iter implementation
	//for ( std::vector<Bullet>::iterator iter = (game.getBullets()).begin(); iter != (game.getBullets()).end(); iter++)
	for (int i = 0; i != (game.getBullets()).size(); i++)
	{
		Bullet b = *((game.getBullets())[i]);
		filledCircleRGBA(ren,
						width/2 + b.getR() * cos(b.getTheta() * radPerDeg),
						height/2 + b.getR() * sin(b.getTheta() * radPerDeg),
						b.getCollisionRadius(),
						250, 100, 100, 255);
	}
	//Update the screen
	SDL_RenderPresent(ren);
}

void renderSystem::cleanup()
{
	SDL_DestroyRenderer((*this).ren);
	SDL_DestroyWindow((*this).win);	
	SDL_Quit();
}