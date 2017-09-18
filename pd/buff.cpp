#include "buff.h"
#include "char.h"
#include "buffindicator.h"

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

void Buff::detach(Char* target, Buff* cl)
{
	detach(target->buff(cl->getClassName()));
}

int Buff::icon()
{
	return BuffIndicator::NONE;
}

Buff* Buff::append(Char* target, Buff* buff)
{
	buff->attachTo(target);
	return buff;
}

FlavourBuff* Buff::append(Char* target, FlavourBuff* buffClass, float duration)
{
	FlavourBuff* buff = (FlavourBuff*)append(target, buffClass);
	buff->spend(duration);
	return buff;
}

Buff* Buff::affect(Char* target, Buff* buffClass)
{
	Buff* buff = target->buff(buffClass->getClassName());
	if (buff != NULL) 
	{
		return buff;
	}
	else 
	{
		return append(target, buffClass);
	}
}

FlavourBuff* Buff::affect(Char* target, FlavourBuff* buffClass, float duration)
{
	FlavourBuff* buff = (FlavourBuff*)affect(target, buffClass);
	buff->spend(duration);
	return buff;
}

FlavourBuff* Buff::prolong(Char* target, FlavourBuff* buffClass, float duration)
{
	FlavourBuff* buff = (FlavourBuff*)affect(target, buffClass);
	buff->postpone(duration);
	return buff;
}
