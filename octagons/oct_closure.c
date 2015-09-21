/*
 * oct_closure.c
 *
 * Half-matrices - Closure algorithms
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


/* All closures are in-place. */


/* ============================================================ */
/* Full Closure */
/* ============================================================ */


/* unary constraint propagation */
bool hmat_s_step(bound_t* m, size_t dim)
{
  size_t i,j,k;
  bound_t *c,ik,ij;

  bound_init(ik); bound_init(ij);

  /* lone S step */
  c = m;
  for (i=0;i<2*dim;i++) {
    bound_div_2(ij,m[matpos(i,i^1)]);
    for (j=0;j<=(i|1);j++,c++) {
      bound_div_2(ik,m[matpos(j^1,j)]);
      bound_badd(ik,ij);
      bound_bmin(*c,ik);
    }
  }

  bound_clear(ik); bound_clear(ij);

  /* emptyness checking */
  for (i=0;i<2*dim;i++) {
    c = m+matpos(i,i);
    if (bound_sgn(*c)<0) return true;
    bound_set_int(*c,0);
  }

  return false;
}

/* We use a variant of Floyd-Warshall shortest-path closure algorithm, with
   some extra constraint propagation step.
   

   Returns true if the resulting matrix is empty, false otherwise
   (does not free the matrix)

   Cubic time. Constant space.
 */

bool hmat_close(bound_t* m, size_t dim)
{
  size_t i,j,k;
  bound_t *c,ik,ik2,ij;

  bound_init(ik); bound_init(ik2); bound_init(ij);

  /* Floyd-Warshall */
  for (k=0;k<2*dim;k++) {
    size_t k2 = k^1;
    c = m;
    for (i=0;i<2*dim;i++) {
      size_t i2 = i|1;
      size_t br = k<i2 ? k : i2;
      bound_set(ik,m[matpos2(i,k)]);
      bound_set(ik2,m[matpos2(i,k2)]);
      for (j=0;j<=br;j++,c++) {
	bound_add(ij,ik,m[matpos(k,j)]);    /* ik+kj */
	bound_bmin(*c,ij);
	bound_add(ij,ik2,m[matpos(k2,j)]);  /* ik2+k2j */
	bound_bmin(*c,ij);
      }
      for (;j<=i2;j++,c++) {
	bound_add(ij,ik,m[matpos(j^1,k2)]); /* ik+kj */
	bound_bmin(*c,ij);
	bound_add(ij,ik2,m[matpos(j^1,k)]); /* ik2+k2j */
	bound_bmin(*c,ij);
      }
    }
 
  }
  
  bound_clear(ik); bound_clear(ik2); bound_clear(ij);

  return hmat_s_step(m,dim);
}


/* ============================================================ */
/* Incremental Closure */
/* ============================================================ */

/* m must equal to a closed matrix, except for constraints involving
   variable v

   Quadratic time. Constant space.
*/

bool hmat_close_incremental(bound_t* m, size_t dim, size_t v)
{
  size_t i,j,k;
  bound_t *c;
  bound_t ij,ik,ik2;

  bound_init(ik); bound_init(ik2); bound_init(ij);

  /* incremental Floyd-Warshall : v in end-point position */
  for (k=0;k<2*dim;k++) {
    size_t kk = k^1;
    size_t ii = 2*v+1;
    size_t br  = k<ii ? k : ii;
    for (i=2*v;i<2*v+2;i++) {
      /* v in first end-point position */
      c = m+matpos(i,0);
      bound_set(ik,m[matpos2(i,k)]);
      bound_set(ik2,m[matpos2(i,kk)]);
      for (j=0;j<=br;j++,c++) {
	bound_add(ij,ik,m[matpos(k,j)]);    /* ik+kj */
	bound_bmin(*c,ij);
	bound_add(ij,ik2,m[matpos(kk,j)]);  /* ik2+k2j */
	bound_bmin(*c,ij);
      }
      for (;j<=ii;j++,c++) {
	bound_add(ij,ik,m[matpos(j^1,kk)]); /* ik+kj */
	bound_bmin(*c,ij);
	bound_add(ij,ik2,m[matpos(j^1,k)]); /* ik2+k2j */
	bound_bmin(*c,ij);
      }
      /* v in second end-point position */   
      bound_set(ik,m[matpos2(k,i)]);
      bound_set(ik2,m[matpos2(kk,i)]);
      for (j=i;j<k;j++) {
	c = m+matpos(j,i);
	bound_add(ij,ik,m[matpos(kk,j^1)]); /* ik+kj */
	bound_bmin(*c,ij);
	bound_add(ij,ik2,m[matpos(k,j^1)]); /* ik2+k2j */
	bound_bmin(*c,ij);
      }
      for (;j<2*dim;j++) {
	c = m+matpos(j,i);
	bound_add(ij,ik,m[matpos(j,k)]);    /* ik+kj */
	bound_bmin(*c,ij);
	bound_add(ij,ik2,m[matpos(j,kk)]);  /* ik2+k2j */
	bound_bmin(*c,ij);
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
      bound_set(ik,m[matpos2(i,k)]);
      bound_set(ik2,m[matpos2(i,kk)]);
      for (j=0;j<=br;j++,c++) {
	bound_add(ij,ik,m[matpos(k,j)]);    /* ik+kj */
	bound_bmin(*c,ij);
	bound_add(ij,ik2,m[matpos(kk,j)]);  /* ik2+k2j */
	bound_bmin(*c,ij);
      }
      for (;j<=ii;j++,c++) {
	bound_add(ij,ik,m[matpos(j^1,kk)]); /* ik+kj */
	bound_bmin(*c,ij);
	bound_add(ij,ik2,m[matpos(j^1,k)]); /* ik2+k2j */
	bound_bmin(*c,ij);
      }
    } 
  }
  
  bound_clear(ik); bound_clear(ik2); bound_clear(ij);

  return hmat_s_step(m,dim);
}



/* ============================================================ */
/* Sanity Check */
/* ============================================================ */

bool hmat_check_closed(bound_t* m, size_t dim)
{
  bool closed = true;
  size_t i,j,k;
  bound_t w;

  bound_init(w);

  for (i=0;i<2*dim;i++)
    for (j=0;j<2*dim;j++)
      for (k=0;k<2*dim;k++) {
	bound_add(w,m[matpos2(i,k)],m[matpos2(k,j)]);
	if (bound_cmp(m[matpos2(i,j)],w)>0) closed = false;
      }

  for (i=0;i<2*dim;i++)
    for (j=0;j<2*dim;j++)
      if (bound_cmp(m[matpos2(i,j)],m[matpos2(j^1,i^1)])>0) closed = false;
    
  for (i=0;i<2*dim;i++)
    for (j=0;j<2*dim;j++) {
      bound_add(w,m[matpos2(i,i^1)],m[matpos2(j^1,j)]);
      bound_div_2(w,w);
      if (bound_cmp(m[matpos2(i,j)],w)>0) closed = false;
    }

  bound_clear(w);

  return closed;
}

