/* -*- mode: c -*- */

/* ************************************************************************* */
/* ap_linexpr0.c: linear expressions */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "ap_linexpr0.h"
#include <stdarg.h>

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_linexpr0_init(ap_linexpr0_t* res, ap_scalar_discr_t discr, size_t size)
{
  res->discr = discr;
  res->ref = false;
  DISPATCH_D_N(discr,res->linexpr,linexpr_alloc,size)
}
void ap_linexpr0_init_set(ap_linexpr0_t* res,ap_linexpr0_t* e)
{
  res->discr = e->discr;
  res->ref = false;
  DISPATCH_D_D(e->discr,res->linexpr,linexpr_alloc_set,e->linexpr)
}
void ap_linexpr0_set(ap_linexpr0_t* res, ap_linexpr0_t* e)
{
  if (res->discr == e->discr){
    DISPATCH_0_DD(e->discr,linexpr_set,res->linexpr,e->linexpr)
  }
  else {
    ap_linexpr0_clear(res);
    ap_linexpr0_init_set(res,e);
  }
}

void ap_linexpr0_clear(ap_linexpr0_t* e)
{
  if (e->ref==false){
    DISPATCH_0_D(e->discr,linexpr_clear,e->linexpr);
  }
}

ap_linexpr0_t ap_linexpr0_make(ap_scalar_discr_t discr)
{
  ap_linexpr0_t res;
  ap_linexpr0_init(&res,discr);
  return res;
}
ap_linexpr0_t ap_linexpr0_make_D(itvD_linexpr_t e)
{
  ap_linexpr0_t res;
  res.discr = AP_SCALAR_D;
  res.ref = false;
  res.linexpr.D = itvD_linexpr_alloc_set(e);
  return res;
};
ap_linexpr0_t ap_linexpr0_make_MPQ(itvMPQ_linexpr_t e)
{
  ap_linexpr0_t res;
  res.discr = AP_SCALAR_MPQ;
  res.ref = false;
  res.linexpr.MPQ = itvMPQ_linexpr_alloc_set(e);
  return res;
};
ap_linexpr0_t ap_linexpr0_make_MPFR(itvMPFR_linexpr_t e)
{
  ap_linexpr0_t res;
  res.discr = AP_SCALAR_MPFR;
  res.ref = false;
  res.linexpr.MPFR = itvMPFR_linexpr_alloc_set(e);
  return res;
};
ap_linexpr0_t ap_linexpr0_copy(ap_linexpr0_t* e)
{
  ap_linexpr0_t res;
  ap_linexpr0_init_set(&res,e);
  return res;
}

void ap_linexpr0_fprint(FILE* stream, ap_linexpr0_t* e, char** name_of_dim)
{
  DISPATCH_0_NDN(e->discr,linexpr_fprint,stream,e->linexpr,name_of_dim)
}
void ap_linexpr0_array_fprint(FILE* stream, ap_linexpr0_array_t* tab, char** name_of_dim)
{
  DISPATCH_0_NDNN(tab->discr,linexpr_array_fprint,stream,tab->p,tab->size,name_of_dim)
}

/* ====================================================================== */
/* II. Access */
/* ====================================================================== */

/* Get the constant and assign it to coeff with possible conversion */
bool ap_linexpr0_get_cst(ap_coeff_t* coeff, ap_linexpr0_t* expr, numinternal_t intern)
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
bool ap_linexpr0_get_coeff(ap_coeff_t* coeff, ap_linexpr0_t* expr, ap_dim_t dim, numinternal_t intern)
{
  switch (expr->discr){
  case AP_SCALAR_D:
    {
      eitvD_ptr eitv = itvD_linexpr_eitvref(expr->linexpr.D,dim);
      return ap_coeff_set_eitvD(coeff,eitv,intern);
    }
  case AP_SCALAR_MPQ:
    {
      eitvMPQ_ptr eitv = itvMPQ_linexpr_eitvref(expr->linexpr.MPQ,dim);
      return ap_coeff_set_eitvMPQ(coeff,eitv,intern);
    }
  case AP_SCALAR_MPFR:
    {
      eitvMPFR_ptr eitv = itvMPFR_linexpr_eitvref(expr->linexpr.MPFR,dim);
      return ap_coeff_set_eitvMPFR(coeff,eitv,intern);
    }
  default:
    abort();
  }
}
ap_coeff_t ap_linexpr0_cstref(ap_linexpr0_t* expr)
{
  switch (expr->discr){
  case AP_SCALAR_D:
    return ap_coeff_ref_D(expr->linexpr.D->cst);
  case AP_SCALAR_MPQ:
    return ap_coeff_ref_MPQ(expr->linexpr.MPQ->cst);
  case AP_SCALAR_MPFR:
    return ap_coeff_ref_MPFR(expr->linexpr.MPFR->cst);
  default:
    abort();
  }
}
ap_coeff_t ap_linexpr0_coeffref(ap_linexpr0_t* expr, ap_dim_t dim)
{
  switch (expr->discr){
  case AP_SCALAR_D:
    {
      eitvD_ptr eitv = itvD_linexpr_eitvref(expr->linexpr.D,dim);
      return ap_coeff_ref_D(expr->linexpr.D->cst);
    }
  case AP_SCALAR_MPQ:
    {
      eitvMPQ_ptr eitv = itvMPQ_linexpr_eitvref(expr->linexpr.MPQ,dim);
      return ap_coeff_ref_MPQ(expr->linexpr.MPQ->cst);
   }
  case AP_SCALAR_MPFR:
    {
      eitvMPFR_ptr eitv = itvMPFR_linexpr_eitvref(expr->linexpr.MPFR,dim);
      return ap_coeff_ref_MPFR(expr->linexpr.MPFR->cst);
    }
  default:
    abort();
  }
}

bool ap_linexpr0_set_list(numinternal_t intern,
			  ap_linexpr0_t* expr, ...)
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
/* III. Change of dimensions and permutations */
/* ====================================================================== */

void ap_linexpr0_add_dimensions_with(ap_linexpr0_t* expr,
				     ap_dimchange_t* dimchange)
{
  DISPATCH_0_DDN(expr->discr,linexpr_add_dimensions,expr->linexpr,expr->linexpr,dimchange)
}
ap_linexpr0_t ap_linexpr0_add_dimensions(ap_linexpr0_t* expr,
					 ap_dimchange_t* dimchange)
{
  ap_linexpr0_t res;
  ap_linexpr0_init(&res,expr->discr);
  DISPATCH_0_DDN(expr->discr,linexpr_add_dimensions,res.linexpr,expr->linexpr,dimchange)
  return res;
}
void ap_linexpr0_permute_dimensions_with(ap_linexpr0_t* expr,
					 ap_dimperm_t* perm)
{
  DISPATCH_0_DDN(expr->discr,linexpr_permute_dimensions,expr->linexpr,expr->linexpr,perm)
}

ap_linexpr0_t ap_linexpr0_permute_dimensions(ap_linexpr0_t* expr,
					     ap_dimperm_t* perm)
{
  ap_linexpr0_t res;
  ap_linexpr0_init(&res,expr->discr);
  DISPATCH_0_DDN(expr->discr,linexpr_permute_dimensions,res.linexpr,expr->linexpr,perm)
  return res;
}

/* ====================================================================== */
/* IV. Hashing, comparison */
/* ====================================================================== */

/* Induces reduction of the coefficients */

int ap_linexpr0_hash(ap_linexpr0_t* expr)
{
  int res;
  switch (expr->discr){
  case AP_SCALAR_D:
    res = itvD_linexpr_hash(expr->linexpr.D);
    break;
  case AP_SCALAR_MPQ:
    res = 1 + itvMPQ_linexpr_hash(expr->linexpr.MPQ);
    break;
  case AP_SCALAR_MPFR:
    res = 2 + itvMPFR_linexpr_hash(expr->linexpr.MPFR);
    break;
  default:
    abort();
  }
  return res;
}
bool ap_linexpr0_equal(ap_linexpr0_t* expr1,
		       ap_linexpr0_t* expr2)
{
  bool res;
  if (expr1->discr != expr2->discr)
    res = false;
  else {
    DISPATCH_N_DD(expr1->discr,res,linexpr_equal,expr1->linexpr,expr2->linexpr)
  }
  return res;
}
/* Lexicographic ordering, terminating by constant coefficients */
int ap_linexpr0_compare(ap_linexpr0_t* expr1,
			ap_linexpr0_t* expr2)
{
  int res;
  if (expr1->discr != expr2->discr){
    res = (expr1->discr - expr2->discr);
    res = res>0 ? 3 : -3;
  }
  else {
    DISPATCH_N_DD(expr1->discr,res,linexpr_compare,expr1->linexpr,expr2->linexpr)
  }
  return res;
}
