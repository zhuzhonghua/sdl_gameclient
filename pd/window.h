#pragma once

#include "group.h"
#include "signal.h"
#include "keys.h"
#include "ninepatch.h"
#include "toucharea.h"
#include "shadowbox.h"

class Window:public Group, public Signal<Keys::Key>::Listener{
public:
	int width;
	int height;

	TouchArea* blocker;
	ShadowBox* shadow;
	NinePatch* chrome;

public:
	static const int TITLE_COLOR = 0xFFFF44;

	Window();
	Window(int width, int height);
	Window(int width, int height, NinePatch* chrome);

	virtual void onBackPressed() 
	{
		hide();
	}
	void hide() 
	{
		parent->erase(this);
		destroy();
		delete this;
	}

	virtual void resize(int w, int h);
	virtual void destroy();

	virtual void onSignal(Keys::Key* key);
};