#pragma once

#include "app.h"

class PixelGenerator :public Zengine::App{
public:
	virtual void init(const std::string& tile, int w, int h);
	virtual void run();
};