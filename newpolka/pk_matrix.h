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

typedef struct pk_matrix_t {
  /* public part */
  numint_t** p;     /* array of pointers to rows */
  size_t nbrows;      /* number of effective rows */
  size_t nbcolumns;   /* size of rows */

  /* private part */
  size_t  _maxrows;   /* number of rows allocated */
  bool _sorted;
} pk_matrix_t;

/* Normal functions */

/* information about private part */
static inline size_t pk_matrix_get_maxrows(pk_matrix_t* mat)
{ return mat->_maxrows; }
static inline bool pk_matrix_is_sorted(pk_matrix_t* mat)
{ return mat->_sorted; }

/* Basic Operations */
pk_matrix_t* pk_matrix_alloc(size_t nbrows, size_t nbcols, bool s);
void      pk_matrix_resize_rows(pk_matrix_t* mat, size_t nbrows);
void      pk_matrix_resize_rows_lazy(pk_matrix_t* mat, size_t nbrows);
void      pk_matrix_minimize(pk_matrix_t* mat);
void      pk_matrix_free(pk_matrix_t* mat);
void      pk_matrix_clear(pk_matrix_t* mat);
void      pk_matrix_print(pk_matrix_t* mat);
void      pk_matrix_fprint(FILE* stream, pk_matrix_t* mat);
pk_matrix_t* pk_matrix_copy(pk_matrix_t* mat);
bool      pk_matrix_equal(pk_matrix_t* mata, pk_matrix_t* matb);

/* Operations on rows */
void pk_matrix_normalize_row(pk_internal_t* pk,
			  pk_matrix_t* mat, size_t l);
void pk_matrix_combine_rows(pk_internal_t* pk,
			 pk_matrix_t* mat, size_t l1, size_t l2, size_t l3, size_t k);
int pk_matrix_compare_rows(pk_internal_t* pk,
			pk_matrix_t* mat, size_t l1, size_t l2);
void pk_matrix_exch_rows(pk_matrix_t* mat, size_t l1, size_t l2);
void pk_matrix_move_rows(pk_matrix_t* mat, size_t destrow, size_t orgrow, size_t size);

/* Normalization of rows */
bool pk_matrix_normalize_constraint(pk_internal_t* pk,
				   pk_matrix_t* mat, 
				   size_t intdim, size_t realdim);
bool pk_matrix_normalize_constraint_int(pk_internal_t* pk,
				       pk_matrix_t* mat, 
				       size_t intdim, size_t realdim);

/* Sorting & Merging */
void pk_matrix_sort_rows(pk_internal_t* pk,
		      pk_matrix_t* mat);
void pk_matrix_sort_rows_with_sat(pk_internal_t* pk,
			       pk_matrix_t* mat, satmat_t* sat);

pk_matrix_t* pk_matrix_append(pk_matrix_t* ma, pk_matrix_t* mb);
void pk_matrix_append_with(pk_matrix_t* ma, pk_matrix_t* mb);
void pk_matrix_revappend_with(pk_matrix_t* ma, pk_matrix_t* mb);

pk_matrix_t* pk_matrix_merge_sort(pk_internal_t* pk,
			    pk_matrix_t* ma, pk_matrix_t* mb);
void pk_matrix_merge_sort_with(pk_internal_t* pk,
			    pk_matrix_t* mat, pk_matrix_t* cmat);


/* Predicates that can be useful for users */
static inline 
bool pk_matrix_is_row_dummy_constraint(pk_internal_t* pk,
				    pk_matrix_t* mat,
				    size_t l)
{
  return vector_is_dummy_constraint(pk,
				    mat->p[l],mat->nbcolumns);
}


/* Functions meant to be internal */
pk_matrix_t* _pk_matrix_alloc_int(size_t nr, size_t nc, bool s);

#ifdef __cplusplus
}
#endif

#endif
