#pragma once

#include "charsprite.h"
#include "pixelparticle.h"

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

class GooSprite :public MobSprite{
public:
	class GooParticle :public Shrinking{
	public:
		static Emitter::Factory* FACTORY;

		GooParticle();

		void reset(float x, float y);
		virtual void update();
	};
public:
	GooSprite();

	void pumpUp() { MobSprite::play(pump); }
	virtual void play(Animation* anim, bool force);
	virtual int blood() { return 0xFF000000; }
private:
	Animation* pump;
	Animation* jump;

	Emitter* spray;
};

class ShamanSprite :public MobSprite{
public:
	ShamanSprite();
	void Zap(int pos);
private:
	int points[2];
};

class BatSprite :public MobSprite{
public:
	BatSprite();
};

class BruteSprite :public MobSprite{
public:
	BruteSprite();
};

class TenguSprite :public MobSprite{
public:
	TenguSprite();

	virtual void move(int from, int to);
	virtual void Attack(int cell);
	virtual void onComplete(Animation* anim);
private:
	Animation* cast;
};

class SpinnerSprite :public MobSprite{
public:
	SpinnerSprite();

	virtual int blood() { return 0xFFBFE5B8; }
};

class ElementalSprite :public MobSprite{
public:
	ElementalSprite();

	virtual int blood() { return 0xFFFF7D13; }
	virtual void link(Char* ch)
	{
		MobSprite::link(ch);
		add(State::BURNING);
	}
	virtual void Die()
	{
		MobSprite::Die();
	}
};

class MonkSprite :public MobSprite{
private:
	Animation* kick;
public:
	MonkSprite();

	virtual void Attack(int cell);
	virtual void onComplete(Animation* anim);
};

class DM300Sprite :public MobSprite{
public:
	DM300Sprite();

	virtual void onComplete(Animation* anim);
	virtual int blood() { return 0xFFFFFF88; }
};