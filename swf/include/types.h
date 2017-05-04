#ifndef __TYPES_H__
#define __TYPES_H__

#include "SDL_stdinc.h"
typedef unsigned int UInt32;
//typedef UInt32 Uint32;
typedef int SInt32;
//typedef SInt32 Sint32;
typedef unsigned char UInt8;
//typedef UInt8 Uint8;
typedef char SInt8;
//typedef SInt8 Sint8;
typedef unsigned short UInt16;
//typedef UInt16 Uint16;
typedef short SInt16;
//typedef SInt16 Sint16;

#define TWIPS_TO_PIXELS(x)	((x) / 20.f)
#define PIXELS_TO_TWIPS(x)	((x) * 20.f)

template <typename T>
const T Min(const T a, const T b)
{
	return (a < b) ? a : b;
}
template <typename T>
const T Max(const T a, const T b)
{
	return (a < b) ? b : a;
}

inline float sqr(float number) {
	long i;
	float x, y;
	const float f = 1.5F;
	x = number * 0.5F;
	y = number;
	i = *(long *)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float *)&i;
	y = y * (f - (x * y * y));
	y = y * (f - (x * y * y));
	return number * y;
}

inline float	flerp(float a, float b, float f) { return (b - a) * f + a; }
#define Assert(repr) do { if (!(repr)) *(int*)0 = 0; } while (0)

inline int		fchop(float f) { return (int)f; }	// replace w/ inline asm if desired
inline int		frnd(float f) { return fchop(f + 0.5f); }	// replace with inline asm if desired
inline int		iclamp(int i, int min, int max)
{
	Assert(min <= max);
	return Max(min, Min(i, max));
}

inline float	fclamp(float f, float xmin, float xmax) {
	Assert(xmin <= xmax);
	return Max(xmin, Min(f, xmax));
}

#endif // !__TYPES_H__
