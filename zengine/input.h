#ifndef __INPUT__
#define __INPUT__

#include "stdafx.h"
#include "eventlistener.h"

namespace Zengine{
class Input
{
private:
	static Input*	_inst;
protected:
	Input();
public:
	bool stop;
	std::list<EventListener*> listeners;
public:
	static Input*	inst();
	void dispatch(SDL_Event * e);
	void addEventListener(EventListener* el);
	void removeEventListener(EventListener* el);
};
}
#endif