/*
 * knnsearch.h
 *
 *  Created on: Mar 13, 2011
 *      Author: xiaochuanq
 */

#ifndef KNNSEARCH_H_
#define KNNSEARCH_H_
#include <vector>
#include <list>
#include <map>
using namespace std;


class Descriptor;
class NeuronType;
class Parameter;
struct NDPair{
	Neuron* neuron;
	Descriptor* desc;
};

struct Vote{
	float weight;
	NeuronType type;
	float value() { return type; }
	float key() { return weight; }
};

class NeuronKNN
{
public:
	NeuronKNN();
	~NeuronKNN();
	void train(const vector<NDPair>& data);
	NeuronType predict( const Descriptor&);
	void predict( const vector<NDPair>& data);
	void report();
private:
	void generateDesc( vector<NDPair>&);
	void crossValidate();
	void vote();
	NeuronType countVote();
private:
	Parameter m_Param2learn;
	map< NeuronType, list<NDPair*> > m_Voters;
};

#endif /* KNNSEARCH_H_ */
