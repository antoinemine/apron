/*
 * itv_linear.c
 *
 * Convert the interval coefficient of kth variable into plain one
 *
 * APRON Library / itvpol Domain
 *
 * Copyright (C) Liqian CHEN' 2009
 *
 */
/* This file is part of the APRON Library, released under LGPL license.  
   Please read the COPYING file packaged in the distribution.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fenv.h> /* floating-point lib */
#include <math.h> 

#include "itvp_internal.h"
#include "itvp_config.h"
#include "itv_linear.h"
#include "numitv.h"

/* ====================================================================== */
/* Internal functions */
/* ====================================================================== */
/*  parameter k stores the index(start from 1) of the variable to be linearized. */
/*  return 0: universal constraint; 1: the normalized result changed in itvcons */
int itv_linearisation_kthVar(int dim, numdbl_t *itvcons, numdbl_t *bnds, int k)
{
  numdbl_t a_c;
  numdbl_t a_l,a_u;
  numitv_t *xk_bnd,*akc_itv;
  numdbl_t b; /* constant */
  numitv_t b_change;

  a_l=*(itvcons+1+2*(k-1));
  a_u=*(itvcons+1+2*(k-1)+1);
  xk_bnd=(numitv_t *)(bnds+2*(k-1));

  a_c=(a_l+a_u)/2;
  b=*itvcons;
  akc_itv=(numitv_t *)malloc(sizeof(numitv_t));
  akc_itv->inf=num_sub_downward(a_l, a_c);
  akc_itv->sup=a_u-a_c;
  b_change=numitv_mul(*akc_itv,*xk_bnd); 
  if(isfinite(b_change.inf)){
     *(itvcons+1+2*(k-1))=a_c;
     *(itvcons+1+2*(k-1)+1)=a_c;	
     *itvcons=b-b_change.inf;
     return 1;
  }
  else{
     return 0;
  }
}

/*  parameter k stores the index(start from 1) of the variable to be linearized. */
/*  return 0: universal constraint; 1: the normalized result changed in itvcons */
int itv_linearisation_kthVar_toZero(int dim, numdbl_t *itvcons, numdbl_t *bnds, int k)
{
  numdbl_t a_c;
  numdbl_t a_l,a_u;
  numitv_t *xk_bnd,*akc_itv;
  numdbl_t b; /* constant */
  numitv_t b_change;

  a_l=*(itvcons+1+2*(k-1));
  a_u=*(itvcons+1+2*(k-1)+1);
  xk_bnd=(numitv_t *)(bnds+2*(k-1));

  a_c=0;/*(a_l+a_u)/2;*/
  b=*itvcons;
  akc_itv=(numitv_t *)malloc(sizeof(numitv_t));
  akc_itv->inf=num_sub_downward(a_l, a_c);
  akc_itv->sup=a_u-a_c;
  b_change=numitv_mul(*akc_itv,*xk_bnd); 
  if(isfinite(b_change.inf)){
     *(itvcons+1+2*(k-1))=a_c;
     *(itvcons+1+2*(k-1)+1)=a_c;	
     *itvcons=b-b_change.inf;
     return 1;
  }
  else{
     return 0;
  }
}
