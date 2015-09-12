//SDL2 lesson 1 from http://www.willusher.io/sdl2%20tutorials/2013/08/17/lesson-1-hello-world/

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <iostream>

int main() 
{
	//first let's initialise SDL2, 
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	
	//now create a window with title "Hello world!" at position (100, 100) with res (640x480)
	SDL_Window *win = SDL_CreateWindow("Polar World", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
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
	
	//A sleepy rendering loop, wait for 3 seconds and render and present the screen each time
	for (int i = 0; i < 3; ++i)
	{
		//First clear the renderer
		SDL_RenderClear(ren);
		//Set the colour of the next object to render
		SDL_SetRenderDrawColor(ren, 255, 0, 0, 100);
		//draw a filled circle at screen centre (ish)
		filledCircleRGBA(ren, 100, 100, 50, 0, 255, 0, 200);
		//set the colour back to white for the bg
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 100);
		//Update the screen
		SDL_RenderPresent(ren);
		//Take a quick break after all that hard work
		SDL_Delay(1000);
	}
	
	//cleanup
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}
