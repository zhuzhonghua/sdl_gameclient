#pragma once

class Mob;

class Bestiary{
public:
	static Mob* mob(int depth);

private:
	static Mob* mobClass(int depth);
};