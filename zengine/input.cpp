#include "stdafx.h"

#include "input.h"
namespace Zengine{
Input* Input::_inst;

Input::Input()
{
	stop = false;
}

void Input::dispatch(SDL_Event * e)
{
	for (std::list<EventListener*>::iterator itr = listeners.begin();
		itr != listeners.end(); itr++)
	{
		(*itr)->onEvent(e);
	}
}

Input* Input::inst()
{
	if (_inst == NULL)
	{
		_inst = new Input();
	}

	return _inst;
}

void Input::addEventListener(EventListener* el)
{
	listeners.push_back(el);
}

void Input::removeEventListener(EventListener* el)
{
	for (std::list<EventListener*>::iterator itr = listeners.begin();
		itr != listeners.end(); itr++)
	{
		if ((*itr) == el)
		{
			listeners.erase(itr);
			break;
		}
	}
}
}