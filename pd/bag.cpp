#include "bag.h"

const std::string Bag::AC_OPEN = "OPEN";
const std::string Bag::ITEMS = "inventory";

Bag::Bag()
{
	image = 11;

	defaultAction = AC_OPEN;

	owner = NULL;
	size = 1;
}

void Bag::storeInBundle(Bundle* bundle)
{
	Item::storeInBundle(bundle);
	//bundle.put(ITEMS, items);
}

void Bag::restoreFromBundle(Bundle* bundle)
{
	Item::restoreFromBundle(bundle);
	//for (Bundlable item : bundle.getCollection(ITEMS)) {
	//	((Item)item).collect(this);
	//};
}
