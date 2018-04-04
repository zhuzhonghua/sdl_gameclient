#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "gizmo.h"
#include "texture.h"
#include <string>

namespace Zhu{
	class Sprite : public Gizmo
	{
	protected:
		//GLuint texture[1];
		float _x;
		float _y;
		float _width;
		float _height;

		GLuint _vboID;
		Texture _tex;
	public:
		Sprite();
		~Sprite();

		void init(float x, float y, float width, float height, std::string texPath);

		void draw();
	};
}
#endif