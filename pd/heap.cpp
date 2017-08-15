#include "heap.h"
#include "simpleresource.h"
#include "itemsprite.h"
#include "item.h"

Heap::Heap()
{
	type = Type::HEAP;
	pos = 0;
}

ItemSprite::Glowing* Heap::glowing() 
{
	return (type == Type::HEAP || type == Type::FOR_SALE) && items.size() > 0 ? items[0]->glowing() : NULL;
}

int Heap::image()
{
	switch (type) 
	{
	case HEAP:
	case FOR_SALE:
		return size() > 0 ? items[0]->Image() : 0;
	case CHEST:
	case MIMIC:
		return ItemSpriteSheet::CHEST;
	case LOCKED_CHEST:
		return ItemSpriteSheet::LOCKED_CHEST;
	case CRYSTAL_CHEST:
		return ItemSpriteSheet::CRYSTAL_CHEST;
	case TOMB:
		return ItemSpriteSheet::TOMB;
	case SKELETON:
		return ItemSpriteSheet::BONES;
	case HIDDEN:
		return ItemSpriteSheet::HIDDEN;
	default:
		return 0;
	}
}