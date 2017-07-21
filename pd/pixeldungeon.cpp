#include "stdafx.h"

#include "pixeldungeon.h"

PixelDungeon::PixelDungeon()
{

}

PixelDungeon::~PixelDungeon()
{

}

bool PixelDungeon::landscape()
{
	return width > height;
}

bool PixelDungeon::scaleUp()
{
	//return Preferences.INSTANCE.getBoolean(Preferences.KEY_SCALE_UP, true);
	return false;
}