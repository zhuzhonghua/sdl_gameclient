#include "camera2d.h"

using namespace Zhu;

Camera2D::Camera2D() :_position(0,0),
_scale(1),
_orthoMatrix(1),
_cameraMatrix(1),
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
	_cameraMatrix = _orthoMatrix;
}

void Camera2D::update()
{
	if (_needsTransformUpdate)
	{
		glm::vec3 translate(-_position.x+_screenWidth/2, -_position.y+_screenHeight/2, 0);
		_cameraMatrix = glm::translate(_orthoMatrix,translate);
	
		glm::vec3 scale(_scale, _scale, 0);
		_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;
	
		_needsTransformUpdate = false;
	}
}

glm::vec2 Camera2D::convertScreenToWorld(glm::vec2 screenCoords)
{
	screenCoords.y = _screenHeight - screenCoords.y;
	screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
	screenCoords /= _scale;
	screenCoords += _position;

	return screenCoords;
}