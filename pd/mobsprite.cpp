#include "mobsprite.h"
#include "mob.h"

const float MobSprite::FADE_TIME = 3.0f;
const float MobSprite::FALL_TIME = 1.0f;

void MobSprite::update()
{
	sleeping = ch != NULL && ((Mob*)ch)->state == ((Mob*)ch)->SLEEPEING;
	CharSprite::update();
}
