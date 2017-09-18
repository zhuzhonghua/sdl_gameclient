#pragma once

#include "kindofweapon.h"
#include "typedefine.h"

class Weapon :public KindOfWeapon{
public:
	class Enchantment :public Bundlable{
	private:
		static std::string _enchants[];
		static std::vector<std::string> enchants;
		static float _chances[];
		static std::vector<float> chances;

	public:
		virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage) = 0;
		virtual String name(const String& weaponName) {
			return weaponName;
		}
		virtual void restoreFromBundle(Bundle* bundle) {
		}
		virtual void storeInBundle(Bundle* bundle) {
		}
		virtual ItemSprite::Glowing* glowing();

		static Enchantment* random();
	};
private:
	static const int HITS_TO_KNOW = 20;

	static const String TXT_IDENTIFY;
	static const String TXT_INCOMPATIBLE;

	static const String TXT_TO_STRING;
	static const String TXT_BROKEN;
	static const String UNFAMILIRIARITY;
	static const String ENCHANTMENT;
	static const String IMBUE;

	int hitsToKnow;
public:
	int		STR;
	float	ACU;
	float	DLY;

	enum Imbue {
		NONE, SPEED, ACCURACY
	};
	Imbue imbue;

	Weapon();
	virtual void proc(Char* attacker, Char* defender, int damage);

	virtual std::string getClassName() { return "Weapon"; }
	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);
	virtual float acuracyFactor(Hero* hero);
	virtual float speedFactor(Hero* hero);
	virtual int damageRoll(Hero* owner);

	Item* upgrade(boolean enchant);

	virtual int maxDurability(int lvl) {
		return 5 * (lvl < 16 ? 16 - lvl : 1);
	}
	//public String toString() {
	//	return levelKnown ? Utils.format(isBroken() ? TXT_BROKEN : TXT_TO_STRING, super.toString(), STR) : super.toString();
	//}
	virtual String Name();
	virtual Item* random();
	Weapon* enchant(Enchantment* ench);
	Weapon* enchant();
	boolean isEnchanted() {
		return enchantment != NULL;
	}
	virtual ItemSprite::Glowing* glowing();
protected:
	Enchantment* enchantment;
};

class EnchantmentFire :public Weapon::Enchantment{
private:
	static const String TXT_BLAZING;

	static ItemSprite::Glowing* ORANGE;

public:
	virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage);
	virtual ItemSprite::Glowing* glowing() { return ORANGE; }
	virtual String name(const String& weaponName);
};

class EnchantmentPoison :public Weapon::Enchantment{
private:
	static const String TXT_VENOMOUS;

	static ItemSprite::Glowing* PURPLE;

public:
	virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage);
	virtual ItemSprite::Glowing* glowing() { return PURPLE; }
	virtual String name(const String& weaponName);
};

class Death :public Weapon::Enchantment{
private:
	static const String TXT_GRIM;

	static ItemSprite::Glowing* BLACK;

public:
	virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage);
	virtual ItemSprite::Glowing* glowing() { return BLACK; }
	virtual String name(const String& weaponName);
};

class EnchantmentParalysis :public Weapon::Enchantment{
private:
	static const String TXT_STUNNING;

	static ItemSprite::Glowing* YELLOW;

public:
	virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage);
	virtual ItemSprite::Glowing* glowing() { return YELLOW; }
	virtual String name(const String& weaponName);
};

class Leech :public Weapon::Enchantment{
private:
	static const String TXT_VAMPIRIC;

	static ItemSprite::Glowing* RED;

public:
	virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage);
	virtual ItemSprite::Glowing* glowing() { return RED; }
	virtual String name(const String& weaponName);
};

class EnchantmentSlow :public Weapon::Enchantment{
private:
	static const String TXT_CHILLING;

	static ItemSprite::Glowing* BLUE;

public:
	virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage);
	virtual ItemSprite::Glowing* glowing() { return BLUE; }
	virtual String name(const String& weaponName);
};

class Shock :public Weapon::Enchantment{
private:
	static const String TXT_SHOCKING;

	List<Char*> affected;
	int points[20];;
	int nPoints;

	void hit(Char* ch, int damage);
public:
	virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage);
	virtual String name(const String& weaponName);
};

class Instability :public Weapon::Enchantment{
private:
	static const String TXT_UNSTABLE;
public:
	virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage);
	virtual String name(const String& weaponName);
};

class Horror :public Weapon::Enchantment{
private:
	static const String TXT_ELDRITCH;

	static ItemSprite::Glowing* GREY;

public:
	virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage);
	virtual ItemSprite::Glowing* glowing() { return GREY; }
	virtual String name(const String& weaponName);
};

class Luck :public Weapon::Enchantment{
private:
	static const String TXT_LUCKY;

	static ItemSprite::Glowing* GREEN;

public:
	virtual std::string getClassName() { return "Luck"; }

	virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage);
	virtual ItemSprite::Glowing* glowing() { return GREEN; }
	virtual String name(const String& weaponName);
};

class Tempering :public Weapon::Enchantment{
private:
	static const String TXT_TEMPERED;

	static ItemSprite::Glowing* GRAY;

public:
	virtual std::string getClassName() { return "Tempering"; }

	virtual boolean proc(Weapon* weapon, Char* attacker, Char* defender, int damage);
	virtual ItemSprite::Glowing* glowing() { return GRAY; }
	virtual String name(const String& weaponName);
};