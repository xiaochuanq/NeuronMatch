/*
 * createdesc.cpp
 *
 *  Created on: Mar 14, 2011
 *      Author: xiaochuanq
 */

#include <iostream>
#include <fstream>
#include "neuron.h"
#include "desc.h"
//#include "desc.h"
using namespace std;

int main( int argc, const char* argv[])
{
	if( argc <= 1)
		cout << "Need a .swc file as input. " << endl;
	ifstream ifs( argv[1]);
	if( ifs ){
		Neuron n(ifs);
		n.setName("test");
		n.setType(Neuron::UnknownType);
		n.labelType(Neuron::UnclassifiedType);
		n.resample(0.1f);
		DescParameter pram(12, 24, 20, 0.0);
		Descriptor desc( pram );
		desc.extract(n);
		cout <<"Feature extracted." <<endl;
		desc.dump("test.mat", "test");
	}
	ifs.close();
	return 0;
}
