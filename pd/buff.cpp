#include "buff.h"
#include "char.h"
#include "buffindicator.h"
#include "dungeon.h"
#include "ring.h"
#include "burning.h"
#include "food.h"
#include "belongings.h"
#include "heap.h"
#include "util.h"
#include "splash.h"
#include "charsprite.h"
#include "dungeon.h"
#include "simpleresource.h"
#include "glog.h"

std::map<std::string, FactoryBuff*> FactoryBuff::facs;

Buff::Buff()
{
	target = NULL;
}

bool Buff::attachTo(Char* target)
{
	if (target->immunitiesContain(getClassName())) 
	{
		return false;
	}
	
	this->target = target;
	target->add(this);

	return true;
}

void Buff::detach()
{
	target->remove(this);
}

void Buff::detach(Buff* buff)
{
	if (buff != NULL) 
	{
		buff->detach();
	}
}

void Buff::detach(Char* target, const std::string& cl)
{
	detach(target->buff(cl));
}

int Buff::icon()
{
	return BuffIndicator::NONE;
}

Buff* Buff::append(Char* target, const std::string& buff)
{
	Buff* buf = FactoryBuff::Create(buff);
	buf->attachTo(target);
	return buf;
}

FlavourBuff* Buff::append(Char* target, const std::string& buffClass, float duration)
{
	FlavourBuff* buff = (FlavourBuff*)append(target, buffClass);
	buff->spend(duration);
	return buff;
}

Buff* Buff::affect(Char* target, const std::string& buffClass)
{
	Buff* buff = target->buff(buffClass);
	if (buff != NULL) 
	{
		return buff;
	}
	else 
	{
		return append(target, buffClass);
	}
}

FlavourBuff* Buff::affect(Char* target, const std::string& buffClass, float duration)
{
	FlavourBuff* buff = (FlavourBuff*)affect(target, buffClass);
	buff->spend(duration);
	return buff;
}

FlavourBuff* Buff::prolong(Char* target, const std::string& buffClass, float duration)
{
	FlavourBuff* buff = (FlavourBuff*)affect(target, buffClass);
	buff->postpone(duration);
	return buff;
}

REFLECTBUFF(FlavourBuff);

const float Light::DURATION = 250.0f;

boolean Light::attachTo(Char* target)
{
	if (FlavourBuff::attachTo(target)) {
		if (Dungeon::level != NULL) {
			target->viewDistance = std::max(Dungeon::level->viewDistance, DISTANCE);
			Dungeon::observe();
		}
		return true;
	}
	else {
		return false;
	}
}

void Light::detach()
{
	target->viewDistance = Dungeon::level->viewDistance;
	Dungeon::observe();
	FlavourBuff::detach();
}

int Light::icon()
{
	return BuffIndicator::LIGHT;
}

REFLECTBUFF(Light);

const float BuffParalysis::DURATION = 10.0f;

boolean BuffParalysis::attachTo(Char* target)
{
	if (FlavourBuff::attachTo(target)) {
		target->paralysed = true;
		return true;
	}
	else {
		return false;
	}
}

void BuffParalysis::detach()
{
	FlavourBuff::detach();
	unfreeze(target);
}

int BuffParalysis::icon()
{
	return BuffIndicator::PARALYSIS;
}

float BuffParalysis::duration(Char* ch)
{
	RingOfElements::Resistance* r = (RingOfElements::Resistance*)ch->buff("Resistance");
	return r != NULL ? r->durationFactor() * DURATION : DURATION;
}

void BuffParalysis::unfreeze(Char* ch)
{
	if (ch->buff("BuffParalysis") == NULL &&
		ch->buff("Frost") == NULL) {
		ch->paralysed = false;
	}
}

REFLECTBUFF(BuffParalysis);

const float Frost::DURATION = 5.0f;

boolean Frost::attachTo(Char* target)
{
	if (FlavourBuff::attachTo(target)) {

		target->paralysed = true;
		Burning::detach(target, "Burning");

		if (dynamic_cast<Hero*>(target)) {
			Hero* hero = (Hero*)target;
			Item* item = hero->belongings->randomUnequipped();
			if (dynamic_cast<MysteryMeat*>(item)) {

				item = item->detach(hero->belongings->backpack);
				FrozenCarpaccio* carpaccio = new FrozenCarpaccio();
				if (!carpaccio->collect(hero->belongings->backpack)) {
					Dungeon::level->drop(carpaccio, target->pos)->sprite->drop();
				}
			}
		}

		return true;
	}
	else {
		return false;
	}
}

void Frost::detach()
{
	FlavourBuff::detach();
	BuffParalysis::unfreeze(target);
}

int Frost::icon()
{
	return BuffIndicator::FROST;
}

float Frost::duration(Char* ch)
{
	RingOfElements::Resistance* r = (RingOfElements::Resistance*)ch->buff("Resistance");
	return r != NULL ? r->durationFactor() * DURATION : DURATION;
}

REFLECTBUFF(Frost);

const float BuffSlow::DURATION = 10.0f;

int BuffSlow::icon()
{
	return BuffIndicator::SLOW;
}

float BuffSlow::duration(Char* ch)
{
	RingOfElements::Resistance* r = (RingOfElements::Resistance*)ch->buff("Resistance");
	return r != NULL ? r->durationFactor() * DURATION : DURATION;
}
REFLECTBUFF(BuffSlow);

const float Vertigo::DURATION=10.0f;

int Vertigo::icon()
{
	return BuffIndicator::VERTIGO;
}

float Vertigo::duration(Char* ch)
{
	RingOfElements::Resistance* r = (RingOfElements::Resistance*)ch->buff("Resistance");
	return r != NULL ? r->durationFactor() * DURATION : DURATION;
}

REFLECTBUFF(Vertigo);

const String Terror::OBJECT = "object";

const float Terror::DURATION = 10.0f;

Terror::Terror()
{
	object = 0;
}

void Terror::storeInBundle(Bundle* bundle)
{
	FlavourBuff::storeInBundle(bundle);
	bundle->put(OBJECT, object);
}

void Terror::restoreFromBundle(Bundle* bundle)
{
	FlavourBuff::restoreFromBundle(bundle);
	object = bundle->getInt(OBJECT);
}

int Terror::icon()
{
	return BuffIndicator::TERROR;
}

void Terror::recover(Char* target)
{
	Terror* terror = (Terror*)target->buff("Terror");
	if (terror != NULL && terror->cooldown() < DURATION) {
		target->remove(terror);
	}
}

REFLECTBUFF(Terror);

const float Cripple::DURATION = 10.0f;

int Cripple::icon()
{
	return BuffIndicator::CRIPPLE;
}

REFLECTBUFF(Cripple);

const String Bleeding::LEVEL = "level";

void Bleeding::storeInBundle(Bundle* bundle)
{
	Buff::storeInBundle(bundle);
	bundle->put(LEVEL, level);
}

void Bleeding::restoreFromBundle(Bundle* bundle)
{
	Buff::restoreFromBundle(bundle);
	level = bundle->getInt(LEVEL);
}

int Bleeding::icon()
{
	return BuffIndicator::BLEEDING;
}

boolean Bleeding::act()
{
	if (target->isAlive()) {

		if ((level = Random::Int(level / 2, level)) > 0) {

			target->damage(level, this->getClassName());
			if (target->sprite->visible) {
				Splash::at(target->sprite->center(), -GameMath::POINTF_PI / 2, GameMath::POINTF_PI / 6,
					target->sprite->blood(), std::min(10 * level / target->HT, 10));
			}

			if (target == Dungeon::hero && !target->isAlive()) {
				Dungeon::fail(GameMath::format(ResultDescriptions::BLEEDING.c_str(), Dungeon::depth));
				GLog::n(BPT::getText("lang.bled_to_death").c_str());
			}

			spend(TICK);
		}
		else {
			detach();
		}

	}
	else {

		detach();

	}

	return true;
}

REFLECTBUFF(Bleeding);