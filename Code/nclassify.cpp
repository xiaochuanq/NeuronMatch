#include <iostream>
#include "knnsearch.h"
#include "dataio.h"

int main(int argc, char** argv)
{
	if( argc < 3 )
		std::cout << "Usage: " << argc[0]
		     << "path/to/training/data path/to/testing/data/" << endl;
	try{
		vector<NDPair> training_data;
		vector<NDPair> testing_data;

		load_data( training_data, argv[1], true);
		load_data( testing_data, argv[2], false);

		NeuronKNN predictor;

		predictor.train( training_data );
		predictor.predict( testing_data );

		predictor.report();

		unload_data( training_data ); // release resources
		unload_data( testing_data );
	}
	catch(...){
		std::cout << "Unknown Exception. Program is to be terminated." << endl;
		return -1;
	}
	return 0;
}
