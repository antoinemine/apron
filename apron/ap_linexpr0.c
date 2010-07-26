/* -*- mode: c -*- */

/* ************************************************************************* */
/* ap_linexpr0.c: linear expressions */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "ap_linexpr0.h"
#include "itvD_linexpr_conv.h"
#include "itvMPQ_linexpr_conv.h"
#include "itvMPFR_linexpr_conv.h"
#include <stdarg.h>

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_linexpr0_init(ap_linexpr0_t res, ap_scalar_discr_t discr, size_t size)
{
  res->ref = false;
  res->discr = discr;
  switch (discr){
  case AP_SCALAR_D:
    res->linexpr.D = itvD_linexpr_alloc(size);
    break;
  case AP_SCALAR_MPQ:
    res->linexpr.MPQ = itvMPQ_linexpr_alloc(size);
    break;
  case AP_SCALAR_MPFR:
    res->linexpr.MPFR = itvMPFR_linexpr_alloc(size);
    break;
  default:
    abort();
  }
}
void ap_linexpr0_init_set(ap_linexpr0_t res,ap_linexpr0_t e)
{
  res->ref = false;
  res->discr = e->discr;
  switch (e->discr){
  case AP_SCALAR_D:
    res->linexpr.D = itvD_linexpr_alloc_set(e->linexpr.D);
    break;
  case AP_SCALAR_MPQ:
    res->linexpr.MPQ = itvMPQ_linexpr_alloc_set(e->linexpr.MPQ);
    break;
  case AP_SCALAR_MPFR:
    res->linexpr.MPFR = itvMPFR_linexpr_alloc_set(e->linexpr.MPFR);
    break;
  default:
    abort();
  }
}
void ap_linexpr0_init_set_D(ap_linexpr0_t res, itvD_linexpr_t e)
{
  ap_linexpr0_init(res,AP_SCALAR_D,e->size);
  itvD_linexpr_set(res->linexpr.D,e);
}
void ap_linexpr0_init_set_MPQ(ap_linexpr0_t res, itvMPQ_linexpr_t e)
{
  ap_linexpr0_init(res,AP_SCALAR_MPQ,e->size);
  itvMPQ_linexpr_set(res->linexpr.MPQ,e);
}
void ap_linexpr0_init_set_MPFR(ap_linexpr0_t res, itvMPFR_linexpr_t e)
{
  ap_linexpr0_init(res,AP_SCALAR_MPFR,e->size);
  itvMPFR_linexpr_set(res->linexpr.MPFR,e);
}
void ap_linexpr0_clear(ap_linexpr0_t e)
{
  if (e->ref==false){
    switch (e->discr){
  case AP_SCALAR_D:
    itvD_linexpr_clear(e->linexpr.D);
    break;
  case AP_SCALAR_MPQ:
    itvMPQ_linexpr_clear(e->linexpr.MPQ);
    break;
  case AP_SCALAR_MPFR:
    itvMPFR_linexpr_clear(e->linexpr.MPFR);
    break;
  };
  }
}
void ap_linexpr0_fprint(FILE* stream, ap_linexpr0_t e, char** name_of_dim)
{
  switch (e->discr){
  case AP_SCALAR_D:
    itvD_linexpr_fprint(stream,e->linexpr.D,name_of_dim);
    break;
  case AP_SCALAR_MPQ:
    itvMPQ_linexpr_fprint(stream,e->linexpr.MPQ,name_of_dim);
    break;
  case AP_SCALAR_MPFR:
    itvMPFR_linexpr_fprint(stream,e->linexpr.MPFR,name_of_dim);
    break;
  default:
    abort();
  }
}
void ap_linexpr0_minimize(ap_linexpr0_t a)
{
  switch (a->discr){
  case AP_SCALAR_D:
    itvD_linexpr_minimize(a->linexpr.D);
    break;
  case AP_SCALAR_MPQ:
    itvMPQ_linexpr_minimize(a->linexpr.MPQ);
    break;
  case AP_SCALAR_MPFR:
    itvMPFR_linexpr_minimize(a->linexpr.MPFR);
    break;
  };
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

bool ap_linexpr0_set_itvD_linexpr(ap_linexpr0_t a, itvD_linexpr_t b, numinternal_t intern)
{
  switch (a->discr){
  case AP_SCALAR_D:
    itvD_linexpr_set(a->linexpr.D,b);
    return true;
  case AP_SCALAR_MPQ:
    return itvMPQ_linexpr_set_itvD_linexpr(a->linexpr.MPQ,b,intern);
  case AP_SCALAR_MPFR:
    return itvMPFR_linexpr_set_itvD_linexpr(a->linexpr.MPFR,b,intern);
  default:
    abort();
  }
}
bool ap_linexpr0_set_itvMPQ_linexpr(ap_linexpr0_t a, itvMPQ_linexpr_t b, numinternal_t intern)
{
  switch (a->discr){
  case AP_SCALAR_D:
    return itvD_linexpr_set_itvMPQ_linexpr(a->linexpr.D,b,intern);
  case AP_SCALAR_MPQ:
    itvMPQ_linexpr_set(a->linexpr.MPQ,b);
    return true;
  case AP_SCALAR_MPFR:
    return itvMPFR_linexpr_set_itvMPQ_linexpr(a->linexpr.MPFR,b,intern);
  default:
    abort();
  }
}
bool ap_linexpr0_set_itvMPFR_linexpr(ap_linexpr0_t a, itvMPFR_linexpr_t b, numinternal_t intern)
{
  switch (a->discr){
  case AP_SCALAR_D:
    return itvD_linexpr_set_itvMPFR_linexpr(a->linexpr.D,b,intern);
  case AP_SCALAR_MPQ:
    return itvMPQ_linexpr_set_itvMPFR_linexpr(a->linexpr.MPQ,b,intern);
  case AP_SCALAR_MPFR:
    itvMPFR_linexpr_set(a->linexpr.MPFR,b);
    return true;
  default:
    abort();
  }
}

bool itvMPQ_linexpr_set_ap_linexpr0(itvMPQ_linexpr_t a, ap_linexpr0_t b, numinternal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return itvMPQ_linexpr_set_itvD_linexpr(a,b->linexpr.D,intern);
  case AP_SCALAR_MPQ:
    itvMPQ_linexpr_set(a,b->linexpr.MPQ);
    return true;
  case AP_SCALAR_MPFR:
    return itvMPQ_linexpr_set_itvMPFR_linexpr(a,b->linexpr.MPFR,intern);
  default:
    abort();
  }
}
bool itvD_linexpr_set_ap_linexpr0(itvD_linexpr_t a, ap_linexpr0_t b, numinternal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    itvD_linexpr_set(a,b->linexpr.D);
    return true;
  case AP_SCALAR_MPQ:
    return itvD_linexpr_set_itvMPQ_linexpr(a,b->linexpr.MPQ,intern);
  case AP_SCALAR_MPFR:
    return itvD_linexpr_set_itvMPFR_linexpr(a,b->linexpr.MPFR,intern);
  default:
    abort();
  }
}
bool itvMPFR_linexpr_set_ap_linexpr0(itvMPFR_linexpr_t a, ap_linexpr0_t b, numinternal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return itvMPFR_linexpr_set_itvD_linexpr(a,b->linexpr.D,intern);
  case AP_SCALAR_MPQ:
    return itvMPFR_linexpr_set_itvMPQ_linexpr(a,b->linexpr.MPQ,intern);
  case AP_SCALAR_MPFR:
    itvMPFR_linexpr_set(a,b->linexpr.MPFR);
    return true;
  default:
    abort();
  }
}

/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

/* Get the constant and assign it to coeff with possible conversion */
bool ap_linexpr0_get_cst(ap_coeff_t coeff, ap_linexpr0_t expr, numinternal_t intern)
{
  switch (expr->discr){
  case AP_SCALAR_D:
    return ap_coeff_set_eitvD(coeff,expr->linexpr.D->cst,intern);
  case AP_SCALAR_MPQ:
    return ap_coeff_set_eitvMPQ(coeff,expr->linexpr.MPQ->cst,intern);
  case AP_SCALAR_MPFR:
    return ap_coeff_set_eitvMPFR(coeff,expr->linexpr.MPFR->cst,intern);
  default:
    abort();
  }
}
bool ap_linexpr0_get_coeff(ap_coeff_t coeff, ap_linexpr0_t expr, ap_dim_t dim, numinternal_t intern)
{
  switch (expr->discr){
  case AP_SCALAR_D:
    {
      eitvD_ptr eitv = itvD_linexpr_eitvref(expr->linexpr.D,dim,false);
      if (eitv==NULL) break; else return ap_coeff_set_eitvD(coeff,eitv,intern);
    }
  case AP_SCALAR_MPQ:
    {
      eitvMPQ_ptr eitv = itvMPQ_linexpr_eitvref(expr->linexpr.MPQ,dim,false);
      if (eitv==NULL) break; else return ap_coeff_set_eitvMPQ(coeff,eitv,intern);
    }
  case AP_SCALAR_MPFR:
    {
      eitvMPFR_ptr eitv = itvMPFR_linexpr_eitvref(expr->linexpr.MPFR,dim,false);
      if (eitv==NULL) break; else return ap_coeff_set_eitvMPFR(coeff,eitv,intern);
    }
  default:
    abort();
  }
  ap_coeff_set_int(coeff,0);
  return true;
}
void ap_linexpr0_cstref(ap_coeff_t res, ap_linexpr0_t expr)
{
  switch (expr->discr){
  case AP_SCALAR_D:
    ap_coeff_ref_D(res,expr->linexpr.D->cst);
  case AP_SCALAR_MPQ:
    ap_coeff_ref_MPQ(res,expr->linexpr.MPQ->cst);
  case AP_SCALAR_MPFR:
    ap_coeff_ref_MPFR(res,expr->linexpr.MPFR->cst);
  default:
    abort();
  }
}
void ap_linexpr0_coeffref(ap_coeff_t res, ap_linexpr0_t expr, ap_dim_t dim)
{
  switch (expr->discr){
  case AP_SCALAR_D:
    {
      eitvD_ptr eitv = itvD_linexpr_eitvref(expr->linexpr.D,dim,true);
      ap_coeff_ref_D(res,eitv);
    }
  case AP_SCALAR_MPQ:
    {
      eitvMPQ_ptr eitv = itvMPQ_linexpr_eitvref(expr->linexpr.MPQ,dim,true);
      return ap_coeff_ref_MPQ(res,eitv);
   }
  case AP_SCALAR_MPFR:
    {
      eitvMPFR_ptr eitv = itvMPFR_linexpr_eitvref(expr->linexpr.MPFR,dim,true);
      return ap_coeff_ref_MPFR(res,eitv);
    }
  default:
    abort();
  }
}

bool ap_linexpr0_set_list(numinternal_t intern,
			  ap_linexpr0_t expr, ...)
{
  bool res;
  va_list va;
  va_start(va,expr);
  switch (expr->discr){
  case AP_SCALAR_D:
    res = itvD_linexpr_set_list(intern,expr->linexpr.D,&va);
    break;
  case AP_SCALAR_MPQ:
    res = itvMPQ_linexpr_set_list(intern,expr->linexpr.MPQ,&va);
    break;
  case AP_SCALAR_MPFR:
    res = itvMPFR_linexpr_set_list(intern,expr->linexpr.MPFR,&va);
    break;
  default:
    abort();
  }
  va_end(va);
  return res;
}

/* ====================================================================== */
/* IV. Change of dimensions and permutations */
/* ====================================================================== */

void ap_linexpr0_add_dimensions(ap_linexpr0_t res,
				ap_linexpr0_t expr,
				ap_dimchange_t* dimchange)
{
  if (res->discr!=expr->discr)
    abort();
  switch (expr->discr){
  case AP_SCALAR_D:
    itvD_linexpr_add_dimensions(res->linexpr.D,expr->linexpr.D,dimchange);
    break;
  case AP_SCALAR_MPQ:
    itvMPQ_linexpr_add_dimensions(res->linexpr.MPQ,expr->linexpr.MPQ,dimchange);
    break;
  case AP_SCALAR_MPFR:
    itvMPFR_linexpr_add_dimensions(res->linexpr.MPFR,expr->linexpr.MPFR,dimchange);
    break;
  default:
    abort();
  };
}
void ap_linexpr0_permute_dimensions(ap_linexpr0_t res,
				    ap_linexpr0_t expr,
				    ap_dimperm_t* perm)
{
  if (res->discr!=expr->discr)
    abort();
  switch (expr->discr){
  case AP_SCALAR_D:
    itvD_linexpr_permute_dimensions(res->linexpr.D,expr->linexpr.D,perm);
    break;
  case AP_SCALAR_MPQ:
    itvMPQ_linexpr_permute_dimensions(res->linexpr.MPQ,expr->linexpr.MPQ,perm);
    break;
  case AP_SCALAR_MPFR:
    itvMPFR_linexpr_permute_dimensions(res->linexpr.MPFR,expr->linexpr.MPFR,perm);
    break;
  default:
    abort();
  }
}

/* ====================================================================== */
/* V. Hashing, comparison */
/* ====================================================================== */

/* Induces reduction of the coefficients */

int ap_linexpr0_hash(ap_linexpr0_t expr)
{
  int res;
  switch (expr->discr){
  case AP_SCALAR_D:
    res = itvD_linexpr_hash(expr->linexpr.D);
    break;
  case AP_SCALAR_MPQ:
    res = itvMPQ_linexpr_hash(expr->linexpr.MPQ);
    break;
  case AP_SCALAR_MPFR:
    res = itvMPFR_linexpr_hash(expr->linexpr.MPFR);
    break;
  default:
    abort();
  };
  return res;
}
bool ap_linexpr0_equal(ap_linexpr0_t expr1,
		       ap_linexpr0_t expr2)
{
  bool res;
  if (expr1->discr != expr2->discr)
    res = false;
  else {
    switch (expr1->discr){
  case AP_SCALAR_D:
    res = itvD_linexpr_equal(expr1->linexpr.D,expr2->linexpr.D);
    break;
  case AP_SCALAR_MPQ:
    res = itvMPQ_linexpr_equal(expr1->linexpr.MPQ,expr2->linexpr.MPQ);
    break;
  case AP_SCALAR_MPFR:
    res = itvMPFR_linexpr_equal(expr1->linexpr.MPFR,expr2->linexpr.MPFR);
    break;
  default:
    abort();
  }
      }
  return res;
}
/* Lexicographic ordering, terminating by constant coefficients */
int ap_linexpr0_compare(ap_linexpr0_t expr1,
			ap_linexpr0_t expr2)
{
  int res;
  if (expr1->discr != expr2->discr){
    res = (expr1->discr - expr2->discr);
    res = res>0 ? 3 : -3;
  }
  else {
    switch (expr1->discr){
  case AP_SCALAR_D:
    res = itvD_linexpr_compare(expr1->linexpr.D,expr2->linexpr.D);
    break;
  case AP_SCALAR_MPQ:
    res = itvMPQ_linexpr_compare(expr1->linexpr.MPQ,expr2->linexpr.MPQ);
    break;
  case AP_SCALAR_MPFR:
    res = itvMPFR_linexpr_compare(expr1->linexpr.MPFR,expr2->linexpr.MPFR);
    break;
  default:
    abort();
  }
  }
  return res;
}
