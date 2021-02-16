/*
 * avo_hmat.c
 *
 * Half-matrices - Basic management.
 *
 * APRON Library / Absolute Value Octagonal (AVO) Domain
 *
 * Copyright (C) Liqian Chen & Jiangchao Liu' 2014-
 *
 */

/* This file is part of the APRON Library, released under LGPL license.  
   Please read the COPYING file packaged in the distribution.
*/

#include "avo.h"
#include "avo_internal.h"


/* We consider matrices of 4n*4n upper bounds.
   Let us denote by (i,j) the matrix element at line i, column j; the matrix
   induces the following constraints:

     Vj-Vi <= (2i,2j)
     Vj+Vi <= (2i+1,2j)
    -Vj-Vi <= (2i,2j+1)
    -Vj+Vi <= (2i+1,2j+1)         (* Mat[0:2n, 0:2n] stays the same as octagon *)


     |Vj|-Vi <= (2i,2dim+2j)      (* redundant *)
     |Vj|+Vi <= (2i+1,2dim+2j)    (* redundant *)
    -|Vj|-Vi <= (2i,2dim+2j+1)
    -|Vj|+Vi <= (2i+1,2dim+2j+1)

     Vj-|Vi| <= (2dim+2i,2j)
     Vj+|Vi| <= (2dim+2i+1,2j)    (* redundant *)
    -Vj-|Vi| <= (2dim+2i,2j+1)
    -Vj+|Vi| <= (2dim+2i+1,2j+1)  (* redundant *)

     |Vj|-|Vi| <= (2dim+2i,2dim+2j)  (* redundant *)
     |Vj|+|Vi| <= (2dim+2i+1,2dim+2j)  (* redundant *)
    -|Vj|-|Vi| <= (2dim+2i,2dim+2j+1)
    -|Vj|+|Vi| <= (2dim+2i+1,2dim+2j+1)  (* redundant *)

   Actually, this representation is redundant, and so, we manipulate
   2x2 block lower-triangular matrices.
   Only elements (i,j) such that j/2 <= i/2 are represented:

           x0 -x0 x1 -x1 |x0| -|x0| |x1| -|x1|
      j ->  0 1 2 3 4 5 6 7
            ___
        0  |_|_|
        1  |_|_|___
  i ->  2  |_|_|_|_|
        3  |_|_|_|_|___
        4  |_|_|_|_|_|_|
        5  |_|_|_|_|_|_|___
        6  |_|_|_|_|_|_|_|_|
        7  |_|_|_|_|_|_|_|_|

                 j
           0        -2x0
           2x0      0
       i
           x0-x1    -x0-x1    0        -2x1
           x0+x1    -x0+x1    2x1      0

           x0-|x0|  -x0-|x0|  x1-|x0|  -x1-|x0|  0          -2|x0|
           x0+|x0|  -x0+|x0|  x1+|x0|  -x1+|x0|  2|x0|      0

           x0-|x1|  -x0-|x1|  x1-|x1|  -x1-|x1|  |x0|-|x1|  -|x0|-|x1|   0       -2|x1|
           x0+|x1|  -x0+|x1|  x1+|x1|  -x1+|x1|  |x0|+|x1|  -|x0|+|x1|   2|x1|   0


   Elements such that j/2 > i/2 are retreived by coherence: (i,j) = (j^1,i^1)
*/

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
  free(m);
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

/* The strictness of all difference bounds are initialized to "don't know" */
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

void avo_hmat_fdump(FILE* stream, bound_t* m, size_t dim)
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
