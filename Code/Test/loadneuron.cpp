/*
 * loadneuron.cpp
 *
 *  Created on: Mar 13, 2011
 *      Author: xiaochuanq
 */

#include <iostream>
#include <fstream>
#include "neuron.h"
using namespace std;

int main()
{
	const char* file =
			"/Users/xiaochuanq/Documents/workspace/NeuronMatch/Data/NeuroMorpho.Org/Granule_cell/n26-r01-02-sl3.CNG.swc";
	ifstream ifs( file);
	if( ifs ){
		Neuron n( ifs);
		n.setName("test");
		n.setType(Neuron::UnknownType);
		n.labelType(Neuron::UnclassifiedType);
		cout << n.name() <<": size = " << n.get_bi_num() << endl;
		cout << "Ture Type: = " << n.true_type() << " Predicted Type: = " << n.pred_type() <<endl;
		n.dump();
		n.resample(0.1f);
		cout << "Number of sample points: " << n.get_sa_num() << endl;
	}
	return 0;
}
