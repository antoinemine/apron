/* ********************************************************************** */
/* itv_lincons.h */
/* ********************************************************************** */

#ifndef _ITV_LINCONS_H_
#define _ITV_LINCONS_H_

#include "eitv.h"
#include "itv_linexpr.h"

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

bool itv_lincons_is_linear(itv_lincons_t expr);
bool itv_lincons_is_quasilinear(itv_lincons_t expr);
bool itv_lincons_array_is_linear(itv_lincons_array_t array);
bool itv_lincons_array_is_quasilinear(itv_lincons_array_t array);
static inline bool itv_lincons_is_integer(itv_lincons_t cons, size_t intdim);

tbool_t itv_eval_cstlincons(itv_internal_t* intern,
			    itv_lincons_t lincons);
  /* Is the assumed constant constraint satisfied ? */

bool itv_sat_lincons_is_false(itv_internal_t* intern,
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
     - tbool_true if rmpty array
     - tbool_false if trivially false
     - tbool_top otherwise
  */
tbool_t itv_lincons_array_reduce_integer(itv_internal_t* intern,
					 itv_lincons_array_t array,
					 size_t intdim);
  /* Apply first itv_lincons_reduce_integer, and then
     itv_lincons_array_reduce(.,.,true) */

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

bool itv_lincons_is_integer(itv_lincons_t cons, size_t intdim)
{
  return itv_linexpr_is_integer(cons->linexpr,intdim);
}

#ifdef __cplusplus
}
#endif

#endif
