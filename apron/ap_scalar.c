/* ************************************************************************* */
/* ap_scalar.c: coefficients and intervals */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <assert.h>

#include "ap_scalar.h"

/* ====================================================================== */
/* Basics */
/* ====================================================================== */

ap_scalar_t* ap_scalar_alloc()
{
  ap_scalar_t* scalar = malloc(sizeof(ap_scalar_t));
  scalar->discr = AP_SCALAR_DOUBLE;
  scalar->val.dbl = 0.0;
  return scalar;
}

void ap_scalar_reinit(ap_scalar_t* scalar, ap_scalar_discr_t d)
{ 
  if (scalar->discr != d){
    ap_scalar_clear(scalar);
    ap_scalar_init(scalar,d);
  }
}

void ap_scalar_free(ap_scalar_t* scalar)
{
  ap_scalar_clear(scalar);
  free(scalar);
}

int ap_scalar_print_prec = 20;

void ap_scalar_fprint(FILE* stream, ap_scalar_t* a)
{
  int flag;

  flag = ap_scalar_infty(a);
  if (flag){
    fprintf(stream, flag>0 ? "+oo" : "-oo");
  }
  else {
    switch(a->discr){
    case AP_SCALAR_DOUBLE:
      fprintf(stream,"%.*g",ap_scalar_print_prec,a->val.dbl + 0.0);
      break;
    case AP_SCALAR_MPQ:
      mpq_out_str(stream,10,a->val.mpq);
      break;
    case AP_SCALAR_MPFR:
      {
	double d = mpfr_get_d(a->val.mpfr,GMP_RNDU);
	if (mpfr_cmp_d(a->val.mpfr,d)) 
	  mpfr_out_str(stream,10,ap_scalar_print_prec,a->val.mpfr,GMP_RNDU);
	else 
	  fprintf(stream,"%.*g",ap_scalar_print_prec,d + 0.0);
      }
      break;
    default: 
      abort();
    }
  } 
}

/* ====================================================================== */
/* Combined allocation and assignments */
/* ====================================================================== */

ap_scalar_t* ap_scalar_alloc_set(ap_scalar_t* b)
{
  ap_scalar_t* a = malloc(sizeof(ap_scalar_t));
  a->discr = b->discr;
  switch(b->discr){
  case AP_SCALAR_DOUBLE:
    a->val.dbl = b->val.dbl;
    break;
  case AP_SCALAR_MPQ:
    a->val.mpq = malloc(sizeof(mpq_t)); 
    mpq_init(a->val.mpq);
    mpq_set(a->val.mpq,b->val.mpq);
    break;
  case AP_SCALAR_MPFR:
    a->val.mpfr = malloc(sizeof(mpfr_t)); 
    mpfr_init2(a->val.mpfr,mpfr_get_prec(b->val.mpfr));
    mpfr_set(a->val.mpfr,b->val.mpfr,GMP_RNDU /* exact */);
    break;
  default: 
    abort();
  }
  return a;
}
ap_scalar_t* ap_scalar_alloc_set_mpq(mpq_t mpq)
{
  ap_scalar_t* a = malloc(sizeof(ap_scalar_t));
  a->discr = AP_SCALAR_MPQ;
  a->val.mpq = malloc(sizeof(mpq_t)); 
  mpq_init(a->val.mpq);
  mpq_set(a->val.mpq,mpq);
  return a;
}
ap_scalar_t* ap_scalar_alloc_set_mpfr(mpfr_t mpfr)
{
  ap_scalar_t* a = malloc(sizeof(ap_scalar_t));
  a->discr = AP_SCALAR_MPFR;
  a->val.mpfr = malloc(sizeof(mpfr_t)); 
  mpfr_init2(a->val.mpfr,mpfr_get_prec(mpfr));
  mpfr_set(a->val.mpfr,mpfr,GMP_RNDU /* exact */);
  return a;
}
ap_scalar_t* ap_scalar_alloc_set_double(double k)
{
  ap_scalar_t* a = malloc(sizeof(ap_scalar_t));
  a->discr = AP_SCALAR_DOUBLE;
  a->val.dbl = k;
  return a;
}

/* ====================================================================== */
/* Assignments */
/* ====================================================================== */

void ap_scalar_set(ap_scalar_t* a, ap_scalar_t* b)
{
  if (a==b) return;
  ap_scalar_reinit(a,b->discr);
  switch(b->discr){
  case AP_SCALAR_DOUBLE:
    a->val.dbl = b->val.dbl;
    break;
  case AP_SCALAR_MPQ:
    mpq_set(a->val.mpq,b->val.mpq);
    break;
  case AP_SCALAR_MPFR:
    mpfr_set_prec(a->val.mpfr,mpfr_get_prec(b->val.mpfr));
    mpfr_set(a->val.mpfr,b->val.mpfr,GMP_RNDU /* exact */);
    break;
  default: 
    abort();
  }
}
void ap_scalar_set_mpq(ap_scalar_t* scalar, mpq_t mpq)
  { ap_scalar_reinit(scalar,AP_SCALAR_MPQ); mpq_set(scalar->val.mpq,mpq); }
void ap_scalar_set_mpfr(ap_scalar_t* scalar, mpfr_t mpfr)
{ 
  ap_scalar_reinit(scalar,AP_SCALAR_MPFR);
  mpfr_set_prec(scalar->val.mpfr,mpfr_get_prec(mpfr));
  mpfr_set(scalar->val.mpfr,mpfr,GMP_RNDU /* exact */);
}
void ap_scalar_set_int(ap_scalar_t* scalar, long int i)
  { ap_scalar_reinit(scalar,AP_SCALAR_MPQ); mpq_set_si(scalar->val.mpq,i,1); }
void ap_scalar_set_frac(ap_scalar_t* scalar, long int i, unsigned long int j)
{ ap_scalar_reinit(scalar,AP_SCALAR_MPQ); mpq_set_si(scalar->val.mpq,i,j); 
  mpq_canonicalize(scalar->val.mpq); }
void ap_scalar_set_double(ap_scalar_t* scalar, double k)
  { ap_scalar_reinit(scalar,AP_SCALAR_DOUBLE); scalar->val.dbl = k; }
void ap_scalar_set_infty(ap_scalar_t* scalar, int sgn)
{
  switch(scalar->discr){
  case AP_SCALAR_MPQ:
    mpz_set_si(mpq_numref(scalar->val.mpq),sgn);
    mpz_set_ui(mpq_denref(scalar->val.mpq),sgn ? 0 : 1);
    break;
  case AP_SCALAR_DOUBLE:
    scalar->val.dbl = sgn>0 ? (double)1.0/(double)0.0 : (sgn < 0 ? -(double)1.0/(double)0.0 : 0.0);
    break;
  case AP_SCALAR_MPFR:
    mpfr_set_inf(scalar->val.mpfr,sgn);
    break;
  default:
    abort();
  }
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

int ap_mpq_set_scalar(mpq_t mpq, ap_scalar_t* scalar, mp_rnd_t round)
{
  mp_exp_t e;
  int inf = ap_scalar_infty(scalar);
  (void)round;
  if (inf) {
    mpz_set_si(mpq_numref(mpq),inf);
    mpz_set_si(mpq_denref(mpq),0);
    return 0;
  }
  switch(scalar->discr){
  case AP_SCALAR_MPQ:
    mpq_set(mpq,scalar->val.mpq);
    return 0;
    break;
  case AP_SCALAR_DOUBLE:
    if (!isfinite(scalar->val.dbl)) { mpq_set_si(mpq,0,1); return 1; }
    mpq_set_d(mpq,scalar->val.dbl);
    return 0;
    break;
  case AP_SCALAR_MPFR:
    if (!mpfr_number_p(scalar->val.mpfr)) { mpq_set_si(mpq,0,1); return 1; }
    e = mpfr_get_z_exp(mpq_numref(mpq),scalar->val.mpfr);
    mpz_set_si(mpq_denref(mpq),1);
    if (e>0) mpq_mul_2exp(mpq,mpq,e);
    if (e<0) mpq_div_2exp(mpq,mpq,-e);
    return 0;
  default:
    abort();
  }
}

int ap_double_set_scalar(double* k, ap_scalar_t* scalar, mp_rnd_t round)
{
  switch(scalar->discr){
  case AP_SCALAR_MPQ:
    {
      mpfr_t mpfr;
      mpfr_init2(mpfr,53);
      int res = mpfr_set_q(mpfr,scalar->val.mpq,round);
      *k = mpfr_get_d(mpfr,round); /* normally, exact */
      mpfr_clear(mpfr);
      return res;
    }
  case AP_SCALAR_DOUBLE:
    *k = scalar->val.dbl;
    return 0;
  case AP_SCALAR_MPFR:
    *k = mpfr_get_d(scalar->val.mpfr,round);
    return mpfr_cmp_d(scalar->val.mpfr,*k);
  default:
    abort();
  }
}

int ap_mpfr_set_scalar(mpfr_t mpfr, ap_scalar_t* scalar, mp_rnd_t round)
{
  switch(scalar->discr){
  case AP_SCALAR_MPQ:
    return mpfr_set_q(mpfr,scalar->val.mpq,round);
  case AP_SCALAR_DOUBLE:
    return mpfr_set_d(mpfr,scalar->val.dbl,round);
  case AP_SCALAR_MPFR:
    return mpfr_set(mpfr,scalar->val.mpfr,round);
  default:
    abort();
  }
}

/* ====================================================================== */
/* Tests */
/* ====================================================================== */

int ap_scalar_infty(ap_scalar_t* scalar)
{
  switch(scalar->discr){
  case AP_SCALAR_MPQ:
    return 
      (mpz_sgn(mpq_denref(scalar->val.mpq)))==0 ?
      (mpz_sgn(mpq_numref(scalar->val.mpq))) > 0 ? 1 : -1 : 0;
  case AP_SCALAR_DOUBLE:
    return 
      scalar->val.dbl==(double)1.0/(double)0.0 ?
      1 :
      ( scalar->val.dbl==-(double)1.0/(double)0.0 ?
	-1 :
	0);
  case AP_SCALAR_MPFR:
    return 
      mpfr_inf_p(scalar->val.mpfr) ?
      mpfr_sgn(scalar->val.mpfr) > 0 ? 1 : -1 : 0;
  default:
    abort();
  }
}

int ap_scalar_cmp(ap_scalar_t* a, ap_scalar_t* b)
{
  int s1 = ap_scalar_infty(a);
  int s2 = ap_scalar_infty(b);
  
  if (s1>s2)
    return 1;
  else if (s1<s2)
    return -1;
  else if (s1!=0)
    return 0;
  else if (a->discr == b->discr) {
    switch (a->discr){
    case AP_SCALAR_MPQ:
      return mpq_cmp(a->val.mpq,b->val.mpq);
    case AP_SCALAR_DOUBLE:
      return 
	a->val.dbl > b->val.dbl ?
	1 :
      ( a->val.dbl < b->val.dbl ?
	-1 :
	0);
    case AP_SCALAR_MPFR:
      return mpfr_cmp(a->val.mpfr,b->val.mpfr);
    default:
      abort();
    }
  }
  else if (a->discr==AP_SCALAR_MPFR || b->discr==AP_SCALAR_MPFR) {
    switch (a->discr) {
    case AP_SCALAR_MPFR:
      switch (b->discr) {
      case AP_SCALAR_MPQ:    return mpfr_cmp_q(a->val.mpfr,b->val.mpq);
      case AP_SCALAR_DOUBLE: return mpfr_cmp_d(a->val.mpfr,b->val.dbl);
      default:               abort();
      }
      break;
    case AP_SCALAR_MPQ:    return -mpfr_cmp_q(b->val.mpfr,a->val.mpq);
    case AP_SCALAR_DOUBLE: return -mpfr_cmp_d(b->val.mpfr,a->val.dbl);
    default:               abort();
    }
  }
  else {
    mpq_t mpq;
    int res;
    
    mpq_init(mpq);
    if (a->discr==AP_SCALAR_DOUBLE){
      mpq_set_d(mpq,a->val.dbl);
      res = mpq_cmp(mpq,b->val.mpq);
    }
    else {
      mpq_set_d(mpq,b->val.dbl);
      res = mpq_cmp(a->val.mpq,mpq);
    }
    mpq_clear(mpq);
    return res;
  }
}
int ap_scalar_cmp_int(ap_scalar_t* a, int b)
{
  int s1 = ap_scalar_infty(a);
  
  if (s1>0)
    return 1;
  else if (s1<0)
    return -1;
  else {
    switch (a->discr){
    case AP_SCALAR_DOUBLE:
      return a->val.dbl > b ? 1 : (a->val.dbl < b ? (-1) : 0);
    case AP_SCALAR_MPQ:
      return mpq_cmp_si(a->val.mpq,b,1);  
    case AP_SCALAR_MPFR:
      return mpfr_cmp_si(a->val.mpfr,b);
    default:
      abort();
    }
  }
}
bool ap_scalar_equal(ap_scalar_t* a, ap_scalar_t* b)
{
  int s1 = ap_scalar_infty(a);
  int s2 = ap_scalar_infty(b);
  
  if (s1!=s2)
    return false;
  else if (s1!=0)
    return true;
  else if (a->discr == b->discr) {
    switch (a->discr){
    case AP_SCALAR_MPQ:    return mpq_equal(a->val.mpq,b->val.mpq);
    case AP_SCALAR_DOUBLE: return a->val.dbl == b->val.dbl;
    case AP_SCALAR_MPFR:   return mpfr_equal_p(a->val.mpfr,b->val.mpfr);
    default:               abort();
    }
  }
  else return ap_scalar_cmp(a,b)==0;
}
bool ap_scalar_equal_int(ap_scalar_t* a, int b)
{
  int s1 = ap_scalar_infty(a);
  
  if (s1!=0)
    return false;
  else {
    switch (a->discr){
    case AP_SCALAR_DOUBLE:  return a->val.dbl == b;
    case AP_SCALAR_MPQ:     return mpq_cmp_si(a->val.mpq,b,1)==0;
    case AP_SCALAR_MPFR:    return mpfr_cmp_si(a->val.mpfr,b)==0;
    default:                abort();
    }
  }
}

int ap_scalar_sgn(ap_scalar_t* a)
{
  int res;
  res = ap_scalar_infty(a);
  if (!res) {
    switch (a->discr){
    case AP_SCALAR_MPQ:    res = mpq_sgn(a->val.mpq); break;
    case AP_SCALAR_DOUBLE: res = a->val.dbl > 0.0 ? 1 : (a->val.dbl == 0.0 ? 0 : (-1)); break;
    case AP_SCALAR_MPFR:   res = mpfr_sgn(a->val.mpfr) > 0 ? 1 : (mpfr_sgn(a->val.mpfr) == 0.0 ? 0 : (-1)); break;
    default:               abort();
    }
  }
  return res > 0 ? 1 : res < 0 ? -1 : 0;
}

/* ====================================================================== */
/* Other operations */
/* ====================================================================== */

void ap_scalar_neg(ap_scalar_t* a, ap_scalar_t* b)
{
  ap_scalar_reinit(a,b->discr);
  switch(b->discr){
  case AP_SCALAR_MPQ:
    mpq_neg(a->val.mpq,b->val.mpq);
    break;
  case AP_SCALAR_DOUBLE:
    a->val.dbl = -b->val.dbl;
    break;
  case AP_SCALAR_MPFR:
    if (a!=b) mpfr_set_prec(a->val.mpfr,mpfr_get_prec(b->val.mpfr));
    mpfr_neg(a->val.mpfr,b->val.mpfr,GMP_RNDU);
    break;
  default:
    abort();
  }
}
void ap_scalar_inv(ap_scalar_t* a, ap_scalar_t* b)
{
  ap_scalar_reinit(a,b->discr);
  switch(b->discr){
  case AP_SCALAR_MPQ:
    mpq_inv(a->val.mpq,b->val.mpq);
    break;
  case AP_SCALAR_DOUBLE:
    a->val.dbl = 1.0/b->val.dbl;
    break;
  case AP_SCALAR_MPFR:
    if (a!=b) mpfr_set_prec(a->val.mpfr,mpfr_get_prec(b->val.mpfr));
    mpfr_ui_div(a->val.mpfr,1,b->val.mpfr,GMP_RNDU);
    break;
  default:
    abort();
  }
}

long ap_scalar_hash(ap_scalar_t* a)
{
  int infty = ap_scalar_infty(a);
  if (infty<0) return LONG_MIN;
  else if (infty>0) return LONG_MAX;
  else {
    switch (a->discr){
    case AP_SCALAR_MPQ:
      return mpz_get_si(mpq_numref(a->val.mpq))/mpz_get_ui(mpq_denref(a->val.mpq));
    case AP_SCALAR_DOUBLE:
      return (int)(a->val.dbl);
    case AP_SCALAR_MPFR:
      return mpfr_get_si(a->val.mpfr,GMP_RNDU); /* XXX improve this */
    default:
      abort();
    }
  }
}

