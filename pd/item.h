#pragma once

#include "bundlable.h"
#include "hero.h"
#include "itemsprite.h"

#include <string>
#include <vector>

class ItemSprite;
class ItemSprite::Glowing;

class Item :public Bundlable{
private:
	static const char* TXT_PACK_FULL;

	static const char* TXT_BROKEN;
	static const char* TXT_GONNA_BREAK;

	static const char* TXT_TO_STRING;
	static const char* TXT_TO_STRING_X;
	static const char* TXT_TO_STRING_LVL;
	static const char* TXT_TO_STRING_LVL_X;

	static const float DURABILITY_WARNING_LEVEL;

protected:
	static const float TIME_TO_THROW;
	static const float TIME_TO_PICK_UP;
	static const float TIME_TO_DROP;

	static const std::string AC_DROP;
	static const std::string AC_THROW;

public:
	std::string defaultAction;

	std::string name = "smth";
	int image = 0;


public:
	bool stackable = false;
	bool levelKnown = false;
	bool cursed;
	bool cursedKnown;

	bool unique = false;

protected:
	int quantity = 1;

private:
	int level = 0;
	int durability;// = maxDurability();

public:
	void actions(Hero* hero, std::vector<std::string>& actions);
	bool doPickUp(Hero* hero);
	void doDrop(Hero* hero);

	std::string toString();
	std::string Name();
	int Image();

	virtual ItemSprite::Glowing* glowing() { return NULL; }

	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);
};