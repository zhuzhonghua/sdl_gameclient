#pragma once

#include "bundlable.h"
#include "item.h"

class PlantSprite;
class Char;

class Plant :public Bundlable{
public:
	class Seed :public Item{
	public:
		Plant* p;
		static const std::string AC_PLANT;

		Seed(Plant* pl);
		virtual void actions(Hero* hero, std::vector<std::string>& actions);
		virtual void execute(Hero* hero, std::string action);

		Plant* couch(int pos);

		virtual bool isUpgradable() { return false; }
		virtual bool isIdentified() { return true; }
		virtual int price() { return 10 * quantity; }
		virtual std::string info();
	protected:
		virtual void onThrow(int cell);
	private:
		static const std::string TXT_INFO;
		static const float TIME_TO_PLANT;
	};
public:
	std::string plantName;

	int image;
	int pos;

	PlantSprite* sprite;

	void activate(Char* ch);
	void wither();

	virtual void restoreFromBundle(Bundle* bundle);
	virtual void storeInBundle(Bundle* bundle);
	virtual std::string getClassName() { return "Plant"; }

	virtual std::string desc() { return NULL; }
private:
	static const std::string POS;
};