/* ********************************************************************** */
/* pk_matrix.h: operations on matrices */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

/*
A matrix is represented in the following manner: the coefficients are stored
in an private array of pkint_t p_init of size
_maxrows*nbcolumns. To access to elements, one use an array of
pointers p, the $i^{\mbox{\scriptsize nth}}$ element of which points
to the $i^{\mbox{\scriptsize nth}}$ row of the matrix. This array is
initialized by the constructor. The advantage of this representation is to be
able to exchange easily rows of the matrix by exchanging the pointers,
without having to allocate at construction _maxrows arrays for each
rows.  nbrows indicates that only the first nbrows rows are
used.
*/

#ifndef __PK_MATRIX_H__
#define __PK_MATRIX_H__

#include "pk_config.h"
#include "pk_vector.h"
#include "pk_satmat.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct matrix_t {
  /* public part */
  numint_t** p;     /* array of pointers to rows */
  size_t nbrows;      /* number of effective rows */
  size_t nbcolumns;   /* size of rows */

  /* private part */
  size_t  _maxrows;   /* number of rows allocated */
  bool _sorted;
} matrix_t;

/* Normal functions */

/* information about private part */
static inline size_t matrix_get_maxrows(matrix_t* mat)
{ return mat->_maxrows; }
static inline bool matrix_is_sorted(matrix_t* mat)
{ return mat->_sorted; }

/* Basic Operations */
matrix_t* matrix_alloc(size_t nbrows, size_t nbcols, bool s);
void      matrix_resize_rows(matrix_t* mat, size_t nbrows);
void      matrix_resize_rows_lazy(matrix_t* mat, size_t nbrows);
void      matrix_minimize(matrix_t* mat);
void      matrix_free(matrix_t* mat);
void      matrix_clear(matrix_t* mat);
void      matrix_print(matrix_t* mat);
void      matrix_fprint(FILE* stream, matrix_t* mat);
matrix_t* matrix_copy(matrix_t* mat);
bool      matrix_equal(matrix_t* mata, matrix_t* matb);

/* Operations on rows */
void matrix_normalize_row(pk_internal_t* pk,
			  matrix_t* mat, size_t l);
void matrix_combine_rows(pk_internal_t* pk,
			 matrix_t* mat, size_t l1, size_t l2, size_t l3, size_t k);
int matrix_compare_rows(pk_internal_t* pk,
			matrix_t* mat, size_t l1, size_t l2);
void matrix_exch_rows(matrix_t* mat, size_t l1, size_t l2);
void matrix_move_rows(matrix_t* mat, size_t destrow, size_t orgrow, size_t size);

/* Normalization of rows */
bool matrix_normalize_constraint(pk_internal_t* pk,
				   matrix_t* mat, 
				   size_t intdim, size_t realdim);
bool matrix_normalize_constraint_int(pk_internal_t* pk,
				       matrix_t* mat, 
				       size_t intdim, size_t realdim);

/* Sorting & Merging */
void matrix_sort_rows(pk_internal_t* pk,
		      matrix_t* mat);
void matrix_sort_rows_with_sat(pk_internal_t* pk,
			       matrix_t* mat, satmat_t* sat);

matrix_t* matrix_append(matrix_t* ma, matrix_t* mb);
void matrix_append_with(matrix_t* ma, matrix_t* mb);
void matrix_revappend_with(matrix_t* ma, matrix_t* mb);

matrix_t* matrix_merge_sort(pk_internal_t* pk,
			    matrix_t* ma, matrix_t* mb);
void matrix_merge_sort_with(pk_internal_t* pk,
			    matrix_t* mat, matrix_t* cmat);


/* Predicates that can be useful for users */
static inline 
bool matrix_is_row_dummy_constraint(pk_internal_t* pk,
				    matrix_t* mat,
				    size_t l)
{
  return vector_is_dummy_constraint(pk,
				    mat->p[l],mat->nbcolumns);
}


/* Functions meant to be internal */
matrix_t* _matrix_alloc_int(size_t nr, size_t nc, bool s);

#ifdef __cplusplus
}
#endif

#endif
