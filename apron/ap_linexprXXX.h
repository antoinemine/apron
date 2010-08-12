/* ********************************************************************** */
/* ap_linexprXXX.h */
/* ********************************************************************** */

#ifndef _AP_LINEXPRXXX_H_
#define _AP_LINEXPRXXX_H_

#include <stdarg.h>
#include "eitvXXX.h"
#include "ap_dimension.h"
#include "ap_coeff.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ap_linexpr_type_t_
#define  _ap_linexpr_type_t_
typedef enum ap_linexpr_type_t {
  AP_LINEXPR_INTLINEAR,
  AP_LINEXPR_QUASILINEAR,
  AP_LINEXPR_LINEAR
} ap_linexpr_type_t;
#endif

/* Interval linear term */
typedef struct ap_lintermXXX_struct {
  eitvXXX_t eitv;
  ap_dim_t dim;
} ap_lintermXXX_struct;
typedef ap_lintermXXX_struct* ap_lintermXXX_ptr;
typedef ap_lintermXXX_struct ap_lintermXXX_t[1];

/* Interval linear expression */
typedef struct ap_linexprXXX_struct {
  ap_lintermXXX_t* linterm;
  size_t size;
  eitvXXX_t cst;
} ap_linexprXXX_struct;
typedef ap_linexprXXX_struct ap_linexprXXX_t[1];
typedef ap_linexprXXX_struct* ap_linexprXXX_ptr;

/* Array of interval linear expressions */
typedef struct ap_linexprXXX_array_struct {
  ap_linexprXXX_t* p;
  size_t size;
} ap_linexprXXX_array_struct;
typedef ap_linexprXXX_array_struct ap_linexprXXX_array_t[1];
typedef ap_linexprXXX_array_struct* ap_linexprXXX_array_ptr;

/* ********************************************************************** */
/* I. Constructor and Destructor */
/* ********************************************************************** */

void ap_lintermXXX_init(ap_lintermXXX_t term);
void ap_lintermXXX_init_set(ap_lintermXXX_t res, ap_lintermXXX_t term);
void ap_lintermXXX_set(ap_lintermXXX_t res, ap_lintermXXX_t term);
void ap_lintermXXX_clear(ap_lintermXXX_t term);
void ap_lintermXXX_swap(ap_lintermXXX_t a, ap_lintermXXX_t b);

void ap_linexprXXX_init(ap_linexprXXX_t expr, size_t size);
void ap_linexprXXX_init_set(ap_linexprXXX_t res, ap_linexprXXX_t expr);
void ap_linexprXXX_set(ap_linexprXXX_t res, ap_linexprXXX_t expr);
void ap_linexprXXX_resize(ap_linexprXXX_t expr, size_t size);
void ap_linexprXXX_minimize(ap_linexprXXX_t e);
void ap_linexprXXX_clear(ap_linexprXXX_t expr);

ap_linexprXXX_ptr ap_linexprXXX_alloc(size_t size);
ap_linexprXXX_ptr ap_linexprXXX_alloc_set(ap_linexprXXX_t expr);
void ap_linexprXXX_free(ap_linexprXXX_ptr expr);

void ap_linexprXXX_fprint(FILE* stream, ap_linexprXXX_t expr, char** name);
static inline void ap_linexprXXX_print(ap_linexprXXX_t expr, char** name);

void ap_linexprXXX_array_init(ap_linexprXXX_array_t array, size_t size);
void ap_linexprXXX_array_init_set(ap_linexprXXX_array_t res, ap_linexprXXX_array_t array);
void ap_linexprXXX_array_set(ap_linexprXXX_array_t res, ap_linexprXXX_array_t array);
void ap_linexprXXX_array_resize(ap_linexprXXX_array_t array, size_t size);
void ap_linexprXXX_array_minimize(ap_linexprXXX_array_t array);
void ap_linexprXXX_array_clear(ap_linexprXXX_array_t array);

ap_linexprXXX_array_ptr ap_linexprXXX_array_alloc(size_t size);
ap_linexprXXX_array_ptr ap_linexprXXX_array_alloc_set(ap_linexprXXX_array_t a);
  void ap_linexprXXX_array_free(ap_linexprXXX_array_ptr expr);

void ap_linexprXXX_array_fprint(FILE* stream, ap_linexprXXX_array_t array, char** name);
static inline void ap_linexprXXX_array_print(ap_linexprXXX_array_t array, char** name);

/* ********************************************************************** */
/* II. Tests */
/* ********************************************************************** */

bool ap_linexprXXX_is_integer(ap_linexprXXX_t expr, size_t intdim);
  /* Does the expression depends only on integer variables ? assuming
     that the first intdim dimensions are integer */
bool ap_linexprXXX_is_real(ap_linexprXXX_t expr, size_t intdim);
  /* Does the expression depends only on real variables ? assuming
     that the first intdim dimensions are integer */

  /* Expression classification */

bool ap_linexprXXX_is_linear(ap_linexprXXX_t expr);
  /* Return true iff all involved coefficients are scalars */
bool ap_linexprXXX_is_quasilinear(ap_linexprXXX_t expr);
  /* Return true iff all involved coefficients but the constant are scalars. */
ap_linexpr_type_t ap_linexprXXX_type(ap_linexprXXX_t a);
  /* Classify an expression */

bool ap_linexprXXX_array_is_linear(ap_linexprXXX_array_t array);
bool ap_linexprXXX_array_is_quasilinear(ap_linexprXXX_array_t array);
ap_linexpr_type_t ap_linexprXXX_array_type(ap_linexprXXX_array_t array);


/* ********************************************************************** */
/* III. Access */
/* ********************************************************************** */

static inline
size_t ap_linexprXXX_size(ap_linexprXXX_t expr);
  /* Get the size of the linear expression */

eitvXXX_ptr ap_linexprXXX_eitvref(ap_linexprXXX_t expr, ap_dim_t dim, bool create);
  /* Get a (possibly NULL) reference to the coefficient associated to the
     dimension.
     If create==false, returns NULL if the corresponding dimension does not exist.
     If create==true, a new coefficient may be added.
 */

/* Set the coefficient of dimension dim in the expression */
static inline void ap_linexprXXX_set_eitv(ap_linexprXXX_t expr, ap_dim_t dim, eitvXXX_t eitv);

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

bool ap_linexprXXX_set_list_generic(eitvXXX_ptr (*get_eitvXXX_of_dimvar)(void* env, void* expr, va_list* va),
				     void* env,
				     num_internal_t intern,
				     void* expr, va_list* va);
eitvXXX_ptr ap_linexprXXX_set_list_get_eitvXXX_of_dim(void* env, void* expr, va_list* va);

bool ap_linexprXXX_set_list(num_internal_t intern, ap_linexprXXX_t expr, ...);
  /* This function assigns the linear expression from a list of tags of type
     itv_coefftag_t, each followed by a number of arguments as specified in
     the definition of the type ap_coefftag_t, and ended by the tag ITV_END;

     - The dimension AP_DIM_MAX is used to refer to the constat coefficient.
     - If the same dimension appears several times, only the last tag
       referring to it is taken into account.

     Returns true iff all conversions were exact.

     Example:
     ap_linexprXXX_set_list(intern,
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
   ap_linexprXXX_ForeachLinterm(ap_linexprXXX_t e, size_t i, ap_dim_t d, eitvXXX_ptr eitv){
     ..
   }
   where
   - e is the inspected expression (of type ap_linexprXXX_t)
   - i is the internal iterator (of type size_t or int)
   - dim is the dimension of one linear term (of type ap_dim_t)
   - peitv is a pointer to the corresponding interval (of type eitvXXX_ptr)

*/
#define ap_linexprXXX_ForeachLinterm(_p_e, _p_i, _p_d, _p_eitv) \
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

void ap_linexprXXX_neg(ap_linexprXXX_t res, ap_linexprXXX_t expr);
  /* Negate an expression */
void ap_linexprXXX_scale(itvXXX_internal_t intern,
			  ap_linexprXXX_t res, ap_linexprXXX_t expr, eitvXXX_t coeff);
void ap_linexprXXX_div(itvXXX_internal_t intern,
			ap_linexprXXX_t res, ap_linexprXXX_t expr, eitvXXX_t coeff);
  /* Scale an expression by an interval */

void ap_linexprXXX_add(itvXXX_internal_t intern,
			ap_linexprXXX_t expr,
			ap_linexprXXX_t exprA,
			ap_linexprXXX_t exprB);
void ap_linexprXXX_sub(itvXXX_internal_t intern,
			ap_linexprXXX_t expr,
			ap_linexprXXX_t exprA,
			ap_linexprXXX_t exprB);
  /* Resp. add and substract two linear epxression.
     (Substraction temporarily negates exprB, and then restores it */

/* ********************************************************************** */
/* V. Evaluation and Quasilinearisation of interval linear expressions */
/* ********************************************************************** */

bool ap_linexprXXX_eval(itvXXX_internal_t intern,
			 itvXXX_t res, ap_linexprXXX_t expr, itvXXX_t* env);
  /* Evaluate an interval linear expression. Return true if no
     approximations. */

size_t ap_linexprXXX_supportinterval(ap_linexprXXX_t expr, ap_dim_t* tdim);
/* Fills the array tdim with the dimensions associated with intervals in the
   linear expression, in increasing order, and return the number of such
   dimensions.

   tdim is supposed to be of size at least the maximum dimension + 1 in the
   expression.
 */
size_t ap_linexprXXX_array_supportinterval(ap_linexprXXX_array_t array, ap_dim_t* tdim, size_t maxdim1);
  /* Idem for arrays.

     For the parameter maxdim1: corresponds to the maximal possible dimension
     + 1 */

/* These functions quasilinearize in-place expressions and constraints.  They
   optimize (sets of) constraints when the parameter meet is true, by
   deducing things. If constraints are quasilinearized for testing
   satisfaction, meet should be set to false.
*/
bool ap_linexprXXX_quasilinearize(itvXXX_internal_t intern,
				   ap_linexprXXX_t linexpr,
				   itvXXX_t* env,
				   bool for_meet_inequality);
  /* Quasilinearize in-place linexpr using the bounding box itv. Return true
     if no approximations. */

bool ap_linexprXXX_array_quasilinearize(itvXXX_internal_t intern,
					 ap_linexprXXX_array_t array,
					 itvXXX_t* env);
  /* Same for an array */

/* ********************************************************************** */
/* VI. Change of dimensions and permutations */
/* ********************************************************************** */

/* These two functions add dimensions to the expressions, following the
   semantics of dimchange (see the type definition of dimchange).  */
void ap_linexprXXX_add_dimensions(ap_linexprXXX_t res,
				   ap_linexprXXX_t expr,
				   ap_dimchange_t* dimchange);
void ap_linexprXXX_array_add_dimensions(ap_linexprXXX_array_t res,
					 ap_linexprXXX_array_t array,
					 ap_dimchange_t* dimchange);

/* These two functions apply the given permutation to the dimensions.
   The dimensions present in the expression should just be less
   than the size of the permutation. */
void ap_linexprXXX_permute_dimensions(ap_linexprXXX_t res,
				       ap_linexprXXX_t expr,
				       ap_dimperm_t* perm);
void ap_linexprXXX_array_permute_dimensions(ap_linexprXXX_array_t res,
					     ap_linexprXXX_array_t array,
					     ap_dimperm_t* dimchange);

/* ********************************************************************** */
/* VII. Hashing, comparison */
/* ********************************************************************** */

int ap_linexprXXX_hash(ap_linexprXXX_t expr);
static inline bool ap_linexprXXX_equal(ap_linexprXXX_t expr1,ap_linexprXXX_t expr2);

/* Lexicographic ordering, terminating by constant coefficients */
int ap_linexprXXX_compare(ap_linexprXXX_t expr1, ap_linexprXXX_t expr2);

/* ********************************************************************** */
/* Definition of inline functions */
/* ********************************************************************** */

static inline void ap_linexprXXX_print(ap_linexprXXX_t expr, char** name)
{ ap_linexprXXX_fprint(stdout,expr,name); }
static inline void ap_linexprXXX_array_print(ap_linexprXXX_array_t array, char** name)
{ ap_linexprXXX_array_fprint(stdout,array,name); }
static inline size_t ap_linexprXXX_size(ap_linexprXXX_t expr)
{
  size_t i,dim,size;
  eitvXXX_ptr eitv;

  size = 0;
  ap_linexprXXX_ForeachLinterm(expr,i,dim,eitv){
    size++;
  }
  return size;
}

static inline void ap_linexprXXX_set_eitv(ap_linexprXXX_t expr, ap_dim_t dim, eitvXXX_t eitv)
{
  eitvXXX_ptr r = ap_linexprXXX_eitvref(expr,dim,true);
  eitvXXX_set(r,eitv);
}
static inline bool ap_linexprXXX_equal(ap_linexprXXX_t expr1,ap_linexprXXX_t expr2)
{ return ap_linexprXXX_compare(expr1,expr2)==0; }

/* Internal function */
void itvXXX_support_merge(ap_dim_t* ttdim[3], size_t tnb[3], size_t* pk);

#ifdef __cplusplus
}
#endif

#endif
