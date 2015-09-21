/*
 * ctest2.c
 *
 * Unit testing. Policies
 *
 * Copyright (C) Frank Védrine 2010
 *
 */

/* This file is part of the APRON Library, released under GPL license

   Please read the COPYING file packaged in the distribution.
*/

#include "ap_global0.h"
#include "ap_global1.h"

#include "../box/box_policy.h"

void demo(ap_policy_manager_t* pman, ap_manager_t* man) {
  ap_current_policy_t policy;
  ap_var_t local_variables[] = { "i", "j" };
  int nb_local_variables = sizeof(local_variables)/sizeof(ap_var_t);
  ap_environment_t* env = ap_environment_alloc(&local_variables[0], nb_local_variables, NULL, 0);

  ap_linexpr1_t one = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)0);
  ap_linexpr1_t ten = ap_linexpr1_make (env, AP_LINEXPR_SPARSE, (size_t)0);

  ap_linexpr1_set_cst_scalar_int(&one, 1);
  ap_linexpr1_set_cst_scalar_int(&ten, 10);
  ap_set_initial_policy(pman, &policy, 2, 100);

  /* ANALYSE */
  ap_abstract1_t _1 = ap_abstract1_top (man, env);
  ap_abstract1_t _2 = ap_abstract1_policy_assign_linexpr(man, pman, &policy, false, &_1, "i", &one, NULL);
  ap_abstract1_t _3 = ap_abstract1_policy_assign_linexpr(man, pman, &policy, false, &_2, "j", &ten, NULL);
  ap_abstract1_t _3bis = ap_abstract1_copy(man, &_3);

  ap_lincons1_array_t constraint_j_minus_i_geq_0 = ap_lincons1_array_make(env,1);
  ap_linexpr1_t expr_j_minus_i = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,2);
  ap_lincons1_t cons_j_minus_i_geq_0 = ap_lincons1_make(AP_CONS_SUPEQ,&expr_j_minus_i,NULL);

  ap_lincons1_set_list(&cons_j_minus_i_geq_0,
		       AP_COEFF_S_INT,1,"j",
		       AP_COEFF_S_INT,-1,"i",
		       AP_CST_S_INT,0,
		       AP_END);
  ap_lincons1_array_set(&constraint_j_minus_i_geq_0,0,&cons_j_minus_i_geq_0);
  ap_abstract1_t enter_while_constraint = ap_abstract1_policy_of_lincons_array(man, pman, &policy,env,&constraint_j_minus_i_geq_0);

  ap_lincons1_array_t constraint_i_minus_j_greater_0 = ap_lincons1_array_make(env,1);
  ap_linexpr1_t expr_i_minus_j = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,2);
  ap_lincons1_t cons_i_minus_j_greater_0 = ap_lincons1_make(AP_CONS_SUP,&expr_i_minus_j,NULL);

  ap_lincons1_set_list(&cons_i_minus_j_greater_0,
		       AP_COEFF_S_INT,1,"i",
		       AP_COEFF_S_INT,-1,"j",
		       AP_CST_S_INT,0,
		       AP_END);
  ap_lincons1_array_set(&constraint_i_minus_j_greater_0,0,&cons_i_minus_j_greater_0);
  ap_abstract1_t exit_while_constraint = ap_abstract1_policy_of_lincons_array(man, pman, &policy,env,&constraint_i_minus_j_greater_0);

  ap_abstract1_t _4 = ap_abstract1_policy_meet(man, pman, &policy,false,&_3,&enter_while_constraint);
  ap_abstract1_t _8 = ap_abstract1_policy_meet(man, pman, &policy,false,&_3bis,&exit_while_constraint);

  ap_linexpr1_t expr_i_plus_2 = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,0);
  ap_linexpr1_set_list(&expr_i_plus_2,
		       AP_COEFF_S_INT,1,"i",
		       AP_CST_I_INT,0,2,
		       AP_END);
  ap_abstract1_t _6 = ap_abstract1_policy_assign_linexpr(man, pman, &policy,false,&_4,"i",&expr_i_plus_2,NULL);

  ap_linexpr1_t expr_j_minus_1 = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,0);
  ap_linexpr1_set_list(&expr_j_minus_1,
		       AP_COEFF_S_INT,1,"j",
		       AP_CST_I_INT,0,-1,
		       AP_END);
  ap_abstract1_t _7 = ap_abstract1_policy_assign_linexpr(man, pman, &policy,false,&_6,"j",&expr_j_minus_1,NULL);
  ap_abstract1_t _7bis = ap_abstract1_copy(man, &_7);
  ap_abstract1_t _4bis = ap_abstract1_policy_meet(man, pman, &policy,false,&_7,&enter_while_constraint);
  ap_abstract1_t _8bis = ap_abstract1_policy_meet(man, pman, &policy,false,&_7bis,&exit_while_constraint);

  _4 = ap_abstract1_join(man,true,&_4,&_4bis);
  _8 = ap_abstract1_join(man,true,&_8,&_8bis);
  policy.index = 0;
  policy.mode = AP_POLICY_APPLY;

  ap_linexpr1_clear(&expr_i_plus_2);
  ap_linexpr1_clear(&expr_j_minus_1);
  ap_abstract1_clear(man,&_1);
  ap_abstract1_clear(man,&_2);
  ap_abstract1_clear(man,&_3);
  ap_abstract1_clear(man,&_3bis);
  ap_abstract1_clear(man,&_4);
  ap_abstract1_clear(man,&_4bis);
  ap_abstract1_clear(man,&_6);
  ap_abstract1_clear(man,&_7);
  ap_abstract1_clear(man,&_7bis);
  ap_abstract1_clear(man,&_8);
  ap_abstract1_clear(man,&_8bis);
  ap_lincons1_array_clear(&constraint_j_minus_i_geq_0);
  ap_lincons1_array_clear(&constraint_i_minus_j_greater_0);

  ap_linexpr1_clear(&one);
  ap_linexpr1_clear(&ten);
  ap_linexpr1_clear(&expr_j_minus_1);
  ap_linexpr1_clear(&expr_i_plus_2);

  ap_abstract1_clear(man, &enter_while_constraint);
  ap_abstract1_clear(man, &exit_while_constraint);

  ap_environment_free(env);
  ap_free_policy(pman, &policy);
}

int main(void) {
  ap_manager_t* man = box_manager_alloc();
  ap_policy_manager_t* pman = box_policy_manager_alloc();
  demo(pman, man);
  ap_policy_manager_free(pman);
  ap_manager_free(man);
  return 0;
}
