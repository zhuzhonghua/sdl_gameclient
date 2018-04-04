#include "root.h"
#include "movie.h"

Root::Root()
{

}

Root::~Root()
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);

	SDL_Quit();
}

bool Root::init(int height, int width)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	_window = SDL_CreateWindow("MyGame", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	_renderer = SDL_CreateRenderer(_window, -1, 0);

	return true;
}

Movie* Root::createMovie(const char* filename)
{
	Movie* m = new Movie(this);
	m->load(filename);
	_allMovies.push_back(m);
	return m;
}

void Root::addPlayList(Movie* m)
{
	_displayMovies.push_back(m);
}

void Root::update()
{
	std::vector<Movie*>::iterator iter = _displayMovies.begin();
	for (; iter != _displayMovies.end(); ++iter)
	{
		(*iter)->advance();
		(*iter)->update();
	}
}

void Root::run()
{
	SDL_Event e;
	SDL_StartTextInput();

	double lastTime = SDL_GetTicks();
	bool quit = false;
	//While application is running
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			//Handle keypress with current mouse position
			else if (e.type == SDL_TEXTINPUT)
			{
				int x = 0, y = 0;
				SDL_GetMouseState(&x, &y);
			}
		}

		this->update();

		SDL_RenderPresent(_renderer);
		SDL_Delay(1000.0f / 60.0f);
	}

	SDL_StopTextInput();
}