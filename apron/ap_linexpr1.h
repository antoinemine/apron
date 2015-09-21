/* ************************************************************************* */
/* ap_linexpr1.h: linear expressions */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

/* normally included from ap_expr1.h */

#ifndef _AP_LINEXPR1_H_
#define _AP_LINEXPR1_H_

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ap_coeff.h"
#include "ap_environment.h"
#include "ap_linexpr0.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* Linear expressions */
typedef struct ap_linexpr1_t {
  ap_linexpr0_t* linexpr0;
  ap_environment_t* env;
} ap_linexpr1_t;


/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

/* For internal use */
static inline
ap_linexpr1_t ap_linexpr1_of_linexpr0(ap_environment_t* env, ap_linexpr0_t* linexpr0)
{
  ap_linexpr1_t res;
  res.linexpr0 = linexpr0;
  res.env = ap_environment_copy(env);
  return res;
}

ap_linexpr1_t ap_linexpr1_make(ap_environment_t* env,
			       ap_linexpr_discr_t lin_discr, size_t size);
  /* Build a linear expressions with by default coefficients
     of type SCALAR and DOUBLE.

     - If lin_discr selects a dense representation, the size of
       the expression is the size of the environment.

     - Otherwise, the initial size is given by size and the
       expression may be resized lazily.  */

static inline
void ap_linexpr1_minimize(ap_linexpr1_t* e);
  /* In case of sparse representation, remove zero coefficients */

ap_linexpr1_t ap_linexpr1_copy(ap_linexpr1_t* e);
  /* Duplication */

void ap_linexpr1_clear(ap_linexpr1_t* linexpr);
  /* Clear the linear expression:
     - Free the linear expression of level 0
     - Dereference the environment
     - Set fields to NULL
  */

void ap_linexpr1_fprint(FILE* stream, ap_linexpr1_t* a);
  /* Printing a linear expression */

/* ====================================================================== */
/* II. Tests */
/* ====================================================================== */

static inline
bool ap_linexpr1_is_integer(ap_linexpr1_t* e);
  /* Does the expression depends only on integer variables ? */
static inline
bool ap_linexpr1_is_real(ap_linexpr1_t* e);
  /* Does the expression depends only on real variables ? */

static inline
ap_linexpr_type_t ap_linexpr1_type(ap_linexpr1_t* a);
  /* Return the type of the linear expression */
static inline
bool ap_linexpr1_is_linear(ap_linexpr1_t* e);
  /* Return true iff all involved coefficients are scalars */
static inline
bool ap_linexpr1_is_quasilinear(ap_linexpr1_t* e);
  /* Return true iff all involved coefficients but the constant are scalars */

/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

static inline
ap_environment_t* ap_linexpr1_envref(ap_linexpr1_t* expr);
  /* Get a reference to the environment. Do not free it. */
static inline
ap_linexpr0_t* ap_linexpr1_linexpr0ref(ap_linexpr1_t* e);
  /* Get a reference to the underlying linear expression of level
     0. Do not free it. */


static inline
ap_coeff_t* ap_linexpr1_cstref(ap_linexpr1_t* expr);
  /* Get a reference to the constant. Do not free it. */

ap_coeff_t* ap_linexpr1_coeffref(ap_linexpr1_t* expr, ap_var_t var);
  /* Get a reference to the coefficient associated to the variable.
     Do not free it.
     In case of sparse representation,
     possibly induce the addition of a new linear term.
     Return NULL if var is unknown in the environment.
 */

/* Get the constant and assign it to coeff */
static inline
void ap_linexpr1_get_cst(ap_coeff_t* coeff, ap_linexpr1_t* expr);

/* Get coefficient of variable var in the expression and assign it to coeff.
   Return true if var is unknown in the environment */
bool ap_linexpr1_get_coeff(ap_coeff_t* coeff, ap_linexpr1_t* expr, ap_var_t var);

/* Set the constant of the linear expression */
static inline void ap_linexpr1_set_cst(ap_linexpr1_t* expr, ap_coeff_t* cst);
static inline void ap_linexpr1_set_cst_scalar(ap_linexpr1_t* expr, ap_scalar_t* scalar);
static inline void ap_linexpr1_set_cst_scalar_int(ap_linexpr1_t* expr, int num);
static inline void ap_linexpr1_set_cst_scalar_frac(ap_linexpr1_t* expr, int num, unsigned int den);
static inline void ap_linexpr1_set_cst_scalar_double(ap_linexpr1_t* expr, double num);
static inline void ap_linexpr1_set_cst_interval(ap_linexpr1_t* expr, ap_interval_t* itv);
static inline void ap_linexpr1_set_cst_interval_scalar(ap_linexpr1_t* expr, ap_scalar_t* inf, ap_scalar_t* sup);
static inline void ap_linexpr1_set_cst_interval_int(ap_linexpr1_t* expr, int inf, int sup);
static inline void ap_linexpr1_set_cst_interval_frac(ap_linexpr1_t* expr,
							 int numinf, unsigned int deninf,
							 int numsup, unsigned int densup);
static inline void ap_linexpr1_set_cst_interval_double(ap_linexpr1_t* expr, double inf, double sup);

/* Set the coefficient of variable var in the expression.
   Return true if var is unknown in the environment */
static inline bool ap_linexpr1_set_coeff(ap_linexpr1_t* expr, ap_var_t var, ap_coeff_t* coeff);
static inline bool ap_linexpr1_set_coeff_scalar(ap_linexpr1_t* expr, ap_var_t var, ap_scalar_t* scalar);
static inline bool ap_linexpr1_set_coeff_scalar_int(ap_linexpr1_t* expr, ap_var_t var, int num);
static inline bool ap_linexpr1_set_coeff_scalar_frac(ap_linexpr1_t* expr, ap_var_t var, int num, unsigned int den);
static inline bool ap_linexpr1_set_coeff_scalar_double(ap_linexpr1_t* expr, ap_var_t var, double num);
static inline bool ap_linexpr1_set_coeff_interval(ap_linexpr1_t* expr, ap_var_t var, ap_interval_t* itv);
static inline bool ap_linexpr1_set_coeff_interval_scalar(ap_linexpr1_t* expr, ap_var_t var, ap_scalar_t* inf, ap_scalar_t* sup);
static inline bool ap_linexpr1_set_coeff_interval_int(ap_linexpr1_t* expr, ap_var_t var, int inf, int sup);
static inline bool ap_linexpr1_set_coeff_interval_frac(ap_linexpr1_t* expr, ap_var_t var,
							   int numinf, unsigned int deninf,
							   int numsup, unsigned int densup);
static inline bool ap_linexpr1_set_coeff_interval_double(ap_linexpr1_t* expr, ap_var_t var, double inf, double sup);

/*
bool ap_linexpr1_set_format(ap_linexpr1_t* expr, char* fmt, ...);
*/

bool ap_linexpr1_set_list(ap_linexpr1_t* expr, ...);


/* Iterator (Macro): use:
   ap_linexpr1_ForeachLinterm(ap_linexpr1_t* e, size_t i, ap_var_t v, ap_coeff_t* coeff){
     ..
   }
   where
   - e is the inspected expression,
   - i is the internal iterator
   - var is the variable of one linear term
   - coeff is a pointer to the corresponding coefficient
*/
#define ap_linexpr1_ForeachLinterm1(_p_e_, _p_i_, _p_var_, _p_ap_coeff_) \
  for ((_p_i_)=0;							\
       (_p_i_)<(_p_e_)->linexpr0->size ?						\
	 ((_p_e_)->linexpr0->discr==AP_LINEXPR_DENSE ?				\
	  ((_p_var_) = ap_environment_var_of_dim((_p_e_)->env,(_p_i_)),	\
	   (_p_ap_coeff_) = &(_p_e_)->linexpr0->p.coeff[(_p_i_)],			\
	   true) :							\
	  ((_p_var_) = ap_environment_var_of_dim((_p_e_)->env,\
					      (_p_e_)->linexpr0->p.linterm[(_p_i_)].dim),	\
	   (_p_ap_coeff_) = &(_p_e_)->linexpr0->p.linterm[(_p_i_)].coeff,		\
	   (_p_var_)!=NULL)) :						\
	 false;								\
       (_p_i_)++)

/* ====================================================================== */
/* IV. Change of dimensions and permutations */
/* ====================================================================== */

/* Change current environment with a super-environment.
   Return true if nenv is not a superenvironment  */
bool ap_linexpr1_extend_environment(ap_linexpr1_t* nexpr,
				 ap_linexpr1_t* expr,
				 ap_environment_t* nenv);
bool ap_linexpr1_extend_environment_with(ap_linexpr1_t* expr,
				      ap_environment_t* nenv);

/* ====================================================================== */
/* V. Inline function definitions */
/* ====================================================================== */

static inline
void ap_linexpr1_minimize(ap_linexpr1_t* e){
  ap_linexpr0_minimize(e->linexpr0);
}
static inline
bool ap_linexpr1_is_integer(ap_linexpr1_t* e){
  return ap_linexpr0_is_integer(e->linexpr0,e->env->intdim);
}
static inline
bool ap_linexpr1_is_real(ap_linexpr1_t* e){
  return ap_linexpr0_is_real(e->linexpr0,e->env->intdim);
}
static inline
ap_linexpr_type_t ap_linexpr1_type(ap_linexpr1_t* e){
  return ap_linexpr0_type(e->linexpr0);
}
static inline
bool ap_linexpr1_is_linear(ap_linexpr1_t* e){
  return ap_linexpr0_is_linear(e->linexpr0);
}
static inline
bool ap_linexpr1_is_quasilinear(ap_linexpr1_t* e){
  return ap_linexpr0_is_quasilinear(e->linexpr0);
}

static inline
ap_environment_t* ap_linexpr1_envref(ap_linexpr1_t* e)
  { return e->env; }
static inline
ap_linexpr0_t* ap_linexpr1_linexpr0ref(ap_linexpr1_t* e)
  { return e->linexpr0; }

static inline
ap_coeff_t* ap_linexpr1_cstref(ap_linexpr1_t* expr){
  return &expr->linexpr0->cst;
}
static inline
void ap_linexpr1_get_cst(ap_coeff_t* cst, ap_linexpr1_t* expr){
 ap_coeff_set(cst, &expr->linexpr0->cst);
}

static inline
void ap_linexpr1_set_cst(ap_linexpr1_t* expr, ap_coeff_t* cst)
  { ap_coeff_set(&expr->linexpr0->cst,cst); }
static inline
void ap_linexpr1_set_cst_scalar(ap_linexpr1_t* expr, ap_scalar_t* scalar)
  { ap_coeff_set_scalar(&expr->linexpr0->cst, scalar); }
static inline
void ap_linexpr1_set_cst_scalar_int(ap_linexpr1_t* expr, int num)
  { ap_coeff_set_scalar_int(&expr->linexpr0->cst, num); }
static inline
void ap_linexpr1_set_cst_scalar_frac(ap_linexpr1_t* expr, int num, unsigned int den)
  { ap_coeff_set_scalar_frac(&expr->linexpr0->cst, num, den); }
static inline
void ap_linexpr1_set_cst_scalar_double(ap_linexpr1_t* expr, double num)
  { ap_coeff_set_scalar_double(&expr->linexpr0->cst, num); }
static inline
void ap_linexpr1_set_cst_interval(ap_linexpr1_t* expr, ap_interval_t* itv)
  { ap_coeff_set_interval(&expr->linexpr0->cst, itv); }
static inline
void ap_linexpr1_set_cst_interval_int(ap_linexpr1_t* expr, int inf, int sup)
  { ap_coeff_set_interval_int(&expr->linexpr0->cst, inf,sup); }
static inline
void ap_linexpr1_set_cst_interval_scalar(ap_linexpr1_t* expr, ap_scalar_t* inf, ap_scalar_t* sup)
  { ap_coeff_set_interval_scalar(&expr->linexpr0->cst, inf,sup); }
static inline
void ap_linexpr1_set_cst_interval_frac(ap_linexpr1_t* expr,
				  int numinf, unsigned int deninf,
				  int numsup, unsigned int densup)
  { ap_coeff_set_interval_frac(&expr->linexpr0->cst, numinf,deninf, numsup,densup); }
static inline
void ap_linexpr1_set_cst_interval_double(ap_linexpr1_t* expr, double inf, double sup)
  { ap_coeff_set_interval_double(&expr->linexpr0->cst, inf,sup); }

static inline
bool ap_linexpr1_set_coeff(ap_linexpr1_t* expr, ap_var_t var, ap_coeff_t* coeff)
  { ap_coeff_t* ecoeff = ap_linexpr1_coeffref(expr,var); if (ecoeff){ap_coeff_set(ecoeff,coeff); return false;} else return true; }
static inline
bool ap_linexpr1_set_coeff_scalar(ap_linexpr1_t* expr, ap_var_t var, ap_scalar_t* scalar)
  { ap_coeff_t* ecoeff = ap_linexpr1_coeffref(expr,var); if (ecoeff){ ap_coeff_set_scalar(ecoeff,scalar); return false; } else return true; }
static inline
bool ap_linexpr1_set_coeff_scalar_int(ap_linexpr1_t* expr, ap_var_t var, int num)
  { ap_coeff_t* ecoeff = ap_linexpr1_coeffref(expr,var); if (ecoeff){ ap_coeff_set_scalar_int(ecoeff,num); return false; } else return true; }
static inline
bool ap_linexpr1_set_coeff_scalar_frac(ap_linexpr1_t* expr, ap_var_t var, int num, unsigned int den)
  { ap_coeff_t* ecoeff = ap_linexpr1_coeffref(expr,var); if (ecoeff){ ap_coeff_set_scalar_frac(ecoeff,num, den); return false; } else return true; }
static inline
bool ap_linexpr1_set_coeff_scalar_double(ap_linexpr1_t* expr, ap_var_t var, double num)
  { ap_coeff_t* ecoeff = ap_linexpr1_coeffref(expr,var); if (ecoeff){ ap_coeff_set_scalar_double(ecoeff,num); return false; } else return true; }
static inline
bool ap_linexpr1_set_coeff_interval(ap_linexpr1_t* expr, ap_var_t var, ap_interval_t* itv)
  { ap_coeff_t* ecoeff = ap_linexpr1_coeffref(expr,var); if (ecoeff){ ap_coeff_set_interval(ecoeff,itv); return false; } else return true; }
static inline
bool ap_linexpr1_set_coeff_interval_int(ap_linexpr1_t* expr, ap_var_t var, int inf, int sup)
  { ap_coeff_t* ecoeff = ap_linexpr1_coeffref(expr,var); if (ecoeff){ ap_coeff_set_interval_int(ecoeff,inf,sup); return false; } else return true; }
static inline
bool ap_linexpr1_set_coeff_interval_scalar(ap_linexpr1_t* expr, ap_var_t var, ap_scalar_t* inf, ap_scalar_t* sup)
  { ap_coeff_t* ecoeff = ap_linexpr1_coeffref(expr,var); if (ecoeff){ ap_coeff_set_interval_scalar(ecoeff,inf,sup); return false; } else return true; }
static inline
bool ap_linexpr1_set_coeff_interval_frac(ap_linexpr1_t* expr, ap_var_t var,
				  int numinf, unsigned int deninf,
				  int numsup, unsigned int densup)
  { ap_coeff_t* ecoeff = ap_linexpr1_coeffref(expr,var); if (ecoeff){ ap_coeff_set_interval_frac(ecoeff,numinf,deninf, numsup,densup); return false; } else return true; }
static inline
bool ap_linexpr1_set_coeff_interval_double(ap_linexpr1_t* expr, ap_var_t var, double inf, double sup)
  { ap_coeff_t* ecoeff = ap_linexpr1_coeffref(expr,var); if (ecoeff){ ap_coeff_set_interval_double(ecoeff,inf,sup); return false; } else return true; }

#ifdef __cplusplus
}
#endif

#endif
