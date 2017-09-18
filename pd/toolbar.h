#pragma once

#include "component.h"
#include "button.h"
#include "itemsprite.h"

class Image;

class Toolbar :public Component{
private:
	class Tool :public Button{
	private:
		static const int BGCOLOR = 0x7B8073;

	protected:
		Image* base;

		virtual void createChildren();
		virtual void layout();
		virtual void onTouchDown();
		virtual void onTouchUp();
	public:
		Tool(int x, int y, int width, int height);
		void enable(bool value);
	};

	class QuickslotTool : public Tool{

	};

	class PickedUpItem : public ItemSprite{
	private:
		static const float DISTANCE;
		static const float DURATION;

		float dstX;
		float dstY;
		float left;
	public:
		PickedUpItem();
		void reset(Item* item, float dstX, float dstY);

		virtual void update();
	};

public:
	void pickup(Item* item);
};