#include "stdafx.h"

#include "pixelscene.h"

#include "sprite.h"
#include "app.h"

using namespace Zengine;
PixelScene::PixelScene()
{
	//
	img = new Sprite("gllogo.png");
	float x = App::width - img->getWidth();
	x /= 2;
	x -= App::width / 2;
	float y = App::height - img->getHeight();
	y /= 2;
	y -= App::height / 2;
	img->setPosition(x, y);

	add(img);
}

PixelScene::~PixelScene()
{

}