#include "inputmanager.h"

using namespace Zhu;

InputManager::InputManager() :_mouseCoords(0)
{

}

void InputManager::pressKey(unsigned int key)
{
	_keys[key] = true;
}
void InputManager::releaseKey(unsigned int key)
{
	_keys[key] = false;
}

void InputManager::setMouseCoords(float x, float y)
{
	_mouseCoords.x = x;
	_mouseCoords.y = y;
}

bool InputManager::isKeyPressed(unsigned int key)
{
	std::unordered_map<unsigned int, bool>::iterator itr = _keys.find(key);
	if (itr != _keys.end())
	{
		return itr->second;
	}
	else
	{
		return false;
	}
}