/* ********************************************************************** */
/* test1.c: testing level 1 */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include <stdlib.h>
#include <stdio.h>
#include "ap_global1.h"
#include "pk.h"

void test_environment()
{
  size_t i;
  ap_environment_t* e1;
  ap_environment_t* e2;
  ap_environment_t* e3;
  ap_environment_t* e4;
  ap_environment_t* e5;
  ap_dimchange_t* change1;
  ap_dimchange_t* change2;

  ap_var_t name_of_intdim1[3] = {
    "a30","a10","a20"
  };
  ap_var_t name_of_realdim1[3] = {
    "a11","a31","a21"
  };
  ap_var_t name_of_intdim2[4] = {
    "a2","a0","a22","a24"
  };
  ap_var_t name_of_realdim2[4] = {
    "a27","a23","a33","a37"
  };
  ap_var_t name_of_intdim5[4] = {
    "a2","a10","a22","a20"
  };
  ap_var_t name_of_realdim5[4] = {
    "a27","a31","a11","a37"
  };
  e1 = ap_environment_alloc_empty();
  printf("env 1\n");
  ap_environment_fdump(stdout,e1);
  ap_environment_free(e1);
  e1 = ap_environment_alloc(name_of_intdim1,3,
			name_of_realdim1,3);
  printf("env 1\n");
  ap_environment_fdump(stdout,e1);
  e2 = ap_environment_alloc(name_of_intdim2,4,
			name_of_realdim2,4);
  printf("env 2\n");
  ap_environment_fdump(stdout,e2);
  e5 = ap_environment_alloc(name_of_intdim5,4,
			name_of_realdim5,4);
  printf("env 5\n");
  ap_environment_fdump(stdout,e5);

  e3 = ap_environment_add(e1,name_of_intdim2,4,
			name_of_realdim2,4);
  printf("env 3\n");
  ap_environment_fdump(stdout,e3);
  e4 = ap_environment_add(e2,name_of_intdim1,3,
		       name_of_realdim1,3);
  printf("env 4\n");
  ap_environment_fdump(stdout,e4);
  for(i=0; i<e4->intdim+e4->realdim; i++){
    ap_var_t var = ap_environment_var_of_dim(e4,i);
    ap_dim_t dim = ap_environment_dim_of_var(e4,var);
    char* name = ap_var_operations->to_string(var);
    printf("i=%d, var=%s, dim=%d\n",i,name,dim);
    free(name);
  }
  ap_environment_free(e4);
  ap_environment_free(e3);

  e4 = ap_environment_remove(e5,name_of_intdim2,4,
			name_of_realdim2,4);
  assert(e4==NULL);

  e3 = ap_environment_lce(e1,e2,&change1,&change2);
  printf("env 3\n");
  ap_environment_fdump(stdout,e3);
  ap_dimchange_fprint(stdout,change1);
  ap_dimchange_fprint(stdout,change2);
  ap_dimchange_free(change1);
  ap_dimchange_free(change2);

  e4 = ap_environment_lce(e3,e2,&change1,&change2);
  assert(e3==e4);
  printf("env 4\n");
  ap_environment_fdump(stdout,e4);
  assert(change1==NULL);
  ap_dimchange_fprint(stdout,change2);
  ap_dimchange_free(change2);
  ap_environment_free(e3);
  ap_environment_free(e4);

  e3 = ap_environment_lce(e1,e5,&change1,&change2);
  printf("env 3\n");
  ap_environment_fdump(stdout,e3);
  ap_dimchange_fprint(stdout,change1);
  ap_dimchange_fprint(stdout,change2);
  ap_dimchange_free(change1);
  ap_dimchange_free(change2);
  ap_environment_free(e3);
  ap_environment_free(e1);
  ap_environment_free(e2);
  ap_environment_free(e5);
}


/* ********************************************************************** */
/* Polyhedra 1 */
/* ********************************************************************** */

void absval1(ap_manager_t* man, ap_abstract1_t* ppo1, ap_abstract1_t* ppo2)
{
  /* Creation du polyèdre
     1/2x+2/3y=1, [1,2]<=z+2w<=4, -2<=1/3z-w<=3,
     u non contraint */
  ap_var_t name_of_realdim1[6] = {
    "x","y","z","u","w","v"
  };
  ap_var_t name_of_realdimXY[2] = {
    "x","y"
  };
  ap_var_t name_of_realdimZW[2] = {
    "z","w"
  };
  ap_lincons1_t cons;
  ap_lincons1_array_t array;
  ap_generator1_array_t garray;
  ap_abstract1_t poly;
  ap_abstract1_t poly2;
  ap_abstract1_t poly3;
  ap_box1_t box;
  ap_interval_t* itv;
  size_t i;
  ap_linexpr1_t expr;
  tbool_t tb;
  ap_environment_t* env;
  ap_environment_t* envXY;
  ap_environment_t* envZW;
  ap_environment_t* envXYZW;
  bool b;

  env = ap_environment_alloc(NULL,0,name_of_realdim1,6);
  envXY = ap_environment_alloc(NULL,0,name_of_realdimXY,2);
  envZW = ap_environment_alloc(NULL,0,name_of_realdimZW,2);
  envXYZW = ap_environment_add(envXY,NULL,0,name_of_realdimZW,2);

  array = ap_lincons1_array_make(envXYZW,5);

  /* 1. Constraint system */
  /* 1/2x+2/3y=1 */
  expr = ap_linexpr1_make(envXY,AP_LINEXPR_SPARSE,3);
  ap_linexpr1_set_cst_scalar_int(&expr,1);
  assert(! ap_linexpr1_set_coeff_scalar_frac(&expr,"x",1,2) );
  assert(! ap_linexpr1_set_coeff_scalar_frac(&expr,"y",2,3) );
  cons = ap_lincons1_make(AP_CONS_EQ,&expr);
  ap_lincons1_fprint(stdout,&cons);fprintf(stdout,"\n");
  ap_lincons1_extend_environment_with(&cons,envXYZW);
  ap_lincons1_fprint(stdout,&cons);fprintf(stdout,"\n");
  assert(! ap_lincons1_array_set(&array,0,&cons) );
  /* [1,2]<=z+2w */
  expr = ap_linexpr1_make(envZW,AP_LINEXPR_SPARSE,1);
  b = ap_linexpr1_set_list(&expr,
			   AP_CST_I_INT,-2,-1,
			   AP_COEFF_S_INT, 1, "z",
			   AP_COEFF_S_INT, 2, "w",
			   AP_END);
  assert(!b);
  cons = ap_lincons1_make(AP_CONS_SUPEQ,&expr);
  ap_lincons1_fprint(stdout,&cons);fprintf(stdout,"\n");
  ap_lincons1_extend_environment_with(&cons,envXYZW);
  ap_lincons1_fprint(stdout,&cons);fprintf(stdout,"\n");
  assert(! ap_lincons1_array_set(&array,1,&cons) );
  /* z+2w<=4 */
  expr = ap_linexpr1_make(envZW,AP_LINEXPR_SPARSE,0);
  b = ap_linexpr1_set_list(&expr,
			   AP_CST_S_INT,-4,
			   AP_COEFF_S_INT, -1, "z",
			   AP_COEFF_S_INT, -2, "w",
			   AP_END);
  assert(!b);
  cons = ap_lincons1_make(AP_CONS_SUPEQ,&expr);
  ap_lincons1_fprint(stdout,&cons);fprintf(stdout,"\n");
  ap_lincons1_extend_environment_with(&cons,envXYZW);
  ap_lincons1_fprint(stdout,&cons);fprintf(stdout,"\n");
  assert(! ap_lincons1_array_set(&array,2,&cons) );
  /* -2<=1/3z-w */
  expr = ap_linexpr1_make(envZW,AP_LINEXPR_SPARSE,2);
  cons = ap_lincons1_make(AP_CONS_SUPEQ,&expr);
  b = ap_lincons1_set_list(&cons,
			   AP_CST_S_INT,2,
			   AP_COEFF_S_FRAC, 1, 3, "z",
			   AP_COEFF_S_INT, -1, "w",
			   AP_END);
  assert(!b);
  ap_lincons1_fprint(stdout,&cons);fprintf(stdout,"\n");
  ap_lincons1_extend_environment_with(&cons,envXYZW);
  ap_lincons1_fprint(stdout,&cons);fprintf(stdout,"\n");
  assert(! ap_lincons1_array_set(&array,3,&cons) );
  /* 1/3z-w<=3 */
  expr = ap_linexpr1_make(envZW,AP_LINEXPR_SPARSE,1);
  ap_linexpr1_set_cst_scalar_int(&expr,3);
  assert(! ap_linexpr1_set_coeff_scalar_frac(&expr,"z", -1,3) );
  assert(! ap_linexpr1_set_coeff_scalar_int(&expr,"w", 1) );
  cons = ap_lincons1_make(AP_CONS_SUPEQ,&expr);
  ap_lincons1_fprint(stdout,&cons);fprintf(stdout,"\n");
  ap_lincons1_extend_environment_with(&cons,envXYZW);
  ap_lincons1_fprint(stdout,&cons);fprintf(stdout,"\n");
  assert(! ap_lincons1_array_set(&array,4,&cons) );

  /* Creation */
  ap_lincons1_array_fprint(stdout,&array);
  poly = ap_abstract1_of_lincons_array(man,env,&array);
  ap_abstract1_fprint(stdout,man,&poly);
  ap_abstract1_canonicalize(man,&poly);
  ap_abstract1_fprint(stdout,man,&poly);
  garray = ap_abstract1_to_generator_array(man,&poly);
  ap_generator1_array_fprint(stdout,&garray);
  ap_generator1_array_clear(&garray);
  ap_abstract1_fdump(stdout,man,&poly);
  /* 2. Constraint system */
  /* Conversion (to_lincons_array already tested with print) */
  /* Should be
     x: [-oo,+oo]
     y: [-oo,+oo]
     z: [-6/5,6]
     w: [-7/5,2]
     u: [-oo,+oo]
     v: [-oo,+oo]
  */
  box = ap_abstract1_to_box(man,&poly);
  ap_box1_fprint(stdout,&box);
  ap_box1_clear(&box);

  /* Extraction (we first extract values for existing constraints, then for
     dimensions) */
  /* existing constraints */
  for (i=0; i<5; i++){
    cons = ap_lincons1_array_get(&array,i);
    expr = ap_lincons1_linexpr1ref(&cons);
    itv = ap_abstract1_bound_linexpr(man,&poly,&expr);
    fprintf(stdout,"Bound of ");
    ap_linexpr1_fprint(stdout,&expr);
    fprintf(stdout,": ");
    ap_interval_fprint(stdout,itv);
    fprintf(stdout,"\n");
    ap_interval_free(itv);
  }
  /* dimensions */
  for (i=0; i<env->intdim+env->realdim; i++){
    expr = ap_linexpr1_make(env,AP_LINEXPR_SPARSE,1);
    ap_linexpr1_set_cst_scalar_double(&expr,0.0);
    ap_linexpr1_set_coeff_scalar_int(&expr,ap_environment_var_of_dim(env,i),1);
    itv = ap_abstract1_bound_linexpr(man,&poly,&expr);
    fprintf(stdout,"Bound of ");
    ap_linexpr1_fprint(stdout,&expr);
    fprintf(stdout,": ");
    ap_interval_fprint(stdout,itv);
    fprintf(stdout,"\n");
    ap_interval_free(itv);
    ap_linexpr1_clear(&expr);
  }
  /* 3. of box z: [-6/5,6], w: [-7/5,2] */
  {
    ap_var_t name[2] = { "z", "w" };
    ap_interval_t* itv[2];

    itv[0] = ap_interval_alloc();
    itv[1] = ap_interval_alloc();
    ap_interval_set_frac(itv[0],-6,5,6,1);
    ap_interval_set_frac(itv[1],-7,5,2,1);
    poly2 = ap_abstract1_of_box(man,env,name,itv,2);
    ap_abstract1_fprint(stdout,man,&poly2);
    ap_abstract1_canonicalize(man,&poly2);
    ap_abstract1_fprint(stdout,man,&poly2);
    ap_abstract1_fdump(stdout,man,&poly2);
    ap_interval_free(itv[0]);
    ap_interval_free(itv[1]);
  }

  /* 4. Tests top and bottom */
  poly3 = ap_abstract1_bottom(man,env);
  tb = ap_abstract1_is_bottom(man,&poly3);
  fprintf(stdout,"ap_abstract1_is_bottom(poly3)=%d\n",tb);
  tb = ap_abstract1_is_top(man,&poly3);
  fprintf(stdout,"ap_abstract1_is_top(poly3)=%d\n",tb);
  ap_abstract1_clear(man,&poly3);

  poly3 = ap_abstract1_top(man,env);
  tb = ap_abstract1_is_bottom(man,&poly3);
  fprintf(stdout,"ap_abstract1_is_bottom(poly3)=%d\n",tb);
  tb = ap_abstract1_is_top(man,&poly3);
  fprintf(stdout,"ap_abstract1_is_top(poly3)=%d\n",tb);
  ap_abstract1_clear(man,&poly3);

  ap_abstract1_minimize(man,&poly2);
  ap_abstract1_fprint(stdout,man,&poly2);
  tb = ap_abstract1_is_bottom(man,&poly2);
  fprintf(stdout,"ap_abstract1_is_bottom(poly2)=%d\n",tb);
   tb = ap_abstract1_is_top(man,&poly2);
  fprintf(stdout,"ap_abstract1_is_top(poly2)=%d\n",tb);

  /* 5. Tests leq */
  tb = ap_abstract1_is_leq(man,&poly,&poly2);
  fprintf(stdout,"ap_abstract1_is_leq(poly,&poly2)=%d\n",tb);
  tb = ap_abstract1_is_leq(man,&poly2,&poly);
  fprintf(stdout,"ap_abstract1_is_leq(poly,&poly2)=%d\n",tb);

  /* 6. Tests sat_interval */
  itv = ap_interval_alloc();
  ap_interval_set_double(itv,-6.0,6.0);
  tb = ap_abstract1_sat_interval(man,&poly,"z",itv);
  fprintf(stdout,"ap_abstract1_sat_interval(poly,z)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",tb);

  tb = ap_abstract1_sat_interval(man,&poly,"w",itv);
  fprintf(stdout,"ap_abstract1_sat_interval(poly,w)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",tb);

  tb = ap_abstract1_sat_interval(man,&poly,"u",itv);
  fprintf(stdout,"ap_abstract1_sat_interval(poly,u)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",tb);

  ap_interval_set_double(itv,-2.5,2.5);
  tb = ap_abstract1_sat_interval(man,&poly,"z",itv);
  fprintf(stdout,"ap_abstract1_sat_interval(poly,z)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",tb);
  tb = ap_abstract1_sat_interval(man,&poly,"w",itv);
  fprintf(stdout,"ap_abstract1_sat_interval(poly,w)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",tb);
  tb = ap_abstract1_sat_interval(man,&poly,"u",itv);
  fprintf(stdout,"ap_abstract1_sat_interval(poly,u)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",tb);

  ap_interval_set_double(itv,-1.4,2.0);
  tb = ap_abstract1_sat_interval(man,&poly,"z",itv);
  fprintf(stdout,"ap_abstract1_sat_interval(poly,z)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",tb);
  tb = ap_abstract1_sat_interval(man,&poly,"w",itv);
  fprintf(stdout,"ap_abstract1_sat_interval(poly,w)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",tb);
  tb = ap_abstract1_sat_interval(man,&poly,"u",itv);
  fprintf(stdout,"ap_abstract1_sat_interval(poly,u)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",tb);

  ap_scalar_set_frac(itv->inf,-14,10);
  ap_scalar_set_double(itv->sup,2.0);
  tb = ap_abstract1_sat_interval(man,&poly,"z",itv);
  fprintf(stdout,"ap_abstract1_sat_interval(poly,z)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",tb);
  tb = ap_abstract1_sat_interval(man,&poly,"w",itv);
  fprintf(stdout,"ap_abstract1_sat_interval(poly,w)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",tb);
  tb = ap_abstract1_sat_interval(man,&poly,"u",itv);
  fprintf(stdout,"ap_abstract1_sat_interval(poly,u)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",tb);

  ap_interval_free(itv);

  /* 7. Tests sat_lincons with -3x-4y+z-w */
  expr = ap_linexpr1_make(env,AP_LINEXPR_DENSE,0);
  ap_linexpr1_set_cst_scalar_int(&expr,0);
  ap_linexpr1_set_coeff_scalar_int(&expr,"x",-3);
  ap_linexpr1_set_coeff_scalar_int(&expr,"y",-4);
  ap_linexpr1_set_coeff_scalar_int(&expr,"z",1);
  ap_linexpr1_set_coeff_scalar_int(&expr,"w",-1);
  cons = ap_lincons1_make(AP_CONS_SUPEQ,&expr);

  itv = ap_abstract1_bound_linexpr(man,&poly,&expr);
  fprintf(stdout,"Bound of ");
  ap_linexpr1_fprint(stdout,&expr);
  fprintf(stdout,": ");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout,"\n");
  ap_interval_free(itv);

  for (i=0; i<6; i++){
    ap_linexpr1_set_cst_scalar_frac(&expr,-26 + (int)i*10, 5);
    *(ap_lincons1_constypref(&cons)) = AP_CONS_SUPEQ;
    tb = ap_abstract1_sat_lincons(man,&poly,&cons);
    fprintf(stdout,"ap_abstract1_sat_lincons(poly)");
    ap_lincons1_fprint(stdout,&cons);
    fprintf(stdout,": %d\n",tb);
    *(ap_lincons1_constypref(&cons)) = AP_CONS_SUP;
    tb = ap_abstract1_sat_lincons(man,&poly,&cons);
    fprintf(stdout,"ap_abstract1_sat_lincons(poly)");
    ap_lincons1_fprint(stdout,&cons);
    fprintf(stdout,": %d\n",tb);
  }

  ap_linexpr1_set_cst_scalar_int(&expr,0);
  ap_linexpr1_set_coeff_scalar_frac(&expr,"z",-2,3);
  ap_linexpr1_set_coeff_scalar_double(&expr,"w",2.0);

  itv = ap_abstract1_bound_linexpr(man,&poly,&expr);
  fprintf(stdout,"Bound of ");
  ap_linexpr1_fprint(stdout,&expr);
  fprintf(stdout,": ");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout,"\n");
  ap_interval_free(itv);

  for (i=0; i<6; i++){
    ap_linexpr1_set_cst_scalar_int(&expr,-1+(int)i);
    *(ap_lincons1_constypref(&cons)) = AP_CONS_SUPEQ;
    tb = ap_abstract1_sat_lincons(man,&poly,&cons);
    fprintf(stdout,"ap_abstract1_sat_lincons(poly)");
    ap_lincons1_fprint(stdout,&cons);
    fprintf(stdout,": %d\n",tb);
    *(ap_lincons1_constypref(&cons)) = AP_CONS_SUP;
    tb = ap_abstract1_sat_lincons(man,&poly,&cons);
    fprintf(stdout,"ap_abstract1_sat_lincons(poly)");
    ap_lincons1_fprint(stdout,&cons);
    fprintf(stdout,": %d\n",tb);
  }
  ap_linexpr1_clear(&expr);

  ap_lincons1_array_clear(&array);
  ap_environment_free(env);
  ap_environment_free(envXY);
  ap_environment_free(envZW);
  ap_environment_free(envXYZW);
  *ppo1 = poly;
  *ppo2 = poly2;
}

int main(int argc, char**argv)
{
  ap_manager_t* man;
  ap_abstract1_t a1,a2;

  test_environment();
  man = pk_manager_alloc(true);
  absval1(man,&a1,&a2);
  ap_abstract1_clear(man,&a1);
  ap_abstract1_clear(man,&a2);
  ap_manager_free(man);
}
