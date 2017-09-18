#include "weapon.h"
#include "util.h"
#include "speck.h"
#include "charsprite.h"
#include "level.h"
#include "dungeon.h"

 const String Weapon::TXT_IDENTIFY =
"You are now familiar enough with your %s to identify it. It is %s.";
 const String Weapon::TXT_INCOMPATIBLE =
"Interaction of different types of magic has negated the enchantment on this weapon!";

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
 }

 void Weapon::proc(Char* attacker, Char* defender, int damage)
 {
	 //if (enchantment != null) {
	//	 enchantment.proc(this, attacker, defender, damage);
	 //}
	 //
	 //if (!levelKnown) {
	//	 if (--hitsToKnow <= 0) {
	//		 levelKnown = true;
	//		 GLog.i(TXT_IDENTIFY, name(), toString());
	//		 Badges.validateItemLevelAquired(this);
	//	 }
	 //}

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
	 //enchantment = (Enchantment)bundle.get(ENCHANTMENT);
	 //imbue = bundle.getEnum(IMBUE, Imbue.class);
 }

 float Weapon::acuracyFactor(Hero* hero)
 {
	//int encumbrance = STR - hero->STR();
	//
	//if (this instanceof MissileWeapon) {
	//	 switch (hero.heroClass) {
	//	 case WARRIOR:
	//		 encumbrance += 3;
	//		 break;
	//	 case HUNTRESS:
	//		 encumbrance -= 2;
	//		 break;
	//	 default:
	//	 }
	//}
	//
	//return
	//	 (encumbrance > 0 ? (float)(ACU / Math.pow(1.5, encumbrance)) : ACU) *
	//	 (imbue == Imbue.ACCURACY ? 1.5f : 1.0f);
	 return 0;
 }

 float Weapon::speedFactor(Hero* hero)
 {
	//int encumrance = STR - hero.STR();
	//if (this instanceof MissileWeapon && hero.heroClass == HeroClass.HUNTRESS) {
	//	 encumrance -= 2;
	//}
	//
	//return
	//	 (encumrance > 0 ? (float)(DLY * Math.pow(1.2, encumrance)) : DLY) *
	//	 (imbue == Imbue.SPEED ? 0.6f : 1.0f);
	 return 0;
 }

 int Weapon::damageRoll(Hero* hero)
 {
	 int damage = KindOfWeapon::damageRoll(hero);

	 //if ((hero->rangedWeapon != null) == (hero.heroClass == HeroClass.HUNTRESS)) {
	//	 int exStr = hero.STR() - STR;
	//	 if (exStr > 0) {
	//		 damage += Random.IntRange(0, exStr);
	//	 }
	 //}

	 return damage;
 }

 Item* Weapon::upgrade(boolean enchant)
 {
	 //if (enchantment != null) {
	//	 if (!enchant && Random.Int(level()) > 0) {
	//		 GLog.w(TXT_INCOMPATIBLE);
	//		 enchant(null);
	//	 }
	 //}
	 //else {
	//	 if (enchant) {
	//		 enchant();
	//	 }
	 //}

	 return KindOfWeapon::upgrade();
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
 {	 //
	 //Class< ? extends Enchantment> oldEnchantment = enchantment != null ? enchantment.getClass() : null;
	 //Enchantment ench = Enchantment.random();
	 //while (ench.getClass() == oldEnchantment) {
	//	 ench = Enchantment.random();
	 //}

	 //return enchant(ench);
	 return this;
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

 ItemSprite::Glowing* Weapon::Enchantment::glowing()
 {
	 return ItemSprite::Glowing::WHITE;
 }

 Weapon::Enchantment* Weapon::Enchantment::random()
 {
	 //return ((Class<Enchantment>)enchants[Random.chances(chances)]).newInstance();
	 return NULL;
 }

 const String EnchantmentFire::TXT_BLAZING = "blazing %s";
 ItemSprite::Glowing* EnchantmentFire::ORANGE = new ItemSprite::Glowing(0xFF4400);

 boolean EnchantmentFire::proc(Weapon* weapon, Char* attacker, Char* defender, int damage)
 {
	 // lvl 0 - 33%
	 // lvl 1 - 50%
	 // lvl 2 - 60%
	 //int level = Math.max(0, weapon.effectiveLevel());
	 //
	 //if (Random.Int(level + 3) >= 2) {
	 //
	//	 if (Random.Int(2) == 0) {
	//		 Buff.affect(defender, Burning.class).reignite(defender);
	//	 }
	//	 defender.damage(Random.Int(1, level + 2), this);
	 //
	//	 defender.sprite.emitter().burst(FlameParticle.FACTORY, level + 1);
	 //
	//	 return true;
	 //
	 //}
	 //else {

		 return false;

	// }
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
	 //int level = Math.max(0, weapon.effectiveLevel());
	 //
	 //if (Random.Int(level + 3) >= 2) {
	 //
	//	 Buff.affect(defender, com.watabou.pixeldungeon.actors.buffs.Poison.class).
	//		 set(com.watabou.pixeldungeon.actors.buffs.Poison.durationFactor(defender) * (level + 1));
	 //
	//	 return true;
	 //}
	 //else {
		 return false;
	 //}
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
	 //int level = Math.max(0, weapon.effectiveLevel());
	 //
	 //if (Random.Int(level + 100) >= 92) {
	 //
	//	 defender.damage(defender.HP, this);
	//	 defender.sprite.emitter().burst(ShadowParticle.UP, 5);
	 //
	//	 if (!defender.isAlive() && attacker instanceof Hero) {
	//		 Badges.validateGrimWeapon();
	//	 }
	 //
	//	 return true;
	 //
	 //}
	 //else {

		 return false;

	 //}
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

		 //Buff.prolong(defender, com.watabou.pixeldungeon.actors.buffs.Paralysis.class,
		//	 Random.Float(1, 1.5f + level));

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
