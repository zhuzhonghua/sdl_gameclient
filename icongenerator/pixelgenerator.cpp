#include "stdafx.h"

#include "pixelgenerator.h"
#include "pixelscene.h"

void PixelGenerator::init(const std::string& tile, int w, int h)
{
	App::init(tile, w, h);
}

void PixelGenerator::run()
{
	_scene = new PixelScene();
	App::run();
}