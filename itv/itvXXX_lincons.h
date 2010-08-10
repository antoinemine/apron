/* ********************************************************************** */
/* itvXXX_lincons.h */
/* ********************************************************************** */

#ifndef _ITVXXX_LINCONS_H_
#define _ITVXXX_LINCONS_H_

#include "itvXXX_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* I. Constructor and Destructor */
/* ********************************************************************** */

static inline void itvXXX_lincons_init(itvXXX_lincons_t cons,size_t size);
static inline void itvXXX_lincons_init_set(itvXXX_lincons_t res, itvXXX_lincons_t cons);
static inline void itvXXX_lincons_init_set_linexpr(itvXXX_lincons_t res, itvXXX_linexpr_t expr, itvconstyp_t constyp, mpq_ptr mpq);
void itvXXX_lincons_set_bool(itvXXX_lincons_t res, bool value);
static inline void itvXXX_lincons_resize(itvXXX_lincons_t expr, size_t size);
static inline void itvXXX_lincons_minimize(itvXXX_lincons_t e);
static inline void itvXXX_lincons_clear(itvXXX_lincons_t cons);

itvXXX_lincons_ptr itvXXX_lincons_alloc(size_t size);
itvXXX_lincons_ptr itvXXX_lincons_alloc_set(itvXXX_lincons_t cons);
itvXXX_lincons_ptr itvXXX_lincons_alloc_set_linexpr(itvXXX_linexpr_t expr, itvconstyp_t constyp, mpq_ptr mpq);
void itvXXX_lincons_free(itvXXX_lincons_ptr cons);

void itvXXX_lincons_fprint(FILE* stream, itvXXX_lincons_t cons, char** name);
static inline void itvXXX_lincons_print(itvXXX_lincons_t cons, char** name);
static inline void itvXXX_lincons_swap(itvXXX_lincons_t a, itvXXX_lincons_t b);

void itvXXX_lincons_array_init(itvXXX_lincons_array_t array, size_t size);
void itvXXX_lincons_array_init_set(itvXXX_lincons_array_t res, itvXXX_lincons_array_t array);
void itvXXX_lincons_array_set(itvXXX_lincons_array_t res, itvXXX_lincons_array_t array);
void itvXXX_lincons_array_resize(itvXXX_lincons_array_t array, size_t size);
void itvXXX_lincons_array_minimize(itvXXX_lincons_array_t array);
void itvXXX_lincons_array_clear(itvXXX_lincons_array_t array);
itvXXX_lincons_array_ptr itvXXX_lincons_array_alloc(size_t size);
itvXXX_lincons_array_ptr itvXXX_lincons_array_alloc_set(itvXXX_lincons_array_t a);
void itvXXX_lincons_array_fprint(FILE* stream, itvXXX_lincons_array_t array, char** name);
static inline void itvXXX_lincons_array_print(itvXXX_lincons_array_t array, char** name);

/* ********************************************************************** */
/* II. Tests and Simplifications */
/* ********************************************************************** */

static inline bool itvXXX_lincons_is_integer(itvXXX_lincons_t cons, size_t intdim);
static inline bool itvXXX_lincons_is_linear(itvXXX_lincons_t cons);
  /* Return true iff all involved coefficients are scalars */
static inline bool itvXXX_lincons_is_quasilinear(itvXXX_lincons_t cons);
  /* Return true iff all involved coefficients but the constant are scalars. */
static inline itvlinexpr_type_t itvXXX_lincons_type(itvXXX_lincons_t cons);
  /* Classify a constraint */

bool itvXXX_lincons_array_is_linear(itvXXX_lincons_array_t array);
bool itvXXX_lincons_array_is_quasilinear(itvXXX_lincons_array_t array);
itvlinexpr_type_t itvXXX_lincons_array_type(itvXXX_lincons_array_t array);

/* ********************************************************************** */
/* III. Evaluation, simplification and linearisation */
/* ********************************************************************** */

tbool_t itvXXX_lincons_evalcst(itvXXX_internal_t* intern,
			    itvXXX_lincons_t lincons);
  /* Is the assumed constant constraint satisfied ? */

bool itvXXX_lincons_sat_is_false(itvXXX_internal_t* intern,
			      itvXXX_lincons_t lincons);
  /* Return true if the constraint is not satisfiable
     *for all* deterministic expressions contained in it
     Cases taken into account:
     - expr + [a,A] = 0 (mod k) with a!=A
     - expr + [-oo,+oo] != 0
     - expr + (-oo,x] >|>= 0
     - the cases where itvXXX_eval_cstlincons returns tbool_false
  */

void itvXXX_lincons_reduce_integer(itvXXX_internal_t* intern,
				itvXXX_lincons_t cons, size_t intdim);
  /* Transform a constraint involving only integer variables and only scalar
     coefficients (with the exception of the constant) as follows:
     - divide expr by the pgcd of non-constant coefficients
       details differ according to type of num (integer, rational,
       floating-point)

     - expr + [min,sup] >= 0  ==>  expr + floor(sup) >= 0
     - expr + [min,sup] > 0   ==>  expr + sup - 1 >=      if sup is integer
				   expr + floor(sup) >= 0 otherwise
     - expr + [min,sup] = 0   ==>  expr + [ceil(min),floor(sup)] = 0
  */

tbool_t itvXXX_lincons_array_reduce(itvXXX_internal_t* intern,
				 itvXXX_lincons_array_t array, bool meet);
  /* Simplify the array as follows:
     - remove trivially true constraints (like 1>=0)
     - if a constraint is trivially false, reduce the array to the constraint
       1=0
     Return
     - tbool_true if empty array
     - tbool_false if trivially false
     - tbool_top otherwise
  */
tbool_t itvXXX_lincons_array_reduce_integer(itvXXX_internal_t* intern,
					 itvXXX_lincons_array_t array,
					 size_t intdim);
  /* Apply first itvXXX_lincons_reduce_integer, and then
     itvXXX_lincons_array_reduce(.,.,true) */

static inline
size_t itvXXX_lincons_supportinterval(itvXXX_lincons_t cons, ap_dim_t* tdim);
  /* Fills the array tdim with the dimensions associated with intervals in
     the constraint, in increasing order, and return the number of such
     dimensions.

     tdim is supposed to be of size at least the maximum dimension + 1 in the
     expression.
 */
size_t itvXXX_lincons_array_supportinterval(itvXXX_lincons_array_t array, ap_dim_t* tdim, size_t maxdim1);
  /* Idem, with maxdim1 indicating the maximum possible dimension plus 1 */

/* These functions quasilinearize in-place expressions and constraints.  They
   optimize (sets of) constraints when the parameter meet is true, by
   deducing things. If constraints are quasilinearized for testing
   satisfaction, meet should be set to false.
*/
static inline
bool itvXXX_lincons_quasilinearize(itvXXX_internal_t* intern,
				itvXXX_lincons_t lincons,
				itvXXX_t* env,
				bool for_meet_inequality);
  /* Quasilinearize in-place lincons using the bounding box itv. Return true
     if no approximations. */

bool itvXXX_lincons_array_quasilinearize(itvXXX_internal_t* intern,
				      itvXXX_lincons_array_t array,
				      itvXXX_t* env,
				      bool for_meet_inequality);
  /* Same for an array */


/* ********************************************************************** */
/* IV. Boxization of interval linear expressions */
/* ********************************************************************** */

bool itvXXX_lincons_array_boxize(itvXXX_internal_t* intern,
			      itvXXX_t* res,
			      bool* tchange,
			      itvXXX_lincons_array_t array,
			      itvXXX_t* env,size_t intdim,
			      size_t kmax,
			      bool intervalonly);
  /* Deduce interval constraints from a set of interval linear constraints.

     Return true if some bounds have been inferred.

     - The inferred bounds are stored in res (which may be equal to env)
     - If tchange!=NULL, tchange[2dim] (resp. 2dim+1) set to true indicates
       that the inf (resp. sup) bound of dimension dim has been improved.
     - env is the current bounds for variables
     - kmax specifies the maximum number of iterations
     - if intervalonly is true, deduces bounds from a constraint only when the
x       coefficient associated to the current dimension is an interval.
  */

/* ********************************************************************** */
/* V. Change of dimensions and permutations */
/* ********************************************************************** */

/* These two functions add dimensions to the expressions, following the
   semantics of dimchange (see the type definition of dimchange).  */
static inline
void itvXXX_lincons_add_dimensions(itvXXX_lincons_t res,
				itvXXX_lincons_t cons,
				ap_dimchange_t* dimchange);
void itvXXX_lincons_array_add_dimensions(itvXXX_lincons_array_t res,
				      itvXXX_lincons_array_t array,
				      ap_dimchange_t* dimchange);

/* These two functions apply the given permutation to the dimensions.
   The dimensions present in the consession should just be less
   than the size of the permutation. */
static inline
void itvXXX_lincons_permute_dimensions(itvXXX_lincons_t res,
				    itvXXX_lincons_t cons,
				    ap_dimperm_t* perm);
void itvXXX_lincons_array_permute_dimensions(itvXXX_lincons_array_t res,
					  itvXXX_lincons_array_t array,
					  ap_dimperm_t* dimchange);

/* ********************************************************************** */
/* VI. Hashing, comparison */
/* ********************************************************************** */

static inline int itvXXX_lincons_hash(itvXXX_lincons_t cons);
bool itvXXX_lincons_equal(itvXXX_lincons_t cons1,itvXXX_lincons_t cons2);

/* Lexicographic ordering, terminating by constant coefficients */
int itvXXX_lincons_compare(itvXXX_lincons_t cons1, itvXXX_lincons_t cons2);

/* ********************************************************************** */
/* Definition of inline functions */
/* ********************************************************************** */

static inline void itvXXX_lincons_init(itvXXX_lincons_t cons,size_t size)
{ itvXXX_linexpr_init(cons->linexpr,size); cons->constyp = ITVXXX_CONS_EQ; mpq_init(cons->mpq); }
static inline void itvXXX_lincons_set(itvXXX_lincons_t a, itvXXX_lincons_t b)
{ if (a!=b){ itvXXX_linexpr_set(a->linexpr,b->linexpr); mpq_set(a->mpq,b->mpq); a->constyp = b->constyp; } }
static inline void itvXXX_lincons_init_set(itvXXX_lincons_t a, itvXXX_lincons_t b)
{ assert(a!=b); itvXXX_linexpr_init_set(a->linexpr,b->linexpr); a->constyp = b->constyp; mpq_init(a->mpq); mpq_set(a->mpq,b->mpq); }
static inline void itvXXX_lincons_init_set_linexpr(itvXXX_lincons_t res, itvXXX_linexpr_t expr, itvconstyp_t constyp, mpq_ptr mpq)
{ 
  itvXXX_linexpr_init_set(res->linexpr,expr); res->constyp = constyp; 
  mpq_init(res->mpq);
  if (mpq) mpq_set(res->mpq,mpq);
}
static inline void itvXXX_lincons_resize(itvXXX_lincons_t e, size_t size)
{ itvXXX_linexpr_resize(e->linexpr,size); }
static inline void itvXXX_lincons_minimize(itvXXX_lincons_t e)
{ itvXXX_linexpr_minimize(e->linexpr); }
static inline void itvXXX_lincons_clear(itvXXX_lincons_t cons)
{ itvXXX_linexpr_clear(cons->linexpr); mpq_clear(cons->mpq); }
static inline void itvXXX_lincons_print(itvXXX_lincons_t cons, char** name)
{ itvXXX_lincons_fprint(stdout,cons,name); }
static inline void itvXXX_lincons_swap(itvXXX_lincons_t a, itvXXX_lincons_t b)
{ if (a!=b){ itvXXX_lincons_struct t=*a; *a=*b; *b=t; } }
static inline void itvXXX_lincons_array_print(itvXXX_lincons_array_t array, char** name)
{ itvXXX_lincons_array_fprint(stdout,array,name); }

static inline bool itvXXX_lincons_is_integer(itvXXX_lincons_t cons, size_t intdim)
{
  return itvXXX_linexpr_is_integer(cons->linexpr,intdim);
}
static inline bool itvXXX_lincons_is_linear(itvXXX_lincons_t cons)
{
  return itvXXX_linexpr_is_linear(cons->linexpr);
}
static inline bool itvXXX_lincons_is_quasilinear(itvXXX_lincons_t cons)
{
  return itvXXX_linexpr_is_quasilinear(cons->linexpr);
}
static inline itvlinexpr_type_t itvXXX_lincons_type(itvXXX_lincons_t cons)
{
  return itvXXX_linexpr_type(cons->linexpr);
}
static inline size_t itvXXX_lincons_supportinterval(itvXXX_lincons_t cons, ap_dim_t* tdim)
{
  return itvXXX_linexpr_supportinterval(cons->linexpr,tdim);
}
static inline
bool itvXXX_lincons_quasilinearize(itvXXX_internal_t* intern,
				itvXXX_lincons_t lincons,
				itvXXX_t* env,
				bool for_meet_inequality)
{
  return itvXXX_linexpr_quasilinearize(intern,lincons->linexpr,env,for_meet_inequality);
}
static inline
void itvXXX_lincons_add_dimensions(itvXXX_lincons_t res,
				itvXXX_lincons_t cons,
				ap_dimchange_t* dimchange)
{ itvXXX_linexpr_add_dimensions(res->linexpr,cons->linexpr,dimchange); }

static inline
void itvXXX_lincons_permute_dimensions(itvXXX_lincons_t res,
				    itvXXX_lincons_t cons,
				    ap_dimperm_t* dimperm)
{ itvXXX_linexpr_permute_dimensions(res->linexpr,cons->linexpr,dimperm); }

static inline
int itvXXX_lincons_hash(itvXXX_lincons_t cons)
{
  return (int)cons->constyp + itvXXX_linexpr_hash(cons->linexpr);
}

#ifdef __cplusplus
}
#endif

#endif
