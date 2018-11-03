#pragma once

#include <string>

namespace Zengine{
class Group;
class Camera2D;

class Gizmo{
public:
	Gizmo();

	virtual ~Gizmo(){ destroy(); }

	virtual void destroy(){}
	virtual void update(){}
	virtual void draw(){}

	virtual void kill()
	{
		alive = false;
		exists = false;
	}
	void revive()
	{
		alive = true;
		exists = true;
	}

	bool isVisible();
	bool isActive();
	void killAndErase();
	void remove();

	virtual Camera2D* getCamera();


	float getWidth() { return _width; }
	float getHeight() { return _height; }
	float getPosX() { return _x; }
	float getPosY() { return _y; }

	virtual void setPosition(float x, float y) { _x = x; _y = y;}
	virtual void setSize(float w, float h) { _width = w; _height = h;}
public:
	bool exists;
	bool alive;
	bool active;
	bool visible;

	Group* parent;
	Camera2D* camera;

protected:
	float _width;
	float _height;
	float _x;
	float _y;
};
}