#ifndef __TOUCH_AREA_H__
#define __TOUCH_AREA_H__

#include "stdafx.h"
#include "eventlistener.h"

namespace Zhu{
	class TouchArea : public EventListener
	{
	public:
		float x;
		float y;
		float width;
		float height;

	protected:
		bool overlapPoint(float x, float y);

		bool touchDown;
	public:
		void onEvent(SDL_Event *e);

		virtual void onTouchDown(){}
		virtual void onTouchUp(){}
		virtual void onClick(){}

		TouchArea();
		~TouchArea();
	};
}
#endif