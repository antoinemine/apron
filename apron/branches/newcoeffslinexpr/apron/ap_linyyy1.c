/* ************************************************************************* */
/* ap_linyyy1.c: linear expressions or constraints */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include <limits.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "ap_linyyy1.h"

#define _AP_linyyy1_MARK_

/* ********************************************************************** */
/* I. ap_linyyy1_t */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Memory management and printing */
/* ====================================================================== */

void ap_linyyy1_init(ap_linyyy1_t res, ap_scalar_discr_t discr, size_t size, ap_environment_t* env)
{
  ap_linyyy0_init(res->linyyy0,discr,size);
  res->env = ap_environment_copy(env);
}
void ap_linyyy1_init_set(ap_linyyy1_t res, ap_linyyy1_t e)
{
  if (res!=e){
    ap_linyyy0_init_set(res->linyyy0,e->linyyy0);
    res->env = ap_environment_copy(e->env);
  }
}
void ap_linyyy1_init_set_linexpr0(ap_linyyy1_t res, ap_linyyy0_t e, ap_environment_t* env)
{
  ap_linyyy0_init_set(res->linyyy0,e);
  res->env = ap_environment_copy(env);
 }

MACRO_FOREACH ZZZ @MainNum
void ap_linyyy1_init_set_linyyyZZZ(ap_linyyy1_t res, ap_linyyyZZZ_t e, ap_environment_t* env)
{
  ap_linyyy0_init_set_linyyyZZZ(res->linyyy0,e);
  res->env = ap_environment_copy(env);
}
ENDMACRO

void ap_linyyy1_clear(ap_linyyy1_t e)
{
  ap_linyyy0_clear(e->linyyy0);
  ap_environment_free(e->env);
}
void ap_linyyy1_fprint(FILE* stream, ap_linyyy1_t e)
{
  ap_environment_name_of_dim_t* name_of_dim;

  name_of_dim = ap_environment_name_of_dim_alloc(e->env);
  ap_linyyy0_fprint(stream, e->linyyy0, name_of_dim->p);
  ap_environment_name_of_dim_free(name_of_dim);
}
void ap_linyyy1_minimize(ap_linyyy1_t e)
{
  ap_linyyy0_minimize(e->linyyy0);
}

/* ====================================================================== */
/* I.2 Conversions */
/* ====================================================================== */

bool ap_linyyy1_set(ap_linyyy1_t res, ap_linyyy1_t e, num_internal_t intern)
{
  if (res!=e){
    ap_environment_set(&res->env,e->env);
    return ap_linyyy0_set(res->linyyy0,e->linyyy0,intern);
  }
  else
    return true;
}
bool ap_linyyy1_set_linyyy0(ap_linyyy1_t res, ap_linyyy0_t e, ap_environment_t* env, num_internal_t intern)
{
  ap_environment_set(&res->env,env);
  return ap_linyyy0_set(res->linyyy0,e,intern);
}
MACRO_FOREACH ZZZ @MainNum
bool ap_linyyy1_set_linyyyZZZ(ap_linyyy1_t res, ap_linyyyZZZ_t e, ap_environment_t* env, num_internal_t intern)
{
  ap_environment_set(&res->env,env);
  return ap_linyyy0_set_linyyyZZZ(res->linyyy0,e,intern);
}
bool ap_linyyyZZZ_set_linyyy1(ap_linyyyZZZ_t res, ap_linyyy1_t e, num_internal_t intern)
{
  return ap_linyyyZZZ_set_linyyy0(res,e->linyyy0,intern);
}
ENDMACRO

/* ====================================================================== */
/* I.3 Access */
/* ====================================================================== */

void ap_linyyy1_cstref(ap_coeff_t coeff, ap_linyyy1_t expr)
{ ap_linyyy0_cstref(coeff,expr->linyyy0); }
void ap_linyyy1_coeffref(ap_coeff_t coeff, bool* perror, ap_linyyy1_t expr, ap_var_t var)
{
  ap_dim_t dim = ap_environment_dim_of_var(expr->env,var);
  *perror = (dim==AP_DIM_MAX);
  if (! (*perror))
    ap_linyyy0_coeffref(coeff,expr->linyyy0,dim);
}
bool ap_linyyy1_get_cst(ap_coeff_t coeff, ap_linyyy1_t expr, num_internal_t intern)
{ return ap_linyyy0_get_cst(coeff,expr->linyyy0,intern); }

bool ap_linyyy1_get_coeff(ap_coeff_t coeff, bool* perror, ap_linyyy1_t expr, ap_var_t var, num_internal_t intern)
{
  ap_coeff_t ref;
  ap_linyyy1_coeffref(ref,perror,expr,var);
  if (*perror)
    return false;
  else
    return ap_coeff_set(coeff,ref,intern);
}

bool ap_linyyy1_set_cst(ap_linyyy1_t expr, ap_coeff_t coeff, num_internal_t intern)
{ return ap_linyyy0_set_cst(expr->linyyy0, coeff, intern); }
bool ap_linyyy1_set_coeff(ap_linyyy1_t expr, bool* perror, ap_var_t var, ap_coeff_t coeff, num_internal_t intern)
{
  ap_coeff_t ref;
  ap_linyyy1_coeffref(ref,perror,expr,var);
  if (*perror)
    return false;
  else
    return ap_coeff_set(ref,coeff,intern);
}

bool ap_linyyy1_set_list(num_internal_t intern, ap_linyyy1_t expr, bool* perror, ...)
{
  bool res;
  va_list va;
  va_start(va,perror);
  MACRO_SWITCH(expr->linyyy0->discr) XXX
    res = ap_linexprXXX_set_list_generic(
	ap_linexprXXX_set_list_get_eitvXXX_of_var,
#if defined (_AP_linexpr1_MARK_)
	expr->linyyy0->linexpr.XXX,perror,
#else
	expr->linyyy0->linyyy.XXX->linexpr,perror,
#endif
	expr->env,&va,intern);
  ENDMACRO;
  va_end(va);
  return res;
}

#if defined (_AP_lincons1_MARK_)
mpq_ptr ap_lincons1_mpqref(ap_lincons1_t c)
{ return ap_lincons0_mpqref(c->lincons0); }
void ap_lincons1_get_mpq(mpq_t mpq, ap_lincons1_t c)
{ ap_lincons0_get_mpq(mpq,c->lincons0); }
void ap_lincons1_set_mpq(ap_lincons1_t c, mpq_t mpq)
{ ap_lincons0_set_mpq(c->lincons0,mpq); }
#endif

#if defined(_AP_lincons1_MARK_) || defined (_AP_lingen1_MARK_)

void ap_linyyy1_linexpr1ref(ap_linexpr1_t e, ap_linyyy1_t c)
{
  ap_linyyy0_linexpr0ref(e->linexpr0,c->linyyy0);
  e->env = c->env;
}
ap_yyytyp_t* ap_linyyy1_yyytypref(ap_linyyy1_t c)
{ return ap_linyyy0_yyytypref(c->linyyy0); }
bool ap_linyyy1_get_linexpr1(ap_linexpr1_t e, ap_linyyy1_t c, num_internal_t intern)
{
  ap_linexpr1_t ref;
  ap_linyyy1_linexpr1ref(ref,c);
  return ap_linexpr1_set(e,ref,intern);
}
ap_yyytyp_t ap_linyyy1_get_yyytyp(ap_linyyy1_t c)
{ return ap_linyyy0_get_yyytyp(c->linyyy0); }

bool ap_linyyy1_set_linexpr1(ap_linyyy1_t c, ap_linexpr1_t e, num_internal_t intern)
{
  ap_linexpr1_t ref;
  ap_linyyy1_linexpr1ref(ref,c);
  return ap_linexpr1_set(ref,e,intern);
}
void ap_linyyy1_set_yyytyp(ap_linyyy1_t c, ap_yyytyp_t yyytyp)
{ ap_linyyy0_set_yyytyp(c->linyyy0,yyytyp); }


#endif

/* ====================================================================== */
/* I.4 Change of dimensions and permutations */
/* ====================================================================== */

void ap_linyyy1_extend_environment(ap_linyyy1_t nexpr,
				bool* perror,
				ap_linyyy1_t expr,
				ap_environment_t* nenv)
{
  if (nexpr->linyyy0->discr != expr->linyyy0->discr)
    abort();
  MACRO_SWITCH(nexpr->linyyy0->discr) XXX
    ap_linyyyXXX_extend_environment(
	nexpr->linyyy0->linyyy.XXX,perror,
	nenv,
	expr->linyyy0->linyyy.XXX,
	expr->env
    );
  ENDMACRO;
  ap_environment_set(&nexpr->env,nenv);
}


/* ********************************************************************** */
/* II. ap_linyyy1_array_t */
/* ********************************************************************** */

/* ====================================================================== */
/* II.1 Memory management and printing */
/* ====================================================================== */

void ap_linyyy1_array_init(ap_linyyy1_array_t res, ap_scalar_discr_t discr, size_t size, ap_environment_t* env)
{
  ap_linyyy0_array_init(res->linyyy0_array,discr,size);
  res->env = ap_environment_copy(env);
}
void ap_linyyy1_array_init_set(ap_linyyy1_array_t res, ap_linyyy1_array_t e)
{
  ap_linyyy0_array_init_set(res->linyyy0_array,e->linyyy0_array);
  res->env = ap_environment_copy(e->env);
}
void ap_linyyy1_array_init_set_linyyy0_array(ap_linyyy1_array_t res, ap_linyyy0_array_t e, ap_environment_t* env)
{
  ap_linyyy0_array_init_set(res->linyyy0_array,e);
  res->env = ap_environment_copy(env);
}
MACRO_FOREACH ZZZ @MainNum
void ap_linyyy1_array_init_set_linyyyZZZ_array(ap_linyyy1_array_t res, ap_linyyyZZZ_array_t e, ap_environment_t* env)
{
  ap_linyyy0_array_init_set_linyyyZZZ_array(res->linyyy0_array,e);
  res->env = ap_environment_copy(env);
}
ENDMACRO
void ap_linyyy1_array_minimize(ap_linyyy1_array_t e)
{ ap_linyyy0_array_minimize(e->linyyy0_array); }
void ap_linyyy1_array_clear(ap_linyyy1_array_t e)
{
  ap_linyyy0_array_clear(e->linyyy0_array);
  ap_environment_free(e->env);
}
void ap_linyyy1_array_fprint(FILE* stream, ap_linyyy1_array_t e)
{
  ap_environment_name_of_dim_t* name_of_dim;

  name_of_dim = ap_environment_name_of_dim_alloc(e->env);
  ap_linyyy0_array_fprint(stream, e->linyyy0_array, name_of_dim->p);
  ap_environment_name_of_dim_free(name_of_dim);
}

/* ====================================================================== */
/* II.2 Access with possible conversions */
/* ====================================================================== */

size_t ap_linyyy1_array_size(ap_linyyy1_array_t p)
{ return ap_linyyy0_array_size(p->linyyy0_array); }
#if !defined(_AP_lingen1_ARRAY_H_)
ap_linexpr_type_t ap_linyyy1_array_type(ap_linyyy1_array_t array)
{ return ap_linyyy0_array_type(array->linyyy0_array); }
bool ap_linyyy1_array_is_quasilinear(ap_linyyy1_array_t array)
{ return ap_linyyy0_array_is_quasilinear(array->linyyy0_array); }
#endif
bool ap_linyyy1_array_is_linear(ap_linyyy1_array_t array)
{ return ap_linyyy0_array_is_linear(array->linyyy0_array); }

bool ap_linyyy1_array_set(ap_linyyy1_array_t res, ap_linyyy1_array_t e, num_internal_t intern)
{
  if (res!=e){
    ap_environment_set(&res->env,e->env);
    return ap_linyyy0_array_set(res->linyyy0_array,e->linyyy0_array,intern);
  }
  else
    return true;
}
bool ap_linyyy1_array_set_index(ap_linyyy1_array_t p, bool* perror, size_t i, ap_linyyy1_t a, num_internal_t intern)
{
  *perror = !ap_environment_is_eq(p->env,a->env);
  if (*perror) return false;
  else
    return ap_linyyy0_array_set_index(p->linyyy0_array,i,a->linyyy0,intern);
}
bool ap_linyyy1_array_get_index(ap_linyyy1_t a, ap_linyyy1_array_t p, size_t i, num_internal_t intern)
{
  ap_environment_set(&a->env,p->env);
  return ap_linyyy0_array_get_index(a->linyyy0, p->linyyy0_array, i, intern);
}
void ap_linyyy1_array_ref_index(ap_linyyy1_t a, ap_linyyy1_array_t p, size_t i)
{
  a->env = p->env;
  ap_linyyy0_array_ref_index(a->linyyy0,p->linyyy0_array,i);
}
bool ap_linyyy1_array_set_linyyy0_array(ap_linyyy1_array_t res, ap_linyyy0_array_t e, ap_environment_t* env, num_internal_t intern)
{
  ap_environment_set(&res->env,env);
  return ap_linyyy0_array_set(res->linyyy0_array,e,intern);
}
MACRO_FOREACH XXX @AllNum
bool ap_linyyy1_array_set_linyyyXXX_array(ap_linyyy1_array_t a, ap_linyyyXXX_array_t b, ap_environment_t* env, num_internal_t intern)
{
  ap_environment_set(&a->env,env);
  return ap_linyyy0_array_set_linyyyXXX_array(a->linyyy0_array,b,intern);
}
bool ap_linyyyXXX_array_set_linyyy1_array(ap_linyyyXXX_array_t a, ap_linyyy1_array_t b, num_internal_t intern)
{
  return ap_linyyyXXX_array_set_linyyy0_array(a,b->linyyy0_array,intern);
}
bool ap_linyyy1_array_set_index_linyyyXXX(ap_linyyy1_array_t p, size_t i, ap_linyyyXXX_t a, num_internal_t intern)
{
  return ap_linyyy0_array_set_index_linyyyXXX(p->linyyy0_array,i,a,intern);
}
bool ap_linyyy1_array_get_index_linyyyXXX(ap_linyyyXXX_t a, ap_linyyy1_array_t p, size_t i, num_internal_t intern)
{
  return ap_linyyy0_array_get_index_linyyyXXX(a,p->linyyy0_array,i,intern);
}
ENDMACRO

/* ====================================================================== */
/* II.3 Change of dimensions and permutations */
/* ====================================================================== */

void ap_linyyy1_array_extend_environment(ap_linyyy1_array_t nexpr,
					 bool* perror,
					 ap_linyyy1_array_t expr,
					 ap_environment_t* nenv)
{
  if (nexpr->linyyy0_array->discr != expr->linyyy0_array->discr)
    abort();
  MACRO_SWITCH(nexpr->linyyy0_array->discr) XXX
    ap_linyyyXXX_array_extend_environment(
	nexpr->linyyy0_array->linyyy_array.XXX,perror,
	nenv,
	expr->linyyy0_array->linyyy_array.XXX,
	expr->env
    );
  ENDMACRO;
  ap_environment_set(&nexpr->env,nenv);
}

#undef _AP_linyyy1_MARK_
