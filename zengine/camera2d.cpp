#include "stdafx.h"
#include "camera2d.h"

namespace Zengine{
Camera2D* Camera2D::mainCamera;
Camera2D::Camera2D() :_position(0, 0),
	_scale(1),
	_orthoMatrix(1),
	cameraMatrix(1),
	_needsTransformUpdate(true),
	_screenWidth(960),
	_screenHeight(640)
{
	//_orthoMatrix = glm::ortho(0, _screenWidth, 0, _screenHeight);
}

Camera2D::~Camera2D()
{

}

void Camera2D::init(int screenWidth, int screenHeight)
{
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);
	cameraMatrix = _orthoMatrix;
}

void Camera2D::update()
{
	if (_needsTransformUpdate)
	{
		glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0);
		cameraMatrix = glm::translate(_orthoMatrix, translate);

		glm::vec3 scale(_scale, _scale, 0);
		cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * cameraMatrix;

		_needsTransformUpdate = false;
	}
}

bool Camera2D::isBoxInView(const glm::vec2 position, const glm::vec2 dimension)
{
	glm::vec2 scaledScreenDimensions = glm::vec2(_screenWidth, _screenHeight) / (_scale);

	float MIN_DISTANCE_X = dimension.x / 2.0f + scaledScreenDimensions.x / 2.0f;
	float MIN_DISTANCE_Y = dimension.y / 2.0f + scaledScreenDimensions.y / 2.0f;

	glm::vec2 centerPosition = position + dimension / 2.0f;
	glm::vec2 centerCamPos = _position;

	glm::vec2 distVec = centerPosition - centerCamPos;

	float xDepth = MIN_DISTANCE_X - abs(distVec.x);
	float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

	if (xDepth > 0 || yDepth > 0)
	{
		return true;
	}

	return false;
}
}