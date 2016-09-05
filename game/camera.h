#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "gameobject.h"

#define MAINCAMERA() Camera::inst()

class Character;

class Camera
{
	int cameraX;
	int cameraY;

	Camera();
	static Camera* main;
	Character*	mainCh;

	int distMeX;
	int distMeY;
public:
	void	setMe(Character* p);

	static Camera* inst();
	~Camera();

	int getX();
	int getY();

	void setPos(int x, int y);

	void update();

	SDL_Point	screenToWorld(SDL_Point *p);
};
#endif
