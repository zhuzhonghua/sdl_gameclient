#include "item.h"

#include "util.h"

const char* Item::TXT_PACK_FULL = "Your pack is too full for the %s";

const char* Item::TXT_BROKEN = "Because of frequent use, your %s has broken.";
const char* Item::TXT_GONNA_BREAK = "Because of frequent use, your %s is going to break soon.";

const char* Item::TXT_TO_STRING = "%s";
const char* Item::TXT_TO_STRING_X = "%s x%d";
const char* Item::TXT_TO_STRING_LVL = "%s%+d";
const char* Item::TXT_TO_STRING_LVL_X = "%s%+d x%d";

const float Item::DURABILITY_WARNING_LEVEL = 1 / 6.0f;

const float Item::TIME_TO_THROW = 1.0f;
const float Item::TIME_TO_PICK_UP = 1.0f;
const float Item::TIME_TO_DROP = 0.5f;

const std::string Item::AC_DROP = "DROP";
const std::string Item::AC_THROW = "THROW";

void Item::actions(Hero* hero, std::vector<std::string>& actions)
{
	actions.push_back(AC_DROP);
	actions.push_back(AC_THROW);
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