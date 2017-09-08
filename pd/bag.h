#pragma once

#include "item.h"

#include <list>

class Char;

class Bag:public Item{
public:
	static const std::string AC_OPEN;
	Char* owner;
	std::list<Item*> items;
	int size;

	
	Bag();
	virtual bool grab(Item* item) { return false; }

	virtual std::string getClassName() { return "Bag"; }
	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);
private:
	static const std::string ITEMS;
};