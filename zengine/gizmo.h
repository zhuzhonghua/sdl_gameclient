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
public:
	bool exists;
	bool alive;
	bool active;
	bool visible;

	Group* parent;
	Camera2D* camera;
};
}