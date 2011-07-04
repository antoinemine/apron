/*
 * ctest1.c
 *
 * Unit testing. Compares domains & implementations.
 *
 * Copyright (C) Antoine Mine' 2006, bertrand Jeannet' 2007
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
#include "box.h"
#include "oct.h"
#include "pk.h"
#include "pkeq.h"
#include "ap_ppl.h"
#include "ap_pkgrid.h"

ap_linexpr0_t* random_linexpr_linear(int);
ap_abstract0_t* random_abstract_std(ap_manager_t* man, int dim);
void random_abstract2_std(ap_manager_t*, ap_manager_t*, int,
			  ap_abstract0_t**, ap_abstract0_t**);
ap_texpr0_t* random_texpr_std(int, int);
ap_texpr0_t* random_texpr_real(int, int);

/* ********************************************************************** */
/* Global paramaters */
/* ********************************************************************** */

FILE* stream=NULL;

ap_manager_t* manprec;
ap_manager_t* manrough;
int intdim;

ap_linexpr0_t* (*random_linexpr)(int) = &random_linexpr_linear;
ap_texpr0_t* (*random_texpr)(int,int) = &random_texpr_real;
ap_abstract0_t* (*random_abstract)(ap_manager_t*, int) = &random_abstract_std;
void (*random_abstract2)(ap_manager_t*, ap_manager_t*, int,
			 ap_abstract0_t**, ap_abstract0_t**) = &random_abstract2_std;
bool random_abstract2_equal = 1;
/* Indicates whether the last call to random_abstract2 guarantees that the two
   abstract values are semantically equal */

size_t nb_meetjoin_array = 4;
size_t nb_asssub_array = 4;

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

void random_vertex(ap_interval_t** tdim, int size)
{
  int i;
  for (i=0;i<size;i++){
    int n = rand()%20-10;
    int d  = rand()%4+1;
    ap_interval_set_frac(tdim[i],n,d,n,d);
  }
}

void random_interval(ap_interval_t* i)
{
  int n1 = rand()%20-10;
  int n2 = n1 + rand()%10;
  int d1  = rand()%4+1;
  int d2  = rand()%d1+1;
  ap_interval_set_frac(i,n1,d1,n2,d2);
}

/* random affine expression, linear */
ap_linexpr0_t* random_linexpr_linear(int dim)
{
  ap_linexpr0_t* l = ap_linexpr0_alloc(AP_LINEXPR_DENSE,dim);
  int i;
  for (i=0;i<dim;i++)
    ap_coeff_set_scalar_frac(l->p.coeff+i,rand()%20-10,rand()%4+1);
  ap_coeff_set_scalar_frac(&l->cst,rand()%20-10,rand()%4+1);
  return l;
}

/* random affine expression, quasilinear */
ap_linexpr0_t* random_linexpr_quasilinear(int dim)
{
  ap_linexpr0_t* l = random_linexpr_linear(dim);
  int n1 = rand()%20-10;
  int n2 = n1 + rand()%10;
  int d1  = rand()%4+1;
  int d2  = rand()%d1+1;
  ap_linexpr0_set_cst_interval_frac(l,n1,d1,n2,d2);
  return l;
}

/* random affine expression, intlinear */
ap_linexpr0_t* random_linexpr_intlinear(int dim)
{
  ap_linexpr0_t* l = ap_linexpr0_alloc(AP_LINEXPR_DENSE,dim);
  int i,n1,n2,d1,d2;
  for (i=0;i<dim;i++){
    n1 = rand()%20-10;
    n2 = n1 + rand()%10;
    d1 = rand()%4+1;
    d2 = rand()%d1+1;
    ap_coeff_set_interval_frac(l->p.coeff+i,n1,d1,n2,d2);
  }
  n1 = rand()%20-10;
  n2 = n1 + rand()%10;
  d1 = rand()%4+1;
  d2 = rand()%d1+1;
  ap_coeff_set_interval_frac(&l->cst,n1,d1,n2,d2);
  return l;
}
/* random affine expression, linear, chosen from an interval linear one */
ap_linexpr0_t* random_linexpr_linear_from_intlinear(ap_linexpr0_t* a)
{
  size_t i;
  ap_linexpr0_t* l = ap_linexpr0_alloc(AP_LINEXPR_DENSE,a->size);
  for (i=0;i<a->size;i++) {
    switch (a->p.coeff[i].discr) {
    case AP_COEFF_SCALAR:
      ap_coeff_set_scalar(&l->p.coeff[i],a->p.coeff[i].val.scalar);
      break;
    case AP_COEFF_INTERVAL:
      if (rand()%2)
	ap_coeff_set_scalar(&l->p.coeff[i],a->p.coeff[i].val.interval->inf);
      else
	ap_coeff_set_scalar(&l->p.coeff[i],a->p.coeff[i].val.interval->sup);
      break;
    }
  }
  switch (a->cst.discr) {
  case AP_COEFF_SCALAR:
    ap_coeff_set_scalar(&l->cst,a->cst.val.scalar);
    break;
  case AP_COEFF_INTERVAL:
    if (rand()%2) ap_coeff_set_scalar(&l->cst,a->cst.val.interval->inf);
    else ap_coeff_set_scalar(&l->cst,a->cst.val.interval->sup);
    break;

  }
 return l;
}

ap_lincons0_t random_lincons_linear_from_intlinear(ap_lincons0_t a)
{
  return ap_lincons0_make(a.constyp,
			  random_linexpr_linear_from_intlinear(a.linexpr0),
			  NULL);
}

/* random tree expression */
ap_texpr0_t* random_texpr_std(int dim, int depth)
{
  if (depth==0) {
    switch (rand()%3) {
    case 0: return ap_texpr0_dim(rand()%dim);
    case 1: return ap_texpr0_cst_scalar_frac(rand()%20-2,rand()%4+1);
    default:
      {
	int n1 = rand()%20-10;
	int n2 = n1 + rand()%10;
	int d1  = rand()%4+1;
	int d2  = n2>=0 ? rand()%d1+1 : d1+1+rand()%4;
	return ap_texpr0_cst_interval_frac(n1,d1,n2,d2);
      }
    }
  }
  else {
    if (rand()%2)
      return ap_texpr0_unop(AP_TEXPR_NEG+rand()%(AP_TEXPR_SQRT-AP_TEXPR_NEG+1),
			    random_texpr(dim,rand()%depth),
			    rand()%AP_RTYPE_SIZE, rand()%AP_RDIR_SIZE);
    else
      return ap_texpr0_binop(rand()%AP_TEXPR_NEG,
			     random_texpr(dim,rand()%depth),
			     random_texpr(dim,rand()%depth),
			     rand()%AP_RTYPE_SIZE, rand()%AP_RDIR_SIZE);
  }
}

ap_texpr0_t* random_texpr_real(int dim, int depth)
{
  if (depth==0) {
    switch (rand()%3) {
    case 0: return ap_texpr0_dim(rand()%dim);
    case 1: return ap_texpr0_cst_scalar_frac(rand()%20-2,rand()%4+1);
    default:
      {
	int n1 = rand()%20-10;
	int n2 = n1 + rand()%10;
	int d1  = rand()%4+1;
	int d2  = n2>=0 ? rand()%d1+1 : d1+1+rand()%4;
	return ap_texpr0_cst_interval_frac(n1,d1,n2,d2);
      }
    }
  }
  else {
    if (rand()%2)
      return ap_texpr0_unop(AP_TEXPR_NEG+rand()%(AP_TEXPR_SQRT-AP_TEXPR_NEG+1),
			    random_texpr(dim,rand()%depth),
			    AP_RTYPE_REAL, rand()%AP_RDIR_SIZE);
    else
      return ap_texpr0_binop(rand()%AP_TEXPR_NEG,
			     random_texpr(dim,rand()%depth),
			     random_texpr(dim,rand()%depth),
			     AP_RTYPE_REAL, rand()%AP_RDIR_SIZE);
  }
}

/* random generator of specified type */
ap_generator0_t random_generator(int dim, ap_gentyp_t g)
{
  ap_linexpr0_t* l = random_linexpr_linear(dim);
  ap_coeff_set_scalar_int(&l->cst,0);
  return ap_generator0_make(g,l);
}

/* random constraint of specified type */
ap_lincons0_t random_constraint(int dim, ap_constyp_t c)
{
  ap_linexpr0_t* l = random_linexpr(dim);
  switch (c) {
  case AP_CONS_EQ:
  case AP_CONS_SUPEQ:
  case AP_CONS_SUP:
    return ap_lincons0_make(c,l,NULL);
    break;
  case AP_CONS_EQMOD:
    {
      ap_scalar_t* scalar = ap_scalar_alloc();
      ap_scalar_set_frac(scalar,rand()%20-10,rand()%4+1);
      return ap_lincons0_make(c,l,scalar);
    }
    break;
  default:
    abort();
  }
}

/* ********************************************************************** */
/* Random generation of abstract values */
/* ********************************************************************** */

/* random abstract value, created with the vdim vertices, and rdim
   rays. */
ap_abstract0_t* random_abstract_std(ap_manager_t* man, int dim)
{
  int i;
  ap_abstract0_t* p;
  ap_interval_t** t;
  ap_generator0_array_t ar;

  int vdim,rdim;

  vdim = (dim * 90) / 100;
  rdim = dim-vdim;

  t = ap_interval_array_alloc(dim);
  if (vdim==0){
    /* creating an abstract value with the origin */
    for (i=0;i<dim;i++)
      ap_interval_set_int(t[i],0,0);

    p = intdim ?
      ap_abstract0_of_box(man,dim/2,dim-dim/2,t) :
      ap_abstract0_of_box(man,0,dim,t);
  }
  else {
    /* creating an abstract value with a random vertex */
    random_vertex(t,dim);
    p = intdim ?
      ap_abstract0_of_box(man,dim/2,dim-dim/2,t) :
      ap_abstract0_of_box(man,0,dim,t);
    /* adding other vertices */
    for (i=1; i<vdim; i++){
      ap_abstract0_t* tmp;
      random_vertex(t,dim);
      tmp = intdim ?
	ap_abstract0_of_box(man,dim/2,dim-dim/2,t) :
	ap_abstract0_of_box(man,0,dim,t);
      p = ap_abstract0_join(man,true,p,tmp);
      ap_abstract0_free(tmp->man,tmp);
    }
  }

  /* adding rays */
  ar = ap_generator0_array_make(rdim);
  for (i=0;i<rdim;i++)
    ar.p[i] = random_generator(dim,AP_GEN_RAY);
  p = ap_abstract0_add_ray_array(man,true,p,&ar);

  /* clearing */
  ap_interval_array_free(t,dim);
  ap_generator0_array_clear(&ar);

  // fprintf(stdout,"abs0:\n"); ap_abstract0_fprint(stdout,man,p,0);

  return p;
}

/* random abstract value, created with dim/RATIOEQ equalities */
#define RATIOEQ 3

ap_abstract0_t* random_abstract_eq(ap_manager_t* man, int dim)
{
  int i;
  ap_abstract0_t* p;
  ap_lincons0_array_t ar;

  /* creating a top abstract value */
  p = intdim ?
    ap_abstract0_top(man,dim/2,dim-dim/2) :
    ap_abstract0_top(man,0,dim);

  ar = ap_lincons0_array_make(dim/RATIOEQ);
  for (i=0;i<dim/RATIOEQ;i++)
    ar.p[i] = random_constraint(dim,AP_CONS_EQ);
  p = ap_abstract0_meet_lincons_array(man,true,p,&ar);
  ap_lincons0_array_clear(&ar);
  // fprintf(stdout,"abs0:\n"); ap_abstract0_fprint(stdout,man,p,0);

  return p;
}

/* random abstract value, created with dim/RATIOEQ equalities modulo */
#define RATIOEQMOD 2
ap_abstract0_t* random_abstract_eqmod(ap_manager_t* man, int dim)
{
  int i;
  ap_abstract0_t* p;
  ap_lincons0_array_t ar;

  /* creating a top abstract value */
  p = intdim ?
    ap_abstract0_top(man,dim/2,dim-dim/2) :
    ap_abstract0_top(man,0,dim);

  ar = ap_lincons0_array_make(dim/RATIOEQ);
  for (i=0;i<dim/RATIOEQ;i++){
    ap_constyp_t constyp = (rand() % RATIOEQMOD == 0) ? AP_CONS_EQMOD : AP_CONS_EQ;
    ar.p[i] = random_constraint(dim,constyp);
  }
  p = ap_abstract0_meet_lincons_array(man,true,p,&ar);
  ap_lincons0_array_clear(&ar);

  // fprintf(stdout,"abs0:\n"); ap_abstract0_fprint(stdout,man,p,0);

  return p;
}

/* convert to specified manager */
ap_abstract0_t* convert(ap_manager_t* man, ap_abstract0_t* a)
{
  ap_abstract0_t* res;
  ap_lincons0_array_t l;
  ap_dimension_t d = ap_abstract0_dimension(a->man,a);
  if (a->man==man) return a;
  l = ap_abstract0_to_lincons_array(a->man,a);
  res = ap_abstract0_of_lincons_array(man,d.intdim,d.realdim,&l);
  ap_lincons0_array_clear(&l);
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
  ap_abstract0_fdump(stream,p->man,p);
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

  bot = ap_abstract0_bottom(manprec,0,D);
  top = ap_abstract0_top(manprec,0,D);
  ap_dimension_t d1 = ap_abstract0_dimension(manprec,bot);
  ap_dimension_t d2 = ap_abstract0_dimension(manprec,top);
  printf("\nperforming various tests\n");
  if (d1.intdim || d1.realdim!=D) printf("ap_abstract0_dimension failed #1\n");
  if (d2.intdim || d2.realdim!=D) printf("ap_abstract0_dimension failed #2\n");
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
    if (d.intdim || d.realdim!=D) printf("ap_abstract0_dimension failed #14\n");
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
    if (!is_eq(rough0,rough1) ||
	!is_eq(prec0,prec1) ||
	!is_leq(prec0,rough0)) {
      ERROR("different results");
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
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    RESULT('*');
    for (i=0;i<dim;i++) {
      bool roughd,precd;
      ap_interval_t* roughi,*preci;
      roughd = ap_abstract0_is_dimension_unconstrained(manrough,rougha,i);
      precd = ap_abstract0_is_dimension_unconstrained(manprec,preca,i);

      roughi = ap_abstract0_bound_dimension(manrough,rougha,i);
      preci = ap_abstract0_bound_dimension(manprec,preca,i);
      cmp = ap_interval_cmp(preci,roughi);
      if (roughd!=precd ||
	  random_abstract2_equal && cmp!=0 ||
	  cmp!=0 && cmp!=(-1)){
	ERROR("different results");
	print_abstract("rougha",rougha);
	print_abstract("preca",preca);
	fprintf(stream,"roughi[%i]=",(int)i); ap_interval_fprint(stream,roughi);
	fprintf(stream," preci[%i]=",(int)i); ap_interval_fprint(stream,preci);
	fprintf(stream," roughd=%i precd=%i\n",roughd,precd);
      }
      ap_interval_free(roughi);
      ap_interval_free(preci);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
  } ENDLOOP;
}

void test_bound_linexpr(void)
{
  printf("\nbound_linexpr\n");
  LOOP {
    int cmp;
    size_t dim = 6;
    ap_abstract0_t* rougha,*preca;
    ap_interval_t* roughi,*preci;
    ap_linexpr0_t* l = random_linexpr(dim);
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    roughi = ap_abstract0_bound_linexpr(manrough,rougha,l);
    preci = ap_abstract0_bound_linexpr(manprec,preca,l);
    cmp = ap_interval_cmp(preci,roughi);
    RESULT('*');
    if (random_abstract2_equal && cmp!=0 ||
	cmp!=0 && cmp!=(-1)){
      ERROR("different results");
      print_abstract("rougha",rougha); print_abstract("preca",preca);
      ap_linexpr0_fprint(stream,l,NULL);
      fprintf(stream,"\nroughi="); ap_interval_fprint(stream,roughi);
      fprintf(stream,"\npreci="); ap_interval_fprint(stream,preci);
      fprintf(stream,"\n");
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_interval_free(roughi); ap_interval_free(preci);
    ap_linexpr0_free(l);
  } ENDLOOP;
}

void test_bound_texpr(void)
{
  printf("\nbound_texpr\n");
  LOOP {
    int cmp;
    size_t dim = 6, depth=3;
    ap_abstract0_t* rougha,*preca;
    ap_interval_t* roughi,*preci;
    ap_texpr0_t* e = random_texpr(dim,depth);
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    roughi = ap_abstract0_bound_texpr(manrough,rougha,e);
    preci = ap_abstract0_bound_texpr(manprec,preca,e);
    cmp = ap_interval_cmp(preci,roughi);
    RESULT('*');
    if (random_abstract2_equal && cmp!=0 ||
	cmp!=0 && cmp!=(-1)){
      ERROR("different results");
      print_abstract("rougha",rougha); print_abstract("preca",preca);
      ap_texpr0_fprint(stream,e,NULL);
      fprintf(stream,"\nroughi="); ap_interval_fprint(stream,roughi);
      fprintf(stream,"\npreci="); ap_interval_fprint(stream,preci);
      fprintf(stream,"\n");
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_interval_free(roughi); ap_interval_free(preci);
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
    ap_interval_t* i = ap_interval_alloc();
    bool roughs,precs;
    random_interval(i);
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    roughs = ap_abstract0_sat_interval(manrough,rougha,p,i);
    precs = ap_abstract0_sat_interval(manprec,preca,p,i);
    RESULT('*');
    if (random_abstract2_equal &&
	((roughs && roughs!=precs) ||
	 (precs && roughs!=precs)) ||
	roughs && !precs){
      ERROR("different results");
      print_abstract("rougha",rougha);
      print_abstract("preca",preca);
      ap_interval_fprint(stream,i);
      fprintf(stream,"\nvar=%i\nroughs=%i precs=%i\n",(int)p,roughs,precs);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_interval_free(i);
  } ENDLOOP;
}

void test_sat_lincons(void)
{
  printf("\nsat_lincons\n");
  LOOP {
    size_t dim = 6;
    ap_abstract0_t* rougha,*preca;
    ap_lincons0_t l = ap_lincons0_make((rand()%100>=90)?AP_CONS_EQ:
				       (rand()%100>=90)?AP_CONS_SUP:AP_CONS_SUPEQ,
				       random_linexpr(dim),NULL);
    bool roughs,precs;
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    roughs = ap_abstract0_sat_lincons(manrough,rougha,&l);
    precs = ap_abstract0_sat_lincons(manprec,preca,&l);
    RESULT('*');
    if (random_abstract2_equal &&
	((roughs && roughs!=precs) ||
	 (precs && roughs!=precs)) ||
	roughs && !precs){
      ERROR("different results");
      print_abstract("rougha",rougha);print_abstract("preca",preca);
      ap_lincons0_fprint(stream,&l,NULL);
      fprintf(stream,"\nroughs=%i precs=%i\n",roughs,precs);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_lincons0_clear(&l);
  } ENDLOOP;
}

void test_sat_tcons(void)
{
  printf("\nsat_tcons\n");
  LOOP {
    size_t dim = 6, depth=3;
    ap_abstract0_t* rougha,*preca;
    ap_tcons0_t l = ap_tcons0_make((rand()%100>=90)?AP_CONS_EQ:
				     (rand()%100>=90)?AP_CONS_SUP:AP_CONS_SUPEQ,
				     random_texpr(dim,depth),NULL);
    bool roughs,precs;
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    roughs = ap_abstract0_sat_tcons(manrough,rougha,&l);
    precs = ap_abstract0_sat_tcons(manprec,preca,&l);
    RESULT('*');
    if (random_abstract2_equal &&
	((roughs && roughs!=precs) ||
	 (precs && roughs!=precs)) ||
	roughs && !precs){
      ERROR("different results");
      print_abstract("rougha",rougha);print_abstract("preca",preca);
      ap_tcons0_fprint(stream,&l,NULL);
      fprintf(stream,"\nroughs=%i precs=%i\n",roughs,precs);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_tcons0_clear(&l);
  } ENDLOOP;
}

void test_to_box(void)
{
  printf("\nbox conversion\n");
  LOOP {
    size_t i, dim = 6;
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_interval_t** roughi,**preci;
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    roughi = ap_abstract0_to_box(manrough,rougha);
    preci = ap_abstract0_to_box(manprec,preca);
    roughr = ap_abstract0_of_box(manrough,0,dim,(ap_interval_t**)roughi);
    precr = ap_abstract0_of_box(manprec,0,dim,(ap_interval_t**)preci);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      for (i=0;i<dim;i++) {
	fprintf(stream,"roughi[%i]=",(int)i); ap_interval_fprint(stream,roughi[i]);
	fprintf(stream," preci[%i]=",(int)i); ap_interval_fprint(stream,preci[i]);
	fprintf(stream,"\n");
      }
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_interval_array_free(roughi,dim); ap_interval_array_free(preci,dim);
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
    ap_lincons0_array_t ar = ap_lincons0_array_make(nb);
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    for (i=0;i<nb;i++) {
      ar.p[i] = ap_lincons0_make((rand()%100>=90)?AP_CONS_EQ:
				 (rand()%100>=90)?AP_CONS_SUP:AP_CONS_SUPEQ,
				 random_linexpr(dim),NULL);
    }
    roughr = ap_abstract0_meet_lincons_array(manrough,false,rougha,&ar);
    precr = ap_abstract0_meet_lincons_array(manprec,false,preca,&ar);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      ap_lincons0_array_fprint(stream,&ar,NULL);
      print_abstract("rougha",rougha); print_abstract("preca",preca);
      print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_lincons0_array_clear(&ar);
  } ENDLOOP;
}

void test_meet_tcons_array(void)
{
  printf("\nmeet_tcons_array\n");
  LOOP {
    size_t i, dim = 6, depth = 4, nb = 3;
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_tcons0_array_t ar = ap_tcons0_array_make(nb);
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    for (i=0;i<nb;i++) {
      ar.p[i] = ap_tcons0_make((rand()%100>=90)?AP_CONS_EQ:
			       (rand()%100>=90)?AP_CONS_SUP:AP_CONS_SUPEQ,
			       random_texpr(dim,depth),NULL);
    }
    roughr = ap_abstract0_meet_tcons_array(manrough,false,rougha,&ar);
    precr = ap_abstract0_meet_tcons_array(manprec,false,preca,&ar);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      ap_tcons0_array_fprint(stream,&ar,NULL);
      print_abstract("rougha",rougha); print_abstract("preca",preca);
      print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_tcons0_array_clear(&ar);
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
    ap_generator0_array_t ar = ap_generator0_array_make(nb);
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    for (i=0;i<nb;i++)
      ar.p[i] = random_generator(dim,(rand()%100>=80)?AP_GEN_LINE:AP_GEN_RAY);
    roughr = ap_abstract0_add_ray_array(manrough,false,rougha,&ar);
    precr = ap_abstract0_add_ray_array(manprec,false,preca,&ar);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      ap_generator0_array_fprint(stream,&ar,NULL);
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_generator0_array_clear(&ar);
  } ENDLOOP;
}

void test_assign_linexpr(void)
{
  printf("\nassign_linexpr\n");
  LOOP {
    size_t dim = 7;
    size_t p = rand() % dim;
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_linexpr0_t* l = random_linexpr(dim);
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
    ap_linexpr0_free(l);
  } ENDLOOP;
}

void test_assign_linexpr_array(void)
{
  printf("\nassign_linexpr_array\n");
  LOOP {
    size_t i, dim = 5;
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_dim_t d[nb_asssub_array];
    ap_linexpr0_t *l[nb_asssub_array];
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    for (i=0;i<nb_asssub_array;i++) {
      l[i]  = random_linexpr(dim);
      if (!i) d[i] = rand()%(dim-nb_asssub_array);
      else d[i] = d[i-1] + 1 + (rand()%(dim-nb_asssub_array+i-d[i-1]));
    }
    roughr = ap_abstract0_assign_linexpr_array(manrough,false,rougha,d,(ap_linexpr0_t**)l,nb_asssub_array,NULL);
    precr = ap_abstract0_assign_linexpr_array(manprec,false,preca,d,(ap_linexpr0_t**)l,nb_asssub_array,NULL);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      for (i=0;i<nb_asssub_array;i++) {
	fprintf(stream,"x%i <- ",d[i]);
	ap_linexpr0_fprint(stream,l[i],NULL);
	fprintf(stream,"\n");
      }
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("preca",preca); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    for (i=0;i<nb_asssub_array;i++) ap_linexpr0_free(l[i]);
  } ENDLOOP;
}

void test_substitute_linexpr(void)
{
  printf("\nsubstitute_linexpr\n");
  LOOP {
    size_t dim = 7;
    size_t p = rand() % dim;
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_linexpr0_t* l = random_linexpr(dim);
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    roughr = ap_abstract0_substitute_linexpr(manrough,false,rougha,p,l,NULL);
    precr = ap_abstract0_substitute_linexpr(manprec,false,preca,p,l,NULL);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      fprintf(stream,"x%i -> ",(int)p); ap_linexpr0_fprint(stream,l,NULL); fprintf(stream,"\n");
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_linexpr0_free(l);
  } ENDLOOP;
}

void test_substitute_linexpr_array(void)
{
  printf("\nsubstitute_linexpr_array\n");
  LOOP {
    size_t i, dim = 7;
    size_t p = rand() % dim;
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_dim_t d[nb_asssub_array];
    ap_linexpr0_t *l[nb_asssub_array];
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    for (i=0;i<nb_asssub_array;i++) {
      l[i]  = random_linexpr(dim);
      if (!i) d[i] = rand()%(dim-nb_asssub_array);
      else d[i] = d[i-1] + 1 + (rand()%(dim-nb_asssub_array+i-d[i-1]));
    }
    roughr = ap_abstract0_substitute_linexpr_array(manrough,false,rougha,d,(ap_linexpr0_t**)l,nb_asssub_array,NULL);
    precr = ap_abstract0_substitute_linexpr_array(manprec,false,preca,d,(ap_linexpr0_t**)l,nb_asssub_array,NULL);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      for (i=0;i<nb_asssub_array;i++) {
	fprintf(stream,"x%i -> ",d[i]);
	ap_linexpr0_fprint(stream,l[i],NULL);
	fprintf(stream,"\n");
      }
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    for (i=0;i<nb_asssub_array;i++) ap_linexpr0_free(l[i]);
  } ENDLOOP;
}

void test_assign_texpr(void)
{
  printf("\nassign_texpr\n");
  LOOP {
    size_t dim = 3, depth=3;
    size_t p = rand() % dim;
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_texpr0_t* l = random_texpr(dim, depth);
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    roughr = ap_abstract0_assign_texpr(manrough,false,rougha,p,l,NULL);
    precr = ap_abstract0_assign_texpr(manprec,false,preca,p,l,NULL);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
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
    ap_dim_t d[nb_asssub_array];
    ap_texpr0_t *l[nb_asssub_array];
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    for (i=0;i<nb_asssub_array;i++) {
      l[i]  = random_texpr(dim, depth);
      if (!i) d[i] = rand()%(dim-nb_asssub_array);
      else d[i] = d[i-1] + 1 + (rand()%(dim-nb_asssub_array+i-d[i-1]));
    }
    roughr = ap_abstract0_assign_texpr_array(manrough,false,rougha,d,(ap_texpr0_t**)l,nb_asssub_array,NULL);
    precr = ap_abstract0_assign_texpr_array(manprec,false,preca,d,(ap_texpr0_t**)l,nb_asssub_array,NULL);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      for (i=0;i<nb_asssub_array;i++) {
	fprintf(stream,"x%i <- ",d[i]);
	ap_texpr0_fprint(stream,l[i],NULL);
	fprintf(stream,"\n");
      }
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("preca",preca); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    for (i=0;i<nb_asssub_array;i++) ap_texpr0_free(l[i]);
  } ENDLOOP;
}

void test_substitute_texpr(void)
{
  printf("\nsubstitute_texpr\n");
  LOOP {
    size_t dim = 7, depth=3;
    size_t p = rand() % dim;
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_texpr0_t* l = random_texpr(dim, depth);
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    roughr = ap_abstract0_substitute_texpr(manrough,false,rougha,p,l,NULL);
    precr = ap_abstract0_substitute_texpr(manprec,false,preca,p,l,NULL);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      fprintf(stream,"x%i -> ",(int)p); ap_texpr0_fprint(stream,l,NULL); fprintf(stream,"\n");
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
    ap_dim_t d[nb_asssub_array];
    ap_texpr0_t *l[nb_asssub_array];
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    for (i=0;i<nb_asssub_array;i++) {
      l[i]  = random_texpr(dim, depth);
      if (!i) d[i] = rand()%(dim-nb_asssub_array);
      else d[i] = d[i-1] + 1 + (rand()%(dim-nb_asssub_array+i-d[i-1]));
    }
    roughr = ap_abstract0_substitute_texpr_array(manrough,false,rougha,d,(ap_texpr0_t**)l,nb_asssub_array,NULL);
    precr = ap_abstract0_substitute_texpr_array(manprec,false,preca,d,(ap_texpr0_t**)l,nb_asssub_array,NULL);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      for (i=0;i<nb_asssub_array;i++) {
	fprintf(stream,"x%i -> ",d[i]);
	ap_texpr0_fprint(stream,l[i],NULL);
	fprintf(stream,"\n");
      }
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    for (i=0;i<nb_asssub_array;i++) ap_texpr0_free(l[i]);
  } ENDLOOP;
}


void test_forget_array(void)
{
  printf("\nforget_array\n");
  LOOP {
    size_t i, dim = 8;
    ap_dimchange_t* a = ap_dimchange_alloc(0,2);
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_dimension_t d;
    int proj = rand()%2;
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    d = ap_abstract0_dimension(manrough,rougha);
    for (i=0;i<a->intdim+a->realdim;i++) {
      a->dim[i] = rand()%3 + 1;
      if (i) a->dim[i] += a->dim[i-1];
      if (a->dim[i]<d.intdim) { a->intdim++; a->realdim--; }
      assert(a->dim[i]<dim);
    }
    roughr = ap_abstract0_forget_array(manrough,false,rougha,a->dim,a->realdim,proj);
    precr = ap_abstract0_forget_array(manprec,false,preca,a->dim,a->realdim,proj);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      ap_dimchange_fprint(stream,a);
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_dimchange_free(a);
  } ENDLOOP;
}

void test_add_dimensions(void)
{
  printf("\nadd dimensions\n");
  LOOP {
    size_t i, dim = 8;
    ap_dimchange_t* a = ap_dimchange_alloc(0,3);
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    int proj = rand()%2;
    ap_dimension_t d;
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    d = ap_abstract0_dimension(manrough,rougha);
    for (i=0;i<a->intdim+a->realdim;i++) {
      a->dim[i] = rand()%3;
      if (i) a->dim[i] += a->dim[i-1];
      if (a->dim[i]<d.intdim) { a->intdim++; a->realdim--; }
      assert(a->dim[i]<dim);
    }
    roughr = ap_abstract0_add_dimensions(manrough,false,rougha,a,proj);
    precr = ap_abstract0_add_dimensions(manprec,false,preca,a,proj);
    RESULT('*');
    if (!is_eq(precr,roughr)) {
      ERROR("different results");
      ap_dimchange_fprint(stream,a);
      fprintf(stream,"proj=%d\n",proj);
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("preca",preca); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_dimchange_free(a);
  } ENDLOOP;
}

void test_remove_dimensions(void)
{
  printf("\nremove dimensions\n");
  LOOP {
    size_t i, dim = 8;
    ap_dimchange_t* a = ap_dimchange_alloc(0,2);
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_dimension_t d;
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    d = ap_abstract0_dimension(manrough,rougha);
    for (i=0;i<a->intdim+a->realdim;i++) {
      a->dim[i] = rand()%3 + 1;
      if (i) a->dim[i] += a->dim[i-1];
      if (a->dim[i]<d.intdim) { a->intdim++; a->realdim--; }
      assert(a->dim[i]<dim);
    }
    roughr = ap_abstract0_remove_dimensions(manrough,false,rougha,a);
    precr = ap_abstract0_remove_dimensions(manprec,false,preca,a);
    RESULT('*');
    if (!is_leq(precr,roughr)) {
      ERROR("different results");
      ap_dimchange_fprint(stream,a);
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_dimchange_free(a);
  } ENDLOOP;
}

void test_permute_dimensions(void)
{
  printf("\npermute dimensions\n");
  LOOP {
    size_t i, dim = 6;
    ap_dimperm_t* p = ap_dimperm_alloc(dim);
    ap_abstract0_t* rougha,*roughr, *preca,*precr;
    ap_dimension_t d;
    random_abstract2(manprec,manrough,dim,&preca,&rougha);
    d = ap_abstract0_dimension(manrough,rougha);
    /* random permutation */
    ap_dimperm_set_id(p);
    for (i=0;i<p->size-1;i++) {
      int a, j;
      if (i<d.intdim) j = i + (rand() % (d.intdim-i));
      else j = i + (rand() % (d.intdim+d.realdim-i));
      a = p->dim[j]; p->dim[j] = p->dim[i]; p->dim[i] = a;
    }
    roughr = ap_abstract0_permute_dimensions(manrough,false,rougha,p);
    precr = ap_abstract0_permute_dimensions(manprec,false,preca,p);
    RESULT('*');
    if (!is_eq(precr,roughr)) {
      ERROR("different results");
      ap_dimperm_fprint(stream,p);
      print_abstract("rougha",rougha); print_abstract("roughr",roughr); print_abstract("precr",precr);
    }
    ap_abstract0_free(manrough,rougha); ap_abstract0_free(manprec,preca);
    ap_abstract0_free(manrough,roughr); ap_abstract0_free(manprec,precr);
    ap_dimperm_free(p);
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
      if (dd[0]<d.intdim) dd[1] = rand() % (d.intdim-1);
      else dd[1] = d.intdim + rand() % (d.realdim-1);
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

  manprec = man1;
  manrough = man2;
  for (i=0;i<AP_EXC_SIZE;i++){
    manprec->option.abort_if_exception[i] = true;
    manrough->option.abort_if_exception[i] = true;
  }
  for (intdim=0;intdim<1;intdim++) {
    printf("\n\ncomparing libraries:\n- %s (%s)\n- %s (%s)\nwith int=%i\n\n",
	   manprec->library,manprec->version,manrough->library,manrough->version,intdim);
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
    /*
    test_substitute_texpr();
    test_substitute_texpr_array();
    test_forget_array();

    test_add_dimensions();
    test_remove_dimensions();
    test_permute_dimensions();
    test_expand();
    test_fold();
    */
  }
  if (error_) printf("\n%i error(s)!\n",error_);
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
  ap_manager_t* manbox = box_manager_alloc();
  ap_manager_t* manoct = oct_manager_alloc();
  ap_manager_t* manpkl = pk_manager_alloc(false);
  ap_manager_t* manpks = pk_manager_alloc(true);
  ap_manager_t* manpkeq = pkeq_manager_alloc();

  ap_funopt_t funopt = ap_manager_get_funopt(manpkl,AP_FUNID_REMOVE_DIMENSIONS);
  funopt.algorithm = -1;
  ap_manager_set_funopt(manpkl,AP_FUNID_REMOVE_DIMENSIONS,&funopt);

  ap_manager_t* manppll = ap_ppl_poly_manager_alloc(false);
  ap_manager_t* manppls = ap_ppl_poly_manager_alloc(true);
  ap_manager_t* manpplgrid = ap_ppl_grid_manager_alloc();
  ap_manager_t* manpkgrid = ap_pkgrid_manager_alloc(manpkl,manpplgrid);

  /* First serie */
  intdim = 0;
  random_linexpr = &random_linexpr_linear;
  random_abstract = &random_abstract_std;
  for (i=0; i<1; i++){
    random_abstract2 = i==0 ? &random_abstract2_std : &random_abstract2_inv;
    /*
    // box/polyhedra
    test(manpkl,manbox);
    test(manpks,manbox);
    test(manppll,manbox);
    test(manppls,manbox);

    // polyhedra/polyhedra (same library)
    test(manpks,manpkl);
    test(manpkl,manpks);
    test(manppls,manppll);
    test(manppll,manppls);
    // Polka polyhedra/PPL polyhedra
    test(manppll,manpkl);
    test(manpkl,manppll);
    test(manppls,manpks);
    test(manpks,manppls);
    */
    // Grid/Equalities
    test(manpplgrid,manpkeq);
    random_abstract = &random_abstract_eq;
    /*
    test(manpplgrid,manpkeq);
    random_abstract = &random_abstract_eqmod;
    test(manpplgrid,manpkeq);
    */
    // manpkgrid
    /*
    random_abstract = &random_abstract_eqmod;
    test(manpkgrid,manpplgrid);
    test(manpkgrid,manpkl);
    */
    // Oct/Box and Oct/Poly
    /*
    random_abstract = &random_abstract_std;
    test(manoct,manbox);
    test(manpkl,manoct);
    test(manpks,manoct);
    //    test(manppll,manoct);
    //test(manppls,manoct);
    */
  }


  /* clean-up */
  ap_manager_free(manbox);
  ap_manager_free(manoct);
  ap_manager_free(manpkl);
  ap_manager_free(manpks);
  ap_manager_free(manpkeq);
  ap_manager_free(manppll);
  ap_manager_free(manppls);
  ap_manager_free(manpplgrid);
  ap_manager_free(manpkgrid);
  return 0;
}
