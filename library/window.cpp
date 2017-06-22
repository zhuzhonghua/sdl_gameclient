#include "window.h"

using namespace Zhu;
Window::Window()
{

}

Window::~Window()
{

}

int Window::create(std::string windowName, int width, int height)
{
	_window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	//renderer = SDL_CreateRenderer(window, -1, 0);
	_width = width;
	_height = height;

	SDL_GLContext context = SDL_GL_CreateContext(_window);
	SDL_GL_SetSwapInterval(1);

	glewInit();

	std::printf("*** OpenGL Version: %s ***\n", glGetString(GL_VERSION));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

void Window::swapBuffer()
{
	SDL_GL_SwapWindow(_window);
}