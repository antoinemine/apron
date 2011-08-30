/*
 * octXXX_closure.c
 *
 * Half-matrices - Closure algorithms
 *
 * APRON Library / Octagonal Domain
 *
 * Copyright (C) Antoine Mine' 2006
 *
 */

/* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution.
*/

#include "octXXX.h"
#include "octXXX_internal.h"


/* All closures are in-place. */


/* ============================================================ */
/* Full Closure */
/* ============================================================ */


/* unary constraint propagation */
bool hmatXXX_s_step(boundXXX_t* m, size_t dim)
{
  size_t i,j,k;
  boundXXX_t *c,ik,ij;

  boundXXX_init(ik); boundXXX_init(ij);

  /* lone S step */
  c = m;
  for (i=0;i<2*dim;i++) {
    boundXXX_mul_2exp(ij,m[matpos(i,i^1)],-1);
    for (j=0;j<=(i|1);j++,c++) {
      boundXXX_mul_2exp(ik,m[matpos(j^1,j)],-1);
      boundXXX_badd(ik,ij);
      boundXXX_bmin(*c,ik);
    }
  }

  boundXXX_clear(ik); boundXXX_clear(ij);

  /* emptyness checking */
  for (i=0;i<2*dim;i++) {
    c = m+matpos(i,i);
    if (boundXXX_sgn(*c)<0) return true;
    boundXXX_set_int(*c,0);
  }

  return false;
}

/* We use a variant of Floyd-Warshall shortest-path closure algorithm, with
   some extra constraint propagation step.


   Returns true if the resulting matrix is empty, false otherwise
   (does not free the matrix)

   Cubic time. Constant space.
 */

bool hmatXXX_close(boundXXX_t* m, size_t dim)
{
  size_t i,j,k;
  boundXXX_t *c,ik,ik2,ij;

  boundXXX_init(ik); boundXXX_init(ik2); boundXXX_init(ij);

  /* Floyd-Warshall */
  for (k=0;k<2*dim;k++) {
    size_t k2 = k^1;
    c = m;
    for (i=0;i<2*dim;i++) {
      size_t i2 = i|1;
      size_t br = k<i2 ? k : i2;
      boundXXX_set(ik,m[matpos2(i,k)]);
      boundXXX_set(ik2,m[matpos2(i,k2)]);
      for (j=0;j<=br;j++,c++) {
	boundXXX_add(ij,ik,m[matpos(k,j)]);    /* ik+kj */
	boundXXX_bmin(*c,ij);
	boundXXX_add(ij,ik2,m[matpos(k2,j)]);  /* ik2+k2j */
	boundXXX_bmin(*c,ij);
      }
      for (;j<=i2;j++,c++) {
	boundXXX_add(ij,ik,m[matpos(j^1,k2)]); /* ik+kj */
	boundXXX_bmin(*c,ij);
	boundXXX_add(ij,ik2,m[matpos(j^1,k)]); /* ik2+k2j */
	boundXXX_bmin(*c,ij);
      }
    }

  }

  boundXXX_clear(ik); boundXXX_clear(ik2); boundXXX_clear(ij);

  return hmatXXX_s_step(m,dim);
}


/* ============================================================ */
/* Incremental Closure */
/* ============================================================ */

/* m must equal to a closed matrix, except for constraints involving
   variable v

   Quadratic time. Constant space.
*/

bool hmatXXX_close_incremental(boundXXX_t* m, size_t dim, size_t v)
{
  size_t i,j,k;
  boundXXX_t *c;
  boundXXX_t ij,ik,ik2;

  boundXXX_init(ik); boundXXX_init(ik2); boundXXX_init(ij);

  /* incremental Floyd-Warshall : v in end-point position */
  for (k=0;k<2*dim;k++) {
    size_t kk = k^1;
    size_t ii = 2*v+1;
    size_t br  = k<ii ? k : ii;
    for (i=2*v;i<2*v+2;i++) {
      /* v in first end-point position */
      c = m+matpos(i,0);
      boundXXX_set(ik,m[matpos2(i,k)]);
      boundXXX_set(ik2,m[matpos2(i,kk)]);
      for (j=0;j<=br;j++,c++) {
	boundXXX_add(ij,ik,m[matpos(k,j)]);    /* ik+kj */
	boundXXX_bmin(*c,ij);
	boundXXX_add(ij,ik2,m[matpos(kk,j)]);  /* ik2+k2j */
	boundXXX_bmin(*c,ij);
      }
      for (;j<=ii;j++,c++) {
	boundXXX_add(ij,ik,m[matpos(j^1,kk)]); /* ik+kj */
	boundXXX_bmin(*c,ij);
	boundXXX_add(ij,ik2,m[matpos(j^1,k)]); /* ik2+k2j */
	boundXXX_bmin(*c,ij);
      }
      /* v in second end-point position */
      boundXXX_set(ik,m[matpos2(k,i)]);
      boundXXX_set(ik2,m[matpos2(kk,i)]);
      for (j=i;j<k;j++) {
	c = m+matpos(j,i);
	boundXXX_add(ij,ik,m[matpos(kk,j^1)]); /* ik+kj */
	boundXXX_bmin(*c,ij);
	boundXXX_add(ij,ik2,m[matpos(k,j^1)]); /* ik2+k2j */
	boundXXX_bmin(*c,ij);
      }
      for (;j<2*dim;j++) {
	c = m+matpos(j,i);
	boundXXX_add(ij,ik,m[matpos(j,k)]);    /* ik+kj */
	boundXXX_bmin(*c,ij);
	boundXXX_add(ij,ik2,m[matpos(j,kk)]);  /* ik2+k2j */
	boundXXX_bmin(*c,ij);
      }
    }
  }

  /* incremental Floyd-Warshall : v in pivot position */
  for (k=2*v;k<2*v+2;k++) {
    size_t kk = k^1;
    c = m;
    for (i=0;i<2*dim;i++) {
      size_t ii = i|1;
      size_t br = k<ii ? k : ii;
      boundXXX_set(ik,m[matpos2(i,k)]);
      boundXXX_set(ik2,m[matpos2(i,kk)]);
      for (j=0;j<=br;j++,c++) {
	boundXXX_add(ij,ik,m[matpos(k,j)]);    /* ik+kj */
	boundXXX_bmin(*c,ij);
	boundXXX_add(ij,ik2,m[matpos(kk,j)]);  /* ik2+k2j */
	boundXXX_bmin(*c,ij);
      }
      for (;j<=ii;j++,c++) {
	boundXXX_add(ij,ik,m[matpos(j^1,kk)]); /* ik+kj */
	boundXXX_bmin(*c,ij);
	boundXXX_add(ij,ik2,m[matpos(j^1,k)]); /* ik2+k2j */
	boundXXX_bmin(*c,ij);
      }
    }
  }

  boundXXX_clear(ik); boundXXX_clear(ik2); boundXXX_clear(ij);

  return hmatXXX_s_step(m,dim);
}



/* ============================================================ */
/* Sanity Check */
/* ============================================================ */

bool hmatXXX_check_closed(boundXXX_t* m, size_t dim)
{
  bool closed = true;
  size_t i,j,k;
  boundXXX_t w;

  boundXXX_init(w);

  for (i=0;i<2*dim;i++)
    for (j=0;j<2*dim;j++)
      for (k=0;k<2*dim;k++) {
	boundXXX_add(w,m[matpos2(i,k)],m[matpos2(k,j)]);
	if (boundXXX_cmp(m[matpos2(i,j)],w)>0) closed = false;
      }

  for (i=0;i<2*dim;i++)
    for (j=0;j<2*dim;j++)
      if (boundXXX_cmp(m[matpos2(i,j)],m[matpos2(j^1,i^1)])>0) closed = false;

  for (i=0;i<2*dim;i++)
    for (j=0;j<2*dim;j++) {
      boundXXX_add(w,m[matpos2(i,i^1)],m[matpos2(j^1,j)]);
      boundXXX_mul_2exp(w,w,-1);
      if (boundXXX_cmp(m[matpos2(i,j)],w)>0) closed = false;
    }

  boundXXX_clear(w);

  return closed;
}
