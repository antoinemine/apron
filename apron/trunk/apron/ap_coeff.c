/* ************************************************************************* */
/* ap_coeff.c: coefficients, that are either scalars or intervals */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <assert.h>

#include "ap_coeff.h"

/* ====================================================================== */
/* Basics */
/* ====================================================================== */

/* FOR INTERNAL USE ONLY */
void ap_coeff_init(ap_coeff_t* coeff, ap_coeff_discr_t coeff_discr)
{
  coeff->discr = coeff_discr;
  switch (coeff_discr){
  case AP_COEFF_SCALAR:
    coeff->val.scalar = ap_scalar_alloc();
    break;
  case AP_COEFF_INTERVAL:
    coeff->val.interval = ap_interval_alloc();
    break;
  }
}
/* FOR INTERNAL USE ONLY */
void ap_coeff_init_set(ap_coeff_t* coeff, ap_coeff_t* coeff2)
{
  coeff->discr = coeff2->discr;
  switch (coeff2->discr){
  case AP_COEFF_SCALAR:
    coeff->val.scalar = ap_scalar_alloc_set(coeff2->val.scalar);
    break;
  case AP_COEFF_INTERVAL:
    coeff->val.interval = ap_interval_alloc_set(coeff2->val.interval);
    break;
  }
}
/* FOR INTERNAL USE ONLY */
void ap_coeff_clear(ap_coeff_t* coeff)
{
  switch (coeff->discr){
  case AP_COEFF_SCALAR:
    ap_scalar_free(coeff->val.scalar);
    break;
  case AP_COEFF_INTERVAL:
    ap_interval_free(coeff->val.interval);
    break;
  }
}

ap_coeff_t* ap_coeff_alloc(ap_coeff_discr_t coeff_discr)
{
  ap_coeff_t* coeff = malloc(sizeof(ap_coeff_t));
  coeff->discr = coeff_discr;
  switch (coeff_discr){
  case AP_COEFF_SCALAR:
    coeff->val.scalar = ap_scalar_alloc();
    break;
  case AP_COEFF_INTERVAL:
    coeff->val.interval = ap_interval_alloc();
    break;
  }
  return coeff;
}
void ap_coeff_reinit(ap_coeff_t* coeff, ap_coeff_discr_t coeff_discr, ap_scalar_discr_t ap_scalar_discr)
{
  switch (coeff->discr){
  case AP_COEFF_SCALAR:
    if (coeff_discr != coeff->discr){
      ap_scalar_free(coeff->val.scalar);
      coeff->val.interval = ap_interval_alloc();
      ap_interval_reinit(coeff->val.interval,ap_scalar_discr);
    }
    else {
      ap_scalar_reinit(coeff->val.scalar,ap_scalar_discr);
    }
    break;
  case AP_COEFF_INTERVAL:
    if (coeff_discr != coeff->discr){
      ap_interval_free(coeff->val.interval);
      coeff->val.scalar = ap_scalar_alloc();
      ap_scalar_reinit(coeff->val.scalar,ap_scalar_discr);
    }
    else {
      ap_interval_reinit(coeff->val.interval,ap_scalar_discr);
    }
    break;
  }
  coeff->discr = coeff_discr;
}

void ap_coeff_free(ap_coeff_t* coeff)
{
  switch (coeff->discr){
  case AP_COEFF_SCALAR:
    ap_scalar_free(coeff->val.scalar);
    break;
  case AP_COEFF_INTERVAL:
    ap_interval_free(coeff->val.interval);
    break;
  }
  free(coeff);
}

void ap_coeff_fprint(FILE* stream, ap_coeff_t* a)
{
  switch(a->discr){
  case AP_COEFF_SCALAR:
    ap_scalar_fprint(stream,a->val.scalar);
    break;
  case AP_COEFF_INTERVAL:
    ap_interval_fprint(stream,a->val.interval);
    break;
  }
}

void ap_coeff_reduce(ap_coeff_t* coeff)
{
  if (coeff->discr==AP_COEFF_INTERVAL){
    if (ap_scalar_equal(coeff->val.interval->inf,coeff->val.interval->sup)){
      /* We cheat with the good rules */
      ap_scalar_t* scalar = coeff->val.interval->inf;
      ap_scalar_free(coeff->val.interval->sup);
      free(coeff->val.interval);
      coeff->val.scalar = scalar;
      coeff->discr=AP_COEFF_SCALAR;
    }
  }
}

/* ====================================================================== */
/* Combined allocation and assignment */
/* ====================================================================== */
ap_coeff_t* ap_coeff_alloc_set_scalar(ap_scalar_t* scalar)
{
  ap_coeff_t* coeff = malloc(sizeof(ap_coeff_t));
  coeff->discr = AP_COEFF_SCALAR;
  coeff->val.scalar = ap_scalar_alloc_set(scalar);
  return coeff;
}
ap_coeff_t* ap_coeff_alloc_set_interval(ap_interval_t* interval)
{
  ap_coeff_t* coeff = malloc(sizeof(ap_coeff_t));
  coeff->discr = AP_COEFF_INTERVAL;
  coeff->val.interval = ap_interval_alloc_set(interval);
  return coeff;
}
ap_coeff_t* ap_coeff_alloc_set(ap_coeff_t* coeff)
{
  switch (coeff->discr){
  case AP_COEFF_SCALAR:
    return ap_coeff_alloc_set_scalar(coeff->val.scalar);
  case AP_COEFF_INTERVAL:
    return ap_coeff_alloc_set_interval(coeff->val.interval);
    break;
  default:
    return NULL;
  }
}

/* ====================================================================== */
/* Assignments */
/* ====================================================================== */

void ap_coeff_set(ap_coeff_t* a, ap_coeff_t* b)
{

  if(a->discr != b->discr){
    ap_coeff_reinit(a,b->discr,AP_SCALAR_DOUBLE);
  }
  switch(b->discr){
  case AP_COEFF_SCALAR:
    ap_scalar_set(a->val.scalar,b->val.scalar);
    break;
  case AP_COEFF_INTERVAL:
    ap_interval_set(a->val.interval,b->val.interval);
    break;
  }
}
void ap_coeff_set_scalar(ap_coeff_t* coeff, ap_scalar_t* scalar)
{
  ap_coeff_reinit(coeff,AP_COEFF_SCALAR,scalar->discr); 
  ap_scalar_set(coeff->val.scalar,scalar); 
}
void ap_coeff_set_scalar_mpq(ap_coeff_t* coeff, mpq_t mpq)
{
  ap_coeff_reinit(coeff,AP_COEFF_SCALAR,AP_SCALAR_MPQ); 
  ap_scalar_set_mpq(coeff->val.scalar,mpq); 
}
void ap_coeff_set_scalar_mpfr(ap_coeff_t* coeff, mpfr_t mpfr)
{
  ap_coeff_reinit(coeff,AP_COEFF_SCALAR,AP_SCALAR_MPFR); 
  ap_scalar_set_mpfr(coeff->val.scalar,mpfr); 
}
void ap_coeff_set_scalar_int(ap_coeff_t* coeff, long int num)
{
  ap_coeff_reinit(coeff,AP_COEFF_SCALAR,AP_SCALAR_MPQ); 
  ap_scalar_set_int(coeff->val.scalar,num); 
}
void ap_coeff_set_scalar_frac(ap_coeff_t* coeff, long int num, unsigned long int den)
{
  ap_coeff_reinit(coeff,AP_COEFF_SCALAR,AP_SCALAR_MPQ); 
  ap_scalar_set_frac(coeff->val.scalar,num,den); 
}
void ap_coeff_set_scalar_double(ap_coeff_t* coeff, double num)
{ 
  ap_coeff_reinit(coeff,AP_COEFF_SCALAR,AP_SCALAR_DOUBLE); 
  ap_scalar_set_double(coeff->val.scalar,num); 
}
void ap_coeff_set_interval(ap_coeff_t* coeff, ap_interval_t* itv)
{
  ap_coeff_reinit(coeff,AP_COEFF_INTERVAL,AP_SCALAR_DOUBLE); 
  ap_interval_set(coeff->val.interval,itv); 
}
void ap_coeff_set_interval_scalar(ap_coeff_t* coeff, ap_scalar_t* inf, ap_scalar_t* sup)
{
  ap_coeff_reinit(coeff,AP_COEFF_INTERVAL,AP_SCALAR_DOUBLE);
  ap_interval_set_scalar(coeff->val.interval,inf,sup);
}
void ap_coeff_set_interval_mpq(ap_coeff_t* coeff, mpq_t inf, mpq_t sup)
{
  ap_coeff_reinit(coeff,AP_COEFF_INTERVAL,AP_SCALAR_MPQ); 
  ap_interval_set_mpq(coeff->val.interval,inf,sup);
}
void ap_coeff_set_interval_mpfr(ap_coeff_t* coeff, mpfr_t inf, mpfr_t sup)
{
  ap_coeff_reinit(coeff,AP_COEFF_INTERVAL,AP_SCALAR_MPFR); 
  ap_interval_set_mpfr(coeff->val.interval,inf,sup);
}
void ap_coeff_set_interval_int(ap_coeff_t* coeff, long int inf, long int sup)
{
  ap_coeff_reinit(coeff,AP_COEFF_INTERVAL,AP_SCALAR_MPQ); 
  ap_interval_set_int(coeff->val.interval,inf,sup); 
}
void ap_coeff_set_interval_frac(ap_coeff_t* coeff,
				long int numinf, unsigned long int deninf, 
				long int numsup, unsigned long int densup)
{
  ap_coeff_reinit(coeff,AP_COEFF_INTERVAL,AP_SCALAR_MPQ); 
  ap_interval_set_frac(coeff->val.interval,numinf,deninf,numsup,densup); 
}
void ap_coeff_set_interval_double(ap_coeff_t* coeff, double inf, double sup)
{
  ap_coeff_reinit(coeff,AP_COEFF_INTERVAL,AP_SCALAR_DOUBLE); 
  ap_interval_set_double(coeff->val.interval,inf,sup); 
}
void ap_coeff_set_interval_top(ap_coeff_t* coeff)
{
  if (coeff->discr == AP_COEFF_SCALAR)
    ap_coeff_reinit(coeff,AP_COEFF_INTERVAL,AP_SCALAR_DOUBLE);
  ap_interval_set_top(coeff->val.interval);
}

/* ====================================================================== */
/* Tests */
/* ====================================================================== */

int ap_coeff_cmp(ap_coeff_t* coeff1, ap_coeff_t* coeff2)
{
  if (coeff1->discr==coeff2->discr){
    switch (coeff1->discr){
    case AP_COEFF_SCALAR:
      return ap_scalar_cmp(coeff1->val.scalar,coeff2->val.scalar);
    case AP_COEFF_INTERVAL:
      return ap_interval_cmp(coeff1->val.interval,coeff2->val.interval);
    default:
      abort();
      return 0;
    }
  } 
  else {
    return (coeff1->discr==AP_COEFF_SCALAR) ? -3 : 3;
  }
}
bool ap_coeff_equal(ap_coeff_t* coeff1, ap_coeff_t* coeff2)
{
  if (coeff1->discr==coeff2->discr){
    switch (coeff1->discr){
    case AP_COEFF_SCALAR:
      return ap_scalar_equal(coeff1->val.scalar,coeff2->val.scalar);
    case AP_COEFF_INTERVAL:
      return ap_interval_equal(coeff1->val.interval,coeff2->val.interval);
    default:
      abort();
    }
  }
  else
    return false;
}
bool ap_coeff_zero(ap_coeff_t* coeff)
{
  switch (coeff->discr){
  case AP_COEFF_SCALAR:
    return ap_scalar_sgn(coeff->val.scalar)==0;
  case AP_COEFF_INTERVAL:
    return (ap_scalar_sgn(coeff->val.interval->inf)==0) && (ap_scalar_sgn(coeff->val.interval->sup)==0);
  default:
    abort();
  }
}
bool ap_coeff_equal_int(ap_coeff_t* coeff, int i)
{
  switch (coeff->discr){
  case AP_COEFF_SCALAR:
    return ap_scalar_equal_int(coeff->val.scalar,i)==0;
  case AP_COEFF_INTERVAL:
    return (ap_scalar_equal_int(coeff->val.interval->inf,i)==0) && (ap_scalar_equal_int(coeff->val.interval->sup,i)==0);
  default:
    abort();
  }
}

/* ====================================================================== */
/* Other operations */
/* ====================================================================== */

void ap_coeff_neg(ap_coeff_t* a, ap_coeff_t* b)
{
  ap_coeff_set(a,b);
  switch(b->discr){
  case AP_COEFF_SCALAR:
    ap_scalar_neg(a->val.scalar,b->val.scalar);
    break;
  case AP_COEFF_INTERVAL:
    ap_interval_neg(a->val.interval,b->val.interval);
    break;
  default:
    abort();
  }
}

/* Hash */

long ap_coeff_hash(ap_coeff_t* coeff)
{
  switch (coeff->discr){
  case AP_COEFF_SCALAR:
    return ap_scalar_hash(coeff->val.scalar);
  case AP_COEFF_INTERVAL:
    return ap_interval_hash(coeff->val.interval);
  default:
    abort();
    return 0;
  }
}
