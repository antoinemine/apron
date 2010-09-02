#include <stdio.h>
#include <stdlib.h>
#include <vector.h>
#include <time.h>
//#include <cstring.h>

#include "vSDP.h"
#include "infeassdp.h"

#define coeffC 1e+3

//--------------------------------------------------------------------------------
int runSDPVerification(char* inputFileName)
{
  int result = -2;
  bool random = 0;								//random problem to be generated?
 //checking the input file extension
  int backShift = min(6, (int)strlen(inputFileName));
  char *extension = inputFileName+strlen(inputFileName)-backShift;		//stepping back from the end of file name to study the extension
  for(int i=0; i<backShift; i++) if (*extension == '.') break;
  				   else extension++;
  if(strlen(extension)==0) extension="this";
  if (strcmp(extension, ".dat")==0 || strcmp(extension, ".dat-s")==0) random = 0;  //the input file is assumed to be in SDPA format
    else if (strcmp(extension, ".stct")==0) random = 1;				   //structure file for random problem generator
      else 
      {
        fprintf(stdout, "%s file format is not supported\n", extension);
	return(-1);
      }
  
  FILE *outstream;
  outstream = fopen((string(SDPL_WORKING_DIR)+string("sdpl.out")).c_str(), "a");
  vSDP *sdp;
  sdp = new vSDP();
  
  if (random)									//random problem will be created, solved and verified
  {
    fprintf(stdout, "Random SDP problem generator (feasible) is not implemented.\n");
    result = -1;
  }
  else										//SDPA problem from file will be solved and verified
  {
    if (sdp->readProblemFromFile(inputFileName))
    {
     // extracting the problem name
      int strlength = 12;
      char buf[strlength];
         //for(int i=0; i<strlength; i++) buf[i] = ' ';
      int beg_pos, end_pos;
      for(end_pos = strlen(inputFileName)-1; end_pos>=0; end_pos--)
      {
        if(inputFileName[end_pos] == '.') {end_pos--; break;}
      }
      for(beg_pos = end_pos; beg_pos>=0; beg_pos--)
      {
        if(inputFileName[beg_pos] == '/') {beg_pos++; break;}
      }
      if(beg_pos < 0) beg_pos = 0;
      int namelen = end_pos-beg_pos+1;		//the length of the problem name string
      if (namelen > strlength) namelen = strlength;
      strncpy(buf, inputFileName+beg_pos, namelen);
      memset(buf+namelen, ' ', strlength-namelen);
      buf[strlength-1] = 0;
      fprintf(outstream, buf);
    
     //solving the problem
      rSdpaLib::phaseType sdpret;		//return value of SDPA
      clock_t start, stop;			//used to measure the running times
      double Tp, Tu, Tl;			//running times in seconds
      
      start = clock();
      sdpret = sdp->solveProblem();
      stop = clock();
      Tp = ((double)(stop-start))/CLOCKS_PER_SEC;
      double primVal;
      //primVal = -sdp->sdpProblem->getDualObj(); //doesn't always deliver reliable values
      //optimal values will be derived from vector/matrix approximations returned by SDPA
      GeneralVector vC, vX;
      svec(sdp->C,2,vC);
      vX = sdp->extractX();
      if (!vC.isSparse)
      {
        primVal = Mid(vC.dvec*vX.dvec);
      }
      else
      {
        primVal = Mid(Col(vC.svec, 1)*vX.dvec);
      }
      //double dualVal = -sdp->sdpProblem->getPrimalObj(); //doesn't always deliver reliable values
      double dualVal = 0.0;
      double *yy = sdp->sdpProblem->getResultXVec();
      for (int iy=0; iy<sdp->m; iy++) dualVal += Mid(sdp->b)(iy+1)*yy[iy];
    
      fprintf(stdout, "The primal objective value :%.16e\n", primVal);
      fprintf(stdout, "The dual objective value   :%.16e\n", dualVal);

      //fprintf(outstream, "%+.5e  ", primVal);
      //fprintf(outstream, "%+.5e  ", dualVal);
      fprintf(outstream, "%+.12e  ", primVal);
      fprintf(outstream, "%+.12e  ", dualVal);
      fprintf(outstream, SDPA_ReturnValue(sdpret).c_str());		//interpreting the return value of SDPA

      sdp->pointToFile();						//saving the approximate solution for future use in verification algorithms
    
      start = clock();
      //double upperBound = Machine::PosInfinity;
      double upperBound = sdp->sdpUp();
      stop = clock();
      Tu = ((double)(stop-start))/CLOCKS_PER_SEC;
      fprintf(stdout, "The upper bound :%.16e\n", upperBound);

      start = clock();
      //double lowerBound = Machine::NegInfinity;
      double lowerBound = sdp->sdpLow();
      stop = clock();
      Tl = ((double)(stop-start))/CLOCKS_PER_SEC;
      fprintf(stdout, "The lower bound :%.16e\n", lowerBound);

      if (primVal < dualVal) fprintf(stdout, "Weak duality violation!!!\n");
      if (primVal > upperBound || dualVal > upperBound) fprintf(stdout, "Upper bound not verified!!!\n");
      if (lowerBound > dualVal || lowerBound > primVal) fprintf(stdout, "Lower bound not verified!!!\n");
    
      //fprintf(outstream, "  %+.5e  ", upperBound);
      //fprintf(outstream, "%+.5e  ", lowerBound);
      fprintf(outstream, "  %+.12e  ", upperBound);
      fprintf(outstream, "%+.12e  ", lowerBound);
      fprintf(outstream, "%.5e  ", mu(primVal, dualVal));
      fprintf(outstream, "%.5e  ", mu(upperBound, lowerBound));
      fprintf(outstream, "%.2lf  ", Tp);
      fprintf(outstream, "%.2lf  ", Tu);
      fprintf(outstream, "%.2lf  ", Tl);
      result = 1;
    }
    else
    {
      fprintf(stdout, "Problems reading SDPA file.");
      result = -1;
    }
  }
  fprintf(outstream, "\n");
  fclose(outstream);
  delete(sdp);
  return(result);
  /*
   result = -1  - file format or file reading problem
             1  - algorithm was applied (no information about the result)
  */	     
}
//--------------------------------------------------------------------------------
int runSDPInfeasibilityCheck(char* inputFileName)
{
  int result = -2;
  bool random = 0;								//random problem to be generated?
  bool primalInfeasToBeChecked = 1;						//defines if primal infeasibility certificate is necessary
  bool dualInfeasToBeChecked = 1;						//defines if dual infeasibility certificate is necessary
 //checking the input file extension
  int backShift = min(6, (int)strlen(inputFileName));
  char *extension = inputFileName+strlen(inputFileName)-backShift;		//stepping back from the end of file name to study the extension
  for(int i=0; i<backShift; i++) if (*extension == '.') break;
  				   else extension++;
  if(strlen(extension)==0) extension="this";
  if (strcmp(extension, ".dat")==0 || strcmp(extension, ".dat-s")==0) random = 0;  //the input file is assumed to be in SDPA format
    else if (strcmp(extension, ".stct")==0) random = 1;				   //structure file for random problem generator
      else 
      {
        fprintf(stdout, "%s file format is not supported\n", extension);
	return(-1);
      }
  
  FILE *outstream;
  outstream = fopen((string(SDPL_WORKING_DIR)+string("sdpl.out")).c_str(), "a");
  vSDP *sdp;
  sdp = new vSDP();  
  int infeasCert;
  if (random)									//random problem will be created, solved and verified
  {
    int blS[2];
    blS[0] = blS[1] = 2;
    int m = 2;
    int n = 2;
    bool dual = 1;			//to be read from .stct file later. Can also affect the values of primalInfeasToBeChecked/dualInfeasToBeChecked
    //the above data should come from stct file (routine for file reading should be a member of infeassdp class) 
    
    randInfeasSDP(sdp, n, m, blS, dual);					//generating a random infeasible problem
  }    //if(random)
  else	if (!sdp->readProblemFromFile(inputFileName))				//SDPA problem from file will be solved and verified
  {
    fprintf(stdout, "Problems reading SDPA file.");
    result = -1;
  }
  if (result != -1)								//SDP problem could be read or generated
  {
    sdp->solveProblem();							//solving the generated problem with SDPA (getting approximation)
    fprintf(stdout, "The primal objective value :%.16e\n", -sdp->sdpProblem->getDualObj());
    fprintf(stdout, "The dual objective value   :%.16e\n", -sdp->sdpProblem->getPrimalObj());
    sdp->pointToFile();
    if (dualInfeasToBeChecked)
    {
      infeasCert = infeasCertificate(sdp, 1);					//checking if a certificate of dual infeasibility can be issued
      if(infeasCert == 0) 
      {
        fprintf(stdout, "A certificate of dual infeasibility for the problem has been found.\n");
	result = 2;
      }
      else
      {
        fprintf(stdout, "No certificate of dual infeasibility for the problem has been found.\n");
        fprintf(stdout, "Return code : %.d\n", infeasCert);
	result = 0;
      }
    }
    if (primalInfeasToBeChecked)
    {
      infeasCert = infeasCertificate(sdp, 0);					//checking if a certificate of primal infeasibility can be issued
      if(infeasCert == 0) 
      {
        fprintf(stdout, "A certificate of primal infeasibility for the problem has been found.\n");
	result++;
      }
      else
      {
        fprintf(stdout, "No certificate of primal infeasibility for the problem has been found.\n");
        fprintf(stdout, "Return code : %.d\n", infeasCert);
      }
    }
  }
  fprintf(outstream, "\n");
  fclose(outstream);
  delete(sdp);
  return(result);
  /*
   result = -1  - file format or file reading problem
             0  - neither primal nor dual infeasibility could be proved
	     1  - only primal infeasibility has been shown
	     2  - only dual infeasibility has been shown
	     3  - both primal and dual infeasibilities have been proved
  */
}
//--------------------------------------------------------------------------------
int runTest(char* inputFileName)
{
  rSdpaLib::phaseType sdpret;		//return value of SDPA
  vSDP *sdp;
  sdp = new vSDP();
  sdp->readProblemFromFile(inputFileName);
  
  //Solving the problem with SDPA (no verification here yet).
  sdpret = sdp->solveProblem();			
  
  //SDPA optimal solution:
  double primVal = -sdp->sdpProblem->getDualObj();
  double dualVal = -sdp->sdpProblem->getPrimalObj();
  fprintf(stdout, "The primal objective value :%.16e\n", primVal);
  fprintf(stdout, "The dual objective value   :%.16e\n", dualVal);
  
  sdp->pointToFile();						//saving the approximate solution for future use in verification algorithms
    
  //Now the actual verification comes.
  double upperBound = sdp->sdpUp();
  fprintf(stdout, "The upper bound :%.16e\n", upperBound);

  double lowerBound = sdp->sdpLow();
  fprintf(stdout, "The lower bound :%.16e\n", lowerBound);

  delete(sdp);
  return(1);
}
//--------------------------------------------------------------------------------
//********************************************************************************
//--------------------------------------------------------------------------------
void printSyntax()
{
  fprintf(stdout, "Use sdpl -{v|i|t} \"input file\"\n");
  fprintf(stdout, "-v : for rigorous verification of an SDP problem\n");
  fprintf(stdout, "-i : for infeasibility check of an SDP problem\n");
  fprintf(stdout, "-t : for rigorous verification of an SDP problem (simple version)\n");
}
//--------------------------------------------------------------------------------
int main (int argc, char *argv[])
{
  if (argc !=3)
  {
    printSyntax();
    exit(1);
  }
  int result = -2;
  argv[1]++;
  switch (*argv[1])
  {
    case 'v': result = runSDPVerification(argv[2]); break;
    case 'i': result = runSDPInfeasibilityCheck(argv[2]); break;    
    case 't': result = runTest(argv[2]); break;
    default:
      fprintf(stdout, --argv[1]);
      fprintf(stdout, " is not recognised as a valid argument.\n");
      printSyntax();
      exit(-1);
  }
  exit(0);
}
//--------------------------------------------------------------------------------
