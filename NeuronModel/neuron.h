#ifndef NEURON_H
#define NEURON_H
#include <list>
#include <vector>

class NeuronNode
{
public:
    NeuronNode();
    int id(){ return m_nID; }
    int m_nType(){ m_nType; }
    float x(){ return m_coordc[0];}
    float y(){ return m_coordc[1];}
    float z(){ return m_coordc[2];}
    float* ccord(){ return m_cartcoord; }
private:
    std::lis<NeuronNode*> m_pChildren;
    int m_nID;
    int m_nType;
    float m_cartcoord[4];
    float m_diameter;
};

class Neuron
{
public:
    Neuron();
    void addNode();
    void resample(vector<float>&;
    void vectorize();
private:
    NeuronNode* m_pSoma;
};

#endif // NEURON_H
