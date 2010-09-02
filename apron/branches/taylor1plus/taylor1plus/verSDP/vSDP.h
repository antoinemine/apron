#include "vSDP_struct.h"
#include "const_str.h"		//string constants (environment dependent)
#include "init.h"		//programm parameters

#include "sdpa_call.h"

#ifndef __vSDP__
#define __vSDP__
class vSDP
{
public:

  GeneralMatrix* A;
  GeneralMatrix C;
  INTERVAL_VECTOR b;  //GeneralVector structure is not necessary because it's always dense.

  SDPA* sdpProblem;
    
  int n;    									//number of blocks
  int m;    									//number of conditions
  int* blockStruct;								//block structure of C and A matrices
    
  vSDP();
  ~vSDP();

  bool readProblemFromFile(char* inputFileName); 				//Reads a file in SDPA format.
  rSdpaLib::phaseType solveProblem();						//Solves the SDP problem with SDPA.
  double sdpLow(double* xu);							//Calculation of the verified lower bound of the approximal optimal solution.
  double sdpLow();								//Interface function for the case when no reasonable upper bounds
  										//  for the eigenvalues of X matrix are known.
  double sdpUp(int maxIterationNumber=9);						//Calculation of the verified upper bound of the approximal optimal solution.
  static double minEig(INTERVAL_MATRIX A, double tol, int maxit);
									        //Computes verified lower bound of the minimal eigenvalue of a symmetric interval matrix.
         									//tol - shifting tolerance. For using default value (1e-12) pass -1.
         									//maxit - maximal number of iterations. For using default value (20) pass -1.
  static double minEig(SPARSE_INTERVAL_MATRIX, double tol, int maxit);
  static int	 eigSolver(SPARSE_MATRIX A, double* d, int param);		//ARPACK/LAPACK eigenvalue solver
  										//param = 1 calculating the eigenvector corresponding to the largest eigenvalue
										//	= 2 calculating the smallest eigenvalue
  
  GeneralVector feas(RectangularMatrix& B, INTERVAL_VECTOR& b, GeneralVector& x0, int& l_info); 
  										//Trying to find a verified solution of the underdetermined system 
										//of interval liner equations B*x = b, using x0 as approximation.
										//Returnes the solution vector in case of success.
										//l_info = 0 - failed to find a solution (the matrix B might not have full rank);
										//	 = 1 - a solution was found and returned.
  
  static void Cholesky(MATRIX A, MATRIX& U, int& p);				//Cholesky factorization with LAPACK
  static double infNorm(MATRIX A);						//Calculation
  static double infNorm(SPARSE_MATRIX A);					//  of different
  static double oneNorm(SPARSE_MATRIX A);					//    matrix norms.

  friend string SDPA_ReturnValue(int val);					//interpreting the SDPA return value
  friend double mu(double a, double b);						//Used for accuracy calculations.
  bool pointToFile(char* fileName=(char*)SOLUTION_FILE);
  void test();
  
  GeneralVector extractX();  							//extracting an approximate primal optimal solution from SDPA structures
  GeneralVector extractXfromFile(char* fileName=(char*)SOLUTION_FILE);
  GeneralMatrix extractXfromFile2(char* fileName=(char*)SOLUTION_FILE); 	//as previous routine, but without svec conversion
  void extractYfromFile(double* y);

private:

  void getbFromSdpa(INTERVAL_VECTOR& bs, rVector& b);
};
#endif /* __vSDP__ */
