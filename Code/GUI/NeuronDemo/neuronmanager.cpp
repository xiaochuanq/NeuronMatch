#include <iostream>
#include <fstream>
#include "neuronmanager.h"

NeuronManager::NeuronManager()
{

}

void NeuronManager::describe()
{

}

void NeuronManager::describe(const string & key)
{

}

void classify(){

}

NeuronEntry NeuronManager::add(const string &filename)
{
    std::ifstream ifs(filename.c_str());
    if( ifs ){
        Neuron* pn = new Neuron( ifs);
        if( !pn->verify()){
            delete pn;
            return 0;
        }
        pn->setName(filename);
        pn->setType(Neuron::UnknownType);
        pn->labelType(Neuron::UnclassifiedType);
        Vector3 axis[3];
        pn->decomposeAxis2(axis[0], axis[1], axis[2]);
        pn->alignAxis(axis[0], axis[1], axis[2]);
        pn->normalize(true);
        pn->resample(0.01f);
        m_mesh.create(6, 12, 24, false);
        m_mesh.blur(0.15);
        Descriptor * pd = new Descriptor(*pn, m_mesh);
        float h = pd->maxElement();
        NeuronEntry ne( pn, pd, h);
        put(filename, ne);
        return ne;
    }
    return NeuronEntry();
}
