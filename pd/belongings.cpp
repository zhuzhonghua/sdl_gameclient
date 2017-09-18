#include "belongings.h"
#include "bag.h"

const std::string Belongings::WEAPON = "weapon";
const std::string Belongings::ARMOR = "armor";
const std::string Belongings::RING1 = "ring1";
const std::string Belongings::RING2 = "ring2";

Belongings::Belongings(Hero* owner)
{
	this->owner = owner;

	backpack = new Bag();
	backpack->name = "backpack";
	backpack->size = BACKPACK_SIZE;
	backpack->owner = owner;

	weapon = NULL;
	armor = NULL;
	ring1 = NULL;
	ring2 = NULL;
}