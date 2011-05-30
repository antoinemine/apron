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

MACRO_FOREACH XXX @AllNum
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
  MACRO_SWITCH(discr) XXX
    res->linyyy.XXX = ap_linyyyXXX_alloc(size);
  ENDMACRO;
}
void ap_linyyy0_init_set(ap_linyyy0_t res,ap_linyyy0_t e)
{
  res->discr = e->discr;
  MACRO_SWITCH(e->discr) XXX
    res->linyyy.XXX = ap_linyyyXXX_alloc_set(e->linyyy.XXX);
  ENDMACRO;
}
MACRO_FOREACH ZZZ @MainNum
void ap_linyyy0_init_set_linyyyZZZ(ap_linyyy0_t res, ap_linyyyZZZ_t e)
{
  ap_linyyy0_init(res,AP_SCALAR_ZZZ,0);
  ap_linyyyZZZ_set(res->linyyy.ZZZ,e);
}
ENDMACRO
#if defined (_AP_lincons0_MARK_)
void ap_lincons0_set_bool(ap_lincons0_t e, bool b)
{
  MACRO_SWITCH(e->discr) XXX
    ap_linconsXXX_set_bool(e->lincons.XXX,b);
  ENDMACRO;
}
#endif
void ap_linyyy0_resize(ap_linyyy0_t e, size_t size)
{
  MACRO_SWITCH(e->discr) XXX
    ap_linyyyXXX_resize(e->linyyy.XXX,size);
  ENDMACRO;
}
void ap_linyyy0_clear(ap_linyyy0_t e)
{
  MACRO_SWITCH(e->discr) XXX
    ap_linyyyXXX_clear(e->linyyy.XXX);
  ENDMACRO;
}
void ap_linyyy0_fprint(FILE* stream, ap_linyyy0_t e, char** name_of_dim)
{
  MACRO_SWITCH(e->discr) XXX
    ap_linyyyXXX_fprint(stream,e->linyyy.XXX,name_of_dim);
  ENDMACRO;
}
void ap_linyyy0_print(ap_linyyy0_t a, char** name_of_dim)
{ ap_linyyy0_fprint(stdout,a,name_of_dim); }
void ap_linyyy0_minimize(ap_linyyy0_t e)
{
  MACRO_SWITCH(e->discr) XXX
    ap_linyyyXXX_minimize(e->linyyy.XXX);
  ENDMACRO;
}


/* ====================================================================== */
/* I.2 Conversions */
/* ====================================================================== */

bool ap_linyyy0_set(ap_linyyy0_t res, ap_linyyy0_t e, num_internal_t intern)
{
  MACRO_SWITCH(e->discr) XXX
    return ap_linyyy0_set_linyyyXXX(res,e->linyyy.XXX,intern);
  ENDMACRO;
}

MACRO_FOREACH XXX @AllNum
bool ap_linyyy0_set_linyyyXXX(ap_linyyy0_t a, ap_linyyyXXX_t b, num_internal_t intern)
{
  MACRO_SWITCH(a->discr) ZZZ
    return ap_linyyyZZZ_set_linyyyXXX(a->linyyy.ZZZ,b,intern);
  ENDMACRO
}
bool ap_linyyyXXX_set_linyyy0(ap_linyyyXXX_t a, ap_linyyy0_t b, num_internal_t intern)
{
  MACRO_SWITCH(b->discr) ZZZ
    return ap_linyyyXXX_set_linyyyZZZ(a,b->linyyy.ZZZ,intern);
  ENDMACRO
}
ENDMACRO

/* ====================================================================== */
/* I.3 Tests */
/* ====================================================================== */

#if defined (_AP_linexpr0_MARK_) || defined (_AP_lincons0_MARK_)

bool ap_linyyy0_is_linear(ap_linyyy0_t expr)
{
  MACRO_SWITCH(expr->discr) XXX
    return ap_linyyyXXX_is_linear(expr->linyyy.XXX);
  ENDMACRO;
}
bool ap_linyyy0_is_quasilinear(ap_linyyy0_t expr)
{
  MACRO_SWITCH(expr->discr) XXX
    return ap_linyyyXXX_is_quasilinear(expr->linyyy.XXX);
  ENDMACRO;
}
ap_linexpr_type_t ap_linyyy0_type(ap_linyyy0_t expr)
{
  MACRO_SWITCH(expr->discr) XXX
    return ap_linyyyXXX_type(expr->linyyy.XXX);
  ENDMACRO;
}

bool ap_linyyy0_is_integer(ap_linyyy0_t expr, size_t intdim)
{
  MACRO_SWITCH(expr->discr) XXX
    return ap_linyyyXXX_is_integer(expr->linyyy.XXX,intdim);
  ENDMACRO;
}
bool ap_linyyy0_is_real(ap_linyyy0_t expr, size_t intdim)
{
  MACRO_SWITCH(expr->discr) XXX
    return ap_linyyyXXX_is_real(expr->linyyy.XXX,intdim);
  ENDMACRO;
}
#endif

/* ====================================================================== */
/* I.4 Access */
/* ====================================================================== */

#if defined(_AP_linexpr0_MARK_)

/* Get the constant and assign it to coeff with possible conversion */
bool ap_linexpr0_get_cst(ap_coeff_t coeff, ap_linexpr0_t expr, num_internal_t intern)
{
  MACRO_SWITCH(expr->discr) XXX
    return ap_coeff_set_eitvXXX(coeff,expr->linexpr.XXX->cst,intern);
  ENDMACRO;
}
bool ap_linexpr0_get_coeff(ap_coeff_t coeff, ap_linexpr0_t expr, ap_dim_t dim, num_internal_t intern)
{
  MACRO_SWITCH(expr->discr) XXX {
    eitvXXX_ptr eitv = ap_linexprXXX_eitvref0(expr->linexpr.XXX,dim,false);
    if (eitv==NULL) break; else return ap_coeff_set_eitvXXX(coeff,eitv,intern);
  }
  ENDMACRO;
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
  MACRO_SWITCH(expr->discr) XXX
    ap_coeff_cons_eitvXXX(res,expr->linexpr.XXX->cst);
  ENDMACRO;
}
void ap_linexpr0_coeffref(ap_coeff_t res, ap_linexpr0_t expr, ap_dim_t dim)
{
  MACRO_SWITCH(expr->discr) XXX {
    eitvXXX_ptr eitv = ap_linexprXXX_eitvref0(expr->linexpr.XXX,dim,true);
    ap_coeff_cons_eitvXXX(res,eitv);
  }
  ENDMACRO;
}
void ap_linexpr0_coeffrefdim_of_index(ap_coeff_t res, ap_dim_t* pdim, ap_linexpr0_t expr, size_t index)
{
  MACRO_SWITCH(expr->discr) XXX {
    if (index>=expr->linexpr.XXX->effsize){
      *pdim = AP_DIM_MAX;
    }
    else {
      ap_lintermXXX_ptr linterm = expr->linexpr.XXX->linterm[index];
      ap_coeff_cons_eitvXXX(res,linterm->eitv);
      *pdim = linterm->dim;
    }
  }
  ENDMACRO;
}
bool ap_linexpr0_set_list(num_internal_t intern,
			  ap_linexpr0_t expr, bool* perror, ...)
{
  bool res;
  va_list va;
  va_start(va,perror);
  MACRO_SWITCH(expr->discr) XXX
    res = ap_linexprXXX_set_list_generic(
	ap_linexprXXX_set_list_get_eitvXXX_of_dim,
	expr->linexpr.XXX,perror,NULL,&va,intern
    );
  ENDMACRO;
  va_end(va);
  return res;
}

#elif defined (_AP_lincons0_MARK_) || defined (_AP_lingen0_MARK_)
/* Get the constant and assign it to coeff with possible conversion */
bool ap_linyyy0_get_cst(ap_coeff_t coeff, ap_linyyy0_t expr, num_internal_t intern)
{
  MACRO_SWITCH(expr->discr) XXX
    return ap_coeff_set_eitvXXX(coeff,expr->linyyy.XXX->linexpr->cst,intern);
  ENDMACRO;
}
bool ap_linyyy0_get_coeff(ap_coeff_t coeff, ap_linyyy0_t expr, ap_dim_t dim, num_internal_t intern)
{
  MACRO_SWITCH(expr->discr) XXX {
    eitvXXX_ptr eitv = ap_linexprXXX_eitvref0(expr->linyyy.XXX->linexpr,dim,false);
    if (eitv==NULL) break; else return ap_coeff_set_eitvXXX(coeff,eitv,intern);
  }
  ENDMACRO;
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
  MACRO_SWITCH(expr->discr) XXX
    ap_coeff_cons_eitvXXX(res,expr->linyyy.XXX->linexpr->cst);
  ENDMACRO;
}
void ap_linyyy0_coeffref(ap_coeff_t res, ap_linyyy0_t expr, ap_dim_t dim)
{
  MACRO_SWITCH(expr->discr) XXX {
    eitvXXX_ptr eitv = ap_linexprXXX_eitvref0(expr->linyyy.XXX->linexpr,dim,true);
    ap_coeff_cons_eitvXXX(res,eitv);
  }
  ENDMACRO;
}
void ap_linyyy0_coeffdimref_of_index(ap_coeff_t res, ap_dim_t* pdim, ap_linyyy0_t expr, size_t index)
{
  MACRO_SWITCH(expr->discr) XXX {
    ap_lintermXXX_ptr linterm = expr->linyyy.XXX->linexpr->linterm[index];
    ap_coeff_cons_eitvXXX(res,linterm->eitv);
    pdim = &linterm->dim;
  }
  ENDMACRO;
}
bool ap_linyyy0_set_list(num_internal_t intern,
			 ap_linyyy0_t expr, bool* perror, ...)
{
  bool res;
  va_list va;
  va_start(va,perror);
  MACRO_SWITCH(expr->discr) XXX
    res = ap_linexprXXX_set_list_generic(
	ap_linexprXXX_set_list_get_eitvXXX_of_dim,
	expr->linyyy.XXX->linexpr,perror,NULL,&va,intern
    );
  ENDMACRO;
  va_end(va);
  return res;
}

void ap_linyyy0_linexpr0ref(ap_linexpr0_t e, ap_linyyy0_t c)
{
  MACRO_SWITCH(c->discr) XXX
    ap_linexpr0_cons_XXX(e,c->linyyy.XXX->linexpr);
  ENDMACRO;
}
ap_yyytyp_t* ap_linyyy0_yyytypref(ap_linyyy0_t c)
{
  ap_yyytyp_t* p;
  MACRO_SWITCH(c->discr) XXX
    p = &c->linyyy.XXX->yyytyp;
  ENDMACRO;
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
  MACRO_SWITCH(c->discr) XXX
    p = c->lincons.XXX->mpq;
  ENDMACRO;
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
  MACRO_SWITCH(expr->discr) XXX
    ap_linyyyXXX_add_dimensions(res->linyyy.XXX,expr->linyyy.XXX,dimchange);
  ENDMACRO;
}
void ap_linyyy0_permute_dimensions(ap_linyyy0_t res,
				   ap_linyyy0_t expr,
				   ap_dimperm_t* perm)
{
  if (res->discr!=expr->discr)
    abort();
  MACRO_SWITCH(expr->discr) XXX
    ap_linyyyXXX_permute_dimensions(res->linyyy.XXX,expr->linyyy.XXX,perm);
  ENDMACRO;
}

/* ====================================================================== */
/* I.6 Hashing, comparison */
/* ====================================================================== */

/* Induces reduction of the coefficients */

int ap_linyyy0_hash(ap_linyyy0_t expr)
{
  int res;
  MACRO_SWITCH(expr->discr) XXX
    res = ap_linyyyXXX_hash(expr->linyyy.XXX);
  ENDMACRO;
  return res;
}
bool ap_linyyy0_equal(ap_linyyy0_t expr1,
		       ap_linyyy0_t expr2)
{
  bool res;
  if (expr1->discr != expr2->discr)
    res = false;
  else {
    MACRO_SWITCH(expr1->discr) XXX
      res = ap_linyyyXXX_equal(expr1->linyyy.XXX,expr2->linyyy.XXX);
    ENDMACRO;
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
    MACRO_SWITCH(expr1->discr) XXX
      res = ap_linyyyXXX_cmp(expr1->linyyy.XXX,expr2->linyyy.XXX);
    ENDMACRO;
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
  MACRO_SWITCH(discr) XXX
    a->linyyy_array.XXX = ap_linyyyXXX_array_alloc(size);
  ENDMACRO;
}
void ap_linyyy0_array_init_set(ap_linyyy0_array_t a,ap_linyyy0_array_t b)
{
  a->discr = b->discr;
  MACRO_SWITCH(a->discr) XXX
    a->linyyy_array.XXX = ap_linyyyXXX_array_alloc_set(b->linyyy_array.XXX);
  ENDMACRO;
}
MACRO_FOREACH ZZZ @MainNum
void ap_linyyy0_array_init_set_linyyyZZZ_array(ap_linyyy0_array_t res, ap_linyyyZZZ_array_t e)
 {
   ap_linyyy0_array_init(res,AP_SCALAR_ZZZ,0);
   ap_linyyyZZZ_array_set(res->linyyy_array.ZZZ,e);
}
ENDMACRO
void ap_linyyy0_array_resize(ap_linyyy0_array_t a, size_t size)
{
  MACRO_SWITCH(a->discr) XXX
    ap_linyyyXXX_array_resize(a->linyyy_array.XXX,size);
  ENDMACRO;
}
void ap_linyyy0_array_minimize(ap_linyyy0_array_t a)
{
  MACRO_SWITCH(a->discr) XXX
    ap_linyyyXXX_array_minimize(a->linyyy_array.XXX);
  ENDMACRO;
}
void ap_linyyy0_array_clear(ap_linyyy0_array_t a)
{
  MACRO_SWITCH(a->discr) XXX
    ap_linyyyXXX_array_clear(a->linyyy_array.XXX);
  ENDMACRO;
}

void ap_linyyy0_array_fprint(FILE* stream, ap_linyyy0_array_t e, char** name_of_dim)
{
  MACRO_SWITCH(e->discr) XXX
    ap_linyyyXXX_array_fprint(stream,e->linyyy_array.XXX,name_of_dim);
  ENDMACRO;
}
void ap_linyyy0_array_print(ap_linyyy0_array_t a, char** name_of_dim)
{ ap_linyyy0_array_fprint(stdout,a,name_of_dim); }

/* ====================================================================== */
/* II.2 Conversions */
/* ====================================================================== */

bool ap_linyyy0_array_set(ap_linyyy0_array_t res, ap_linyyy0_array_t e, num_internal_t intern)
{
  MACRO_SWITCH(e->discr) XXX
    return ap_linyyy0_array_set_linyyyXXX_array(res,e->linyyy_array.XXX,intern);
  ENDMACRO;
}

MACRO_FOREACH XXX @AllNum
bool ap_linyyy0_array_set_linyyyXXX_array(
    ap_linyyy0_array_t a, ap_linyyyXXX_array_t b, num_internal_t intern
)
{
  MACRO_SWITCH(a->discr) ZZZ
    return ap_linyyyZZZ_array_set_linyyyXXX_array(a->linyyy_array.ZZZ,b,intern);
  ENDMACRO;
}
bool ap_linyyyXXX_array_set_linyyy0_array(
    ap_linyyyXXX_array_t a, ap_linyyy0_array_t b, num_internal_t intern
)
{
  MACRO_SWITCH(b->discr) ZZZ
    return ap_linyyyXXX_array_set_linyyyZZZ_array(a,b->linyyy_array.ZZZ,intern);
  ENDMACRO;
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
  MACRO_SWITCH(array->discr) XXX
    return ap_linyyyXXX_array_is_linear(array->linyyy_array.XXX);
  ENDMACRO;
}
bool ap_linyyy0_array_is_quasilinear(ap_linyyy0_array_t array)
{
  MACRO_SWITCH(array->discr) XXX
    return ap_linyyyXXX_array_is_quasilinear(array->linyyy_array.XXX);
  ENDMACRO;
}
ap_linexpr_type_t ap_linyyy0_array_type(ap_linyyy0_array_t array)
{
  MACRO_SWITCH(array->discr) XXX
    return ap_linyyyXXX_array_type(array->linyyy_array.XXX);
  ENDMACRO;
}
#endif

/* ====================================================================== */
/* II.4 Access */
/* ====================================================================== */

size_t ap_linyyy0_array_size(ap_linyyy0_array_t p)
{
  size_t res;
  MACRO_SWITCH(p->discr) XXX
    res = p->linyyy_array.XXX->size;
  ENDMACRO;
  return res;
}
void ap_linyyy0_array_ref_index(ap_linyyy0_t a, ap_linyyy0_array_t p, size_t i)
{
  MACRO_SWITCH(p->discr) XXX
    ap_linyyy0_cons_XXX(a,p->linyyy_array.XXX->p[i]);
  ENDMACRO;
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
  MACRO_SWITCH(expr->discr) XXX
    ap_linyyyXXX_array_add_dimensions(res->linyyy_array.XXX,
					expr->linyyy_array.XXX,
					dimchange);
  ENDMACRO;
}
void ap_linyyy0_array_permute_dimensions(ap_linyyy0_array_t res,
					 ap_linyyy0_array_t expr,
					 ap_dimperm_t* perm)
{
  if (res->discr!=expr->discr)
    abort();
  MACRO_SWITCH(expr->discr) XXX
    ap_linyyyXXX_array_permute_dimensions(res->linyyy_array.XXX,
					expr->linyyy_array.XXX,
					perm);
  ENDMACRO;
}


#undef _AP_linyyy0_MARK_
