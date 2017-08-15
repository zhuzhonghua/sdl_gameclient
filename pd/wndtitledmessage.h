#pragma once

#include "window.h"
#include "image.h"
#include "component.h"

class WndTitledMessage :public Window{
private:
	static const int WIDTH_P = 120;
	static const int WIDTH_L = 144;

	static const int GAP = 2;

public:
	WndTitledMessage(Image* icon, const std::string& title, const std::string& message);
	WndTitledMessage(Component* titlebar, const std::string& message);
};