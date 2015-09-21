/* ************************************************************************* */
/* ap_interval.c: intervals */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <assert.h>

#include "ap_interval.h"

/* ====================================================================== */
/* Basics */
/* ====================================================================== */

ap_interval_t* ap_interval_alloc()
{
  ap_interval_t* itv = malloc(sizeof(ap_interval_t));
  itv->inf = ap_scalar_alloc();
  itv->sup = ap_scalar_alloc();
  return itv;
}
void ap_interval_reinit(ap_interval_t* itv,  ap_scalar_discr_t d)
{
  ap_scalar_reinit(itv->inf,d);
  ap_scalar_reinit(itv->sup,d);
}
void ap_interval_free(ap_interval_t* itv)
{
  ap_scalar_free(itv->inf);
  ap_scalar_free(itv->sup);
  free(itv);
}
void ap_interval_fprint(FILE* stream, ap_interval_t* a)
{
  fprintf(stream,"[");
  ap_scalar_fprint(stream,a->inf);
  fprintf(stream,",");
  ap_scalar_fprint(stream,a->sup);
  fprintf(stream,"]");
}

/* ====================================================================== */
/* Assignments */
/* ====================================================================== */

void ap_interval_set(ap_interval_t* interval, ap_interval_t* interval2)
{
  ap_scalar_set(interval->inf,interval2->inf);
  ap_scalar_set(interval->sup,interval2->sup);
}
void ap_interval_set_scalar(ap_interval_t* interval, ap_scalar_t* inf, ap_scalar_t* sup)
{
  ap_scalar_set(interval->inf,inf);
  ap_scalar_set(interval->sup,sup);
}
void ap_interval_set_mpq(ap_interval_t* interval, mpq_t inf, mpq_t sup)
{
  ap_scalar_set_mpq(interval->inf,inf);
  ap_scalar_set_mpq(interval->sup,sup);
}
void ap_interval_set_int(ap_interval_t* interval, long int inf, long int sup)
{
  ap_scalar_set_int(interval->inf,inf);
  ap_scalar_set_int(interval->sup,sup);
}
void ap_interval_set_frac(ap_interval_t* interval, long int numinf, unsigned long int deninf, long int numsup, unsigned long int densup)
{
  ap_scalar_set_frac(interval->inf,numinf,deninf);
  ap_scalar_set_frac(interval->sup,numsup,densup);
}
void ap_interval_set_double(ap_interval_t* interval, double inf, double sup)
{
  ap_scalar_set_double(interval->inf,inf);
  ap_scalar_set_double(interval->sup,sup);
}
void ap_interval_set_mpfr(ap_interval_t* interval, mpfr_t inf, mpfr_t sup)
{
  ap_scalar_set_mpfr(interval->inf,inf);
  ap_scalar_set_mpfr(interval->sup,sup);
}
void ap_interval_set_top(ap_interval_t* interval)
{
  ap_scalar_set_infty(interval->inf,-1);
  ap_scalar_set_infty(interval->sup,+1);
}
void ap_interval_set_bottom(ap_interval_t* interval)
{
  switch (interval->inf->discr) {
  case AP_SCALAR_DOUBLE: interval->inf->val.dbl = 1.; break;
  case AP_SCALAR_MPQ:    mpq_set_si(interval->inf->val.mpq,1,1); break;
  case AP_SCALAR_MPFR:   mpfr_set_si(interval->inf->val.mpfr,1,GMP_RNDU); break;
  default:               abort();
  }
  switch (interval->sup->discr) {
  case AP_SCALAR_DOUBLE: interval->sup->val.dbl = -1.; break;
  case AP_SCALAR_MPQ:    mpq_set_si(interval->sup->val.mpq,-1,1); break;
  case AP_SCALAR_MPFR:   mpfr_set_si(interval->sup->val.mpfr,-1,GMP_RNDD); break;
  default:               abort();
  }
}

/* ====================================================================== */
/* Combined allocation and assignments */
/* ====================================================================== */

ap_interval_t* ap_interval_alloc_set(ap_interval_t* interval)
{
  ap_interval_t* itv = malloc(sizeof(ap_interval_t));
  itv->inf = ap_scalar_alloc_set(interval->inf);
  itv->sup = ap_scalar_alloc_set(interval->sup);
  return itv;
}

/* ====================================================================== */
/* Tests */
/* ====================================================================== */

bool ap_interval_is_top(ap_interval_t* interval)
{
  return ap_scalar_infty(interval->inf)<0 && ap_scalar_infty(interval->sup)>0;
}
bool ap_interval_is_bottom(ap_interval_t* interval)
{
  return ap_scalar_cmp(interval->inf,interval->sup)>0;
}
bool ap_interval_is_leq(ap_interval_t* itv1, ap_interval_t* itv2)
{
  int sinf = ap_scalar_cmp(itv1->inf,itv2->inf);
  int ssup = ap_scalar_cmp(itv1->sup,itv2->sup);
  return (sinf>=0 && ssup<=0) || ap_interval_is_bottom(itv1);
}
int ap_interval_cmp(ap_interval_t* itv1, ap_interval_t* itv2)
{
  int sinf = ap_scalar_cmp(itv1->inf,itv2->inf);
  int ssup = ap_scalar_cmp(itv1->sup,itv2->sup);

  if (sinf==0 && ssup==0) return 0;
  else if (sinf>=0 && ssup<=0) return -1;
  else if (sinf<=0 && ssup>=0) return 1;
  else {
    bool b1 = ap_interval_is_bottom(itv1);
    bool b2 = ap_interval_is_bottom(itv2);
    if (b1 && b2) return 0;
    else if (b1) return -1;
    else if (b2) return 1;
    else return sinf > 0 ? 2 : -2;
  }
}
bool ap_interval_equal(ap_interval_t* itv1, ap_interval_t* itv2)
{  
  bool inf = ap_scalar_equal(itv1->inf,itv2->inf);
  bool sup = ap_scalar_equal(itv1->sup,itv2->sup);
  if (inf && sup) return true;
  else return ap_interval_is_bottom(itv1) && ap_interval_is_bottom(itv2);
}
bool ap_interval_equal_int(ap_interval_t* itv, int b)
{  
  return ap_scalar_equal_int(itv->inf,b) && ap_scalar_equal_int(itv->sup,b);
}

/* ====================================================================== */
/* Other operations */
/* ====================================================================== */

void ap_interval_neg(ap_interval_t* a, ap_interval_t* b)
{
  if (a==b){
    ap_scalar_swap(a->inf,a->sup);
    ap_scalar_neg(a->inf,a->inf);
    ap_scalar_neg(a->sup,a->sup);
  } else {
    ap_scalar_neg(a->inf,b->sup);
    ap_scalar_neg(a->sup,b->inf);
  }
}

long ap_interval_hash(ap_interval_t* itv)
{
  if (ap_interval_is_bottom(itv)) return 0;
  else return 5*ap_scalar_hash(itv->inf) + 7*ap_scalar_hash(itv->sup);
}


/* ====================================================================== */
/* Array of intervals */
/* ====================================================================== */

ap_interval_t** ap_interval_array_alloc(size_t size)
{
  size_t i;

  ap_interval_t** array = malloc(size*sizeof(ap_interval_t*));
  for (i=0;i<size;i++){
    array[i] = ap_interval_alloc();
  }
  return array;
}

void ap_interval_array_free(ap_interval_t** array, size_t size)
{
  size_t i;
  for (i=0; i<size; i++){
    ap_interval_free(array[i]);
  }
  free(array);
}
