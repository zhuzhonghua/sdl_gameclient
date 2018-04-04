#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "gameobject.h"

class Character: public GameObject
{
	SDL_Texture*	tex;
	SDL_Renderer*	rend;
	
	SDL_Point		dest;
	bool			ani;
public:
	Character();
	~Character();	

	void update();
	void draw();
	void handleEvent(SDL_Event* e);
	void setPosition(int px, int py);

	void aniToPos(int x, int y);

};
#endif