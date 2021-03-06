#ifndef __STDAFX_H__
#define __STDAFX_H__

#define NULL 0

#define ASSERT(expr) do {if(!(expr)){ std::printf("assert fail! %s:%d, %s", __FILE__, __LINE__, #expr);*(int*)0 = 0; }} while (0);
#define INT_MAX_VALUE 0x3FFFFFFF

#include "GL/glew.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "regex/pcrecpp.h"
#include "util.h"
//#include <GL/gl.h>
//#include <GL/glu.h>

#include <stddef.h>
#include <stdio.h>
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <map>

static inline void fatalError(std::string info)
{
	std::cout << info << std::endl;
	std::cout << "Enter Any Key To Exit" << std::endl;
	int tmp;
	std::cin >> tmp;
	SDL_Quit();
	exit(1);
}

typedef unsigned char byte;

typedef glm::mat4 Matrix;
typedef glm::vec2 Point;

#include "rect.h"

#endif // !__STDAFX_H__
