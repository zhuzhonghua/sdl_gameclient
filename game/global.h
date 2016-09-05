#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#define GETWINDOWWIDTH() Global::getWidth()
#define GETWINDOWHEIGHT() Global::getHeight()

class Global
{

	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;

	static SDL_Window*		window;
	static SDL_Renderer*	renderer;
public:
	static void				init();
	static SDL_Window*		getWindow();
	static SDL_Renderer*	getRenderer();
	static int				getWidth();
	static int				getHeight();
};
#endif