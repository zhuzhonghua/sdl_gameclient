#pragma once

#include "gamesinprogress.h"
#include "bundle.h"
#include "char.h"
#include "heroclass.h"

#include <string>

class Belongings;
class HeroAction;
class Mob;

class Hero:public Char{
private:
	static const std::string TXT_LEAVE;

	static const std::string TXT_LEVEL_UP;
	static const std::string TXT_NEW_LEVEL;

	static const std::string TXT_SOMETHING_ELSE;
	static const std::string TXT_LOCKED_CHEST;
	static const std::string TXT_LOCKED_DOOR;
	static const std::string TXT_NOTICED_SMTH;

	static const std::string TXT_WAIT;
	static const std::string TXT_SEARCH;
	static const float TIME_TO_REST;
	static const float TIME_TO_SEARCH;

private:
	static const std::string ATTACK;
	static const std::string DEFENSE;
	static const std::string STRENGTH;
	static const std::string LEVEL;
	static const std::string EXPERIENCE;

	int attackSkill;
	int defenseSkill;

	std::list<Mob*> visibleEnemies;

	void Ready();
public:
	static const std::string TXT_YOU_NOW_HAVE;
	static const int STARTING_STR = 10;
public:
	static void preview(GamesInProgress::Info& info, Bundle* bundle);
	int tier();
	void live();
	void updateAwareness();

	Hero();

	HeroClass heroClass;
	HeroSubClass subClass;

	Belongings* belongings;

	int STR;

	int lvl;
	int exp;

	bool restoreHealth;
	bool ready;
	HeroAction* curAction;
	HeroAction* lastAction;
	float awareness;
public:
	void resurrect(int resetLevel);
	virtual bool act();
	bool handle(int cell);

	virtual std::string getClassName() { return "Hero"; }
	int VisibleEnemies();
	Mob* visibleEnemy(int index);

	void resume();
	int maxExp() { return 5 + lvl * 5; }
};