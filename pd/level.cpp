#include "level.h"
#include "util.h"
#include "dungeon.h"
#include "terrain.h"

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

Level::Level()
{
	color1 = 0x004400;
	color2 = 0x88CC44;

	feeling = Feeling::NONE;

	viewDistance = Dungeon::isChallenged(Challenges::DARKNESS) ? 3 : 8;
}

void Level::create()
{
	resizingNeeded = false;
	
	map.resize(LENGTH);
	visited.resize(LENGTH);
	Arrays<bool>::fill(visited, false);

	mapped.resize(LENGTH);
	Arrays<bool>::fill(mapped, false);

	//mobs = new HashSet<Mob>();
	//heaps = new SparseArray<Heap>();
	//blobs = new HashMap<Class< ? extends Blob>, Blob>();
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
	//
	//buildFlagMaps();
	//cleanWalls();
	//
	//createMobs();
	//createItems();
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

	//boolean sighted = c->buff(Blindness.class) == null && c.buff(Shadows.class) == null && c.isAlive();
	//if (sighted) {
	//	ShadowCaster.castShadow(cx, cy, fieldOfView, c.viewDistance);
	//}
	//else {
	//	Arrays.fill(fieldOfView, false);
	//}
	//
	//int sense = 1;
	//if (c.isAlive()) {
	//	for (Buff b : c.buffs(MindVision.class)) {
	//		sense = Math.max(((MindVision)b).distance, sense);
	//	}
	//}
	//
	//if ((sighted && sense > 1) || !sighted) {
	//
	//	int ax = Math.max(0, cx - sense);
	//	int bx = Math.min(cx + sense, WIDTH - 1);
	//	int ay = Math.max(0, cy - sense);
	//	int by = Math.min(cy + sense, HEIGHT - 1);
	//
	//	int len = bx - ax + 1;
	//	int pos = ax + ay * WIDTH;
	//	for (int y = ay; y <= by; y++, pos += WIDTH) {
	//		Arrays.fill(fieldOfView, pos, pos + len, true);
	//	}
	//
	//	for (int i = 0; i < LENGTH; i++) {
	//		fieldOfView[i] &= discoverable[i];
	//	}
	//}
	//
	//if (c.isAlive()) {
	//	if (c.buff(MindVision.class) != null) {
	//		for (Mob mob : mobs) {
	//			int p = mob.pos;
	//			fieldOfView[p] = true;
	//			fieldOfView[p + 1] = true;
	//			fieldOfView[p - 1] = true;
	//			fieldOfView[p + WIDTH + 1] = true;
	//			fieldOfView[p + WIDTH - 1] = true;
	//			fieldOfView[p - WIDTH + 1] = true;
	//			fieldOfView[p - WIDTH - 1] = true;
	//			fieldOfView[p + WIDTH] = true;
	//			fieldOfView[p - WIDTH] = true;
	//		}
	//	}
	//	else if (c == Dungeon.hero && ((Hero)c).heroClass == HeroClass.HUNTRESS) {
	//		for (Mob mob : mobs) {
	//			int p = mob.pos;
	//			if (distance(c.pos, p) == 2) {
	//				fieldOfView[p] = true;
	//				fieldOfView[p + 1] = true;
	//				fieldOfView[p - 1] = true;
	//				fieldOfView[p + WIDTH + 1] = true;
	//				fieldOfView[p + WIDTH - 1] = true;
	//				fieldOfView[p - WIDTH + 1] = true;
	//				fieldOfView[p - WIDTH - 1] = true;
	//				fieldOfView[p + WIDTH] = true;
	//				fieldOfView[p - WIDTH] = true;
	//			}
	//		}
	//	}
	//	if (c.buff(Awareness.class) != null) {
	//		for (Heap heap : heaps.values()) {
	//			int p = heap.pos;
	//			fieldOfView[p] = true;
	//			fieldOfView[p + 1] = true;
	//			fieldOfView[p - 1] = true;
	//			fieldOfView[p + WIDTH + 1] = true;
	//			fieldOfView[p + WIDTH - 1] = true;
	//			fieldOfView[p - WIDTH + 1] = true;
	//			fieldOfView[p - WIDTH - 1] = true;
	//			fieldOfView[p + WIDTH] = true;
	//			fieldOfView[p - WIDTH] = true;
	//		}
	//	}
	//}

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
