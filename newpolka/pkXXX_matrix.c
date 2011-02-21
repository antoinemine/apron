/* ********************************************************************** */
/* matrix.c: operations on matrices */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "pkXXX_internal.h"
#include "mf_qsort.h"

/* ********************************************************************** */
/* I. basic operations: creation, destruction, copying and printing */
/* ********************************************************************** */

/* Internal allocation function: the elements are not initialized.
   mr is the maximum number of rows, and nc the number of
   columns. By default, nbrows is initialized to mr . */
matrixXXX_t* _matrixXXX_alloc_int(size_t nbrows, size_t nbcols, bool s)
{
  size_t i;

  assert(nbcols>0 || nbrows==0);

  matrixXXX_t* mat = (matrixXXX_t*)malloc(sizeof(matrixXXX_t));
  mat->nbrows = mat->_maxrows = nbrows;
  mat->nbcolumns = nbcols;
  mat->_sorted = s;
  mat->p = (numintXXX_t**)malloc(nbrows * sizeof(numintXXX_t*));
  for (i=0;i<nbrows;i++){
    mat->p[i] = _vectorXXX_alloc_int(nbcols);
  }
  return mat;
}

/* Standard allocation function, with initialization of the elements. */
matrixXXX_t* matrixXXX_alloc(size_t nbrows, size_t nbcols, bool s)
{
  size_t i;

  assert(nbcols>0 || nbrows==0);

  matrixXXX_t* mat = (matrixXXX_t*)malloc(sizeof(matrixXXX_t));
  mat->nbrows = mat->_maxrows = nbrows;
  mat->nbcolumns = nbcols;
  mat->_sorted = s;
  mat->p = (numintXXX_t**)malloc(nbrows * sizeof(numintXXX_t*));
  for (i=0;i<nbrows;i++){
    mat->p[i] = vectorXXX_alloc(nbcols);
  }
  return mat;
}

/* Reallocation function, to scale up or to downsize a matrix */
void matrixXXX_resize_rows(matrixXXX_t* mat, size_t nbrows)
{
  size_t i;

  assert (nbrows>0);

  if (nbrows > mat->_maxrows){
    mat->p = (numintXXX_t**)realloc(mat->p, nbrows * sizeof(numintXXX_t*));
    for (i=mat->_maxrows; i<nbrows; i++){
      mat->p[i] = vectorXXX_alloc(mat->nbcolumns);
    }
    mat->_sorted = false;
  }
  else if (nbrows < mat->_maxrows){
    for (i=nbrows; i<mat->_maxrows; i++){
      vectorXXX_free(mat->p[i],mat->nbcolumns);
    }
    mat->p = (numintXXX_t**)realloc(mat->p,nbrows * sizeof(numintXXX_t*));
  }
  mat->_maxrows = nbrows;
  mat->nbrows = nbrows;
}

/* Ensures a minimum size */
void matrixXXX_resize_rows_lazy(matrixXXX_t* mat, size_t nbrows)
{
  if (nbrows>mat->_maxrows)
    matrixXXX_resize_rows(mat,nbrows);
  else {
    mat->_sorted = mat->_sorted && nbrows<mat->nbrows;
    mat->nbrows = nbrows;
  }
}

/* Minimization */
void matrixXXX_minimize(matrixXXX_t* mat)
{
  matrixXXX_resize_rows(mat,mat->nbrows);
}

/* Deallocation function. */
void matrixXXX_free(matrixXXX_t* mat)
{
  size_t i;

  for (i=0;i<mat->_maxrows;i++){
    vectorXXX_free(mat->p[i],mat->nbcolumns);
  }
  free(mat->p);
  free(mat);
}

/* Set all elements to zero. */
void matrixXXX_clear(matrixXXX_t* mat)
{
  size_t i,j;
  for (i=0; i<mat->nbrows; i++){
    for (j=0; j<mat->nbcolumns; j++){
      numintXXX_set_int(mat->p[i][j],0);
    }
  }
}

/* Create a copy of the matrix of size nbrows (and not
   _maxrows). Only ``used'' rows are copied. */
matrixXXX_t* matrixXXX_copy(matrixXXX_t* mat)
{
  size_t i,j;
  matrixXXX_t* nmat = _matrixXXX_alloc_int(mat->nbrows,mat->nbcolumns,mat->_sorted);
  for (i=0;i<mat->nbrows;i++){
    for (j=0; j<mat->nbcolumns; j++){
      numintXXX_init_set(nmat->p[i][j],mat->p[i][j]);
    }
  }
  return nmat;
}

/* Return true iff the matrices are equal, coeff by coeff */
bool matrixXXX_equal(matrixXXX_t* mata, matrixXXX_t* matb)
{
  int i;
  size_t j;
  bool res;

  res = mata->nbrows==matb->nbrows && mata->nbcolumns==matb->nbcolumns;
  if (!res) return res;
  for (i=(int)mata->nbrows-1;i>=0;i--){
    for (j=0; j<mata->nbcolumns; j++){
      res = numintXXX_equal(mata->p[i][j],matb->p[i][j]);
      if (!res) return res;
    }
  }
  return res;
}

/* Raw printing function. */
void matrixXXX_fprint(FILE* stream, matrixXXX_t* mat)
{
  size_t i,j;
  fprintf(stream,"%lu %lu\n",
	  (unsigned long)mat->nbrows, (unsigned long)mat->nbcolumns);
  for (i=0;i<mat->nbrows;i++) {
    for (j=0;j<mat->nbcolumns;j++){
      numintXXX_fprint(stream,mat->p[i][j]);
      fprintf(stream," ");
    }
    fprintf(stream,"\n");
  }
}
void matrixXXX_print(matrixXXX_t* mat)
{
  matrixXXX_fprint(stdout,mat);
}


/* ********************************************************************** */
/* II.Operation on rows */
/* ********************************************************************** */

/* compare_rows compares rows of matrix, exch_rows exchanges
   two rows; normalize_row normalizes a row of a matrix but without
   considering the first coefficient; combine_rows combine rows
   l1 and l2 and puts the result in l3 such that
   l3[k] is zero. */

int matrixXXX_compare_rows(pkXXX_internal_t* pk,
			   matrixXXX_t* mat, size_t l1, size_t l2)
{
  return vectorXXX_compare(pk,
			   mat->p[l1],
			   mat->p[l2],mat->nbcolumns);
}
void matrixXXX_normalize_row(pkXXX_internal_t* pk,
			     matrixXXX_t* mat, size_t l)
{
  vectorXXX_normalize(pk, mat->p[l],mat->nbcolumns);
}
void matrixXXX_combine_rows(pkXXX_internal_t* pk,
			    matrixXXX_t* mat, size_t l1, size_t l2, size_t l3, size_t k)
{
  vectorXXX_combine(pk,
		    mat->p[l1],
		    mat->p[l2],
		    mat->p[l3],k,mat->nbcolumns);
}
void matrixXXX_exch_rows(matrixXXX_t* mat, size_t l1, size_t l2)
{
  numintXXX_t* aux=mat->p[l1];
  mat->p[l1]=mat->p[l2];
  mat->p[l2]=aux;
}

void matrixXXX_move_rows(matrixXXX_t* mat, size_t destrow, size_t orgrow, size_t size)
{
  int offset;
  int i;

  offset = destrow-orgrow;
  if (offset>0){
    assert(destrow+size<=mat->_maxrows);
    for (i=(int)(destrow+size)-1; i>=(int)destrow; i--){
      matrixXXX_exch_rows(mat,(size_t)i,(size_t)(i-offset));
    }
  } else {
    assert(orgrow+size<=mat->_maxrows);
    for(i=(int)destrow; i<(int)(destrow+size); i++){
      matrixXXX_exch_rows(mat,(size_t)i,(size_t)(i-offset));
    }
  }
}

/* ********************************************************************** */
/* */
/* ********************************************************************** */

bool matrixXXX_normalize_constraint(pkXXX_internal_t* pk,
				    matrixXXX_t* mat,
				    ap_dimension_t dim)
{
  bool change1, change2;
  size_t i;

  if ( pk->strict && dim.reald>0 ){
    change2=false;
    for (i=0; i<mat->nbrows; i++){
      change1 = vectorXXX_normalize_constraint(pk,mat->p[i],dim);
      change2 = change2 || change1;
    }
    if (change2){
      mat->_sorted = false;
      /* Add again \xi-\epsilon<=1 */
      size_t nbrows= mat->nbrows;
      matrixXXX_resize_rows_lazy(mat,nbrows+1);
      vectorXXX_clear(mat->p[nbrows],mat->nbcolumns);
      numintXXX_set_int(mat->p[nbrows][0],1);
      numintXXX_set_int(mat->p[nbrows][polka_cst],1);
      numintXXX_set_int(mat->p[nbrows][polka_eps],-1);
    }
    return change2;
  }
  else
    return false;
}
bool matrixXXX_normalize_constraint_int(pkXXX_internal_t* pk,
					matrixXXX_t* mat,
					ap_dimension_t dim)
{
  bool change1, change2;
  size_t i;

  if (dim.intd>0){
    change2=false;
    for (i=0; i<mat->nbrows; i++){
      change1 = vectorXXX_normalize_constraint_int(pk,mat->p[i],dim);
      change2 = change2 || change1;
    }
    if (change2)
      mat->_sorted = false;
    return change2;
  }
  else
    return false;
}


/* ********************************************************************** */
/* III. Sorting and merging */
/* ********************************************************************** */

/* ====================================================================== */
/* III.1 Sorting */
/* ====================================================================== */

/* We use here the quick sort. There is here no handling of doublons */
typedef struct qsort_man_t {
  pkXXX_internal_t* pk;
  size_t size;
} qsort_man_t;

static int qsort_rows_compar(void* qsort_man, void* pq1, void* pq2)
{
  qsort_man_t* qm = (qsort_man_t*)qsort_man;
  numintXXX_t* q1 = *((numintXXX_t**)pq1);
  numintXXX_t* q2 = *((numintXXX_t**)pq2);
  return vectorXXX_compare(qm->pk,q1,q2,qm->size);
}

void matrixXXX_sort_rows(pkXXX_internal_t* pk,
			 matrixXXX_t* mat)
{
  qsort_man_t qsort_man;

  if (!mat->_sorted){
    qsort_man.pk = pk;
    qsort_man.size = mat->nbcolumns;
    qsort2(mat->p, mat->nbrows, sizeof(numintXXX_t*),
	   qsort_rows_compar,
	   &qsort_man);
    mat->_sorted = true;
  }
}

/* This variant permutes also the saturation matrix together with the matrix.
   There is here no handling of doublons. */

typedef struct qsort_t {
  numintXXX_t* p;
  bitstring_t* satp;
} qsort_t;

static int qsort_rows_with_sat_compar(void* qsort_man, void* q1, void* q2)
{
  qsort_man_t* qm = (qsort_man_t*)qsort_man;
  qsort_t* qs1 = (qsort_t*)q1;
  qsort_t* qs2 = (qsort_t*)q2;
  return vectorXXX_compare( qm->pk,
			    qs1->p,
			    qs2->p,
			    qm->size );
}

void matrixXXX_sort_rows_with_sat(pkXXX_internal_t* pk,
				  matrixXXX_t* mat, satmat_t* sat)
{
  size_t i;
  qsort_t* qsort_tab;
  qsort_man_t qsort_man;

  if (!mat->_sorted){
    qsort_man.pk = pk;
    qsort_man.size = mat->nbcolumns;
    qsort_tab = (qsort_t*)malloc(mat->nbrows * sizeof(qsort_t));
    for (i=0; i<mat->nbrows; i++){
      qsort_tab[i].p = mat->p[i];
      qsort_tab[i].satp = sat->p[i];
    }
    qsort2(qsort_tab,
	   mat->nbrows, sizeof(qsort_t),
	   qsort_rows_with_sat_compar,
	   &qsort_man);
    for (i=0; i<mat->nbrows; i++){
      mat->p[i] = qsort_tab[i].p;
      sat->p[i] = qsort_tab[i].satp;
    }
    free(qsort_tab);
    mat->_sorted = true;
  }
}

/* ====================================================================== */
/* III.2 Append */
/* ====================================================================== */

/* Appending matrices */
matrixXXX_t* matrixXXX_append(matrixXXX_t* mata, matrixXXX_t* matb)
{
  matrixXXX_t* mat;
  size_t i,l;

  assert (mata->nbcolumns == matb->nbcolumns);
  mat = _matrixXXX_alloc_int(mata->nbrows+matb->nbrows,mata->nbcolumns,false);
  for (i=0;i<mata->nbrows; i++){
    for (l=0; l<mata->nbcolumns; l++)
      numintXXX_init_set(mat->p[i][l],mata->p[i][l]);
  }
  for (i=0;i<matb->nbrows; i++){
    for (l=0; l<matb->nbcolumns; l++)
      numintXXX_init_set(mat->p[mata->nbrows+i][l],matb->p[i][l]);
  }
  return mat;
}

void matrixXXX_append_with(matrixXXX_t* mat, matrixXXX_t* cmat)
{
  size_t i,l;
  size_t nbrows;

  assert (mat->nbcolumns == cmat->nbcolumns);

  nbrows = mat->nbrows;
  matrixXXX_resize_rows_lazy(mat,nbrows+cmat->nbrows);
  for (i=0;i<cmat->nbrows; i++){
    for (l=0; l<cmat->nbcolumns; l++)
      numintXXX_set(mat->p[nbrows+i][l],cmat->p[i][l]);
  }
  mat->_sorted = false;
}

/* Given matrices with rows p1,p2,... and q1,q2,...., fills the initial matrix
   with rows q1,q2,...,p1,p2,.... */

void matrixXXX_revappend_with(matrixXXX_t* mat, matrixXXX_t* cmat)
{
  int i;
  size_t l;
  size_t nbrows;

  assert(mat->nbcolumns == cmat->nbcolumns);
  nbrows = mat->nbrows;
  matrixXXX_resize_rows_lazy(mat,nbrows+cmat->nbrows);
  for (i=nbrows-1; i>=0; i--){
    /* exchanging rows i and i+cmat->nbrows */
    numintXXX_t* q = mat->p[i+cmat->nbrows];
    mat->p[i+cmat->nbrows] = mat->p[i];
    mat->p[i] = q;
  }
  for (i=0; i<(int)cmat->nbrows; i++){
    for (l=0;l<cmat->nbcolumns; l++){
      numintXXX_set(mat->p[i][l],cmat->p[i][l]);
    }
  }
}


/* ====================================================================== */
/* III.3 Addition of sorted rows */
/* ====================================================================== */

/* Merging with sorting */

matrixXXX_t* matrixXXX_merge_sort(pkXXX_internal_t* pk,
				  matrixXXX_t* mata, matrixXXX_t* matb)
{
  size_t i,ia,ib,l;
  matrixXXX_t* mat;
  size_t nbrows;
  assert (mata->nbcolumns == matb->nbcolumns);
  if (!mata->_sorted || !matb->_sorted){
    mat = matrixXXX_append(mata,matb);
    matrixXXX_sort_rows(pk,mat);
  }
  else {
    mat = _matrixXXX_alloc_int(mata->nbrows+matb->nbrows,mata->nbcolumns,true);
    i = 0;
    ia = 0;
    ib = 0;
    while (ia < mata->nbrows && ib < matb->nbrows) {
      int res = vectorXXX_compare(pk,
				  mata->p[ia],
				  matb->p[ib],
				  mat->nbcolumns);
      if (res<=0){
	for (l=0; l<mat->nbcolumns; l++)
	  numintXXX_init_set(mat->p[i][l],mata->p[ia][l]);
	ia++;
	if (res==0) ib++;
      }
      else {
	for (l=0; l<mat->nbcolumns; l++)
	  numintXXX_init_set(mat->p[i][l],matb->p[ib][l]);
	ib++;
      }
      i++;
    }
    /* does some constraint remain ? */
    if (ia < mata->nbrows) {
      do {
	for (l=0; l<mat->nbcolumns; l++)
	  numintXXX_init_set(mat->p[i][l],mata->p[ia][l]);
	ia++; i++;
      } while (ia < mata->nbrows);
    } else {
      while (ib < matb->nbrows){
	for (l=0; l<mat->nbcolumns; l++)
	  numintXXX_init_set(mat->p[i][l],matb->p[ib][l]);
	ib++; i++;
      }
    }
    nbrows = (size_t)i;
    /* initialize last rows of mat to zero */
    while (i<mat->nbrows){
      for (l=0; l<mat->nbcolumns; l++)
	numintXXX_init(mat->p[i][l]);
      i++;
    }
    mat->nbrows = nbrows;
  }

  return mat;
}

/* This function adds to a sorted matrix the rows of another sorted matrix
   and leaves the resulting matrix sorted. Identical rows are eliminated. The
   modified matrix is supposed to be big enough to store the new rows. */

void matrixXXX_merge_sort_with(pkXXX_internal_t* pk,
			       matrixXXX_t* mata, matrixXXX_t* matb)
{
  size_t i,ia,ib,j,k,nbrows,nbrowsa, nbcols;
  numintXXX_t** numintpp;

  assert (mata->nbcolumns == matb->nbcolumns);
  assert (mata->_sorted && matb->_sorted);

  nbrowsa = mata->nbrows;
  nbcols = mata->nbcolumns;
  matrixXXX_resize_rows_lazy(mata, nbrowsa + matb->nbrows);

  /* one adds the coefficients of matb to mata */
  for (i=0; i<matb->nbrows; i++){
    for (j=0; j<nbcols; j++){
      numintXXX_set(mata->p[nbrowsa+i][j],matb->p[i][j]);
    }
  }
  /* now we fill numintpp, which will contain the unsorted rows */
  nbrows = nbrowsa + matb->nbrows;
  numintpp = malloc(nbrows*sizeof(numintXXX_t*));
  for (i=0; i<nbrows; i++){
    numintpp[i] = mata->p[i];
  }

  /* Now we fill mata->p from numintpp */
  ia = 0;
  ib = nbrowsa;
  i = 0;
  k = 0;
  while (ia < nbrowsa && ib < nbrows){
    int res = vectorXXX_compare(pk,
				numintpp[ia],
				numintpp[ib],nbcols);
    if (res<=0){
      mata->p[i] = numintpp[ia]; ia++;
      if (res==0){
	k++;
	mata->p[nbrows-k] = numintpp[ib]; ib++;
      }
    }
    else {
      mata->p[i] = numintpp[ib]; ib++;
    }
    i++;
  }
  /* Are there still constraints ? */
  while (ia < nbrowsa){
    mata->p[i] = numintpp[ia];
    i++; ia++;
  }
  while (ib < nbrows){
    mata->p[i] = numintpp[ib];
    i++; ib++;
  }
  mata->nbrows -= k;
  mata->_sorted = true;
  free(numintpp);
}
