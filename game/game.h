#ifndef __GAME_H__
#define __GAME_H__

#include "SDL.h"

const int SCREEN_WIDTH = 550;
const int SCREEN_HEIGHT = 400;

class Game
{
public:
	void			init();
public:	
	static Game*	get();
private:
	static Game*	_inst;
private:
	SDL_Texture*	bg_btn;
protected:
	Game();
	~Game();
public:
	void update();
	void handleMouseEvent(SDL_Event* e);
	void run();
};
#endif