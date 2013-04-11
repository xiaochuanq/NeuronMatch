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
    public:
        const Vector3& position() const { return Coordinate; }
        float diameter() const { return Diameter;}
        float radius() const{ return Radius; }
        bool hasChild() const { return !Children.empty();}
        int parentID() const { return Parent; }
    private:
        void addChild(NeuronNode* p) {
		Children.push_back(p);
	}
        void scale(float s, bool usedOnDiameter) { Coordinate*=s; Radius*=s;
            /*  Diameter *= s; Radius is used for visualizatoin, diameter is used for
                weigthing */ if(usedOnDiameter) Diameter *= s; }
        void transform( const Matrix3& M){
            Coordinate = M * Coordinate;
        }
	void markVisited( bool v)	{ visited = v; 	}
	bool isVisited() { return visited; }

	std::list<NeuronNode*> Children;
	int ID;
	int Type;
	int Parent;
	Vector3 Coordinate;
	float Diameter;
        float Radius;
	bool visited;
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
// get/set functions
    size_t get_sa_num() const { return m_vv3Samples.size(); }
    size_t get_bi_num() const { return m_vBifurPtrArry.size(); }
    size_t get_tip_num() const {return m_vLeafPtrArry.size();}
    size_t get_edge_num() const { return m_vv3Edges.size(); }
    const std::vector<Vector3>& get_sa_points() const { return m_vv3Samples; }
    const std::vector<Vector3>& get_edges() const { return m_vv3Edges; }
    const std::vector<Vector3>& get_tips() const{ return m_vv3Tips; }
    const std::vector<float>& get_sa_radius() const { return m_vSampleRadius; }
    const std::vector<float>& get_sa_weights() const { return m_vSampleDiameters;}
    NeuronType true_type() const { return m_nType; }
    NeuronType pred_type() const { return m_nPred; }
    const std::string& name() const { return m_strName; }
    void setType(NeuronType t) { m_nType = t; }
    void labelType(NeuronType t) {m_nPred = t; }
    void setName( const std::string & str){ m_strName = str; }
    void dump() const; // for debug uses
    float maxRadius() const { return m_fMaxR; }
    bool verify();
    void traverse(void(*f)(const NeuronNode*, const NeuronNode*));
    int resample(float, bool usebifur =true);
//transform
    void decomposeAxis1(Vector3& xAxis,  Vector3& yAxis, Vector3& zAxis);
    void decomposeAxis2(Vector3& xAxis,  Vector3& yAxis, Vector3& zAxis);
    void alignAxis(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
    void normalize(bool = false );
private: // facilities
    Neuron();
    void gettips();
    void transform(const Matrix3 &);
    void traverse_sample( NeuronNode*, float, float, bool);
    void append(NeuronNode*);
    void traverse_dump(NeuronNode*) const;
    int traverse_verify( NeuronNode* node);
    void traverse_visit( const NeuronNode*, const NeuronNode*, void(*f)(const NeuronNode*, const NeuronNode*) );
private: // data
    NeuronType m_nType;
    NeuronType m_nPred;
    NeuronNode* m_pSoma; // root;
    std::string m_strName;
    std::vector<NeuronNode*> m_vNodePtrArry;
    std::vector<NeuronNode*> m_vLeafPtrArry;
    std::vector<NeuronNode*> m_vBifurPtrArry;
    std::vector<Vector3> m_vv3Samples;
    std::vector<float> m_vSampleDiameters;
    std::vector<float> m_vSampleRadius;
    std::vector<Vector3> m_vv3Edges;
    std::vector<Vector3> m_vv3Tips;
    float m_fMaxR;
};


#endif // NEURON_H
