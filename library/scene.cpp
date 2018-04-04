#include "scene.h"

#include "toucharea.h"
using namespace Zhu;
Scene::Scene()
{
	//test = new TouchArea();
	//test->x = 0;
	//test->y = 0;
	//
	//test->width = 200;
	//test->height = 200;

	sp = new Sprite();
}

void Scene::draw()
{
	sp->draw();
}