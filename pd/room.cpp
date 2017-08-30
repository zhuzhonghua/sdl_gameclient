#include "room.h"
#include "util.h"
#include "bundle.h"
#include "level.h"
#include "standardpainter.h"

const Room::Type Room::_SPECIALS[] = { 
	Room::Type::ARMORY, 
	Room::Type::WEAK_FLOOR, 
	Room::Type::MAGIC_WELL, 
	Room::Type::CRYPT, 
	Room::Type::POOL, 
	Room::Type::GARDEN, 
	Room::Type::LIBRARY,
	Room::Type::TREASURY, 
	Room::Type::TRAPS, 
	Room::Type::STORAGE, 
	Room::Type::STATUE, 
	Room::Type::LABORATORY, 
	Room::Type::VAULT, 
	Room::Type::ALTAR };

std::list<Room::Type> Room::SPECIALS(_SPECIALS, _SPECIALS + sizeof(_SPECIALS) / sizeof(Room::Type));

std::map<Room::Type, Room::PaintMethod> Room::paintMethods;

Room::Room()
{
	type = Type::NONE;
	price = 1;
}

void Room::set(int left, int top, int right, int bottom)
{
	bounds = GameMath::RECT(left,top,right,bottom);
}

void Room::set(Rect b)
{
	bounds = b;
}

void Room::addNeigbour(Room* other)
{
	Rect i = GameMath::RECTIntersect(bounds, other->bounds);
	int w = GameMath::RECTWidth(i);
	int h = GameMath::RECTHeight(i);

	if ((w == 0 && h >= 3) || (w == 0 && h >= 3)) 
	{
		neigbours.insert(other);
		other->neigbours.insert(this);
	}
}

int Room::width()
{
	return GameMath::RECTWidth(bounds);
}

int Room::height()
{
	return GameMath::RECTHeight(bounds);
}

void Room::storeInBundle(Bundle* bundle)
{
	int left = GameMath::RECTLeft(bounds);
	int right = GameMath::RECTRight(bounds);
	int top = GameMath::RECTTop(bounds);
	int bottom = GameMath::RECTBottom(bounds);

	bundle->put("left", left);
	bundle->put("top", top);
	bundle->put("right", right);
	bundle->put("bottom", bottom);
	//bundle->put("type", type.toString());
}

void Room::restoreFromBundle(Bundle* bundle)
{
	int left = bundle->getInt("left");
	int top = bundle->getInt("top");
	int right = bundle->getInt("right");
	int bottom = bundle->getInt("bottom");
	//type = Type.valueOf(bundle.getString("type"));

	bounds = GameMath::RECT(left, top, right, bottom);
}

void Room::connect(Room* room)
{
	if (connected.find(room) == connected.end())
	{
		connected.insert(std::make_pair(room, (Door*)NULL));
	}
}

int Room::random(int m)
{
	int left = GameMath::RECTLeft(bounds);
	int right = GameMath::RECTRight(bounds);
	int top = GameMath::RECTTop(bounds);
	int bottom = GameMath::RECTBottom(bounds);

	int x = Random::Int(left + 1 + m, right - m);
	int y = Random::Int(top + 1 + m, bottom - m);
	return x + y * Level::WIDTH;
}

void Room::useType(Type type)
{
	if (std::find(SPECIALS.begin(), SPECIALS.end(), type) != SPECIALS.end())
	{
		SPECIALS.remove(type);
		SPECIALS.push_back(type);
	}
}

void Room::paint(Type type, Level* level, Room* r)
{
	PaintMethod pm = getPaintMethod(type);
	if (pm)	pm(level, r);
}

Room::PaintMethod Room::getPaintMethod(Type type)
{
	if (paintMethods.size() == 0)
	{
		paintMethods.insert(std::make_pair(STANDARD, StandardPainter::paint));
		paintMethods.insert(std::make_pair(ENTRANCE, EntrancePainter::paint));
		paintMethods.insert(std::make_pair(EXIT, ExitPainter::paint));
	}
	return paintMethods[type];
}

Room::Door::Door(int x, int y)
{
	type = Type::EMPTY;
	p.x = x;
	p.y = y;
}

void Room::Door::set(Type type)
{
	if (type - this->type > 0)
	{
		this->type = type;
	}
}
