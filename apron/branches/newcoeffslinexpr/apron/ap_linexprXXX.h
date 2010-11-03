/* ********************************************************************** */
/* ap_linexprXXX.h */
/* ********************************************************************** */

#ifndef _AP_LINEXPRXXX_H_
#define _AP_LINEXPRXXX_H_

#include <stdarg.h>
#include "eitvXXX.h"
#include "ap_dimension.h"
#include "ap_coeff.h"
#include "ap_var.h"
#include "ap_environment.h"

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

static inline void ap_linexprXXX_get_cst(eitvXXX_t eitv, ap_linexprXXX_t expr);
static inline void ap_linexprXXX_get_eitv0(eitvXXX_t eitv, ap_linexprXXX_t expr, ap_dim_t dim);
static inline void ap_linexprXXX_get_eitv1(eitvXXX_t eitv, bool* perror, ap_linexprXXX_t expr, ap_environment_t* env, ap_var_t var);
  /* Get the coefficient of cst/dimension dim/variable var and assign it to
     eitv */

static inline void ap_linexprXXX_set_cst(ap_linexprXXX_t expr, eitvXXX_t eitv);
static inline void ap_linexprXXX_set_eitv0(ap_linexprXXX_t expr, ap_dim_t dim, eitvXXX_t eitv);
static inline void ap_linexprXXX_set_eitv1(ap_linexprXXX_t expr, bool* perror, ap_environment_t* env, ap_var_t var, eitvXXX_t eitv);
  /* Assign the coefficient of cst/dimension dim/variable var in the
     expression */

static inline eitvXXX_ptr ap_linexprXXX_cstref(ap_linexprXXX_t expr);
  /* Get a reference to the constant */
eitvXXX_ptr ap_linexprXXX_eitvref0(ap_linexprXXX_t expr, ap_dim_t dim, bool create);
static inline eitvXXX_ptr ap_linexprXXX_eitvref1(bool* perror, ap_linexprXXX_t expr, ap_environment_t* env, ap_var_t var, bool create);
  /* Get a (possibly NULL) reference to the coefficient associated to the
     dimension/variable.
     If create==false, returns NULL if the corresponding dimension does not exist.
     If create==true, a new coefficient may be added.
 */

bool ap_linexprXXX_set_list0(num_internal_t intern, ap_linexprXXX_t expr, bool* perror, ...);
  /* This function assigns the linear expression from a list of tags of type
     ap_coeff_tag_t, defined in ap_coeff.h, each followed by a number of
     arguments as specified in the definition of the type ap_coefftag_t, and
     ended by the tag AP_END;

     - The dimension AP_DIM_MAX is used to refer to the constat coefficient.
     - If the same dimension appears several times, only the last tag
       referring to it is taken into account.

     Returns true iff all conversions were exact.

     Example:
     ap_linexprXXX_set_list(intern,
			  expr,
			  AP_COEFF_LFRAC,7,9,0,
			  AP_COEFF_DOUBLE2,-3.0,4.5,1,
			  AP_COEFF_LLINT,3LL,AP_DIM_MAX,
			  AP_END)
     sets expr to "7/9 x0 + [-3,4.5] x1 + 3"
     assuming that the expression was "0" before the call and that all the
     number conversions were exact.
  */
bool ap_linexprXXX_set_list1(num_internal_t intern, ap_linexprXXX_t expr, bool* perror, ap_environment_t* env, ...);

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
       (_p_i)<(_p_e)->size ?						\
	 (((_p_d) = (_p_e)->linterm[_p_i]->dim),			\
	  ((_p_eitv) = (_p_e)->linterm[_p_i]->eitv),			\
	  ((_p_d)!=AP_DIM_MAX)) :					\
	 false;								\
       (_p_i)++)
#define ap_linexprXXX_ForeachLinterm1(_p_e, _p_env, _p_i, _p_v, _p_eitv) \
  for ((_p_i)=0;							\
       (_p_i)<(_p_e)->size ?						\
	 (((_p_v) = ap_environment_var_of_dim((_p_e)->linterm[_p_i]->dim)), \
	  ((_p_eitv) = (_p_e)->linterm[_p_i]->eitv),			\
	  ((_p_v)!=NULL)) :						\
	 false;								\
       (_p_i)++)

/* ********************************************************************** */
/* IV. Arithmetic */
/* ********************************************************************** */

void ap_linexprXXX_neg(ap_linexprXXX_t res, ap_linexprXXX_t expr);
  /* Negate an expression */
bool ap_linexprXXX_scale(ap_linexprXXX_t res, ap_linexprXXX_t expr, eitvXXX_t coeff, itv_internal_t intern);
bool ap_linexprXXX_div(ap_linexprXXX_t res, ap_linexprXXX_t expr, eitvXXX_t coeff, itv_internal_t intern);
  /* Scale an expression by an interval */

bool ap_linexprXXX_add(ap_linexprXXX_t expr,
		       ap_linexprXXX_t exprA,
		       ap_linexprXXX_t exprB, itv_internal_t intern);
bool ap_linexprXXX_sub(ap_linexprXXX_t expr,
		       ap_linexprXXX_t exprA,
		       ap_linexprXXX_t exprB, itv_internal_t intern);
  /* Resp. add and substract two linear epxression.
     (Substraction temporarily negates exprB, and then restores it */

/* ********************************************************************** */
/* Va. Evaluation and Quasilinearisation of interval linear expressions */
/* ********************************************************************** */
struct ap_linexpr0_struct;
bool eitvXXX_eval_ap_linexpr0(eitvXXX_t res, struct ap_linexpr0_struct* expr, eitvXXX_t* env, itv_internal_t intern);

bool ap_linexprXXX_eval(eitvXXX_t res, ap_linexprXXX_t expr, eitvXXX_t* env, itv_internal_t intern);
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
bool ap_linexprXXX_quasilinearize(ap_linexprXXX_t linexpr,
				  eitvXXX_t* env,
				  bool for_meet_inequality, itv_internal_t intern);
  /* Quasilinearize in-place linexpr using the bounding box itv. Return true
     if no approximations. */

bool ap_linexprXXX_array_quasilinearize(ap_linexprXXX_array_t array,
					eitvXXX_t* env, itv_internal_t intern);
  /* Same for an array */

/* ********************************************************************** */
/* Vb. Evaluation and interval linearisation of tree expressions */
/* ********************************************************************** */

struct ap_texpr0_t;
bool ap_linexprXXX_set_texpr0(ap_linexprXXX_t linexpr, bool* perror, struct ap_texpr0_t* expr, itv_internal_t intern);
bool ap_linexprXXX_array_set_texpr0_array(ap_linexprXXX_array_t linexpr, bool* perror, 
					  struct ap_texpr0_t** expr, size_t size, 
					  itv_internal_t intern);
  /* Linearize a tree expression that is (syntaxically) interval linear.
     If the precondition is violated, sets *perror to true.

     Return true if no approximations.
  */

bool eitvXXX_eval_ap_texpr0(eitvXXX_t res, struct ap_texpr0_t* texpr, eitvXXX_t* env, itv_internal_t intern);
void ap_linexprXXX_intlinearize_texpr0(ap_linexprXXX_t linexpr, struct ap_texpr0_t* expr, eitvXXX_t* env, size_t intdim, itv_internal_t intern);
void ap_linexprXXX_array_intlinearize_texpr0_array(ap_linexprXXX_array_t linexpr, struct ap_texpr0_t** expr, size_t size, eitvXXX_t* env, size_t intdim, itv_internal_t intern);
  /* Return true if no approximations. */

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
void ap_linexprXXX_extend_environment(ap_linexprXXX_t res,
				      bool* perror,
				      ap_environment_t* nenv,
				      ap_linexprXXX_t expr,
				      ap_environment_t* env);

/* These two functions apply the given permutation to the dimensions.
   The dimensions present in the expression should just be less
   than the size of the permutation. */
void ap_linexprXXX_permute_dimensions(ap_linexprXXX_t res,
				      ap_linexprXXX_t expr,
				      ap_dimperm_t* perm);
void ap_linexprXXX_array_permute_dimensions(ap_linexprXXX_array_t res,
					    ap_linexprXXX_array_t array,
					    ap_dimperm_t* dimchange);
void ap_linexprXXX_array_extend_environment(ap_linexprXXX_array_t res,
					    bool* perror,
					    ap_environment_t* nenv,
					    ap_linexprXXX_array_t expr,
					    ap_environment_t* env);

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
  ap_linexprXXX_ForeachLinterm0(expr,i,dim,eitv){
    size++;
  }
  return size;
}
static inline void ap_linexprXXX_get_cst(eitvXXX_t eitv, ap_linexprXXX_t expr)
{ eitvXXX_set(eitv,expr->cst); }
static inline void ap_linexprXXX_get_eitv0(eitvXXX_t eitv, ap_linexprXXX_t expr, ap_dim_t dim)
{
  eitvXXX_ptr r = ap_linexprXXX_eitvref0(expr,dim,false);
  if (r==NULL)
    eitvXXX_set_int(eitv,0);
  else
    eitvXXX_set(eitv,r);
}
static inline void ap_linexprXXX_get_eitv1(eitvXXX_t eitv, bool* perror, ap_linexprXXX_t expr, ap_environment_t* env, ap_var_t var)
{
  ap_dim_t dim = ap_environment_dim_of_var(env,var);
  *perror = (dim==AP_DIM_MAX);
  if (! *perror) ap_linexprXXX_get_eitv0(eitv,expr,dim);
}
static inline void ap_linexprXXX_set_cst(ap_linexprXXX_t expr, eitvXXX_t eitv)
  { eitvXXX_set(expr->cst,eitv); }
static inline void ap_linexprXXX_set_eitv0(ap_linexprXXX_t expr, ap_dim_t dim, eitvXXX_t eitv)
{
  eitvXXX_ptr r = ap_linexprXXX_eitvref0(expr,dim,true);
  eitvXXX_set(r,eitv);
}
static inline void ap_linexprXXX_set_eitv1(ap_linexprXXX_t expr, bool* perror, ap_environment_t* env, ap_var_t var, eitvXXX_t eitv)
{
  ap_dim_t dim = ap_environment_dim_of_var(env,var);
  *perror = (dim==AP_DIM_MAX);
  if (! *perror) ap_linexprXXX_set_eitv0(expr,dim,eitv);
}
static inline eitvXXX_ptr ap_linexprXXX_eitvref1(bool* perror, ap_linexprXXX_t expr, ap_environment_t* env, ap_var_t var, bool create)
{
  ap_dim_t dim = ap_environment_dim_of_var(env,var);
  *perror = (dim==AP_DIM_MAX);
  if (*perror)
    return NULL;
  else
    return ap_linexprXXX_eitvref0(expr,dim,create);
}
static inline eitvXXX_ptr ap_linexprXXX_cstref(ap_linexprXXX_t expr)
{ return expr->cst; }
static inline bool ap_linexprXXX_equal(ap_linexprXXX_t expr1,ap_linexprXXX_t expr2)
{ return ap_linexprXXX_compare(expr1,expr2)==0; }

/* Internal function */
void itvXXX_support_merge(ap_dim_t* ttdim[3], size_t tnb[3], size_t* pk);
eitvXXX_ptr ap_linexprXXX_set_list_get_eitvXXX_of_dim(ap_linexprXXX_t expr, ap_environment_t* env, bool cst, va_list* va);
eitvXXX_ptr ap_linexprXXX_set_list_get_eitvXXX_of_var(ap_linexprXXX_t expr, ap_environment_t* env, bool cst, va_list* va);
bool ap_linexprXXX_set_list_generic(eitvXXX_ptr (*get_eitvXXX_of_dimvar)(ap_linexprXXX_t expr, ap_environment_t* env, bool cst, va_list* va),
				    num_internal_t intern,
				    ap_linexprXXX_t expr, 
				    bool* perror, 
				    ap_environment_t* env,
				    va_list* va);

#ifdef __cplusplus
}
#endif

#endif
