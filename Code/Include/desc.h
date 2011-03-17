/*
 * desc.h
 *
 *  Created on: Mar 12, 2011
 *      Author: xiaochuanq
 */

#ifndef DESC_H_
#define DESC_H_

#include <vector>
#include <utility>
#include <iostream>
#include "del_functor.hpp"
#include "matrixmath.h"
using std::vector;
using std::pair;

class Descriptor;
class SphericalMesh;

Matrixf vv2matrix(const vector<Vector3>& points); // dump a vector of Vector3 to a matrix
bool is_comparable(const Descriptor &, const Descriptor &);
float distance(const Descriptor &, const Descriptor &);

class Parameter {
public:
	friend class Descriptor;
	friend class SphericalMesh;
	Parameter(size_t z, size_t a, float r, float b = 0) :
		nz(z), na(a), nr(0), dr(r), dz(pi/nz), da(two_pi/na), brate(b) {};
	friend ostream& operator <<( ostream&, Parameter&);
	void update_nr(float rmax){ nr = std::ceil( rmax / dr); }
public:
	size_t nz;
	size_t na;
	size_t nr;
	float dr;
	float dz;
	float da;
	float brate;
	// TODO: add function ptr to weighting function
};

typedef pair<float, float> Range;
struct SphericalMesh {
	friend ostream& operator <<( ostream&, SphericalMesh&);
	vector<Range> zenithLimits;
	vector<Range> azimuthLimits;
	vector<Range> radiusLimits;
	void create(const Parameter&);
	void get_index(const Vector3 &, size_t&, size_t&, size_t&);
	void blur(const Parameter&);
};

class Neuron;
class Descriptor {
public:
	Descriptor(const Parameter& param) :m_Param(param){	};
	~Descriptor();
public:
	float operator()(size_t i, size_t j, size_t k) const {
		return m_vHistPtr[k]->operator ()(i, j);
	}
//	float& operator()(size_t i, size_t j, size_t k) {
//		return m_vHistPtr[k]->operator ()(i, j);
//	}
	bool is_blurred() const {
		return m_Param.brate > eps;
	}
	bool empty() const {
		return m_vHistPtr.empty();
	}
	void dump(const char * file, const char* name) const;

	size_t size_a() const { return m_Param.na; }
	size_t size_z() const { return m_Param.nz; }
	size_t page() const { return m_Param.na; }
	size_t height() const { return m_Param.na * m_Param.nz; }
	size_t width() const { return m_Param.nr; }

	void extract(const Neuron&);
private:
	Vector3 calcZenith(const Matrixf&);
	float alignZenith(const Vector3&);
	void gatherPoints(const Neuron&);
	//void calcWeights() { };
	void createHist();
	void flipHist();
	void rotateHist();
private:
	Parameter m_Param; // parameters
	SphericalMesh m_Mesh;//shape context mesh
	vector<Vector3> m_Points; // all points collected to be used in the shape context descriptor
	vector<float> m_Weights; // weights calculated corresponding to all the points above.
	vector<Matrixf*> m_vHistPtr;
};

ostream& operator <<( ostream&, Parameter&);
ostream& operator <<( ostream&, SphericalMesh&);

#endif /* DESC_H_ */
