#include "dungeon.h"
#include "util.h"
#include "hero.h"
#include "level.h"

#include <sstream>

Hero* Dungeon::hero = NULL;
Level* Dungeon::level;

int Dungeon::depth = 0;
int Dungeon::gold = 0;

std::vector<bool> Dungeon::visible(Level::LENGTH);

std::set<int> Dungeon::chapters;

Bundle* Dungeon::gameBundle(const std::string& fileName)
{
	std::stringstream ss;
	if (IOManager::readFileToBuffer(fileName, ss, true))
	{
		return Bundle::read(ss);
	}
	else
	{
		return NULL;
	}
}

std::string Dungeon::gameFile(const HeroClass& cl)
{
	switch (cl.type()) {
	case HeroClass::E_WARRIOR:
		return WR_GAME_FILE;
	case HeroClass::E_MAGE:
		return MG_GAME_FILE;
	case HeroClass::E_HUNTRESS:
		return RN_GAME_FILE;
	default:
		return RG_GAME_FILE;
	}
}

void Dungeon::preview(GamesInProgress::Info& info, Bundle* bundle)
{
	info.depth = bundle->getInt(DEPTH);
	info.challenges = (bundle->getInt(CHALLENGES) != 0);
	if (info.depth == -1) 
	{
		info.depth = bundle->getInt("maxDepth");	// FIXME
	}
	Bundle* tmp = bundle->getBundle(HERO);
	Hero::preview(info, tmp);
	delete tmp;
}

const std::string Dungeon::RG_GAME_FILE = "game.dat";
const std::string Dungeon::RG_DEPTH_FILE = "depth%d.dat";

const std::string Dungeon::WR_GAME_FILE = "warrior.dat";
const std::string Dungeon::WR_DEPTH_FILE = "warrior%d.dat";

const std::string Dungeon::MG_GAME_FILE = "mage.dat";
const std::string Dungeon::MG_DEPTH_FILE = "mage%d.dat";

const std::string Dungeon::RN_GAME_FILE = "ranger.dat";
const std::string Dungeon::RN_DEPTH_FILE = "ranger%d.dat";

const std::string Dungeon::VERSION = "version";
const std::string Dungeon::CHALLENGES = "challenges";
const std::string Dungeon::HERO = "hero";
const std::string Dungeon::GOLD = "gold";
const std::string Dungeon::DEPTH = "depth";
const std::string Dungeon::LEVEL = "level";
const std::string Dungeon::DROPPED = "dropped%d";
const std::string Dungeon::POS = "potionsOfStrength";
const std::string Dungeon::SOU = "scrollsOfEnhancement";
const std::string Dungeon::SOE = "scrollsOfEnchantment";
const std::string Dungeon::DV = "dewVial";
const std::string Dungeon::CHAPTERS = "chapters";
const std::string Dungeon::QUESTS = "quests";
const std::string Dungeon::BADGES = "badges";