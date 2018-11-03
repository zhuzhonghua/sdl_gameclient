#pragma once

#include "scene.h"
#include "sprite.h"

class PixelScene :public Zengine::Scene{
public:
	PixelScene();
	~PixelScene();

	Zengine::Sprite* img;
};