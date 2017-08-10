#include "badge.h"
#include "util.h"
#include "bundle.h"
#include "statistics.h"
#include "dungeon.h"

#include <sstream>

std::map<std::string, Badges::Badge*> Badges::Badge::mapBadge;

const Badges::Badge Badges::MONSTERS_SLAIN_1("MONSTERS_SLAIN_1","10 enemies slain", 0);
const Badges::Badge Badges::MONSTERS_SLAIN_2("MONSTERS_SLAIN_2","50 enemies slain", 1);
const Badges::Badge Badges::MONSTERS_SLAIN_3("MONSTERS_SLAIN_3","150 enemies slain", 2);
const Badges::Badge Badges::MONSTERS_SLAIN_4("MONSTERS_SLAIN_4","250 enemies slain", 3);
const Badges::Badge Badges::GOLD_COLLECTED_1("GOLD_COLLECTED_1","100 gold collected", 4);
const Badges::Badge Badges::GOLD_COLLECTED_2("GOLD_COLLECTED_2","500 gold collected", 5);
const Badges::Badge Badges::GOLD_COLLECTED_3("GOLD_COLLECTED_3","2500 gold collected", 6);
const Badges::Badge Badges::GOLD_COLLECTED_4("GOLD_COLLECTED_4","7500 gold collected", 7);
const Badges::Badge Badges::LEVEL_REACHED_1("LEVEL_REACHED_1","Level 6 reached", 8);
const Badges::Badge Badges::LEVEL_REACHED_2("LEVEL_REACHED_2","Level 12 reached", 9);
const Badges::Badge Badges::LEVEL_REACHED_3("LEVEL_REACHED_3","Level 18 reached", 10);
const Badges::Badge Badges::LEVEL_REACHED_4("LEVEL_REACHED_4","Level 24 reached", 11);
const Badges::Badge Badges::ALL_POTIONS_IDENTIFIED("ALL_POTIONS_IDENTIFIED","All potions identified", 16);
const Badges::Badge Badges::ALL_SCROLLS_IDENTIFIED("ALL_SCROLLS_IDENTIFIED","All scrolls identified", 17);
const Badges::Badge Badges::ALL_RINGS_IDENTIFIED("ALL_RINGS_IDENTIFIED","All rings identified", 18);
const Badges::Badge Badges::ALL_WANDS_IDENTIFIED("ALL_WANDS_IDENTIFIED","All wands identified", 19);
const Badges::Badge Badges::ALL_ITEMS_IDENTIFIED("ALL_ITEMS_IDENTIFIED","All potions, scrolls, rings & wands identified", 35, true);
const Badges::Badge Badges::BAG_BOUGHT_SEED_POUCH("BAG_BOUGHT_SEED_POUCH");
const Badges::Badge Badges::BAG_BOUGHT_SCROLL_HOLDER("BAG_BOUGHT_SCROLL_HOLDER");
const Badges::Badge Badges::BAG_BOUGHT_WAND_HOLSTER("BAG_BOUGHT_WAND_HOLSTER");
const Badges::Badge Badges::ALL_BAGS_BOUGHT("ALL_BAGS_BOUGHT","All bags bought", 23);
const Badges::Badge Badges::DEATH_FROM_FIRE("DEATH_FROM_FIRE","Death from fire", 24);
const Badges::Badge Badges::DEATH_FROM_POISON("DEATH_FROM_POISON","Death from poison", 25);
const Badges::Badge Badges::DEATH_FROM_GAS("DEATH_FROM_GAS","Death from toxic gas", 26);
const Badges::Badge Badges::DEATH_FROM_HUNGER("DEATH_FROM_HUNGER","Death from hunger", 27);
const Badges::Badge Badges::DEATH_FROM_GLYPH("DEATH_FROM_GLYPH","Death from an enchantment", 57);
const Badges::Badge Badges::DEATH_FROM_FALLING("DEATH_FROM_FALLING","Death from falling down", 59);
const Badges::Badge Badges::YASD("YASD","Death from fire, poison, toxic gas & hunger", 34, true);
const Badges::Badge Badges::BOSS_SLAIN_1_WARRIOR("BOSS_SLAIN_1_WARRIOR");
const Badges::Badge Badges::BOSS_SLAIN_1_MAGE("BOSS_SLAIN_1_MAGE");
const Badges::Badge Badges::BOSS_SLAIN_1_ROGUE("BOSS_SLAIN_1_ROGUE");
const Badges::Badge Badges::BOSS_SLAIN_1_HUNTRESS("BOSS_SLAIN_1_HUNTRESS");
const Badges::Badge Badges::BOSS_SLAIN_1("BOSS_SLAIN_1","1st boss slain", 12);
const Badges::Badge Badges::BOSS_SLAIN_2("BOSS_SLAIN_2","2nd boss slain", 13);
const Badges::Badge Badges::BOSS_SLAIN_3("BOSS_SLAIN_3","3rd boss slain", 14);
const Badges::Badge Badges::BOSS_SLAIN_4("BOSS_SLAIN_4","4th boss slain", 15);
const Badges::Badge Badges::BOSS_SLAIN_1_ALL_CLASSES("BOSS_SLAIN_1_ALL_CLASSES","1st boss slain by Warrior, Mage, Rogue & Huntress", 32, true);
const Badges::Badge Badges::BOSS_SLAIN_3_GLADIATOR("BOSS_SLAIN_3_GLADIATOR");
const Badges::Badge Badges::BOSS_SLAIN_3_BERSERKER("BOSS_SLAIN_3_BERSERKER");
const Badges::Badge Badges::BOSS_SLAIN_3_WARLOCK("BOSS_SLAIN_3_WARLOCK");
const Badges::Badge Badges::BOSS_SLAIN_3_BATTLEMAGE("BOSS_SLAIN_3_BATTLEMAGE");
const Badges::Badge Badges::BOSS_SLAIN_3_FREERUNNER("BOSS_SLAIN_3_FREERUNNER");
const Badges::Badge Badges::BOSS_SLAIN_3_ASSASSIN("BOSS_SLAIN_3_ASSASSIN");
const Badges::Badge Badges::BOSS_SLAIN_3_SNIPER("BOSS_SLAIN_3_SNIPER");
const Badges::Badge Badges::BOSS_SLAIN_3_WARDEN("BOSS_SLAIN_3_WARDEN");
const Badges::Badge Badges::BOSS_SLAIN_3_ALL_SUBCLASSES("BOSS_SLAIN_3_ALL_SUBCLASSES","3rd boss slain by Gladiator, Berserker, Warlock, Battlemage, Freerunner, Assassin, Sniper & Warden", 33, true);
const Badges::Badge Badges::RING_OF_HAGGLER("RING_OF_HAGGLER","Ring of Haggler obtained", 20);
const Badges::Badge Badges::RING_OF_THORNS("RING_OF_THORNS","Ring of Thorns obtained", 21);
const Badges::Badge Badges::STRENGTH_ATTAINED_1("STRENGTH_ATTAINED_1","13 points of Strength attained", 40);
const Badges::Badge Badges::STRENGTH_ATTAINED_2("STRENGTH_ATTAINED_2","15 points of Strength attained", 41);
const Badges::Badge Badges::STRENGTH_ATTAINED_3("STRENGTH_ATTAINED_3","17 points of Strength attained", 42);
const Badges::Badge Badges::STRENGTH_ATTAINED_4("STRENGTH_ATTAINED_4","19 points of Strength attained", 43);
const Badges::Badge Badges::FOOD_EATEN_1("FOOD_EATEN_1","10 pieces of food eaten", 44);
const Badges::Badge Badges::FOOD_EATEN_2("FOOD_EATEN_2","20 pieces of food eaten", 45);
const Badges::Badge Badges::FOOD_EATEN_3("FOOD_EATEN_3","30 pieces of food eaten", 46);
const Badges::Badge Badges::FOOD_EATEN_4("FOOD_EATEN_4","40 pieces of food eaten", 47);
const Badges::Badge Badges::MASTERY_WARRIOR("MASTERY_WARRIOR");
const Badges::Badge Badges::MASTERY_MAGE("MASTERY_MAGE");
const Badges::Badge Badges::MASTERY_ROGUE("MASTERY_ROGUE");
const Badges::Badge Badges::MASTERY_HUNTRESS("MASTERY_HUNTRESS");
const Badges::Badge Badges::ITEM_LEVEL_1("ITEM_LEVEL_1","Item of level 3 acquired", 48);
const Badges::Badge Badges::ITEM_LEVEL_2("ITEM_LEVEL_2","Item of level 6 acquired", 49);
const Badges::Badge Badges::ITEM_LEVEL_3("ITEM_LEVEL_3","Item of level 9 acquired", 50);
const Badges::Badge Badges::ITEM_LEVEL_4("ITEM_LEVEL_4","Item of level 12 acquired", 51);
const Badges::Badge Badges::RARE_ALBINO("RARE_ALBINO");
const Badges::Badge Badges::RARE_BANDIT("RARE_BANDIT");
const Badges::Badge Badges::RARE_SHIELDED("RARE_SHIELDED");
const Badges::Badge Badges::RARE_SENIOR("RARE_SENIOR");
const Badges::Badge Badges::RARE_ACIDIC("RARE_ACIDIC");
const Badges::Badge Badges::RARE("RARE","All rare monsters slain", 37, true);
const Badges::Badge Badges::VICTORY_WARRIOR("VICTORY_WARRIOR");
const Badges::Badge Badges::VICTORY_MAGE("VICTORY_MAGE");
const Badges::Badge Badges::VICTORY_ROGUE("VICTORY_ROGUE");
const Badges::Badge Badges::VICTORY_HUNTRESS("VICTORY_HUNTRESS");
const Badges::Badge Badges::VICTORY("VICTORY","Amulet of Yendor obtained", 22);
const Badges::Badge Badges::VICTORY_ALL_CLASSES("VICTORY_ALL_CLASSES","Amulet of Yendor obtained by Warrior, Mage, Rogue & Huntress", 36, true);
const Badges::Badge Badges::MASTERY_COMBO("MASTERY_COMBO","7-hit combo", 56);
const Badges::Badge Badges::POTIONS_COOKED_1("POTIONS_COOKED_1","3 potions cooked", 52);
const Badges::Badge Badges::POTIONS_COOKED_2("POTIONS_COOKED_2","6 potions cooked", 53);
const Badges::Badge Badges::POTIONS_COOKED_3("POTIONS_COOKED_3","9 potions cooked", 54);
const Badges::Badge Badges::POTIONS_COOKED_4("POTIONS_COOKED_4","12 potions cooked", 55);
const Badges::Badge Badges::NO_MONSTERS_SLAIN("NO_MONSTERS_SLAIN","Level completed without killing any monsters", 28);
const Badges::Badge Badges::GRIM_WEAPON("GRIM_WEAPON","Monster killed by a Grim weapon", 29);
const Badges::Badge Badges::PIRANHAS("PIRANHAS","6 piranhas killed", 30);
const Badges::Badge Badges::NIGHT_HUNTER("NIGHT_HUNTER","15 monsters killed at nighttime", 58);
const Badges::Badge Badges::GAMES_PLAYED_1("GAMES_PLAYED_1","10 games played", 60, true);
const Badges::Badge Badges::GAMES_PLAYED_2("GAMES_PLAYED_2","100 games played", 61, true);
const Badges::Badge Badges::GAMES_PLAYED_3("GAMES_PLAYED_3","500 games played", 62, true);
const Badges::Badge Badges::GAMES_PLAYED_4("GAMES_PLAYED_4","2000 games played", 63, true);
const Badges::Badge Badges::HAPPY_END("HAPPY_END","Happy end", 38);
const Badges::Badge Badges::CHAMPION("CHAMPION","Challenge won", 39, true);
const Badges::Badge Badges::SUPPORTER("SUPPORTER","Thanks for your support!", 31, true);

std::set<Badges::Badge> Badges::global;
std::set<Badges::Badge> Badges::local;

bool Badges::saveNeeded = false;

Callback* Badges::loadingListener = NULL;

const std::string Badges::BADGES_FILE = "badges.dat";
const std::string Badges::BADGES = "badges";

void Badges::reset()
{
	local.clear();
	loadGlobal();
}

void Badges::restore(Bundle* bundle, std::set<Badges::Badge>& badges)
{
	std::list<std::string> names;
	bundle->getStringArray(BADGES, names);

	for (std::list<std::string>::iterator itr = names.begin();
		itr != names.end(); itr++)
	{
		badges.insert(*(Badge::mapBadge.find(*itr)->second));
	}
}

void Badges::store(Bundle* bundle, std::set<Badges::Badge>& badges)
{
	std::vector<std::string> names;
	names.resize(badges.size());
	
	int count = 0;
	for (std::set<Badges::Badge>::iterator itr = badges.begin();
		itr != badges.end(); itr++)
	{
		names[count++] = itr->name;
	}

	bundle->put(BADGES, names);
}

void Badges::displayBadge(const Badges::Badge* badge)
{

}

void Badges::loadGlobal()
{
	if (global.size() <= 0) 
	{
		std::stringstream ss;
		if (IOManager::readFileToBuffer(BADGES_FILE, ss, true))
		{
			Bundle* bund = Bundle::read(ss);
			restore(bund, global);
			delete bund;
		}
	}
}

void Badges::saveGlobal()
{
	Bundle bundle;
	if (saveNeeded)
	{
		store(&bundle, global);

		std::stringstream ss;
		Bundle::write(&bundle, ss);

		IOManager::writeFile(BADGES_FILE, ss.str());

		saveNeeded = false;
	}
}

void Badges::loadLocal(Bundle* bundle)
{
	restore(bundle, local);
}

void Badges::saveLocal(Bundle* bundle)
{
	store(bundle, local);
}

void Badges::validateMonstersSlain()
{
	const Badge* badge = NULL;

	if (local.find(Badges::MONSTERS_SLAIN_1)==local.end() && Statistics::enemiesSlain >= 10) 
	{
		badge = &Badges::MONSTERS_SLAIN_1;
		local.insert(*badge);
	}
	if (local.find(Badges::MONSTERS_SLAIN_2) == local.end() && Statistics::enemiesSlain >= 50) 
	{
		badge = &Badges::MONSTERS_SLAIN_2;
		local.insert(*badge);
	}
	if (local.find(Badges::MONSTERS_SLAIN_3) == local.end() && Statistics::enemiesSlain >= 150) 
	{
		badge = &Badges::MONSTERS_SLAIN_3;
		local.insert(*badge);
	}
	if (local.find(Badges::MONSTERS_SLAIN_4) == local.end() && Statistics::enemiesSlain >= 250) 
	{
		badge = &Badges::MONSTERS_SLAIN_4;
		local.insert(*badge);
	}

	displayBadge(badge);
}

void Badges::validateGoldCollected()
{
	const Badge* badge = NULL;

	if (local.find(Badges::GOLD_COLLECTED_1) == local.end() && Statistics::goldCollected >= 100) 
	{
		badge = &Badges::GOLD_COLLECTED_1;
		local.insert(*badge);
	}
	if (local.find(Badges::GOLD_COLLECTED_2) == local.end() && Statistics::goldCollected >= 500) 
	{
		badge = &Badges::GOLD_COLLECTED_2;
		local.insert(*badge);
	}
	if (local.find(Badges::GOLD_COLLECTED_3) == local.end() && Statistics::goldCollected >= 2500) 
	{
		badge = &Badges::GOLD_COLLECTED_3;
		local.insert(*badge);
	}
	if (local.find(Badges::GOLD_COLLECTED_4) == local.end() && Statistics::goldCollected >= 7500) 
	{
		badge = &Badges::GOLD_COLLECTED_4;
		local.insert(*badge);
	}

	displayBadge(badge);
}

void Badges::validateLevelReached()
{
	const Badge* badge = NULL;

	if (local.find(Badges::LEVEL_REACHED_1) == local.end() && Dungeon::hero->lvl >= 6) 
	{
		badge = &Badges::LEVEL_REACHED_1;
		local.insert(*badge);
	}
	if (local.find(Badges::LEVEL_REACHED_2) == local.end() && Dungeon::hero->lvl >= 12)
	{
		badge = &Badges::LEVEL_REACHED_2;
		local.insert(*badge);
	}
	if (local.find(Badges::LEVEL_REACHED_3) == local.end() && Dungeon::hero->lvl >= 18)
	{
		badge = &Badges::LEVEL_REACHED_3;
		local.insert(*badge);
	}
	if (local.find(Badges::LEVEL_REACHED_4) == local.end() && Dungeon::hero->lvl >= 24)
	{
		badge = &Badges::LEVEL_REACHED_4;
		local.insert(*badge);
	}

	displayBadge(badge);
}

void Badges::validateStrengthAttained()
{
	const Badge* badge = NULL;

	if (local.find(Badges::STRENGTH_ATTAINED_1) == local.end() && Dungeon::hero->STR >= 13) {
		badge = &Badges::STRENGTH_ATTAINED_1;
		local.insert(*badge);
	}
	if (local.find(Badges::STRENGTH_ATTAINED_2) == local.end() && Dungeon::hero->STR >= 15) {
		badge = &Badges::STRENGTH_ATTAINED_2;
		local.insert(*badge);
	}
	if (local.find(Badges::STRENGTH_ATTAINED_3) == local.end() && Dungeon::hero->STR >= 17) {
		badge = &Badges::STRENGTH_ATTAINED_3;
		local.insert(*badge);
	}
	if (local.find(Badges::STRENGTH_ATTAINED_4) == local.end() && Dungeon::hero->STR >= 19) {
		badge = &Badges::STRENGTH_ATTAINED_4;
		local.insert(*badge);
	}

	displayBadge(badge);
}

void Badges::validateFoodEaten()
{
	const Badge* badge = NULL;

	if (local.find(Badges::FOOD_EATEN_1) == local.end() && Statistics::foodEaten >= 10) 
	{
		badge = &Badges::FOOD_EATEN_1;
		local.insert(*badge);
	}
	if (local.find(Badges::FOOD_EATEN_2) == local.end() && Statistics::foodEaten >= 20) 
	{
		badge = &Badges::FOOD_EATEN_2;
		local.insert(*badge);
	}
	if (local.find(Badges::FOOD_EATEN_3) == local.end() && Statistics::foodEaten >= 30) 
	{
		badge = &Badges::FOOD_EATEN_3;
		local.insert(*badge);
	}
	if (local.find(Badges::FOOD_EATEN_4) == local.end() && Statistics::foodEaten >= 40) 
	{
		badge = &Badges::FOOD_EATEN_4;
		local.insert(*badge);
	}

	displayBadge(badge);
}

void Badges::validatePotionsCooked()
{
	const Badge* badge = NULL;

	if (local.find(Badges::POTIONS_COOKED_1) == local.end() && Statistics::potionsCooked >= 3) 
	{
		badge = &Badges::POTIONS_COOKED_1;
		local.insert(*badge);
	}
	if (local.find(Badges::POTIONS_COOKED_2) == local.end() && Statistics::potionsCooked >= 6) 
	{
		badge = &Badges::POTIONS_COOKED_2;
		local.insert(*badge);
	}
	if (local.find(Badges::POTIONS_COOKED_3) == local.end() && Statistics::potionsCooked >= 9) 
	{
		badge = &Badges::POTIONS_COOKED_3;
		local.insert(*badge);
	}
	if (local.find(Badges::POTIONS_COOKED_4) == local.end() && Statistics::potionsCooked >= 12) 
	{
		badge = &Badges::POTIONS_COOKED_4;
		local.insert(*badge);
	}

	displayBadge(badge);
}

void Badges::validatePiranhasKilled()
{
	const Badge* badge = NULL;

	if (local.find(Badges::PIRANHAS) == local.end() && Statistics::piranhasKilled >= 6) 
	{
		badge = &Badges::PIRANHAS;
		local.insert(*badge);
	}

	displayBadge(badge);
}

bool Badges::isUnlocked(Badges::Badge badge)
{
	return global.find(badge) != global.end();
}