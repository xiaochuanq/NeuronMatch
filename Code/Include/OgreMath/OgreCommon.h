#ifndef OGRECOMMON_H
#define OGRECOMMON_H

#define _OgreExport

#include <memory.h>
#include <iostream>
#include <cassert>
#include <cstdlib>

#define _USE_MATH_DEFINES
#include <cmath>
#define TWO_PI (M_PI*2)
#define HALF_PI (M_PI/2)
namespace Ogre
{
	class Vector2;
	class Vector3;
	class Vector4;
	class Matrix3;
	class Matrix4;
	class Quaternion;
}

inline float isqrtf(float x)
{
	return 1/sqrtf(x);
}

const float eps = 1e-6f;
const float inf = 1e10f;
const float delta = 1e-3f;
const float pi = 3.1415927f;
const float half_pi = 1.570796327f;
const float two_pi = 6.2831853072f;

#endif
