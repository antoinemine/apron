/* ************************************************************************* */
/* pk_satmat.h: saturation matrices */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#ifndef _PK_SATMAT_H_
#define _PK_SATMAT_H_

#include <stdio.h>
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

static inline bitstring_t satmat_get(satmat_t* sat, size_t i, bitindex_t jx);
static inline void satmat_set(satmat_t* sat, size_t i, bitindex_t jx);
static inline void satmat_clr(satmat_t* sat, size_t i, bitindex_t jx);
/* These function allow to read and to clear or set individual bits. i
   indicates the row and jx the column. */

satmat_t* satmat_transpose(satmat_t* org, size_t nbcols);

static inline void satmat_exch_rows(satmat_t* sat, size_t l1, size_t l2);
void satmat_move_rows(satmat_t* sat, size_t destrow, size_t orgrow, size_t size);

/* ********************************************************************** */
/* ********************************************************************** */
/* Inline functions definitions */

static inline bitstring_t satmat_get(satmat_t* sat, size_t i, bitindex_t jx){
  return bitstring_get(sat->p[i],jx);
}
static inline void satmat_set(satmat_t* sat, size_t i, bitindex_t jx){
  bitstring_set(sat->p[i],jx);
}
static inline void satmat_clr(satmat_t* sat, size_t i, bitindex_t jx){
  bitstring_clr(sat->p[i],jx);
}

/* Row exchange. */
static inline void satmat_exch_rows(satmat_t* sat, size_t l1, size_t l2)
{
  bitstring_t* aux=sat->p[l1];
  sat->p[l1]=sat->p[l2];
  sat->p[l2]=aux;
}



#endif
