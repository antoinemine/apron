/*
 * avo_hmat.c
 *
 * Half-matrices - Basic management.
 *
 * APRON Library / Absolute Value Octagonal (AVO) Domain
 *
 * Copyright (C) Liqian Chen & Jiangchao Liu' 2014
 *
 */

/* This file is part of the APRON Library, released under LGPL license.  
   Please read the COPYING file packaged in the distribution.
*/

#include "avo.h"
#include "avo_internal.h"


/* alloced but not initialized */
inline bound_t* avo_hmat_alloc(avo_internal_t* pr, size_t dim)
{
  bound_t* r;
  size_t sz = avo_matsize(dim);
  if (!sz) sz = 1; /* make sure we never malloc a O-sized block */
  checked_malloc(r,bound_t,sz,return NULL;);
  bound_init_array(r,avo_matsize(dim));
  return r;
}

inline void avo_hmat_free(avo_internal_t* pr, bound_t* m, size_t dim)
{

  bound_clear_array(m,avo_matsize(dim));

}

/* all variables are initialized to 0 */
inline bound_t* avo_hmat_alloc_zero(avo_internal_t* pr, size_t dim)
{
  size_t i;
  bound_t* r = avo_hmat_alloc(pr,dim);
  for (i=0;i<avo_matsize(dim);i++) bound_set_int(r[i],0);
  return r;
}

/* all variables are initialized to "don't know" */
inline bound_t* avo_hmat_alloc_top(avo_internal_t* pr, size_t dim)
{
  size_t i;
  bound_t* r = avo_hmat_alloc(pr,dim);
  for (i=0;i<avo_matsize(dim);i++)
	  bound_set_infty(r[i],1);
  for (i=0;i<4*dim;i++)
	  bound_set_int(r[avo_matpos(i,i)],0);
  return r;
}
inline bound_t* avo_hmat_alloc_top_nsc(avo_internal_t* pr, size_t dim)
{
  size_t i;
  bound_t* r = avo_hmat_alloc(pr,dim);
  for (i=0;i<avo_matsize(dim);i++)
	  bound_set_infty(r[i],1);
  return r;
}
inline bound_t* avo_hmat_copy(avo_internal_t* pr, bound_t* m, size_t dim)
{
  if (m) {
    bound_t* r = avo_hmat_alloc(pr,dim);
    bound_set_array(r,m,avo_matsize(dim));
    return r;
  }
  else return NULL;
}

void avo_hmat_fdump(FILE* stream, avo_internal_t* pr, bound_t* m, size_t dim)
{
  size_t i,j;
  for (i=0;i<4*dim;i++) {
    for (j=0;j<=(i|1);j++,m++) {
      if (j) fprintf(stream," ");
      bound_fprint(stream,*m);
    }
    fprintf(stream,"\n");
  }
}
