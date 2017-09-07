#include "gamescene.h"
#include "floatingtext.h"
#include "pixeldungeon.h"
#include "dungeon.h"
#include "skinnedblock.h"
#include "dungeontilemap.h"
#include "fogofwar.h"
#include "herosprite.h"
#include "cellselector.h"
#include "toolbar.h"
#include "gamelog.h"
#include "busyindicator.h"
#include "interlevelscene.h"
#include "wndstory.h"
#include "statistics.h"
#include "glog.h"
#include "statuspane.h"
#include "regularlevel.h"
#include "blob.h"
#include "blobemitter.h"
#include "mob.h"
#include "healthindicator.h"
#include "attackindicator.h"

const std::string GameScene::TXT_WELCOME = "Welcome to the level %d of Pixel Dungeon!";
const std::string GameScene::TXT_WELCOME_BACK = "Welcome back to the level %d of Pixel Dungeon!";
const std::string GameScene::TXT_NIGHT_MODE = "Be cautious, since the dungeon is even more dangerous at night!";
const std::string GameScene::TXT_CHASM = "Your steps echo across the dungeon.";
const std::string GameScene::TXT_WATER = "You hear the water splashing around you.";
const std::string GameScene::TXT_GRASS = "The smell of vegetation is thick in the air.";
const std::string GameScene::TXT_SECRETS = "The atmosphere hints that this floor hides many secrets.";

GameScene* GameScene::scene;
CellSelector* GameScene::cellSelector;

void GameScene::addBlobSprite(Blob* gas)
{
	if (gas->emitter == NULL) 
	{
		gases->add(new BlobEmitter(gas));
	}
}

void GameScene::addMobSprite(Mob* mob)
{
	CharSprite* sprite = mob->Sprite();
	sprite->visible = Dungeon::visible[mob->pos];
	mobs->add(sprite);
	sprite->link(mob);
}

void GameScene::brightness(bool value)
{
	water->rm = water->gm = water->bm =
		tiles->rm = tiles->gm = tiles->bm =
		value ? 1.5f : 1.0f;
	if (value) 
	{
		fog->am = +2.0f;
		fog->aa = -1.0f;
	}
	else 
	{
		fog->am = +1.0f;
		fog->aa = 0.0f;
	}
}

void GameScene::init()
{
	//Music.INSTANCE.play(Assets.TUNE, true);
	//Music.INSTANCE.volume(1f);

	PixelDungeon::lastClass(Dungeon::hero->heroClass.ordinal());

	PixelScene::init();
	Camera::mainCamera->zoom(defaultZoom + PixelDungeon::zoom());

	scene = this;

	terrain = new Group();
	add(terrain);

	water = new SkinnedBlock(
		Level::WIDTH * DungeonTilemap::SIZE,
		Level::HEIGHT * DungeonTilemap::SIZE,
		Dungeon::level->waterTex());
	terrain->add(water);
	
	ripples = new Group();
	terrain->add(ripples);

	tiles = new DungeonTilemap();
	terrain->add(tiles);

	Dungeon::level->addVisuals(this);

	plants = new Group();
	add(plants);

	//int size = Dungeon.level.plants.size();
	//for (int i = 0; i < size; i++) {
	//	addPlantSprite(Dungeon.level.plants.valueAt(i));
	//}

	heaps = new Group();
	add(heaps);

	//size = Dungeon.level.heaps.size();
	//for (int i = 0; i < size; i++) {
	//	addHeapSprite(Dungeon.level.heaps.valueAt(i));
	//}

	emitters = new Group();
	effects = new Group();
	emoicons = new Group();
	
	mobs = new Group();
	add(mobs);

	for (std::set<Mob*>::iterator itr = Dungeon::level->mobs.begin();
		itr != Dungeon::level->mobs.end(); itr++)
	{
		Mob* mob = *itr;
		addMobSprite(mob);
		if (Statistics::amuletObtained) 
		{
			mob->beckon(Dungeon::hero->pos);
		}
	}
	add(emitters);
	add(effects);
	
	gases = new Group();
	add(gases);

	for (std::map<std::string, Blob*>::iterator itr = Dungeon::level->blobs.begin();
		itr != Dungeon::level->blobs.end(); itr++)
	{
		Blob* blob = itr->second;
		blob->emitter = NULL;
		addBlobSprite(blob);
	}

	fog = new FogOfWar(Level::WIDTH, Level::HEIGHT);
	fog->updateVisibility(Dungeon::visible, Dungeon::level->visited, Dungeon::level->mapped);
	add(fog);
	
	brightness(PixelDungeon::brightness());
	
	spells = new Group();
	add(spells);
	
	statuses = new Group();
	add(statuses);
	
	add(emoicons);
	//

	hero = new HeroSprite();
	hero->place(Dungeon::hero->pos);
	hero->updateArmor();
	mobs->add(hero);

	add(new HealthIndicator());

	add(cellSelector = new CellSelector(tiles));

	StatusPane* sb = new StatusPane();
	sb->cameraf = uiCamera;
	sb->setSize(uiCamera->width, 0);
	add(sb);

	toolbar = new Toolbar();
	toolbar->cameraf = uiCamera;
	toolbar->setRect(0, uiCamera->height - toolbar->height(), uiCamera->width, toolbar->height());
	add(toolbar);

	AttackIndicator* attack = new AttackIndicator();
	attack->cameraf = uiCamera;
	attack->setPos(
		uiCamera->width - attack->width(),
		toolbar->top() - attack->height());
	add(attack);

	log = new GameLog();
	log->cameraf = uiCamera;
	log->setRect(0, toolbar->top(), attack->left(), 0);
	add(log);

	busy = new BusyIndicator();
	busy->cameraf = uiCamera;
	busy->x = 1;
	busy->y = sb->bottom() + 1;
	add(busy);

	switch (InterlevelScene::mode) 
	{
	case InterlevelScene::RESURRECT:
		//WandOfBlink.appear(Dungeon.hero, Dungeon.level.entrance);
		//new Flare(8, 32).color(0xFFFF66, true).show(hero, 2f);
		break;
	case InterlevelScene::RETURN:
		//WandOfBlink.appear(Dungeon.hero, Dungeon.hero.pos);
		break;
	case InterlevelScene::FALL:
		//Chasm.heroLand();
		break;
	case InterlevelScene::DESCEND:
		switch (Dungeon::depth) {
		case 1:
			WndStory::showChapter(WndStory::ID_SEWERS);
			break;
		case 6:
			WndStory::showChapter(WndStory::ID_PRISON);
			break;
		case 11:
			WndStory::showChapter(WndStory::ID_CAVES);
			break;
		case 16:
			WndStory::showChapter(WndStory::ID_METROPOLIS);
			break;
		case 22:
			WndStory::showChapter(WndStory::ID_HALLS);
			break;
		}
		if (Dungeon::hero->isAlive() && Dungeon::depth != 22) 
		{
			Badges::validateNoKilling();
		}
		break;
	default:
		break;
	}

	//ArrayList<Item> dropped = Dungeon.droppedItems.get(Dungeon.depth);
	//if (dropped != null) {
	//	for (Item item : dropped) {
	//		int pos = Dungeon.level.randomRespawnCell();
	//		if (item instanceof Potion) {
	//			((Potion)item).shatter(pos);
	//		}
	//		else if (item instanceof Plant.Seed) {
	//			Dungeon.level.plant((Plant.Seed)item, pos);
	//		}
	//		else {
	//			Dungeon.level.drop(item, pos);
	//		}
	//	}
	//	Dungeon.droppedItems.remove(Dungeon.depth);
	//}

	Camera::mainCamera->target = hero;

	if (InterlevelScene::mode != InterlevelScene::Mode::NONE) 
	{
		if (Dungeon::depth < Statistics::deepestFloor) 
		{
			GLog::h(TXT_WELCOME_BACK, Dungeon::depth);
		}
		else 
		{
			GLog::h(TXT_WELCOME, Dungeon::depth);
			//Sample.INSTANCE.play(Assets.SND_DESCEND);
		}
		switch (Dungeon::level->feeling) 
		{
		case Level::CHASM:
			GLog::w(TXT_CHASM);
			break;
		case Level::WATER:
			GLog::w(TXT_WATER);
			break;
		case Level::GRASS:
			GLog::w(TXT_GRASS);
			break;
		default:
			break;
		}
		if (dynamic_cast<RegularLevel*>(Dungeon::level) != NULL &&
			((RegularLevel*)Dungeon::level)->secretDoors > Random::IntRange(3, 4)) 
		{
			GLog::w(TXT_SECRETS);
		}
		if (Dungeon::nightMode && !Dungeon::bossLevel()) 
		{
			GLog::w(TXT_NIGHT_MODE);
		}

		InterlevelScene::mode = InterlevelScene::Mode::NONE;

		fadeIn();
	}
}

void GameScene::destroy()
{
	scene = NULL;
	Badges::saveGlobal();

	PixelScene::destroy();
}

Emitter* GameScene::emitter()
{
	if (scene != NULL) 
	{
		Emitter* emitter = (Emitter*)scene->emitters->recycle("Emitter");
		if (emitter == NULL)
		{
			emitter = new Emitter();
			scene->emitters->add(emitter);
		}
		emitter->revive();
		return emitter;
	}
	else 
	{
		return NULL;
	}
}

FloatingText* GameScene::status()
{
	if (scene == NULL) return NULL;

	FloatingText* txt = (FloatingText*)scene->statuses->recycle("FloatingText");
	if (txt == NULL)
	{
		txt = new FloatingText();
	}
	
	return txt;
}

Ripple* GameScene::ripple(int pos)
{
	Ripple* ripple = (Ripple*)scene->ripples->recycle("Ripple");
	if (ripple == NULL)
	{
		ripple = new Ripple();
		scene->ripples->add(ripple);
	}
	ripple->reset(pos);
	return ripple;
}

void GameScene::effect(Visual* eff)
{
	scene->effects->add(eff);
}

void GameScene::afterObserve()
{
	if (scene != NULL) 
	{
		scene->fog->updateVisibility(Dungeon::visible, Dungeon::level->visited, Dungeon::level->mapped);

		for (std::set<Mob*>::iterator itr = Dungeon::level->mobs.begin();
			itr != Dungeon::level->mobs.end(); itr++)
		{
			Mob* mob = *itr;
			mob->sprite->visible = Dungeon::visible[mob->pos];
		}
	}
}

void GameScene::ready()
{
	//selectCell(defaultCellListener);
	//QuickSlot.cancel();
}

bool GameScene::cancel()
{
	//if (Dungeon.hero.curAction != null || Dungeon.hero.restoreHealth) {
	//
	//	Dungeon.hero.curAction = null;
	//	Dungeon.hero.restoreHealth = false;
	//	return true;
	//
	//}
	//else {
	//
	//	return cancelCellSelector();
	//
	//}
	return true;
}

void GameScene::addMob(Mob* mob, float delay)
{
	Dungeon::level->mobs.insert(mob);
	Actor::addDelayed(mob, delay);
	Actor::occupyCell(mob);
	scene->addMobSprite(mob);
}

void GameScene::addMob(Mob* mob)
{
	Dungeon::level->mobs.insert(mob);
	Actor::add(mob);
	Actor::occupyCell(mob);
	scene->addMobSprite(mob);
}
