#include "game.h"
#include "SDL_image.h"

SDL_Window* gWindow = NULL;
SDL_Renderer* renderer = NULL;

Game* Game::_inst;
Game* Game::get()
{
	if (_inst)
	{
		return _inst;
	}
	else
	{
		_inst = new Game();
		return _inst;
	}
}

void Game::init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	gWindow = SDL_CreateWindow("MyGame", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(gWindow, -1, 0);

	bg_btn = IMG_LoadTexture(renderer, "gllogo.png");
}

Game::Game()
{

}

Game::~Game()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(gWindow);

	SDL_Quit();
}

void Game::update()
{
	int w, h;
	SDL_QueryTexture(bg_btn, NULL, NULL, &w, &h);
	SDL_Rect tex_rect;
	tex_rect.x = SCREEN_WIDTH / 2 - w / 2;
	tex_rect.y = SCREEN_HEIGHT / 2 - h / 2;
	tex_rect.w = w;
	tex_rect.h = h;

	SDL_RenderCopy(renderer, bg_btn, NULL, &tex_rect);
}

void Game::handleMouseEvent(SDL_Event* e)
{

}

void Game::run()
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
			else if (e.type == SDL_MOUSEMOTION ||
					 e.type == SDL_MOUSEBUTTONDOWN ||
					 e.type == SDL_MOUSEBUTTONUP)
			{
				handleMouseEvent(&e);
			}
		}

		update();
		SDL_RenderPresent(renderer);
		SDL_Delay(1000.0f / 60.0f);
	}

	SDL_StopTextInput();
}