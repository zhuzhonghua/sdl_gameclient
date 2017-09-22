#include "armor.h"
#include "belongings.h"
#include "herosprite.h"
#include "util.h"
#include "glog.h"
#include "level.h"
#include "dungeon.h"
#include "mob.h"
#include "buffindicator.h"
#include "simpleresource.h"

const String Armor::TXT_EQUIP_CURSED = "your %s constricts around you painfully";
const String Armor::TXT_IDENTIFY = "you are now familiar enough with your %s to identify it. It is %s.";
const String Armor::TXT_TO_STRING = "%s :%d";
const String Armor::TXT_BROKEN = "broken %s :%d";
const String Armor::TXT_INCOMPATIBLE =
"Interaction of different types of magic has erased the glyph on this armor!";

const String Armor::UNFAMILIRIARITY = "unfamiliarity";
const String Armor::GLYPH = "glyph";

Armor::Armor(int tier)
{
	hitsToKnow = HITS_TO_KNOW;

	this->tier = tier;

	STR = typicalSTR();
}

void Armor::storeInBundle(Bundle* bundle)
{
	EquipableItem::storeInBundle(bundle);
	bundle->put(UNFAMILIRIARITY, hitsToKnow);
	bundle->put(GLYPH, glyph);
}

void Armor::restoreFromBundle(Bundle* bundle)
{
	EquipableItem::restoreFromBundle(bundle);
	if ((hitsToKnow = bundle->getInt(UNFAMILIRIARITY)) == 0) {
		hitsToKnow = HITS_TO_KNOW;
	}
	//inscribe((Glyph)bundle.get(GLYPH));
}

void Armor::actions(Hero* hero, std::vector<std::string>& actions)
{
	EquipableItem::actions(hero, actions);
	actions.push_back(isEquipped(hero) ? AC_UNEQUIP : AC_EQUIP);
}

bool Armor::doEquip(Hero* hero)
{
	//detach(hero.belongings.backpack);
	//
	//if (hero.belongings.armor == null || hero.belongings.armor.doUnequip(hero, true, false)) {
	//
	//	hero.belongings.armor = this;
	//
	//	cursedKnown = true;
	//	if (cursed) {
	//		equipCursed(hero);
	//		GLog.n(TXT_EQUIP_CURSED, toString());
	//	}
	//
	//	((HeroSprite)hero.sprite).updateArmor();
	//
	//	hero.spendAndNext(time2equip(hero));
	//	return true;
	//
	//}
	//else {
	//
	//	collect(hero.belongings.backpack);
	//	return false;
	//
	//}
}

float Armor::time2equip(Hero* hero)
{
	return 2 / hero->speed();
}

bool Armor::doUnequip(Hero* hero, bool collect, bool single)
{
	if (EquipableItem::doUnequip(hero, collect, single)) {

		hero->belongings->armor = NULL;
		((HeroSprite*)hero->sprite)->updateArmor();

		return true;

	}
	else {

		return false;

	}
}

bool Armor::isEquipped(Hero* hero)
{
	return hero->belongings->armor == this;
}

Item* Armor::upgrade(boolean ins)
{
	if (glyph != NULL) {
		if (!ins && Random::Int(Level()) > 0) {
			GLog::w(TXT_INCOMPATIBLE.c_str());
			inscribe(NULL);
		}
	}
	else {
		if (ins) {
			inscribe();
		}
	};

	STR--;

	return EquipableItem::upgrade();
}

int Armor::proc(Char* attacker, Char* defender, int damage)
{
	if (glyph != NULL) {
		damage = glyph->proc(this, attacker, defender, damage);
	}

	if (!levelKnown) {
		if (--hitsToKnow <= 0) {
			levelKnown = true;
			//GLog::w(TXT_IDENTIFY, name(), toString());
			//Badges.validateItemLevelAquired(this);
		}
	}

	use();

	return damage;
}

String Armor::info()
{
	//String name = name();
	//StringBuilder info = new StringBuilder(desc());
	//
	//if (levelKnown) {
	//	info.append(
	//		"\n\nThis " + name + " provides damage absorption up to " +
	//		"" + Math.max(DR(), 0) + " points per attack. ");
	//
	//	if (STR > Dungeon.hero.STR()) {
	//
	//		if (isEquipped(Dungeon.hero)) {
	//			info.append(
	//				"\n\nBecause of your inadequate strength your " +
	//				"movement speed and defense skill is decreased. ");
	//		}
	//		else {
	//			info.append(
	//				"\n\nBecause of your inadequate strength wearing this armor " +
	//				"will decrease your movement speed and defense skill. ");
	//		}
	//
	//	}
	//}
	//else {
	//	info.append(
	//		"\n\nTypical " + name + " provides damage absorption up to " + typicalDR() + " points per attack " +
	//		" and requires " + typicalSTR() + " points of strength. ");
	//	if (typicalSTR() > Dungeon.hero.STR()) {
	//		info.append("Probably this armor is too heavy for you. ");
	//	}
	//}
	//
	//if (glyph != null) {
	//	info.append("It is enchanted.");
	//}
	//
	//if (isEquipped(Dungeon.hero)) {
	//	info.append("\n\nYou are wearing the " + name +
	//		(cursed ? ", and because it is cursed, you are powerless to remove it." : "."));
	//}
	//else {
	//	if (cursedKnown && cursed) {
	//		info.append("\n\nYou can feel a malevolent magic lurking within the " + name + ".");
	//	}
	//}
	//
	//return info.toString();
	return "";
}

Item* Armor::random()
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
			EquipableItem::degrade(n);
			cursed = true;
		}
	}

	if (Random::Int(10) == 0) {
		inscribe();
	}

	return this;
}

int Armor::price()
{
	int price = 10 * (1 << (tier - 1));
	if (glyph != NULL) {
		price *= 1.5;
	}
	return considerState(price);
}

Armor* Armor::inscribe()
{
	//Class< ? extends Glyph> oldGlyphClass = glyph != null ? glyph.getClass() : null;
	//Glyph gl = Glyph.random();
	//while (gl.getClass() == oldGlyphClass) {
	//	gl = Armor.Glyph.random();
	//}
	//
	//return inscribe(gl);
	return NULL;
}

float Armor::Glyph::_chances[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

std::vector<float > Armor::Glyph::chances(_chances, _chances+sizeof(_chances)/sizeof(float));

std::string Armor::Glyph::_glyphs[] = {
	"Bounce", "Affection", "AntiEntropy", "Multiplicity",
	"Potential", "Metabolism", "Stench", "Viscosity",
	"Displacement", "Entanglement", "AutoRepair" };

std::vector<std::string> Armor::Glyph::glyphs(_glyphs, _glyphs+sizeof(_glyphs)/sizeof(std::string));

boolean Armor::Glyph::checkOwner(Char* owner)
{
	if (!owner->isAlive() && dynamic_cast<Hero*>(owner)) {

		((Hero*)owner)->killerGlyph = this;
		//Badges.validateDeathFromGlyph();
		return true;

	}
	else {
		return false;
	}
}

Armor::Glyph* Armor::Glyph::random()
{
	//return ((Class<Glyph>)glyphs[Random.chances(chances)]).newInstance();
	return NULL;
}

const String Bounce::TXT_BOUNCE = "%s of bounce";

int Bounce::proc(Armor* armor, Char* attacker, Char* defender, int damage)
{
	int level = std::max(0, armor->effectiveLevel());

	if (Level::adjacent(attacker->pos, defender->pos) && Random::Int(level + 5) >= 4) {

		for (int i = 0; i < 8; i++) {
			int ofs = Level::NEIGHBOURS8[i];
			if (attacker->pos - defender->pos == ofs) {
				int newPos = attacker->pos + ofs;
				if ((Level::passable[newPos] || Level::avoid[newPos]) && Actor::findChar(newPos) == NULL) {

					//Actor.addDelayed(new Pushing(attacker, attacker.pos, newPos), -1);

					attacker->pos = newPos;
					// FIXME
					if (dynamic_cast<Mob*>(attacker)) {
						Dungeon::level->mobPress((Mob*)attacker);
					}
					else {
						Dungeon::level->press(newPos, attacker);
					}

				}
				break;
			}
		}

	}

	return damage;
}

String Bounce::name(const String& weaponName)
{
	return GameMath::format(TXT_BOUNCE.c_str(), weaponName);
}

const String Affection::TXT_AFFECTION = "%s of affection";
ItemSprite::Glowing* Affection::PINK = new ItemSprite::Glowing(0xFF4488);

int Affection::proc(Armor* armor, Char* attacker, Char* defender, int damage)
{
	//int level = (int)GameMath.gate(0, armor.effectiveLevel(), 6);
	//
	//if (Level.adjacent(attacker.pos, defender.pos) && Random.Int(level / 2 + 5) >= 4) {
	//
	//	int duration = Random.IntRange(3, 7);
	//
	//	Buff.affect(attacker, Charm.class, Charm.durationFactor(attacker) * duration).object = defender.id();
	//	attacker.sprite.centerEmitter().start(Speck.factory(Speck.HEART), 0.2f, 5);
	//
	//	duration *= Random.Float(0.5f, 1);
	//
	//	Buff.affect(defender, Charm.class, Charm.durationFactor(defender) * duration).object = attacker.id();
	//	defender.sprite.centerEmitter().start(Speck.factory(Speck.HEART), 0.2f, 5);
	//}

	return damage;
}

String Affection::name(const String& weaponName)
{
	return GameMath::format(TXT_AFFECTION.c_str(), weaponName);
}

const String AntiEntropy::TXT_ANTI_ENTROPY = "%s of anti-entropy";

ItemSprite::Glowing* AntiEntropy::BLUE = new ItemSprite::Glowing(0x0000FF);

int AntiEntropy::proc(Armor* armor, Char* attacker, Char* defender, int damage)
{
	//int level = Math.max(0, armor.effectiveLevel());
	//
	//if (Level.adjacent(attacker.pos, defender.pos) && Random.Int(level + 6) >= 5) {
	//
	//	Buff.prolong(attacker, Frost.class, Frost.duration(attacker) * Random.Float(1f, 1.5f));
	//	CellEmitter.get(attacker.pos).start(SnowParticle.FACTORY, 0.2f, 6);
	//
	//	Buff.affect(defender, Burning.class).reignite(defender);
	//	defender.sprite.emitter().burst(FlameParticle.FACTORY, 5);
	//
	//}

	return damage;
}

String AntiEntropy::name(const String& weaponName)
{
	return GameMath::format(TXT_ANTI_ENTROPY.c_str(), weaponName);
}

const String Multiplicity::TXT_MULTIPLICITY = "%s of multiplicity";
ItemSprite::Glowing* Multiplicity::PINK = new ItemSprite::Glowing(0xCCAA88);

int Multiplicity::proc(Armor* armor, Char* attacker, Char* defender, int damage)
{
	//int level = Math.max(0, armor.effectiveLevel());
	//
	//if (Random.Int(level / 2 + 6) >= 5) {
	//
	//	ArrayList<Integer> respawnPoints = new ArrayList<Integer>();
	//
	//	for (int i = 0; i < Level.NEIGHBOURS8.length; i++) {
	//		int p = defender.pos + Level.NEIGHBOURS8[i];
	//		if (Actor.findChar(p) == null && (Level.passable[p] || Level.avoid[p])) {
	//			respawnPoints.add(p);
	//		}
	//	}
	//
	//	if (respawnPoints.size() > 0) {
	//		MirrorImage mob = new MirrorImage();
	//		mob.duplicate((Hero)defender);
	//		GameScene.add(mob);
	//		WandOfBlink.appear(mob, Random.element(respawnPoints));
	//
	//		defender.damage(Random.IntRange(1, defender.HT / 6), this);
	//		checkOwner(defender);
	//	}
	//
	//}

	return damage;
}

String Multiplicity::name(const String& weaponName)
{
	return GameMath::format(TXT_MULTIPLICITY.c_str(), weaponName);
}

const String Potential::TXT_POTENTIAL = "%s of potential";
ItemSprite::Glowing* Potential::BLUE = new ItemSprite::Glowing(0x66CCEE);

int Potential::proc(Armor* armor, Char* attacker, Char* defender, int damage)
{
	//int level = Math.max(0, armor.effectiveLevel());
	//
	//if (Level.adjacent(attacker.pos, defender.pos) && Random.Int(level + 7) >= 6) {
	//
	//	int dmg = Random.IntRange(1, damage);
	//	attacker.damage(dmg, LightningTrap.LIGHTNING);
	//	dmg = Random.IntRange(1, dmg);
	//	defender.damage(dmg, LightningTrap.LIGHTNING);
	//
	//	checkOwner(defender);
	//	if (defender == Dungeon.hero) {
	//		Camera.main.shake(2, 0.3f);
	//	}
	//
	//	int[] points = { attacker.pos, defender.pos };
	//	attacker.sprite.parent.add(new Lightning(points, 2, null));
	//
	//}

	return damage;
}

String Potential::name(const String& weaponName)
{
	return GameMath::format(TXT_POTENTIAL.c_str(), weaponName);
}

const String Metabolism::TXT_METABOLISM = "%s of metabolism";
ItemSprite::Glowing* Metabolism::RED = new ItemSprite::Glowing(0xCC0000);

int Metabolism::proc(Armor* armor, Char* attacker, Char* defender, int damage)
{
	//int level = Math.max(0, armor.effectiveLevel());
	//if (Random.Int(level / 2 + 5) >= 4) {
	//
	//	int healing = Math.min(defender.HT - defender.HP, Random.Int(1, defender.HT / 5));
	//	if (healing > 0) {
	//
	//		Hunger hunger = defender.buff(Hunger.class);
	//
	//		if (hunger != null && !hunger.isStarving()) {
	//
	//			hunger.satisfy(-Hunger.STARVING / 10);
	//			BuffIndicator.refreshHero();
	//
	//			defender.HP += healing;
	//			defender.sprite.emitter().burst(Speck.factory(Speck.HEALING), 1);
	//			defender.sprite.showStatus(CharSprite.POSITIVE, Integer.toString(healing));
	//		}
	//	}
	//
	//}

	return damage;
}

String Metabolism::name(const String& weaponName)
{
	return GameMath::format(TXT_METABOLISM.c_str(), weaponName);
}

const String Stench::TXT_STENCH = "%s of stench";
ItemSprite::Glowing* Stench::GREEN = new ItemSprite::Glowing(0x22CC44);

int Stench::proc(Armor* armor, Char* attacker, Char* defender, int damage)
{
	//int level = Math.max(0, armor.effectiveLevel());
	//
	//if (Level.adjacent(attacker.pos, defender.pos) && Random.Int(level + 5) >= 4) {
	//
	//	GameScene.add(Blob.seed(attacker.pos, 20, ToxicGas.class));
	//
	//}

	return damage;
}

String Stench::name(const String& weaponName)
{
	return GameMath::format(TXT_STENCH.c_str(), weaponName);
}

const String Viscosity::TXT_VISCOSITY = "%s of viscosity";
ItemSprite::Glowing* Viscosity::PURPLE = new ItemSprite::Glowing(0x8844CC);

int Viscosity::proc(Armor* armor, Char* attacker, Char* defender, int damage)
{
	if (damage == 0) {
		return 0;
	}

	//int level = Math.max(0, armor.effectiveLevel());
	//
	//if (Random.Int(level + 7) >= 6) {
	//
	//	DeferedDamage debuff = defender.buff(DeferedDamage.class);
	//	if (debuff == null) {
	//		debuff = new DeferedDamage();
	//		debuff.attachTo(defender);
	//	}
	//	debuff.prolong(damage);
	//
	//	defender.sprite.showStatus(CharSprite.WARNING, "deferred %d", damage);
	//
	//	return 0;
	//
	//}
	//else {
		return damage;
	//}
}

String Viscosity::name(const String& weaponName)
{
	return GameMath::format(TXT_VISCOSITY.c_str(), weaponName);
}

const String Viscosity::DeferedDamage::DAMAGE = "damage";
Viscosity::DeferedDamage::DeferedDamage()
{
	damage = 0;
}

void Viscosity::DeferedDamage::storeInBundle(Bundle* bundle)
{
	Buff::storeInBundle(bundle);
	bundle->put(DAMAGE, damage);
}

void Viscosity::DeferedDamage::restoreFromBundle(Bundle* bundle)
{
	Buff::restoreFromBundle(bundle);
	damage = bundle->getInt(DAMAGE);
}

boolean Viscosity::DeferedDamage::attachTo(Char* target)
{
	if (Buff::attachTo(target)) {
		postpone(TICK);
		return true;
	}
	else {
		return false;
	}
}

int Viscosity::DeferedDamage::icon()
{
	return BuffIndicator::DEFERRED;
}

boolean Viscosity::DeferedDamage::act()
{
	if (target->isAlive()) {

		target->damage(1, this->getClassName());
		if (target == Dungeon::hero && !target->isAlive()) {
			// FIXME
			//Dungeon.fail(Utils.format(ResultDescriptions.GLYPH, "enchantment of viscosity", Dungeon.depth));
			GLog::n("The enchantment of viscosity killed you...");

			//Badges.validateDeathFromGlyph();
		}
		spend(TICK);

		if (--damage <= 0) {
			detach();
		}

	}
	else {

		detach();

	}
	return true;
}

const String Displacement::TXT_DISPLACEMENT = "%s of displacement";
ItemSprite::Glowing* Displacement::BLUE = new ItemSprite::Glowing(0x66AAFF);

int Displacement::proc(Armor* armor, Char* attacker, Char* defender, int damage)
{
	//if (Dungeon.bossLevel()) {
	//	return damage;
	//}
	//
	//int level = armor.effectiveLevel();
	//int nTries = (level < 0 ? 1 : level + 1) * 5;
	//for (int i = 0; i < nTries; i++) {
	//	int pos = Random.Int(Level.LENGTH);
	//	if (Dungeon.visible[pos] && Level.passable[pos] && Actor.findChar(pos) == null) {
	//
	//		WandOfBlink.appear(defender, pos);
	//		Dungeon.level.press(pos, defender);
	//		Dungeon.observe();
	//
	//		break;
	//	}
	//}

	return damage;
}

String Displacement::name(const String& weaponName)
{
	return GameMath::format(TXT_DISPLACEMENT.c_str(), weaponName);
}

const String Entanglement::TXT_ENTANGLEMENT = "%s of entanglement";
ItemSprite::Glowing* Entanglement::GREEN = new ItemSprite::Glowing(0x448822);

int Entanglement::proc(Armor* armor, Char* attacker, Char* defender, int damage)
{
	//int level = Math.max(0, armor.effectiveLevel());
	//
	//if (Random.Int(4) == 0) {
	//
	//	Buff.prolong(defender, Roots.class, 5 - level / 5);
	//	Buff.affect(defender, Earthroot.Armor.class).level(5 * (level + 1));
	//	CellEmitter.bottom(defender.pos).start(EarthParticle.FACTORY, 0.05f, 8);
	//	Camera.main.shake(1, 0.4f);
	//
	//}

	return damage;
}

String Entanglement::name(const String& weaponName)
{
	return GameMath::format(TXT_ENTANGLEMENT.c_str(), weaponName);
}

const String AutoRepair::TXT_AUTO_REPAIR = "%s of auto-repair";
ItemSprite::Glowing* AutoRepair::GRAY = new ItemSprite::Glowing(0xCC8888);

int AutoRepair::proc(Armor* armor, Char* attacker, Char* defender, int damage)
{
	//if (defender instanceof Hero && Dungeon.gold >= armor.tier) {
	//	Dungeon.gold -= armor.tier;
	//	armor.polish();
	//}
	return damage;
}

String AutoRepair::name(const String& weaponName)
{
	return GameMath::format(TXT_AUTO_REPAIR.c_str(), weaponName);
}

ClothArmor::ClothArmor()
:Armor(1)
{
	name = "cloth armor";
	image = ItemSpriteSheet::ARMOR_CLOTH;
}

LeatherArmor::LeatherArmor()
:Armor(2)
{
	name = "leather armor";
	image = ItemSpriteSheet::ARMOR_LEATHER;
}

MailArmor::MailArmor()
:Armor(3)
{
	name = "mail armor";
	image = ItemSpriteSheet::ARMOR_MAIL;
}

ScaleArmor::ScaleArmor()
:Armor(4)
{
	name = "scale armor";
	image = ItemSpriteSheet::ARMOR_SCALE;
}

PlateArmor::PlateArmor()
:Armor(5)
{
	name = "plate armor";
	image = ItemSpriteSheet::ARMOR_PLATE;
}
