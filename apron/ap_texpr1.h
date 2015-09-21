/* ************************************************************************* */
/* ap_texpr0.h: tree expressions */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include <stdarg.h>

#ifndef _AP_TEXPR1_H_
#define _AP_TEXPR1_H_

#include "ap_dimension.h"
#include "ap_coeff.h"
#include "ap_texpr0.h"
#include "ap_environment.h"
#include "ap_linexpr1.h"

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

/* Tree expressions */
typedef struct ap_texpr1_t {
  ap_texpr0_t* texpr0;
  ap_environment_t* env;
} ap_texpr1_t;

/* ====================================================================== */
/* I. Constructors and Destructors */
/* ====================================================================== */

ap_texpr1_t* ap_texpr1_cst                 (ap_environment_t* env, ap_coeff_t* coeff);
ap_texpr1_t* ap_texpr1_cst_scalar          (ap_environment_t* env, ap_scalar_t* scalar);
ap_texpr1_t* ap_texpr1_cst_scalar_mpq      (ap_environment_t* env, mpq_t mpq);
ap_texpr1_t* ap_texpr1_cst_scalar_mpfr     (ap_environment_t* env, mpfr_t mpfr);
ap_texpr1_t* ap_texpr1_cst_scalar_int      (ap_environment_t* env, long int num);
ap_texpr1_t* ap_texpr1_cst_scalar_frac     (ap_environment_t* env, long int num, unsigned long int den);
ap_texpr1_t* ap_texpr1_cst_scalar_double   (ap_environment_t* env, double num);
ap_texpr1_t* ap_texpr1_cst_interval        (ap_environment_t* env, ap_interval_t* itv);
ap_texpr1_t* ap_texpr1_cst_interval_scalar (ap_environment_t* env, ap_scalar_t* inf, ap_scalar_t* sup);
ap_texpr1_t* ap_texpr1_cst_interval_mpq    (ap_environment_t* env, mpq_t inf, mpq_t sup);
ap_texpr1_t* ap_texpr1_cst_interval_mpfr   (ap_environment_t* env, mpfr_t inf, mpfr_t sup);
ap_texpr1_t* ap_texpr1_cst_interval_int    (ap_environment_t* env, long int inf, long int sup);
ap_texpr1_t* ap_texpr1_cst_interval_frac   (ap_environment_t* env, long int numinf, unsigned long int deninf,
					    long int numsup, unsigned long int densup);
ap_texpr1_t* ap_texpr1_cst_interval_double (ap_environment_t* env, double inf, double sup);
ap_texpr1_t* ap_texpr1_cst_interval_top    (ap_environment_t* env);
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

ap_texpr1_t* ap_texpr1_copy(ap_texpr1_t* expr);
  /* Recursive (deep) copy */

void ap_texpr1_free(ap_texpr1_t* expr);
  /* Recursive (deep) free */

ap_texpr1_t* ap_texpr1_from_linexpr1(ap_linexpr1_t* e);
  /* From linear expression to comb-like expression tree */

/* ====================================================================== */
/* II. Printing */
/* ====================================================================== */

void ap_texpr1_fprint(FILE* stream, ap_texpr1_t* a);
void ap_texpr1_print(ap_texpr1_t* a);
  /* Prints the expression */


/* ====================================================================== */
/* III. Tests, size */
/* ====================================================================== */

bool ap_texpr1_has_var(ap_texpr1_t* e, ap_var_t var);
   /* Returns true if variable var appears in the expression */

  /* Expression classification */

static inline
bool ap_texpr1_is_interval_cst(ap_texpr1_t* e);
  /* no-variable, only constant leaves */

static inline
bool ap_texpr1_is_interval_linear(ap_texpr1_t* e);
  /* linear with possibly interval coefficients, no rounding */

static inline
bool ap_texpr1_is_interval_polynomial(ap_texpr1_t* e);
  /* polynomial with possibly interval coefficients, no rounding  */

static inline
bool ap_texpr1_is_interval_polyfrac(ap_texpr1_t* e);
  /* polynomial fraction with possibly interval coefficients, no rounding */

static inline
bool ap_texpr1_is_scalar(ap_texpr1_t* e);
  /* all coefficients are scalar (non-interval) */

/* ====================================================================== */
/* IV. Operations */
/* ====================================================================== */

ap_texpr1_t* ap_texpr1_substitute(ap_texpr1_t* e, ap_var_t var, ap_texpr1_t *dst);
  /* Substitute every occurrence of variable var with a copy of dst.
     Return NULL in case of incorrect argument (w.r.t. var and/or environment
     compatibility). */
bool ap_texpr1_substitute_with   (ap_texpr1_t* e, ap_var_t var, ap_texpr1_t *dst);
  /* Substitute every occurrence of variable var with a copy of dst.
     Return true in case of incorrect argument (w.r.t. var and/or environment
     compatibility) */

/* ====================================================================== */
/* V. Change of environments */
/* ====================================================================== */

ap_texpr1_t* ap_texpr1_extend_environment(ap_texpr1_t* expr, ap_environment_t* nenv);
  /* Change current environment with a super-environment.
     Return NULL if nenv is not a superenvironment  */
bool ap_texpr1_extend_environment_with(ap_texpr1_t* expr, ap_environment_t* nenv);
  /* Change current environment with a super-environment.
     Return true if nenv is not a superenvironment  */

/* ====================================================================== */
/* VI. Hashing, comparisons */
/* ====================================================================== */

bool ap_texpr1_equal(ap_texpr1_t* a1, ap_texpr1_t* a2);
  /* Structural (recursive) equality */

/* ====================================================================== */
/* Inline function definitions */
/* ====================================================================== */

static inline bool ap_texpr1_is_interval_cst(ap_texpr1_t* a)
{ return ap_texpr0_is_interval_cst(a->texpr0); }

static inline bool ap_texpr1_is_interval_linear(ap_texpr1_t* a)
{ return ap_texpr0_is_interval_linear(a->texpr0); }

static inline bool ap_texpr1_is_interval_polynomial(ap_texpr1_t* a)
{ return ap_texpr0_is_interval_polynomial(a->texpr0); }

static inline bool ap_texpr1_is_interval_polyfrac(ap_texpr1_t* a)
{ return ap_texpr0_is_interval_polyfrac(a->texpr0); }

static inline bool ap_texpr1_is_scalar(ap_texpr1_t* a)
{ return ap_texpr0_is_scalar(a->texpr0); }

#ifdef __cplusplus
}
#endif

#endif
