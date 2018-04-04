#include "stdafx.h"

#include "ground.h"
#include "global.h"

#include "camera.h"

Ground::Ground()
{
	rend = Global::getRenderer();
	sfc = IMG_LoadTexture(rend, "ground.jpg");
	rect.x = 0;
	rect.y = 0;
	rect.w = 600;
	rect.h = 400;
}

Ground::~Ground()
{
	SDL_DestroyTexture(sfc);
}

void Ground::draw()
{	
	SDL_Rect tmp = rect;
	tmp.x = rect.x - MAINCAMERA()->getX();
	tmp.y = rect.y - MAINCAMERA()->getY();

	SDL_RenderCopy(rend, sfc, NULL, &tmp);
}