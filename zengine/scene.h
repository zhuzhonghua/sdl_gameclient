#pragma once

#include "group.h"
#include "camera2D.h"

namespace Zengine{
class Scene :public Group{
public:
	Scene(){}
	~Scene(){}

	virtual void init(){};
	virtual void pause(){};
	virtual void resume(){}

	virtual Camera2D* camera()
	{
		return Camera2D::mainCamera;
	}

	virtual void onBackPressed(){}
	virtual void onMenuPressed(){}
};
}