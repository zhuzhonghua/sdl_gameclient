#include "buffpoison.h"
#include "buffindicator.h"
#include "glog.h"

const std::string BuffPoison::LEFT = "left";

void BuffPoison::storeInBundle(Bundle* bundle)
{
	Buff::storeInBundle(bundle);
	bundle->put(LEFT, left);
}

void BuffPoison::restoreFromBundle(Bundle* bundle)
{
	Buff::restoreFromBundle(bundle);
	left = bundle->getFloat(LEFT);
}

int BuffPoison::icon()
{
	return BuffIndicator::POISON;
}

bool BuffPoison::act()
{
	if (target->isAlive()) 
	{
		target->damage((int)(left / 3) + 1, this->getClassName());
		spend(TICK);

		if ((left -= TICK) <= 0) 
		{
			detach();
		}
	}
	else 
	{
		detach();
	}

	return true;
}

float BuffPoison::durationFactor(Char* ch)
{
	//Resistance r = ch.buff(Resistance.class);
	//return r != null ? r.durationFactor() : 1;
	return 1;
}

void BuffPoison::onDeath()
{
	//Badges.validateDeathFromPoison();

	//Dungeon.fail(Utils.format(ResultDescriptions.POISON, Dungeon.depth));
	GLog::n("You died from poison...");
}
