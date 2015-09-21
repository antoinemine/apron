/* ********************************************************************** */
/* pk_satmat.c: operations on saturation matrices */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.
   
   Please read the COPYING file packaged in the distribution */

/* Operations on saturation matrices */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "pk_satmat.h"

/* ********************************************************************** */
/* I. basic operations: creation, destruction, copying and printing */
/* ********************************************************************** */

/* Set all bits to zero. */
void satmat_clear(satmat_t* sat)
{
  size_t i,j;
  for (i=0; i<sat->nbrows; i++)
    for (j=0; j<sat->nbcolumns; j++)
      sat->p[i][j] = 0;
}

/* Standard allocation function, with initialization of the elements. */
satmat_t* satmat_alloc(size_t nbrows, size_t nbcols)
{
  size_t i,j;

  satmat_t* sat = (satmat_t*)malloc(sizeof(satmat_t));
  sat->nbrows = sat->_maxrows = nbrows;
  sat->nbcolumns = nbcols;
  sat->p = (bitstring_t**)malloc(nbrows*sizeof(bitstring_t*));
  for (i=0; i<nbrows; i++){
    sat->p[i] = bitstring_alloc(nbcols);
    for (j=0; j<sat->nbcolumns; j++)
      sat->p[i][j] = 0;
  }
  return sat;
}

/* Deallocation function. */
void satmat_free(satmat_t* sat)
{
  size_t i;

  for (i=0;i<sat->_maxrows;i++){
    bitstring_free(sat->p[i]);
  }
  free(sat->p);
  free(sat);
}

/* Reallocation function, to scale up or to downsize a matrix */
void satmat_resize_rows(satmat_t* sat, size_t nbrows)
{
  size_t i;

  if (nbrows > sat->_maxrows){
    sat->p = (bitstring_t**)realloc(sat->p, nbrows * sizeof(bitstring_t*));
    for (i=sat->nbrows; i<nbrows; i++){
      sat->p[i] = bitstring_alloc(sat->nbcolumns);
    }
  }
  else if (nbrows < sat->_maxrows){
    for (i=nbrows; i<sat->_maxrows; i++){
      bitstring_free(sat->p[i]);
    }
    sat->p = (bitstring_t**)realloc(sat->p,nbrows * sizeof(bitstring_t*));
  } 
  sat->nbrows = nbrows;
  sat->_maxrows = nbrows;
}

/* Reallocation function, to scale up or to downsize a matrix */
void satmat_resize_cols(satmat_t* sat, size_t nbcols)
{
  size_t i,j;

  if (nbcols!=sat->nbcolumns){
    for (i=0; i<sat->_maxrows; i++){
      sat->p[i] = bitstring_realloc(sat->p[i],nbcols);
      for (j=sat->nbcolumns; j<nbcols; j++){
	sat->p[i][j] = 0;
      }
    }
    sat->nbcolumns = nbcols;
  }
}

/* Create a copy of the matrix of size nbrows (and not
   _maxrows) and extends columns. Only ``used'' rows are copied. */
satmat_t* satmat_copy_resize_cols(satmat_t* sat, size_t nbcols)
{
  size_t i,j;
  satmat_t* nsat;

  assert(nbcols>=sat->nbcolumns);
  nsat = satmat_alloc(sat->nbrows,nbcols);
  for (i=0; i<sat->nbrows; i++){
    for (j=0; j<sat->nbcolumns; j++){
      nsat->p[i][j] = sat->p[i][j];
    }
    for (j=sat->nbcolumns; j<nbcols; j++)
      nsat->p[i][j] = 0;
  }
  return nsat;
}

/* Create a copy of the matrix of size nbrows (and not
   _maxrows). Only ``used'' rows are copied. */
satmat_t* satmat_copy(satmat_t* sat)
{
  size_t i,j;
  satmat_t* nsat = satmat_alloc(sat->nbrows,sat->nbcolumns);
  for (i=0; i<sat->nbrows; i++){
    for (j=0; j<sat->nbcolumns; j++){
      nsat->p[i][j] = sat->p[i][j];
    }
  }
  return nsat;
}

/* Raw printing function. */
void satmat_fprint(FILE* stream, satmat_t* sat)
{
  size_t i;

  fprintf(stream,"%lu %lu\n",
	  (unsigned long)sat->nbrows,(unsigned long)sat->nbcolumns);
  for (i=0; i<sat->nbrows; i++){
    bitstring_fprint(stream,sat->p[i],sat->nbcolumns);
    fprintf(stream,"\n");
  }
}
void satmat_print(satmat_t* sat)
{
  satmat_fprint(stdout,sat);
}

/* ********************************************************************** */
/* II. Bit operations */
/* ********************************************************************** */

/* These function allow to read and to clear or set individual bits. i
   indicates the row and jx the column. */

bitstring_t satmat_get(satmat_t* sat, size_t i, bitindex_t jx){
  return bitstring_get(sat->p[i],jx);
}
void satmat_set(satmat_t* sat, size_t i, bitindex_t jx){
  bitstring_set(sat->p[i],jx);
}
void satmat_clr(satmat_t* sat, size_t i, bitindex_t jx){
  bitstring_clr(sat->p[i],jx);
}

/* ********************************************************************** */
/* III. Matrix operations */
/* ********************************************************************** */

/* Transposition.

nbcols indicates the number of bits to be transposed (the number of columns of
the matrix is the size of the row of bitstring_t, not the number of bits really
used). */

satmat_t* satmat_transpose(satmat_t* org, size_t nbcols)
{
  bitindex_t i,j;
  satmat_t* dest = satmat_alloc(nbcols,bitindex_size(org->nbrows));

  for (i = bitindex_init(0); i.index < org->nbrows; bitindex_inc(&i) ) {
    for (j = bitindex_init(0); j.index < nbcols; bitindex_inc(&j) ){
      if (satmat_get(org,i.index,j)) satmat_set(dest,j.index,i);
    }
  }
  return dest;
}

/* Row exchange. */
void satmat_exch_rows(satmat_t* sat, size_t l1, size_t l2)
{
  bitstring_t* aux=sat->p[l1];
  sat->p[l1]=sat->p[l2];
  sat->p[l2]=aux;
}

void satmat_move_rows(satmat_t* sat, size_t destrow, size_t orgrow, size_t size)
{
  int i,offset;

  offset = destrow-orgrow;
  if (offset>0){
    assert(destrow+size<=sat->_maxrows);
    for (i=(int)(destrow+size)-1; i>=(int)destrow; i--){
      satmat_exch_rows(sat,(size_t)i,(size_t)(i-offset));
    }
  } else {
    assert(orgrow+size<=sat->_maxrows);
    for(i=(int)destrow; i<(int)(destrow+size); i++){
      satmat_exch_rows(sat,(size_t)i,(size_t)(i-offset));
    }
  }
}
