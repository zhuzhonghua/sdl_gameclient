#pragma once

#include "level.h"
#include "define.h"
#include "room.h"

#include <set>

class RegularLevel :public Level{
public:
	int secretDoors;

	RegularLevel();
	virtual ~RegularLevel();

	virtual void storeInBundle(Bundle* bundle);
	virtual void restoreFromBundle(Bundle* bundle);
protected:
	std::list<Room::Type> specials;
	std::set<Room*> rooms;
	int minRoomSize;
	int maxRoomSize;

	Room* roomEntrance;
	Room* roomExit;

	virtual bool build();
	bool initRooms();
	void split(Rect rect);
	void paint();
	void paintDoors(Room* r);
	void assignRoomType();
	Room* randomRoom(Room::Type type, int tries);
	bool joinRooms(Room* r, Room* n);
private:
	void placeDoors(Room* r);
};