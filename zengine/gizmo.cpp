#include "stdafx.h"

#include "gizmo.h"

#include "group.h"

namespace Zengine{
Gizmo::Gizmo()
{
	exists = true;
	alive = true;
	active = true;
	visible = true;

	parent = NULL;
	camera = NULL;

	_x = 0; 
	_y = 0; 
	_width = 0; 
	_height = 0;
}

bool Gizmo::isVisible()
{
	if (parent == NULL)
	{
		return visible;
	}
	else
	{
		return visible && parent->isVisible();
	}
}

bool Gizmo::isActive()
{
	if (parent == NULL)
	{
		return active;
	}
	else
	{
		return active && parent->isActive();
	}
}

void Gizmo::killAndErase()
{
	kill();
	if (parent != NULL)
	{
		parent->erase(this);
	}
}

void Gizmo::remove()
{
	if (parent != NULL)
	{
		parent->remove(this);
	}
}

Camera2D* Gizmo::getCamera()
{
	if (camera != NULL)
	{
		return camera;
	}
	else if (parent != NULL)
	{
		return parent->getCamera();
	}
	else
	{
		return NULL;
	}
}

}