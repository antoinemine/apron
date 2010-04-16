/* ************************************************************************* */
/* ap_abstract0.h: generic operations on numerical abstract values */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#ifndef _AP_POLICY_H_
#define _AP_POLICY_H_

#include "ap_manager.h"
#include "ap_lincons0.h"
#include "ap_tcons0.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ap_policy_t {
  void* value;       /* Abstract value of the underlying library */
  char* library;     /* name of the effective library */
} ap_policy_t;

typedef enum ap_funpolicyid_t {
  AP_FUNPOLICYID_UNKNOWN,
  AP_FUNPOLICYID_CREATE,
  AP_FUNPOLICYID_FREE_ALL,

  AP_FUNPOLICYID_COPY,
  AP_FUNPOLICYID_FREE,
  AP_FUNPOLICYID_PRINTF,
  AP_FUNPOLICYID_MEET,
  AP_FUNPOLICYID_MEET_ARRAY,
  AP_FUNPOLICYID_MEET_LINCONS_ARRAY,
  AP_FUNPOLICYID_MEET_TCONS_ARRAY,
  AP_FUNPOLICYID_ASSIGN_LINEXPR_ARRAY,
  AP_FUNPOLICYID_ASSIGN_TEXPR_ARRAY,
  AP_FUNPOLICYID_SIZE
} ap_funpolicyid_t;

typedef void (*funptr_t)(void*);
void ap_result_clear(ap_result_t* result);
void ap_option_init(ap_option_t* opt);
void ap_result_init(ap_result_t* result);

typedef enum ap_policy_mode_t { AP_POLICY_APPLY, AP_POLICY_CHANGE } ap_policy_mode_t;

typedef struct _ap_current_policy_t {
   void** policies;
   size_t index;
   size_t size;
   ap_policy_mode_t mode;
} ap_current_policy_t;

typedef struct ap_policy_manager_t {
  void* internal;                /* library dependent, should be different for each thread (working space) */
  void* funpolicy_array[AP_FUNPOLICYID_SIZE]; /* Array of function pointers, initialized by the effective library */
  void (*internal_free)(void*);  /* deallocation function for internal */
  size_t count;
} ap_policy_manager_t;

void ap_policy_manager_free(ap_policy_manager_t* man);
  /* dereference the counter, and possibly free internal field if it is not yet put to NULL */
ap_policy_manager_t* ap_policy_manager_alloc(void* internal, void (*internal_free)(void*));

static inline ap_policy_manager_t* ap_policy_manager_copy(ap_policy_manager_t* man)
  { man->count++; return man; }
  /* Increment the reference counter and return its argument */

#if 0
typedef struct ap_policy_manager_t {
   ap_manager_t standard_manager;
   char ident; /* = 0x5a */
   funptr_t funpolicy_array[AP_FUNPOLICYID_SIZE];
   /* mutable */ ap_current_policy_t current_policy;
} ap_policy_manager_t;

extern ap_policy_manager_t _ap_policy_manager_instance;

static inline
ap_policy_manager_t* as_policy_manager(ap_manager_t* standard_manager)
{  // shift may be different from 0 for alignment reasons: shift is the alignment difference in bytes
   int shift = (void*) &_ap_policy_manager_instance - (void*) (&_ap_policy_manager_instance.standard_manager);
   ap_policy_manager_t* result = (ap_policy_manager_t*) (((void*) standard_manager) + shift);
   assert(result->ident == 0x5a);
   return result;
}

void* transfer_policy_meet(ap_manager_t* standard_manager, bool destructive, ap_scalar_t* a1, ap_scalar_t* a2);
void* transfer_policy_meet_lincons_array(ap_manager_t* standard_manager, bool destructive, ap_scalar_t* a, ap_lincons0_array_t* array);
void* transfer_policy_meet_array(ap_manager_t* standard_manager, ap_scalar_t** tab, size_t size);
void* transfer_policy_meet_tcons_array(ap_manager_t* standard_manager, bool destructive, ap_scalar_t* a, ap_tcons0_array_t* array);
#endif

/*
static inline void ap_policy_free(ap_policy_t* policy);

static inline void ap_policy_clear(ap_policy_t* policy)
{
   policy->free(policy->value);
   policy->value = NULL;
   policy->library = NULL;
}
static inline void ap_policy_free(ap_policy_t* policy)
{
  ap_policy_clear(policy);
  free(policy);
}
*/

static inline
void ap_set_initial_policy(ap_policy_manager_t* manager, ap_current_policy_t* current_policy, int dimension, int nb_policies) {
   typedef void (*fun_policy_create_t)(ap_policy_manager_t* man, ap_current_policy_t* policy, size_t dimension, size_t nb_policies);
   fun_policy_create_t fun_create = (fun_policy_create_t) manager->funpolicy_array[AP_FUNPOLICYID_CREATE];
   (*fun_create)(manager, current_policy, dimension, nb_policies);
}

static inline
void ap_free_policy(ap_policy_manager_t* manager, ap_current_policy_t* current_policy) {
   typedef void* (*fun_policy_free_all_t)(ap_policy_manager_t* man, ap_current_policy_t* policy);
   fun_policy_free_all_t fun_free_all = (fun_policy_free_all_t) manager->funpolicy_array[AP_FUNPOLICYID_FREE_ALL];
   (*fun_free_all)(manager, current_policy);
}

#if 0
int ap_policy_get_index(ap_policy_manager_t* man);
void* ap_policy_set_change_index(ap_policy_manager_t* man, int index);
void* ap_policy_set_apply_index(ap_policy_manager_t* man, int index);
#endif

ap_policy_manager_t* box_policy_manager_alloc(void);
/*
void ap_policy_manager_free(ap_policy_manager_t* man);
*/

#ifdef __cplusplus
}
#endif

#endif // _AP_POLICY_H_
