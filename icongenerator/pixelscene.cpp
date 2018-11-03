#include "stdafx.h"

#include "pixelscene.h"

#include "sprite.h"
#include "app.h"

using namespace Zengine;
PixelScene::PixelScene()
{
	Sprite* bg = new Sprite("icongenerator/bg.png");
	bg->setPosition(-App::width / 2, -App::height / 2);
	bg->setSize(App::width, App::height);
	add(bg);

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