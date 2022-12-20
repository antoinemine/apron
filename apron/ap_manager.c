/* ************************************************************************* */
/* ap_manager.c: global manager passed to all functions */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "ap_manager.h"

const char* ap_name_of_funid[AP_FUNID_SIZE2] = {
  "unknown",
  "copy",
  "free",
  "size",
  "minimize",
  "canonicalize",
  "hash",
  "approximate",
  "print",
  "printdiff",
  "dump",
  "serialize_raw",
  "deserialize_raw",
  "bottom",
  "top",
  "of_box",
  "dimension",
  "is_bottom",
  "is_top",
  "is_leq",
  "is_eq",
  "is_dimension_unconstrained",
  "sat_interval",
  "sat_lincons",
  "sat_tcons",
  "bound_dimension",
  "bound_linexpr",
  "bound_texpr",
  "to_box",
  "to_lincons_array",
  "to_tcons_array",
  "to_generator_array",
  "meet",
  "meet_array",
  "meet_lincons_array",
  "meet_tcons_array",
  "join",
  "join_array",
  "add_ray_array",
  "assign_linexpr_array",
  "substitute_linexpr_array",
  "assign_texpr_array",
  "substitute_texpr_array",
  "add_dimensions",
  "remove_dimensions",
  "permute_dimensions",
  "forget_array",
  "expand",
  "fold",
  "widening",
  "closure",
  "unknown",
  "change_environment",
  "rename"
};

const char* ap_name_of_exception[AP_EXC_SIZE] = {
  "NONE",
  "TIMEOUT",
  "OUT_OF_SPACE",
  "OVERFLOW",
  "INVALID_ARGUMENT",
  "NOT_IMPLEMENTED"
};

/* ********************************************************************** */
/* I. Constructor and destructor for options */
/* ********************************************************************** */

void ap_funopt_init(ap_funopt_t* opt)
{
  opt->algorithm = 0;
  opt->timeout = 0;
  opt->max_object_size = 0;
  opt->flag_exact_wanted = false;
  opt->flag_best_wanted = false;
}

void ap_option_init(ap_option_t* opt)
{
  ap_funid_t funid;
  ap_exc_t exn;

  for (funid=0; funid<AP_FUNID_SIZE; funid++){
    ap_funopt_init(&opt->funopt[funid]);
  }
  for (exn=0; exn<AP_EXC_SIZE; exn++){
    opt->abort_if_exception[exn] = true;
  }
  opt->scalar_discr = AP_SCALAR_DOUBLE;
}

/* ********************************************************************** */
/* II. Constructor and destructor for result */
/* ********************************************************************** */

ap_exclog_t* ap_exc_cons(ap_exc_t exn,
			 ap_funid_t funid,
			 const char* msg,
			 ap_exclog_t* tail)
{
  ap_exclog_t* head = (ap_exclog_t*)malloc(sizeof(ap_exclog_t));
  head->exn = exn;
  head->funid = funid;
  head->msg = strdup(msg ? msg : "");
  head->tail = tail;
  return head;
}

void ap_exclog_free(ap_exclog_t* head)
{
  ap_exclog_t* p = head;
  while (p!=NULL) {
    ap_exclog_t* tail = p->tail;
    free(p->msg);
    free(p);
    p = tail;
  }
}

void ap_result_add_exception(ap_result_t* result, ap_exc_t exn, ap_funid_t funid, const char* msg)
{
  result->exclog = ap_exc_cons(exn,funid,msg,result->exclog);
  result->exn = exn;
}

void ap_result_init(ap_result_t* result)
{
  result->exclog = NULL;
  result->exn = AP_EXC_NONE;
  result->flag_exact = false;
  result->flag_best = false;
}
void ap_result_clear(ap_result_t* result)
{
  ap_exclog_free(result->exclog);
  ap_result_init(result);
}

/* ********************************************************************** */
/* III. Constructor and destructor for manager */
/* ********************************************************************** */

/* Constructor and destructor for manager */
ap_manager_t* ap_manager_alloc(const char* library, const char* version,
			       void* internal,
			       void (*internal_free)(void*))
{
  ap_manager_t* man;

  assert(sizeof(bool)==1);

  man = (ap_manager_t*)malloc(sizeof(ap_manager_t));
  man->library = library;
  man->version = version;
  man->internal = internal;
  man->internal_free = internal_free;
  man->count = 1;
  ap_option_init(&man->option);
  ap_result_init(&man->result);
  return man;
}
void ap_manager_free(ap_manager_t* man)
{
  assert(man->count>=1);
  if (man->count>1){
    man->count--;
  }
  else {
    if (man->internal != NULL){
      man->internal_free(man->internal);
      man->internal = NULL;
    }
    ap_result_clear(&man->result);
    man->count = 0;
    free(man);
  }
}


/* ********************************************************************** */
/* IV. Other User functions */
/* ********************************************************************** */

const char* ap_manager_get_library(ap_manager_t* man)
{ return man->library; }
const char* ap_manager_get_version(ap_manager_t* man)
{ return man->version; }
ap_funopt_t ap_manager_get_funopt(ap_manager_t* man, ap_funid_t funid)
{
  if (funid<AP_FUNID_SIZE)
    return man->option.funopt[funid];
  else {
    fprintf(stderr,"ap_manager.c: ap_manager_get_funopt: funid should be less than AP_FUNID_SIZE\n");
    abort();
  }
}
bool ap_manager_get_abort_if_exception(ap_manager_t* man, ap_exc_t exn)
{ return man->option.abort_if_exception[exn]; }
bool ap_manager_get_flag_exact(ap_manager_t* man)
{ return man->result.flag_exact; }
bool ap_manager_get_flag_best(ap_manager_t* man)
{ return man->result.flag_best; }


void ap_manager_set_funopt(ap_manager_t* man, ap_funid_t funid, ap_funopt_t* funopt)
{ if (funid<AP_FUNID_SIZE) man->option.funopt[funid] = *funopt; }

void ap_manager_set_abort_if_exception(ap_manager_t* man, ap_exc_t exn, bool flag)
{ man->option.abort_if_exception[exn] = flag; }
void ap_manager_clear_exclog(ap_manager_t* man)
{
  ap_exclog_free(man->result.exclog);
  man->result.exclog = NULL;
}

/* ********************************************************************** */
/* V. Other Implementor functions */
/* ********************************************************************** */

void ap_manager_raise_exception(ap_manager_t* man,
				ap_exc_t exn,
				ap_funid_t funid,
				const char* msg)
{
  bool pabort;

  if (exn!=AP_EXC_NONE){
    pabort = man->option.abort_if_exception[exn];
    if (pabort){
      fprintf(stderr,"Apron: Abort because of following exception:\nexception %s in function %s:\n%s\n",
	      ap_name_of_exception[exn], ap_name_of_funid[funid],
	      msg);
      abort();
    }
    else {
      ap_result_add_exception(&man->result,exn,funid,msg);
      man->result.flag_exact = man->result.flag_best = false;
    }
  }
  return;
}


/* ********************************************************************** */
/* V. FPU init */
/* ********************************************************************** */

/* simple run-time test that fpu behaves correctly */
static bool test_fpu(void)
{
  int i;
  long double d = 1., dd;
  /* find the minimal long double, as the fixpoint of x -> x/2 with rounding
     towards +oo;
     the max iteration value should be enough for 128-bit floating-point */
  for (i=0;i<5000000;i++) {
    dd = d;
    d /= 2;
    if (d==dd || d==0.) break;
  }
  /* fails if flush to 0 */
  if (d!=dd) { fprintf(stderr,"test_fpu failed test #1 after %i iterations\n",i); return false; }
  /* fails if long double rounding is not towards +oo */
  if (d*0.25!=dd) { fprintf(stderr,"test_fpu failed test #2\n"); return false; }
  /* fails if double rounding is not towards +oo */
  if ((double)d<dd) { fprintf(stderr,"test_fpu failed test #3\n"); return false; }
  /* fails if float rounding is not towards +oo */
  if ((float)d<dd) { fprintf(stderr,"test_fpu failed test #4\n"); return false; }
  return true;
}

#if defined(__ppc__)
bool ap_fpu_init(void)
{
  __asm volatile ("mtfsfi 7,2");
  return test_fpu();
}

#elif defined(__linux) || defined (__APPLE__) || defined(__FreeBSD__)
#include <fenv.h>
bool ap_fpu_init(void)
{
  if (!fesetround(FE_UPWARD)) return test_fpu();
  fprintf(stderr,"could not set fpu rounding mode: fesetround failed\n");
  return false;
}

#elif defined(sun)
#include <ieeefp.h>
bool ap_fpu_init(void)
{
  fpsetround(FP_RP);
  return test_fpu();
}

#else
bool ap_fpu_init(void)
{
  fprintf(stderr,"could not set fpu rounding mode: platform not supported\n");
  return false;
}

#endif
