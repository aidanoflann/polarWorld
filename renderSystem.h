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
	SDL_Texture* planet_texture;
	SDL_Texture* player_texture;
	SDL_Texture* guns_texture;
	SDL_Texture* cloud_texture;
	SDL_Texture* groundEnemy_texture;
	SDL_Texture* skyEnemy_texture;
	SDL_Texture* background_texture;
	SDL_Texture* backgroundSky_texture;
	SDL_Texture* controls_texture;
	TTF_Font *font;
	//window width and height
	double width;
	double height;
	void renderPlayer(Player*);
	//inputs are the object and its SDL_Surface as read from png
	void renderEnemy(gameObject*, SDL_Texture*, double); //third input is rotAngle
	void renderPlanet(gameObject*, SDL_Texture*);
	void renderCloud(gameObject*, SDL_Texture*);
	void renderGun(Player*);
	//inputs are message, xcoordinate and ycoordinate
	void renderText(std::string, int, int);
	void renderBackground();
	int cameraX;
	int cameraY;
	double cameraZoom;
	double cameraTheta;
	
public:
	int init();
	void draw(Game&);
	void cleanup();
};

#endif
