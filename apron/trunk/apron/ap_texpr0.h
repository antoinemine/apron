/* ************************************************************************* */
/* ap_texpr0.h: tree expressions */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include <stdarg.h>

#ifndef _AP_TEXPR0_H_
#define _AP_TEXPR0_H_

#include "ap_dimension.h"
#include "ap_coeff.h"
#include "ap_linexpr0.h"

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



/* Operators */
typedef enum ap_texpr_op_t {

  /* Binary operators */
  AP_TEXPR_ADD, AP_TEXPR_SUB, AP_TEXPR_MUL, AP_TEXPR_DIV,
  AP_TEXPR_MOD,  /* either integer or real, no rounding */
  AP_TEXPR_POW,

  /* Unary operators */
  AP_TEXPR_NEG   /* no rounding */,
  AP_TEXPR_CAST, AP_TEXPR_SQRT,

} ap_texpr_op_t;

/* Numerical type (destination of rounding) */
typedef enum ap_texpr_rtype_t {
  AP_RTYPE_REAL,     /* real (no rounding) */
  AP_RTYPE_INT,      /* integer */
  AP_RTYPE_SINGLE,   /* IEEE 754 32-bit single precision, e.g.: C's float */
  AP_RTYPE_DOUBLE,   /* IEEE 754 64-bit double precision, e.g.: C's double */
  AP_RTYPE_EXTENDED, /* non-standard 80-bit double extended, e.g.: Intel's long double */
  AP_RTYPE_QUAD,     /* non-standard 128-bit quadruple precision, e.g.: Motorola's long double */
  AP_RTYPE_SIZE      /* Not to be used ! */
} ap_texpr_rtype_t;

/* Rounding direction */
typedef enum ap_texpr_rdir_t {
  AP_RDIR_NEAREST = GMP_RNDN, /* Nearest */
  AP_RDIR_ZERO    = GMP_RNDZ, /* Zero (truncation for integers) */
  AP_RDIR_UP      = GMP_RNDU, /* + Infinity */
  AP_RDIR_DOWN    = GMP_RNDD, /* - Infinity */
  AP_RDIR_RND,    /* All possible mode, non deterministically */
  AP_RDIR_SIZE    /* Not to be used ! */
} ap_texpr_rdir_t;

/* Internal (operator) nodes */
typedef struct ap_texpr0_node_t {
  ap_texpr_op_t    op;
  ap_texpr_rtype_t type;
  ap_texpr_rdir_t  dir;
  struct ap_texpr0_t* exprA; /* First operand */
  struct ap_texpr0_t* exprB; /* Second operand (for binary operations) or NULL */
} ap_texpr0_node_t;

/* Node types */
typedef enum ap_texpr_discr_t {
  AP_TEXPR_CST, AP_TEXPR_DIM, AP_TEXPR_NODE
} ap_texpr_discr_t;

typedef struct ap_texpr0_t {
  ap_texpr_discr_t discr;
  union {
    ap_coeff_t cst;
    ap_dim_t dim;
    ap_texpr0_node_t* node;
  } val;
} ap_texpr0_t;

/* ====================================================================== */
/* I. Constructors and Destructors */
/* ====================================================================== */

ap_texpr0_t* ap_texpr0_cst                 (ap_coeff_t* coeff);
ap_texpr0_t* ap_texpr0_cst_scalar          (ap_scalar_t* scalar);
ap_texpr0_t* ap_texpr0_cst_scalar_mpq      (mpq_t mpq);
ap_texpr0_t* ap_texpr0_cst_scalar_mpfr     (mpfr_t mpfr);
ap_texpr0_t* ap_texpr0_cst_scalar_int      (long int num);
ap_texpr0_t* ap_texpr0_cst_scalar_frac     (long int num, unsigned long int den);
ap_texpr0_t* ap_texpr0_cst_scalar_double   (double num);
ap_texpr0_t* ap_texpr0_cst_interval        (ap_interval_t* itv);
ap_texpr0_t* ap_texpr0_cst_interval_scalar (ap_scalar_t* inf, ap_scalar_t* sup);
ap_texpr0_t* ap_texpr0_cst_interval_mpq    (mpq_t inf, mpq_t sup);
ap_texpr0_t* ap_texpr0_cst_interval_mpfr   (mpfr_t inf, mpfr_t sup);
ap_texpr0_t* ap_texpr0_cst_interval_int    (long int inf, long int sup);
ap_texpr0_t* ap_texpr0_cst_interval_frac   (long int numinf, unsigned long int deninf,
					    long int numsup, unsigned long int densup);
ap_texpr0_t* ap_texpr0_cst_interval_double (double inf, double sup);
ap_texpr0_t* ap_texpr0_cst_interval_top    (void);
  /* Create a constant leaf expression */

ap_texpr0_t* ap_texpr0_dim(ap_dim_t dim);
  /* Create a dimension (variable) leaf expression */

ap_texpr0_t* ap_texpr0_unop(ap_texpr_op_t op,
			    ap_texpr0_t* opA,
			    ap_texpr_rtype_t type, ap_texpr_rdir_t dir);
  /* Create unary operator node */

ap_texpr0_t* ap_texpr0_binop(ap_texpr_op_t op,
			     ap_texpr0_t* opA, ap_texpr0_t* opB,
			     ap_texpr_rtype_t type, ap_texpr_rdir_t dir);
  /* Create binary operator node */

ap_texpr0_t* ap_texpr0_copy(ap_texpr0_t* expr);
  /* Recursive (deep) copy */

void ap_texpr0_free(ap_texpr0_t* expr);
  /* Recursive (deep) free */

ap_texpr0_t* ap_texpr0_from_linexpr0(ap_linexpr0_t* e);
  /* From linear expression to comb-like expression tree */


/* ====================================================================== */
/* II. Printing */
/* ====================================================================== */

void ap_texpr0_fprint(FILE* stream, ap_texpr0_t* a, char** name_of_dim);
void ap_texpr0_print(ap_texpr0_t* a, char** name_of_dim);
  /* Prints the expression, name_of_dim can be NULL */


/* ====================================================================== */
/* III. Tests, size */
/* ====================================================================== */

static inline bool ap_texpr_is_unop(ap_texpr_op_t op){
  return (op>=AP_TEXPR_NEG && op<=AP_TEXPR_SQRT);
}
static inline bool ap_texpr_is_binop(ap_texpr_op_t op){
  return (op<=AP_TEXPR_POW);
}
  /* Operator classification */

size_t ap_texpr0_depth(ap_texpr0_t* a);
  /* Returns the depth, in operator nodes */

size_t ap_texpr0_size(ap_texpr0_t* a);
  /* Returns the number of operator nodes */

ap_dim_t ap_texpr0_max_dim(ap_texpr0_t* a);
  /* Returns the maximum ap_dim_t PLUS ONE of all dimensions in expression, and
     0 if no dimension at all. 

     For instance, it returns 3 on the expression x2. */

bool ap_texpr0_has_dim(ap_texpr0_t* a, ap_dim_t d);
   /* Returns true if dimension d appears in the expression */

ap_dim_t* ap_texpr0_dimlist(ap_texpr0_t* a);
  /* Returns an ordered, AP_DIM_MAX-terminated array of occurring dimensions;
     caller should free() the array after use
   */


  /* Expression classification */

bool ap_texpr0_is_interval_cst(ap_texpr0_t* a);
  /* no-variable, only constant leaves */

bool ap_texpr0_is_interval_linear(ap_texpr0_t* a);
  /* linear with possibly interval coefficients, no rounding */
bool ap_texpr0_is_interval_polynomial(ap_texpr0_t* a);
  /* polynomial with possibly interval coefficients, no rounding  */
bool ap_texpr0_is_interval_polyfrac(ap_texpr0_t* a);
  /* polynomial fraction with possibly interval coefficients, no rounding */
bool ap_texpr0_is_scalar(ap_texpr0_t* a);
  /* all coefficients are scalar (non-interval) */

bool ap_texpr0_array_is_interval_linear(ap_texpr0_t** texpr, size_t size);
bool ap_texpr0_array_is_interval_polynomial(ap_texpr0_t** texpr, size_t size);
bool ap_texpr0_array_is_interval_polyfrac(ap_texpr0_t** texpr, size_t size);
bool ap_texpr0_array_is_scalar(ap_texpr0_t** texpr, size_t size);
  /* idem for arrays */

/* ====================================================================== */
/* IV. Operations */
/* ====================================================================== */


ap_texpr0_t* ap_texpr0_substitute(ap_texpr0_t* a, ap_dim_t dim, ap_texpr0_t *dst);
void ap_texpr0_substitute_with   (ap_texpr0_t* a, ap_dim_t dim, ap_texpr0_t *dst);
  /* Substitute every occurrence of dimension dim with a copy of dst  */

/* ====================================================================== */
/* V. Change of dimensions and permutations */
/* ====================================================================== */

ap_texpr0_t* ap_texpr0_add_dimensions(ap_texpr0_t* expr,
				      ap_dimchange_t* dimchange);
ap_texpr0_t* ap_texpr0_remove_dimensions(ap_texpr0_t* expr,
					 ap_dimchange_t* dimchange);
ap_texpr0_t* ap_texpr0_permute_dimensions(ap_texpr0_t* expr,
					  ap_dimperm_t* dimperm);
void ap_texpr0_add_dimensions_with(ap_texpr0_t* expr,
				   ap_dimchange_t* dimchange);
void ap_texpr0_remove_dimensions_with(ap_texpr0_t* expr,
					ap_dimchange_t* dimchange);
void ap_texpr0_permute_dimensions_with(ap_texpr0_t* expr,
				       ap_dimperm_t* perm);

/* Note: removed variables are replaced with a top interval */

/* ====================================================================== */
/* VI. Hashing, comparisons */
/* ====================================================================== */

long ap_texpr0_hash(ap_texpr0_t* a);
  /* Recursive hashing */

bool ap_texpr0_equal(ap_texpr0_t* a1, ap_texpr0_t* a2);
  /* Structural (recursive) equality */



/* used internally */
ap_texpr0_t* ap_texpr0_node(ap_texpr_op_t op,
			    ap_texpr_rtype_t type, ap_texpr_rdir_t dir,
			    ap_texpr0_t* opA, ap_texpr0_t* opB);
void ap_texpr0_node_free(ap_texpr0_node_t* node);
void ap_texpr0_clear(ap_texpr0_t* node);


#ifdef __cplusplus
}
#endif

#endif
