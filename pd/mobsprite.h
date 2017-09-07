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

class RatSprite :public MobSprite{
public:
	RatSprite();
};

class GnollSprite :public MobSprite{
public:
	GnollSprite();
};

class CrabSprite :public MobSprite{
public:
	CrabSprite();

	virtual int blood() { return 0xFFFFEA80; }
};

class SwarmSprite :public MobSprite{
public:
	SwarmSprite();

	virtual int blood() { return 0xFF8BA077; }
};

class SkeletonSprite :public MobSprite{
public:
	SkeletonSprite();

	virtual void Die();
	virtual int blood() { return 0xFFcccccc; }
};

class ThiefSprite :public MobSprite{
public:
	ThiefSprite();
};