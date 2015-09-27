#include <vector>
#include <cmath>
#include <iostream>

#include <SDL.h>
#include <math.h>
//#include <SDL2_gfxPrimitives.h>
//#include <SDL_ttf.h>
#include <SDL_image.h>

#include <typeinfo>

#include "renderSystem.h"
#include "gameObject.h"
#include "planet.h"
#include "game.h"
#include "player.h"
#include "groundEnemy.h"
#include "skyEnemy.h"

using std::cout; using std::endl;

double radPerDeg = 0.01745329251;

void renderSystem::init()
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
	
	//set up window
	SDL_Window *win = SDL_CreateWindow("Polar World", 100, 100, width, height, SDL_WINDOW_SHOWN);
	//if the window creation fails, *win will be NULL
	if (win == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	
	//set up a renderer
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED || SDL_RENDERER_SOFTWARE || SDL_RENDERER_PRESENTVSYNC);
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
	
	//player
	SDL_Surface* guns_png = IMG_Load("images/guns.png");
	guns_texture = SDL_CreateTextureFromSurface( ren, guns_png);
	delete guns_png;
	
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
	
	//controls
	SDL_Surface* controls_png = IMG_Load("images/controls.png");
	controls_texture = SDL_CreateTextureFromSurface(ren, controls_png);
	delete controls_png;
}

void renderSystem::draw( Game& game)
{
	//update camera attributes
	cameraTheta = game.getPlayer()->getTheta() + 90;
	
	//First clear the renderer
	SDL_RenderClear(ren);
	
	//draw the background
	renderBackground();
	
	//render the player
	renderPlayer(game.getPlayer());
	renderGun(game.getPlayer());
	
	//render each enemy
	for (int i = 0; i != (game.getEnemies()).size(); i++)
	{
		gameObject en = *((game.getEnemies())[i]);
		if (en.getEnemyType() == "ground")
			renderEnemy(&en, groundEnemy_texture,0);
		else if (en.getEnemyType() == "sky")
			renderEnemy(&en, skyEnemy_texture,0);
	}
	
	//render the cloud
	renderCloud(game.getCloud(), cloud_texture);
	
	//render the Planet
	renderPlanet(game.getPlanet(), planet_texture);
	
	//render each bullet
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

	//if the game is paused, overlay a semitransparent black rect
	if ( game.isPaused() )
	{
		boxRGBA(ren, 0, 0, width, height, 0,0,0, 200);
	}
	
	if ( game.isGameOverEnemy() )
	{
		boxRGBA(ren, 0, 0, width, height/2 - game.getPlayer()->getR() - 40, 0,0,0, 200);
		boxRGBA(ren, 0, height/2 - game.getPlayer()->getR() + 40, width, height, 0,0,0, 200);
		//render pause text
		renderText("The planet belongs to the aliens now.", width/2 - 195, height/2 - 25);
		renderText("Score: "  + std::to_string(game.getPlayer()->getNumKills()), width/2 - 50, height/2 );
		renderText("Press R to restart.", width/2 - 100, height/2 + 25);
	}
	
	if ( game.isGameOverSelf() )
	{
		boxRGBA(ren, 0, 0, width, height/2 - game.getPlayer()->getR() - 40, 0,0,0, 200);
		boxRGBA(ren, 0, height/2 - game.getPlayer()->getR() + 40, width, height, 0,0,0, 200);
		//render pause text
		renderText("You were the monster all along.", width/2 - 170, height/2 - 25);
		renderText("Score: "  + std::to_string(game.getPlayer()->getNumKills()), width/2 - 50, height/2 );
		renderText("Press R to restart.", width/2 - 100, height/2 + 25);
	}
	
	if ( game.isStartup() )
	{
		boxRGBA(ren, 0, 0, width, height/2 - game.getPlayer()->getR() - 40, 0,0,0, 240);
		boxRGBA(ren, 0, height/2 - game.getPlayer()->getR() + 40, width, height, 0,0,0, 240);

		//render a couple of enemies
		gameObject gen = GroundEnemy(270, 220, +1, 0);
		gen.setCollisionRadius( 100 );
		renderEnemy(&gen, groundEnemy_texture, 50);

		gameObject sen = SkyEnemy(270, 320, -1, 0);
		sen.setCollisionRadius( 100 );
		renderEnemy(&sen, skyEnemy_texture, -50);
		//render pause text
		
		renderText("Prepare to defend your planet.", width/2 - 165, height/2 - 25);
		renderText("Press R to start.", width/2 - 85, height/2 + 25);
		
		SDL_Rect targetRect = {0, 350, 800, 200};
		SDL_RenderCopy(ren, controls_texture, NULL, &targetRect);
	}
	
	if ( game.isRunning() )
	{
		//render the score text
		renderText("Score: " + std::to_string(game.getPlayer()->getNumKills()), 10, 0 );
		
		//render the fps text
		std::cout << std::to_string(game.getFPS()) << std::endl;
		//renderText("FPS: " + std::to_string(game.getFPS()), 10, 32 );
	}
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
							 40,40};
	//depending on the direction player is facing, point the texture right or left
	int frame = player->getAnimationFrame();
	SDL_Rect playerRect;
	if (player->getRunningRight())
	{
		if (player->ifGrounded() )
			SDL_Rect playerRect = {0,frame*20, 20, 20};
		else if (player->ifMidair())
			SDL_Rect playerRect = {40,frame*20, 20, 20};
		if (player->getThetaVelDirection() != 0)
			SDL_Rect playerRect = {80,frame*20, 20, 20};
	}
	else
	{
		if (player->ifGrounded())
			SDL_Rect playerRect = {20,frame*20, 20, 20};
		else if (player->ifMidair())
			SDL_Rect playerRect = {60,frame*20, 20, 20};
		if (player->getThetaVelDirection() != 0)
			SDL_Rect playerRect = {100,frame*20, 20, 20};
	}
	SDL_RenderCopy(ren, player_texture, &playerRect, &targetRect);
}

void renderSystem::renderGun(Player* player)
{
	//determine the target rectangle to render into
	SDL_Rect targetRect = { (int)(cameraX + (player->getR() +2) * cos( (-cameraTheta + player->getTheta()) * radPerDeg) - player->getCollisionRadius()),
							 (int)(cameraY + (player->getR() +2) * sin( (-cameraTheta + player->getTheta()) * radPerDeg) - player->getCollisionRadius()),
							 40,40};
	int frame = player->getAnimationFrame();
	//render gun on top
	SDL_Rect gunsRect;
	if (player->getShootingRight())
	{
		SDL_Rect gunsRect = {0, frame*20, 20, 20};
	}
	else
	{
		SDL_Rect gunsRect = {20, frame*20, 20, 20};
	}
	SDL_RenderCopy(ren, guns_texture, &gunsRect, &targetRect);
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
							 50, 50};
	int frame = cloud->getAnimationFrame();
	SDL_Rect cloudRect = {0, frame*25, 25, 25};
	//render the planet
	SDL_RenderCopyEx(ren, tex , &cloudRect, &targetRect, 90 - cameraTheta + cloud->getTheta(), NULL, SDL_FLIP_NONE);
}

void renderSystem::renderEnemy(gameObject* obj, SDL_Texture* tex, double rotAngle)
{
	//determine the target rectangle to render into
	SDL_Rect targetRect = { (int)(cameraX + (obj->getR() ) * cos( (-cameraTheta + obj->getTheta()) * radPerDeg) - obj->getCollisionRadius()),
							 (int)(cameraY + (obj->getR() ) * sin( (-cameraTheta + obj->getTheta()) * radPerDeg) - obj->getCollisionRadius()),
							 (int)(obj->getCollisionRadius()*2), (int)(obj->getCollisionRadius()*2)};
	SDL_Rect enemyRect;
	int frame = obj->getAnimationFrame();
	if (obj->isNotExploding())
	{
		if (obj->getThetaVelDirection() == 1)
			SDL_Rect enemyRect = {0,frame*20, 20, 20};
		else if (obj->getThetaVelDirection() == -1)
			SDL_Rect enemyRect = {20, frame*20, 20, 20};
	}
	else
	{
		if (obj->getThetaVelDirection() == 1)
			SDL_Rect enemyRect = {40,frame*20, 20, 20};
		else if (obj->getThetaVelDirection() == -1)
			SDL_Rect enemyRect = {60, frame*20, 20, 20};
	}
	//render
	SDL_RenderCopyEx(ren, tex , &enemyRect, &targetRect, rotAngle + 90 -cameraTheta + obj->getTheta(), NULL, SDL_FLIP_NONE);
}

void renderSystem::renderText(std::string message, int xcoordinate, int ycoordinate)
{
	char const* scoreChar = message.c_str();
	SDL_Surface* scoreText = TTF_RenderText_Solid( font, scoreChar,  {255, 255, 255, 255} );
	SDL_Rect scoreRect = { xcoordinate , ycoordinate, 0, 0};
	SDL_BlitSurface( scoreText,NULL, SDL_GetWindowSurface(win), &scoreRect );
	
	//cleanup
	//delete scoreChar;
	SDL_FreeSurface( scoreText );
}

void renderSystem::renderBackground()
{
		//determine the target rectangle to render into
	SDL_SetRenderDrawColor(ren, 200, 0, 0, 255);
	SDL_Rect targetRect = { -100, -200, 1000, 1000};
	SDL_Rect backgroundRect = {0, 40, 250, 170};
	//render the blue background
	SDL_RenderCopy(ren, backgroundSky_texture, NULL, NULL);
	//rotate and render the clouds
	#ifndef EMSCRIPTEN
	SDL_RenderCopyEx(ren, background_texture , NULL, NULL, -cameraTheta*0.7, NULL, SDL_FLIP_NONE);
	#endif
	//SDL_RenderCopy(ren, tex , NULL, &screenRect);
}