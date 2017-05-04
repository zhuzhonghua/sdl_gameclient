#pragma once

#include <SDL.h>
#include "GL/glew.h"

#include <string>

namespace Zhu{
	class Window
	{
	public:
		Window();
		~Window();

		int create(std::string windowName, int width, int height);

		void swapBuffer();
		int getWidth() { return _width; }
		int getHeight() { return _height; }
	private:

		int _width, _height;

		SDL_Window* _window;
	};
}