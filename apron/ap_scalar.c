/* ************************************************************************* */
/* ap_scalar.c: coefficients and intervals */
/* ************************************************************************* */

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
    switch(scalar->discr){
    case AP_SCALAR_MPQ:
      mpq_clear(scalar->val.mpq);
      free(scalar->val.mpq);
      scalar->val.dbl = 0.0;
      break;
    case AP_SCALAR_DOUBLE:
      scalar->val.mpq = malloc(sizeof(mpq_t));
      mpq_init(scalar->val.mpq);
      break;
    }
    scalar->discr = d;
  }
}

void ap_scalar_free(ap_scalar_t* scalar)
{
  ap_scalar_clear(scalar);
  free(scalar);
}

void ap_scalar_fprint(FILE* stream, const ap_scalar_t* a)
{
  int flag;

  flag = ap_scalar_infty(a);
  if (flag){
    fprintf(stream, flag>0 ? "+oo" : "-oo");
  }
  else {
    switch(a->discr){
    case AP_SCALAR_DOUBLE:
      fprintf(stream,"%.20g",a->val.dbl + 0.0);
      break;
    case AP_SCALAR_MPQ:
      mpq_out_str(stream,10,a->val.mpq);
      break;
    }
  } 
}

/* ====================================================================== */
/* Combined allocation and assignements */
/* ====================================================================== */

ap_scalar_t* ap_scalar_alloc_set(const ap_scalar_t* b)
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
  }
  return a;
}
ap_scalar_t* ap_scalar_alloc_set_mpq(const mpq_t mpq)
{
  ap_scalar_t* a = malloc(sizeof(ap_scalar_t));
  a->discr = AP_SCALAR_MPQ;
  a->val.mpq = malloc(sizeof(mpq_t)); 
  mpq_init(a->val.mpq);
  mpq_set(a->val.mpq,mpq);
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
/* Assignements */
/* ====================================================================== */

void ap_scalar_set(ap_scalar_t* a, const ap_scalar_t* b)
{
  ap_scalar_reinit(a,b->discr);
  switch(b->discr){
  case AP_SCALAR_DOUBLE:
    a->val.dbl = b->val.dbl;
    break;
  case AP_SCALAR_MPQ:
    mpq_set(a->val.mpq,b->val.mpq);
    break;
  }
}
void ap_scalar_set_mpq(ap_scalar_t* scalar, const mpq_t mpq)
  { ap_scalar_reinit(scalar,AP_SCALAR_MPQ); mpq_set(scalar->val.mpq,mpq); }
void ap_scalar_set_int(ap_scalar_t* scalar, long int i)
  { ap_scalar_reinit(scalar,AP_SCALAR_MPQ); mpq_set_si(scalar->val.mpq,i,1); }
void ap_scalar_set_frac(ap_scalar_t* scalar, long int i, unsigned long int j)
  { ap_scalar_reinit(scalar,AP_SCALAR_MPQ); mpq_set_si(scalar->val.mpq,i,j); }
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
  }
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

void ap_mpq_set_scalar(mpq_t mpq, const ap_scalar_t* scalar, int round)
{
  switch(scalar->discr){
  case AP_SCALAR_MPQ:
    mpq_set(mpq,scalar->val.mpq);
    break;
  case AP_SCALAR_DOUBLE:
    mpq_set_d(mpq,scalar->val.dbl);
    break;
  }
}

double ap_scalar_get_double(const ap_scalar_t* scalar, int round)
{
  switch(scalar->discr){
  case AP_SCALAR_MPQ:
    return mpq_get_d(scalar->val.mpq);
  case AP_SCALAR_DOUBLE:
    return scalar->val.dbl;
  default:
    abort();
  }
}

/* ====================================================================== */
/* Tests */
/* ====================================================================== */

int ap_scalar_infty(const ap_scalar_t* scalar)
{
  switch(scalar->discr){
  case AP_SCALAR_MPQ:
    return 
      (mpz_sgn(mpq_denref(scalar->val.mpq)))==0 ?
      (mpz_sgn(mpq_numref(scalar->val.mpq))) :
      0;
  case AP_SCALAR_DOUBLE:
    return 
      scalar->val.dbl==(double)1.0/(double)0.0 ?
      1 :
      ( scalar->val.dbl==-(double)1.0/(double)0.0 ?
	-1 :
	0);
  default:
    assert(0);
    return 0;
  }
}

int ap_scalar_cmp(const ap_scalar_t* a, const ap_scalar_t* b)
{
  int s1 = ap_scalar_infty(a);
  int s2 = ap_scalar_infty(b);
  
  if (s1>s2)
    return 1;
  else if (s1<s2)
    return -1;
  else if (s1!=0)
    return 0;
  else {
    if (a->discr != b->discr){
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
    else {
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
      default:
	assert(0);
	return 0;
      }
    }
  }
}
int ap_scalar_cmp_int(const ap_scalar_t* a, int b)
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
    default:
      abort();
    }
  }
}
bool ap_scalar_equal(const ap_scalar_t* a, const ap_scalar_t* b)
{
  int s1 = ap_scalar_infty(a);
  int s2 = ap_scalar_infty(b);
  
  if (s1!=s2)
    return false;
  else if (s1!=0)
    return true;
  else {
    if (a->discr != b->discr){
      mpq_t mpq;
      bool res;

      mpq_init(mpq);
      if (a->discr==AP_SCALAR_DOUBLE){
	mpq_set_d(mpq,a->val.dbl);
	res = mpq_cmp(mpq,b->val.mpq)==0;
      }
      else {
	mpq_set_d(mpq,b->val.dbl);
	res = mpq_cmp(a->val.mpq,mpq)==0;
      }
      mpq_clear(mpq);
      return res;
    } 
    else {
      switch (a->discr){
      case AP_SCALAR_MPQ:
	return mpq_equal(a->val.mpq,b->val.mpq);
      case AP_SCALAR_DOUBLE:
	return a->val.dbl == b->val.dbl;
      default:
	assert(0);
	return 0;
      }
    }
  }
}
bool ap_scalar_equal_int(const ap_scalar_t* a, int b)
{
  int s1 = ap_scalar_infty(a);
  
  if (s1!=0)
    return false;
  else {
    switch (a->discr){
    case AP_SCALAR_DOUBLE:
      return a->val.dbl == b;
    case AP_SCALAR_MPQ:
      return mpq_cmp_si(a->val.mpq,b,1)==0;
    default:
      abort();
    }
  }
}

int ap_scalar_sgn(const ap_scalar_t* a)
{
  int res;
  res = ap_scalar_infty(a);
  if (res) 
    return res;
  else {
    switch (a->discr){
    case AP_SCALAR_MPQ:
      res = mpq_sgn(a->val.mpq);
      break;
    case AP_SCALAR_DOUBLE:
      res = a->val.dbl > 0.0 ? 1 : (a->val.dbl == 0.0 ? 0 : (-1));
      break;
    default:
      abort();
      break;
    }
  }
  return res;
}

/* ====================================================================== */
/* Other operations */
/* ====================================================================== */

void ap_scalar_neg(ap_scalar_t* a, const ap_scalar_t* b)
{
  ap_scalar_reinit(a,b->discr);
  switch(b->discr){
  case AP_SCALAR_MPQ:
    mpq_neg(a->val.mpq,b->val.mpq);
    break;
  case AP_SCALAR_DOUBLE:
    a->val.dbl = -b->val.dbl;
    break;
  }
}
void ap_scalar_inv(ap_scalar_t* a, const ap_scalar_t* b)
{
  ap_scalar_reinit(a,b->discr);
  switch(b->discr){
  case AP_SCALAR_MPQ:
    mpq_inv(a->val.mpq,b->val.mpq);
    break;
  case AP_SCALAR_DOUBLE:
    a->val.dbl = 1.0/b->val.dbl;
    break;
  }
}

int ap_scalar_hash(const ap_scalar_t* a)
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
    default:
      abort();
      return 0;
    }
  }
}

