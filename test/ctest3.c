/*
 * ctest3.c
 *
 * Unit testing. Policies
 *
 * Copyright (C) Frank Védrine 2010
 *
 */

/* This file is part of the APRON Library, released under GPL license

   Please read the COPYING file packaged in the distribution.
*/

#include "../apron/ap_global0.h"
#include "../apron/ap_global1.h"

#include "../box/box_policy.h"

int main(int argc, char** argv) {
   ap_manager_t* manager = box_manager_alloc();
   ap_policy_manager_t* policy_manager = box_policy_manager_alloc();
   ap_current_policy_t policy;

   box_policy_t* loop_policy = box_policy_alloc(2);
   box_policy_t* exit_loop_policy = box_policy_alloc(2);

   ap_var_t name_of_dim[2] = {"i","j"};
   ap_environment_t* env = ap_environment_alloc(NULL,0,&name_of_dim[0],2);

   ap_linexpr1_t zero = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)0);
   ap_linexpr1_t ten = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)0);
   ap_linexpr1_t top = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)0);
   ap_linexpr1_t bot = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)0);
   ap_linexpr1_t i = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
   ap_linexpr1_t j = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);

   ap_linexpr1_set_cst_scalar_double(&zero, 0.0);
   ap_linexpr1_set_cst_scalar_double(&ten, 10.0);
   ap_linexpr1_set_coeff_scalar_double(&i, "i", 1.);
   ap_linexpr1_set_coeff_scalar_double(&j, "j", 1.);

   ap_interval_t* itop = ap_interval_alloc();
   ap_interval_set_top(itop);
   ap_linexpr1_set_cst_interval(&top, itop);

   ap_interval_t* ibot = ap_interval_alloc();
   ap_interval_set_bottom(ibot);
   ap_linexpr1_set_cst_interval(&bot, ibot);

   ap_lincons1_array_t array_j_minus_i_greater_or_equal_0 = ap_lincons1_array_make(env, 1);
   /* Transfer function */
   ap_linexpr1_t h_i = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
   ap_linexpr1_set_list(&h_i, AP_COEFF_S_DOUBLE, 1.0,"i", AP_CST_S_DOUBLE, 1.0, AP_END);
   ap_linexpr1_t h_j = ap_linexpr1_make(env, AP_LINEXPR_SPARSE, (size_t)1);
   ap_linexpr1_set_list(&h_j, AP_COEFF_S_DOUBLE, 1.0,"j", AP_CST_S_DOUBLE, -2.0, AP_END);

   ap_linexpr1_t onemE = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)1);
   ap_lincons1_t j_minus_i_greater_or_equal_0 = ap_lincons1_make(AP_CONS_SUPEQ,&onemE,NULL);
   ap_lincons1_set_list(&j_minus_i_greater_or_equal_0, AP_COEFF_S_DOUBLE, -1., "i", AP_COEFF_S_DOUBLE, 1., "j", AP_END);
   ap_lincons1_array_set(&array_j_minus_i_greater_or_equal_0,0,&j_minus_i_greater_or_equal_0);

   ap_set_initial_policy(policy_manager, &policy, 2, 100);

   /* analyse */
   ap_abstract1_t _1 = ap_abstract1_top (manager, env);
   ap_abstract1_t _2 = ap_abstract1_policy_assign_linexpr(manager, policy_manager, &policy, false, &_1, "i", &zero, NULL);
   ap_abstract1_t _3 = ap_abstract1_policy_assign_linexpr(manager, policy_manager, &policy, false, &_2, "j", &ten, NULL);
   ap_abstract1_t _4 = ap_abstract1_policy_meet_lincons_array(manager, policy_manager, &policy, false, &_3, &array_j_minus_i_greater_or_equal_0);

   ap_abstract1_t _5,_6,_7,pivot;
   pivot = ap_abstract1_copy(manager, &_4);
   int iter = 0;
   int loop_policy_index = policy.index;
   while (iter <4) {
      _5 = ap_abstract1_policy_assign_linexpr(manager, policy_manager, &policy, false, &pivot, "i", &h_i, NULL);
      _6 = ap_abstract1_policy_assign_linexpr(manager, policy_manager, &policy, false, &_5, "j", &h_j, NULL);
      _6 = ap_abstract1_join(manager,true,&_6,&_3);
      _7 = ap_abstract1_policy_meet_lincons_array(manager, policy_manager, &policy, false, &_6, &array_j_minus_i_greater_or_equal_0);
      ap_abstract1_clear(manager, &pivot);
      pivot = ap_abstract1_copy(manager, &_7);
      ap_abstract1_clear(manager, &_5);
      ap_abstract1_clear(manager, &_6);
      ap_abstract1_clear(manager, &_7);
      iter++;
      policy.index = loop_policy_index;
      policy.mode = AP_POLICY_APPLY;
   };
   policy.index = loop_policy_index;
   policy.mode = AP_POLICY_CHANGE;
   iter = 0;
   while (iter <4) {
      _5 = ap_abstract1_policy_assign_linexpr(manager, policy_manager, &policy, false, &pivot, "i", &h_i, NULL);
      _6 = ap_abstract1_policy_assign_linexpr(manager, policy_manager, &policy, false, &_5, "j", &h_j, NULL);
      _6 = ap_abstract1_join(manager,true,&_6,&_3);
      _7 = ap_abstract1_policy_meet_lincons_array(manager, policy_manager, &policy, false, &_6, &array_j_minus_i_greater_or_equal_0);
      ap_abstract1_clear(manager, &pivot);
      pivot = ap_abstract1_copy(manager, &_7);
      ap_abstract1_clear(manager, &_5);
      ap_abstract1_clear(manager, &_6);
      ap_abstract1_clear(manager, &_7);
      iter++;
      policy.index = loop_policy_index;
      policy.mode = AP_POLICY_APPLY;
   };
   policy.index = loop_policy_index;
   policy.mode = AP_POLICY_CHANGE;
   iter = 0;
   while (iter <4) {
      _5 = ap_abstract1_policy_assign_linexpr(manager, policy_manager, &policy, false, &pivot, "i", &h_i, NULL);
      _6 = ap_abstract1_policy_assign_linexpr(manager, policy_manager, &policy, false, &_5, "j", &h_j, NULL);
      _6 = ap_abstract1_join(manager,true,&_6,&_3);
      _7 = ap_abstract1_policy_meet_lincons_array(manager, policy_manager, &policy, false, &_6, &array_j_minus_i_greater_or_equal_0);
      ap_abstract1_clear(manager, &pivot);
      pivot = ap_abstract1_copy(manager, &_7);
      ap_abstract1_clear(manager, &_5);
      ap_abstract1_clear(manager, &_6);
      ap_abstract1_clear(manager, &_7);
      iter++;
      policy.index = loop_policy_index;
      policy.mode = AP_POLICY_APPLY;
   };

	ap_lincons1_array_clear(&array_j_minus_i_greater_or_equal_0);
	ap_linexpr1_clear(&zero);
	ap_linexpr1_clear(&top);
	ap_linexpr1_clear(&bot);
	ap_linexpr1_clear(&h_i);
	ap_linexpr1_clear(&h_j);

	ap_abstract1_clear(manager, &_1);
	ap_abstract1_clear(manager, &_2);
	ap_abstract1_clear(manager, &_3);
	ap_abstract1_clear(manager, &_4);
	ap_abstract1_clear(manager, &pivot);

   ap_interval_free(itop);
   ap_interval_free(ibot);

   ap_linexpr1_clear(&zero);
   ap_linexpr1_clear(&ten);
   ap_linexpr1_clear(&top);
   ap_linexpr1_clear(&bot);
   ap_linexpr1_clear(&i);
   ap_linexpr1_clear(&j);

   box_policy_free(loop_policy);
   box_policy_free(exit_loop_policy);
   ap_free_policy(policy_manager, &policy);
   ap_policy_manager_free(policy_manager);
   ap_manager_free(manager);
	ap_environment_free(env);
}
