#include "SparseMatrix.h"
#include "SparseIntervalMatrix.h"
#include "Constants.h"
#include "Utilities.h"
#include "SparseUtilities.h"
#include "Error.h"

#include "sdpa_right.h"
#include "sdpa_struct.h"

//#include <stdio>
#include <vector>

#ifndef __BD_INTERVAL_MATRIX__
#define __BD_INTERVAL_MATRIX__
typedef vector<INTERVAL_MATRIX> BD_INTERVAL_MATRIX;
#endif /*__BD_INTERVAL_MATRIX__*/

#ifndef __BD_SPARSE_INTERVAL_MATRIX__
#define __BD_SPARSE_INTERVAL_MATRIX__
typedef vector<SPARSE_INTERVAL_MATRIX> BD_SPARSE_INTERVAL_MATRIX;
#endif /*__BD_SPARSE_INTERVAL_MATRIX__*/

//---------------------------------------------------------------------------------
#ifndef __GeneralMatrix__
#define __GeneralMatrix__
class GeneralMatrix
{
public:  
  
  int nBlock;
  int* blockStruct;
  bool isSparse;

  BD_INTERVAL_MATRIX        mat;
  BD_SPARSE_INTERVAL_MATRIX smat;
  
  GeneralMatrix();
  GeneralMatrix(bool sparse, int nb, int* blStruct);
  GeneralMatrix(SparseMatrix M, bool sparse);
  GeneralMatrix(double** data, int n, int* blkStruct);
  GeneralMatrix(const GeneralMatrix &source);
  ~GeneralMatrix();

  GeneralMatrix & operator += (const GeneralMatrix &);
  GeneralMatrix & operator -= (const GeneralMatrix &);
  GeneralMatrix & operator *= (double);
  GeneralMatrix  operator * (double);

  INTERVAL getTrace();
  void copyFrom(GeneralMatrix* source);
  void initialize(SparseMatrix &M, bool sparse, bool inverse);
  void initialize(GeneralMatrix &source);
  void initialize(bool sparse, int nb, int* blStruct);
  void blockResize(); //increases the GM with one block of size 1x1
  void display(FILE* fpout=stdout);
	
private:	
  
  int matrixCopy(INTERVAL_MATRIX& dest, rSparseMatrix& source, bool inverse);
  int matrixCopy(SPARSE_INTERVAL_MATRIX& dest, rSparseMatrix& source, bool inverse);
};
#endif /*__GeneralMatrix__*/
//---------------------------------------------------------------------------------
#ifndef __GeneralVector__
#define __GeneralVector__
class GeneralVector
{
public:
  
  int n;
  bool isSparse;

  INTERVAL_VECTOR        dvec;
  SPARSE_INTERVAL_MATRIX svec;
  
  GeneralVector();
  ~GeneralVector();
  
  GeneralVector  operator + (GeneralVector);
  GeneralVector  operator - (GeneralVector);

  void copyFrom(GeneralVector* source); 
};
#endif /*__GeneralVector__*/
//---------------------------------------------------------------------------------
#ifndef __RectangularMatrix__
#define __RectangularMatrix__
class RectangularMatrix
{
public:
  
  int m;
  int n;
  bool isSparse;

  INTERVAL_MATRIX        dmat;
  SPARSE_INTERVAL_MATRIX smat;

  RectangularMatrix();
  RectangularMatrix(int nRow, int nCol, bool sparse);
  ~RectangularMatrix();

  void setRow(GeneralVector& vec, int i);
};
#endif /*__RectangularMatrix__*/
//---------------------------------------------------------------------------------

void svec(GeneralMatrix& M, double s, GeneralVector& vec);
void smat(GeneralMatrix& M, double s, GeneralVector& vec, int* blkStruct, int nb);
void smat(GeneralMatrix& M, double s, GeneralVector& vec);     				//to be used when M is the original location of the matrix
							       				//  and structure information is not necessary
