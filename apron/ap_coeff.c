/* ************************************************************************* */
/* ap_coeff.c: coefficients, that are either scalars or intervals */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <assert.h>

#include "ap_coeff.h"
#include "eitvD_conv.h"
#include "eitvMPQ_conv.h"
#include "eitvMPFR_conv.h"

/* ====================================================================== */
/* Basics */
/* ====================================================================== */

void ap_coeff_init_set(ap_coeff_t* a, ap_coeff_t* b)
{
  a->discr = b->discr;
  switch (b->discr){
  case AP_SCALAR_MPQ:
    eitvMPQ_init_set(a->eitv.MPQ,b->eitv.MPQ);
    break;
  case AP_SCALAR_D:
    eitvD_init_set(a->eitv.D,b->eitv.D);
    break;
  case AP_SCALAR_MPFR:
    eitvMPFR_init_set(a->eitv.MPFR,b->eitv.MPFR);
    break;
  }
}

inline void ap_coeff_clear(ap_coeff_t* coeff)
{
  switch (coeff->discr){
  case AP_SCALAR_MPQ:
    eitvMPQ_clear(coeff->eitv.MPQ);
    break;
  case AP_SCALAR_MPFR:
    eitvMPFR_clear(coeff->eitv.MPFR);
    break;
  default:
    break;
  }
}

ap_coeff_t* ap_coeff_alloc(ap_scalar_discr_t discr)
{
  ap_coeff_t* coeff = malloc(sizeof(ap_coeff_t));
  ap_coeff_init(coeff,discr);
  return coeff;
}
void ap_coeff_reinit(ap_coeff_t* coeff, ap_scalar_discr_t discr)
{
  if (coeff->discr != discr){
    ap_coeff_clear(coeff);
    ap_coeff_init(coeff,discr);
  }
  else {
    ap_coeff_set_int(coeff,1);
  }
}

void ap_coeff_free(ap_coeff_t* coeff)
{
  ap_coeff_clear(coeff);
  free(coeff);
}

void ap_coeff_fprint(FILE* stream, ap_coeff_t* a)
{
  switch(a->discr){
  case AP_SCALAR_D:
    eitvD_fprint(stream,a->eitv.D);
    break;
  case AP_SCALAR_MPQ:
    eitvMPQ_fprint(stream,a->eitv.MPQ);
    break;
  case AP_SCALAR_MPFR:
    eitvMPFR_fprint(stream,a->eitv.MPFR);
    break;
  }
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

void ap_coeff_set_numMPQ(ap_coeff_t* a, numMPQ_t b)
{
  ap_coeff_reinit(a,AP_SCALAR_MPQ);
  eitvMPQ_set_numMPQ(a->eitv.MPQ,b);
}
void ap_coeff_set_numD(ap_coeff_t* a, numD_t b)
{
  ap_coeff_reinit(a,AP_SCALAR_D);
  eitvD_set_numD(a->eitv.D,b);
}
void ap_coeff_set_numMPFR(ap_coeff_t* a, numMPFR_t b)
{
  ap_coeff_reinit(a,AP_SCALAR_MPFR);
  eitvMPFR_set_numMPFR(a->eitv.MPFR,b);
}
void ap_coeff_set_eitvMPQ(ap_coeff_t* a, eitvMPQ_t b)
{
  ap_coeff_reinit(a,AP_SCALAR_MPQ);
  eitvMPQ_set(a->eitv.MPQ,b);
}
void ap_coeff_set_eitvD(ap_coeff_t* a, eitvD_t b)
{
  ap_coeff_reinit(a,AP_SCALAR_D);
  eitvD_set(a->eitv.D,b);
}
void ap_coeff_set_eitvMPFR(ap_coeff_t* a, eitvMPFR_t b)
{
  ap_coeff_reinit(a,AP_SCALAR_MPFR);
  eitvMPFR_set(a->eitv.MPFR,b);
}

void ap_coeff_set(ap_coeff_t* a, ap_coeff_t* b)
{
  if(a->discr != b->discr){
    ap_coeff_reinit(a,b->discr);
  }
  switch(b->discr){
  case AP_SCALAR_D:
    eitvD_set(a->eitv.D, b->eitv.D);
    break;
  case AP_SCALAR_MPQ:
    eitvMPQ_set(a->eitv.MPQ, b->eitv.MPQ);
    break;
  case AP_SCALAR_MPFR:
    eitvMPFR_set(a->eitv.MPFR, b->eitv.MPFR);
    break;
  }
}
void ap_coeff_set_lint(ap_coeff_t* a, long int b)
{
  ap_coeff_reinit(a,AP_SCALAR_MPQ);
  eitvMPQ_set_lint(a->eitv.MPQ,b,NULL);
}
void ap_coeff_set_lint2(ap_coeff_t* a, long int b, long int c)
{
  ap_coeff_reinit(a,AP_SCALAR_MPQ);
  eitvMPQ_set_lint2(a->eitv.MPQ,b,c,NULL);
}
void ap_coeff_set_lfrac(ap_coeff_t* a, long int b, long int c)
{
  ap_coeff_reinit(a,AP_SCALAR_MPQ);
  eitvMPQ_set_lfrac(a->eitv.MPQ,b,c,NULL);
}
void ap_coeff_set_lfrac2(ap_coeff_t* a,
			long int b, long int c,
			long int d, long int e)
{
  ap_coeff_reinit(a,AP_SCALAR_MPQ);
  eitvMPQ_set_lfrac2(a->eitv.MPQ,b,c,d,e,NULL);
}
void ap_coeff_set_llfrac(ap_coeff_t* a, long long int b, long long int c)
{
  ap_coeff_reinit(a,AP_SCALAR_MPQ);
  eitvMPQ_set_lfrac(a->eitv.MPQ,b,c,NULL);
}
void ap_coeff_set_llfrac2(ap_coeff_t* a,
			long long int b, long long int c,
			long long int d, long long int e)
{
  ap_coeff_reinit(a,AP_SCALAR_MPQ);
  eitvMPQ_set_lfrac2(a->eitv.MPQ,b,c,d,e,NULL);
}
void ap_coeff_set_mpq(ap_coeff_t* a, mpq_t b)
{ ap_coeff_set_numMPQ(a,b); }
void ap_coeff_set_mpq2(ap_coeff_t* a, mpq_t b, mpq_t c)
{
  ap_coeff_reinit(a,AP_SCALAR_MPQ);
  eitvMPQ_set_numMPQ2(a->eitv.MPQ,b,c);
}
void ap_coeff_set_double(ap_coeff_t* a, double b)
{ ap_coeff_set_numD(a,&b); }
void ap_coeff_set_double2(ap_coeff_t* a, double b, double c)
{
  ap_coeff_reinit(a,AP_SCALAR_D);
  eitvD_set_numD2(a->eitv.D,&b,&c);
}
void ap_coeff_set_ldouble(ap_coeff_t* a, long double b)
{
  ap_coeff_reinit(a,AP_SCALAR_MPFR);
  eitvMPFR_set_ldouble(a->eitv.MPFR,b,NULL);
}
void ap_coeff_set_ldouble2(ap_coeff_t* a, long double b, long double c)
{
  ap_coeff_reinit(a,AP_SCALAR_MPFR);
  eitvMPFR_set_ldouble2(a->eitv.MPFR,b,c,NULL);
}
void ap_coeff_set_mpfr(ap_coeff_t* a, mpfr_t b)
{ ap_coeff_set_numMPFR(a,b); }
void ap_coeff_set_mpfr2(ap_coeff_t* a, mpfr_t b, mpfr_t c)
{
  ap_coeff_reinit(a,AP_SCALAR_MPFR);
  eitvMPFR_set_numMPFR2(a->eitv.MPFR,b,c);
}
void ap_coeff_set_int(ap_coeff_t* a, int b)
{
  switch(a->discr){
  case AP_SCALAR_D:
    eitvD_set_int(a->eitv.D,b);
    break;
  case AP_SCALAR_MPQ:
    eitvMPQ_set_int(a->eitv.MPQ,b);
    break;
  case AP_SCALAR_MPFR:
    eitvMPFR_set_int(a->eitv.MPFR,b);
    break;
  }
}
void ap_coeff_set_int2(ap_coeff_t* a, int b, int c)
{
  switch(a->discr){
  case AP_SCALAR_D:
    eitvD_set_int2(a->eitv.D,b,c);
    break;
  case AP_SCALAR_MPQ:
    eitvMPQ_set_int2(a->eitv.MPQ,b,c);
    break;
  case AP_SCALAR_MPFR:
    eitvMPFR_set_int2(a->eitv.MPFR,b,c);
    break;
  }
}
void ap_coeff_set_top(ap_coeff_t* a)
{
  switch(a->discr){
  case AP_SCALAR_D:
    eitvD_set_top(a->eitv.D);
    break;
  case AP_SCALAR_MPQ:
    eitvMPQ_set_top(a->eitv.MPQ);
    break;
  case AP_SCALAR_MPFR:
    eitvMPFR_set_top(a->eitv.MPFR);
    break;
  }
}
bool eitvMPQ_set_ap_coeff(eitvMPQ_t a, ap_coeff_t* b, numinternal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return eitvMPQ_set_eitvD(a,b->eitv.D,intern);
  case AP_SCALAR_MPQ:
    eitvMPQ_set(a,b->eitv.MPQ);
    return true;
  case AP_SCALAR_MPFR:
    return eitvMPQ_set_eitvMPFR(a,b->eitv.MPFR,intern);
  default:
    abort();
  }
}
bool eitvD_set_ap_coeff(eitvD_t a, ap_coeff_t* b, numinternal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    eitvD_set(a,b->eitv.D);
    return true;
  case AP_SCALAR_MPQ:
    return eitvD_set_eitvMPQ(a,b->eitv.MPQ,intern);
  case AP_SCALAR_MPFR:
    return eitvD_set_eitvMPFR(a,b->eitv.MPFR,intern);
  default:
    abort();
  }
}
bool eitvMPFR_set_ap_coeff(eitvMPFR_t a, ap_coeff_t* b, numinternal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return eitvMPFR_set_eitvD(a,b->eitv.D,intern);
  case AP_SCALAR_MPQ:
    return eitvMPFR_set_eitvMPQ(a,b->eitv.MPQ,intern);
  case AP_SCALAR_MPFR:
    eitvMPFR_set(a,b->eitv.MPFR);
    return true;
  default:
    abort();
  }
}


/* ====================================================================== */
/* Tests */
/* ====================================================================== */

bool ap_coeff_zero(ap_coeff_t* a)
{
  switch(a->discr){
  case AP_SCALAR_D:
    return eitvD_is_zero(a->eitv.D);
  case AP_SCALAR_MPQ:
    return eitvMPQ_is_zero(a->eitv.MPQ);
  case AP_SCALAR_MPFR:
    return eitvMPFR_is_zero(a->eitv.MPFR);
  default:
    abort();
  }
}

bool ap_coeff_equal(ap_coeff_t* a, ap_coeff_t* b)
{
  if (a->discr==b->discr){
    switch(a->discr){
    case AP_SCALAR_D:
      return eitvD_is_eq(a->eitv.D,b->eitv.D);
    case AP_SCALAR_MPQ:
      return eitvMPQ_is_eq(a->eitv.MPQ,b->eitv.MPQ);
    case AP_SCALAR_MPFR:
      return eitvMPFR_is_eq(a->eitv.MPFR,b->eitv.MPFR);
    default:
      abort();
    }
  }
  else
    return false;
}

/* ====================================================================== */
/* Other operations */
/* ====================================================================== */

/* Hash */
long ap_coeff_hash(ap_coeff_t* a)
{
  switch(a->discr){
  case AP_SCALAR_D:
    return eitvD_hash(a->eitv.D);
  case AP_SCALAR_MPQ:
    return eitvMPQ_hash(a->eitv.MPQ);
  case AP_SCALAR_MPFR:
    return eitvMPFR_hash(a->eitv.MPFR);
  default:
    abort();
  }
}
