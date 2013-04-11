#include "matrixmath.h"

Matrixf vv2matrix(const std::vector<Vector3>& points) {
        size_t n = points.size();
        Matrixf M(n,3);
        for( size_t i = 0; i < n; ++i)
                for( size_t j = 0; j < 3; ++j)
                        M(i, j ) = points[i][j];
        return M;
} // dump a vector of Vector3 to a matrix
