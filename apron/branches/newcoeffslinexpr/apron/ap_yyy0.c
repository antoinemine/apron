/* ************************************************************************* */
/* ap_yyy0.c: linear expressions */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "ap_yyy0.h"
#include "ap_linexprconsD_conv.h"
#include "ap_linexprconsMPQ_conv.h"
#include "ap_linexprconsMPFR_conv.h"
#include <stdarg.h>

#define _AP_yyy0_MARK_

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_yyy0_init(ap_yyy0_t res, ap_scalar_discr_t discr, size_t size)
{
  res->discr = discr;
  SWITCH(discr)
    res->yyy.XXX = ap_yyyXXX_alloc(size);
  ENDSWITCH
}
void ap_yyy0_init_set(ap_yyy0_t res,ap_yyy0_t e)
{
  res->discr = e->discr;
  SWITCH(e->discr)
    res->yyy.XXX = ap_yyyXXX_alloc_set(e->yyy.XXX);
  ENDSWITCH
}
MACRO_MAINZ
void ap_yyy0_init_set_yyyZZZ(ap_yyy0_t res, ap_yyyZZZ_t e)
{
  ap_yyy0_init(res,AP_SCALAR_ZZZ,0);
  ap_yyyZZZ_set(res->yyy.ZZZ,e);
}
ENDMACRO
void ap_yyy0_clear(ap_yyy0_t e)
{
  SWITCH(e->discr)
    ap_yyyXXX_clear(e->yyy.XXX);
  ENDSWITCH
}
void ap_yyy0_fprint(FILE* stream, ap_yyy0_t e, char** name_of_dim)
{
  SWITCH(e->discr){
    ap_yyyXXX_fprint(stream,e->yyy.XXX,name_of_dim);
  ENDSWITCH
}
void ap_yyy0_minimize(ap_yyy0_t e)
{
  SWITCH(e->discr){
    ap_yyyXXX_minimize(e->yyy.XXX);
  ENDSWITCH
}

/* ====================================================================== */
/* Conversions */
/* ====================================================================== */

bool ap_yyy0_set(ap_yyy0_t res, ap_yyy0_t e, num_internal_t intern)
{
  SWITCH(e->discr)
    return ap_yyy0_set_yyyXXX(res,e->yyy.XXX,intern);
  ENDSWITCH
}

MACRO_MAINZ
bool ap_yyy0_set_yyyZZZ(ap_yyy0_t a, ap_yyyZZZ_t b, num_internal_t intern)
{
  switch (a->discr){
  case AP_SCALAR_D:
    return ap_yyyD_set_yyyZZZ(a->yyy.D,b,intern);
  case AP_SCALAR_MPQ:
    return ap_yyyMPQ_set_yyyZZZ(a->yyy.MPQ,b,intern);
  case AP_SCALAR_MPFR:
    return ap_yyyMPFR_set_yyyZZZ(a->yyy.MPFR,b,intern);
  default:
    abort();
  }
}
bool ap_yyyZZZ_set_yyy0(ap_yyyZZZ_t a, ap_yyy0_t b, num_internal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return ap_yyyZZZ_set_yyyD(a,b->yyy.D,intern);
  case AP_SCALAR_MPQ:
    return ap_yyyZZZ_set_yyyMPQ(a,b->yyy.MPQ,intern);
  case AP_SCALAR_MPFR:
    return ap_yyyZZZ_set_yyyMPFR(a,b->yyy.MPFR,intern);
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
    ap_coeff_cons_XXX(res,expr->linexpr.XXX->cst);
  ENDSWITCH
}
void ap_linexpr0_coeffref(ap_coeff_t res, ap_linexpr0_t expr, ap_dim_t dim)
{
  SWITCH (expr->discr)
    eitvXXX_ptr eitv = ap_linexprXXX_eitvref0(expr->linexpr.XXX,dim,true);
    ap_coeff_cons_XXX(res,eitv);
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

#elif defined(_AP_lincons0_MARK_)

void ap_lincons0_linexpr0ref(ap_linexpr0_t e, ap_lincons0_t c)
{
  SWITCH (c->discr)
    ap_linexpr0_cons_XXX(e,c->lincons.XXX->linexpr);
  ENDSWITCH
}
ap_constyp_t* ap_lincons0_constypref(ap_lincons0_t c)
{
  ap_constyp_t* p;
  SWITCH (c->discr)
    p = &c->lincons.XXX->constyp;
  ENDSWITCH
  return p;
}
mpq_ptr ap_lincons0_mpqref(ap_lincons0_t c)
{
  mpq_ptr p;
  SWITCH (c->discr)
    p = c->lincons.XXX->mpq;
  ENDSWITCH
  return p;
}

bool ap_lincons0_get_linexpr0(ap_linexpr0_t e, ap_lincons0_t c, num_internal_t intern)
{
  ap_linexpr0_t ref;
  ap_lincons0_linexpr0ref(ref,c);
  return ap_linexpr0_set(e,ref,intern);
}
ap_constyp_t ap_lincons0_get_constyp(ap_lincons0_t c)
{
  ap_constyp_t* ref = ap_lincons0_constypref(c);
  return *ref;
}
void ap_lincons0_get_mpq(mpq_t mpq, ap_lincons0_t c)
{
  mpq_ptr ref = ap_lincons0_mpqref(c);
  mpq_set(mpq,ref);
}

bool ap_lincons0_set_linexpr0(ap_lincons0_t c, ap_linexpr0_t e, num_internal_t intern)
{
  ap_linexpr0_t ref;
  ap_lincons0_linexpr0ref(ref,c);
  return ap_linexpr0_set(ref,e,intern);
}
void ap_lincons0_set_constyp(ap_lincons0_t c, ap_constyp_t constyp)
{
  ap_constyp_t* ref = ap_lincons0_constypref(c);
  *ref = constyp;
}
void ap_lincons0_set_mpq(ap_lincons0_t c, mpq_t mpq)
{
  mpq_ptr ref = ap_lincons0_mpqref(c);
  mpq_set(ref,mpq);
}
#else
#error "HERE"
#endif

/* ====================================================================== */
/* IV. Change of dimensions and permutations */
/* ====================================================================== */

void ap_yyy0_add_dimensions(ap_yyy0_t res,
			    ap_yyy0_t expr,
			    ap_dimchange_t* dimchange)
{
  if (res->discr!=expr->discr)
    abort();
  SWITCH (expr->discr)
    ap_yyyXXX_add_dimensions(res->yyy.XXX,expr->yyy.XXX,dimchange);
  ENDSWITCH
}
void ap_yyy0_permute_dimensions(ap_yyy0_t res,
				ap_yyy0_t expr,
				ap_dimperm_t* perm)
{
  if (res->discr!=expr->discr)
    abort();
  SWITCH (expr->discr)
    ap_yyyXXX_permute_dimensions(res->yyy.XXX,expr->yyy.XXX,perm);
  ENDSWITCH
}

/* ====================================================================== */
/* V. Hashing, comparison */
/* ====================================================================== */

/* Induces reduction of the coefficients */

int ap_yyy0_hash(ap_yyy0_t expr)
{
  int res;
  SWITCH(expr->discr)
    res = ap_yyyXXX_hash(expr->yyy.XXX);
  ENDSWITCH
  return res;
}
bool ap_yyy0_equal(ap_yyy0_t expr1,
		       ap_yyy0_t expr2)
{
  bool res;
  if (expr1->discr != expr2->discr)
    res = false;
  else {
    SWITCH(expr1->discr)
      res = ap_yyyXXX_equal(expr1->yyy.XXX,expr2->yyy.XXX);
    ENDSWITCH
  }
  return res;
}
/* Lexicographic ordering, terminating by constant coefficients */
int ap_yyy0_compare(ap_yyy0_t expr1,
			ap_yyy0_t expr2)
{
  int res;
  if (expr1->discr != expr2->discr){
    res = (expr1->discr - expr2->discr);
    res = res>0 ? 3 : -3;
  }
  else {
    SWITCH(expr1->discr)
      res = ap_yyyXXX_compare(expr1->yyy.XXX,expr2->yyy.XXX);
    ENDSWITCH
  }
  return res;
}

#undef _AP_yyy0_MARK_
