#include "level.h"
#include "util.h"
#include "dungeon.h"
#include "terrain.h"
#include "mob.h"
#include "painter.h"

std::vector<bool> Level::fieldOfView(LENGTH);

std::vector<bool> Level::passable(LENGTH);
std::vector<bool> Level::losBlocking(LENGTH);
std::vector<bool> Level::flamable(LENGTH);
std::vector<bool> Level::secret(LENGTH);
std::vector<bool> Level::solid(LENGTH);
std::vector<bool> Level::avoid(LENGTH);
std::vector<bool> Level::water(LENGTH);
std::vector<bool> Level::pit(LENGTH);

std::vector<bool> Level::discoverable(LENGTH);

const int Level::NEIGHBOURS4[] = { -Level::WIDTH, +1, +Level::WIDTH, -1 };
const int Level::NEIGHBOURS8[] = { +1, -1, +Level::WIDTH, -Level::WIDTH, +1 + Level::WIDTH, +1 - Level::WIDTH, -1 + Level::WIDTH, -1 - Level::WIDTH };
const int Level::NEIGHBOURS9[] = { 0, +1, -1, +Level::WIDTH, -Level::WIDTH, +1 + Level::WIDTH, +1 - Level::WIDTH, -1 + Level::WIDTH, -1 - Level::WIDTH };

const float Level::TIME_TO_RESPAWN = 50;
const std::string Level::TXT_HIDDEN_PLATE_CLICKS = "A hidden pressure plate clicks!";

bool Level::resizingNeeded;
int Level::loadedMapSize;

bool Level::pitRoomNeeded = false;
bool Level::weakFloorCreated = false;

const std::string Level::MAP = "map";
const std::string Level::VISITED = "visited";
const std::string Level::MAPPED = "mapped";
const std::string Level::ENTRANCE = "entrance";
const std::string Level::EXIT = "exit";
const std::string Level::HEAPS = "heaps";
const std::string Level::PLANTS = "plants";
const std::string Level::MOBS = "mobs";
const std::string Level::BLOBS = "blobs";

void Level::buildFlagMaps()
{
	for (int i = 0; i < LENGTH; i++) 
	{
		int flags = Terrain::flags[map[i]];
		passable[i] = (flags & Terrain::PASSABLE) != 0;
		losBlocking[i] = (flags & Terrain::LOS_BLOCKING) != 0;
		flamable[i] = (flags & Terrain::FLAMABLE) != 0;
		secret[i] = (flags & Terrain::SECRET) != 0;
		solid[i] = (flags & Terrain::SOLID) != 0;
		avoid[i] = (flags & Terrain::AVOID) != 0;
		water[i] = (flags & Terrain::LIQUID) != 0;
		pit[i] = (flags & Terrain::PIT) != 0;
	}

	int lastRow = LENGTH - WIDTH;
	for (int i = 0; i < WIDTH; i++) 
	{
		passable[i] = avoid[i] = false;
		passable[lastRow + i] = avoid[lastRow + i] = false;
	}
	for (int i = WIDTH; i < lastRow; i += WIDTH) 
	{
		passable[i] = avoid[i] = false;
		passable[i + WIDTH - 1] = avoid[i + WIDTH - 1] = false;
	}

	for (int i = WIDTH; i < LENGTH - WIDTH; i++) 
	{
		if (water[i]) 
		{
			map[i] = getWaterTile(i);
		}

		if (pit[i]) 
		{
			if (!pit[i - WIDTH]) 
			{
				int c = map[i - WIDTH];
				if (c == Terrain::EMPTY_SP || c == Terrain::STATUE_SP) 
				{
					map[i] = Terrain::CHASM_FLOOR_SP;
				}
				else if (water[i - WIDTH]) 
				{
					map[i] = Terrain::CHASM_WATER;
				}
				else if ((Terrain::flags[c] & Terrain::UNSTITCHABLE) != 0) 
				{
					map[i] = Terrain::CHASM_WALL;
				}
				else 
				{
					map[i] = Terrain::CHASM_FLOOR;
				}
			}
		}
	}
}

int Level::getWaterTile(int pos)
{
	int t = Terrain::WATER_TILES;
	for (int j = 0; j < sizeof(NEIGHBOURS4)/sizeof(int); j++) 
	{
		if ((Terrain::flags[map[pos + NEIGHBOURS4[j]]] & Terrain::UNSTITCHABLE) != 0) 
		{
			t += 1 << j;
		}
	}
	return t;
}

void Level::cleanWalls()
{
	for (int i = 0; i < LENGTH; i++) 
	{
		bool d = false;

		for (int j = 0; j < sizeof(NEIGHBOURS9)/sizeof(int); j++) {
			int n = i + NEIGHBOURS9[j];
			if (n >= 0 && n < LENGTH && map[n] != Terrain::WALL && map[n] != Terrain::WALL_DECO) 
			{
				d = true;
				break;
			}
		}

		if (d) 
		{
			d = false;

			for (int j = 0; j < sizeof(NEIGHBOURS9) / sizeof(int); j++) 
			{
				int n = i + NEIGHBOURS9[j];
				if (n >= 0 && n < LENGTH && !pit[n]) 
				{
					d = true;
					break;
				}
			}
		}

		discoverable[i] = d;
	}
}

Level::Level()
{
	color1 = 0x004400;
	color2 = 0x88CC44;

	feeling = Feeling::NONE;

	viewDistance = Dungeon::isChallenged(Challenges::DARKNESS) ? 3 : 8;
}

Level::~Level()
{
	clear();
}

void Level::create()
{
	resizingNeeded = false;
	
	map.resize(LENGTH);
	visited.resize(LENGTH);
	Arrays<bool>::fill(visited, false);

	mapped.resize(LENGTH);
	Arrays<bool>::fill(mapped, false);

	clear();
	//heaps = new SparseArray<Heap>();

	//plants = new SparseArray<Plant>();
	//
	//if (!Dungeon.bossLevel()) {
	//	addItemToSpawn(Generator.random(Generator.Category.FOOD));
	//	if (Dungeon.posNeeded()) {
	//		addItemToSpawn(new PotionOfStrength());
	//		Dungeon.potionOfStrength++;
	//	}
	//	if (Dungeon.souNeeded()) {
	//		addItemToSpawn(new ScrollOfUpgrade());
	//		Dungeon.scrollsOfUpgrade++;
	//	}
	//	if (Dungeon.soeNeeded()) {
	//		addItemToSpawn(new ScrollOfEnchantment());
	//		Dungeon.scrollsOfEnchantment++;
	//	}
	//
	//	if (Dungeon.depth > 1) {
	//		switch (Random.Int(10)) {
	//		case 0:
	//			if (!Dungeon.bossLevel(Dungeon.depth + 1)) {
	//				feeling = Feeling.CHASM;
	//			}
	//			break;
	//		case 1:
	//			feeling = Feeling.WATER;
	//			break;
	//		case 2:
	//			feeling = Feeling.GRASS;
	//			break;
	//		}
	//	}
	//}
	//
	bool pitNeeded = Dungeon::depth > 1 && weakFloorCreated;
	
	do {
		Arrays<int>::fill(map, feeling == Feeling::CHASM ? Terrain::CHASM : Terrain::WALL);
	
		pitRoomNeeded = pitNeeded;
		weakFloorCreated = false;
	
	} while (!build());
	decorate();
	
	buildFlagMaps();
	cleanWalls();

	createMobs();
	createItems();
}

void Level::clear()
{
	for (std::set<Mob*>::iterator itr = mobs.begin();
		itr != mobs.end(); itr++)
	{
		delete *itr;
	}
	mobs.clear();

	for (std::map<std::string, Blob*>::iterator itr = blobs.begin();
		itr != blobs.end(); itr++)
	{
		delete itr->second;
	}
	blobs.clear();
}

int Level::pitCell()
{
	return randomRespawnCell();
}

int Level::randomRespawnCell()
{
	int cell;
	do {
		cell = Random::Int(LENGTH);
	} while (!passable[cell] || Dungeon::visible[cell] || Actor::findChar(cell) != NULL);
	return cell;
}

int Level::adjustPos(int pos)
{
	return (pos / loadedMapSize) * WIDTH + (pos % loadedMapSize);
}

void Level::reset()
{
	//for (Mob mob : mobs.toArray(new Mob[0])) {
	//	if (!mob.reset()) {
	//		mobs.remove(mob);
	//	}
	//}
	//createMobs();
}

std::vector<bool> Level::updateFieldOfView(Char* c)
{
	int cx = c->pos % WIDTH;
	int cy = c->pos / WIDTH;

	bool sighted = false;
	//boolean sighted = c->buff(Blindness.class) == null && c.buff(Shadows.class) == null && c.isAlive();
	//if (sighted) {
	//	ShadowCaster.castShadow(cx, cy, fieldOfView, c.viewDistance);
	//}
	//else {
		Arrays<bool>::fill(fieldOfView, false);
	//}
	//
	int sense = 1;
	//if (c.isAlive()) {
	//	for (Buff b : c.buffs(MindVision.class)) {
	//		sense = Math.max(((MindVision)b).distance, sense);
	//	}
	//}
	
	if ((sighted && sense > 1) || !sighted) 
	{
	
		int ax = std::max(0, cx - sense);
		int bx = std::min(cx + sense, WIDTH - 1);
		int ay = std::max(0, cy - sense);
		int by = std::min(cy + sense, HEIGHT - 1);
	
		int len = bx - ax + 1;
		int pos = ax + ay * WIDTH;
		for (int y = ay; y <= by; y++, pos += WIDTH) 
		{
			Arrays<bool>::fill(fieldOfView, pos, pos + len, true);
		}
	
		for (int i = 0; i < LENGTH; i++) 
		{
			fieldOfView[i] = fieldOfView[i] & discoverable[i];
		}
	}
	
	if (c->isAlive()) {
		//if (c.buff(MindVision.class) != null) {
		//	for (Mob mob : mobs) {
		//		int p = mob.pos;
		//		fieldOfView[p] = true;
		//		fieldOfView[p + 1] = true;
		//		fieldOfView[p - 1] = true;
		//		fieldOfView[p + WIDTH + 1] = true;
		//		fieldOfView[p + WIDTH - 1] = true;
		//		fieldOfView[p - WIDTH + 1] = true;
		//		fieldOfView[p - WIDTH - 1] = true;
		//		fieldOfView[p + WIDTH] = true;
		//		fieldOfView[p - WIDTH] = true;
		//	}
		//}
		//else 
		if (c == Dungeon::hero && ((Hero*)c)->heroClass.type() == HeroClass::E_HUNTRESS) 
		{
			for (std::set<Mob*>::iterator itr = mobs.begin();
				itr != mobs.end(); itr++)
			{
				Mob* mob = *itr;;
				int p = mob->pos;
				if (distance(c->pos, p) == 2) 
				{
					fieldOfView[p] = true;
					fieldOfView[p + 1] = true;
					fieldOfView[p - 1] = true;
					fieldOfView[p + WIDTH + 1] = true;
					fieldOfView[p + WIDTH - 1] = true;
					fieldOfView[p - WIDTH + 1] = true;
					fieldOfView[p - WIDTH - 1] = true;
					fieldOfView[p + WIDTH] = true;
					fieldOfView[p - WIDTH] = true;
				}
			}
		}
		//if (c.buff(Awareness.class) != null) {
		//	for (Heap heap : heaps.values()) {
		//		int p = heap.pos;
		//		fieldOfView[p] = true;
		//		fieldOfView[p + 1] = true;
		//		fieldOfView[p - 1] = true;
		//		fieldOfView[p + WIDTH + 1] = true;
		//		fieldOfView[p + WIDTH - 1] = true;
		//		fieldOfView[p - WIDTH + 1] = true;
		//		fieldOfView[p - WIDTH - 1] = true;
		//		fieldOfView[p + WIDTH] = true;
		//		fieldOfView[p - WIDTH] = true;
		//	}
		//}
	}

	return fieldOfView;
}

int Level::distance(int a, int b)
{
	int ax = a % WIDTH;
	int ay = a / WIDTH;
	int bx = b % WIDTH;
	int by = b / WIDTH;
	return std::max(std::abs(ax - bx), std::abs(ay - by));
}

int Level::tunnelTile()
{
	return feeling == Feeling::CHASM ? Terrain::EMPTY_SP : Terrain::EMPTY;
}

int Level::randomDestination()
{
	int cell;
	do 
	{
		cell = Random::Int(LENGTH);
	} while (!passable[cell]);
	return cell;
}

bool Level::adjacent(int a, int b)
{
	int diff = std::abs(a - b);
	return diff == 1 || diff == WIDTH || diff == WIDTH + 1 || diff == WIDTH - 1;
}

void Level::mobPress(Mob* mob)
{
	//int cell = mob.pos;
	//
	//if (pit[cell] && !mob.flying) {
	//	Chasm.mobFall(mob);
	//	return;
	//}
	//
	//boolean trap = true;
	//switch (map[cell]) {
	//
	//case Terrain::TOXIC_TRAP:
	//	ToxicTrap.trigger(cell, mob);
	//	break;
	//
	//case Terrain::FIRE_TRAP:
	//	FireTrap.trigger(cell, mob);
	//	break;
	//
	//case Terrain::PARALYTIC_TRAP:
	//	ParalyticTrap.trigger(cell, mob);
	//	break;
	//
	//case Terrain::POISON_TRAP:
	//	PoisonTrap.trigger(cell, mob);
	//	break;
	//
	//case Terrain::ALARM_TRAP:
	//	AlarmTrap.trigger(cell, mob);
	//	break;
	//
	//case Terrain::LIGHTNING_TRAP:
	//	LightningTrap.trigger(cell, mob);
	//	break;
	//
	//case Terrain::GRIPPING_TRAP:
	//	GrippingTrap.trigger(cell, mob);
	//	break;
	//
	//case Terrain::SUMMONING_TRAP:
	//	SummoningTrap.trigger(cell, mob);
	//	break;
	//
	//case Terrain::DOOR:
	//	Door.enter(cell);
	//
	//default:
	//	trap = false;
	//}
	//
	//if (trap) {
	//	if (Dungeon.visible[cell]) {
	//		Sample.INSTANCE.play(Assets.SND_TRAP);
	//	}
	//	set(cell, Terrain::INACTIVE_TRAP);
	//	GameScene.updateMap(cell);
	//}
	//
	//Plant plant = plants.get(cell);
	//if (plant != null) {
	//	plant.activate(mob);
	//}
}

void Level::set(int cell, int terrain)
{
	Painter::set(Dungeon::level, cell, terrain);

	int flags = Terrain::flags[terrain];
	passable[cell] = (flags & Terrain::PASSABLE) != 0;
	losBlocking[cell] = (flags & Terrain::LOS_BLOCKING) != 0;
	flamable[cell] = (flags & Terrain::FLAMABLE) != 0;
	secret[cell] = (flags & Terrain::SECRET) != 0;
	solid[cell] = (flags & Terrain::SOLID) != 0;
	avoid[cell] = (flags & Terrain::AVOID) != 0;
	pit[cell] = (flags & Terrain::PIT) != 0;
	water[cell] = terrain == Terrain::WATER || terrain >= Terrain::WATER_TILES;
}

void Level::addVisuals(Scene* scene)
{
	//for (int i = 0; i < LENGTH; i++) {
	//	if (pit[i]) {
	//		scene.add(new WindParticle.Wind(i));
	//		if (i >= WIDTH && water[i - WIDTH]) {
	//			scene.add(new FlowParticle.Flow(i - WIDTH));
	//		}
	//	}
	//}
}
