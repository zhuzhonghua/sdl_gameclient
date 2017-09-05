#include "blob.h"
#include "bundle.h"
#include "util.h"
#include "level.h"
#include "dungeon.h"

const std::string Blob::CUR = "cur";
const std::string Blob::START = "start";

std::vector<int> Blob::trim(int start, int end)
{
	int len = end - start;
	std::vector<int> copy(len);
	copy.assign(&cur[start], &cur[end]);

	return copy;
}

Blob* Blob::getTypeInstance(const std::string& type)
{

	return NULL;
}

void Blob::storeInBundle(Bundle* bundle)
{
	Actor::storeInBundle(bundle);

	if (volume > 0) 
	{
		int start;
		for (start = 0; start < LENGTH; start++) 
		{
			if (cur[start] > 0) 
			{
				break;
			}
		}
		int end;
		for (end = LENGTH - 1; end > start; end--) 
		{
			if (cur[end] > 0) 
			{
				break;
			}
		}

		bundle->put(START, start);
		bundle->put(CUR, trim(start, end + 1));

	}
}

void Blob::restoreFromBundle(Bundle* bundle)
{
	Actor::restoreFromBundle(bundle);

	std::vector<int> data;
	bundle->getIntArray(CUR, data);
	if (data.size() > 0) 
	{
		int start = bundle->getInt(START);
		for (int i = 0; i < data.size(); i++) 
		{
			cur[i + start] = data[i];
			volume += data[i];
		}
	}

	if (Level::resizingNeeded) 
	{
		std::vector<int> cur(Level::LENGTH);

		int loadedMapSize = Level::loadedMapSize;
		for (int i = 0; i < loadedMapSize; i++) 
		{			
			Arrays<int>::arraycopy(this->cur, i * loadedMapSize, cur, i * Level::WIDTH, loadedMapSize);
		}

		this->cur = cur;
	}
}

bool Blob::act()
{
	spend(TICK);

	if (volume > 0) 
	{
		volume = 0;
		evolve();

		std::vector<int> tmp = off;
		off = cur;
		cur = tmp;

	}

	return true;
}

void Blob::seed(int cell, int amount)
{
	cur[cell] += amount;
	volume += amount;
}

Blob* Blob::seed(int cell, int amount, const std::string& type)
{
	Blob* gas = NULL;
	std::map<std::string, Blob*>::iterator itr = Dungeon::level->blobs.find(type);
	if (itr == Dungeon::level->blobs.end())
	{
		gas = getTypeInstance(type);
		Dungeon::level->blobs.insert(std::make_pair(type, gas));
	}
	else
	{
		gas = itr->second;
	}

	gas->seed(cell, amount);

	return gas;
}

void Blob::clear(int cell)
{
	volume -= cur[cell];
	cur[cell] = 0;
}

Blob::Blob()
{
	cur.resize(LENGTH);
	off.resize(LENGTH);

	volume = 0;
}

void Blob::evolve()
{
	std::vector<bool> notBlocking = BArray::not(Level::solid, std::vector<bool>());

	for (int i = 1; i < HEIGHT - 1; i++) 
	{
		int from = i * WIDTH + 1;
		int to = from + WIDTH - 2;

		for (int pos = from; pos < to; pos++) 
		{
			if (notBlocking[pos]) 
			{
				int count = 1;
				int sum = cur[pos];

				if (notBlocking[pos - 1]) 
				{
					sum += cur[pos - 1];
					count++;
				}
				if (notBlocking[pos + 1]) 
				{
					sum += cur[pos + 1];
					count++;
				}
				if (notBlocking[pos - WIDTH]) 
				{
					sum += cur[pos - WIDTH];
					count++;
				}
				if (notBlocking[pos + WIDTH]) 
				{
					sum += cur[pos + WIDTH];
					count++;
				}

				int value = sum >= count ? (sum / count) - 1 : 0;
				off[pos] = value;

				volume += value;
			}
			else 
			{
				off[pos] = 0;
			}
		}
	}
}
