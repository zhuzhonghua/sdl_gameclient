#include "iomanager.h"
using namespace Zhu;
bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char> &buffer)
{
	SDL_RWops* fileOP = SDL_RWFromFile(filePath.c_str(), "r");
	if (fileOP == NULL)
	{
		perror(filePath.c_str());
		return false;
	}

	int len = SDL_RWsize(fileOP);

	buffer.resize(len);
	SDL_RWread(fileOP, &buffer[0], len, 1);
	SDL_RWclose(fileOP);

	return true;
}

bool IOManager::readFileToBuffer(std::string filePath, std::string &buffer)
{
	SDL_RWops* fileOP = SDL_RWFromFile(filePath.c_str(), "r");
	if (fileOP == NULL)
	{
		perror(filePath.c_str());
		return false;
	}

	int len = SDL_RWsize(fileOP);

	buffer.resize(len);
	SDL_RWread(fileOP, &buffer[0], len, 1);
	SDL_RWclose(fileOP);

	return true;
}