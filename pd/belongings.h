#pragma once

#include <string>
#include <vector>

class Hero;
class Bag;
class KindOfWeapon;
class Armor;
class Ring;

class Belongings{
public:
	static const int BACKPACK_SIZE = 19;

	Bag* backpack;
	KindOfWeapon* weapon;
	Armor* armor;
	Ring* ring1;
	Ring* ring2;

	Belongings(Hero* owner);

	Item* getItem(const std::string& itemClass);
	Item* randomUnequipped();
private:
	Hero* owner;
	static const std::string WEAPON;
	static const std::string ARMOR;
	static const std::string RING1;
	static const std::string RING2;

	std::vector<Item*> _items;
};