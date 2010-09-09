/* ************************************************************************* */
/* ap_linyyy0.c: linear expressions */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "ap_linyyy0.h"
#include "ap_linexprconsgenD_conv.h"
#include "ap_linexprconsgenMPQ_conv.h"
#include "ap_linexprconsgenMPFR_conv.h"
#include <stdarg.h>

#define _AP_linyyy0_MARK_

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_linyyy0_init(ap_linyyy0_t res, ap_scalar_discr_t discr, size_t size)
{
  res->discr = discr;
  SWITCH(discr)
    res->linyyy.XXX = ap_linyyyXXX_alloc(size);
  ENDSWITCH
}
void ap_linyyy0_init_set(ap_linyyy0_t res,ap_linyyy0_t e)
{
  res->discr = e->discr;
  SWITCH(e->discr)
    res->linyyy.XXX = ap_linyyyXXX_alloc_set(e->linyyy.XXX);
  ENDSWITCH
}
MACRO_MAINZ
void ap_linyyy0_init_set_linyyyZZZ(ap_linyyy0_t res, ap_linyyyZZZ_t e)
{
  ap_linyyy0_init(res,AP_SCALAR_ZZZ,0);
  ap_linyyyZZZ_set(res->linyyy.ZZZ,e);
}
ENDMACRO
void ap_linyyy0_clear(ap_linyyy0_t e)
{
  SWITCH(e->discr)
    ap_linyyyXXX_clear(e->linyyy.XXX);
  ENDSWITCH
}
void ap_linyyy0_fprint(FILE* stream, ap_linyyy0_t e, char** name_of_dim)
{
  SWITCH(e->discr){
    ap_linyyyXXX_fprint(stream,e->linyyy.XXX,name_of_dim);
  ENDSWITCH
}
void ap_linyyy0_minimize(ap_linyyy0_t e)
{
  SWITCH(e->discr){
    ap_linyyyXXX_minimize(e->linyyy.XXX);
  ENDSWITCH
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

bool ap_linyyy0_set(ap_linyyy0_t res, ap_linyyy0_t e, num_internal_t intern)
{
  SWITCH(e->discr)
    return ap_linyyy0_set_linyyyXXX(res,e->linyyy.XXX,intern);
  ENDSWITCH
}

MACRO_MAINZ
bool ap_linyyy0_set_linyyyZZZ(ap_linyyy0_t a, ap_linyyyZZZ_t b, num_internal_t intern)
{
  switch (a->discr){
  case AP_SCALAR_D:
    return ap_linyyyD_set_linyyyZZZ(a->linyyy.D,b,intern);
  case AP_SCALAR_MPQ:
    return ap_linyyyMPQ_set_linyyyZZZ(a->linyyy.MPQ,b,intern);
  case AP_SCALAR_MPFR:
    return ap_linyyyMPFR_set_linyyyZZZ(a->linyyy.MPFR,b,intern);
  default:
    abort();
  }
}
bool ap_linyyyZZZ_set_linyyy0(ap_linyyyZZZ_t a, ap_linyyy0_t b, num_internal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return ap_linyyyZZZ_set_linyyyD(a,b->linyyy.D,intern);
  case AP_SCALAR_MPQ:
    return ap_linyyyZZZ_set_linyyyMPQ(a,b->linyyy.MPQ,intern);
  case AP_SCALAR_MPFR:
    return ap_linyyyZZZ_set_linyyyMPFR(a,b->linyyy.MPFR,intern);
  default:
    abort();
  }
}
ENDMACRO

/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

#if defined(_AP_linexpr0_MARK_)

/* Get the constant and assign it to coeff with possible conversion */
bool ap_linexpr0_get_cst(ap_coeff_t coeff, ap_linexpr0_t expr, num_internal_t intern)
{
  SWITCH (expr->discr)
    return ap_coeff_set_eitvXXX(coeff,expr->linexpr.XXX->cst,intern);
  ENDSWITCH
}
bool ap_linexpr0_get_coeff(ap_coeff_t coeff, ap_linexpr0_t expr, ap_dim_t dim, num_internal_t intern)
{
  SWITCH (expr->discr)
    eitvXXX_ptr eitv = ap_linexprXXX_eitvref0(expr->linexpr.XXX,dim,false);
    if (eitv==NULL) break; else return ap_coeff_set_eitvXXX(coeff,eitv,intern);
  ENDSWITCH
  ap_coeff_set_int(coeff,0);
  return true;
}
bool ap_linexpr0_set_cst(ap_linexpr0_t expr, ap_coeff_t coeff, num_internal_t intern)
{
  ap_coeff_t ref;
  ap_linexpr0_cstref(ref,expr);
  return ap_coeff_set(ref,coeff,intern);
}
bool ap_linexpr0_set_coeff(ap_linexpr0_t expr, ap_dim_t dim, ap_coeff_t coeff, num_internal_t intern)
{
  ap_coeff_t ref;
  ap_linexpr0_coeffref(ref,expr,dim);
  return ap_coeff_set(ref,coeff,intern);
}
void ap_linexpr0_cstref(ap_coeff_t res, ap_linexpr0_t expr)
{
  SWITCH (expr->discr)
    ap_coeff_cons_eitvXXX(res,expr->linexpr.XXX->cst);
  ENDSWITCH
}
void ap_linexpr0_coeffref(ap_coeff_t res, ap_linexpr0_t expr, ap_dim_t dim)
{
  SWITCH (expr->discr)
    eitvXXX_ptr eitv = ap_linexprXXX_eitvref0(expr->linexpr.XXX,dim,true);
    ap_coeff_cons_eitvXXX(res,eitv);
  ENDSWITCH
}

bool ap_linexpr0_set_list(num_internal_t intern,
			  ap_linexpr0_t expr, bool* perror, ...)
{
  bool res;
  va_list va;
  va_start(va,perror);
  SWITCH (expr->discr)
    res = ap_linexprXXX_set_list_generic(ap_linexprXXX_set_list_get_eitvXXX_of_dim,
					 NULL,
					 intern,expr->linexpr.XXX,perror,&va);
  ENDSWITCH
  va_end(va);
  return res;
}

#elif defined (_AP_lincons0_MARK_) || defined (_AP_lingen0_MARK_)

void ap_linyyy0_linexpr0ref(ap_linexpr0_t e, ap_linyyy0_t c)
{
  SWITCH (c->discr)
    ap_linexpr0_cons_XXX(e,c->linyyy.XXX->linexpr);
  ENDSWITCH
}
ap_yyytyp_t* ap_linyyy0_yyytypref(ap_linyyy0_t c)
{
  ap_yyytyp_t* p;
  SWITCH (c->discr)
    p = &c->linyyy.XXX->yyytyp;
  ENDSWITCH
  return p;
}

bool ap_linyyy0_get_linexpr0(ap_linexpr0_t e, ap_linyyy0_t c, num_internal_t intern)
{
  ap_linexpr0_t ref;
  ap_linyyy0_linexpr0ref(ref,c);
  return ap_linexpr0_set(e,ref,intern);
}
ap_yyytyp_t ap_linyyy0_get_yyytyp(ap_linyyy0_t c)
{
  ap_yyytyp_t* ref = ap_linyyy0_yyytypref(c);
  return *ref;
}

bool ap_linyyy0_set_linexpr0(ap_linyyy0_t c, ap_linexpr0_t e, num_internal_t intern)
{
  ap_linexpr0_t ref;
  ap_linyyy0_linexpr0ref(ref,c);
  return ap_linexpr0_set(ref,e,intern);
}
void ap_linyyy0_set_yyytyp(ap_linyyy0_t c, ap_yyytyp_t yyytyp)
{
  ap_yyytyp_t* ref = ap_linyyy0_yyytypref(c);
  *ref = yyytyp;
}

#if defined (_AP_lincons0_MARK_)
mpq_ptr ap_lincons0_mpqref(ap_lincons0_t c)
{
  mpq_ptr p;
  SWITCH (c->discr)
    p = c->lincons.XXX->mpq;
  ENDSWITCH
  return p;
}
void ap_lincons0_get_mpq(mpq_t mpq, ap_lincons0_t c)
{
  mpq_ptr ref = ap_lincons0_mpqref(c);
  mpq_set(mpq,ref);
}
void ap_lincons0_set_mpq(ap_lincons0_t c, mpq_t mpq)
{
  mpq_ptr ref = ap_lincons0_mpqref(c);
  mpq_set(ref,mpq);
}
#endif

#else
#error "HERE"
#endif

/* ====================================================================== */
/* IV. Change of dimensions and permutations */
/* ====================================================================== */

void ap_linyyy0_add_dimensions(ap_linyyy0_t res,
			    ap_linyyy0_t expr,
			    ap_dimchange_t* dimchange)
{
  if (res->discr!=expr->discr)
    abort();
  SWITCH (expr->discr)
    ap_linyyyXXX_add_dimensions(res->linyyy.XXX,expr->linyyy.XXX,dimchange);
  ENDSWITCH
}
void ap_linyyy0_permute_dimensions(ap_linyyy0_t res,
				ap_linyyy0_t expr,
				ap_dimperm_t* perm)
{
  if (res->discr!=expr->discr)
    abort();
  SWITCH (expr->discr)
    ap_linyyyXXX_permute_dimensions(res->linyyy.XXX,expr->linyyy.XXX,perm);
  ENDSWITCH
}

/* ====================================================================== */
/* V. Hashing, comparison */
/* ====================================================================== */

/* Induces reduction of the coefficients */

int ap_linyyy0_hash(ap_linyyy0_t expr)
{
  int res;
  SWITCH(expr->discr)
    res = ap_linyyyXXX_hash(expr->linyyy.XXX);
  ENDSWITCH
  return res;
}
bool ap_linyyy0_equal(ap_linyyy0_t expr1,
		       ap_linyyy0_t expr2)
{
  bool res;
  if (expr1->discr != expr2->discr)
    res = false;
  else {
    SWITCH(expr1->discr)
      res = ap_linyyyXXX_equal(expr1->linyyy.XXX,expr2->linyyy.XXX);
    ENDSWITCH
  }
  return res;
}
/* Lexicographic ordering, terminating by constant coefficients */
int ap_linyyy0_compare(ap_linyyy0_t expr1,
			ap_linyyy0_t expr2)
{
  int res;
  if (expr1->discr != expr2->discr){
    res = (expr1->discr - expr2->discr);
    res = res>0 ? 3 : -3;
  }
  else {
    SWITCH(expr1->discr)
      res = ap_linyyyXXX_compare(expr1->linyyy.XXX,expr2->linyyy.XXX);
    ENDSWITCH
  }
  return res;
}

#undef _AP_linyyy0_MARK_
