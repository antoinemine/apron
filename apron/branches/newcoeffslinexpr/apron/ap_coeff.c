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
  switch (discr){
  case AP_SCALAR_D:
    a->eitv.D = eitvD_alloc();
    break;
  case AP_SCALAR_MPQ:
    a->eitv.MPQ = eitvMPQ_alloc();
    break;
  case AP_SCALAR_MPFR:
    a->eitv.MPFR = eitvMPFR_alloc();
    break;
  default:
    abort();
  };
}

void ap_coeff_init_set(ap_coeff_t a, ap_coeff_t b)
{
  a->ref = false;
  a->discr = b->discr;
  switch (b->discr){
  case AP_SCALAR_D:
    a->eitv.D = eitvD_alloc_set(b->eitv.D);
    break;
  case AP_SCALAR_MPQ:
    a->eitv.MPQ = eitvMPQ_alloc_set(b->eitv.MPQ);
    break;
  case AP_SCALAR_MPFR:
    a->eitv.MPFR = eitvMPFR_alloc_set(b->eitv.MPFR);
    break;
  default:
    abort();
  };
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
    switch (a->discr){
  case AP_SCALAR_D:
    eitvD_clear(a->eitv.D);
    break;
  case AP_SCALAR_MPQ:
    eitvMPQ_clear(a->eitv.MPQ);
    break;
  case AP_SCALAR_MPFR:
    eitvMPFR_clear(a->eitv.MPFR);
    break;
  };
  }
}
void ap_coeff_fprint(FILE* stream, ap_coeff_t a)
{
  switch (a->discr){
  case AP_SCALAR_D:
    eitvD_fprint(stream,a->eitv.D);
    break;
  case AP_SCALAR_MPQ:
    eitvMPQ_fprint(stream,a->eitv.MPQ);
    break;
  case AP_SCALAR_MPFR:
    eitvMPFR_fprint(stream,a->eitv.MPFR);
    break;
  };
}
void ap_coeff_set_int(ap_coeff_t a, int b)
{
  switch (a->discr){
  case AP_SCALAR_D:
    eitvD_set_int(a->eitv.D,b);
    break;
  case AP_SCALAR_MPQ:
    eitvMPQ_set_int(a->eitv.MPQ,b);
    break;
  case AP_SCALAR_MPFR:
    eitvMPFR_set_int(a->eitv.MPFR,b);
    break;
  default:
    abort();
  };
}
bool ap_coeff_set(ap_coeff_t a, ap_coeff_t b, numinternal_t intern)
{
  bool res;
  if (a->discr==b->discr){
    switch (a->discr){
  case AP_SCALAR_D:
    eitvD_set(a->eitv.D,b->eitv.D);
    break;
  case AP_SCALAR_MPQ:
    eitvMPQ_set(a->eitv.MPQ,b->eitv.MPQ);
    break;
  case AP_SCALAR_MPFR:
    eitvMPFR_set(a->eitv.MPFR,b->eitv.MPFR);
    break;
  default:
    abort();
  };
    return true;
  }
  else {
    switch (a->discr){
  case AP_SCALAR_D:
    res = eitvD_set_ap_coeff(a->eitv.D,b,intern);
    break;
  case AP_SCALAR_MPQ:
    res = eitvMPQ_set_ap_coeff(a->eitv.MPQ,b,intern);
    break;
  case AP_SCALAR_MPFR:
    res = eitvMPFR_set_ap_coeff(a->eitv.MPFR,b,intern);
    break;
  default:
    abort();
  };
    return res;
  }
}
void ap_coeff_set_top(ap_coeff_t a)
{
  switch (a->discr){
  case AP_SCALAR_D:
    eitvD_set_top(a->eitv.D);
    break;
  case AP_SCALAR_MPQ:
    eitvMPQ_set_top(a->eitv.MPQ);
    break;
  case AP_SCALAR_MPFR:
    eitvMPFR_set_top(a->eitv.MPFR);
    break;
  };
}
void ap_coeff_set_bottom(ap_coeff_t a)
{
  switch (a->discr){
  case AP_SCALAR_D:
    eitvD_set_bottom(a->eitv.D);
    break;
  case AP_SCALAR_MPQ:
    eitvMPQ_set_bottom(a->eitv.MPQ);
    break;
  case AP_SCALAR_MPFR:
    eitvMPFR_set_bottom(a->eitv.MPFR);
    break;
  };
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */


bool ap_coeff_set_eitvIl(ap_coeff_t a, eitvIl_t b, numinternal_t intern)
{
  switch(a->discr){
  case AP_SCALAR_D:
    return eitvD_set_eitvIl(a->eitv.D,b,intern);
  case AP_SCALAR_MPQ:
    return eitvMPQ_set_eitvIl(a->eitv.MPQ,b,intern);
  case AP_SCALAR_MPFR:
    return eitvMPFR_set_eitvIl(a->eitv.MPFR,b,intern);
  default:
    abort();
  }
}
bool ap_coeff_set_eitvIll(ap_coeff_t a, eitvIll_t b, numinternal_t intern)
{
  switch(a->discr){
  case AP_SCALAR_D:
    return eitvD_set_eitvIll(a->eitv.D,b,intern);
  case AP_SCALAR_MPQ:
    return eitvMPQ_set_eitvIll(a->eitv.MPQ,b,intern);
  case AP_SCALAR_MPFR:
    return eitvMPFR_set_eitvIll(a->eitv.MPFR,b,intern);
  default:
    abort();
  }
}
bool ap_coeff_set_eitvMPZ(ap_coeff_t a, eitvMPZ_t b, numinternal_t intern)
{
  switch(a->discr){
  case AP_SCALAR_D:
    return eitvD_set_eitvMPZ(a->eitv.D,b,intern);
  case AP_SCALAR_MPQ:
    return eitvMPQ_set_eitvMPZ(a->eitv.MPQ,b,intern);
  case AP_SCALAR_MPFR:
    return eitvMPFR_set_eitvMPZ(a->eitv.MPFR,b,intern);
  default:
    abort();
  }
}
bool ap_coeff_set_eitvRl(ap_coeff_t a, eitvRl_t b, numinternal_t intern)
{
  switch(a->discr){
  case AP_SCALAR_D:
    return eitvD_set_eitvRl(a->eitv.D,b,intern);
  case AP_SCALAR_MPQ:
    return eitvMPQ_set_eitvRl(a->eitv.MPQ,b,intern);
  case AP_SCALAR_MPFR:
    return eitvMPFR_set_eitvRl(a->eitv.MPFR,b,intern);
  default:
    abort();
  }
}
bool ap_coeff_set_eitvRll(ap_coeff_t a, eitvRll_t b, numinternal_t intern)
{
  switch(a->discr){
  case AP_SCALAR_D:
    return eitvD_set_eitvRll(a->eitv.D,b,intern);
  case AP_SCALAR_MPQ:
    return eitvMPQ_set_eitvRll(a->eitv.MPQ,b,intern);
  case AP_SCALAR_MPFR:
    return eitvMPFR_set_eitvRll(a->eitv.MPFR,b,intern);
  default:
    abort();
  }
}
bool ap_coeff_set_eitvDl(ap_coeff_t a, eitvDl_t b, numinternal_t intern)
{
  switch(a->discr){
  case AP_SCALAR_D:
    return eitvD_set_eitvDl(a->eitv.D,b,intern);
  case AP_SCALAR_MPQ:
    return eitvMPQ_set_eitvDl(a->eitv.MPQ,b,intern);
  case AP_SCALAR_MPFR:
    return eitvMPFR_set_eitvDl(a->eitv.MPFR,b,intern);
  default:
    abort();
  }
}

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


bool eitvIl_set_ap_coeff(eitvIl_t a, ap_coeff_t b, numinternal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return eitvIl_set_eitvD(a,b->eitv.D,intern);
  case AP_SCALAR_MPQ:
    return eitvIl_set_eitvMPQ(a,b->eitv.MPQ,intern);
  case AP_SCALAR_MPFR:
    return eitvIl_set_eitvMPFR(a,b->eitv.MPFR,intern);
  default:
    abort();
  }
}
bool eitvIll_set_ap_coeff(eitvIll_t a, ap_coeff_t b, numinternal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return eitvIll_set_eitvD(a,b->eitv.D,intern);
  case AP_SCALAR_MPQ:
    return eitvIll_set_eitvMPQ(a,b->eitv.MPQ,intern);
  case AP_SCALAR_MPFR:
    return eitvIll_set_eitvMPFR(a,b->eitv.MPFR,intern);
  default:
    abort();
  }
}
bool eitvMPZ_set_ap_coeff(eitvMPZ_t a, ap_coeff_t b, numinternal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return eitvMPZ_set_eitvD(a,b->eitv.D,intern);
  case AP_SCALAR_MPQ:
    return eitvMPZ_set_eitvMPQ(a,b->eitv.MPQ,intern);
  case AP_SCALAR_MPFR:
    return eitvMPZ_set_eitvMPFR(a,b->eitv.MPFR,intern);
  default:
    abort();
  }
}
bool eitvRl_set_ap_coeff(eitvRl_t a, ap_coeff_t b, numinternal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return eitvRl_set_eitvD(a,b->eitv.D,intern);
  case AP_SCALAR_MPQ:
    return eitvRl_set_eitvMPQ(a,b->eitv.MPQ,intern);
  case AP_SCALAR_MPFR:
    return eitvRl_set_eitvMPFR(a,b->eitv.MPFR,intern);
  default:
    abort();
  }
}
bool eitvRll_set_ap_coeff(eitvRll_t a, ap_coeff_t b, numinternal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return eitvRll_set_eitvD(a,b->eitv.D,intern);
  case AP_SCALAR_MPQ:
    return eitvRll_set_eitvMPQ(a,b->eitv.MPQ,intern);
  case AP_SCALAR_MPFR:
    return eitvRll_set_eitvMPFR(a,b->eitv.MPFR,intern);
  default:
    abort();
  }
}
bool eitvDl_set_ap_coeff(eitvDl_t a, ap_coeff_t b, numinternal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return eitvDl_set_eitvD(a,b->eitv.D,intern);
  case AP_SCALAR_MPQ:
    return eitvDl_set_eitvMPQ(a,b->eitv.MPQ,intern);
  case AP_SCALAR_MPFR:
    return eitvDl_set_eitvMPFR(a,b->eitv.MPFR,intern);
  default:
    abort();
  }
}

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
  switch (a->discr){
  case AP_SCALAR_D:
    res = eitvD_is_point(a->eitv.D);
    break;
  case AP_SCALAR_MPQ:
    res = eitvMPQ_is_point(a->eitv.MPQ);
    break;
  case AP_SCALAR_MPFR:
    res = eitvMPFR_is_point(a->eitv.MPFR);
    break;
  default:
    abort();
  };
  return res;
}
bool ap_coeff_is_zero(ap_coeff_t a)
{
  bool res;
  switch (a->discr){
  case AP_SCALAR_D:
    res = eitvD_is_zero(a->eitv.D);
    break;
  case AP_SCALAR_MPQ:
    res = eitvMPQ_is_zero(a->eitv.MPQ);
    break;
  case AP_SCALAR_MPFR:
    res = eitvMPFR_is_zero(a->eitv.MPFR);
    break;
  default:
    abort();
  };
  return res;
}
bool ap_coeff_is_pos(ap_coeff_t a)
{
  bool res;
  switch (a->discr){
  case AP_SCALAR_D:
    res = eitvD_is_pos(a->eitv.D);
    break;
  case AP_SCALAR_MPQ:
    res = eitvMPQ_is_pos(a->eitv.MPQ);
    break;
  case AP_SCALAR_MPFR:
    res = eitvMPFR_is_pos(a->eitv.MPFR);
    break;
  default:
    abort();
  };
  return res;
}
bool ap_coeff_is_neg(ap_coeff_t a)
{
  bool res;
  switch (a->discr){
  case AP_SCALAR_D:
    res = eitvD_is_neg(a->eitv.D);
    break;
  case AP_SCALAR_MPQ:
    res = eitvMPQ_is_neg(a->eitv.MPQ);
    break;
  case AP_SCALAR_MPFR:
    res = eitvMPFR_is_neg(a->eitv.MPFR);
    break;
  default:
    abort();
  };
  return res;
}
bool ap_coeff_is_top(ap_coeff_t a)
{
  bool res;
  switch (a->discr){
  case AP_SCALAR_D:
    res = eitvD_is_top(a->eitv.D);
    break;
  case AP_SCALAR_MPQ:
    res = eitvMPQ_is_top(a->eitv.MPQ);
    break;
  case AP_SCALAR_MPFR:
    res = eitvMPFR_is_top(a->eitv.MPFR);
    break;
  default:
    abort();
  };
  return res;
}
bool ap_coeff_equal(ap_coeff_t a, ap_coeff_t b)
{
  bool res;
   if (a->discr==b->discr){
    switch (a->discr){
  case AP_SCALAR_D:
    res = eitvD_is_eq(a->eitv.D,b->eitv.D);
    break;
  case AP_SCALAR_MPQ:
    res = eitvMPQ_is_eq(a->eitv.MPQ,b->eitv.MPQ);
    break;
  case AP_SCALAR_MPFR:
    res = eitvMPFR_is_eq(a->eitv.MPFR,b->eitv.MPFR);
    break;
  default:
    abort();
  };
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
  switch (a->discr){
  case AP_SCALAR_D:
    res = eitvD_hash(a->eitv.D);
    break;
  case AP_SCALAR_MPQ:
    res = eitvMPQ_hash(a->eitv.MPQ);
    break;
  case AP_SCALAR_MPFR:
    res = eitvMPFR_hash(a->eitv.MPFR);
    break;
  default:
    abort();
  };
  return res;
}
