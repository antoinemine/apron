#include "infeassdp.h"
#include "vSDP.h"
#include <cstdlib>

//----------------------------------------------------------------------------------
int infeasCertificate(vSDP* problem, bool dual)
{
  int res = 0;  // 0 - certificate of dual/primal infeasibility
  		// 1 - the primal optimal solution of the original problem does not seem to be an improving ray
		// 2 - lambda_min < 0
		// 3 - <A,X> >= 0
		// 4 - b^T*y <= 0
		// 5 - lambda_max > 0
  
  if(!(problem->n && problem->m && problem->A))
  {
    fprintf(stdout, "The problem data are not defined. Please initialize the problem before checking infeasibility.\n");
  }
  if (!problem->sdpProblem) 
  {
    problem->solveProblem();
    problem->pointToFile();
  }
  
  if(dual)
  {
   //checking if the returned primal solution is negative
    double primVal;
    GeneralVector vC, vX;
    svec(problem->C,2,vC);
    vX = problem->extractX();
    if (!vC.isSparse)
    {
      primVal = Mid(vC.dvec*vX.dvec);
    }
    else
    {
      primVal = Mid(Col(vC.svec, 1)*vX.dvec);
    }
    //fprintf(stdout, "Primal optimal value of the original problem (<mid C, X>) : %.6e\n", primVal);
    if (primVal >= 0)
    {
      //fprintf(stdout, "The primal optimal solution of the original problem does not seem to be an improving ray.\n");
      return 1;
    }
 
   //solving the underdetermined linear system 
    GeneralMatrix X;
    INTERVAL_VECTOR b_buf(problem->m);
    double* lb = new double[problem->n];
    svec(problem->C,2,vC);
    vX = problem->extractXfromFile();
    RectangularMatrix Amat(problem->m, vC.n, vC.isSparse);
    for (int i=0; i<problem->m; i++)
    {
      GeneralVector buf;
      svec(problem->A[i],2,buf);
      Amat.setRow(buf,i+1);
      b_buf(i+1) = 0;
    }
    int info;  
    GeneralVector kl = problem->feas(Amat, b_buf, vX, info);
    if (info == 0)
    {
      fprintf(stdout, "infeasCertificate: system matrix may have no full rank (feas).\n");
    }
    smat(X, 1, kl, problem->blockStruct, problem->n); //restoring the matrix 
    double minEigValue = 1;
    for (int j=0; j<problem->n; j++) // calculating eigenvalues of each block of X
    {
      if (X.isSparse)
      {
        lb[j] = problem->minEig(X.smat[j], -1, -1);
      }
      else
      {
        lb[j] = problem->minEig(X.mat[j], -1, -1);
      }
      if (lb[j]<minEigValue) minEigValue = lb[j];
    }
    if (minEigValue >= 0) 
    {
      if (!vC.isSparse)
      {
        primVal = Sup(vC.dvec*vX.dvec);
      }
      else
      {
        primVal = Sup(Col(vC.svec, 1)*vX.dvec);
      }
      if (primVal >= 0) res = 3;
    }
    else res = 2;
    delete[] lb;
  }
  else   //if(dual)
  {
   //checking if the returned dual solution is positive
    double dualVal;
    INTERVAL intDualVal;
    double *yy = problem->sdpProblem->getResultXVec();
    for (int iy=0; iy<problem->m; iy++) intDualVal += problem->b(iy+1)*yy[iy];
    if (dualVal=Inf(intDualVal) <= 0) return 4;
    
   //checking the eigenvalues condition
    GeneralMatrix D(problem->C.isSparse, problem->n, problem->blockStruct);
    double* lb = new double[problem->n];
    for (int i=0; i<problem->m; i++)
    {
      D -= problem->A[i]*yy[i];
    }
    double minEigValue = 1;
    for (int j=0; j<problem->n; j++)
    {
      if (D.isSparse)
      {
	lb[j] = problem->minEig(D.smat[j], -1, -1);
      }
      else
      {
	lb[j] = problem->minEig(D.mat[j], -1, -1);
      } 
      if (lb[j]<minEigValue) minEigValue = lb[j];
    }
    if (minEigValue >= 0) res = 0;
      else res = 5;
    delete[] lb;
  }  //else(dual)
  return res;
}
//----------------------------------------------------------------------------------
int phaseI(vSDP* problem, bool dual)
{
  int res = 0;
  //on exit res = -1 in case of detected infesibility (primal/dual according to the value of 'dual' in input)
  //        res = 1  in case of proved fesibility (primal/dual according to the value of 'dual' in input)
  //        res = 0  otherwise
  if(!(problem->n && problem->m && problem->A))
  {
    fprintf(stdout, "The problem data are not defined. Please initialize the problem before checking infeasibility.\n");
  }
 
  if(dual) 	//checking dual feasibility
  {
    //creating new semidefinite program for feasibility check
	
	int m = problem->m+1;
	int n = problem->n+1;
	
	vSDP* feasSDP = new vSDP();
	feasSDP->m = m;
	feasSDP->n = n;
	feasSDP->blockStruct = new int[n];
	feasSDP->b = INTERVAL_VECTOR(m);
	feasSDP->A = new GeneralMatrix[m];
	//copying data from the original problem now...
	for (int j=0; j<n-1; j++) feasSDP->blockStruct[j] = problem->blockStruct[j];
	//for (int j=0; j<n; j++) feasSDP->blockStruct[j] = problem->blockStruct[j];
	feasSDP->blockStruct[n-1] = 1;
	feasSDP->C.initialize(problem->C);
	feasSDP->C.blockResize();
	double alpha = 1e3;
	double Cnorm = -1;
	double buf;
	for (int j=0; j<n-1; j++)
    {
	  if (problem->C.isSparse)
      {
	    buf = problem->vSDP::infNorm(Mid(problem->C.smat[j]));
      }
      else
      {
	    buf = problem->vSDP::infNorm(Mid(problem->C.mat[j]));
      } 
	  if (buf > Cnorm) Cnorm = buf;
	}
	if (problem->C.isSparse) SetElement(feasSDP->C.smat[n-1], 1, 1, Hull(alpha*Cnorm));
      else feasSDP->C.mat[n-1](1,1) = Hull(alpha*Cnorm);
	
	for (int i=0; i<m-1; i++)
	{
	  feasSDP->b(i+1) = 0;
	  feasSDP->A[i].initialize(problem->A[i]);
	}
	feasSDP->b(m) = Hull(1.0);
	for (int i=0; i<m-1; i++)
	{
	  feasSDP->A[i].blockResize();
	  if (feasSDP->C.isSparse) SetElement(feasSDP->A[i].smat[n-1], 1, 1, 0);
	    else feasSDP->A[i].mat[n-1](1,1) = 0;
	}
	
	
	feasSDP->A[m-1].initialize(feasSDP->C.isSparse, n, feasSDP->blockStruct);
	for (int j=0; j<n; j++)
	{
	  if (feasSDP->C.isSparse) feasSDP->A[m-1].smat[j] = SpId(feasSDP->blockStruct[j]);
	    else feasSDP->A[m-1].mat[j] = Id(feasSDP->blockStruct[j]);
    }
	
	//solving the feasibility SDP now
	rSdpaLib::phaseType sdpret = feasSDP->solveProblem();
	
	double *yy = feasSDP->sdpProblem->getResultXVec();
	fprintf(stdout, "y_u :  %e\n", alpha*Cnorm);
	fprintf(stdout, "feasSDP primal :  %e\n", -feasSDP->sdpProblem->getPrimalObj());
	fprintf(stdout, "feasSDP dual   :  %e\n", -feasSDP->sdpProblem->getDualObj());
	
	
	feasSDP->pointToFile();
	
	double lBound = feasSDP->sdpLow();
	//fprintf(stdout, "sdpLow :  %e\n", lBound);
	if (lBound > 0) res = 1;
	  else   //if feasibility could not be proved, trying to verify infeasibility
	  {
	    double uBound = feasSDP->sdpUp(1);
		if (uBound < 0) res = -1;
		//fprintf(stdout, "sdpUp  :  %e\n", uBound);
	  }
	delete feasSDP;
  }
  else		//checking primal feasibility  
  {
    int m = problem->m;
	int n = problem->n+2;
	
    vSDP* feasSDP = new vSDP();
	feasSDP->m = m;
	feasSDP->n = n;
	feasSDP->blockStruct = new int[n];
	feasSDP->b = INTERVAL_VECTOR(m);
	feasSDP->A = new GeneralMatrix[m];
	//copying data from the original problem now...
	for (int j=0; j<n-2; j++) feasSDP->blockStruct[j] = problem->blockStruct[j];
	feasSDP->blockStruct[n-2] = feasSDP->blockStruct[n-1] = 1;
	feasSDP->C.initialize(problem->C);
	for (int j=0; j<n-2; j++) 
	{
	  if (problem->C.isSparse) Clear(feasSDP->C.smat[j]);
	    else  Clear(feasSDP->C.mat[j]);
	}
	feasSDP->C.blockResize();
	feasSDP->C.blockResize();
	if (problem->C.isSparse) 
	{
	  SetElement(feasSDP->C.smat[n-2], 1, 1, -1);
	  SetElement(feasSDP->C.smat[n-1], 1, 1, 1);
	}  
    else 
	{
	  feasSDP->C.mat[n-2](1,1) = Hull(-1);
	  feasSDP->C.mat[n-1](1,1) = Hull(1);
	}
	
	INTERVAL g;
	for (int i=0; i<m; i++)
	{
	  feasSDP->b(i+1) = problem->b(i+1);
	  feasSDP->A[i].initialize(problem->A[i]);
	  
	  g = problem->A[i].getTrace();
	  feasSDP->A[i].blockResize();
	  feasSDP->A[i].blockResize();
	  if (problem->C.isSparse) 
	  {
	    SetElement(feasSDP->A[i].smat[n-2], 1, 1, g);
	    SetElement(feasSDP->A[i].smat[n-1], 1, 1, -g);
	  }  
      else 
	  {
	    feasSDP->A[i].mat[n-2](1,1) = g;
	    feasSDP->A[i].mat[n-1](1,1) = -g;
	  }
	}
	
	//solving the feasibility SDP now
	rSdpaLib::phaseType sdpret = feasSDP->solveProblem();
	   fprintf(stdout, "feasSDP primal :  %e\n", -feasSDP->sdpProblem->getPrimalObj());
	   fprintf(stdout, "feasSDP dual   :  %e\n", -feasSDP->sdpProblem->getDualObj());
    feasSDP->pointToFile();
	
	double uBound = feasSDP->sdpUp();
	fprintf(stdout, "sdpUp  :  %e\n", uBound); 
	if (uBound < 0) res = 1;
	  else   //if feasibility could not be proved, trying to verify infeasibility
	  {
	    double lBound = feasSDP->sdpLow();
		fprintf(stdout, "sdpLow :  %e\n", lBound);
		if (lBound > 0) res = -1;
	  }
	  
	  
    
	  
	  
	delete feasSDP;
  }         //else if(dual)
  return res;
}
//----------------------------------------------------------------------------------

