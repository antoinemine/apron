/* ********************************************************************** */
/* itv_lincons.h */
/* ********************************************************************** */

#ifndef _ITV_LINCONS_H_
#define _ITV_LINCONS_H_

#include "eitv.h"
#include "itv_linexpr.h"
#include "itv_lincons.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* Dataypes */
/* ********************************************************************** */

/* Datatype for type of constraints */
typedef enum itv_constyp_t {
  ITV_CONS_EQ,    /* equality constraint */
  ITV_CONS_SUPEQ, /* >= constraint */
  ITV_CONS_SUP,   /* > constraint */
  ITV_CONS_EQMOD, /* congruence equality constraint */
  ITV_CONS_DISEQ  /* disequality constraint */
} itv_constyp_t;

/* Interval linear constraint */
typedef struct itv_lincons_struct {
  itv_linexpr_t linexpr;
  itv_constyp_t constyp;
  num_t num;
} itv_lincons_struct;
typedef itv_lincons_struct itv_lincons_t[1];
typedef itv_lincons_struct* itv_lincons_ptr;

/* Array of interval linear constraint */
typedef struct itv_lincons_array_struct {
  itv_lincons_t* p;
  size_t size;
} itv_lincons_array_struct;
typedef itv_lincons_array_struct itv_lincons_array_t[1];
typedef itv_lincons_array_struct* itv_lincons_array_ptr;

/* ********************************************************************** */
/* I. Constructor and Destructor */
/* ********************************************************************** */

static inline void itv_lincons_init(itv_lincons_t cons);
static inline void itv_lincons_set(itv_lincons_t res, itv_lincons_t expr);
void itv_lincons_set_bool(itv_lincons_t res, bool value);
static inline void itv_lincons_clear(itv_lincons_t cons);
void itv_lincons_fprint(FILE* stream, itv_lincons_t cons, char** name);
static inline void itv_lincons_print(itv_lincons_t cons, char** name);
static inline void itv_lincons_swap(itv_lincons_t a, itv_lincons_t b);

void itv_lincons_array_init(itv_lincons_array_t array, size_t size);
void itv_lincons_array_reinit(itv_lincons_array_t array, size_t size);
void itv_lincons_array_clear(itv_lincons_array_t array);
void itv_lincons_array_fprint(FILE* stream, itv_lincons_array_t array, char** name);
static inline void itv_lincons_array_print(itv_lincons_array_t array, char** name);

/* ********************************************************************** */
/* II. Tests and Simplifications */
/* ********************************************************************** */

static inline bool itv_lincons_is_integer(itv_lincons_t cons, size_t intdim);

static inline bool itv_lincons_is_linear(itv_lincons_t cons);
  /* Return true iff all involved coefficients are scalars */
static inline bool itv_lincons_is_quasilinear(itv_lincons_t cons);
  /* Return true iff all involved coefficients but the constant are scalars. */
static inline itvlinexpr_type_t itv_lincons_type(itv_lincons_t cons);
  /* Classify a constraint */

bool itv_lincons_array_is_linear(itv_lincons_array_t array);
bool itv_lincons_array_is_quasilinear(itv_lincons_array_t array);
itvlinexpr_type_t itv_lincons_array_type(itv_lincons_array_t array);

/* ********************************************************************** */
/* III. Evaluation, simplification and linearisation */
/* ********************************************************************** */

tbool_t itv_lincons_evalcst(itv_internal_t* intern,
			    itv_lincons_t lincons);
  /* Is the assumed constant constraint satisfied ? */

bool itv_lincons_sat_is_false(itv_internal_t* intern,
			      itv_lincons_t lincons);
  /* Return true if the constraint is not satisfiable
     *for all* deterministic expressions contained in it
     Cases taken into account:
     - expr + [a,A] = 0 (mod k) with a!=A
     - expr + [-oo,+oo] != 0
     - expr + (-oo,x] >|>= 0
     - the cases where itv_eval_cstlincons returns tbool_false
  */

void itv_lincons_reduce_integer(itv_internal_t* intern,
				itv_lincons_t cons, size_t intdim);
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

tbool_t itv_lincons_array_reduce(itv_internal_t* intern,
				 itv_lincons_array_t array, bool meet);
  /* Simplify the array as follows:
     - remove trivially true constraints (like 1>=0)
     - if a constraint is trivially false, reduce the array to the constraint
       1=0
     Return
     - tbool_true if empty array
     - tbool_false if trivially false
     - tbool_top otherwise
  */
tbool_t itv_lincons_array_reduce_integer(itv_internal_t* intern,
					 itv_lincons_array_t array,
					 size_t intdim);
  /* Apply first itv_lincons_reduce_integer, and then
     itv_lincons_array_reduce(.,.,true) */

static inline
size_t itv_lincons_supportinterval(itv_lincons_t cons, ap_dim_t* tdim);
  /* Fills the array tdim with the dimensions associated with intervals in
     the constraint, in increasing order, and return the number of such
     dimensions.

     tdim is supposed to be of size at least the maximum dimension + 1 in the
     expression.
 */
size_t itv_lincons_array_supportinterval(itv_lincons_array_t array, ap_dim_t* tdim, size_t maxdim1);
  /* Idem, with maxdim1 indicating the maximum possible dimension plus 1 */

/* These functions quasilinearize in-place expressions and constraints.  They
   optimize (sets of) constraints when the parameter meet is true, by
   deducing things. If constraints are quasilinearized for testing
   satisfaction, meet should be set to false.
*/
static inline
bool itv_lincons_quasilinearize(itv_internal_t* intern,
				itv_lincons_t lincons,
				itv_t* env,
				bool for_meet_inequality);
  /* Quasilinearize in-place lincons using the bounding box itv. Return true
     if no approximations. */

bool itv_lincons_array_quasilinearize(itv_internal_t* intern,
				      itv_lincons_array_t array,
				      itv_t* env,
				      bool for_meet_inequality);
  /* Same for an array */


/* ********************************************************************** */
/* IV. Boxization of interval linear expressions */
/* ********************************************************************** */

bool itv_boxize_lincons_array(itv_internal_t* intern,
			      itv_t* res,
			      bool* tchange,
			      itv_lincons_array_t array,
			      itv_t* env,size_t intdim,
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
       coefficient associated to the current dimension is an interval.
  */

/* ********************************************************************** */
/* V. Change of dimensions and permutations */
/* ********************************************************************** */

/* These two functions add dimensions to the expressions, following the
   semantics of dimchange (see the type definition of dimchange).  */
static inline
void itv_lincons_add_dimension(itv_lincons_t res,
			       itv_lincons_t cons,
			       ap_dimchange_t* dimchange);
void itv_lincons_array_add_dimension(itv_lincons_array_t res,
				     itv_lincons_array_t array,
				     ap_dimchange_t* dimchange);

/* These two functions apply the given permutation to the dimensions.
   The dimensions present in the consession should just be less
   than the size of the permutation. */
static inline
void itv_lincons_permute_dimensions(itv_lincons_t res,
				    itv_lincons_t cons,
				    ap_dimperm_t* perm);
void itv_lincons_array_permute_dimensions(itv_lincons_array_t res,
					  itv_lincons_array_t array,
					  ap_dimperm_t* dimchange);

/* ********************************************************************** */
/* VI. Hashing, comparison */
/* ********************************************************************** */

static inline int itv_lincons_hash(itv_lincons_t cons);
bool itv_lincons_equal(itv_lincons_t cons1,itv_lincons_t cons2);

/* Lexicographic ordering, terminating by constant coefficients */
int itv_lincons_compare(itv_lincons_t cons1, itv_lincons_t cons2);

/* ********************************************************************** */
/* Definition of inline functions */
/* ********************************************************************** */

static inline void itv_lincons_init(itv_lincons_t cons)
{ itv_linexpr_init(cons->linexpr,0); num_init(cons->num); }
static inline void itv_lincons_set(itv_lincons_t a, itv_lincons_t b)
{ if (a!=b){ itv_linexpr_set(a->linexpr,b->linexpr); num_set(a->num,b->num); a->constyp = b->constyp; } }
static inline void itv_lincons_clear(itv_lincons_t cons)
{ itv_linexpr_clear(cons->linexpr); num_clear(cons->num); }
static inline void itv_lincons_print(itv_lincons_t cons, char** name)
{ itv_lincons_fprint(stdout,cons,name); }
static inline void itv_lincons_swap(itv_lincons_t a, itv_lincons_t b)
{ if (a!=b){ itv_lincons_struct t=*a; *a=*b; *b=t; } }
static inline void itv_lincons_array_print(itv_lincons_array_t array, char** name)
{ itv_lincons_array_fprint(stdout,array,name); }

static inline bool itv_lincons_is_integer(itv_lincons_t cons, size_t intdim)
{
  return itv_linexpr_is_integer(cons->linexpr,intdim);
}
static inline bool itv_lincons_is_linear(itv_lincons_t cons)
{
  return itv_linexpr_is_linear(cons->linexpr);
}
static inline bool itv_lincons_is_quasilinear(itv_lincons_t cons)
{
  return itv_linexpr_is_quasilinear(cons->linexpr);
}
static inline itvlinexpr_type_t itv_lincons_type(itv_lincons_t cons)
{
  return itv_linexpr_type(cons->linexpr);
}
static inline size_t itv_lincons_supportinterval(itv_lincons_t cons, ap_dim_t* tdim)
{
  return itv_linexpr_supportinterval(cons->linexpr,tdim);
}
static inline
bool itv_lincons_quasilinearize(itv_internal_t* intern,
				itv_lincons_t lincons,
				itv_t* env,
				bool for_meet_inequality)
{
  return itv_linexpr_quasilinearize(intern,lincons->linexpr,env,for_meet_inequality);
}
static inline
void itv_lincons_add_dimension(itv_lincons_t res,
			       itv_lincons_t cons,
			       ap_dimchange_t* dimchange)
{ itv_linexpr_add_dimension(res->linexpr,cons->linexpr,dimchange); }

static inline
void itv_lincons_permute_dimensions(itv_lincons_t res,
				    itv_lincons_t cons,
				    ap_dimperm_t* dimperm)
{ itv_linexpr_permute_dimensions(res->linexpr,cons->linexpr,dimperm); }

static inline
int itv_lincons_hash(itv_lincons_t cons)
{
  return (int)cons->constyp + itv_linexpr_hash(cons->linexpr);
}

#ifdef __cplusplus
}
#endif

#endif
