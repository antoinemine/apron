/* GENERATED FROM ap_yyy1.c, DO NOT MODIFY */
#line 1 "/Users/bjeannet/dev/newcoeffslinexpr/apron/ap_yyy1.c"
/* ************************************************************************* */
/* ap_yyy1.c: linear expressions or constraints */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "ap_lincons1.h"

#define _AP_lincons1_MARK_

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_lincons1_init(ap_lincons1_t res, ap_scalar_discr_t discr, size_t size, ap_environment_t* env)
{
  ap_lincons0_init(res->lincons0,discr,size);
  res->env = ap_environment_copy(env);
}
void ap_lincons1_init_set(ap_lincons1_t res, ap_lincons1_t e)
{
  if (res!=e){
    ap_lincons0_init_set(res->lincons0,e->lincons0);
    res->env = ap_environment_copy(e->env);
  }
}
void ap_lincons1_init_set_linexpr0(ap_lincons1_t res, ap_lincons0_t e, ap_environment_t* env)
{
  ap_lincons0_init_set(res->lincons0,e);
  res->env = ap_environment_copy(env);
 }

#line 35 "/Users/bjeannet/dev/newcoeffslinexpr/apron/ap_yyy1.c"
void ap_lincons1_init_set_linconsD(ap_lincons1_t res, ap_linconsD_t e, ap_environment_t* env)
{
  ap_lincons0_init_set_linconsD(res->lincons0,e);
  res->env = ap_environment_copy(env);
}
#line 35 "/Users/bjeannet/dev/newcoeffslinexpr/apron/ap_yyy1.c"
void ap_lincons1_init_set_linconsMPQ(ap_lincons1_t res, ap_linconsMPQ_t e, ap_environment_t* env)
{
  ap_lincons0_init_set_linconsMPQ(res->lincons0,e);
  res->env = ap_environment_copy(env);
}
#line 35 "/Users/bjeannet/dev/newcoeffslinexpr/apron/ap_yyy1.c"
void ap_lincons1_init_set_linconsMPFR(ap_lincons1_t res, ap_linconsMPFR_t e, ap_environment_t* env)
{
  ap_lincons0_init_set_linconsMPFR(res->lincons0,e);
  res->env = ap_environment_copy(env);
}

void ap_lincons1_clear(ap_lincons1_t e)
{
  ap_lincons0_clear(e->lincons0);
  ap_environment_free(e->env);
}
void ap_lincons1_fprint(FILE* stream, ap_lincons1_t e)
{
  ap_environment_name_of_dim_t* name_of_dim;

  name_of_dim = ap_environment_name_of_dim_alloc(e->env);
  ap_lincons0_fprint(stream, e->lincons0, name_of_dim->p);
  ap_environment_name_of_dim_free(name_of_dim);
}
void ap_lincons1_minimize(ap_lincons1_t e)
{
  ap_lincons0_minimize(e->lincons0);
}

/* ====================================================================== */
/* II. Conversions */
/* ====================================================================== */

bool ap_lincons1_set(ap_lincons1_t res, ap_lincons1_t e, num_internal_t intern)
{
  if (res!=e){
    ap_environment_set(&res->env,e->env);
    return ap_lincons0_set(res->lincons0,e->lincons0,intern);
  }
  else
    return true;
}
#line 74 "/Users/bjeannet/dev/newcoeffslinexpr/apron/ap_yyy1.c"
bool ap_lincons1_set_linconsD(ap_lincons1_t res, ap_linconsD_t e, ap_environment_t* env, num_internal_t intern)
{
  ap_environment_set(&res->env,env);
  return ap_lincons0_set_linconsD(res->lincons0,e,intern);
}
bool ap_linconsD_set_lincons1(ap_linconsD_t res, ap_lincons1_t e, num_internal_t intern)
{
  return ap_linconsD_set_lincons0(res,e->lincons0,intern);
}
#line 74 "/Users/bjeannet/dev/newcoeffslinexpr/apron/ap_yyy1.c"
bool ap_lincons1_set_linconsMPQ(ap_lincons1_t res, ap_linconsMPQ_t e, ap_environment_t* env, num_internal_t intern)
{
  ap_environment_set(&res->env,env);
  return ap_lincons0_set_linconsMPQ(res->lincons0,e,intern);
}
bool ap_linconsMPQ_set_lincons1(ap_linconsMPQ_t res, ap_lincons1_t e, num_internal_t intern)
{
  return ap_linconsMPQ_set_lincons0(res,e->lincons0,intern);
}
#line 74 "/Users/bjeannet/dev/newcoeffslinexpr/apron/ap_yyy1.c"
bool ap_lincons1_set_linconsMPFR(ap_lincons1_t res, ap_linconsMPFR_t e, ap_environment_t* env, num_internal_t intern)
{
  ap_environment_set(&res->env,env);
  return ap_lincons0_set_linconsMPFR(res->lincons0,e,intern);
}
bool ap_linconsMPFR_set_lincons1(ap_linconsMPFR_t res, ap_lincons1_t e, num_internal_t intern)
{
  return ap_linconsMPFR_set_lincons0(res,e->lincons0,intern);
}

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
  switch (expr->linexpr0->discr) {
#line 130 "/Users/bjeannet/dev/newcoeffslinexpr/apron/ap_yyy1.c"
  case AP_SCALAR_D: {
    res = ap_linexprD_set_list_generic(ap_linexprD_set_list_get_eitvD_of_var,
					 expr->env,
					 intern,expr->linexpr0->linexpr.D,perror,&va);
    }
    break;
#line 130 "/Users/bjeannet/dev/newcoeffslinexpr/apron/ap_yyy1.c"
  case AP_SCALAR_MPQ: {
    res = ap_linexprMPQ_set_list_generic(ap_linexprMPQ_set_list_get_eitvMPQ_of_var,
					 expr->env,
					 intern,expr->linexpr0->linexpr.MPQ,perror,&va);
    }
    break;
#line 130 "/Users/bjeannet/dev/newcoeffslinexpr/apron/ap_yyy1.c"
  case AP_SCALAR_MPFR: {
    res = ap_linexprMPFR_set_list_generic(ap_linexprMPFR_set_list_get_eitvMPFR_of_var,
					 expr->env,
					 intern,expr->linexpr0->linexpr.MPFR,perror,&va);
    }
    break;
  default: abort();
  }
#line 130 "/Users/bjeannet/dev/newcoeffslinexpr/apron/ap_yyy1.c"
  va_end(va);
  return res;
}

#elif defined(_AP_lincons1_MARK_)

void ap_lincons1_linexpr1ref(ap_linexpr1_t e, ap_lincons1_t c)
{
  ap_lincons0_linexpr0ref(e->linexpr0,c->lincons0);
  e->env = c->env;
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
void ap_lincons1_get_mpq(mpq_t mpq, ap_lincons1_t c)
{ ap_lincons0_get_mpq(mpq,c->lincons0); }

bool ap_lincons1_set_linexpr1(ap_lincons1_t c, ap_linexpr1_t e, num_internal_t intern)
{
  ap_linexpr1_t ref;
  ap_lincons1_linexpr1ref(ref,c);
  return ap_linexpr1_set(ref,e,intern);
}
void ap_lincons1_set_constyp(ap_lincons1_t c, ap_constyp_t constyp)
{ ap_lincons0_set_constyp(c->lincons0,constyp); }
void ap_lincons1_set_mpq(ap_lincons1_t c, mpq_t mpq)
{ ap_lincons0_set_mpq(c->lincons0,mpq); }

#else
#error "HERE"
#endif

/* ====================================================================== */
/* IV. Change of dimensions and permutations */
/* ====================================================================== */

void ap_lincons1_extend_environment(ap_lincons1_t nexpr,
				bool* perror,
				ap_lincons1_t expr,
				ap_environment_t* nenv)
{
  if (nexpr->lincons0->discr != expr->lincons0->discr)
    abort();
  switch (nexpr->lincons0->discr) {
#line 187 "/Users/bjeannet/dev/newcoeffslinexpr/apron/ap_yyy1.c"
  case AP_SCALAR_D: {
    ap_linconsD_extend_environment(nexpr->lincons0->lincons.D,perror,
				 nenv,
				 expr->lincons0->lincons.D,
				 expr->env);
    }
    break;
#line 187 "/Users/bjeannet/dev/newcoeffslinexpr/apron/ap_yyy1.c"
  case AP_SCALAR_MPQ: {
    ap_linconsMPQ_extend_environment(nexpr->lincons0->lincons.MPQ,perror,
				 nenv,
				 expr->lincons0->lincons.MPQ,
				 expr->env);
    }
    break;
#line 187 "/Users/bjeannet/dev/newcoeffslinexpr/apron/ap_yyy1.c"
  case AP_SCALAR_MPFR: {
    ap_linconsMPFR_extend_environment(nexpr->lincons0->lincons.MPFR,perror,
				 nenv,
				 expr->lincons0->lincons.MPFR,
				 expr->env);
    }
    break;
  default: abort();
  }
#line 187 "/Users/bjeannet/dev/newcoeffslinexpr/apron/ap_yyy1.c"
  ap_environment_set(&nexpr->env,nenv);
}
#undef _AP_lincons1_MARK_
