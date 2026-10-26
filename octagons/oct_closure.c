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

/* Tightening step to get complete closure on Z. 
    cf. R. Bagnara, P. M. Hill, E. Zaffanella, "An Improved Tight Closure
   Algorithm for Integer Octagonal Constraints"
   
   A unary constraint x <= c is encoded as x+ - x- = 2x <= w with w = 2c,
   stored in m[2k+1][2k] (and -x <= c in m[2k][2k+1]).
   When x is an integer, 2x is even, so 2x <= w is equivalent on Z to
   2x <= 2*floor(w/2), the largest even integer not greater than w.
   Only the first intdim (integer) variables are tightened.
  
   Then hmat_s_step performs the strengthening, which also detects
   Z-inconsistency (Theorem 3).
*/
bool hmat_tightening(bound_t* m, size_t dim, size_t intdim)
{
  size_t i;
  bound_t *c, t;

  bound_init(t);
  
  for (i=0;i<2*intdim;i++) {
    c = m+matpos(i,i^1);
    bound_fdiv_2(t,*c);
    bound_mul_2(*c,t);      /* w[i,i^1] := 2*floor(w[i,i^1]/2) */
  }
  
  bound_clear(t);

  return hmat_s_step(m,dim);
}


/* Rounds down all bounds between integer variables, which is sound on Z.
   This yields an integer octagonal graph, as required
   by the tight closure.

   Quadratic time. Constant space.
*/
void hmat_floor_int(bound_t* m, size_t intdim)
{
  size_t i,j;
  
  for (i=0;i<2*intdim;i++)
    for (j=0;j<=(i|1);j++)
      bound_floor(m[matpos(i,j)],m[matpos(i,j)]);
}

/* We use a variant of Floyd-Warshall shortest-path closure algorithm, with
   some extra constraint propagation step.

   When intdim > 0 (tight closure enabled), bounds between integer
   variables are first rounded down (hmat_floor_int), then Floyd-Warshall
   is followed by the tightening of unary constraints and a single
   strengthening step (hmat_tightening).
   The result is the tight closure when all variables are integer and the
   arithmetic is exact (e.g., MPQ, MPZ). With mixed integer/real variables,
   or inexact arithmetic, it is sound but may not be the most precise.

   Returns true if the resulting matrix is empty, false otherwise
   (does not free the matrix)

   Cubic time. Constant space.
*/
bool hmat_close(bound_t* m, size_t dim, size_t intdim)
{
  size_t i,j,k;
  bound_t *c,ik,ik2,ij;

  bound_init(ik); bound_init(ik2); bound_init(ij);
  hmat_floor_int(m,intdim);
  
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

  return hmat_tightening(m,dim,intdim);
}


/* ============================================================ */
/* Incremental Closure */
/* ============================================================ */

/* m must equal to a closed matrix, except for constraints involving
   variable v

   Quadratic time. Constant space.
*/

bool hmat_close_incremental(bound_t* m, size_t dim, size_t intdim, size_t v)
{
  size_t i,j,k;
  bound_t *c;
  bound_t ij,ik,ik2;

  bound_init(ik); bound_init(ik2); bound_init(ij);
  hmat_floor_int(m,intdim);

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

  return hmat_tightening(m,dim,intdim);
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

/* checks that bounds between integer variables are integer,
   and unary bounds on integer variables are even */
bool hmat_check_tight(bound_t* m, size_t intdim)
{
  bool tight = true;
  size_t i,j;
  bound_t t;
  bound_init(t);
  for (i=0;i<2*intdim;i++)
    for (j=0;j<2*intdim;j++) {
      bound_floor(t,m[matpos2(i,j)]);
      if (bound_cmp(t,m[matpos2(i,j)])) tight = false;
    }
  for (i=0;i<2*intdim;i++) {
    bound_fdiv_2(t,m[matpos2(i,i^1)]);
    bound_mul_2(t,t);
    if (bound_cmp(t,m[matpos2(i,i^1)])) tight = false;
  }
  bound_clear(t);
  return tight;
}

