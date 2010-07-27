/* -*- mode: c -*- */

/* ************************************************************************* */
/* ap_lincons0_array.c: linear constraints and arrays */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "ap_lincons0_array.h"
#include "itvD_lincons_conv.h"
#include "itvMPQ_lincons_conv.h"
#include "itvMPFR_lincons_conv.h"

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_lincons0_array_init(ap_lincons0_array_t a, ap_scalar_discr_t discr, size_t size)
{
  a->ref = false;
  a->discr = discr;
  DISPATCH_D_N(discr,a->lincons_array,lincons_array_alloc,size)
}
void ap_lincons0_array_init_set(ap_lincons0_array_t a,ap_lincons0_array_t b)
{
  a->ref = false;
  a->discr = b->discr;
  DISPATCH_D_D(a->discr,a->lincons_array,lincons_array_alloc_set,b->lincons_array)
}
void ap_lincons0_array_minimize(ap_lincons0_array_t a)
{
  DISPATCH_0_D(a->discr,lincons_array_minimize,a->lincons_array)
}
void ap_lincons0_array_clear(ap_lincons0_array_t a)
{
  DISPATCH_0_D(a->discr,lincons_array_clear,a->lincons_array)
}

void ap_lincons0_array_fprint(FILE* stream, ap_lincons0_array_t e, char** name_of_dim)
{
  DISPATCH_0_NDN(e->discr,lincons_array_fprint,stream,e->lincons_array,name_of_dim)
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

bool ap_lincons0_array_set_itvD_lincons_array(ap_lincons0_array_t a, itvD_lincons_array_t b, numinternal_t intern)
{
  switch (a->discr){
  case AP_SCALAR_D:
    itvD_lincons_array_set(a->lincons_array.D,b);
    return true;
  case AP_SCALAR_MPQ:
    return itvMPQ_lincons_array_set_itvD_lincons_array(a->lincons_array.MPQ,b,intern);
  case AP_SCALAR_MPFR:
    return itvMPFR_lincons_array_set_itvD_lincons_array(a->lincons_array.MPFR,b,intern);
  default:
    abort();
  }
}
bool ap_lincons0_array_set_itvMPQ_lincons_array(ap_lincons0_array_t a, itvMPQ_lincons_array_t b, numinternal_t intern)
{
  switch (a->discr){
  case AP_SCALAR_D:
    return itvD_lincons_array_set_itvMPQ_lincons_array(a->lincons_array.D,b,intern);
  case AP_SCALAR_MPQ:
    itvMPQ_lincons_array_set(a->lincons_array.MPQ,b);
    return true;
  case AP_SCALAR_MPFR:
    return itvMPFR_lincons_array_set_itvMPQ_lincons_array(a->lincons_array.MPFR,b,intern);
  default:
    abort();
  }
}
bool ap_lincons0_array_set_itvMPFR_lincons_array(ap_lincons0_array_t a, itvMPFR_lincons_array_t b, numinternal_t intern)
{
  switch (a->discr){
  case AP_SCALAR_D:
    return itvD_lincons_array_set_itvMPFR_lincons_array(a->lincons_array.D,b,intern);
  case AP_SCALAR_MPQ:
    return itvMPQ_lincons_array_set_itvMPFR_lincons_array(a->lincons_array.MPQ,b,intern);
  case AP_SCALAR_MPFR:
    itvMPFR_lincons_array_set(a->lincons_array.MPFR,b);
    return true;
  default:
    abort();
  }
}

bool itvMPQ_lincons_array_set_ap_lincons0_array(itvMPQ_lincons_array_t a, ap_lincons0_array_t b, numinternal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return itvMPQ_lincons_array_set_itvD_lincons_array(a,b->lincons_array.D,intern);
  case AP_SCALAR_MPQ:
    itvMPQ_lincons_array_set(a,b->lincons_array.MPQ);
    return true;
  case AP_SCALAR_MPFR:
    return itvMPQ_lincons_array_set_itvMPFR_lincons_array(a,b->lincons_array.MPFR,intern);
  default:
    abort();
  }
}
bool itvD_lincons_array_set_ap_lincons0_array(itvD_lincons_array_t a, ap_lincons0_array_t b, numinternal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    itvD_lincons_array_set(a,b->lincons_array.D);
    return true;
  case AP_SCALAR_MPQ:
    return itvD_lincons_array_set_itvMPQ_lincons_array(a,b->lincons_array.MPQ,intern);
  case AP_SCALAR_MPFR:
    return itvD_lincons_array_set_itvMPFR_lincons_array(a,b->lincons_array.MPFR,intern);
  default:
    abort();
  }
}
bool itvMPFR_lincons_array_set_ap_lincons0_array(itvMPFR_lincons_array_t a, ap_lincons0_array_t b, numinternal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return itvMPFR_lincons_array_set_itvD_lincons_array(a,b->lincons_array.D,intern);
  case AP_SCALAR_MPQ:
    return itvMPFR_lincons_array_set_itvMPQ_lincons_array(a,b->lincons_array.MPQ,intern);
  case AP_SCALAR_MPFR:
    itvMPFR_lincons_array_set(a,b->lincons_array.MPFR);
    return true;
  default:
    abort();
  }
}

/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

itvlinexpr_type_t ap_lincons0_array_type(ap_lincons0_array_t p)
{
  itvlinexpr_type_t res;
  DISPATCH_N_D(p->discr,res,lincons_array_type,p->lincons_array);
  return res;
}
bool ap_lincons0_array_is_linear(ap_lincons0_array_t p)
{
  bool res;
  DISPATCH_N_D(p->discr,res,lincons_array_is_linear,p->lincons_array);
  return res;
}
bool ap_lincons0_array_is_quasilinear(ap_lincons0_array_t p)
{
  bool res;
  DISPATCH_N_D(p->discr,res,lincons_array_is_quasilinear,p->lincons_array);
  return res;
}

void ap_lincons0_array_ref_index(ap_lincons0_t a, ap_lincons0_array_t p, size_t i)
{
  switch (p->discr){
  case AP_SCALAR_D:
    ap_lincons0_ref_D(a,p->lincons_array.D->p[i]);
    break;
  case AP_SCALAR_MPQ:
    ap_lincons0_ref_MPQ(a,p->lincons_array.MPQ->p[i]);
    break;
  case AP_SCALAR_MPFR:
    ap_lincons0_ref_MPFR(a,p->lincons_array.MPFR->p[i]);
    break;
  }
}

bool ap_lincons0_array_get_index(ap_lincons0_t a, ap_lincons0_array_t p, size_t i, numinternal_t intern)
{
  ap_lincons0_t ref;
  ap_lincons0_array_ref_index(ref,p,i);
  return ap_lincons0_set(a,ref,intern);
}
bool ap_lincons0_array_set_index(ap_lincons0_array_t p, size_t i, ap_lincons0_t a, numinternal_t intern)
{
  ap_lincons0_t ref;
  ap_lincons0_array_ref_index(ref,p,i);
  return ap_lincons0_set(ref,a,intern);
}
/* ====================================================================== */
/* IV. Change of dimensions and permutations */
/* ====================================================================== */

void ap_lincons0_array_add_dimensions(ap_lincons0_array_t res,
				ap_lincons0_array_t expr,
				ap_dimchange_t* dimchange)
{
  if (res->discr!=expr->discr)
    abort();
  DISPATCH_0_DDN(expr->discr,lincons_array_add_dimensions,res->lincons_array,expr->lincons_array,dimchange);
}
void ap_lincons0_array_permute_dimensions(ap_lincons0_array_t res,
				    ap_lincons0_array_t expr,
				    ap_dimperm_t* perm)
{
  if (res->discr!=expr->discr)
    abort();
  DISPATCH_0_DDN(expr->discr,lincons_array_permute_dimensions,res->lincons_array,expr->lincons_array,perm)
}
