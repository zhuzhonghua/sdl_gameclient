#pragma once

namespace Zengine{
class Camera2D
{
public:
	Camera2D();
	~Camera2D();

	void init(int screenWidth, int screenHeight);
	void update();

	bool isBoxInView(const glm::vec2 position, const glm::vec2 dimention);

	void setPosition(glm::vec2 newPosition) { _position = newPosition; _needsTransformUpdate = true; }
	void setScale(float scale) { _scale = scale; _needsTransformUpdate = true; }

	glm::vec2 getPosition(){ return _position; }
	float getScale() { return _scale; }

	Matrix cameraMatrix;

	static Camera2D* mainCamera;
protected:
	int _screenWidth, _screenHeight;
	bool _needsTransformUpdate;
	float _scale;
	glm::vec2 _position;
	Matrix _orthoMatrix;
};
}