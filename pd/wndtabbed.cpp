#include "wndtabbed.h"
#include "simpleresource.h"
#include "pixelscene.h"
#include "game.h"

WndTabbed::Tab::Tab(WndTabbed* wnd)
{
	this->wnd = wnd;
	bg = NULL;
	init();
}

void WndTabbed::Tab::layout()
{
	Button::layout();

	if (bg != NULL) 
	{
		bg->x = _x;
		bg->y = _y;
		bg->size(_width, _height);
	}
}

void WndTabbed::Tab::select(bool value)
{
	active = !(selected = value);

	if (bg != NULL) 
	{
		remove(bg);
	}

	bg = Chrome::get(selected ? Chrome::Type::TAB_SELECTED : Chrome::Type::TAB_UNSELECTED);
	addToBack(bg);

	layout();
}

void WndTabbed::Tab::onClick()
{
	//Sample.INSTANCE.play(Assets.SND_CLICK, 0.7f, 0.7f, 1.2f);
	wnd->onClick(this);
}

WndTabbed::LabeledTab::LabeledTab(WndTabbed* wnd, const std::string& label) 
:Tab(wnd)
{
	init();
	btLabel->text(label);
	btLabel->measure();
}

void WndTabbed::LabeledTab::createChildren()
{
	btLabel = PixelScene::createText(9);
	add(btLabel);
}

void WndTabbed::LabeledTab::layout()
{
	Tab::layout();

	btLabel->x = PixelScene::align(_x + (_width - btLabel->Width()) / 2);
	btLabel->y = PixelScene::align(_y + (_height - btLabel->baseLine()) / 2) - 1;
	if (!selected) 
	{
		btLabel->y -= 2;
	}
}

void WndTabbed::LabeledTab::select(bool value)
{
	Tab::select(value);
	btLabel->am = selected ? 1.0f : 0.6f;
}

void WndTabbed::onClick(Tab* tab)
{
	select(tab);
}

void WndTabbed::resize(int w, int h)
{
	this->width = w;
	this->height = h;

	chrome->size(
		width + chrome->marginHor(),
		height + chrome->marginVer());

	cameraf->resize((int)chrome->widthf, (int)(chrome->marginTop() + height + tabHeight()));
	cameraf->x = (int)(Game::width - cameraf->screenWidth()) / 2;
	cameraf->y = (int)(Game::height - cameraf->screenHeight()) / 2;

	shadow->boxRect(
		cameraf->x / cameraf->zoomf,
		cameraf->y / cameraf->zoomf,
		chrome->Width(), chrome->heightf);

	for (int i = 0; i < tabs.size();i++)
	{
		remove(tabs[i]);
	}

	std::vector<Tab*> newTabs = this->tabs;
	
	this->tabs.clear();

	for (int i = 0; i < newTabs.size();i++)
	{
		add(newTabs[i]);
	}
}

void WndTabbed::select(Tab* tab)
{
	if (tab != selected) 
	{
		for (int i = 0; i < tabs.size();i++)
		{
			if (tabs[i] == selected)
			{
				tabs[i]->select(false);
			}
			else if (tabs[i] == tab)
			{
				tabs[i]->select(true);
			}
		}

		selected = tab;
	}
}

void WndTabbed::select(int index)
{
	select(tabs[index]);
}

WndTabbed::Tab* WndTabbed::add(Tab* tab)
{
	tab->setPos(tabs.size() == 0 ?
		-chrome->marginLeft() + 1 :
		tabs[tabs.size() - 1]->right(), height);
	tab->select(false);

	Window::add(tab);

	tabs.push_back(tab);

	return tab;
}

WndTabbed::WndTabbed()
:Window(0, 0, Chrome::get(Chrome::Type::TAB_SET))
{

}