#pragma once

#include "gamesinprogress.h"
#include "bundle.h"

#include <string>

class Hero{
private:
	static const std::string ATTACK;
	static const std::string DEFENSE;
	static const std::string STRENGTH;
	static const std::string LEVEL;
	static const std::string EXPERIENCE;
public:
	static void preview(GamesInProgress::Info& info, Bundle* bundle);

	Hero();

	int STR;

	int lvl;
	int exp;
};