/*
 * desc.h
 *
 *  Created on: Mar 12, 2011
 *      Author: xiaochuanq
 */

#ifndef DESC_H_
#define DESC_H_
#include <vector>
#include "Math/OgreMath.h"
#include "Math/matrixmath.h"

using std::vector;

class Vector3;
class NeuronModel;
class Descriptor;

Matrixf vv2matrix( const vector<Vector3>& points ); // dump a vector of Vector3 to a matrix
bool is_comparable( const Descritpor&, const Descriptor& );
float distance( const Descritpor&, const Descriptor& );
bool inregion( const Vector3& shpercoord);


struct Parameter{
	int k;
	int nz;
	int na;
	int brate;
	// function ptr weighting function
};

class Descriptor
{
public:
	Descriptor(int nz, int na, const vector<float> rlimits);
	~Descriptor();
	void create();
	void reset();
public:
	bool is_blurred() { return m_bBlur; }
	bool empty() { return m_vHistPtr.empty(); }
	int dimension() { return m_vecDimSize.size(); }
	int size( int dim ) { return m_vecDimSize[dim]; }
	const vector<int> & size() { return m_vecDimSize(); }
	void extract( const NeuronModel&, float bi_weigth = 1.0f, bool blur = true);
private:
	void calcZenith();
	void alignZenith();
	void card2Polar();
private:
	bool m_bBlur;
	vector<float> m_vfZenithBins;
	vector<float> m_vfAzimuthBins;
	vector<float> m_vfRadiusBins;
	vector<int> m_vecDimSize;
    Vector3 m_v3zenith;
    vector<Matrixf*> m_vHistPtr;
};

#endif /* DESC_H_ */
