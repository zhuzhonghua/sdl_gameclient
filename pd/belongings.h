#pragma once

#include <string>

class Hero;
class Bag;

class Belongings{
public:
	static const int BACKPACK_SIZE = 19;

	Bag* backpack;

	Belongings(Hero* owner);
private:
	Hero* owner;
	static const std::string WEAPON;
	static const std::string ARMOR;
	static const std::string RING1;
	static const std::string RING2;
};