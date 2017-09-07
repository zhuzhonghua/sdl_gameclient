#include "bestiary.h"
#include "mob.h"
#include "rat.h"
#include "crab.h"
#include "gnoll.h"
#include "swarm.h"
#include "skeleton.h"
#include "thief.h"
#include "util.h"
#include <vector>

Mob* Bestiary::mob(int depth)
{
	return mobClass(depth);

	return NULL;
}

Mob* Bestiary::mobClass(int depth)
{
	std::vector<float> chances;
	std::vector<CreateMob>	classes;

	switch (depth)
	{
	case 1:{
		float tmp[] = { 1 };
		chances.assign(tmp, tmp + sizeof(tmp) / sizeof(float));
		CreateMob tmp2[] = { Rat::CreateRat };
		classes.assign(tmp2, tmp2 + sizeof(tmp2) / sizeof(CreateMob));
		break;
	}
	case 2:{
		float tmp[] = { 1, 1 };
		chances.assign(tmp, tmp + sizeof(tmp) / sizeof(float));
		CreateMob tmp2[] = { Rat::CreateRat, Gnoll::CreateGnoll };
		classes.assign(tmp2, tmp2 + sizeof(tmp2) / sizeof(CreateMob));
		break;
	}
	case 3:{
		float tmp[] = { 1, 2, 1, 0.02f };
		chances.assign(tmp, tmp + sizeof(tmp) / sizeof(float));
		CreateMob tmp2[] = { Rat::CreateRat, Gnoll::CreateGnoll, Crab::CreateCrab,  Swarm::CreateSwarm};
		classes.assign(tmp2, tmp2 + sizeof(tmp2) / sizeof(CreateMob));
		break;
	}
	case 4:{
		float tmp[] = { 1, 2, 3, 0.02f, 0.01f, 0.01f };
		chances.assign(tmp, tmp + sizeof(tmp) / sizeof(float));
		CreateMob tmp2[] = { Rat::CreateRat, Gnoll::CreateGnoll, Crab::CreateCrab, Swarm::CreateSwarm, Skeleton::CreateSkeleton, Thief::CreateThief };
		classes.assign(tmp2, tmp2 + sizeof(tmp2) / sizeof(CreateMob));
		break;
	}
	}

	return classes[Random::chances(chances)]();
}