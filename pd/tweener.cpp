#include "tweener.h"

#include "game.h"

Tweener::Tweener(Gizmo* target, float interval)
{
	this->target = target;
	this->interval = interval;
	listener = NULL;
	elapsed = 0;
}

void Tweener::update()
{
	elapsed += Game::elapsed;
	if (elapsed >= interval) 
	{
		updateValues(1);
		onComplete();
		kill();
	}
	else 
	{
		updateValues(elapsed / interval);
	}
}
