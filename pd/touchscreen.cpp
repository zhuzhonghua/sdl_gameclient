#include "touchscreen.h"

Signal<TouchScreen::Touch> TouchScreen::event(true);

std::map<int, TouchScreen::Touch*> TouchScreen::pointers;

float TouchScreen::x;
float TouchScreen::y;
bool TouchScreen::touched;

TouchScreen::Touch::Touch(SDL_Event* e, int index)
{
	float x = e->motion.x;
	float y = e->motion.y;

	start.x = x;
	start.y = y;

	current = start;

	down = true;
}

void TouchScreen::Touch::update(SDL_Event* e, int index)
{
	//current.set(e.getX(index), e.getY(index));
}

TouchScreen::Touch* TouchScreen::Touch::up()
{
	down = false;
	return this;
}

void TouchScreen::processTouchEvents(std::vector<SDL_Event>& events)
{
	for (int i = 0; i < events.size(); i++)
	{
		SDL_Event& e = events[i];

		Touch* touch = NULL;
		switch (e.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			touched = true;
			touch = new Touch(&e, 0);
			//pointers.put(e.getPointerId(0), touch);
			pointers.insert(std::make_pair(0, touch));
			event.dispatch(touch);
			break;
		case SDL_MOUSEBUTTONUP:{
			touched = false;			
			std::map<int, TouchScreen::Touch*>::iterator itr = pointers.find(0);
			touch = itr->second;
			pointers.erase(itr);
			event.dispatch(touch->up());
			delete touch;
			break;
		}
		}
	}
}