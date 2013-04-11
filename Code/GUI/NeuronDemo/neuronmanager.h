#ifndef NEURONMANAGER_H
#define NEURONMANAGER_H
#include <map>
#include <string>
#include "neuron.h"
#include "desc.h"
using std::map;
using std::string;

struct NeuronEntry{
    NeuronEntry(Neuron* pn=0, Descriptor* pd=0, float h = 1.0)
        :neuron(pn), desc(pd), maxH(max( 1.0, h) ){}
    Neuron* neuron;
    Descriptor* desc;
    float maxH;
    void clear(){
        delete neuron;
        delete desc;
        neuron = 0;
        desc = 0;
    }
    void resetDesc(Descriptor* pd){
        delete desc;
        desc = pd;
    }
};

class NeuronManager
{
public:
    NeuronManager();

    NeuronEntry add(const string& file);

    void put(const string& key, NeuronEntry& ne){
        m_neuronMap[key] = ne;
    }

    void tag( const string& key, NeuronType type){
        m_neuronMap[key].neuron->labelType(type);
    }

    void describe( );

    void describe( const string& key);

    void classify( );

    bool contains(const string& pathname) const{
        return m_neuronMap.find(pathname) != m_neuronMap.end();
    }

    bool notContains(const string& pathname) const{
        return m_neuronMap.find(pathname) == m_neuronMap.end();
    }

    NeuronEntry& get( const string& key) {
        return m_neuronMap[key];
    }

    const SphericalMesh& mesh(){ return m_mesh; }

    void setMesh(int nz, int na, int nr, float brate = 0.15f){
        m_mesh.create(nz, na, nr, true);
        m_mesh.blur(brate);
    }

private:
    map<string, NeuronEntry> m_neuronMap;
    SphericalMesh m_mesh;
};

#endif // NEURONMANAGER_H
