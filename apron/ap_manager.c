/* ************************************************************************* */
/* ap_manager.c: global manager passed to all functions */
/* ************************************************************************* */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ap_manager.h"

const char* ap_name_of_funid[AP_FUNID_SIZE2] = {
  "unkown",
  "copy",
  "free",
  "size",
  "minimize",
  "canonicalize",
  "approximate",
  "is_minimal",
  "is_canonical",
  "print",
  "printdiff",
  "dump",
  "serialize_raw",
  "deserialize_raw",
  "bottom",
  "top",
  "of_box",
  "of_lincons_array",
  "dimensiun",
  "is_bottom",
  "is_top",
  "is_leq",
  "is_eq",
  "is_dimension_unconstrained",
  "sat_interval",
  "sat_lincons",
  "bound_dimension",
  "bound_linexpr",
  "to_box",
  "to_lincons_array",
  "to_generator_array",
  "meet",
  "meet_array",
  "meet_lincons_array",
  "join",
  "join_array",
  "add_ray_array",
  "assign_linexpr",
  "assign_linexpr_array",
  "substitute_linexpr",
  "substitute_linexpr_array",
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
  opt->approx_before = 0;
  opt->approx_after = 0;
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

static inline char* strdup(const char* s)
{
  char* s2;

  s2 = malloc(strlen(s)+1);
  strcpy(s2,s);
  return s2;
}

ap_exclog_t* ap_exc_cons(ap_exc_t exn, 
				ap_funid_t funid,
				const char* msg, 
				ap_exclog_t* tail)
{
  ap_exclog_t* head = (ap_exclog_t*)malloc(sizeof(ap_exclog_t));
  head->exn = exn;
  head->funid = funid;
  head->msg = strdup(msg);
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
  result->flag_exact = tbool_top;
  result->flag_best = tbool_top;
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
ap_manager_t* ap_manager_alloc(char* library, char* version, 
			 void* internal, 
			 void (*internal_free)(void*))
{
  ap_manager_t* man;

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
  if (man->count>1){
    man->count--;
  }
  else {
    if (man->internal != NULL){
      man->internal_free(man->internal);
      man->internal = NULL;
    }
    ap_result_clear(&man->result);
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
{ return man->option.funopt[funid]; }
bool ap_manager_get_abort_if_exception(ap_manager_t* man, ap_exc_t exn)
{ return man->option.abort_if_exception[exn]; }
tbool_t ap_manager_get_flag_exact(ap_manager_t* man)
{ return man->result.flag_exact; }
tbool_t ap_manager_get_flag_best(ap_manager_t* man)
{ return man->result.flag_best; }


void ap_manager_set_funopt(ap_manager_t* man, ap_funid_t funid, ap_funopt_t* funopt)
{ man->option.funopt[funid] = *funopt; }

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
      man->result.flag_exact = man->result.flag_best = tbool_false;
    }
  }
  return;
}



