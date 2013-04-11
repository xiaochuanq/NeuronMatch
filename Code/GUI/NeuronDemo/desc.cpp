/*
 * desc.cpp
 *
 *  Created on: Mar 12, 2011
 *      Author: xiaochuanq
 */
#include <memory>
#include <sstream>
#include <cassert>
#include <algorithm>
#include "desc.h"
#include "dataio.h"
#include "neuron.h"
#include "myexception.hpp"


bool is_comparable( const Descriptor & desc1, const Descriptor & desc2){
	return !desc1.empty() && !desc2.empty()
                        && desc1.size_a() == desc2.size_a()
                        && desc1.size_z() == desc2.size_z()
                        && desc1.size_r() == desc2.size_r()
                        && desc1.page() == desc2.page();
}

bool is_comparable2( const Descriptor & desc1, const Descriptor & desc2){
        return !desc1.empty() && !desc2.empty()
                        && desc1.size_a() == desc2.size_a()
                        && desc1.size_z() == desc2.size_z();
}


float chi_diff(float f1, float f2)
{
    assert( f1 >= 0 && f2 >= 0); // rule out invalid inputs which make f1 + f2 == 0
    if( f1 < 1e-12 && f2 < 1e-12)
        return 0.0f;
    float diff = f1 - f2;
    diff *= diff;
    return diff / (f1 + f2);
}
float abs_diff(float f1, float f2)
{
    return abs(f1 - f2);
}
float square_diff(float f1, float f2)
{
    float f = f1 - f2;
    return f*f;
}

float difference(const Descriptor & desc1, const Descriptor & desc2, Descriptor& desc, float (*diff) (float, float) ){
    size_t idx = 0;
    float mindiff = inf;
    size_t height = desc1.height();
    size_t width = desc1.width();
    float sum;
    for( size_t k = 0; k < desc1.page(); ++k)
    { // k: page # of flipped/rotated descriptors
        sum = 0.0f;
        for( size_t i = 0; i < height; ++i){ //i, iterates all zenith and azimuth angle bins
            for( size_t j = 0; j < width; ++j) //j iterates all radius bins
                sum += diff( desc1(i, j, 0), desc2(i,j, k));
        }
        if( mindiff > sum){
            idx = k;
            mindiff = sum;
        }
    }
    desc.clear();
    Matrixf *pm = new Matrixf( *desc2.getPage( idx ) );
    pm->clone();
    *pm -= *desc1.getPage(0);
  //  pm->getAbsoluteValue();
    desc.m_vHistPtr.push_back( pm );
    desc.m_na = desc1.size_a();
    desc.m_nz = desc1.size_z();
    desc.m_nr = desc1.size_r();
    return mindiff / (desc1.sumWeight() + desc2.sumWeight());
}

float  difference(const Descriptor* desc1, const Descriptor* desc2, Descriptor& desc, float (*diff) (float, float) )
{
    // assuming desc is valid
    if( desc2 == 0){
        desc.clear();
        desc.m_na = desc1->size_a();
        desc.m_nz = desc1->size_z();
        desc.m_nr = desc1->size_r();
        Matrixf *pm = new Matrixf( * desc1->getPage(0)  );
        pm->clone();
        desc.m_vHistPtr.push_back(pm);
        return 0.0f;
    }
    else if( desc1 == desc2 ){
        desc.clear();
        desc.m_na = desc1->size_a();
        desc.m_nz = desc1->size_z();
        desc.m_nr = desc1->size_r();
        Matrixf *pm = new Matrixf( desc.m_na * desc.m_nz, desc.m_nr  );
        pm->setAllZeros();
        desc.m_vHistPtr.push_back(pm);
        return 0.0f;
    }
    else
        return difference(*desc1, *desc2, desc, diff);
}

float distance(const Descriptor &desc1, const Descriptor &desc2, float (*diff) (float, float) ){
    // assuming they are comparable
 //   float mindiff = inf;
    size_t height = desc1.height();
    size_t width = desc1.width();
    vector<float> sum( desc1.page(), 0.0f);
    for( size_t k = 0; k < desc1.page(); ++k)
    { // k: page # of flipped/rotated descriptors
            for( size_t i = 0; i < height; ++i){ //i, iterates all zenith and azimuth angle bins
                    for( size_t j = 0; j < width; ++j) //j iterates all radius bins
                            sum[k] += diff( desc1(i, j, 0), desc2(i,j, k));
            }
      //      mindiff = min( sum[k], mindiff);
    }
    return *std::min_element(sum.begin(), sum.end()) /( desc1.sumWeight()+desc2.sumWeight());
}

float distance2( const Descriptor & desc1, const Descriptor & desc2, float (*diff) (float, float) ){
        if( is_comparable2( desc1, desc2))
		return -1.0f;
	// check comparability before calling this function;
	size_t ns = min( desc1.width(), desc2.width());
	size_t nl = max( desc1.width(), desc2.width());
	const Descriptor& desc_longer = desc1.width() > desc2.width() ? desc1 : desc2;
	const Descriptor& desc_shorter = desc1.width() > desc2.width() ? desc2 : desc1;
	vector<float> sum( desc1.page(), 0.0f);
//	float mindiff = inf;
	size_t height = desc1.height();
	for( size_t k = 0; k < desc1.page(); ++k)
	{ // k: page # of rotated descriptors
		for( size_t i = 0; i < height; ++i){ //i, iterates all zenith and azimuth angle bins
			size_t j = 0;
			for( ; j < ns; ++j) //j iterates all radius bins
                                sum[k] += diff( desc_longer(i, j, 0), desc_shorter(i,j, k));
			for( ; j < nl; ++j)
                                sum[k] += diff( desc_longer(i, j, 0 ), 0);
		}
        //	mindiff = min( sum[k], mindiff);
	}
        return *std::min_element(sum.begin(), sum.end()) / (desc1.sumWeight() + desc2.sumWeight() );
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

SphericalMesh::SphericalMesh( size_t nz, size_t na, size_t nr,
                              bool uselog, float brate){
    create(nz, na, nr, uselog);
    blur(brate);
}

void SphericalMesh::create(size_t nz, size_t na, size_t nr, bool uselog){
    zenithLimits.clear();
    azimuthLimits.clear();
    radiusLimits.clear();
    float angle = - half_pi;
    float dz = pi / nz;
    Range rgn;
    for (size_t i = 0; i < nz; ++i) {
            rgn.first = angle;
            angle += dz;
            rgn.second = angle;
            zenithLimits.push_back( rgn);
    }
    angle = -pi;
    float da = 2 * pi / na;
    for (size_t i = 0; i < na; ++i) {
            rgn.first = angle;
            angle += da;
            rgn.second = angle;
            azimuthLimits.push_back(rgn);
    }
    if( uselog){
        for( int r = -nr+1; r <= 0; ++r){
            rgn.second = exp(r);
            radiusLimits.push_back(rgn);
        }
        for( int i = 2; i < radiusLimits.size(); ++i)
            radiusLimits.at(i).first = radiusLimits.at(i-1).second;
        radiusLimits.front().first = 0.0f;
    }
    else{
        float r = 0;
        float dr = 1.0 / nr;
        for (size_t i = 0; i < nr; ++i) {
                rgn.first = r;
                r += dr;
                rgn.second = r;
                radiusLimits.push_back( rgn );
        }
    }
}

void SphericalMesh::blur( float brate)
{
        if( brate <= eps)
                return;

        float delta = (zenithLimits.front().second -
                       zenithLimits.front().first ) * brate;
        for( size_t i = 0; i < zenithLimits.size(); ++i )
        {
                zenithLimits[i].first -= delta;
                zenithLimits[i].second += delta;
        }

        delta = ( azimuthLimits.front().second -
                  azimuthLimits.front().first) * brate;
        for (size_t i = 0; i < azimuthLimits.size(); ++i) {
                azimuthLimits[i].first -= delta;
                azimuthLimits[i].second += delta;
        }

        for (size_t i = 0; i < radiusLimits.size(); ++i) {
            delta = (radiusLimits[i].second - radiusLimits[i].first) * brate;
                radiusLimits[i].first -= delta;
                radiusLimits[i].second += delta;
        }
}


SphericalMesh::SphericalMesh( const vector<Range>& zrange,
                              const vector<Range>& arange,
                              const vector<Range>& rrange):
    zenithLimits(zrange), azimuthLimits(arange), radiusLimits(rrange)
{}

void SphericalMesh::get_index(const Vector3 & sphCoord, vector<Index3D>& index) const
{
    int zm = int( (sphCoord.x + half_pi) / ( pi/zenithLimits.size() ) ); //mean
    int am = int( (sphCoord.y + pi) / ( 2*pi/azimuthLimits.size() ) );
    size_t z0 = max(0, zm - 1); //lower possible bound
    size_t z1 = min(zm+1,  zenithLimits.size()-1); //upper possible bound
    size_t a0 = max(0, am - 1);
    size_t a1 = min(am+1, azimuthLimits.size()-1);

    z0 = 0; z1 = zenithLimits.size();
    a0 = 0; a1 = azimuthLimits.size();

    index.clear();
    // index theta, for definition, check "Vector3.h"
    for (int zi = z0; zi < z1; ++zi) {
        for( int ai = a0; ai < a1; ++ai){
            for( int ri = 0; ri < radiusLimits.size(); ++ri){
                // I am too lazy to estimate the possible radius range,
                // especially for the case that log radius is used.
                if (sphCoord.x >= zenithLimits[zi].first && sphCoord.x
                    < zenithLimits[zi].second && (
                     sphCoord.y >= azimuthLimits[ai].first && sphCoord.y
                            < azimuthLimits[ai].second ||
                            sphCoord.y-2*pi >= azimuthLimits[ai].first && sphCoord.y-2*pi
                            < azimuthLimits[ai].second ||
                    sphCoord.y+2*pi >= azimuthLimits[ai].first && sphCoord.y+2*pi
                    < azimuthLimits[ai].second )&&
                    sphCoord.z >= radiusLimits[ri].first && sphCoord.z
                    < radiusLimits[ri].second  )
                {
//                    if ( zi < 0 || ai < 0 || ri < 0 || zi >= numZenith()
//                     || ai >= numAzimuth() || ri >= numRadius() )
//                    {
//                        std::stringstream ss;
//                        ss << sphCoord;
//                        throw MyException(ss.str() + "Invalid coordinates or range limits.");
//                    }
                    Index3D idx( zi, ai, ri);
                    index.push_back(idx);
                }
            }
        }
    }
}

///////////////////////// Class Descriptor ////////////////////////////
void Descriptor::clear(){
	for_each( m_vHistPtr.begin(), m_vHistPtr.end(), del_fun<Matrixf*>());
}

Descriptor::Descriptor( const Neuron& neuron, const SphericalMesh& mesh)
    : m_fSumWeight(0)
{
    m_na = mesh.numAzimuth();
    m_nz = mesh.numZenith();
    m_nr = mesh.numRadius();
    vector<Vector3> points;
    vector<float> weights;
    gatherPoints( neuron, points, weights );
    Matrixf* pHist = new Matrixf(height(), width());
    pHist->setAllZeros();
    for( size_t i = 0; i < points.size(); ++i){
        vector<Index3D> index;
        mesh.get_index( points[i].spherical(), index );
        for( size_t j = 0; j < index.size(); ++j){
            float weight = 1.0f;// weights[i];
            assert(weight);
            (*pHist)( index[j].aidx * m_nz +
                      index[j].zidx, index[j].ridx) +=  weight;
            m_fSumWeight += weight;
        }

    }
    m_vHistPtr.push_back( pHist);
    vFlip(true);
    hFlip(true);
    rotate(0); // rotate the original histogram
    rotate(1); // rotate the virtically flipped one
    rotate(2); // rotate the horizontically flipped one
}

void Descriptor::gatherPoints( const Neuron& n,
          vector<Vector3>& points, vector<float>& weights)
{
 // all points collected to be used in the shape context descriptor
        points.clear();
        points.assign( n.get_sa_points().begin(), n.get_sa_points().end() );
        weights.clear();
        weights.assign( n.get_sa_weights().begin(), n.get_sa_weights().end() );
}

void Descriptor::rotate( size_t i)
{
    // we will rotate and duplicate the i-th histogram. i generally is 0 ~ 2;
    Matrixf* pm = new Matrixf(*m_vHistPtr[i] );
    pm->clone();
    pm->rshrows( size_z() );
    m_vHistPtr.push_back( pm ); //k = 1
    for( size_t k = 2; k < size_a(); ++k ){
        Matrixf* pm = new Matrixf( *m_vHistPtr.back() );
        pm->clone();
        pm->rshrows( size_z() );
        m_vHistPtr.push_back( pm );
    }
}

void Descriptor::vFlip(bool duplicate)
{
    Matrixf *pm = m_vHistPtr[0];
    if( duplicate ){
        pm = new Matrixf( *m_vHistPtr[0] );
        pm->clone();
        m_vHistPtr.push_back(pm);
    }
    for( size_t a = 0; a < size_a(); ++a){
        for( size_t z = 0; z < size_z() /2; ++z){
            pm->swaprows(a * size_z() + z, a* size_z() + size_z() - 1 - z);
        }
    }
}

void Descriptor::hFlip(bool duplicate)
{
    Matrixf *pm = m_vHistPtr[0];
    if( duplicate ){
        pm = new Matrixf( *m_vHistPtr[0] );
        pm->clone();
        m_vHistPtr.push_back(pm);
    }
    for( size_t z = 0; z < size_z(); ++z){
        for( size_t a = 0; a < size_a()/2; ++a){
            pm->swaprows(a * size_z() + z, ( size_a() - a -1 )* size_z() + z);
        }
    }
}

void Descriptor::dump(const char * file, const char* name) const {
/*        vector<float* > buffer;
        vector<size_t> m(page(), height());
        vector<size_t> n(page(), width());
        for( size_t k = 0; k < page(); ++k){
                buffer.push_back( m_vHistPtr[k]->col_major());
        }
        write_matfile(file, name, m, n, buffer);
        for_each(buffer.begin(), buffer.end(), del_array_fun<float*>());
        */
}
