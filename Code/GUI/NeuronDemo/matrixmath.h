/*
 * matrixmath.h
 *
 *  Created on: Mar 13, 2011
 *      Author: xiaochuanq
 */

#ifndef MATRIXMATH_H_
#define MATRIXMATH_H_

#include <vector>
#include "OgreMath/OgreMath.h"
#include "matrix.hpp"
using namespace matrixmath;

typedef matrix<double> Matrixd;
typedef matrix<float> Matrixf;
typedef matrix<bool> Matrixb;
typedef matrix<int> Matrixi;
typedef matrix<long> Matrixl;
typedef matrix<unsigned int> Matrixu;

Matrixf vv2matrix(const std::vector<Vector3>&);
#endif /* MATRIXMATH_H_ */
