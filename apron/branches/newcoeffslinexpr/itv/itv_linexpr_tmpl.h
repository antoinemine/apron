/* ********************************************************************** */
/* itv_linexpr.h */
/* ********************************************************************** */

#ifndef _ITV_LINEXPR_H_
#define _ITV_LINEXPR_H_

#include <stdarg.h>
#include "eitv.h"
#include "ap_dimension.h"
#include "ap_coeff.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* Dataypes */
/* ********************************************************************** */

/* Interval linear term */
typedef struct itv_linterm_struct {
  eitv_t eitv;
  ap_dim_t dim;
} itv_linterm_struct;
typedef itv_linterm_struct* itv_linterm_ptr;
typedef itv_linterm_struct itv_linterm_t[1];

/* Interval linear expression */
typedef struct itv_linexpr_struct {
  itv_linterm_t* linterm;
  size_t size;
  eitv_t cst;
} itv_linexpr_struct;
typedef itv_linexpr_struct itv_linexpr_t[1];
typedef itv_linexpr_struct* itv_linexpr_ptr;

/* Array of interval linear expressions */
typedef struct itv_linexpr_array_struct {
  itv_linexpr_t* p;
  size_t size;
} itv_linexpr_array_struct;
typedef itv_linexpr_array_struct itv_linexpr_array_t[1];
typedef itv_linexpr_array_struct* itv_linexpr_array_ptr;

/*
  Defined in itvConfig.h for avoiding multiple definitions

 - An interval linear expression is the more general form.
   - A quasilinear expression is such that the only non-scalar
     coefficient is the constant coefficient.
   - A linear expression contains no non-scalar coefficients

   Be cautious, order of labels matters in some functions
*/
/*
typedef enum itvlinexpr_type_t {
  ITV_LINEXPR_INTLINEAR,
  ITV_LINEXPR_QUASILINEAR,
  ITV_LINEXPR_LINEAR
} itvlinexpr_type_t;
*/
/* ********************************************************************** */
/* I. Constructor and Destructor */
/* ********************************************************************** */

static inline void itv_linterm_init(itv_linterm_t term);
static inline void itv_linterm_init_set(itv_linterm_t res, itv_linterm_t term);
static inline void itv_linterm_set(itv_linterm_t res, itv_linterm_t term);
static inline void itv_linterm_clear(itv_linterm_t term);
static inline void itv_linterm_swap(itv_linterm_t a, itv_linterm_t b);

void itv_linexpr_init(itv_linexpr_t expr, size_t size);
void itv_linexpr_init_set(itv_linexpr_t res, itv_linexpr_t expr);
void itv_linexpr_set(itv_linexpr_t res, itv_linexpr_t expr);
void itv_linexpr_resize(itv_linexpr_t expr, size_t size);
void itv_linexpr_minimize(itv_linexpr_t e);
void itv_linexpr_clear(itv_linexpr_t expr);
itv_linexpr_ptr itv_linexpr_alloc(size_t size);
itv_linexpr_ptr itv_linexpr_alloc_set(itv_linexpr_t expr);
void itv_linexpr_free(itv_linexpr_ptr expr);

void itv_linexpr_fprint(FILE* stream, itv_linexpr_t expr, char** name);
static inline void itv_linexpr_print(itv_linexpr_t expr, char** name);

void itv_linexpr_array_init(itv_linexpr_array_t array, size_t size);
void itv_linexpr_array_init_set(itv_linexpr_array_t res, itv_linexpr_array_t array);
void itv_linexpr_array_set(itv_linexpr_array_t res, itv_linexpr_array_t array);
void itv_linexpr_array_resize(itv_linexpr_array_t array, size_t size);
void itv_linexpr_array_minimize(itv_linexpr_array_t array);
void itv_linexpr_array_clear(itv_linexpr_array_t array);
itv_linexpr_array_ptr itv_linexpr_array_alloc(size_t size);
itv_linexpr_array_ptr itv_linexpr_array_alloc_set(itv_linexpr_array_t a);
void itv_linexpr_array_fprint(FILE* stream, itv_linexpr_array_t array, char** name);
static inline void itv_linexpr_array_print(itv_linexpr_array_t array, char** name);

/* ********************************************************************** */
/* II. Tests */
/* ********************************************************************** */

bool itv_linexpr_is_integer(itv_linexpr_t expr, size_t intdim);
  /* Does the expression depends only on integer variables ? assuming
     that the first intdim dimensions are integer */
bool itv_linexpr_is_real(itv_linexpr_t expr, size_t intdim);
  /* Does the expression depends only on real variables ? assuming
     that the first intdim dimensions are integer */

  /* Expression classification */

static inline bool itv_linexpr_is_linear(itv_linexpr_t expr);
  /* Return true iff all involved coefficients are scalars */
bool itv_linexpr_is_quasilinear(itv_linexpr_t expr);
  /* Return true iff all involved coefficients but the constant are scalars. */
itvlinexpr_type_t itv_linexpr_type(itv_linexpr_t a);
  /* Classify an expression */

bool itv_linexpr_array_is_linear(itv_linexpr_array_t array);
bool itv_linexpr_array_is_quasilinear(itv_linexpr_array_t array);
itvlinexpr_type_t itv_linexpr_array_type(itv_linexpr_array_t array);


/* ********************************************************************** */
/* III. Access */
/* ********************************************************************** */

static inline
size_t itv_linexpr_size(itv_linexpr_t expr);
  /* Get the size of the linear expression */

eitv_ptr itv_linexpr_eitvref(itv_linexpr_t expr, ap_dim_t dim, bool create);
  /* Get a (possibly NULL) reference to the coefficient associated to the
     dimension.
     If create==false, returns NULL if the corresponding dimension does not exist.
     If create==true, a new coefficient may be added.
 */

/* Set the coefficient of dimension dim in the expression */
static inline void itv_linexpr_set_eitv(itv_linexpr_t expr, ap_dim_t dim, eitv_t eitv);

/* Defined in itvConfig.h for avoiding multiple definitions */
/*
typedef enum itv_coefftag_t {
  ITV_COEFF,           waiting for 1 ap_coeff_t* and a dimension
  ITV_EITV,            waiting for 1 eitv_t and a dimension
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

bool itv_linexpr_set_list_generic(eitv_ptr (*get_eitv_of_dimvar)(void* env, void* expr, va_list* va),
				  void* env,   
				  numinternal_t intern,
				  void* expr, va_list* va);
eitv_ptr itv_linexpr_set_list_get_eitv_of_dim(void* env, void* expr, va_list* va);

bool itv_linexpr_set_list(numinternal_t intern, itv_linexpr_t expr, ...);
  /* This function assigns the linear expression from a list of tags of type
     itv_coefftag_t, each followed by a number of arguments as specified in
     the definition of the type ap_coefftag_t, and ended by the tag ITV_END;

     - The dimension AP_DIM_MAX is used to refer to the constat coefficient.
     - If the same dimension appears several times, only the last tag
       referring to it is taken into account.

     Returns true iff all conversions were exact.

     Example:
     itv_linexpr_set_list(intern,
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
   itv_linexpr_ForeachLinterm(itv_linexpr_t e, size_t i, ap_dim_t d, eitv_ptr eitv){
     ..
   }
   where
   - e is the inspected expression (of type itv_linexpr_t)
   - i is the internal iterator (of type size_t or int)
   - dim is the dimension of one linear term (of type ap_dim_t)
   - peitv is a pointer to the corresponding interval (of type eitv_ptr)

*/
#define itv_linexpr_ForeachLinterm(_p_e, _p_i, _p_d, _p_eitv) \
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

void itv_linexpr_neg(itv_linexpr_t res, itv_linexpr_t expr);
  /* Negate an expression */
void itv_linexpr_scale(itv_internal_t* intern,
		       itv_linexpr_t res, itv_linexpr_t expr, eitv_t coeff);
void itv_linexpr_div(itv_internal_t* intern,
		     itv_linexpr_t res, itv_linexpr_t expr, eitv_t coeff);
  /* Scale an expression by an interval */

void itv_linexpr_add(itv_internal_t* intern,
		     itv_linexpr_t expr,
		     itv_linexpr_t exprA,
		     itv_linexpr_t exprB);
void itv_linexpr_sub(itv_internal_t* intern,
		     itv_linexpr_t expr,
		     itv_linexpr_t exprA,
		     itv_linexpr_t exprB);
  /* Resp. add and substract two linear epxression.
     (Substraction temporarily negates exprB, and then restores it */

/* ********************************************************************** */
/* V. Evaluation and Quasilinearisation of interval linear expressions */
/* ********************************************************************** */

bool itv_linexpr_eval(itv_internal_t* intern,
		      itv_t res, itv_linexpr_t expr, itv_t* env);
  /* Evaluate an interval linear expression. Return true if no
     approximations. */

size_t itv_linexpr_supportinterval(itv_linexpr_t expr, ap_dim_t* tdim);
/* Fills the array tdim with the dimensions associated with intervals in the
   linear expression, in increasing order, and return the number of such
   dimensions.

   tdim is supposed to be of size at least the maximum dimension + 1 in the
   expression.
 */
size_t itv_linexpr_array_supportinterval(itv_linexpr_array_t array, ap_dim_t* tdim, size_t maxdim1);
  /* Idem for arrays.

     For the parameter maxdim1: corresponds to the maximal possible dimension
     + 1 */

/* These functions quasilinearize in-place expressions and constraints.  They
   optimize (sets of) constraints when the parameter meet is true, by
   deducing things. If constraints are quasilinearized for testing
   satisfaction, meet should be set to false.
*/
bool itv_linexpr_quasilinearize(itv_internal_t* intern,
				itv_linexpr_t linexpr,
				itv_t* env,
				bool for_meet_inequality);
  /* Quasilinearize in-place linexpr using the bounding box itv. Return true
     if no approximations. */

bool itv_linexpr_array_quasilinearize(itv_internal_t* intern,
				      itv_linexpr_array_t array,
				      itv_t* env);
  /* Same for an array */

/* ********************************************************************** */
/* VI. Change of dimensions and permutations */
/* ********************************************************************** */

/* These two functions add dimensions to the expressions, following the
   semantics of dimchange (see the type definition of dimchange).  */
void itv_linexpr_add_dimensions(itv_linexpr_t res,
			       itv_linexpr_t expr,
			       ap_dimchange_t* dimchange);
void itv_linexpr_array_add_dimensions(itv_linexpr_array_t res,
				      itv_linexpr_array_t array,
				      ap_dimchange_t* dimchange);

/* These two functions apply the given permutation to the dimensions.
   The dimensions present in the expression should just be less
   than the size of the permutation. */
void itv_linexpr_permute_dimensions(itv_linexpr_t res,
				    itv_linexpr_t expr,
				    ap_dimperm_t* perm);
void itv_linexpr_array_permute_dimensions(itv_linexpr_array_t res,
					  itv_linexpr_array_t array,
					  ap_dimperm_t* dimchange);

/* ********************************************************************** */
/* VII. Hashing, comparison */
/* ********************************************************************** */

int itv_linexpr_hash(itv_linexpr_t expr);
static inline bool itv_linexpr_equal(itv_linexpr_t expr1,itv_linexpr_t expr2);

/* Lexicographic ordering, terminating by constant coefficients */
int itv_linexpr_compare(itv_linexpr_t expr1, itv_linexpr_t expr2);

/* ********************************************************************** */
/* Definition of inline functions */
/* ********************************************************************** */

static inline void itv_linterm_init(itv_linterm_t term)
{ eitv_init(term->eitv); term->dim = AP_DIM_MAX; }
static inline void itv_linterm_init_set(itv_linterm_t res, itv_linterm_t term)
{ eitv_init_set(res->eitv,term->eitv); res->dim = term->dim; }
static inline void itv_linterm_set(itv_linterm_t res, itv_linterm_t term)
{ eitv_set(res->eitv,term->eitv); res->dim = term->dim; }
static inline void itv_linterm_clear(itv_linterm_t term)
{ eitv_clear(term->eitv); }
static inline void itv_linterm_swap(itv_linterm_t a, itv_linterm_t b)
{ if (a!=b){ itv_linterm_struct t=*a; *a=*b; *b=t; } }

static inline void itv_linexpr_print(itv_linexpr_t expr, char** name)
{ itv_linexpr_fprint(stdout,expr,name); }
static inline void itv_linexpr_array_print(itv_linexpr_array_t array, char** name)
{ itv_linexpr_array_fprint(stdout,array,name); }

static inline bool itv_linexpr_is_linear(itv_linexpr_t expr)
{ return eitv_is_point(expr->cst) && itv_linexpr_is_quasilinear(expr); }

static inline size_t itv_linexpr_size(itv_linexpr_t expr)
{
  size_t i,dim,size;
  eitv_ptr eitv;

  size = 0;
  itv_linexpr_ForeachLinterm(expr,i,dim,eitv){
    size++;
  }
  return size;
}

static inline void itv_linexpr_set_eitv(itv_linexpr_t expr, ap_dim_t dim, eitv_t eitv)
{
  eitv_ptr r = itv_linexpr_eitvref(expr,dim,true);
  eitv_set(r,eitv);
}
static inline bool itv_linexpr_equal(itv_linexpr_t expr1,itv_linexpr_t expr2)
{ return itv_linexpr_compare(expr1,expr2)==0; }

/* Internal function */
void itv_support_merge(ap_dim_t* ttdim[3], size_t tnb[3], size_t* pk);

#ifdef __cplusplus
}
#endif

#endif
