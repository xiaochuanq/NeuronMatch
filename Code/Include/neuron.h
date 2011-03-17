#ifndef NEURON_H
#define NEURON_H
#include <list>
#include <vector>
#include <string>
#include <istream>
#include <ostream>
#include "matrixmath.h"

enum NeuronType{
		INVALID = -1, //for error caught
		UNKNOWN, // types unidentified until now
		UNCLASSIFIED, // types subject to further classification
		PYRAMIDAL,
		GRANULE
};

class Neuron;
class NeuronNode;
std::ostream& operator<<(std::ostream &, const NeuronNode &);
std::istream& operator>>(std::istream &, NeuronNode &);

class NeuronNode {
	friend class Neuron;
	friend std::ostream& operator<<(std::ostream &, const NeuronNode &);
	friend std::istream& operator>>(std::istream &, NeuronNode &);
	void addChild(NeuronNode* p) {
		Children.push_back(p);
	}
	std::list<NeuronNode*> Children;
	int ID;
	int Type;
	int Parent;
	Vector3 Coordinate;
	float Diameter;
};

class Neuron
{
public:
	static const NeuronType InvalidType = INVALID;
	static const NeuronType UnknownType = UNKNOWN;
	static const NeuronType UnclassifiedType = UNCLASSIFIED; // types subject to further classification
	static const NeuronType PyramidalType = PYRAMIDAL;
	static const NeuronType GranuleType = GRANULE;
public: //constructors
    Neuron( std::ifstream& );
    ~Neuron();
    void resample(float);
// get/set functions
    size_t get_sa_num() const { return m_vv3Samples.size(); }
    size_t get_bi_num() const { return m_vv3Bifurcations.size(); }
    size_t get_edge_num() const { return m_vv3Edges.size(); }
    const std::vector<Vector3>& get_sa_points() const { return m_vv3Samples; }
    const std::vector<Vector3>& get_bi_points() const { return m_vv3Bifurcations; }
    const std::vector<Vector3>& get_edges() const { return m_vv3Edges; }
    const std::vector<float>& get_sa_diameters() const { return m_vSampleDiameters; }
    const std::vector<float>& get_bi_diameters() const { return m_vBifurDiameters; }
    NeuronType true_type() const { return m_nType; }
    NeuronType pred_type() const { return m_nPred; }
    const std::string& name() const { return m_strName; }
    void setType(NeuronType t) { m_nType = t; }
    void labelType(NeuronType t) {m_nPred = t; }
    void setName( const std::string & str){ m_strName = str; }
    void dump() const; // for debug uses
private: // facilities
    void sample( NeuronNode*, float, float);
    void append(NeuronNode*);
    void traverse_dump(NeuronNode*) const;
private: // data
    NeuronType m_nType;
    NeuronType m_nPred;
    NeuronNode* m_pSoma; // root;
    std::string m_strName;
    std::vector<NeuronNode*> m_vNodePtrArry;
    std::vector<Vector3> m_vv3Samples;
    std::vector<float> m_vSampleDiameters;
    std::vector<Vector3> m_vv3Edges;
    std::vector<Vector3> m_vv3Bifurcations;
    std::vector<float> m_vBifurDiameters;
};


#endif // NEURON_H
