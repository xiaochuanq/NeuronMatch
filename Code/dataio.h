/*
 * dataio.h
 *
 *  Created on: Mar 13, 2011
 *      Author: xiaochuanq
 */

#ifndef DATAIO_H_
#define DATAIO_H_
#include <vector>
class NDPair;

int load_data(  std::vector<NDPair>&training_data, const char* data_dir, bool isTrainingData);

void unload_data( std::vector<NDPair>& data);

#endif /* DATAIO_H_ */
