/* ************************************************************************* */
/* manager.h: global manager passed to all functions */
/* ************************************************************************* */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "manager.h"

const char* name_of_funid[FUNID_SIZE] = {
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
  "closure"
};

const char* name_of_exception[EXCEPTION_SIZE] = {
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

void funopt_init(funopt_t* opt)
{
  opt->algorithm = 0;
  opt->approx_before = 0;
  opt->approx_after = 0;
  opt->timeout = 0;
  opt->max_object_size = 0;
  opt->flag_exact_wanted = false;
  opt->flag_best_wanted = false;
}

void option_init(option_t* opt)
{
  funid_t funid;
  exception_t exn;

  for (funid=0; funid<FUNID_SIZE; funid++){
    funopt_init(&opt->funopt[funid]);
  }
  for (exn=0; exn<EXCEPTION_SIZE; exn++){
    opt->abort_if_exception[exn] = true;
  }
  opt->scalar_discr = SCALAR_DOUBLE;
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

exception_log_t* exception_cons(exception_t exn, 
				funid_t funid,
				const char* msg, 
				exception_log_t* tail)
{
  exception_log_t* head = (exception_log_t*)malloc(sizeof(exception_log_t));
  head->exn = exn;
  head->funid = funid;
  head->msg = strdup(msg);
  head->tail = tail;
  return head;
}

void exception_free(exception_log_t* head)
{
  exception_log_t* p = head;
  while (p!=NULL) {
    exception_log_t* tail = p->tail;
    free(p->msg);
    free(p);
    p = tail;
  }
}

void result_add_exception(result_t* result, exception_t exn, funid_t funid, const char* msg)
{
  result->exception_log = exception_cons(exn,funid,msg,result->exception_log);
  result->exn = exn;
}

void result_init(result_t* result)
{
  result->exception_log = NULL;
  result->exn = EXC_NONE;
  result->flag_exact = tbool_top;
  result->flag_best = tbool_top;
}
void result_clear(result_t* result)
{
  exception_free(result->exception_log);
  result_init(result);
}

/* ********************************************************************** */
/* III. Constructor and destructor for manager */
/* ********************************************************************** */

/* Constructor and destructor for manager */
manager_t* manager_alloc(char* library, char* version, 
			 void* internal, 
			 void (*internal_free)(void*))
{
  manager_t* man;

  man = (manager_t*)malloc(sizeof(manager_t));
  man->library = library;
  man->version = version;
  man->internal = internal;
  man->internal_free = internal_free;
  man->refcounter = 1;
  option_init(&man->option);
  result_init(&man->result);
  return man;
}
void manager_free(manager_t* man)
{
  if (man->refcounter>1){
    man->refcounter--;
  }
  else {
    if (man->internal != NULL){
      man->internal_free(man->internal);
      man->internal = NULL;
    }
    result_clear(&man->result);
    free(man);
  }
}


/* ********************************************************************** */
/* IV. Other User functions */
/* ********************************************************************** */

const char* manager_get_library(manager_t* man)
{ return man->library; }
const char* manager_get_version(manager_t* man)
{ return man->version; }
funopt_t manager_get_funopt(manager_t* man, funid_t funid)
{ return man->option.funopt[funid]; }
bool manager_get_abort_if_exception(manager_t* man, exception_t exn)
{ return man->option.abort_if_exception[exn]; }
tbool_t manager_get_flag_exact(manager_t* man)
{ return man->result.flag_exact; }
tbool_t manager_get_flag_best(manager_t* man)
{ return man->result.flag_best; }


void manager_set_funopt(manager_t* man, funid_t funid, funopt_t* funopt)
{ man->option.funopt[funid] = *funopt; }

void manager_set_abort_if_exception(manager_t* man, exception_t exn, bool flag)
{ man->option.abort_if_exception[exn] = flag; }
void manager_clear_exception_log(manager_t* man)
{
  exception_free(man->result.exception_log);
  man->result.exception_log = NULL;
}  

/* ********************************************************************** */
/* V. Other Implementor functions */
/* ********************************************************************** */

void manager_raise_exception(manager_t* man, 
			     exception_t exn, 
			     funid_t funid, 
			     const char* msg)
{
  bool pabort;

  if (exn!=EXC_NONE){
    pabort = man->option.abort_if_exception[exn];
    if (pabort){
      fprintf(stderr,"Abort because of following exception:\nexception %s in function %s:\n%s\n",
	      name_of_exception[exn], name_of_funid[funid],
	      msg);
      abort();
    }
    else {
      result_add_exception(&man->result,exn,funid,msg);
      man->result.flag_exact = man->result.flag_best = tbool_false;
    }
  }
  return;
}



