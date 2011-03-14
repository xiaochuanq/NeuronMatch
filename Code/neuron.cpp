#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <functional>
#include <algorithm>

#include "neuron.h"
#include "myexception.hpp"
#include "del_functor.hpp"
#include "Math/OgreVector3.h"

using namespace std;

ostream& operator<<(ostream& os, const NeuronNode& node) {
	os << node.ID << " " << node.Type << " " << node.Coordinate << " "
			<< node.Diameter << " " << node.Parent;
	return os;
}

istream& operator>>(istream& is, NeuronNode& node) {
	is >> node.ID >> node.Type >> node.Coordinate >> node.Diameter
			>> node.Parent;
	return is;
}

/******************************** Neuron Cell/Tree Model *******************************/
Neuron::Neuron(ifstream& fs)
{
	string buffer;
	getline(fs, buffer);
	while (fs) { // skip all the comments
		size_t pos = buffer.find_first_not_of(" \t"); // skill possible blank spaces
		if (pos != string::npos && buffer[pos] != '#') // and check if it is a '#'
			break; //stops only when it is a non empty string not starting with #.
		getline(fs, buffer);
	}

	m_vNodePtrArry.push_back(0); // the m_vNodePtrArry[0] is not in use;
	m_pSoma = new NeuronNode;
	stringstream ss(buffer);
	ss >> *m_pSoma;
	m_vv3Bifurcations.push_back(m_pSoma->Coordinate);
	m_vBifurDiameters.push_back(m_pSoma->Diameter);
	m_vNodePtrArry.push_back(m_pSoma); // place it at m_vNodePtrArry[1];

	while (fs) {
		NeuronNode * pnode = new NeuronNode;
		if( !pnode)
			throw MyException("Memory for NeuronNode object not allocated.");
		getline(fs, buffer);
		if( buffer.find_first_of("1234567890.") == string::npos )
			break;
		stringstream ss(buffer);
		ss >> *pnode;
		pnode->Coordinate -= m_pSoma->Coordinate;
		append(pnode);
	}
	m_pSoma->Coordinate = Vector3(0.0f);
}

void Neuron::dump(){
	for( size_t i = 1; i < m_vNodePtrArry.size(); ++i)
		cout << *m_vNodePtrArry[i] << endl;;
	cout << endl;
	traverse_dump(m_pSoma);
}

void Neuron::traverse_dump( NeuronNode* node)
{
	if( node ){
		cout << *node << endl;
		for_each(node->Children.begin(), node->Children.end(),
			std::bind1st(std::mem_fun(&Neuron::traverse_dump), this));
	}
}

Neuron::~Neuron()
{
	for_each(m_vNodePtrArry.begin(), m_vNodePtrArry.end(), del_fun<NeuronNode*>() );
}

void Neuron::append( NeuronNode* pnode)
{
	m_vNodePtrArry.push_back( pnode );
	m_vv3Bifurcations.push_back( pnode->Coordinate);
	m_vBifurDiameters.push_back( pnode->Diameter);
	m_vv3Edges.push_back( pnode->Coordinate - m_vv3Bifurcations[pnode->Parent]);
	m_vNodePtrArry[pnode->Parent]->addChild( pnode );
}

void Neuron::resample( float steplen)
{
	m_vv3Samples.clear();
	if(m_pSoma)
		sample( m_pSoma, steplen, 0);
}

void Neuron::sample( NeuronNode* proot, float steplen, float start)
{
	float d0 = proot->Diameter;
	for( list<NeuronNode*>::iterator iter = proot->Children.begin();
			iter != proot->Children.end(); ++iter)
	{
		NeuronNode* pchild = *iter;
		float d1 = pchild->Diameter;
		Vector3 dir = pchild->Coordinate - proot->Coordinate; //get direction
		float len = dir.length();
		float len_1 = 1/len;
		dir *= len_1;
		Vector3 pos = proot->Coordinate + start * dir;
		Vector3 step = dir * steplen;
		while( start < len ){
			m_vv3Samples.push_back(pos);
			float d = ( start*d0 + (len - start)*d1 ) * len_1;
			m_vSampleDiameters.push_back( d );
			start += steplen;
			pos += step;
		}
		sample( pchild, steplen, len - start + steplen);
	}
}
