#include "arpack_interface.h"
#include "umfpack.h"
#include "vSDP.h"

#include "Utilities.h"
#include "SparseUtilities.h"
#include "Constants.h"
#include "LSS.h"
//#include "SLSS.h"

#include <math.h>
#include <stdlib.h>
#include <time.h>

//------------------------------------------------------------------------------------------------------
vSDP::vSDP() 
{
  m = n = 0;
  blockStruct = NULL;
  sdpProblem = NULL;
  A = NULL;
};
//------------------------------------------------------------------------------------------------------
vSDP::~vSDP() 
{
  if (sdpProblem) 
  {
    sdpProblem->Delete(); 
    sdpProblem = NULL;
  }
  if (blockStruct) 
  {
    delete[] blockStruct; 
  }
  if (A)
  {
    delete[] A;
  }
};
//------------------------------------------------------------------------------------------------------
rSdpaLib::phaseType vSDP::solveProblem()
{
  if (!sdpProblem) 
  {
    sdpProblem = new SDPA;
    sdpProblem->setDefaultParameter(); /*rParameterType = {rPARAMETER_DEFAULT,
	  		                                 rPARAMETER_AGGRESSIVE,
			       		                 rPARAMETER_STABLE};*/
  }
  strcpy(sdpProblem->OutputFileName, SDPA_OUTPUT);
  sdpProblem->OutputFile = fopen(sdpProblem->OutputFileName, "w");

  sdpProblem->InitialPoint = false;
  
  SDPA_initialize(*sdpProblem);

  sdpProblem->mDIM = m;
  sdpProblem->nBLOCK = n;
  sdpProblem->bLOCKsTRUCT = new int[n];
  CheckMemory(sdpProblem->bLOCKsTRUCT);
  for (int k=0; k<n; k++) sdpProblem->bLOCKsTRUCT[k] = abs(blockStruct[k]);

  SDPA_initialize2(*sdpProblem);

  for (int k=1; k<=m; k++) SDPA_Input_cVECT(*sdpProblem, k, -Mid(b(k)));
  
  int nel;
  for (int k=1; k<=n; k++)
  {
    if (!C.isSparse) {nel = abs(blockStruct[k-1]); nel = (nel*(nel+1))/2;}
      else 
      {
	nel = 0;
	for (int j=0; j<ColDimension(C.smat[k-1]); j++)
	  for (int i=C.smat[k-1].colStarts[j]; i<C.smat[k-1].colStarts[j+1]; i++)
	  {
	    if (C.smat[k-1].rowIndices[i]<=j) nel++;
	      else break;
	  }
      }
    SDPA_CountUpperTriangle(*sdpProblem, 0, k, nel);  
    for (int l=1; l<=m; l++)
    {
      if (!A[l-1].isSparse) {nel = abs(blockStruct[k-1]); nel = (nel*(nel+1))/2;}
        else 
	{
	  nel = 0;
          for (int j=0; j<ColDimension(A[l-1].smat[k-1]); j++)
            for (int i=A[l-1].smat[k-1].colStarts[j]; i<A[l-1].smat[k-1].colStarts[j+1]; i++)
            {
              if (A[l-1].smat[k-1].rowIndices[i]<=j) nel++;
                else break;
            }
	}
      SDPA_CountUpperTriangle(*sdpProblem, l, k, nel);	
    }
  }

  SDPA_Make_sfMAT(*sdpProblem);

  for (int k=1; k<=n; k++)
  {
    if (!C.isSparse)
    {
      for (int i=1; i<=abs(blockStruct[k-1]); i++)
	for (int j=1; j<=i; j++) {
		SDPA_InputElement(*sdpProblem, 0, k, j, i, -Mid(C.mat[k-1](i,j)));
	}
    }
    else
    {
      for (int j=0; j<ColDimension(C.smat[k-1]); j++)
        for (int i=C.smat[k-1].colStarts[j]; i<C.smat[k-1].colStarts[j+1]; i++)
        {
          if (C.smat[k-1].rowIndices[i]<=j) 
	  {
	    SDPA_InputElement(*sdpProblem, 0, k, C.smat[k-1].rowIndices[i]+1, j+1, -BiasMid(C.smat[k-1].theElements+i));
	  }  
          else break;
        }
    }
    for (int l=1; l<=m; l++)
      if (!A[l-1].isSparse)
      {
        for (int i=1; i<=abs(blockStruct[k-1]); i++)
          for (int j=1; j<=i; j++) SDPA_InputElement(*sdpProblem, l, k, j, i, -Mid(A[l-1].mat[k-1](i,j)));
      }
      else
      {
	for (int j=0; j<ColDimension(A[l-1].smat[k-1]); j++)
          for (int i=A[l-1].smat[k-1].colStarts[j]; i<A[l-1].smat[k-1].colStarts[j+1]; i++)
          {
            if (A[l-1].smat[k-1].rowIndices[i]<=j)
            {
              SDPA_InputElement(*sdpProblem, l, k, A[l-1].smat[k-1].rowIndices[i]+1, j+1, -BiasMid(A[l-1].smat[k-1].theElements+i));
            }
            else break;
        }		    
      }
  }
  sdpProblem->dumpData((string(SDPL_WORKING_DIR)+string("data.dump")).c_str());
  if (!SDPA_Check_sfMAT(*sdpProblem)) cout << "SDPA indicates inconsistency of the input data.\n";
  
  SDPA_Solve(*sdpProblem);

  if(sdpProblem->ParameterFile) fclose(sdpProblem->ParameterFile);
  if(sdpProblem->OutputFile) fclose(sdpProblem->OutputFile);
  
  return sdpProblem->getPhaseValue();
}
//------------------------------------------------------------------------------------------------------
double vSDP::sdpLow(double* xu)
{
  if(!(n && m && A))
  {
    cout << "The data structures are not defined. Please initialize the problem before calculating the lower bound.\n";
    return(Machine::NaN);
  }
  if (!sdpProblem) 
  {
    solveProblem();
    pointToFile();
  }

  //Data backup
  double*  vecY = new double[m];				//preserving the problem matrix structures and initial solution approximations
  double** matX = new double*[n];				//  not to be destroyed during the iterations
  CheckMemory(vecY);
  CheckMemory(matX);
  double*  resY = sdpProblem->getResultXVec();
  double*  resX;
  for (int i=0; i<m; i++) vecY[i] = resY[i];
  for (int j=0; j<n; j++)
  {
    int nRow = abs(blockStruct[j]);
    matX[j] = new double[nRow*nRow];
    CheckMemory(matX[j]);
    resX = sdpProblem->getResultYMat(j);
    for (int k=0; k<nRow; k++)
      for (int l=0; l<nRow; l++) matX[j][k + nRow*l] = resX[k + nRow*l];   
  }
  GeneralMatrix bufC;
  bufC.copyFrom(&C);
	
  //Initialization
  double fL = Machine::NegInfinity;
  int lmax = 9;							//the maximal number of iterations
  int l = 0;
  double alpha = 2.0;						//parameters defining the problem modification
  int k = 0;							// for the next iteration
  VECTOR epsj(n);
  Initialize(epsj, 0.0);
  double* y0;
  bool stop = false;
  double* dl = new double[n];
  CheckMemory(dl);
  double* dlminus = new double[n];
  CheckMemory(dlminus);

  GeneralMatrix D;

  //Algorithm
  while(!stop && l<=lmax)
  {
    bool enough = true;
    if (l==0)							//for the first iteration the solution vector y is taken from file
    {
      y0 = new double[m];
      CheckMemory(y0);
      extractYfromFile(y0);
    }
    else							//later y is taken directly from SDPA structures
    {
      if (l==1) delete[] y0;
      y0 = sdpProblem->getResultXVec();
    }
    //D.copyFrom(&C);
    D.copyFrom(&bufC);
    for (int i=0; i<m; i++)
    {
      D -= A[i]*y0[i];
    }

    for (int j=0; j<n; j++)					//finding all eigenvalues of C-Ai*yi
    {
      if (D.isSparse)
      {
	dl[j] = minEig(D.smat[j], -1, -1);
      }
      else
      {
	dl[j] = minEig(D.mat[j], -1, -1);
      }
      if (dl[j]<0) dlminus[j] = dl[j];
        else dlminus[j] = 0;
      if (dl[j]<0 && xu[j]==Machine::PosInfinity) enough = false;	
    }
    if (enough)							//success (all necessary eigenvalues are nonnegative)
    {
      INTERVAL buf(0.0); 
      for (int i=0; i<m; i++) buf += b(i+1)*y0[i];
      fL = Inf(buf);
      for (int j=0; j<n; j++)
	if (xu[j]!=Machine::PosInfinity) fL += abs(blockStruct[j])*dlminus[j]*xu[j];
      stop = true;
    }
    else							//another iteration will be performed
    {
      k++;
      C.copyFrom(&bufC);
      for (int j=0; j<n; j++)
      {
	if (dl[j]<0 && xu[j]==Machine::PosInfinity)		//changing constraints where necessary
	{
	  epsj(j+1) -= pow(alpha, k)*dl[j];
	  if (C.isSparse)
	  {
	    C.smat[j] -= epsj(j+1)*SpId(abs(blockStruct[j]));
	  }
	  else
	  {
	    C.mat[j] -= epsj(j+1)*Id(abs(blockStruct[j]));
	  }
	}
      }
      solveProblem();
    }
    l++;
  }

  if (l==1) delete[] y0;
  delete[] dl;
  delete[] dlminus;

  C.copyFrom(&bufC);						//restoring preserved data
  for (int i=0; i<m; i++) sdpProblem->inputInitXVec(i+1,vecY[i]);
  for (int j=0; j<n; j++)
  {
    int nRow = abs(blockStruct[j]);
    for (int l=0; l<nRow; l++)
    for (int k=0; k<=l; k++) 
    {
      sdpProblem->inputInitYMat(j+1,k+1,l+1,matX[j][k + nRow*l]);
    }
  }
  delete[] vecY;
  for (int i=0; i<n; i++) delete[] matX[i];
  delete[] matX;

  return(fL);
}
//------------------------------------------------------------------------------------------------------
double vSDP::sdpLow()
{
  double* x = new double[n];
  CheckMemory(x);
    //  for (int j=0; j<n; j++) x[j] = 1e+10;
  for (int j=0; j<n; j++) x[j] = Machine::PosInfinity;
  double res = sdpLow(x);
  delete [] x;
  return res;
}
//------------------------------------------------------------------------------------------------------
double vSDP::sdpUp(int maxIterationNumber)
{
  clock_t tstart, tstop;
//Due to the problems with SLSS, sparse structures are barely used here.
  if(!(n && m && A))
  {
    cout << "The data structures are not defined. Please initialize the problem before calculating the upper bound.\n";
    return(Machine::NaN);
  }
  if (!sdpProblem) 
  {
    solveProblem();
    pointToFile();
  }
   
  //Backup
  INTERVAL_VECTOR b_buf(m);
  b_buf = b;							//preserving the right hand side vector
	
  //Initialization
  double fU = Machine::PosInfinity;
  int lmax = maxIterationNumber;		//the maximal number of iterations
  int l = 0;
  double alpha = 2.0;						//parameters defining the problem modification
  int k = 0;							// for the next iteration
  bool stop = false;
  GeneralMatrix Eps(false, n, blockStruct);
  GeneralMatrix II(false, n, blockStruct);			//sparse block diagonal identity matrix
  for (int j=0; j<n; j++) II.mat[j] = Id(abs(blockStruct[j]));

  GeneralMatrix X;
  GeneralVector vC, vX;
  svec(C,2,vC);
  //vX = extractX();
  vX = extractXfromFile();					//extracting primal solution matrix from file
  
  RectangularMatrix Amat(m, vC.n, vC.isSparse);
  for (int i=0; i<m; i++)
  {
    GeneralVector buf;
    svec(A[i],2,buf);
    Amat.setRow(buf,i+1);
  }
  double* lb = new double[n];
  CheckMemory(lb);
  while (!stop && l<=lmax)					//main iterations loop
  {
    int info;  
    tstart = clock();
    GeneralVector kl = feas(Amat, b_buf, vX, info);		//trying to find a verified solution kl of the system Amat*kl=b_buf
    tstop = clock();
	    
	if (PRINT_TIMING_INFORMATION) fprintf(stdout, "\"feas\" time: %.2lf\n", ((double)(tstop-tstart))/CLOCKS_PER_SEC);
    if (info == 0)						//no solution could be found
    {
      if (PRINT_WARNINGS)
        fprintf(stdout, "vSDP::sdpUp : system matrix may have no full rank (the \"feas\" routine failed to find a verified solution).\n");
      fU = Machine::PosInfinity;
      stop = true;
    }
    else
    {
      smat(X, 1, kl, blockStruct, n); 				//restoring the matrix X from kl vector
	  double minEigValue = 1;
      tstart = clock();
      for (int j=0; j<n; j++) 					//calculating eigenvalues of each block of X
      {
        if (X.isSparse)
        {
          lb[j] = minEig(X.smat[j], -1, -1);
		}
		else
		{
	  	  lb[j] = minEig(X.mat[j], -1, -1);
		}
        if (lb[j]<minEigValue) minEigValue = lb[j];
      }
      tstop = clock();
      if (PRINT_TIMING_INFORMATION) fprintf(stdout, "time spent calculating eigenvalues: %.2lf\n", ((double)(tstop-tstart))/CLOCKS_PER_SEC);
      
	  if (minEigValue >= 0) 					//success (all eigenvectors of X are nonnegative)
      {
		if (!vC.isSparse)
		{
	  	  fU = Sup(vC.dvec*vX.dvec);
		}
		else
		{
	  	  fU = Sup(Col(vC.svec, 1)*vX.dvec);
		}
        stop = true;	
	  }
      else 							//another iteration necessary
      {
		k++;
		for (int j=0; j<n; j++)
		{
	  	  if (lb[j] < 0)
	  	  {
	    	Eps.mat[j] -= pow(alpha, k)*lb[j]*II.mat[j];
		  }
		}
		GeneralVector vEps;
		svec(Eps, 1, vEps);
		if (!Amat.isSparse)             			//introducing perturbation
		{
	  	  b = b_buf - Amat.dmat*vEps.dvec;     
		}
		else
		{
	  	  b = b_buf - Amat.smat*vEps.dvec;
		}
		solveProblem();
		vX = extractX() + vEps;
	  }
    }      
    l++;
  }
  b = b_buf;							//restoring backup
  delete[] lb;
  return(fU);	
}
//------------------------------------------------------------------------------------------------------
bool vSDP::readProblemFromFile(char* inputFileName) 		//Based on rSdpa::initializeFromFile()
{
  bool res = true;
  int mDim;
  int nDim;
  int nBlock;
  rVector b;
  rBlockSparseMatrix C;
  rBlockSparseMatrix* A;
 
  FILE* InputFile = fopen(inputFileName, "r");
  if (InputFile == NULL) {cout << "vSDP::readProblemFromFile : invalid input file name.\n"; return res;}
						  
  bool isDataSparse = false;
  int len = strlen(inputFileName);
  if (inputFileName[len-1] == 's' && inputFileName[len-2] == '-') isDataSparse = true;

  char titleAndComment[1024];
  rIO::read(InputFile, stdout, mDim, titleAndComment);
  rIO::read(InputFile, nBlock);
  if (blockStruct) delete[] blockStruct;
  blockStruct = new int[nBlock];
  CheckMemory(blockStruct);
  rIO::read(InputFile, nBlock, blockStruct);
  nDim = 0;
  for (int i=0; i<nBlock; i++) 
  { 
    blockStruct[i] = abs(blockStruct[i]);
	nDim += blockStruct[i];
  }
  b.initialize(mDim);
  rIO::read(InputFile, b);
  A = new rBlockSparseMatrix[mDim];
  CheckMemory(A);
  long position = ftell(InputFile); 				//saves the current position in the stream to access C&A twice
  int* CNonZeroCount;
  CNonZeroCount = new int[nBlock];
  CheckMemory(CNonZeroCount);
  int* ANonZeroCount;
  ANonZeroCount = new int [nBlock*mDim];
  CheckMemory(ANonZeroCount);
  rIO::read(InputFile,mDim,nBlock,blockStruct,CNonZeroCount,ANonZeroCount,isDataSparse);
  C.initialize(nBlock,blockStruct);
  for (int i=0; i<nBlock; i++)
  {
    int size = blockStruct[i];
    if (size>0) C.ele[i].initialize(size,size,rSparseMatrix::SPARSE,CNonZeroCount[i]);
      else C.ele[i].initialize(-size,-size,rSparseMatrix::DIAGONAL,-size);
  }
  for (int i=0; i<mDim; i++)
  {
    A[i].initialize(nBlock,blockStruct);
    for (int j=0; j<nBlock; j++)
    {
      int size = blockStruct[j];
      if (size>0) A[i].ele[j].initialize(size,size,rSparseMatrix::SPARSE,ANonZeroCount[i*nBlock+j]);
        else A[i].ele[j].initialize(-size,-size,rSparseMatrix::DIAGONAL,-size);
    }
  }
  rIO::read(InputFile,C,A,mDim,nBlock,blockStruct,position,isDataSparse);

  delete[] CNonZeroCount;
  delete[] ANonZeroCount;
  fclose(InputFile);

  //Initializing internal data structures
  this->C.~GeneralMatrix();
  this->C.initialize(C, isDataSparse, 0);
  if (this->A)
  {
    for (int k=0; k<this->m; ++k) this->A[k].~GeneralMatrix();
    delete[] this->A;
  }
  this->A = new GeneralMatrix[mDim];
  CheckMemory(this->A);
  for (int k=0; k<mDim; ++k) this->A[k].initialize(A[k], isDataSparse, 1);

  this->n = nBlock;
  this->m = mDim;
  getbFromSdpa(this->b, b);

  //Deleting auxiliary structures
  b.~rVector();
  C.~rBlockSparseMatrix();
  if (A)
  {
    for (int k=0; k<mDim; ++k) A[k].~rBlockSparseMatrix();
    delete[] A;
  }
  return res;
}
//-----------------------------------------------------------------------------------------------------
double vSDP::minEig(INTERVAL_MATRIX A, double tol, int maxit)
{
  if (tol <= 0) tol = 1e-12;					//algorithm tolerance
  if (maxit <= 0) maxit = 20;	  				//maximal iteration number
  //BiasInit();
  double l = Machine::NaN;  					//the resulting value of the lower bound
  int n = RowDimension(A);
  if (n==1)							//trivial case of 1x1 matrix
  {
    l = Inf(A)(1,1);
    return l;
  }
  MATRIX radA = 0.5*Diam(A);					//radius matrix (of the original interval A)
  MakePermanent(radA);
  MATRIX midA = Mid(A);						//midpoint matrix
  MakePermanent(midA);

  // calculation of the spectral radius of radA............................................................
  double mabs = 0;
  for (int i=1; i<=n; i++)
    for (int j=1; j<=n; j++) 
    {
      double bb = fabs(radA(i,j));
      if(bb > mabs) mabs = bb;
    }
  double rho = -1;
  if (mabs == 0) rho = 0;  					//the case of point matrix A
  else
  {
    VECTOR v(n);
    char uplo = 'U';						//preparing data for LAPACK DSYEV routine
    char jobz = 'V';
    int info;
    double *a = (double *)calloc(n*n, sizeof(double));
    CheckMemory(a);
    for (int i=0; i<n; i++)          				//copying the matrix for DSYEV
      for (int j=0; j<=i; j++) a[i*n+j] = radA(j+1, i+1);
    double *w = (double *)calloc(n, sizeof(double));
    CheckMemory(w);
    int lda = n;
    int lwork = -1;
    double *work = (double *)calloc(1, sizeof(double));
    CheckMemory(work);
    dsyev_(&jobz, &uplo, &n, a, &lda, w, work, &lwork, &info);	//first run is used to adjust memory allocations...
    lwork = (int)work[0];
    work = (double *)realloc(work, lwork*sizeof(double));
    CheckMemory(work);

    dsyev_(&jobz, &uplo, &n, a, &lda, w, work, &lwork, &info);	//solving the eigenvector problem		
    
    if ((*work!=lwork) && PRINT_WARNINGS) fprintf(stdout, "The chosen value for 'lwork' is not optimal (DSYEV).\n");
    if (info != 0)						//no solution found
    {
      if (PRINT_WARNINGS) {
      	fprintf(stdout, "Difficulties encountered when solving the eigen-value/vector problem for the radius matrix.\n");
      	fprintf(stdout, "Rough estimation of the spectral radius of radA will be done without the use of eigenvectors.\n"); }
      for (int i=1; i<=n; i++) v(i)=rand();
      v = v / Norm(v);
    }
    else
    {
      for (int i=1; i<=n; i++) v(i)=a[n*(n-1)+i-1];
      v = v / Norm(v);
    }
    free(a);
    free(w);
    free(work);
    for (int i=1; i<=n; i++)  					// dealing with the case when all the components of the returned eigenvector are negative
    { 
      if (v(i)<0) v = -v;
      if (v(i)!=0) break;
    }
    if (Min(v)<0)
    {
      if (PRINT_WARNINGS) {
      	fprintf(stdout, "Warning: The returned eigenvector corresponding to the maximum eigenvalue of a nonnegative matrix has negative components.\n"); 
      	fprintf(stdout, "Rough estimation of the spectral radius will be done without the use of eigenvectors.\n");}
      for (int i=1; i<=n; i++) v(i)=rand();
      v = v / Norm(v);
    }
    for (int i=1; i<=n; i++) if (v(i)<1e-6*Max(v)) v(i)=1e-6*Max(v);
    INTERVAL_VECTOR iv(n);
    iv = Hull(v);
    BiasRoundUp();
    INTERVAL_VECTOR bv = radA*iv;
    for (int i=1; i<=n; i++) bv(i) = bv(i)/iv(i);
    rho = Max(Sup(bv));						//verified upper bound of the spectral radius of radA using the Collatz bound
    BiasRoundNear();
  }
  
  //calculating the minimal eigenvalue of midA............................................................
  mabs = 0;
  for (int i=1; i<=n; i++)
    for (int j=1; j<=n; j++)
    {
      double bb = fabs(midA(i,j));
      if(bb > mabs) mabs = bb;
    }
  double la;
  char uplo = 'U';
  char jobz = 'N';
  int info;
  double *a = (double *)calloc(n*n, sizeof(double));
  CheckMemory(a);
  for (int i=0; i<n; i++)          				// copying the matrix for LAPACK routines
    for (int j=0; j<=i; j++) a[i*n+j] = midA(j+1, i+1);
  double *w = (double *)calloc(n, sizeof(double));
  CheckMemory(w);
  int lda = n;
  int lwork = -1;
  double *work = (double *)calloc(1, sizeof(double));
  CheckMemory(work);
  dsyev_(&jobz, &uplo, &n, a, &lda, w, work, &lwork, &info);	//first run is used to adjust memory allocations...
  lwork = (int)work[0];
  work = (double *)realloc(work, lwork*sizeof(double));
  CheckMemory(work);

  dsyev_(&jobz, &uplo, &n, a, &lda, w, work, &lwork, &info);	//solving the eigenvalue problem
  
  if ((*work!=lwork) && PRINT_WARNINGS) fprintf(stdout, "The chosen value for 'lwork' is not optimal (DSYEV).\n");
  if (info != 0)						//no solution found
  {
    if (PRINT_WARNINGS) {
    	fprintf(stdout, "Difficulties encountered when solving the eigen-value/vector problem for the midpoint matrix.\n");
    	fprintf(stdout, "Rough estimation of the minimal eigenvalue of midA will be done through a matrix norm.\n");}
    la = -infNorm(midA);
  }
  else
  {
    la = w[0];
  }
  free(a);
  free(w);
  free(work);

  //verification......................................................................
  double alpha1 = 1 - tol;
  double alpha2 = 1 + tol;
  double beta = 1;
  double normest = sqrt(double(n))*mabs;  			//norm estimation
  double s;
  MATRIX I(n,n);
  I = Id(n);
  MATRIX U(n,n); 						//the upper triangular matrix used in cholesky decomposition
  //BiasInit();
  int p = 1;
  int k = 0;
  while  (p!=0 && k<maxit)
  {
    k++;
    if (la>=0) s = alpha1*la - beta*(Machine::Epsilon*normest + Machine::MinPositive);
      else    s = alpha2*la - beta*(Machine::Epsilon*normest + Machine::MinPositive);
    BiasRoundDown();
    MATRIX As(n,n);
    As = midA - s*I;
    BiasRoundNear();
    Cholesky(As, U, p);
    if (p==0)							//cholesky decomposition was successfull
    {
      BiasRoundDown();
      MATRIX p1 = U*Transpose(U) - As;
      BiasRoundUp();
      MATRIX p2 = U*Transpose(U) - As;
      double r = infNorm(Transpose(p2-p1));
      BiasRoundDown();
      l = s - r - rho;						//the required lower bound
      BiasRoundNear();
    }
    else
    {
      beta = 10*beta;
      alpha1 -= beta*tol;
      alpha2 += beta*tol;
    }
  }
  if ((k==maxit) && (p!=0) && PRINT_WARNINGS) fprintf(stdout, "The verified bound of an eigenvalue could not be found within the specified number of iterations. \n");
  return l;
}
//-----------------------------------------------------------------------------------------------------
double vSDP::minEig(SPARSE_INTERVAL_MATRIX A, double tol, int maxit)
{
  
  if (tol <= 0) tol = 1e-12;
  if (maxit <= 0) maxit = 20;
  //BiasInit();
  double l = Machine::NaN;  // the resulting value of the lower bound
  int n = RowDimension(A);
  if (n==1)
  {
    l = Inf(A)(1,1);
    return l;
  }
  SPARSE_MATRIX radA = 0.5*Diam(A);
  MakePermanent(radA);
  SPARSE_MATRIX midA = Mid(A);
  MakePermanent(midA);
  int nnz = A.colStarts[n];

  // calculation of the spectral radius of radA............................................................
  double mabs = 0;
  for (int i=0; i<radA.colStarts[ColDimension(radA)]; i++)
  {
    double bb = fabs(radA.theElements[i]);
    if (bb > mabs) mabs = bb; 
  }
  double rho = -1;
  if (mabs == 0) rho = 0;  					//the case of point matrix A
  else
  {
    VECTOR v(n);
    double* d = (double *)calloc(n, sizeof(double));
    CheckMemory(d);
    if (eigSolver(radA, d, 1) != 1)
    {
      if (PRINT_WARNINGS) {
      	fprintf(stdout, "Difficulties encountered when solving the eigen-value/vector problem for the sparse radius matrix.\n");
      	fprintf(stdout, "Rough estimation of the spectral radius of radA will be done without the use of eigenvectors.\n");}
      for (int i=1; i<=n; i++) v(i)=rand();			//random vector will provide a rough approximation in Collatz formula
      v = v / Norm(v);
    }
    else
    {
      for (int i=0; i<n; i++) v(i+1)=d[i];
    }
    free(d);
    for (int i=1; i<=n; i++)  					//dealing with the case when all the components of the returned eigenvector are negative
    {
      if (v(i)<0) v = -v;
      if (v(i)!=0) break;
    }
    if (Min(v)<0)
    {
      if (PRINT_WARNINGS) {
      	fprintf(stdout, "Warning: The returned eigenvector corresponding to the maximum eigenvalue of a nonnegative matrix has negative components.\n");
      	fprintf(stdout, "Rough estimation of the spectral radius will be done without the use of eigenvectors.\n");}
      for (int i=1; i<=n; i++) v(i)=rand();
      v = v / Norm(v);
    }
    for (int i=1; i<=n; i++) if (v(i)<1e-6*Max(v)) v(i)=1e-6*Max(v);
    INTERVAL_VECTOR iv(n);
    iv = Hull(v);
    BiasRoundUp();
    INTERVAL_VECTOR bv = radA*iv;
    for (int i=1; i<=n; i++) bv(i) = bv(i)/iv(i);
    rho = Max(Sup(bv));						//verified upper bound of the spectral radius of radA using the Collatz bound
    BiasRoundNear();
  } 
  
  //lower bound of the smallest eigenvalue of midA.........................................................
  mabs = 0;
  for (int i=0; i<midA.colStarts[ColDimension(midA)]; i++)
  {
    double bb = fabs(midA.theElements[i]);
    if (bb > mabs) mabs = bb;
  }
  double la;
  if (eigSolver(midA, &la, 2) != 1)
  {
    if (PRINT_WARNINGS) {
    	fprintf(stdout, "Difficulties encountered when solving the eigen-value/vector problem for the sparse midpoint matrix.\n");
    	fprintf(stdout, "Rough estimation of the minimal eigenvalue of midA will be done through a matrix norm.\n");}
    la = -infNorm(midA);
  }
  double alpha1 = 1 - tol;
  double alpha2 = 1 + tol;
  double beta = 1;
  double normest = sqrt(double(n))*mabs;			//matrix norm estimation
  double s;
  SPARSE_MATRIX I(n,n);
  I = SpId(n);
  SPARSE_MATRIX U(n,n); 					//the upper triangular matrix used in cholesky decomposition
  //BiasInit();
  int p = -1;
  int k = 0;
  while  (p!=n && k<maxit)
  {
    k++;
    if (la>=0) s = alpha1*la - beta*(Machine::Epsilon*normest + Machine::MinPositive);
      else    s = alpha2*la - beta*(Machine::Epsilon*normest + Machine::MinPositive);
    BiasRoundDown();
    SPARSE_MATRIX As(n,n);
    As = midA - s*I;
    BiasRoundNear();
    p = ::Cholesky(As, U);
    if (p==n)
    {
      BiasRoundDown();
      SPARSE_MATRIX p1 = AAT(U, As.colStarts[n]) - As;
      BiasRoundUp();
      SPARSE_MATRIX p2 = AAT(U, As.colStarts[n]) - As;
      double r = oneNorm(p2-p1);
      BiasRoundDown();
      l = s - r - rho;
      BiasRoundNear();
    }
    else
    {
      beta = 10*beta;
      alpha1 -= beta*tol;
      alpha2 += beta*tol;
    }
  }
  if ((k==maxit) & (p!=n) && PRINT_WARNINGS) fprintf(stdout, "The verified bound of an eigenvalue could not be found within the specified number of iterations. \n");
  return l;
}
//-----------------------------------------------------------------------------------------------------
int vSDP::eigSolver(SPARSE_MATRIX A, double* d, int param)
{
  int res = -1;
  
  int n = RowDimension(A);
  int nnz = A.colStarts[n];					//the number of nonzero elements in the matrix
  if (d == NULL) 
  {
    if (PRINT_WARNINGS) fprintf(stdout, "vSDP::eigSolver:  input data structures are not defined.\n"); 
    return res;
  };
  if (SPARSE_EIG_SOLVER == 0)					//ARPACK is to be used to solve the eigenvalue problem
  {
    SPARSE_MATRIX ldA(n, n, (int)(0.5*(nnz+n)));        	//lower triangular matrix to be used with arpack
    Clear(ldA);
    for (int j=0; j<n; j++)					//preparing the elements for ARPACK
      for (int i=A.colStarts[j]; i<A.colStarts[j+1]; i++)
      {
	if (A.rowIndices[i]>=j)
	  SetElement(ldA, A.rowIndices[i]+1, j+1, A.theElements[i]);
      }
    res = arpackEig(n, ldA.colStarts[n], ldA.theElements, ldA.rowIndices, ldA.colStarts, d, param);
  }
  else								//LAPACK dense routines are to be used to solve the eigenvalue problem
  {
    char uplo = 'U';
    char jobz;
    if (param == 1) jobz = 'V';
      else jobz = 'N';
    int info;
    double *a = (double *)calloc(n*n, sizeof(double));
    CheckMemory(a);
    for (int i=0; i<n*n; i++) a[i] = 0;				//reset the array
    for (int j=0; j<n; j++)					//going through the columns 
      for (int i=A.colStarts[j]; i<A.colStarts[j+1]; i++)	
      {
	if (A.rowIndices[i]>=j)					//taking only the lower diagonal		
	  a[A.rowIndices[i]*n+j] = A.theElements[i];
      } 
    double *w = (double *)calloc(n, sizeof(double));
    CheckMemory(w);
    int lda = n;
    int lwork = -1;
    double *work = (double *)calloc(1, sizeof(double));
    CheckMemory(work);
    dsyev_(&jobz, &uplo, &n, a, &lda, w, work, &lwork, &info);
    lwork = (int)work[0];
    work = (double *)realloc(work, lwork*sizeof(double));
    CheckMemory(work);

    dsyev_(&jobz, &uplo, &n, a, &lda, w, work, &lwork, &info);
    if ((*work!=lwork) && (PRINT_WARNINGS)) fprintf(stdout, "The chosen value for 'lwork' is not optimal (DSYEV).\n");
    if (info == 0)
    {
      if (param == 1) for (int i=0; i<n; i++) d[i] = a[n*(n-1) + i];
        else *d = w[0];
      res = 1;
    }
    free(a);
    free(w);
    free(work);
  }
  return res;
}
//-----------------------------------------------------------------------------------------------------
void vSDP::Cholesky(MATRIX A, MATRIX& U, int& p)
{
  int n = RowDimension(A);	
  char uplo = 'U';
  int lda = n;
  double* m = (double *)calloc(n*lda, sizeof(double));
  CheckMemory(m);
  for (int i=0; i<n; i++)          				//copying the matrix for LAPACK routines
    for (int j=0; j<n; j++) m[i*n+j] = A(j+1, i+1);
  dpotrf_(&uplo, &n, m, &lda, &p);
  Initialize(U, 0.0);
  for (int i=1; i<=n; i++)
    for (int j=1; j<=i; j++) U(i, j)=m[(i-1)*n+j-1];
  free(m);
}
//-----------------------------------------------------------------------------------------------------
double vSDP::infNorm(MATRIX A)
{
  int n = RowDimension(A);	
  VECTOR midV(n);
  for (int i=1; i<=n; i++)
  {
    midV(i)=0;
    for (int j=1; j<=n; j++) midV(i) += fabs(A(i,j));
  }
  return Max(midV);
}
//-----------------------------------------------------------------------------------------------------
double vSDP::infNorm(SPARSE_MATRIX A)
{
  int n = RowDimension(A);
  VECTOR midV(n);
  Clear(midV);

  for (int i=0; i<A.colStarts[n]; i++)
  {
    midV(A.rowIndices[i]+1) += fabs(A.theElements[i]);
  }
  return Max(midV);
}
//-----------------------------------------------------------------------------------------------------
double vSDP::oneNorm(SPARSE_MATRIX A)
{
  int n = ColDimension(A);
  VECTOR V(n);
  for (int j=0; j<n; j++)
  {
    V(j+1) = 0;	  
    for (int i=A.colStarts[j]; i<A.colStarts[j+1]; i++)
    {
      V(j+1) += abs(A.theElements[i]);
    }
  }
  return Max(V);
}
//-----------------------------------------------------------------------------------------------------
void vSDP::getbFromSdpa(INTERVAL_VECTOR& bs, rVector& b)
{
  Resize(bs, b.nDim);
  for (int i=0; i<b.nDim; i++) bs(i+1) = -Hull(b.ele[i]);
}
//-----------------------------------------------------------------------------------------------------
GeneralVector vSDP::feas(RectangularMatrix& B, INTERVAL_VECTOR& b, GeneralVector& x0, int& l_info)
{
  GeneralVector res;
  res.copyFrom(&x0);
  l_info = 0;
  if (B.m > B.n)  							//the case when the system matrix is overdetermined
  {									//(does not happen for the problems from SDPLIB)
    if (PRINT_WARNINGS) 
    {
      fprintf(stdout, "vSDP::feas : dimension problem (overdetermined system matrix).\n");
      fprintf(stdout, "             The calculation in the subroutine will be ceased.\n");
    }
    return res;
  }
 //having problems with umfpack in the moment...
  bool modified = false;
  if (B.isSparse && SPARSE_LU==0)					//in this section sparse matrix is converted into dense format for
  {													//  future processing with LAPACK (instead of UMFPACK).
    //the changes are nondestructive. we just add a dense duplicate which will be deleted at the end.
	//the original sparse .smat remains untouched.
    Resize(B.dmat, B.m, B.n);
    Clear(B.dmat);
    for (int j=0; j<B.n; j++)
      for (int i=B.smat.colStarts[j]; i<B.smat.colStarts[j+1]; i++)
      {
		B.dmat(B.smat.rowIndices[i]+1, j+1) = B.smat(B.smat.rowIndices[i]+1,j+1);
      }
    B.isSparse = false;
    modified = true;
  }
  
  if (!B.isSparse)
  {
    int dn = B.m;
    int dm = B.n;
    int info;
    int sq_dim = min(B.m, B.n);
    int rect_dim = max(B.m, B.n);
    int* ipiv = new int[sq_dim];
    CheckMemory(ipiv);
    int* ipiv_buf = new int[rect_dim];
    CheckMemory(ipiv_buf);
    int lda = dm;
    double *a = (double *)calloc(dn*dm, sizeof(double));
    CheckMemory(a);
    for (int i=0; i<dm; i++)                                 		// copying the matrix for LAPACK routines
      for (int j=0; j<dn; j++) a[j*dm+i] = Mid(B.dmat(j+1, i+1));  	// (transposed version is used)
    
    dgetrf_(&dm, &dn, a, &lda, ipiv, &info);				//P*L*U decomposition by LAPACK
									//(for transposed matrix since rows are interchanged in dgetrf)
    if (info != 0)
    {
      if (PRINT_WARNINGS) fprintf(stdout, "vSDP::feas : LU decomposition problems (DGETRF).\n");
    }

    for (int k=0; k<rect_dim; k++) ipiv_buf[k] = k+1;
    for (int k=0; k<sq_dim; k++)           				//creating pivot indices according to the permutations returned in 'ipiv'
    {
      int buf = ipiv_buf[k];
      ipiv_buf[k] = ipiv_buf[ipiv[k]-1];
      ipiv_buf[ipiv[k]-1] = buf;
    }
    INTERVAL_MATRIX Asq(sq_dim, sq_dim);
    int n_dim = max(0,rect_dim-sq_dim);
    INTERVAL_VECTOR Xi(sq_dim);
    INTERVAL_VECTOR Xn(n_dim);
    INTERVAL_MATRIX An(sq_dim, n_dim);
    for (int j=1; j<=sq_dim; j++) SetCol(Asq, j, Col(B.dmat, ipiv_buf[j-1]));
	for (int j=1; j<=n_dim; j++) 
    {
      SetCol(An, j, Col(B.dmat, ipiv_buf[sq_dim+j-1]));
      Xn(j) = x0.dvec(ipiv_buf[sq_dim+j-1]);
    }
    
    INTERVAL_VECTOR nb(b);
    if (n_dim > 0) nb -= An*Xn;

    Xi = ILSS(Asq, nb, info);						//verified solution of the square linear system
    
    if (info == 1)
    {
      for (int j=1; j<=sq_dim; j++) res.dvec(ipiv_buf[j-1]) = Xi(j);
      l_info = 1;
    }
    else
    {
      if (PRINT_WARNINGS) fprintf(stdout, "vSDP::feas : ILSS failed to calculate the enclosure.\n");
      l_info = 0;
    }
    delete[] ipiv;
    delete[] ipiv_buf;
    free(a);
  }
  else
  {
    int dn = B.n;
    int dm = B.m;
    int nz = B.smat.colStarts[dn];
    // pre-LU permutation
    SPARSE_INTERVAL_MATRIX bufB(B.smat); 
    int status, *P, *nuI = (int *) NULL;;
    double *mB, *null = (double *) NULL;
    void *Symbolic;
    mB = new double[nz];
    CheckMemory(mB);
    P = new int[dn];
    CheckMemory(P);
    for (int i=0; i<nz; i++) mB[i] = BiasMid(&(bufB.theElements[i]));
    status = umfpack_di_symbolic (dm, dn, bufB.colStarts, bufB.rowIndices, mB, &Symbolic, null, null);
    if (status<0 && PRINT_WARNINGS) fprintf(stdout, "vSDP::feas : umfpack_di_symbolic failed.\n");
    else
    {
      status = umfpack_di_get_symbolic (nuI, nuI, nuI, nuI, nuI, nuI, nuI, P, nuI, nuI, nuI, nuI, nuI, nuI, nuI, Symbolic);
      if (status<0 && PRINT_WARNINGS) fprintf(stdout, "vSDP::feas : umfpack_di_get_symbolic failed.\n");
    }
    umfpack_di_free_symbolic (&Symbolic); 
    delete[] mB;
    // Creating square matrix
	//SPARSE_INTERVAL_MATRIX sqB(dm, dm);
    INTERVAL_MATRIX dsqB(dm, dm);
    int n_dim = max(0, dn-dm);
    INTERVAL_VECTOR Xi(dm);
    INTERVAL_VECTOR Xn(n_dim);
    SPARSE_INTERVAL_MATRIX rctB(dm, n_dim);
    for (int i=1; i<=dm; i++) SetCol(dsqB, i, Col(B.smat, P[i-1]+1));
    for (int j=1; j<=n_dim; j++)
    {
      SetCol(rctB, j, Col(B.smat, P[dm+j-1]+1));
      	//Xn(j) = x0.svec(P[dm+j-1]+1, 1);
      Xn(j) = x0.dvec(P[dm+j-1]+1);					//x0 is always dense
    }
    // Solve
    INTERVAL_VECTOR nb(b);
    if (n_dim > 0) nb -= rctB*Xn;

	//Xi = SLSS(sqB, nb, status);
    Xi = ILSS(dsqB, nb, status);
        
    if (status == 1)
    {
      //for (int j=1; j<=dm; j++) SetElement(res.svec, P[j-1]+1, 1, Xi(j));
      for (int j=1; j<=dm; j++) res.dvec(P[j-1]+1) =  Xi(j);
      l_info = 1;
    }
    else
    {
      if (PRINT_WARNINGS) fprintf(stdout, "vSDP::feas : ILSS failed to calculate the enclosure.\n");
      l_info = 0;
    }
    delete[] P;
  }
 //---------------------
  if (modified)
  {
    B.isSparse = true;
    Resize(B.dmat, 0, 0);
  }
 //---------------------  
  return res;
}
//-----------------------------------------------------------------------------------------------------
GeneralVector vSDP::extractX()
{
  GeneralVector res;
  double** matX = new double*[n];
  CheckMemory(matX);
  double*  resX;
  for (int j=0; j<n; j++)
  {
    int nRow = abs(blockStruct[j]);
    matX[j] = new double[nRow*nRow];
    CheckMemory(matX[j]);
    resX = sdpProblem->getResultYMat(j);
    for (int k=0; k<nRow; k++)
      for (int l=0; l<nRow; l++) matX[j][k + nRow*l] = resX[k + nRow*l];
  }
  GeneralMatrix X(matX, n, blockStruct);
  svec(X,1,res);
  for (int i=0; i<n; i++) delete[] matX[i];
  delete[] matX;
  return res;
}
//-----------------------------------------------------------------------------------------------------
GeneralVector vSDP::extractXfromFile(char *fileName)
{
  GeneralVector res;
  FILE *file = fopen(fileName, "r");
  int bkN, k, l;
  double val;
  double** matX = new double*[n];
  CheckMemory(matX);
  int *nRow = new int[n];
  CheckMemory(nRow);
  for (int j=0; j<n; j++)
  {
    nRow[j] = abs(blockStruct[j]);
    matX[j] = new double[nRow[j]*nRow[j]];
    CheckMemory(matX[j]);
  }
  for (int kl=0; kl<m; ++kl) fscanf(file,"%lf",&val);
  while(fscanf(file,"%d ",&bkN)!=EOF)
  {
    fscanf(file,"%d",&k)!=1;
    fscanf(file,"%d",&l)!=1;
    fscanf(file,"%lf",&val)!=1;
    matX[bkN-1][k-1+nRow[bkN-1]*(l-1)] = matX[bkN-1][l-1+nRow[bkN-1]*(k-1)] = val;
  }
  GeneralMatrix X(matX, n, blockStruct);
  svec(X,1,res);
  for (int j=0; j<n; j++) delete[] matX[j];
  delete[] matX;
  delete[] nRow;
  fclose(file);
  return res;
}
//-----------------------------------------------------------------------------------------------------
GeneralMatrix vSDP::extractXfromFile2(char *fileName)
{
  FILE *file = fopen(fileName, "r");
  int bkN, k, l;
  double val;
  double** matX = new double*[n];
  CheckMemory(matX);
  int *nRow = new int[n];
  CheckMemory(nRow);
  for (int j=0; j<n; j++)
  {
    nRow[j] = abs(blockStruct[j]);
    matX[j] = new double[nRow[j]*nRow[j]];
    CheckMemory(matX[j]);
  }
  for (int kl=0; kl<m; ++kl) fscanf(file,"%lf",&val);
  while(fscanf(file,"%d ",&bkN)!=EOF)
  {
    fscanf(file,"%d",&k)!=1;
    fscanf(file,"%d",&l)!=1;
    fscanf(file,"%lf",&val)!=1;
    matX[bkN-1][k-1+nRow[bkN-1]*(l-1)] = matX[bkN-1][l-1+nRow[bkN-1]*(k-1)] = val;
  }
  GeneralMatrix X(matX, n, blockStruct);
  for (int j=0; j<n; j++) delete[] matX[j];
  delete[] matX;
  delete[] nRow;
  fclose(file);
  return X;
}
//-----------------------------------------------------------------------------------------------------
void vSDP::extractYfromFile(double* y)
{
  FILE *file = fopen(SOLUTION_FILE, "r");
  for (int k=0; k<m; ++k)
  { 
    fscanf(file,"%lf",&y[k]);	  
  }
  fclose(file);	
}
//-----------------------------------------------------------------------------------------------------
bool vSDP::pointToFile(char* fileName)
{
  FILE *pointFile;
  pointFile = fopen(fileName, "w");
  double *y = sdpProblem->getResultXVec();
  for (int i=0; i<m; i++) fprintf(pointFile, "%.16e  ", y[i]);
  fprintf(pointFile, "\n");
  double *X;
  for (int j=0; j<n; j++)
  {
    X = sdpProblem->getResultYMat(j);
    int nRow = abs(blockStruct[j]);
    for (int k=0; k<nRow; k++)
      for (int l=0; l<=k; l++) fprintf(pointFile, "%i %i %i %.16e\n", j+1, l+1, k+1, X[k + nRow*l]);
  }
  fclose(pointFile);	
  return true;	
}
//-----------------------------------------------------------------------------------------------------
string SDPA_ReturnValue(int val)
{
  string res;
  switch(val)
  {
    case 0:
      res = "noINFO";
      break;
    case 1:
      res = "pFEAS";
      break;
    case 2:
      res = "dFEAS";
      break;
    case 3:
      res = "pdFEAS";
      break;
    case 4:
      res = "pdINF";
      break;
    case 5:
      res = "pFEAS_dINF";
      break;
    case 6:
      res = "pINF_dFEAS";
      break;
    case 7:
      res = "pdOPT";
      break;
    case 8:
      res = "pUNBD";
      break;
    case 9:
      res = "dUNBD";
      break;  
    default:
      res = "---";
      break;      
  }
  return res;
}
//-----------------------------------------------------------------------------------------------------
double mu(double a, double b)
{
  return (a-b)/max(1.0,(abs(a)+abs(b))/(double)2);
}
//-----------------------------------------------------------------------------------------------------
