#pragma once

#include <string>
#include <vector>

class HeroClass{
public:
	enum Type{
		E_WARRIOR,
		E_MAGE,
		E_ROGUE,
		E_HUNTRESS
	};
	static const char* WARRIOR;
	static const char* MAGE;
	static const char* ROGUE;
	static const char* HUNTRESS;
private:
	static const char* WAR_PERKS[];
	static const char* MAG_PERKS[];
	static const char* ROG_PERKS[];
	static const char* HUN_PERKS[];

	static const char* CLASS;

	std::string _title;
	Type _type;

	static void initCommon(/*Hero hero*/);
	static void initWarrior(/*Hero hero*/);
	static void initMage(/*Hero hero*/);
	static void initRogue(/*Hero hero*/);
	static void initHuntress(/*Hero hero*/);
public:
	HeroClass(const std::string& title);
	HeroClass(const HeroClass& cl);

	void initHero(/*Hero hero*/);
	std::string title() { return _title; }
	std::string name() { return _title; }
	std::string spritesheet();
	void perks(std::vector<std::string>& out);
	void storeInBundle(/*Bundle bundle*/);
	static HeroClass restoreInBundle(/*Bundle bundle*/);
	int ordinal() { return _type; }

	bool operator < (const HeroClass &c) const
	{
		return _type < c._type;
	}
};