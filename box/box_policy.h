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

typedef struct box_policy_one_t {
  box_policy_dim_t* p; /* Usage: p[0<=dim<nbdims] */
  size_t nbdims; /* Numbers of dimensions */
} box_policy_one_t;

typedef struct box_policy_t {
  box_policy_one_t* p;
  size_t size;
  bool first;
} box_policy_t;

ap_policy_manager_t* box_policy_manager_alloc(ap_manager_t* man);

ap_policy_t* box_policy_alloc(ap_policy_manager_t* man, ap_funid_t funid, size_t nbdims);
void box_policy_free(ap_policy_manager_t* man, ap_policy_t* policy);
ap_policy_t* box_policy_copy(ap_policy_manager_t* man, ap_policy_t* policy);
void box_policy_fprint(FILE* stdout, ap_policy_manager_t* man, ap_policy_t* policy);
bool box_policy_equal(ap_policy_manager_t* man, ap_policy_t* policy1, ap_policy_t* policy2);

box_t* box_policy_meet(ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode,
		       bool destructive, box_t* a1, box_t* a2);

box_t* box_policy_meet_array(ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode,
			     box_t** tab, size_t size);

box_t* box_policy_meet_lincons_array(ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode,
				     bool destructive,
				     box_t* a,
				     ap_lincons0_array_t* array);
  /* Meet of an box value with a set of constraints */
box_t*
box_policy_meet_tcons_array(ap_policy_manager_t* pman, ap_policy_t* policy, ap_policy_mode_t mode,
			    bool destructive, box_t* a, ap_tcons0_array_t* array);
  /* Meet of an box value with a set of tree expressions constraints */

#ifdef __cplusplus
}
#endif

#endif
