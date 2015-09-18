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

using std::cout; using std::endl;

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
	cameraX = width/2;
	cameraY = height/2;
	cameraZoom = 1;
	cameraTheta = 0;
	
	//first let's initialise SDL2, 
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		exit(1);
	}
	
	//set up window
	SDL_Window *win = SDL_CreateWindow("Polar World", 100, 100, (*this).width, (*this).height, SDL_WINDOW_SHOWN);
	//if the window creation fails, *win will be NULL
	if (win == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	
	//set up a renderer
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, 0);
	if (ren == nullptr)
	{
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
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
	//player
	SDL_Surface* player_png = IMG_Load("images/player.png");
	player_texture = SDL_CreateTextureFromSurface( ren, player_png);
	delete player_png;
	
	//planet
	SDL_Surface* planet_png = IMG_Load("images/planet.png");
	planet_texture = SDL_CreateTextureFromSurface( ren, planet_png );
	delete planet_png;
	
	//background
	SDL_Surface* background_png = IMG_Load("images/background.png");
	background_texture = SDL_CreateTextureFromSurface(ren, background_png);
	delete background_png;
	SDL_Surface* backgroundSky_png = IMG_Load("images/backgroundSky.png");
	backgroundSky_texture = SDL_CreateTextureFromSurface(ren, backgroundSky_png);
	delete backgroundSky_png;
	
	//cloud
	SDL_Surface* cloud_png = IMG_Load("images/cloud.png");
	cloud_texture = SDL_CreateTextureFromSurface(ren, cloud_png);
	delete cloud_png;
	
	//groundEnemy
	SDL_Surface* groundEnemy_png = IMG_Load("images/groundEnemy.png");
	groundEnemy_texture = SDL_CreateTextureFromSurface(ren, groundEnemy_png);
	delete groundEnemy_png;
	
	//skyEnemy
	SDL_Surface* skyEnemy_png = IMG_Load("images/skyEnemy.png");
	skyEnemy_texture = SDL_CreateTextureFromSurface(ren, skyEnemy_png);
	delete skyEnemy_png;
}

void renderSystem::draw( Game& game)
{
	//update camera attributes
	cameraTheta = game.getPlayer()->getTheta() + 90;
	
	//First clear the renderer
	SDL_RenderClear(ren);
	
	//draw the background
	renderBackground();
// 	SDL_SetRenderDrawColor((*this).ren, 255, 255, 255, 255);
// 	SDL_Rect bg = {0,0, (int)std::round(width), (int)std::round(height)};
// 	SDL_RenderFillRect((*this).ren, &bg);
	
	//render the player
	renderPlayer(game.getPlayer());
	
	//render the Planet
	renderPlanet(game.getPlanet(), planet_texture);
	
	//render the cloud
	renderCloud(game.getCloud(), cloud_texture);
	
	//render each enemy
	for (int i = 0; i != (game.getEnemies()).size(); i++)
	{
		gameObject en = *((game.getEnemies())[i]);
		if (en.getEnemyType() == "ground")
			renderEnemy(&en, groundEnemy_texture);
		else if (en.getEnemyType() == "sky")
			renderEnemy(&en, skyEnemy_texture);
	}
	
	//render each bullet
	//TODO: Figure out iter implementation
	//for ( std::vector<Bullet>::iterator iter = (game.getBullets()).begin(); iter != (game.getBullets()).end(); iter++)
	for (int i = 0; i != (game.getBullets()).size(); i++)
	{
		Bullet b = *((game.getBullets())[i]);
		filledCircleRGBA(ren,
						cameraX + b.getR() * cos( (-cameraTheta + b.getTheta()) * radPerDeg),
						cameraY + b.getR() * sin( (-cameraTheta + b.getTheta()) * radPerDeg),
						b.getCollisionRadius(),
						250, 100, 100, 255);
	}
	//render the score text
	renderText("Score: " + std::to_string(game.getPlayer()->getNumKills()), 10, 0 );
	
 	//render the fps text
	renderText("FPS: " + std::to_string(game.getFPS()), 10, 32 );
// 	
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

void renderSystem::renderPlayer(Player* player)
{
	//determine the target rectangle to render into
	SDL_Rect targetRect = { (int)(cameraX + (player->getR() +2) * cos( (-cameraTheta + player->getTheta()) * radPerDeg) - player->getCollisionRadius()),
							 (int)(cameraY + (player->getR() +2) * sin( (-cameraTheta + player->getTheta()) * radPerDeg) - player->getCollisionRadius()),
							 38,38};
	//depending on the direction player is facing, point the texture right or left
	SDL_Rect playerRect;
	if (player->getShootingRight())
		SDL_Rect playerRect = {0,0, 32, 32};
	else 
		SDL_Rect playerRect = {32, 0, 32, 32};
	//render
	SDL_RenderCopy(ren, player_texture, &playerRect, &targetRect);
}

void renderSystem::renderPlanet(gameObject* obj, SDL_Texture* tex)
{
	//determine the target rectangle to render into
	SDL_Rect planetRect = { (int)(cameraX - obj->getCollisionRadius() - 5),
							 (int)(cameraY - obj->getCollisionRadius() - 5),
							 (int)(obj->getCollisionRadius()*2)+10, (int)(obj->getCollisionRadius()*2)+10};
	//render the planet
	SDL_RenderCopyEx(ren, tex , NULL, &planetRect, -cameraTheta + obj->getTheta(), NULL, SDL_FLIP_NONE);
}

void renderSystem::renderCloud(gameObject* cloud, SDL_Texture* tex)
{
	//determine the target rectangle to render into
	SDL_Rect targetRect = { (int)(cameraX + (cloud->getR() ) * cos( (-cameraTheta + cloud->getTheta()) * radPerDeg) - cloud->getCollisionRadius()),
							 (int)(cameraY + (cloud->getR() ) * sin( (-cameraTheta + cloud->getTheta()) * radPerDeg) - cloud->getCollisionRadius()),
							 (int)(cloud->getCollisionRadius()*2.5), (int)(cloud->getCollisionRadius()*2.5)};
	//render the planet
	SDL_RenderCopy(ren, tex , NULL, &targetRect);
}

void renderSystem::renderEnemy(gameObject* obj, SDL_Texture* tex)
{
	//determine the target rectangle to render into
	SDL_Rect targetRect = { (int)(cameraX + (obj->getR() ) * cos( (-cameraTheta + obj->getTheta()) * radPerDeg) - obj->getCollisionRadius()),
							 (int)(cameraY + (obj->getR() ) * sin( (-cameraTheta + obj->getTheta()) * radPerDeg) - obj->getCollisionRadius()),
							 (int)(obj->getCollisionRadius()*2), (int)(obj->getCollisionRadius()*2)};
	SDL_Rect enemyRect;
	if (obj->getThetaVelDirection() == 1)
		SDL_Rect enemyRect = {0,0, 20, 20};
	else if (obj->getThetaVelDirection() == -1)
		SDL_Rect enemyRect = {20, 0, 20, 20};
	//render
	SDL_RenderCopyEx(ren, tex , &enemyRect, &targetRect, 90 -cameraTheta + obj->getTheta(), NULL, SDL_FLIP_NONE);
}

void renderSystem::renderText(std::string message, int xcoordinate, int ycoordinate)
{
	char const* scoreChar = message.c_str();
	SDL_Surface* scoreText = TTF_RenderText_Solid( font, scoreChar,  {0, 0, 0, 255} );
	SDL_Rect scoreRect = { xcoordinate , ycoordinate, 0, 0};
	SDL_BlitSurface( scoreText,NULL, SDL_GetWindowSurface(win), &scoreRect );
	
	//cleanup
	//delete scoreChar;
	SDL_FreeSurface( scoreText );
}

void renderSystem::renderBackground()
{
		//determine the target rectangle to render into
	SDL_Rect targetRect = { -100, -200, 1000, 1000};
	SDL_Rect backgroundRect = {0, 40, 250, 170};
	//render the blue background
	SDL_RenderCopy(ren, backgroundSky_texture, NULL, NULL);
	//rotate and render the clouds
	SDL_RenderCopyEx(ren, background_texture , NULL, NULL, -cameraTheta*0.7, NULL, SDL_FLIP_NONE);
	//SDL_RenderCopy(ren, tex , NULL, &screenRect);
}