#include "arpack_interface.h"

#include "arlsmat.h"
#include "arlssym.h"
int arpackEig(int n, int nzn, double* elements, int* rIndices, int* cStarts, double* res, int mode)
{
  int retValue = 0;	
  ARluSymMatrix<double> matrix(n, nzn, elements, rIndices, cStarts);
  switch (mode)  {
    case 1: {        //calculating the eigenvector corresponding to the largest eigenvalue
      ARluSymStdEig<double> problem(1, matrix, "LA");
      problem.FindEigenvectors();
      if (problem.ConvergedEigenvalues() >= 0)
      {
        retValue = 1;
        memcpy(res, problem.RawEigenvector(0), n*sizeof(double));
      }
      else
      {
        retValue = 0;
      }
      break;}
    case 2: {        //calculating the smallest eigenvalue
      ARluSymStdEig<double> problem(n-1, matrix, "SA");
      problem.FindEigenvectors();
      if (problem.ConvergedEigenvalues() >= 0)
      {
        retValue = 1;
        *res = problem.Eigenvalue(0);
      }
      else
      {
        retValue = 0;
      }
      break;}
    default:
      retValue = 0;  
  }
  return retValue;
}
