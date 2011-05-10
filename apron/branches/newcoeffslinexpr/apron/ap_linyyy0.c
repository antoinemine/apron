/* ************************************************************************* */
/* ap_linyyy0.c: linear expressions */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ap_linyyy0.h"

MACRO_ALLX
#include "ap_linyyyXXX.h"
ENDMACRO

#define _AP_linyyy0_MARK_


/* ********************************************************************** */
/* I. ap_linyyy0_t */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Memory management and printing */
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
#if defined (_AP_lincons0_MARK_)
void ap_lincons0_set_bool(ap_lincons0_t e, bool b)
{
  SWITCH(e->discr)
    ap_linconsXXX_set_bool(e->lincons.XXX,b);
  ENDSWITCH
}
#endif
void ap_linyyy0_resize(ap_linyyy0_t e, size_t size)
{
  SWITCH(e->discr)
    ap_linyyyXXX_resize(e->linyyy.XXX,size);
  ENDSWITCH
}
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
/* I.2 Conversions */
/* ====================================================================== */

bool ap_linyyy0_set(ap_linyyy0_t res, ap_linyyy0_t e, num_internal_t intern)
{
  SWITCH(e->discr)
    return ap_linyyy0_set_linyyyXXX(res,e->linyyy.XXX,intern);
  ENDSWITCH
}

MACRO_ALLX
bool ap_linyyy0_set_linyyyXXX(ap_linyyy0_t a, ap_linyyyXXX_t b, num_internal_t intern)
{
  switch (a->discr){
  case AP_SCALAR_D:
    return ap_linyyyD_set_linyyyXXX(a->linyyy.D,b,intern);
  case AP_SCALAR_MPQ:
    return ap_linyyyMPQ_set_linyyyXXX(a->linyyy.MPQ,b,intern);
  case AP_SCALAR_MPFR:
    return ap_linyyyMPFR_set_linyyyXXX(a->linyyy.MPFR,b,intern);
  default:
    abort();
  }
}
bool ap_linyyyXXX_set_linyyy0(ap_linyyyXXX_t a, ap_linyyy0_t b, num_internal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return ap_linyyyXXX_set_linyyyD(a,b->linyyy.D,intern);
  case AP_SCALAR_MPQ:
    return ap_linyyyXXX_set_linyyyMPQ(a,b->linyyy.MPQ,intern);
  case AP_SCALAR_MPFR:
    return ap_linyyyXXX_set_linyyyMPFR(a,b->linyyy.MPFR,intern);
  default:
    abort();
  }
}
ENDMACRO

/* ====================================================================== */
/* I.3 Tests */
/* ====================================================================== */

#if defined (_AP_linexpr0_MARK_) || defined (_AP_lincons0_MARK_)

bool ap_linyyy0_is_linear(ap_linyyy0_t expr)
{
  SWITCH(expr->discr)
    return ap_linyyyXXX_is_linear(expr->linyyy.XXX);
  ENDSWITCH
}
bool ap_linyyy0_is_quasilinear(ap_linyyy0_t expr)
{
  SWITCH(expr->discr)
    return ap_linyyyXXX_is_quasilinear(expr->linyyy.XXX);
  ENDSWITCH
}
ap_linexpr_type_t ap_linyyy0_type(ap_linyyy0_t expr)
{
  SWITCH(expr->discr)
    return ap_linyyyXXX_type(expr->linyyy.XXX);
  ENDSWITCH
}

bool ap_linyyy0_is_integer(ap_linyyy0_t expr, size_t intdim)
{
  SWITCH(expr->discr)
    return ap_linyyyXXX_is_integer(expr->linyyy.XXX,intdim);
  ENDSWITCH
}
bool ap_linyyy0_is_real(ap_linyyy0_t expr, size_t intdim)
{
  SWITCH(expr->discr)
    return ap_linyyyXXX_is_real(expr->linyyy.XXX,intdim);
  ENDSWITCH
}
#endif

/* ====================================================================== */
/* I.4 Access */
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
void ap_linexpr0_coeffrefdim_of_index(ap_coeff_t res, ap_dim_t* pdim, ap_linexpr0_t expr, size_t index)
{
  SWITCH (expr->discr)
    if (index>=expr->linexpr.XXX->effsize){
      *pdim = AP_DIM_MAX;
    }
    else {
      ap_lintermXXX_ptr linterm = expr->linexpr.XXX->linterm[index];
      ap_coeff_cons_eitvXXX(res,linterm->eitv);
      *pdim = linterm->dim;
    }
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
					 expr->linexpr.XXX,perror,NULL,&va,intern);
  ENDSWITCH
  va_end(va);
  return res;
}

#elif defined (_AP_lincons0_MARK_) || defined (_AP_lingen0_MARK_)
/* Get the constant and assign it to coeff with possible conversion */
bool ap_linyyy0_get_cst(ap_coeff_t coeff, ap_linyyy0_t expr, num_internal_t intern)
{
  SWITCH (expr->discr)
    return ap_coeff_set_eitvXXX(coeff,expr->linyyy.XXX->linexpr->cst,intern);
  ENDSWITCH
}
bool ap_linyyy0_get_coeff(ap_coeff_t coeff, ap_linyyy0_t expr, ap_dim_t dim, num_internal_t intern)
{
  SWITCH (expr->discr)
    eitvXXX_ptr eitv = ap_linexprXXX_eitvref0(expr->linyyy.XXX->linexpr,dim,false);
    if (eitv==NULL) break; else return ap_coeff_set_eitvXXX(coeff,eitv,intern);
  ENDSWITCH
  ap_coeff_set_int(coeff,0);
  return true;
}
bool ap_linyyy0_set_cst(ap_linyyy0_t expr, ap_coeff_t coeff, num_internal_t intern)
{
  ap_coeff_t ref;
  ap_linyyy0_cstref(ref,expr);
  return ap_coeff_set(ref,coeff,intern);
}
bool ap_linyyy0_set_coeff(ap_linyyy0_t expr, ap_dim_t dim, ap_coeff_t coeff, num_internal_t intern)
{
  ap_coeff_t ref;
  ap_linyyy0_coeffref(ref,expr,dim);
  return ap_coeff_set(ref,coeff,intern);
}
void ap_linyyy0_cstref(ap_coeff_t res, ap_linyyy0_t expr)
{
  SWITCH (expr->discr)
    ap_coeff_cons_eitvXXX(res,expr->linyyy.XXX->linexpr->cst);
  ENDSWITCH
}
void ap_linyyy0_coeffref(ap_coeff_t res, ap_linyyy0_t expr, ap_dim_t dim)
{
  SWITCH (expr->discr)
    eitvXXX_ptr eitv = ap_linexprXXX_eitvref0(expr->linyyy.XXX->linexpr,dim,true);
    ap_coeff_cons_eitvXXX(res,eitv);
  ENDSWITCH
}
void ap_linyyy0_coeffdimref_of_index(ap_coeff_t res, ap_dim_t* pdim, ap_linyyy0_t expr, size_t index)
{
  SWITCH (expr->discr)
    ap_lintermXXX_ptr linterm = expr->linyyy.XXX->linexpr->linterm[index];
    ap_coeff_cons_eitvXXX(res,linterm->eitv);
    pdim = &linterm->dim;
  ENDSWITCH
}
bool ap_linyyy0_set_list(num_internal_t intern,
			 ap_linyyy0_t expr, bool* perror, ...)
{
  bool res;
  va_list va;
  va_start(va,perror);
  SWITCH (expr->discr)
    res = ap_linexprXXX_set_list_generic(ap_linexprXXX_set_list_get_eitvXXX_of_dim,
					 expr->linyyy.XXX->linexpr,perror,NULL,&va,intern);
  ENDSWITCH
  va_end(va);
  return res;
}

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
/* I.5 Change of dimensions and permutations */
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
/* I.6 Hashing, comparison */
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
int ap_linyyy0_cmp(ap_linyyy0_t expr1,
                   ap_linyyy0_t expr2)
{
  int res;
  if (expr1->discr != expr2->discr){
    res = (expr1->discr - expr2->discr);
    res = res>0 ? 3 : -3;
  }
  else {
    SWITCH(expr1->discr)
      res = ap_linyyyXXX_cmp(expr1->linyyy.XXX,expr2->linyyy.XXX);
    ENDSWITCH
  }
  return res;
}

/* ********************************************************************** */
/* II. ap_linyyy0_array_t */
/* ********************************************************************** */

/* ====================================================================== */
/* II.1 Memory management and printing */
/* ====================================================================== */

void ap_linyyy0_array_init(ap_linyyy0_array_t a, ap_scalar_discr_t discr, size_t size)
{
  a->discr = discr;
  SWITCH(discr)
    a->linyyy_array.XXX = ap_linyyyXXX_array_alloc(size);
  ENDSWITCH
}
void ap_linyyy0_array_init_set(ap_linyyy0_array_t a,ap_linyyy0_array_t b)
{
  a->discr = b->discr;
  SWITCH(a->discr)
    a->linyyy_array.XXX = ap_linyyyXXX_array_alloc_set(b->linyyy_array.XXX);
  ENDSWITCH
}
MACRO_MAINZ
void ap_linyyy0_array_init_set_linyyyZZZ_array(ap_linyyy0_array_t res, ap_linyyyZZZ_array_t e)
 {
   ap_linyyy0_array_init(res,AP_SCALAR_ZZZ,0);
   ap_linyyyZZZ_array_set(res->linyyy_array.ZZZ,e);
}
ENDMACRO
void ap_linyyy0_array_resize(ap_linyyy0_array_t a, size_t size)
{
  SWITCH(a->discr)
    ap_linyyyXXX_array_resize(a->linyyy_array.XXX,size);
  ENDSWITCH
}
void ap_linyyy0_array_minimize(ap_linyyy0_array_t a)
{
  SWITCH(a->discr)
    ap_linyyyXXX_array_minimize(a->linyyy_array.XXX);
  ENDSWITCH
}
void ap_linyyy0_array_clear(ap_linyyy0_array_t a)
{
  SWITCH(a->discr)
    ap_linyyyXXX_array_clear(a->linyyy_array.XXX);
  ENDSWITCH
}

void ap_linyyy0_array_fprint(FILE* stream, ap_linyyy0_array_t e, char** name_of_dim)
{
  SWITCH(e->discr)
    ap_linyyyXXX_array_fprint(stream,e->linyyy_array.XXX,name_of_dim);
  ENDSWITCH
}

/* ====================================================================== */
/* II.2 Conversions */
/* ====================================================================== */

bool ap_linyyy0_array_set(ap_linyyy0_array_t res, ap_linyyy0_array_t e, num_internal_t intern)
{
  SWITCH(e->discr)
    return ap_linyyy0_array_set_linyyyXXX_array(res,e->linyyy_array.XXX,intern);
  ENDSWITCH
}
MACRO_ALLX
bool ap_linyyy0_array_set_linyyyXXX_array(ap_linyyy0_array_t a, ap_linyyyXXX_array_t b, num_internal_t intern)
{
  switch (a->discr){
  case AP_SCALAR_D:
    return ap_linyyyD_array_set_linyyyXXX_array(a->linyyy_array.D,b,intern);
  case AP_SCALAR_MPQ:
    return ap_linyyyMPQ_array_set_linyyyXXX_array(a->linyyy_array.MPQ,b,intern);
  case AP_SCALAR_MPFR:
    return ap_linyyyMPFR_array_set_linyyyXXX_array(a->linyyy_array.MPFR,b,intern);
  default:
    abort();
  }
}
bool ap_linyyyXXX_array_set_linyyy0_array(ap_linyyyXXX_array_t a, ap_linyyy0_array_t b, num_internal_t intern)
{
  switch(b->discr){
  case AP_SCALAR_D:
    return ap_linyyyXXX_array_set_linyyyD_array(a,b->linyyy_array.D,intern);
  case AP_SCALAR_MPQ:
    return ap_linyyyXXX_array_set_linyyyMPQ_array(a,b->linyyy_array.MPQ,intern);
  case AP_SCALAR_MPFR:
    return ap_linyyyXXX_array_set_linyyyMPFR_array(a,b->linyyy_array.MPFR,intern);
  default:
    abort();
  }
}
bool ap_linyyy0_array_set_index_linyyyXXX(ap_linyyy0_array_t p, size_t i, ap_linyyyXXX_t a, num_internal_t intern)
{
  ap_linyyy0_t ref;
  ap_linyyy0_array_ref_index(ref,p,i);
  return ap_linyyy0_set_linyyyXXX(ref,a,intern);
}
bool ap_linyyy0_array_get_index_linyyyXXX(ap_linyyyXXX_t a, ap_linyyy0_array_t p, size_t i, num_internal_t intern)
{
  ap_linyyy0_t ref;
  ap_linyyy0_array_ref_index(ref,p,i);
  return ap_linyyyXXX_set_linyyy0(a,ref,intern);
}
ENDMACRO

/* ====================================================================== */
/* II.3 Tests */
/* ====================================================================== */

#if defined (_AP_linexpr0_MARK_) || defined (_AP_lincons0_MARK_)
bool ap_linyyy0_array_is_linear(ap_linyyy0_array_t array)
{
  SWITCH(array->discr)
    return ap_linyyyXXX_array_is_linear(array->linyyy_array.XXX);
  ENDSWITCH
}
bool ap_linyyy0_array_is_quasilinear(ap_linyyy0_array_t array)
{
  SWITCH(array->discr)
    return ap_linyyyXXX_array_is_quasilinear(array->linyyy_array.XXX);
  ENDSWITCH
}
ap_linexpr_type_t ap_linyyy0_array_type(ap_linyyy0_array_t array)
{
  SWITCH(array->discr)
    return ap_linyyyXXX_array_type(array->linyyy_array.XXX);
  ENDSWITCH
}
#endif

/* ====================================================================== */
/* II.4 Access */
/* ====================================================================== */

size_t ap_linyyy0_array_size(ap_linyyy0_array_t p)
{
  size_t res;
  SWITCH(p->discr)
    res = p->linyyy_array.XXX->size;
  ENDSWITCH
  return res;
}
void ap_linyyy0_array_ref_index(ap_linyyy0_t a, ap_linyyy0_array_t p, size_t i)
{
  SWITCH (p->discr)
    ap_linyyy0_cons_XXX(a,p->linyyy_array.XXX->p[i]);
  ENDSWITCH
}
bool ap_linyyy0_array_get_index(ap_linyyy0_t a, ap_linyyy0_array_t p, size_t i, num_internal_t intern)
{
  ap_linyyy0_t ref;
  ap_linyyy0_array_ref_index(ref,p,i);
  return ap_linyyy0_set(a,ref,intern);
}
bool ap_linyyy0_array_set_index(ap_linyyy0_array_t p, size_t i, ap_linyyy0_t a, num_internal_t intern)
{
  ap_linyyy0_t ref;
  ap_linyyy0_array_ref_index(ref,p,i);
  return ap_linyyy0_set(ref,a,intern);
}

/* ====================================================================== */
/* II.5 Change of dimensions and permutations */
/* ====================================================================== */

void ap_linyyy0_array_add_dimensions(ap_linyyy0_array_t res,
				     ap_linyyy0_array_t expr,
				     ap_dimchange_t* dimchange)
{
  if (res->discr!=expr->discr)
    abort();
  SWITCH(expr->discr)
    ap_linyyyXXX_array_add_dimensions(res->linyyy_array.XXX,
					expr->linyyy_array.XXX,
					dimchange);
  ENDSWITCH
}
void ap_linyyy0_array_permute_dimensions(ap_linyyy0_array_t res,
					 ap_linyyy0_array_t expr,
					 ap_dimperm_t* perm)
{
  if (res->discr!=expr->discr)
    abort();
  SWITCH(expr->discr)
    ap_linyyyXXX_array_permute_dimensions(res->linyyy_array.XXX,
					expr->linyyy_array.XXX,
					perm);
  ENDSWITCH
}


#undef _AP_linyyy0_MARK_
