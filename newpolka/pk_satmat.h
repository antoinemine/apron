/* ********************************************************************** */
/* pk_satmat.h: operations on saturation matrices */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.
   
   Please read the COPYING file packaged in the distribution */

/* This header file define operations on saturation matrices. A
   saturation matrix is an array of bits (or an array of bitstrings) used to
   memorize wether a given generator saturate a given constraint. */

/* Saturation matrices are very similar to normal matrices as defined in
   matrix.h, where explanations can be found. */

#ifndef __PK_SATMAT_H__
#define __PK_SATMAT_H__

#include <stdlib.h>
#include "pk_bit.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct satmat_t {
  /* public part */
  bitstring_t** p;
  size_t nbrows;
  size_t nbcolumns;
  /* private part */
  size_t  _maxrows;   /* number of rows allocated */
} satmat_t;

satmat_t* satmat_alloc(size_t nbrows, size_t nbcols);
void satmat_resize_rows(satmat_t* sat, size_t nbrows);
void satmat_resize_cols(satmat_t* sat, size_t nbcols);
satmat_t* satmat_copy_resize_cols(satmat_t* sat, size_t nbcols);
void satmat_free(satmat_t* sat);
void satmat_clear(satmat_t* sat);
satmat_t* satmat_copy(satmat_t* sat);
void satmat_print(satmat_t* sat);
void satmat_fprint(FILE* stream, satmat_t* sat);

bitstring_t satmat_get(satmat_t* sat, size_t i, bitindex_t jx);
void satmat_set(satmat_t* sat, size_t i, bitindex_t jx);
void satmat_clr(satmat_t* sat, size_t i, bitindex_t jx);

satmat_t* satmat_transpose(satmat_t* org, size_t nbcols);

void satmat_exch_rows(satmat_t* sat, size_t l1, size_t l2);
void satmat_move_rows(satmat_t* sat, size_t destrow, size_t orgrow, size_t size);

#ifdef __cplusplus
}
#endif

#endif
