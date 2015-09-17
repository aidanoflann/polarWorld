#ifndef GUARD_renderSystem
#define GUARD_renderSystem

class renderSystem;

#include <SDL.h>
#include <SDL_ttf.h>
#include "game.h"
#include "player.h"

#include "gameObject.h"
#include <vector>

class renderSystem
{
private:
	//renderer
	SDL_Renderer* ren;
	SDL_Window* win;
	SDL_Surface* planet_png;
	SDL_Surface* player_png;
	TTF_Font *font;
	//window width and height
	double width;
	double height;
	void renderPlayer(Player*);
	//inputs are the object and its SDL_Surface as read from png
	void renderSprite(gameObject*, SDL_Surface*);
	
public:
	int init();
	void draw(Game&);
	void cleanup();
};

#endif
