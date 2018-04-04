#ifndef __SCENE_H__
#define __SCENE_H__

#include "toucharea.h"
#include "sprite.h"

namespace Zhu{
	class Scene
	{
	public:
		TouchArea* test;
		Sprite* sp;
	public:
		Scene();
		void draw();
	};
}
#endif