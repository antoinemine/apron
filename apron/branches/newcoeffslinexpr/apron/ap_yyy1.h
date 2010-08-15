/* ************************************************************************* */
/* ap_yyy1.h: linear expressions or constraints */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

/* normally included from ap_expr1.h */

#ifndef _AP_yyy1_H_
#define _AP_yyy1_H_

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "ap_yyy0.h"
#include "ap_environment.h"
#ifndef _AP_linexpr1_H_
#include "ap_linexpr1.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define _AP_yyy1_MARK_

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* Linear expressions */
typedef struct ap_yyy1_struct {
  ap_yyy0_t yyy0;
  ap_environment_t* env;
} ap_yyy1_struct;
typedef ap_yyy1_struct ap_yyy1_t[1];
typedef ap_yyy1_struct* ap_yyy1_ptr;

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_yyy1_init(ap_yyy1_t res, ap_scalar_discr_t discr, size_t size, ap_environment_t* env);
void ap_yyy1_init_set(ap_yyy1_t res, ap_yyy1_t e);
void ap_yyy1_init_set_linexpr0(ap_yyy1_t res, ap_yyy0_t e, ap_environment_t* env);
MACRO_MAINZ
void ap_yyy1_init_set_yyyZZZ(ap_yyy1_t res, ap_yyyZZZ_t e, ap_environment_t* env);
ENDMACRO
void ap_yyy1_clear(ap_yyy1_t e);
void ap_yyy1_fprint(FILE* stream, ap_yyy1_t e);
void ap_yyy1_minimize(ap_yyy1_t a);

/* ====================================================================== */
/* II. Conversions */
/* ====================================================================== */

bool ap_yyy1_set(ap_yyy1_t res, ap_yyy1_t e, num_internal_t intern);
bool ap_yyy1_set_yyy0(ap_yyy1_t res, ap_yyy0_t e, ap_environment_t* env, num_internal_t intern);
MACRO_MAINZ
bool ap_yyy1_set_yyyZZZ(ap_yyy1_t a, ap_yyyZZZ_t b, ap_environment_t* env, num_internal_t intern);
bool ap_yyyZZZ_set_yyy1(ap_yyyZZZ_t a, ap_yyy1_t b, num_internal_t intern);
ENDMACRO

/* ====================================================================== */
/* III. Access */
/* ====================================================================== */

static inline
ap_environment_t* ap_yyy1_envref(ap_yyy1_t expr);
  /* Get a reference to the environment. Do not free it. */
static inline 
ap_yyy0_ptr ap_yyy1_yyy0ref(ap_yyy1_t e);
  /* Get a reference to the underlying linear expression of level
     0. Do not free it. */

#if defined(_AP_linexpr1_MARK_)
bool ap_linexpr1_get_cst(ap_coeff_t coeff, ap_linexpr1_t expr, num_internal_t intern);
  /* Get the constant and assign it to coeff with possible conversion */
  bool ap_linexpr1_get_coeff(ap_coeff_t coeff, bool* perror, ap_linexpr1_t expr, ap_var_t var, num_internal_t intern);
  /* Get coefficient of dimension dim in the expression and assign it to
     coeff with possible conversion. */

bool ap_linexpr1_set_cst(ap_linexpr1_t expr, ap_coeff_t coeff, num_internal_t intern);
  /* Assign the constant with possible conversion. */
  bool ap_linexpr1_set_coeff(ap_linexpr1_t expr, bool* perror, ap_var_t var, ap_coeff_t coeff, num_internal_t intern);
  /* Assign the coefficient of dimension dim in the expression, with possible
     conversion. */

void ap_linexpr1_cstref(ap_coeff_t coeff, ap_linexpr1_t expr);
void ap_linexpr1_coeffref(ap_coeff_t coeff, bool* perror, ap_linexpr1_t expr, ap_var_t var);

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
				bool* perror,
				ap_yyy1_t expr,
				ap_environment_t* nenv);

static inline
ap_environment_t* ap_yyy1_envref(ap_yyy1_t e)
{ return e->env; }
static inline
ap_yyy0_ptr ap_yyy1_yyy0ref(ap_yyy1_t e)
{ return e->yyy0; }

static inline void ap_yyy1_cons(ap_yyy1_t res, ap_yyy0_t e, ap_environment_t* env)
{
  ap_yyy0_cons(res->yyy0,e);
  res->env = env;
}
MACRO_MAINZ
static inline void ap_yyy1_cons_ZZZ(ap_yyy1_t res, ap_yyyZZZ_t e, ap_environment_t* env)
{
  res->yyy0->discr = AP_SCALAR_ZZZ;
  res->yyy0->yyy.ZZZ = e;
  res->env = env;
}
ENDMACRO

#undef _AP_yyy1_MARK_

#ifdef __cplusplus
}
#endif

#endif
