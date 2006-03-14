/* ************************************************************************* */
/* coeff.c: coefficients and intervals */
/* ************************************************************************* */

#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <assert.h>

#include "coeff.h"

/* ********************************************************************** */
/* I. Scalars  */
/* ********************************************************************** */

static inline 
void scalar_init(scalar_t* scalar, scalar_discr_t d)
{
  switch(scalar->discr){
  case SCALAR_MPQ:
    scalar->val.mpq = malloc(sizeof(mpq_t));
    mpq_init(scalar->val.mpq);
    break;
  case SCALAR_DOUBLE:
    scalar->val.dbl = 0.0;
    break;
  }
}

static inline 
void scalar_clear(scalar_t* scalar)
{
  switch(scalar->discr){
  case SCALAR_MPQ:
    mpq_clear(scalar->val.mpq);
    free(scalar->val.mpq);
    break;
  case SCALAR_DOUBLE:
    break;
  }
}

scalar_t* scalar_alloc()
{
  scalar_t* scalar = malloc(sizeof(scalar_t));
  scalar->discr = SCALAR_DOUBLE;
  scalar->val.dbl = 0.0;
  return scalar;
}

void scalar_reinit(scalar_t* scalar, scalar_discr_t d)
{ 
  if (scalar->discr != d){ 
    scalar_clear(scalar); 
    scalar_init(scalar,d); 
  }
}

void scalar_free(scalar_t* scalar)
{
  scalar_clear(scalar);
  free(scalar);
}

/* Combined allocation and assignements */

scalar_t* scalar_alloc_set(const scalar_t* b)
{
  scalar_t* a = malloc(sizeof(scalar_t));
  a->discr = b->discr;
  switch(b->discr){
  case SCALAR_DOUBLE:
    a->val.dbl = b->val.dbl;
    break;
  case SCALAR_MPQ:
    a->val.mpq = malloc(sizeof(mpq_t)); 
    mpq_init(a->val.mpq);
    mpq_set(a->val.mpq,b->val.mpq);
    break;
  }
  return a;
}
scalar_t* scalar_alloc_set_mpq(const mpq_t mpq)
{
  scalar_t* a = malloc(sizeof(scalar_t));
  a->discr = SCALAR_MPQ;
  a->val.mpq = malloc(sizeof(mpq_t)); 
  mpq_init(a->val.mpq);
  mpq_set(a->val.mpq,mpq);
  return a;
}
scalar_t* scalar_alloc_set_double(double k)
{
  scalar_t* a = malloc(sizeof(scalar_t));
  a->discr = SCALAR_DOUBLE;
  a->val.dbl = k;
  return a;
}

/* Assignements */

void scalar_set(scalar_t* a, const scalar_t* b)
{
  scalar_reinit(a,b->discr);
  switch(b->discr){
  case SCALAR_DOUBLE:
    a->val.dbl = b->val.dbl;
    break;
  case SCALAR_MPQ:
    mpq_set(a->val.mpq,b->val.mpq);
    break;
  }
}
void scalar_set_mpq(scalar_t* scalar, const mpq_t mpq)
  { scalar_reinit(scalar,SCALAR_MPQ); mpq_set(scalar->val.mpq,mpq); }
void scalar_set_int(scalar_t* scalar, long int i)
  { scalar_reinit(scalar,SCALAR_MPQ); mpq_set_si(scalar->val.mpq,i,1); }
void scalar_set_frac(scalar_t* scalar, long int i, unsigned long int j)
  { scalar_reinit(scalar,SCALAR_MPQ); mpq_set_si(scalar->val.mpq,i,j); }
void scalar_set_double(scalar_t* scalar, double k)
  { scalar_reinit(scalar,SCALAR_DOUBLE); scalar->val.dbl = k; }
void scalar_set_infty(scalar_t* scalar, int sgn)
{
  switch(scalar->discr){
  case SCALAR_MPQ:
    mpz_set_si(mpq_numref(scalar->val.mpq),sgn);
    mpz_set_ui(mpq_denref(scalar->val.mpq),sgn ? 0 : 1);
    break;
  case SCALAR_DOUBLE:
    scalar->val.dbl = sgn>0 ? (double)1.0/(double)0.0 : (sgn < 0 ? -(double)1.0/(double)0.0 : 0.0);
    break;
  }
}

/* Conversions */
void mpq_set_scalar(mpq_t mpq, const scalar_t* scalar, int round)
{
  switch(scalar->discr){
  case SCALAR_MPQ:
    mpq_set(mpq,scalar->val.mpq);
    break;
  case SCALAR_DOUBLE:
    mpq_set_d(mpq,scalar->val.dbl);
    break;
  }
}

double scalar_get_double(const scalar_t* scalar, int round)
{
  switch(scalar->discr){
  case SCALAR_MPQ:
    return mpq_get_d(scalar->val.mpq);
  case SCALAR_DOUBLE:
    return scalar->val.dbl;
  default:
    abort();
  }
}

/* Tests */

int scalar_infty(const scalar_t* scalar)
{
  switch(scalar->discr){
  case SCALAR_MPQ:
    return 
      (mpz_sgn(mpq_denref(scalar->val.mpq)))==0 ?
      (mpz_sgn(mpq_numref(scalar->val.mpq))) :
      0;
  case SCALAR_DOUBLE:
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

int scalar_cmp(const scalar_t* a, const scalar_t* b)
{
  int s1 = scalar_infty(a);
  int s2 = scalar_infty(b);
  
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
      if (a->discr==SCALAR_DOUBLE){
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
      case SCALAR_MPQ:
	return mpq_cmp(a->val.mpq,b->val.mpq);
      case SCALAR_DOUBLE:
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
bool scalar_equal(const scalar_t* a, const scalar_t* b)
{
  int s1 = scalar_infty(a);
  int s2 = scalar_infty(b);
  
  if (s1!=s2)
    return false;
  else if (s1!=0)
    return true;
  else {
    if (a->discr != b->discr){
      mpq_t mpq;
      bool res;

      mpq_init(mpq);
      if (a->discr==SCALAR_DOUBLE){
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
      case SCALAR_MPQ:
	return mpq_equal(a->val.mpq,b->val.mpq);
      case SCALAR_DOUBLE:
	return a->val.dbl == b->val.dbl;
      default:
	assert(0);
	return 0;
      }
    }
  }
}

int scalar_sgn(const scalar_t* a)
{
  int res;
  res = scalar_infty(a);
  if (res) 
    return res;
  else {
    switch (a->discr){
    case SCALAR_MPQ:
      res = mpq_sgn(a->val.mpq);
      break;
    case SCALAR_DOUBLE:
      res = a->val.dbl > 0.0 ? 1 : (a->val.dbl == 0.0 ? 0 : (-1));
      break;
    default:
      abort();
      break;
    }
  }
  return res;
}

/* Other operations */

void scalar_neg(scalar_t* a, const scalar_t* b)
{
  scalar_reinit(a,b->discr);
  switch(a->discr){
  case SCALAR_MPQ:
    mpq_neg(a->val.mpq,b->val.mpq);
    break;
  case SCALAR_DOUBLE:
    a->val.dbl = -b->val.dbl;
    break;
  }
}
void scalar_inv(scalar_t* a, const scalar_t* b)
{
  scalar_reinit(a,b->discr);
  switch(a->discr){
  case SCALAR_MPQ:
    mpq_inv(a->val.mpq,b->val.mpq);
    break;
  case SCALAR_DOUBLE:
    a->val.dbl = 1.0/b->val.dbl;
    break;
  }
}

int scalar_hash(const scalar_t* a)
{
  int infty = scalar_infty(a);
  if (infty<0) return LONG_MIN;
  else if (infty>0) return LONG_MAX;
  else {
    switch (a->discr){
    case SCALAR_MPQ:
      return mpz_get_si(mpq_numref(a->val.mpq))/mpz_get_ui(mpq_denref(a->val.mpq));
    case SCALAR_DOUBLE:
      return (int)(a->val.dbl);
    default:
      abort();
      return 0;
    }
  }
}

void scalar_fprint(FILE* stream, const scalar_t* a)
{
  int flag;

  flag = scalar_infty(a);
  if (flag){
    fprintf(stream, flag>0 ? "+oo" : "-oo");
  }
  else {
    switch(a->discr){
    case SCALAR_DOUBLE:
      fprintf(stream,"%.20g",a->val.dbl + 0.0);
      break;
    case SCALAR_MPQ:
      mpq_out_str(stream,10,a->val.mpq);
      break;
    }
  } 
}

/* ********************************************************************** */
/* II. Intervals  */
/* ********************************************************************** */

interval_t* interval_alloc()
{
  interval_t* itv = malloc(sizeof(interval_t));
  itv->inf = scalar_alloc();
  itv->sup = scalar_alloc();
  return itv;
}
void interval_reinit(interval_t* itv,  scalar_discr_t d)
{
  scalar_reinit(itv->inf,d);
  scalar_reinit(itv->sup,d);
}
void interval_free(interval_t* itv)
{
  scalar_free(itv->inf);
  scalar_free(itv->sup);
  free(itv);
}

/* Assignements */

void interval_set(interval_t* interval, const interval_t* interval2)
{
  scalar_set(interval->inf,interval2->inf);
  scalar_set(interval->sup,interval2->sup);
}
void interval_set_infsup(interval_t* interval, const scalar_t* inf, const scalar_t* sup)
{
  scalar_set(interval->inf,inf);
  scalar_set(interval->sup,sup);
}
void interval_set_infsup_int(interval_t* interval, int inf, int sup)
{
  scalar_set_int(interval->inf,inf);
  scalar_set_int(interval->sup,sup);
}
void interval_set_infsup_frac(interval_t* interval, int numinf, unsigned int deninf, int numsup, unsigned int densup)
{
  scalar_set_frac(interval->inf,numinf,deninf);
  scalar_set_frac(interval->sup,numsup,densup);
}
void interval_set_infsup_double(interval_t* interval, double inf, double sup)
{
  scalar_set_double(interval->inf,inf);
  scalar_set_double(interval->sup,sup);
}
void interval_set_top(interval_t* interval)
{
  scalar_set_infty(interval->inf,-1);
  scalar_set_infty(interval->sup,+1);
}
void interval_set_bottom(interval_t* interval)
{
  if (interval->inf->discr==SCALAR_DOUBLE)
    scalar_set_double(interval->inf,+1.0);
  else
    scalar_set_int(interval->inf,1);
  if (interval->sup->discr==SCALAR_DOUBLE)
    scalar_set_double(interval->sup,-1.0);
  else
    scalar_set_int(interval->sup,-1);
}

/* Combined allocation and assignements */

interval_t* interval_alloc_set(const interval_t* interval)
{
  interval_t* itv = malloc(sizeof(interval_t));
  itv->inf = scalar_alloc_set(interval->inf);
  itv->sup = scalar_alloc_set(interval->sup);
  return itv;
}
interval_t* interval_alloc_set_infsup(const scalar_t* inf, const scalar_t* sup)
{
  interval_t* itv = malloc(sizeof(interval_t));
  itv->inf = scalar_alloc_set(inf);
  itv->sup = scalar_alloc_set(sup);
  return itv;
}

/* Tests */

bool interval_is_top(const interval_t* interval)
{
  return scalar_infty(interval->inf)<0 && scalar_infty(interval->sup)>0;
}
bool interval_is_bottom(const interval_t* interval)
{
  return scalar_cmp(interval->inf,interval->sup)>0;
}
bool interval_is_leq(const interval_t* itv1, const interval_t* itv2)
{
  int sinf = scalar_cmp(itv1->inf,itv2->inf);
  int ssup = scalar_cmp(itv1->sup,itv2->sup);
  return (sinf>=0 && ssup<=0) || interval_is_bottom(itv1);
}
int interval_cmp(const interval_t* itv1, const interval_t* itv2)
{
  int sinf = scalar_cmp(itv1->inf,itv2->inf);
  int ssup = scalar_cmp(itv1->sup,itv2->sup);

  if (sinf==0 && ssup==0) return 0;
  else if (sinf>=0 && ssup<=0) return -1;
  else if (sinf<=0 && ssup>=0) return 1;
  else {
    bool b1 = interval_is_bottom(itv1);
    bool b2 = interval_is_bottom(itv2);
    if (b1 && b2) return 0;
    else if (b1) return -1;
    else if (b2) return 1;
    else return sinf > 0 ? 2 : -2;
  }
}
bool interval_equal(const interval_t* itv1, const interval_t* itv2)
{  
  bool inf = scalar_equal(itv1->inf,itv2->inf);
  bool sup = scalar_equal(itv1->sup,itv2->sup);
  if (inf && sup) return true;
  else return interval_is_bottom(itv1) && interval_is_bottom(itv2);
}
/* Other operations */

int interval_hash(const interval_t* itv)
{
  if (interval_is_bottom(itv)) return 0;
  else return scalar_hash(itv->inf) + 2*scalar_hash(itv->sup);
}

void interval_fprint(FILE* stream, const interval_t* a)
{
  fprintf(stream,"[");
  scalar_fprint(stream,a->inf);
  fprintf(stream,",");
  scalar_fprint(stream,a->sup);
  fprintf(stream,"]");
}

/* Array of intervals */
interval_t** interval_array_alloc(size_t size)
{
  size_t i;

  interval_t** array = malloc(size*sizeof(interval_t*));
  for (i=0;i<size;i++){
    array[i] = interval_alloc();
  }
  return array;
}

void interval_array_free(interval_t** array, size_t size)
{
  size_t i;
  for (i=0; i<size; i++){
    interval_free(array[i]);
  }
  free(array);
}

/* ********************************************************************** */
/* III. Coefficients  */
/* ********************************************************************** */

/* FOR INTERNAL USE ONLY */
void coeff_init(coeff_t* coeff, coeff_discr_t coeff_discr)
{
  coeff->discr = coeff_discr;
  switch (coeff_discr){
  case COEFF_SCALAR:
    coeff->val.scalar = scalar_alloc();
    break;
  case COEFF_INTERVAL:
    coeff->val.interval = interval_alloc();
    break;
  }
}
/* FOR INTERNAL USE ONLY */
void coeff_init_set(coeff_t* coeff, coeff_t* coeff2)
{
  coeff->discr = coeff2->discr;
  switch (coeff2->discr){
  case COEFF_SCALAR:
    coeff->val.scalar = scalar_alloc_set(coeff2->val.scalar);
    break;
  case COEFF_INTERVAL:
    coeff->val.interval = interval_alloc_set(coeff2->val.interval);
    break;
  }
}
/* FOR INTERNAL USE ONLY */
void coeff_clear(coeff_t* coeff)
{
  switch (coeff->discr){
  case COEFF_SCALAR:
    scalar_free(coeff->val.scalar);
    break;
  case COEFF_INTERVAL:
    interval_free(coeff->val.interval);
    break;
  }
}

coeff_t* coeff_alloc(coeff_discr_t coeff_discr)
{
  coeff_t* coeff = malloc(sizeof(coeff_t));
  coeff->discr = coeff_discr;
  switch (coeff_discr){
  case COEFF_SCALAR:
    coeff->val.scalar = scalar_alloc();
    break;
  case COEFF_INTERVAL:
    coeff->val.interval = interval_alloc();
    break;
  }
  return coeff;
}
void coeff_reinit(coeff_t* coeff, coeff_discr_t coeff_discr, scalar_discr_t scalar_discr)
{
  switch (coeff_discr){
  case COEFF_SCALAR:
    if (coeff_discr != coeff->discr){
      scalar_free(coeff->val.scalar);
      coeff->val.interval = interval_alloc();
    }
    interval_reinit(coeff->val.interval,scalar_discr);
    break;
  case COEFF_INTERVAL:
    if (coeff_discr != coeff->discr){
      interval_free(coeff->val.interval);
      coeff->val.scalar = scalar_alloc();
    }
    scalar_reinit(coeff->val.scalar,scalar_discr);
    break;
  }
  coeff->discr = coeff_discr;
}

void coeff_free(coeff_t* coeff)
{
  switch (coeff->discr){
  case COEFF_SCALAR:
    scalar_free(coeff->val.scalar);
    break;
  case COEFF_INTERVAL:
    interval_free(coeff->val.interval);
    break;
  }
  free(coeff);
}

void coeff_reduce(coeff_t* coeff)
{
  if (coeff->discr==COEFF_INTERVAL){
    if (scalar_equal(coeff->val.interval->inf,coeff->val.interval->sup)){
      /* We cheat with the good rules */
      scalar_t* scalar = coeff->val.interval->inf;
      scalar_free(coeff->val.interval->sup);
      free(coeff->val.interval);
      coeff->val.scalar = scalar;
    }
  }
}

/* Combined allocation and assignement */
coeff_t* coeff_alloc_set_scalar(const scalar_t* scalar)
{
  coeff_t* coeff = malloc(sizeof(coeff_t));
  coeff->discr = COEFF_SCALAR;
  coeff->val.scalar = scalar_alloc_set(scalar);
  return coeff;
}
coeff_t* coeff_alloc_set_interval(const interval_t* interval)
{
  coeff_t* coeff = malloc(sizeof(coeff_t));
  coeff->discr = COEFF_INTERVAL;
  coeff->val.interval = interval_alloc_set(interval);
  return coeff;
}
coeff_t* coeff_alloc_set(const coeff_t* coeff)
{
  switch (coeff->discr){
  case COEFF_SCALAR:
    return coeff_alloc_set_scalar(coeff->val.scalar);
  case COEFF_INTERVAL:
    return coeff_alloc_set_interval(coeff->val.interval);
    break;
  default:
    return NULL;
  }
}

/* Assignements */

void coeff_set(coeff_t* a, const coeff_t* b)
{

  if(a->discr != b->discr){
    coeff_reinit(a,b->discr,SCALAR_DOUBLE);
  }
  switch(b->discr){
  case COEFF_SCALAR:
    scalar_set(a->val.scalar,b->val.scalar);
    break;
  case COEFF_INTERVAL:
    interval_set(a->val.interval,b->val.interval);
    break;
  }
}
void coeff_set_scalar(coeff_t* coeff, const scalar_t* scalar)
{
  coeff_reinit(coeff,COEFF_SCALAR,scalar->discr); 
  scalar_set(coeff->val.scalar,scalar); 
}
void coeff_set_scalar_int(coeff_t* coeff, int num)
{
  coeff_reinit(coeff,COEFF_SCALAR,SCALAR_MPQ); 
  scalar_set_int(coeff->val.scalar,num); 
}
void coeff_set_scalar_frac(coeff_t* coeff, int num, unsigned int den)
{
  coeff_reinit(coeff,COEFF_SCALAR,SCALAR_MPQ); 
  scalar_set_frac(coeff->val.scalar,num,den); 
}
void coeff_set_scalar_double(coeff_t* coeff, double num)
{ 
  coeff_reinit(coeff,COEFF_SCALAR,SCALAR_DOUBLE); 
  scalar_set_double(coeff->val.scalar,num); 
}
void coeff_set_interval(coeff_t* coeff, const interval_t* itv)
{
  coeff_reinit(coeff,COEFF_INTERVAL,SCALAR_DOUBLE); 
  interval_set(coeff->val.interval,itv); 
}
void coeff_set_interval_infsup(coeff_t* coeff, const scalar_t* inf, const scalar_t* sup)
{
  interval_set_infsup(coeff->val.interval,inf,sup);
}
void coeff_set_interval_infsup_int(coeff_t* coeff, int inf, int sup)
{
  coeff_reinit(coeff,COEFF_INTERVAL,SCALAR_MPQ); 
  interval_set_infsup_int(coeff->val.interval,inf,sup); 
}
void coeff_set_interval_infsup_frac(coeff_t* coeff,
				  int numinf, unsigned int deninf, 
				  int numsup, unsigned int densup)
{
  coeff_reinit(coeff,COEFF_INTERVAL,SCALAR_MPQ); 
  interval_set_infsup_frac(coeff->val.interval,numinf,deninf,numsup,densup); 
}
void coeff_set_interval_infsup_double(coeff_t* coeff, double inf, double sup)
{
  coeff_reinit(coeff,COEFF_INTERVAL,SCALAR_DOUBLE); 
  interval_set_infsup_double(coeff->val.interval,inf,sup); 
}

/* Tests */

int coeff_cmp(const coeff_t* coeff1, const coeff_t* coeff2)
{
  if (coeff1->discr==coeff2->discr){
    switch (coeff1->discr){
    case COEFF_SCALAR:
      return scalar_cmp(coeff1->val.scalar,coeff2->val.scalar);
    case COEFF_INTERVAL:
      return interval_cmp(coeff1->val.interval,coeff2->val.interval);
    default:
      abort();
      return 0;
    }
  } 
  else {
    return (coeff1->discr==COEFF_SCALAR) ? -3 : 3;
  }
}
bool coeff_equal(const coeff_t* coeff1, const coeff_t* coeff2)
{
  if (coeff1->discr==coeff2->discr){
    switch (coeff1->discr){
    case COEFF_SCALAR:
      return scalar_equal(coeff1->val.scalar,coeff2->val.scalar);
    case COEFF_INTERVAL:
      return interval_equal(coeff1->val.interval,coeff2->val.interval);
    default:
      abort();
    }
  }
  else
    return false;
}
bool coeff_zero(const coeff_t* coeff)
{
  switch (coeff->discr){
  case COEFF_SCALAR:
    return scalar_sgn(coeff->val.scalar)==0;
  case COEFF_INTERVAL:
    return (scalar_sgn(coeff->val.interval->inf)==0) && (scalar_sgn(coeff->val.interval->sup)==0);
  default:
    abort();
  }
}


/* Hash */

int coeff_hash(const coeff_t* coeff)
{
  switch (coeff->discr){
  case COEFF_SCALAR:
    return scalar_hash(coeff->val.scalar);
  case COEFF_INTERVAL:
    return interval_hash(coeff->val.interval);
  default:
    abort();
    return 0;
  }
}
