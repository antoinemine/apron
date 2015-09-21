/* ************************************************************************* */
/* ap_texpr1.c: tree expressions */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include "ap_texpr1.h"

#include <stdarg.h>

/* ====================================================================== */
/* I. Constructors and Destructors */
/* ====================================================================== */

static inline
ap_texpr1_t* ap_texpr1_of_texpr0(ap_environment_t* env, ap_texpr0_t* texpr0)
{
  ap_texpr1_t* res = malloc(sizeof(ap_texpr1_t));
  res->texpr0 = texpr0;
  res->env = ap_environment_copy(env);
  return res;
}

ap_texpr1_t* ap_texpr1_cst                 (ap_environment_t* env, ap_coeff_t* coeff)
{
  return ap_texpr1_of_texpr0(env, ap_texpr0_cst(coeff));
}

ap_texpr1_t* ap_texpr1_cst_scalar          (ap_environment_t* env, ap_scalar_t* scalar)
{
  return ap_texpr1_of_texpr0(env, ap_texpr0_cst_scalar(scalar));
}

ap_texpr1_t* ap_texpr1_cst_scalar_mpq      (ap_environment_t* env, mpq_t mpq)
{
  return ap_texpr1_of_texpr0(env, ap_texpr0_cst_scalar_mpq(mpq));
}

ap_texpr1_t* ap_texpr1_cst_scalar_mpfr     (ap_environment_t* env, mpfr_t mpfr)
{
  return ap_texpr1_of_texpr0(env, ap_texpr0_cst_scalar_mpfr(mpfr));
}

ap_texpr1_t* ap_texpr1_cst_scalar_int      (ap_environment_t* env, long int num)
{
  return ap_texpr1_of_texpr0(env, ap_texpr0_cst_scalar_int(num));
}

ap_texpr1_t* ap_texpr1_cst_scalar_frac     (ap_environment_t* env, long int num, unsigned long int den)
{
  return ap_texpr1_of_texpr0(env, ap_texpr0_cst_scalar_frac(num,den));
}

ap_texpr1_t* ap_texpr1_cst_scalar_double   (ap_environment_t* env, double num)
{
  return ap_texpr1_of_texpr0(env, ap_texpr0_cst_scalar_double(num));
}

ap_texpr1_t* ap_texpr1_cst_interval        (ap_environment_t* env, ap_interval_t* itv)
{
  return ap_texpr1_of_texpr0(env, ap_texpr0_cst_interval(itv));
}

ap_texpr1_t* ap_texpr1_cst_interval_scalar (ap_environment_t* env, ap_scalar_t* inf, ap_scalar_t* sup)
{
  return ap_texpr1_of_texpr0(env, ap_texpr0_cst_interval_scalar(inf,sup));
}

ap_texpr1_t* ap_texpr1_cst_interval_mpq    (ap_environment_t* env, mpq_t inf, mpq_t sup)
{
  return ap_texpr1_of_texpr0(env, ap_texpr0_cst_interval_mpq(inf,sup));
}

ap_texpr1_t* ap_texpr1_cst_interval_mpfr   (ap_environment_t* env, mpfr_t inf, mpfr_t sup)
{
  return ap_texpr1_of_texpr0(env, ap_texpr0_cst_interval_mpfr(inf,sup));
}

ap_texpr1_t* ap_texpr1_cst_interval_int    (ap_environment_t* env, long int inf, long int sup)
{
  return ap_texpr1_of_texpr0(env, ap_texpr0_cst_interval_int(inf,sup));
}

ap_texpr1_t* ap_texpr1_cst_interval_frac   (ap_environment_t* env, long int numinf, unsigned long int deninf,
					    long int numsup, unsigned long int densup)
{
  return ap_texpr1_of_texpr0(env, ap_texpr0_cst_interval_frac(numinf,deninf,numsup,densup));
}

ap_texpr1_t* ap_texpr1_cst_interval_double (ap_environment_t* env, double inf, double sup)
{
  return ap_texpr1_of_texpr0(env, ap_texpr0_cst_interval_double(inf,sup));
}

ap_texpr1_t* ap_texpr1_cst_interval_top    (ap_environment_t* env)
{
  return ap_texpr1_of_texpr0(env, ap_texpr0_cst_interval_top());
}

ap_texpr1_t* ap_texpr1_var             (ap_environment_t* env, ap_var_t var)
{
  ap_dim_t dim = ap_environment_dim_of_var(env, var);
  if (dim==AP_DIM_MAX)
    return NULL;
  else
    return ap_texpr1_of_texpr0(env, ap_texpr0_dim(dim));
}

ap_texpr1_t* ap_texpr1_node(ap_texpr_op_t op, ap_texpr_rtype_t type, ap_texpr_rdir_t dir, ap_texpr1_t* opA, ap_texpr1_t* opB)
{
  if (opB!=NULL && !ap_environment_is_eq(opA->env, opB->env))
    return NULL;

  ap_texpr0_t* texpr0 = ap_texpr0_node(op,type,dir,opA->texpr0,opB ? opB->texpr0 : NULL);
  ap_texpr1_t* texpr1 = ap_texpr1_of_texpr0(opA->env,texpr0);
  ap_environment_free(opA->env);
  free(opA);
  if (opB){
    ap_environment_free(opB->env);
    free(opB);
  }
  return texpr1;
}
ap_texpr1_t* ap_texpr1_unop(ap_texpr_op_t op, ap_texpr1_t* opA, ap_texpr_rtype_t type, ap_texpr_rdir_t dir)
{
  return ap_texpr1_node(op,type,dir,opA,NULL);
}
ap_texpr1_t* ap_texpr1_binop(ap_texpr_op_t op, ap_texpr1_t* opA, ap_texpr1_t* opB, ap_texpr_rtype_t type, ap_texpr_rdir_t dir)
{
  return ap_texpr1_node(op,type,dir,opA,opB);
}

ap_texpr1_t* ap_texpr1_copy(ap_texpr1_t* expr)
{
  if (!expr) return NULL;
  return ap_texpr1_of_texpr0(expr->env,ap_texpr0_copy(expr->texpr0));
}
void ap_texpr1_free(ap_texpr1_t* expr)
{
  if (!expr) return;
  ap_environment_free(expr->env);
  ap_texpr0_free(expr->texpr0);
  free(expr);
}
ap_texpr1_t* ap_texpr1_from_linexpr1(ap_linexpr1_t* e)
{
  return ap_texpr1_of_texpr0(e->env, ap_texpr0_from_linexpr0(e->linexpr0));
}

/* ====================================================================== */
/* II. Printing */
/* ====================================================================== */

void ap_texpr1_fprint(FILE* stream, ap_texpr1_t* a)
{
  if (!a) return;
  ap_environment_name_of_dim_t* name_of_dim;

  name_of_dim = ap_environment_name_of_dim_alloc(a->env);
  ap_texpr0_fprint(stream,a->texpr0,name_of_dim->p);
  ap_environment_name_of_dim_free(name_of_dim);
}
void ap_texpr1_print(ap_texpr1_t* a)
{ ap_texpr1_fprint(stdout, a); }

/* ====================================================================== */
/* III. Tests, size */
/* ====================================================================== */

bool ap_texpr1_has_var(ap_texpr1_t* a, ap_var_t var)
{
  if (!a) return false;
  ap_dim_t dim = ap_environment_dim_of_var(a->env, var);
  if (dim==AP_DIM_MAX)
    return false;
  else
    return ap_texpr0_has_dim(a->texpr0, dim);
}

/* ====================================================================== */
/* IV. Operations */
/* ====================================================================== */

ap_texpr1_t* ap_texpr1_substitute(ap_texpr1_t* a, ap_var_t var, ap_texpr1_t *dst)
{
  if (!a || !dst || !ap_environment_is_eq(a->env, dst->env))
    return NULL;
  ap_dim_t dim = ap_environment_dim_of_var(a->env,var);
  if (dim==AP_DIM_MAX)
    return NULL;
  return ap_texpr1_of_texpr0(a->env,
			     ap_texpr0_substitute(a->texpr0,dim,dst->texpr0));
}

/* Return true if var is unknown, or incompatible environment */
bool ap_texpr1_substitute_with(ap_texpr1_t* a, ap_var_t var, ap_texpr1_t *dst)
{
  if (!a || !dst || !ap_environment_is_eq(a->env, dst->env))
    return true;
  ap_dim_t dim = ap_environment_dim_of_var(a->env,var);
  if (dim==AP_DIM_MAX)
    return true;
  ap_texpr0_substitute_with(a->texpr0,dim,dst->texpr0);
  return false;
}

/* ====================================================================== */
/* V. Change of dimensions and permutations */
/* ====================================================================== */

ap_texpr1_t* ap_texpr1_extend_environment_dimchange(ap_texpr1_t* expr,
						   ap_environment_t* nenv,
						   ap_dimchange_t* dimchange)
{
  return ap_texpr1_of_texpr0(nenv,ap_texpr0_add_dimensions(expr->texpr0,dimchange));
}
void ap_texpr1_extend_environment_dimchange_with(ap_texpr1_t* expr,
						 ap_environment_t* nenv,
						 ap_dimchange_t* dimchange)
{
  ap_environment_t* env;
  ap_texpr0_add_dimensions_with(expr->texpr0,dimchange);
  env = expr->env;
  expr->env = ap_environment_copy(nenv);
  ap_environment_free(env);
}
ap_texpr1_t* ap_texpr1_extend_environment(ap_texpr1_t* expr,
					  ap_environment_t* nenv)
{
  ap_dimchange_t* dimchange = ap_environment_dimchange(expr->env,nenv);
  if (dimchange==NULL)
    return NULL;
  ap_texpr1_t* res = ap_texpr1_extend_environment_dimchange(expr,nenv,dimchange);
  ap_dimchange_free(dimchange);
  return res;
}
bool ap_texpr1_extend_environment_with(ap_texpr1_t* expr,
				       ap_environment_t* nenv)
{
  ap_dimchange_t* dimchange = ap_environment_dimchange(expr->env,nenv);
  if (dimchange==NULL)
    return true;
  ap_texpr1_extend_environment_dimchange_with(expr,nenv,dimchange);
  ap_dimchange_free(dimchange);
  return false;
}

bool ap_texpr1_equal(ap_texpr1_t* a1, ap_texpr1_t* a2)
{
  if (!a1 && !a2) return true;
  if (!a1 || !a2) return false;
  if (!ap_environment_is_eq(a1->env,a2->env))
    return false;
  else
    return ap_texpr0_equal(a1->texpr0,a2->texpr0);
}
