#ifndef __TOUCH_AREA_H__
#define __TOUCH_AREA_H__

#include "eventlistener.h"
#include "gizmo.h"
namespace Zengine{
class TouchArea : public EventListener, public Gizmo
{
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