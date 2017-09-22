#pragma once

#include "item.h"
#include "typedefine.h"

class Food :public Item{
private:
	static const float TIME_TO_EAT;

public:
	static const std::string AC_EAT;

	float energy;
	std::string message;

	Food();
	virtual std::string getClassName() { return "Food"; }
	static Item* Create() { return new Food(); }

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

class MysteryMeat :public Food{
public:
	MysteryMeat();
	virtual std::string getClassName() { return "MysteryMeat"; }
	static Item* Create() { return new MysteryMeat(); }

	virtual void execute(Hero* hero, const std::string& action);

	virtual String info() {
		return "Eat at your own risk!";
	}

	virtual int price() {
		return 5 * quantity;
	};

};

class ChargrilledMeat :public Food{
public:
	ChargrilledMeat();
	virtual std::string getClassName() { return "ChargrilledMeat"; }
	static Item* Create() { return new ChargrilledMeat(); }

	virtual String info() {
		return "It looks like a decent steak.";
	}

	virtual int price() {
		return 5 * quantity;
	}

	static Food* cook(MysteryMeat* ingredient);

};

class FrozenCarpaccio :public Food{
public:
	FrozenCarpaccio();

	virtual std::string getClassName() { return "FrozenCarpaccio"; }
	static Item* Create() { return new FrozenCarpaccio(); }

	virtual void execute(Hero* hero, const std::string& action);

	virtual String info() {
		return
			std::string("It's a piece of frozen raw meat. The only way to eat it is ") +
			std::string("by cutting thin slices of it. And this way it's suprisingly good.");
	}
	virtual int price() {
		return 10 * quantity;
	}
	static Food* cook(MysteryMeat* ingredient);
};

class Pasty :public Food{
public:
	Pasty();
	virtual std::string getClassName() { return "Pasty"; }
	static Item* Create() { return new Pasty(); }

	virtual String info() {
		return "This is authentic Cornish pasty with traditional filling of beef and potato.";
	}
	virtual int price() {
		return 20 * quantity;
	}
};