#include "hero.h"

#include "dungeon.h"

const std::string Hero::ATTACK = "attackSkill";
const std::string Hero::DEFENSE = "defenseSkill";
const std::string Hero::STRENGTH = "STR";
const std::string Hero::LEVEL = "lvl";
const std::string Hero::EXPERIENCE = "exp";

const std::string Hero::TXT_LEAVE = "One does not simply leave Pixel Dungeon.";

const std::string Hero::TXT_LEVEL_UP = "level up!";
const std::string Hero::TXT_NEW_LEVEL =
std::string("Welcome to level %d! Now you are healthier and more focused. ") +
std::string("It's easier for you to hit enemies and dodge their attacks.");

const std::string Hero::TXT_SOMETHING_ELSE = "There is something else here";
const std::string Hero::TXT_LOCKED_CHEST = "This chest is locked and you don't have matching key";
const std::string Hero::TXT_LOCKED_DOOR = "You don't have a matching key";
const std::string Hero::TXT_NOTICED_SMTH = "You noticed something";

const std::string Hero::TXT_WAIT = "...";
const std::string Hero::TXT_SEARCH = "search";
const float Hero::TIME_TO_REST = 1.0f;
const float Hero::TIME_TO_SEARCH = 2.0f;

const std::string Hero::TXT_YOU_NOW_HAVE = "You now have %s";

void Hero::preview(GamesInProgress::Info& info, Bundle* bundle)
{
	info.level = bundle->getInt(LEVEL);
}

int Hero::tier()
{
	return 0;//belongings.armor == null ? 0 : belongings.armor.tier;
}

void Hero::live()
{
	//Buff.affect(this, Regeneration.class);
	//Buff.affect(this, Hunger.class);
}

void Hero::updateAwareness()
{
	awareness = (float)(1 - std::pow(
		(heroClass.type() == HeroClass::E_ROGUE ? 0.85 : 0.90),
		(1 + std::min(lvl, 9)) * 0.5
		));
}

Hero::Hero()
:heroClass(HeroClass::ROGUE)
, subClass(HeroSubClass::NONE)
{
	lvl = 1;
	exp = 0;

	attackSkill = 10;
	defenseSkill = 5;

	restoreHealth = false;
	ready = false;

	curAction = NULL;
	lastAction = NULL;
}

void Hero::resurrect(int resetLevel)
{
	HP = HT;
	Dungeon::gold = 0;
	exp = 0;

	//belongings.resurrect(resetLevel);

	//live();
}

int Hero::visibleEnemies()
{
	return 0;
	//return visibleEnemies.size();
}

void Hero::resume()
{
	//curAction = lastAction;
	//lastAction = null;
	//act();
}
