#pragma once

class Mob;

class Bestiary{
public:
	static Mob* mob(int depth);
	static boolean isBoss(Char* mob);
private:
	static Mob* mobClass(int depth);
};