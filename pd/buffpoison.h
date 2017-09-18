#pragma once

#include "buff.h"
#include "hero.h"

class BuffPoison :public Buff, public Hero::Doom{
public:
	virtual std::string getClassName() { return "BuffPoison"; }

	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);

	void set(float duration) { left = duration; };
	virtual int icon();
	virtual std::string toString() { return "Poisoned"; }
	virtual bool act();
	static float durationFactor(Char* ch);

	virtual void onDeath();
protected:
	float left;

private:
	static const std::string LEFT;
};