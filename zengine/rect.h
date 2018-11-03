#pragma once
#include "glm/glm.hpp"
#include <algorithm>

namespace Zengine{
struct Rect{
	// left = x
	// top = y
	// right = z
	// bottom = w
	glm::vec4 v;

	Rect(){}

	Rect(const Rect& r){ v = r.v; }
	Rect(int l, int t, int r, int b){ v = glm::vec4(l, t, r, b); }
	
	int left() const { return v.x; }
	int top()  const { return v.y; }
	int right() const { return v.z; }
	int bottom()  const { return v.w; }
	int width() const { return std::abs(v.z - v.x); }
	int height() const { return std::abs(v.y - v.w); }
	int square() const { return width()*height(); }
	bool isEmpty() const { return width() <= 0 || height() <= 0; }
	void setEmpty() { v = glm::vec4(0, 0, 0, 0); }

	Rect intersect(const Rect& o) {
		int l = std::max(this->left(), o.left());
		int t = std::min(this->top(), o.top());
		int r = std::min(this->right(), o.right());
		int b = std::max(this->bottom(), o.bottom());

		return Rect(l,t,r,b);
	}

	bool inside(Point p) {
		return p.x >= left() && p.x < right() && p.y >= bottom() && p.y < top();
	}

	Rect shrink(int d) {
		return Rect(left() + d, top() - d, right() - d, bottom() + d);
	}

	Rect shrink() {
		return shrink(1);
	}
};
};