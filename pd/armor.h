#pragma once

#include "equipableitem.h"
#include "typedefine.h"
#include "buff.h"

class Armor :public EquipableItem{
public:
	class Glyph :public Bundlable{
	private:
		static float _chances[];
		static std::vector<float > chances;

		static std::string _glyphs[];
		static std::vector<std::string> glyphs;
	public:
		Glyph();
		virtual std::string getClassName() { return "Glyph"; }

		virtual int proc(Armor* armor, Char* attacker, Char* defender, int damage) = 0;

		virtual String name(const String& armorName) {
			return armorName;
		}

		virtual void restoreFromBundle(Bundle* bundle) {
		}
		virtual void storeInBundle(Bundle* bundle) {
		}
		virtual ItemSprite::Glowing* glowing() {
			return ItemSprite::Glowing::WHITE;
		}
		boolean checkOwner(Char* owner);

		static Glyph* random();
	};
private:
	static const int HITS_TO_KNOW = 10;

	static const String TXT_EQUIP_CURSED;

	static const String TXT_IDENTIFY;

	static const String TXT_TO_STRING;
	static const String TXT_BROKEN;

	static const String TXT_INCOMPATIBLE;
	static const String UNFAMILIRIARITY;
	static const String GLYPH;

	int hitsToKnow;
public:
	int tier;
	int STR;

	Glyph* glyph;
public:
	Armor(int tier);

	virtual std::string getClassName(){ return "Armor"; }
	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);

	virtual void actions(Hero* hero, std::vector<std::string>& actions);
	virtual bool doEquip(Hero* hero);

	virtual float time2equip(Hero* hero);
	virtual bool doUnequip(Hero* hero, bool collect, bool single);
	virtual bool isEquipped(Hero* hero);

	int DR() {
		return tier * (2 + effectiveLevel() + (glyph == NULL ? 0 : 1));
	}
	virtual Item* upgrade() {
		return upgrade(false);
	}
	Item* upgrade(boolean inscribe);
	Item* safeUpgrade() {
		return upgrade(glyph != NULL);
	}
	virtual Item* degrade() {
		STR++;
		return EquipableItem::degrade();
	}
	virtual int maxDurability(int lvl) {
		return 6 * (lvl < 16 ? 16 - lvl : 1);
	}

	int proc(Char* attacker, Char* defender, int damage);
	//public String toString() {
	//	return levelKnown ? Utils.format(isBroken() ? TXT_BROKEN : TXT_TO_STRING, super.toString(), STR) : super.toString();
	//}

	virtual String Name() {
		return glyph == NULL ? EquipableItem::Name() : glyph->name(EquipableItem::Name());
	}

	virtual String info();

	virtual Item* random();
	int typicalSTR() {
		return 7 + tier * 2;
	}

	int typicalDR() {
		return tier * 2;
	}
	virtual int price();
	Armor* inscribe(Glyph* glyph) {
		this->glyph = glyph;
		return this;
	}
	Armor* inscribe();
	boolean isInscribed() {
		return glyph != NULL;
	}
	virtual ItemSprite::Glowing* glowing() {
		return glyph != NULL ? glyph->glowing() : NULL;
	}
};

class Bounce :public Armor::Glyph{
private:
	static const String TXT_BOUNCE;

public:
	virtual int proc(Armor* armor, Char* attacker, Char* defender, int damage);

	virtual String name(const String& weaponName);
};

class Affection :public Armor::Glyph{
private:
	static const String TXT_AFFECTION;

	static ItemSprite::Glowing* PINK;
public:
	virtual int proc(Armor* armor, Char* attacker, Char* defender, int damage);
	virtual String name(const String& weaponName);
	virtual ItemSprite::Glowing* glowing() {
		return PINK;
	}
};

class AntiEntropy :public Armor::Glyph{
private:
	static const String TXT_ANTI_ENTROPY;

	static ItemSprite::Glowing* BLUE;
public:
	virtual int proc(Armor* armor, Char* attacker, Char* defender, int damage);
	virtual String name(const String& weaponName);
	virtual ItemSprite::Glowing* glowing() {
		return BLUE;
	}
};

class Multiplicity :public Armor::Glyph{
private:
	static const String TXT_MULTIPLICITY;

	static ItemSprite::Glowing* PINK;
public:
	virtual int proc(Armor* armor, Char* attacker, Char* defender, int damage);
	virtual String name(const String& weaponName);
	virtual ItemSprite::Glowing* glowing() {
		return PINK;
	}
};

class Potential :public Armor::Glyph{
private:
	static const String TXT_POTENTIAL;

	static ItemSprite::Glowing* BLUE;

public:
	virtual int proc(Armor* armor, Char* attacker, Char* defender, int damage);
	virtual String name(const String& weaponName);
	virtual ItemSprite::Glowing* glowing() {
		return BLUE;
	}
};

class Metabolism :public Armor::Glyph{
private:
	static const String TXT_METABOLISM;

	static ItemSprite::Glowing* RED;
public:
	virtual int proc(Armor* armor, Char* attacker, Char* defender, int damage);
	virtual String name(const String& weaponName);
	virtual ItemSprite::Glowing* glowing() {
		return RED;
	}
};

class Stench :public Armor::Glyph{
private:
	static const String TXT_STENCH;

	static ItemSprite::Glowing* GREEN;

public:
	virtual int proc(Armor* armor, Char* attacker, Char* defender, int damage);
	virtual String name(const String& weaponName);
	virtual ItemSprite::Glowing* glowing() {
		return GREEN;
	}
};

class Viscosity :public Armor::Glyph{
private:
	static const String TXT_VISCOSITY;

	static ItemSprite::Glowing* PURPLE;

public:
	virtual int proc(Armor* armor, Char* attacker, Char* defender, int damage);
	virtual String name(const String& weaponName);
	virtual ItemSprite::Glowing* glowing() {
		return PURPLE;
	}

	class DeferedDamage :public Buff{
	protected:
		int damage;

	private:
		static const String DAMAGE;

	public:
		DeferedDamage();
		virtual std::string getClassName() { return "DeferedDamage"; }
		virtual void storeInBundle(Bundle* bundle);
		virtual void restoreFromBundle(Bundle* bundle);

		virtual boolean attachTo(Char* target);

		void prolong(int damage) {
			this->damage += damage;
		};
		virtual int icon();

		//public String toString() {
		//	return Utils.format("Defered damage (%d)", damage);
		//}
		virtual boolean act();
	};
};

class Displacement :public Armor::Glyph{
private:
	static const String TXT_DISPLACEMENT;

	static ItemSprite::Glowing* BLUE;

public:
	virtual int proc(Armor* armor, Char* attacker, Char* defender, int damage);
	virtual String name(const String& weaponName);
	virtual ItemSprite::Glowing* glowing() {
		return BLUE;
	}
};

class Entanglement :public Armor::Glyph{
private:
	static const String TXT_ENTANGLEMENT;

	static ItemSprite::Glowing* GREEN;
public:
	virtual int proc(Armor* armor, Char* attacker, Char* defender, int damage);
	virtual String name(const String& weaponName);
	virtual ItemSprite::Glowing* glowing() {
		return GREEN;
	}
};

class AutoRepair :public Armor::Glyph{
private:
	static const String TXT_AUTO_REPAIR;

	static ItemSprite::Glowing* GRAY;

public:
	virtual int proc(Armor* armor, Char* attacker, Char* defender, int damage);
	virtual String name(const String& weaponName);
	virtual ItemSprite::Glowing* glowing() {
		return GRAY;
	}
};

class ClothArmor :public Armor{
public:
	ClothArmor();
	virtual std::string getClassName() { return "ClothArmor"; }
	static Item* Create() { return new ClothArmor(); }

	virtual String desc() {
		return "This lightweight armor offers basic protection.";
	}
};

class LeatherArmor :public Armor{
public:
	LeatherArmor();
	virtual std::string getClassName() { return "LeatherArmor"; }
	static Item* Create() { return new LeatherArmor(); }

	virtual String desc() {
		return "Armor made from tanned monster hide. Not as light as cloth armor but provides better protection.";
	}
};

class MailArmor :public Armor{
public:
	MailArmor();
	virtual std::string getClassName() { return "MailArmor"; }
	static Item* Create() { return new MailArmor(); }

	virtual String desc() {
		return
			"Interlocking metal links make for a tough but flexible suit of armor.";
	}
};

class ScaleArmor :public Armor{
public:
	ScaleArmor();
	virtual std::string getClassName() { return "ScaleArmor"; }
	static Item* Create() { return new ScaleArmor(); }

	virtual String desc() {
		return
			"The metal scales sewn onto a leather vest create a flexible, yet protective armor.";
	}
};

class PlateArmor :public Armor{
public:
	PlateArmor();
	virtual std::string getClassName() { return "PlateArmor"; }
	static Item* Create() { return new PlateArmor(); }

	virtual String desc() {
		return
			std::string("Enormous plates of metal are joined together into a suit that provides ") +
			std::string("unmatched protection to any adventurer strong enough to bear its staggering weight.");
	}
};