/* ********************************************************************** */
/* itvXXX_linexpr.h */
/* ********************************************************************** */

#ifndef _ITVXXX_LINEXPR_H_
#define _ITVXXX_LINEXPR_H_

#include <stdarg.h>
#include "itvXXX_types.h"
#include "ap_dimension.h"
#include "ap_coeff.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* I. Constructor and Destructor */
/* ********************************************************************** */

static inline void itvXXX_linterm_init(itvXXX_linterm_t term);
static inline void itvXXX_linterm_init_set(itvXXX_linterm_t res, itvXXX_linterm_t term);
static inline void itvXXX_linterm_set(itvXXX_linterm_t res, itvXXX_linterm_t term);
static inline void itvXXX_linterm_clear(itvXXX_linterm_t term);
static inline void itvXXX_linterm_swap(itvXXX_linterm_t a, itvXXX_linterm_t b);

void itvXXX_linexpr_init(itvXXX_linexpr_t expr, size_t size);
void itvXXX_linexpr_init_set(itvXXX_linexpr_t res, itvXXX_linexpr_t expr);
void itvXXX_linexpr_set(itvXXX_linexpr_t res, itvXXX_linexpr_t expr);
void itvXXX_linexpr_resize(itvXXX_linexpr_t expr, size_t size);
void itvXXX_linexpr_minimize(itvXXX_linexpr_t e);
void itvXXX_linexpr_clear(itvXXX_linexpr_t expr);

itvXXX_linexpr_ptr itvXXX_linexpr_alloc(size_t size);
itvXXX_linexpr_ptr itvXXX_linexpr_alloc_set(itvXXX_linexpr_t expr);
void itvXXX_linexpr_free(itvXXX_linexpr_ptr expr);

void itvXXX_linexpr_fprint(FILE* stream, itvXXX_linexpr_t expr, char** name);
static inline void itvXXX_linexpr_print(itvXXX_linexpr_t expr, char** name);

void itvXXX_linexpr_array_init(itvXXX_linexpr_array_t array, size_t size);
void itvXXX_linexpr_array_init_set(itvXXX_linexpr_array_t res, itvXXX_linexpr_array_t array);
void itvXXX_linexpr_array_set(itvXXX_linexpr_array_t res, itvXXX_linexpr_array_t array);
void itvXXX_linexpr_array_resize(itvXXX_linexpr_array_t array, size_t size);
void itvXXX_linexpr_array_minimize(itvXXX_linexpr_array_t array);
void itvXXX_linexpr_array_clear(itvXXX_linexpr_array_t array);

itvXXX_linexpr_array_ptr itvXXX_linexpr_array_alloc(size_t size);
itvXXX_linexpr_array_ptr itvXXX_linexpr_array_alloc_set(itvXXX_linexpr_array_t a);
  void itvXXX_linexpr_array_free(itvXXX_linexpr_array_ptr expr);

void itvXXX_linexpr_array_fprint(FILE* stream, itvXXX_linexpr_array_t array, char** name);
static inline void itvXXX_linexpr_array_print(itvXXX_linexpr_array_t array, char** name);

/* ********************************************************************** */
/* II. Tests */
/* ********************************************************************** */

bool itvXXX_linexpr_is_integer(itvXXX_linexpr_t expr, size_t intdim);
  /* Does the expression depends only on integer variables ? assuming
     that the first intdim dimensions are integer */
bool itvXXX_linexpr_is_real(itvXXX_linexpr_t expr, size_t intdim);
  /* Does the expression depends only on real variables ? assuming
     that the first intdim dimensions are integer */

  /* Expression classification */

static inline bool itvXXX_linexpr_is_linear(itvXXX_linexpr_t expr);
  /* Return true iff all involved coefficients are scalars */
bool itvXXX_linexpr_is_quasilinear(itvXXX_linexpr_t expr);
  /* Return true iff all involved coefficients but the constant are scalars. */
itvlinexpr_type_t itvXXX_linexpr_type(itvXXX_linexpr_t a);
  /* Classify an expression */

bool itvXXX_linexpr_array_is_linear(itvXXX_linexpr_array_t array);
bool itvXXX_linexpr_array_is_quasilinear(itvXXX_linexpr_array_t array);
itvlinexpr_type_t itvXXX_linexpr_array_type(itvXXX_linexpr_array_t array);


/* ********************************************************************** */
/* III. Access */
/* ********************************************************************** */

static inline
size_t itvXXX_linexpr_size(itvXXX_linexpr_t expr);
  /* Get the size of the linear expression */

eitvXXX_ptr itvXXX_linexpr_eitvref(itvXXX_linexpr_t expr, ap_dim_t dim, bool create);
  /* Get a (possibly NULL) reference to the coefficient associated to the
     dimension.
     If create==false, returns NULL if the corresponding dimension does not exist.
     If create==true, a new coefficient may be added.
 */

/* Set the coefficient of dimension dim in the expression */
static inline void itvXXX_linexpr_set_eitv(itvXXX_linexpr_t expr, ap_dim_t dim, eitvXXX_t eitv);

/* Defined in itvConfig.h for avoiding multiple definitions */
/*
typedef enum itv_coefftag_t {
  ITV_COEFF,           waiting for 1 ap_coeff_t* and a dimension
  ITV_EITV,            waiting for 1 eitvXXX_t and a dimension
  ITV_NUM,             waiting for 1 num_t and a dimension
  ITV_NUM2,            waiting for 2 num_t and a dimension
  ITV_LINT,            waiting for 1 long int and a dimension
  ITV_LINT2,           waiting for 2 long int and a dimension
  ITV_LLINT,           waiting for 1 long long int and a dimension
  ITV_LLINT2,          waiting for 2 long long int and a dimension
  ITV_MPZ,             waiting for 1 mpz_t and a dimension
  ITV_MPZ2,            waiting for 2 mpz_t and a dimension
  ITV_LFRAC,           waiting for 2 long int and a dimension
  ITV_LFRAC2,          waiting for 4 long int and a dimension
  ITV_LLFRAC,          waiting for 2 long long int and a dimension
  ITV_LLFRAC2,         waiting for 4 long long int and a dimension
  ITV_MPQ,             waiting for 1 mpq_t and a dimension
  ITV_MPQ2,            waiting for 2 mpq_t and a dimension
  ITV_DOUBLE,          waiting for 1 double and a dimension
  ITV_DOUBLE2,         waiting for 2 double and a dimension
  ITV_LDOUBLE,         waiting for 1 long double and a dimension
  ITV_LDOUBLE2,        waiting for 2 long double and a dimension
  ITV_MPFR,            waiting for 1 mpfr_t and a dimension
  ITV_MPFR2,           waiting for 2 mpfr_t double and a dimension
  ITV_END
} itvcoefftag_t;
*/

bool itvXXX_linexpr_set_list_generic(eitvXXX_ptr (*get_eitvXXX_of_dimvar)(void* env, void* expr, va_list* va),
				     void* env,
				     numinternal_t intern,
				     void* expr, va_list* va);
eitvXXX_ptr itvXXX_linexpr_set_list_get_eitvXXX_of_dim(void* env, void* expr, va_list* va);

bool itvXXX_linexpr_set_list(numinternal_t intern, itvXXX_linexpr_t expr, ...);
  /* This function assigns the linear expression from a list of tags of type
     itv_coefftag_t, each followed by a number of arguments as specified in
     the definition of the type ap_coefftag_t, and ended by the tag ITV_END;

     - The dimension AP_DIM_MAX is used to refer to the constat coefficient.
     - If the same dimension appears several times, only the last tag
       referring to it is taken into account.

     Returns true iff all conversions were exact.

     Example:
     itvXXX_linexpr_set_list(intern,
			  expr,
			  ITV_LFRAC,7,9,0,
			  ITV_DOUBLE2,-3.0,4.5,1,
			  ITV_LLINT,3LL,AP_DIM_MAX,
			  ITV_END)
     sets expr to "7/9 x0 + [-3,4.5] x1 + 3"
     assuming that the expression was "0" before the call and that all the
     number conversions were exact.
  */


/* Iterator (Macro): use:
   itvXXX_linexpr_ForeachLinterm(itvXXX_linexpr_t e, size_t i, ap_dim_t d, eitvXXX_ptr eitv){
     ..
   }
   where
   - e is the inspected expression (of type itvXXX_linexpr_t)
   - i is the internal iterator (of type size_t or int)
   - dim is the dimension of one linear term (of type ap_dim_t)
   - peitv is a pointer to the corresponding interval (of type eitvXXX_ptr)

*/
#define itvXXX_linexpr_ForeachLinterm(_p_e, _p_i, _p_d, _p_eitv) \
  for ((_p_i)=0;							\
       (_p_i)<(_p_e)->size ?						\
	 (((_p_d) = (_p_e)->linterm[_p_i]->dim),			\
	  ((_p_eitv) = (_p_e)->linterm[_p_i]->eitv),			\
	  ((_p_d)!=AP_DIM_MAX)) :					\
	 false;								\
       (_p_i)++)

/* ********************************************************************** */
/* IV. Arithmetic */
/* ********************************************************************** */

void itvXXX_linexpr_neg(itvXXX_linexpr_t res, itvXXX_linexpr_t expr);
  /* Negate an expression */
void itvXXX_linexpr_scale(itvXXX_internal_t* intern,
			  itvXXX_linexpr_t res, itvXXX_linexpr_t expr, eitvXXX_t coeff);
void itvXXX_linexpr_div(itvXXX_internal_t* intern,
			itvXXX_linexpr_t res, itvXXX_linexpr_t expr, eitvXXX_t coeff);
  /* Scale an expression by an interval */

void itvXXX_linexpr_add(itvXXX_internal_t* intern,
			itvXXX_linexpr_t expr,
			itvXXX_linexpr_t exprA,
			itvXXX_linexpr_t exprB);
void itvXXX_linexpr_sub(itvXXX_internal_t* intern,
			itvXXX_linexpr_t expr,
			itvXXX_linexpr_t exprA,
			itvXXX_linexpr_t exprB);
  /* Resp. add and substract two linear epxression.
     (Substraction temporarily negates exprB, and then restores it */

/* ********************************************************************** */
/* V. Evaluation and Quasilinearisation of interval linear expressions */
/* ********************************************************************** */

bool itvXXX_linexpr_eval(itvXXX_internal_t* intern,
			 itvXXX_t res, itvXXX_linexpr_t expr, itvXXX_t* env);
  /* Evaluate an interval linear expression. Return true if no
     approximations. */

size_t itvXXX_linexpr_supportinterval(itvXXX_linexpr_t expr, ap_dim_t* tdim);
/* Fills the array tdim with the dimensions associated with intervals in the
   linear expression, in increasing order, and return the number of such
   dimensions.

   tdim is supposed to be of size at least the maximum dimension + 1 in the
   expression.
 */
size_t itvXXX_linexpr_array_supportinterval(itvXXX_linexpr_array_t array, ap_dim_t* tdim, size_t maxdim1);
  /* Idem for arrays.

     For the parameter maxdim1: corresponds to the maximal possible dimension
     + 1 */

/* These functions quasilinearize in-place expressions and constraints.  They
   optimize (sets of) constraints when the parameter meet is true, by
   deducing things. If constraints are quasilinearized for testing
   satisfaction, meet should be set to false.
*/
bool itvXXX_linexpr_quasilinearize(itvXXX_internal_t* intern,
				   itvXXX_linexpr_t linexpr,
				   itvXXX_t* env,
				   bool for_meet_inequality);
  /* Quasilinearize in-place linexpr using the bounding box itv. Return true
     if no approximations. */

bool itvXXX_linexpr_array_quasilinearize(itvXXX_internal_t* intern,
					 itvXXX_linexpr_array_t array,
					 itvXXX_t* env);
  /* Same for an array */

/* ********************************************************************** */
/* VI. Change of dimensions and permutations */
/* ********************************************************************** */

/* These two functions add dimensions to the expressions, following the
   semantics of dimchange (see the type definition of dimchange).  */
void itvXXX_linexpr_add_dimensions(itvXXX_linexpr_t res,
				   itvXXX_linexpr_t expr,
				   ap_dimchange_t* dimchange);
void itvXXX_linexpr_array_add_dimensions(itvXXX_linexpr_array_t res,
					 itvXXX_linexpr_array_t array,
					 ap_dimchange_t* dimchange);

/* These two functions apply the given permutation to the dimensions.
   The dimensions present in the expression should just be less
   than the size of the permutation. */
void itvXXX_linexpr_permute_dimensions(itvXXX_linexpr_t res,
				       itvXXX_linexpr_t expr,
				       ap_dimperm_t* perm);
void itvXXX_linexpr_array_permute_dimensions(itvXXX_linexpr_array_t res,
					     itvXXX_linexpr_array_t array,
					     ap_dimperm_t* dimchange);

/* ********************************************************************** */
/* VII. Hashing, comparison */
/* ********************************************************************** */

int itvXXX_linexpr_hash(itvXXX_linexpr_t expr);
static inline bool itvXXX_linexpr_equal(itvXXX_linexpr_t expr1,itvXXX_linexpr_t expr2);

/* Lexicographic ordering, terminating by constant coefficients */
int itvXXX_linexpr_compare(itvXXX_linexpr_t expr1, itvXXX_linexpr_t expr2);

/* ********************************************************************** */
/* Definition of inline functions */
/* ********************************************************************** */

static inline void itvXXX_linterm_init(itvXXX_linterm_t term)
{ eitvXXX_init(term->eitv); term->dim = AP_DIM_MAX; }
static inline void itvXXX_linterm_init_set(itvXXX_linterm_t res, itvXXX_linterm_t term)
{ eitvXXX_init_set(res->eitv,term->eitv); res->dim = term->dim; }
static inline void itvXXX_linterm_set(itvXXX_linterm_t res, itvXXX_linterm_t term)
{ eitvXXX_set(res->eitv,term->eitv); res->dim = term->dim; }
static inline void itvXXX_linterm_clear(itvXXX_linterm_t term)
{ eitvXXX_clear(term->eitv); }
static inline void itvXXX_linterm_swap(itvXXX_linterm_t a, itvXXX_linterm_t b)
{ if (a!=b){ itvXXX_linterm_struct t=*a; *a=*b; *b=t; } }

static inline void itvXXX_linexpr_print(itvXXX_linexpr_t expr, char** name)
{ itvXXX_linexpr_fprint(stdout,expr,name); }
static inline void itvXXX_linexpr_array_print(itvXXX_linexpr_array_t array, char** name)
{ itvXXX_linexpr_array_fprint(stdout,array,name); }

static inline bool itvXXX_linexpr_is_linear(itvXXX_linexpr_t expr)
{ return eitvXXX_is_point(expr->cst) && itvXXX_linexpr_is_quasilinear(expr); }

static inline size_t itvXXX_linexpr_size(itvXXX_linexpr_t expr)
{
  size_t i,dim,size;
  eitvXXX_ptr eitv;

  size = 0;
  itvXXX_linexpr_ForeachLinterm(expr,i,dim,eitv){
    size++;
  }
  return size;
}

static inline void itvXXX_linexpr_set_eitv(itvXXX_linexpr_t expr, ap_dim_t dim, eitvXXX_t eitv)
{
  eitvXXX_ptr r = itvXXX_linexpr_eitvref(expr,dim,true);
  eitvXXX_set(r,eitv);
}
static inline bool itvXXX_linexpr_equal(itvXXX_linexpr_t expr1,itvXXX_linexpr_t expr2)
{ return itvXXX_linexpr_compare(expr1,expr2)==0; }

/* Internal function */
void itvXXX_support_merge(ap_dim_t* ttdim[3], size_t tnb[3], size_t* pk);

#ifdef __cplusplus
}
#endif

#endif
