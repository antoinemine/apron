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

typedef char box_policy_choice_t;
#define BOX_POLICY_1 0
#define BOX_POLICY_2 1
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
  size_t nbdims;
} box_policy_t;

ap_policy_manager_t* box_policy_manager_alloc(ap_manager_t* man);

void box_policy_free(ap_policy_manager_t* man, box_policy_t* policy);
box_policy_t* box_policy_copy(ap_policy_manager_t* man, box_policy_t* policy);
void box_policy_fprint(FILE* stdoutput, ap_policy_manager_t* man, box_policy_t* policy);
char* box_policy_sprint(ap_policy_manager_t* man, box_policy_t* policy);
size_t box_policy_dimension(ap_policy_manager_t* man, box_policy_t* policy);
bool box_policy_equal(ap_policy_manager_t* man, box_policy_t* policy1, box_policy_t* policy2);
long box_policy_hash(ap_policy_manager_t* man, box_policy_t* policy);

box_t* box_policy_meet_apply(ap_policy_manager_t* pman,
			     box_policy_t* policy,
			     bool destructive, box_t* a1, box_t* a2);
box_t* box_policy_meet_array_apply(ap_policy_manager_t* pman,
				   box_policy_t* policy,
				   box_t** tab, size_t size);
box_t* box_policy_meet_lincons_array_apply(ap_policy_manager_t* pman,
					   box_policy_t* policy,
					   bool destructive,
					   box_t* a,
					   ap_lincons0_array_t* array);
box_t* box_policy_meet_tcons_array_apply(ap_policy_manager_t* pman,
					 box_policy_t* policy,
					 bool destructive, box_t* a, ap_tcons0_array_t* array);

box_policy_t* box_policy_meet_improve(
    ap_policy_manager_t* pman,
    box_policy_t* policy,
    box_t* a1, box_t* a2);
box_policy_t* box_policy_meet_array_improve(
    ap_policy_manager_t* pman,
    box_policy_t* policy,
    box_t** tab, size_t size);
box_policy_t* box_policy_meet_lincons_array_improve(
    ap_policy_manager_t* pman,
    box_policy_t* policy,
    box_t* a, ap_lincons0_array_t* array);
box_policy_t* box_policy_meet_tcons_array_improve(
    ap_policy_manager_t* pman,
    box_policy_t* policy,
    box_t* a, ap_tcons0_array_t* array);

#ifdef __cplusplus
}
#endif

#endif
