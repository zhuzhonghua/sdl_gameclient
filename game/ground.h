#ifndef __GROUND_H__
#define __GROUND_H__

#include "gameobject.h"

class Ground: public GameObject
{
	SDL_Texture*	sfc;
	SDL_Renderer*	rend;
public:
	Ground();
	~Ground();

	void draw();
};
#endif