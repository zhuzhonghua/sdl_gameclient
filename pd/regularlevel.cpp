#include "regularlevel.h"
#include "util.h"
#include "room.h"
#include "graph.h"
#include "dungeon.h"
#include "terrain.h"
#include "painter.h"

RegularLevel::RegularLevel()
{
	secretDoors = 0;

	minRoomSize = 7;
	maxRoomSize = 9;
}

RegularLevel::~RegularLevel()
{
	for (std::set<Room*>::iterator itr = rooms.begin();
		itr != rooms.end(); itr++)
	{
		delete *itr;
	}
	rooms.clear();
}

void RegularLevel::storeInBundle(Bundle* bundle)
{
	//super.storeInBundle(bundle);
	//bundle.put("rooms", rooms);
}

void RegularLevel::restoreFromBundle(Bundle* bundle)
{
	//super.restoreFromBundle(bundle);
	//
	//rooms = new HashSet<Room>((Collection< ? extends Room>) bundle.getCollection("rooms"));
	//for (Room r : rooms) {
	//	if (r.type == Type.WEAK_FLOOR) {
	//		weakFloorCreated = true;
	//		break;
	//	}
	//}
}

bool RegularLevel::build()
{
	if (!initRooms()) 
	{
		return false;
	}

	std::set<Graph::Node*> tmpRooms(rooms.begin(), rooms.end());

	int distance;
	int retry = 0;
	int minDistance = (int)std::sqrt(rooms.size());
	do {
		do {
			roomEntrance = Random::element(rooms);
		} while (roomEntrance->width() < 4 || roomEntrance->height() < 4);

		do {
			roomExit = Random::element(rooms);
		} while (roomExit == roomEntrance || roomExit->width() < 4 || roomExit->height() < 4);

		Graph::buildDistanceMap(tmpRooms, roomExit);
		distance = roomEntrance->Distance();

		if (retry++ > 10) {
			return false;
		}

	} while (distance < minDistance);

	roomEntrance->type = Room::Type::ENTRANCE;
	roomExit->type = Room::Type::EXIT;

	std::set<Room*> connected;
	connected.insert(roomEntrance);

	Graph::buildDistanceMap(tmpRooms, roomExit);
	std::list<Graph::Node*> path = Graph::buildPath(tmpRooms, roomEntrance, roomExit);

	Room* room = roomEntrance;
	for (std::list<Graph::Node*>::iterator itr = path.begin();
		itr != path.end(); itr++){
		Room* next = (Room*)*itr;
		room->connect(next);
		room = next;
		connected.insert(room);
	}

	Graph::setPrice(path, roomEntrance->distance);

	Graph::buildDistanceMap(tmpRooms, roomExit);
	path = Graph::buildPath(tmpRooms, roomEntrance, roomExit);

	room = roomEntrance;
	for (std::list<Graph::Node*>::iterator itr = path.begin();
		itr != path.end(); itr++)
	{
		Room* next = (Room*)*itr;
		room->connect(next);
		room = next;
		connected.insert(room);
	}

	int nConnected = (int)(rooms.size() * Random::Float(0.5f, 0.7f));
	while (connected.size() < nConnected) 
	{
		Room* cr = Random::element(connected);
		Room* or = Random::element(cr->neigbours);
		if (connected.find(or) == connected.end()) 
		{
			cr->connect(or);
			connected.insert(or);
		}
	}

	//if (Dungeon.shopOnLevel()) {
	//	Room shop = null;
	//	for (Room r : roomEntrance.connected.keySet()) {
	//		if (r.connected.size() == 1 && r.width() >= 5 && r.height() >= 5) {
	//			shop = r;
	//			break;
	//		}
	//	}
	//
	//	if (shop == null) {
	//		return false;
	//	}
	//	else {
	//		shop.type = Room.Type.SHOP;
	//	}
	//}
	//
	//specials = new ArrayList<Room.Type>(Room.SPECIALS);
	//if (Dungeon.bossLevel(Dungeon.depth + 1)) {
	//	specials.remove(Room.Type.WEAK_FLOOR);
	//}
	assignRoomType();
	//
	paint();
	//paintWater();
	//paintGrass();

	//placeTraps();

	return true;
}

bool RegularLevel::initRooms()
{
	for (std::set<Room*>::iterator itr = rooms.begin();
		itr != rooms.end(); itr++)
	{
		delete *itr;
	}
	rooms.clear();

	split(GameMath::RECT(0, 0, WIDTH - 1, HEIGHT - 1));

	if (rooms.size() < 8) 
	{
		return false;
	}

	std::vector<Room*> ra(rooms.begin(), rooms.end());
	for (int i = 0; i < ra.size() - 1; i++) 
	{
		for (int j = i + 1; j < ra.size(); j++) 
		{
			ra[i]->addNeigbour(ra[j]);
		}
	}

	return true;
}

void RegularLevel::split(Rect rect)
{
	int w = GameMath::RECTWidth(rect);
	int h = GameMath::RECTHeight(rect);

	int left = GameMath::RECTLeft(rect);
	int right = GameMath::RECTRight(rect);
	int top = GameMath::RECTTop(rect);
	int bottom = GameMath::RECTBottom(rect);

	int square = GameMath::RECTSquare(rect);

	if (w > maxRoomSize && h < minRoomSize) 
	{
		int vw = Random::Int(left + 3, right - 3);
		split(GameMath::RECT(left, top, vw, bottom));
		split(GameMath::RECT(vw, top, right, bottom));
	}
	else if (h > maxRoomSize && w < minRoomSize) 
	{
		int vh = Random::Int(top + 3, bottom - 3);
		split(GameMath::RECT(left, top, right, vh));
		split(GameMath::RECT(left, vh, right, bottom));
	}
	else if ((Random::random() <= (minRoomSize * minRoomSize / square) && w <= maxRoomSize && h <= maxRoomSize) || w < minRoomSize || h < minRoomSize) 
	{
		Room* r = new Room();
		r->set(rect);
		rooms.insert(r);
	}
	else 
	{
		if (Random::Float() < (float)(w - 2) / (w + h - 4)) 
		{
			int vw = Random::Int(left + 3, right - 3);
			split(GameMath::RECT(left, top, vw, bottom));
			split(GameMath::RECT(vw, top, right, bottom));
		}
		else 
		{
			int vh = Random::Int(top + 3, bottom - 3);
			split(GameMath::RECT(left, top, right, vh));
			split(GameMath::RECT(left, vh, right, bottom));
		}
	}
}

void RegularLevel::paint()
{
	for (std::set<Room*>::iterator itr = rooms.begin();
		itr != rooms.end(); itr++)
	{
		Room* r = *itr;
		if (r->type != Room::Type::NONE)
		{
			placeDoors(r);
			Room::paint(r->type, this, r);
		}
		else 
		{
			if (feeling == Feeling::CHASM && Random::Int(2) == 0) {
				Painter::fill(this, r->getBounds(), Terrain::WALL);
			}
		}
	}

	for (std::set<Room*>::iterator itr = rooms.begin();
		itr != rooms.end(); itr++)
	{
		Room* r = *itr;
		paintDoors(r);
	}
}

void RegularLevel::paintDoors(Room* r)
{
	for (std::map<Room*, Room::Door*>::iterator itr = r->connected.begin();
		itr != r->connected.end(); itr++)
	{
		Room* n = itr->first;

		if (joinRooms(r, n)) 
		{
			continue;
		}

		Room::Door* d = itr->second;
		int door = d->getPoint().x + d->getPoint().y * WIDTH;

		switch (d->type) {
		case Room::Door::EMPTY:
			map[door] = Terrain::EMPTY;
			break;
		case Room::Door::TUNNEL:
			map[door] = tunnelTile();
			break;
		case Room::Door::REGULAR:
			if (Dungeon::depth <= 1) 
			{
				map[door] = Terrain::DOOR;
			}
			else 
			{
				bool secret = (Dungeon::depth < 6 ? Random::Int(12 - Dungeon::depth) : Random::Int(6)) == 0;
				map[door] = secret ? Terrain::SECRET_DOOR : Terrain::DOOR;
				if (secret) 
				{
					secretDoors++;
				}
			}
			break;
		case Room::Door::UNLOCKED:
			map[door] = Terrain::DOOR;
			break;
		case Room::Door::HIDDEN:
			map[door] = Terrain::SECRET_DOOR;
			secretDoors++;
			break;
		case Room::Door::BARRICADE:
			map[door] = Random::Int(3) == 0 ? Terrain::BOOKSHELF : Terrain::BARRICADE;
			break;
		case Room::Door::LOCKED:
			map[door] = Terrain::LOCKED_DOOR;
			break;
		}
	}
}

void RegularLevel::assignRoomType()
{
	int specialRooms = 0;

	for (std::set<Room*>::iterator itr = rooms.begin();
		itr != rooms.end(); itr++)
	{
		Room* r = *itr;
		if (r->type == Room::Type::NONE &&
			r->connected.size() == 1) 
		{
			int width = GameMath::RECTWidth(r->getBounds());
			int height = GameMath::RECTHeight(r->getBounds());

			if (specials.size() > 0 &&
				width > 3 && height > 3 &&
				Random::Int(specialRooms * specialRooms + 2) == 0) 
			{
				if (pitRoomNeeded) 
				{
					r->type = Room::Type::PIT;
					pitRoomNeeded = false;

					specials.remove(Room::Type::ARMORY);
					specials.remove(Room::Type::CRYPT);
					specials.remove(Room::Type::LABORATORY);
					specials.remove(Room::Type::LIBRARY);
					specials.remove(Room::Type::STATUE);
					specials.remove(Room::Type::TREASURY);
					specials.remove(Room::Type::VAULT);
					specials.remove(Room::Type::WEAK_FLOOR);
				}
				else if (Dungeon::depth % 5 == 2 && 
						std::find(specials.begin(), specials.end(), Room::Type::LABORATORY) != specials.end()) 
				{
					r->type = Room::Type::LABORATORY;
				}
				else 
				{
					int n = specials.size();
					int idx = std::min(Random::Int(n), Random::Int(n));
					int c = 0;
					for (std::list<Room::Type>::iterator itr = specials.begin();
						itr != specials.end(); itr++)
					{
						if (c == idx)
						{
							r->type = *itr;
							break;
						}
						c++;
					}
					if (r->type == Room::Type::WEAK_FLOOR) 
					{
						weakFloorCreated = true;
					}
				}

				Room::useType(r->type);
				specials.remove(r->type);
				specialRooms++;

			}
			else if (Random::Int(2) == 0)
			{
				std::set<Room*> neigbours;
				for (std::set<Room*>::iterator itr = r->neigbours.begin();
					itr != r->neigbours.end(); itr++)				
				{
					Room* n = *itr;
					if (r->connected.end() == r->connected.find(n) &&
						Room::SPECIALS.end() == std::find(Room::SPECIALS.begin(), Room::SPECIALS.end(), n->type) &&
						n->type != Room::Type::PIT) 
					{
						neigbours.insert(n);
					}
				}
				if (neigbours.size() > 1) 
				{
					r->connect(Random::element(neigbours));
				}
			}
		}
	}

	int count = 0;
	for (std::set<Room*>::iterator itr = rooms.begin();
		itr != rooms.end(); itr++)
	{
		Room* r = *itr;
		if (r->type == Room::Type::NONE)
		{
			int connections = r->connected.size();
			if (connections == 0) 
			{

			}
			else if (Random::Int(connections * connections) == 0) 
			{
				r->type = Room::Type::STANDARD;
				count++;
			}
			else 
			{
				r->type = Room::Type::TUNNEL;
			}
		}
	}

	while (count < 4) 
	{
		Room* r = randomRoom(Room::Type::TUNNEL, 1);
		if (r != NULL) 
		{
			r->type = Room::Type::STANDARD;
			count++;
		}
	}
}

Room* RegularLevel::randomRoom(Room::Type type, int tries)
{
	for (int i = 0; i < tries; i++) 
	{
		Room* room = Random::element(rooms);
		if (room->type == type) 
		{
			return room;
		}
	}
	return NULL;
}

bool RegularLevel::joinRooms(Room* r, Room* n)
{
	if (r->type != Room::Type::STANDARD || n->type != Room::Type::STANDARD) 
	{
		return false;
	}

	Rect w = GameMath::RECTIntersect(r->getBounds(), n->getBounds());

	int wleft = GameMath::RECTLeft(w);
	int wright = GameMath::RECTRight(w);
	int wtop = GameMath::RECTTop(w);
	int wbottom = GameMath::RECTBottom(w);
	int wheight = GameMath::RECTHeight(w);
	int wwidth = GameMath::RECTWidth(w);

	int rheight = GameMath::RECTHeight(r->getBounds());
	int rwidth = GameMath::RECTWidth(r->getBounds());
	int nheight = GameMath::RECTHeight(n->getBounds());
	int nwidth = GameMath::RECTWidth(n->getBounds());

	if (wleft == wright) 
	{
		if (wbottom - wtop < 3) 
		{
			return false;
		}

		if (wheight == std::max(rheight, nheight)) 
		{
			return false;
		}

		if (rwidth + nwidth > maxRoomSize) 
		{
			return false;
		}

		wtop += 1;
		wbottom -= 0;

		wright++;

		Painter::fill(this, wleft, wtop, 1, wheight, Terrain::EMPTY);

	}
	else 
	{
		if (wright - wleft < 3) 
		{
			return false;
		}

		if (wwidth == std::max(rwidth, nwidth)) {
			return false;
		}

		if (rheight + nheight > maxRoomSize) 
		{
			return false;
		}

		wleft += 1;
		wright -= 0;

		wbottom++;

		Painter::fill(this, wleft, wtop, wwidth, 1, Terrain::EMPTY);
	}

	return true;
}

void RegularLevel::placeDoors(Room* r)
{
	for (std::map<Room*, Room::Door*>::iterator itr = r->connected.begin();
		itr != r->connected.end(); itr++)
	{
		Room* n = itr->first;
		Room::Door* door = itr->second;

		if (door == NULL)
		{
			Rect i = GameMath::RECTIntersect(r->getBounds(), n->getBounds());
			int width = GameMath::RECTWidth(i);

			int top = GameMath::RECTTop(i);
			int bottom = GameMath::RECTBottom(i);
			int left = GameMath::RECTLeft(i);
			int right = GameMath::RECTRight(i);

			if (width == 0)
			{
				door = new Room::Door(left, Random::Int(top+1, bottom));
			}
			else
			{
				door = new Room::Door(Random::Int(left, right), top);
			}

			r->connected[n] = door;
			n->connected[r] = door;
		}
	}
}
