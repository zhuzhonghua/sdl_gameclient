#pragma once

#include "actor.h"

class FlavourBuff;

class Buff :public Actor{
public:
	Buff();
	Char* target;

	virtual bool attachTo(Char* target);
	void detach();
	virtual std::string getClassName(){ return "Buff"; }

	static std::string className() { return "Buff"; }

	virtual bool act() {
		diactivate();
		return true;
	}

	virtual int icon();

	static Buff* append(Char* target, Buff* buff);
	static FlavourBuff* append(Char* target, FlavourBuff* buffClass, float duration);
	static Buff* affect(Char* target, Buff* buffClass);
	static FlavourBuff* affect(Char* target, FlavourBuff* buffClass, float duration);
	static FlavourBuff* prolong(Char* target, FlavourBuff* buffClass, float duration);

	static void detach(Buff* buff);
	static void detach(Char* target, Buff* cl);
};

class FlavourBuff :public Buff{
public:
	virtual bool act() { detach(); return true; }
	virtual std::string getClassName(){ return "FlavourBuff"; }
};