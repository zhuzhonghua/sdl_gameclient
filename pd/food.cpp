#include "food.h"
#include "simpleresource.h"
#include "hero.h"
#include "belongings.h"
#include "glog.h"
#include "speck.h"
#include "charsprite.h"
#include "statistics.h"

const float Food::TIME_TO_EAT = 3.0f;

const std::string Food::AC_EAT = "EAT";

Food::Food()
{
	stackable = true;
	name = "ration of food";
	image = ItemSpriteSheet::RATION;

	//energy = Hunger.HUNGRY;
	message = "That food tasted delicious!";
}

void Food::actions(Hero* hero, std::vector<std::string>& actions)
{
	Item::actions(hero, actions);
	actions.push_back(AC_EAT);
}

void Food::execute(Hero* hero, const std::string& action)
{
	if (action.compare(AC_EAT) == 0) 
	{
		detach(hero->belongings->backpack);

		//((Hunger)hero.buff(Hunger.class)).satisfy(energy);
		GLog::i(message.c_str());

		switch (hero->heroClass.type()) 
		{
		case HeroClass::E_WARRIOR:
			if (hero->HP < hero->HT) 
			{
				hero->HP = std::min(hero->HP + 5, hero->HT);
				hero->sprite->emitter()->burst(Speck::factory(Speck::HEALING), 1);
			}
			break;
		case HeroClass::E_MAGE:
			//hero->belongings->charge(false);
			//ScrollOfRecharging.charge(hero);
			break;
		case HeroClass::E_ROGUE:
		case HeroClass::E_HUNTRESS:
			break;
		}

		//hero->sprite->operate(hero->pos);
		hero->busy();
		//SpellSprite.show(hero, SpellSprite.FOOD);
		//Sample.INSTANCE.play(Assets.SND_EAT);

		//hero->spend(TIME_TO_EAT);

		Statistics::foodEaten++;
		Badges::validateFoodEaten();
	}
	else 
	{
		Item::execute(hero, action);
	}
}
