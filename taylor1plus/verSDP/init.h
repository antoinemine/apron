#ifndef SPARSE_EIG_SOLVER
#define SPARSE_EIG_SOLVER 0		//0 for ARPACK sparse eigenvalue solver
					//1 for standard LAPACK eigenvalue solver for full matrices
#endif
#ifndef SPARSE_LU
#define SPARSE_LU	 1		//0 for converting sparse matrix into dense format and performing LU with LAPACK
					//1 for using UMFPACK sparse routines to find LU decomposition
					//Introduced because sometimes UMFPACK failes :(
#endif
#ifndef PRINT_TIMING_INFORMATION
#define PRINT_TIMING_INFORMATION 0	//0 for skipping
					//1 for printing some process timings to stdout
#endif
#ifndef PRINT_WARNINGS
#define PRINT_WARNINGS 1		//0 for skipping
					//1 for printing calculation warnings (algorithmic) to stdout
#endif
