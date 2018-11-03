#ifndef __EVENT_LISTENER_H__
#define __EVENT_LISTENER_H__

namespace Zengine{
class EventListener
{
public:
	virtual void onEvent(SDL_Event *e){}
	virtual ~EventListener(){}
};
}
#endif