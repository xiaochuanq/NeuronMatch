/*
 * dataio.cpp
 *
 *  Created on: Mar 13, 2011
 *      Author: xiaochuanq
 */

#include <ftw.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include "dataio.h"
using std::vector;

int load_data( vector<NDPair>& training_data,  const char* datapath, bool isTrainingData)
{
	if( isTrainingData)
	return 0;
}

void unload_data( vector<NDPair>& data)
{

}


