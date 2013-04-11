/*
 * desc.h
 *
 *  Created on: Mar 12, 2011
 *      Author: Xiaochuan Qin
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

ostream& operator <<( ostream&, SphericalMesh&);
float chi_diff(float, float);
float abs_diff(float, float);
float square_diff(float, float);
bool is_comparable(const Descriptor &, const Descriptor &);
bool is_comparable2(const Descriptor &, const Descriptor &);
float distance(const Descriptor &, const Descriptor &, float (*diff) (float, float) );
float distance2(const Descriptor &, const Descriptor &, float (*diff) (float, float) );
float difference(const Descriptor & desc1,
            const Descriptor & desc2, Descriptor& desc, float (*diff) (float, float) );
float difference(const Descriptor* desc1, const Descriptor* desc2,
                 Descriptor& desc, float (*diff) (float, float) );


struct Index3D{
    Index3D(int z, int a, int r):zidx(z), aidx(a), ridx(r){}
	int zidx;
	int aidx;
	int ridx;
};

typedef pair<float, float> Range;

class SphericalMesh {
public:
	friend ostream& operator <<( ostream&, SphericalMesh&);
        SphericalMesh(){}
        SphericalMesh( size_t nz, size_t na, size_t nr,
                       bool uselog = false, float brate = 0.0f);
        SphericalMesh( const vector<Range>&, const vector<Range>&,
                        const vector<Range>&);
        void get_index(const Vector3&, vector<Index3D>&) const;
        size_t numZenith() const {return zenithLimits.size();}
        size_t numAzimuth() const {return azimuthLimits.size();}
        size_t numRadius() const {return radiusLimits.size();}
        void blur(float brate = 0.0f);
        void create( size_t nz, size_t na, size_t nr, bool useog = false);
private:
	vector<Range> zenithLimits;
	vector<Range> azimuthLimits;
        vector<Range> radiusLimits;

};

class Neuron;
class Descriptor {
public:

    friend float difference(const Descriptor & desc1,
                            const Descriptor & desc2, Descriptor& desc, float (*diff) (float, float) );
    friend float difference(const Descriptor* desc1,
                            const Descriptor* desc2, Descriptor& desc, float (*diff) (float, float) );
public:
    Descriptor(){}
    Descriptor(const Neuron&, const SphericalMesh&);
    ~Descriptor(){ clear(); }
    void clear();
public:
	float operator()(size_t i, size_t j, size_t k) const {
		return m_vHistPtr[k]->operator ()(i, j);
	}
	bool empty() const {
		return m_vHistPtr.empty();
	}
	void dump(const char * file, const char* name) const;

        size_t size_a() const { return m_na; }
        size_t size_z() const { return m_nz; }
        size_t size_r() const { return m_nr; }
        const Matrixf* getPage(size_t i ) const { return m_vHistPtr[i];}
        size_t page() const { /*return m_na;*/
                    return m_vHistPtr.size(); }
        size_t height() const { return m_na * m_nz; }
        size_t width() const { return m_nr; }
        float maxElement() const { return m_vHistPtr[0]->getMaxElement(); }
        float sumWeight() const { return m_fSumWeight; }
    private:
//        float& operator()(size_t i, size_t j, size_t k) {
//                return m_vHistPtr[k]->operator ()(i, j);
//        }
private:
        void gatherPoints(const Neuron&, vector<Vector3>&, vector<float>&);
        void vFlip(bool duplicate = true );
        void hFlip( bool duplicate = true);
        void rotate( size_t i);
private:
        size_t m_na;
        size_t m_nz;
        size_t m_nr;
        float m_fSumWeight;
	vector<Matrixf*> m_vHistPtr;
};

#endif /* DESC_H_ */
