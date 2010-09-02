#include "vSDP_struct.h"

#define P_FORMAT "%+8.3e"

//----------------------------------------------------------------------
GeneralMatrix::GeneralMatrix()
{
  mat.clear();
  smat.clear();	  
  blockStruct = NULL;
  nBlock = 0;
  isSparse = false;
}
//----------------------------------------------------------------------
GeneralMatrix::GeneralMatrix(bool sparse, int nb, int* blStruct)
{
  CheckVectorRange(nb);
  nBlock = nb;
  isSparse = sparse;
  blockStruct = new int[nb];
  CheckMemory(blockStruct);
  if (!isSparse)
  {
    mat.resize(nBlock);
  }
  else
  {
    smat.resize(nBlock);
  }
  for (int j=0; j<nBlock; j++)
  {
    blockStruct[j] = blStruct[j];
    int nRow = abs(blStruct[j]);
    if (!isSparse)
    {
      Resize(mat[j], nRow, nRow);
      Initialize(mat[j], 0);
    }
    else
    {
      Resize(smat[j], nRow, nRow);
    }
  }
}
//----------------------------------------------------------------------
GeneralMatrix::GeneralMatrix(rBlockSparseMatrix M, bool sparse)
{
  initialize(M, sparse, 0);
}
//----------------------------------------------------------------------
GeneralMatrix::GeneralMatrix(double** data, int n, int* blkStruct)
{
  CheckVectorRange(n);
  nBlock = n;
  blockStruct = new int[n];
  CheckMemory(blockStruct);
  isSparse = false;
  mat.resize(n);
  for (int j=0; j<n; j++)
  {
    blockStruct[j] = blkStruct[j];  
    int nRow = abs(blkStruct[j]);  
    Resize(mat[j], nRow, nRow);
    Initialize(mat[j], 0);
    for (int k=0; k<nRow; k++)
      for (int l=0; l<nRow; l++) mat[j](k+1,l+1) = data[j][k + nRow*l];
  }
}
//----------------------------------------------------------------------
GeneralMatrix::GeneralMatrix(const GeneralMatrix &source)
{
  isSparse = source.isSparse;
  nBlock = source.nBlock;
  blockStruct = new int[nBlock];
  CheckMemory(blockStruct);
  if (isSparse)
  {
    mat.clear();
    smat.resize(nBlock);
  }
  else
  {
    smat.clear();
    mat.resize(nBlock);
  }
  for(int j=0; j<nBlock; j++)
  {
    blockStruct[j] = source.blockStruct[j];
    if (isSparse)
    {
      smat[j] = SPARSE_INTERVAL_MATRIX(source.smat[j]);
    }
    else
    {
      mat[j] = INTERVAL_MATRIX(source.mat[j]);
    }
  }
}
//----------------------------------------------------------------------
GeneralMatrix::~GeneralMatrix()
{
  if (blockStruct)
  {
    delete[] blockStruct;
    nBlock = 0;
  }
  if (!smat.empty())
  {
    smat.clear();
  }
  if (!mat.empty())
  {
    mat.clear();
  }
}
//----------------------------------------------------------------------
GeneralMatrix & GeneralMatrix::operator += (const GeneralMatrix & x)
{
  if (!isSparse)
  {
    for (int j=0; j<nBlock; j++) mat[j] += x.mat[j];
    return *this;	    
  }
  else
  {
    for (int j=0; j<nBlock; j++) smat[j] += x.smat[j];
    return *this;	  
  }
}
//----------------------------------------------------------------------
GeneralMatrix & GeneralMatrix::operator -= (const GeneralMatrix & x)
{
  if (!isSparse)
  {
    for (int j=0; j<nBlock; j++) mat[j] -= x.mat[j];
    return *this;
  }
  else
  {
    for (int j=0; j<nBlock; j++) smat[j] -= x.smat[j];
    return *this;
  }
}
//----------------------------------------------------------------------
GeneralMatrix & GeneralMatrix::operator *= (double x)
{
  if (!isSparse)
  {
    for (int j=0; j<nBlock; j++) mat[j] *= x;
    return *this;
  }
  else
  {
    for (int j=0; j<nBlock; j++) smat[j] *= x;
    return *this;
  }
}
//----------------------------------------------------------------------
GeneralMatrix  GeneralMatrix::operator * (double x)
{
  GeneralMatrix m;
  m.copyFrom(this);
  if (!isSparse)
  {
    for (int j=0; j<nBlock; j++) m.mat[j] = x*mat[j];
    return m;
  }
  else
  {
    for (int j=0; j<nBlock; j++) m.smat[j] = x*smat[j];
    return m;
  }
}
//----------------------------------------------------------------------
INTERVAL GeneralMatrix::getTrace()
{
  INTERVAL res = 0;
  for (int j=0; j<nBlock; j++)
  {
    for (int k=1; k<=blockStruct[j]; k++)
	{
	  if(isSparse) res += smat[j](k,k);
	    else res += mat[j](k,k);
	}
  }
  return res;
}
//----------------------------------------------------------------------
void GeneralMatrix::copyFrom(GeneralMatrix* source)
{
  CheckVectorRange(source->nBlock);
  nBlock = source->nBlock;
  if (blockStruct) delete[] blockStruct;
  blockStruct = new int[nBlock];
  CheckMemory(blockStruct);
  for (int l=0; l<nBlock; ++l) blockStruct[l] = source->blockStruct[l];
  mat.clear();
  smat.clear();
  
  if(source->isSparse)
  {
    isSparse = true;
    smat.resize(nBlock);
    for (int l=0; l<nBlock; ++l)
    {
	MakePermanent(source->smat[l]);    
	smat[l] = SPARSE_INTERVAL_MATRIX(source->smat[l]);    
    }
  }
  else
  {
    isSparse = false;
    mat.resize(nBlock);
    for (int l=0; l<nBlock; ++l) 
    {
      int dim = abs(blockStruct[l]);
      Resize(mat[l], dim, dim);
      Initialize(mat[l], 0);
      for (int i=1; i<=dim; i++)
        for (int j=1; j<=i; j++) mat[l](i,j) = mat[l](j,i) = source->mat[l](i,j);
    }
  }
}
//----------------------------------------------------------------------
void GeneralMatrix::initialize(rBlockSparseMatrix &M, bool sparse, bool inverse)
{
  CheckVectorRange(M.nBlock);
  nBlock = M.nBlock;
  if (blockStruct) delete[] blockStruct;
  blockStruct = new int[nBlock];
  CheckMemory(blockStruct);
  for (int l=0; l<nBlock; ++l) blockStruct[l] = abs(M.blockStruct[l]);
  if(sparse)
  {
    isSparse = true;
    smat.resize(nBlock);  
    mat.clear();
    for (int l=0; l<nBlock; ++l) 
    {
      matrixCopy(smat[l], M.ele[l], inverse); 
    }
  }
  else
  {
    isSparse = false;
    mat.resize(nBlock);
    smat.clear();
    for (int l=0; l<nBlock; ++l) matrixCopy(mat[l], M.ele[l], inverse);
  } 
}
//----------------------------------------------------------------------
void GeneralMatrix::initialize(GeneralMatrix &source)
{
  isSparse = source.isSparse;
  nBlock = source.nBlock;
  if (blockStruct) delete[] blockStruct;
  blockStruct = new int[nBlock];
  CheckMemory(blockStruct);
  if (isSparse)
  {
    if (!mat.empty()) mat.clear();
    smat.resize(nBlock);
  }
  else
  {
    if (!smat.empty()) smat.clear();
    mat.resize(nBlock);
  }
  for(int j=0; j<nBlock; j++)
  {
    blockStruct[j] = source.blockStruct[j];
    if (isSparse)
    {
      Resize(smat[j],RowDimension(source.smat[j]),ColDimension(source.smat[j]),Allocated(source.smat[j]));
      for (int i=1; i<=ColDimension(smat[j]); i++) SetCol(smat[j], i, Col(source.smat[j], i));
    }
    else
    {
      mat[j] = source.mat[j];
    }
  }
}
//---------------------------------------------------------------------//
void GeneralMatrix::initialize(bool sparse, int nb, int* blStruct)
{
  CheckVectorRange(nb);
  nBlock = nb;
  isSparse = sparse;
  if (blockStruct) delete[] blockStruct; 
  blockStruct = new int[nb];
  CheckMemory(blockStruct);
  if (!isSparse)
  {
    mat.resize(nBlock);
  }
  else
  {
    smat.resize(nBlock);
  }
  for (int j=0; j<nBlock; j++)
  {
    blockStruct[j] = blStruct[j];
    int nRow = abs(blStruct[j]);
    if (!isSparse)
    {
      Resize(mat[j], nRow, nRow);
      Initialize(mat[j], 0);
    }
    else
    {
      Resize(smat[j], nRow, nRow);
    }
  }
}
//----------------------------------------------------------------------
void GeneralMatrix::blockResize()
{
  nBlock++;
  int* bufBlStr = new int[nBlock];
  CheckMemory(bufBlStr);
  for (int i=0; i<nBlock-1; i++) bufBlStr[i] = blockStruct[i];
  bufBlStr[nBlock-1] = 1;
  blockStruct = bufBlStr;
  if (isSparse)
  {
    smat.push_back(SPARSE_INTERVAL_MATRIX(1,1,1));
  }
  else
  {
    mat.push_back(INTERVAL_MATRIX(1,1));
    Clear(mat[nBlock-1]);
  }
}
//----------------------------------------------------------------------
void GeneralMatrix::display(FILE* fpout)
{
  if (fpout == NULL) return;
  fprintf(fpout,"{\n");
  if (nBlock>0 && blockStruct) 
  {
    for (int l=0; l<nBlock; ++l) 
    {
      fprintf(fpout, "{");
      if (!isSparse)
      {
        int nRow = RowDimension(mat[l]);
        int nCol = ColDimension(mat[l]);	
        for(int i=0; i<nRow-1; i++)
        {
          if(i==0) fprintf(fpout," ");
            else fprintf(fpout,"  ");
	  fprintf(fpout,"{");
	  for(int j=0; j<nCol-1; j++) fprintf(fpout, P_FORMAT",", Mid(mat[l](i+1,j+1)));
	  fprintf(fpout, P_FORMAT" },\n", Mid(mat[l](i+1,nCol)));
        }
        if (nRow>1) fprintf(fpout, "  {");
        for (int j=0; j<nCol-1; j++) fprintf(fpout, P_FORMAT",", Mid(mat[l](nRow, j+1)));
        fprintf(fpout, P_FORMAT" }", Mid(mat[l](nRow,nCol)));
        if (nRow>1) fprintf(fpout, "   }\n");
         else fprintf(fpout, "\n");
      }
      else
      {
        for(int i=0; i<ColDimension(smat[l]); i++)
	{
	  for (int j=smat[l].colStarts[i]; j<smat[l].colStarts[i+1]; j++)
	  {
	    fprintf(fpout, "(%u,%u)", smat[l].rowIndices[j], i);
	    fprintf(fpout, P_FORMAT, BiasMid(&smat[l].theElements[j]));
	    if (j == smat[l].colStarts[ColDimension(smat[l])]-1) fprintf(fpout, "}\n");
	      else fprintf(fpout, ",\n");
	  }
	}
      }
    }
  }
  fprintf(fpout,"} \n");
}
//----------------------------------------------------------------------
int GeneralMatrix::matrixCopy(INTERVAL_MATRIX& dest, rSparseMatrix& source, bool inverse)
{
  Resize(dest, source.nRow, source.nCol);
  Initialize(dest, 0);
  switch(source.Sp_De_Di)
  {
  case rSparseMatrix::SPARSE:
    for(int k=0; k<source.NonZeroCount; k++)
    {
      int i = source.row_index[k];
      int j = source.column_index[k];
      double val = source.sp_ele[k];
      if (inverse) dest(i+1,j+1) = dest(j+1,i+1) = -Hull(val);
        else dest(i+1,j+1) = dest(j+1,i+1) = Hull(val);
    }
    break;
  case rSparseMatrix::DENSE:
    for (int i=0; i<source.nRow; i++)
      for (int j=0; j<source.nCol; j++)  
	if (inverse) dest(i+1,j+1) = dest(j+1,i+1) = -Hull(source.de_ele[j+source.nCol*i]);
          else dest(i+1,j+1) = dest(j+1,i+1) = Hull(source.de_ele[j+source.nCol*i]);
    break;
  case rSparseMatrix::DIAGONAL:
    for (int k=0; k<source.nCol; k++) 
      if (inverse) dest(k+1,k+1) = -Hull(source.di_ele[k]);  
        else dest(k+1,k+1) = Hull(source.di_ele[k]);
    break;  	      
  }	  
  return 0;	
}
//----------------------------------------------------------------------
int GeneralMatrix::matrixCopy(SPARSE_INTERVAL_MATRIX& dest, rSparseMatrix& source, bool inverse)
{
  switch(source.Sp_De_Di)
  {
  case rSparseMatrix::SPARSE:
    Resize(dest, source.nRow, source.nCol, source.NonZeroCount);
    for (int k=0; k<source.NonZeroCount; k++) 
    {
      if (inverse) 
      {
        SetElement(dest, source.row_index[k]+1, source.column_index[k]+1, -Hull(source.sp_ele[k]));
	SetElement(dest, source.column_index[k]+1, source.row_index[k]+1, -Hull(source.sp_ele[k]));
      }
      else 
      {
        SetElement(dest, source.row_index[k]+1, source.column_index[k]+1, Hull(source.sp_ele[k]));
	SetElement(dest, source.column_index[k]+1, source.row_index[k]+1, Hull(source.sp_ele[k]));
      }
    }
    break;
  case rSparseMatrix::DENSE:
    Resize(dest, source.nRow, source.nCol, source.nRow*source.nCol);
    for (int i=0; i<source.nRow; i++)
      for (int j=0; j<source.nCol; j++)
        if (inverse) 
	{
	  SetElement(dest, i+1, j+1, -Hull(source.de_ele[j+source.nCol*i]));
	  SetElement(dest, j+1, i+1, -Hull(source.de_ele[j+source.nCol*i]));
	}
        else 
	{
	  SetElement(dest, i+1, j+1, Hull(source.de_ele[j+source.nCol*i]));
	  SetElement(dest, j+1, i+1, Hull(source.de_ele[j+source.nCol*i]));
	}
    break;
  case rSparseMatrix::DIAGONAL:
    Resize(dest, source.nRow, source.nCol, source.nCol);
    for (int k=0; k<source.nCol; k++)
      if (inverse) SetElement(dest, k+1, k+1, -Hull(source.di_ele[k]));
        else SetElement(dest, k+1, k+1, Hull(source.di_ele[k]));
    break;
  }
  return 0;
}
//----------------------------------------------------------------------
RectangularMatrix::RectangularMatrix()
{
  m = 0;
  n = 0;
  isSparse = false;
  Resize(dmat, 0, 0);
  Resize(smat, 0, 0);
}
//----------------------------------------------------------------------
RectangularMatrix::RectangularMatrix(int nRow, int nCol, bool sparse)
{
  CheckMatrixRange(nCol, nRow);
  isSparse = sparse;
  m = nRow;
  n = nCol;
  if (isSparse)
  {
    Resize(smat, nRow, nCol);
    Clear(smat);
  }
  else
  {
    Resize(dmat, nRow, nCol);
    Clear(dmat);
  }
}
//----------------------------------------------------------------------
RectangularMatrix::~RectangularMatrix()
{
}
//----------------------------------------------------------------------
void RectangularMatrix::setRow(GeneralVector& vec, int i)
{
  if ((isSparse && !(vec.isSparse)) || (!isSparse && (vec.isSparse))) 			//logical XOR
    fprintf(stdout, "RectangularMatrix: attempt to assign a vector to a matrix row with different sparse mode.\n");
  if (isSparse)
  {
    int vectorNonZeroCount = vec.svec.colStarts[ColDimension(vec.svec)];
    int matrixNonZeroCount = smat.colStarts[ColDimension(smat)];
    int *coln = vec.svec.rowIndices;
    if (matrixNonZeroCount+vectorNonZeroCount > Allocated(smat))
      IncAlloc(smat, matrixNonZeroCount + vectorNonZeroCount - Allocated(smat));
    for (int j=0; j<vectorNonZeroCount; j++)
    {
      BIASINTERVAL buf = vec.svec.theElements[j];
      SetElement(smat, i, coln[j]+1, INTERVAL(BiasInf(&buf), BiasSup(&buf)));
    }
  }
  else
  {
    SetRow(dmat, i, vec.dvec);
  }
}
//----------------------------------------------------------------------
GeneralVector::GeneralVector()
{
  Clear(dvec);
  Clear(svec);
  isSparse = false;
  n = 0;
}
//----------------------------------------------------------------------
GeneralVector::~GeneralVector()
{
}
//----------------------------------------------------------------------
GeneralVector  GeneralVector::operator + (GeneralVector x)
{
  GeneralVector m;
  m.copyFrom(this);
  if (!isSparse)
  {
    m.dvec += x.dvec;
    return m;
  }
  else
  {
    m.svec += x.svec;
    return m;
  }
}
//----------------------------------------------------------------------
GeneralVector  GeneralVector::operator - (GeneralVector x)
{
  GeneralVector m;
  m.copyFrom(this);
  if (!isSparse)
  {
    m.dvec -= x.dvec;
    return m;
  }
  else
  {
    m.svec -= x.svec;
    return m;
  }
}
//----------------------------------------------------------------------
void GeneralVector::copyFrom(GeneralVector* source)
{
  CheckVectorRange(source->n);
  n = source->n;
  if(source->isSparse)
  {
    isSparse = true;
    svec = source->svec;
  }
  else
  {
    isSparse = false;
    dvec = source->dvec;
  }
}
//----------------------------------------------------------------------
void svec(GeneralMatrix& M, double s, GeneralVector& dres)
{
  Clear(dres.dvec);
  Clear(dres.svec);
  
  int size = 0;
  int NonZeroCount = 0;
  for (int j=0; j<M.nBlock; j++) 
  {
    int bs = abs(M.blockStruct[j]);
    size += (bs*(bs+1))/2;
    if (M.isSparse) NonZeroCount += (bs+M.smat[j].colStarts[ColDimension(M.smat[j])])/2;
  }
  dres.n = size;
  if (M.isSparse)
  {
    dres.isSparse = true;
    Resize(dres.svec, size, 1, NonZeroCount);
    int blockShift = 0;
    for (int k=0; k<M.nBlock; k++)
    {
      int bs = abs(M.blockStruct[k]);
      int vectorBlockSize = (bs*(bs+1))/2;
      int vecPosition;
      int rowNumber;
      for (int j=0; j<bs; j++)
        for (int i=M.smat[k].colStarts[j]; i<M.smat[k].colStarts[j+1]; i++)
        {
	  rowNumber = M.smat[k].rowIndices[i];
	  if (rowNumber>=j)
	  {
	    vecPosition = blockShift+(bs+1)*j-(j*(j+3))/2+M.smat[k].rowIndices[i];
	    if (rowNumber==j) SetElement(dres.svec, vecPosition+1, 1, M.smat[k](rowNumber+1, j+1));
	      else SetElement(dres.svec, vecPosition+1, 1, M.smat[k](rowNumber+1, j+1)*s);
	  }
	}
      blockShift += vectorBlockSize;
    } 
  }
  else
  {
    dres.isSparse = false;
    Resize(dres.dvec, size);
    int pos = 1;
    for (int k=0; k<M.nBlock; k++)
    {
      int bs = abs(M.blockStruct[k]);
      for (int j=1; j<=bs; j++)
        for (int i=j; i<=bs; i++)
        {
          INTERVAL buf;
          if (i==j) buf = M.mat[k](i,j);
            else buf = s*M.mat[k](i,j);
          dres.dvec(pos) = buf;
	  pos++;
        }
    }
  }
  return;
}
//----------------------------------------------------------------------
void smat(GeneralMatrix& M, double s, GeneralVector& vec, int* blkStruct, int nb)
{
  CheckVectorRange(nb);
  M.nBlock = nb;
  if (M.blockStruct) delete[] M.blockStruct; 
  M.blockStruct = new int[nb];
  CheckMemory(M.blockStruct);
  M.isSparse = vec.isSparse;
  if (!vec.isSparse)
  { 
    M.mat.resize(nb);
    int pos = 1;
    for (int l=0; l<nb; ++l) 
    {
      int bls = abs(blkStruct[l]);
      M.blockStruct[l] = blkStruct[l];
      Resize(M.mat[l], bls, bls);
      Initialize(M.mat[l], 0.0);
      for (int j=1; j<=bls; j++)
        for (int i=j; i<=bls; i++, pos++)
        {
	  INTERVAL buf;
	  CheckVectorIndex(pos, vec.n);
	  buf = vec.dvec(pos);
          if (i==j) M.mat[l](i,j) = buf;
            else M.mat[l](i,j) = M.mat[l](j,i) = buf/s;
        }
    }
  }
  else
  {
    M.smat.resize(nb);
    BIASINTERVAL *bbuf;
    int NonZeroInBlock;
    int blockShift = 0;
    int NonZeroCount = 0;
    for (int l=0; l<nb; ++l)
    {
      int bls = abs(blkStruct[l]);
      int vecbls = (bls*(bls+1))/2;
      M.blockStruct[l] = blkStruct[l];
      NonZeroInBlock = NonZeroCount;
      while (vec.svec.rowIndices[NonZeroInBlock]-blockShift < vecbls) NonZeroInBlock++;
      NonZeroInBlock -= NonZeroCount;
      Resize(M.smat[l], bls, bls, NonZeroInBlock);
      for (int eleCount=NonZeroCount; eleCount<NonZeroCount+NonZeroInBlock; eleCount++)
      {
	bbuf = vec.svec.theElements + eleCount;      
	int p = vec.svec.rowIndices[eleCount]-blockShift;
        int st = bls;
	int i = 0, j = 0;
	while (p > 0) {p -= st; st--; j++;}
	i = bls + p;	
	if (i==j)
        {
          SetElement(vec.svec, i, j, *((INTERVAL *)bbuf));
        }
        else
        {
          SetElement(vec.svec, i, j, *((INTERVAL *)bbuf)/s);
          SetElement(vec.svec, j, i, *((INTERVAL *)bbuf)/s);
        }
      }
      blockShift += vecbls;      
      NonZeroCount += NonZeroInBlock;
    }
    //if (NonZeroCount != vec.svec.colStarts[1]) cout << "smat: wrong NonZeroCount value...\n"; // just for debugging purposes
  }
}
//----------------------------------------------------------------------
void smat(GeneralMatrix& M, double s, GeneralVector& vec)
{
  if(M.blockStruct == NULL || M.nBlock<=0)
  {
    cout << "smat: Output matrix M has no default structure information.\n";
    return;
  }
  if (!M.isSparse)
  {
    int pos = 1;
    for (int l=0; l<M.nBlock; ++l)
    {
      int bls = abs(M.blockStruct[l]);
      Initialize(M.mat[l], 0.0);
      for (int j=1; j<=bls; j++)
        for (int i=j; i<=bls; i++, pos++)
        {
	  INTERVAL buf;
	  CheckVectorIndex(pos, vec.n);
	  buf = vec.dvec(pos);
          if (i==j) M.mat[l](i,j) = buf;
            else M.mat[l](i,j) = M.mat[l](j,i) = buf/s;
        }
    }
  }
  else
  {
    BIASINTERVAL *bbuf;
    int NonZeroInBlock;
    int blockShift = 0;
    int NonZeroCount = 0;
    for (int l=0; l<M.nBlock; ++l)
    {
      int bls = abs(M.blockStruct[l]);
      int vecbls = (bls*(bls+1))/2;
      NonZeroInBlock = NonZeroCount;
      while (vec.svec.rowIndices[NonZeroInBlock]-blockShift < vecbls) NonZeroInBlock++;
      NonZeroInBlock -= NonZeroCount;
      Resize(M.smat[l], bls, bls, NonZeroInBlock);
      for (int eleCount=NonZeroCount; eleCount<NonZeroCount+NonZeroInBlock; eleCount++)
      {
        bbuf = vec.svec.theElements + eleCount;
        int p = vec.svec.rowIndices[eleCount]-blockShift;
        int st = bls;
	int i = 0, j = 0;
        while (p > 0) {p -= st; st--; j++;}
	i = bls + p;
        if (i==j)
        {
          SetElement(vec.svec, i, j, *((INTERVAL *)bbuf));
        }
        else
        {
          SetElement(vec.svec, i, j, *((INTERVAL *)bbuf)/s);
          SetElement(vec.svec, j, i, *((INTERVAL *)bbuf)/s);
        }
      }
      blockShift += vecbls;
      NonZeroCount += NonZeroInBlock;
    }
    //if (NonZeroCount != vec.svec.colStarts[1]) cout << "smat: wrong NonZeroCount value...\n"; // just for debugging purposes
  }
}
//----------------------------------------------------------------------
