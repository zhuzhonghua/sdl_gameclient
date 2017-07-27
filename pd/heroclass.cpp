#include "heroclass.h"

#include "define.h"

const char* HeroClass::WARRIOR = "warrior";
const char* HeroClass::MAGE = "mage";
const char* HeroClass::ROGUE = "rogue";
const char* HeroClass::HUNTRESS = "huntress";

const char* HeroClass::WAR_PERKS[] = {
	"Warriors start with 11 points of Strength.",
	"Warriors start with a unique short sword. This sword can be later \"reforged\" to upgrade another melee weapon.",
	"Warriors are less proficient with missile weapons.",
	"Any piece of food restores some health when eaten.",
	"Potions of Strength are identified from the beginning.",
};

const char* HeroClass::MAG_PERKS[] = {
	"Mages start with a unique Wand of Magic Missile. This wand can be later \"disenchanted\" to upgrade another wand.",
	"Mages recharge their wands faster.",
	"When eaten, any piece of food restores 1 charge for all wands in the inventory.",
	"Mages can use wands as a melee weapon.",
	"Scrolls of Identify are identified from the beginning."
};

const char* HeroClass::ROG_PERKS[] = {
	"Rogues start with a Ring of Shadows+1.",
	"Rogues identify a type of a ring on equipping it.",
	"Rogues are proficient with light armor, dodging better while wearing one.",
	"Rogues are proficient in detecting hidden doors and traps.",
	"Rogues can go without food longer.",
	"Scrolls of Magic Mapping are identified from the beginning."
};

const char* HeroClass::HUN_PERKS[] = {
	"Huntresses start with 15 points of Health.",
	"Huntresses start with a unique upgradeable boomerang.",
	"Huntresses are proficient with missile weapons and get a damage bonus for excessive strength when using them.",
	"Huntresses gain more health from dewdrops.",
	"Huntresses sense neighbouring monsters even if they are hidden behind obstacles."
};

const char* HeroClass::CLASS = "class";

HeroClass::HeroClass(const std::string& title)
{
	this->_title = title;

	_type = Type::E_WARRIOR;

	if (_title.compare(HeroClass::WARRIOR) == 0)
	{
		_type = Type::E_WARRIOR;
	}
	else if (_title.compare(HeroClass::MAGE) == 0)
	{
		_type = Type::E_MAGE;
	}
	else if (_title.compare(HeroClass::ROGUE) == 0)
	{
		_type = Type::E_ROGUE;
	}
	else if (_title.compare(HeroClass::HUNTRESS) == 0)
	{
		_type = Type::E_HUNTRESS;
	}	
}

HeroClass::HeroClass(const HeroClass& cl)
{
	this->_title = cl._title;
	this->_type = cl._type;
}

std::string HeroClass::spritesheet()
{
	if (_title.compare(HeroClass::WARRIOR) == 0)
	{
		return Assets::WARRIOR;
	}
	else if (_title.compare(HeroClass::MAGE) == 0)
	{
		return Assets::MAGE;
	}
	else if (_title.compare(HeroClass::ROGUE) == 0)
	{
		return Assets::ROGUE;
	}
	else if (_title.compare(HeroClass::HUNTRESS) == 0)
	{
		return Assets::HUNTRESS;
	}

	return "";
}

void HeroClass::perks(std::vector<std::string>& out)
{
	if (_title.compare(HeroClass::WARRIOR) == 0)
	{
		out.assign(HeroClass::WAR_PERKS, HeroClass::WAR_PERKS + sizeof(HeroClass::WAR_PERKS)/sizeof(char*));
	}
	else if (_title.compare(HeroClass::MAGE) == 0)
	{
		out.assign(HeroClass::MAG_PERKS, HeroClass::MAG_PERKS + sizeof(HeroClass::MAG_PERKS) / sizeof(char*));
	}
	else if (_title.compare(HeroClass::ROGUE) == 0)
	{
		out.assign(HeroClass::ROG_PERKS, HeroClass::ROG_PERKS + sizeof(HeroClass::ROG_PERKS) / sizeof(char*));
	}
	else if (_title.compare(HeroClass::HUNTRESS) == 0)
	{
		out.assign(HeroClass::HUN_PERKS, HeroClass::HUN_PERKS + sizeof(HeroClass::HUN_PERKS) / sizeof(char*));
	}
}