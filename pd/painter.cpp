#include "painter.h"
#include "level.h"
#include "util.h"
#include "room.h"
#include "terrain.h"

void Painter::set(Level* level, int cell, int value)
{
	level->map[cell] = value;
}

void Painter::set(Level* level, int x, int y, int value)
{
	set(level, x + y * Level::WIDTH, value);
}

void Painter::set(Level* level, Point p, int value)
{
	set(level, p.x, p.y, value);
}

void Painter::fill(Level* level, int x, int y, int w, int h, int value)
{
	int width = Level::WIDTH;

	int pos = y * width + x;
	for (int i = y; i < y + h; i++, pos += width) {
		Arrays<int>::fill(level->map, pos, pos + w, value);
	}
}

void Painter::fill(Level* level, Rect rect, int value)
{
	int left = GameMath::RECTLeft(rect);
	int top = GameMath::RECTTop(rect);
	int width = GameMath::RECTWidth(rect);
	int height = GameMath::RECTHeight(rect);

	fill(level, left, top, width + 1, height + 1, value);
}

void Painter::fill(Level* level, Rect rect, int m, int value)
{
	int left = GameMath::RECTLeft(rect);
	int top = GameMath::RECTTop(rect);
	int width = GameMath::RECTWidth(rect);
	int height = GameMath::RECTHeight(rect);

	fill(level, left + m, top + m, width + 1 - m * 2, height + 1 - m * 2, value);
}

void Painter::fill(Level* level, Rect rect, int l, int t, int r, int b, int value)
{
	int left = GameMath::RECTLeft(rect);
	int top = GameMath::RECTTop(rect);
	int width = GameMath::RECTWidth(rect);
	int height = GameMath::RECTHeight(rect);

	fill(level, left + l, top + t, width + 1 - (l + r), height + 1 - (t + b), value);
}

Point Painter::drawInside(Level* level, Room* room, Point from, int n, int value)
{
	int left = GameMath::RECTLeft(room->getBounds());
	int top = GameMath::RECTTop(room->getBounds());
	int right = GameMath::RECTRight(room->getBounds());
	int bottom = GameMath::RECTBottom(room->getBounds());

	Point step;
	if (from.x == left) {
		//step.set(+1, 0);
		step.x = 1;
		step.y = 0;
	}
	else if (from.x == right) {
		//step.set(-1, 0);
		step.x = -1;
		step.y = 0;
	}
	else if (from.y == top) {
		//step.set(0, +1);
		step.x = 0;
		step.y = 1;
	}
	else if (from.y == bottom) {
		//step.set(0, -1);
		step.x = 0;
		step.y = -1;
	}

	Point p = from;
	p.x += step.x;
	p.y += step.y;
	for (int i = 0; i < n; i++) 
	{
		if (value != -1) 
		{
			set(level, p, value);
		}
		//p.offset(step);
		p.x += step.x;
		p.y += step.y;
	}

	return p;
}

void EntrancePainter::paint(Level* level, Room* room)
{
	fill(level, room->getBounds(), Terrain::WALL);
	fill(level, room->getBounds(), 1, Terrain::EMPTY);

	for (std::map<Room*, Room::Door*>::iterator itr = room->connected.begin();
		itr != room->connected.end(); itr++)
	{
		itr->second->set(Room::Door::Type::REGULAR);
	}

	level->entrance = room->random(1);
	set(level, level->entrance, Terrain::ENTRANCE);
}

void ExitPainter::paint(Level* level, Room* room)
{
	fill(level, room->getBounds(), Terrain::WALL);
	fill(level, room->getBounds(), 1, Terrain::EMPTY);

	for (std::map<Room*, Room::Door*>::iterator itr = room->connected.begin();
		itr != room->connected.end(); itr++)
	{
		itr->second->set(Room::Door::Type::REGULAR);
	}

	level->exit = room->random(1);
	set(level, level->exit, Terrain::EXIT);
}
