/* ************************************************************************* */
/* ap_policy.h */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#ifndef _AP_POLICY_H_
#define _AP_POLICY_H_

#include "ap_manager.h"
#include "ap_abstract0.h"
#include "ap_abstract1.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* Types */
/* ********************************************************************** */

typedef enum ap_funpolicyid_t {
  AP_FUNPOLICYID_UNKNOWN,
  AP_FUNPOLICYID_COPY,
  AP_FUNPOLICYID_FREE,
  AP_FUNPOLICYID_FPRINT,
  AP_FUNPOLICYID_SPRINT,
  AP_FUNPOLICYID_DIMENSION,
  AP_FUNPOLICYID_EQUAL,
  AP_FUNPOLICYID_HASH,
  AP_FUNPOLICYID_MEET_APPLY,
  AP_FUNPOLICYID_MEET_ARRAY_APPLY,
  AP_FUNPOLICYID_MEET_LINCONS_ARRAY_APPLY,
  AP_FUNPOLICYID_MEET_TCONS_ARRAY_APPLY,
  AP_FUNPOLICYID_MEET_IMPROVE,
  AP_FUNPOLICYID_MEET_ARRAY_IMPROVE,
  AP_FUNPOLICYID_MEET_LINCONS_ARRAY_IMPROVE,
  AP_FUNPOLICYID_MEET_TCONS_ARRAY_IMPROVE,
  AP_FUNPOLICYID_SIZE} ap_funpolicyid_t;

typedef struct ap_policy_manager_t {
  ap_manager_t* man;
  /* Standard manager */
  void* internal;
    /* library dependent working space, should be different for each
       thread */
  void (*internal_free)(void*);
    /* deallocation function for internal */
  void* funptr[AP_FUNPOLICYID_SIZE];
    /* Array of function pointers, initialized by the effective library */
  size_t count;
} ap_policy_manager_t;

typedef struct ap_policy_t {
  void* value; /* Abstract policy of the underlying library */
  ap_policy_manager_t* pman; /* There for dynamic typing and GC purposes */
} ap_policy_t;

ap_policy_manager_t* ap_policy_manager_alloc(ap_manager_t* man,
					     void* internal,
					     void (*internal_free)(void*));
  /* For implementors */

/* ********************************************************************** */
/* I. Policy managers */
/* ********************************************************************** */

void ap_policy_manager_free(ap_policy_manager_t* pman);
  /* dereference the counter, and possibly free internal field if it is not
     yet put to NULL */
static inline
ap_policy_manager_t* ap_policy_manager_copy(ap_policy_manager_t* pman);
  /* Increment the reference counter and return its argument */

/* ********************************************************************** */
/* II. Policy, general */
/* ********************************************************************** */

void ap_policy_free(ap_policy_manager_t* pman, ap_policy_t* policy);
ap_policy_t* ap_policy_copy(ap_policy_manager_t* pman, ap_policy_t* policy);
void ap_policy_fprint(FILE* name, ap_policy_manager_t* pman, ap_policy_t* policy);
char* ap_policy_sprint(ap_policy_manager_t* pman, ap_policy_t* policy);
bool ap_policy_equal(ap_policy_manager_t* pman, ap_policy_t* policy1, ap_policy_t* policy2);
long ap_policy_hash(ap_policy_manager_t* pman, ap_policy_t* policy);
size_t ap_policy_dimension(ap_policy_manager_t* pman, ap_policy_t* policy);

/* ********************************************************************** */
/* III. Policy, level 0 */
/* ********************************************************************** */

ap_abstract0_t*
ap_abstract0_policy_meet_apply(ap_policy_manager_t* pman,
			       ap_policy_t* policy,
			       bool destructive, ap_abstract0_t* a1, ap_abstract0_t* a2);
ap_abstract0_t*
ap_abstract0_policy_meet_array_apply(ap_policy_manager_t* pman,
				     ap_policy_t* policy,
				     ap_abstract0_t** tab, size_t size);
ap_abstract0_t*
ap_abstract0_policy_meet_lincons_array_apply(ap_policy_manager_t* pman,
					     ap_policy_t* policy,
					     bool destructive, ap_abstract0_t* a, ap_lincons0_array_t* array);
ap_abstract0_t*
ap_abstract0_policy_meet_tcons_array_apply(ap_policy_manager_t* pman,
					   ap_policy_t* policy,
					   bool destructive, ap_abstract0_t* a, ap_tcons0_array_t* array);

ap_policy_t*
ap_abstract0_policy_meet_improve(ap_policy_manager_t* pman,
				 ap_policy_t* policy,
				 ap_abstract0_t* a1, ap_abstract0_t* a2);
ap_policy_t*
ap_abstract0_policy_meet_array_improve(ap_policy_manager_t* pman,
				       ap_policy_t* policy,
				       ap_abstract0_t** tab, size_t size);
ap_policy_t*
ap_abstract0_policy_meet_lincons_array_improve(ap_policy_manager_t* pman,
					       ap_policy_t* policy,
					       ap_abstract0_t* a, ap_lincons0_array_t* array);
ap_policy_t*
ap_abstract0_policy_meet_tcons_array_improve(ap_policy_manager_t* pman,
					     ap_policy_t* policy,
					     ap_abstract0_t* a, ap_tcons0_array_t* array);

/* ********************************************************************** */
/* IV. Policy, level 1 */
/* ********************************************************************** */

ap_abstract1_t
ap_abstract1_policy_meet_apply(ap_policy_manager_t* pman,
			       ap_policy_t* policy,
			       bool destructive, ap_abstract1_t* a1, ap_abstract1_t* a2);
ap_abstract1_t
ap_abstract1_policy_meet_array_apply(ap_policy_manager_t* pman,
				     ap_policy_t* policy,
				     ap_abstract1_t* tab, size_t size);
ap_abstract1_t
ap_abstract1_policy_meet_lincons_array_apply(ap_policy_manager_t* pman,
					     ap_policy_t* policy,
					     bool destructive,
					     ap_abstract1_t* a,
					     ap_lincons1_array_t* array);
ap_abstract1_t
ap_abstract1_policy_meet_tcons_array_apply(ap_policy_manager_t* pman,
					   ap_policy_t* policy,
					   bool destructive, ap_abstract1_t* a, ap_tcons1_array_t* array);

ap_policy_t*
ap_abstract1_policy_meet_improve(ap_policy_manager_t* pman,
				 ap_policy_t* policy,
				 ap_abstract1_t* a1, ap_abstract1_t* a2);
ap_policy_t*
ap_abstract1_policy_meet_array_improve(ap_policy_manager_t* pman,
				       ap_policy_t* policy,
				       ap_abstract1_t* tab, size_t size);
ap_policy_t*
ap_abstract1_policy_meet_lincons_array_improve(ap_policy_manager_t* pman,
					       ap_policy_t* policy,
					       ap_abstract1_t* a, ap_lincons1_array_t* array);
ap_policy_t*
ap_abstract1_policy_meet_tcons_array_improve(ap_policy_manager_t* pman,
					     ap_policy_t* policy,
					     ap_abstract1_t* a, ap_tcons1_array_t* array);


/* ********************************************************************** */
/* Definition of inline functions */
/* ********************************************************************** */
static inline
ap_policy_manager_t* ap_policy_manager_copy(ap_policy_manager_t* pman)
{
  pman->count++;
  return pman;
}
static inline
ap_policy_t* ap_policy_cons(ap_policy_manager_t* pman, ap_policy_t* oldpolicy, void* newvalue)
{
  (void)oldpolicy;
  if (newvalue==NULL){
    return NULL;
  } else {
    ap_policy_t* res = (ap_policy_t*)malloc(sizeof(ap_policy_t));
    res->value = newvalue;
    res->pman = ap_policy_manager_copy(pman);
    return res;
  }
}

#ifdef __cplusplus
}
#endif

#endif // _AP_POLICY_H_
