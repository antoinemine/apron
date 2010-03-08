/* ********************************************************************** */
/* itv_linexpr.c: */
/* ********************************************************************** */

#ifndef _ITV_LINEXPR_H_
#define _ITV_LINEXPR_H_

#include "eitv.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* Dataypes */
/* ********************************************************************** */

/* Interval linear term */
typedef struct itv_linterm_t {
  eitv_t eitv;
  ap_dim_t dim;
} itv_linterm_t;

/* Interval linear expression */
typedef struct itv_linexpr_t {
  itv_linterm_t* linterm;
  size_t size;
  eitv_t cst;
} itv_linexpr_t;

/* Interval linear constraint */
typedef struct itv_lincons_t {
  itv_linexpr_t linexpr;
  ap_constyp_t constyp;
  num_t num;
} itv_lincons_t;

/* Array of interval linear constraint */
typedef struct itv_lincons_array_t {
  itv_lincons_t* p;
  size_t size;
} itv_lincons_array_t;

/* Iterator (Macro): use:
   itv_linexpr_ForeachLinterm(itv_linexpr_t* e, size_t i, ap_dim_t d, itv_ptr pitv, bool* pequality){
     ..
   }
   where
   - e is the inspected expression,
   - i is the internal iterator (of type size_t or int)
   - dim is the dimension of one linear term
   - peitv is a pointer to the corresponding coefficient
   - pequality indicates if the interval is actually a point

*/
#define itv_linexpr_ForeachLinterm(_p_e, _p_i, _p_d, _p_eitv, _p_equality) \
  for ((_p_i)=0;							\
       (_p_i)<(_p_e)->size ?						\
	 (((_p_d) = (_p_e)->linterm[i].dim),				\
	  ((_p_eitv) = (_p_e)->linterm[i].itv),				\
	  ((_p_equality) = &((_p_e)->linterm[i].equality)),		\
	  ((_p_d)!=AP_DIM_MAX)) :					\
	 false;								\
       (_p_i)++)

/* ********************************************************************** */
/* I. Constructor and Destructor */
/* ********************************************************************** */

static inline void itv_linterm_init(itv_linterm_t* term);
static inline void itv_linterm_init_set(itv_linterm_t* res, itv_linterm_t* term);
static inline void itv_linterm_set(itv_linterm_t* res, itv_linterm_t* term);
static inline void itv_linterm_clear(itv_linterm_t* term);
static inline void itv_linterm_swap(itv_linterm_t* a, itv_linterm_t* b);

void itv_linexpr_init(itv_linexpr_t* expr, size_t size);
void itv_linexpr_init_set(itv_linexpr_t* res, itv_linexpr_t* expr);
void itv_linexpr_set(itv_linexpr_t* res, itv_linexpr_t* expr);
void itv_linexpr_reinit(itv_linexpr_t* expr, size_t size);
void itv_linexpr_clear(itv_linexpr_t* expr);
void itv_linexpr_fprint(FILE* stream, itv_linexpr_t* expr, char** name);
static inline void itv_linexpr_print(itv_linexpr_t* expr, char** name);

static inline void itv_lincons_init(itv_lincons_t* cons);
static inline void itv_lincons_set(itv_lincons_t* res, itv_lincons_t* expr);
void itv_lincons_set_bool(itv_lincons_t* res, bool value);
static inline void itv_lincons_clear(itv_lincons_t* cons);
void itv_lincons_fprint(FILE* stream, itv_lincons_t* cons, char** name);
static inline void itv_lincons_print(itv_lincons_t* cons, char** name);
static inline void itv_lincons_swap(itv_lincons_t* a, itv_lincons_t* b);

void itv_lincons_array_init(itv_lincons_array_t* array, size_t size);
void itv_lincons_array_reinit(itv_lincons_array_t* array, size_t size);
void itv_lincons_array_clear(itv_lincons_array_t* array);
void itv_lincons_array_fprint(FILE* stream, itv_lincons_array_t* array, char** name);
static inline void itv_lincons_array_print(itv_lincons_array_t* array, char** name);

/* ********************************************************************** */
/* II. Arithmetic */
/* ********************************************************************** */

void itv_linexpr_neg(itv_linexpr_t* expr);
  /* Negate in-place an expression */
void itv_linexpr_scale(itv_internal_t* intern,
				     itv_linexpr_t* expr, itv_t coeff);
  /* Scale in-place an expression by an interval */

void itv_linexpr_add(itv_internal_t* intern,
				   itv_linexpr_t* expr,
				   itv_linexpr_t* exprA,
				   itv_linexpr_t* exprB);
void itv_linexpr_sub(itv_internal_t* intern,
				   itv_linexpr_t* expr,
				   itv_linexpr_t* exprA,
				   itv_linexpr_t* exprB);
  /* Resp. add and substract two linear epxression.
     (Substraction temporarily negates exprB, and then restores it */

/* ********************************************************************** */
/* III. Tests and Simplifications */
/* ********************************************************************** */

bool itv_linexpr_is_integer(itv_linexpr_t* expr, size_t intdim);
bool itv_lincons_is_integer(itv_lincons_t* cons, size_t intdim);
bool itv_linexpr_is_scalar(itv_linexpr_t* expr);
bool itv_linexpr_is_quasilinear(itv_linexpr_t* expr);
bool itv_linexpr_is_integer(itv_linexpr_t* expr, size_t intdim);
bool itv_lincons_is_scalar(itv_lincons_t* expr);
bool itv_lincons_is_quasilinear(itv_lincons_t* expr);
bool itv_lincons_array_is_scalar(itv_lincons_array_t* array);
bool itv_lincons_array_is_quasilinear(itv_lincons_array_t* array);
bool itv_linexpr_is_integer(itv_linexpr_t* expr, size_t intdim);
bool itv_lincons_is_integer(itv_lincons_t* cons, size_t intdim);

tbool_t itv_eval_cstlincons(itv_internal_t* intern,
					  itv_lincons_t* lincons);
  /* Is the assumed constant constraint satisfied ? */

bool itv_sat_lincons_is_false(itv_internal_t* intern,
					    itv_lincons_t* lincons);
  /* Return true if the constraint is not satisfiable
     *for all* deterministic expressions contained in it
     Cases taken into account:
     - expr + [a,A] = 0 (mod k) with a!=A
     - expr + [-oo,+oo] != 0
     - expr + (-oo,x] >|>= 0
     - the cases where itv_eval_cstlincons returns tbool_false
  */

void itv_lincons_reduce_integer(itv_internal_t* intern,
					      itv_lincons_t* cons, size_t intdim);
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
					       itv_lincons_array_t* array, bool meet);
  /* Simplify the array as follows:
     - remove trivially true constraints (like 1>=0)
     - if a constraint is trivially false, reduce the array to the constraint
       1=0
     Return
     - tbool_true if rmpty array
     - tbool_false if trivially false
     - tbool_top otherwise
  */
tbool_t itv_lincons_array_reduce_integer(itv_internal_t* intern,
						       itv_lincons_array_t* array,
						       size_t intdim);
  /* Apply first itv_lincons_reduce_integer, and then
     itv_lincons_array_reduce(.,.,true) */

/* ********************************************************************** */
/* Definition of inline functions */
/* ********************************************************************** */

static inline void itv_linterm_init(itv_linterm_t* term)
{ itv_init(term->itv); term->equality = true; term->dim = AP_DIM_MAX; }
static inline void itv_linterm_init_set(itv_linterm_t* res, itv_linterm_t* term)
{ itv_init_set(res->itv,term->itv); res->equality = term->equality; res->dim = term->dim; }
static inline void itv_linterm_set(itv_linterm_t* res, itv_linterm_t* term)
{ itv_set(res->itv,term->itv); res->equality = term->equality; res->dim = term->dim; }
static inline void itv_linterm_clear(itv_linterm_t* term)
{ itv_clear(term->itv); }
static inline void itv_linterm_swap(itv_linterm_t* a, itv_linterm_t* b)
{ if (a!=b){ itv_linterm_t t=*a; *a=*b; *b=t; } }

static inline void itv_linexpr_print(itv_linexpr_t* expr, char** name)
{ itv_linexpr_fprint(stdout,expr,name); }

static inline void itv_lincons_init(itv_lincons_t* cons)
{ itv_linexpr_init(&cons->linexpr,0); num_init(cons->num); }
static inline void itv_lincons_set(itv_lincons_t* a, itv_lincons_t* b)
{ if (a!=b){ itv_linexpr_set(&a->linexpr,&b->linexpr); num_set(a->num,b->num); a->constyp = b->constyp; } }
static inline void itv_lincons_clear(itv_lincons_t* cons)
{ itv_linexpr_clear(&cons->linexpr); num_clear(cons->num); }
static inline void itv_lincons_print(itv_lincons_t* cons, char** name)
{ itv_lincons_fprint(stdout,cons,name); }
static inline void itv_lincons_swap(itv_lincons_t* a, itv_lincons_t* b)
{ if (a!=b){ itv_lincons_t t=*a; *a=*b; *b=t; } }
static inline void itv_lincons_array_print(itv_lincons_array_t* array, char** name)
{ itv_lincons_array_fprint(stdout,array,name); }

#ifdef __cplusplus
}
#endif

#endif
