#include "stdafx.h"

#include "game.h"

Game* Game::_inst;

Game::Game()
{

}

Game::~Game()
{

}

Game* Game::inst()
{
	if (_inst == NULL)
	{
		_inst = new Game();
	}
	return _inst;
}

void Game::init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	window = SDL_CreateWindow("Library", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	context = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(1);

	// InitGL
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ASSERT(glGetError() == GL_NO_ERROR);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	ASSERT(glGetError() == GL_NO_ERROR);

	glClearColor(0.f, 0.f, 0.f, 1.f);
	ASSERT(glGetError() == GL_NO_ERROR);
}

void Game::run()
{
	SDL_StartTextInput();
	bool quit = false;
	while (!quit)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		glBegin(GL_QUADS);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.5f, -0.5f);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(-0.5f, 0.5f);
		glEnd();

		SDL_GL_SwapWindow(window);
	}
	SDL_StopTextInput();
}

void Game::exit()
{

}