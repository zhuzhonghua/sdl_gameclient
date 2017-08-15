#include "gamescene.h"

GameScene* GameScene::scene;

void GameScene::brightness(bool value)
{

}

void GameScene::init()
{

}

Emitter* GameScene::emitter()
{
	if (scene != NULL) 
	{
		Emitter* emitter = (Emitter*)scene->emitters->recycle("Emitter");
		if (emitter == NULL)
		{
			emitter = new Emitter();
			scene->emitters->add(emitter);
		}
		emitter->revive();
		return emitter;
	}
	else 
	{
		return NULL;
	}
}

Ripple* GameScene::ripple(int pos)
{
	Ripple* ripple = (Ripple*)scene->ripples->recycle("Ripple");
	if (ripple == NULL)
	{
		ripple = new Ripple();
		scene->ripples->add(ripple);
	}
	ripple->reset(pos);
	return ripple;
}