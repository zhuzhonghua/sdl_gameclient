#include "stdafx.h"

#include "toucharea.h"

#include "input.h"
namespace Zengine{
void TouchArea::onEvent(SDL_Event *e)
{
	if (e->type == SDL_MOUSEBUTTONDOWN &&
		e->button.button == SDL_BUTTON_LEFT)
	{
		int x = e->button.x;
		int y = e->button.y;

		if (this->overlapPoint(x, y))
		{
			touchDown = true;
			this->onTouchDown();
		}
	}

	if (e->type == SDL_MOUSEBUTTONUP &&
		e->button.button == SDL_BUTTON_LEFT)
	{
		int x = e->button.x;
		int y = e->button.y;

		if (this->overlapPoint(x, y))
		{
			touchDown = false;
			this->onTouchUp();
			this->onClick();
		}
		else
		{
			if (touchDown)
			{
				touchDown = false;
				this->onTouchUp();
			}
		}
	}
}

bool TouchArea::overlapPoint(float x, float y)
{
	return x >= this->_x && x < this->_x + _width && y >= this->_y && y < this->_y + _height;
}

TouchArea::TouchArea()
{
	touchDown = false;
	Input::inst()->addEventListener(this);
}

TouchArea::~TouchArea()
{
	Input::inst()->removeEventListener(this);
}
}