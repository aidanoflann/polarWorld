#ifndef GUARD_renderSystem
#define GUARD_renderSystem

class renderSystem;

#include <SDL.h>
#include <SDL_ttf.h>
#include "game.h"
#include "player.h"

#include "gameObject.h"
#include <vector>
#include <string>

class renderSystem
{
private:
	//renderer
	SDL_Renderer* ren;
	SDL_Window* win;
	//text surface and texture
	SDL_Texture* messageTextureStartUp1;
	SDL_Texture* messageTextureStartUp2;
	SDL_Texture* messageTexturePaused;
	SDL_Texture* messageTextureGameOverSelf;
	SDL_Texture* messageTextureGameOverEnemy;
	SDL_Texture* messageTextureScore;
	SDL_Texture* messageTexturePressR;
	SDL_Texture* messageTextureNumbers;
	SDL_Surface* messageSurface;
	SDL_Texture* planet_texture;
	SDL_Texture* player_texture;
	SDL_Texture* guns_texture;
	SDL_Texture* cloud_texture;
	SDL_Texture* groundEnemy_texture;
	SDL_Texture* skyEnemy_texture;
	SDL_Texture* background_texture;
	SDL_Texture* backgroundSky_texture;
	SDL_Texture* controls_texture;
	SDL_Texture* bullet_texture;
	TTF_Font* font;
	//the startup monsters
	gameObject gen;
	gameObject sen;
	//window width and height
	double width;
	double height;
	void renderPlayer(Player*);
	//inputs are the object and its SDL_Surface as read from png
	void renderEnemy(gameObject*, SDL_Texture*, double); //third input is rotAngle
	void renderPlanet(gameObject*, SDL_Texture*);
	void renderCloud(gameObject*, SDL_Texture*);
	void renderGun(Player*);
	void renderBullet(gameObject*, SDL_Texture*);
	//inputs are message, xcoordinate and ycoordinate
	void renderText(SDL_Texture*, int, int);
	void renderScore(int, int, int);
	void renderBackground();
	int cameraX;
	int cameraY;
	double cameraZoom;
	double cameraTheta;
	
public:
	void init();
	void draw(Game&);
	void cleanup();
};

#endif
