/* ************************************************************************* */
/* ap_linyyy1.h: linear expressions or constraints */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

/* normally included from ap_expr1.h */

#ifndef _AP_linyyy1_H_
#define _AP_linyyy1_H_

#include <stdio.h>
#include "ap_linyyy0.h"
#include "ap_environment.h"
#ifndef _AP_linexpr1_H_
#include "ap_linexpr1.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define _AP_linyyy1_MARK_

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* Linear expressions */
typedef struct ap_linyyy1_struct {
  ap_linyyy0_t linyyy0;
  ap_environment_t* env;
} ap_linyyy1_struct;
typedef ap_linyyy1_struct ap_linyyy1_t[1];
typedef ap_linyyy1_struct* ap_linyyy1_ptr;

/* Array of linear expressions */
typedef struct ap_linyyy1_array_struct {
  ap_linyyy0_array_t linyyy0_array;
  ap_environment_t* env;
} ap_linyyy1_array_struct;
typedef ap_linyyy1_array_struct* ap_linyyy1_array_ptr;
typedef ap_linyyy1_array_struct ap_linyyy1_array_t[1];

/* ********************************************************************** */
/* I. ap_linyyy1_t */
/* ********************************************************************** */


/* ====================================================================== */
/* I.1 Memory management and printing */
/* ====================================================================== */

void ap_linyyy1_init(ap_linyyy1_t res, ap_scalar_discr_t discr, size_t size, ap_environment_t* env);
void ap_linyyy1_init_set(ap_linyyy1_t res, ap_linyyy1_t e);
void ap_linyyy1_init_set_linyyy0(ap_linyyy1_t res, ap_linyyy0_t e, ap_environment_t* env);
MACRO_FOREACH ZZZ @MainNum
void ap_linyyy1_init_set_linyyyZZZ(ap_linyyy1_t res, ap_linyyyZZZ_t e, ap_environment_t* env);
ENDMACRO
void ap_linyyy1_clear(ap_linyyy1_t e);
void ap_linyyy1_fprint(FILE* stream, ap_linyyy1_t e);
void ap_linyyy1_minimize(ap_linyyy1_t a);

/* ====================================================================== */
/* I.2 Conversions */
/* ====================================================================== */

bool ap_linyyy1_set(ap_linyyy1_t res, ap_linyyy1_t e, num_internal_t intern);
bool ap_linyyy1_set_linyyy0(ap_linyyy1_t res, ap_linyyy0_t e, ap_environment_t* env, num_internal_t intern);
MACRO_FOREACH ZZZ @MainNum
bool ap_linyyy1_set_linyyyZZZ(ap_linyyy1_t a, ap_linyyyZZZ_t b, ap_environment_t* env, num_internal_t intern);
bool ap_linyyyZZZ_set_linyyy1(ap_linyyyZZZ_t a, ap_linyyy1_t b, num_internal_t intern);
ENDMACRO

/* ====================================================================== */
/* I.3 Access */
/* ====================================================================== */

static inline
ap_scalar_discr_t ap_linyyy1_discr(ap_linyyy1_t expr);
static inline
ap_environment_t* ap_linyyy1_envref(ap_linyyy1_t expr);
  /* Get a reference to the environment. Do not free it. */
static inline
ap_linyyy0_ptr ap_linyyy1_linyyy0ref(ap_linyyy1_t e);
  /* Get a reference to the underlying linear expression of level
     0. Do not free it. */

bool ap_linyyy1_get_cst(ap_coeff_t coeff, ap_linyyy1_t expr, num_internal_t intern);
  /* Get the constant and assign it to coeff with possible conversion */
bool ap_linyyy1_get_coeff(ap_coeff_t coeff, bool* perror, ap_linyyy1_t expr, ap_var_t var, num_internal_t intern);
  /* Get coefficient of dimension dim in the expression and assign it to
     coeff with possible conversion. */

bool ap_linyyy1_set_cst(ap_linyyy1_t expr, ap_coeff_t coeff, num_internal_t intern);
  /* Assign the constant with possible conversion. */
  bool ap_linyyy1_set_coeff(ap_linyyy1_t expr, bool* perror, ap_var_t var, ap_coeff_t coeff, num_internal_t intern);
  /* Assign the coefficient of dimension dim in the expression, with possible
     conversion. */

void ap_linyyy1_cstref(ap_coeff_t coeff, ap_linyyy1_t expr);
void ap_linyyy1_coeffref(ap_coeff_t coeff, bool* perror, ap_linyyy1_t expr, ap_var_t var);

bool ap_linyyy1_set_list(num_internal_t intern, ap_linyyy1_t expr, bool* perror, ...);
  /* This function assigns the linear expression from a list of tags of type
     itv_coefftag_t, each followed by a number of arguments as specified in
     the definition of the type ap_coeff_tag_t_t, and ended by the tag AP_END;

     - The dimension AP_COEFF_DIM_MAX/AP_DIM_MAX is used to refer to the constat coefficient.
     - If the same dimension appears several times, only the last tag
       referring to it is taken into account.

     Returns true iff all conversions were exact.

     Example:
     ap_linyyy1_set_list(intern,
			  expr,
			  AP_COEFF_LFRAC,7,9,0,
			  AP_COEFF_DOUBLE2,-3.0,4.5,1,
			  AP_COEFF_LLINT,3LL,AP_COEFF_DIM_MAX,
			  AP_END)
     sets expr to "7/9 x0 + [-3,4.5] x1 + 3"
     assuming that the expression was "0" before the call and that all the
     number conversions were exact.
  */

#if defined (_AP_lincons1_MARK_)
void ap_lincons1_get_mpq(mpq_t mpq, ap_lincons1_t c);
mpq_ptr ap_lincons1_mpqref(ap_lincons1_t c);
void ap_lincons1_set_mpq(ap_lincons1_t c, mpq_t mpq);
#endif

#if defined(_AP_lincons1_MARK_) || defined (_AP_lingen1_MARK_)

bool ap_linyyy1_get_linexpr1(ap_linexpr1_t e, ap_linyyy1_t c, num_internal_t intern);
  /* Get the underlying expression and assign it to e with possible
     conversion */
ap_yyytyp_t ap_linyyy1_get_yyytyp(ap_linyyy1_t c);

void ap_linyyy1_linexpr1ref(ap_linexpr1_t e, ap_linyyy1_t c);
ap_yyytyp_t* ap_linyyy1_yyytypref(ap_linyyy1_t c);

bool ap_linyyy1_set_linexpr1(ap_linyyy1_t c, ap_linexpr1_t e, num_internal_t intern);
  /* Assign the underlying expression of c to e with possible conversion */
void ap_linyyy1_set_yyytyp(ap_linyyy1_t c, ap_yyytyp_t yyytyp);

#endif

/* ====================================================================== */
/* I.4 Change of dimensions and permutations */
/* ====================================================================== */

void ap_linyyy1_extend_environment(ap_linyyy1_t nexpr,
				   bool* perror,
				   ap_linyyy1_t expr,
				   ap_environment_t* nenv);

/* ********************************************************************** */
/* II. ap_lincons1_array_t */
/* ********************************************************************** */

/* ====================================================================== */
/* II.1 Memory management and printing */
/* ====================================================================== */

void ap_linyyy1_array_init(ap_linyyy1_array_t e, ap_scalar_discr_t discr, size_t size, ap_environment_t* env);
void ap_linyyy1_array_init_set(ap_linyyy1_array_t res, ap_linyyy1_array_t e);
void ap_linyyy1_array_init_set_linyyy0_array(ap_linyyy1_array_t res, ap_linyyy0_array_t e, ap_environment_t* env);
MACRO_FOREACH ZZZ @MainNum
void ap_linyyy1_array_init_set_linyyyZZZ_array(ap_linyyy1_array_t res, ap_linyyyZZZ_array_t e, ap_environment_t* env);
ENDMACRO
void ap_linyyy1_array_minimize(ap_linyyy1_array_t a);
void ap_linyyy1_array_clear(ap_linyyy1_array_t e);
void ap_linyyy1_array_fprint(FILE* stream, ap_linyyy1_array_t e);

/* ====================================================================== */
/* II.2 Access, possibly with conversions */
/* ====================================================================== */

static inline
ap_scalar_discr_t ap_linyyy1_discr(ap_linyyy1_t expr);
static inline
ap_environment_t* ap_linyyy1_envref(ap_linyyy1_t expr);
  /* Get a reference to the environment. Do not free it. */
static inline
ap_linyyy0_ptr ap_linyyy1_linyyy0ref(ap_linyyy1_t e);
  /* Get a reference to the underlying linear expression of level
     0. Do not free it. */


static inline ap_scalar_discr_t ap_linyyy1_array_discr(ap_linyyy1_array_t p);
size_t ap_linyyy1_array_size(ap_linyyy1_array_t p);

#if !defined(_AP_lingen1_ARRAY_H_)
ap_linexpr_type_t ap_linyyy1_array_type(ap_linyyy1_array_t array);
bool ap_linyyy1_array_is_quasilinear(ap_linyyy1_array_t array);
bool ap_linyyy1_array_is_linear(ap_linyyy1_array_t array);
  /* Are all the constraints involved linear (resp. quasilinear) */
#endif

bool ap_linyyy1_array_set(ap_linyyy1_array_t res, ap_linyyy1_array_t e, num_internal_t intern);
bool ap_linyyy1_array_set_index(ap_linyyy1_array_t p, bool* perror, size_t i, ap_linyyy1_t a, num_internal_t intern);
bool ap_linyyy1_array_get_index(ap_linyyy1_t a, ap_linyyy1_array_t p, size_t i, num_internal_t intern);
void ap_linyyy1_array_ref_index(ap_linyyy1_t a, ap_linyyy1_array_t p, size_t i);

bool ap_linyyy1_array_set_linyyy0_array(ap_linyyy1_array_t res, ap_linyyy0_array_t e, ap_environment_t* env, num_internal_t intern);

MACRO_FOREACH XXX @AllNum
bool ap_linyyy1_array_set_linyyyXXX_array(ap_linyyy1_array_t a, ap_linyyyXXX_array_t b, ap_environment_t* env, num_internal_t intern);
bool ap_linyyyXXX_array_set_linyyy1_array(ap_linyyyXXX_array_t a, ap_linyyy1_array_t b, num_internal_t intern);

bool ap_linyyy1_array_set_index_linyyyXXX(ap_linyyy1_array_t p, size_t i, ap_linyyyXXX_t a, num_internal_t intern);
bool ap_linyyy1_array_get_index_linyyyXXX(ap_linyyyXXX_t a, ap_linyyy1_array_t p, size_t i, num_internal_t intern);
ENDMACRO

/* ====================================================================== */
/* II.3 Change of dimensions and permutations */
/* ====================================================================== */

void ap_linyyy1_array_extend_environment(ap_linyyy1_array_t nexpr,
					 bool* perror,
					 ap_linyyy1_array_t expr,
					 ap_environment_t* nenv);

/* ********************************************************************** */
/* III. Inline definitions */
/* ********************************************************************** */

static inline
ap_scalar_discr_t ap_linyyy1_discr(ap_linyyy1_t expr)
{ return expr->linyyy0->discr; }
static inline
ap_environment_t* ap_linyyy1_envref(ap_linyyy1_t e)
{ return e->env; }
static inline
ap_linyyy0_ptr ap_linyyy1_linyyy0ref(ap_linyyy1_t e)
{ return e->linyyy0; }

static inline void ap_linyyy1_cons(ap_linyyy1_t res, ap_linyyy0_t e, ap_environment_t* env)
{
  ap_linyyy0_cons(res->linyyy0,e);
  res->env = env;
}
MACRO_FOREACH ZZZ @MainNum
static inline void ap_linyyy1_cons_ZZZ(ap_linyyy1_t res, ap_linyyyZZZ_t e, ap_environment_t* env)
{
  res->linyyy0->discr = AP_SCALAR_ZZZ;
  res->linyyy0->linyyy.ZZZ = e;
  res->env = env;
}
ENDMACRO

static inline ap_scalar_discr_t ap_linyyy1_array_discr(ap_linyyy1_array_t p)
{ return p->linyyy0_array->discr; }
static inline
ap_environment_t* ap_linyyy1_array_envref(ap_linyyy1_array_t e)
{ return e->env; }
static inline
ap_linyyy0_array_ptr ap_linyyy1_array_linyyy0_arrayref(ap_linyyy1_array_t e)
{ return e->linyyy0_array; }

MACRO_FOREACH ZZZ @MainNum
static inline void ap_linyyy1_array_cons_ZZZ(ap_linyyy1_array_t res, ap_linyyyZZZ_array_t e, ap_environment_t* env)
{
  res->linyyy0_array->discr = AP_SCALAR_ZZZ;
  res->linyyy0_array->linyyy_array.ZZZ = e;
  res->env = env;
};
ENDMACRO



#undef _AP_linyyy1_MARK_

#ifdef __cplusplus
}
#endif

#endif
