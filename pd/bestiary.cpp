#include "bestiary.h"
#include "mob.h"
#include "rat.h"
#include "crab.h"
#include "gnoll.h"
#include "swarm.h"
#include "skeleton.h"
#include "thief.h"
#include "goo.h"
#include "shaman.h"
#include "bat.h"
#include "brute.h"
#include "tengu.h"
#include "spinner.h"
#include "elemental.h"
#include "monk.h"
#include "dm300.h"
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
	case 5:{
		float tmp[] = { 1 };
		chances.assign(tmp, tmp + sizeof(tmp) / sizeof(float));
		CreateMob tmp2[] = { Goo::CreateGoo };
		classes.assign(tmp2, tmp2 + sizeof(tmp2) / sizeof(CreateMob));
		break;
	}
	case 6:{
		float tmp[] = { 4, 2, 1, 0.2f };
		chances.assign(tmp, tmp + sizeof(tmp) / sizeof(float));
		CreateMob tmp2[] = { Skeleton::CreateSkeleton, Thief::CreateThief, Swarm::CreateSwarm, Shaman::CreateShaman };
		classes.assign(tmp2, tmp2 + sizeof(tmp2) / sizeof(CreateMob));
		break;
	}
	case 7:{
		float tmp[] = { 3, 1, 1, 1 };
		chances.assign(tmp, tmp + sizeof(tmp) / sizeof(float));
		CreateMob tmp2[] = { Skeleton::CreateSkeleton, Shaman::CreateShaman, Thief::CreateThief, Swarm::CreateSwarm };
		classes.assign(tmp2, tmp2 + sizeof(tmp2) / sizeof(CreateMob));
		break;
	}
	case 8:{
		float tmp[] = { 3, 2, 1, 1, 1, 0.02f };
		chances.assign(tmp, tmp + sizeof(tmp) / sizeof(float));
		CreateMob tmp2[] = { Skeleton::CreateSkeleton, Shaman::CreateShaman, Gnoll::CreateGnoll, Thief::CreateThief, Swarm::CreateSwarm, };
		classes.assign(tmp2, tmp2 + sizeof(tmp2) / sizeof(CreateMob));
		break;
	}
	case 9:{
		float tmp[] = { 3, 3, 1, 1, 0.02f, 0.01f };
		chances.assign(tmp, tmp + sizeof(tmp) / sizeof(float));
		CreateMob tmp2[] = { Skeleton::CreateSkeleton, Shaman::CreateShaman, Thief::CreateThief, Swarm::CreateSwarm, Bat::CreateBat, Brute::CreateBrute};
		classes.assign(tmp2, tmp2 + sizeof(tmp2) / sizeof(CreateMob));
		break;
	}
	case 10:{
		float tmp[] = { 1 };
		chances.assign(tmp, tmp + sizeof(tmp) / sizeof(float));
		CreateMob tmp2[] = { Tengu::CreateTengu };
		classes.assign(tmp2, tmp2 + sizeof(tmp2) / sizeof(CreateMob));
		break;
	}
	case 11:{
		float tmp[] = { 1, 0.2f };
		chances.assign(tmp, tmp + sizeof(tmp) / sizeof(float));
		CreateMob tmp2[] = { Bat::CreateBat, Brute::CreateBrute };
		classes.assign(tmp2, tmp2 + sizeof(tmp2) / sizeof(CreateMob));
		break;
	}
	case 12:{
		float tmp[] = { 1, 1, 0.2f };
		chances.assign(tmp, tmp + sizeof(tmp) / sizeof(float));
		CreateMob tmp2[] = { Bat::CreateBat, Brute::CreateBrute, Spinner::CreateSpinner };
		classes.assign(tmp2, tmp2 + sizeof(tmp2) / sizeof(CreateMob));
		break;
	}
	case 13:{
		float tmp[] = { 1, 3, 1, 1, 0.02f };
		chances.assign(tmp, tmp + sizeof(tmp) / sizeof(float));
		CreateMob tmp2[] = { Bat::CreateBat, Brute::CreateBrute, Shaman::CreateShaman, Spinner::CreateSpinner, Elemental::CreateElemental };
		classes.assign(tmp2, tmp2 + sizeof(tmp2) / sizeof(CreateMob));
		break;
	}
	case 14:{
		float tmp[] = { 1, 3, 1, 4, 0.02f, 0.01f };
		chances.assign(tmp, tmp + sizeof(tmp) / sizeof(float));
		CreateMob tmp2[] = { Bat::CreateBat, Brute::CreateBrute, Shaman::CreateShaman, Spinner::CreateSpinner, Elemental::CreateElemental, Monk::CreateMonk };
		classes.assign(tmp2, tmp2 + sizeof(tmp2) / sizeof(CreateMob));
		break;
	}
	case 15:{
		float tmp[] = { 1 };
		chances.assign(tmp, tmp + sizeof(tmp) / sizeof(float));
		CreateMob tmp2[] = { DM300::CreateDM300 };
		classes.assign(tmp2, tmp2 + sizeof(tmp2) / sizeof(CreateMob));
		break;
	}
	}

	return classes[Random::chances(chances)]();
}
