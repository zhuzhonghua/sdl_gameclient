#ifndef __GAME_H__
#define __GAME_H__

class Game
{
	static Game* _inst;
	Game();
public:
	SDL_Window* window;
	SDL_GLContext context;

	const int WIN_WIDTH = 1024;
	const int WIN_HEIGHT = 768;
public:	
	~Game();
	static Game* inst();
	void init();
	void run();
	void exit();
};
#endif