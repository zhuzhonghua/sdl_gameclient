#pragma once

#include "item.h"

class Food :public Item{
private:
	static const float TIME_TO_EAT;

public:
	static const std::string AC_EAT;

	float energy;
	std::string message;

	Food();

	virtual void actions(Hero* hero, std::vector<std::string>& actions);
	virtual void execute(Hero* hero, const std::string& action);

	virtual std::string info() 
	{
		return
			std::string("Nothing fancy here: dried meat, ") +
			std::string("some biscuits - things like that.");
	}
	virtual bool isUpgradable() { return false; }
	virtual bool isIdentified() { return true; }
	virtual int price() { return 10 * quantity; }
};