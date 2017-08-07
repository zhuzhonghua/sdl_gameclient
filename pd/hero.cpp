#include "hero.h"

const std::string Hero::ATTACK = "attackSkill";
const std::string Hero::DEFENSE = "defenseSkill";
const std::string Hero::STRENGTH = "STR";
const std::string Hero::LEVEL = "lvl";
const std::string Hero::EXPERIENCE = "exp";

void Hero::preview(GamesInProgress::Info& info, Bundle* bundle)
{
	info.level = bundle->getInt(LEVEL);
}