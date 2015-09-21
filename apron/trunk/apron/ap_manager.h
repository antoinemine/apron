/* ************************************************************************* */
/* manager.h: global manager passed to all functions */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#ifndef _AP_MANAGER_H_
#define _AP_MANAGER_H_

#include <stdlib.h>
#include <stdio.h>

#include "ap_coeff.h"

#ifdef __cplusplus
extern "C" {
#endif


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
static inline tbool_t tbool_not(tbool_t a);

/* For serialization */
typedef struct ap_membuf_t {
  void* ptr;
  size_t size;
} ap_membuf_t;

/* ====================================================================== */
/* I.1 Identifying functions */
/* ====================================================================== */

typedef enum ap_funid_t {
  AP_FUNID_UNKNOWN,
  AP_FUNID_COPY,
  AP_FUNID_FREE,
  AP_FUNID_ASIZE, /* For avoiding name conflict with AP_FUNID_SIZE */
  AP_FUNID_MINIMIZE,
  AP_FUNID_CANONICALIZE,
  AP_FUNID_HASH,
  AP_FUNID_APPROXIMATE,
  AP_FUNID_FPRINT,
  AP_FUNID_FPRINTDIFF,
  AP_FUNID_FDUMP,
  AP_FUNID_SERIALIZE_RAW,
  AP_FUNID_DESERIALIZE_RAW,
  AP_FUNID_BOTTOM,
  AP_FUNID_TOP,
  AP_FUNID_OF_BOX,
  AP_FUNID_DIMENSION,
  AP_FUNID_IS_BOTTOM,
  AP_FUNID_IS_TOP,
  AP_FUNID_IS_LEQ,
  AP_FUNID_IS_EQ,
  AP_FUNID_IS_DIMENSION_UNCONSTRAINED,
  AP_FUNID_SAT_INTERVAL,
  AP_FUNID_SAT_LINCONS,
  AP_FUNID_SAT_TCONS,
  AP_FUNID_BOUND_DIMENSION,
  AP_FUNID_BOUND_LINEXPR,
  AP_FUNID_BOUND_TEXPR,
  AP_FUNID_TO_BOX,
  AP_FUNID_TO_LINCONS_ARRAY,
  AP_FUNID_TO_TCONS_ARRAY,
  AP_FUNID_TO_GENERATOR_ARRAY,
  AP_FUNID_MEET,
  AP_FUNID_MEET_ARRAY,
  AP_FUNID_MEET_LINCONS_ARRAY,
  AP_FUNID_MEET_TCONS_ARRAY,
  AP_FUNID_JOIN,
  AP_FUNID_JOIN_ARRAY,
  AP_FUNID_ADD_RAY_ARRAY,
  AP_FUNID_ASSIGN_LINEXPR_ARRAY,
  AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY,
  AP_FUNID_ASSIGN_TEXPR_ARRAY,
  AP_FUNID_SUBSTITUTE_TEXPR_ARRAY,
  AP_FUNID_ADD_DIMENSIONS,
  AP_FUNID_REMOVE_DIMENSIONS,
  AP_FUNID_PERMUTE_DIMENSIONS,
  AP_FUNID_FORGET_ARRAY,
  AP_FUNID_EXPAND,
  AP_FUNID_FOLD,
  AP_FUNID_WIDENING,
  AP_FUNID_CLOSURE,
  AP_FUNID_SIZE,
  AP_FUNID_CHANGE_ENVIRONMENT,
  AP_FUNID_RENAME_ARRAY,
  AP_FUNID_SIZE2
} ap_funid_t;

extern const char* ap_name_of_funid[AP_FUNID_SIZE2];
/* give the name of a function identifier */


/* ====================================================================== */
/* I.2 Exceptions */
/* ====================================================================== */

/* Exceptions (public type) */
typedef enum ap_exc_t {
  AP_EXC_NONE,             /* no exception detected */
  AP_EXC_TIMEOUT,          /* timeout detected */
  AP_EXC_OUT_OF_SPACE,     /* out of space detected */
  AP_EXC_OVERFLOW,         /* magnitude overflow detected */
  AP_EXC_INVALID_ARGUMENT, /* invalid arguments */
  AP_EXC_NOT_IMPLEMENTED,  /* not implemented */
  AP_EXC_SIZE
} ap_exc_t;

extern const char* ap_name_of_exception[AP_EXC_SIZE];

/* Exception log */
typedef struct ap_exclog_t {
  ap_exc_t exn;
  ap_funid_t funid;
  char* msg;                   /* dynamically allocated */
  struct ap_exclog_t* tail;
} ap_exclog_t;

/* Exceptions and other indications (out) (opaque type) */
typedef struct ap_result_t {
  ap_exclog_t* exclog; /* history of exceptions */
  ap_exc_t exn;        /* exception for the last called function */
  bool flag_exact;  /* result is mathematically exact or don't know */
  bool flag_best;   /* result is best correct approximation or don't know */
} ap_result_t;


/* ====================================================================== */
/* I.2 Options */
/* ====================================================================== */

/* Option associated to each function (public type) */
typedef struct ap_funopt_t {
  int algorithm;
  /* Algorithm selection:
     - 0 is default algorithm;
     - MAX_INT is most accurate available;
     - MIN_INT is most efficient available;
     - otherwise, no accuracy or speed meaning
  */
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
} ap_funopt_t;

/* Options (in) (opaque type) */
typedef struct ap_option_t {
  ap_funopt_t funopt[AP_FUNID_SIZE];
  bool abort_if_exception[AP_EXC_SIZE];
  ap_scalar_discr_t scalar_discr; /* Preferred type for scalars */
} ap_option_t;

/* ====================================================================== */
/* I.3 Manager */
/* ====================================================================== */

/* Manager (opaque type) */
typedef struct ap_manager_t {
  const char* library;                 /* name of the effective library */
  const char* version;                 /* version of the effective library */
  void* internal;                /* library dependent,
				    should be different for each thread
				    (working space) */
  void* funptr[AP_FUNID_SIZE];     /* Array of function pointers,
				   initialized by the effective library */
  ap_option_t option;            /* Options (in) */
  ap_result_t result;            /* Exceptions and other indications (out) */
  void (*internal_free)(void*);  /* deallocation function for internal */
  size_t count;                  /* reference counter */
} ap_manager_t;

/* ********************************************************************** */
/* II. User Functions */
/* ********************************************************************** */

void ap_manager_clear_exclog(ap_manager_t* man);
  /* erase the current log of exception */
void ap_manager_free(ap_manager_t* man);
  /* dereference the counter,
     and possibly free internal field if it is not yet put to NULL */

/* Reading fields */
const char* ap_manager_get_library(ap_manager_t* man);
const char* ap_manager_get_version(ap_manager_t* man);

ap_funopt_t ap_manager_get_funopt(ap_manager_t* man, ap_funid_t funid);
bool ap_manager_get_abort_if_exception(ap_manager_t* man, ap_exc_t exn);

ap_exc_t ap_manager_get_exception(ap_manager_t* man);
  /* Get the last exception raised */
ap_exclog_t* ap_manager_get_exclog(ap_manager_t* man);
  /* Get the full log of exception */
bool ap_manager_get_flag_exact(ap_manager_t* man);
bool ap_manager_get_flag_best(ap_manager_t* man);

/* Settings fields */
void ap_funopt_init(ap_funopt_t* fopt);
void ap_manager_set_funopt(ap_manager_t* man, ap_funid_t funid, ap_funopt_t* funopt);
void ap_manager_set_abort_if_exception(ap_manager_t* man, ap_exc_t exn, bool flag);

bool ap_fpu_init(void);
/* tries to set the FPU rounding-mode towards +oo, returns true if successful */


/* ********************************************************************** */
/* III. Implementor Functions */
/* ********************************************************************** */

ap_manager_t* ap_manager_alloc(const char* library, const char* version,
			       void* internal,
			       void (*internal_free)(void*));
static inline
ap_manager_t* ap_manager_copy(ap_manager_t* man);
  /* Increment the reference counter and return its argument */
void ap_manager_raise_exception(ap_manager_t* man,
				ap_exc_t exn, ap_funid_t funid, const char* msg);
  /* raise an exception and put fields
     man->result.flag_exact and man->result.flag_best to
     false
  */
ap_exclog_t* ap_exc_cons(ap_exc_t exn,
			 ap_funid_t funid, const char* msg,
			 ap_exclog_t* tail);
void ap_exclog_free(ap_exclog_t* head);

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
static inline tbool_t tbool_not(tbool_t a)
{
  return
    a==tbool_false ? tbool_true :
    a==tbool_true  ? tbool_false : a;
}
static inline
ap_manager_t* ap_manager_copy(ap_manager_t* man)
{ man->count++; return man; }
#ifdef __cplusplus
}
#endif

#endif
