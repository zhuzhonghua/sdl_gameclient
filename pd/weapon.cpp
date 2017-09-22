#include "weapon.h"
#include "util.h"
#include "speck.h"
#include "charsprite.h"
#include "level.h"
#include "dungeon.h"
#include "simpleresource.h"
#include "belongings.h"
#include "gamescene.h"
#include "scroll.h"
#include "glog.h"
#include "heap.h"
#include "burning.h"
#include "flameparticle.h"
#include "buffpoison.h"

const String Weapon::TXT_IDENTIFY = BPT::getText("lang.weapon_identity");
//"You are now familiar enough with your %s to identify it. It is %s.";
const String Weapon::TXT_INCOMPATIBLE = BPT::getText("lang.weapon_incompatible");
//"Interaction of different types of magic has negated the enchantment on this weapon!";

 const String Weapon::TXT_TO_STRING = "%s :%d";
 const String Weapon::TXT_BROKEN = "broken %s :%d";

 const String Weapon::UNFAMILIRIARITY = "unfamiliarity";
 const String Weapon::ENCHANTMENT = "enchantment";
 const String Weapon::IMBUE = "imbue";

 Weapon::Weapon()
 {
	 hitsToKnow = HITS_TO_KNOW;

	 STR = 10;
	 ACU = 1;
	 DLY = 1.0f;

	 imbue = Imbue::NONE;

	 enchantment = NULL;
 }

void Weapon::proc(Char* attacker, Char* defender, int damage)
{
	if (enchantment != NULL) {
		enchantment->proc(this, attacker, defender, damage);
	}
	
	if (!levelKnown) {
		if (--hitsToKnow <= 0) {
			levelKnown = true;
			GLog::i(TXT_IDENTIFY.c_str(), Name(), toString());
			Badges::validateItemLevelAquired(this);
		}
	}

	use();
}

 void Weapon::storeInBundle(Bundle* bundle)
 {
	 KindOfWeapon::storeInBundle(bundle);
	 bundle->put(UNFAMILIRIARITY, hitsToKnow);
	 bundle->put(ENCHANTMENT, enchantment);
	 bundle->put(IMBUE, imbue);
 }

 void Weapon::restoreFromBundle(Bundle* bundle)
 {
	 KindOfWeapon::restoreFromBundle(bundle);
	 if ((hitsToKnow = bundle->getInt(UNFAMILIRIARITY)) == 0) {
		 hitsToKnow = HITS_TO_KNOW;
	 }
	 enchantment = (Enchantment*)bundle->get(ENCHANTMENT);
	 //imbue = bundle.getEnum(IMBUE, Imbue.class);
 }

float Weapon::acuracyFactor(Hero* hero)
{
	int encumbrance = STR - hero->sTR();
	
	if (dynamic_cast<MissileWeapon*>(this)) {
		 switch (hero->heroClass.type()) {
		 case HeroClass::E_WARRIOR:
			 encumbrance += 3;
			 break;
		 case HeroClass::E_HUNTRESS:
			 encumbrance -= 2;
			 break;
		 default:
		 }
	}
	
	return
		 (encumbrance > 0 ? (float)(ACU / std::pow(1.5, encumbrance)) : ACU) *
		 (imbue == Imbue::ACCURACY ? 1.5f : 1.0f);
	return 0;
}

float Weapon::speedFactor(Hero* hero)
{
	int encumrance = STR - hero->sTR();
	if (dynamic_cast<MissileWeapon*>(this) && hero->heroClass.type() == HeroClass::E_HUNTRESS) {
		 encumrance -= 2;
	}
	
	return
		 (encumrance > 0 ? (float)(DLY * std::pow(1.2, encumrance)) : DLY) *
		 (imbue == Imbue::SPEED ? 0.6f : 1.0f);
}

int Weapon::damageRoll(Hero* hero)
{
	int damage = KindOfWeapon::damageRoll(hero);

	if ((hero->rangedWeapon != NULL) == (hero->heroClass.type() == HeroClass::E_HUNTRESS)) {
		int exStr = hero->sTR() - STR;
		if (exStr > 0) {
			damage += Random::IntRange(0, exStr);
		}
	}

	return damage;
}

Item* Weapon::upgrade(boolean enchant)
{
	if (enchantment != NULL) {
		if (!enchant && Random::Int(Level()) > 0) {
			GLog::w(TXT_INCOMPATIBLE.c_str());
			this->enchant(NULL);
		}
	}
	else {
		if (enchant) {
			this->enchant();
		}
	}

	return KindOfWeapon::upgrade();
}

 String Weapon::toString()
 {
	 return levelKnown ? 
		 GameMath::format(isBroken() ? 
						TXT_BROKEN.c_str() 
						: TXT_TO_STRING.c_str(), KindOfWeapon::toString().c_str(), STR) 
		 : KindOfWeapon::toString();
 }

 String Weapon::Name()
 {
	 return enchantment == NULL ? KindOfWeapon::Name() : enchantment->name(KindOfWeapon::Name());
 }

 Item* Weapon::random()
 {
	 if (Random::Float() < 0.4) {
		 int n = 1;
		 if (Random::Int(3) == 0) {
			 n++;
			 if (Random::Int(3) == 0) {
				 n++;
			 }
		 }
		 if (Random::Int(2) == 0) {
			 upgrade(n);
		 }
		 else {
			 degrade(n);
			 cursed = true;
		 }
	 }
	 return this;
 }

 Weapon* Weapon::enchant(Enchantment* ench)
 {
	 enchantment = ench;
	 return this;
 }

Weapon* Weapon::enchant()
{	 
	std::string oldEnchantment;
	if (enchantment != NULL)
	{
		oldEnchantment = enchantment->getClassName();
	}
	 
	Enchantment* ench = Enchantment::random();
	while (ench->getClassName() == oldEnchantment) {
		ench = Enchantment::random();
	}

	return enchant(ench);
}

 ItemSprite::Glowing* Weapon::glowing()
 {
	 return enchantment != NULL ? enchantment->glowing() : NULL;
 }

 std::string Weapon::Enchantment::_enchants[] ={
	 "EnchantmentFire", "EnchantmentPoison", "Death", "EnchantmentParalysis", "Leech",
	 "EnchantmentSlow", "Shock", "Instability", "Horror", "Luck",
	 "Tempering" };

 std::vector<std::string> Weapon::Enchantment::enchants(_enchants, _enchants+sizeof(_enchants)/sizeof(std::string));
 float Weapon::Enchantment::_chances[] = { 10, 10, 1, 2, 1, 2, 6, 3, 2, 2, 3 };

 std::vector<float> Weapon::Enchantment::chances(_chances, _chances+sizeof(_chances)/sizeof(float));

 std::map<std::string, Weapon::Enchantment::CreateEnchant> Weapon::Enchantment::clsToInst;

 ItemSprite::Glowing* Weapon::Enchantment::glowing()
 {
	 return ItemSprite::Glowing::WHITE;
 }

Weapon::Enchantment* Weapon::Enchantment::random()
{
	 int idx = Random::chances(chances);
	 std::string clsName = enchants[idx];
	 
	 if (clsToInst.size() <= 0) init();

	 return clsToInst[clsName]();
}

void Weapon::Enchantment::init()
{
	 clsToInst.insert(std::make_pair("EnchantmentFire", EnchantmentFire::Create));
	 clsToInst.insert(std::make_pair("EnchantmentPoison", EnchantmentPoison::Create));
	 clsToInst.insert(std::make_pair("Death",Death::Create));
	 clsToInst.insert(std::make_pair("EnchantmentParalysis",EnchantmentParalysis::Create));
	 clsToInst.insert(std::make_pair("Leech",Leech::Create));
	 clsToInst.insert(std::make_pair("EnchantmentSlow",EnchantmentSlow::Create));
	 clsToInst.insert(std::make_pair("Shock",Shock::Create));
	 clsToInst.insert(std::make_pair("Instability",Instability::Create));
	 clsToInst.insert(std::make_pair("Horror",Horror::Create));
	 clsToInst.insert(std::make_pair("Luck",Luck::Create));
	 clsToInst.insert(std::make_pair("Tempering",Tempering::Create));
}

const String EnchantmentFire::TXT_BLAZING = "blazing %s";
ItemSprite::Glowing* EnchantmentFire::ORANGE = new ItemSprite::Glowing(0xFF4400);

boolean EnchantmentFire::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
{
	 // lvl 0 - 33%
	 // lvl 1 - 50%
	 // lvl 2 - 60%
	int level = std::max(0, weapon->effectiveLevel());
	
	if (Random::Int(level + 3) >= 2) {	
		if (Random::Int(2) == 0) {
			((Burning*)Buff::affect(defender, "Burning"))->reignite(defender);
		}
		defender->damage(Random::Int(1, level + 2), this->getClassName());
	
		defender->sprite->emitter()->burst(FlameParticle::FACTORY, level + 1);
	
		return true;	
	}
	else {	
		return false;
	}
}

 String EnchantmentFire::name(const String& weaponName)
 {
	 return GameMath::format(TXT_BLAZING.c_str(), weaponName);
 }

 const String EnchantmentPoison::TXT_VENOMOUS = "venomous %s";
 ItemSprite::Glowing* EnchantmentPoison::PURPLE = new ItemSprite::Glowing(0x4400AA);

boolean EnchantmentPoison::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
{
	 // lvl 0 - 33%
	 // lvl 1 - 50%
	 // lvl 2 - 60%
	int level = std::max(0, weapon->effectiveLevel());
	
	if (Random::Int(level + 3) >= 2) {		
		BuffPoison* buf = (BuffPoison*)Buff::affect(defender, "BuffPoison");
		buf->set(BuffPoison::durationFactor(defender) * (level + 1));
	
		return true;
	}
	else {
		return false;
	}
}

 String EnchantmentPoison::name(const String& weaponName)
 {
	 return GameMath::format(TXT_VENOMOUS.c_str(), weaponName);
 }

const String Death::TXT_GRIM = "grim %s";
ItemSprite::Glowing* Death::BLACK = new ItemSprite::Glowing(0x000000);

boolean Death::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
{
	 // lvl 0 - 8%
	 // lvl 1 ~ 9%
	 // lvl 2 ~ 10%
	int level = std::max(0, weapon->effectiveLevel());

	if (Random::Int(level + 100) >= 92) {
		defender->damage(defender->HP, this->getClassName());
		defender->sprite->emitter()->burst(ShadowParticle::UP, 5);

		if (!defender->isAlive() && dynamic_cast<Hero*>(attacker)) {
			Badges::validateGrimWeapon();
		}

		return true;

	}
	else {
		return false;
	}
}

 String Death::name(const String& weaponName)
 {
	 return GameMath::format(TXT_GRIM.c_str(), weaponName);
 }

 const String EnchantmentParalysis::TXT_STUNNING = "stunning %s";
 ItemSprite::Glowing* EnchantmentParalysis::YELLOW = new ItemSprite::Glowing(0xCCAA44);

 boolean EnchantmentParalysis::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
 {
	 // lvl 0 - 13%
	 // lvl 1 - 22%
	 // lvl 2 - 30%
	 int level = std::max(0, weapon->effectiveLevel());

	 if (Random::Int(level + 8) >= 7) {

		 Buff::prolong(defender, com.watabou.pixeldungeon.actors.buffs.Paralysis.class,
			 Random.Float(1, 1.5f + level));

		 return true;
	 }
	 else {
		 return false;
	 }
 }

 String EnchantmentParalysis::name(const String& weaponName)
 {
	 return GameMath::format(TXT_STUNNING.c_str(), weaponName);
 }

 const String Leech::TXT_VAMPIRIC = "vampiric %s";
 ItemSprite::Glowing* Leech::RED = new ItemSprite::Glowing(0x660022);

 boolean Leech::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
 {
	 int level = std::max(0, weapon->effectiveLevel());

	 // lvl 0 - 33%
	 // lvl 1 - 43%
	 // lvl 2 - 50%
	 int maxValue = damage * (level + 2) / (level + 6);
	 int effValue = std::min(Random::IntRange(0, maxValue), attacker->HT - attacker->HP);

	 if (effValue > 0) {

		 attacker->HP += effValue;
		 attacker->sprite->emitter()->start(Speck::factory(Speck::HEALING), 0.4f, 1);
		 attacker->sprite->showStatus(CharSprite::POSITIVE, GameMath::format("%d", effValue));

		 return true;

	 }
	 else {
		 return false;
	 }
 }

 String Leech::name(const String& weaponName)
 {
	 return GameMath::format(TXT_VAMPIRIC.c_str(), weaponName);
 }

 const String EnchantmentSlow::TXT_CHILLING = "chilling %s";
 ItemSprite::Glowing* EnchantmentSlow::BLUE = new ItemSprite::Glowing(0x0044FF);

 boolean EnchantmentSlow::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
 {
	 // lvl 0 - 25%
	 // lvl 1 - 40%
	 // lvl 2 - 50%
	 int level = std::max(0, weapon->effectiveLevel());

	 if (Random::Int(level + 4) >= 3) {

		 //Buff.prolong(defender, com.watabou.pixeldungeon.actors.buffs.Slow.class,
			 //Random.Float(1, 1.5f + level));

		 return true;
	 }
	 else {
		 return false;
	 }
 }

 String EnchantmentSlow::name(const String& weaponName)
 {
	 return GameMath::format(TXT_CHILLING.c_str(), weaponName);
 }

 const String Shock::TXT_SHOCKING = "shocking %s";

 boolean Shock::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
 {
	 // lvl 0 - 25%
	 // lvl 1 - 40%
	 // lvl 2 - 50%
	 int level = std::max(0, weapon->effectiveLevel());

	 if (Random::Int(level + 4) >= 3) {

		 points[0] = attacker->pos;
		 nPoints = 1;

		 affected.clear();
		 affected.push_back(attacker);

		 hit(defender, Random::Int(1, damage / 2));

		 //attacker.sprite.parent.add(new Lightning(points, nPoints, null));

		 return true;

	 }
	 else {

		 return false;

	 }
 }

 String Shock::name(const String& weaponName)
 {
	 return GameMath::format(TXT_SHOCKING.c_str(), weaponName);
 }

 void Shock::hit(Char* ch, int damage)
 {
	 if (damage < 1) {
		 return;
	 }

	 affected.push_back(ch);
	 //ch->damage(Level::water[ch->pos] && !ch->flying ? (int)(damage * 2) : damage, LightningTrap.LIGHTNING);

	 //ch->sprite->centerEmitter()->burst(SparkParticle::FACTORY, 3);
	 ch->sprite->flash();

	 points[nPoints++] = ch->pos;

	 std::set<Char*> ns;
	 for (int i = 0; i < 8; i++) {
		 Char* n = Actor::findChar(ch->pos + Level::NEIGHBOURS8[i]);
		 if (n != NULL && !affected.contains(n)) {
			 ns.insert(n);
		 }
	 }

	 if (ns.size() > 0) {
		 hit(RandomT<Char*>::element(ns), Random::Int(damage / 2, damage));
	 }
 }

 const String Instability::TXT_UNSTABLE = "unstable %s";

 boolean Instability::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
 {
	 return random()->proc(weapon, attacker, defender, damage);
 }

 String Instability::name(const String& weaponName)
 {
	 return GameMath::format(TXT_UNSTABLE.c_str(), weaponName);
 }


 const String Horror::TXT_ELDRITCH = "eldritch %s";
 ItemSprite::Glowing* Horror::GREY = new ItemSprite::Glowing(0x222222);

 boolean Horror::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
 {
	 // lvl 0 - 20%
	 // lvl 1 - 33%
	 // lvl 2 - 43%
	 int level = std::max(0, weapon->effectiveLevel());

	 if (Random::Int(level + 5) >= 4) {

		 if (defender == Dungeon::hero) {
			 //Buff.affect(defender, Vertigo.class, Vertigo.duration(defender));
		 }
		 else {
			 //Buff.affect(defender, Terror.class, Terror.DURATION).object = attacker.id();
		 }

		 return true;
	 }
	 else {
		 return false;
	 }
 }

 String Horror::name(const String& weaponName)
 {
	 return GameMath::format(TXT_ELDRITCH.c_str(), weaponName);
 }

 const String Luck::TXT_LUCKY = "lucky %s";
 ItemSprite::Glowing* Luck::GREEN = new ItemSprite::Glowing(0x00FF00);

 boolean Luck::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
 {
	 int level = std::max(0, weapon->effectiveLevel());

	 int dmg = damage;
	 for (int i = 1; i <= level + 1; i++) {
		 dmg = std::max(dmg, attacker->damageRoll() - i);
	 }

	 if (dmg > damage) {
		 defender->damage(dmg - damage, this->getClassName());
		 return true;
	 }
	 else {
		 return false;
	 }
 }

 String Luck::name(const String& weaponName)
 {
	 return GameMath::format(TXT_LUCKY.c_str(), weaponName);
 }

 const String Tempering::TXT_TEMPERED = "tempered %s";
 ItemSprite::Glowing* Tempering::GRAY = new ItemSprite::Glowing(0xCC8888);

 boolean Tempering::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
 {
	 weapon->polish();
	 return true;
 }

 String Tempering::name(const String& weaponName)
 {
	 return GameMath::format(TXT_TEMPERED.c_str(), weaponName);
 }

 MeleeWeapon::MeleeWeapon(int tier, float acu, float dly)
 {
	 this->tier = tier;

	 ACU = acu;
	 DLY = dly;

	 STR = typicalSTR();
 }

 String MeleeWeapon::info()
 {
	 //final String p = "\n\n";
	 //
	 //StringBuilder info = new StringBuilder(desc());
	 //
	 //int lvl = visiblyUpgraded();
	 //String quality = lvl != 0 ?
	//	 (lvl > 0 ?
	//	 (isBroken() ? "broken" : "upgraded") :
	//	 "degraded") :
	//	 "";
	 //info.append(p);
	 //info.append("This " + name + " is " + Utils.indefinite(quality));
	 //info.append(" tier-" + tier + " melee weapon. ");
	 //
	 //if (levelKnown) {
	//	 int min = min();
	//	 int max = max();
	//	 info.append("Its average damage is " + (min + (max - min) / 2) + " points per hit. ");
	 //}
	 //else {
	//	 int min = min0();
	//	 int max = max0();
	//	 info.append(
	//		 "Its typical average damage is " + (min + (max - min) / 2) + " points per hit " +
	//		 "and usually it requires " + typicalSTR() + " points of strength. ");
	//	 if (typicalSTR() > Dungeon.hero.STR()) {
	//		 info.append("Probably this weapon is too heavy for you. ");
	//	 }
	 //}
	 //
	 //if (DLY != 1f) {
	//	 info.append("This is a rather " + (DLY < 1f ? "fast" : "slow"));
	//	 if (ACU != 1f) {
	//		 if ((ACU > 1f) == (DLY < 1f)) {
	//			 info.append(" and ");
	//		 }
	//		 else {
	//			 info.append(" but ");
	//		 }
	//		 info.append(ACU > 1f ? "accurate" : "inaccurate");
	//	 }
	//	 info.append(" weapon. ");
	 //}
	 //else if (ACU != 1f) {
	//	 info.append("This is a rather " + (ACU > 1f ? "accurate" : "inaccurate") + " weapon. ");
	 //}
	 //switch (imbue) {
	 //case SPEED:
	//	 info.append("It was balanced to make it faster. ");
	//	 break;
	 //case ACCURACY:
	//	 info.append("It was balanced to make it more accurate. ");
	//	 break;
	 //case NONE:
	 //}
	 //
	 //if (enchantment != null) {
	//	 info.append("It is enchanted.");
	 //}
	 //
	 //if (levelKnown && Dungeon.hero.belongings.backpack.items.contains(this)) {
	//	 if (STR > Dungeon.hero.STR()) {
	//		 info.append(p);
	//		 info.append(
	//			 "Because of your inadequate strength the accuracy and speed " +
	//			 "of your attack with this " + name + " is decreased.");
	//	 }
	//	 if (STR < Dungeon.hero.STR()) {
	//		 info.append(p);
	//		 info.append(
	//			 "Because of your excess strength the damage " +
	//			 "of your attack with this " + name + " is increased.");
	//	 }
	 //}
	 //
	 //if (isEquipped(Dungeon.hero)) {
	//	 info.append(p);
	//	 info.append("You hold the " + name + " at the ready" +
	//		 (cursed ? ", and because it is cursed, you are powerless to let go." : "."));
	 //}
	 //else {
	//	 if (cursedKnown && cursed) {
	//		 info.append(p);
	//		 info.append("You can feel a malevolent magic lurking within " + name + ".");
	//	 }
	 //}
	 //
	 //return info.toString();
	 return "";
 }

 int MeleeWeapon::price()
 {
	 int price = 20 * (1 << (tier - 1));
	 if (enchantment != NULL) {
		 price *= 1.5;
	 }
	 return considerState(price);
 }

 Item* MeleeWeapon::random()
 {
	 Weapon::random();

	 if (Random::Int(10 + Level()) == 0) {
		 enchant();
	 }

	 return this;
 }

Dagger::Dagger()
:MeleeWeapon(1, 1.2f, 1.0f)
{
	name = "dagger";
	image = ItemSpriteSheet::DAGGER;
}

Knuckles::Knuckles()
:MeleeWeapon(1, 1.0f, 0.5f)
{
	name = "knuckleduster";
	image = ItemSpriteSheet::KNUCKLEDUSTER;
}

Quarterstaff::Quarterstaff()
:MeleeWeapon(2, 1.0f, 1.0f)
{
	name = "quarterstaff";
	image = ItemSpriteSheet::QUARTERSTAFF;
}

Spear::Spear()
:MeleeWeapon(2, 1.0f, 1.5f)
{
	name = "spear";
	image = ItemSpriteSheet::SPEAR;
}

Mace::Mace()
:MeleeWeapon(3, 1.0f, 0.8f)
{
	name = "mace";
	image = ItemSpriteSheet::MACE;
}

Sword::Sword()
:MeleeWeapon(3, 1.0f, 1.0f)
{
	name = "sword";
	image = ItemSpriteSheet::SWORD;
}

Longsword::Longsword()
:MeleeWeapon(4, 1.0f, 1.0f)
{
	name = "longsword";
	image = ItemSpriteSheet::LONG_SWORD;
}

BattleAxe::BattleAxe()
:MeleeWeapon(4, 1.2f, 1.0f)
{
	name = "battle axe";
	image = ItemSpriteSheet::BATTLE_AXE;
}

WarHammer::WarHammer()
:MeleeWeapon(5, 1.2f, 1.0f)
{
	name = "war hammer";
	image = ItemSpriteSheet::WAR_HAMMER;
}

Glaive::Glaive()
:MeleeWeapon(5, 1.0f, 1.0f)
{
	name = "glaive";
	image = ItemSpriteSheet::GLAIVE;
}

namespace{
	class WndBagListenerNew :public WndBag::Listener{
	public:
		ShortSword* ss;
		WndBagListenerNew(ShortSword* ssw) :ss(ssw){}

		virtual void onSelect(Item* item){
			if (item != NULL && !(dynamic_cast<Boomerang*>(item))) {

				//Sample.INSTANCE.play(Assets.SND_EVOKE);
				ScrollOfUpgrade::upgrade(Item::curUser);
				Item::evoke(Item::curUser);

				GLog::w(ShortSword::TXT_REFORGED.c_str(), item->Name());

				((MeleeWeapon*)item)->safeUpgrade();
				Item::curUser->spendAndNext(ShortSword::TIME_TO_REFORGE);

				//Badges::validateItemLevelAquired(item);

			}
			else {

				if (dynamic_cast<Boomerang*>(item)) {
					GLog::w(ShortSword::TXT_NOT_BOOMERANG.c_str());
				}

				if (ss->equipped) {
					Item::curUser->belongings->weapon = ss;
				}
				else {
					//ss->collect(Item::curUser->belongings->backpack);
				}
			}
		}
	};
}

const String ShortSword::AC_REFORGE = "REFORGE";
const String ShortSword::TXT_SELECT_WEAPON = "Select a weapon to upgrade";
const String ShortSword::TXT_REFORGED =
"you reforged the short sword to upgrade your %s";
const String ShortSword::TXT_NOT_BOOMERANG =
"you can't upgrade a boomerang this way";

const float ShortSword::TIME_TO_REFORGE = 2.0f;

ShortSword::ShortSword()
:MeleeWeapon(1, 1.0f, 1.0f)
{
	name = "short sword";
	image = ItemSpriteSheet::SHORT_SWORD;

	STR = 11;

	itemSelector = new WndBagListenerNew(this);
}

void ShortSword::actions(Hero* hero, std::vector<std::string>& actions)
{
	MeleeWeapon::actions(hero, actions);
	if (Level() > 0) {
		actions.push_back(AC_REFORGE);
	}
}

void ShortSword::execute(Hero* hero, std::string action)
{
	if (action == AC_REFORGE) {

		if (hero->belongings->weapon == this) {
			equipped = true;
			hero->belongings->weapon = NULL;
		}
		else {
			equipped = false;
			detach(hero->belongings->backpack);
		}

		curUser = hero;

		GameScene::selectItem(itemSelector, WndBag::Mode::WEAPON, TXT_SELECT_WEAPON);

	}
	else {

		MeleeWeapon::execute(hero, action);

	}
}

const String MissileWeapon::TXT_MISSILES = "Missile weapon";
const String MissileWeapon::TXT_YES = "Yes, I know what I'm doing";
const String MissileWeapon::TXT_NO = "No, I changed my mind";
const String MissileWeapon::TXT_R_U_SURE =
"Do you really want to equip it as a melee weapon?";

MissileWeapon::MissileWeapon()
{
	stackable = true;
	levelKnown = true;
	defaultAction = AC_THROW;
}

void MissileWeapon::actions(Hero* hero, std::vector<std::string>& actions)
{
	Weapon::actions(hero, actions);
	if (hero->heroClass.type() != HeroClass::E_HUNTRESS && hero->heroClass.type() != HeroClass::E_ROGUE) {
		for (std::vector<std::string>::iterator itr = actions.begin();
			itr != actions.end(); )
		{
			if ((*itr).compare(AC_EQUIP) == 0)
			{
				itr = actions.erase(itr);
			}
			else if ((*itr).compare(AC_UNEQUIP) == 0)
			{
				itr = actions.erase(itr);
			}
			else
			{
				itr++;
			}
		}
	}
}

void MissileWeapon::proc(Char* attacker, Char* defender, int damage)
{
	Weapon::proc(attacker, defender, damage);

	Hero* hero = (Hero*)attacker;
	if (hero->rangedWeapon == NULL && stackable) {
		if (quantity == 1) {
			doUnequip(hero, false, false);
		}
		else {
			detach(NULL);
		}
	}
}

bool MissileWeapon::doEquip(Hero* hero)
{
	//GameScene.show(
	//	new WndOptions(TXT_MISSILES, TXT_R_U_SURE, TXT_YES, TXT_NO){
	//	@Override
	//	protected void onSelect(int index) {
	//		if (index == 0) {
	//			MissileWeapon.super.doEquip(hero);
	//		}
	//	};
	//}
	//);

	return false;
}

String MissileWeapon::info()
{
	//StringBuilder info = new StringBuilder(desc());
	//
	//int min = min();
	//int max = max();
	//info.append("\n\nAverage damage of this weapon equals to " + (min + (max - min) / 2) + " points per hit. ");
	//
	//if (Dungeon.hero.belongings.backpack.items.contains(this)) {
	//	if (STR > Dungeon.hero.STR()) {
	//		info.append(
	//			"Because of your inadequate strength the accuracy and speed " +
	//			"of your attack with this " + name + " is decreased.");
	//	}
	//	if (STR < Dungeon.hero.STR()) {
	//		info.append(
	//			"Because of your excess strength the damage " +
	//			"of your attack with this " + name + " is increased.");
	//	}
	//}
	//
	//if (isEquipped(Dungeon.hero)) {
	//	info.append("\n\nYou hold the " + name + " at the ready.");
	//}
	//
	//return info.toString();
	return "";
}

void MissileWeapon::onThrow(int cell)
{
	Char* enemy = Actor::findChar(cell);
	if (enemy == NULL || enemy == curUser) {
		Weapon::onThrow(cell);
	}
	else {
		if (!curUser->shoot(enemy, this)) {
			miss(cell);
		}
	}
}

Boomerang::Boomerang()
{
	name = "boomerang";
	image = ItemSpriteSheet::BOOMERANG;

	STR = 10;

	stackable = false;
}

void Boomerang::proc(Char* attacker, Char* defender, int damage)
{
	MissileWeapon::proc(attacker, defender, damage);
	if (dynamic_cast<Hero*>(attacker) && ((Hero*)attacker)->rangedWeapon == this) {
		circleBack(defender->pos, (Hero*)attacker);
	}
}

void Boomerang::cast(Hero* user, int dst)
{
	throwEquiped = isEquipped(user);
	MissileWeapon::cast(user, dst);
}

void Boomerang::circleBack(int from, Hero* owner)
{
	//((MissileSprite)curUser.sprite.parent.recycle(MissileSprite.class)).
	//	reset(from, curUser.pos, curItem, null);

	if (throwEquiped) {
		owner->belongings->weapon = this;
		owner->spend(-TIME_TO_EQUIP);
	}
	//else
	//if (!collect(curUser.belongings.backpack)) {
	//	Dungeon.level.drop(this, owner.pos).sprite.drop();
	//}
}

Dart::Dart()
:Dart(1)
{

}

Dart::Dart(int number)
{
	quantity = number;
	name = "dart";
	image = ItemSpriteSheet::DART;
}

Item* Dart::random()
{
	quantity = Random::Int(5, 15);
	return this;
}

Javelin::Javelin()
:Javelin(1)
{

}

Javelin::Javelin(int number)
{
	quantity = number;
}

void Javelin::proc(Char* attacker, Char* defender, int damage)
{
	MissileWeapon::proc(attacker, defender, damage);
	//Buff::prolong(defender, Cripple.class, Cripple.DURATION);
}

Item* Javelin::random()
{
	quantity = Random::Int(5, 15);
	return this;
}

IncendiaryDart::IncendiaryDart()
:IncendiaryDart(1)
{

}

IncendiaryDart::IncendiaryDart(int number)
{
	quantity = number;
}

void IncendiaryDart::proc(Char* attacker, Char* defender, int damage)
{
	//Buff.affect(defender, Burning.class).reignite(defender);
	MissileWeapon::proc(attacker, defender, damage);
}

Item* IncendiaryDart::random()
{
	quantity = Random::Int(3, 6);
	return this;
}

void IncendiaryDart::onThrow(int cell)
{
	Char* enemy = Actor::findChar(cell);
	if (enemy == NULL || enemy == Item::curUser) {
		if (Level::flamable[cell]) {
			//GameScene.add(Blob.seed(cell, 4, Fire.class));
		}
		else {
			MissileWeapon::onThrow(cell);
		}
	}
	else {
		if (!Item::curUser->shoot(enemy, this)) {
			Dungeon::level->drop(this, cell)->sprite->drop();
		}
	}
}

CurareDart::CurareDart()
:CurareDart(1)
{

}

CurareDart::CurareDart(int number)
{
	quantity = number;
}

void CurareDart::proc(Char* attacker, Char* defender, int damage)
{
	//Buff.prolong(defender, Paralysis.class, DURATION);
	MissileWeapon::proc(attacker, defender, damage);
}

Item* CurareDart::random()
{
	quantity = Random::Int(2, 5);
	return this;
}

Shuriken::Shuriken()
:Shuriken(1)
{
	
}

Shuriken::Shuriken(int number)
{
	quantity = number;

	name = "shuriken";
	image = ItemSpriteSheet::SHURIKEN;

	STR = 13;

	DLY = 0.5f;
}

Item* Shuriken::random()
{
	quantity = Random::Int(5, 15);
	return this;
}

Tamahawk::Tamahawk()
:Tamahawk(1)
{

}

Tamahawk::Tamahawk(int number)
{
	quantity = number;

	name = "tomahawk";
	image = ItemSpriteSheet::TOMAHAWK;

	STR = 17;
}

void Tamahawk::proc(Char* attacker, Char* defender, int damage)
{
	MissileWeapon::proc(attacker, defender, damage);
	//Buff.affect(defender, Bleeding.class).set(damage);
}

Item* Tamahawk::random()
{
	quantity = Random::Int(5, 12);
	return this;
}
