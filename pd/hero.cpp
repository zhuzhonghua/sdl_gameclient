#include "hero.h"
#include "attackindicator.h"
#include "dungeon.h"
#include "mobsprite.h"
#include "gamescene.h"

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

bool Hero::act()
{
	Char::act();

	if (paralysed) 
	{
		curAction = NULL;

		//spendAndNext(TICK);
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

		//if (curAction instanceof HeroAction.Move) {
		//
		//	return actMove((HeroAction.Move)curAction);
		//
		//}
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
	//if (cell == -1) {
	//	return false;
	//}
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
	//	curAction = new HeroAction.Move(cell);
	//	lastAction = null;
	//
	//}
	//
	//return act();
	return true;
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
