/* ********************************************************************** */
/* test.c: unit testing  */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include <fenv.h>
#include <string.h>

#include "permutation.h"
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
#include "pk_project.h"

static size_t count = 0;

/* ********************************************************************** */
/* Vectors */
/* ********************************************************************** */

void vector(pk_internal_t* pk, size_t nbdims, numint_t factor)
{
  size_t size;
  numint_t* q[5];
  numint_t* r1;
  int i,j;
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
    printf("r1=%ldx\n",r1);
    vector_print(r1,size);

    vector_realloc(&r1,size,pk->dec+3);
    printf("r1=%ldx\n",r1);
    vector_print(r1,pk->dec+3);

    vector_realloc(&r1,pk->dec+3,2*size);
    printf("r1=%ldx\n",r1);
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

void poly1(manager_t* man, char** name_of_dim, poly_t** ppo1, poly_t** ppo2)
{
  /* Creation du polyèdre 
     1/2x+2/3y=1, [1,2]<=z+2w<=4, -2<=1/3z-w<=3,
     u non contraint */
  mpq_t mpq;
  lincons0_t cons;
  lincons0_array_t array;
  generator0_array_t garray;
  poly_t* poly;
  poly_t* poly2;
  poly_t* poly3;
  struct interval_t* titv;
  struct interval_t itv;
  int i;
  linexpr0_t* expr;
  bool b;

  mpq_init(mpq);
  array = lincons0_array_make(5);

  /* 1. Constraint system */
  array.p[0].constyp = CONS_EQ;
  array.p[0].linexpr0 = linexpr0_alloc(COEFF_MPQ,LINEXPR_SPARSE,2,CST_COEFF);
  coeff_set_int(cst_coeffref(&array.p[0].linexpr0->cst), 1);
  array.p[0].linexpr0->p.linterm[0].dim = 0;
  coeff_set_double(&array.p[0].linexpr0->p.linterm[0].coeff,0.5);
  array.p[0].linexpr0->p.linterm[1].dim = 1;
  coeff_set_frac(&array.p[0].linexpr0->p.linterm[1].coeff,2,3);
  
  array.p[1].constyp = CONS_SUPEQ;
  array.p[1].linexpr0 = linexpr0_alloc(COEFF_MPQ,LINEXPR_SPARSE,2,CST_INTERVAL);
  coeff_set_int(interval_infref(cst_intervalref(&array.p[1].linexpr0->cst)),
		-2);
  coeff_set_int(interval_supref(cst_intervalref(&array.p[1].linexpr0->cst)),
		-1);
  array.p[1].linexpr0->p.linterm[0].dim = 2;
  coeff_set_double(&array.p[1].linexpr0->p.linterm[0].coeff,1.0);
  array.p[1].linexpr0->p.linterm[1].dim = 3;
  coeff_set_frac(&array.p[1].linexpr0->p.linterm[1].coeff,2,1);

  array.p[2].constyp = CONS_SUPEQ;
  array.p[2].linexpr0 = linexpr0_alloc(COEFF_MPQ,LINEXPR_SPARSE,2,CST_COEFF);
  coeff_set_int(cst_coeffref(&array.p[2].linexpr0->cst),
		4);
  array.p[2].linexpr0->p.linterm[0].dim = 2;
  coeff_set_double(&array.p[2].linexpr0->p.linterm[0].coeff,-1.0);
  array.p[2].linexpr0->p.linterm[1].dim = 3;
  coeff_set_frac(&array.p[2].linexpr0->p.linterm[1].coeff,-2,1);
  
  array.p[3].constyp = CONS_SUPEQ;
  array.p[3].linexpr0 = linexpr0_alloc(COEFF_MPQ,LINEXPR_SPARSE,2,CST_COEFF);
  coeff_set_int(cst_coeffref(&array.p[3].linexpr0->cst),
		2);
  array.p[3].linexpr0->p.linterm[0].dim = 2;
  coeff_set_frac(&array.p[3].linexpr0->p.linterm[0].coeff, 1,3);
  array.p[3].linexpr0->p.linterm[1].dim = 3;
  coeff_set_frac(&array.p[3].linexpr0->p.linterm[1].coeff, -1,1);
  
  array.p[4].constyp = CONS_SUP;
  array.p[4].linexpr0 = linexpr0_alloc(COEFF_MPQ,LINEXPR_SPARSE,2,CST_COEFF);
  coeff_set_int(cst_coeffref(&array.p[4].linexpr0->cst),
		3);
  array.p[4].linexpr0->p.linterm[0].dim = 2;
  coeff_set_frac(&array.p[4].linexpr0->p.linterm[0].coeff,-1,3);
  array.p[4].linexpr0->p.linterm[1].dim = 3;
  coeff_set_frac(&array.p[4].linexpr0->p.linterm[1].coeff,1,1);
  
  /* Creation */
  lincons0_array_fprint(stdout,&array,name_of_dim);
  poly = poly_of_lincons_array(man,0,6,&array);
  poly_fprint(stdout,man,poly,name_of_dim);
  poly_canonicalize(man,poly);
  poly_fprint(stdout,man,poly,name_of_dim);  
  garray = poly_to_generator_array(man,poly);
  generator0_array_fprint(stdout,&garray,name_of_dim);
  generator0_array_clear(&garray);
  poly_fdump(stdout,man,poly);  
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
  titv = poly_to_box(man,poly);
  for (i=0; i<6; i++){
    fprintf(stdout,"%2d: ",i);
    interval_fprint(stdout,&titv[i]);
    fprintf(stdout,"\n");
  }

  /* Extraction (we first extract values for existing constraints, then for
     dimensions) */
  /* existing constraints */
  for (i=0; i<5; i++){
    itv = poly_bound_linexpr(man,poly,array.p[i].linexpr0);
    fprintf(stdout,"Bound of ");
    linexpr0_fprint(stdout,array.p[i].linexpr0,name_of_dim);
    fprintf(stdout,": ");
    interval_fprint(stdout,&itv);
    fprintf(stdout,"\n");
    interval_clear(&itv);
  }
  /* dimensions */
  expr = linexpr0_alloc(COEFF_MPQ,LINEXPR_SPARSE,1,CST_COEFF);
  coeff_set_double(cst_coeffref(&expr->cst),0.0);
  coeff_set_double(&expr->p.linterm[0].coeff,1.0); 
  for (i=0; i<6; i++){
    expr->p.linterm[0].dim = (dim_t)i;
    itv = poly_bound_linexpr(man,poly,expr);
    fprintf(stdout,"Bound of ");
    linexpr0_fprint(stdout,expr,name_of_dim);
    fprintf(stdout,": ");
    interval_fprint(stdout,&itv);
    fprintf(stdout,"\n");
    interval_clear(&itv);
  }
  linexpr0_free(expr);
  /* 3. of box */
  poly2 = poly_of_box(man,0,6,titv);
  poly_fprint(stdout,man,poly2,name_of_dim);
  poly_canonicalize(man,poly2);
  poly_fprint(stdout,man,poly2,name_of_dim);  
  poly_fdump(stdout,man,poly2);  

  /* 4. Tests top and bottom */
  poly3 = poly_bottom(man,2,3);
  b = poly_is_bottom(man,poly3);
  fprintf(stdout,"poly_is_bottom(poly3)=%d\n",b);
  b = poly_is_top(man,poly3);
  fprintf(stdout,"poly_is_top(poly3)=%d\n",b);
  poly_free(man,poly3);

  poly3 = poly_top(man,2,3);
  b = poly_is_bottom(man,poly3);
  fprintf(stdout,"poly_is_bottom(poly3)=%d\n",b);
  b = poly_is_top(man,poly3);
  fprintf(stdout,"poly_is_top(poly3)=%d\n",b);
  poly_free(man,poly3);

  poly3 = poly_top(man,0,0);
  b = poly_is_bottom(man,poly3);
  fprintf(stdout,"poly_is_bottom(poly3)=%d\n",b);
  b = poly_is_top(man,poly3);
  fprintf(stdout,"poly_is_top(poly3)=%d\n",b);
  poly_free(man,poly3);
  
  poly_minimize(man,poly2);
  poly_fdump(stdout,man,poly2);  
  b = poly_is_bottom(man,poly2);
  fprintf(stdout,"poly_is_bottom(poly2)=%d\n",b);
   b = poly_is_top(man,poly2);
  fprintf(stdout,"poly_is_top(poly2)=%d\n",b);

  /* 5. Tests leq */
  b = poly_is_leq(man,poly,poly2);
  fprintf(stdout,"poly_is_leq(poly,poly2)=%d\n",b);
  b = poly_is_leq(man,poly2,poly);
  fprintf(stdout,"poly_is_leq(poly,poly2)=%d\n",b);

  /* 6. Tests sat_interval */
  interval_init(&itv,COEFF_DOUBLE);
  coeff_set_int(interval_infref(&itv),-6);
  coeff_set_int(interval_supref(&itv),6);
  b = poly_sat_interval(man,poly,2,&itv);
  fprintf(stdout,"poly_sat_interval(poly,2)");
  interval_fprint(stdout,&itv);
  fprintf(stdout," = %d\n",b);
  b = poly_sat_interval(man,poly,3,&itv);
  fprintf(stdout,"poly_sat_interval(poly,3)");
  interval_fprint(stdout,&itv);
  fprintf(stdout," = %d\n",b);
  b = poly_sat_interval(man,poly,4,&itv);
  fprintf(stdout,"poly_sat_interval(poly,4)");
  interval_fprint(stdout,&itv);
  fprintf(stdout," = %d\n",b);

  coeff_set_double(interval_infref(&itv),-2.5);
  coeff_set_double(interval_supref(&itv),2.5);
  b = poly_sat_interval(man,poly,2,&itv);
  fprintf(stdout,"poly_sat_interval(poly,2)");
  interval_fprint(stdout,&itv);
  fprintf(stdout," = %d\n",b);
  b = poly_sat_interval(man,poly,3,&itv);
  fprintf(stdout,"poly_sat_interval(poly,3)");
  interval_fprint(stdout,&itv);
  fprintf(stdout," = %d\n",b);
  b = poly_sat_interval(man,poly,4,&itv);
  fprintf(stdout,"poly_sat_interval(poly,4)");
  interval_fprint(stdout,&itv);
  fprintf(stdout," = %d\n",b);

  coeff_set_double(interval_infref(&itv),-1.4);
  coeff_set_double(interval_supref(&itv),2.0);
  b = poly_sat_interval(man,poly,2,&itv);
  fprintf(stdout,"poly_sat_interval(poly,2)");
  interval_fprint(stdout,&itv);
  fprintf(stdout," = %d\n",b);
  b = poly_sat_interval(man,poly,3,&itv);
  fprintf(stdout,"poly_sat_interval(poly,3)");
  interval_fprint(stdout,&itv);
  fprintf(stdout," = %d\n",b);
  b = poly_sat_interval(man,poly,4,&itv);
  fprintf(stdout,"poly_sat_interval(poly,4)");
  interval_fprint(stdout,&itv);
  fprintf(stdout," = %d\n",b);

  mpq_set_si(mpq,-14,10);
  coeff_set_mpq(interval_infref(&itv),mpq);
  coeff_set_double(interval_supref(&itv),2.0);
  b = poly_sat_interval(man,poly,2,&itv);
  fprintf(stdout,"poly_sat_interval(poly,2)");
  interval_fprint(stdout,&itv);
  fprintf(stdout," = %d\n",b);
  b = poly_sat_interval(man,poly,3,&itv);
  fprintf(stdout,"poly_sat_interval(poly,3)");
  interval_fprint(stdout,&itv);
  fprintf(stdout," = %d\n",b);
  b = poly_sat_interval(man,poly,4,&itv);
  fprintf(stdout,"poly_sat_interval(poly,4)");
  interval_fprint(stdout,&itv);
  fprintf(stdout," = %d\n",b);

  interval_clear(&itv);

  /* 7. Tests sat_lincons */
  expr = linexpr0_alloc(COEFF_MPQ,LINEXPR_SPARSE,4,CST_COEFF);
  cons.constyp = CONS_SUPEQ;
  cons.linexpr0 = expr;
  expr->p.linterm[0].dim = 0;
  expr->p.linterm[1].dim = 1;
  expr->p.linterm[2].dim = 2;
  expr->p.linterm[3].dim = 3;
  coeff_set_double(&expr->p.linterm[0].coeff,-3.0); 
  coeff_set_double(&expr->p.linterm[1].coeff,-4.0); 
  coeff_set_double(&expr->p.linterm[2].coeff,1.0); 
  coeff_set_double(&expr->p.linterm[3].coeff,-1.0);
  coeff_set_int(cst_coeffref(&expr->cst),0);

  itv = poly_bound_linexpr(man,poly,expr);
  fprintf(stdout,"Bound of ");
  linexpr0_fprint(stdout,expr,name_of_dim);
  fprintf(stdout,": ");
  interval_fprint(stdout,&itv);
  fprintf(stdout,"\n");
  interval_clear(&itv);
  
  for (i=0; i<6; i++){
    coeff_reinit(cst_coeffref(&expr->cst),COEFF_MPQ);
    mpq_set_si(coeff_mpqref(cst_coeffref(&expr->cst)),
	       -26 + i*10, 5);
    cons.constyp = CONS_SUPEQ;
    b = poly_sat_lincons(man,poly,&cons);
    fprintf(stdout,"poly_sat_lincons(poly)");
    lincons0_fprint(stdout,&cons,name_of_dim);
    fprintf(stdout,": %d\n",b);
    cons.constyp = CONS_SUP;
    b = poly_sat_lincons(man,poly,&cons);
    fprintf(stdout,"poly_sat_lincons(poly)");
    lincons0_fprint(stdout,&cons,name_of_dim);
    fprintf(stdout,": %d\n",b);
  }
  
  mpq_set_si(mpq,-2,3);
  coeff_set_mpq(&expr->p.linterm[2].coeff,mpq); 
  coeff_set_double(&expr->p.linterm[3].coeff,2.0);
  coeff_set_int(cst_coeffref(&expr->cst),0);

  itv = poly_bound_linexpr(man,poly,expr);
  fprintf(stdout,"Bound of ");
  linexpr0_fprint(stdout,expr,name_of_dim);
  fprintf(stdout,": ");
  interval_fprint(stdout,&itv);
  fprintf(stdout,"\n");
  interval_clear(&itv);
  
  for (i=0; i<6; i++){
    coeff_reinit(cst_coeffref(&expr->cst),COEFF_MPQ);
    mpq_set_si(coeff_mpqref(cst_coeffref(&expr->cst)),
	       -1+i, 1);
    cons.constyp = CONS_SUPEQ;
    b = poly_sat_lincons(man,poly,&cons);
    fprintf(stdout,"poly_sat_lincons(poly)");
    lincons0_fprint(stdout,&cons,name_of_dim);
    fprintf(stdout,": %d\n",b);
    cons.constyp = CONS_SUP;
    b = poly_sat_lincons(man,poly,&cons);
    fprintf(stdout,"poly_sat_lincons(poly)");
    lincons0_fprint(stdout,&cons,name_of_dim);
    fprintf(stdout,": %d\n",b);
  }
  

  linexpr0_free(expr);
  
  interval_array_free(titv,6);  
  lincons0_array_clear(&array);
  mpq_clear(mpq);
  *ppo1 = poly;
  *ppo2 = poly2;
}

/* ********************************************************************** */
/* Polyhedra 2 */
/* ********************************************************************** */

void poly2(manager_t* man, char** name_of_dim, poly_t* po1, poly_t* po2)
{
  bool b;
  poly_t* po;
  linexpr0_t* expr[2];
  lincons0_t cons;
  lincons0_array_t array = { &cons, 1 };
  generator0_t gen;
  generator0_array_t garray, garray2;
  int i;

  /* Meet and join (without meet_array et join_array) */
  fprintf(stdout,"********* Meet and Join for 2 polyhedra *********\n");
  fprintf(stdout,"po1:\n");
  poly_fprint(stdout,man,po1,name_of_dim);
  fprintf(stdout,"po2:\n");
  poly_fprint(stdout,man,po2,name_of_dim);
  
  fprintf(stdout,"po=meet(po1,po2)\n");
  po = poly_meet(man,po1,po2);
  poly_fprint(stdout,man,po,name_of_dim);
  b = poly_is_leq(man,po,po1);
  fprintf(stdout,"poly_is_leq(man,po,po1)=%d\n",b);
  assert(b);
  b = poly_is_leq(man,po,po2);
  fprintf(stdout,"poly_is_leq(man,po,po2)=%d\n",b);
  assert(b);
  b = poly_is_leq(man,po1,po);
  fprintf(stdout,"poly_is_leq(man,po1,po)=%d\n",b);
  b = poly_is_leq(man,po2,po);
  fprintf(stdout,"poly_is_leq(man,po2,po)=%d\n",b);
  poly_free(man,po);

  fprintf(stdout,"po=po1; meet_with(po,po2)\n");
  po = poly_copy(man,po1);
  poly_meet_with(man,po,po2);
  poly_fprint(stdout,man,po,name_of_dim);
  b = poly_is_leq(man,po,po1);
  fprintf(stdout,"poly_is_leq(man,po,po1)=%d\n",b);
  assert(b);
  b = poly_is_leq(man,po,po2);
  fprintf(stdout,"poly_is_leq(man,po,po2)=%d\n",b);
  assert(b);
  b = poly_is_leq(man,po1,po);
  fprintf(stdout,"poly_is_leq(man,po1,po)=%d\n",b);
  b = poly_is_leq(man,po2,po);
  fprintf(stdout,"poly_is_leq(man,po2,po)=%d\n",b);
  poly_free(man,po);
  
  fprintf(stdout,"po = join(po1,po2)\n");
  po = poly_join(man,po1,po2);
  poly_fprint(stdout,man,po,name_of_dim);
  b = poly_is_top(man,po);
  fprintf(stdout,"poly_is_top(man,po)=%d\n",b);
  
  b = poly_is_leq(man,po,po1);
  fprintf(stdout,"poly_is_leq(man,po,po1)=%d\n",b);
  b = poly_is_leq(man,po,po2);
  fprintf(stdout,"poly_is_leq(man,po,po2)=%d\n",b);
  b = poly_is_leq(man,po1,po);
  fprintf(stdout,"poly_is_leq(man,po1,po)=%d\n",b);
  assert(b);
  b = poly_is_leq(man,po2,po);
  fprintf(stdout,"poly_is_leq(man,po2,po)=%d\n",b);
  assert(b);
  poly_free(man,po);
  
  fprintf(stdout,"po = po1; join_with(po,po2)\n");
  po = poly_copy(man,po1);
  poly_join_with(man,po,po2);
  poly_fprint(stdout,man,po,name_of_dim);
  b = poly_is_leq(man,po,po1);
  fprintf(stdout,"poly_is_leq(man,po,po1)=%d\n",b);
  b = poly_is_leq(man,po,po2);
  fprintf(stdout,"poly_is_leq(man,po,po2)=%d\n",b);
  b = poly_is_leq(man,po1,po);
  fprintf(stdout,"poly_is_leq(man,po1,po)=%d\n",b);
  assert(b);
  b = poly_is_leq(man,po2,po);
  fprintf(stdout,"poly_is_leq(man,po2,po)=%d\n",b);
  assert(b);
  poly_free(man,po);

  /* Additions of constraints */
  fprintf(stdout,"********* Addition of constraint *********\n");
  fprintf(stdout,"po1:\n");
  poly_fprint(stdout,man,po1,name_of_dim);
  garray = poly_to_generator_array(man,po1);
  generator0_array_fprint(stdout,&garray,name_of_dim);
  generator0_array_clear(&garray);

  /* expression z>=-1/2 */
  expr[0] = linexpr0_alloc(COEFF_MPQ,LINEXPR_SPARSE,1,CST_COEFF);
  coeff_reinit(cst_coeffref(&expr[0]->cst),COEFF_MPQ);
  coeff_set_frac(cst_coeffref(&expr[0]->cst),1,2);
  expr[0]->p.linterm[0].dim = 2;
  coeff_set_int(&expr[0]->p.linterm[0].coeff,1);
  /* expression x+u>=1 */
  expr[1] = linexpr0_alloc(COEFF_MPQ,LINEXPR_SPARSE,2,CST_COEFF);
  coeff_set_int(cst_coeffref(&expr[1]->cst),-1);
  expr[1]->p.linterm[0].dim = 0;
  expr[1]->p.linterm[1].dim = 4;
  coeff_set_int(&expr[1]->p.linterm[0].coeff,1);
  coeff_set_int(&expr[1]->p.linterm[1].coeff,1);

  for (i=0; i<2; i++){
    cons.linexpr0 = expr[i];
    for (cons.constyp=0; cons.constyp<3; cons.constyp++){
      fprintf(stdout,"po=meet_lincons(po1) ");
      lincons0_fprint(stdout,&cons,name_of_dim);
      fprintf(stdout,"\n");
      po = poly_meet_lincons_array(man,po1,&array);
      poly_fprint(stdout,man,po,name_of_dim);
      garray = poly_to_generator_array(man,po);
      generator0_array_fprint(stdout,&garray,name_of_dim);
      generator0_array_clear(&garray);
      b = poly_is_leq(man,po,po1);
      fprintf(stdout,"poly_is_leq(po,po1)=%d\n",b);
      assert(b);
      b = poly_is_leq(man,po1,po);
      fprintf(stdout,"poly_is_leq(po1,po)=%d\n",b);
      poly_free(man,po);

      fprintf(stdout,"po=po1; meet_lincons_with(po) ");
      lincons0_fprint(stdout,&cons,name_of_dim);
      fprintf(stdout,"\n");
      po = poly_copy(man,po1);
      poly_meet_lincons_array_with(man,po,&array);
      poly_fprint(stdout,man,po,name_of_dim);
      garray = poly_to_generator_array(man,po);
      generator0_array_fprint(stdout,&garray,name_of_dim);
      generator0_array_clear(&garray);
      b = poly_is_leq(man,po,po1);
      fprintf(stdout,"poly_is_leq(po,po1)=%d\n",b);
      assert(b);
      b = poly_is_leq(man,po1,po);
      fprintf(stdout,"poly_is_leq(po1,po)=%d\n",b);
      poly_free(man,po);
    }
    linexpr0_free(expr[i]);
  }

  /* Additions of rays */
  fprintf(stdout,"********* Addition of rays *********\n");
  fprintf(stdout,"po1:\n");
  poly_fprint(stdout,man,po1,name_of_dim);
  garray = poly_to_generator_array(man,po1);
  generator0_array_fprint(stdout,&garray,name_of_dim);
  generator0_array_clear(&garray);

  /* expression z+w/8 */
  expr[0] = linexpr0_alloc(COEFF_MPQ,LINEXPR_SPARSE,2,CST_COEFF);
  expr[0]->p.linterm[0].dim = 2;
  expr[0]->p.linterm[1].dim = 3;
  coeff_set_int(&expr[0]->p.linterm[0].coeff,1);
  coeff_set_frac(&expr[0]->p.linterm[1].coeff,1,8);
  /* expression z+w */
  expr[1] = linexpr0_alloc(COEFF_MPQ,LINEXPR_SPARSE,2,CST_COEFF);
  expr[1]->p.linterm[0].dim = 2;
  expr[1]->p.linterm[1].dim = 3;
  coeff_set_int(&expr[1]->p.linterm[0].coeff,1);
  coeff_set_int(&expr[1]->p.linterm[1].coeff,1);
  garray2 = generator0_array_make(1);
  for (i=0; i<2; i++){
    gen.linexpr0 = expr[i];
    gen.gentyp = GEN_RAY;
    garray2.p[0] = gen;
    fprintf(stdout,"po=add_ray_array(po1) ");
    generator0_fprint(stdout,&gen,name_of_dim);
    fprintf(stdout,"\n");
    po = poly_add_ray_array(man,po1,&garray2);
    poly_fprint(stdout,man,po,name_of_dim);
    garray = poly_to_generator_array(man,po);
    generator0_array_fprint(stdout,&garray,name_of_dim);
    generator0_array_clear(&garray);
    b = poly_is_leq(man,po,po1);
    fprintf(stdout,"poly_is_leq(po,po1)=%d\n",b);
    b = poly_is_leq(man,po1,po);
    fprintf(stdout,"poly_is_leq(po1,po)=%d\n",b);
    assert(b);
    poly_free(man,po);
    
    fprintf(stdout,"po=po1; add_ray_array_with(po) ");
    generator0_fprint(stdout,&gen,name_of_dim);
    fprintf(stdout,"\n");
    po = poly_copy(man,po1);
    poly_add_ray_array_with(man,po,&garray2);
    poly_fprint(stdout,man,po,name_of_dim);
    garray = poly_to_generator_array(man,po);
    generator0_array_fprint(stdout,&garray,name_of_dim);
    generator0_array_clear(&garray);
    b = poly_is_leq(man,po,po1);
    fprintf(stdout,"poly_is_leq(po,po1)=%d\n",b);
    b = poly_is_leq(man,po1,po);
    fprintf(stdout,"poly_is_leq(po1,po)=%d\n",b);
    poly_free(man,po);
    linexpr0_free(expr[i]);
  }
  garray2.p[0].linexpr0 = NULL;
  generator0_array_clear(&garray2);
}


/* ********************************************************************** */
/* Polyhedra 3 (assignement and substitution of a single expression) */
/* ********************************************************************** */

void poly3(manager_t* man, char** name_of_dim, poly_t* po1, poly_t* po2)
{
  bool b;
  poly_t* poly1;
  poly_t* poly2;
  poly_t* poly3;
  poly_t* tpo[2];
  linexpr0_t* expr[3];
  generator0_array_t garray;
  int i,j,inplace,undet;

  fprintf(stdout,"********* Assignement and Substitution, single *********\n");
  fprintf(stdout,"po1:\n");
  poly_fprint(stdout,man,po1,name_of_dim);
  garray = poly_to_generator_array(man,po1);
  generator0_array_fprint(stdout,&garray,name_of_dim);
  generator0_array_clear(&garray);
  fprintf(stdout,"po2:\n");
  poly_fprint(stdout,man,po2,name_of_dim);
  garray = poly_to_generator_array(man,po2);
  generator0_array_fprint(stdout,&garray,name_of_dim);
  generator0_array_clear(&garray);
  tpo[0] = po1;
  tpo[1] = po2;

  /* Single equation z+5w-1 */
  expr[0] = linexpr0_alloc(COEFF_MPQ,LINEXPR_SPARSE,2,CST_COEFF);
  expr[0]->p.linterm[0].dim = 2;
  expr[0]->p.linterm[1].dim = 3;
  coeff_set_int(cst_coeffref(&expr[0]->cst),-1);
  coeff_set_int(&expr[0]->p.linterm[0].coeff,1);
  coeff_set_int(&expr[0]->p.linterm[1].coeff,5);
  /* Single equation 2x+1 */
  expr[1] = linexpr0_alloc(COEFF_MPQ,LINEXPR_SPARSE,1,CST_COEFF);
  expr[1]->p.linterm[0].dim = 0;
  coeff_set_int(cst_coeffref(&expr[1]->cst),1);
  coeff_set_int(&expr[1]->p.linterm[0].coeff,2);

  for (undet=0; undet<2; undet++){
    if (undet==1){
      for (j=0;j<2;j++){
	cst_reinit(&expr[j]->cst,COEFF_MPQ,CST_INTERVAL);
	coeff_set_int(interval_infref(cst_intervalref(&expr[j]->cst)),
		      -1);
	coeff_set_int(interval_supref(cst_intervalref(&expr[j]->cst)),
		      +1);
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
	  linexpr0_fprint(stdout,&expr[j],name_of_dim);
	  fprintf(stdout,"\n");
	  if (inplace){
	    poly1 = poly_copy(man,tpo[i]);
	    poly_assign_linexpr_with(man,poly1,2,expr[j]);
	  } else {
	    poly1 = poly_assign_linexpr(man,tpo[i],2,expr[j]);
	  }
	  poly_fprint(stdout,man,poly1,name_of_dim);
	  garray = poly_to_generator_array(man,poly1);
	  generator0_array_fprint(stdout,&garray,name_of_dim);
	  generator0_array_clear(&garray);

	  fprintf(stdout,
		  inplace ?
		  "poly2 = poly1; substitute_linexpr_with(poly2) z:=" :
		  "poly2=substitute_linexpr(poly1) z:="
		  );
	  linexpr0_fprint(stdout,expr[j],name_of_dim);
	  fprintf(stdout,"\n");
	  if (inplace){
	    poly2 = poly_copy(man,poly1);
	    poly_substitute_linexpr_with(man,poly2,2,expr[j]);
	  } else {
	    poly2 = poly_substitute_linexpr(man,poly1,2,expr[j]);
	  }
	  poly_fprint(stdout,man,poly2,name_of_dim);
	  garray = poly_to_generator_array(man,poly2);
	  generator0_array_fprint(stdout,&garray,name_of_dim);
	  generator0_array_clear(&garray);

	  b = poly_is_leq(man,tpo[i],poly2);
	  fprintf(stdout,"poly_is_leq(po%d,poly2)=%d\n",i+1,b);
	  assert(b);

	  b = poly_is_leq(man,poly2,tpo[i]);
	  fprintf(stdout,"poly_is_leq(poly2,po%d)=%d\n",i+1,b);
	  assert(!undet && j==0 ? b : true);

	  fprintf(stdout,
		  inplace ?
		  "poly3 = poly2; assign_linexpr_with(poly3) z:=" :
		  "poly3=assign_linexpr(poly2) z:="
		  );
	  linexpr0_fprint(stdout,expr[j],name_of_dim);
	  fprintf(stdout,"\n");
	  if (inplace){
	    poly3 = poly_copy(man,poly2);
	    poly_assign_linexpr_with(man,poly3,2,expr[j]);
	  } else {
	    poly3 = poly_assign_linexpr(man,poly2,2,expr[j]);
	  }
	  poly_fprint(stdout,man,poly3,name_of_dim);
	  garray = poly_to_generator_array(man,poly3);
	  generator0_array_fprint(stdout,&garray,name_of_dim);
	  generator0_array_clear(&garray);

	  b = poly_is_eq(man,poly1,poly3);
	  fprintf(stdout,"poly_is_eq(poly1,poly3)=%d\n",b);
	  assert(!(undet && j==0) ? b : true);
       
	  poly_free(man,poly1);
	  poly_free(man,poly2);
	  poly_free(man,poly3);
	}
      }
    }
  }
  linexpr0_free(expr[0]);
  linexpr0_free(expr[1]);
}

void poly_test_example_aux(manager_t* man)
{
  poly_t* p1;
  poly_t* p2;
  char** name_of_dim;
  dim_t* perm;
  name_of_dim = malloc(6*sizeof(char*));
  name_of_dim[0] = "x";
  name_of_dim[1] = "y";
  name_of_dim[2] = "z";
  name_of_dim[3] = "w";
  name_of_dim[4] = "u";
  name_of_dim[5] = "v";
  perm = malloc(6*sizeof(dim_t));
  permutation_set_id(perm,6);
  perm[0] = 2;
  perm[2] = 0;
  perm[3] = 5;
  perm[5] = 3;

  poly1(man,name_of_dim, &p1, &p2);

  assert(poly_check((pk_internal_t*)man->internal,p1));
  assert(poly_check((pk_internal_t*)man->internal,p2));
  poly2(man,name_of_dim,p1,p2);

  poly_add_permute_dimensions_with(man,p2,0,0,perm,true);
  poly2(man,name_of_dim,p1,p2);

  poly3(man,name_of_dim,p1,p2);

  poly_free(man,p2);
  p2 = poly_bottom(man,0,6);
  poly2(man,name_of_dim,p1,p2);
  poly2(man,name_of_dim,p2,p1);
  poly3(man,name_of_dim,p2,p1);

  poly_free(man,p1);
  poly_free(man,p2);

  /* */
  free(name_of_dim);
  free(perm);
}

void poly_test_example()
{
  pk_internal_t* pk;
  manager_t* man;
  numint_t num;
  funid_t funid;

  man = pk_manager_alloc(true);
  pk = (pk_internal_t*)man->internal;
  pk_internal_realloc_lazy(pk,20);

  numint_init(num);
  numint_set_int(num,1);
  vector(pk,7,num);
  numint_clear(num);

 
  poly_test_example_aux(man);

  for (funid=0; funid<FUNID_SIZE; funid++){
    funopt_t funopt;
    funopt_init(&funopt);
    funopt.algorithm = -1;
    manager_set_funopt(man,funid,&funopt);
  }

  poly_test_example_aux(man);

  manager_free(man);
}

/* ********************************************************************** */
/* Polyhedra random */
/* ********************************************************************** */

linexpr0_t* expr_random(size_t intdim, size_t realdim, 
		       size_t maxcoeff, /* Maximum size of non-null coefficients */
		       unsigned int mag /* magnitude of coefficients */
		       )
{
  linexpr0_t* expr;
  unsigned long int r;
  size_t j,nbcoeff;
  dim_t dim;  
  double coeff;

  /* Expression */
  expr = linexpr0_alloc(COEFF_MPQ,LINEXPR_DENSE,intdim+realdim,CST_COEFF);
  coeff_set_double(cst_coeffref(&expr->cst),0.0);
  for (j=0; j<intdim+realdim; j++){
    coeff_set_double(&expr->p.coeff[j],0.0);
  }
  /* Fill the expression */
  r = rand();
  coeff = (double)( (long int)(r % (2*mag)) - (long int)mag);
  coeff_set_double(cst_coeffref(&expr->cst),coeff);

  r = rand();
  nbcoeff = (r % maxcoeff) + 1;
  for (j=0; j<nbcoeff; j++){
    r = rand();
    dim = r % (intdim+realdim);
    r = rand();
    coeff = (double)( (long int)(r % (2*mag)) - (long int)mag);
    coeff_set_double(&expr->p.coeff[dim],coeff);
  }
  return expr;
}

poly_t* poly_random(manager_t* man, size_t intdim, size_t realdim, 
		    size_t nbcons, /* Number of constraints */
		    size_t maxeq, /* Maximum number of equations */
		    size_t maxcoeff, /* Maximum size of non-null coefficients */
		    unsigned int mag /* magnitude of coefficients */
		    )
{
  lincons0_array_t array;
  long int r;
  lincons0_t cons;
  linexpr0_t* expr;
  poly_t* poly;
  int i;

  array = lincons0_array_make(nbcons);
  r = rand();
  maxeq = r % (maxeq+1);
  for (i=0; i<nbcons; i++){
    expr = expr_random(intdim,realdim,maxcoeff,mag);
    /* Equality or inequality ? */
    if (i<maxeq){
      cons.constyp = CONS_EQ;
    } else {
      r = rand();
      cons.constyp = (r%3==0) ? CONS_SUP : CONS_SUPEQ;
    }
    cons.linexpr0 = expr;
    array.p[i] = cons;
  }
  poly = poly_of_lincons_array(man,intdim,realdim,&array);
  lincons0_array_clear(&array);
  return poly;
}


/* ********************************************************************** */
/* Approximate */
/* ********************************************************************** */

void test_approximate()
{
  linexpr0_t* expr;
  lincons0_t cons;
  lincons0_array_t array = { &cons, 1 };
  poly_t* po, *pa;
  pk_internal_t* pk;
  manager_t* man;
  size_t mag = 1000000;

  man = pk_manager_alloc(true);
  pk = (pk_internal_t*)man->internal;
  pk_set_approximate_max_coeff_size(pk,1);

  /* Build a polyhedron x>=0, x<=mag, x-(mag*mag)y>=0 */
  po = poly_top(man,0,2);

  /* Expression */
  expr = linexpr0_alloc(COEFF_MPQ,LINEXPR_SPARSE,1,CST_COEFF);
  cons.constyp = CONS_SUPEQ;
  cons.linexpr0 = expr;
  coeff_set_int(cst_coeffref(&expr->cst),0);
  expr->p.linterm[0].dim=0;
  coeff_set_int(&expr->p.linterm[0].coeff,1);
  poly_meet_lincons_array_with(man,po,&array);
  coeff_set_int(cst_coeffref(&expr->cst),mag);
  coeff_set_int(&expr->p.linterm[0].coeff,-1);
  poly_meet_lincons_array_with(man,po,&array);
  linexpr0_free(expr);
  expr = linexpr0_alloc(COEFF_MPQ,LINEXPR_SPARSE,2,CST_COEFF);
  cons.constyp = CONS_SUPEQ;
  cons.linexpr0 = expr;
  coeff_set_int(cst_coeffref(&expr->cst),0);
  expr->p.linterm[0].dim=0;
  expr->p.linterm[1].dim=1;
  coeff_set_int(&expr->p.linterm[0].coeff,1);
  coeff_reinit(&expr->p.linterm[1].coeff,COEFF_MPQ);
  coeff_set_int(&expr->p.linterm[1].coeff,mag);
  mpq_mul(coeff_mpqref(&expr->p.linterm[1].coeff),
	  coeff_mpqref(&expr->p.linterm[1].coeff),coeff_mpqref(&expr->p.linterm[1].coeff));
  mpq_neg(coeff_mpqref(&expr->p.linterm[1].coeff),coeff_mpqref(&expr->p.linterm[1].coeff));
  poly_meet_lincons_array_with(man,po,&array);
  linexpr0_free(expr);

  pa = poly_copy(man,po);
  poly_approximate(man,po,3);
  poly_fprint(stdout,man,pa,NULL);
  poly_fprint(stdout,man,po,NULL);
  assert(poly_is_leq(man,pa,po));
  
  poly_free(man,po);
  poly_free(man,pa);
  manager_free(man);
  return;
}


/* ********************************************************************** */
/* Polyhedra 10 */
/* ********************************************************************** */

void poly_test_gen(manager_t* man, size_t intdim, size_t realdim, 
		   size_t nbcons, /* Number of constraints */
		   size_t maxeq, /* Maximum number of equations */
		   size_t maxcoeff, /* Maximum size of non-null coefficients */
		   unsigned int mag, /* magnitude of coefficients */
		   char*** pname_of_dim,
		   poly_t*** ptpoly, /* of size 6, 0==2, 1==3 */
		   linexpr0_t*** ptexpr, /* of size 3 */
		   dim_t** ptdim /* of size 3 */
		   )
{
  char buffer[80];
  int i;  
  poly_t* p1;
  poly_t* p2;

  *pname_of_dim = malloc((intdim+realdim)*sizeof(char*));
  for (i=0; i<intdim; i++){
    snprintf(buffer,sizeof(buffer),"n%d",i);
    (*pname_of_dim)[i] = strdup(buffer);
  }
  for (i=0; i<realdim; i++){
    snprintf(buffer,sizeof(buffer),"r%d",i);
    (*pname_of_dim)[intdim+i] = strdup(buffer);
  }
  p1 = poly_random(man,intdim,realdim,nbcons,maxeq,maxcoeff,mag);
  p2 = poly_random(man,intdim,realdim,nbcons,maxeq,maxcoeff,mag);
  *ptpoly = malloc(6*sizeof(poly_t*));
  (*ptpoly)[0] = (*ptpoly)[2] = p1;
  (*ptpoly)[1] = (*ptpoly)[3] = p2;
  (*ptpoly)[4] = poly_random(man,intdim,realdim,nbcons,maxeq,maxcoeff,mag);
  (*ptpoly)[5] = poly_random(man,intdim,realdim,nbcons,maxeq,maxcoeff,mag);
  *ptexpr = malloc(3*sizeof(linexpr0_t*));
  for (i=0;i<3;i++){
    (*ptexpr)[i] = expr_random(intdim,realdim,maxcoeff,mag);
  }
  *ptdim = malloc(3*sizeof(dim_t));
  (*ptdim)[0] = 0;
  (*ptdim)[1] = 1;
  (*ptdim)[2] = intdim+realdim-1;
  assert(intdim+realdim>=3);
}

void poly_test_free(manager_t* man, size_t intdim, size_t realdim, 
		    char** name_of_dim,
		     poly_t** tpoly,
		     linexpr0_t** texpr,
		     dim_t* tdim)
{
  int j;

  for (j=0;j<intdim+realdim;j++){
    free(name_of_dim[j]);
  }
  free(name_of_dim);
  poly_free(man,tpoly[0]);
  poly_free(man,tpoly[1]);
  poly_free(man,tpoly[4]);
  poly_free(man,tpoly[5]);
  free(tpoly);
  for (j=0;j<3;j++){
    linexpr0_free(texpr[j]);
  }
  free(texpr);
  free(tdim);
}



//#define PRINT(a)
#define PRINT(a) a

void poly_test_check(manager_t* man, size_t intdim, size_t realdim,
		     char** name_of_dim,
		     poly_t** tpoly,
		     linexpr0_t** texpr,
		     dim_t* tdim)
{
  poly_t* p1;
  poly_t* p2;
  poly_t* p3;
  poly_t* p4;
  poly_t* p5;
  dim_t* perm;
  size_t i,k;
  mpq_t mpq, mpqone;
  coeff_t coeffzero;
  interval_t interval;
  lincons0_t cons;
  lincons0_array_t consarray = { &cons, 1 };
  lincons0_array_t array;
  generator0_array_t garray;
  double d;
  struct interval_t* box;
  linexpr0_t* expr;
  t res;
  dim_t dim;
  bool b;

  mpq_init(mpq);
  mpq_set_si(mpq,0,1);
  mpq_init(mpqone);
  mpq_set_si(mpqone,1,1);
  coeff_init(&coeffzero,COEFF_MPQ);
  coeff_set_mpq(&coeffzero,mpq);

  for (i=0;i<6; i++){
    PRINT((printf("poly %d:\n",i)));
    PRINT((poly_fprint(stdout,man,tpoly[i],name_of_dim)));
  }
  for (i=0;i<3; i++){
    PRINT((printf("%d: dim = %d\nexpr = ",i,tdim[i])));
    PRINT((linexpr0_fprint(stdout,texpr[i],name_of_dim)));
    PRINT((printf("\n")));
  }
      
  p1 = tpoly[0];
  p2 = tpoly[1];
  
  /* minimize */
  PRINT((printf("minimize\n")));
  p3 = poly_copy(man,p1);
  poly_minimize(man,p3);
  assert(poly_is_canonical(man,p3)==false || poly_is_bottom(man,p3));
  assert(poly_is_eq(man,p1,p3));
  poly_free(man,p3);

  /* canonicalize */
  PRINT((printf("canonicalize\n")));
  p3 = poly_copy(man,p1);
  poly_canonicalize(man,p3);
  assert(poly_is_eq(man,p1,p3));
  assert(poly_is_canonical(man,p3));
  poly_free(man,p3);

  /* approximate */
  p3 = poly_copy(man,p1);
  poly_approximate(man,p3,0);
  assert(poly_is_eq(man,p1,p3));
  poly_free(man,p3);
  p3 = poly_copy(man,p1);
  poly_approximate(man,p3,-1);
  poly_fprint(stdout,man,p1,name_of_dim);
  poly_fprint(stdout,man,p3,name_of_dim);
  assert(poly_is_leq(man,p3,p1));
  if (poly_is_leq(man,p1,p3)!=true){
    printf("approximate(-1) smaller\n");
  }
  poly_free(man,p3);
  p3 = poly_copy(man,p1);
  poly_approximate(man,p3,3);
  poly_fprint(stdout,man,p1,name_of_dim);
  poly_fprint(stdout,man,p3,name_of_dim);
  assert(poly_is_leq(man,p1,p3));
  if (poly_is_leq(man,p3,p1)!=true){
    printf("approximate(3) greater\n");
    assert(man->result.flag_exact!=true);
  }
  poly_free(man,p3);

  /* bound_dimension and sat_interval */
  PRINT((printf("bound_dimension and sat_interval\n")));
  for (i=0;i<intdim+realdim; i++){
    interval = poly_bound_dimension(man,p1,i);
    assert(poly_sat_interval(man,p1,i,&interval));
    if (!coeff_infty(&interval.inf) && !coeff_infty(&interval.sup)){
      mpq_set_coeff(mpq,&interval.inf,0);
      mpq_add(mpq,mpq,mpqone);
      coeff_set_mpq(&interval.inf,mpq);
      mpq_set_coeff(mpq,&interval.sup,0);
      mpq_sub(mpq,mpq,mpqone);
      coeff_set_mpq(&interval.sup,mpq);
      assert(poly_sat_interval(man,p1,i,&interval)==false || poly_is_bottom(man,p1));
    }
    if (!coeff_infty(&interval.inf)){
      mpq_set_coeff(mpq,&interval.inf,0);
      mpq_sub(mpq,mpq,mpqone);
      mpq_sub(mpq,mpq,mpqone);
      coeff_set_mpq(&interval.inf,mpq);
    }
    if (!coeff_infty(&interval.sup)){
      mpq_set_coeff(mpq,&interval.sup,0);
      mpq_add(mpq,mpq,mpqone);
      mpq_add(mpq,mpq,mpqone);
      coeff_set_mpq(&interval.sup,mpq);
    }
    assert(poly_sat_interval(man,p1,i,&interval));
    interval_clear(&interval);
  }

  /* bound_linexpr and sat_lincons */
  PRINT((printf("bound_linexpr and sat_lincons\n")));
  for (k=0; k<3; k++){
    expr = texpr[k];
    cons.constyp = CONS_SUPEQ;
    cons.linexpr0 = expr;
    d = *coeff_doubleref(&expr->cst.val.coeff);
    interval = poly_bound_linexpr(man,p1,expr);
    if (!coeff_infty(&interval.inf)){
      *coeff_doubleref(&expr->cst.val.coeff) = 
	d + 1.0 - coeff_get_double(&interval.inf,0);
      assert(poly_sat_lincons(man,p1,&cons));
      
      *coeff_doubleref(&expr->cst.val.coeff) =       
	d - 1.0 - coeff_get_double(&interval.inf,0);
      assert(poly_sat_lincons(man,p1,&cons)==false ||
	     poly_is_bottom(man,p1));
    }
    if (!coeff_infty(&interval.sup)){
      for (i=0; i<intdim+realdim; i++){
	*coeff_doubleref(&expr->p.coeff[i]) =
	  - *coeff_doubleref(&expr->p.coeff[i]);
      }
      *coeff_doubleref(&expr->cst.val.coeff) =       
	coeff_get_double(&interval.sup,0) - d + 1.0;
      assert(poly_sat_lincons(man,p1,&cons));
      *coeff_doubleref(&expr->cst.val.coeff) =       
	coeff_get_double(&interval.sup,0) - d - 1.0;
      assert(poly_sat_lincons(man,p1,&cons)==false ||
	     poly_is_bottom(man,p1));
    }
    *coeff_doubleref(&expr->cst.val.coeff) = d; 
    interval_clear(&interval);
  }

  /* to_box */
  PRINT((printf("to_box\n")));
  box = poly_to_box(man,p1);
  for (i=0;i<intdim+realdim; i++){
    assert(poly_sat_interval(man,p1,i,&box[i]));
  }
  p3 = poly_of_box(man,intdim,realdim,box);
  if (poly_is_leq(man,p1,p3)!=true &&
      poly_is_bottom(man,p1)==false){
    if (intdim==0) assert(false);
    else {
      printf("of_box(to_box) not greater\n");
    }
  }
  poly_free(man,p3);
  interval_array_free(box,intdim+realdim);

  /* to_lincons_array */
  PRINT((printf("to_lincons_array\n")));
  array = poly_to_lincons_array(man,p1);
  for (k=0; k<array.size; k++){
    res = poly_sat_lincons(man,p1,&array.p[k]);
    if (intdim==0) assert(res);
    else if (res!=true){
      printf("sat_lincons(to_lincons) not true\n");
    }
  }
  p3 = poly_of_lincons_array(man,intdim,realdim,&array);
  assert(poly_is_leq(man,p3,p1));
  if (poly_is_leq(man,p1,p3)!=true){
    if (intdim==0) assert(false);
    else
      printf("of_lincons(to_lincons) smaller\n");
  }
  poly_free(man,p3);
  lincons0_array_clear(&array);

  /* meet, meet_with, meet_array, join, join_with, join_array */
  PRINT((printf("meet, meet_with\n")));
  p3 = poly_meet(man,p1,p2);
  p4 = poly_copy(man,p1);
  poly_meet_with(man,p4,p2);
  assert(poly_is_eq(man,p3,p4));
  assert(poly_is_leq(man,p3,p1));
  assert(poly_is_leq(man,p3,p2));
  poly_free(man,p4);
  PRINT((printf("meet_array\n")));
  p4 = poly_meet_array(man,tpoly,6);
  for (k=0;k<6;k++){
    assert(poly_is_leq(man,p4,tpoly[k]));
  }
  assert(poly_is_leq(man,p4,p3));
  poly_free(man,p3);
  poly_free(man,p4);

  PRINT((printf("join, join_with\n")));
  p3 = poly_join(man,p1,p2);
  p4 = poly_copy(man,p1);
  poly_join_with(man,p4,p2);
  assert(poly_is_eq(man,p3,p4));
  assert(poly_is_leq(man,p1,p3));
  assert(poly_is_leq(man,p2,p3));
  poly_free(man,p4);
  PRINT((printf("join_array\n")));
  count++;
  p4 = poly_join_array(man,tpoly,6);
  poly_fprint(stdout,man,p4,name_of_dim);
  assert(poly_is_leq(man,p3,p4));
  for (k=0;k<6;k++){
    printf("k = %d\n",k);
    assert(poly_is_leq(man,tpoly[k],p4));
  }
  /* approximate */
  PRINT((printf("approximate on the result of join_array\n")));
  p5 = poly_copy(man,p4);
  poly_approximate(man,p5,0);
  assert(poly_is_eq(man,p4,p5));
  poly_free(man,p5);
  p5 = poly_copy(man,p4);
  poly_approximate(man,p5,-1);
  poly_fprint(stdout,man,p5,name_of_dim);
  assert(poly_is_leq(man,p5,p4));
  if (poly_is_leq(man,p4,p5)!=true){
    printf("approximate(-1) smaller\n");
  }
  poly_free(man,p5);
  p5 = poly_copy(man,p4);
  poly_approximate(man,p5,3);
  res = man->result.flag_exact;
  poly_fprint(stdout,man,p5,name_of_dim);
  assert(poly_is_leq(man,p4,p5));
  if (poly_is_leq(man,p5,p4)!=true){
    printf("approximate(3) greater\n");
    assert(res!=true);
  }
  else {
    assert(res);
  }
  poly_free(man,p5);

  poly_free(man,p3);
  poly_free(man,p4);

  /* meet_lincons, add_ray_array */
  PRINT((printf("meet_lincons, add_ray_array\n"))); 
  for (k=0; k<3; k++){
    expr = texpr[k];
    cons.constyp = CONS_SUPEQ;
    cons.linexpr0 = expr;
    p3 = poly_meet_lincons_array(man,p1,&consarray);
    p4 = poly_copy(man,p1);
    poly_meet_lincons_array_with(man,p4,&consarray);
    assert(poly_is_eq(man,p3,p4));
    assert(poly_is_leq(man,p3,p1));
    poly_free(man,p3);
    poly_free(man,p4);
    garray = generator0_array_make(1);
    garray.p[0].gentyp = GEN_RAY;
    garray.p[0].linexpr0 = expr;
    p3 = poly_add_ray_array(man,p1,&garray);
    p4 = poly_copy(man,p1);
    poly_add_ray_array_with(man,p4,&garray);
    assert(poly_is_eq(man,p3,p4));
    assert(poly_is_leq(man,p1,p3));
    free(garray.p);
    poly_free(man,p3);
    poly_free(man,p4);
  }
  /* assign and substitute (single, deterministic) */
  PRINT((printf("assign and substitute (single, deterministic)\n"))); 
  for (k=0; k<3; k++){
    dim = tdim[k];
    expr = texpr[k];
    p3 = poly_assign_linexpr(man,p1,dim,expr);
    p4 = poly_copy(man,p1);
    poly_assign_linexpr_with(man,p4,dim,expr);
    assert(poly_is_eq(man,p3,p4));
    poly_free(man,p4);
    p4 = poly_substitute_linexpr(man,p3,dim,expr);
    p5 = poly_copy(man,p3);
    poly_substitute_linexpr_with(man,p5,dim,expr);
    assert(poly_is_eq(man,p4,p5));
    poly_free(man,p5);
    p5 = poly_assign_linexpr(man,p4,dim,expr);
    assert(poly_is_leq(man,p1,p4));
    assert(poly_is_eq(man,p3,p5));
    poly_free(man,p3);
    poly_free(man,p4);
    poly_free(man,p5);
  }
  /* parallel assign and substitute (deterministic) */
  PRINT((printf("parallel assign and substitute (deterministic)\n"))); 
  p3 = poly_assign_linexpr_array(man,p1,tdim,texpr,3);
  p4 = poly_copy(man,p1);
  poly_assign_linexpr_array_with(man,p4,tdim,texpr,3);
  assert(poly_is_eq(man,p3,p4));
  poly_free(man,p4);
  p4 = poly_substitute_linexpr_array(man,p3,tdim,texpr,3);
  p5 = poly_copy(man,p3);
  poly_substitute_linexpr_array_with(man,p5,tdim,texpr,3);
  assert(poly_is_eq(man,p4,p5));
  poly_free(man,p5);
  p5 = poly_assign_linexpr_array(man,p4,tdim,texpr,3);
  assert(poly_is_leq(man,p1,p4));
  assert(poly_is_eq(man,p3,p5));
  poly_free(man,p3);
  poly_free(man,p4);
  poly_free(man,p5);

  /* project and forget */
  PRINT((printf("project and forget\n"))); 
  box = poly_to_box(man,p1);
  for (k=0; k<3; k++){
    dim = tdim[k];
    p3 = poly_project_array(man,p1,&dim,1);
    p4 = poly_copy(man,p1);
    poly_project_array_with(man,p4,&dim,1);  
    assert(poly_is_eq(man,p3,p4));
    poly_free(man,p4);
    p4 = poly_forget_array(man,p1,&dim,1);
    p5 = poly_copy(man,p1);
    poly_forget_array_with(man,p5,&dim,1);  
    assert(poly_is_eq(man,p4,p5));
    poly_free(man,p5);
    assert(poly_is_leq(man,p3,p4));
    assert(poly_is_leq(man,p4,p3)==false || 
	   poly_is_bottom(man,p1));

    interval = poly_bound_dimension(man,p3,dim);
    assert(coeff_equal(&interval.inf,&interval.sup) || 
	   poly_is_bottom(man,p1));
    assert(coeff_cmp(&interval.inf,&coeffzero)==0 || 
	   poly_is_bottom(man,p1));
    interval_clear(&interval);
    interval = poly_bound_dimension(man,p4,dim);
    assert(interval_is_top(&interval) || 
	   poly_is_bottom(man,p1));
    interval_clear(&interval);
    for (i=0; i<intdim+realdim; i++){
      if (i!=dim){
	interval = poly_bound_dimension(man,p3,i);
	assert(interval_is_leq(&interval,&box[i]));
	assert(interval_is_leq(&box[i],&interval));
	interval_clear(&interval);
	interval = poly_bound_dimension(man,p4,i);
	assert(interval_is_leq(&interval,&box[i]) || 
	       poly_is_bottom(man,p1));
	assert(interval_is_leq(&box[i],&interval) || 
	       poly_is_bottom(man,p1));
	interval_clear(&interval);
      }
    }
    poly_free(man,p3);
    poly_free(man,p4);
  }
  interval_array_free(box,intdim+realdim);
  /* project and forget array */
  PRINT((printf("project and forget array\n"))); 
  p3 = poly_project_array(man,p1,tdim,3);
  p4 = poly_copy(man,p1);
  poly_project_array_with(man,p4,tdim,3);  
  assert(poly_is_eq(man,p3,p4));
  poly_free(man,p4);
  p4 = poly_forget_array(man,p1,tdim,3);
  p5 = poly_copy(man,p1);
  poly_forget_array_with(man,p5,tdim,3);  
  assert(poly_is_eq(man,p4,p5));
  poly_free(man,p5);
  assert(poly_is_leq(man,p3,p4));
  assert(poly_is_leq(man,p4,p3)==false || 
	   poly_is_bottom(man,p1));
  for (k=0;k<3;k++){
    dim = tdim[k];
    interval = poly_bound_dimension(man,p3,dim);
    assert(coeff_equal(&interval.inf,&interval.sup)|| 
	   poly_is_bottom(man,p1));
    assert(coeff_cmp(&interval.inf,&coeffzero)==0|| 
	   poly_is_bottom(man,p1));
    interval_clear(&interval);
    interval = poly_bound_dimension(man,p4,dim);
    assert(interval_is_top(&interval) || 
	   poly_is_bottom(man,p1));
    interval_clear(&interval);
  }  
  poly_free(man,p3);
  poly_free(man,p4);
  
  /* change and permutation of dimensions */
  PRINT((printf("change and permutation of dimensions\n"))); 
  perm = malloc((intdim+realdim)*2*sizeof(dim_t));
  permutation_set_id(perm,(intdim+realdim)*2);
  for (i=0;i<2*intdim; i++){
    perm[i] = 2*intdim-1-i;
  }
  for (i=0;i<2*realdim; i++){
    perm[2*intdim+i] = 2*(intdim+realdim)-1-i;
  }
  for (b=0;b<2;b++){
    p3 = poly_add_permute_dimensions(man,p1,intdim,realdim,perm,b);
    p4 = poly_copy(man,p1);
    poly_add_permute_dimensions_with(man,p4,intdim,realdim,perm,b);
    assert(poly_is_eq(man,p3,p4));
    poly_free(man,p4);
    p4 = poly_permute_remove_dimensions(man,p3,intdim,realdim,perm);
    p5 = poly_copy(man,p3);
    poly_permute_remove_dimensions_with(man,p5,intdim,realdim,perm);
    assert(poly_is_eq(man,p4,p5));
    poly_free(man,p5);
    assert(poly_is_eq(man,p1,p4));
    poly_free(man,p3);
    poly_free(man,p4);
  }
  free(perm);

  mpq_clear(mpq);
  mpq_clear(mpqone);
  coeff_clear(&coeffzero);
}

void poly_test(size_t intdim, size_t realdim,
	       size_t nbcons, /* Number of constraints */
	       size_t maxeq, /* Maximum number of equations */
	       size_t maxcoeff, /* Maximum size of non-null coefficients */
	       unsigned int mag /* magnitude of coefficients */
	       )
{
  pk_internal_t* pk;
  manager_t* man;
  char** name_of_dim;
  poly_t** tpoly;
  linexpr0_t** texpr;
  dim_t* tdim;
  int i;
  funid_t funid;

  man = pk_manager_alloc(false);
  pk = (pk_internal_t*)man->internal;
  pk_set_max_coeff_size(pk,0);
  pk_set_approximate_max_coeff_size(pk,10);
  
  for (i=0; i<3; i++){
    printf("%d******************************************************************\n",i);
    poly_test_gen(man,intdim,realdim,
		  nbcons,maxeq,maxcoeff,mag,
		  &name_of_dim,
		  &tpoly,
		  &texpr,
		  &tdim);
    poly_test_check(man,intdim,realdim,
		    name_of_dim,tpoly,texpr,tdim);

    for (funid=0; funid<FUNID_SIZE; funid++){
      funopt_t funopt;
      funopt_init(&funopt);
      funopt.algorithm = -1;
      manager_set_funopt(man,funid,&funopt);
    }
    
    poly_test_check(man,intdim,realdim,
		    name_of_dim,tpoly,texpr,tdim);
    
    poly_test_free(man,intdim,realdim,
		   name_of_dim,tpoly,texpr,tdim);
  }
  manager_free(man);
}

int main(int argc, char**argv)
{
  poly_test_example();

  srand(31);

  poly_test(0,6,6,1,3,20);
  poly_test(6,0,6,1,3,20);
  poly_test(6,10,6,1,3,20);
  poly_test(6,10,10,2,5,20);
 

  /* good for approximate(1) */
  /*
  srand(31);
  poly_test(10,10,14,2,5,1000000); 
  */
  /*
  srand(31);
  poly_test(0,10,25,0,4,1000000); 
  */
  /*
  poly_test(10,10,30,2,5,5);
  */
}
