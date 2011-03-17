#include <cmath>
#include <iostream>
#include "matrixmath.h"
using namespace std;

int main(void) {
	cout << "---1--- Testing rotation row shift of a matrix" << endl;
	Matrixf m1(12, 4);
	for (size_t i = 0; i < m1.RowNo(); ++i)
		for (size_t j = 0; j < m1.ColNo(); ++j)
			m1(i, j) = i;
	cout << "Original M1" << endl;
	cout << m1 << endl;
	cout << "M1 down 0" << endl;
	m1.rshrows(0);
	cout << m1 << endl;
	cout << "M1 down 1" << endl;
	m1.rshrows(1);
	cout << m1 << endl;
	cout << "M1 down 2" << endl;
	m1.rshrows(2);
	cout << m1 << endl;
	cout << "M1 down 5" << endl;
	m1.rshrows(5);
	cout << m1 << endl;
	cout << "M1 down 12" << endl;
	m1.rshrows(12);
	cout << m1 << endl << endl;

	Matrixf m11(m1);
	cout <<"Cloning as M11" << endl;
	m11.rshrows(0);
	cout << m11 << endl;
	cout << "M11 up 1" << endl;
	m11.rshrows(-1);
	cout << m11 << endl;
	cout << "M11 up  2" << endl;
	m11.rshrows(-2);
	cout << m11 << endl;
	cout << "M11 up 5" << endl;
	m11.rshrows(-5);
	cout << m11 << endl;
	cout << "M11 up 12" << endl;
	m11.rshrows(-12);
	cout << m11 << endl << endl;

	cout << " also check if m1 is cloned correctly" << endl;
	cout << m1;

	cout << "---2---Testing Column major and row major dump." << endl;
	Matrixf m2(3, 4);
	float t = 0.0f;
	for (size_t i = 0; i < m2.RowNo(); ++i)
		for (size_t j = 0; j < m2.ColNo(); ++j)
			m2(i, j) = (t += 1.0);
	cout << m2 << endl;
	size_t N = m2.RowNo() * m2.ColNo();
	float *p = m2.col_major();
	for (size_t i = 0; i < N; ++i) {
		cout << *(p + i) << " ";
	}
	cout << endl;
	delete[] p;
	float* q = m2.row_major();
	for (size_t i = 0; i < N; ++i) {
		cout << *(q + i) << " ";
	}
	cout <<endl;
	delete[] q;

	cout << "---3---Testing swap of rows of matrix" << endl;
	cout << m2 << endl << endl;
	m2.swaprows(0, 2);
	cout << m2 << endl << endl;
	m2.swaprows(1, 2);
	cout << m2 << endl << endl;
	cout << " check same row swap" << endl;
	m2.swaprows(2, 2);
	cout << m2 << endl << endl;
	cout << "also check invalid row swap" << endl;
	m2.swaprows(3, 3);
	cout << m2 << endl << endl;
	m2.swaprows(4, 3);
	cout << m2 << endl;

	cout <<endl <<"---4--- Test transpose and multiplication and eigenvector()" <<endl;
	cout <<" M3 = M2'" <<endl;
	Matrixf m3 = ~m2;
	cout << m2<<endl;
	cout << m3<<endl;
	cout <<" M1*M3"<<endl;
	cout << m1*m3<<endl;
	cout <<" M4 = M2 * M3" <<endl;
	Matrixf m4( m2 * m3);
	cout << m4 <<endl;

	cout <<"Compute M4's eigenvector and values" <<endl;
	Matrix3 m5;
	for( int i = 0; i < 3; ++i)
		for(int j =0; j < 3; ++j)
			m5[i][j]= m4(i,j);
	float eigval[3];
	Vector3 eigvec[3];
	m5.EigenSolveSymmetric(eigval, eigvec);
	// val is reused here. not a very good practice, but, i'm lazy
	cout <<"Three eigenvalues: " << endl;
	for( int i =0; i < 3; ++ i)
		cout << eigval[i] <<"\t";
	cout << endl <<"Three eigenvectors:"<<endl;
	for( int i = 0; i < 3; ++i)
		cout << eigvec[i]<<endl;

	return 0;
}
