/* ************************************************************************* */
/* ap_yyy1.c: linear expressions or constraints */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "ap_yyy1.h"

#define _AP_yyy1_MARK_

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_yyy1_init(ap_yyy1_t res, ap_scalar_discr_t discr, size_t size, ap_environment_t* env)
{
  ap_yyy0_init(res->yyy0,discr,size);
  res->env = ap_environment_copy(env);
}
void ap_yyy1_init_set(ap_yyy1_t res, ap_yyy1_t e)
{
  if (res!=e){
    ap_yyy0_init_set(res->yyy0,e->yyy0);
    res->env = ap_environment_copy(e->env);
  }
}
void ap_yyy1_init_set_linexpr0(ap_yyy1_t res, ap_yyy0_t e, ap_environment_t* env)
{
  ap_yyy0_init_set(res->yyy0,e);
  res->env = ap_environment_copy(env);
 }

MACRO_MAINZ
void ap_yyy1_init_set_yyyZZZ(ap_yyy1_t res, ap_yyyZZZ_t e, ap_environment_t* env)
{
  ap_yyy0_init_set_yyyZZZ(res->yyy0,e);
  res->env = ap_environment_copy(env);
}
ENDMACRO
void ap_yyy1_clear(ap_yyy1_t e)
{
  ap_yyy0_clear(e->yyy0);
  ap_environment_free(e->env);
}
void ap_yyy1_fprint(FILE* stream, ap_yyy1_t e, char** name_of_dim)
{
  ap_environment_name_of_dim_t* name_of_dim;

  name_of_dim = ap_environment_name_of_dim_alloc(e->env);
  ap_yyy0_fprint(stream, e->linexpr0, name_of_dim->p);
  ap_environment_name_of_dim_free(name_of_dim);
}
void ap_yyy1_minimize(ap_yyy1_t e)
{
  ap_yyy0_minimize(e->yyy0);
}

/* ====================================================================== */
/* II. Conversions */
/* ====================================================================== */

bool ap_yyy1_set(ap_yyy1_t res, ap_yyy1_t e, num_internal_t intern)
{
  if (res!=e){
    ap_environment_set(&res->env,e->env);
    return ap_yyy0_set(res->yyy0,e->yyy0,intern);
  }
  else
    return true;
}
MACRO_MAINZ
bool ap_yyy1_set_yyyZZZ(ap_yyy1_t res, ap_yyyZZZ_t e, ap_environment_t* env, num_internal_t intern)
{
  ap_environment_set(&res->env,env);
  return ap_yyy0_set_yyyZZZ(res->yyy0,e,intern);
}
bool ap_yyyZZZ_set_yyy1(ap_yyyZZZ_t res, ap_yyy1_t e, num_internal_t intern)
{
  return ap_yyyZZZ_set_yyy0(res,e->yyy0);
}
ENDMACRO

/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

#if defined(_AP_linexpr1_MARK_)

void ap_linexpr1_cstref(ap_coeff_t coeff, ap_linexpr1_t expr)
{ ap_linexpr0_cstref(coeff,expr->linexpr0); }
void ap_linexpr1_coeffref(ap_coeff_t coeff, bool* perror, ap_linexpr1_t expr, ap_var_t var);
{
  ap_dim_t dim = ap_environment_dim_of_var(expr->env,dim);
  *perror = (dim==AP_DIM_MAX);
  if (! (*perror))
    ap_linexpr0_coeffref(coeff,expr->linexpr0,dim);
}
bool ap_linexpr1_get_cst(ap_coeff_t coeff, ap_linexpr1_t expr, num_internal_t intern)
{ return ap_linexpr0_get_cst(coeff,expr->linexpr0,intern); }

bool ap_linexpr1_get_coeff(ap_coeff_t coeff, bool* perror, ap_linexpr1_t expr, ap_var_t var, num_internal_t intern)
{
  ap_coeff_t ref;
  ap_linexpr1_coeffref(ref,perror,expr,var,intern);
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
  ap_linexpr1_coeffref(ref,perror,expr,var,intern);
  if (*perror)
    return false;
  else
    return ap_coeff_set(ref,coeff,intern);
}

bool ap_linexpr1_set_list(num_internal_t intern, ap_linexpr1_t expr, bool* perror, ...);
{
  bool res;
  va_list va;
  va_start(va,perror);
  SWITCH (expr->discr)
    res = ap_linexprXXX_set_list_generic(ap_linexprXXX_set_list_get_eitvXXX_of_var,
					 expr->env,
					 intern,expr->linexpr0->linexpr.XXX,perror,&va);
  ENDSWITCH
  va_end(va);
  return res;
}

#elif defined(_AP_lincons1_MARK_)

void ap_lincons1_linexpr1ref(ap_linexpr1_t e, ap_lincons1_t c)
{
  ap_lincons0_linexpr0ref(e->linexpr0,c->lincons0);
  e->env = env;
}
ap_constyp_t* ap_lincons1_constypref(ap_lincons1_t c)
{ return ap_lincons0_constypref(c->lincons0); }
mpq_ptr ap_lincons1_mpqref(ap_lincons1_t c)
{ return ap_lincons0_mpqref(c->lincons0); }
bool ap_lincons1_get_linexpr1(ap_linexpr1_t e, ap_lincons1_t c, num_internal_t intern)
{
  ap_linexpr1_t ref;
  ap_lincons1_linexpr1ref(ref,c);
  return ap_linexpr1_set(e,ref,intern);
}
ap_constyp_t ap_lincons1_get_constyp(ap_lincons1_t c)
{ return ap_lincons0_get_constyp(c->lincons0); }
void ap_lincons1_get_mpq(mpq_t mpq, ap_lincons1_t c);
{ ap_lincons0_get_mpq(mpq,c->lincons0); }

bool ap_lincons1_set_linexpr1(ap_lincons1_t c, ap_linexpr1_t e, num_internal_t intern)
{
  ap_linexpr1_t ref;
  ap_lincons1_linexpr1ref(ref,c);
  return ap_linexpr1_set(ref,e,intern);
}
void ap_lincons1_set_constyp(ap_lincons1_t c, ap_constyp_t constyp)
{ ap_lincons0_set_constyp(c->lincons0,constyp); }
void ap_lincons1_set_mpq(ap_lincons1_t c, mpq_t mpq);
{ ap_lincons0_set_constyp(c->lincons0,mpq); }

#else
#error "HERE"
#endif

/* ====================================================================== */
/* IV. Change of dimensions and permutations */
/* ====================================================================== */

void ap_yyy1_extend_environment(ap_yyy1_t nexpr,
				bool* perror,
				ap_yyy1_t expr,
				ap_environment_t* nenv)
{
  if (nexpr->yyy0->discr != expr->yyy0->discr)
    abort();
  SWITCH(nexpr->yyy0->discr)
    ap_yyyXXX_extend_environment(nexpr->yyy0->yyy.XXX,perror,
				 nenv,
				 expr->yyy0->yyy.XXX,
				 expr->env);
  ENDSWITCH
  ap_environment_set(&nexpr->env,nenv);
}
#undef _AP_yyy1_MARK_
