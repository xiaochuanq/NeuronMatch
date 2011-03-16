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
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include "dataio.h"
#include "desc.h"
#include "mat.h"
using std::vector;
/*
 int load_data(vector<NDPair>& training_data, const char* datapath,
 bool isTrainingData) {
 if (isTrainingData)
 return 0;
 }

 void unload_data(vector<NDPair>& data) {

 }*/
void read_matfile(const char* file, Descriptor& desc) {
	/*

	 pmat = matOpen(file, "r");
	 if (pmat == NULL) {
	 printf("Error reopening file %s\n", file);
	 return(EXIT_FAILURE);
	 }


	 pa1 = matGetVariable(pmat, "LocalDouble");
	 if (pa1 == NULL) {
	 printf("Error reading existing matrix LocalDouble\n");
	 return(EXIT_FAILURE);
	 }
	 if (mxGetNumberOfDimensions(pa1) != 2) {
	 printf("Error saving matrix: result does not have two dimensions\n");
	 return(EXIT_FAILURE);
	 }

	 pa2 = matGetVariable(pmat, "GlobalDouble");
	 if (pa2 == NULL) {
	 printf("Error reading existing matrix GlobalDouble\n");
	 return(EXIT_FAILURE);
	 }
	 if (!(mxIsFromGlobalWS(pa2))) {
	 printf("Error saving global matrix: result is not global\n");
	 return(EXIT_FAILURE);
	 }

	 pa3 = matGetVariable(pmat, "LocalString");
	 if (pa3 == NULL) {
	 printf("Error reading existing matrix LocalString\n");
	 return(EXIT_FAILURE);
	 }

	 status = mxGetString(pa3, str, sizeof(str));
	 if(status != 0) {
	 printf("Not enough space. String is truncated.");
	 return(EXIT_FAILURE);
	 }
	 if (strcmp(str, "MATLAB: the language of technical computing")) {
	 printf("Error saving string: result has incorrect contents\n");
	 return(EXIT_FAILURE);
	 }

	 mxDestroyArray(pa1);
	 mxDestroyArray(pa2);
	 mxDestroyArray(pa3);

	 if (matClose(pmat) != 0) {
	 printf("Error closing file %s\n",file);
	 return(EXIT_FAILURE);
	 }
	 printf("Done\n");
	 return(EXIT_SUCCESS);
	 */
}

int write_matfile(const char* file, const char* name,
		const vector<size_t>& m,
		const vector<size_t>& n,
		const vector<float*>& data)
{
	MATFile *pmat;
	int status;
	printf("Creating file %s...\n\n", file);
	pmat = matOpen(file, "w");
	if (pmat == NULL) {
		printf("Error creating file %s\n", file);
		printf("(Do you have write permission in this directory?)\n");
		return -1;
	}

	for (size_t k = 0; k < data.size(); ++k) {
		mxArray* pdesc = mxCreateDoubleMatrix(m[k], n[k], mxREAL);
		if (pdesc == NULL) {
			printf("%s : Out of memory on line %d\n", __FILE__, __LINE__);
			printf("Unable to create mxArray.\n");
			return (-1);
		}

		memcpy((void *) (mxGetPr(pdesc)), (void *) data[k], sizeof(data[k]));
		char str[256];
		sprintf(str, "Desc_%s_%u", name, static_cast<unsigned>(k) );
		status = matPutVariable(pmat, str, pdesc);
		if (status != 0) {
			printf("%s :  Error using matPutVariable on line %d\n", __FILE__,
					__LINE__);
			return (-1);
		}
		mxDestroyArray(pdesc);
	}
	if (matClose(pmat) != 0) {
		printf("Error closing file %s\n", file);
		return (-1);
	}
	return 0;

}

