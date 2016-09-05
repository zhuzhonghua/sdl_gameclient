#include "stdafx.h"
#include "character.h"
#include "camera.h"

Camera* Camera::main;

Camera* Camera::inst()
{
	if (main)
	{
		return main;
	}
	else
	{
		main = new Camera();
		return main;
	}
}

Camera::Camera()
{
	cameraX = 0;
	cameraY = 0;

	mainCh = NULL;
}


Camera::~Camera()
{
	
}

int Camera::getX()
{
	return cameraX;
}

int Camera::getY()
{
	return cameraY;
}

void Camera::setPos(int x, int y)
{
	cameraX = x;
	cameraY = y;
}

void Camera::setMe(Character* p)
{
	mainCh = p;

	int px, py;
	p->getPosition(&px, &py);

	distMeX = px-cameraX;
	distMeY = py-cameraY;
}

void Camera::update()
{
	int px, py;
	mainCh->getPosition(&px, &py);

	if (px - cameraX != distMeX ||
		py - cameraY != distMeY)
	{
		cameraX = px - distMeX;
		cameraY = py - distMeY;
	}
}

SDL_Point Camera::screenToWorld(SDL_Point *p)
{
	SDL_Point ret;
	ret.x = p->x + cameraX;
	ret.y = p->y + cameraY;
	return ret;
}