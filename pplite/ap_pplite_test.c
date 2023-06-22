/*
 * ap_pplite_test.c
 *
 * APRON Library / PPLite library wrapper
 * Unit testing. Compares NewPolka & PPLite implementations.
 *
 * Copyright (C) Antoine Mine' 2006
 * Copyright (C) 2021-2023 Enea Zaffanella <enea.zaffanella@unipr.it>
 *
 */

/*
 * This file is part of the APRON Library, released under GPL license.
 * Please read the COPYING file packaged in the distribution.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "ap_global0.h"
#include "../newpolka/pk.h"
#include "ap_pplite.h"

ap_manager_t* eq_check_man;
ap_manager_t* d1_man;
ap_manager_t* d2_man;

/* these globals are interpreted as booleans */
/* strict == 1 ---> NNC polyhedra */
int strict;
/* use_intdims == 1 ---> some dimensions are on integral values */
int use_intdims;

/*
  EZ: when timing operations, lazy/eager library modes cause a bias;
  to reduce this bias, we systematically switch to eager mode
  by calling ap_abstract0_minimize().
*/

clock_t c_start, c_end;
double incr;
/* These will be reset after changing strict/intdim mode */
double d1_time = 0.0;
double d2_time = 0.0;
double ctor_time = 0.0;
double eq_check_time = 0.0;
/* These will collect total time (no reset) */
double total_d1_time = 0.0;
double total_d2_time = 0.0;
double total_ctor_time = 0.0;
double total_eq_check_time = 0.0;

#define START_CLOCK                             \
  do { c_start = clock(); } while (0)

#define STOP_CLOCK(time)                                \
  do {                                                  \
    c_end = clock();                                    \
    incr = (double) (c_end - c_start) / CLOCKS_PER_SEC; \
    (time) += incr;                                     \
  } while (0)

#define PRINT_CLOCKS                                           \
  do {                                                         \
    printf("\n\nTime spent in poly operators"                  \
           " (strict=%i, use_intdims=%i)"                      \
           "\n- 1st library (%s) ops: %f s"                    \
           "\n- 2nd library (%s) ops: %f s"                    \
           "\n- constructions/conversions: %f s"               \
           "\n- equality checks: %f s\n\n",                    \
           (strict), (use_intdims),                            \
           d1_man->library, d1_time,                           \
           d2_man->library, d2_time,                           \
           ctor_time, eq_check_time);                          \
    total_d1_time += d1_time;                                  \
    d1_time = 0.0;                                             \
    total_d2_time += d2_time;                                  \
    d2_time = 0.0;                                             \
    total_ctor_time += ctor_time;                              \
    ctor_time = 0.0;                                           \
    total_eq_check_time += eq_check_time;                      \
    eq_check_time = 0.0;                                       \
  } while (0)

#define PRINT_TOTAL_CLOCKS                                     \
  do {                                                         \
    printf("\n\nTotal time spent in poly operators"            \
           "\n- 1st library ops: %f s"                         \
           "\n- 2nd library ops: %f s"                         \
           "\n- constructions/conversions: %f s"               \
           "\n- equality checks: %f s\n\n",                    \
           total_d1_time, total_d2_time,                       \
           total_ctor_time, total_eq_check_time);              \
  } while (0)

/* utilities */
/* ********* */

void free_aux2(ap_abstract0_t* a1, ap_abstract0_t* a2) {
  ap_abstract0_free(a1->man, a1);
  ap_abstract0_free(a2->man, a2);
}
void free_aux4(ap_abstract0_t* a1, ap_abstract0_t* a2,
            ap_abstract0_t* a3, ap_abstract0_t* a4) {
  free_aux2(a1, a2);
  free_aux2(a3, a4);
}
void free_aux6(ap_abstract0_t* a1, ap_abstract0_t* a2,
            ap_abstract0_t* a3, ap_abstract0_t* a4,
            ap_abstract0_t* a5, ap_abstract0_t* a6) {
  free_aux4(a1, a2, a3, a4);
  free_aux2(a5, a6);
}

void random_interval(ap_interval_t* i)
{
  int n1 = rand()%10-5;
  int n2 = n1 + rand()%10;
  int d  = rand()%3+1;
  /* EZ: this increment makes sure that the interval will not be empty
     even when the space dimension is an integral one; otherwise,
     we will likely cause a precondition failure in test_add_ray()
     (it is illegal to add rays to an empty polyhedron). */
  n2 += d;
  ap_interval_set_frac(i,n1,d,n2,d);
}

/* random affine expression */
ap_linexpr0_t* random_linexpr(int dim)
{
  ap_linexpr0_t* l = ap_linexpr0_alloc(AP_LINEXPR_DENSE,dim);
  int i;
  for (i=0;i<dim;i++)
    ap_coeff_set_scalar_frac(l->p.coeff+i,rand()%20-10,rand()%4+1);
  ap_coeff_set_scalar_frac(&l->cst,rand()%20-10,rand()%4+1);
  return l;
}

/* random affine expression with scalar constant coefficient */
ap_linexpr0_t* random_linexpr_inter(int dim)
{
  ap_linexpr0_t* l = random_linexpr(dim);
  int n1 = rand()%20-10;
  int n2 = n1 + rand()%20;
  int d  = rand()%4+1;
  ap_linexpr0_set_cst_interval_frac(l,n1,d,n2,d);
  return l;
}

/* random generator of specified type */
ap_generator0_t random_generator(int dim, ap_gentyp_t g)
{
  ap_linexpr0_t* l = ap_linexpr0_alloc(AP_LINEXPR_DENSE,dim);
  int i;
  for (i=0;i<dim;i++)
    ap_coeff_set_scalar_frac(l->p.coeff+i,rand()%20-2,rand()%4+1);
  ap_coeff_set_scalar_int(&l->cst,1);
  return ap_generator0_make(g,l);
}

/* random polyhedron */
ap_abstract0_t* random_poly(ap_manager_t* man,int dim)
{
  int i;
  ap_abstract0_t* p;
  ap_interval_t** t = ap_interval_array_alloc(dim);
  ap_generator0_array_t ar = ap_generator0_array_make(dim);
  for (i=0;i<dim;i++)
    random_interval(t[i]);
  for (i=0;i<dim;i++)
    ar.p[i] = random_generator(dim,AP_GEN_RAY);
  START_CLOCK;
  if (use_intdims)
    p = ap_abstract0_of_box(man,dim/2,dim-dim/2,t);
  else
    p = ap_abstract0_of_box(man,0,dim,t);
  ap_abstract0_add_ray_array(man,true,p,&ar);
  ap_abstract0_minimize(man,p);
  STOP_CLOCK(ctor_time);
  ap_generator0_array_clear(&ar);
  ap_interval_array_free(t,dim);
  return p;
}

/* convert to specified manager */
ap_abstract0_t* convert(ap_manager_t* man, ap_abstract0_t* a)
{
  /* EZ: conversion to same manager is NOT the identity function
     (e.g., it may tighten constraints on integral dims). */
  ap_lincons0_array_t l;
  ap_dimension_t d = ap_abstract0_dimension(a->man,a);
  START_CLOCK;
  l = ap_abstract0_to_lincons_array(a->man,a);
  a = ap_abstract0_of_lincons_array(man,d.intdim,d.realdim,&l);
  ap_abstract0_minimize(man,a);
  STOP_CLOCK(ctor_time);
  ap_lincons0_array_clear(&l);
  return a;
}

/* compare */
bool is_eq(ap_abstract0_t* a1,ap_abstract0_t* a2)
{
  /* EZ: converting both, so that integrality is taken into account */
  ap_abstract0_t* aa1 = convert(eq_check_man,a1);
  ap_abstract0_t* aa2 = convert(eq_check_man,a2);
  START_CLOCK;
  bool r = ap_abstract0_is_eq(aa1->man,aa1,aa2);
  STOP_CLOCK(eq_check_time);
  free_aux2(aa1,aa2);
  return r;
}

/* print */
void print_poly(const char* msg, ap_abstract0_t* p)
{
  fprintf(stderr,"%s (%s) = ",msg,p->man->library);
  ap_abstract0_fprint(stderr,p->man,p,NULL);
  ap_abstract0_fdump(stderr,p->man,p);
  fprintf(stderr,"\n");
}


/* loop */
/* **** */

#define N 70

char b1_[N+4]= " [";
char b2_[N+4];
int i_;
int error_ = 0;

#define LOOP                                        \
  {                                                 \
    memset(b1_+2,' ',N); b1_[N+2]=']'; b1_[N+3]=0;  \
    memset(b2_,8,N+3); b2_[N+3]=0;                  \
    for (i_=0;i_<N;i_++) {                          \
    printf("%s%s",b1_,b2_);                         \
    fflush(stdout);

#define RESULT(c) b1_[i_+2]=c

#define ERROR(msg)                 \
  do {                             \
    fprintf(stderr,"\n%s\n",msg);  \
    RESULT('!');                   \
    error_++;                      \
  } while (0)

#define ENDLOOP        } } printf("%s%s\n",b1_,b2_)




/* tests */
/* ***** */

void test_conv(void)
{
  int dim = 5;
  printf("\nconversions\n");
  LOOP {
    ap_abstract0_t* d1_0 = random_poly(d1_man,dim);
    ap_abstract0_t* d2_0 = convert(d2_man,d1_0);
    ap_abstract0_t* d1_1 = convert(d1_man,d2_0);
    ap_abstract0_t* d2_1 = convert(d2_man,d1_1);
    RESULT('*');
    if (!is_eq(d1_0,d1_1) || !is_eq(d2_0,d2_1)) {
      ERROR("different results");
      print_poly("d1_0",d1_0); print_poly("d2_0",d2_0);
      print_poly("d1_1",d1_1); print_poly("d2_1",d2_1);
    }
    free_aux4(d1_0,d1_1,d2_0,d2_1);
  } ENDLOOP;
}


void test_join(void)
{
  int dim = 4;
  printf("\nbinary join\n");
  LOOP {
    ap_abstract0_t* d1_0 = random_poly(d1_man,dim);
    ap_abstract0_t* d1_1 = random_poly(d1_man,dim);
    START_CLOCK;
    ap_abstract0_t* d1_r = ap_abstract0_join(d1_man,false,d1_0,d1_1);
    ap_abstract0_minimize(d1_man,d1_r);
    STOP_CLOCK(d1_time);
    ap_abstract0_t* d2_0 = convert(d2_man,d1_0);
    ap_abstract0_t* d2_1 = convert(d2_man,d1_1);
    START_CLOCK;
    ap_abstract0_t* d2_r = ap_abstract0_join(d2_man,false,d2_0,d2_1);
    ap_abstract0_minimize(d2_man,d2_r);
    STOP_CLOCK(d2_time);
    RESULT('*');
    if (!is_eq(d1_r,d2_r)) {
      ERROR("different results");
      print_poly("d1_0",d1_0); print_poly("d1_1",d1_1);
      print_poly("d1_r",d1_r); print_poly("d2_r",d2_r);
    }
    free_aux6(d1_0,d1_1,d1_r,d2_0,d2_1,d2_r);
  } ENDLOOP;
}

#define NB 3

void test_join_array(void)
{
  int i,dim = 3;
  printf("\narray join\n");
  LOOP {
    ap_abstract0_t* d1_a[NB], *d1_r;
    ap_abstract0_t* d2_a[NB], *d2_r;
    for (i=0;i<NB;i++) {
      d1_a[i] = random_poly(d1_man,dim);
      d2_a[i] = convert(d2_man,d1_a[i]);
    }
    START_CLOCK;
    d1_r = ap_abstract0_join_array(d1_man,(ap_abstract0_t**)d1_a,NB);
    ap_abstract0_minimize(d1_man,d1_r);
    STOP_CLOCK(d1_time);
    START_CLOCK;
    d2_r = ap_abstract0_join_array(d2_man,(ap_abstract0_t**)d2_a,NB);
    ap_abstract0_minimize(d2_man,d2_r);
    STOP_CLOCK(d2_time);
    RESULT('*');
    if (!is_eq(d1_r,d2_r)) {
      ERROR("different results");
      print_poly("d1_r",d1_r); print_poly("d2_r",d2_r);
    }
    for (i=0;i<NB;i++) {
      free_aux2(d1_a[i],d2_a[i]);
    }
    free_aux2(d1_r,d2_r);
  } ENDLOOP;
}

void test_meet(void)
{
  int dim = 4;
  printf("\nbinary meet\n");
  LOOP {
    ap_abstract0_t* d1_0 = random_poly(d1_man,dim);
    ap_abstract0_t* d1_1 = random_poly(d1_man,dim);
    START_CLOCK;
    ap_abstract0_t* d1_r = ap_abstract0_meet(d1_man,false,d1_0,d1_1);
    ap_abstract0_minimize(d1_man,d1_r);
    STOP_CLOCK(d1_time);
    ap_abstract0_t* d2_0 = convert(d2_man,d1_0);
    ap_abstract0_t* d2_1 = convert(d2_man,d1_1);
    START_CLOCK;
    ap_abstract0_t* d2_r = ap_abstract0_meet(d2_man,false,d2_0,d2_1);
    ap_abstract0_minimize(d2_man,d2_r);
    STOP_CLOCK(d2_time);
    RESULT('*');
    if (!is_eq(d1_r,d2_r)) {
      ERROR("different results");
      print_poly("d1_0",d1_0); print_poly("d1_1",d1_1);
      print_poly("d1_r",d1_r); print_poly("d2_r",d2_r);
    }
    free_aux6(d1_0,d1_1,d1_r,d2_0,d2_1,d2_r);
  } ENDLOOP;
}

void test_meet_array(void)
{
  int i,dim = 3;
  printf("\narray meet\n");
  LOOP {
    ap_abstract0_t* d1_a[NB], *d1_r;
    ap_abstract0_t* d2_a[NB], *d2_r;
    for (i=0;i<NB;i++) {
      d1_a[i] = random_poly(d1_man,dim);
      d2_a[i] = convert(d2_man,d1_a[i]);
    }
    START_CLOCK;
    d1_r = ap_abstract0_meet_array(d1_man,(ap_abstract0_t**)d1_a,NB);
    ap_abstract0_minimize(d1_man,d1_r);
    STOP_CLOCK(d1_time);
    START_CLOCK;
    d2_r = ap_abstract0_meet_array(d2_man,(ap_abstract0_t**)d2_a,NB);
    ap_abstract0_minimize(d2_man,d2_r);
    STOP_CLOCK(d2_time);
    RESULT('*');
    if (!is_eq(d1_r,d2_r)) {
      ERROR("different results");
      print_poly("d1_r",d1_r);
      print_poly("d2_r",d2_r);
    }
    for (i=0;i<NB;i++) {
      free_aux2(d1_a[i],d2_a[i]);
    }
    free_aux2(d1_r,d2_r);
  } ENDLOOP;
}


void test_dimadd(void)
{
  printf("\nadd dimensions\n");
  LOOP {
    size_t i, dim = 5;
    ap_dimchange_t* a = ap_dimchange_alloc(0,3);
    ap_abstract0_t* d1_a,*d1_r, *d2_a,*d2_r;
    int proj = rand()%2;
    ap_dimension_t d;
    d1_a = random_poly(d1_man,dim);
    d2_a = convert(d2_man,d1_a);
    d = ap_abstract0_dimension(d1_man,d1_a);
    for (i=0;i<a->intdim+a->realdim;i++) {
      a->dim[i] = rand()%2;
      if (i) a->dim[i] += a->dim[i-1];
      if (a->dim[i]<d.intdim) { a->intdim++; a->realdim--; }
      assert(a->dim[i]<dim);
    }
    START_CLOCK;
    d1_r = ap_abstract0_add_dimensions(d1_man,false,d1_a,a,proj);
    ap_abstract0_minimize(d1_man,d1_r);
    STOP_CLOCK(d1_time);
    START_CLOCK;
    d2_r = ap_abstract0_add_dimensions(d2_man,false,d2_a,a,proj);
    ap_abstract0_minimize(d2_man,d2_r);
    STOP_CLOCK(d2_time);
    RESULT('*');
    if (!is_eq(d1_r,d2_r)) {
      ERROR("different results");
      ap_dimchange_fprint(stderr,a);
      print_poly("d1_a",d1_a);
      print_poly("d1_r",d1_r);
      print_poly("d2_r",d2_r);
    }
    free_aux4(d1_a,d1_r,d2_a,d2_r);
    ap_dimchange_free(a);
  } ENDLOOP;
}

void test_dimrem(void)
{
  printf("\nremove dimensions\n");
  LOOP {
    size_t i, dim = 5;
    ap_dimchange_t* a = ap_dimchange_alloc(0,2);
    ap_abstract0_t* d1_a,*d1_r, *d2_a,*d2_r;
    ap_dimension_t d;
    d1_a = random_poly(d1_man,dim);
    d2_a = convert(d2_man,d1_a);
    d = ap_abstract0_dimension(d1_man,d1_a);
    for (i=0;i<a->intdim+a->realdim;i++) {
      a->dim[i] = rand()%2 + 1;
      if (i) a->dim[i] += a->dim[i-1];
      if (a->dim[i]<d.intdim) { a->intdim++; a->realdim--; }
      assert(a->dim[i]<dim);
    }
    START_CLOCK;
    d1_r = ap_abstract0_remove_dimensions(d1_man,false,d1_a,a);
    ap_abstract0_minimize(d1_man,d1_r);
    STOP_CLOCK(d1_time);
    START_CLOCK;
    d2_r = ap_abstract0_remove_dimensions(d2_man,false,d2_a,a);
    ap_abstract0_minimize(d2_man,d2_r);
    STOP_CLOCK(d2_time);
    RESULT('*');
    if (!is_eq(d1_r,d2_r)) {
      ERROR("different results");
      ap_dimchange_fprint(stderr,a);
      print_poly("d1_a",d1_a);
      print_poly("d1_r",d1_r);
      print_poly("d2_r",d2_r);
    }
    free_aux4(d1_a,d1_r,d2_a,d2_r);
    ap_dimchange_free(a);
  } ENDLOOP;
}

void test_forget(void)
{
  printf("\nforget\n");
  LOOP {
    size_t i, dim = 5;
    ap_dimchange_t* a = ap_dimchange_alloc(0,2);
    ap_abstract0_t* d1_a,*d1_r, *d2_a,*d2_r;
    ap_dimension_t d;
    int proj = rand()%2;
    d1_a = random_poly(d1_man,dim);
    d2_a = convert(d2_man,d1_a);
    d = ap_abstract0_dimension(d1_man,d1_a);
    for (i=0;i<a->intdim+a->realdim;i++) {
      a->dim[i] = rand()%2 + 1;
      if (i) a->dim[i] += a->dim[i-1];
      if (a->dim[i]<d.intdim) { a->intdim++; a->realdim--; }
      assert(a->dim[i]<dim);
    }
    START_CLOCK;
    d1_r = ap_abstract0_forget_array(d1_man,false,d1_a,a->dim,a->realdim,proj);
    ap_abstract0_minimize(d1_man,d1_r);
    STOP_CLOCK(d1_time);
    START_CLOCK;
    d2_r = ap_abstract0_forget_array(d2_man,false,d2_a,a->dim,a->realdim,proj);
    ap_abstract0_minimize(d2_man,d2_r);
    STOP_CLOCK(d2_time);
    RESULT('*');
    if (!is_eq(d1_r,d2_r)) {
      ERROR("different results");
      ap_dimchange_fprint(stderr,a);
      print_poly("d1_a",d1_a);
      print_poly("d1_r",d1_r);
      print_poly("d2_r",d2_r);
    }
    free_aux4(d1_a,d1_r,d2_a,d2_r);
    ap_dimchange_free(a);
  } ENDLOOP;
}

void test_permute(void)
{
  printf("\npermute dimensions\n");
  LOOP {
    size_t i, dim = 5;
    ap_dimperm_t* p = ap_dimperm_alloc(dim);
    ap_abstract0_t* d1_a,*d1_r, *d2_a,*d2_r;
    ap_dimension_t d;
    d1_a = random_poly(d1_man,dim);
    d2_a = convert(d2_man,d1_a);
    d = ap_abstract0_dimension(d1_man,d1_a);
    /* random permutation */
    ap_dimperm_set_id(p);
    for (i=0;i<p->size-1;i++) {
      int a, j;
      if (i<d.intdim) j = i + (rand() % (d.intdim-i));
      else j = i + (rand() % (d.intdim+d.realdim-i));
      a = p->dim[j]; p->dim[j] = p->dim[i]; p->dim[i] = a;
    }
    START_CLOCK;
    d1_r = ap_abstract0_permute_dimensions(d1_man,false,d1_a,p);
    ap_abstract0_minimize(d1_man,d1_r);
    STOP_CLOCK(d1_time);
    START_CLOCK;
    d2_r = ap_abstract0_permute_dimensions(d2_man,false,d2_a,p);
    ap_abstract0_minimize(d2_man,d2_r);
    STOP_CLOCK(d2_time);
    RESULT('*');
    if (!is_eq(d1_r,d2_r)) {
      ERROR("different results");
      ap_dimperm_fprint(stderr,p);
      print_poly("d1_a",d1_a);
      print_poly("d1_r",d1_r);
      print_poly("d2_r",d2_r);
    }
    free_aux4(d1_a,d1_r,d2_a,d2_r);
    ap_dimperm_free(p);
  } ENDLOOP;
}

void test_expand(void)
{
  printf("\nexpand dimensions\n");
  LOOP {
    size_t dim = 4; /* EZ: was 5, leading to a very slow test */
    ap_dim_t dd = rand() % dim;
    size_t n = (rand() % 2) + 1;
    ap_abstract0_t* d1_a,*d1_r, *d2_a,*d2_r;
    d1_a = random_poly(d1_man,dim);
    d2_a = convert(d2_man,d1_a);
    START_CLOCK;
    d1_r = ap_abstract0_expand(d1_man,false,d1_a,dd,n);
    ap_abstract0_minimize(d1_man,d1_r);
    STOP_CLOCK(d1_time);
    START_CLOCK;
    d2_r = ap_abstract0_expand(d2_man,false,d2_a,dd,n);
    ap_abstract0_minimize(d2_man,d2_r);
    STOP_CLOCK(d2_time);
    RESULT('*');
    if (!is_eq(d1_r,d2_r)) {
      ERROR("different results");
      fprintf(stderr,"dim %i expanded %i times\n",(int)dd,(int)n);
      print_poly("d1_a",d1_a);
      print_poly("d1_r",d1_r);
      print_poly("d2_r",d2_r);
    }
    free_aux4(d1_a,d1_r,d2_a,d2_r);
  } ENDLOOP;
}

void test_fold(void)
{
  printf("\nfold dimensions\n");
  LOOP {
    size_t dim = 5;
    ap_dim_t dd[3];
    ap_abstract0_t* d1_a,*d1_r, *d2_a,*d2_r;
    ap_dimension_t d;
    d1_a = random_poly(d1_man,dim);
    d2_a = convert(d2_man,d1_a);
    d = ap_abstract0_dimension(d1_man,d1_a);
    do {
      dd[0] = rand() % dim;
      if (dd[0]<d.intdim) dd[1] = rand() % (d.intdim-1);
      else dd[1] = d.intdim + rand() % (d.realdim-1);
    }
    while (dd[0]>=dd[1]);
    START_CLOCK;
    d1_r = ap_abstract0_fold(d1_man,false,d1_a,dd,2);
    ap_abstract0_minimize(d1_man,d1_r);
    STOP_CLOCK(d1_time);
    START_CLOCK;
    d2_r = ap_abstract0_fold(d2_man,false,d2_a,dd,2);
    ap_abstract0_minimize(d2_man,d2_r);
    STOP_CLOCK(d2_time);
    RESULT('*');
    if (!is_eq(d1_r,d2_r)) {
      ERROR("different results");
      fprintf(stderr,"fold %i,%i,%i\n",(int)dd[0],(int)dd[1],(int)dd[2]);
      print_poly("d1_a",d1_a);
      print_poly("d1_r",d1_r);
      print_poly("d2_r",d2_r);
    }
    free_aux4(d1_a,d1_r,d2_a,d2_r);
  } ENDLOOP;
}


void test_add_lincons(void)
{
  printf("\nadd lincons\n");
  LOOP {
    size_t i, dim = 5, nb = 4;
    ap_abstract0_t* d1_a,*d1_r, *d2_a,*d2_r;
    ap_lincons0_array_t ar = ap_lincons0_array_make(nb);
    d1_a = random_poly(d1_man,dim);
    d2_a = convert(d2_man,d1_a);
    for (i=0;i<nb;i++) {
      ar.p[i] = ap_lincons0_make((rand()%100>=90)?AP_CONS_EQ:
                                 (rand()%100>=90)?AP_CONS_SUP:AP_CONS_SUPEQ,
                                 random_linexpr(dim),NULL);
    }
    START_CLOCK;
    d1_r = ap_abstract0_meet_lincons_array(d1_man,false,d1_a,&ar);
    ap_abstract0_minimize(d1_man,d1_r);
    STOP_CLOCK(d1_time);
    START_CLOCK;
    d2_r = ap_abstract0_meet_lincons_array(d2_man,false,d2_a,&ar);
    ap_abstract0_minimize(d2_man,d2_r);
    STOP_CLOCK(d2_time);
    RESULT('*');
    if (!is_eq(d1_r,d2_r)) {
      ERROR("different results");
      ap_lincons0_array_fprint(stderr,&ar,NULL);
      print_poly("d1_a",d1_a);
      print_poly("d1_r",d1_r);
      print_poly("d2_r",d2_r);
    }
    free_aux4(d1_a,d1_r,d2_a,d2_r);
    ap_lincons0_array_clear(&ar);
  } ENDLOOP;
}

void test_add_ray(void)
{
  printf("\nadd rays\n");
  LOOP {
    size_t i, dim = 4, nb = 4;
    ap_abstract0_t* d1_a,*d1_r, *d2_a,*d2_r;
    ap_generator0_array_t ar = ap_generator0_array_make(nb);
    d1_a = random_poly(d1_man,dim);
    d2_a = convert(d2_man,d1_a);
    for (i=0;i<nb;i++)
      ar.p[i] = random_generator(dim,(rand()%100>=80)?AP_GEN_LINE:AP_GEN_RAY);
    START_CLOCK;
    d1_r = ap_abstract0_add_ray_array(d1_man,false,d1_a,&ar);
    ap_abstract0_minimize(d1_man,d1_r);
    STOP_CLOCK(d1_time);
    START_CLOCK;
    d2_r = ap_abstract0_add_ray_array(d2_man,false,d2_a,&ar);
    ap_abstract0_minimize(d2_man,d2_r);
    STOP_CLOCK(d2_time);
    RESULT('*');
    if (!is_eq(d1_r,d2_r)) {
      ERROR("different results");
      ap_generator0_array_fprint(stderr,&ar,NULL);
      print_poly("d1_a",d1_a);
      print_poly("d1_r",d1_r);
      print_poly("d2_r",d2_r);
    }
    free_aux4(d1_a,d1_r,d2_a,d2_r);
    ap_generator0_array_clear(&ar);
  } ENDLOOP;
}

void test_box(void)
{
  printf("\nbox conversion\n");
  LOOP {
    size_t i, dim = 5;
    ap_abstract0_t* d1_a,*d1_r, *d2_a,*d2_r;
    ap_interval_t** d1_i,**d2_i;
    d1_a = random_poly(d1_man,dim);
    d2_a = convert(d2_man,d1_a);
    START_CLOCK;
    d1_i = ap_abstract0_to_box(d1_man,d1_a);
    d1_r = ap_abstract0_of_box(d1_man,0,dim,(ap_interval_t**)d1_i);
    ap_abstract0_minimize(d1_man,d1_r);
    STOP_CLOCK(d1_time);
    START_CLOCK;
    d2_i = ap_abstract0_to_box(d2_man,d2_a);
    d2_r = ap_abstract0_of_box(d2_man,0,dim,(ap_interval_t**)d2_i);
    ap_abstract0_minimize(d2_man,d2_r);
    STOP_CLOCK(d2_time);
    RESULT('*');
    if (!is_eq(d1_r,d2_r)) {
      ERROR("different results");
      for (i=0;i<dim;i++) {
        fprintf(stderr,"d1_i[%i]=",(int)i);
        ap_interval_fprint(stderr,d1_i[i]);
        fprintf(stderr," d2_i[%i]=",(int)i);
        ap_interval_fprint(stderr,d2_i[i]);
        fprintf(stderr,"\n");
      }
      print_poly("d1_a",d1_a);
      print_poly("d1_r",d1_r);
      print_poly("d2_r",d2_r);
    }
    free_aux4(d1_a,d1_r,d2_a,d2_r);
    ap_interval_array_free(d1_i,dim);
    ap_interval_array_free(d2_i,dim);
  } ENDLOOP;

}

void test_vbound(void)
{
  printf("\nvariable bound\n");
  LOOP {
    size_t i, dim = 5;
    ap_abstract0_t* d1_a,*d2_a;
    d1_a = random_poly(d1_man,dim);
    d2_a = convert(d2_man,d1_a);
    RESULT('*');
    for (i=0;i<dim;i++) {
      bool d1_d,d2_d;
      ap_interval_t* d1_i,*d2_i;
      START_CLOCK;
      d1_d = ap_abstract0_is_dimension_unconstrained(d1_man,d1_a,i);
      d1_i = ap_abstract0_bound_dimension(d1_man,d1_a,i);
      STOP_CLOCK(d1_time);
      START_CLOCK;
      d2_d = ap_abstract0_is_dimension_unconstrained(d2_man,d2_a,i);
      d2_i = ap_abstract0_bound_dimension(d2_man,d2_a,i);
      STOP_CLOCK(d2_time);
      if (d1_d!=d2_d || ap_interval_cmp(d1_i,d2_i)) {
        ERROR("different results");
        print_poly("d1_a",d1_a);
        fprintf(stderr,"d1_i[%i]=",(int)i); ap_interval_fprint(stderr,d1_i);
        fprintf(stderr," d2_i[%i]=",(int)i); ap_interval_fprint(stderr,d2_i);
        fprintf(stderr," d1_d=%i d2_d=%i\n",d1_d,d2_d);
      }
      ap_interval_free(d1_i);
      ap_interval_free(d2_i);
    }
    free_aux2(d1_a,d2_a);
  } ENDLOOP;
}

void test_lbound(void)
{
  printf("\nlinear expression bound\n");
  LOOP {
    size_t dim = 5;
    ap_abstract0_t* d1_a,*d2_a;
    ap_interval_t* d1_i,*d2_i;
    ap_linexpr0_t* l = random_linexpr(dim);
    d1_a = random_poly(d1_man,dim);
    d2_a = convert(d2_man,d1_a);
    START_CLOCK;
    d1_i = ap_abstract0_bound_linexpr(d1_man,d1_a,l);
    STOP_CLOCK(d1_time);
    START_CLOCK;
    d2_i = ap_abstract0_bound_linexpr(d2_man,d2_a,l);
    STOP_CLOCK(d2_time);
    RESULT('*');
    if (ap_interval_cmp(d1_i,d2_i)) {
      ERROR("different results");
      print_poly("d1_a",d1_a);
      ap_linexpr0_fprint(stderr,l,NULL);
      fprintf(stderr,"\nd1_i="); ap_interval_fprint(stderr,d1_i);
      fprintf(stderr,"\nd2_i="); ap_interval_fprint(stderr,d2_i);
      fprintf(stderr,"\n");
    }
    free_aux2(d1_a,d2_a);
    ap_interval_free(d1_i);
    ap_interval_free(d2_i);
    ap_linexpr0_free(l);
  } ENDLOOP;
}

void test_csat(void)
{
  printf("\nconstraint saturation\n");
  LOOP {
    size_t dim = 5;
    ap_abstract0_t* d1_a,*d2_a;
    ap_lincons0_t l = ap_lincons0_make((rand()%100>=90)?AP_CONS_EQ:
                                       (rand()%100>=90)?AP_CONS_SUP:AP_CONS_SUPEQ,
                                       random_linexpr(dim),NULL);
    bool d1_s,d2_s;
    d1_a = random_poly(d1_man,dim);
    d2_a = convert(d2_man,d1_a);
    START_CLOCK;
    d1_s = ap_abstract0_sat_lincons(d1_man,d1_a,&l);
    STOP_CLOCK(d1_time);
    START_CLOCK;
    d2_s = ap_abstract0_sat_lincons(d2_man,d2_a,&l);
    STOP_CLOCK(d2_time);
    RESULT('*');
    if (d1_s!=d2_s) {
      ERROR("different results");
      print_poly("d1_a",d1_a);
      ap_lincons0_fprint(stderr,&l,NULL);
      fprintf(stderr,"\nd1_s=%i d2_s=%i\n",d1_s,d2_s);
    }
    free_aux2(d1_a,d2_a);
    ap_lincons0_clear(&l);
  } ENDLOOP;
}

void test_isat(void)
{
  printf("\ninterval saturation\n");
  LOOP {
    size_t dim = 5;
    size_t p = rand() % dim;
    ap_abstract0_t* d1_a,*d2_a;
    ap_interval_t* i = ap_interval_alloc();
    bool d1_s,d2_s;
    random_interval(i);
    d1_a = random_poly(d1_man,dim);
    d2_a = convert(d2_man,d1_a);
    START_CLOCK;
    d1_s = ap_abstract0_sat_interval(d1_man,d1_a,p,i);
    STOP_CLOCK(d1_time);
    START_CLOCK;
    d2_s = ap_abstract0_sat_interval(d2_man,d2_a,p,i);
    STOP_CLOCK(d2_time);
    RESULT('*');
    if (d1_s!=d2_s) {
      ERROR("different results");
      print_poly("d1_a",d1_a);
      ap_interval_fprint(stderr,i);
      fprintf(stderr,"\nvar=%i\nd1_s=%i d2_s=%i\n",(int)p,d1_s,d2_s);
    }
    free_aux2(d1_a,d2_a);
    ap_interval_free(i);
  } ENDLOOP;
}

void test_assign(void)
{
  printf("\nassign\n");
  LOOP {
    size_t dim = 5;
    size_t p = rand() % dim;
    ap_abstract0_t* d1_a,*d1_r, *d2_a,*d2_r;
    ap_linexpr0_t* l = random_linexpr(dim);

    d1_a = random_poly(d1_man,dim);
    d2_a = convert(d2_man,d1_a);
    START_CLOCK;
    d1_r = ap_abstract0_assign_linexpr(d1_man,false,d1_a,p,l,NULL);
    ap_abstract0_minimize(d1_man,d1_r);
    STOP_CLOCK(d1_time);
    START_CLOCK;
    d2_r = ap_abstract0_assign_linexpr(d2_man,false,d2_a,p,l,NULL);
    ap_abstract0_minimize(d2_man,d2_r);
    STOP_CLOCK(d2_time);
    RESULT('*');
    if (!is_eq(d1_r,d2_r)) {
      ERROR("different results");
      fprintf(stderr,"x%i <- ",(int)p);
      ap_linexpr0_fprint(stderr,l,NULL); fprintf(stderr,"\n");
      print_poly("d1_a",d1_a);
      print_poly("d1_r",d1_r);
      print_poly("d2_r",d2_r);
    }
    free_aux4(d1_a,d2_a,d1_r,d2_r);
    ap_linexpr0_free(l);
  } ENDLOOP;
}

void test_par_assign(void)
{
  printf("\nparallel assign\n");
  LOOP {
    size_t i, dim = 5;
    ap_abstract0_t* d1_a,*d1_r, *d2_a,*d2_r;
    ap_dim_t d[NB];
    ap_linexpr0_t *l[NB];
    d1_a = random_poly(d1_man,dim);
    d2_a = convert(d2_man,d1_a);
    for (i=0;i<NB;i++) {
      l[i]  = random_linexpr(dim);
      if (!i) d[i] = rand()%(dim-NB);
      else d[i] = d[i-1] + 1 + (rand()%(dim-NB+i-d[i-1]));
    }
    START_CLOCK;
    d1_r = ap_abstract0_assign_linexpr_array(d1_man,false,d1_a,
                                             d,l,NB,NULL);
    ap_abstract0_minimize(d1_man,d1_r);
    STOP_CLOCK(d1_time);
    START_CLOCK;
    d2_r = ap_abstract0_assign_linexpr_array(d2_man,false,d2_a,
                                             d,l,NB,NULL);
    ap_abstract0_minimize(d2_man,d2_r);
    STOP_CLOCK(d2_time);
    RESULT('*');
    if (!is_eq(d1_r,d2_r)) {
      ERROR("different results");
      for (i=0;i<NB;i++) {
        fprintf(stderr,"x%i <- ",d[i]);
        ap_linexpr0_fprint(stderr,l[i],NULL);
        fprintf(stderr,"\n");
      }
      print_poly("d1_a",d1_a);
      print_poly("d1_r",d1_r);
      print_poly("d2_r",d2_r);
    }
    free_aux4(d1_a,d2_a,d1_r,d2_r);
    for (i=0;i<NB;i++) ap_linexpr0_free(l[i]);
  } ENDLOOP;
}

void test_subst(void)
{
  printf("\nsubst\n");
  LOOP {
    size_t dim = 5;
    size_t p = rand() % dim;
    ap_abstract0_t* d1_a,*d1_r, *d2_a,*d2_r;
    ap_linexpr0_t* l = random_linexpr(dim);
    d1_a = random_poly(d1_man,dim);
    d2_a = convert(d2_man,d1_a);
    START_CLOCK;
    d1_r = ap_abstract0_substitute_linexpr(d1_man,false,d1_a,p,l,NULL);
    ap_abstract0_minimize(d1_man,d1_r);
    STOP_CLOCK(d1_time);
    START_CLOCK;
    d2_r = ap_abstract0_substitute_linexpr(d2_man,false,d2_a,p,l,NULL);
    ap_abstract0_minimize(d2_man,d2_r);
    STOP_CLOCK(d2_time);
    RESULT('*');
    if (!is_eq(d1_r,d2_r)) {
      ERROR("different results");
      fprintf(stderr,"x%i -> ",(int)p);
      ap_linexpr0_fprint(stderr,l,NULL); fprintf(stderr,"\n");
      print_poly("d1_a",d1_a);
      print_poly("d1_r",d1_r);
      print_poly("d2_r",d2_r);
    }
    free_aux4(d1_a,d2_a,d1_r,d2_r);
    ap_linexpr0_free(l);
  } ENDLOOP;
}

void test_par_subst(void)
{
  printf("\nparallel subst\n");
  LOOP {
    size_t i, dim = 5;
    ap_abstract0_t* d1_a,*d1_r, *d2_a,*d2_r;
    ap_dim_t d[NB];
    ap_linexpr0_t *l[NB];
    d1_a = random_poly(d1_man,dim);
    d2_a = convert(d2_man,d1_a);
    for (i=0;i<NB;i++) {
      l[i]  = random_linexpr(dim);
      if (!i) d[i] = rand()%(dim-NB);
      else d[i] = d[i-1] + 1 + (rand()%(dim-NB+i-d[i-1]));
    }
    START_CLOCK;
    d1_r = ap_abstract0_substitute_linexpr_array(d1_man,false,d1_a,
                                                 d,l,NB,NULL);
    ap_abstract0_minimize(d1_man,d1_r);
    STOP_CLOCK(d1_time);
    START_CLOCK;
    d2_r = ap_abstract0_substitute_linexpr_array(d2_man,false,d2_a,
                                                 d,l,NB,NULL);
    ap_abstract0_minimize(d2_man,d2_r);
    STOP_CLOCK(d2_time);
    RESULT('*');
    if (!is_eq(d1_r,d2_r)) {
      ERROR("different results");
      for (i=0;i<NB;i++) {
        fprintf(stderr,"x%i -> ",d[i]);
        ap_linexpr0_fprint(stderr,l[i],NULL);
        fprintf(stderr,"\n");
      }
      print_poly("d1_a",d1_a);
      print_poly("d1_r",d1_r);
      print_poly("d2_r",d2_r);
    }
    free_aux4(d1_a,d2_a,d1_r,d2_r);
    for (i=0;i<NB;i++) ap_linexpr0_free(l[i]);
  } ENDLOOP;
}

void test_widen(void)
{
  int dim = 5;
  printf("\nwidening\n");
  LOOP {
    ap_abstract0_t* d1_0, *d1_1, *d1_r;
    ap_abstract0_t* d2_0, *d2_1, *d2_r;
    d1_0 = random_poly(d1_man,dim);
    d1_1 = random_poly(d1_man,dim);
    d1_1 = ap_abstract0_join(d1_man,true,d1_1,d1_0);
    START_CLOCK;
    d1_r = ap_abstract0_widening(d1_man,d1_0,d1_1);
    ap_abstract0_minimize(d1_man,d1_r);
    STOP_CLOCK(d1_time);
    d2_0 = convert(d2_man,d1_0);
    d2_1 = convert(d2_man,d1_1);
    START_CLOCK;
    d2_r = ap_abstract0_widening(d2_man,d2_0,d2_1);
    ap_abstract0_minimize(d2_man,d2_r);
    STOP_CLOCK(d2_time);
    RESULT('*');
    if (!is_eq(d1_r,d2_r)) {
      ERROR("different results");
      print_poly("d1_0",d1_0); print_poly("d1_1",d1_1);
      print_poly("d1_r",d1_r); print_poly("d2_r",d2_r);
    }
    free_aux6(d1_0,d1_1,d1_r,d2_0,d2_1,d2_r);
  } ENDLOOP;
}


/* main */
/* **** */

int main(int argc, const char** argv)
{
  /* use first command line argument (if provided)
     to select a specific PPLite's poly kind (default Poly) */
  const char* poly_kind = (argc >= 2) ? argv[1] : "Poly";

  /* srand(time(NULL)); */
  srand(0);

  /* EZ: strict and use_intdims are "boolean" globals */
  for (use_intdims = 0; use_intdims < 2; ++use_intdims) {
  for (strict = 0; strict < 2; ++strict) {

    /* init managers */

    /* eq_check_man is the manager used to perform comparisons */
#if 1
    /* since we mean to check pplite's results, the comparisons
       are performed on domain pk, even though slower */
    eq_check_man = pk_manager_alloc(strict);
#else
    /* using PPLite's Poly kind (faster) */
    eq_check_man = ap_pplite_manager_alloc(strict);
    ap_pplite_manager_set_kind(eq_check_man, "Poly");
#endif

    /* d1_man is PPLite (using kind from command-line argument) */
    d1_man = ap_pplite_manager_alloc(strict);
    ap_pplite_manager_set_kind(d1_man, poly_kind);
    /* To be fair, select risky widening (inclusion precondition) */
    ap_pplite_manager_set_widen_spec(d1_man, "risky");

    /* d2_man is Apron's pk */
    d2_man = pk_manager_alloc(strict);

    for (int i=0;i<AP_EXC_SIZE;i++){
      eq_check_man->option.abort_if_exception[i] = true;
      d1_man->option.abort_if_exception[i] = true;
      d2_man->option.abort_if_exception[i] = true;
    }

    printf("\n\ncomparing libraries:\n- %s (%s), kind %s\n- %s (%s)\n"
           "with strict=%i use_intdims=%i\n\n",
           d1_man->library, d1_man->version, poly_kind,
           d2_man->library, d2_man->version,
           strict, use_intdims);

    /* run tests */
    test_conv();
    test_join();
    test_meet();
    test_join_array();
    test_meet_array();
    test_dimadd();
    test_dimrem();
    test_forget();
    test_permute();
    test_expand();
    test_fold();
    test_add_lincons();
    test_add_ray();
    test_box();
    test_vbound();
    test_lbound();
    test_csat();
    test_isat();
    test_assign();
    test_par_assign();
    test_subst();
    test_par_subst();
    /* EZ: widening can behave differently in strict mode, but we are lucky */
    /* if (!strict) test_widen(); */
    test_widen();

    PRINT_CLOCKS;

    /* clean-up */
    ap_manager_free(eq_check_man);
    ap_manager_free(d1_man);
    ap_manager_free(d2_man);

  } /* loop on strict */
  } /* loop on intdim */

  PRINT_TOTAL_CLOCKS;

  if (error_) {
    printf("\n%i error(s)!\n",error_);
    return -1;
  }
  printf("\nall tests passed\n");
  return 0;
}
