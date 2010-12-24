/* ************************************************************************* */
/* ap_linyyy1.c: linear expressions or constraints */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "ap_linyyy1.h"

#define _AP_linyyy1_MARK_

/* ====================================================================== */
/* I. Memory management and printing */
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

MACRO_MAINZ
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
/* II. Conversions */
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
MACRO_MAINZ
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
/* III. Access */
/* ====================================================================== */

#if defined(_AP_linexpr1_MARK_)

void ap_linexpr1_cstref(ap_coeff_t coeff, ap_linexpr1_t expr)
{ ap_linexpr0_cstref(coeff,expr->linexpr0); }
void ap_linexpr1_coeffref(ap_coeff_t coeff, bool* perror, ap_linexpr1_t expr, ap_var_t var)
{
  ap_dim_t dim = ap_environment_dim_of_var(expr->env,var);
  *perror = (dim==AP_DIM_MAX);
  if (! (*perror))
    ap_linexpr0_coeffref(coeff,expr->linexpr0,dim);
}
bool ap_linexpr1_get_cst(ap_coeff_t coeff, ap_linexpr1_t expr, num_internal_t intern)
{ return ap_linexpr0_get_cst(coeff,expr->linexpr0,intern); }

bool ap_linexpr1_get_coeff(ap_coeff_t coeff, bool* perror, ap_linexpr1_t expr, ap_var_t var, num_internal_t intern)
{
  ap_coeff_t ref;
  ap_linexpr1_coeffref(ref,perror,expr,var);
  if (*perror)
    return false;
  else
    return ap_coeff_set(coeff,ref,intern);
}

bool ap_linexpr1_set_cst(ap_linexpr1_t expr, ap_coeff_t coeff, num_internal_t intern)
{ return ap_linexpr0_set_cst(expr->linexpr0, coeff, intern); }
bool ap_linexpr1_set_coeff(ap_linexpr1_t expr, bool* perror, ap_var_t var, ap_coeff_t coeff, num_internal_t intern)
{
  ap_coeff_t ref;
  ap_linexpr1_coeffref(ref,perror,expr,var);
  if (*perror)
    return false;
  else
    return ap_coeff_set(ref,coeff,intern);
}

bool ap_linexpr1_set_list(num_internal_t intern, ap_linexpr1_t expr, bool* perror, ...)
{
  bool res;
  va_list va;
  va_start(va,perror);
  SWITCH (expr->linexpr0->discr)
    res = ap_linexprXXX_set_list_generic(ap_linexprXXX_set_list_get_eitvXXX_of_var,
					 expr->linexpr0->linexpr.XXX,perror,
					 expr->env,&va,intern);
  ENDSWITCH
  va_end(va);
  return res;
}

#elif defined(_AP_lincons1_MARK_) || defined (_AP_lingen1_MARK_)

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

#if defined (_AP_lincons0_MARK_)
mpq_ptr ap_lincons1_mpqref(ap_lincons1_t c)
{ return ap_lincons0_mpqref(c->lincons0); }
void ap_lincons1_get_mpq(mpq_t mpq, ap_lincons1_t c)
{ ap_lincons0_get_mpq(mpq,c->lincons0); }
void ap_lincons1_set_mpq(ap_lincons1_t c, mpq_t mpq)
{ ap_lincons0_set_mpq(c->lincons0,mpq); }
#endif

#else
#error "HERE"
#endif

/* ====================================================================== */
/* IV. Change of dimensions and permutations */
/* ====================================================================== */

void ap_linyyy1_extend_environment(ap_linyyy1_t nexpr,
				bool* perror,
				ap_linyyy1_t expr,
				ap_environment_t* nenv)
{
  if (nexpr->linyyy0->discr != expr->linyyy0->discr)
    abort();
  SWITCH(nexpr->linyyy0->discr)
    ap_linyyyXXX_extend_environment(nexpr->linyyy0->linyyy.XXX,perror,
				 nenv,
				 expr->linyyy0->linyyy.XXX,
				 expr->env);
  ENDSWITCH
  ap_environment_set(&nexpr->env,nenv);
}
#undef _AP_linyyy1_MARK_
