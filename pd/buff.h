#pragma once

#include "actor.h"

class Buff :public Actor{
public:
	Char* target;

	bool attachTo(Char* target);
	void detach();
	virtual std::string getClassName(){ return "Buff"; }

	virtual bool act() {
		//diactivate();
		return true;
	}
};