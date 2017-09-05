#pragma once

#include "charsprite.h"

class MobSprite :public CharSprite{
private:
	static const float FADE_TIME;
	static const float FALL_TIME;

public:
	virtual void update();
	virtual void onComplete(Animation* anim);
};