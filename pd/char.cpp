#include "char.h"
#include "terrain.h"
#include "dungeon.h"
#include "buff.h"
#include "util.h"
#include "charsprite.h"
#include "glog.h"

const std::string Char::TXT_HIT = "%s hit %s";
const std::string Char::TXT_KILL = "%s killed you...";
const std::string Char::TXT_DEFEAT = "%s defeated %s";

const std::string Char::TXT_YOU_MISSED = "%s %s your attack";
const std::string Char::TXT_SMB_MISSED = "%s %s %s's attack";

const std::string Char::TXT_OUT_OF_PARALYSIS = "The pain snapped %s out of paralysis";

const std::string Char::POS = "pos";
const std::string Char::TAG_HP = "HP";
const std::string Char::TAG_HT = "HT";
const std::string Char::BUFFS = "buffs";

std::set<std::string> Char::EMPTY;

Char::Char()
{
	paralysed = false;
	rooted = false;
	flying = false;
	invisible = 0;

	viewDistance = 8;

	baseSpeed = 1;
	name = "mob";
	pos = 0;
	sprite = NULL;
}

bool Char::act()
{
	Dungeon::level->updateFieldOfView(this);
	return false;
}

void Char::spend(float time)
{
	float timeScale = 1.0f;
	//if (buff(Slow.class) != null) {
	//	timeScale *= 0.5f;
	//}
	//if (buff(Speed.class) != null) {
	//	timeScale *= 2.0f;
	//}

	Actor::spend(time / timeScale);
}

void Char::onRemove()
{
	std::vector<Buff*> arr(buffs.begin(), buffs.end());
	for (int i = 0; i < arr.size(); i++)
	{
		arr[i]->detach();
	}
}

void Char::storeInBundle(Bundle* bundle)
{
	Actor::storeInBundle(bundle);

	bundle->put(POS, pos);
	bundle->put(TAG_HP, HP);
	bundle->put(TAG_HT, HT);

	// 
	std::vector<Bundlable*> temp;
	for (std::set<Buff*>::iterator itr = buffs.begin();
		itr != buffs.end(); itr++)
	{
		temp.push_back(*itr);
	}
	bundle->put(BUFFS, temp);
}

namespace{
	static Bundlable* getBuffInstance()
	{
		return new Buff();
	}

}
void Char::restoreFromBundle(Bundle* bundle)
{
	Actor::restoreFromBundle(bundle);

	pos = bundle->getInt(POS);
	HP = bundle->getInt(TAG_HP);
	HT = bundle->getInt(TAG_HT);

	std::vector<Bundlable*> result;
	bundle->getCollection(BUFFS, result, getBuffInstance);

	for (int i = 0; i < result.size();i++)
	{
		((Buff*)result[i])->attachTo(this);
	}
}

HashSet<std::string>& Char::resistances()
{
	return EMPTY;
}

HashSet<std::string>& Char::immunities()
{
	return EMPTY;
}

bool Char::immunitiesContain(const std::string& cls)
{
	return EMPTY.find(cls) != EMPTY.end();
}

void Char::add(Buff* buff)
{
	buffs.insert(buff);
	Actor::add(buff);

	//if (sprite != null) {
	//	if (buff instanceof Poison) {
	//
	//		CellEmitter.center(pos).burst(PoisonParticle.SPLASH, 5);
	//		sprite.showStatus(CharSprite.NEGATIVE, "poisoned");
	//
	//	}
	//	else if (buff instanceof Amok) {
	//
	//		sprite.showStatus(CharSprite.NEGATIVE, "amok");
	//
	//	}
	//	else if (buff instanceof Slow) {
	//
	//		sprite.showStatus(CharSprite.NEGATIVE, "slowed");
	//
	//	}
	//	else if (buff instanceof MindVision) {
	//
	//		sprite.showStatus(CharSprite.POSITIVE, "mind");
	//		sprite.showStatus(CharSprite.POSITIVE, "vision");
	//
	//	}
	//	else if (buff instanceof Paralysis) {
	//
	//		sprite.add(CharSprite.State.PARALYSED);
	//		sprite.showStatus(CharSprite.NEGATIVE, "paralysed");
	//
	//	}
	//	else if (buff instanceof Terror) {
	//
	//		sprite.showStatus(CharSprite.NEGATIVE, "frightened");
	//
	//	}
	//	else if (buff instanceof Roots) {
	//
	//		sprite.showStatus(CharSprite.NEGATIVE, "rooted");
	//
	//	}
	//	else if (buff instanceof Cripple) {
	//
	//		sprite.showStatus(CharSprite.NEGATIVE, "crippled");
	//
	//	}
	//	else if (buff instanceof Bleeding) {
	//
	//		sprite.showStatus(CharSprite.NEGATIVE, "bleeding");
	//
	//	}
	//	else if (buff instanceof Vertigo) {
	//
	//		sprite.showStatus(CharSprite.NEGATIVE, "dizzy");
	//
	//	}
	//	else if (buff instanceof Sleep) {
	//		sprite.idle();
	//	}
	//
	//	else if (buff instanceof Burning) {
	//		sprite.add(CharSprite.State.BURNING);
	//	}
	//	else if (buff instanceof Levitation) {
	//		sprite.add(CharSprite.State.LEVITATING);
	//	}
	//	else if (buff instanceof Frost) {
	//		sprite.add(CharSprite.State.FROZEN);
	//	}
	//	else if (buff instanceof Invisibility) {
	//		if (!(buff instanceof Shadows)) {
	//			sprite.showStatus(CharSprite.POSITIVE, "invisible");
	//		}
	//		sprite.add(CharSprite.State.INVISIBLE);
	//	}
	//}
}

void Char::remove(Buff* buff)
{
	std::set<Buff*>::iterator itr = buffs.find(buff);
	if (itr != buffs.end()) buffs.erase(itr);
	
	Actor::remove(buff);

	//if (buff instanceof Burning) {
	//	sprite.remove(CharSprite.State.BURNING);
	//}
	//else if (buff instanceof Levitation) {
	//	sprite.remove(CharSprite.State.LEVITATING);
	//}
	//else if (buff instanceof Invisibility && invisible <= 0) {
	//	sprite.remove(CharSprite.State.INVISIBLE);
	//}
	//else if (buff instanceof Paralysis) {
	//	sprite.remove(CharSprite.State.PARALYSED);
	//}
	//else if (buff instanceof Frost) {
	//	sprite.remove(CharSprite.State.FROZEN);
	//}
}

void Char::remove(const std::string& buffClass)
{
	std::set<Buff*> re;
	Buffs(buffClass, re);
	for (std::set<Buff*>::iterator itr = re.begin();
		itr != re.end(); itr++)
	{
		remove(*itr);
	}
}

bool Char::attack(Char* enemy)
{
	bool visibleFight = Dungeon::visible[pos] || Dungeon::visible[enemy->pos];

	//if (hit(this, enemy, false)) 
	//{
	//
	//	if (visibleFight) {
	//		GLog.i(TXT_HIT, name, enemy.name);
	//	}
	//
	//	// FIXME
	//	int dr = this instanceof Hero && ((Hero)this).rangedWeapon != null && ((Hero)this).subClass == HeroSubClass.SNIPER ? 0 :
	//		Random.IntRange(0, enemy.dr());
	//
	//	int dmg = damageRoll();
	//	int effectiveDamage = Math.max(dmg - dr, 0);
	//
	//	effectiveDamage = attackProc(enemy, effectiveDamage);
	//	effectiveDamage = enemy.defenseProc(this, effectiveDamage);
	//	enemy.damage(effectiveDamage, this);
	//
	//	if (visibleFight) {
	//		Sample.INSTANCE.play(Assets.SND_HIT, 1, 1, Random.Float(0.8f, 1.25f));
	//	}
	//
	//	if (enemy == Dungeon.hero) {
	//		Dungeon.hero.interrupt();
	//		if (effectiveDamage > enemy.HT / 4) {
	//			Camera.main.shake(GameMath.gate(1, effectiveDamage / (enemy.HT / 4), 5), 0.3f);
	//		}
	//	}
	//
	//	enemy.sprite.bloodBurstA(sprite.center(), effectiveDamage);
	//	enemy.sprite.flash();
	//
	//	if (!enemy.isAlive() && visibleFight) {
	//		if (enemy == Dungeon.hero) {
	//
	//			if (Dungeon.hero.killerGlyph != null) {
	//
	//				// FIXME
	//				//	Dungeon.fail( Utils.format( ResultDescriptions.GLYPH, Dungeon.hero.killerGlyph.name(), Dungeon.depth ) );
	//				//	GLog.n( TXT_KILL, Dungeon.hero.killerGlyph.name() );
	//
	//			}
	//			else {
	//				if (Bestiary.isBoss(this)) {
	//					Dungeon.fail(Utils.format(ResultDescriptions.BOSS, name, Dungeon.depth));
	//				}
	//				else {
	//					Dungeon.fail(Utils.format(ResultDescriptions.MOB,
	//						Utils.indefinite(name), Dungeon.depth));
	//				}
	//
	//				GLog.n(TXT_KILL, name);
	//			}
	//
	//		}
	//		else {
	//			GLog.i(TXT_DEFEAT, name, enemy.name);
	//		}
	//	}
	//
	//	return true;
	//
	//}
	//else {
	//
	//	if (visibleFight) {
	//		String defense = enemy.defenseVerb();
	//		enemy.sprite.showStatus(CharSprite.NEUTRAL, defense);
	//		if (this == Dungeon.hero) {
	//			GLog.i(TXT_YOU_MISSED, enemy.name, defense);
	//		}
	//		else {
	//			GLog.i(TXT_SMB_MISSED, enemy.name, defense, name);
	//		}
	//
	//		Sample.INSTANCE.play(Assets.SND_MISS);
	//	}
	//
		return false;
	//
	//}
}

bool Char::hit(Char* attacker, Char* defender, bool magic)
{
	float acuRoll = Random::Float(attacker->attackSkill(defender));
	float defRoll = Random::Float(defender->defenseSkill(attacker));
	return (magic ? acuRoll * 2 : acuRoll) >= defRoll;
}

float Char::speed()
{
	return /*buff(Cripple.class) == null ? baseSpeed :*/ baseSpeed * 0.5f;
}

void Char::damage(int dmg, Object* src)
{
	if (HP <= 0) {
		return;
	}

	Buff::detach(this, "Frost");
	
	if (immunities().contains(src->getObject())) {
		dmg = 0;
	}
	else if (resistances().contains(src->getObject())) {
		dmg = Random::IntRange(0, dmg);
	}
	
	if (buff("BuffParalysis") != NULL) {
		if (Random::Int(dmg) >= Random::Int(HP)) {
			Buff::detach(this, "BuffParalysis");
			if (Dungeon::visible[pos]) {
				GLog::i(TXT_OUT_OF_PARALYSIS.c_str(), name.c_str());
			}
		}
	}
	
	HP -= dmg;
	if (dmg > 0 || dynamic_cast<Char*>(src)) {
		std::stringstream ss;
		ss << dmg;
		sprite->showStatus(HP > HT / 2 ?
			CharSprite::WARNING :
			CharSprite::NEGATIVE,
			ss.str());
	}
	if (HP <= 0) {
		die(src);
	}
}

void Char::destroy()
{
	HP = 0;
	Actor::remove(this);
	Actor::freeCell(pos);
}

void Char::die(Object* src)
{
	destroy();
	sprite->Die();
}

void Char::Buffs(const std::string& c, std::set<Buff*>& re)
{
	for (std::set<Buff*>::iterator itr = buffs.begin();
		itr != buffs.end(); itr++)
	{
		if ((*itr)->getClassName().compare(c) == 0)
		{
			re.insert(*itr);
		}
	}
}

Buff* Char::buff(const std::string& c)
{
	for (std::set<Buff*>::iterator itr = buffs.begin();
		itr != buffs.end(); itr++)
	{
		if ((*itr)->getClassName().compare(c) == 0)
		{
			return *itr;
		}
	}
	return NULL;
}

std::set<Buff*> Char::buffSet(const std::string& c)
{
	std::set<Buff*> filter;
	for (std::set<Buff*>::iterator itr = buffs.begin();
		itr != buffs.end(); itr++)
	{
		if ((*itr)->getClassName().compare(c) == 0)
		{
			filter.insert(*itr);
		}
	}
	return filter;
}

bool Char::isCharmedBy(Char* ch)
{
	int chID = ch->Id();
	//for (Buff b : buffs) {
	//	if (b instanceof Charm && ((Charm)b).object == chID) {
	//		return true;
	//	}
	//}
	return false;
}

void Char::updateSpriteState()
{
	for (std::set<Buff*>::iterator itr = buffs.begin();
		itr != buffs.end(); itr++)
	{
		Buff* buff = *itr;
		//if (buff instanceof Burning) {
		//	sprite.add(CharSprite.State.BURNING);
		//}
		//else if (buff instanceof Levitation) {
		//	sprite.add(CharSprite.State.LEVITATING);
		//}
		//else if (buff instanceof Invisibility) {
		//	sprite.add(CharSprite.State.INVISIBLE);
		//}
		//else if (buff instanceof Paralysis) {
		//	sprite.add(CharSprite.State.PARALYSED);
		//}
		//else if (buff instanceof Frost) {
		//	sprite.add(CharSprite.State.FROZEN);
		//}
		//else if (buff instanceof Light) {
		//	sprite.add(CharSprite.State.ILLUMINATED);
		//}
	}
}

void Char::move(int step)
{
	if (Level::adjacent(step, pos) /*&& buff(Vertigo.class) != null*/) 
	{
		step = pos + Level::NEIGHBOURS8[Random::Int(8)];
		if (!(Level::passable[step] || Level::avoid[step]) || Actor::findChar(step) != NULL) 
		{
			return;
		}
	}
	
	if (Dungeon::level->map[pos] == Terrain::OPEN_DOOR) 
	{
		//Door::leave(pos);
	}
	
	pos = step;
	
	if (flying && Dungeon::level->map[pos] == Terrain::DOOR) 
	{
		//Door.enter(pos);
	}
	
	if (this != Dungeon::hero) 
	{
		sprite->visible = Dungeon::visible[pos];
	}
}

int Char::distance(Char* other)
{
	return Level::distance(pos, other->pos);
}
