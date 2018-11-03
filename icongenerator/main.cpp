#include "stdafx.h"

#include "pixelgenerator.h"

int main(int argc, char* argv[])
{
	PixelGenerator app;
	app.init("PixelGenerator", 640, 960);
	app.run();

	return 0;
}