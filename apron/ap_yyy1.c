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
  /* This function assigns the linear expression from a list of tags of type
     itv_coefftag_t, each followed by a number of arguments as specified in
     the definition of the type ap_coeff_tag_t_t, and ended by the tag ITV_END;

     - The dimension ITV_DIM_MAX/AP_DIM_MAX is used to refer to the constat coefficient.
     - If the same dimension appears several times, only the last tag
       referring to it is taken into account.

     Returns true iff all conversions were exact.

     Example:
     ap_linexpr1_set_list(intern,
			  expr,
			  ITV_LFRAC,7,9,0,
			  ITV_DOUBLE2,-3.0,4.5,1,
			  ITV_LLINT,3LL,ITV_DIM_MAX,
			  ITV_END)
     sets expr to "7/9 x0 + [-3,4.5] x1 + 3"
     assuming that the expression was "0" before the call and that all the
     number conversions were exact.
  */

#elif defined(_AP_lincons1_MARK_)

bool ap_lincons1_get_linexpr1(ap_linexpr1_t e, ap_lincons1_t c, num_internal_t intern);
  /* Get the underlying expression and assign it to e with possible
     conversion */
ap_constyp_t ap_lincons1_get_constyp(ap_lincons1_t c);
void ap_lincons1_get_mpq(mpq_t mpq, ap_lincons1_t c);

void ap_lincons1_linexpr1ref(ap_linexpr1_t e, ap_lincons1_t c);
ap_constyp_t* ap_lincons1_constypref(ap_lincons1_t c);
mpq_ptr ap_lincons1_mpqref(ap_lincons1_t c);

bool ap_lincons1_set_linexpr1(ap_lincons1_t c, ap_linexpr1_t e, num_internal_t intern);
  /* Assign the underlying expression of c to e with possible conversion */
void ap_lincons1_set_constyp(ap_lincons1_t c, ap_constyp_t constyp);
void ap_lincons1_set_mpq(ap_lincons1_t c, mpq_t mpq);

#else
#error "HERE"
#endif

/* ====================================================================== */
/* IV. Change of dimensions and permutations */
/* ====================================================================== */

void ap_yyy1_extend_environment(ap_yyy1_t nexpr,
				    ap_yyy1_t expr,
				    ap_environment_t* nenv);

#undef _AP_yyy1_MARK_
