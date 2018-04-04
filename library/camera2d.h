#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Zhu
{
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

		glm::vec2 convertScreenToWorld(glm::vec2 screenCoords);

		glm::vec2 getPosition(){ return _position; }
		float getScale() { return _scale; }
		glm::mat4 getCameraMatrix() { return _cameraMatrix; }

	private:
		int _screenWidth, _screenHeight;
		bool _needsTransformUpdate;
		float _scale;
		glm::vec2 _position;
		glm::mat4 _orthoMatrix;
		glm::mat4 _cameraMatrix;
	};
}