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
#include "del_functor.hpp"
#include "Math/OgreMath.h"
#include "Math/matrixmath.h"
using std::vector;
using std::pair;

class Descriptor;
Matrixf vv2matrix( const vector<Vector3>& points ); // dump a vector of Vector3 to a matrix
bool is_comparable( const Descriptor &, const Descriptor & );
float distance( const Descriptor &, const Descriptor & );

struct Parameter{
	int nz;
	int na;
	float dr;
	float brate;
	// TODO: add function ptr to weighting function
};

typedef pair<float, float> Range;
struct SphericalMesh{
	vector<Range> zenithLimits;
	vector<Range> azimuthLimits;
	vector<Range> radiusLimits;
	int create( const Parameter&, float);
	void get_index(const Vector3 & , int& , int& , int& );
	void blur( float rate);
};

class Neuron;
class Descriptor
{
public:
	Descriptor( const Parameter& param)
		: m_Param( param) , m_Size( vector<int>(3, 0) ) {};
	~Descriptor();
public:
	bool is_blurred() const { return m_Param.brate > eps; }
	bool empty() const { return m_vHistPtr.empty(); }
	void dump( const char * file = 0) const {};
	int dimension() const { return m_Size.size(); }
	size_t size( size_t dim ) const { return dim < m_Size.size() ? m_Size[dim] : 0; }
	void extract( const Neuron&);
private:
	Vector3 calcZenith(const Matrixf&);
	float alignZenith(const Vector3&);
	void gatherPoints(const Neuron& );
	void calcWeights(){};
	void createHist();
	void flipHist();
	void rotateHist();
private:
	Parameter m_Param; // parameters
	SphericalMesh m_Mesh;//shape context mesh
	vector<int> m_Size; // the descriptor size
	vector<Vector3> m_Points; // all points collected to be used in the shape context descriptor
	vector<float> m_Weights; // weights calculated corresponding to all the ponts above.
    vector<Matrixf*> m_vHistPtr;
};

#endif /* DESC_H_ */
