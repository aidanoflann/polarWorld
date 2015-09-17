#include <vector>
#include <cmath>
#include <iostream>

#include <SDL.h>
#include <math.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

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
	
	//set up window
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
	
	// Initialize SDL_ttf library
	if (TTF_Init() != 0)
	{
		std::cout << "TTF_Init() Failed: " << TTF_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	
	// Load a font
	font = TTF_OpenFont("FreeSans.ttf", 24);
	if (font == NULL)
	{
		std::cout << "TTF_OpenFont() Failed: " << TTF_GetError() << std::endl;
		TTF_Quit();
		SDL_Quit();
		exit(1);
	}
	
	(*this).ren = ren;
	(*this).win = win;
	
	//load all textures
	planet_png = IMG_Load("images/planet.png");
	player_png = IMG_Load("images/player.png");
}

void renderSystem::draw( Game& game)
{
	//First clear the renderer
	SDL_RenderClear(ren);
	
	//draw the background
	SDL_SetRenderDrawColor((*this).ren, 255, 255, 255, 255);
	SDL_Rect bg = {0,0, (int)std::round(width), (int)std::round(height)};
	SDL_RenderFillRect((*this).ren, &bg);
	
	//render the Planet
	renderSprite(game.getPlanet(), planet_png);
	
	//render the player
	renderPlayer(game.getPlayer());
	
	//render the cloud
	filledCircleRGBA(ren,
					 width/2 + game.getCloud()->getR() * cos(game.getCloud()->getTheta() * radPerDeg),
					 height/2 + game.getCloud()->getR() * sin(game.getCloud()->getTheta() * radPerDeg),
					 game.getCloud()->getCollisionRadius(),
					 100, 100, 100, 100);
	
	//render each groundenemy
	for (int i = 0; i != (game.getEnemies()).size(); i++)
	{
		gameObject en = *((game.getEnemies())[i]);
		filledCircleRGBA(ren,
					width/2 + en.getR() * cos(en.getTheta() * radPerDeg),
					height/2 + en.getR() * sin(en.getTheta() * radPerDeg),
					en.getCollisionRadius(),
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
	
	//render the score text
	std::string score = "Score: " + std::to_string(game.getPlayer()->getNumKills());
	char const* scoreChar = score.c_str();
	SDL_Surface *scoreText = TTF_RenderText_Solid( font, scoreChar,  {0, 0, 0, 255} );
	SDL_Rect scoreRect = { (int)(width/2) - 100 ,0,0,0};
	SDL_BlitSurface( scoreText,NULL, SDL_GetWindowSurface(win), &scoreRect );
	
	//render the fps text
	std::string fps = "FPS: " + std::to_string(game.getFPS());
	char const* FPSChar = fps.c_str();
	SDL_Surface *fpsText = TTF_RenderText_Solid( font, FPSChar,  {0, 0, 0, 255} );
	SDL_Rect fpsRect = { (int)(width/2) - 100 ,32,0,0};
	SDL_BlitSurface( fpsText,NULL, SDL_GetWindowSurface(win), &fpsRect );
	
	//Update the screen
	SDL_RenderPresent(ren);
}

void renderSystem::cleanup()
{
	TTF_CloseFont(font);
	SDL_DestroyRenderer((*this).ren);
	SDL_DestroyWindow((*this).win);	
	SDL_Quit();
}

//TODO: fix memory leak in these functions
//TODO: also fix more subtle memory leaks elsewhere
void renderSystem::renderPlayer(Player* player)
{
	//determine the target rectangle to render into
// 	SDL_Rect playerRect = { (int)(width/2 + player->getR() * cos(player->getTheta() * radPerDeg) - player->getCollisionRadius()),
// 							 (int)(height/2 + player->getR() * sin(player->getTheta() * radPerDeg) - player->getCollisionRadius()),
// 							 32,32};
// 	//generate the texture from the png data 
// 	SDL_Texture* tex = SDL_CreateTextureFromSurface( ren, player_png);
// 	//depending on the direction player is facing, point the texture right or left
// 	if (player->getShootingRight())
// 		SDL_RenderCopyEx(ren, tex, NULL, &playerRect, player->getTheta() + 90, NULL, SDL_FLIP_NONE);
// 	else
// 		SDL_RenderCopyEx(ren, tex, NULL, &playerRect, player->getTheta() + 90, NULL, SDL_FLIP_HORIZONTAL);
}

void renderSystem::renderSprite(gameObject* obj, SDL_Surface* png)
{
// 	SDL_Rect planetRect = { (int)(width/2 - obj->getCollisionRadius() - 5),
// 							 (int)(height/2- obj->getCollisionRadius() - 5),
// 							 (int)(obj->getCollisionRadius()*2)+10, (int)(obj->getCollisionRadius()*2)+10};
// 	SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, png);
// 	//render the planet
// 	SDL_RenderCopyEx(ren, tex, NULL, &planetRect, obj->getTheta(), NULL, SDL_FLIP_NONE);
}