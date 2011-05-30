/* ************************************************************************* */
/* ap_linyyy0.h: linear expressions or constraints */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

/* normally included from ap_expr0.h */

#ifndef _AP_linyyy0_H_
#define _AP_linyyy0_H_

#include <stdio.h>
#include "ap_coeff.h"
#include "ap_dimension.h"
#include "ap_linyyyD.h"
#include "ap_linyyyMPQ.h"
#include "ap_linyyyMPFR.h"
#ifndef _AP_linexpr0_H_
#include "ap_linexpr0.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif

#define _AP_linyyy0_MARK_

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* A linear expression. */
typedef struct ap_linyyy0_struct {
  ap_scalar_discr_t discr;
  union {
    ap_linyyyD_ptr    D;
    ap_linyyyMPQ_ptr  MPQ;
    ap_linyyyMPFR_ptr MPFR;
  } linyyy;
} ap_linyyy0_struct;
typedef ap_linyyy0_struct ap_linyyy0_t[1];
typedef ap_linyyy0_struct* ap_linyyy0_ptr;

/* Array of linear expressions */
typedef struct ap_linyyy0_array_struct {
  ap_scalar_discr_t discr;
  union {
    ap_linyyyD_array_ptr    D;
    ap_linyyyMPQ_array_ptr  MPQ;
    ap_linyyyMPFR_array_ptr MPFR;
  } linyyy_array;
} ap_linyyy0_array_struct;
typedef ap_linyyy0_array_struct* ap_linyyy0_array_ptr;
typedef ap_linyyy0_array_struct ap_linyyy0_array_t[1];

/* ********************************************************************** */
/* I. ap_linyyy0_t */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Memory management and printing */
/* ====================================================================== */

void ap_linyyy0_init(ap_linyyy0_t res, ap_scalar_discr_t discr, size_t size);
void ap_linyyy0_init_set(ap_linyyy0_t res, ap_linyyy0_t e);
MACRO_FOREACH ZZZ @MainNum
void ap_linyyy0_init_set_linyyyZZZ(ap_linyyy0_t res, ap_linyyyZZZ_t e);
ENDMACRO
#if defined (_AP_lincons0_MARK_)
void ap_lincons0_set_bool(ap_lincons0_t res, bool b);
#endif
void ap_linyyy0_resize(ap_linyyy0_t e, size_t size);
void ap_linyyy0_clear(ap_linyyy0_t e);
void ap_linyyy0_fprint(FILE* stream, ap_linyyy0_t e, char** name_of_dim);
void ap_linyyy0_print(ap_linyyy0_t e, char** name_of_dim);
void ap_linyyy0_minimize(ap_linyyy0_t a);

/* ====================================================================== */
/* I.2 Conversions */
/* ====================================================================== */

bool ap_linyyy0_set(ap_linyyy0_t res, ap_linyyy0_t e, num_internal_t intern);
MACRO_FOREACH ZZZ @AllNum
bool ap_linyyy0_set_linyyyZZZ(ap_linyyy0_t a, ap_linyyyZZZ_t b, num_internal_t intern);
bool ap_linyyyZZZ_set_linyyy0(ap_linyyyZZZ_t a, ap_linyyy0_t b, num_internal_t intern);
ENDMACRO

/* ====================================================================== */
/* I.3 Tests */
/* ====================================================================== */

#if defined (_AP_linexpr0_MARK_) || defined (_AP_lincons0_MARK_)
/* Classification */
bool ap_linyyy0_is_linear(ap_linyyy0_t expr);
  /* Return true iff all involved coefficients are scalars */
bool ap_linyyy0_is_quasilinear(ap_linyyy0_t expr);
  /* Return true iff all involved coefficients but the constant are scalars. */
ap_linexpr_type_t ap_linyyy0_type(ap_linyyy0_t expr);
  /* Classify an expression */
bool ap_linyyy0_is_integer(ap_linyyy0_t expr, size_t intdim);
bool ap_linyyy0_is_real(ap_linyyy0_t expr, size_t intdim);
  /* Does the expression depends only on integer (resp. real) variables ? assuming
     that the first intdim dimensions are integer */
#endif

/* ====================================================================== */
/* I.4 Access */
/* ====================================================================== */

static inline
ap_scalar_discr_t ap_linyyy0_discr(ap_linyyy0_t a);

bool ap_linyyy0_get_cst(ap_coeff_t coeff, ap_linyyy0_t expr, num_internal_t intern);
  /* Get the constant and assign it to coeff with possible conversion */
bool ap_linyyy0_get_coeff(ap_coeff_t coeff, ap_linyyy0_t expr, ap_dim_t dim, num_internal_t intern);
  /* Get coefficient of dimension dim in expr and assign it to
     coeff with possible conversion. */

bool ap_linyyy0_set_cst(ap_linyyy0_t expr, ap_coeff_t coeff, num_internal_t intern);
  /* Assign the constant with possible conversion. */
bool ap_linyyy0_set_coeff(ap_linyyy0_t expr, ap_dim_t dim, ap_coeff_t coeff, num_internal_t intern);
  /* Assign the coefficient of dimension dim in the expression, with possible
     conversion. */

void ap_linyyy0_cstref(ap_coeff_t coeff, ap_linyyy0_t expr);
void ap_linyyy0_coeffref(ap_coeff_t coeff, ap_linyyy0_t expr, ap_dim_t dim);
void ap_linyyy0_coeffrefdim_of_index(ap_coeff_t res, ap_dim_t* pdim, ap_linyyy0_t expr, size_t index);

bool ap_linyyy0_set_list(num_internal_t intern, ap_linyyy0_t expr, bool* perror, ...);
  /* This function assigns the linear expression from a list of tags of type
     itv_coefftag_t, each followed by a number of arguments as specified in
     the definition of the type ap_coeff_tag_t_t, and ended by the tag AP_END;

     - The dimension AP_COEFF_DIM_MAX/AP_DIM_MAX is used to refer to the constat coefficient.
     - If the same dimension appears several times, only the last tag
       referring to it is taken into account.

     Returns true iff all conversions were exact.

     Example:
     ap_linexpr0_set_list(intern,
			  expr,
			  AP_COEFF_LFRAC,7,9,0,
			  AP_COEFF_DOUBLE2,-3.0,4.5,1,
			  AP_COEFF_LLINT,3LL,AP_COEFF_DIM_MAX,
			  AP_END)
     sets expr to "7/9 x0 + [-3,4.5] x1 + 3"
     assuming that the expression was "0" before the call and that all the
     number conversions were exact.
  */

#if defined (_AP_linexpr0_MARK_)
/* Iterator (Macro): use:
   ap_linexprXXX_ForeachLinterm(ap_linexpr0_t e, size_t i, ap_dim_t d, ap_coeff_t coeff){
     ..
   }
   where
   - e is the inspected expression (of type ap_linexprXXX_t)
   - i is the internal iterator (of type size_t or int)
   - dim is the dimension of one linear term (of type ap_dim_t)
   - peitv is a pointer to the corresponding interval (of type eitvXXX_ptr)

*/

#define ap_linexpr0_ForeachLinterm(_p_e, _p_i, _p_d, _p_coeff)          \
  for (_p_i=0;                                                          \
       (ap_linexpr0_coeffrefdim_of_index(_p_coeff,&_p_d,_p_e,_p_i),     \
	_p_d!=AP_DIM_MAX);                                              \
       _p_i++)
#endif

#if defined (_AP_lincons0_MARK_)
void ap_lincons0_get_mpq(mpq_t mpq, ap_lincons0_t c);
mpq_ptr ap_lincons0_mpqref(ap_lincons0_t c);
void ap_lincons0_set_mpq(ap_lincons0_t c, mpq_t mpq);
#endif

#if defined (_AP_lincons0_MARK_) || defined (_AP_lingen0_MARK_)

bool ap_linyyy0_get_linexpr0(ap_linexpr0_t e, ap_linyyy0_t c, num_internal_t intern);
  /* Get the underlying expression and assign it to e with possible
     conversion */
ap_yyytyp_t ap_linyyy0_get_yyytyp(ap_linyyy0_t c);

void ap_linyyy0_linexpr0ref(ap_linexpr0_t e, ap_linyyy0_t c);
ap_yyytyp_t* ap_linyyy0_yyytypref(ap_linyyy0_t c);

bool ap_linyyy0_set_linexpr0(ap_linyyy0_t c, ap_linexpr0_t e, num_internal_t intern);
  /* Assign the underlying expression of c to e with possible conversion */
void ap_linyyy0_set_yyytyp(ap_linyyy0_t c, ap_yyytyp_t yyytyp);
#endif

/* ====================================================================== */
/* I.5 Change of dimensions and permutations */
/* ====================================================================== */

/* This function add dimensions to the expressions, following the
   semantics of dimchange (see the type definition of dimchange).  */
void ap_linyyy0_add_dimensions(ap_linyyy0_t a,
			    ap_linyyy0_t b,
			    ap_dimchange_t* dimchange);

/* This function apply the given permutation to the dimensions. If dense
   representation, the size of the permutation should be expr->size. If sparse
   representation, the dimensions present in the expression should just be less
   than the size of the permutation. */
void ap_linyyy0_permute_dimensions(ap_linyyy0_t a,
				   ap_linyyy0_t b,
				   ap_dimperm_t* perm);

/* ====================================================================== */
/* I.6 Hashing, comparison */
/* ====================================================================== */

/* Induces reduction of the coefficients */

int ap_linyyy0_hash(ap_linyyy0_t expr);
bool ap_linyyy0_equal(ap_linyyy0_t expr1, ap_linyyy0_t expr2);

/* Lexicographic ordering, terminating by constant coefficients */
int ap_linyyy0_compare(ap_linyyy0_t expr1, ap_linyyy0_t expr2);

/* ********************************************************************** */
/* II. ap_linyyy0_array_t */
/* ********************************************************************** */

/* ====================================================================== */
/* II.1 Memory management and printing */
/* ====================================================================== */

void ap_linyyy0_array_init(ap_linyyy0_array_t e, ap_scalar_discr_t discr, size_t size);
void ap_linyyy0_array_init_set(ap_linyyy0_array_t res, ap_linyyy0_array_t e);
MACRO_FOREACH ZZZ @MainNum
void ap_linyyy0_array_init_set_linyyyZZZ_array(ap_linyyy0_array_t res, ap_linyyyZZZ_array_t e);
ENDMACRO
void ap_linyyy0_array_resize(ap_linyyy0_array_t a, size_t size);
void ap_linyyy0_array_minimize(ap_linyyy0_array_t a);
void ap_linyyy0_array_clear(ap_linyyy0_array_t e);
void ap_linyyy0_array_fprint(FILE* stream, ap_linyyy0_array_t e, char** name_of_dim);
void ap_linyyy0_array_print(ap_linyyy0_array_t e, char** name_of_dim);

/* ====================================================================== */
/* II.2 Conversions */
/* ====================================================================== */

bool ap_linyyy0_array_set(ap_linyyy0_array_t res, ap_linyyy0_array_t e, num_internal_t intern);
MACRO_FOREACH ZZZ @AllNum
bool ap_linyyy0_array_set_linyyyZZZ_array(ap_linyyy0_array_t a, ap_linyyyZZZ_array_t b, num_internal_t intern);
bool ap_linyyyZZZ_array_set_linyyy0_array(ap_linyyyZZZ_array_t a, ap_linyyy0_array_t b, num_internal_t intern);

bool ap_linyyy0_array_set_index_linyyyZZZ(ap_linyyy0_array_t p, size_t i, ap_linyyyZZZ_t a, num_internal_t intern);
bool ap_linyyy0_array_get_index_linyyyZZZ(ap_linyyyZZZ_t a, ap_linyyy0_array_t p, size_t i, num_internal_t intern);
ENDMACRO

/* ====================================================================== */
/* II.3 Tests */
/* ====================================================================== */

#if !defined(_AP_lingen0_ARRAY_H_)
bool ap_linyyy0_array_is_linear(ap_linyyy0_array_t array);
bool ap_linyyy0_array_is_quasilinear(ap_linyyy0_array_t array);
  /* Are all the constraints involved linear (resp. quasilinear) */
ap_linexpr_type_t ap_linyyy0_array_type(ap_linyyy0_array_t array);
#endif

/* ====================================================================== */
/* II.4 Access */
/* ====================================================================== */

static inline ap_scalar_discr_t ap_linyyy0_array_discr(ap_linyyy0_array_t p);
size_t ap_linyyy0_array_size(ap_linyyy0_array_t p);

bool ap_linyyy0_array_set_index(ap_linyyy0_array_t p, size_t i, ap_linyyy0_t a, num_internal_t intern);
bool ap_linyyy0_array_get_index(ap_linyyy0_t a, ap_linyyy0_array_t p, size_t i, num_internal_t intern);
void ap_linyyy0_array_ref_index(ap_linyyy0_t a, ap_linyyy0_array_t p, size_t i);

/* ====================================================================== */
/* II.5 Change of dimensions and permutations */
/* ====================================================================== */

void ap_linyyy0_array_add_dimensions(ap_linyyy0_array_t a,
				     ap_linyyy0_array_t b,
				     ap_dimchange_t* dimchange);
void ap_linyyy0_array_permute_dimensions(ap_linyyy0_array_t a,
					 ap_linyyy0_array_t b,
					 ap_dimperm_t* perm);

/* ********************************************************************** */
/* ********************************************************************** */
/* Inline functions definitions */
/* ********************************************************************** */

static inline
ap_scalar_discr_t ap_linyyy0_discr(ap_linyyy0_t expr)
{ return expr->discr; }
static inline void ap_linyyy0_cons(ap_linyyy0_t res, ap_linyyy0_t e)
{
  res->discr = e->discr;
  res->linyyy = e->linyyy;
}
MACRO_FOREACH ZZZ @MainNum
static inline void ap_linyyy0_cons_ZZZ(ap_linyyy0_t res, ap_linyyyZZZ_t e)
{ res->discr = AP_SCALAR_ZZZ; res->linyyy.ZZZ = e; };
ENDMACRO

static inline ap_scalar_discr_t ap_linyyy0_array_discr(ap_linyyy0_array_t p)
{ return p->discr; }
MACRO_FOREACH ZZZ @MainNum
static inline void ap_linyyy0_array_cons_ZZZ(ap_linyyy0_array_t res, ap_linyyyZZZ_array_t e)
{ res->discr = AP_SCALAR_ZZZ; res->linyyy_array.ZZZ = e; };
ENDMACRO

#undef _AP_linyyy0_MARK_

#ifdef __cplusplus
}
#endif

#endif
