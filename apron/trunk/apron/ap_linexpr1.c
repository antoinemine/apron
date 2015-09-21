/* ************************************************************************* */
/* linexpr1.c: linear expressions */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include "ap_linexpr1.h"
#include <stdarg.h>

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

ap_linexpr1_t ap_linexpr1_make(ap_environment_t* env,
			       ap_linexpr_discr_t lin_discr,
			       size_t size)
{
  ap_linexpr1_t e;
  e.linexpr0 = ap_linexpr0_alloc(lin_discr,
				 lin_discr==AP_LINEXPR_DENSE ?
				 env->intdim+env->realdim :
				 size);
  e.env = ap_environment_copy(env);
  return e;
}
ap_linexpr1_t ap_linexpr1_copy(ap_linexpr1_t* e)
{
  ap_linexpr1_t res;
  res.linexpr0 = ap_linexpr0_copy(e->linexpr0);
  res.env = ap_environment_copy(e->env);
  return res;
}
void ap_linexpr1_clear(ap_linexpr1_t* e)
{
  if (e->linexpr0){
    ap_linexpr0_free(e->linexpr0); e->linexpr0 = NULL;
  }
  if (e->env){
    ap_environment_free(e->env);
    e->env = NULL;
  }
}
void ap_linexpr1_fprint(FILE* stream, ap_linexpr1_t* e)
{
  ap_environment_name_of_dim_t* name_of_dim;

  name_of_dim = ap_environment_name_of_dim_alloc(e->env);
  ap_linexpr0_fprint(stream, e->linexpr0, name_of_dim->p);
  ap_environment_name_of_dim_free(name_of_dim);
}

/* ====================================================================== */
/* II. Tests */
/* ====================================================================== */

/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

/* Return NULL if var undeclared */
ap_coeff_t* ap_linexpr1_coeffref(ap_linexpr1_t* expr, ap_var_t var)
{
  ap_dim_t dim;

  dim = ap_environment_dim_of_var(expr->env,var);
  return ap_linexpr0_coeffref(expr->linexpr0,dim);
}
/* Return true if var undeclared */
bool ap_linexpr1_get_coeff(ap_coeff_t* coeff, ap_linexpr1_t* expr, ap_var_t var)
{
  ap_dim_t dim;

  dim = ap_environment_dim_of_var(expr->env,var);
  return ap_linexpr0_get_coeff(coeff,expr->linexpr0,dim);
}

ap_coeff_t* ap_linexpr1_set_list_get_pcoeff(void* expr, bool cst, va_list* va)
{
  ap_coeff_t* pcoeff;
  if (cst){
    pcoeff = ap_linexpr1_cstref(expr);
  } else {
    ap_var_t var = va_arg(*va,ap_var_t);
    pcoeff = ap_linexpr1_coeffref(expr,var);
  }
  return pcoeff;
}

bool ap_linexpr1_set_list(ap_linexpr1_t* expr, ...)
{
  va_list va;
  bool res;
  va_start(va,expr);
  res = ap_linexpr0_set_list_generic(ap_linexpr1_set_list_get_pcoeff,
				     expr,&va);
  va_end(va);
  return res;
}


/* ====================================================================== */
/* IV. Change of dimensions and permutations */
/* ====================================================================== */
ap_linexpr1_t ap_linexpr1_extend_environment_dimchange(ap_linexpr1_t* expr,
						       ap_environment_t* nenv,
						       ap_dimchange_t* dimchange)
{
  ap_linexpr1_t nexpr;
  nexpr.linexpr0 = ap_linexpr0_add_dimensions(expr->linexpr0,dimchange);
  nexpr.env = ap_environment_copy(nenv);
  return nexpr;
}
void ap_linexpr1_extend_environment_dimchange_with(ap_linexpr1_t* expr,
						   ap_environment_t* nenv,
						   ap_dimchange_t* dimchange)
{
  ap_environment_t* env;
  ap_linexpr0_add_dimensions_with(expr->linexpr0,dimchange);
  env = expr->env;
  expr->env = ap_environment_copy(nenv);
  ap_environment_free(env);
}
bool ap_linexpr1_extend_environment(ap_linexpr1_t* nexpr,
				    ap_linexpr1_t* expr,
				    ap_environment_t* nenv)
{
  ap_dimchange_t* dimchange = ap_environment_dimchange(expr->env,nenv);
  if (dimchange==NULL)
    return true;
  *nexpr = ap_linexpr1_extend_environment_dimchange(expr,nenv,dimchange);
  ap_dimchange_free(dimchange);
  return false;
}
bool ap_linexpr1_extend_environment_with(ap_linexpr1_t* expr,
					 ap_environment_t* nenv)
{
  ap_dimchange_t* dimchange = ap_environment_dimchange(expr->env,nenv);
  if (dimchange==NULL)
    return true;
  ap_linexpr1_extend_environment_dimchange_with(expr,nenv,dimchange);
  ap_dimchange_free(dimchange);
  return false;
}
