#pragma once
#include "typedefine.h"

class Hero;
class AlchemyPot{
private:
	static const String TXT_SELECT_SEED;

	static Hero* hero;
	static int pos;

	class WndBagListenerNew :public WndBag::Listener{
	public:
		virtual void onSelect(Item* item){
			if (item != NULL){
				item->cast(AlchemyPot::hero, AlchemyPot::pos);
			}
		}
	};

	static WndBag::Listener* itemSelector;
public:
	static void operate(Hero* hero, int pos);
};