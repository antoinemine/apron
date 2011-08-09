/* GENERATED FROM domain.c.tmpl, DO NOT MODIFY */

/* -*- mode: c -*- */

/*
 * test_domains.c.tmpl
 *
 * Unit testing. Compares domains & implementations.
 *
 * Copyright (C) Antoine Mine' 2006, bertrand Jeannet' 2007-2011
 *
 */

/* This file is part of the APRON Library, released under GPL license
   (use of PPL library).
   Please read the COPYING file packaged in the distribution.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "ap_global0.h"
#include "boxMPQ.h"
#include "ml2c_wrapper.h"


ap_linexpr0_t* random_linexpr_linear(int);
ap_abstract0_t* random_abstract_std(ap_manager_t* man, int dim);
void random_abstract2_std(ap_manager_t*, ap_manager_t*, int,
			  ap_abstract0_t**, ap_abstract0_t**);
ap_texpr0_t* random_texpr0_std(int, int);
ap_texpr0_t* random_texpr0_real(int, int);

/* ********************************************************************** */
/* Global paramaters */
/* ********************************************************************** */

FILE* stream=NULL;
bool warning = true;

ap_manager_t* manprec;
ap_manager_t* manrough;
num_internal_ptr intern = NULL;
ap_scalar_discr_t global_discr = AP_SCALAR_MPQ;
ap_linexpr_type_t global_linexpr_typ = AP_LINEXPR_LINEAR;
bool global_texpr_real = true;
bool global_intdim;



ap_abstract0_t* (*random_abstract)(ap_manager_t*, int) = &random_abstract_std;
void (*random_abstract2)(ap_manager_t*, ap_manager_t*, int,
			 ap_abstract0_t**, ap_abstract0_t**) = &random_abstract2_std;
bool random_abstract2_equal = 1;
/* Indicates whether the last call to random_abstract2 guarantees that the two
   abstract values are semantically equal */

int nb_meetjoin_array = 4;
unsigned int nb_assign_array = 4;
unsigned int nb_substitute_array = 4;

#define N 10

char b1_[N+4]= " [";
char b2_[N+4];
int i_;
int error_ = 0;

#define LOOP								\
  {									\
    memset(b1_+2,' ',N); b1_[N+2]=']'; b1_[N+3]=0;			\
    memset(b2_,8,N+3); b2_[N+3]=0;					\
    for (i_=0;i_<N;i_++) {						\
      printf("%s%s",b1_,b2_);						\
      fflush(stdout);

#define RESULT(c) b1_[i_+2]=c

#define ERROR(msg)					\
  do {							\
    fprintf(stream,"\n%s\n",msg);			\
    RESULT('!');					\
    error_++;						\
  } while (0)

#define ENDLOOP	} } printf("%s%s\n",b1_,b2_); // if (error_) abort();

/* ********************************************************************** */
/* Random generations of expressions, constraints, rays */
/* ********************************************************************** */

void ap_coeff_set_random_lfrac(ap_coeff_t i)
{
  long n = rand()%20-10;
  long d  = rand()%4+1;
  ap_coeff_set_val(intern,i,AP_COEFF_LFRAC,n,d);
}
void ap_coeff_set_random_lfrac2(ap_coeff_t i)
{
  long n1 = rand()%20-10;
  long d1  = rand()%4+1;
  long n2 = rand()%20;
  long d2  = rand()%4+1;
  n2 = n1*d2+n2*d1;
  d2 = d1*d2;
  ap_coeff_set_val(intern,i,AP_COEFF_LFRAC2,n1,d1,n2,d2);
}

void ap_linexpr0_set_random_vertex(ap_linexpr0_t vertex, int size)
{
  ap_coeff_t coeff;
  int i;
  for (i=0;i<size;i++){
    ap_linexpr0_coeffref(coeff,vertex,i);
    ap_coeff_set_random_lfrac(coeff);
  }
  ap_linexpr0_cstref(coeff,vertex);
  ap_coeff_set_int(coeff,0);
}

/* random affine expression, linear */
void ap_linexpr0_set_random_linear(ap_linexpr0_t l, int dim)
{
  ap_coeff_t coeff;
  ap_linexpr0_set_random_vertex(l,dim);
  ap_linexpr0_cstref(coeff,l);
  ap_coeff_set_random_lfrac(coeff);
}

/* random affine expression, quasilinear */
void ap_linexpr0_set_random_quasilinear(ap_linexpr0_t l, int dim)
{
  ap_coeff_t coeff;
  ap_linexpr0_set_random_vertex(l,dim);
  ap_linexpr0_cstref(coeff,l);
  ap_coeff_set_random_lfrac2(coeff);
}

/* random affine expression, intlinear */
void ap_linexpr0_set_random_intlinear(ap_linexpr0_t l, int dim)
{
  ap_coeff_t coeff;
  int i;
  for (i=0;i<dim;i++){
    ap_linexpr0_coeffref(coeff,l,i);
    ap_coeff_set_random_lfrac2(coeff);
  }
  ap_linexpr0_cstref(coeff,l);
  ap_coeff_set_random_lfrac2(coeff);
}
/* random affine expression, generic */
void ap_linexpr0_set_random(ap_linexpr0_t l, int dim)
{
  switch(global_linexpr_typ){
  case AP_LINEXPR_LINEAR:
    ap_linexpr0_set_random_linear(l,dim);
    break;
  case AP_LINEXPR_QUASILINEAR:
    ap_linexpr0_set_random_quasilinear(l,dim);
    break;
  case AP_LINEXPR_INTLINEAR:
    ap_linexpr0_set_random_intlinear(l,dim);
    break;
  }
}
/* random generator of specified type */
void ap_lingen0_set_random(ap_lingen0_t res, int dim, ap_gentyp_t g)
{
  ap_linexpr0_t linexpr;
  ap_lingen0_linexpr0ref(linexpr,res);
  ap_linexpr0_set_random_linear(linexpr,dim);
  if (g>=100){
    g = (rand()%100>=80)?AP_GEN_LINE:AP_GEN_RAY;
  }
  ap_lingen0_set_gentyp(res,g);
}

/* random constraint of specified type */
void ap_lincons0_set_random(ap_lincons0_t res, int dim, ap_constyp_t c)
{
  ap_linexpr0_t linexpr;
  ap_lincons0_linexpr0ref(linexpr,res);
  ap_linexpr0_set_random(linexpr,dim);
  if (c>=100){
    c =
      (rand()%100>=90) ? AP_CONS_EQ :
      (rand()%100>=90) ? AP_CONS_SUP :
      AP_CONS_SUPEQ;
  }
  ap_lincons0_set_constyp(res,c);
  if (c==AP_CONS_EQMOD){
    mpq_ptr mpq = ap_lincons0_mpqref(res);
    mpq_set_si(mpq,rand()%20-10,rand()%4+1);
  }
}

/* random affine expression, linear, chosen from an interval linear one */
void ap_coeff_pick_value(ap_coeff_t res, ap_coeff_t a)
{
  assert(res->discr==a->discr);
  switch(a->discr){
  case AP_SCALAR_D:{
    eitvD_ptr ires = res->eitv.D;
    eitvD_ptr ia = a->eitv.D;
    if (eitvD_is_point(ia)){
      eitvD_set(ires,ia);
    }
    else {
      if (boundD_infty(ia->itv->sup)){
	if (boundD_infty(ia->itv->neginf)){
	  ap_coeff_set_random_lfrac(res);
	} else {
	  eitvD_set_num(ires,ia->itv->neginf);
	  eitvD_neg(ires,ires);
	}
      } else {
	if (boundD_infty(ia->itv->neginf)){
	  eitvD_set_num(ires,ia->itv->sup);
	} else {
	  if (rand()%2){
	    eitvD_set_num(ires,ia->itv->neginf);
	    eitvD_neg(ires,ires);
	  } else {
	    eitvD_set_num(ires,ia->itv->sup);
	  }
	}
      }
    }
  }
  break;
  case AP_SCALAR_MPQ:{
    eitvMPQ_ptr ires = res->eitv.MPQ;
    eitvMPQ_ptr ia = a->eitv.MPQ;
    if (eitvMPQ_is_point(ia)){
      eitvMPQ_set(ires,ia);
    }
    else {
      if (boundMPQ_infty(ia->itv->sup)){
	if (boundMPQ_infty(ia->itv->neginf)){
	  ap_coeff_set_random_lfrac(res);
	} else {
	  eitvMPQ_set_num(ires,ia->itv->neginf);
	  eitvMPQ_neg(ires,ires);
	}
      } else {
	if (boundMPQ_infty(ia->itv->neginf)){
	  eitvMPQ_set_num(ires,ia->itv->sup);
	} else {
	  if (rand()%2){
	    eitvMPQ_set_num(ires,ia->itv->neginf);
	    eitvMPQ_neg(ires,ires);
	  } else {
	    eitvMPQ_set_num(ires,ia->itv->sup);
	  }
	}
      }
    }
  }
  break;
  case AP_SCALAR_MPFR:{
    eitvMPFR_ptr ires = res->eitv.MPFR;
    eitvMPFR_ptr ia = a->eitv.MPFR;
    if (eitvMPFR_is_point(ia)){
      eitvMPFR_set(ires,ia);
    }
    else {
      if (boundMPFR_infty(ia->itv->sup)){
	if (boundMPFR_infty(ia->itv->neginf)){
	  ap_coeff_set_random_lfrac(res);
	} else {
	  eitvMPFR_set_num(ires,ia->itv->neginf);
	  eitvMPFR_neg(ires,ires);
	}
      } else {
	if (boundMPFR_infty(ia->itv->neginf)){
	  eitvMPFR_set_num(ires,ia->itv->sup);
	} else {
	  if (rand()%2){
	    eitvMPFR_set_num(ires,ia->itv->neginf);
	    eitvMPFR_neg(ires,ires);
	  } else {
	    eitvMPFR_set_num(ires,ia->itv->sup);
	  }
	}
      }
    }
  }
  break;
  default:
    abort();
  }
}

void ap_linexpr0_pick_linear(ap_linexpr0_t l, ap_linexpr0_t a, int dim)
{
  assert(l->discr==a->discr);
  ap_coeff_t coeffa,coeffl;
  int i;

  for (i=0;i<dim;i++) {
    ap_linexpr0_coeffref(coeffa,a,i);
    ap_linexpr0_coeffref(coeffl,l,i);
    ap_coeff_pick_value(coeffl,coeffa);
  }
  ap_linexpr0_cstref(coeffa,a);
  ap_linexpr0_cstref(coeffl,l);
  ap_coeff_pick_value(coeffl,coeffa);
}

void ap_lincons0_pick_linear(ap_lincons0_t l, ap_lincons0_t a, int dim)
{
  assert(l->discr==a->discr);
  ap_linexpr0_t exprl, expra;
  ap_lincons0_linexpr0ref(expra,a);
  ap_lincons0_linexpr0ref(exprl,l);
  ap_linexpr0_pick_linear(exprl,expra,dim);
}

/* random tree expression */
ap_texpr0_t* random_texpr0_std(int dim, int depth)
{
  ap_coeff_t coeff;
  ap_texpr0_t* res;
  if (depth==0) {
    switch (rand()%3) {
    case 0: return ap_texpr0_dim(rand()%dim);
    case 1:
      ap_coeff_init(coeff,global_discr);
      ap_coeff_set_random_lfrac(coeff);
      res = ap_texpr0_cst(coeff);
      ap_coeff_clear(coeff);
      return res;
    default:
      ap_coeff_init(coeff,global_discr);
      ap_coeff_set_random_lfrac2(coeff);
      res = ap_texpr0_cst(coeff);
      ap_coeff_clear(coeff);
      return res;
    }
  }
  else {
    if (rand()%2)
      return ap_texpr0_unop(AP_TEXPR_NEG+rand()%(AP_TEXPR_SQRT-AP_TEXPR_NEG+1),
			    random_texpr0_std(dim,rand()%depth),
			    rand()%AP_RTYPE_SIZE, rand()%AP_RDIR_SIZE);
    else
      return ap_texpr0_binop(rand()%AP_TEXPR_NEG,
			     random_texpr0_std(dim,rand()%depth),
			     random_texpr0_std(dim,rand()%depth),
			     rand()%AP_RTYPE_SIZE, rand()%AP_RDIR_SIZE);
  }
}
ap_texpr0_t* random_texpr0_real(int dim, int depth)
{
  ap_coeff_t coeff;
  ap_texpr0_t* res;
  if (depth==0) {
    switch (rand()%3) {
    case 0: return ap_texpr0_dim(rand()%dim);
    case 1:
      ap_coeff_init(coeff,global_discr);
      ap_coeff_set_random_lfrac(coeff);
      res = ap_texpr0_cst(coeff);
      ap_coeff_clear(coeff);
      return res;
    default:
      ap_coeff_init(coeff,global_discr);
      ap_coeff_set_random_lfrac2(coeff);
      res = ap_texpr0_cst(coeff);
      ap_coeff_clear(coeff);
      return res;
    }
  }
  else {
    if (rand()%2){
      return ap_texpr0_unop(AP_TEXPR_NEG+rand()%(AP_TEXPR_SQRT-AP_TEXPR_NEG+1),
			    random_texpr0_real(dim,rand()%depth),
			    AP_RTYPE_REAL, rand()%AP_RDIR_SIZE);
    } else {
      return ap_texpr0_binop(rand()%AP_TEXPR_NEG,
			     random_texpr0_real(dim,rand()%depth),
			     random_texpr0_real(dim,rand()%depth),
			     AP_RTYPE_REAL, rand()%AP_RDIR_SIZE);
    }
  }
}
ap_texpr0_t* random_texpr0(int dim, int depth)
{
  if (global_texpr_real)
    return random_texpr0_real(dim,depth);
  else
    return random_texpr0_std(dim,depth);
}

ap_tcons0_t* random_tcons0(int dim, int depth, ap_constyp_t c)
{
  if (c>=100){
    c =
      (rand()%100>=90) ? AP_CONS_EQ :
      (rand()%100>=90) ? AP_CONS_SUP :
      AP_CONS_SUPEQ;
  }
  return ap_tcons0_make(random_texpr0(dim,depth),c,NULL);
}


#define MARK_expr
void ap_linexpr0_array_set_random(ap_linexpr0_array_t array, int dim, int nb)
{
  ap_linexpr0_t elt;
  int i;
  ap_linexpr0_array_resize(array,nb);
  for (i=0;i<nb;i++){
    ap_linexpr0_array_ref_index(elt,array,i);
#if defined(MARK_expr)
    ap_linexpr0_set_random(elt,dim);
#else
    ap_linexpr0_set_random(elt,dim,100);
#endif
  }
}
#undef MARK_expr
#define MARK_cons
void ap_lincons0_array_set_random(ap_lincons0_array_t array, int dim, int nb)
{
  ap_lincons0_t elt;
  int i;
  ap_lincons0_array_resize(array,nb);
  for (i=0;i<nb;i++){
    ap_lincons0_array_ref_index(elt,array,i);
#if defined(MARK_expr)
    ap_lincons0_set_random(elt,dim);
#else
    ap_lincons0_set_random(elt,dim,100);
#endif
  }
}
#undef MARK_cons
#define MARK_gen
void ap_lingen0_array_set_random(ap_lingen0_array_t array, int dim, int nb)
{
  ap_lingen0_t elt;
  int i;
  ap_lingen0_array_resize(array,nb);
  for (i=0;i<nb;i++){
    ap_lingen0_array_ref_index(elt,array,i);
#if defined(MARK_expr)
    ap_lingen0_set_random(elt,dim);
#else
    ap_lingen0_set_random(elt,dim,100);
#endif
  }
}
#undef MARK_gen



#define MARK_expr
ap_texpr0_array_t random_texpr0_array(int dim, int depth, int nb)
{
  ap_texpr0_array_t array = ap_texpr0_array_make(nb);
  int i;
  for (i=0;i<nb;i++){
    array.p[i] =
#if defined(MARK_expr)
      random_texpr0(dim,depth)
#else
      random_tcons0(dim,depth,100)
#endif
      ;
  }
  return array;
}
#undef MARK_expr
#define MARK_cons
ap_tcons0_array_t random_tcons0_array(int dim, int depth, int nb)
{
  ap_tcons0_array_t array = ap_tcons0_array_make(nb);
  int i;
  for (i=0;i<nb;i++){
    array.p[i] =
#if defined(MARK_expr)
      random_texpr0(dim,depth)
#else
      random_tcons0(dim,depth,100)
#endif
      ;
  }
  return array;
}
#undef MARK_cons



/* ********************************************************************** */
/* Random generation of abstract values */
/* ********************************************************************** */

/* random abstract value, created with the vdim vertices, and rdim
   rays. */
ap_abstract0_t* random_abstract_std(ap_manager_t* man, int dim)
{
  int i;
  ap_abstract0_t* p;
  ap_coeff_t coeff;
  ap_linexpr0_t vertex;
  ap_dimension_t dimension;
  ap_lingen0_array_t array;
  ap_lingen0_t gen;

  int vdim,rdim;

  vdim = (dim * 90) / 100;
  rdim = dim-vdim;

  if (global_intdim){
    dimension.intd = dim/2;
    dimension.reald = dim-dim/2;
  }
  else {
    dimension.intd = 0;
    dimension.reald = dim;
  }
  ap_linexpr0_init(vertex,global_discr,dim);
  if (vdim==0){
    /* creating an abstract value with the origin */
    for (i=0;i<dim;i++){
      ap_linexpr0_coeffref(coeff,vertex,i);
      ap_coeff_set_int(coeff,0);
    }
    p = ap_abstract0_of_box(man,dimension,vertex);
  }
  else {
    /* creating an abstract value with a random vertex */
    ap_linexpr0_set_random_vertex(vertex,dim);
    p = ap_abstract0_of_box(man,dimension,vertex);
    /* adding other vertices */
    for (i=1; i<vdim; i++){
      ap_abstract0_t* tmp;
      ap_linexpr0_set_random_vertex(vertex,dim);
      tmp = ap_abstract0_of_box(man,dimension,vertex);
      p = ap_abstract0_join(man,true,p,tmp);
      ap_abstract0_free(tmp->man,tmp);
    }
  }

  /* adding rays */
  ap_lingen0_array_init(array,global_discr,rdim);
  for (i=0;i<rdim;i++){
    ap_lingen0_array_ref_index(gen,array,i);
    ap_lingen0_set_random(gen,dim,AP_GEN_RAY);
  }
  p = ap_abstract0_add_ray_array(man,true,p,array);

  /* clearing */
  ap_linexpr0_clear(vertex);
  ap_lingen0_array_clear(array);

  // fprintf(stdout,"abs0:\n"); ap_abstract0_fprint(stdout,man,p,0);

  return p;
}

/* random abstract value, created with dim/RATIOEQ equalities */
#define RATIOEQ 3

ap_abstract0_t* random_abstract_eq(ap_manager_t* man, int dim)
{
  int i;
  ap_abstract0_t* p;
  ap_lincons0_array_t array;
  ap_lincons0_t cons;
  ap_dimension_t dimension;
  if (global_intdim){
    dimension.intd = dim/2;
    dimension.reald = dim-dim/2;
  }
  else {
    dimension.intd = 0;
    dimension.reald = dim;
  }
  /* creating a top abstract value */
  p = ap_abstract0_top(man,dimension);

  ap_lincons0_array_init(array,global_discr,dim/RATIOEQ);
  for (i=0;i<dim/RATIOEQ;i++){
    ap_lincons0_array_ref_index(cons,array,i);
    ap_lincons0_set_random(cons,dim,AP_CONS_EQ);
  }
  p = ap_abstract0_meet_lincons_array(man,true,p,array);
  ap_lincons0_array_clear(array);
  // fprintf(stdout,"abs0:\n"); ap_abstract0_fprint(stdout,man,p,0);

  return p;
}

/* random abstract value, created with dim/RATIOEQ equalities modulo */
#define RATIOEQMOD 2
ap_abstract0_t* random_abstract_eqmod(ap_manager_t* man, int dim)
{
  int i;
  ap_abstract0_t* p;
  ap_lincons0_array_t array;

  ap_lincons0_t cons;
  ap_dimension_t dimension;
  if (global_intdim){
    dimension.intd = dim/2;
    dimension.reald = dim-dim/2;
  }
  else {
    dimension.intd = 0;
    dimension.reald = dim;
  }
  /* creating a top abstract value */
  p = ap_abstract0_top(man,dimension);

  ap_lincons0_array_init(array,global_discr,dim/RATIOEQMOD);
  for (i=0;i<dim/RATIOEQMOD;i++){
    ap_lincons0_array_ref_index(cons,array,i);
    ap_constyp_t constyp = (rand() % RATIOEQMOD == 0) ? AP_CONS_EQMOD : AP_CONS_EQ;
    ap_lincons0_set_random(cons,dim,constyp);
  }
  p = ap_abstract0_meet_lincons_array(man,true,p,array);
  ap_lincons0_array_clear(array);

  // fprintf(stdout,"abs0:\n"); ap_abstract0_fprint(stdout,man,p,0);

  return p;
}

/* convert to specified manager */
size_t convert_count = 0;
ap_abstract0_t* convert(ap_manager_t* man, ap_abstract0_t* a)
{
  convert_count++;
  ap_abstract0_t* res;
  ap_lincons0_array_t array;
  ap_dimension_t d = ap_abstract0_dimension(a->man,a);
  if (a->man==man) return a;
  ap_lincons0_array_init(array,global_discr,0);
  ap_abstract0_to_lincons_array(a->man,array,a);
  res = ap_abstract0_of_lincons_array(man,d,array);
  ap_lincons0_array_clear(array);
  return res;
}

/* generate a pair of related abstract values */
void random_abstract2_std(ap_manager_t* man1,/* assumed to be the most precise */
			  ap_manager_t* man2,/* assumed to be the less precise */
			  int dim,
			  ap_abstract0_t** abs1, ap_abstract0_t** abs2)
{
  *abs2 = random_abstract(man2,dim);
  *abs1 = convert(man1,*abs2);
  random_abstract2_equal = true;
}
void random_abstract2_inv(ap_manager_t* man1,/* assumed to be the most precise */
			  ap_manager_t* man2,/* assumed to be the less precise */
			  int dim,
			  ap_abstract0_t** abs1, ap_abstract0_t** abs2)
{
  *abs1 = random_abstract(man1,dim);
  *abs2 = convert(man2,*abs1);
  random_abstract2_equal = false;
}

/* print */
void print_abstract(const char* msg, ap_abstract0_t* p)
{
  fprintf(stream,"%s (%s) = ",msg,p->man->library);
  ap_abstract0_fprint(stream,p->man,p,NULL);
  /*ap_abstract0_fdump(stream,p->man,p);*/
  fprintf(stream,"\n");
}

/* ********************************************************************** */
/* Comparison of abstract values */
/* ********************************************************************** */

   /*
The first abstract value is supposed to be defined on a more precise abstract
domain than the second one. Hence we convert a2 to the first abstract domain
and then performs the comparison.
*/
bool is_eq(ap_abstract0_t* a1, ap_abstract0_t* a2)
{
  ap_abstract0_t* aa2 = convert(a1->man,a2);
  bool r = ap_abstract0_is_eq(a1->man,a1,aa2);
  if (a2!=aa2) ap_abstract0_free(aa2->man,aa2);
  return r;
}
bool is_leq(ap_abstract0_t* a1, ap_abstract0_t* a2)
{
  ap_abstract0_t* aa2 = convert(a1->man,a2);
  bool r = ap_abstract0_is_leq(a1->man,a1,aa2);
  if (a2!=aa2) ap_abstract0_free(aa2->man,aa2);
  return r;
}


/* ********************************************************************** */
/* Tests */
/* ********************************************************************** */

/* ====================================================================== */
/* Various tests on special cases */
/* ====================================================================== */

/* test on prec domain */
void test_misc(void)
{
  size_t D = 5;
  int i;
  ap_abstract0_t *bot,*top;

  ap_dimension_t dimension = { 0, D };

  bot = ap_abstract0_bottom(manprec,dimension);
  top = ap_abstract0_top(manprec,dimension);
  ap_dimension_t d1 = ap_abstract0_dimension(manprec,bot);
  ap_dimension_t d2 = ap_abstract0_dimension(manprec,top);
  printf("\nperforming various tests\n");
  if (d1.intd || d1.reald!=D) printf("ap_abstract0_dimension failed #1\n");
  if (d2.intd || d2.reald!=D) printf("ap_abstract0_dimension failed #2\n");
  if (ap_abstract0_is_bottom(manprec,bot)==false)  printf("ap_abstract0_is_bottom failed #3\n");
  if (ap_abstract0_is_bottom(manprec,top))   printf("ap_abstract0_is_bottom failed #4\n");
  if (ap_abstract0_is_top(manprec,bot))      printf("ap_abstract0_is_top failed #5\n");
  if (ap_abstract0_is_top(manprec,top)==false)     printf("ap_abstract0_is_top failed #6\n");
  if (ap_abstract0_is_leq(manprec,bot,top)==false) printf("ap_abstract0_is_leq failed #7\n");
  if (ap_abstract0_is_leq(manprec,top,bot))  printf("ap_abstract0_is_leq failed #8\n");
  if (ap_abstract0_is_eq(manprec,bot,bot)==false)  printf("ap_abstract0_is_eq failed #9\n");
  if (ap_abstract0_is_eq(manprec,top,top)==false)  printf("ap_abstract0_is_eq failed #10\n");
  if (ap_abstract0_is_eq(manprec,bot,top))   printf("ap_abstract0_is_eq failed #11\n");
  if (ap_abstract0_is_dimension_unconstrained(manprec,bot,0))
    printf("ap_abstract0_is_dimension_unconstrained #12\n");
  if (ap_abstract0_is_dimension_unconstrained(manprec,top,0)==false)
    printf("ap_abstract0_is_dimension_unconstrained #13\n");
  for (i=0;i<N;i++) {
    ap_abstract0_t* o = random_abstract(manprec,D);
    ap_abstract0_t* c = ap_abstract0_copy(manprec,o);
    ap_abstract0_t* l = ap_abstract0_closure(manprec,false,o);
    ap_dimension_t d = ap_abstract0_dimension(manprec,o);
    if (d.intd || d.reald!=D) printf("ap_abstract0_dimension failed #14\n");
    if (ap_abstract0_is_leq(manprec,bot,o)==false)  printf("ap_abstract0_is_leq failed #15\n");
    if (ap_abstract0_is_leq(manprec,o,top)==false)  printf("ap_abstract0_is_leq failed #16\n");
    if (ap_abstract0_is_eq(manprec,o,c)==false)     printf("ap_abstract0_is_eq failed #17\n");
    if (ap_abstract0_is_eq(manprec,o,l)==false)     printf("ap_abstract0_is_eq failed #18\n");
    // not implemented
    //ap_abstract0_minimize(manprec,o);
    //ap_abstract0_canonicalize(manprec,o);
    //ap_abstract0_approximate(manprec,o,0);
    //ap_abstract0_is_minimal(manprec,o);
    //ap_abstract0_is_canonical(manprec,o);
    ap_abstract0_free(manprec,o); ap_abstract0_free(manprec,c); ap_abstract0_free(manprec,l);
  }
  ap_abstract0_free(manprec,bot); ap_abstract0_free(manprec,top);
}

/* ====================================================================== */
/* Comparison tests */
/* ====================================================================== */

void test_conv(void)
{
  int dim = 5;
  printf("\nconversions\n");
  LOOP {
    ap_abstract0_t* rough0 = random_abstract(manrough,dim);
    ap_abstract0_t* prec0 = convert(manprec,rough0);
    ap_abstract0_t* rough1 = convert(manrough,prec0);
    ap_abstract0_t* prec1 = convert(manprec,rough1);
    RESULT('*');
    bool t1 = !is_eq(rough0,rough1);
    bool t2 = !is_eq(prec0,prec1);
    bool t3 = !is_leq(prec0,rough0);

    if (t1 || t2 || t3){
      char msg[256];
      int n = 0;
      if (t1) n+=snprintf(msg+n,256-n,"rough0!=rough1 (error) ");
      if (t2) n+=snprintf(msg+n,256-n,"prec0!=prec1 (error) ");
      if (t3) n+=snprintf(msg+n,256-n,"prec0 not in rough0 (error) ");
      ERROR(msg);
      print_abstract("rough0",rough0); print_abstract("prec0",prec0);
      print_abstract("rough1",rough1); print_abstract("prec1",prec1);
    }
    ap_abstract0_free(manrough,rough0); ap_abstract0_free(manrough,rough1);
    ap_abstract0_free(manprec,prec0); ap_abstract0_free(manprec,prec1);
  } ENDLOOP;
}

void test_bound_dimension(void)
{
  printf("\nbound_dimension\n");
  LOOP {
    int cmp;
    size_t i, dim = 6;
    ap_abstract0_t* rougha,*preca;
    ap_coeff_t roughi,preci;

    ap_coeff_init(roughi,global_discr);
    ap_coeff_init(preci,global_discr);
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    RESULT('*');
    for (i=0;i<dim;i++) {
      bool roughd,precd;
      roughd = ap_abstract0_is_dimension_unconstrained(manrough,rougha,i);
      precd = ap_abstract0_is_dimension_unconstrained(manprec,preca,i);

      ap_abstract0_bound_dimension(manrough,roughi,rougha,i);
      ap_abstract0_bound_dimension(manprec,preci,preca,i);
      cmp = ap_coeff_cmp(preci,roughi);
      if (roughd!=precd ||
	  (random_abstract2_equal && cmp!=0) ||
	  (cmp!=0 && cmp!=(-1))){
	ERROR("different results");
	print_abstract("rougha",rougha);
	print_abstract("preca",preca);
	fprintf(stream,"roughi[%i]=",(int)i); ap_coeff_fprint(stream,roughi);
	fprintf(stream," preci[%i]=",(int)i); ap_coeff_fprint(stream,preci);
	fprintf(stream," roughd=%i precd=%i\n",roughd,precd);
      }
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_coeff_clear(roughi);
    ap_coeff_clear(preci);

  } ENDLOOP;
}

size_t test_bound_linexpr_count = 0;
void test_bound_linexpr(void)
{
  test_bound_linexpr_count++;
  printf("\nbound_linexpr\n");
  LOOP {
    int cmp;
    size_t dim = 6;
    ap_abstract0_t *rougha,*preca;
    ap_coeff_t roughi,preci;
    ap_linexpr0_t l;

    ap_coeff_init(roughi,global_discr); ap_coeff_init(preci,global_discr);
    ap_linexpr0_init(l,global_discr,dim);
    ap_linexpr0_set_random_intlinear(l,dim);
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    ap_abstract0_bound_linexpr(manrough,roughi,rougha,l);
    ap_abstract0_bound_linexpr(manprec,preci,preca,l);
    cmp = ap_coeff_cmp(preci,roughi);
    RESULT('*');
    bool t1 = (random_abstract2_equal && cmp!=0);
    bool t2 = (cmp!=0 && cmp!=(-1));
    if (t1 || t2){
      char msg[256];
      int n=0;
      if (t1) n+=snprintf(msg+n,256-n,"rougha==preca but different results (sound) ");
      if (t2) n+=snprintf(msg+n,256-n,"no inclusion (likely to be an error)");
      ERROR(msg);
      if (t2 || (t1 && warning)){
	print_abstract("rougha",rougha); print_abstract("preca",preca);
	ap_linexpr0_fprint(stream,l,NULL);
	fprintf(stream,"\nroughi="); ap_coeff_fprint(stream,roughi);
	fprintf(stream,"\npreci="); ap_coeff_fprint(stream,preci);
	fprintf(stream,"\n");
      }
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_coeff_clear(roughi); ap_coeff_clear(preci);
    ap_linexpr0_clear(l);
  } ENDLOOP;
}

void test_bound_texpr(void)
{
  printf("\nbound_texpr\n");
  LOOP {
    int cmp;
    size_t dim = 6, depth=3;
    ap_abstract0_t* rougha,*preca;
    ap_coeff_t roughi,preci;

    ap_coeff_init(roughi,global_discr); ap_coeff_init(preci,global_discr);
    ap_texpr0_t* e = random_texpr0(dim,depth);
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    ap_abstract0_bound_texpr(manrough,roughi,rougha,e);
    ap_abstract0_bound_texpr(manprec,preci,preca,e);
    cmp = ap_coeff_cmp(preci,roughi);
    RESULT('*');
    bool t1 = (random_abstract2_equal && cmp!=0);
    bool t2 = (cmp!=0 && cmp!=(-1));
    if (t1 || t2){
      char msg[256];
      int n=0;
      if (t1) n+=snprintf(msg+n,256-n,"rougha==preca but different results (sound) ");
      if (t2) n+=snprintf(msg+n,256-n,"no inclusion (likely to be an error)");
      ERROR(msg);
      if (t2 || (t1 && warning)){
	print_abstract("rougha",rougha); print_abstract("preca",preca);
	ap_texpr0_fprint(stream,e,NULL);
	fprintf(stream,"\nroughi="); ap_coeff_fprint(stream,roughi);
	fprintf(stream,"\npreci="); ap_coeff_fprint(stream,preci);
	fprintf(stream,"\n");
      }
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_coeff_clear(roughi); ap_coeff_clear(preci);
    ap_texpr0_free(e);
  } ENDLOOP;
}

void test_sat_interval(void)
{
  printf("\nsat_interval\n");
  LOOP {
    size_t dim = 6;
    size_t p = rand() % dim;
    ap_abstract0_t* rougha,*preca;
    ap_coeff_t i;
    bool roughs,precs;

    ap_coeff_init(i,global_discr);
    ap_coeff_set_random_lfrac2(i);
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    roughs = ap_abstract0_sat_interval(manrough,rougha,p,i);
    precs = ap_abstract0_sat_interval(manprec,preca,p,i);
    RESULT('*');
    bool t1 = (random_abstract2_equal &&
	       ((roughs && roughs!=precs) ||
		(precs && roughs!=precs)));
    bool t2 = roughs && !precs;
    if (t1 || t2){
      char msg[256];
      int n=0;
      if (t1) n+=snprintf(msg+n,256-n,"rougha==preca but different results (sound) ");
      if (t2) n+=snprintf(msg+n,256-n,"no implication (likely to be an error)");
      ERROR(msg);
      if (t2 || (t1 && warning)){
	print_abstract("rougha",rougha);
	print_abstract("preca",preca);
	ap_coeff_fprint(stream,i);
	fprintf(stream,"\nvar=%i\nroughs=%i precs=%i\n",(int)p,roughs,precs);
      }
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_coeff_clear(i);
  } ENDLOOP;
}

void test_sat_lincons(void)
{
  printf("\nsat_lincons\n");
  LOOP {
    size_t dim = 6;
    ap_abstract0_t* rougha,*preca;
    ap_lincons0_t l;

    ap_lincons0_init(l,global_discr,dim);
    ap_lincons0_set_random(l,dim,100);
    bool roughs,precs;
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    roughs = ap_abstract0_sat_lincons(manrough,rougha,l);
    precs = ap_abstract0_sat_lincons(manprec,preca,l);
    RESULT('*');
    bool t1 = (random_abstract2_equal &&
	       ((roughs && roughs!=precs) ||
		(precs && roughs!=precs)));
    bool t2 = roughs && !precs;
    if (t1 || t2){
      char msg[256];
      int n=0;
      if (t1) n+=snprintf(msg+n,256-n,"rougha==preca but different results (sound) ");
      if (t2) n+=snprintf(msg+n,256-n,"no implication (likely to be an error)");
      ERROR(msg);
      if (t2 || (t1 && warning)){
	print_abstract("rougha",rougha);print_abstract("preca",preca);
	ap_lincons0_fprint(stream,l,NULL);
	fprintf(stream,"\nroughs=%i precs=%i\n",roughs,precs);
      }
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_lincons0_clear(l);
  } ENDLOOP;
}

void test_sat_tcons(void)
{
  printf("\nsat_tcons\n");
  LOOP {
    size_t dim = 6, depth=3;
    ap_abstract0_t* rougha,*preca;
    ap_tcons0_t* l = random_tcons0(dim,depth,100);

    if (0){
      printf("\n");
      ap_tcons0_print(l,NULL);
      printf("\n");
    }
    bool roughs,precs;
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    roughs = ap_abstract0_sat_tcons(manrough,rougha,l);
    precs = ap_abstract0_sat_tcons(manprec,preca,l);
    RESULT('*');
    bool t1 = (random_abstract2_equal &&
	       ((roughs && roughs!=precs) ||
		(precs && roughs!=precs)));
    bool t2 = roughs && !precs;
    if (t1 || t2){
      char msg[256];
      int n=0;
      if (t1) n+=snprintf(msg+n,256-n,"rougha==preca but different results (sound) ");
      if (t2) n+=snprintf(msg+n,256-n,"no implication (likely to be an error)");
      ERROR(msg);
      if (t2 || (t1 && warning)){
	print_abstract("rougha",rougha);print_abstract("preca",preca);
	ap_tcons0_fprint(stream,l,NULL);
	fprintf(stream,"\nroughs=%i precs=%i\n",roughs,precs);
      }
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_tcons0_free(l);
  } ENDLOOP;
}

void test_to_box(void)
{
  printf("\nbox conversion\n");
  LOOP {
    int i, dim = 6;
    ap_dimension_t dimension = { 0, dim };
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_linexpr0_t roughi,preci;
    ap_linexpr0_init(roughi,global_discr,dim);
    ap_linexpr0_init(preci,global_discr,dim);
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    ap_abstract0_to_box(manrough,roughi,rougha);
    ap_abstract0_to_box(manprec,preci,preca);
    roughr = ap_abstract0_of_box(manrough,dimension,roughi);
    precr = ap_abstract0_of_box(manprec,dimension,preci);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      fprintf(stream,"roughi="); ap_linexpr0_fprint(stream,roughi,NULL); fprintf(stream,"\n");
      fprintf(stream,"preci="); ap_linexpr0_fprint(stream,preci,NULL); fprintf(stream,"\n");
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_linexpr0_clear(roughi); ap_linexpr0_clear(preci);
  } ENDLOOP;

}


void test_meet(void)
{
  int dim = 4;
  printf("\nmeet\n");
  LOOP {
    ap_abstract0_t *prec0,*prec1,*precr,*rough0,*rough1,*roughr;
    random_abstract2(manprec,manrough,dim,&prec0,&rough0);
    random_abstract2(manprec,manrough,dim,&prec1,&rough1);
    roughr = ap_abstract0_meet(manrough,false,rough0,rough1);
    precr = ap_abstract0_meet(manprec,false,prec0,prec1);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      print_abstract("rough0",rough0); print_abstract("rough1",rough1);
      print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rough0); ap_abstract0_free(manrough,rough1); ap_abstract0_free(manrough,roughr);
    ap_abstract0_free(manprec,prec0); ap_abstract0_free(manprec,prec1); ap_abstract0_free(manprec,precr);
  } ENDLOOP;
}

void test_meet_array(void)
{
  int i,dim = 3;
  printf("\nmeet_array\n");
  LOOP {
    ap_abstract0_t* rougha[nb_meetjoin_array], *roughr;
    ap_abstract0_t* preca[nb_meetjoin_array], *precr;
    for (i=0;i<nb_meetjoin_array;i++) {
      random_abstract2(manprec,manrough,dim,&preca[i],&rougha[i]);
    }
    roughr = ap_abstract0_meet_array(manrough,(ap_abstract0_t**)rougha,nb_meetjoin_array);
    precr = ap_abstract0_meet_array(manprec,(ap_abstract0_t**)preca,nb_meetjoin_array);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    for (i=0;i<nb_meetjoin_array;i++) { ap_abstract0_free(manrough,rougha[i]); ap_abstract0_free(manprec,preca[i]); }
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
  } ENDLOOP;
}


void test_meet_lincons_array(void)
{
  printf("\nmeet_lincons_array\n");
  LOOP {
    size_t i, dim = 6, nb = 3;
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_lincons0_array_t array;
    ap_lincons0_array_init(array,global_discr,nb);
    ap_lincons0_array_set_random(array,dim,nb);
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    roughr = ap_abstract0_meet_lincons_array(manrough,false,rougha,array);
    precr = ap_abstract0_meet_lincons_array(manprec,false,preca,array);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      ap_lincons0_array_fprint(stream,array,NULL);
      print_abstract("rougha",rougha); print_abstract("preca",preca);
      print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_lincons0_array_clear(array);
  } ENDLOOP;
}

void test_meet_tcons_array(void)
{
  printf("\nmeet_tcons_array\n");
  LOOP {
    size_t i, dim = 6, depth = 4, nb = 3;
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_tcons0_array_t array = random_tcons0_array(dim,depth,nb);
    if (1){
      printf("\n");
      ap_tcons0_array_print(&array,NULL);
      printf("\n");
    }
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    roughr = ap_abstract0_meet_tcons_array(manrough,false,rougha,&array);
    precr = ap_abstract0_meet_tcons_array(manprec,false,preca,&array);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("no inclusion (likely to be an error)");
      ap_tcons0_array_fprint(stream,&array,NULL);
      print_abstract("rougha",rougha); print_abstract("preca",preca);
      print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_tcons0_array_clear(&array);
  } ENDLOOP;
}

void test_join(void)
{
  int dim = 4;
  printf("\njoin\n");
  LOOP {
    ap_abstract0_t *prec0,*prec1,*precr,*rough0,*rough1,*roughr;
    random_abstract2(manprec,manrough,dim,&prec0,&rough0);
    random_abstract2(manprec,manrough,dim,&prec1,&rough1);
    roughr = ap_abstract0_join(manrough,false,rough0,rough1);
    precr = ap_abstract0_join(manprec,false,prec0,prec1);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      print_abstract("rough0",rough0); print_abstract("rough1",rough1);
      print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rough0); ap_abstract0_free(manrough,rough1); ap_abstract0_free(manrough,roughr);
    ap_abstract0_free(manprec,prec0); ap_abstract0_free(manprec,prec1); ap_abstract0_free(manprec,precr);
  } ENDLOOP;
}

void test_join_array(void)
{
  int i,dim = 3;
  printf("\njoin_array\n");
  LOOP {
    ap_abstract0_t* rougha[nb_meetjoin_array], *roughr;
    ap_abstract0_t* preca[nb_meetjoin_array], *precr;
    for (i=0;i<nb_meetjoin_array;i++) {
      random_abstract2(manprec,manrough,dim,&preca[i],&rougha[i]);
    }
    roughr = ap_abstract0_join_array(manrough,(ap_abstract0_t**)rougha,nb_meetjoin_array);
    precr = ap_abstract0_join_array(manprec,(ap_abstract0_t**)preca,nb_meetjoin_array);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    for (i=0;i<nb_meetjoin_array;i++) { ap_abstract0_free(manrough,rougha[i]); ap_abstract0_free(manprec,preca[i]); }
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
  } ENDLOOP;
}

void test_add_ray(void)
{
  printf("\nadd_rays_array\n");
  LOOP {
    size_t i, dim = 4, nb = 4;
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_lingen0_array_t array;
    ap_lingen0_array_init(array,global_discr,nb);
    ap_lingen0_array_set_random(array,dim,nb);
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    roughr = ap_abstract0_add_ray_array(manrough,false,rougha,array);
    precr = ap_abstract0_add_ray_array(manprec,false,preca,array);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      ap_lingen0_array_fprint(stream,array,NULL);
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_lingen0_array_clear(array);
  } ENDLOOP;
}


void test_assign_linexpr(void)
{
  printf("\nassign_linexpr\n");
  LOOP {
    size_t dim = 7;
    size_t p = rand() % dim;
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_linexpr0_t l;
    ap_linexpr0_init(l,global_discr,dim);
    ap_linexpr0_set_random(l,dim);
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    roughr = ap_abstract0_assign_linexpr(manrough,false,rougha,p,l,NULL);
    precr = ap_abstract0_assign_linexpr(manprec,false,preca,p,l,NULL);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      fprintf(stream,"x%i <- ",(int)p); ap_linexpr0_fprint(stream,l,NULL); fprintf(stream,"\n");
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_linexpr0_clear(l);
  } ENDLOOP;
}
void test_assign_linexpr_array(void)
{
  printf("\nassign_linexpr_array\n");
  LOOP {
    size_t i, dim = 5;
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_dim_t d[nb_assign_array];
    ap_linexpr0_array_t array;
    ap_linexpr0_array_init(array,global_discr,nb_assign_array);
    ap_linexpr0_array_set_random(array,dim,nb_assign_array);
    for (i=0;i<nb_assign_array;i++) {
      if (!i) d[i] = rand()%(dim-nb_assign_array);
      else d[i] = d[i-1] + 1 + (rand()%(dim-nb_assign_array+i-d[i-1]));
    }
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    roughr = ap_abstract0_assign_linexpr_array(manrough,false,rougha,d,array,NULL);
    precr = ap_abstract0_assign_linexpr_array(manprec,false,preca,d,array,NULL);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      for (i=0;i<nb_assign_array;i++) {
	fprintf(stream,"x%i <- ",d[i]);
	ap_linexpr0_t elt;
	ap_linexpr0_array_ref_index(elt,array,i);
	ap_linexpr0_fprint(stream,elt,NULL);
	fprintf(stream,"\n");
      }
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("preca",preca); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_linexpr0_array_clear(array);
  } ENDLOOP;
}

void test_assign_texpr(void)
{
  printf("\nassign_texpr\n");
  LOOP {
    size_t dim = 3, depth=3;
    size_t p = rand() % dim;
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_texpr0_t* l = random_texpr0(dim, depth);
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    if (1){
      printf("\n");
      fprintf(stream,"x%i <- ",(int)p); ap_texpr0_print(l,NULL);
      printf("\n");
    }
    roughr = ap_abstract0_assign_texpr(manrough,false,rougha,p,l,NULL);
    precr = ap_abstract0_assign_texpr(manprec,false,preca,p,l,NULL);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("no inclusion (likely to be an error)");
      fprintf(stream,"x%i <- ",(int)p); ap_texpr0_fprint(stream,l,NULL); fprintf(stream,"\n");
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_texpr0_free(l);
  } ENDLOOP;
}
void test_assign_texpr_array(void)
{
  printf("\nassign_texpr_array\n");
  LOOP {
    size_t i, dim = 7, depth=3;
    size_t p = rand() % dim;
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_dim_t d[nb_assign_array];
    ap_texpr0_array_t array = random_texpr0_array(dim,depth,nb_assign_array);
    for (i=0;i<nb_assign_array;i++) {
      if (!i) d[i] = rand()%(dim-nb_assign_array);
      else d[i] = d[i-1] + 1 + (rand()%(dim-nb_assign_array+i-d[i-1]));
    }
    if (false){
      printf("\ntdim = ");
      for (i=0;i<nb_assign_array;i++) {
	printf("%d,",(int)d[i]);
      }
      printf("\n");
      ap_texpr0_array_print(&array,NULL);
      printf("\n");
    }
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    roughr = ap_abstract0_assign_texpr_array(manrough,false,rougha,d,&array,NULL);
    precr = ap_abstract0_assign_texpr_array(manprec,false,preca,d,&array,NULL);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      for (i=0;i<nb_assign_array;i++) {
	fprintf(stream,"x%i <- ",d[i]);
	ap_texpr0_fprint(stream,array.p[i],NULL);
	fprintf(stream,"\n");
      }
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("preca",preca); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_texpr0_array_clear(&array);
  } ENDLOOP;
}
void test_substitute_linexpr(void)
{
  printf("\nsubstitute_linexpr\n");
  LOOP {
    size_t dim = 7;
    size_t p = rand() % dim;
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_linexpr0_t l;
    ap_linexpr0_init(l,global_discr,dim);
    ap_linexpr0_set_random(l,dim);
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    roughr = ap_abstract0_substitute_linexpr(manrough,false,rougha,p,l,NULL);
    precr = ap_abstract0_substitute_linexpr(manprec,false,preca,p,l,NULL);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      fprintf(stream,"x%i <- ",(int)p); ap_linexpr0_fprint(stream,l,NULL); fprintf(stream,"\n");
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_linexpr0_clear(l);
  } ENDLOOP;
}
void test_substitute_linexpr_array(void)
{
  printf("\nsubstitute_linexpr_array\n");
  LOOP {
    size_t i, dim = 5;
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_dim_t d[nb_substitute_array];
    ap_linexpr0_array_t array;
    ap_linexpr0_array_init(array,global_discr,nb_substitute_array);
    ap_linexpr0_array_set_random(array,dim,nb_substitute_array);
    for (i=0;i<nb_substitute_array;i++) {
      if (!i) d[i] = rand()%(dim-nb_substitute_array);
      else d[i] = d[i-1] + 1 + (rand()%(dim-nb_substitute_array+i-d[i-1]));
    }
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    roughr = ap_abstract0_substitute_linexpr_array(manrough,false,rougha,d,array,NULL);
    precr = ap_abstract0_substitute_linexpr_array(manprec,false,preca,d,array,NULL);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      for (i=0;i<nb_substitute_array;i++) {
	fprintf(stream,"x%i <- ",d[i]);
	ap_linexpr0_t elt;
	ap_linexpr0_array_ref_index(elt,array,i);
	ap_linexpr0_fprint(stream,elt,NULL);
	fprintf(stream,"\n");
      }
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("preca",preca); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_linexpr0_array_clear(array);
  } ENDLOOP;
}

void test_substitute_texpr(void)
{
  printf("\nsubstitute_texpr\n");
  LOOP {
    size_t dim = 3, depth=3;
    size_t p = rand() % dim;
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_texpr0_t* l = random_texpr0(dim, depth);
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    if (1){
      printf("\n");
      fprintf(stream,"x%i <- ",(int)p); ap_texpr0_print(l,NULL);
      printf("\n");
    }
    roughr = ap_abstract0_substitute_texpr(manrough,false,rougha,p,l,NULL);
    precr = ap_abstract0_substitute_texpr(manprec,false,preca,p,l,NULL);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("no inclusion (likely to be an error)");
      fprintf(stream,"x%i <- ",(int)p); ap_texpr0_fprint(stream,l,NULL); fprintf(stream,"\n");
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_texpr0_free(l);
  } ENDLOOP;
}
void test_substitute_texpr_array(void)
{
  printf("\nsubstitute_texpr_array\n");
  LOOP {
    size_t i, dim = 7, depth=3;
    size_t p = rand() % dim;
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_dim_t d[nb_substitute_array];
    ap_texpr0_array_t array = random_texpr0_array(dim,depth,nb_substitute_array);
    for (i=0;i<nb_substitute_array;i++) {
      if (!i) d[i] = rand()%(dim-nb_substitute_array);
      else d[i] = d[i-1] + 1 + (rand()%(dim-nb_substitute_array+i-d[i-1]));
    }
    if (false){
      printf("\ntdim = ");
      for (i=0;i<nb_substitute_array;i++) {
	printf("%d,",(int)d[i]);
      }
      printf("\n");
      ap_texpr0_array_print(&array,NULL);
      printf("\n");
    }
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    roughr = ap_abstract0_substitute_texpr_array(manrough,false,rougha,d,&array,NULL);
    precr = ap_abstract0_substitute_texpr_array(manprec,false,preca,d,&array,NULL);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      for (i=0;i<nb_substitute_array;i++) {
	fprintf(stream,"x%i <- ",d[i]);
	ap_texpr0_fprint(stream,array.p[i],NULL);
	fprintf(stream,"\n");
      }
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("preca",preca); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_texpr0_array_clear(&array);
  } ENDLOOP;
}


void test_forget_array(void)
{
  printf("\nforget_array\n");
  LOOP {
    size_t i, dim = 8;
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_dimension_t d;
    int proj = rand()%2;
    ap_dimension_t dimension = { 0,2 };
    ap_dimchange_t a;

    ap_dimchange_init(&a,dimension);
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    d = ap_abstract0_dimension(manrough,rougha);

    for (i=0;i<dimension.intd+dimension.reald;i++) {
      a.p[i] = rand()%3 + 1;
      if (i) a.p[i] += a.p[i-1];
      if (a.p[i]<d.intd) { a.dim.intd++; a.dim.reald--; }
      assert(a.p[i]<dim);
    }
    if (0){
      printf("\n");
      ap_dimchange_fprint(stream,&a);
      print_abstract("rougha",rougha); print_abstract("preca",preca);
      printf("\n");
    }
    roughr = ap_abstract0_forget_array(manrough,false,rougha,a.p, a.dim.intd+a.dim.reald, proj);
    precr = ap_abstract0_forget_array(manprec,false,preca,a.p, a.dim.intd+a.dim.reald,proj);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("no inclusion (error)");
      ap_dimchange_fprint(stream,&a);
      print_abstract("rougha",rougha); print_abstract("roughr",roughr);
      print_abstract("precaa",preca); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_dimchange_clear(&a);
  } ENDLOOP;
}

void test_add_dimensions(void)
{
  printf("\nadd dimensions\n");
  LOOP {
    size_t i, dim = 8;
    ap_dimension_t dimension = { 0, 3 };
    ap_dimchange_t a; ap_dimchange_init(&a,dimension);
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    int proj = rand()%2;
    ap_dimension_t d;
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    d = ap_abstract0_dimension(manrough,rougha);
    for (i=0;i<a.dim.intd+a.dim.reald;i++) {
      a.p[i] = rand()%3;
      if (i) a.p[i] += a.p[i-1];
      if (a.p[i]<d.intd) { a.dim.intd++; a.dim.reald--; }
      assert(a.p[i]<dim);
    }
    roughr = ap_abstract0_add_dimensions(manrough,false,rougha,&a,proj);
    precr = ap_abstract0_add_dimensions(manprec,false,preca,&a,proj);
    RESULT('*');
    if (!is_eq(precr,roughr)) {
      ERROR("different results");
      ap_dimchange_fprint(stream,&a);
      fprintf(stream,"proj=%d\n",proj);
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("preca",preca); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_dimchange_clear(&a);
  } ENDLOOP;
}

void test_remove_dimensions(void)
{
  printf("\nremove dimensions\n");
  LOOP {
    size_t i, dim = 8;
    ap_dimension_t dimension = { 0, 2 };
    ap_dimchange_t a; ap_dimchange_init(&a,dimension);
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_dimension_t d;
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    d = ap_abstract0_dimension(manrough,rougha);
    for (i=0;i<a.dim.intd+a.dim.reald;i++) {
      a.p[i] = rand()%3 + 1;
      if (i) a.p[i] += a.p[i-1];
      if (a.p[i]<d.intd) { a.dim.intd++; a.dim.reald--; }
      assert(a.p[i]<dim);
    }
    roughr = ap_abstract0_remove_dimensions(manrough,false,rougha,&a);
    precr = ap_abstract0_remove_dimensions(manprec,false,preca,&a);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      ap_dimchange_fprint(stream,&a);
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_dimchange_clear(&a);
  } ENDLOOP;
}

void test_permute_dimensions(void)
{
  printf("\npermute dimensions\n");
  LOOP {
    size_t i, dim = 6;
    ap_dimperm_t p; ap_dimperm_init(&p,6);
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_dimension_t d;
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    d = ap_abstract0_dimension(manrough,rougha);
    /* random permutation */
    ap_dimperm_set_id(&p);
    for (i=0;i<p.size-1;i++) {
      int a, j;
      if (i<d.intd) j = i + (rand() % (d.intd-i));
      else j = i + (rand() % (d.intd+d.reald-i));
      a = p.p[j]; p.p[j] = p.p[i]; p.p[i] = a;
    }
    roughr = ap_abstract0_permute_dimensions(manrough,false,rougha,&p);
    precr = ap_abstract0_permute_dimensions(manprec,false,preca,&p);
    RESULT('*');
    if (!is_eq(precr,roughr)) {
      ERROR("different results");
      ap_dimperm_fprint(stream,&p);
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_dimperm_clear(&p);
  } ENDLOOP;
}

void test_expand(void)
{
  printf("\nexpand dimensions\n");
  LOOP {
    size_t dim = 6;
    ap_dim_t dd = rand() % dim;
    size_t n = (rand() % 2) + 1;
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_dimension_t d;
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    d = ap_abstract0_dimension(manrough,rougha);
    roughr = ap_abstract0_expand(manrough,false,rougha,dd,n);
    precr = ap_abstract0_expand(manprec,false,preca,dd,n);
    RESULT('*');
    if (!is_eq(precr,roughr)) {
      ERROR("different results");
      fprintf(stream,"dim %i expanded %i times\n",(int)dd,(int)n);
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
  } ENDLOOP;
}

void test_fold(void)
{
  printf("\nfold dimensions\n");
  LOOP {
    size_t dim = 6;
    ap_dim_t dd[3];
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_dimension_t d;
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    d = ap_abstract0_dimension(manrough,rougha);
    do {
      dd[0] = rand() % dim;
      if (dd[0]<d.intd) dd[1] = rand() % (d.intd-1);
      else dd[1] = d.intd + rand() % (d.reald-1);
    }
    while (dd[0]>=dd[1]);
    roughr = ap_abstract0_fold(manrough,false,rougha,dd,2);
    precr = ap_abstract0_fold(manprec,false,preca,dd,2);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      fprintf(stream,"fold %i,%i,%i\n",(int)dd[0],(int)dd[1],(int)dd[2]);
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
  } ENDLOOP;
}

void test_widen(void)
{
  int dim = 5;
  printf("\nwidening\n");
  LOOP {
    ap_abstract0_t* rough0, *rough1, *roughr;
    ap_abstract0_t* prec0, *prec1, *precr;
    rough0 = random_abstract(manrough,dim);
    rough1 = random_abstract(manrough,dim);
    rough1 = ap_abstract0_join(manrough,true,rough1,rough0);
    roughr = ap_abstract0_widening(manrough,rough0,rough1);
    prec0 = convert(manprec,rough0);
    prec1 = convert(manprec,rough1);
    precr = ap_abstract0_widening(manprec,prec0,prec1);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      print_abstract("rough0",rough0); print_abstract("rough1",rough1);
      print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rough0); ap_abstract0_free(manrough,rough1); ap_abstract0_free(manrough,roughr);
    ap_abstract0_free(manprec,prec0); ap_abstract0_free(manprec,prec1); ap_abstract0_free(manprec,precr);
  } ENDLOOP;
}

/* Main test function */
/* Depends on global variables manprec,manrough,intdim */
void test(ap_manager_t* man1, /* the most precise */
	  ap_manager_t* man2 /* the less precise */
	  )
{
  int i;
  int intdim;

  manprec = man1;
  manrough = man2;
  for (i=0;i<AP_EXC_SIZE;i++){
    manprec->option.abort_if_exception[i] = true;
    manrough->option.abort_if_exception[i] = true;
  }
  for (intdim=0; intdim<=0; intdim++) {
    global_intdim = (intdim>0);
    printf("\n\ncomparing libraries:\n- %s (%s)\n- %s (%s)\nwith int=%i\n\n",
	   manprec->library,manprec->version,manrough->library,manrough->version,global_intdim);
    /* run tests */
    test_conv();
    test_bound_dimension();
    test_bound_linexpr();
    test_bound_texpr();
    test_sat_interval();
    test_sat_lincons();
    test_sat_tcons();
    test_to_box();

    test_meet();
    test_meet_array();
    test_meet_lincons_array();
    test_meet_tcons_array();
    test_join();
    test_join_array();
    test_add_ray();

    test_assign_linexpr();
    test_assign_linexpr_array();
    test_substitute_linexpr();
    test_substitute_linexpr_array();

    test_assign_texpr();
    test_assign_texpr_array();
    test_substitute_texpr();
    test_substitute_texpr_array();
    test_forget_array();

    test_add_dimensions();
    test_remove_dimensions();
    test_permute_dimensions();
    test_expand();
    test_fold();
  }
  if (error_){
    printf("\n%i error(s)!\n",error_);
  }
  else printf("\nall tests passed\n");
}


/* main */
/* **** */

int main(int argc, char** argv)
{
  int i;

  stream = stdout;

  if (argc==1){
    srand(time(NULL));
  }
  else if (argc==2){
    unsigned int seed = atoi(argv[1]);
    srand(seed);
  }
  else {
    fprintf(stream,"usage: %s [<seed>]\n",argv[0]);
    exit(1);
  }
  /* allocating all managers */
  ap_manager_t* manbox = boxMPQ_manager_alloc();
  intern = manbox->num;
  ap_manager_t* manwrapper = wrapper_manager_alloc();

  /*
  ap_manager_t* manoct = oct_manager_alloc();
  ap_manager_t* manppll = ap_ppl_poly_manager_alloc(false);
  ap_manager_t* manppls = ap_ppl_poly_manager_alloc(true);
  ap_manager_t* manpplgrid = ap_ppl_grid_manager_alloc();
  ap_manager_t* manpkgrid = ap_pkgrid_manager_alloc(manpkl,manpplgrid);
  */
  /* First serie */
  global_intdim = false;
  global_linexpr_typ = AP_LINEXPR_LINEAR;
  random_abstract = &random_abstract_std;
  for (i=0; i<2; i++){
    random_abstract2 = i==0 ? &random_abstract2_std : &random_abstract2_inv;
    // box/polyhedra
    test(manwrapper,manbox);

    // Oct/Box and Oct/Poly
    random_abstract = &random_abstract_std;
    test(manwrapper,manbox);

  }


  /* clean-up */
  ap_manager_free(manbox);
  ap_manager_free(manwrapper);

  return 0;
}
