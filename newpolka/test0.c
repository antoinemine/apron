/* ********************************************************************** */
/* test.c: unit testing  */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include <fenv.h>
#include <string.h>

#include "ap_dimension.h"
#include "pk_config.h"
#include "pk_vector.h"
#include "pk_satmat.h"
#include "pk_matrix.h"
#include "pk.h"
#include "pk_representation.h"
#include "pk_constructor.h"
#include "pk_extract.h"
#include "pk_test.h"
#include "pk_meetjoin.h"
#include "pk_resize.h"
#include "pk_assign.h"

static size_t count = 0;

/* ********************************************************************** */
/* Vectors */
/* ********************************************************************** */

void vector(pk_internal_t* pk, size_t nbdims, numint_t factor)
{
  size_t size;
  numint_t* q[5];
  numint_t* r1;
  size_t i;
  int j;
  bool change;

  pk_internal_realloc_lazy(pk,30);

  size = pk->dec + nbdims;

  for (i=1; i<=4; i++)
    q[i] = vector_alloc(size);

  /* q[1] = [0 10 [0] 1 2 3 4 ...] * factor */
  numint_set_int(q[1][polka_cst],10L);
  for (i=pk->dec; i<size; i++){
    numint_set_int(q[1][i],(int)(i-pk->dec+1));
    numint_mul(q[1][i],q[1][i],factor);
  }
  vector_print(q[1],size);
  /* q[2] = [1 10 [-2] 1 2 3 4 ...] * factor */
  vector_copy(q[2],q[1],size);
  numint_set_int(q[2][0],1);
  if (pk->strict) numint_set_int(q[2][polka_eps],-2);
  vector_print(q[2],size);
  /* q[3] = [1 10 [0] size-1 size-2 ... ] * 2 * factor */
  numint_set_int(q[3][0],1);
  numint_set_int(q[3][polka_cst],10);
  for (i=pk->dec; i<size; i++){
    numint_set_int(q[3][i],size-i);
    numint_mul(q[3][i],q[3][i],factor);
  }
  vector_print(q[3],size);
  /* q[4] = [1 10 [-1] size-1 size-2 ... ] * 2 * factor */
  numint_set_int(q[4][0],1);
  if (pk->strict) numint_set_int(q[4][polka_eps],-1);
  numint_set_int(q[4][polka_cst],10);
  for (i=pk->dec; i<size; i++){
    numint_set_int(q[4][i],3*(size-i));
    numint_mul(q[4][i],q[4][i],factor);
  }
  vector_print(q[4],size);
  
  /* vector_realloc */
  for (i=1; i<=4; i++){
    printf("vector_realloc %d\n",i);
    r1 = vector_alloc(size);
    vector_copy(r1,q[i],size);
    printf("r1=%p\n",r1);
    vector_print(r1,size);

    vector_realloc(&r1,size,pk->dec+3);
    printf("r1=%p\n",r1);
    vector_print(r1,pk->dec+3);

    vector_realloc(&r1,pk->dec+3,2*size);
    printf("r1=%p\n",r1);
    vector_print(r1,2*size);

    vector_free(r1,2*size);
  }
  printf("\n");

  /* vector_normalize */
  r1 = vector_alloc(size);
  for (i=1; i<=4; i++){
    printf("vector_normalize %d\n",i);
    vector_copy(r1,q[i],size);
    vector_print(r1,size);

    vector_normalize(pk,r1,size);
    vector_print(r1,size);

    printf("vector_normalize_constraint %d\n",i);
    vector_copy(r1,q[i],size);
    change = vector_normalize_constraint(pk,r1,size/2,size-size/2-pk->dec);
    vector_print(r1,size);
    printf("change=%d\n",change);

    printf("vector_normalize_constraint_int %d\n",i);
    vector_copy(r1,q[i],size);
    change = vector_normalize_constraint_int(pk,r1,size/2,size-size/2-pk->dec);
    vector_print(r1,size);
    printf("change=%d\n",change);

    printf("vector_normalize_constraint_int %d\n",i);
    vector_copy(r1,q[i],size);
    change = vector_normalize_constraint_int(pk,r1,size-pk->dec,0);
    vector_print(r1,size);
    printf("change=%d\n",change);
  }
  vector_free(r1,size);
  printf("\n");
  
  /* vector_compare */
  for (i=1; i<=4; i++){
    for (j=1; j<=4; j++){
      int res = vector_compare(pk,q[i],q[j],size);
      printf("vector_compare(%d,%d)=%d\n",i,j,res);
    }
  }
  /* vector_combine */
  r1 = vector_alloc(size);
  for (i=1; i<=4; i++){
    for (j=i; j<=4; j++){
      vector_combine(pk,q[i],q[j],r1,pk->dec+3,size);
      printf("vector_combine(%d,%d,k=%d)\n",i,j,pk->dec+3);
      vector_print(r1,size);
      vector_combine(pk,q[j],q[i],r1,pk->dec+3,size);
      printf("vector_combine(%d,%d,k=%d)\n",j,i,pk->dec+3);
      vector_print(r1,size);
    }
  }
  vector_free(r1,size);
  printf("\n");

  for (i=1; i<=4; i++){
    vector_free(q[i],size);
  }
}


/* ********************************************************************** */
/* Polyhedra 1 */
/* ********************************************************************** */

void poly1(ap_manager_t* man, char** name_of_dim, pk_t** ppo1, pk_t** ppo2)
{
  /* Creation du polyèdre 
     1/2x+2/3y=1, [1,2]<=z+2w<=4, -2<=1/3z-w<=3,
     u non contraint */
  mpq_t mpq;
  ap_lincons0_t cons;
  ap_lincons0_array_t array;
  ap_generator0_array_t garray;
  pk_t* poly;
  pk_t* poly2;
  pk_t* poly3;
  ap_interval_t** titv;
  ap_interval_t* itv;
  int i;
  ap_linexpr0_t* expr;
  bool b;

  mpq_init(mpq);
  array = ap_lincons0_array_make(5);

  /* 1. Constraint system */
  array.p[0].constyp = AP_CONS_EQ;
  array.p[0].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,2);
  ap_linexpr0_set_cst_scalar_int(array.p[0].linexpr0,1);
  ap_linexpr0_set_coeff_scalar_double(array.p[0].linexpr0,0,0.5);
  ap_linexpr0_set_coeff_scalar_frac(array.p[0].linexpr0,1,2,3);

  array.p[1].constyp = AP_CONS_SUPEQ;
  array.p[1].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,2);
  ap_linexpr0_set_cst_interval_int(array.p[1].linexpr0,-2,-1);
  ap_linexpr0_set_coeff_scalar_double(array.p[1].linexpr0,2,1.0);
  ap_linexpr0_set_coeff_scalar_frac(array.p[1].linexpr0,3,2,1);

  array.p[2].constyp = AP_CONS_SUPEQ;
  array.p[2].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,2);
  ap_linexpr0_set_list(array.p[2].linexpr0,
		       AP_COEFF_S_DOUBLE,-1.0, 2,
		       AP_COEFF_S_FRAC,-2,1, 3,
		       AP_CST_S_INT, 4,
		       AP_END);
  
  array.p[3].constyp = AP_CONS_SUPEQ;
  array.p[3].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,2);
  ap_linexpr0_set_cst_scalar_int(array.p[3].linexpr0,2);
  ap_linexpr0_set_list(array.p[3].linexpr0,
		       AP_COEFF_S_FRAC,1,3, 2,
		       AP_COEFF_S_INT,-1, 3,
		       AP_END);
  
  array.p[4].constyp = AP_CONS_SUP;
  array.p[4].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,2);
  ap_linexpr0_set_list(array.p[4].linexpr0,
		       AP_COEFF_S_FRAC, -1,3, 2,
		       AP_COEFF_S_INT, 1, 3,
		       AP_CST_S_INT, 3,
		       AP_END);

  /* Creation */
  ap_lincons0_array_fprint(stdout,&array,name_of_dim);
  poly = pk_top(man,0,6);
  poly = pk_meet_lincons_array(man,true,poly,&array);
  pk_fprint(stdout,man,poly,name_of_dim);
  pk_canonicalize(man,poly);
  pk_fprint(stdout,man,poly,name_of_dim);  
  garray = pk_to_generator_array(man,poly);
  ap_generator0_array_fprint(stdout,&garray,name_of_dim);
  ap_generator0_array_clear(&garray);
  pk_fdump(stdout,man,poly);  
  /* 2. Constraint system */
  /* Conversion (to_lincons_array already tested with print) */
  /* Should be
     0: [-oo,+oo]
     1: [-oo,+oo]
     2: [-6/5,6]
     3: [-7/5,2]
     4: [-oo,+oo]
     5: [-oo,+oo]
  */
  titv = pk_to_box(man,poly);
  for (i=0; i<6; i++){
    fprintf(stdout,"%2d: ",i);
    ap_interval_fprint(stdout,titv[i]);
    fprintf(stdout,"\n");
  }

  /* Extraction (we first extract values for existing constraints, then for
     dimensions) */
  /* existing constraints */
  for (i=0; i<5; i++){
    itv = pk_bound_linexpr(man,poly,array.p[i].linexpr0);
    fprintf(stdout,"Bound of ");
    ap_linexpr0_fprint(stdout,array.p[i].linexpr0,name_of_dim);
    fprintf(stdout,": ");
    ap_interval_fprint(stdout,itv);
    fprintf(stdout,"\n");
    ap_interval_free(itv);
  }
  /* dimensions */
  expr = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,1);
  ap_coeff_set_scalar_double(&expr->cst,0.0);
  ap_coeff_set_scalar_double(&expr->p.linterm[0].coeff,1.0); 
  for (i=0; i<6; i++){
    expr->p.linterm[0].dim = (ap_dim_t)i;
    itv = pk_bound_linexpr(man,poly,expr);
    fprintf(stdout,"Bound of ");
    ap_linexpr0_fprint(stdout,expr,name_of_dim);
    fprintf(stdout,": ");
    ap_interval_fprint(stdout,itv);
    fprintf(stdout,"\n");
    ap_interval_free(itv);
  }
  ap_linexpr0_free(expr);
  /* 3. of box */
  poly2 = pk_of_box(man,0,6,(ap_interval_t**)titv);
  pk_fprint(stdout,man,poly2,name_of_dim);
  pk_canonicalize(man,poly2);
  pk_fprint(stdout,man,poly2,name_of_dim);  
  pk_fdump(stdout,man,poly2);  

  /* 4. Tests top and bottom */
  poly3 = pk_bottom(man,2,3);
  b = pk_is_bottom(man,poly3);
  fprintf(stdout,"pk_is_bottom(poly3)=%d\n",b);
  b = pk_is_top(man,poly3);
  fprintf(stdout,"pk_is_top(poly3)=%d\n",b);
  pk_free(man,poly3);

  poly3 = pk_top(man,2,3);
  b = pk_is_bottom(man,poly3);
  fprintf(stdout,"pk_is_bottom(poly3)=%d\n",b);
  b = pk_is_top(man,poly3);
  fprintf(stdout,"pk_is_top(poly3)=%d\n",b);
  pk_free(man,poly3);

  poly3 = pk_top(man,0,0);
  b = pk_is_bottom(man,poly3);
  fprintf(stdout,"pk_is_bottom(poly3)=%d\n",b);
  b = pk_is_top(man,poly3);
  fprintf(stdout,"pk_is_top(poly3)=%d\n",b);
  pk_free(man,poly3);
  
  pk_minimize(man,poly2);
  pk_fdump(stdout,man,poly2);  
  b = pk_is_bottom(man,poly2);
  fprintf(stdout,"pk_is_bottom(poly2)=%d\n",b);
  b = pk_is_top(man,poly2);
  fprintf(stdout,"pk_is_top(poly2)=%d\n",b);

  /* 5. Tests leq */
  b = pk_is_leq(man,poly,poly2);
  fprintf(stdout,"pk_is_leq(poly,poly2)=%d\n",b);
  b = pk_is_leq(man,poly2,poly);
  fprintf(stdout,"pk_is_leq(poly,poly2)=%d\n",b);

  /* 6. Tests sat_interval */
  itv = ap_interval_alloc();
  ap_interval_set_int(itv,-6,6);
  b = pk_sat_interval(man,poly,2,itv);
  fprintf(stdout,"pk_sat_interval(poly,2)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",b);
  b = pk_sat_interval(man,poly,3,itv);
  fprintf(stdout,"pk_sat_interval(poly,3)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",b);
  b = pk_sat_interval(man,poly,4,itv);
  fprintf(stdout,"pk_sat_interval(poly,4)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",b);

  ap_interval_set_double(itv,-2.5,2.5);
  b = pk_sat_interval(man,poly,2,itv);
  fprintf(stdout,"pk_sat_interval(poly,2)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",b);
  b = pk_sat_interval(man,poly,3,itv);
  fprintf(stdout,"pk_sat_interval(poly,3)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",b);
  b = pk_sat_interval(man,poly,4,itv);
  fprintf(stdout,"pk_sat_interval(poly,4)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",b);

  ap_interval_set_double(itv,-1.4,2.0);
  b = pk_sat_interval(man,poly,2,itv);
  fprintf(stdout,"pk_sat_interval(poly,2)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",b);
  b = pk_sat_interval(man,poly,3,itv);
  fprintf(stdout,"pk_sat_interval(poly,3)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",b);
  b = pk_sat_interval(man,poly,4,itv);
  fprintf(stdout,"pk_sat_interval(poly,4)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",b);

  mpq_set_si(mpq,-14,10);
  ap_scalar_set_mpq(itv->inf,mpq);
  ap_scalar_set_double(itv->sup,2.0);
  b = pk_sat_interval(man,poly,2,itv);
  fprintf(stdout,"pk_sat_interval(poly,2)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",b);
  b = pk_sat_interval(man,poly,3,itv);
  fprintf(stdout,"pk_sat_interval(poly,3)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",b);
  b = pk_sat_interval(man,poly,4,itv);
  fprintf(stdout,"pk_sat_interval(poly,4)");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout," = %d\n",b);

  ap_interval_free(itv);

  /* 7. Tests sat_lincons */
  expr = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,4);
  cons.constyp = AP_CONS_SUPEQ;
  cons.linexpr0 = expr;
  ap_linexpr0_set_list(expr,
		       AP_COEFF_S_DOUBLE, -3.0, 0,
		       AP_COEFF_S_DOUBLE, -4.0, 1,
		       AP_COEFF_S_DOUBLE, 1.0, 2,
		       AP_COEFF_S_DOUBLE, -1.0, 3,
		       AP_CST_S_INT, 0,
		       AP_END);

  itv = pk_bound_linexpr(man,poly,expr);
  fprintf(stdout,"Bound of ");
  ap_linexpr0_fprint(stdout,expr,name_of_dim);
  fprintf(stdout,": ");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout,"\n");
  ap_interval_free(itv);
  
  for (i=0; i<6; i++){
    ap_linexpr0_set_cst_scalar_frac(expr,-26 + i*10, 5);
    cons.constyp = AP_CONS_SUPEQ;
    b = pk_sat_lincons(man,poly,&cons);
    fprintf(stdout,"pk_sat_lincons(poly)");
    ap_lincons0_fprint(stdout,&cons,name_of_dim);
    fprintf(stdout,": %d\n",b);
    cons.constyp = AP_CONS_SUP;
    b = pk_sat_lincons(man,poly,&cons);
    fprintf(stdout,"pk_sat_lincons(poly)");
    ap_lincons0_fprint(stdout,&cons,name_of_dim);
    fprintf(stdout,": %d\n",b);
  }
  
  ap_linexpr0_set_list(expr,
		       AP_COEFF_S_FRAC, -2,3, 2,
		       AP_COEFF_S_DOUBLE, 2.0, 3,
		       AP_CST_S_FRAC, 0,0,
		       AP_END);

  itv = pk_bound_linexpr(man,poly,expr);
  fprintf(stdout,"Bound of ");
  ap_linexpr0_fprint(stdout,expr,name_of_dim);
  fprintf(stdout,": ");
  ap_interval_fprint(stdout,itv);
  fprintf(stdout,"\n");
  ap_interval_free(itv);
  
  for (i=0; i<6; i++){
    ap_linexpr0_set_cst_scalar_int(expr,-1+i);
    cons.constyp = AP_CONS_SUPEQ;
    b = pk_sat_lincons(man,poly,&cons);
    fprintf(stdout,"pk_sat_lincons(poly)");
    ap_lincons0_fprint(stdout,&cons,name_of_dim);
    fprintf(stdout,": %d\n",b);
    cons.constyp = AP_CONS_SUP;
    b = pk_sat_lincons(man,poly,&cons);
    fprintf(stdout,"pk_sat_lincons(poly)");
    ap_lincons0_fprint(stdout,&cons,name_of_dim);
    fprintf(stdout,": %d\n",b);
  }
  

  ap_linexpr0_free(expr);
  
  ap_interval_array_free(titv,6);  
  ap_lincons0_array_clear(&array);
  mpq_clear(mpq);
  *ppo1 = poly;
  *ppo2 = poly2;
}

/* ********************************************************************** */
/* Polyhedra 2 */
/* ********************************************************************** */

void poly2(ap_manager_t* man, char** name_of_dim, pk_t* po1, pk_t* po2)
{
  bool b;
  pk_t* po;
  ap_linexpr0_t* expr[2];
  ap_lincons0_t cons;
  ap_lincons0_array_t array = { &cons, 1 };
  ap_generator0_t gen;
  ap_generator0_array_t garray, garray2;
  int i;

  /* Meet and join (without meet_array et join_array) */
  fprintf(stdout,"********* Meet and Join for 2 polyhedra *********\n");
  fprintf(stdout,"po1:\n");
  pk_fprint(stdout,man,po1,name_of_dim);
  fprintf(stdout,"po2:\n");
  pk_fprint(stdout,man,po2,name_of_dim);
  
  fprintf(stdout,"po=meet(po1,po2)\n");
  po = pk_meet(man,false,po1,po2);
  pk_fprint(stdout,man,po,name_of_dim);
  b = pk_is_leq(man,po,po1);
  fprintf(stdout,"pk_is_leq(man,po,po1)=%d\n",b);
  assert(b);
  b = pk_is_leq(man,po,po2);
  fprintf(stdout,"pk_is_leq(man,po,po2)=%d\n",b);
  assert(b);
  b = pk_is_leq(man,po1,po);
  fprintf(stdout,"pk_is_leq(man,po1,po)=%d\n",b);
  b = pk_is_leq(man,po2,po);
  fprintf(stdout,"pk_is_leq(man,po2,po)=%d\n",b);
  pk_free(man,po);

  fprintf(stdout,"po=po1; meet_with(po,po2)\n");
  po = pk_copy(man,po1);
  po = pk_meet(man,true,po,po2);
  pk_fprint(stdout,man,po,name_of_dim);
  b = pk_is_leq(man,po,po1);
  fprintf(stdout,"pk_is_leq(man,po,po1)=%d\n",b);
  assert(b);
  b = pk_is_leq(man,po,po2);
  fprintf(stdout,"pk_is_leq(man,po,po2)=%d\n",b);
  assert(b);
  b = pk_is_leq(man,po1,po);
  fprintf(stdout,"pk_is_leq(man,po1,po)=%d\n",b);
  b = pk_is_leq(man,po2,po);
  fprintf(stdout,"pk_is_leq(man,po2,po)=%d\n",b);
  pk_free(man,po);
  
  fprintf(stdout,"po = join(po1,po2)\n");
  po = pk_join(man,false,po1,po2);
  pk_fprint(stdout,man,po,name_of_dim);
  b = pk_is_top(man,po);
  fprintf(stdout,"pk_is_top(man,po)=%d\n",b);
  
  b = pk_is_leq(man,po,po1);
  fprintf(stdout,"pk_is_leq(man,po,po1)=%d\n",b);
  b = pk_is_leq(man,po,po2);
  fprintf(stdout,"pk_is_leq(man,po,po2)=%d\n",b);
  b = pk_is_leq(man,po1,po);
  fprintf(stdout,"pk_is_leq(man,po1,po)=%d\n",b);
  assert(b);
  b = pk_is_leq(man,po2,po);
  fprintf(stdout,"pk_is_leq(man,po2,po)=%d\n",b);
  assert(b);
  pk_free(man,po);
  
  fprintf(stdout,"po = po1; join_with(po,po2)\n");
  po = pk_copy(man,po1);
  po = pk_join(man,true,po,po2);
  pk_fprint(stdout,man,po,name_of_dim);
  b = pk_is_leq(man,po,po1);
  fprintf(stdout,"pk_is_leq(man,po,po1)=%d\n",b);
  b = pk_is_leq(man,po,po2);
  fprintf(stdout,"pk_is_leq(man,po,po2)=%d\n",b);
  b = pk_is_leq(man,po1,po);
  fprintf(stdout,"pk_is_leq(man,po1,po)=%d\n",b);
  assert(b);
  b = pk_is_leq(man,po2,po);
  fprintf(stdout,"pk_is_leq(man,po2,po)=%d\n",b);
  assert(b);
  pk_free(man,po);

  /* Additions of constraints */
  fprintf(stdout,"********* Addition of constraint *********\n");
  fprintf(stdout,"po1:\n");
  pk_fprint(stdout,man,po1,name_of_dim);
  garray = pk_to_generator_array(man,po1);
  ap_generator0_array_fprint(stdout,&garray,name_of_dim);
  ap_generator0_array_clear(&garray);

  /* expression z>=-1/2 */
  expr[0] = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,1);
  ap_linexpr0_set_cst_scalar_frac(expr[0],1,2);
  ap_linexpr0_set_coeff_scalar_int(expr[0],2,1);
  /* expression x+u>=1 */
  expr[1] = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,2);
  ap_linexpr0_set_list(expr[1],
		       AP_COEFF_S_INT, 1, 0,
		       AP_COEFF_S_INT, 1, 4,
		       AP_CST_S_INT, -1,
		       AP_END);
  for (i=0; i<2; i++){
    cons.linexpr0 = expr[i];
    for (cons.constyp=0; cons.constyp<3; cons.constyp++){
      fprintf(stdout,"po=meet_lincons(po1) ");
      ap_lincons0_fprint(stdout,&cons,name_of_dim);
      fprintf(stdout,"\n");
      po = pk_meet_lincons_array(man,false,po1,&array);
      pk_fprint(stdout,man,po,name_of_dim);
      garray = pk_to_generator_array(man,po);
      ap_generator0_array_fprint(stdout,&garray,name_of_dim);
      ap_generator0_array_clear(&garray);
      b = pk_is_leq(man,po,po1);
      fprintf(stdout,"pk_is_leq(po,po1)=%d\n",b);
      assert(b);
      b = pk_is_leq(man,po1,po);
      fprintf(stdout,"pk_is_leq(po1,po)=%d\n",b);
      pk_free(man,po);

      fprintf(stdout,"po=po1; meet_lincons_with(po) ");
      ap_lincons0_fprint(stdout,&cons,name_of_dim);
      fprintf(stdout,"\n");
      po = pk_copy(man,po1);
      po = pk_meet_lincons_array(man,true,po,&array);
      pk_fprint(stdout,man,po,name_of_dim);
      garray = pk_to_generator_array(man,po);
      ap_generator0_array_fprint(stdout,&garray,name_of_dim);
      ap_generator0_array_clear(&garray);
      b = pk_is_leq(man,po,po1);
      fprintf(stdout,"pk_is_leq(po,po1)=%d\n",b);
      assert(b);
      b = pk_is_leq(man,po1,po);
      fprintf(stdout,"pk_is_leq(po1,po)=%d\n",b);
      pk_free(man,po);
    }
    ap_linexpr0_free(expr[i]);
  }

  /* Additions of rays */
  fprintf(stdout,"********* Addition of rays *********\n");
  fprintf(stdout,"po1:\n");
  pk_fprint(stdout,man,po1,name_of_dim);
  garray = pk_to_generator_array(man,po1);
  ap_generator0_array_fprint(stdout,&garray,name_of_dim);
  ap_generator0_array_clear(&garray);

  /* expression z+w/8 */
  expr[0] = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,2);
  ap_linexpr0_set_list(expr[0],
		       AP_COEFF_S_INT, 1, 2,
		       AP_COEFF_S_FRAC, 1,8, 3,
		       AP_END);
  /* expression z+w */
  expr[1] = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,2);
  ap_linexpr0_set_list(expr[1],
		       AP_COEFF_S_INT, 1, 2,
		       AP_COEFF_S_INT, 1, 3,
		       AP_END);
  garray2 = ap_generator0_array_make(1);
  for (i=0; i<2; i++){
    gen.linexpr0 = expr[i];
    gen.gentyp = AP_GEN_RAY;
    garray2.p[0] = gen;
    fprintf(stdout,"po=add_ray_array(po1) ");
    ap_generator0_fprint(stdout,&gen,name_of_dim);
    fprintf(stdout,"\n");
    po = pk_add_ray_array(man,false,po1,&garray2);
    pk_fprint(stdout,man,po,name_of_dim);
    garray = pk_to_generator_array(man,po);
    ap_generator0_array_fprint(stdout,&garray,name_of_dim);
    ap_generator0_array_clear(&garray);
    b = pk_is_leq(man,po,po1);
    fprintf(stdout,"pk_is_leq(po,po1)=%d\n",b);
    b = pk_is_leq(man,po1,po);
    fprintf(stdout,"pk_is_leq(po1,po)=%d\n",b);
    assert(b);
    pk_free(man,po);
    
    fprintf(stdout,"po=po1; add_ray_array_with(po) ");
    ap_generator0_fprint(stdout,&gen,name_of_dim);
    fprintf(stdout,"\n");
    po = pk_copy(man,po1);
    po = pk_add_ray_array(man,true,po,&garray2);
    pk_fprint(stdout,man,po,name_of_dim);
    garray = pk_to_generator_array(man,po);
    ap_generator0_array_fprint(stdout,&garray,name_of_dim);
    ap_generator0_array_clear(&garray);
    b = pk_is_leq(man,po,po1);
    fprintf(stdout,"pk_is_leq(po,po1)=%d\n",b);
    b = pk_is_leq(man,po1,po);
    fprintf(stdout,"pk_is_leq(po1,po)=%d\n",b);
    pk_free(man,po);
    ap_linexpr0_free(expr[i]);
  }
  garray2.p[0].linexpr0 = NULL;
  ap_generator0_array_clear(&garray2);
}


/* ********************************************************************** */
/* Polyhedra 3 (assignement and substitution of a single expression) */
/* ********************************************************************** */

void poly3(ap_manager_t* man, char** name_of_dim, pk_t* po1, pk_t* po2)
{
  bool b;
  pk_t* poly1;
  pk_t* poly2;
  pk_t* poly3;
  pk_t* tpo[2];
  ap_linexpr0_t* expr[3];
  ap_generator0_array_t garray;
  int i,j,inplace,undet;

  fprintf(stdout,"********* Assignement and Substitution, single *********\n");
  fprintf(stdout,"po1:\n");
  pk_fprint(stdout,man,po1,name_of_dim);
  garray = pk_to_generator_array(man,po1);
  ap_generator0_array_fprint(stdout,&garray,name_of_dim);
  ap_generator0_array_clear(&garray);
  fprintf(stdout,"po2:\n");
  pk_fprint(stdout,man,po2,name_of_dim);
  garray = pk_to_generator_array(man,po2);
  ap_generator0_array_fprint(stdout,&garray,name_of_dim);
  ap_generator0_array_clear(&garray);
  tpo[0] = po1;
  tpo[1] = po2;

  /* Single equation z+5w-1 */
  expr[0] = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,2);
  ap_linexpr0_set_list(expr[0],
		       AP_COEFF_S_INT, 1, 2,
		       AP_COEFF_S_INT, 5, 3,
		       AP_CST_S_INT, -1,
		       AP_END);

  /* Single equation 2x+1 */
  expr[1] = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,1);
  ap_linexpr0_set_coeff_scalar_int(expr[1],0,2);
  ap_linexpr0_set_cst_scalar_int(expr[1],1);
  expr[1]->p.linterm[0].dim = 0;

  for (undet=0; undet<2; undet++){
    if (undet==1){
      for (j=0;j<2;j++){
	ap_linexpr0_set_cst_interval_int(expr[j],-1,1);
      }
    }
    for (inplace=0; inplace<2; inplace++){
      for (i=0; i<2; i++){
	for (j=0;j<2;j++){
	  fprintf(stdout,
		  inplace ?
		  "poly1 = po%d; assign_linexpr_with(poly1) z:=" :
		  "poly1=assign_linexpr(po%d) z:=",
		  i+1);
	  ap_linexpr0_fprint(stdout,expr[j],name_of_dim);
	  fprintf(stdout,"\n");
	  if (inplace){
	    ap_dim_t dim = 2;
	    poly1 = pk_copy(man,tpo[i]);
	    poly1 = pk_assign_linexpr_array(man,true,poly1,&dim,&expr[j],1,NULL);
	  } else {
	    ap_dim_t dim = 2;
	    poly1 = pk_assign_linexpr_array(man,false,tpo[i],&dim,&expr[j],1,NULL);
	  }
	  pk_fprint(stdout,man,poly1,name_of_dim);
	  garray = pk_to_generator_array(man,poly1);
	  ap_generator0_array_fprint(stdout,&garray,name_of_dim);
	  ap_generator0_array_clear(&garray);

	  fprintf(stdout,
		  inplace ?
		  "poly2 = poly1; substitute_linexpr_with(poly2) z:=" :
		  "poly2=substitute_linexpr(poly1) z:="
		  );
	  ap_linexpr0_fprint(stdout,expr[j],name_of_dim);
	  fprintf(stdout,"\n");
	  if (inplace){
	    ap_dim_t dim = 2;
	    poly2 = pk_copy(man,poly1);
	    poly2 = pk_substitute_linexpr_array(man,true,poly2,&dim,&expr[j],1,NULL);
	  } else {
	    ap_dim_t dim = 2;
	    poly2 = pk_substitute_linexpr_array(man,false,poly1,&dim,&expr[j],1,NULL);
	  }
	  pk_fprint(stdout,man,poly2,name_of_dim);
	  garray = pk_to_generator_array(man,poly2);
	  ap_generator0_array_fprint(stdout,&garray,name_of_dim);
	  ap_generator0_array_clear(&garray);

	  b = pk_is_leq(man,tpo[i],poly2);
	  fprintf(stdout,"pk_is_leq(po%d,poly2)=%d\n",i+1,b);
	  assert(b);

	  b = pk_is_leq(man,poly2,tpo[i]);
	  fprintf(stdout,"pk_is_leq(poly2,po%d)=%d\n",i+1,b);
	  assert(!undet && j==0 ? b : true);

	  fprintf(stdout,
		  inplace ?
		  "poly3 = poly2; assign_linexpr_with(poly3) z:=" :
		  "poly3=assign_linexpr(poly2) z:="
		  );
	  ap_linexpr0_fprint(stdout,expr[j],name_of_dim);
	  fprintf(stdout,"\n");
	  if (inplace){
	    ap_dim_t dim = 2;
	    poly3 = pk_copy(man,poly2);
	    poly3 = pk_assign_linexpr_array(man,true,poly3,&dim,&expr[j],1,NULL);
	  } else {
	    ap_dim_t dim = 2;
	    poly3 = pk_assign_linexpr_array(man,false,poly2,&dim,&expr[j],1,NULL);
	  }
	  pk_fprint(stdout,man,poly3,name_of_dim);
	  garray = pk_to_generator_array(man,poly3);
	  ap_generator0_array_fprint(stdout,&garray,name_of_dim);
	  ap_generator0_array_clear(&garray);

	  b = pk_is_eq(man,poly1,poly3);
	  fprintf(stdout,"pk_is_eq(poly1,poly3)=%d\n",b);
	  assert(!(undet && j==0) ? b : true);
       
	  pk_free(man,poly1);
	  pk_free(man,poly2);
	  pk_free(man,poly3);
	}
      }
    }
  }
  ap_linexpr0_free(expr[0]);
  ap_linexpr0_free(expr[1]);
}

void pk_test_example_aux(ap_manager_t* man)
{
  pk_t* p1;
  pk_t* p2;
  char** name_of_dim;
  ap_dimperm_t perm;
  name_of_dim = malloc(6*sizeof(char*));
  name_of_dim[0] = "x";
  name_of_dim[1] = "y";
  name_of_dim[2] = "z";
  name_of_dim[3] = "w";
  name_of_dim[4] = "u";
  name_of_dim[5] = "v";
  ap_dimperm_init(&perm,6);
  ap_dimperm_set_id(&perm);
  perm.dim[0] = 2;
  perm.dim[2] = 0;
  perm.dim[3] = 5;
  perm.dim[5] = 3;

  poly1(man,name_of_dim, &p1, &p2);

  assert(poly_check((pk_internal_t*)man->internal,p1));
  assert(poly_check((pk_internal_t*)man->internal,p2));
  poly2(man,name_of_dim,p1,p2);

  p2 = pk_permute_dimensions(man,true,p2,&perm);
  poly2(man,name_of_dim,p1,p2);

  poly3(man,name_of_dim,p1,p2);

  pk_free(man,p2);
  p2 = pk_bottom(man,0,6);
  poly2(man,name_of_dim,p1,p2);
  poly2(man,name_of_dim,p2,p1);
  poly3(man,name_of_dim,p2,p1);

  pk_free(man,p1);
  pk_free(man,p2);

  /* */
  free(name_of_dim);
  ap_dimperm_clear(&perm);
}

void pk_test_example(void)
{
  pk_internal_t* pk;
  ap_manager_t* man;
  numint_t num;
  ap_funid_t funid;

  man = pk_manager_alloc(true);
  pk = (pk_internal_t*)man->internal;
  pk_internal_realloc_lazy(pk,20);

  numint_init(num);
  numint_set_int(num,1);
  vector(pk,7,num);
  numint_clear(num);

 
  pk_test_example_aux(man);

  for (funid=0; funid<AP_FUNID_SIZE; funid++){
    ap_funopt_t funopt;
    ap_funopt_init(&funopt);
    funopt.algorithm = -1;
    ap_manager_set_funopt(man,funid,&funopt);
  }

  pk_test_example_aux(man);

  ap_manager_free(man);
}

/* ********************************************************************** */
/* Polyhedra random */
/* ********************************************************************** */

ap_linexpr0_t* expr_random(size_t intdim, size_t realdim, 
		       size_t maxcoeff, /* Maximum size of non-null coefficients */
		       unsigned int mag /* magnitude of coefficients */
		       )
{
  ap_linexpr0_t* expr;
  unsigned long int r;
  size_t j,nbcoeff;
  ap_dim_t dim;  
  double coeff;

  /* Expression */
  expr = ap_linexpr0_alloc(AP_LINEXPR_DENSE,intdim+realdim);
  ap_linexpr0_set_cst_scalar_double(expr,0.0);
  for (j=0; j<intdim+realdim; j++){
    ap_linexpr0_set_coeff_scalar_double(expr,j,0.0);
  }
  /* Fill the expression */
  r = rand();
  coeff = (double)( (long int)(r % (2*mag)) - (long int)mag);
  ap_linexpr0_set_cst_scalar_double(expr,coeff);

  r = rand();
  nbcoeff = (r % maxcoeff) + 1;
  for (j=0; j<nbcoeff; j++){
    r = rand();
    dim = r % (intdim+realdim);
    r = rand();
    coeff = (double)( (long int)(r % (2*mag)) - (long int)mag);
    ap_linexpr0_set_coeff_scalar_double(expr,dim,coeff);
  }
  return expr;
}

pk_t* pk_random(ap_manager_t* man, size_t intdim, size_t realdim, 
		    size_t nbcons, /* Number of constraints */
		    size_t maxeq, /* Maximum number of equations */
		    size_t maxcoeff, /* Maximum size of non-null coefficients */
		    unsigned int mag /* magnitude of coefficients */
		    )
{
  ap_lincons0_array_t array;
  long int r;
  ap_lincons0_t cons;
  ap_linexpr0_t* expr;
  pk_t* poly;
  size_t i;

  array = ap_lincons0_array_make(nbcons);
  r = rand();
  maxeq = r % (maxeq+1);
  for (i=0; i<nbcons; i++){
    expr = expr_random(intdim,realdim,maxcoeff,mag);
    /* Equality or inequality ? */
    if (i<maxeq){
      cons.constyp = AP_CONS_EQ;
    } else {
      r = rand();
      cons.constyp = (r%3==0) ? AP_CONS_SUP : AP_CONS_SUPEQ;
    }
    cons.linexpr0 = expr;
    array.p[i] = cons;
  }
  poly = pk_top(man,intdim,realdim);
  poly = pk_meet_lincons_array(man,true,poly,&array);
  ap_lincons0_array_clear(&array);
  return poly;
}


/* ********************************************************************** */
/* Approximate */
/* ********************************************************************** */

void test_approximate(void)
{
  ap_linexpr0_t* expr;
  ap_lincons0_t cons;
  ap_lincons0_array_t array = { &cons, 1 };
  pk_t* po, *pa;
  pk_internal_t* pk;
  ap_manager_t* man;
  size_t mag = 1000000;
  ap_coeff_t* coeff;

  man = pk_manager_alloc(true);
  pk = (pk_internal_t*)man->internal;
  pk_set_approximate_max_coeff_size(pk,10);

  /* Build a polyhedron x>=0, x<=mag, x-(mag*mag)y>=0 */
  po = pk_top(man,0,2);

  /* Expression */
  expr = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,1);
  cons.constyp = AP_CONS_SUPEQ;
  cons.linexpr0 = expr;
  ap_linexpr0_set_cst_scalar_int(expr,0);
  ap_linexpr0_set_coeff_scalar_int(expr,0,1);
  po = pk_meet_lincons_array(man,true,po,&array);
  ap_linexpr0_set_cst_scalar_int(expr,(int)mag);
  ap_linexpr0_set_coeff_scalar_int(expr,0,-1);
  po = pk_meet_lincons_array(man,true,po,&array);
  ap_linexpr0_free(expr);
  expr = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,2);
  cons.constyp = AP_CONS_SUPEQ;
  cons.linexpr0 = expr;
  ap_linexpr0_set_cst_scalar_int(expr,0);
  ap_linexpr0_set_list(expr,
		       AP_COEFF_S_INT, 1, 0,
		       AP_COEFF_S_INT, mag, 1,
		       AP_END);
  coeff = ap_linexpr0_coeffref(expr,1);
  mpq_mul(coeff->val.scalar->val.mpq,
	  coeff->val.scalar->val.mpq,
	  coeff->val.scalar->val.mpq);
  mpq_neg(coeff->val.scalar->val.mpq,
	  coeff->val.scalar->val.mpq);
  po = pk_meet_lincons_array(man,true,po,&array);
  ap_linexpr0_free(expr);

  pa = pk_copy(man,po);
  pk_approximate(man,pa,3);
  pk_fprint(stdout,man,po,NULL);
  pk_fprint(stdout,man,pa,NULL);
  assert(pk_is_leq(man,po,pa));
  pk_free(man,pa);

  pa = pk_copy(man,po);
  pk_approximate(man,pa,10);
  pk_fprint(stdout,man,po,NULL);
  pk_fprint(stdout,man,pa,NULL);
  assert(pk_is_leq(man,po,pa));
  pk_free(man,pa);

  pk_free(man,po);
  ap_manager_free(man);
  return;
}


/* ********************************************************************** */
/* Polyhedra 10 */
/* ********************************************************************** */

void pk_test_gen(ap_manager_t* man, size_t intdim, size_t realdim, 
		   size_t nbcons, /* Number of constraints */
		   size_t maxeq, /* Maximum number of equations */
		   size_t maxcoeff, /* Maximum size of non-null coefficients */
		   unsigned int mag, /* magnitude of coefficients */
		   char*** pname_of_dim,
		   pk_t*** ptpoly, /* of size 6, 0==2, 1==3 */
		   ap_linexpr0_t*** ptexpr, /* of size 3 */
		   ap_dim_t** ptdim /* of size 3 */
		   )
{
  char buffer[80];
  size_t i;  
  pk_t* p1;
  pk_t* p2;

  *pname_of_dim = malloc((intdim+realdim)*sizeof(char*));
  for (i=0; i<intdim; i++){
    snprintf(buffer,sizeof(buffer),"n%d",i);
    (*pname_of_dim)[i] = strdup(buffer);
  }
  for (i=0; i<realdim; i++){
    snprintf(buffer,sizeof(buffer),"r%d",i);
    (*pname_of_dim)[intdim+i] = strdup(buffer);
  }
  p1 = pk_random(man,intdim,realdim,nbcons,maxeq,maxcoeff,mag);
  p2 = pk_random(man,intdim,realdim,nbcons,maxeq,maxcoeff,mag);
  *ptpoly = malloc(6*sizeof(pk_t*));
  (*ptpoly)[0] = (*ptpoly)[2] = p1;
  (*ptpoly)[1] = (*ptpoly)[3] = p2;
  (*ptpoly)[4] = pk_random(man,intdim,realdim,nbcons,maxeq,maxcoeff,mag);
  (*ptpoly)[5] = pk_random(man,intdim,realdim,nbcons,maxeq,maxcoeff,mag);
  *ptexpr = malloc(3*sizeof(ap_linexpr0_t*));
  for (i=0;i<3;i++){
    (*ptexpr)[i] = expr_random(intdim,realdim,maxcoeff,mag);
  }
  *ptdim = malloc(3*sizeof(ap_dim_t));
  (*ptdim)[0] = 0;
  (*ptdim)[1] = 1;
  (*ptdim)[2] = intdim+realdim-1;
  assert(intdim+realdim>=3);
}

void pk_test_free(ap_manager_t* man, size_t intdim, size_t realdim, 
		    char** name_of_dim,
		     pk_t** tpoly,
		     ap_linexpr0_t** texpr,
		     ap_dim_t* tdim)
{
  size_t j;

  for (j=0;j<intdim+realdim;j++){
    free(name_of_dim[j]);
  }
  free(name_of_dim);
  pk_free(man,tpoly[0]);
  pk_free(man,tpoly[1]);
  pk_free(man,tpoly[4]);
  pk_free(man,tpoly[5]);
  free(tpoly);
  for (j=0;j<3;j++){
    ap_linexpr0_free(texpr[j]);
  }
  free(texpr);
  free(tdim);
}



//#define PRINT(a)
#define PRINT(a) a

void pk_test_check(ap_manager_t* man, size_t intdim, size_t realdim,
		     char** name_of_dim,
		     pk_t** tpoly,
		     ap_linexpr0_t** texpr,
		     ap_dim_t* tdim)
{
  pk_t* p1;
  pk_t* p2;
  pk_t* p3;
  pk_t* p4;
  pk_t* p5;
  ap_dimperm_t perm;
  size_t i,k;
  mpq_t mpq, mpqone;
  ap_coeff_t* pcoeff;
  ap_interval_t* interval;
  ap_lincons0_t cons;
  ap_lincons0_array_t consarray = { &cons, 1 };
  ap_lincons0_array_t array;
  ap_generator0_array_t garray;
  double d;
  double* pdbl;
  ap_interval_t** box;
  ap_linexpr0_t* expr;
  bool res;
  ap_dim_t dim;
  ap_dimchange_t* dimchange;

  pk_internal_t* pk = pk_manager_get_internal(man);
  pk_set_approximate_max_coeff_size(pk,10);
  mpq_init(mpq);
  mpq_set_si(mpq,0,1);
  mpq_init(mpqone);
  mpq_set_si(mpqone,1,1);

  for (i=0;i<6; i++){
    PRINT((printf("poly %d:\n",i)));
    PRINT((pk_fprint(stdout,man,tpoly[i],name_of_dim)));
  }
  for (i=0;i<3; i++){
    PRINT((printf("%d: dim = %d\nexpr = ",i,tdim[i])));
    PRINT((ap_linexpr0_fprint(stdout,texpr[i],name_of_dim)));
    PRINT((printf("\n")));
  }
      
  p1 = tpoly[0];
  p2 = tpoly[1];
  
  /* minimize */
  PRINT((printf("minimize\n")));
  p3 = pk_copy(man,p1);
  pk_minimize(man,p3);
  assert(pk_is_eq(man,p1,p3));
  pk_free(man,p3);

  /* canonicalize */
  PRINT((printf("canonicalize\n")));
  p3 = pk_copy(man,p1);
  pk_canonicalize(man,p3);
  assert(pk_is_eq(man,p1,p3));
  assert(pk_is_canonical(man,p3));
  pk_free(man,p3);

  /* approximate */
  p3 = pk_copy(man,p1);
  pk_approximate(man,p3,0);
  assert(pk_is_eq(man,p1,p3));
  pk_free(man,p3);
  p3 = pk_copy(man,p1);
  pk_approximate(man,p3,-1);
  pk_fprint(stdout,man,p1,name_of_dim);
  pk_fprint(stdout,man,p3,name_of_dim);
  assert(pk_is_leq(man,p3,p1));
  if (pk_is_leq(man,p1,p3)!=true){
    printf("approximate(-1) smaller\n");
  }
  pk_free(man,p3);

  p3 = pk_copy(man,p1);
  pk_approximate(man,p3,3);
  pk_fprint(stdout,man,p1,name_of_dim);
  pk_fprint(stdout,man,p3,name_of_dim);
  assert(pk_is_leq(man,p1,p3));
  if (pk_is_leq(man,p3,p1)!=true){
    printf("approximate(3) greater\n");
    assert(man->result.flag_exact!=true);
  }
  pk_free(man,p3);

  p3 = pk_copy(man,p1);
  pk_approximate(man,p3,10);
  pk_fprint(stdout,man,p1,name_of_dim);
  pk_fprint(stdout,man,p3,name_of_dim);
  assert(pk_is_leq(man,p1,p3));
  if (pk_is_leq(man,p3,p1)!=true){
    printf("approximate(10) greater\n");
    assert(man->result.flag_exact!=true);
  }
  pk_free(man,p3);

  /* bound_dimension and sat_interval */
  PRINT((printf("bound_dimension and sat_interval\n")));
  for (i=0;i<intdim+realdim; i++){
    interval = pk_bound_dimension(man,p1,i);
    assert(pk_sat_interval(man,p1,i,interval));
    if (!ap_scalar_infty(interval->inf) && !ap_scalar_infty(interval->sup)){
      ap_mpq_set_scalar(mpq,interval->inf,0);
      mpq_add(mpq,mpq,mpqone);
      ap_scalar_set_mpq(interval->inf,mpq);
      ap_mpq_set_scalar(mpq,interval->sup,0);
      mpq_sub(mpq,mpq,mpqone);
      ap_scalar_set_mpq(interval->sup,mpq);
      assert(pk_sat_interval(man,p1,i,interval)==false || pk_is_bottom(man,p1));
    }
    if (!ap_scalar_infty(interval->inf)){
      ap_mpq_set_scalar(mpq,interval->inf,0);
      mpq_sub(mpq,mpq,mpqone);
      mpq_sub(mpq,mpq,mpqone);
      ap_scalar_set_mpq(interval->inf,mpq);
    }
    if (!ap_scalar_infty(interval->sup)){
      ap_mpq_set_scalar(mpq,interval->sup,0);
      mpq_add(mpq,mpq,mpqone);
      mpq_add(mpq,mpq,mpqone);
      ap_scalar_set_mpq(interval->sup,mpq);
    }
    assert(pk_sat_interval(man,p1,i,interval));
    ap_interval_free(interval);
  }

  /* bound_linexpr and sat_lincons */
  PRINT((printf("bound_linexpr and sat_lincons\n")));
  for (k=0; k<3; k++){
    expr = texpr[k];
    cons.constyp = AP_CONS_SUPEQ;
    cons.linexpr0 = expr;
    pcoeff = ap_linexpr0_cstref(expr);
    pdbl = &pcoeff->val.scalar->val.dbl;
    d = *pdbl;
    interval = pk_bound_linexpr(man,p1,expr);
    if (!ap_scalar_infty(interval->inf)){
      ap_double_set_scalar(pdbl,interval->inf,GMP_RNDZ);
      *pdbl = d + 1.0 - *pdbl;
      assert(pk_sat_lincons(man,p1,&cons));

      ap_double_set_scalar(pdbl,interval->inf,GMP_RNDZ);      
      *pdbl = d - 1.0 - *pdbl;
      assert(pk_sat_lincons(man,p1,&cons)==false ||
	     pk_is_bottom(man,p1));
    }
    if (!ap_scalar_infty(interval->sup)){
      for (i=0; i<intdim+realdim; i++){
	ap_coeff_neg(&expr->p.coeff[i],&expr->p.coeff[i]);
      }
      ap_double_set_scalar(pdbl,interval->sup,GMP_RNDZ);
      *pdbl = *pdbl - d + 1.0;
      assert(pk_sat_lincons(man,p1,&cons));

      ap_double_set_scalar(pdbl,interval->sup,GMP_RNDZ);
      *pdbl = *pdbl - d - 1.0;
      assert(pk_sat_lincons(man,p1,&cons)==false ||
	     pk_is_bottom(man,p1));
    }
    *pdbl = d;
    ap_interval_free(interval);
  }

  /* to_box */
  PRINT((printf("to_box\n")));
  box = pk_to_box(man,p1);
  for (i=0;i<intdim+realdim; i++){
    assert(pk_sat_interval(man,p1,i,box[i]));
  }
  p3 = pk_of_box(man,intdim,realdim,(ap_interval_t**)box);
  if (pk_is_leq(man,p1,p3)!=true &&
      pk_is_bottom(man,p1)==false){
    if (intdim==0) assert(false);
    else {
      printf("of_box(to_box) not greater\n");
    }
  }
  pk_free(man,p3);
  ap_interval_array_free(box,intdim+realdim);

  /* to_lincons_array */
  PRINT((printf("to_lincons_array\n")));
  array = pk_to_lincons_array(man,p1);
  for (k=0; k<array.size; k++){
    res = pk_sat_lincons(man,p1,&array.p[k]);
    if (intdim==0) assert(res);
    else if (res!=true){
      printf("sat_lincons(to_lincons) not true\n");
    }
  }
  p3 = pk_top(man,intdim,realdim);
  p3 = pk_meet_lincons_array(man,true,p3,&array);
  assert(pk_is_leq(man,p3,p1));
  if (pk_is_leq(man,p1,p3)!=true){
    if (intdim==0) assert(false);
    else
      printf("of_lincons(to_lincons) smaller\n");
  }
  pk_free(man,p3);
  ap_lincons0_array_clear(&array);

  /* meet, meet_with, meet_array, join, join_with, join_array */
  PRINT((printf("meet, meet_with\n")));
  p3 = pk_meet(man,false,p1,p2);
  p4 = pk_copy(man,p1);
  p4 = pk_meet(man,true,p4,p2);
  assert(pk_is_eq(man,p3,p4));
  assert(pk_is_leq(man,p3,p1));
  assert(pk_is_leq(man,p3,p2));
  pk_free(man,p4);
  PRINT((printf("meet_array\n")));
  p4 = pk_meet_array(man,tpoly,6);
  for (k=0;k<6;k++){
    assert(pk_is_leq(man,p4,tpoly[k]));
  }
  assert(pk_is_leq(man,p4,p3));
  pk_free(man,p3);
  pk_free(man,p4);

  PRINT((printf("join, join_with\n")));
  p3 = pk_join(man,false,p1,p2);
  p4 = pk_copy(man,p1);
  p4 = pk_join(man,true,p4,p2);
  assert(pk_is_eq(man,p3,p4));
  assert(pk_is_leq(man,p1,p3));
  assert(pk_is_leq(man,p2,p3));
  pk_free(man,p4);
  PRINT((printf("join_array\n")));
  count++;
  p4 = pk_join_array(man,tpoly,6);
  pk_fprint(stdout,man,p4,name_of_dim);
  assert(pk_is_leq(man,p3,p4));
  for (k=0;k<6;k++){
    printf("k = %d\n",k);
    assert(pk_is_leq(man,tpoly[k],p4));
  }
  /* approximate */
  PRINT((printf("approximate on the result of join_array\n")));
  p5 = pk_copy(man,p4);
  pk_approximate(man,p5,0);
  assert(pk_is_eq(man,p4,p5));
  pk_free(man,p5);

  p5 = pk_copy(man,p4);
  pk_approximate(man,p5,-1);
  pk_fprint(stdout,man,p5,name_of_dim);
  assert(pk_is_leq(man,p5,p4));
  if (pk_is_leq(man,p4,p5)!=true){
    printf("approximate(-1) smaller\n");
  }
  pk_free(man,p5);

  p5 = pk_copy(man,p4);
  pk_approximate(man,p5,3);
  res = man->result.flag_exact;
  pk_fprint(stdout,man,p5,name_of_dim);
  assert(pk_is_leq(man,p4,p5));
  if (pk_is_leq(man,p5,p4)!=true){
    printf("approximate(3) greater\n");
    assert(res!=true);
  }
  else {
    assert(res);
  }
  pk_free(man,p5);

  printf("approximate(10)\n");
  p5 = pk_copy(man,p4);
  pk_approximate(man,p5,10);
  res = man->result.flag_exact;
  pk_fprint(stdout,man,p5,name_of_dim);
  assert(pk_is_leq(man,p4,p5));
  if (pk_is_leq(man,p5,p4)!=true){
    printf("approximate(10) greater\n");
    assert(res!=true);
  }
  else {
    assert(res);
  }
  pk_free(man,p5);

  pk_free(man,p3);
  pk_free(man,p4);

  /* meet_lincons, add_ray_array */
  PRINT((printf("meet_lincons, add_ray_array\n"))); 
  for (k=0; k<3; k++){
    expr = texpr[k];
    cons.constyp = AP_CONS_SUPEQ;
    cons.linexpr0 = expr;
    p3 = pk_meet_lincons_array(man,false,p1,&consarray);
    p4 = pk_copy(man,p1);
    p4 = pk_meet_lincons_array(man,true,p4,&consarray);
    assert(pk_is_eq(man,p3,p4));
    assert(pk_is_leq(man,p3,p1));
    pk_free(man,p3);
    pk_free(man,p4);
    garray = ap_generator0_array_make(1);
    garray.p[0].gentyp = AP_GEN_RAY;
    garray.p[0].linexpr0 = expr;
    p3 = pk_add_ray_array(man,false,p1,&garray);
    p4 = pk_copy(man,p1);
    p4 = pk_add_ray_array(man,true,p4,&garray);
    assert(pk_is_eq(man,p3,p4));
    assert(pk_is_leq(man,p1,p3));
    free(garray.p);
    pk_free(man,p3);
    pk_free(man,p4);
  }
  /* assign and substitute (single, deterministic) */
  PRINT((printf("assign and substitute (single, deterministic)\n"))); 
  for (k=0; k<3; k++){
    dim = tdim[k];
    expr = texpr[k];
    p3 = pk_assign_linexpr_array(man,false,p1,&dim,&expr,1,NULL);
    p4 = pk_copy(man,p1);
    p4 = pk_assign_linexpr_array(man,true,p4,&dim,&expr,1,NULL);
    assert(pk_is_eq(man,p3,p4));
    pk_free(man,p4);
    p4 = pk_substitute_linexpr_array(man,false,p3,&dim,&expr,1,NULL);
    p5 = pk_copy(man,p3);
    p5 = pk_substitute_linexpr_array(man,true,p5,&dim,&expr,1,NULL);
    assert(pk_is_eq(man,p4,p5));
    pk_free(man,p5);
    p5 = pk_assign_linexpr_array(man,false,p4,&dim,&expr,1,NULL);
    assert(pk_is_leq(man,p1,p4));
    assert(pk_is_eq(man,p3,p5));
    pk_free(man,p3);
    pk_free(man,p4);
    pk_free(man,p5);
  }
  /* parallel assign and substitute (deterministic) */
  PRINT((printf("parallel assign and substitute (deterministic)\n"))); 
  p3 = pk_assign_linexpr_array(man,false,p1,tdim,texpr,3,NULL);
  p4 = pk_copy(man,p1);
  p4 = pk_assign_linexpr_array(man,true,p4,tdim,texpr,3,NULL);
  assert(pk_is_eq(man,p3,p4));
  pk_free(man,p4);
  p4 = pk_substitute_linexpr_array(man,false,p3,tdim,texpr,3,NULL);
  p5 = pk_copy(man,p3);
  p5 = pk_substitute_linexpr_array(man,true,p5,tdim,texpr,3,NULL);
  assert(pk_is_eq(man,p4,p5));
  pk_free(man,p5);
  p5 = pk_assign_linexpr_array(man,false,p4,tdim,texpr,3,NULL);
  assert(pk_is_leq(man,p1,p4));
  assert(pk_is_eq(man,p3,p5));
  pk_free(man,p3);
  pk_free(man,p4);
  pk_free(man,p5);

  /* project and forget */
  PRINT((printf("project and forget\n"))); 
  box = pk_to_box(man,p1);
  for (k=0; k<3; k++){
    dim = tdim[k];
    p3 = pk_forget_array(man,false,p1,&dim,1,true);
    p4 = pk_copy(man,p1);
    p4 = pk_forget_array(man,true,p4,&dim,1,true);  
    assert(pk_is_eq(man,p3,p4));
    pk_free(man,p4);
    p4 = pk_forget_array(man,false,p1,&dim,1,false);
    p5 = pk_copy(man,p1);
    pk_forget_array(man,true,p5,&dim,1,false);  
    assert(pk_is_eq(man,p4,p5));
    pk_free(man,p5);
    assert(pk_is_leq(man,p3,p4));
    assert(pk_is_leq(man,p4,p3)==false || 
	   pk_is_bottom(man,p1));

    interval = pk_bound_dimension(man,p3,dim);
    assert(ap_scalar_equal(interval->inf,interval->sup) || 
	   pk_is_bottom(man,p1));
    assert(ap_scalar_cmp_int(interval->inf,0)==0 || 
	   pk_is_bottom(man,p1));
    ap_interval_free(interval);
    interval = pk_bound_dimension(man,p4,dim);
    assert(ap_interval_is_top(interval) || 
	   pk_is_bottom(man,p1));
    ap_interval_free(interval);
    for (i=0; i<intdim+realdim; i++){
      if (i!=dim){
	interval = pk_bound_dimension(man,p3,i);
	assert(ap_interval_is_leq(interval,box[i]));
	assert(ap_interval_is_leq(box[i],interval));
	ap_interval_free(interval);
	interval = pk_bound_dimension(man,p4,i);
	assert(ap_interval_is_leq(interval,box[i]) || 
	       pk_is_bottom(man,p1));
	assert(ap_interval_is_leq(box[i],interval) || 
	       pk_is_bottom(man,p1));
	ap_interval_free(interval);
      }
    }
    pk_free(man,p3);
    pk_free(man,p4);
  }
  ap_interval_array_free(box,intdim+realdim);
  /* project and forget array */
  PRINT((printf("project and forget array\n"))); 
  p3 = pk_forget_array(man,false,p1,tdim,3,true);
  p4 = pk_copy(man,p1);
  p4 = pk_forget_array(man,true,p4,tdim,3,true);  
  assert(pk_is_eq(man,p3,p4));
  pk_free(man,p4);
  p4 = pk_forget_array(man,false,p1,tdim,3,false);
  p5 = pk_copy(man,p1);
  p5 = pk_forget_array(man,true,p5,tdim,3,false);  
  assert(pk_is_eq(man,p4,p5));
  pk_free(man,p5);
  assert(pk_is_leq(man,p3,p4));
  assert(pk_is_leq(man,p4,p3)==false || 
	   pk_is_bottom(man,p1));
  for (k=0;k<3;k++){
    dim = tdim[k];
    interval = pk_bound_dimension(man,p3,dim);
    assert(ap_scalar_equal(interval->inf,interval->sup)|| 
	   pk_is_bottom(man,p1));
    assert(ap_scalar_cmp_int(interval->inf,0)==0|| 
	   pk_is_bottom(man,p1));
    ap_interval_free(interval);
    interval = pk_bound_dimension(man,p4,dim);
    assert(ap_interval_is_top(interval) || 
	   pk_is_bottom(man,p1));
    ap_interval_free(interval);
  }  
  pk_free(man,p3);
  pk_free(man,p4);
  
  /* change and permutation of dimensions */
  PRINT((printf("change and permutation of dimensions\n"))); 
  ap_dimperm_init(&perm,(intdim+realdim)*2);
  ap_dimperm_set_id(&perm);
  for (i=0;i<2*intdim; i++){
    perm.dim[i] = 2*intdim-1-i;
  }
  for (i=0;i<2*realdim; i++){
    perm.dim[2*intdim+i] = 2*(intdim+realdim)-1-i;
  }
  dimchange = ap_dimchange_alloc(intdim,realdim);
  for (i=0;i<intdim+realdim;i++){
    dimchange->dim[i]=i;
  }
  
  p3 = pk_add_dimensions(man,false,p1,dimchange,true);
  p4 = pk_copy(man,p1);
  p4 = pk_add_dimensions(man,true,p4,dimchange,true);
  assert(pk_is_eq(man,p3,p4));
  pk_free(man,p3);
  pk_free(man,p4);
  p3 = pk_add_dimensions(man,false,p1,dimchange,false);
  p4 = pk_copy(man,p1);
  p4 = pk_add_dimensions(man,true,p4,dimchange,false);
  assert(pk_is_eq(man,p3,p4));
  pk_free(man,p4);
  p4 = pk_permute_dimensions(man,false,p3,&perm);
  p5 = pk_copy(man,p3);
  p5 = pk_permute_dimensions(man,true,p5,&perm);
  assert(pk_is_eq(man,p4,p5));
  p5 = pk_permute_dimensions(man,true,p5,&perm);
  assert(pk_is_eq(man,p3,p5));
  pk_free(man,p4);
  pk_free(man,p5);
  ap_dimchange_add_invert(dimchange);
  p4 = pk_remove_dimensions(man,false,p3,dimchange);
  assert(pk_is_eq(man,p1,p4));
  p5 = pk_copy(man,p3);
  p5 = pk_remove_dimensions(man,true,p5,dimchange);
  assert(pk_is_eq(man,p4,p5));
  pk_free(man,p3);
  pk_free(man,p4);
  pk_free(man,p5);

  ap_dimperm_clear(&perm);
  ap_dimchange_free(dimchange);
  mpq_clear(mpq);
  mpq_clear(mpqone);
}

void pk_test(size_t intdim, size_t realdim,
	       size_t nbcons, /* Number of constraints */
	       size_t maxeq, /* Maximum number of equations */
	       size_t maxcoeff, /* Maximum size of non-null coefficients */
	       unsigned int mag /* magnitude of coefficients */
	       )
{
  pk_internal_t* pk;
  ap_manager_t* man;
  char** name_of_dim;
  pk_t** tpoly;
  ap_linexpr0_t** texpr;
  ap_dim_t* tdim;
  int i;
  ap_funid_t funid;

  man = pk_manager_alloc(false);

  pk = (pk_internal_t*)man->internal;
  pk_set_max_coeff_size(pk,0);
  pk_set_approximate_max_coeff_size(pk,10);
  
  for (i=0; i<3; i++){
    printf("%d******************************************************************\n",i);
    pk_test_gen(man,intdim,realdim,
		  nbcons,maxeq,maxcoeff,mag,
		  &name_of_dim,
		  &tpoly,
		  &texpr,
		  &tdim);
    pk_test_check(man,intdim,realdim,
		    name_of_dim,tpoly,texpr,tdim);

    for (funid=0; funid<AP_FUNID_SIZE; funid++){
      ap_funopt_t funopt;
      ap_funopt_init(&funopt);
      funopt.algorithm = -1;
      ap_manager_set_funopt(man,funid,&funopt);
    }
    
    pk_test_check(man,intdim,realdim,
		    name_of_dim,tpoly,texpr,tdim);
    
    pk_test_free(man,intdim,realdim,
		   name_of_dim,tpoly,texpr,tdim);
  }
  ap_manager_free(man);
}

void mine2(void)
{
  ap_manager_t* man;
  ap_funopt_t funopt;
  /* Creation du polyèdre 4x1 + 4x2 + 17 >= 0 */
  ap_lincons0_array_t array;
  pk_t* poly;
  pk_t* poly2;
  pk_t* poly3;

  man = pk_manager_alloc(false);
  array = ap_lincons0_array_make(1);

  /* 1. Constraint system */
  array.p[0].constyp = AP_CONS_SUPEQ;
  array.p[0].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,2);
  ap_linexpr0_set_list(array.p[0].linexpr0,
		       AP_COEFF_S_INT,4,1,
		       AP_COEFF_S_INT,4,2,
		       AP_CST_S_INT,17,
		       AP_END);
  /* Creation */
  ap_lincons0_array_fprint(stdout,&array,NULL);
  poly = pk_top(man,0,3);
  poly = pk_meet_lincons_array(man,true,poly,&array);
  pk_fprint(stdout,man,poly,NULL);

  poly2 = pk_bottom(man,0,3);
  pk_fprint(stdout,man,poly2,NULL);
  poly3 = pk_join(man,false,poly,poly2);
  pk_fprint(stdout,man,poly3,NULL);

  funopt = ap_manager_get_funopt(man, AP_FUNID_JOIN);
  funopt.flag_exact_wanted = true;
  funopt.flag_best_wanted = true;
  ap_manager_set_funopt(man, AP_FUNID_JOIN, &funopt);
  pk_free(man,poly3);
  poly3 = pk_join(man,false,poly,poly2);
  pk_fprint(stdout,man,poly3,NULL);
  
  pk_free(man,poly2);
  pk_free(man,poly);
  ap_lincons0_array_clear(&array);
  ap_manager_free(man);
}

void mine3(void)
{
  ap_manager_t* man;
  ap_funopt_t funopt;
  ap_lincons0_array_t array;
  pk_t* poly0;
  pk_t* poly1;
  pk_t* poly2;
  pk_t* poly3;
  pk_t* poly;
  pk_t* tpoly[5];

  man = pk_manager_alloc(false);

  /* 0. Constraint system */
  /* array of constraints of size 5
     0: -4x0 + 4x1 + 5 >= 0
     1: -x0 - x2 + 5 >= 0
     2: -x0 + x2 + 7 >= 0
     3: 4x0 - 4x1 + 11 >= 0
     4: 2x1 - 1 >= 0
  */
  array = ap_lincons0_array_make(5);
  array.p[0].constyp = AP_CONS_SUPEQ;
  array.p[0].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,2);
  ap_linexpr0_set_list(array.p[0].linexpr0,
		       AP_COEFF_S_INT,-4,0,
		       AP_COEFF_S_INT,4,1,
		       AP_CST_S_INT,5,
		       AP_END);
  array.p[1].constyp = AP_CONS_SUPEQ;
  array.p[1].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,2);
  ap_linexpr0_set_list(array.p[1].linexpr0,
		       AP_COEFF_S_INT,-1,0,
		       AP_COEFF_S_INT,-1,2,
		       AP_CST_S_INT,5,
		       AP_END);
  array.p[2].constyp = AP_CONS_SUPEQ;
  array.p[2].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,2);
  ap_linexpr0_set_list(array.p[2].linexpr0,
		       AP_COEFF_S_INT,-1,0,
		       AP_COEFF_S_INT,1,2,
		       AP_CST_S_INT,7,
		       AP_END);
  array.p[3].constyp = AP_CONS_SUPEQ;
  array.p[3].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,2);
  ap_linexpr0_set_list(array.p[3].linexpr0,
		       AP_COEFF_S_INT,4,0,
		       AP_COEFF_S_INT,-4,1,
		       AP_CST_S_INT,11,
		       AP_END);
  array.p[4].constyp = AP_CONS_SUPEQ;
  array.p[4].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,1);
  ap_linexpr0_set_list(array.p[4].linexpr0,
		       AP_COEFF_S_INT,2,1,
		       AP_CST_S_INT,-1,
		       AP_END);
  poly0 = pk_top(man,0,3);
  poly0 = pk_meet_lincons_array(man,true,poly0,&array);
  pk_fprint(stdout,man,poly0,NULL);

  /* 1. Constraint system */
  /* array of constraints of size 5
     0: -x0 - x2 + 3 >= 0
     1: -x0 + 3 >= 0
     2: 3x1 + 5 >= 0
  */
  ap_lincons0_array_clear(&array);
  array = ap_lincons0_array_make(3);
  array.p[0].constyp = AP_CONS_SUPEQ;
  array.p[0].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,2);
  ap_linexpr0_set_list(array.p[0].linexpr0,
		       AP_COEFF_S_INT,-1,0,
		       AP_COEFF_S_INT,-1,2,
		       AP_CST_S_INT,3,
		       AP_END);
  array.p[1].constyp = AP_CONS_SUPEQ;
  array.p[1].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,1);
  ap_linexpr0_set_list(array.p[1].linexpr0,
		       AP_COEFF_S_INT,-1,0,
		       AP_CST_S_INT,3,
		       AP_END);
  array.p[2].constyp = AP_CONS_SUPEQ;
  array.p[2].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,1);
  ap_linexpr0_set_list(array.p[2].linexpr0,
		       AP_COEFF_S_INT,3,1,
		       AP_CST_S_INT,5,
		       AP_END);

  poly1 = pk_top(man,0,3);
  poly1 = pk_meet_lincons_array(man,true,poly1,&array);
  pk_fprint(stdout,man,poly1,NULL);

   /* 2. Constraint system */
  /* array of constraints of size 5
     0: -3x0 + 3x2 + 4 >= 0
     1: -2x1 + 5 >= 0
     2: 4x0 - 4x2 - 1 >= 0
     3: x0 + x2 >= 0
  */
  ap_lincons0_array_clear(&array);
  array = ap_lincons0_array_make(4);
  array.p[0].constyp = AP_CONS_SUPEQ;
  array.p[0].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,2);
  ap_linexpr0_set_list(array.p[0].linexpr0,
		       AP_COEFF_S_INT,-3,0,
		       AP_COEFF_S_INT,3,2,
		       AP_CST_S_INT,4,
		       AP_END);
  array.p[1].constyp = AP_CONS_SUPEQ;
  array.p[1].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,1);
  ap_linexpr0_set_list(array.p[1].linexpr0,
		       AP_COEFF_S_INT,-2,1,
		       AP_CST_S_INT,5,
		       AP_END);
  array.p[2].constyp = AP_CONS_SUPEQ;
  array.p[2].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,2);
  ap_linexpr0_set_list(array.p[2].linexpr0,
		       AP_COEFF_S_INT,4,0,
		       AP_COEFF_S_INT,-4,2,
		       AP_CST_S_INT,-1,
		       AP_END);
  array.p[3].constyp = AP_CONS_SUPEQ;
  array.p[3].linexpr0 = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,2);
  ap_linexpr0_set_list(array.p[3].linexpr0,
		       AP_COEFF_S_INT,1,0,
		       AP_COEFF_S_INT,1,2,
		       AP_END);

  poly2 = pk_top(man,0,3);
  poly2 = pk_meet_lincons_array(man,true,poly2,&array);
  pk_fprint(stdout,man,poly2,NULL);

  tpoly[0] = poly0;
  tpoly[1] = poly1;
  tpoly[2] = poly2;

  poly = pk_join_array(man,tpoly,3);
  fprintf(stdout,"res\n");
  pk_fprint(stdout,man,poly,NULL);
  ap_lincons0_array_clear(&array);
  pk_free(man,poly0);
  pk_free(man,poly1);
  pk_free(man,poly2);
  pk_free(man,poly);
  ap_manager_free(man);
}

int main(int argc, char**argv)
{

  mine3();

  pk_test_example();

  srand(31);

  pk_test(0,6,6,1,3,20);
  pk_test(6,0,6,1,3,20);
  pk_test(6,10,6,1,3,20);
  pk_test(6,10,10,2,5,20);
 

  /* good for approximate(1) */
  /*
  srand(31);
  pk_test(10,10,14,2,5,1000000); 
  */
  /*
  srand(31);
  pk_test(0,10,25,0,4,1000000); 
  */
  /*
  pk_test(10,10,30,2,5,5);
  */
}
