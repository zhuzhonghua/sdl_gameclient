#include "stdafx.h"
#include "game.h"
#include "global.h"

#include "ground.h"
#include "character.h"
#include "camera.h"

void Game::init()
{
	Global::init();

	rend = Global::getRenderer();

	Ground* g = new Ground();
	g->setZ(0);
	_objects.push_back(g);

	Character* me = new Character();
	me->setPosition(GETWINDOWWIDTH() / 2, GETWINDOWHEIGHT() / 2);
	me->setZ(1);
	_objects.push_back(me);	

	MAINCAMERA()->setMe(me);
}

Game::Game()
{

}

Game::~Game()
{
	for (std::list<GameObject*>::iterator itr = _objects.begin();
		itr != _objects.end(); ++itr)
	{
		delete *itr;
	}

	SDL_Quit();
}

bool goZOrder(const GameObject* i, const GameObject* j)
{ 
	return (const_cast<GameObject*>(i)->getZ() < const_cast<GameObject*>(j)->getZ()); 
}

void Game::update()
{
	MAINCAMERA()->update();

	_objects.sort(goZOrder);

	for (std::list<GameObject*>::iterator itr = _objects.begin();
		itr != _objects.end(); ++itr)
	{
		(*itr)->update();
	}

	for (std::list<GameObject*>::iterator itr = _objects.begin();
		itr != _objects.end(); ++itr)
	{
		(*itr)->draw();
	}
}

void Game::handleMouseEvent(SDL_Event* e)
{
	for (std::list<GameObject*>::iterator itr = _objects.begin();
		itr != _objects.end(); ++itr)
	{
		(*itr)->handleEvent(e);
	}
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
			else if (/*e.type == SDL_MOUSEMOTION ||*/
					 e.type == SDL_MOUSEBUTTONDOWN ||
					 e.type == SDL_MOUSEBUTTONUP)
			{
				handleMouseEvent(&e);
			}
		}

		SDL_RenderClear(rend);
		update();
		SDL_RenderPresent(rend);
		SDL_Delay(1000.0f / 60.0f);
	}

	SDL_StopTextInput();
}