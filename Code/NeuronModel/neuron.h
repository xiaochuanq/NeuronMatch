#ifndef NEURON_H
#define NEURON_H
#include <list>
#include <vector>
#include <iostream>
#include "../OgreMath/OgreMath.h"

enum NeuronType
{
	INVALID = -1, 	//for error caught
	UNKNOWN, 		// types unidentified until now
	UNCLASSIFIED,   	// types subject to further classification
	PYRAMIDAL,
	GRANULE
};

class NeuronNode
{
	friend class Neuron;
public:
    NeuronNode(int id, int type, float x, float y, float z, float d)
		: m_nID(id), m_nType(type), m_v4coord(x, y, z, 0), m_fDiameter(d) {};
    int id(){ return m_nID; }
    int type(){ return m_nType; }
    float diameter() { return m_fDiameter; }
    const Vector4 & coord(){ return m_v4Coord; }
    void addChild(NeuronNode* pnn) { m_pNext.push_back(pnn); }
private: //Do not implement purposely
	NeuronNode();
	NeuronNode(const NeuronNode&);
	NeuronNode& operator=(const NeuronNode);
private:
    std::list<NeuronNode*> m_pNext;
    int m_nID;
    int m_nType;
    Vector4 m_v4Coord;
    float m_fDiameter;
};

class NeuronModel
{
public:
    NeuronModel();
    ~NeuronModel();
    void addNode();
    void resample(float step);
    void vectorize();
    const NeuronNode* root(){ return m_pSoma; };
    NeuronType type();

    const Vector4& zenith() { return m_v4zenith;}
    const Vector4& azimuth() { return m_v4azimuth; }
private:
    NeuronType m_enType;
    NeuronNode* m_pSoma; // root;
    Vector4 m_v4zenith;
    Vector4 m_v4azimuth;
    vector<NeuronNode*> m_vNodePtrArry;
    float m_fMaxR;
    std::vector<Vector4> m_vv4Samples;
    std::vector<Vector4> m_vv4Edges;
};

std::istream& operator>>(std::istream&, Neuron& );

#endif // NEURON_H
