#include "dungeontilemap.h"
#include "level.h"
#include "util.h"

PointF DungeonTilemap::tileToWorld(int pos)
{
	PointF p(pos % Level::WIDTH, pos / Level::WIDTH);
	GameMath::PointFScale(&p, SIZE);
	return p;
}