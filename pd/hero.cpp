#include "hero.h"
#include "attackindicator.h"
#include "dungeon.h"
#include "mobsprite.h"
#include "gamescene.h"
#include "heroaction.h"
#include "terrain.h"
#include "ring.h"
#include "checkedcell.h"
#include "glog.h"
#include "speck.h"

const std::string Hero::ATTACK = "attackSkill";
const std::string Hero::DEFENSE = "defenseSkill";
const std::string Hero::STRENGTH = "STR";
const std::string Hero::LEVEL = "lvl";
const std::string Hero::EXPERIENCE = "exp";


const std::string Hero::TXT_LEAVE = "One does not simply leave Pixel Dungeon.";

const std::string Hero::TXT_LEVEL_UP = "level up!";
const std::string Hero::TXT_NEW_LEVEL =
std::string("Welcome to level %d! Now you are healthier and more focused. ") +
std::string("It's easier for you to hit enemies and dodge their attacks.");

const std::string Hero::TXT_SOMETHING_ELSE = "There is something else here";
const std::string Hero::TXT_LOCKED_CHEST = "This chest is locked and you don't have matching key";
const std::string Hero::TXT_LOCKED_DOOR = "You don't have a matching key";
const std::string Hero::TXT_NOTICED_SMTH = "You noticed something";

const std::string Hero::TXT_WAIT = "...";
const std::string Hero::TXT_SEARCH = "search";
const float Hero::TIME_TO_REST = 1.0f;
const float Hero::TIME_TO_SEARCH = 2.0f;

const std::string Hero::TXT_YOU_NOW_HAVE = "You now have %s";

void Hero::preview(GamesInProgress::Info& info, Bundle* bundle)
{
	info.level = bundle->getInt(LEVEL);
}

int Hero::tier()
{
	return 0;//belongings.armor == null ? 0 : belongings.armor.tier;
}

boolean Hero::shoot(Char* enemy, MissileWeapon* wep)
{
	rangedWeapon = wep;
	boolean result = attack(enemy);
	rangedWeapon = NULL;

	return result;
}

void Hero::live()
{
	//Buff.affect(this, Regeneration.class);
	//Buff.affect(this, Hunger.class);
}

void Hero::updateAwareness()
{
	awareness = (float)(1 - std::pow(
		(heroClass.type() == HeroClass::E_ROGUE ? 0.85 : 0.90),
		(1 + std::min(lvl, 9)) * 0.5
		));
}

Hero::Hero()
:heroClass(HeroClass::ROGUE)
, subClass(HeroSubClass::NONE)
{
	lvl = 1;
	exp = 0;

	attackSkill = 10;
	defenseSkill = 5;

	restoreHealth = false;
	ready = false;

	curAction = NULL;
	lastAction = NULL;

	killerGlyph = NULL;

	rangedWeapon = NULL;

	weakened = false
}

void Hero::resurrect(int resetLevel)
{
	HP = HT;
	Dungeon::gold = 0;
	exp = 0;

	//belongings.resurrect(resetLevel);

	//live();
}

void Hero::Ready()
{
	sprite->Idle();
	curAction = NULL;
	ready = true;

	GameScene::ready();
}

bool Hero::actMove(HeroActionMove* action)
{
	if (getCloser(action->dst)) 
	{
		return true;
	}
	else
	{
		if (Dungeon::level->map[pos] == Terrain::SIGN) 
		{
			//Sign.read(pos);
		}
		Ready();

		return false;
	}
}

bool Hero::getCloser(int target)
{
	if (rooted) 
	{
		Camera::mainCamera->shake(1, 1.0f);
		return false;
	}

	int step = -1;

	if (Level::adjacent(pos, target)) 
	{
		if (Actor::findChar(target) == NULL) 
		{
			//if (Level.pit[target] && !flying && !Chasm.jumpConfirmed) {
			//	Chasm.heroJump(this);
			//	interrupt();
			//	return false;
			//}
			if (Level::passable[target] || Level::avoid[target]) 
			{
				step = target;
			}
		}
	}
	else 
	{
		int len = Level::LENGTH;
		std::vector<bool> p = Level::passable;
		std::vector<bool> v = Dungeon::level->visited;
		std::vector<bool> m = Dungeon::level->mapped;
		std::vector<bool> passable(len);
		for (int i = 0; i < len; i++) 
		{
			passable[i] = p[i] && (v[i] || m[i]);
		}

		step = Dungeon::findPath(this, pos, target, passable, Level::fieldOfView);
	}

	if (step != -1) 
	{
		int oldPos = pos;
		move(step);
		sprite->move(oldPos, pos);
		spend(1 / speed());

		return true;
	}
	else 
	{
		return false;
	}
}

bool Hero::act()
{
	Char::act();

	if (paralysed) 
	{
		curAction = NULL;

		spendAndNext(TICK);
		return false;
	}

	//checkVisibleMobs();
	AttackIndicator::updateState();

	if (curAction == NULL) 
	{
		//if (restoreHealth) {
		//	if (isStarving() || HP >= HT) {
		//		restoreHealth = false;
		//	}
		//	else {
		//		spend(TIME_TO_REST); next();
		//		return false;
		//	}
		//}

		Ready();
		return false;
	}
	else 
	{
		restoreHealth = false;

		ready = false;

		if (dynamic_cast<HeroActionMove*>(curAction) != NULL) 
		{		
			return actMove((HeroActionMove*)curAction);
		}
		//else
		//if (curAction instanceof HeroAction.Interact) {
		//
		//	return actInteract((HeroAction.Interact)curAction);
		//
		//}
		//else
		//if (curAction instanceof HeroAction.Buy) {
		//
		//	return actBuy((HeroAction.Buy)curAction);
		//
		//}
		//else
		//if (curAction instanceof HeroAction.PickUp) {
		//
		//	return actPickUp((HeroAction.PickUp)curAction);
		//
		//}
		//else
		//if (curAction instanceof HeroAction.OpenChest) {
		//
		//	return actOpenChest((HeroAction.OpenChest)curAction);
		//
		//}
		//else
		//if (curAction instanceof HeroAction.Unlock) {
		//
		//	return actUnlock((HeroAction.Unlock)curAction);
		//
		//}
		//else
		//if (curAction instanceof HeroAction.Descend) {
		//
		//	return actDescend((HeroAction.Descend)curAction);
		//
		//}
		//else
		//if (curAction instanceof HeroAction.Ascend) {
		//
		//	return actAscend((HeroAction.Ascend)curAction);
		//
		//}
		//else
		//if (curAction instanceof HeroAction.Attack) {
		//
		//	return actAttack((HeroAction.Attack)curAction);
		//
		//}
		//else
		//if (curAction instanceof HeroAction.Cook) {
		//
		//	return actCook((HeroAction.Cook)curAction);
		//
		//}
	}

	return false;
}

bool Hero::handle(int cell)
{
	if (cell == -1) 
	{
		return false;
	}
	//
	//Char ch;
	//Heap heap;
	//
	//if (Dungeon.level.map[cell] == Terrain.ALCHEMY && cell != pos) {
	//
	//	curAction = new HeroAction.Cook(cell);
	//
	//}
	//else if (Level.fieldOfView[cell] && (ch = Actor.findChar(cell)) instanceof Mob) {
	//
	//	if (ch instanceof NPC) {
	//		curAction = new HeroAction.Interact((NPC)ch);
	//	}
	//	else {
	//		curAction = new HeroAction.Attack(ch);
	//	}
	//
	//}
	//else if (Level.fieldOfView[cell] && (heap = Dungeon.level.heaps.get(cell)) != null && heap.type != Heap.Type.HIDDEN) {
	//
	//	switch (heap.type) {
	//	case HEAP:
	//		curAction = new HeroAction.PickUp(cell);
	//		break;
	//	case FOR_SALE:
	//		curAction = heap.size() == 1 && heap.peek().price() > 0 ?
	//			new HeroAction.Buy(cell) :
	//			new HeroAction.PickUp(cell);
	//		break;
	//	default:
	//		curAction = new HeroAction.OpenChest(cell);
	//	}
	//
	//}
	//else if (Dungeon.level.map[cell] == Terrain.LOCKED_DOOR || Dungeon.level.map[cell] == Terrain.LOCKED_EXIT) {
	//
	//	curAction = new HeroAction.Unlock(cell);
	//
	//}
	//else if (cell == Dungeon.level.exit) {
	//
	//	curAction = new HeroAction.Descend(cell);
	//
	//}
	//else if (cell == Dungeon.level.entrance) {
	//
	//	curAction = new HeroAction.Ascend(cell);
	//
	//}
	//else  {
	//
		curAction = new HeroActionMove(cell);
		lastAction = NULL;
	//
	//}
	//
	return act();
}

int Hero::VisibleEnemies()
{
	return visibleEnemies.size();
}

Mob* Hero::visibleEnemy(int index)
{
	int count = 0;
	int idx = index % visibleEnemies.size();
	for (std::list<Mob*>::iterator itr = visibleEnemies.begin();
		itr != visibleEnemies.end(); itr++)
	{
		if (count == idx) return *itr;
		count++;
	}
	return NULL;
}

void Hero::resume()
{
	//curAction = lastAction;
	//lastAction = null;
	//act();
}

void Hero::spendAndNext(float time)
{
	busy();
	spend(time);
	next();
}

void Hero::move(int step)
{
	Char::move(step);

	if (!flying) 
	{
		if (Level::water[pos]) 
		{
			//Sample.INSTANCE.play(Assets.SND_WATER, 1, 1, Random.Float(0.8f, 1.25f));
		}
		else 
		{
			//Sample.INSTANCE.play(Assets.SND_STEP);
		}
		Dungeon::level->press(pos, this);
	}
}

void Hero::interrupt()
{
	if (isAlive() && curAction != NULL && curAction->dst != pos) 
	{
		lastAction = curAction;
	}
	curAction = NULL;
}

bool Hero::search(bool intentional)
{
	bool smthFound = false;

	int positive = 0;
	int negative = 0;
	std::set<Buff*> result = buffSet(RingOfDetection::Detection::className());
	//for (Buff buff : buffs(RingOfDetection.Detection.class)) {
	for (std::set<Buff*>::iterator itr = result.begin(); itr != result.end(); itr++)
	{
		Buff* buff = *itr;
		int bonus = ((RingOfDetection::Detection*)buff)->level;
		if (bonus > positive) 
		{
			positive = bonus;
		}
		else if (bonus < 0) 
		{
			negative += bonus;
		}
	}
	int distance = 1 + positive + negative;

	float level = intentional ? (2 * awareness - awareness * awareness) : awareness;
	if (distance <= 0) 
	{
		level /= 2 - distance;
		distance = 1;
	}

	int cx = pos % Level::WIDTH;
	int cy = pos / Level::WIDTH;
	int ax = cx - distance;
	if (ax < 0) 
	{
		ax = 0;
	}
	int bx = cx + distance;
	if (bx >= Level::WIDTH) 
	{
		bx = Level::WIDTH - 1;
	}
	int ay = cy - distance;
	if (ay < 0) {
		ay = 0;
	}
	int by = cy + distance;
	if (by >= Level::HEIGHT) 
	{
		by = Level::HEIGHT - 1;
	}

	for (int y = ay; y <= by; y++) 
	{
		for (int x = ax, p = ax + y * Level::WIDTH; x <= bx; x++, p++) 
		{
			if (Dungeon::visible[p]) 
			{
				if (intentional) 
				{
					sprite->parent->addToBack(new CheckedCell(p));
				}

				if (Level::secret[p] && (intentional || Random::Float() < level))
				{
					int oldValue = Dungeon::level->map[p];

					//GameScene::discoverTile(p, oldValue);

					Level::set(p, Terrain::discover(oldValue));

					GameScene::updateMap(p);

					//ScrollOfMagicMapping.discover(p);

					smthFound = true;
				}

				if (intentional) 
				{
					//Heap heap = Dungeon.level.heaps.get(p);
					//if (heap != null && heap.type == Type.HIDDEN) {
					//	heap.open(this);
					//	smthFound = true;
					//}
				}
			}
		}
	}


	if (intentional) 
	{
		sprite->showStatus(CharSprite::DEFAULT, TXT_SEARCH);
		//sprite->operate(pos);
		if (smthFound) 
		{
			spendAndNext(Random::Float() < level ? TIME_TO_SEARCH : TIME_TO_SEARCH * 2);
		}
		else 
		{
			spendAndNext(TIME_TO_SEARCH);
		}

	}

	if (smthFound) {
		GLog::w(TXT_NOTICED_SMTH.c_str());
		//Sample.INSTANCE.play(Assets.SND_SECRET);
		interrupt();
	}

	return smthFound;
}

void Hero::earnExp(int exp)
{
	this->exp += exp;

	bool levelUp = false;
	while (this->exp >= maxExp()) {
		this->exp -= maxExp();
		lvl++;

		HT += 5;
		HP += 5;
		attackSkill++;
		defenseSkill++;

		if (lvl < 10) {
			updateAwareness();
		}

		levelUp = true;
	}

	if (levelUp) {

		GLog::p(TXT_NEW_LEVEL.c_str(), lvl);
		sprite->showStatus(CharSprite::POSITIVE, TXT_LEVEL_UP);
		//Sample.INSTANCE.play(Assets.SND_LEVELUP);

		//Badges.validateLevelReached();
	}

	if (subClass == HeroSubClass::WARLOCK) {

		int value = std::min(HT - HP, 1 + (Dungeon::depth - 1) / 5);
		if (value > 0) {
			HP += value;
			sprite->emitter()->burst(Speck::factory(Speck::HEALING), 1);
		}

		//((Hunger)buff(Hunger.class)).satisfy(10);
	}
}

void Hero::spend(float time)
{
	int hasteLevel = 0;
	std::set<Buff*> re;
	Buffs("RingOfHaste::Haste", re);
	for (std::set<Buff*>::iterator itr = re.begin();
		itr != re.end(); itr++){
	//for (Buff buff : buffs(RingOfHaste.Haste.class)) {
		Buff* buff = *itr;
		hasteLevel += ((RingOfHaste::Haste*)buff)->level;
	}
	Char::spend(hasteLevel == 0 ? time : (float)(time * std::pow(1.1, -hasteLevel)));
}
