/*
 * mathematics.cpp
 *
 *  Created on: Mar 12, 2011
 *      Author: xiaochuanq
 */

#include "mathematics.h"
template< typename T>
void matrixmul(T* C, const T* A, const T* B, unsigned int hA, unsigned int wA, unsigned int wB)
{

    for (unsigned int i = 0; i < hA; ++i)
        for (unsigned int j = 0; j < wB; ++j) {
            T sum = 0;
            for (unsigned int k = 0; k < wA; ++k) {
                T a = A[i * wA + k];
                T b = B[k * wB + j];
                sum += a * b;
            }
            C[i * wB + j] = sum;
        }
}
