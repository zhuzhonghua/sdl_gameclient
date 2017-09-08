#include "item.h"

#include "util.h"
#include "bpt.h"

const char* Item::TXT_PACK_FULL = "lang.item_packfull";

const char* Item::TXT_BROKEN = "lang.item_broken";
const char* Item::TXT_GONNA_BREAK = "lang.item_goingbroken";

const char* Item::TXT_TO_STRING = "%s";
const char* Item::TXT_TO_STRING_X = "%s x%d";
const char* Item::TXT_TO_STRING_LVL = "%s%+d";
const char* Item::TXT_TO_STRING_LVL_X = "%s%+d x%d";

const float Item::DURABILITY_WARNING_LEVEL = 1 / 6.0f;

const float Item::TIME_TO_THROW = 1.0f;
const float Item::TIME_TO_PICK_UP = 1.0f;
const float Item::TIME_TO_DROP = 0.5f;

const std::string Item::AC_DROP = "lang.item_drop";
const std::string Item::AC_THROW = "lang.item_throw";

void Item::actions(Hero* hero, std::vector<std::string>& actions)
{
	actions.push_back(BPT::getText(AC_DROP));
	actions.push_back(BPT::getText(AC_THROW));
}

bool Item::doPickUp(Hero* hero)
{
	//if (collect(hero.belongings.backpack)) 
	//{
	//
	//	GameScene.pickUp(this);
	//	//Sample.INSTANCE.play(Assets.SND_ITEM);
	//	hero.spendAndNext(TIME_TO_PICK_UP);
	//	return true;
	//}
	//else 
	{
		return false;
	}
}

void Item::doDrop(Hero* hero)
{
	//hero.spendAndNext(TIME_TO_DROP);
	//Dungeon.level.drop(detachAll(hero.belongings.backpack), hero.pos).sprite.drop(hero.pos);
}

std::string Item::toString()
{
	if (levelKnown && level != 0) 
	{
		if (quantity > 1) 
		{
			return GameMath::format(TXT_TO_STRING_LVL_X, Name(), level, quantity);
		}
		else 
		{
			return GameMath::format(TXT_TO_STRING_LVL, Name(), level);
		}
	}
	else 
	{
		if (quantity > 1) 
		{
			return GameMath::format(TXT_TO_STRING_X, Name(), quantity);
		}
		else 
		{
			return GameMath::format(TXT_TO_STRING, Name());
		}
	}
}

std::string Item::Name()
{
	return name;
}

int Item::Image()
{
	return image;
}

void Item::storeInBundle(Bundle* bundle)
{
	//bundle.put(QUANTITY, quantity);
	//bundle.put(LEVEL, level);
	//bundle.put(LEVEL_KNOWN, levelKnown);
	//bundle.put(CURSED, cursed);
	//bundle.put(CURSED_KNOWN, cursedKnown);
	//if (isUpgradable()) {
	//	bundle.put(DURABILITY, durability);
	//}
	//QuickSlot.save(bundle, this);
}

void Item::restoreFromBundle(Bundle* bundle)
{
	//quantity = bundle.getInt(QUANTITY);
	//levelKnown = bundle.getBoolean(LEVEL_KNOWN);
	//cursedKnown = bundle.getBoolean(CURSED_KNOWN);
	//
	//int level = bundle.getInt(LEVEL);
	//if (level > 0) {
	//	upgrade(level);
	//}
	//else if (level < 0) {
	//	degrade(-level);
	//}
	//
	//cursed = bundle.getBoolean(CURSED);
	//
	//if (isUpgradable()) {
	//	durability = bundle.getInt(DURABILITY);
	//}
	//
	//QuickSlot.restore(bundle, this);
}
