/* ************************************************************************* */
/* ap_tzzz1.h: tree expressions */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include <stdarg.h>

#ifndef _AP_Tzzz1_H_
#define _AP_Tzzz1_H_

#include "ap_dimension.h"
#include "ap_coeff.h"
#include "ap_tzzz0.h"
#include "ap_environment.h"
#include "ap_linzzz1.h"
#include "ap_texpr1.h"

#define _AP_Tzzz1_MARK_

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/*
  IMPORTANT NOTE
  --------------
  correct use of floating-point AP_RTYPE_xxx currently supposes that the
  FPU rounds towards +oo
*/

/* Tree expression/constraint */
typedef struct ap_tzzz1_t {
  ap_tzzz0_t* tzzz0;
  ap_environment_t* env;
} ap_tzzz1_t;

/* Array */
typedef struct ap_tzzz1_array_t {
  ap_tzzz0_array_t tzzz0_array;
  ap_environment_t* env;
} ap_tzzz1_array_t;

/* ********************************************************************** */
/* I. ap_tzzz1_t */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Constructors and Destructors */
/* ====================================================================== */

#if defined(_AP_Texpr1_MARK_)
ap_texpr1_t* ap_texpr1_cst(ap_environment_t* env, ap_coeff_t coeff);
  /* Create a constant leaf expression */

ap_texpr1_t* ap_texpr1_var(ap_environment_t* env, ap_var_t var);
  /* Create a variable leaf expression */

ap_texpr1_t* ap_texpr1_unop(ap_texpr_op_t op,
			    ap_texpr1_t* opA,
			    ap_texpr_rtype_t type, ap_texpr_rdir_t dir);
  /* Create unary operator node */

ap_texpr1_t* ap_texpr1_binop(ap_texpr_op_t op,
			     ap_texpr1_t* opA, ap_texpr1_t* opB,
			     ap_texpr_rtype_t type, ap_texpr_rdir_t dir);
  /* Create binary operator node */
#else
ap_tcons1_t* ap_tcons1_make(struct ap_texpr1_t* expr,
			    ap_constyp_t constyp,
			    mpq_t mpq);
  /* Create a constraint of given type with the given expression.
     The expression is not duplicated, just pointed to. */
#endif

ap_tzzz1_t* ap_tzzz1_copy(ap_tzzz1_t* a);
  /* Recursive (deep) copy */

void ap_tzzz1_free(ap_tzzz1_t* a);
  /* Recursive (deep) free */

ap_tzzz1_t* ap_tzzz1_from_linzzz1(ap_linzzz1_t e);
  /* From linear expression/constraint to comb-like expression/constraint tree */

/* ====================================================================== */
/* I.2 Printing */
/* ====================================================================== */

void ap_tzzz1_fprint(FILE* stream, ap_tzzz1_t* a);
static inline
void ap_tzzz1_print(ap_tzzz1_t* a);
  /* Prints the expression/constraint */


/* ====================================================================== */
/* I.3 Tests, size */
/* ====================================================================== */

bool ap_tzzz1_has_var(ap_tzzz1_t* e, ap_var_t var);
   /* Returns true if variable var appears in the expression */

  /* Expression classification */
static inline
bool ap_tzzz1_is_interval_cst(ap_tzzz1_t* e);
  /* no-variable, only constant leaves */

static inline
bool ap_tzzz1_is_interval_linear(ap_tzzz1_t* e);
  /* linear with possibly interval coefficients, no rounding */

static inline
bool ap_tzzz1_is_interval_polynomial(ap_tzzz1_t* e);
  /* polynomial with possibly interval coefficients, no rounding  */

static inline
bool ap_tzzz1_is_interval_polyfrac(ap_tzzz1_t* e);
  /* polynomial fraction with possibly interval coefficients, no rounding */

static inline
bool ap_tzzz1_is_scalar(ap_tzzz1_t* e);
  /* all coefficients are scalar (non-interval) */

/* ====================================================================== */
/* I.4 Operations/Access */
/* ====================================================================== */

#if defined(_AP_Texpr1_MARK_)
ap_texpr1_t* ap_texpr1_substitute(ap_texpr1_t* e, ap_var_t var, ap_texpr1_t *dst);
  /* Substitute every occurrence of variable var with a copy of dst.
     Return NULL in case of incorrect argument (w.r.t. var and/or environment
     compatibility). */
bool ap_texpr1_substitute_with(ap_texpr1_t* e, ap_var_t var, ap_texpr1_t *dst);
  /* Substitute every occurrence of variable var with a copy of dst.
     Return true in case of incorrect argument (w.r.t. var and/or environment
     compatibility) */
#else
static inline
ap_constyp_t* ap_tcons1_constypref(ap_tcons1_t* cons);
  /* Get a reference to the type of constraint */

static inline
mpq_ptr ap_tcons1_mpqref(ap_tcons1_t* cons);
  /* Get a reference to the auxiliary coefficient of the constraint */

static inline
ap_texpr1_t ap_tcons1_texpr1ref(ap_tcons1_t* cons);
  /* Get a reference to the underlying expression of the constraint.
     Do not free it: nothing is duplicated.
     Modifying the argument or the result is equivalent, except for
     change of dimensions/environment. */
#endif

/* ====================================================================== */
/* I.5 Change of environments */
/* ====================================================================== */

ap_tzzz1_t* ap_tzzz1_extend_environment_dimchange(ap_tzzz1_t* a,
						  ap_environment_t* nenv,
						  ap_dimchange_t* dimchange);
void ap_tzzz1_extend_environment_dimchange_with(ap_tzzz1_t* a,
						ap_environment_t* nenv,
						ap_dimchange_t* dimchange);
ap_tzzz1_t* ap_tzzz1_extend_environment(ap_tzzz1_t* a, ap_environment_t* nenv);
  /* Change current environment with a super-environment.
     Return NULL if nenv is not a superenvironment  */
bool ap_tzzz1_extend_environment_with(ap_tzzz1_t* a, ap_environment_t* nenv);
  /* Change current environment with a super-environment.
     Return true if nenv is not a superenvironment  */

/* ********************************************************************** */
/* II. ap_tzzz1_array_t */
/* ********************************************************************** */

/* ====================================================================== */
/* II.1 Memory management and printing */
/* ====================================================================== */

ap_tzzz1_array_t ap_tzzz1_array_make(ap_environment_t* env, size_t size);
  /* Allocate an array of size constraints.
     The constraints are initialized with NULL pointers,
     so that ap_tzzz1_array_free may be safely called */
void ap_tzzz1_array_clear(ap_tzzz1_array_t* array);
  /* Clear the constraints of the array, and then the array itself */
void ap_tzzz1_array_fprint(FILE* stream,
			    ap_tzzz1_array_t* array);
static inline
void ap_tzzz1_array_print(ap_tzzz1_array_t* array);
  /* Printing */

/* ====================================================================== */
/* I.3 Tests, size */
/* ====================================================================== */

  /* Expression classification */
static inline
bool ap_tzzz1_array_is_interval_linear(ap_tzzz1_array_t* e);
  /* linear with possibly interval coefficients, no rounding */

static inline
bool ap_tzzz1_array_is_interval_polynomial(ap_tzzz1_array_t* e);
  /* polynomial with possibly interval coefficients, no rounding  */

static inline
bool ap_tzzz1_array_is_interval_polyfrac(ap_tzzz1_array_t* e);
  /* polynomial fraction with possibly interval coefficients, no rounding */

static inline
bool ap_tzzz1_array_is_scalar(ap_tzzz1_array_t* e);
  /* all coefficients are scalar (non-interval) */

/* ====================================================================== */
/* II.3 Access */
/* ====================================================================== */

static inline
size_t ap_tzzz1_array_size(ap_tzzz1_array_t* array);
  /* Return the size of the array */

static inline
void ap_tzzz1_array_free_index(ap_tzzz1_array_t* array, size_t index);
  /* Free the constraint at index index. */

bool ap_tzzz1_array_set_index(ap_tzzz1_array_t* array, size_t i, ap_tzzz1_t* a);
ap_tzzz1_t* ap_tzzz1_array_get_index(ap_tzzz1_array_t* array, size_t i);
ap_tzzz1_t ap_tzzz1_array_ref_index(ap_tzzz1_array_t* array, size_t i);

/* ====================================================================== */
/* II.4 Change of dimensions and permutations */
/* ====================================================================== */

void ap_tzzz1_array_extend_environment_dimchange_with(
    ap_tzzz1_array_t* array, ap_environment_t* nenv, ap_dimchange_t* dimchange
);
ap_tzzz1_array_t ap_tzzz1_array_extend_environment_dimchange(
    ap_tzzz1_array_t* array, ap_environment_t* nenv, ap_dimchange_t* dimchange
);
bool ap_tzzz1_array_extend_environment_with(ap_tzzz1_array_t* array, ap_environment_t* env);
ap_tzzz1_array_t  ap_tzzz1_array_extend_environment(
    bool* perror, ap_tzzz1_array_t* array, ap_environment_t* env
);

/* ********************************************************************** */
/* Inline function definitions */
/* ********************************************************************** */

static inline void ap_tzzz1_print(ap_tzzz1_t* a)
{ ap_tzzz1_fprint(stdout, a); }

static inline bool ap_tzzz1_is_interval_cst(ap_tzzz1_t* a)
{ return ap_tzzz0_is_interval_cst(a->tzzz0); }

static inline bool ap_tzzz1_is_interval_linear(ap_tzzz1_t* a)
{ return ap_tzzz0_is_interval_linear(a->tzzz0); }

static inline bool ap_tzzz1_is_interval_polynomial(ap_tzzz1_t* a)
{ return ap_tzzz0_is_interval_polynomial(a->tzzz0); }

static inline bool ap_tzzz1_is_interval_polyfrac(ap_tzzz1_t* a)
{ return ap_tzzz0_is_interval_polyfrac(a->tzzz0); }

static inline bool ap_tzzz1_is_scalar(ap_tzzz1_t* a)
{ return ap_tzzz0_is_scalar(a->tzzz0); }

#if defined(_AP_Tcons1_MARK_)
static inline
ap_constyp_t* ap_tcons1_constypref(ap_tcons1_t* cons){
  return &cons->tcons0->constyp;
}
static inline
mpq_ptr ap_tcons1_mpqref(ap_tcons1_t* cons){
  return cons->tcons0->mpq;
}
static inline
ap_texpr1_t ap_tcons1_texpr1ref(ap_tcons1_t* cons){
  ap_texpr1_t expr;
  expr.texpr0 = cons->tcons0->texpr0;
  expr.env = cons->env;
  return expr;
}
#endif

static inline
void ap_tzzz1_array_print(ap_tzzz1_array_t* array)
{ ap_tzzz1_array_fprint(stdout,array); }
static inline
size_t ap_tzzz1_array_size(ap_tzzz1_array_t* array){
  return array->tzzz0_array.size;
}
static inline bool ap_tzzz1_array_is_interval_linear(ap_tzzz1_array_t* a)
{ return ap_tzzz0_array_is_interval_linear(&a->tzzz0_array); }

static inline bool ap_tzzz1_array_is_interval_polynomial(ap_tzzz1_array_t* a)
{ return ap_tzzz0_array_is_interval_polynomial(&a->tzzz0_array); }

static inline bool ap_tzzz1_array_is_interval_polyfrac(ap_tzzz1_array_t* a)
{ return ap_tzzz0_array_is_interval_polyfrac(&a->tzzz0_array); }

static inline bool ap_tzzz1_array_is_scalar(ap_tzzz1_array_t* a)
{ return ap_tzzz0_array_is_scalar(&a->tzzz0_array); }

static inline
void ap_tzzz1_array_free_index(ap_tzzz1_array_t* array, size_t index)
{
  assert(index<array->tzzz0_array.size);
  if (array->tzzz0_array.p[index]){
    ap_tzzz0_free(array->tzzz0_array.p[index]);
    array->tzzz0_array.p[index] = NULL;
  }
}

#undef _AP_Tzzz1_MARK_

#ifdef __cplusplus
}
#endif

#endif
