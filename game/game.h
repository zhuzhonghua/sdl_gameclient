#ifndef __GAME_H__
#define __GAME_H__
#include <list>

#include "gameobject.h"

class Game
{
	SDL_Renderer*	rend;
	std::list<GameObject*>	_objects;

public:
	void			init();
public:
	Game();
	~Game();
public:
	void update();
	void handleMouseEvent(SDL_Event* e);
	void run();
};
#endif