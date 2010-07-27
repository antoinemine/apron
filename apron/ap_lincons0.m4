/* -*- mode: c -*- */

/* ************************************************************************* */
/* ap_lincons0.c: linear constraints and arrays */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "ap_lincons0.h"
#include "itvD_lincons_conv.h"
#include "itvMPQ_lincons_conv.h"
#include "itvMPFR_lincons_conv.h"

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_lincons0_init(ap_lincons0_t res, ap_scalar_discr_t discr, size_t size)
{
  res->ref = false;
  res->discr = discr;
  DISPATCH_D_N(discr,res->lincons,lincons_alloc,size)
}
void ap_lincons0_init_set(ap_lincons0_t res,ap_lincons0_t e)
{
  res->ref = false;
  res->discr = e->discr;
  DISPATCH_D_D(e->discr,res->lincons,lincons_alloc_set,e->lincons)
}
void ap_lincons0_init_set_D(ap_lincons0_t res, itvD_lincons_t e)
{
  ap_lincons0_init(res,AP_SCALAR_D,e->linexpr->size);
  itvD_lincons_set(res->lincons.D,e);
}
void ap_lincons0_init_set_MPQ(ap_lincons0_t res, itvMPQ_lincons_t e)
{
  ap_lincons0_init(res,AP_SCALAR_MPQ,e->linexpr->size);
  itvMPQ_lincons_set(res->lincons.MPQ,e);
}
void ap_lincons0_init_set_MPFR(ap_lincons0_t res, itvMPFR_lincons_t e)
{
  ap_lincons0_init(res,AP_SCALAR_MPFR,e->linexpr->size);
  itvMPFR_lincons_set(res->lincons.MPFR,e);
}
void ap_lincons0_clear(ap_lincons0_t e)
{
  if (e->ref==false){
    DISPATCH_0_D(e->discr,lincons_clear,e->lincons);
  }
}
void ap_lincons0_fprint(FILE* stream, ap_lincons0_t e, char** name_of_dim)
{
  DISPATCH_0_NDN(e->discr,lincons_fprint,stream,e->lincons,name_of_dim)
}
void ap_lincons0_minimize(ap_lincons0_t a)
{
  switch (a->discr){
  case AP_SCALAR_D:
    itvD_linexpr_minimize(a->lincons.D->linexpr);
    break;
  case AP_SCALAR_MPQ:
    itvMPQ_linexpr_minimize(a->lincons.MPQ->linexpr);
    break;
  case AP_SCALAR_MPFR:
    itvMPFR_linexpr_minimize(a->lincons.MPFR->linexpr);
    break;
  };
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

bool ap_lincons0_set_itvD_lincons(ap_lincons0_t a, itvD_lincons_t b, numinternal_t intern)
{
  switch (a->discr){
  case AP_SCALAR_D:
    itvD_lincons_set(a->lincons.D,b);
    return true;
  case AP_SCALAR_MPQ:
    return itvMPQ_lincons_set_itvD_lincons(a->lincons.MPQ,b,intern);
  case AP_SCALAR_MPFR:
    return itvMPFR_lincons_set_itvD_lincons(a->lincons.MPFR,b,intern);
  default:
    abort();
  }
}
bool ap_lincons0_set_itvMPQ_lincons(ap_lincons0_t a, itvMPQ_lincons_t b, numinternal_t intern)
{
  switch (a->discr){
  case AP_SCALAR_D:
    return itvD_lincons_set_itvMPQ_lincons(a->lincons.D,b,intern);
  case AP_SCALAR_MPQ:
    itvMPQ_lincons_set(a->lincons.MPQ,b);
    return true;
  case AP_SCALAR_MPFR:
    return itvMPFR_lincons_set_itvMPQ_lincons(a->lincons.MPFR,b,intern);
  default:
    abort();
  }
}
bool ap_lincons0_set_itvMPFR_lincons(ap_lincons0_t a, itvMPFR_lincons_t b, numinternal_t intern)
{
  switch (a->discr){
  case AP_SCALAR_D:
    return itvD_lincons_set_itvMPFR_lincons(a->lincons.D,b,intern);
  case AP_SCALAR_MPQ:
    return itvMPQ_lincons_set_itvMPFR_lincons(a->lincons.MPQ,b,intern);
  case AP_SCALAR_MPFR:
    itvMPFR_lincons_set(a->lincons.MPFR,b);
    return true;
  default:
    abort();
  }
}

bool itvMPQ_lincons_set_ap_lincons0(itvMPQ_lincons_t a, ap_lincons0_t b, numinternal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return itvMPQ_lincons_set_itvD_lincons(a,b->lincons.D,intern);
  case AP_SCALAR_MPQ:
    itvMPQ_lincons_set(a,b->lincons.MPQ);
    return true;
  case AP_SCALAR_MPFR:
    return itvMPQ_lincons_set_itvMPFR_lincons(a,b->lincons.MPFR,intern);
  default:
    abort();
  }
}
bool itvD_lincons_set_ap_lincons0(itvD_lincons_t a, ap_lincons0_t b, numinternal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    itvD_lincons_set(a,b->lincons.D);
    return true;
  case AP_SCALAR_MPQ:
    return itvD_lincons_set_itvMPQ_lincons(a,b->lincons.MPQ,intern);
  case AP_SCALAR_MPFR:
    return itvD_lincons_set_itvMPFR_lincons(a,b->lincons.MPFR,intern);
  default:
    abort();
  }
}
bool itvMPFR_lincons_set_ap_lincons0(itvMPFR_lincons_t a, ap_lincons0_t b, numinternal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return itvMPFR_lincons_set_itvD_lincons(a,b->lincons.D,intern);
  case AP_SCALAR_MPQ:
    return itvMPFR_lincons_set_itvMPQ_lincons(a,b->lincons.MPQ,intern);
  case AP_SCALAR_MPFR:
    itvMPFR_lincons_set(a,b->lincons.MPFR);
    return true;
  default:
    abort();
  }
}

/* ====================================================================== */
/* III Access */
/* ====================================================================== */

void ap_lincons0_linexpr0ref(ap_linexpr0_t e, ap_lincons0_t c)
{
  e->ref = true;
  e->discr = c->discr;
  switch (e->discr){
  case AP_SCALAR_D:
    e->linexpr.D = c->lincons.D->linexpr;
    break;
  case AP_SCALAR_MPQ:
    e->linexpr.MPQ = c->lincons.MPQ->linexpr;
    break;
  case AP_SCALAR_MPFR:
    e->linexpr.MPFR = c->lincons.MPFR->linexpr;
    break;
  default:
    abort();
  }
}
itvconstyp_t* ap_lincons0_constypref(ap_lincons0_t c)
{
  itvconstyp_t* p;
  switch (c->discr){
  case AP_SCALAR_D:
    p = &c->lincons.D->constyp;
    break;
  case AP_SCALAR_MPQ:
    p = &c->lincons.MPQ->constyp;
    break;
  case AP_SCALAR_MPFR:
    p = &c->lincons.MPFR->constyp;
    break;
  default:
    abort();
  }
  return p;
}
mpq_ptr ap_lincons0_mpqref(ap_lincons0_t c)
{
  mpq_ptr p;
  switch (c->discr){
  case AP_SCALAR_D:
    p = c->lincons.D->mpq;
    break;
  case AP_SCALAR_MPQ:
    p = c->lincons.MPQ->mpq;
    break;
  case AP_SCALAR_MPFR:
    p = c->lincons.MPFR->mpq;
    break;
  default:
    abort();
  }
  return p;
}

/* ====================================================================== */
/* IV. Change of dimensions and permutations */
/* ====================================================================== */

void ap_lincons0_add_dimensions(ap_lincons0_t res,
				ap_lincons0_t expr,
				ap_dimchange_t* dimchange)
{
  if (res->discr!=expr->discr)
    abort();
  DISPATCH_0_DDN(expr->discr,lincons_add_dimensions,res->lincons,expr->lincons,dimchange);
}
void ap_lincons0_permute_dimensions(ap_lincons0_t res,
				    ap_lincons0_t expr,
				    ap_dimperm_t* perm)
{
  if (res->discr!=expr->discr)
    abort();
  DISPATCH_0_DDN(expr->discr,lincons_permute_dimensions,res->lincons,expr->lincons,perm)
}

/* ====================================================================== */
/* V. Hashing, comparison */
/* ====================================================================== */

/* Induces reduction of the coefficients */

int ap_lincons0_hash(ap_lincons0_t expr)
{
  int res;
  DISPATCH_N_D(expr->discr,res,lincons_hash,expr->lincons);
  return res;
}
bool ap_lincons0_equal(ap_lincons0_t expr1,
		       ap_lincons0_t expr2)
{
  bool res;
  if (expr1->discr != expr2->discr)
    res = false;
  else {
    DISPATCH_N_DD(expr1->discr,res,lincons_equal,expr1->lincons,expr2->lincons)
      }
  return res;
}
/* Lexicographic ordering, terminating by constant coefficients */
int ap_lincons0_compare(ap_lincons0_t expr1,
			ap_lincons0_t expr2)
{
  int res;
  if (expr1->discr != expr2->discr){
    res = (expr1->discr - expr2->discr);
    res = res>0 ? 3 : -3;
  }
  else {
    DISPATCH_N_DD(expr1->discr,res,lincons_compare,expr1->lincons,expr2->lincons)
  }
  return res;
}
