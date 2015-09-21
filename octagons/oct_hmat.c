/*
 * oct_hmat.c
 *
 * Half-matrices - Basic management.
 *
 * APRON Library / Octagonal Domain
 *
 * Copyright (C) Antoine Mine' 2006
 *
 */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution.
*/

#include "oct.h"
#include "oct_internal.h"


/* We consider matrices of 2n*2n upper bounds.
   Let us denote by (i,j) the matrix element at line i, column j; the matrix
   induces the following constraints:
     Vj-Vi <= (2i,2j)
     Vj+Vi <= (2i+1,2j)
    -Vj-Vi <= (2i,2j+1)
    -Vj+Vi <= (2i+1,2j+1)

   Actually, this representation is redudant, and so, we manipulate 
   2x2 block lower-triangular matrices. 
   Only elements (i,j) such that j/2 <= i/2 are represented:

       j ->  0 1 2 3 4 5
            ___
        0  |_|_|
        1  |_|_|___
  i ->  2  |_|_|_|_|
        3  |_|_|_|_|___
        4  |_|_|_|_|_|_|
        5  |_|_|_|_|_|_|

                 
                 j
             0     -2x0
            2x0      0
       i
           x0-x1  -x0-x1      0   -2x1
           x0+x1  -x0+x1     2x1    0


   Elements such that j/2 > i/2 are retreived by coherence: (i,j) = (j^1,i^1)
*/


/* alloced but not initialized */
inline bound_t* hmat_alloc(oct_internal_t* pr, size_t dim)
{
  bound_t* r;
  size_t sz = matsize(dim);
  if (!sz) sz = 1; /* make sure we never malloc a O-sized block */
  checked_malloc(r,bound_t,sz,return NULL;);
  bound_init_array(r,matsize(dim));
  return r;
}

inline void hmat_free(oct_internal_t* pr, bound_t* m, size_t dim)
{
  bound_clear_array(m,matsize(dim));
  free(m);
}

/* all variables are initialized to 0 */
inline bound_t* hmat_alloc_zero(oct_internal_t* pr, size_t dim)
{
  size_t i;
  bound_t* r = hmat_alloc(pr,dim);
  for (i=0;i<matsize(dim);i++) bound_set_int(r[i],0);
  return r;
}

/* all variables are initialized to "don't know" */
inline bound_t* hmat_alloc_top(oct_internal_t* pr, size_t dim)
{
  size_t i;
  bound_t* r = hmat_alloc(pr,dim);
  for (i=0;i<matsize(dim);i++) bound_set_infty(r[i],1);
  for (i=0;i<2*dim;i++) bound_set_int(r[matpos(i,i)],0);
  return r;
}

inline bound_t* hmat_copy(oct_internal_t* pr, bound_t* m, size_t dim)
{
  if (m) {
    bound_t* r = hmat_alloc(pr,dim);
    bound_set_array(r,m,matsize(dim));
    return r;
  }
  else return NULL;
}

void hmat_fdump(FILE* stream, oct_internal_t* pr, bound_t* m, size_t dim)
{
  size_t i,j;
  for (i=0;i<2*dim;i++) {
    for (j=0;j<=(i|1);j++,m++) {
      if (j) fprintf(stream," ");
      bound_fprint(stream,*m);
    }
    fprintf(stream,"\n");
  }
}
