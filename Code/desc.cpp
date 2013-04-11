/*
 * desc.cpp
 *
 *  Created on: Mar 12, 2011
 *      Author: xiaochuanq
 */
#include <memory>
#include <sstream>
#include <cassert>
#include "desc.h"
#include "dataio.h"
#include "neuron.h"
#include "myexception.hpp"


Matrixf vv2matrix(const vector<Vector3>& points) {
	size_t n = points.size();
	Matrixf M(n,3);
	for( size_t i = 0; i < n; ++i)
		for( size_t j = 0; j < 3; ++j)
			M(i, j ) = points[i][j];
	return M;
} // dump a vector of Vector3 to a matrix

bool is_comparable( const Descriptor & desc1, const Descriptor & desc2){
	return !desc1.empty() && !desc2.empty()
			&& desc1.size_a() == desc2.size_a() && desc1.size_z() == desc2.size_z();
}

inline float chi_diff(float f1, float f2)
{
	assert( f1 > 0 && f2 > 0); // rule out invalid inputs which make f1 + f2 == 0
	if( f1 < eps && f2 < eps)
		return 0.0f;
	float diff = f1 - f2;
	diff *= diff;
	return diff / (f1 + f2);
}

float distance( const Descriptor & desc1, const Descriptor & desc2){
	if( is_comparable( desc1, desc2))
		return -1.0f;
	// check comparability before calling this function;
	size_t ns = min( desc1.width(), desc2.width());
	size_t nl = max( desc1.width(), desc2.width());
	const Descriptor& desc_longer = desc1.width() > desc2.width() ? desc1 : desc2;
	const Descriptor& desc_shorter = desc1.width() > desc2.width() ? desc2 : desc1;
	vector<float> sum( desc1.page(), 0.0f);
	float mindiff = inf;
	size_t height = desc1.height();
	for( size_t k = 0; k < desc1.page(); ++k)
	{ // k: page # of rotated descriptors
		for( size_t i = 0; i < height; ++i){ //i, iterates all zenith and azimuth angle bins
			size_t j = 0;
			for( ; j < ns; ++j) //j iterates all radius bins
				sum[k] += chi_diff( desc_longer(i, j, 0), desc_shorter(i,j, k));
			for( ; j < nl; ++j)
				sum[k] += chi_diff( desc_longer(i, j, 0 ), 0);
		}
		mindiff = min( sum[k], mindiff);
	}
	return mindiff;
}

ostream& operator <<( ostream& os, DescParameter& p)
{
	os << "Variable:\tnz\tna\tnr\tdz\tda\tdr\tbrate"<<endl;
	os << "   Value:\t"<<p.nz<<"\t"<<p.na<<"\t"<<p.nr<<"\t"
			<<p.dz<<"\t"<<p.da<<"\t"<<p.dr<<"\t"<<p.brate;
	return os;
}

ostream& operator <<( ostream& os, SphericalMesh& s)
{
	for( size_t i = 0; i < s.zenithLimits.size(); ++i){
		cout << s.zenithLimits[i].first <<"\t";
	}
	cout << endl;
	for( size_t i = 0; i < s.zenithLimits.size(); ++i){
		cout << s.zenithLimits[i].second <<"\t";
	}
	cout << endl;
	for( size_t i = 0; i < s.azimuthLimits.size(); ++i){
		cout << s.azimuthLimits[i].first <<"\t";
	}
	cout << endl;
	for( size_t i = 0; i < s.azimuthLimits.size(); ++i){
		cout << s.azimuthLimits[i].second <<"\t";
	}
	cout << endl;
	for( size_t i = 0; i < s.radiusLimits.size(); ++i){
		cout << s.radiusLimits[i].first <<"\t";
	}
	cout << endl;
	for( size_t i = 0; i < s.radiusLimits.size(); ++i){
		cout << s.radiusLimits[i].second <<"\t";
	}
	return os;
}

/////////////////////////////  Class Spherical Mesh ////////////////
void SphericalMesh::create( const DescParameter& param)
{
	zenithLimits.clear();
	azimuthLimits.clear();
	radiusLimits.clear();
	float angle = - half_pi;
	Range rgn;
	for (size_t i = 0; i < param.nz; ++i) {
		rgn.first = angle;
		angle += param.dz;
		rgn.second = angle;
		zenithLimits.push_back( rgn);
	}
	angle = -pi;
	for (size_t i = 0; i < param.na; ++i) {
		rgn.first = angle;
		angle += param.da;
		rgn.second = angle;
		azimuthLimits.push_back(rgn);
	}

	float r = 0;
	for (size_t i = 0; i < param.nr; ++i) {
		rgn.first = r;
		r += param.dr;
		rgn.second = r;
		radiusLimits.push_back( rgn );
	}
}


vector<Index3D>BlurredSphericalMesh:: get_index(const Vector3 &)
{

}

void SphericalMesh::get_index(const Vector3 & sphCoord, size_t& m, size_t& n, size_t& k)
{
	int zi, ai, ri;
	zi = ai = ri = -1;
	// index theta, for definition, check "Vector3.h"
	for (size_t i = 0; i < zenithLimits.size(); ++i) {
		if (sphCoord.x >= zenithLimits[i].first && sphCoord.x
				< zenithLimits[i].second) {
			zi = i;
			break;
		}
	}
	// index phi
	for (size_t i = 0; i < azimuthLimits.size(); ++i) {
		if (sphCoord.y >= azimuthLimits[i].first && sphCoord.y
				< azimuthLimits[i].second) {
			ai = i;
			break;
		}
	}
	// index r
	for (size_t i = 0; i < radiusLimits.size(); ++i) {
		if (sphCoord.z >= radiusLimits[i].first && sphCoord.z
				< radiusLimits[i].second) {
			ri = i;
			break;
		}
	}
	if ( zi < 0 || ai < 0 || ri < 0)
	{
		std::stringstream ss;
		ss << sphCoord;
		throw MyException(ss.str() + "Invalid coordinates or range limits.");
	}
	m = zi; n = ai; k = ri;
}

void BlurredSphericalMesh::blur( const DescParameter& param)
{
	if( param.brate <= eps)
		return;

	float delta = param.dz * param.brate;
	for( size_t i = 0; i < zenithLimits.size(); ++i )
	{
		zenithLimits[i].first -= delta;
		zenithLimits[i].second += delta;
	}
	delta = param.da * param.brate;
	for (size_t i = 0; i < azimuthLimits.size(); ++i) {
		azimuthLimits[i].first -= delta;
		azimuthLimits[i].second += delta;
	}

	delta = param.dr * param.brate;
	for (size_t i = 0; i < radiusLimits.size(); ++i) {
		// this over-complicated computation is for future uneven radius extension
		radiusLimits[i].first -= delta;
		radiusLimits[i].second += delta;
	}
	radiusLimits[0].first = 0.0f; //restrict the origin
}

///////////////////////// Class Descriptor ////////////////////////////
Descriptor::~Descriptor(){
	for_each( m_vHistPtr.begin(), m_vHistPtr.end(), del_fun<Matrixf*>());
}

void Descriptor::extract( const Neuron& neuron)
{
	gatherPoints( neuron );
	Matrixf E = vv2matrix( neuron.get_edges());
	Vector3 zenith = calcZenith( E);
	float rmax = alignZenith(zenith);
	m_Param.update_nr( rmax );
	m_Mesh.create(m_Param);
	create( );
	cout << "created histograms"<<endl;
}
void Descriptor::dump(const char * file, const char* name) const {
	vector<float* > buffer;
	vector<size_t> m(page(), height());
	vector<size_t> n(page(), width());
	for( size_t k = 0; k < page(); ++k){
		buffer.push_back( m_vHistPtr[k]->col_major());
	}
	write_matfile(file, name, m, n, buffer);
	for_each(buffer.begin(), buffer.end(), del_array_fun<float*>());
}

void Descriptor::gatherPoints( const Neuron& n)
{
	m_Points.clear();
	m_Points.assign( n.get_bi_points().begin(), n.get_bi_points().end() );
	m_Points.insert( m_Points.end(), n.get_sa_points().begin(), n.get_sa_points().end() );

	m_Weights.clear();
	m_Weights.assign( n.get_bi_diameters().begin(), n.get_bi_diameters().end() );
	m_Weights.insert(m_Weights.end(), n.get_sa_diameters().begin(), n.get_sa_diameters().end() );
}

Vector3 Descriptor::calcZenith(const Matrixf& E) {
	Matrixf Et = ~E; // E is supposed to be n-by-3, and E transpose is 3-by-n
	Matrixf EtE = Et * E; // Etranspose * E should be 3-by-3
	Matrix3 M( EtE(0,0), EtE(0,1), EtE(0,2),
				EtE(1,0), EtE(1,1), EtE(1,2),
				EtE(2,0), EtE(2,1), EtE(2,2) );
	float eigvalue[3];
	Vector3 eigvector[3];
	M.EigenSolveSymmetric(eigvalue, eigvector);
	// this function always returns positive eigenvalues
	for( int i = 0; i < 3; ++i){
		eigvalue[i] *= eigvector[i].normalise();
	}
	int idx = eigvalue[0] > eigvalue[1] ? 0 : 1;
	idx = eigvalue[idx] > eigvalue[2] ? idx : 2;
	eigvector[idx].normalise();
	return eigvector[idx];
}

void Descriptor::decomposeAxis(const Matrixf&, Vector3& xAxis,  Vector3& yAxis, Vector3& zAxis)
{
	Matrixf Et = ~E; // E is supposed to be n-by-3, and E transpose is 3-by-n
	Matrixf EtE = Et * E; // Etranspose * E should be 3-by-3
	Matrix3 M( EtE(0,0), EtE(0,1), EtE(0,2),
			EtE(1,0), EtE(1,1), EtE(1,2),
			EtE(2,0), EtE(2,1), EtE(2,2) );
	float eigvalue[3];
	Vector3 eigvector[3];
	M.EigenSolveSymmetric(eigvalue, eigvector);
	for( int i = 0; i < 3; ++i){
		eigvalue[i] *= eigvector[i].normalise();
	}
	// Always set y > z > x
	if( eigvalue[0] > eigvalue[1]){ // 0 > 1
		if( eigvalue[1] > eigvalue[2]){//0 > 1 > 2
			yAxis = eigvector[0];
			zAxis = eigvector[1];
			xAxis = eigvector[2];
		}
		else /* 0 > 1, 1 < 2*/
			if( eigvalue[0] < eigvalue[2]){ //2 > 0 > 1
			yAxis = eigvector[2];
			zAxis = eigvector[0];
			xAxis = eigvector[1];
		}
		else{ // 0 > 2 > 1
			yAxis = eigvector[0];
			zAxis = eigvector[2];
			xAxis = eigvector[1];
		}
	}
	else{ // 0 < 1
		if( eigvalue[1] < eigvalue[2]){ // 2 > 1 > 0
			yAxis = eigvector[2];
			zAxis = eigvector[1];
			xAxis = eigvector[0];
		}
		else /* 0<1, 2<1*/
			if( eigvalue[0] < eigvalue[2]){ // 1 > 2 > 0
			yAxis = eigvector[1];
			zAxis = eigvector[2];
			xAxis = eigvector[0];
		}
		else{ // 1 > 0 > 2{
			yAxis = eigvector[1];
			zAxis = eigvector[0];
			xAxis = eigvector[2];
		}
	}
}

// some float * to int * errors below
float Descriptor::alignZenith( const Vector3 & zenith) {
	float cosbeta = zenith.dotProduct( Vector3(0.0, 0.0, 1.0) );
	float beta = asin( sqrt( 1 - cosbeta * cosbeta) );
	float alpha = atan2( zenith.y, zenith.x );
	Matrix3 T1;
	T1.FromAxisAngle(Vector3(0.0f, 0.0f, 1.0f), alpha);
	Matrix3 T2;
	T2.FromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), beta);
	Matrix3 T = T2 * T1;
	int rmax = -1e6;
	for( size_t i = 0; i < m_Points.size(); ++i){
		m_Points[i] = T * m_Points[i];
		m_Points[i].cart2spherical();
		rmax = max( rmax,m_Points[i].z );
	}
	return rmax;
}

void Descriptor::create()
{
	Matrixf* pHist = new Matrixf(height(), width());
	double swt, swb; //summed weight of top half and summed weight of bottom half
	swt = swb = 0;
	size_t zi, ai, ri;
	for( size_t i = 0; i < m_Points.size(); ++i){
		m_Mesh.get_index(m_Points[i], zi, ai, ri);
		(*pHist)( ai * m_Param.nz + zi, ri) += m_Weights[i];
		if( zi <= size_z() / 2)
			swt += m_Weights[i];
		else
			swb += m_Weights[i];
	}
	m_vHistPtr.push_back( pHist);
	if( swt > swb) // always keep
		vFlip();
	rotate();
}

void Descriptor::rotate()
{
	for( size_t k = 1; k < size_a(); ++k ){
		Matrixf* pm = new Matrixf(*m_vHistPtr[k-1]);
		pm->rshrows( size_z() );
		m_vHistPtr.push_back( pm );
	}
}


void Descriptor::vFlip()
{
	int nRow = m_vHistPtr[0]->RowNo();
	for( int i = 0; i < nRow/2; ++i)
		m_vHistPtr[0]->swaprows(i, nRow - i);
}
void Descriptor::hFlip()
{

}
