/*
 * desc.cpp
 *
 *  Created on: Mar 12, 2011
 *      Author: xiaochuanq
 */
#include "desc.h"
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
	return desc1.size(0) == desc2.size(0);
}

float distance( const Descriptor & desc1, const Descriptor & desc2){
	return 0.0f;
}
/////////////////////////////  Class Spherical Mesh ////////////////
int SphericalMesh::create( const Parameter& param, float rmax)
{
	zenithLimits.clear();
	azimuthLimits.clear();
	radiusLimits.clear();
	float angle = - half_pi;
	float delta = pi / param.nz;
	Range rgn;
	for (int i = 0; i < param.nz; ++i) {
		rgn.first = angle;
		angle += delta;
		rgn.second = angle;
		zenithLimits.push_back( rgn);
	}
	angle = -pi;
	delta = two_pi / param.na;
	for (int i = 0; i < param.na; ++i) {
		rgn.first = angle;
		angle += delta;
		rgn.second = angle;
		azimuthLimits.push_back(rgn);
	}
	int nr = std::ceil( rmax / param.dr);
	float r = 0;
	for (int i = 0; i < nr; ++i) {
		rgn.first = r;
		r += param.dr;
		rgn.second = r;
		radiusLimits.push_back( rgn );
	}
	if( param.brate > eps)
		blur( param.brate );
	return nr;
}

void SphericalMesh::get_index(const Vector3 & sphCoord, int& zi, int& ai, int& ri)
{
	zi = ai = ri = -1;
	for (size_t i = 0; i < zenithLimits.size(); ++i) {
		if (sphCoord.y >= zenithLimits[i].first && sphCoord.y
				< zenithLimits[i].second) {
			zi = i;
			break;
		}
	}
	for (size_t i = 0; i < azimuthLimits.size(); ++i) {
		if (sphCoord.x >= azimuthLimits[i].first && sphCoord.x
				< azimuthLimits[i].second) {
			ai = i;
			break;
		}
	}
	for (size_t i = 0; i < radiusLimits.size(); ++i) {
		if (sphCoord.z >= radiusLimits[i].first && sphCoord.z
				< radiusLimits[i].second) {
			ri = i;
			break;
		}
	}
	if ( zi < 0 || ai < 0 || ri < 0)
		throw MyException("Invalid coordinates or range limits.");
}

void SphericalMesh::blur( float rate)
{
	size_t zsz = zenithLimits.size();
	size_t asz = azimuthLimits.size();
	size_t rsz = radiusLimits.size();
	float piXrate = pi * rate;
	float dz = piXrate / zsz;
	float da = piXrate / asz;
	for( size_t i = 0; i < zsz; ++i )
	{
		zenithLimits[i].first -= dz;
		zenithLimits[i].second += dz;
	}
	for (size_t i = 0; i < asz; ++i) {
		azimuthLimits[i].first -= da;
		azimuthLimits[i].second += da;
	}
	float dr;
	for (size_t i = 0; i < rsz; ++i) {
		dr = ( radiusLimits[i].second - radiusLimits[i].first)* rate;
		// this over-complicated computation is for future uneven radius extension
		radiusLimits[i].first -= dr;
		radiusLimits[i].second += dr;
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
	int nr = m_Mesh.create(m_Param, rmax);
	m_Size[0] = m_Param.na * m_Param.nz; // # of rows
	m_Size[1] = nr; //# of cols
	m_Size[2] = m_Param.na; //# of pages
	createHist( );
}

void Descriptor::gatherPoints( const Neuron& n)
{
	m_Points.clear();
	m_Points.assign(n.get_bi_points().begin(), n.get_bi_points().end() );
	m_Points.insert( m_Points.end(), n.get_sa_points().begin(), n.get_sa_points().end() );
	m_Weights.clear();
	m_Weights.assign( n.get_bi_diameters().begin(), n.get_bi_diameters().end() );
	m_Weights.insert( m_Weights.end(), n.get_sa_diameters().begin(), n.get_sa_diameters().end() );
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
	int idx = eigvalue[0] > eigvalue[1] ? 0 : 1;
	idx = eigvalue[idx] > eigvalue[2] ? idx : 2;
	eigvector[idx].normalise();
	return eigvector[idx];
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
		m_Points[i].card2spherical();
		rmax = max( rmax,m_Points[i].z );
	}
	return rmax;
}

void Descriptor::createHist()
{
	Matrixf* pHist = new Matrixf(m_Size[0], m_Size[1]);
	double swt, swb; //summed weight of top half and summed weight of bottom half
	swt = swb = 0;
	int zi, ai, ri;
	for( size_t i = 0; i < m_Points.size(); ++i){
		m_Mesh.get_index(m_Points[i], zi, ai, ri);
		float weight = m_Weights[i];
		(*pHist)( ai * m_Param.nz + zi, ri) += weight;
		if( zi <= m_Param.nz/2)
			swt += weight;
		else
			swb += weight;
	}
	m_vHistPtr.push_back( pHist);
	if( swt > swb) // always keep
		flipHist();
	rotateHist();
}

void Descriptor::flipHist()
{
	int nRow = m_vHistPtr[0]->RowNo();
	for( int i = 0; i < nRow/2; ++i)
		m_vHistPtr[0]->swaprows(i, nRow - i);
}

void Descriptor::rotateHist()
{
	for( int k = 1; k < m_Param.na; ++k ){
		Matrixf* pm = new Matrixf(*m_vHistPtr[k-1]);
		pm->rshrows(m_Param.nz);
		m_vHistPtr.push_back( pm );
	}
}


