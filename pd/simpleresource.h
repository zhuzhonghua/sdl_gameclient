#pragma once
#include "ninepatch.h"

class Chrome{
public:
	enum  Type {
		TOAST,
		TOAST_TR,
		WINDOW,
		BUTTON,
		TAG,
		SCROLL,
		TAB_SET,
		TAB_SELECTED,
		TAB_UNSELECTED
	};

	static NinePatch* get(Type type);
};