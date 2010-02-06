/* ********************************************************************** */
/* itv_linexpr.c: */
/* ********************************************************************** */

#ifndef _ITV_LINEXPR_H_
#define _ITV_LINEXPR_H_

#include "itv.h"
#include "ap_expr0.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* Dataypes */
/* ********************************************************************** */

/* Interval linear term */
typedef struct itv_linterm_t {
  itv_t itv;
  bool equality;
    /* indicate that the interval is reduced to a single value */
  ap_dim_t dim;
} itv_linterm_t;

/* Interval linear expression */
typedef struct itv_linexpr_t {
  itv_linterm_t* linterm;
  size_t size;
  itv_t cst;
  bool equality;
    /* indicate that the interval cst is reduced to a single value */
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
   - pitv is a pointer to the corresponding coefficient
   - pequality indicates if the interval is actually a point

*/
#define itv_linexpr_ForeachLinterm(_p_e, _p_i, _p_d, _p_itv, _p_equality) \
  for ((_p_i)=0;							\
       (_p_i)<(_p_e)->size ?						\
	 (((_p_d) = (_p_e)->linterm[i].dim),				\
	  ((_p_itv) = (_p_e)->linterm[i].itv),				\
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

static inline void itv_linexpr_init(itv_linexpr_t* expr, size_t size);
static inline void itv_linexpr_init_set(itv_linexpr_t* res, itv_linexpr_t* expr);
static inline void itv_linexpr_set(itv_linexpr_t* res, itv_linexpr_t* expr);
static inline void itv_linexpr_reinit(itv_linexpr_t* expr, size_t size);
static inline void itv_linexpr_clear(itv_linexpr_t* expr);
static inline void itv_linexpr_fprint(FILE* stream, itv_linexpr_t* expr, char** name);
static inline void itv_linexpr_print(itv_linexpr_t* expr, char** name);

static inline void itv_lincons_init(itv_lincons_t* cons);
static inline void itv_lincons_set(itv_lincons_t* res, itv_lincons_t* expr);
static inline void itv_lincons_set_bool(itv_lincons_t* res, bool value);
static inline void itv_lincons_clear(itv_lincons_t* cons);
static inline void itv_lincons_fprint(FILE* stream, itv_lincons_t* cons, char** name);
static inline void itv_lincons_print(itv_lincons_t* cons, char** name);
static inline void itv_lincons_swap(itv_lincons_t* a, itv_lincons_t* b);

static inline void itv_lincons_array_init(itv_lincons_array_t* array, size_t size);
static inline void itv_lincons_array_reinit(itv_lincons_array_t* array, size_t size);
static inline void itv_lincons_array_clear(itv_lincons_array_t* array);
static inline void itv_lincons_array_fprint(FILE* stream, itv_lincons_array_t* array, char** name);
static inline void itv_lincons_array_print(itv_lincons_array_t* array, char** name);

/* ********************************************************************** */
/* II. Conversions from and to APRON datatypes */
/* ********************************************************************** */

static inline bool itv_linexpr_set_ap_linexpr0(itv_internal_t* intern,
 itv_linexpr_t* expr,
 ap_linexpr0_t* linexpr0);
  /* Convert an ap_linexpr0_t into an itv_linexpr_t.
     expr points to an initialized object.
     Return true if the conversion is exact
  */
static inline bool itv_lincons_set_ap_lincons0(itv_internal_t* intern,
 itv_lincons_t* cons,
 ap_lincons0_t* lincons0);
  /* Convert an ap_lincons0_t into an itv_lincons_t
     cons points to an initialized object.
     Return true if the conversion is exact
  */

static inline bool itv_lincons_array_set_ap_lincons0_array(itv_internal_t* intern,
							   itv_lincons_array_t* tcons, ap_lincons0_array_t* tlincons0);
  /* Convert an ap_lincons0_array_t into an itv_lincons_array_t
     tcons points to an initialized object.
     Return true if the conversion is exact
  */

static inline void ap_linexpr0_set_itv_linexpr(itv_internal_t* intern,
					       ap_linexpr0_t** plinexpr0, itv_linexpr_t* linexpr);
  /* Convert an itv_linexpr_t into an ap_linexpr0_t.
     If the result *plinexpr0 is not NULL,
     possibly reinitialize *plinexpr0, otherwise allocate it.
  */
static inline void ap_lincons0_set_itv_lincons(itv_internal_t* intern,
					       ap_lincons0_t* plincons0, itv_lincons_t* lincons);
  /* Convert an itv_linexpr_t into an ap_linexpr0_t.
     The result plincons0 is supposed initialized.
  */

/* ********************************************************************** */
/* III. Arithmetic */
/* ********************************************************************** */

static inline void itv_linexpr_neg(itv_linexpr_t* expr);
  /* Negate in-place an expression */
static inline void itv_linexpr_scale(itv_internal_t* intern,
				     itv_linexpr_t* expr, itv_t coeff);
  /* Scale in-place an expression by an interval */

static inline void itv_linexpr_add(itv_internal_t* intern,
				   itv_linexpr_t* expr,
				   itv_linexpr_t* exprA,
				   itv_linexpr_t* exprB);
static inline void itv_linexpr_sub(itv_internal_t* intern,
				   itv_linexpr_t* expr,
				   itv_linexpr_t* exprA,
				   itv_linexpr_t* exprB);
  /* Resp. add and substract two linear epxression.
     (Substraction temporarily negates exprB, and then restores it */

/* ********************************************************************** */
/* IV. Tests and Simplifications */
/* ********************************************************************** */

static inline bool itv_linexpr_is_integer(itv_linexpr_t* expr, size_t intdim);
static inline bool itv_lincons_is_integer(itv_lincons_t* cons, size_t intdim);
static inline bool itv_linexpr_is_scalar(itv_linexpr_t* expr);
static inline bool itv_linexpr_is_quasilinear(itv_linexpr_t* expr);
static inline bool itv_linexpr_is_integer(itv_linexpr_t* expr, size_t intdim);
static inline bool itv_lincons_is_scalar(itv_lincons_t* expr);
static inline bool itv_lincons_is_quasilinear(itv_lincons_t* expr);
static inline bool itv_lincons_array_is_scalar(itv_lincons_array_t* array);
static inline bool itv_lincons_array_is_quasilinear(itv_lincons_array_t* array);
static inline bool itv_linexpr_is_integer(itv_linexpr_t* expr, size_t intdim);
static inline bool itv_lincons_is_integer(itv_lincons_t* cons, size_t intdim);

static inline tbool_t itv_eval_cstlincons(itv_internal_t* intern,
					  itv_lincons_t* lincons);
  /* Is the assumed constant constraint satisfied ? */

static inline bool itv_sat_lincons_is_false(itv_internal_t* intern,
					    itv_lincons_t* lincons);
  /* Return true if the constraint is not satisfiable
     *for all* deterministic expressions contained in it
     Cases taken into account:
     - expr + [a,A] = 0 (mod k) with a!=A
     - expr + [-oo,+oo] != 0
     - expr + (-oo,x] >|>= 0
     - the cases where itv_eval_cstlincons returns tbool_false
  */

static inline void itv_lincons_reduce_integer(itv_internal_t* intern,
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

static inline tbool_t itv_lincons_array_reduce(itv_internal_t* intern,
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
static inline tbool_t itv_lincons_array_reduce_integer(itv_internal_t* intern,
						       itv_lincons_array_t* array,
						       size_t intdim);
  /* Apply first itv_lincons_reduce_integer, and then
     itv_lincons_array_reduce(.,.,true) */

/* ********************************************************************** */
/* Definition of inline functions */
/* ********************************************************************** */

void ITVFUN(itv_linexpr_init)(itv_linexpr_t* expr, size_t size);
void ITVFUN(itv_linexpr_init_set)(itv_linexpr_t* res, itv_linexpr_t* expr);
void ITVFUN(itv_linexpr_set)(itv_linexpr_t* res, itv_linexpr_t* expr);
void ITVFUN(itv_linexpr_reinit)(itv_linexpr_t* expr, size_t size);
void ITVFUN(itv_linexpr_clear)(itv_linexpr_t* expr);
void ITVFUN(itv_linexpr_fprint)(FILE* stream, itv_linexpr_t* expr, char** name);
void ITVFUN(itv_linexpr_print)(itv_linexpr_t* expr, char** name);
void ITVFUN(itv_lincons_set_bool)(itv_lincons_t* cons, bool value);
void ITVFUN(itv_lincons_fprint)(FILE* stream, itv_lincons_t* cons, char** name);
void ITVFUN(itv_lincons_print)(itv_lincons_t* cons, char** name);

void ITVFUN(itv_lincons_array_init)(itv_lincons_array_t* array, size_t size);
void ITVFUN(itv_lincons_array_reinit)(itv_lincons_array_t* array, size_t size);
void ITVFUN(itv_lincons_array_clear)(itv_lincons_array_t* array);
void ITVFUN(itv_lincons_array_fprint)(FILE* stream, itv_lincons_array_t* array, char** name);
void ITVFUN(itv_lincons_array_print)(itv_lincons_array_t* array, char** name);

bool ITVFUN(itv_linexpr_set_ap_linexpr0)(itv_internal_t* intern, itv_linexpr_t* expr, ap_linexpr0_t* linexpr0);
bool ITVFUN(itv_lincons_set_ap_lincons0)(itv_internal_t* intern, itv_lincons_t* cons, ap_lincons0_t* lincons0);
bool ITVFUN(itv_lincons_array_set_ap_lincons0_array)(itv_internal_t* intern, itv_lincons_array_t* tcons, ap_lincons0_array_t* tlincons0);

void ITVFUN(ap_linexpr0_set_itv_linexpr)(itv_internal_t* intern, ap_linexpr0_t** linexpr0, itv_linexpr_t* linexpr);
void ITVFUN(ap_lincons0_set_itv_lincons)(itv_internal_t* intern, ap_lincons0_t* lincons0, itv_lincons_t* lincons);

void ITVFUN(itv_linexpr_neg)(itv_linexpr_t* expr);
void ITVFUN(itv_linexpr_scale)(itv_internal_t* intern, itv_linexpr_t* expr, itv_t coeff);
void ITVFUN(itv_linexpr_div)(itv_internal_t* intern, itv_linexpr_t* expr, itv_t coeff);
void ITVFUN(itv_linexpr_add)(itv_internal_t* intern, itv_linexpr_t* expr, itv_linexpr_t* exprA, itv_linexpr_t* exprB);
void ITVFUN(itv_linexpr_sub)(itv_internal_t* intern, itv_linexpr_t* expr, itv_linexpr_t* exprA, itv_linexpr_t* exprB);

bool ITVFUN(itv_linexpr_is_scalar)(itv_linexpr_t* expr);
bool ITVFUN(itv_linexpr_is_quasilinear)(itv_linexpr_t* expr);
bool ITVFUN(itv_lincons_array_is_scalar)(itv_lincons_array_t* array);
bool ITVFUN(itv_lincons_array_is_quasilinear)(itv_lincons_array_t* array);
bool ITVFUN(itv_linexpr_is_integer)(itv_linexpr_t* expr, size_t intdim);
bool ITVFUN(itv_lincons_is_integer)(itv_lincons_t* cons, size_t intdim);

tbool_t ITVFUN(itv_eval_cstlincons)(itv_internal_t* intern, itv_lincons_t* lincons);
bool ITVFUN(itv_sat_lincons_is_false)(itv_internal_t* intern, itv_lincons_t* lincons);
void ITVFUN(itv_lincons_reduce_integer)(itv_internal_t* intern, itv_lincons_t* cons, size_t intdim);
tbool_t ITVFUN(itv_lincons_array_reduce)(itv_internal_t* intern, itv_lincons_array_t* array, bool meet);
tbool_t ITVFUN(itv_lincons_array_reduce_integer)(itv_internal_t* intern, itv_lincons_array_t* array, size_t intdim);


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

static inline void itv_linexpr_init(itv_linexpr_t* expr, size_t size)
{ ITVFUN(itv_linexpr_init)(expr,size); }
static inline void itv_linexpr_init_set(itv_linexpr_t* res, itv_linexpr_t* expr)
{ ITVFUN(itv_linexpr_init_set)(res,expr); }
static inline void itv_linexpr_set(itv_linexpr_t* res, itv_linexpr_t* expr)
{ ITVFUN(itv_linexpr_set)(res,expr); }

static inline void itv_linexpr_reinit(itv_linexpr_t* expr, size_t size)
{ ITVFUN(itv_linexpr_reinit)(expr,size); }

static inline void itv_linexpr_clear(itv_linexpr_t* expr)
{ ITVFUN(itv_linexpr_clear)(expr); }

static inline void itv_linexpr_fprint(FILE* stream, itv_linexpr_t* expr, char** name)
{ ITVFUN(itv_linexpr_fprint)(stream,expr,name); }

static inline void itv_linexpr_print(itv_linexpr_t* expr, char** name)
{ ITVFUN(itv_linexpr_print)(expr,name); }

static inline void itv_lincons_init(itv_lincons_t* cons)
{ itv_linexpr_init(&cons->linexpr,0); num_init(cons->num); }
static inline void itv_lincons_set(itv_lincons_t* a, itv_lincons_t* b)
{ if (a!=b){ itv_linexpr_set(&a->linexpr,&b->linexpr); num_set(a->num,b->num); a->constyp = b->constyp; } }
static inline void itv_lincons_set_bool(itv_lincons_t* a, bool value)
{ ITVFUN(itv_lincons_set_bool)(a,value); }
static inline void itv_lincons_clear(itv_lincons_t* cons)
{ itv_linexpr_clear(&cons->linexpr); num_clear(cons->num); }

static inline void itv_lincons_fprint(FILE* stream, itv_lincons_t* cons, char** name)
{ ITVFUN(itv_lincons_fprint)(stream,cons,name); }

static inline void itv_lincons_print(itv_lincons_t* cons, char** name)
{ ITVFUN(itv_lincons_print)(cons,name); }
static inline void itv_lincons_swap(itv_lincons_t* a, itv_lincons_t* b)
{ if (a!=b){ itv_lincons_t t=*a; *a=*b; *b=t; } }

static inline void itv_lincons_array_init(itv_lincons_array_t* array, size_t size)
{ ITVFUN(itv_lincons_array_init)(array,size); }

static inline void itv_lincons_array_reinit(itv_lincons_array_t* array, size_t size)
{ ITVFUN(itv_lincons_array_reinit)(array,size); }

static inline void itv_lincons_array_clear(itv_lincons_array_t* array)
{ ITVFUN(itv_lincons_array_clear)(array); }

static inline void itv_lincons_array_fprint(FILE* stream, itv_lincons_array_t* array, char** name)
{ ITVFUN(itv_lincons_array_fprint)(stream,array,name); }

static inline void itv_lincons_array_print(itv_lincons_array_t* array, char** name)
{ ITVFUN(itv_lincons_array_print)(array,name); }


static inline bool itv_linexpr_set_ap_linexpr0(itv_internal_t* intern, itv_linexpr_t* expr, ap_linexpr0_t* linexpr0)
{ return ITVFUN(itv_linexpr_set_ap_linexpr0)(intern,expr,linexpr0); }

static inline bool itv_lincons_set_ap_lincons0(itv_internal_t* intern, itv_lincons_t* cons, ap_lincons0_t* lincons0)
{ return ITVFUN(itv_lincons_set_ap_lincons0)(intern,cons,lincons0); }

static inline bool itv_lincons_array_set_ap_lincons0_array(itv_internal_t* intern, itv_lincons_array_t* tcons, ap_lincons0_array_t* tlincons0)
{ return ITVFUN(itv_lincons_array_set_ap_lincons0_array)(intern,tcons,tlincons0); }

static inline void ap_linexpr0_set_itv_linexpr(itv_internal_t* intern, ap_linexpr0_t** expr, itv_linexpr_t* linexpr0)
{ ITVFUN(ap_linexpr0_set_itv_linexpr)(intern,expr,linexpr0); }

static inline void ap_lincons0_set_itv_lincons(itv_internal_t* intern, ap_lincons0_t* cons, itv_lincons_t* lincons0)
{ ITVFUN(ap_lincons0_set_itv_lincons)(intern,cons,lincons0); }

static inline void itv_linexpr_neg(itv_linexpr_t* expr)
{ ITVFUN(itv_linexpr_neg)(expr); }

static inline void itv_linexpr_scale(itv_internal_t* intern, itv_linexpr_t* expr, itv_t coeff)
{ ITVFUN(itv_linexpr_scale)(intern,expr,coeff); }

static inline void itv_linexpr_div(itv_internal_t* intern, itv_linexpr_t* expr, itv_t coeff)
{ ITVFUN(itv_linexpr_div)(intern,expr,coeff); }

static inline void itv_linexpr_add(itv_internal_t* intern, itv_linexpr_t* expr, itv_linexpr_t* exprA, itv_linexpr_t* exprB)
{ ITVFUN(itv_linexpr_add)(intern,expr,exprA,exprB); }

static inline void itv_linexpr_sub(itv_internal_t* intern, itv_linexpr_t* expr, itv_linexpr_t* exprA, itv_linexpr_t* exprB)
{ ITVFUN(itv_linexpr_sub)(intern,expr,exprA,exprB); }

static inline bool itv_linexpr_is_scalar(itv_linexpr_t* expr)
{ return ITVFUN(itv_linexpr_is_scalar)(expr); }
static inline bool itv_linexpr_is_quasilinear(itv_linexpr_t* expr)
{ return ITVFUN(itv_linexpr_is_quasilinear)(expr); }
static inline bool itv_lincons_is_scalar(itv_lincons_t* cons)
{ return ITVFUN(itv_linexpr_is_scalar)(&cons->linexpr); }
static inline bool itv_lincons_is_quasilinear(itv_lincons_t* cons)
{ return ITVFUN(itv_linexpr_is_quasilinear)(&cons->linexpr); }
static inline bool itv_lincons_array_is_scalar(itv_lincons_array_t* array)
{ return ITVFUN(itv_lincons_array_is_scalar)(array); }
static inline bool itv_lincons_array_is_quasilinear(itv_lincons_array_t* array)
{ return ITVFUN(itv_lincons_array_is_quasilinear)(array); }
static inline bool itv_linexpr_is_integer(itv_linexpr_t* expr, size_t intdim)
{ return ITVFUN(itv_linexpr_is_integer)(expr,intdim); }
static inline bool itv_lincons_is_integer(itv_lincons_t* cons, size_t intdim)
{ return ITVFUN(itv_linexpr_is_integer)(&cons->linexpr,intdim); }
static inline tbool_t itv_eval_cstlincons(itv_internal_t* intern, itv_lincons_t* lincons)
{ return ITVFUN(itv_eval_cstlincons)(intern,lincons); }
static inline bool itv_sat_lincons_is_false(itv_internal_t* intern, itv_lincons_t* lincons)
{ return ITVFUN(itv_sat_lincons_is_false)(intern,lincons); }
static inline void itv_lincons_reduce_integer(itv_internal_t* intern, itv_lincons_t* cons, size_t intdim)
{ ITVFUN(itv_lincons_reduce_integer)(intern,cons,intdim); }
static inline tbool_t itv_lincons_array_reduce(itv_internal_t* intern, itv_lincons_array_t* array, bool meet)
{ return ITVFUN(itv_lincons_array_reduce)(intern,array,meet); }
static inline tbool_t itv_lincons_array_reduce_integer(itv_internal_t* intern, itv_lincons_array_t* array, size_t intdim)
{ return ITVFUN(itv_lincons_array_reduce_integer)(intern,array,intdim); }
#ifdef __cplusplus
}
#endif

#endif
