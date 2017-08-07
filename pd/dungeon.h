#pragma once

#include <map>
#include "bundle.h"
#include "heroclass.h"
#include "gamesinprogress.h"

class Dungeon{
public:
	static Bundle* gameBundle(const std::string& fileName);
	static std::string gameFile(const HeroClass& cl);
	static void preview(GamesInProgress::Info& info, Bundle* bundle);
private:
	static const std::string RG_GAME_FILE ;
	static const std::string RG_DEPTH_FILE ;
	static const std::string WR_GAME_FILE ;
	static const std::string WR_DEPTH_FILE ;
	static const std::string MG_GAME_FILE ;
	static const std::string MG_DEPTH_FILE ;
	static const std::string RN_GAME_FILE ;
	static const std::string RN_DEPTH_FILE ;
	static const std::string VERSION ;
	static const std::string CHALLENGES ;
	static const std::string HERO ;
	static const std::string GOLD ;
	static const std::string DEPTH ;
	static const std::string LEVEL ;
	static const std::string DROPPED ;
	static const std::string POS ;
	static const std::string SOU ;
	static const std::string SOE ;
	static const std::string DV ;
	static const std::string CHAPTERS ;
	static const std::string QUESTS ;
	static const std::string BADGES ;
};