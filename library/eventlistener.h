#ifndef __EVENT_LISTENER_H__
#define __EVENT_LISTENER_H__

class EventListener
{
public:
	virtual void onEvent(SDL_Event *e){}
	virtual ~EventListener(){}
};
#endif