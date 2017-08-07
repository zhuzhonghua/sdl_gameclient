#pragma once

#include "window.h"
#include "button.h"
#include "ninepatch.h"
#include "bitmaptext.h"

class WndTabbed :public Window{
protected:
	class Tab :public Button{
		friend class WndTabbed;
	public:
		WndTabbed* wnd;
		Tab(WndTabbed* wnd);
	protected:
		const int CUT = 5;

		bool selected;

		NinePatch* bg;

		virtual void layout();

		virtual void select(bool value);

		virtual void onClick();
	};

	class LabeledTab :public Tab{
	private:
		BitmapText* btLabel;
	public:
		LabeledTab(WndTabbed* wnd, const std::string& label);
	protected:
		virtual void createChildren();
		virtual void layout();
		virtual void select(bool value);
	};

protected:
	void onClick(Tab* tab);
	int tabHeight() { return 25; }
	Tab* add(Tab* tab);

	std::vector<Tab*> tabs;
	Tab* selected;
public:
	virtual void resize(int w, int h);

	void select(Tab* tab);
	void select(int index);

	WndTabbed();
};