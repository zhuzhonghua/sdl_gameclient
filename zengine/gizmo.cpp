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

Camera2D* Gizmo::camera()
{
	if (camera != NULL)
	{
		return camera;
	}
	else if (parent != NULL)
	{
		return parent->camera();
	}
	else
	{
		return NULL;
	}
}

}