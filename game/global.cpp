#include "stdafx.h"

#include "global.h"

SDL_Window* Global::window;
SDL_Renderer* Global::renderer;

int Global::SCREEN_WIDTH;
int Global::SCREEN_HEIGHT;


SDL_Window*	 Global::getWindow()
{
	return Global::window;
}

SDL_Renderer* Global::getRenderer()
{
	return Global::renderer;
}

void Global::init()
{
	SCREEN_WIDTH = 600;
	SCREEN_HEIGHT = 400;

	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("MyGame", 
								SDL_WINDOWPOS_UNDEFINED, 
								SDL_WINDOWPOS_UNDEFINED, 
								SCREEN_WIDTH, 
								SCREEN_HEIGHT, 
								SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);
}

int	Global::getWidth()
{
	return Global::SCREEN_WIDTH;
}

int	Global::getHeight()
{
	return Global::SCREEN_HEIGHT;
}
