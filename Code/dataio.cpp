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
#include "mat.h"
using std::vector;

int load_data(vector<NDPair>& training_data, const char* datapath,
		bool isTrainingData) {
	if (isTrainingData)
		return 0;
}

void unload_data(vector<NDPair>& data) {

}
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

void write_matfile(const char* file, const Descriptor& desc) {

	const int BUFSIZE = 256;

	int main() {
		MATFile *pmat;
		mxArray *pdesc, *pparam;
		int status;

		printf("Creating file %s...\n\n", file);
		pmat = matOpen(file, "w");
		if (pmat == NULL) {
			printf("Error creating file %s\n", file);
			printf("(Do you have write permission in this directory?)\n");
			return (EXIT_FAILURE);
		}
		// Optional function: mxCreateCellArray

		int M = desc.size(0);
		int N = desc.size(1);
		int K = desc.size(2);

		float * descdata = 0;
		float * paramdata = 0;

		for (int k = 0; k < K; ++k) {
			pdesc = mxCreateDoubleMatrix(m, n, mxREAL);
			if (pdesc == NULL) {
				printf("%s : Out of memory on line %d\n", __FILE__, __LINE__);
				printf("Unable to create mxArray.\n");
				return (EXIT_FAILURE);
			}

			memcpy((void *) (mxGetPr(pdesc)), (void *) descdata,
					sizeof(descdata));

			char name[BUFFERSIZE];
			strcpy(name, "Descriptor");

			status = matPutVariable(pmat, name, pdesc);
			if (status != 0) {
				printf("%s :  Error using matPutVariable on line %d\n",
						__FILE__, __LINE__);
				return (EXIT_FAILURE);
			}

			mxDestroyArray(pdesc);
			mxDestroyArray(pparam);
		}
		if (matClose(pmat) != 0) {
			printf("Error closing file %s\n", file);
			return (EXIT_FAILURE);
		}
	}

