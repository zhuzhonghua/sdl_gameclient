#include "plant.h"
#include "hero.h"
#include "dungeon.h"
#include "level.h"
#include "plantsprite.h"
#include "util.h"
#include "generator.h"
#include "terrain.h"

void Plant::activate(Char* ch)
{
	if (dynamic_cast<Hero*>(ch) != NULL && ((Hero*)ch)->subClass == HeroSubClass::WARDEN) 
	{
		//Buff.affect(ch, Barkskin.class).level(ch.HT / 3);
	}

	wither();
}

void Plant::wither()
{
	Dungeon::level->uproot(pos);

	sprite->kill();
	if (Dungeon::visible[pos]) 
	{
		//CellEmitter.get(pos).burst(LeafParticle.GENERAL, 6);
	}

	if (Dungeon::hero->subClass == HeroSubClass::WARDEN) 
	{
		if (Random::Int(5) == 0) 
		{
			//Dungeon::level->drop(Generator::random(Generator::Category::SEED), pos)->sprite->drop();
		}
		if (Random::Int(5) == 0) 
		{
			//Dungeon.level.drop(new Dewdrop(), pos).sprite.drop();
		}
	}
}

void Plant::restoreFromBundle(Bundle* bundle)
{
	pos = bundle->getInt(POS);
}

void Plant::storeInBundle(Bundle* bundle)
{
	bundle->put(POS, pos);
}

const std::string Plant::POS = "pos";

const std::string Plant::Seed::AC_PLANT = "PLANT";

Plant::Seed::Seed(Plant* pl)
:p(pl)
{
	stackable = true;
	defaultAction = AC_THROW;
}

void Plant::Seed::actions(Hero* hero, std::vector<std::string>& actions)
{
	Item::actions(hero, actions);
	actions.push_back(AC_PLANT);
}

void Plant::Seed::execute(Hero* hero, std::string action)
{
	if (action.compare(AC_PLANT) == 0) 
	{
		//hero->spend(TIME_TO_PLANT);
		hero->busy();
		//((Seed*)detach(hero->belongings->backpack))->onThrow(hero->pos);

		//hero->sprite->operate(hero->pos);
	}
	else 
	{
		Item::execute(hero, action);
	}
}

Plant* Plant::Seed::couch(int pos)
{
	//if (Dungeon.visible[pos]) {
	//	//Sample.INSTANCE.play(Assets.SND_PLANT);
	//}
	//Plant plant = plantClass.newInstance();
	//plant.pos = pos;
	//return plant;
	return NULL;
}

std::string Plant::Seed::info()
{
	return GameMath::format(TXT_INFO.c_str(), p->plantName.c_str(), desc());
}

void Plant::Seed::onThrow(int cell)
{
	if (Dungeon::level->map[cell] == Terrain::ALCHEMY || Level::pit[cell]) 
	{
		Item::onThrow(cell);
	}
	else 
	{
		Dungeon::level->plant(this, cell);
	}
}

const std::string Plant::Seed::TXT_INFO = "Throw this seed to the place where you want to grow %s.\n\n%s";

const float Plant::Seed::TIME_TO_PLANT = 1.0f;
