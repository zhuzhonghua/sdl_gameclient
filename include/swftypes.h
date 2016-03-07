#ifndef __SWFTYPES_H__
#define __SWFTYPES_H__

#include "types.h"

namespace SWFTAG
{
	enum TAG
	{
		END = 0,
		SHOWFRAME = 1,
		DEFINESHAPE = 2,
		SETBACKGROUNDCOLOR = 9,
		DEFINEBITSJPEG2 = 21,
		PLACEOBJECT2 = 26,
		FILEATTRIBUTE = 69,
	};
};
//
// rect: rectangle type, used by render handler
//
class Loader;
class SDL_Rect;

namespace swftypes
{
	struct point;
	struct rect;
	struct matrix
	{
		float	m_[2][3];

		static matrix	identity;

		matrix();
		void	set_identity();
		void	concatenate(const matrix& m);
		void	concatenate_translation(float tx, float ty);
		void	concatenate_scale(float s);
		void	set_lerp(const matrix& m1, const matrix& m2, float t);
		void	set_scale_rotation(float x_scale, float y_scale, float rotation);
		void	read(Loader* in);
		void	print() const;
		void	transform(point* result, const point& p) const;
		void	transform(rect* bound) const;
		void	transform_vector(point* result, const point& p) const;
		void	transform_by_inverse(point* result, const point& p) const;
		void	transform_by_inverse(rect* bound) const;
		void	set_inverse(const matrix& m);
		bool	does_flip() const;	// return true if we flip handedness
		float	get_determinant() const;	// determinant of the 2x2 rotation/scale part only
		float	get_max_scale() const;	// return the maximum scale factor that this transform applies
		float	get_x_scale() const;	// return the magnitude scale of our x coord output
		float	get_y_scale() const;	// return the magnitude scale of our y coord output
		float	get_rotation() const;	// return our rotation component (in radians)

		bool operator==(const matrix& m) const
		{
			return
				m_[0][0] == m.m_[0][0] &&
				m_[0][1] == m.m_[0][1] &&
				m_[0][2] == m.m_[0][2] &&
				m_[1][0] == m.m_[1][0] &&
				m_[1][1] == m.m_[1][1] &&
				m_[1][2] == m.m_[1][2];
		}

		bool operator!=(const matrix& m) const
		{
			return !(*this == m);
		}

	};
	struct point
	{
		float	m_x, m_y;

		point() : m_x(0), m_y(0) {}
		point(float x, float y) : m_x(x), m_y(y) {}

		void	set_lerp(const point& a, const point& b, float t)
			// Set to a + (b - a) * t
		{
			m_x = a.m_x + (b.m_x - a.m_x) * t;
			m_y = a.m_y + (b.m_y - a.m_y) * t;
		}

		bool operator==(const point& p) const { return m_x == p.m_x && m_y == p.m_y; }

		bool	bitwise_equal(const point& p) const;

		float get_length() const;

		void twips_to_pixels();
		void pixels_to_twips();
	};

	struct rect
	{
		float	m_x_min, m_x_max, m_y_min, m_y_max;

		rect() :
			m_x_min(0.0f),
			m_x_max(0.0f),
			m_y_min(0.0f),
			m_y_max(0.0f)
		{
		}

		void	copy(SDL_Rect* size);
		void	read(Loader* in);
		void	print() const;
		bool	point_test(float x, float y) const;
		bool	bound_test(const rect& bound) const;
		void	set_to_point(float x, float y);
		void	set_to_point(const point& p);
		void	expand_to_point(float x, float y);
		void	expand_to_point(const point& p);
		void	expand_to_rect(const rect& r);
		float	width() const { return m_x_max - m_x_min; }
		float	height() const { return m_y_max - m_y_min; }

		point	get_corner(int i) const;

		void	enclose_transformed_rect(const matrix& m, const rect& r);

		void	set_lerp(const rect& a, const rect& b, float t);

		void twips_to_pixels();
		void pixels_to_twips();
	};

	struct rgba
	{
		UInt8	_r, _g, _b, _a;

		rgba() : _r(255), _g(255), _b(255), _a(255) {}

		rgba(Uint8 r, Uint8 g, Uint8 b, Uint8 a) :
			_r(r), _g(g), _b(b), _a(a)
		{
		}

		// For convenience.
		rgba(double color)
		{
			set(color);
		}

		void	read(Loader* in, int tag_type);
		void	read_rgba(Loader* in);
		void	read_rgb(Loader* in);

		void	set(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
		{
			_r = r;
			_g = g;
			_b = b;
			_a = a;
		}

		void	set(double color)
		{
			int rgb = int(color);
			_r = (Uint8)(rgb >> 16) & 0xFF;
			_g = (Uint8)(rgb >> 8) & 0xFF;
			_b = (Uint8)rgb & 0xFF;
			_a = 255;
		}

		void	set_lerp(const rgba& a, const rgba& b, float f);

		void	print();
	};
}
#endif