#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <sdpa_call.h>
#include <Bias2>

VOID BiasMidM(REAL *pr, BIASINTERVAL *pA, INT rows, INT cols)

extern "C" int call_sound_sdp(size_t dim, double* phi)
{
    int i,j;
    /* build interval matrix */
    /* build interval SDP problem */
    /* compute mid(SDP problem) */
    /* Resolve using SDPA solver */
    //SDPA::printSDPAVersion(stdout);
    SDPA  Problem1;
    Problem1.setDisplay(stdout);
    /* All parameteres are renewed */
    Problem1.setParameterType(SDPA::PARAMETER_DEFAULT);
    int mDIM   = dim;
    int nBlock = 2;
    Problem1.inputConstraintNumber(mDIM);
    Problem1.inputBlockNumber(nBlock);
    Problem1.inputBlockSize(1,dim);
    Problem1.inputBlockSize(2,dim);
    Problem1.inputBlockType(1,SDPA::SDP);
    Problem1.inputBlockType(2,SDPA::LP);

    Problem1.initializeUpperTriangleSpace();

    /* set C vector */
    for (i=1; i<=dim; i++) {
	Problem1.inputCVec(i,1);
    }

    /* set Fm matrices */
    for (i=1; i<=mDIM; i++) {
	Problem1.inputElement(i,1,i,i,1);
	Problem1.inputElement(i,2,i,i,1);
    }

    /* set F0 matrix */
    for (i=1; i<=dim; i++) {
	for (j=i; j<=dim; j++) {
	    Problem1.inputElement(0,1,i,j,phi[(i-1)*dim + (j-1)]);
	}
    }

    Problem1.initializeUpperTriangle();
    Problem1.initializeSolve();
    //Problem1.writeInputSparse((char*)"tmp.dat-s",(char*)"%+8.3e");
    Problem1.solve();
//    fprintf(stdout, "objValPrimal   = %+10.6e\n",
//	    Problem1.getPrimalObj());
//    fprintf(stdout, "objValDual     = %+10.6e\n",
//	    Problem1.getDualObj());
    Problem1.terminate();
    //exit(0);
    return 0;
}

