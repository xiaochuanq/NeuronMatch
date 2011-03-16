/*
 * dataio.h
 *
 *  Created on: Mar 13, 2011
 *      Author: xiaochuanq
 */

#ifndef DATAIO_H_
#define DATAIO_H_
#include <vector>
using std::vector;
//class NDPair;

//int load_data(  std::vector<NDPair>&training_data, const char* data_dir, bool isTrainingData);

//void unload_data( std::vector<NDPair>& data);

//void read_matfile( const char* file, Descriptor& desc);
int write_matfile(const char* file, const char* name,
		const vector<size_t>& m,
		const vector<size_t>& n,
		const vector<float*>& data);
#endif /* DATAIO_H_ */
