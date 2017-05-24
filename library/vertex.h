#pragma once
#include "stdafx.h"
namespace Zhu{
	struct Position
	{
		float x;
		float y;
	};

	struct Color
	{
		Color() :r(0), g(0), b(0), a(0){}
		Color(GLubyte R, GLubyte G, GLubyte B, GLubyte A) :
			r(R), g(G), b(B), a(A){}
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};

	struct UI
	{
		float u;
		float v;
	};

	struct Vertex
	{
		Position position;

		Color color;

		UI uv;

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
		{
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}
		void setPosition(float x, float y)
		{
			position.x = x;
			position.y = y;
		}
		void setUV(float u, float v)
		{
			uv.u = u;
			uv.v = v;
		}
	};
}