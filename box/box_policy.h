/* ********************************************************************** */
/* box.h: abstract lattice of intervals */
/* ********************************************************************** */

#ifndef _BOX_POLICY_H_
#define _BOX_POLICY_H_

#include <stdio.h>
#include "ap_global0.h"
#include "ap_policy.h"
#include "box.h"

#ifdef __cplusplus
extern "C" {
#endif
  
ap_policy_manager_t* box_policy_manager_alloc(void);

typedef enum box_policy_choice_t { 
  BOX_POLICY_1, 
  BOX_POLICY_2 
} box_policy_choice_t;
  /* 1: the bound of the first argument is chosen, 
     2: the bound of the second argument is chosen */

typedef struct box_policy_dim_t {
  box_policy_choice_t inf;
  box_policy_choice_t sup;
} box_policy_dim_t;

/* typedef struct box_policy_one_t */
typedef struct box_policy_t {
  box_policy_dim_t* p;
  size_t size;
} box_policy_t;

/*
typedef struct box_policy_t {
  box_policy_one_t* p;
  size_t size;
} box_policy_t;
*/

void box_policy_init(box_policy_t* policy /*, int size */, size_t dimension);
void box_policy_clear(void* policy);
box_policy_t* box_policy_alloc(/* int size, */ int dimension);
void box_policy_free(void* policy);
void* box_policy_copy(void* policy);

void box_policy_printf(FILE* stdout, void* a_policy);

box_t* box_meet_policy(ap_manager_t* man, ap_policy_manager_t* policy_man, ap_current_policy_t* policy, 
		       bool destructive, box_t* a1, box_t* a2);

box_t* box_meet_array_policy(ap_manager_t* man, ap_policy_manager_t* policy_man, ap_current_policy_t* policy, 
			     box_t** tab, size_t size);

box_t* box_meet_lincons_array_policy(ap_manager_t* man, ap_policy_manager_t* policy_man, ap_current_policy_t* policy,
				     bool destructive,
				     box_t* a,
				     ap_lincons0_array_t* array);
  /* Meet of an box value with a set of constraints */

box_t*
box_meet_tcons_array_policy(ap_manager_t* man, ap_policy_manager_t* policy_man, ap_current_policy_t* policy,
			    bool destructive, box_t* a, ap_tcons0_array_t* array);
  /* Meet of an box value with a set of tree expressions constraints */

void box_policy_create(ap_policy_manager_t* man, ap_current_policy_t* current_policy, size_t dimension, size_t nb_policies);
void box_policy_free_all(ap_policy_manager_t* man, ap_current_policy_t* current_policy);

box_t*
box_assign_linexpr_array_policy(ap_manager_t* man, ap_policy_manager_t* policy_man, ap_current_policy_t* policy,
    bool destructive, box_t* a, ap_dim_t* tdim, ap_linexpr0_t** texpr, size_t size, box_t* dest);
box_t*
box_assign_texpr_array_policy(ap_manager_t* man, ap_policy_manager_t* policy_man, ap_current_policy_t* policy,
    bool destructive, box_t* a, ap_dim_t* tdim, ap_texpr0_t** texpr, size_t size, box_t* dest);

#ifdef __cplusplus
}
#endif

#endif
