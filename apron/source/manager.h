#line 11 "/udd/bjeannet/dev/commoninterface/manager.nw"
/* ************************************************************************* */
/* manager.h: global manager passed to all functions */
/* ************************************************************************* */

#ifndef _MANAGER_H_
#define _MANAGER_H_

#include <stdlib.h>
#include <stdio.h>

#ifndef HAS_BOOL
#define HAS_BOOL
typedef enum bool {
  false=0,
  true=1
} bool;
#endif

#include "coeff.h"

#line 38 "/udd/bjeannet/dev/commoninterface/manager.nw"
/* ********************************************************************** */
/* I. Types */
/* ********************************************************************** */

/* ====================================================================== */
/* I.O General usage */
/* ====================================================================== */

/* Boolean with a third value */
typedef enum tbool_t {
  tbool_false=0,
  tbool_true=1,
  tbool_top=2,   /* don't know */
} tbool_t;

static inline tbool_t tbool_of_bool(bool a);
static inline tbool_t tbool_of_int(int n);
static inline tbool_t tbool_or(tbool_t a, tbool_t b);
static inline tbool_t tbool_and(tbool_t a, tbool_t b);

/* For serialization */
typedef struct membuf_t {
  void* ptr;
  size_t size;
} membuf_t;

#line 88 "/udd/bjeannet/dev/commoninterface/manager.nw"
/* ====================================================================== */
/* I.1 Identifying functions */
/* ====================================================================== */

typedef enum funid_t {
  FUN_UNKNOWN,
  FUN_COPY,
  FUN_FREE,
  FUN_SIZE,
  FUN_MINIMIZE,
  FUN_CANONICALIZE,
  FUN_APPROXIMATE,
  FUN_IS_MINIMAL,
  FUN_IS_CANONICAL,
  FUN_FPRINT,
  FUN_FPRINTDIFF,
  FUN_FDUMP,
  FUN_SERIALIZE_RAW,
  FUN_DESERIALIZE_RAW,
  FUN_BOTTOM,
  FUN_TOP,
  FUN_OF_BOX,
  FUN_OF_LINCONS_ARRAY,
  FUN_DIMENSION,
  FUN_IS_BOTTOM,
  FUN_IS_TOP,
  FUN_IS_LEQ,
  FUN_IS_EQ,
  FUN_IS_DIMENSION_UNCONSTRAINED,
  FUN_SAT_INTERVAL,
  FUN_SAT_LINCONS,
  FUN_BOUND_DIMENSION,
  FUN_BOUND_LINEXPR,
  FUN_TO_BOX,
  FUN_TO_LINCONS_ARRAY,
  FUN_TO_GENERATOR_ARRAY,
  FUN_MEET,
  FUN_MEET_ARRAY,
  FUN_MEET_LINCONS_ARRAY,
  FUN_JOIN,
  FUN_JOIN_ARRAY,
  FUN_ADD_RAY_ARRAY,
  FUN_ASSIGN_LINEXPR,
  FUN_ASSIGN_LINEXPR_ARRAY,
  FUN_SUBSTITUTE_LINEXPR,
  FUN_SUBSTITUTE_LINEXPR_ARRAY,
  FUN_ADD_DIMENSIONS,
  FUN_REMOVE_DIMENSIONS,
  FUN_PERMUTE_DIMENSIONS,
  FUN_FORGET_ARRAY,
  FUN_EXPAND,
  FUN_FOLD,
  FUN_WIDENING,
  FUN_CLOSURE,
  FUNID_SIZE
} funid_t;

extern const char* name_of_funid[FUNID_SIZE]; 
/* give the name of a function identifier */


#line 159 "/udd/bjeannet/dev/commoninterface/manager.nw"
/* ====================================================================== */
/* I.2 Exceptions */
/* ====================================================================== */

/* Exceptions (public type) */
typedef enum exception_t {
  EXC_NONE,           /* no exception detected */
  EXC_TIMEOUT,        /* timeout detected */
  EXC_OUT_OF_SPACE,   /* out of space detected */
  EXC_OVERFLOW,        /* magnitude overflow detected */
  EXC_INVALID_ARGUMENT, /* invalid arguments */
  EXC_NOT_IMPLEMENTED, /* not implemented */ 
  EXCEPTION_SIZE
} exception_t;

extern const char* name_of_exception[EXCEPTION_SIZE]; 

/* Exception log */
typedef struct exception_log_t {
  exception_t exn;
  funid_t funid;
  char* msg;                   /* dynamically allocated */
  struct exception_log_t* tail;
} exception_log_t;

/* Exceptions and other indications (out) (opaque type) */
typedef struct result_t {
  exception_log_t* exception_log; /* history of exceptions */
  exception_t exn;                /* exception for the last called function */
  tbool_t flag_exact; /* result is mathematically exact or not 
                         or don't know */ 
  tbool_t flag_best;  /* result is best correct approximation or not 
                         or don't know */ 
} result_t;


#line 242 "/udd/bjeannet/dev/commoninterface/manager.nw"
/* ====================================================================== */
/* I.2 Options */
/* ====================================================================== */

/* Option associated to each function (public type) */
typedef struct funopt_t {
  int algorithm;
  /* Algorithm selection:
     - 0 is default algorithm;
     - MAX_INT is most accurate available;
     - MIN_INT is most efficient available;
     - otherwise, no accuracy or speed meaning
  */
  int approx_before;
  int approx_after;
  /* Related to the notion of approximation/reduction. 
     Indicates which kind of "approximation" may be performed on the 
     argument(s) before the algorithm, and on the result delivered 
     by the algorithm. 0 is default behaviour. */
  size_t timeout; /* unit !? */ 
  /* Above the given computation time, the function may abort with the
     exception flag flag_time_out on. 
  */
  size_t max_object_size; /* in abstract object size unit. */
  /* If during the computation, the size of some object reach this limit, the
     function may abort with the exception flag flag_out_of_space on. 
  */
  bool flag_exact_wanted; 
  /* return information about exactitude if possible 
  */
  bool flag_best_wanted;  
  /* return information about best correct approximation if possible 
  */
} funopt_t;
#line 314 "/udd/bjeannet/dev/commoninterface/manager.nw"
/* Options (in) (opaque type) */
typedef struct option_t {
  struct funopt_t funopt[FUNID_SIZE];
  bool abort_if_exception[EXCEPTION_SIZE];
  scalar_discr_t scalar_discr; /* Preferred type for scalars */
} option_t;

#line 337 "/udd/bjeannet/dev/commoninterface/manager.nw"
/* ====================================================================== */
/* I.3 Manager */
/* ====================================================================== */

/* Manager (opaque type) */
typedef struct manager_t {
  char* library;                 /* name of the effective library */
  char* version;                 /* version of the effective library */
  void* internal;                /* library dependent, 
                                    should be different for each thread 
                                    (working space) */
  void* funptr[FUNID_SIZE];     /* Array of function pointers,
                                   initialized by the effective library */
  struct option_t option;       /* Options (in) */
  struct result_t result;       /* Exceptions and other indications (out) */
  void (*internal_free)(void*); /* deallocation function for internal */
  size_t counter;               /* reference counter */
} manager_t;

#line 391 "/udd/bjeannet/dev/commoninterface/manager.nw"
/* ********************************************************************** */
/* II. User Functions */
/* ********************************************************************** */

void manager_clear_exception_log(manager_t* man);
  /* erase the current log of exception */
void manager_free(manager_t* man);
  /* dereference the counter, 
     and possibly free internal field if it is not yet put to NULL */

/* Reading fields */
const char* manager_get_library(manager_t* man);
const char* manager_get_version(manager_t* man);

funopt_t manager_get_funopt(manager_t* man, funid_t funid);
bool manager_get_abort_if_exception(manager_t* man, exception_t exn);

exception_t manager_get_exception(manager_t* man);
  /* Get the last exception raised */
exception_log_t* manager_get_exception_log(manager_t* man);
  /* Get the full log of exception */
tbool_t manager_get_flag_exact(manager_t* man);
tbool_t manager_get_flag_best(manager_t* man);

/* Settings fields */
void funopt_init(funopt_t* fopt);
void manager_set_funopt(manager_t* man, funid_t funid, funopt_t* funopt);
void manager_set_abort_if_exception(manager_t* man, exception_t exn, bool flag);

#line 423 "/udd/bjeannet/dev/commoninterface/manager.nw"
/* ********************************************************************** */
/* III. Implementor Functions */
/* ********************************************************************** */

manager_t* manager_alloc(char* library, char* version,
                         void* internal, 
                         void (*internal_free)(void*));
static inline
manager_t* manager_copy(manager_t* man);
  /* Increment the reference counter and return its argument */
void manager_raise_exception(manager_t* man, 
                             exception_t exn, funid_t funid, const char* msg);
  /* raise an exception and put fiels 
     man->result.flag_exact et man->result.flag_best to
     tbool_false
  */
exception_log_t* exception_cons(exception_t exn, 
                                funid_t funid, const char* msg, 
                                exception_log_t* tail);
void exception_free(exception_log_t* head);

#line 451 "/udd/bjeannet/dev/commoninterface/manager.nw"
/* ********************************************************************** */
/* IV. Definition of previously declared inline functions */
/* ********************************************************************** */

static inline tbool_t tbool_of_int(int n)
{ return ((n) ? tbool_true : tbool_false); }
static inline tbool_t tbool_of_bool(bool a)
  { return ((a) ? tbool_true : tbool_false); }
static inline tbool_t tbool_or(tbool_t a, tbool_t b)
{
  return
    ( (a==tbool_true || b==tbool_true) ? tbool_true :
      ( (a==tbool_top || b==tbool_top) ? tbool_top :
        tbool_false ) );
}
static inline tbool_t tbool_and(tbool_t a, tbool_t b)
{
  return
    ( (a==tbool_false || b==tbool_false) ? tbool_false :
      ( (a==tbool_top || b==tbool_top) ? tbool_top :
        tbool_true ) );
}
static inline
manager_t* manager_copy(manager_t* man)
{ man->counter++; return man; }
#line 479 "/udd/bjeannet/dev/commoninterface/manager.nw"
#endif
