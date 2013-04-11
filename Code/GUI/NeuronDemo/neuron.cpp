#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <functional>
#include <algorithm>

#include "neuron.h"
#include "myexception.hpp"
#include "del_functor.hpp"

using namespace std;

ostream& operator<<(ostream& os, const NeuronNode& node) {
	os << node.ID << " " << node.Type << " " << node.Coordinate << " "
			<< node.Diameter << " " << node.Parent;
	return os;
}

istream& operator>>(istream& is, NeuronNode& node) {
	is >> node.ID >> node.Type >> node.Coordinate >> node.Diameter
			>> node.Parent;
        node.Radius = node.Diameter * 0.5;
	return is;
}

/******************************** Neuron Cell/Tree Model *******************************/
Neuron::Neuron(ifstream& fs)
{
	m_fMaxR = -1e6;
	string buffer;
	getline(fs, buffer);
	while (fs) { // skip all the comments
                if(buffer.size() == 0)
                    continue;
                size_t pos = buffer.find_first_not_of(" \t\r\n"); // skip possible blank spaces
		if (pos != string::npos && buffer[pos] != '#') // and check if it is a '#'
			break; //stops only when it is a non empty string not starting with #.
		getline(fs, buffer);
	}

	m_vNodePtrArry.push_back(0); // the m_vNodePtrArry[0] is not in use;
	m_pSoma = new NeuronNode;
	stringstream ss(buffer);
        ss >> *m_pSoma;
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
		pnode->Coordinate -= m_pSoma->Coordinate; //centerize to soma;
		float r = pnode->Coordinate.length();
		if( m_fMaxR < r)
			m_fMaxR = r;
		append(pnode);
	}
	m_pSoma->Coordinate = Vector3(0.0f);
        for( size_t i = 1; i < m_vNodePtrArry.size(); ++i){
            if( m_vNodePtrArry[i]->hasChild() )
                m_vBifurPtrArry.push_back( m_vNodePtrArry[i]);
            else{
                m_vLeafPtrArry.push_back( m_vNodePtrArry[i]);
                m_vv3Tips.push_back( m_vNodePtrArry[i]->Coordinate);
            }
        }

}

void Neuron::dump() const {
	for( size_t i = 1; i < m_vNodePtrArry.size(); ++i)
		cout << *m_vNodePtrArry[i] << endl;
	cout << endl;
	traverse_dump(m_pSoma);
}

void Neuron::traverse_dump( NeuronNode* node) const
{
	if( node ){
		cout << *node << endl;
		for_each(node->Children.begin(), node->Children.end(),
			std::bind1st(std::mem_fun(&Neuron::traverse_dump), this));
	}
}

void Neuron::traverse_visit( const  NeuronNode* parent,
     const  NeuronNode* node, void(*f)(const NeuronNode*, const NeuronNode*))
{
    f(parent, node);
    for(list<NeuronNode*>::const_iterator iter = node->Children.begin();
        iter != node->Children.end(); ++iter)
            traverse_visit(node, *iter, f);
}

void Neuron::traverse(void (*f)(const NeuronNode*, const NeuronNode*))
{
    traverse_visit( 0, m_pSoma, f);
}

Neuron::~Neuron()
{
	for_each(m_vNodePtrArry.begin(), m_vNodePtrArry.end(), del_fun<NeuronNode*>() );
}

void Neuron::append( NeuronNode* pnode)
{
        m_vNodePtrArry.push_back( pnode );
        m_vv3Edges.push_back( pnode->Coordinate - m_vNodePtrArry[pnode->Parent]->Coordinate);
	m_vNodePtrArry[pnode->Parent]->addChild( pnode );
}

int Neuron::resample( float steplen, bool usebifur)
{
	m_vv3Samples.clear();
	if(m_pSoma)
                traverse_sample( m_pSoma, steplen, 0, usebifur);
        return m_vv3Samples.size();
}

void Neuron::traverse_sample( NeuronNode* proot, float steplen, float start, bool usebifur)
{
	float d0 = proot->Diameter;
        float r0 = proot->Radius;
	for( list<NeuronNode*>::iterator iter = proot->Children.begin();
			iter != proot->Children.end(); ++iter)
	{
		NeuronNode* pchild = *iter;
		float d1 = pchild->Diameter;
                float r1 = pchild->Radius;
		Vector3 dir = pchild->Coordinate - proot->Coordinate; //get direction
		float len = dir.normalise();
		float len_1 = 1/len;
                Vector3 pos = proot->Coordinate + start * dir; //initial position
		Vector3 step = dir * steplen;
		while( start < len ){
			m_vv3Samples.push_back(pos);
			float d = ( start*d0 + (len - start)*d1 ) * len_1;
                       float r = ( start*r0 + (len - start)*r1 ) * len_1;
			m_vSampleDiameters.push_back( d );
                       m_vSampleRadius.push_back( r);
			start += steplen;
			pos += step;
		}
                traverse_sample( pchild, steplen, usebifur ? 0 : (start - len), usebifur );// len - start + steplen);
	}
}


void Neuron::decomposeAxis2(Vector3& xAxis,  Vector3& yAxis, Vector3& zAxis)
{
    Vector3 sum(0.0f);
    const vector<Vector3> & tips = get_tips();
    for( int i = 0; i < tips.size(); ++i)
        sum += tips.at(i);
    yAxis = sum.normalisedCopy();
    Vector3 sum2(0.0f);
    for( int i = 0; i < tips.size(); ++i)
        sum2 += tips.at(i).crossProduct(yAxis);
    xAxis = sum2.normalisedCopy();
    zAxis = (sum.crossProduct(sum2)).normalisedCopy();
}

void Neuron::decomposeAxis1(Vector3& xAxis,  Vector3& yAxis, Vector3& zAxis)
{
    Matrixf E = vv2matrix( get_edges() );
    Matrixf Et = ~E; // E is supposed to be n-by-3, and E transpose is 3-by-n
    Matrixf EtE = Et * E; // Etranspose * E should be 3-by-3
    Matrix3 M( EtE(0,0), EtE(0,1), EtE(0,2),
                    EtE(1,0), EtE(1,1), EtE(1,2),
                    EtE(2,0), EtE(2,1), EtE(2,2) );
    float eigvalue[3];
    Vector3 eigvector[3];
    M.EigenSolveSymmetric(eigvalue, eigvector);
//    for( int i = 0; i < 3; ++i){
//            eigvalue[i] *= eigvector[i].normalise();
//    }
    for( int i = 0; i < 3; ++i)
        cout << eigvalue[i]<<" "<<eigvector[i]<<" "<< eigvector[i].length()<<endl;
    // Always set y > z > x
    if( eigvalue[0] > eigvalue[1]){ // 0 > 1
        if( eigvalue[1] > eigvalue[2]){//0 > 1 > 2
            yAxis = eigvector[0];
            zAxis = eigvector[1];
            xAxis = eigvector[2];
        }
        else {/* 0 > 1, 1 < 2*/
            if( eigvalue[0] < eigvalue[2]){ //2 > 0 > 1
                yAxis = eigvector[2];
                zAxis = eigvector[0];
                xAxis = eigvector[1];
            }
            else{ // 0 > 2 > 1
                yAxis = eigvector[0];
                zAxis = eigvector[2];
                xAxis = eigvector[1];
            }
        }
    }
    else{ // 0 < 1
        if( eigvalue[1] < eigvalue[2]){ // 2 > 1 > 0
            yAxis = eigvector[2];
            zAxis = eigvector[1];
            xAxis = eigvector[0];
        }
        else {/* 0<1, 2<1*/
            if( eigvalue[0] < eigvalue[2]){ // 1 > 2 > 0
                yAxis = eigvector[1];
                zAxis = eigvector[2];
                xAxis = eigvector[0];
            }
            else{ // 1 > 0 > 2{
                yAxis = eigvector[1];
                zAxis = eigvector[0];
                xAxis = eigvector[2];
            }
        }
    }
    cout << xAxis <<" "<<yAxis<<" "<<zAxis<<endl;
}

void Neuron::alignAxis(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis)
{
    Matrix3 M;
    M.SetColumn(0, xAxis);
    M.SetColumn(1, yAxis);
    M.SetColumn(2, zAxis);
    M = M.Transpose();//M.Inverse();
    transform(M);
}

void Neuron::transform(const Matrix3 & M){
    for( size_t i = 1; i < m_vNodePtrArry.size(); ++i)
        m_vNodePtrArry[i]->transform( M);
}

void Neuron::normalize(bool usedOnDiameter)
{
	if( m_fMaxR == 0)
		return;
        float factor = 1/m_fMaxR;
	for( size_t i = 1; i < m_vNodePtrArry.size(); ++i)
                m_vNodePtrArry[i]->scale(factor, usedOnDiameter);
}

bool Neuron::verify(){
	for( size_t i = 1; i < m_vNodePtrArry.size(); ++i)
			m_vNodePtrArry[i]->markVisited(false);
        return traverse_verify( m_pSoma ) >= 0;
}

int Neuron::traverse_verify( NeuronNode* node)
{
	if( node ){
		if( node->isVisited() )
			return -1; // error in the tree structure!
		node->markVisited(true);
		for( std::list<NeuronNode*>::iterator it = node->Children.begin();
				it != node->Children.end(); ++it){
			if( traverse_verify(*it) ){
				return -1;
			}
		}
	}
	return 0;
}
