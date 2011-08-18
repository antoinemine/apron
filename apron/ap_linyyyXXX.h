/* ********************************************************************** */
/* ap_linyyyXXX.h */
/* ********************************************************************** */

#ifndef _AP_linyyyXXX_H_
#define _AP_linyyyXXX_H_

#include "ap_lin_types.h"
#include "eitvXXX.h"

#define _AP_yyy_MARK_

#if defined(_AP_expr_MARK_)
#include "ap_linexprXXX_eval.h"
#undef cinline
#define cinline
#else
#include "ap_linexprXXX.h"
#include "ap_linconsXXX_eval.h"
#undef cinline
#define cinline static inline
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* ap_linyyyXXX_t */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Constructor and Destructor */
/* ====================================================================== */

#if defined(_AP_expr_MARK_)
static inline void ap_lintermXXX_init(ap_lintermXXX_t term);
static inline void ap_lintermXXX_init_set(ap_lintermXXX_t res, ap_lintermXXX_t term);
static inline void ap_lintermXXX_set(ap_lintermXXX_t res, ap_lintermXXX_t term);
static inline void ap_lintermXXX_clear(ap_lintermXXX_t term);
static inline void ap_lintermXXX_swap(ap_lintermXXX_t a, ap_lintermXXX_t b);
#endif

cinline void ap_linyyyXXX_init(ap_linyyyXXX_t a,size_t size);
cinline void ap_linyyyXXX_init_set(ap_linyyyXXX_t res, ap_linyyyXXX_t a);
cinline void ap_linyyyXXX_set(ap_linyyyXXX_t res, ap_linyyyXXX_t a);
cinline void ap_linyyyXXX_minimize(ap_linyyyXXX_t e);
cinline void ap_linyyyXXX_clear(ap_linyyyXXX_t a);
static inline void ap_linyyyXXX_swap(ap_linyyyXXX_t a, ap_linyyyXXX_t b);

static inline void ap_linyyyXXX_resize(ap_linyyyXXX_t a, size_t size);
static inline void ap_linyyyXXX_set_zero(ap_linyyyXXX_t a);
#if defined(_AP_cons_MARK_)
void ap_linconsXXX_set_bool(ap_linconsXXX_t lincons, bool value);
void ap_linconsXXX_set_dim_num(ap_linconsXXX_t lincons, ap_dim_t dim, bool sgn, numXXX_t num);
  /* Sets lincons to the constraint
     - dim <= num if sgn>0
     - dim == num if sgn==0
     - dim >= -num if sgn<0 */
#endif

ap_linyyyXXX_ptr ap_linyyyXXX_alloc(size_t size);
ap_linyyyXXX_ptr ap_linyyyXXX_alloc_set(ap_linyyyXXX_t a);
void ap_linyyyXXX_free(ap_linyyyXXX_ptr gen);

void ap_linyyyXXX_fprint(FILE* stream, ap_linyyyXXX_t a, char** name);
void ap_linyyyXXX_print(ap_linyyyXXX_t a, char** name);

/* These two resizing functions are internal, look at what they do before
   using them */
cinline void ap_linyyyXXX_resize_strict(ap_linyyyXXX_t a, size_t size);

/* ====================================================================== */
/* I.2 Tests and Simplifications */
/* ====================================================================== */

cinline bool ap_linyyyXXX_is_integer(ap_linyyyXXX_t a, size_t intdim);
  /* Does the expression depends only on integer variables ? assuming
     that the first intdim dimensions are integer */
cinline bool ap_linyyyXXX_is_real(ap_linyyyXXX_t a, size_t intdim);
  /* Does the expression depends only on real variables ? assuming
     that the first intdim dimensions are integer */

  /* Expression classification */
cinline bool ap_linyyyXXX_is_linear(ap_linyyyXXX_t a);
  /* Return true iff all involved coefficients are scalars */
cinline bool ap_linyyyXXX_is_quasilinear(ap_linyyyXXX_t a);
  /* Return true iff all involved coefficients but the constant are scalars. */
cinline ap_linexpr_type_t ap_linyyyXXX_type(ap_linyyyXXX_t a);
  /* Classify an expression */

static inline ap_dim_t ap_linyyyXXX_max_dim(ap_linyyyXXX_t a);
  /* Returns the maximum ap_dim_t PLUS ONE of all dimensions in
     expression/constraint/generator, and 0 if no dimension at all.

     For instance, it returns 3 on the expression x2. */

cinline void ap_linyyyXXX_support_mask(ap_linyyyXXX_t a, ap_dim_t* tdim);
cinline void ap_linyyyXXX_supportinterval_mask(ap_linyyyXXX_t a, ap_dim_t* tdim);

  cinline size_t ap_linyyyXXX_support(ap_linyyyXXX_t a, ap_dim_t* tdim, size_t size);
  cinline size_t ap_linyyyXXX_supportinterval(ap_linyyyXXX_t a, ap_dim_t* tdim, size_t size);
/* Fills the array tdim with the dimensions associated with intervals in the
   linear expression, in increasing order, and return the number of such
   dimensions.

   tdim is supposed to be of size at least the number of terms in the
   expression.
 */

/* ====================================================================== */
/* I.3 Access */
/* ====================================================================== */

static inline size_t ap_linyyyXXX_size(ap_linyyyXXX_t a);
  /* Get the (effective) size of the linear expression */

static inline void ap_linyyyXXX_get_cst(eitvXXX_t eitv, ap_linyyyXXX_t a);
cinline void ap_linyyyXXX_get_eitv0(eitvXXX_t eitv, ap_linyyyXXX_t a, ap_dim_t dim);
cinline void ap_linyyyXXX_get_eitv1(eitvXXX_t eitv, bool* perror, ap_linyyyXXX_t a, ap_environment_t* env, ap_var_t var);
  /* Get the coefficient of cst/dimension dim/variable var and assign it to
     eitv */

static inline void ap_linyyyXXX_set_cst(ap_linyyyXXX_t a, eitvXXX_t eitv);
cinline void ap_linyyyXXX_set_eitv0(ap_linyyyXXX_t a, ap_dim_t dim, eitvXXX_t eitv);
cinline void ap_linyyyXXX_set_eitv1(ap_linyyyXXX_t a, bool* perror, ap_environment_t* env, ap_var_t var, eitvXXX_t eitv);
  /* Assign the coefficient of cst/dimension dim/variable var in the
     expression */

static inline eitvXXX_ptr ap_linyyyXXX_cstref(ap_linyyyXXX_t a);
  /* Get a reference to the constant */
cinline eitvXXX_ptr ap_linyyyXXX_eitvref0(ap_linyyyXXX_t a, ap_dim_t dim, bool create);
cinline eitvXXX_ptr ap_linyyyXXX_eitvref1(bool* perror, ap_linyyyXXX_t a, ap_environment_t* env, ap_var_t var, bool create);
  /* Get a (possibly NULL) reference to the coefficient associated to the
     dimension/variable.
     If create==false, returns NULL if the corresponding dimension does not exist.
     If create==true, a new coefficient may be added.
 */

bool ap_linyyyXXX_set_list0(num_internal_t intern, ap_linyyyXXX_t a, bool* perror, ...);
  /* This function assigns the linear expression from a list of tags of type
     ap_coeff_tag_t, defined in ap_coeff.h, each followed by a number of
     arguments as specified in the definition of the type ap_coefftag_t, and
     ended by the tag AP_END;

     - If the same dimension appears several times, only the last tag
       referring to it is taken into account.

     Returns true iff all conversions were exact.

     Example:
     ap_linyyyXXX_set_list(intern,
			  expr,
			  AP_COEFF_LFRAC,7,9,0,
			  AP_COEFF_DOUBLE2,-3.0,4.5,1,
			  AP_CST_LLINT,3LL,
			  AP_END)
     sets expr to "7/9 x0 + [-3,4.5] x1 + 3"
     assuming that the expression was "0" before the call and that all the
     number conversions were exact.
  */
bool ap_linyyyXXX_set_list1(num_internal_t intern, ap_linyyyXXX_t a, bool* perror, ap_environment_t* env, ...);

#if defined(_AP_expr_MARK_)
/* Internal function */
eitvXXX_ptr ap_linexprXXX_set_list_get_eitvXXX_of_dim(
    ap_linexprXXX_t expr, ap_environment_t* env, bool cst, va_list* va);
eitvXXX_ptr ap_linexprXXX_set_list_get_eitvXXX_of_var(
    ap_linexprXXX_t expr, ap_environment_t* env, bool cst, va_list* va);
bool ap_linexprXXX_set_list_generic(
    eitvXXX_ptr (*get_eitvXXX_of_dimvar)(
	ap_linexprXXX_t expr, ap_environment_t* env, bool cst, va_list* va
    ),
    ap_linexprXXX_t expr, bool* perror,
    ap_environment_t* env, va_list* va, num_internal_t intern
);

/* Iterator (Macro): use:
   ap_linexprXXX_ForeachLinterm(ap_linexprXXX_t e, size_t i, ap_dim_t d, eitvXXX_ptr eitv){
     ..
   }
   where
   - e is the inspected expression (of type ap_linexprXXX_t)
   - i is the internal iterator (of type size_t or int)
   - dim is the dimension of one linear term (of type ap_dim_t)
   - peitv is a pointer to the corresponding interval (of type eitvXXX_ptr)

*/
#define ap_linexprXXX_ForeachLinterm0(_p_e, _p_i, _p_d, _p_eitv)	\
  for ((_p_i)=0;							\
       (_p_i)<(_p_e)->effsize ?						\
	 (((_p_d) = (_p_e)->linterm[_p_i]->dim),			\
	  ((_p_eitv) = (_p_e)->linterm[_p_i]->eitv),			\
	  true) :                                                       \
	 false;								\
       (_p_i)++)
#define ap_linexprXXX_ForeachLinterm1(_p_e, _p_env, _p_i, _p_v, _p_eitv) \
  for ((_p_i)=0;							\
       (_p_i)<(_p_e)->effsize ?						\
	 (((_p_v) = ap_environment_var_of_dim((_p_e)->linterm[_p_i]->dim)), \
	  ((_p_eitv) = (_p_e)->linterm[_p_i]->eitv),			\
	  true) :                                                       \
	 false;								\
       (_p_i)++)
#else
#define ap_linyyyXXX_ForeachLinterm0(_p_a, _p_i, _p_d, _p_eitv) ap_linexprXXX_ForeachLinterm0(_p_a->linexpr,_p_i,_p_d,_p_eitv)
#define ap_linyyyXXX_ForeachLinterm1(_p_a, _p_env, _p_i, _p_v, _p_eitv) ap_linexprXXX_ForeachLinterm1(_p_a->linexpr, _p_env, _p_i,_p_v,_p_eitv)
#endif

/* ====================================================================== */
/* I.4 Conversions */
/* ====================================================================== */

MACRO_FOREACH ZZZ @MainNum
bool ap_linyyyXXX_set_linyyyZZZ(ap_linyyyXXX_t a, ap_linyyyZZZ_t b, num_internal_t intern);
bool ap_linyyyZZZ_set_linyyyXXX(ap_linyyyZZZ_t a, ap_linyyyXXX_t b, num_internal_t intern);
ENDMACRO

/* ====================================================================== */
/* I.5. Change of dimensions and permutations */
/* ====================================================================== */

/* These two functions add dimensions to the expressions, following the
   semantics of dimchange (see the type definition of dimchange).  */
cinline
void ap_linyyyXXX_add_dimensions(ap_linyyyXXX_t res,
				ap_linyyyXXX_t a,
				ap_dimchange_t* dimchange);

/* These two functions apply the given permutation to the dimensions.
   The dimensions present in the generator should just be less
   than the size of the permutation. */
cinline
void ap_linyyyXXX_permute_dimensions(ap_linyyyXXX_t res,
				    ap_linyyyXXX_t a,
				    ap_dimperm_t* perm);
cinline
void ap_linyyyXXX_extend_environment(ap_linyyyXXX_t res,
				      bool* perror,
				      ap_environment_t* nenv,
				      ap_linyyyXXX_t a,
				      ap_environment_t* env);

/* ====================================================================== */
/* I.6 Hashing, comparison */
/* ====================================================================== */

cinline int ap_linyyyXXX_hash(ap_linyyyXXX_t a);
static inline bool ap_linyyyXXX_equal(ap_linyyyXXX_t a1,ap_linyyyXXX_t a2);

/* Lexicographic ordering, terminating by constant coefficients */
int ap_linyyyXXX_cmp(ap_linyyyXXX_t a1, ap_linyyyXXX_t a2);

/* ====================================================================== */
/* I.7 Serialization */
/* ====================================================================== */

size_t ap_linyyyXXX_serialize(void* dst, ap_linyyyXXX_t src);
size_t ap_linyyyXXX_deserialize(ap_linyyyXXX_t dst, const void* src);
size_t ap_linyyyXXX_serialized_size(ap_linyyyXXX_t a);

/* ********************************************************************** */
/* II. ap_linyyyXXX_array_t */
/* ********************************************************************** */

/* ====================================================================== */
/* II.1 Constructor and Destructor */
/* ====================================================================== */

void ap_linyyyXXX_array_init(ap_linyyyXXX_array_t array, size_t size);
void ap_linyyyXXX_array_init_set(ap_linyyyXXX_array_t res, ap_linyyyXXX_array_t array);
void ap_linyyyXXX_array_set(ap_linyyyXXX_array_t res, ap_linyyyXXX_array_t array);
void ap_linyyyXXX_array_resize(ap_linyyyXXX_array_t array, size_t size);
void ap_linyyyXXX_array_minimize(ap_linyyyXXX_array_t array);
void ap_linyyyXXX_array_clear(ap_linyyyXXX_array_t array);

ap_linyyyXXX_array_ptr ap_linyyyXXX_array_alloc(size_t size);
ap_linyyyXXX_array_ptr ap_linyyyXXX_array_alloc_set(ap_linyyyXXX_array_t a);
void ap_linyyyXXX_array_free(ap_linyyyXXX_array_ptr array);

void ap_linyyyXXX_array_fprint(FILE* stream, ap_linyyyXXX_array_t array, char** name);
void ap_linyyyXXX_array_print(ap_linyyyXXX_array_t array, char** name);

/* ====================================================================== */
/* II.2 Tests and Simplifications */
/* ====================================================================== */

bool ap_linyyyXXX_array_is_linear(ap_linyyyXXX_array_t array);
bool ap_linyyyXXX_array_is_quasilinear(ap_linyyyXXX_array_t array);
ap_linexpr_type_t ap_linyyyXXX_array_type(ap_linyyyXXX_array_t array);

ap_dim_t ap_linyyyXXX_array_max_dim(ap_linyyyXXX_array_t a);
  /* Returns the maximum ap_dim_t PLUS ONE of all dimensions in
     expression/constraint/generator, and 0 if no dimension at all.

     For instance, it returns 3 on the expression x2. */

size_t ap_linyyyXXX_array_support(ap_linyyyXXX_array_t array, ap_dim_t* tdim, size_t nbdim);
size_t ap_linyyyXXX_array_supportinterval(ap_linyyyXXX_array_t array, ap_dim_t* tdim, size_t nbdim);
/* Fills the array tdim with the dimensions associated with intervals in the
   linear expression, in increasing order, and return the number of such
   dimensions.

   tdim is supposed to be of size at least the maximum number of different dimensions in the array (as return by max_dim function)
*/

/* ====================================================================== */
/* II.4 Conversions */
/* ====================================================================== */

MACRO_FOREACH ZZZ @MainNum
bool ap_linyyyXXX_array_set_linyyyZZZ_array(ap_linyyyXXX_array_t a, ap_linyyyZZZ_array_t b, num_internal_t intern);
bool ap_linyyyZZZ_array_set_linyyyXXX_array(ap_linyyyZZZ_array_t a, ap_linyyyXXX_array_t b, num_internal_t intern);
ENDMACRO

/* ====================================================================== */
/* II.5 Change of dimensions and permutations */
/* ====================================================================== */

/* These two functions add dimensions to the expressions, following the
   semantics of dimchange (see the type definition of dimchange).  */
void ap_linyyyXXX_array_add_dimensions(ap_linyyyXXX_array_t res,
				      ap_linyyyXXX_array_t array,
				      ap_dimchange_t* dimchange);

/* These two functions apply the given permutation to the dimensions.
   The dimensions present in the generator should just be less
   than the size of the permutation. */
void ap_linyyyXXX_array_permute_dimensions(ap_linyyyXXX_array_t res,
					  ap_linyyyXXX_array_t array,
					  ap_dimperm_t* dimchange);
void ap_linyyyXXX_array_extend_environment(ap_linyyyXXX_array_t res,
					   bool* perror,
					   ap_environment_t* nenv,
					   ap_linyyyXXX_array_t expr,
					   ap_environment_t* env);
/* ====================================================================== */
/* II.6 Hashing, comparison */
/* ====================================================================== */

int ap_linyyyXXX_array_hash(ap_linyyyXXX_array_t a);
bool ap_linyyyXXX_array_equal(ap_linyyyXXX_array_t a1,ap_linyyyXXX_array_t a2);
int ap_linyyyXXX_array_cmp(ap_linyyyXXX_array_t a1, ap_linyyyXXX_array_t a2);

/* ====================================================================== */
/* II.7 Serialization */
/* ====================================================================== */

size_t ap_linyyyXXX_array_serialize(void* dst, ap_linyyyXXX_array_t src);
size_t ap_linyyyXXX_array_deserialize(ap_linyyyXXX_array_t dst, const void* src);
size_t ap_linyyyXXX_array_serialized_size(ap_linyyyXXX_array_t a);

/* ********************************************************************** */
/* ********************************************************************** */
/* Definition of inline functions */
/* ********************************************************************** */
/* ********************************************************************** */

/* ====================================================================== */
/* I.1 Constructor and Destructor */
/* ====================================================================== */

#if defined(_AP_expr_MARK_)
static inline void ap_lintermXXX_init(ap_lintermXXX_t term)
{ eitvXXX_init(term->eitv); term->dim = AP_DIM_MAX; }
static inline void ap_lintermXXX_init_set(ap_lintermXXX_t res, ap_lintermXXX_t term)
{ eitvXXX_init_set(res->eitv,term->eitv); res->dim = term->dim; }
static inline void ap_lintermXXX_set(ap_lintermXXX_t res, ap_lintermXXX_t term)
{ eitvXXX_set(res->eitv,term->eitv); res->dim = term->dim; }
static inline void ap_lintermXXX_clear(ap_lintermXXX_t term)
{ eitvXXX_clear(term->eitv); }
static inline void ap_lintermXXX_swap(ap_lintermXXX_t a, ap_lintermXXX_t b)
{ if (a!=b){ ap_lintermXXX_struct t=*a; *a=*b; *b=t; } }
#endif

#if defined(_AP_expr_MARK_)
static inline void ap_linexprXXX_resize(ap_linexprXXX_t expr, size_t size)
{
  if (size>expr->maxsize) ap_linexprXXX_resize_strict(expr,size);
  if (size<expr->effsize) expr->effsize = size;
}
static inline void ap_linexprXXX_set_zero(ap_linexprXXX_t expr)
{
  ap_linexprXXX_resize(expr,0);
  eitvXXX_set_int(expr->cst, 0);
}
#else
static inline void ap_linyyyXXX_init(ap_linyyyXXX_t a, size_t size)
{
  ap_linexprXXX_init(a->linexpr,size);
  a->yyytyp = 0;
#if defined(_AP_cons_MARK_)
  mpq_init(a->mpq);
#endif
}
static inline void ap_linyyyXXX_set(ap_linyyyXXX_t a, ap_linyyyXXX_t b)
{
  if (a!=b){
    ap_linexprXXX_set(a->linexpr,b->linexpr);
    a->yyytyp = b->yyytyp;
#if defined(_AP_cons_MARK_)
    mpq_set(a->mpq,b->mpq);
#endif
  }
}
static inline void ap_linyyyXXX_init_set(ap_linyyyXXX_t a, ap_linyyyXXX_t b)
{
  assert(a!=b);
  ap_linexprXXX_init_set(a->linexpr,b->linexpr);
  a->yyytyp = b->yyytyp;
#if defined(_AP_cons_MARK_)
  mpq_init(a->mpq);
  mpq_set(a->mpq,b->mpq);
#endif
}
static inline void ap_linyyyXXX_resize(ap_linyyyXXX_t e, size_t size)
{ ap_linexprXXX_resize(e->linexpr,size); }
static inline void ap_linyyyXXX_set_zero(ap_linyyyXXX_t e)
{ ap_linexprXXX_set_zero(e->linexpr); }
static inline void ap_linyyyXXX_resize_strict(ap_linyyyXXX_t e, size_t size)
{ ap_linexprXXX_resize_strict(e->linexpr,size); }
static inline void ap_linyyyXXX_minimize(ap_linyyyXXX_t e)
{ ap_linexprXXX_minimize(e->linexpr); }
static inline void ap_linyyyXXX_clear(ap_linyyyXXX_t a)
{
  ap_linexprXXX_clear(a->linexpr);
#if defined(_AP_cons_MARK_)
  mpq_clear(a->mpq);
#endif
}
#endif

static inline void ap_linyyyXXX_swap(ap_linyyyXXX_t a, ap_linyyyXXX_t b)
{ if (a!=b){ ap_linyyyXXX_struct t=*a; *a=*b; *b=t; } }

/* ====================================================================== */
/* I.2 Test */
/* ====================================================================== */

#if defined(_AP_expr_MARK_)
static inline ap_dim_t ap_linexprXXX_max_dim(ap_linexprXXX_t expr)
{
  return (expr->effsize==0) ? 0 : 1+expr->linterm[expr->effsize-1]->dim;
}
#else
static inline bool ap_linyyyXXX_is_integer(ap_linyyyXXX_t a, size_t intdim)
{
  return ap_linexprXXX_is_integer(a->linexpr,intdim);
}
static inline bool ap_linyyyXXX_is_real(ap_linyyyXXX_t a, size_t intdim)
{
  return ap_linexprXXX_is_real(a->linexpr,intdim);
}
static inline bool ap_linyyyXXX_is_linear(ap_linyyyXXX_t a)
{
  return ap_linexprXXX_is_linear(a->linexpr);
}
static inline bool ap_linyyyXXX_is_quasilinear(ap_linyyyXXX_t a)
{
  return ap_linexprXXX_is_quasilinear(a->linexpr);
}
static inline ap_linexpr_type_t ap_linyyyXXX_type(ap_linyyyXXX_t a)
{
  return ap_linexprXXX_type(a->linexpr);
}
static inline ap_dim_t ap_linyyyXXX_max_dim(ap_linyyyXXX_t a)
{
  return ap_linexprXXX_max_dim(a->linexpr);
}
static inline void ap_linyyyXXX_support_mask(ap_linyyyXXX_t a, ap_dim_t* tdim)
{
  ap_linexprXXX_support_mask(a->linexpr,tdim);
}
static inline void
 ap_linyyyXXX_supportinterval_mask(ap_linyyyXXX_t a, ap_dim_t* tdim)
{
  ap_linexprXXX_supportinterval_mask(a->linexpr,tdim);
}
static inline size_t ap_linyyyXXX_support(ap_linyyyXXX_t a, ap_dim_t* tdim, size_t size)
{
  return ap_linexprXXX_support(a->linexpr,tdim,size);
}
  static inline size_t ap_linyyyXXX_supportinterval(ap_linyyyXXX_t a, ap_dim_t* tdim, size_t size)
{
  return ap_linexprXXX_supportinterval(a->linexpr,tdim,size);
}
#endif

/* ====================================================================== */
/* I.3 Access */
/* ====================================================================== */

#if defined(_AP_expr_MARK_)
static inline size_t ap_linexprXXX_size(ap_linyyyXXX_t a)
{ return a->effsize; }
static inline void ap_linexprXXX_get_cst(eitvXXX_t eitv, ap_linexprXXX_t expr)
{ eitvXXX_set(eitv,expr->cst); }
static inline void ap_linexprXXX_set_cst(ap_linexprXXX_t expr, eitvXXX_t eitv)
  { eitvXXX_set(expr->cst,eitv); }
static inline eitvXXX_ptr ap_linexprXXX_cstref(ap_linexprXXX_t expr)
{ return expr->cst; }
#else
static inline size_t ap_linyyyXXX_size(ap_linyyyXXX_t a)
{ return ap_linexprXXX_size(a->linexpr); }
static inline void ap_linyyyXXX_get_cst(eitvXXX_t eitv, ap_linyyyXXX_t a)
{ ap_linexprXXX_get_cst(eitv,a->linexpr); }
static inline void ap_linyyyXXX_set_cst(ap_linyyyXXX_t a, eitvXXX_t eitv)
{ ap_linexprXXX_set_cst(a->linexpr,eitv); }
static inline eitvXXX_ptr ap_linyyyXXX_cstref(ap_linyyyXXX_t a)
{ return ap_linexprXXX_cstref(a->linexpr); }
#endif

#if !defined(_AP_expr_MARK_)
static inline void ap_linyyyXXX_get_eitv0(eitvXXX_t eitv, ap_linyyyXXX_t a, ap_dim_t dim)
{ ap_linexprXXX_get_eitv0(eitv,a->linexpr,dim); }
static inline void ap_linyyyXXX_get_eitv1(eitvXXX_t eitv, bool* perror, ap_linyyyXXX_t a, ap_environment_t* env, ap_var_t var)
{ ap_linexprXXX_get_eitv1(eitv,perror,a->linexpr,env,var); }
static inline void ap_linyyyXXX_set_eitv0(ap_linyyyXXX_t a, ap_dim_t dim, eitvXXX_t eitv)
{ ap_linexprXXX_set_eitv0(a->linexpr,dim,eitv); }
static inline void ap_linyyyXXX_set_eitv1(ap_linyyyXXX_t a, bool* perror, ap_environment_t* env, ap_var_t var, eitvXXX_t eitv)
{ ap_linexprXXX_set_eitv1(a->linexpr,perror,env,var,eitv); }
static inline eitvXXX_ptr ap_linyyyXXX_eitvref0(ap_linyyyXXX_t a, ap_dim_t dim, bool create)
{ return ap_linexprXXX_eitvref0(a->linexpr,dim,create); }
static inline eitvXXX_ptr ap_linyyyXXX_eitvref1(bool* perror, ap_linyyyXXX_t a, ap_environment_t* env, ap_var_t var, bool create)
{ return ap_linexprXXX_eitvref1(perror,a->linexpr,env,var,create); }
#endif

/* ====================================================================== */
/* I.5. Change of dimensions and permutations */
/* ====================================================================== */

#if !defined(_AP_expr_MARK_)
static inline
void ap_linyyyXXX_add_dimensions(ap_linyyyXXX_t res,
				ap_linyyyXXX_t a,
				ap_dimchange_t* dimchange)
{
  ap_linexprXXX_add_dimensions(res->linexpr,a->linexpr,dimchange);
  if (res!=a){
    res->yyytyp = a->yyytyp;
#if defined(_AP_cons_MARK_)
    mpq_set(res->mpq,a->mpq);
#endif
  }
}
static inline
void ap_linyyyXXX_permute_dimensions(ap_linyyyXXX_t res,
				    ap_linyyyXXX_t a,
				    ap_dimperm_t* dimperm)
{
  ap_linexprXXX_permute_dimensions(res->linexpr,a->linexpr,dimperm);
  if (res!=a){
    res->yyytyp = a->yyytyp;
#if defined(_AP_cons_MARK_)
    mpq_set(res->mpq,a->mpq);
#endif
  }
}
static inline
void ap_linyyyXXX_extend_environment(ap_linyyyXXX_t res,
				      bool* perror,
				      ap_environment_t* nenv,
				      ap_linyyyXXX_t a,
				      ap_environment_t* env)
{
  ap_linexprXXX_extend_environment(res->linexpr,perror,nenv,a->linexpr,env);
  if (res!=a){
    res->yyytyp = a->yyytyp;
#if defined(_AP_cons_MARK_)
    mpq_set(res->mpq,a->mpq);
#endif
  }
}
#endif

#if !defined(_AP_expr_MARK_)
static inline
int ap_linyyyXXX_hash(ap_linyyyXXX_t a)
{
  return (int)a->yyytyp + ap_linexprXXX_hash(a->linexpr);
}
#endif

static inline bool ap_linyyyXXX_equal(ap_linyyyXXX_t a1, ap_linyyyXXX_t a2)
{ return ap_linyyyXXX_cmp(a1,a2)==0; }

#undef cinline
#undef _AP_yyy_MARK_

#ifdef __cplusplus
}
#endif

#endif
