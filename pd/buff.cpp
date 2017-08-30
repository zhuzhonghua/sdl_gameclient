#include "buff.h"
#include "char.h"

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
