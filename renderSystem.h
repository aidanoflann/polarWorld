#ifndef GUARD_renderSystem
#define GUARD_renderSystem

class renderSystem;

#include <SDL.h>
#include "game.h"

#include "gameObject.h"
#include <vector>

class renderSystem
{
private:
	//renderer
	SDL_Renderer *ren;
	SDL_Window *win;
	//window width and height
	double width;
	double height;
	
public:
	int init();
	void draw(Game&);
	void cleanup();
};

#endif
