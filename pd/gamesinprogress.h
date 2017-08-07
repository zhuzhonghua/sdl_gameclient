#pragma once

#include <map>
#include "heroclass.h"

class GamesInProgress{
public:
	struct Info {
		int depth;
		int level;
		bool challenges;
	};

	static bool check(HeroClass cl, Info& info);
private:
	static std::map<HeroClass, Info> state;
};