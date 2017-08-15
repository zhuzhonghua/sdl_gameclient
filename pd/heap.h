#pragma once

#include "bundlable.h"
#include "itemsprite.h"

class Item;

class Heap :public Bundlable{
public:
	enum Type {
		HEAP,
		FOR_SALE,
		CHEST,
		LOCKED_CHEST,
		CRYSTAL_CHEST,
		TOMB,
		SKELETON,
		MIMIC,
		HIDDEN
	};

	Type type;
	int pos;
	std::vector<Item* > items;

	Heap();

	ItemSprite::Glowing* glowing();
	int image();
	int size() { return items.size(); }

	bool isEmpty() { return items.size() == 0; }
	Item* peek() { return items.size() > 0 ? items[0] : NULL; }
};