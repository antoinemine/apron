/* ********************************************************************** */
/* ap_linconsXXX.h */
/* ********************************************************************** */

#ifndef _AP_LINCONSXXX_H_
#define _AP_LINCONSXXX_H_

#include "itvXXX_types.h"
#include "ap_linexprXXX.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ap_constyp_t_
#define _ap_constyp_t_
/* Datatype for type of constraints */
typedef enum ap_constyp_t {
  AP_CONS_EQ,    /* equality constraint */
  AP_CONS_SUPEQ, /* >= constraint */
  AP_CONS_SUP,   /* > constraint */
  AP_CONS_EQMOD, /* congruence equality constraint */
  AP_CONS_DISEQ  /* disequality constraint */
} ap_constyp_t;
#endif

/* Interval linear constraint */
typedef struct ap_linconsXXX_struct {
  ap_linexprXXX_t linexpr;
  ap_constyp_t constyp;
  mpq_t mpq;
} ap_linconsXXX_struct;
typedef ap_linconsXXX_struct ap_linconsXXX_t[1];
typedef ap_linconsXXX_struct* ap_linconsXXX_ptr;

/* Array of interval linear constraints */
typedef struct ap_linconsXXX_array_struct {
  ap_linconsXXX_t* p;
  size_t size;
} ap_linconsXXX_array_struct;
typedef ap_linconsXXX_array_struct ap_linconsXXX_array_t[1];
typedef ap_linconsXXX_array_struct* ap_linconsXXX_array_ptr;

/* ********************************************************************** */
/* I. Constructor and Destructor */
/* ********************************************************************** */

static inline void ap_linconsXXX_init(ap_linconsXXX_t cons,size_t size);
static inline void ap_linconsXXX_init_set(ap_linconsXXX_t res, ap_linconsXXX_t cons);
static inline void ap_linconsXXX_init_set_linexpr(ap_linconsXXX_t res, ap_linexprXXX_t expr, ap_constyp_t constyp, mpq_ptr mpq);
void ap_linconsXXX_set_bool(ap_linconsXXX_t res, bool value);
static inline void ap_linconsXXX_resize(ap_linconsXXX_t expr, size_t size);
static inline void ap_linconsXXX_minimize(ap_linconsXXX_t e);
static inline void ap_linconsXXX_clear(ap_linconsXXX_t cons);

ap_linconsXXX_ptr ap_linconsXXX_alloc(size_t size);
ap_linconsXXX_ptr ap_linconsXXX_alloc_set(ap_linconsXXX_t cons);
ap_linconsXXX_ptr ap_linconsXXX_alloc_set_linexpr(ap_linexprXXX_t expr, ap_constyp_t constyp, mpq_ptr mpq);
void ap_linconsXXX_free(ap_linconsXXX_ptr cons);

void ap_linconsXXX_fprint(FILE* stream, ap_linconsXXX_t cons, char** name);
static inline void ap_linconsXXX_print(ap_linconsXXX_t cons, char** name);
static inline void ap_linconsXXX_swap(ap_linconsXXX_t a, ap_linconsXXX_t b);

void ap_linconsXXX_array_init(ap_linconsXXX_array_t array, size_t size);
void ap_linconsXXX_array_init_set(ap_linconsXXX_array_t res, ap_linconsXXX_array_t array);
void ap_linconsXXX_array_set(ap_linconsXXX_array_t res, ap_linconsXXX_array_t array);
void ap_linconsXXX_array_resize(ap_linconsXXX_array_t array, size_t size);
void ap_linconsXXX_array_minimize(ap_linconsXXX_array_t array);
void ap_linconsXXX_array_clear(ap_linconsXXX_array_t array);

ap_linconsXXX_array_ptr ap_linconsXXX_array_alloc(size_t size);
ap_linconsXXX_array_ptr ap_linconsXXX_array_alloc_set(ap_linconsXXX_array_t a);
void ap_linconsXXX_array_free(ap_linconsXXX_array_ptr a);

void ap_linconsXXX_array_fprint(FILE* stream, ap_linconsXXX_array_t array, char** name);
static inline void ap_linconsXXX_array_print(ap_linconsXXX_array_t array, char** name);

/* ********************************************************************** */
/* II. Tests and Simplifications */
/* ********************************************************************** */

static inline bool ap_linconsXXX_is_integer(ap_linconsXXX_t cons, size_t intdim);
static inline bool ap_linconsXXX_is_linear(ap_linconsXXX_t cons);
  /* Return true iff all involved coefficients are scalars */
static inline bool ap_linconsXXX_is_quasilinear(ap_linconsXXX_t cons);
  /* Return true iff all involved coefficients but the constant are scalars. */
static inline ap_linexpr_type_t ap_linconsXXX_type(ap_linconsXXX_t cons);
  /* Classify a constraint */

bool ap_linconsXXX_array_is_linear(ap_linconsXXX_array_t array);
bool ap_linconsXXX_array_is_quasilinear(ap_linconsXXX_array_t array);
ap_linexpr_type_t ap_linconsXXX_array_type(ap_linconsXXX_array_t array);

/* ********************************************************************** */
/* III. Evaluation, simplification and linearisation */
/* ********************************************************************** */

tbool_t ap_linconsXXX_evalcst(ap_linconsXXX_t lincons, itv_internal_t intern);
  /* Is the assumed constant constraint satisfied ? */

bool ap_linconsXXX_sat_is_false(ap_linconsXXX_t lincons, itv_internal_t intern);
  /* Return true if the constraint is not satisfiable
     *for all* deterministic expressions contained in it
     Cases taken into account:
     - expr + [a,A] = 0 (mod k) with a!=A
     - expr + [-oo,+oo] != 0
     - expr + (-oo,x] >|>= 0
     - the cases where itvXXX_eval_cstlincons returns tbool_false
  */

void ap_linconsXXX_reduce_integer(ap_linconsXXX_t cons, size_t intdim, itv_internal_t intern);
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

tbool_t ap_linconsXXX_array_reduce(ap_linconsXXX_array_t array, bool meet, itv_internal_t intern);
  /* Simplify the array as follows:
     - remove trivially true constraints (like 1>=0)
     - if a constraint is trivially false, reduce the array to the constraint
       1=0
     Return
     - tbool_true if empty array
     - tbool_false if trivially false
     - tbool_top otherwise
  */
tbool_t ap_linconsXXX_array_reduce_integer(ap_linconsXXX_array_t array,
					 size_t intdim, itv_internal_t intern);
  /* Apply first ap_linconsXXX_reduce_integer, and then
     ap_linconsXXX_array_reduce(.,.,true) */

static inline
size_t ap_linconsXXX_supportinterval(ap_linconsXXX_t cons, ap_dim_t* tdim);
  /* Fills the array tdim with the dimensions associated with intervals in
     the constraint, in increasing order, and return the number of such
     dimensions.

     tdim is supposed to be of size at least the maximum dimension + 1 in the
     expression.
 */
size_t ap_linconsXXX_array_supportinterval(ap_linconsXXX_array_t array, ap_dim_t* tdim, size_t maxdim1);
  /* Idem, with maxdim1 indicating the maximum possible dimension plus 1 */

/* These functions quasilinearize in-place expressions and constraints.  They
   optimize (sets of) constraints when the parameter meet is true, by
   deducing things. If constraints are quasilinearized for testing
   satisfaction, meet should be set to false.
*/
static inline
bool ap_linconsXXX_quasilinearize(ap_linconsXXX_t lincons,
				  itvXXX_t* env,
				  bool for_meet_inequality, itv_internal_t intern);
  /* Quasilinearize in-place lincons using the bounding box itv. Return true
     if no approximations. */

bool ap_linconsXXX_array_quasilinearize(ap_linconsXXX_array_t array,
					itvXXX_t* env,
					bool for_meet_inequality, itv_internal_t intern);
  /* Same for an array */


/* ********************************************************************** */
/* IV. Boxization of interval linear expressions */
/* ********************************************************************** */

bool ap_linconsXXX_array_boxize(itvXXX_t* res,
				bool* tchange,
				ap_linconsXXX_array_t array,
				itvXXX_t* env,size_t intdim,
				size_t kmax,
				bool intervalonly, itv_internal_t intern);
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
void ap_linconsXXX_add_dimensions(ap_linconsXXX_t res,
				ap_linconsXXX_t cons,
				ap_dimchange_t* dimchange);
void ap_linconsXXX_array_add_dimensions(ap_linconsXXX_array_t res,
				      ap_linconsXXX_array_t array,
				      ap_dimchange_t* dimchange);

/* These two functions apply the given permutation to the dimensions.
   The dimensions present in the consession should just be less
   than the size of the permutation. */
static inline
void ap_linconsXXX_permute_dimensions(ap_linconsXXX_t res,
				    ap_linconsXXX_t cons,
				    ap_dimperm_t* perm);
void ap_linconsXXX_array_permute_dimensions(ap_linconsXXX_array_t res,
					  ap_linconsXXX_array_t array,
					  ap_dimperm_t* dimchange);
static inline
void ap_linconsXXX_extend_environment(ap_linconsXXX_t res,
				      bool* perror,
				      ap_environment_t* nenv,
				      ap_linconsXXX_t expr,
				      ap_environment_t* env);
void ap_linconsXXX_array_extend_environment(ap_linconsXXX_array_t res,
					    bool* perror,
					    ap_environment_t* nenv,
					    ap_linconsXXX_array_t expr,
					    ap_environment_t* env);

/* ********************************************************************** */
/* VI. Hashing, comparison */
/* ********************************************************************** */

static inline int ap_linconsXXX_hash(ap_linconsXXX_t cons);
bool ap_linconsXXX_equal(ap_linconsXXX_t cons1,ap_linconsXXX_t cons2);

/* Lexicographic ordering, terminating by constant coefficients */
int ap_linconsXXX_compare(ap_linconsXXX_t cons1, ap_linconsXXX_t cons2);

/* ********************************************************************** */
/* Definition of inline functions */
/* ********************************************************************** */

static inline void ap_linconsXXX_init(ap_linconsXXX_t cons,size_t size)
{ ap_linexprXXX_init(cons->linexpr,size); cons->constyp = AP_CONS_EQ; mpq_init(cons->mpq); }
static inline void ap_linconsXXX_set(ap_linconsXXX_t a, ap_linconsXXX_t b)
{ if (a!=b){ ap_linexprXXX_set(a->linexpr,b->linexpr); mpq_set(a->mpq,b->mpq); a->constyp = b->constyp; } }
static inline void ap_linconsXXX_init_set(ap_linconsXXX_t a, ap_linconsXXX_t b)
{ assert(a!=b); ap_linexprXXX_init_set(a->linexpr,b->linexpr); a->constyp = b->constyp; mpq_init(a->mpq); mpq_set(a->mpq,b->mpq); }
static inline void ap_linconsXXX_init_set_linexpr(ap_linconsXXX_t res, ap_linexprXXX_t expr, ap_constyp_t constyp, mpq_ptr mpq)
{ 
  ap_linexprXXX_init_set(res->linexpr,expr); res->constyp = constyp; 
  mpq_init(res->mpq);
  if (mpq) mpq_set(res->mpq,mpq);
}
static inline void ap_linconsXXX_resize(ap_linconsXXX_t e, size_t size)
{ ap_linexprXXX_resize(e->linexpr,size); }
static inline void ap_linconsXXX_minimize(ap_linconsXXX_t e)
{ ap_linexprXXX_minimize(e->linexpr); }
static inline void ap_linconsXXX_clear(ap_linconsXXX_t cons)
{ ap_linexprXXX_clear(cons->linexpr); mpq_clear(cons->mpq); }
static inline void ap_linconsXXX_print(ap_linconsXXX_t cons, char** name)
{ ap_linconsXXX_fprint(stdout,cons,name); }
static inline void ap_linconsXXX_swap(ap_linconsXXX_t a, ap_linconsXXX_t b)
{ if (a!=b){ ap_linconsXXX_struct t=*a; *a=*b; *b=t; } }
static inline void ap_linconsXXX_array_print(ap_linconsXXX_array_t array, char** name)
{ ap_linconsXXX_array_fprint(stdout,array,name); }

static inline bool ap_linconsXXX_is_integer(ap_linconsXXX_t cons, size_t intdim)
{
  return ap_linexprXXX_is_integer(cons->linexpr,intdim);
}
static inline bool ap_linconsXXX_is_linear(ap_linconsXXX_t cons)
{
  return ap_linexprXXX_is_linear(cons->linexpr);
}
static inline bool ap_linconsXXX_is_quasilinear(ap_linconsXXX_t cons)
{
  return ap_linexprXXX_is_quasilinear(cons->linexpr);
}
static inline ap_linexpr_type_t ap_linconsXXX_type(ap_linconsXXX_t cons)
{
  return ap_linexprXXX_type(cons->linexpr);
}
static inline size_t ap_linconsXXX_supportinterval(ap_linconsXXX_t cons, ap_dim_t* tdim)
{
  return ap_linexprXXX_supportinterval(cons->linexpr,tdim);
}
static inline
bool ap_linconsXXX_quasilinearize(
				ap_linconsXXX_t lincons,
				itvXXX_t* env,
				bool for_meet_inequality, itv_internal_t intern)
{
  return ap_linexprXXX_quasilinearize(lincons->linexpr,env,for_meet_inequality, intern);
}
static inline
void ap_linconsXXX_add_dimensions(ap_linconsXXX_t res,
				ap_linconsXXX_t cons,
				ap_dimchange_t* dimchange)
{ ap_linexprXXX_add_dimensions(res->linexpr,cons->linexpr,dimchange); }

static inline
void ap_linconsXXX_permute_dimensions(ap_linconsXXX_t res,
				    ap_linconsXXX_t cons,
				    ap_dimperm_t* dimperm)
{ ap_linexprXXX_permute_dimensions(res->linexpr,cons->linexpr,dimperm); }
static inline
void ap_linconsXXX_extend_environment(ap_linconsXXX_t res,
				      bool* perror,
				      ap_environment_t* nenv,
				      ap_linconsXXX_t expr,
				      ap_environment_t* env)
{ ap_linexprXXX_extend_environment(res->linexpr,perror,nenv,expr->linexpr,env); }

static inline
int ap_linconsXXX_hash(ap_linconsXXX_t cons)
{
  return (int)cons->constyp + ap_linexprXXX_hash(cons->linexpr);
}

#ifdef __cplusplus
}
#endif

#endif
