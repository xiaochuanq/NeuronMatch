#ifndef NEURON_H
#define NEURON_H
#include <list>
#include <vector>
#include <iostream>
#include "../OgreMath/OgreMath.h"

class NeuronNode
{
	friend class Neuron;
	friend std::istream& operator >>(std::istream &, NeuronNode &);
public:
    NeuronNode(int id, int type, float x, float y, float z, float d);
    int id(){ return m_nID; }
    int type(){ return m_nType; }
    float x(){ return m_v4Coord.x;}
    float y(){ return m_v4Coord.y;}
    float z(){ return m_v4Coord.z;}
    const Vector4 & coord(){ return m_v4Coord; }
    void addChild(NeuronNode*);
private:
    std::list<NeuronNode*> m_pNext;
    int m_nID;
    int m_nType;
    Vector4 m_v4Coord;
    float m_diameter;
};

class Neuron
{
public:
    Neuron();
    void addNode();
    void resample(std::vector<Vector4>&);
    void vectorize(std::vector<Vector4>&);
    const NeuronNode* root(){ return m_pSoma; };
private:
    NeuronNode* m_pSoma; // root;
};

std::istream& operator>>(std::istream&, NeuronNode&);
std::istream& operator>>(std::istream&, Neuron& );


#endif // NEURON_H
