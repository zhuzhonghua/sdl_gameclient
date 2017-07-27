#include "stdafx.h"

#include "pixeldungeon.h"
#include "pixelscene.h"

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

void PixelDungeon::switchNoFade(Scene* c)
{
	PixelScene::noFade = true;
	switchScene(c);
}