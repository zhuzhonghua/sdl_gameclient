#include "buff.h"
#include "char.h"
#include "buffindicator.h"
#include "dungeon.h"
#include "ring.h"
#include "burning.h"
#include "food.h"
#include "belongings.h"
#include "heap.h"


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