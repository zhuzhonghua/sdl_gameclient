#pragma once

#include "pixelscene.h"
#include "emitter.h"
#include "ripple.h"

class GameScene :public PixelScene{
private:
	static GameScene* scene;

	Group* terrain;
	Group* ripples;
	Group* plants;
	Group* heaps;
	Group* mobs;
	Group* emitters;
	Group* effects;
	Group* gases;
	Group* spells;
	Group* statuses;
	Group* emoicons;

public:
	void brightness(bool value);

	virtual void init();

	static Emitter* emitter();

	static Ripple* ripple(int pos);
};