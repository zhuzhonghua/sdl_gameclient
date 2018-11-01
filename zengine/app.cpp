#include "stdafx.h"

#include "app.h"
#include "timing.h"
#include "glslprogram.h"
#include "image.h"

namespace Zengine{
App* App::inst;

int App::width = 640;
int App::height = 960;

float App::timeScale = 1.0f;
float App::elapsed = 0.0f;

Image* img;

App::App()
{
	ASSERT(inst == NULL);
	inst = this;

	state = State::PLAY;
	_now = 0;
	_step = 0;
}

App::~App()
{
}

void App::init(const std::string& title)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	_context = SDL_GL_CreateContext(_window);

	SDL_GL_SetSwapInterval(1);
	glewInit();

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	std::printf("*** OpenGL Version: %s ***\n", glGetString(GL_VERSION));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0, 0.0, 1.0, 1.0);

	//TTF_Init();
	//
	//// setup sound
	//int flag = MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG;
	//if (Mix_Init(flag) & flag != flag)
	//{
	//	std::string err = Mix_GetError();
	//	fatalError("Mix_Init error " + err);
	//}
	//
	//// 44100 cd rate, more cpu power
	//if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
	//{
	//	std::string err = Mix_GetError();
	//	fatalError("Mix_OpenAudio error " + err);
	//}
	//
	GLSLProgram::inst()->init();
	//
	img = new Image("workground.jpg");
}


void App::run()
{
	FPSLimiter fps;
	fps.setMaxFPS(60.0f);

	while (state == State::PLAY)
	{
		fps.begin();

		long rightNowTick = SDL_GetTicks();
		_step = (_now == 0 ? 0 : rightNowTick - _now);
		_now = rightNowTick;

		processInput();

		update();

		draw();

		SDL_GL_SwapWindow(_window);

		_fps = fps.end();
	}
}

void App::processInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT:
			state = State::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_motionEvents.push_back(e);
			break;
		case SDL_KEYDOWN:

			break;
		case SDL_KEYUP:

			break;
		case SDL_MOUSEBUTTONDOWN:
			_motionEvents.push_back(e);
			break;
		case SDL_MOUSEBUTTONUP:
			_motionEvents.push_back(e);
			break;
		}
	}

	//TouchScreen::processTouchEvents(_motionEvents);
	_motionEvents.clear();
}


void App::update()
{
	App::elapsed = App::timeScale * _step * 0.001f;

	//_scene->update();	
}

void App::draw()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	img->draw();
}

}