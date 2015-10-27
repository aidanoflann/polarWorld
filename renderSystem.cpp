#include <vector>
#include <cmath>
#include <iostream>

#include <SDL.h>
#include <math.h>
//#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <typeinfo>

#include "SDL2_gfxPrimitives.h"
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
	win = SDL_CreateWindow("Polar World", 800, 100, width, height, SDL_WINDOW_SHOWN);
	//if the window creation fails, *win will be NULL
	if (win == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}

	//set up a renderer
	ren = SDL_CreateRenderer(win, -1, 0);
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

	//load all textures
	//player
	SDL_Surface* player_png = IMG_Load("images/player.png");
	if(player_png == NULL)
		std::cout << "didn't load player.png: " << SDL_GetError() << std::endl;
	player_texture = SDL_CreateTextureFromSurface( ren, player_png);
	if(player_texture == NULL)
		std::cout << "didn't convert player.png to texture: " << SDL_GetError() << std::endl;
	//delete player_png;

	//player
	SDL_Surface* guns_png = IMG_Load("images/guns.png");
	if(guns_png == NULL)
		std::cout << "didn't load guns.png: " << SDL_GetError() << std::endl;
	guns_texture = SDL_CreateTextureFromSurface( ren, guns_png);
	//delete guns_png;

	//planet
	SDL_Surface* planet_png = IMG_Load("images/planet.png");
	if(planet_png == NULL)
		std::cout << "didn't load planet.png: " << SDL_GetError() << std::endl;
	planet_texture = SDL_CreateTextureFromSurface( ren, planet_png );
	//delete planet_png;

	//background
	SDL_Surface* background_png = IMG_Load("images/background.png");
	if(background_png == NULL)
		std::cout << "didn't load background.png: " << SDL_GetError() << std::endl;
	background_texture = SDL_CreateTextureFromSurface(ren, background_png);
	//delete background_png;

	SDL_Surface* backgroundSky_png = IMG_Load("images/backgroundSky.png");
	if(backgroundSky_png == NULL)
		std::cout << "didn't load backgroundSky.png: " << SDL_GetError() << std::endl;
	backgroundSky_texture = SDL_CreateTextureFromSurface(ren, backgroundSky_png);
	//delete backgroundSky_png;

	//cloud
	SDL_Surface* cloud_png = IMG_Load("images/cloud.png");
	if(cloud_png == NULL)
		std::cout << "didn't load cloud.png: " << SDL_GetError() << std::endl;
	cloud_texture = SDL_CreateTextureFromSurface(ren, cloud_png);
	//delete cloud_png;

	//groundEnemy
	SDL_Surface* groundEnemy_png = IMG_Load("images/groundEnemy.png");
	if(groundEnemy_png == NULL)
		std::cout << "didn't load player.png: " << SDL_GetError() << std::endl;
	groundEnemy_texture = SDL_CreateTextureFromSurface(ren, groundEnemy_png);
	//delete groundEnemy_png;

	//skyEnemy
	SDL_Surface* skyEnemy_png = IMG_Load("images/skyEnemy.png");
	if(skyEnemy_png == NULL)
		std::cout << "didn't load skyEnemy.png: " << SDL_GetError() << std::endl;
	skyEnemy_texture = SDL_CreateTextureFromSurface(ren, skyEnemy_png);
	//delete skyEnemy_png;

	//controls
	SDL_Surface* controls_png = IMG_Load("images/controls.png");
	if(controls_png == NULL)
		std::cout << "didn't load controls.png: " << SDL_GetError() << std::endl;
	controls_texture = SDL_CreateTextureFromSurface(ren, controls_png);
	//delete controls_png;
	
	//bullet
	SDL_Surface* bullet_png = IMG_Load("images/bullet.png");
	if(controls_png == NULL)
		std::cout << "didn't load bullet.png: " << SDL_GetError() << std::endl;
	bullet_texture = SDL_CreateTextureFromSurface(ren, bullet_png);
	
	//all messages
	//startup text
	messageSurface = TTF_RenderText_Blended( font, "Prepare to defend your planet.",  {255,255,255} );
	messageTextureStartUp1 = SDL_CreateTextureFromSurface(ren, messageSurface);
	//start key
	messageSurface = TTF_RenderText_Blended( font, "Press R to start.",  {255,255,255} );
	messageTextureStartUp2 = SDL_CreateTextureFromSurface(ren, messageSurface);
	//paused
	messageSurface = TTF_RenderText_Blended( font, "Paused",  {255,255,255} );
	messageTexturePaused = SDL_CreateTextureFromSurface(ren, messageSurface);
	//text on self-defeat
	messageSurface = TTF_RenderText_Blended( font, "You were the enemy all along.",  {255,255,255} );
	messageTextureGameOverSelf = SDL_CreateTextureFromSurface(ren, messageSurface);
	//text on alien touch
	messageSurface = TTF_RenderText_Blended( font, "The planet belongs to the aliens now.",  {255,255,255} );
	messageTextureGameOverEnemy = SDL_CreateTextureFromSurface(ren, messageSurface);
	//score:
	messageSurface = TTF_RenderText_Blended( font, "Score:",  {255,255,255} );
	messageTextureScore = SDL_CreateTextureFromSurface(ren, messageSurface);
	//restart key
	messageSurface = TTF_RenderText_Blended( font, "Press R to restart.",  {255,255,255} );
	messageTexturePressR = SDL_CreateTextureFromSurface(ren, messageSurface);
	//preloaded texture of all digits
	messageSurface = TTF_RenderText_Blended( font, "0123456789",  {255,255,255} );
	messageTextureNumbers = SDL_CreateTextureFromSurface(ren, messageSurface);
	SDL_FreeSurface(messageSurface);
	
	//the static enemies for the startup screen
	gen = GroundEnemy(270, 220, +1, 0);
	gen.setCollisionRadius( 100 );
	sen = SkyEnemy(270, 320, -1, 0);
	sen.setCollisionRadius( 100 );
	
}

void renderSystem::draw( Game& game)
{
	// update camera attributes
	cameraTheta = game.getPlayer()->getTheta() + 90;
	// First clear the renderer
	SDL_RenderClear(ren);
	// draw the background
	renderBackground();
	// render the player
	renderPlayer(game.getPlayer());
	renderGun(game.getPlayer());
	// render each enemy
	for (int i = 0; i != (game.getEnemies()).size(); i++)
	{
		gameObject en = *((game.getEnemies())[i]);
		if (en.getEnemyType() == "ground")
			renderEnemy(&en, groundEnemy_texture,0);
		else if (en.getEnemyType() == "sky")
			renderEnemy(&en, skyEnemy_texture,0);
	}
	// render the cloud
	renderCloud(game.getCloud(), cloud_texture);
	// render the Planet
	renderPlanet(game.getPlanet(), planet_texture);
	//render each bullet
	for (int i = 0; i != (game.getBullets()).size(); i++)
	{
		//Bullet b = *((game.getBullets())[i]);
		// filledCircleRGBA(ren,
						// cameraX + ((game.getBullets())[i])->getR() * cos( (-cameraTheta + ((game.getBullets())[i])->getTheta()) * radPerDeg),
						// cameraY + ((game.getBullets())[i])->getR() * sin( (-cameraTheta + ((game.getBullets())[i])->getTheta()) * radPerDeg),
						// 5,
						// 250, 100, 100, 255);
		renderBullet( (game.getBullets())[i], bullet_texture );
	}
	//if the game is paused, overlay a semitransparent black rect
	if ( game.isPaused() )
	{
		boxRGBA(ren, 0, 0, width, height, 0,0,0, 200);
		//render pause text
		renderText(messageTexturePaused, width/2, height/2- 25);
		renderText(messageTextureScore, width/2 - 20, height/2 + 25);
		renderScore(game.getPlayer()->getNumKills(),width/2 + 40, height/2 + 25);
	}

	else if ( game.isGameOverEnemy() )
	{
		boxRGBA(ren, 0, 0, width, height/2 - game.getPlayer()->getR() - 40, 0,0,0, 200);
		boxRGBA(ren, 0, height/2 - game.getPlayer()->getR() + 40, width, height, 0,0,0, 200);
		//render pause text
		renderText(messageTextureGameOverEnemy, width/2, height/2 - 30);
		renderText(messageTexturePressR, width/2, height/2);
		renderText(messageTextureScore, width/2 - 30, height/2 + 30);
		renderScore(game.getPlayer()->getNumKills(),width/2 + 30, height/2 + 30);
	}

	else if ( game.isGameOverSelf() )
	{
		boxRGBA(ren, 0, 0, width, height/2 - game.getPlayer()->getR() - 40, 0,0,0, 200);
		boxRGBA(ren, 0, height/2 - game.getPlayer()->getR() + 40, width, height, 0,0,0, 200);
		//render gameover text
		renderText(messageTextureGameOverSelf, width/2, height/2 - 30);
		renderText(messageTexturePressR, width/2, height/2);
		renderText(messageTextureScore, width/2 - 30, height/2 + 30);
		renderScore(game.getPlayer()->getNumKills(),width/2 + 30, height/2 + 30);
	}
	else if ( game.isStartup() )
	{
		boxRGBA(ren, 0, 0, width, height/2 - game.getPlayer()->getR() - 40, 0,0,0, 240);
		boxRGBA(ren, 0, height/2 - game.getPlayer()->getR() + 100, width, height, 0,0,0, 240);

		//render a couple of 
		//TODO: don't initialise these each time

		renderEnemy(&gen, groundEnemy_texture, 50);
		renderEnemy(&sen, skyEnemy_texture, -50);
		
		//render startup text
		renderText(messageTextureStartUp1, width/2, height/2 - 20);
		renderText(messageTextureStartUp2, width/2, height/2 + 20);

		SDL_Rect targetRectStartup = {0, 350, 800, 200};
		SDL_RenderCopy(ren, controls_texture, NULL, &targetRectStartup);
	}
	else if ( game.isRunning() )
	{
		//render the score text
		renderText(messageTextureScore, 50, 20 );
		renderScore(game.getPlayer()->getNumKills(),110, 20);
		//render the fps text
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
			playerRect = {0,frame*20, 20, 20};
		else if (player->ifMidair())
			playerRect = {40,frame*20, 20, 20};
		if (player->getThetaVelDirection() != 0)
			playerRect = {80,frame*20, 20, 20};
	}
	else
	{
		if (player->ifGrounded())
			playerRect = {20,frame*20, 20, 20};
		else if (player->ifMidair())
			playerRect = {60,frame*20, 20, 20};
		if (player->getThetaVelDirection() != 0)
			playerRect = {100,frame*20, 20, 20};
	}
	SDL_RenderCopy(ren, player_texture, &playerRect, &targetRect);
}

void renderSystem::renderGun(Player* player)
{
	//determine the target rectangle to render into
	SDL_Rect targetRect;
	if (player->ifJustShot() && player->getShootingRight())
	{
		targetRect = { (int)(cameraX - 2 + (player->getR() +2) * cos( (-cameraTheta + player->getTheta()) * radPerDeg) - player->getCollisionRadius()),
						(int)(cameraY + (player->getR() +2) * sin( (-cameraTheta + player->getTheta()) * radPerDeg) - player->getCollisionRadius()),
						40,40};
	}
	else if (player->ifJustShot() && !player->getShootingRight())
	{
		targetRect = { (int)(cameraX + 2 + (player->getR() +2) * cos( (-cameraTheta + player->getTheta()) * radPerDeg) - player->getCollisionRadius()),
						(int)(cameraY + (player->getR() +2) * sin( (-cameraTheta + player->getTheta()) * radPerDeg) - player->getCollisionRadius()),
						40,40};
	}
	else
		targetRect = { (int)(cameraX + (player->getR() +2) * cos( (-cameraTheta + player->getTheta()) * radPerDeg) - player->getCollisionRadius()),
							 (int)(cameraY + (player->getR() +2) * sin( (-cameraTheta + player->getTheta()) * radPerDeg) - player->getCollisionRadius()),
							 40,40};

	int frame = player->getAnimationFrame();
	//render gun on top
	SDL_Rect gunsRect;
	if (player->getShootingRight())
	{
		gunsRect = {0 , frame*20, 20, 20};
	}
	else
	{
		gunsRect = {20, frame*20, 20, 20};
	}
	SDL_RenderCopy(ren, guns_texture, &gunsRect, &targetRect);

}

void renderSystem::renderPlanet(gameObject* obj, SDL_Texture* tex)
{
	//determine the target rectangle to render into
	SDL_Rect* planetRect = new SDL_Rect;
	(*planetRect).x = (int)(cameraX - 125);
	(*planetRect).y = (int)(cameraY - 125);
	(*planetRect).w = 250;
	(*planetRect).h = 250;
	//render the planet
	SDL_RenderCopyEx(ren, tex , NULL, planetRect, -cameraTheta + obj->getTheta(), NULL, SDL_FLIP_NONE);
	//SDL_RenderCopyEx(ren, tex , NULL, planetRect, 0, NULL, SDL_FLIP_NONE);
	//SDL_RenderCopy(ren, tex , NULL, planetRect);
	delete planetRect;
}

void renderSystem::renderCloud(gameObject* cloud, SDL_Texture* tex)
{
	//determine the target rectangle to render into
	SDL_Rect* targetRect = new SDL_Rect;
	(*targetRect).x = (int)(cameraX + (cloud->getR() ) * cos( (-cameraTheta + cloud->getTheta()) * radPerDeg) - cloud->getCollisionRadius());
	(*targetRect).y = (int)(cameraY + (cloud->getR() ) * sin( (-cameraTheta + cloud->getTheta()) * radPerDeg) - cloud->getCollisionRadius());
	(*targetRect).w = 50;
	(*targetRect).h = 50;
	SDL_Rect* cloudRect = new SDL_Rect;
	(*cloudRect).x = 0;
	(*cloudRect).y =  cloud->getAnimationFrame()*25;
	(*cloudRect).w = 25;
	(*cloudRect).h = 25;
	//render the planet
	SDL_RenderCopyEx(ren, tex , cloudRect, targetRect, 90 - cameraTheta + cloud->getTheta(), NULL, SDL_FLIP_NONE);
	//SDL_RenderCopy(ren, tex , &cloudRect, targetRect);
	delete targetRect;
	delete cloudRect;
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
			enemyRect = {0,frame*20, 20, 20};
		else if (obj->getThetaVelDirection() == -1)
			enemyRect = {20, frame*20, 20, 20};
	}
	else
	{
		if (obj->getThetaVelDirection() == 1)
			enemyRect = {40,frame*20, 20, 20};
		else if (obj->getThetaVelDirection() == -1)
			enemyRect = {60, frame*20, 20, 20};
	}
	//render
	SDL_RenderCopyEx(ren, tex , &enemyRect, &targetRect, rotAngle + 90 -cameraTheta + obj->getTheta(), NULL, SDL_FLIP_NONE);
	//SDL_RenderCopy(ren, tex , &enemyRect, &targetRect);
}

void renderSystem::renderText(SDL_Texture* tex, int xcoordinate, int ycoordinate)
{
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);

	SDL_Rect* targetRect = new SDL_Rect;
	(*targetRect).x = xcoordinate - w/2;
	(*targetRect).y = ycoordinate - h/2;
	(*targetRect).w = w;
	(*targetRect).h = h;

	SDL_RenderCopy( ren, tex, NULL, targetRect);
	delete targetRect;
}

void renderSystem::renderScore(int score, int xcoordinate, int ycoordinate)
{
	//let's use some recursion
	if (score >= 10)
		renderScore(score/10, xcoordinate-12, ycoordinate);
	
	int digit = score % 10;
	
	SDL_Rect* scoreRect = new SDL_Rect;
	(*scoreRect).x = 13*digit;
	(*scoreRect).y = 0;
	(*scoreRect).w = 12;
	(*scoreRect).h = 25;
	
	SDL_Rect* targetRect = new SDL_Rect;
	(*targetRect).x = xcoordinate;
	(*targetRect).y = ycoordinate - 14;
	(*targetRect).w = (*scoreRect).w;
	(*targetRect).h = (*scoreRect).h;

	SDL_RenderCopy( ren, messageTextureNumbers, scoreRect, targetRect);
	delete scoreRect;
	delete targetRect;
}

void renderSystem::renderBackground()
{
	//determine the target rectangle to render into
	//SDL_SetRenderDrawColor(ren, 200, 0, 0, 255);
	SDL_Rect targetRect = { -100, -200, 1000, 1000};
	SDL_Rect backgroundRect = {0, 40, 250, 170};
	//render the blue background
	SDL_RenderCopy(ren, backgroundSky_texture, NULL, NULL);
	//rotate and render the clouds
	#ifndef EMSCRIPTEN
	SDL_RenderCopyEx(ren, background_texture , NULL, NULL, -cameraTheta*0.7, NULL, SDL_FLIP_NONE);
	//SDL_RenderCopy(ren, background_texture , NULL, NULL);
	#endif
	//SDL_RenderCopy(ren, tex , NULL, &screenRect);
}

void renderSystem::renderBullet(gameObject* obj, SDL_Texture* tex)
{
	//determine the target rectangle to render into
	
	// cameraX + ((game.getBullets())[i])->getR() * cos( (-cameraTheta + ((game.getBullets())[i])->getTheta()) * radPerDeg),
	// cameraY + ((game.getBullets())[i])->getR() * sin( (-cameraTheta + ((game.getBullets())[i])->getTheta()) * radPerDeg),
	
	SDL_Rect* targetRect = new SDL_Rect;
	(*targetRect).x = (int)(cameraX + (obj->getR() + 5) * cos( (-cameraTheta + obj->getTheta()) * radPerDeg));
	(*targetRect).y = (int)(cameraY + (obj->getR() + 5) * sin( (-cameraTheta + obj->getTheta()) * radPerDeg));
	(*targetRect).w = 10;
	(*targetRect).h = 10;
	//render the planet
	SDL_RenderCopy(ren, tex , NULL, targetRect);
	delete targetRect;
}
