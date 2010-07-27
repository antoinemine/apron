/* -*- mode: c -*- */

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
#include "eitvIl_conv.h"
#include "eitvIll_conv.h"
#include "eitvMPZ_conv.h"
#include "eitvRl_conv.h"
#include "eitvRll_conv.h"
#include "eitvMPQ_conv.h"
#include "eitvD_conv.h"
#include "eitvDl_conv.h"
#include "eitvMPFR_conv.h"

/* ====================================================================== */
/* Basics */
/* ====================================================================== */

void ap_coeff_init(ap_coeff_t a, ap_scalar_discr_t discr)
{
  a->ref = false;
  a->discr = discr;
  EDISPATCH_D_0(discr,a->eitv,alloc);
}

void ap_coeff_init_set(ap_coeff_t a, ap_coeff_t b)
{
  a->ref = false;
  a->discr = b->discr;
  EDISPATCH_D_D(b->discr,a->eitv,alloc_set,b->eitv);
}
void ap_coeff_init_set_D(ap_coeff_t a, eitvD_t eitv)
{
  a->ref = false;
  a->discr = AP_SCALAR_D;
  eitvD_init_set(a->eitv.D,eitv);
} 
void ap_coeff_init_set_MPQ(ap_coeff_t a, eitvMPQ_t eitv)
{
  a->ref = false;
  a->discr = AP_SCALAR_MPQ;
  eitvMPQ_init_set(a->eitv.MPQ,eitv);
} 
void ap_coeff_init_set_MPFR(ap_coeff_t a, eitvMPFR_t eitv)
{
  a->ref = false;
  a->discr = AP_SCALAR_MPFR;
  eitvMPFR_init_set(a->eitv.MPFR,eitv);
} 
void ap_coeff_clear(ap_coeff_t a)
{
  if (a->ref==false){
    EDISPATCH_0_D(a->discr,clear,a->eitv);
  }
}
void ap_coeff_fprint(FILE* stream, ap_coeff_t a)
{
  EDISPATCH_0_ND(a->discr,fprint,stream,a->eitv);
}
void ap_coeff_set_int(ap_coeff_t a, int b)
{
  EDISPATCH_0_DN(a->discr,set_int,a->eitv,b);
}
bool ap_coeff_set(ap_coeff_t a, ap_coeff_t b, numinternal_t intern)
{
  bool res;
  if (a->discr==b->discr){
    EDISPATCH_0_DD(a->discr,set,a->eitv,b->eitv);
    return true;
  }
  else {
    EDISPATCH_N_DNN(a->discr,res,set_ap_coeff,a->eitv,b,intern);
    return res;
  }
}
void ap_coeff_set_top(ap_coeff_t a)
{
  EDISPATCH_0_D(a->discr,set_top,a->eitv);
}
void ap_coeff_set_bottom(ap_coeff_t a)
{
  EDISPATCH_0_D(a->discr,set_bottom,a->eitv);
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

define([[ap_coeff_set_eitv]],
[[bool ap_coeff_set_eitv$1(ap_coeff_t a, eitv$1_t b, numinternal_t intern)
{
  switch(a->discr){
  case AP_SCALAR_D:
    return eitvD_set_eitv$1(a->eitv.D,b,intern);
  case AP_SCALAR_MPQ:
    return eitvMPQ_set_eitv$1(a->eitv.MPQ,b,intern);
  case AP_SCALAR_MPFR:
    return eitvMPFR_set_eitv$1(a->eitv.MPFR,b,intern);
  default:
    abort();
  }
}]])
ap_coeff_set_eitv(Il)
ap_coeff_set_eitv(Ill)
ap_coeff_set_eitv(MPZ)
ap_coeff_set_eitv(Rl)
ap_coeff_set_eitv(Rll)
ap_coeff_set_eitv(Dl)

bool ap_coeff_set_eitvD(ap_coeff_t a, eitvD_t b, numinternal_t intern)
{
  switch (a->discr){
  case AP_SCALAR_D:
    eitvD_set(a->eitv.D,b);
    return true;
  case AP_SCALAR_MPQ:
    return eitvMPQ_set_eitvD(a->eitv.MPQ,b,intern);
  case AP_SCALAR_MPFR:
    return eitvMPFR_set_eitvD(a->eitv.MPFR,b,intern);
  default:
    abort();
  }
}
bool ap_coeff_set_eitvMPQ(ap_coeff_t a, eitvMPQ_t b, numinternal_t intern)
{
  switch (a->discr){
  case AP_SCALAR_D:
    return eitvD_set_eitvMPQ(a->eitv.D,b,intern);
  case AP_SCALAR_MPQ:
    eitvMPQ_set(a->eitv.MPQ,b);
    return true;
  case AP_SCALAR_MPFR:
    return eitvMPFR_set_eitvMPQ(a->eitv.MPFR,b,intern);
  default:
    abort();
  }
}
bool ap_coeff_set_eitvMPFR(ap_coeff_t a, eitvMPFR_t b, numinternal_t intern)
{
  switch (a->discr){
  case AP_SCALAR_D:
    return eitvD_set_eitvMPFR(a->eitv.D,b,intern);
  case AP_SCALAR_MPQ:
    return eitvMPQ_set_eitvMPFR(a->eitv.MPQ,b,intern);
  case AP_SCALAR_MPFR:
    eitvMPFR_set(a->eitv.MPFR,b);
    return true;
  default:
    abort();
  }
}

define([[eitv_set_ap_coeff]],
[[bool eitv$1_set_ap_coeff(eitv$1_t a, ap_coeff_t b, numinternal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return eitv$1_set_eitvD(a,b->eitv.D,intern);
  case AP_SCALAR_MPQ:
    return eitv$1_set_eitvMPQ(a,b->eitv.MPQ,intern);
  case AP_SCALAR_MPFR:
    return eitv$1_set_eitvMPFR(a,b->eitv.MPFR,intern);
  default:
    abort();
  }
}]])
eitv_set_ap_coeff(Il)
eitv_set_ap_coeff(Ill)
eitv_set_ap_coeff(MPZ)
eitv_set_ap_coeff(Rl)
eitv_set_ap_coeff(Rll)
eitv_set_ap_coeff(Dl)

bool eitvMPQ_set_ap_coeff(eitvMPQ_t a, ap_coeff_t b, numinternal_t intern)
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
bool eitvD_set_ap_coeff(eitvD_t a, ap_coeff_t b, numinternal_t intern)
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
bool eitvMPFR_set_ap_coeff(eitvMPFR_t a, ap_coeff_t b, numinternal_t intern)
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

bool ap_coeff_is_point(ap_coeff_t a)
{
  bool res;
  EDISPATCH_N_D(a->discr,res,is_point,a->eitv);
  return res;
}
bool ap_coeff_is_zero(ap_coeff_t a)
{
  bool res;
  EDISPATCH_N_D(a->discr,res,is_zero,a->eitv);
  return res;
}
bool ap_coeff_is_pos(ap_coeff_t a)
{
  bool res;
  EDISPATCH_N_D(a->discr,res,is_pos,a->eitv);
  return res;
}
bool ap_coeff_is_neg(ap_coeff_t a)
{
  bool res;
  EDISPATCH_N_D(a->discr,res,is_neg,a->eitv);
  return res;
}
bool ap_coeff_is_top(ap_coeff_t a)
{
  bool res;
  EDISPATCH_N_D(a->discr,res,is_top,a->eitv);
  return res;
}
bool ap_coeff_equal(ap_coeff_t a, ap_coeff_t b)
{
  bool res;
   if (a->discr==b->discr){
    EDISPATCH_N_DD(a->discr,res,is_eq,a->eitv,b->eitv);
    return res;
  }
  else {
    return false;
  }
}

/* ====================================================================== */
/* Other operations */
/* ====================================================================== */

/* Hash */
long ap_coeff_hash(ap_coeff_t a)
{
  long res;
  EDISPATCH_N_D(a->discr,res,hash,a->eitv);
  return res;
}
