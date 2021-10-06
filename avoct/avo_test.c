/*
 * avo_test.c
 *
 * Unit testing.
 *
 *
 * APRON Library / Absolute Value Octagonal (AVO) Domain
 *
 * Copyright (C) Liqian Chen & Jiangchao Liu' 2014-
 *
 */

/* This file is part of the APRON Library, released under LGPL license.  
   Please read the COPYING file packaged in the distribution.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

long int lrand48(void);
void srand48(long int seedval);

#include "avo.h"
#include "avo_fun.h"
#include "avo_internal.h"

ap_manager_t* mo; /* avOctagon */

avo_internal_t* pr;

#define MAXN 50000
int N = 70;

char b1_[MAXN+4]= " [";
char b2_[MAXN+4];
int i_;
int error_ = 0;

typedef enum { none = 0, best = 1, exact = 2 } exactness;

exactness flag;

#define LOOP								\
  {									\
    memset(b1_+2,' ',N); b1_[N+2]=']'; b1_[N+3]=0;			\
    memset(b2_,8,N+3); b2_[N+3]=0;					\
    for (i_=0;i_<N;i_++) {						\
      if (N<80) printf("%s%s",b1_,b2_);					\
      else if (i_) printf("%c",b1_[i_+1]);				\
      fflush(stdout);							\
      flag = exact;							\
      
#define FLAG(m)						    \
  if (m->result.flag_exact) ;				    \
  else if (m->result.flag_best && flag==exact) flag = best; \
  else flag = none;

#define RESULT(c) b1_[i_+2]=c

#define ERROR(msg)					\
  do {							\
    fprintf(stderr,"\n%s\n",msg);			\
    RESULT('!');					\
    error_++;						\
  } while (0)

#define ENDLOOP					\
  } }						\
  if (N<80) printf("%s%s\n",b1_,b2_);		\
  else printf("%c",b1_[i_+2])


char check(avo_t* o)
{
  if (!o->closed && !o->m) return 'o'; /* really empty */
  if (o->closed) {
    bound_t* cl;
    bound_t* dl;
    size_t i;
    if (num_incomplete || o->intdim) return '.'; /* incomplete */
    /* check that o->closed is really closed */
    if (!avo_hmat_check_closed(o->closed,o->nsc,o->dim)) {
      if(avo_closure_alg<=1)
    	   ERROR("invalid closure: due to weak join");
      else ERROR("invalid closure: due to strong join");
      return '!';
    }
    if (!o->m) return 'c'; /* ok */
    /* now check that closure(o->m) = o->closed */
    cl = avo_hmat_copy(pr,o->m,o->dim);
    dl = avo_hmat_copy(pr,o->nsc,o->dim);
    avo_hmat_close(cl,dl,o->dim);
    for (i=0;i<avo_matsize(o->dim);i++){
      int order=bound_cmp(cl[i],o->closed[i]);
      if (order!=0 || (order==0 && bound_cmp(dl[i],o->nsc[i])<0)) {
		avo_hmat_free(pr,cl,o->dim);
		avo_hmat_free(pr,dl,o->dim);
		return '#';
      }
    }
    avo_hmat_free(pr,cl,o->dim);
    avo_hmat_free(pr,dl,o->dim);
    return 'C';
  }
  return '.'; /* regular matrix */
}

void random_bound(bound_t b)
{
  num_t n;
  num_init(n);
  num_set_int2(n,lrand48()%20-2,lrand48()%4+1);
  bound_set_num(b,n);
  num_clear(n);
}

avo_t* random_avo(int dim,float frac)
{
  avo_t* o = avo_alloc_internal(pr,dim,0);
  bound_t* b;
  int i,j,x,y;
  num_t n;
  o->m = avo_hmat_alloc_top(pr,dim);
  o->nsc = avo_hmat_alloc_top_nsc(pr,dim);
  b = o->m;
  num_init(n);
  for (i=0;i<4*dim;i++)
    for (j=0;j<=(i|1);j++,b++) {
      if (i==j) continue;
      if (lrand48()%100>frac*100) continue;
      y = lrand48()%4+1;
      x = lrand48()%20-2;      
      num_set_int2(n,x,y);
      bound_set_num(*b,n);
    }
  num_clear(n);
  return o;
}

typedef enum  {
  expr_unary,
  expr_avo,
  expr_lin,
  expr_qlin,
  expr_interv,
} exprmode;

const char* exprname[] = { "unary ", "avogonal ", "", "quasi", "interval " };

ap_linexpr0_t* random_linexpr(exprmode mode, int dim)
{
  ap_linexpr0_t* l = ap_linexpr0_alloc(AP_LINEXPR_DENSE,dim);
  int i,n1,n2,d;
  if (mode<=expr_avo) {
    if (lrand48()%10>2)
      ap_coeff_set_scalar_int(l->p.coeff+(lrand48()%dim),lrand48()%3-1);
    if (mode==expr_avo)
      if (lrand48()%10>2)
	ap_coeff_set_scalar_int(l->p.coeff+(lrand48()%dim),lrand48()%3-1);
  }
  else if (mode<=expr_qlin) {
    for (i=0;i<dim;i++)
      ap_coeff_set_scalar_frac(l->p.coeff+i,lrand48()%20-10,lrand48()%4+1);
  }
  else {
    for (i=0;i<dim;i++) {
      int n1 = lrand48()%20-10;
      int n2 = n1 + lrand48()%20;
      int d  = lrand48()%4+1;
      ap_coeff_set_interval_frac(l->p.coeff+i,n1,d,n2,d);
    }
  }
  if (mode<=expr_lin) {
    ap_coeff_set_scalar_frac(&l->cst,lrand48()%20-10,lrand48()%4+1);
  }
  else {
    int n1 = lrand48()%20-10;
    int n2 = n1 + lrand48()%20;
    int d  = lrand48()%4+1;
    ap_coeff_set_interval_frac(&l->cst,n1,d,n2,d);
  }
  return l;
}

/* chose one linexpr within an interval linexpr */
ap_linexpr0_t* random_from_linexpr(ap_linexpr0_t* a)
{
  size_t i;
  ap_linexpr0_t* l = ap_linexpr0_alloc(AP_LINEXPR_DENSE,a->size);
  assert(a->discr==AP_LINEXPR_DENSE);
  for (i=0;i<a->size;i++) {
    switch (a->p.coeff[i].discr) {
    case AP_COEFF_SCALAR:
      ap_coeff_set_scalar(l->p.coeff+i,a->p.coeff[i].val.scalar);
      break;
    case AP_COEFF_INTERVAL:
      if (lrand48()%2)
    	  ap_coeff_set_scalar(l->p.coeff+i,a->p.coeff[i].val.interval->inf);
      else 
    	  ap_coeff_set_scalar(l->p.coeff+i,a->p.coeff[i].val.interval->sup);
      break;
      
    }
  }
  switch (a->cst.discr) {
  case AP_COEFF_SCALAR:
    ap_coeff_set_scalar(&l->cst,a->cst.val.scalar);
    break;
  case AP_COEFF_INTERVAL:
    if (lrand48()%2) ap_coeff_set_scalar(&l->cst,a->cst.val.interval->inf);
    else ap_coeff_set_scalar(&l->cst,a->cst.val.interval->sup);
    break;
    
  }
 return l;
}

ap_lincons0_t random_from_lincons(ap_lincons0_t a)
{
  return ap_lincons0_make(a.constyp,random_from_linexpr(a.linexpr0),NULL);
}

ap_generator0_t random_generator(int dim, ap_gentyp_t g)
{
  ap_linexpr0_t* l = ap_linexpr0_alloc(AP_LINEXPR_DENSE,dim);
  int i;
  for (i=0;i<dim;i++)
    ap_coeff_set_scalar_frac(l->p.coeff+i,lrand48()%20-2,lrand48()%4+1);
  ap_coeff_set_scalar_int(&l->cst,0);
  return ap_generator0_make(g,l);
}

ap_texpr0_t* random_texpr(int dim, int depth)
{
  if (!depth) {
    switch (lrand48()%3) {
    case 0: return ap_texpr0_dim(lrand48()%dim);
    case 1: return ap_texpr0_cst_scalar_frac(lrand48()%20-2,lrand48()%4+1);
    default:
      {
	int n1 = lrand48()%20-2, d = lrand48()%4+1;
	int n2 = n1 + lrand48()%10;
	return ap_texpr0_cst_interval_frac(n1,d,n2,d);
      }
    }
  }
  else if (lrand48()%2)
    return ap_texpr0_unop(AP_TEXPR_NEG+lrand48()%(AP_TEXPR_SQRT-AP_TEXPR_NEG+1),
			  random_texpr(dim,lrand48()%depth),
			  lrand48()%AP_RTYPE_SIZE, lrand48()%AP_RDIR_SIZE);
  else
    return ap_texpr0_binop(lrand48()%AP_TEXPR_NEG,
			   random_texpr(dim,lrand48()%depth),
			   random_texpr(dim,lrand48()%depth),
			   lrand48()%AP_RTYPE_SIZE, lrand48()%AP_RDIR_SIZE);
}

void print_avo(const char* msg, avo_t* o)
{
  fprintf(stderr,"%s = ",msg);
  avo_fprint(stderr,mo,o,NULL);
  avo_fdump(stderr,mo,o);
  fprintf(stderr,"\n");
}

void print_interval(const char*msg, ap_interval_t* i)
{
  fprintf(stderr,"%s = ",msg);
  ap_interval_fprint(stderr,i);
  fprintf(stderr,"\n");
}

bool avo_is_nleq(ap_manager_t* man, avo_t* a1, avo_t* a2)
{
  return !avo_is_leq(man,a1,a2) && man->result.flag_exact;
}


bool avo_is_neq(ap_manager_t* man, avo_t* a1, avo_t* a2)
{
  return !avo_is_eq(man,a1,a2) && man->result.flag_exact;
}

/* ********************************* */
/*             infos                 */
/* ********************************* */

void info(void)
{
  printf("avOctagons:  %s (%s)\n",mo->library,mo->version);
  printf("nums:      %s (%s wto overflow,%s)\n",num_name,
	 num_safe ? "sound" : "unsound",
	 num_incomplete ? "incomplete" : "complete");
}

/* ********************************* */
/*           various tests           */
/* ********************************* */

void test_misc(void)
{
  size_t D = 5;
  int i;
  avo_t* bot = avo_bottom(mo,0,D);
  avo_t* top = avo_top(mo,0,D);
  ap_dimension_t d1 = avo_dimension(mo,bot);
  ap_dimension_t d2 = avo_dimension(mo,top);
  printf("\nperforming various tests\n"); fflush(stdout);
  check(bot); check(top);
  if (d1.intdim || d1.realdim!=D) printf("avo_dimension failed #1\n");
  if (d2.intdim || d2.realdim!=D) printf("avo_dimension failed #2\n");
  if (!avo_is_bottom(mo,bot))  printf("avo_is_bottom failed #3\n");
  if (avo_is_bottom(mo,top))   printf("avo_is_bottom failed #4\n");
  if (avo_is_top(mo,bot))      printf("avo_is_top failed #5\n");
  if (!avo_is_top(mo,top))     printf("avo_is_top failed #6\n");
  if (!avo_is_leq(mo,bot,top)) printf("avo_is_leq failed #7\n");
  if (avo_is_leq(mo,top,bot))  printf("avo_is_leq failed #8\n");
  if (!avo_is_eq(mo,bot,bot))  printf("avo_is_eq failed #9\n");
  if (!avo_is_eq(mo,top,top))  printf("avo_is_eq failed #10\n");
  if (avo_is_eq(mo,bot,top))   printf("avo_is_eq failed #11\n");
  if (avo_is_dimension_unconstrained(mo,bot,0))
    printf("avo_is_dimension_unconstrained #12\n");
  if (!avo_is_dimension_unconstrained(mo,top,0))
    printf("avo_is_dimension_unconstrained #13\n");
  for (i=0;i<N;i++) {
    avo_t* o = random_avo(D,0.1);
    avo_t* c = avo_copy(mo,o);
    avo_t* l = avo_closure(mo,false,o);
    ap_dimension_t d = avo_dimension(mo,o);
    if (d.intdim || d.realdim!=D) printf("avo_dimension failed #14\n");
    if (!avo_is_leq(mo,bot,o))  printf("avo_is_leq failed #15\n");
    if (!avo_is_leq(mo,o,top))  printf("avo_is_leq failed #16\n");
    if (!avo_is_eq(mo,o,c))     printf("avo_is_eq failed #17\n");
    if (avo_is_neq(mo,o,l))     printf("avo_is_eq failed #18\n");
    avo_size(mo,o);
    avo_close(pr,o);
    avo_free(mo,o); avo_free(mo,c); avo_free(mo,l);
  }
  avo_free(mo,bot); avo_free(mo,top);
}

/* ********************************* */
/*           closure                 */
/* ********************************* */

void test_closure(void)
{
  printf("\nclosure %s\n",num_incomplete?"":"(c,o expected)");
  avo_t* o;
  bound_t* b;
  int dim;
  num_t n;

  /*
   * {y-x <= 24, -z-|x| <= 6,  x - z <= 16, y - |z| <= 10, y-z <= 50 }
   * StrongCloViaAllSigns(): {y-z <= 32, ...}
   * WeakCloVia3Sign():      {y-z <= 32, ...}
   * WeakCloVia1Sign():      {y-z <= 40, ...}
   * */
  dim =3;
  o = avo_alloc_internal(pr,dim,0);
  o->m = avo_hmat_alloc_top(pr,dim);
  o->nsc = avo_hmat_alloc_top_nsc(pr,dim);
  //avo_fdump(stdout, mo, o);
  b = o->m;

  num_init(n);
  for (int i=0;i<4*dim;i++)
    for (int j=0;j<=(i|1);j++,b++) {
      if (i==3 && j==1)        {num_set_int(n,24);   bound_set_num(*b,n);}
      else if (i==4 && j==0)   {num_set_int(n,16);   bound_set_num(*b,n);}
      else if (i==4 && j==2)   {num_set_int(n,50);   bound_set_num(*b,n);}
      else if (i==6 && j==5)   {num_set_int(n,6);    bound_set_num(*b,n);}
      else if (i==10 && j==2)  {num_set_int(n,10);   bound_set_num(*b,n);}
    }
  num_clear(n);
  //avo_fdump(stdout, mo, o);
  avo_close(pr,o);
  RESULT(check(o));
  //avo_fdump(stdout, mo, o);
  avo_free(mo,o);

  /*
   * {y-x <= 24, -z-|x| <= 6,  x - z <= 16, y - |z| <= 10, y-z <= 50, x<=-1 }
   * StrongCloViaAllSigns(): {y-z <= 30, z-|y|<=30, x+y<=22, y<=23, x-|y|<=-1, x-|z|<=-1, ...}
   * WeakCloVia3Sign():      {y-z <= 30, z-|y|<=30, x+y<=22, y<=23, x-|y|<=-1, x-|z|<=-1, ...}
   * WeakCloVia1Sign():      {y-z <= 40, z-|y|<=40, x+y<=22, y<=23, x-|y|<=-1, x-|z|<=-1, ...}
   * */

  dim =3;
  o = avo_alloc_internal(pr,dim,0);
  o->m = avo_hmat_alloc_top(pr,dim);
  o->nsc = avo_hmat_alloc_top_nsc(pr,dim);
  //avo_fdump(stdout, mo, o);
  b = o->m;
  num_init(n);
  for (int i=0;i<4*dim;i++)
    for (int j=0;j<=(i|1);j++,b++) {
      if (i==1 && j==0)        {num_set_int(n,-2);   bound_set_num(*b,n);}
      else if (i==3 && j==1)   {num_set_int(n,24);   bound_set_num(*b,n);}
      else if (i==4 && j==0)   {num_set_int(n,16);   bound_set_num(*b,n);}
      else if (i==4 && j==2)   {num_set_int(n,50);   bound_set_num(*b,n);}
      else if (i==6 && j==5)   {num_set_int(n,6);    bound_set_num(*b,n);}
      else if (i==10 && j==2)  {num_set_int(n,10);   bound_set_num(*b,n);}
  }
  num_clear(n);
  //avo_fdump(stdout, mo, o);
  avo_close(pr,o);
  RESULT(check(o));
  //avo_fdump(stdout, mo, o);
  avo_free(mo,o);

  /*
   * {x-|y|<=10, y<=24, y+s<=80, -y-z<=84, -z-|s|<=8, -s-|x|<=36}
   * Common constraints in the 3 closure algs: {y-z<=132,-z<=108,s-z<=164}
   * StrongCloViaAllSigns(): {x-z <= 112,x-|z|<=94, -|x|-z<=86, x+y<=58, ...}
   * WeakCloVia3Sign():      {x-z <= 142,x-|z|<=94, -|x|-z<=86, x+y<=58}
   * WeakCloVia1Sign():      {x-z <= +oo,x-|z|<=+oo, -|x|-z<=108, x+y<=+oo}
   * */
  dim =4;
  o = avo_alloc_internal(pr,dim,0);
  o->m = avo_hmat_alloc_top(pr,dim);
  o->nsc = avo_hmat_alloc_top_nsc(pr,dim);
  //avo_fdump(stdout, mo, o);
  b = o->m;
  num_init(n);
  for (int i=0;i<4*dim;i++)
    for (int j=0;j<=(i|1);j++,b++) {
      if (j==0 && i==2*dim+2)      {num_set_int(n,10);   bound_set_num(*b,n);}
      else if (j==2 && i==3)       {num_set_int(n,48);   bound_set_num(*b,n);}
      else if (j==2 && i==7)       {num_set_int(n,80);   bound_set_num(*b,n);}
      else if (j==3 && i==4)       {num_set_int(n,84);   bound_set_num(*b,n);}
      else if (j==5 && i==2*dim+6) {num_set_int(n,8);    bound_set_num(*b,n);}
      else if (j==7 && i==2*dim)   {num_set_int(n,36);   bound_set_num(*b,n);}
    }
  num_clear(n);
  //avo_fdump(stdout, mo, o);
  avo_close(pr,o);
  RESULT(check(o));
  //avo_fdump(stdout, mo, o);
  avo_free(mo,o);

  LOOP {
    o = random_avo(8,.01);//14,.01);
    //avo_fdump(stdout, mo, o);
    avo_close(pr,o);
    //avo_fdump(stdout, mo, o);
    RESULT(check(o));
    avo_free(mo,o);
  } ENDLOOP;
}

void test_incremental_closure(void)
{
  printf("\nincremental closure %s\n",num_incomplete?"":"(C,o expected)");
  LOOP {
    size_t dim = 8;
    avo_t* o = random_avo(dim,.005);
    size_t i, v = lrand48() % dim;
    avo_close(pr,o);
    if (o->closed) {
      for (i=0;2*i<dim;i++) {
		if (lrand48()%10>8) random_bound(o->closed[avo_matpos2(i,2*v)]);
		if (lrand48()%10>8) random_bound(o->closed[avo_matpos2(i,2*v+1)]);
      }
      o->m = avo_hmat_copy(pr,o->closed,dim);
      if (avo_hmat_close_incremental(o->closed,o->nsc,dim,v)) RESULT('o');
      else RESULT(check(o));
    }
    else  RESULT('o');
    avo_free(mo,o);
  } ENDLOOP;
}

/* ********************************* */
/*            conversions            */
/* ********************************* */

void test_box_conversion(void)
{
  printf("\nconversion to box %s\n",num_incomplete?"":"(* expected)");
  LOOP {
    size_t dim = 2; //8;
    int i;
    avo_t *o,*o2,*o3;
    ap_interval_t **b,**b2;
    o = random_avo(dim,.1);
    b = avo_to_box(mo,o); FLAG(mo);
    o2 = avo_of_box(mo,0,dim,b); FLAG(mo);
    b2 = avo_to_box(mo,o2); FLAG(mo);
    o3 = avo_of_box(mo,0,dim,b2); FLAG(mo);
    RESULT(check(o));    check(o2);
    if (avo_is_nleq(mo,o,o2) || avo_is_nleq(mo,o2,o3)) {
      ERROR("not included in"); /*
      print_avo("o",o);
      print_avo("o2",o2);
      print_avo("o3",o3); */
    }

    if (avo_is_eq(mo,o2,o3)) RESULT('*');
    if (flag==exact && avo_is_neq(mo,o2,o3)) ERROR("exact flag");
    ap_interval_array_free(b,dim);
    ap_interval_array_free(b2,dim);
    avo_free(mo,o); avo_free(mo,o2); avo_free(mo,o3);
  } ENDLOOP;
}


/* ********************************* */
/*            serialization          */
/* ********************************* */

void test_serialize(void)
{
  printf("\nserialization %s\n","(* expected)");
  LOOP {
    size_t sz;
    avo_t *o, *o2;
    ap_membuf_t b;
    o  = random_avo(6,.1);//(10,.1);
    b  = avo_serialize_raw(mo,o); FLAG(mo);
    o2 = avo_deserialize_raw(mo,b.ptr,&sz); FLAG(mo);
    RESULT(check(o)); check(o2);
    if (avo_is_nleq(mo,o,o2)) {
      ERROR("not included in"); /*
      print_avo("o",o);
      print_avo("o2",o2); */
    }
    if (avo_is_eq(mo,o,o2)) RESULT('*');
    if (flag==exact && avo_is_neq(mo,o,o2)) ERROR("exact flag");
    if (b.size!=sz) ERROR("different size");
    avo_free(mo,o); avo_free(mo,o2); free(b.ptr);
  } ENDLOOP;
}

/* ********************************* */
/*     dimension manipulation        */
/* ********************************* */

void test_dimadd(void)
{
  printf("\nadd dimensions\n");
  LOOP {
    size_t i, dim = 15;
    avo_t *o1, *o2, *o3;
    ap_dimchange_t* a = ap_dimchange_alloc(0,3);
    ap_dimchange_t* r = ap_dimchange_alloc(0,a->realdim);
    o1 = random_avo(dim,.1);
    //if (lrand48()%10>=8)
    avo_close(pr,o1);
    if(!o1->m && !o1->closed) { RESULT('o'); continue; } /* empty avo after avo_close() */
    for (i=0;i<a->realdim;i++) {
      a->dim[i] = lrand48()%3;
      if (i) a->dim[i] += a->dim[i-1];
      r->dim[i] = a->dim[i]+i;
    }
    o2 = avo_add_dimensions(mo,false,o1,a,true);
    o3 = avo_remove_dimensions(mo,false,o2,r);
    RESULT(check(o1)); check(o2); check(o3);
    if (avo_is_neq(mo,o1,o3)) {
      ERROR("not eq");
      ap_dimchange_fprint(stderr,a); ap_dimchange_fprint(stderr,r);
      print_avo("o1",o1); print_avo("o2",o2);print_avo("o3",o3);
    }
    avo_free(mo,o1); avo_free(mo,o2); avo_free(mo,o3);
    ap_dimchange_free(a); ap_dimchange_free(r);
  } ENDLOOP;
}

void test_dimrem(void)
{
  printf("\nremove dimensions, forget\n");
  LOOP {
    size_t i, dim = 15;
    avo_t *o1, *o2, *o3 ,*o4;
    ap_dimchange_t* a = ap_dimchange_alloc(0,3);
    ap_dimchange_t* r = ap_dimchange_alloc(0,a->realdim);
    o1 = random_avo(dim,.01);
    if (lrand48()%10>=8) avo_close(pr,o1);
    if(!o1->m && !o1->closed) { RESULT('o'); continue; } // empty avo after avo_close()
    for (i=0;i<r->realdim;i++) {
      r->dim[i] = lrand48()%3 + 1;
      if (i) r->dim[i] += r->dim[i-1];
      a->dim[i] = r->dim[i]-i;
    }
    o2 = avo_remove_dimensions(mo,false,o1,r);
    o3 = avo_add_dimensions(mo,false,o2,a,false);
    o4 = avo_forget_array(mo,false,o1,r->dim,r->realdim,false);
    RESULT(check(o1)); check(o2); check(o3); check(o4);
    if (avo_is_neq(mo,o3,o4)) {
      ERROR("not eq");
      ap_dimchange_fprint(stderr,r); ap_dimchange_fprint(stderr,a);/*
      print_avo("o1",o1); print_avo("o2",o2);
      print_avo("o3",o3); print_avo("o4",o4); */
    }
    avo_free(mo,o1); avo_free(mo,o2); avo_free(mo,o3); avo_free(mo,o4);
    ap_dimchange_free(a); ap_dimchange_free(r);
  } ENDLOOP;
}


void test_permute(void)
{
  printf("\npermute dimensions\n");
  LOOP {
    size_t i, dim = 14;
    avo_t *o1, *o2, *o3;
    ap_dimperm_t* p = ap_dimperm_alloc(dim);
    ap_dimperm_t* q = ap_dimperm_alloc(dim);
    o1 = random_avo(dim,.1);
    if (lrand48()%10>=8) avo_close(pr,o1);
    if(!o1->m && !o1->closed) { RESULT('o'); continue; } /* empty avo after avo_close() */

    /* random permutation */
    ap_dimperm_set_id(p);
    for (i=0;i<p->size-1;i++) {
      int j = i + (lrand48() % (p->size-i));
      int a = p->dim[j]; p->dim[j] = p->dim[i]; p->dim[i] = a;
    }
    /* inverse permutation */
    for (i=0;i<p->size;i++) q->dim[p->dim[i]] = i;

    o2 = avo_permute_dimensions(mo,false,o1,p);
    o3 = avo_permute_dimensions(mo,false,o2,q);
    RESULT(check(o1)); check(o2); check(o3);
    if (avo_is_neq(mo,o1,o3)) {
      ERROR("not eq");
      ap_dimperm_fprint(stderr,p); ap_dimperm_fprint(stderr,q);
      //print_avo("o1",o1); print_avo("o2",o2); print_avo("o3",o3);
    }
    avo_free(mo,o1); avo_free(mo,o2); avo_free(mo,o3);
    ap_dimperm_free(p); ap_dimperm_free(q);
  } ENDLOOP;
}

/* ********************************* */
/*             widening              */
/* ********************************* */

void test_widening(void)
{
  printf("\nwidening\n");
  LOOP {
    int dim = 8;
    avo_t *o1, *o2, *o;
    o1 = random_avo(dim,.1);
    o2 = random_avo(dim,.1);
    o  = avo_widening(mo,o1,o2);
    RESULT(check(o));
    if (avo_is_nleq(mo,o1,o) || avo_is_nleq(mo,o2,o)) {
      ERROR("not upper bound");
      //print_avo("o1",o1); print_avo("o2",o2); print_avo("o",o);
    }
    avo_free(mo,o1); avo_free(mo,o2); avo_free(mo,o);
  } ENDLOOP;
  printf("\nwidening convergence\n");
  LOOP {
    int dim = 5, nb = (4*dim)*(4*dim);
    avo_t *o1 = random_avo(dim,.1);
    for (;nb>0;nb--) {
      avo_t* o2 = random_avo(dim,.1);
      avo_t* o = avo_widening(mo,o1,o2);
      avo_free(mo,o2);
      if (avo_is_leq(mo,o,o1)) { avo_free(mo,o); break; }
      avo_free(mo,o1); o1 = o;
    }
    RESULT(check(o1));
    avo_free(mo,o1);
    if (!nb) ERROR("not converging in reasonable time");
  } ENDLOOP;
}

void test_widening_thrs(void)
{
  printf("\nwidening with thresholds\n");
  LOOP {
    int dim = 8, n;
    avo_t *o1, *o2, *o;
    o1 = random_avo(dim,.1);
    o2 = random_avo(dim,.1);
    ap_scalar_t* t[10];
    for (n=0;n<10;n++) t[n]=ap_scalar_alloc_set_double((lrand48()%30-15)*0.25);
    o = avo_widening_thresholds(mo,o1,o2,t,10);
    RESULT(check(o));
    if (avo_is_nleq(mo,o1,o) || avo_is_nleq(mo,o2,o)) {
      ERROR("not upper bound");
      //print_avo("o1",o1); print_avo("o2",o2); print_avo("o",o);
    }
    avo_free(mo,o1); avo_free(mo,o2); avo_free(mo,o);
    for (n=0;n<10;n++) ap_scalar_free(t[n]);
  } ENDLOOP;
  printf("\nwidening with thresholds convergence\n");
  LOOP {
    int dim = 5, nb = 4*dim*dim*10, n;
    avo_t *o1 = random_avo(dim,.1);
    ap_scalar_t* t[10];
    for (n=0;n<10;n++) t[n]=ap_scalar_alloc_set_double((lrand48()%30-15)*0.25);
    for (;nb>0;nb--) {
      avo_t* o2 = random_avo(dim,.1);
      avo_t* o = avo_widening_thresholds(mo,o1,o2,t,10);
      avo_free(mo,o2);
      if (avo_is_leq(mo,o,o1)) { avo_free(mo,o); break; }
      avo_free(mo,o1); o1 = o;
    }
    RESULT(check(o1));
    avo_free(mo,o1);
    for (n=0;n<10;n++) ap_scalar_free(t[n]);
    if (!nb) ERROR("not converging in reasonable time");
  } ENDLOOP;
}


/* ********************************* */
/*            narrowing              */
/* ********************************* */

void test_narrowing(void)
{
  printf("\nnarrowing\n");
  LOOP {
    int dim = 8;
    avo_t *o1, *o2, *o, *oo;
    o1 = random_avo(dim,.01);
    o2 = random_avo(dim,.01);
    o  = avo_narrowing(mo,o1,o2);
    oo = avo_meet(mo,false,o1,o2);
    RESULT(check(o));
    if (avo_is_nleq(mo,o,o1) || avo_is_nleq(mo,oo,o)) {
      ERROR("not decreasing");/*
      print_avo("o1",o1); print_avo("o2",o2);
      print_avo("o",o); print_avo("oo",oo);*/
    }
    avo_free(mo,o1); avo_free(mo,o2); avo_free(mo,o); avo_free(mo,oo);
  } ENDLOOP;
  printf("\nwidening narrowing\n");
  LOOP {
    int dim = 5, nb = (4*dim)*(4*dim);
    avo_t *o1 = random_avo(dim,.03);
    for (;nb>0;nb--) {
      avo_t* o2 = random_avo(dim,.03);
      avo_t* o = avo_narrowing(mo,o1,o2);
      avo_free(mo,o2);
      if (avo_is_leq(mo,o,o1)) { avo_free(mo,o); break; }
      avo_free(mo,o1); o1 = o;
    }
    RESULT(check(o1));
    avo_free(mo,o1);
    if (!nb) ERROR("not converging in reasonable time");
  } ENDLOOP;
}

/* ********************************* */
/*     assignments / substitutions   */
/* ********************************* */

void test_par_assign2(int subst, exprmode mode)
{
  printf("\nparallel vs non-parallel %s %slinexpr\n",
	 subst ? "subst" : "assign",exprname[mode]);
  LOOP {
    size_t dim = 6;
    ap_dim_t d = lrand48()%dim;
    avo_t *o, *o1, *o2;
    ap_linexpr0_t* l = random_linexpr(mode,dim);
    o = random_avo(dim,.1);
    if (lrand48()%10>=8) avo_close(pr,o);
    if(!o->m && !o->closed) { RESULT('o'); avo_free(mo,o); continue; } /* empty avo after avo_close() */
    o1 = subst ?
    		avo_substitute_linexpr_array(mo,false,o,&d,&l,1,NULL) :
    		avo_assign_linexpr_array(mo,false,o,&d,&l,1,NULL);
    FLAG(mo);
    o2 = subst ? 
    		avo_substitute_linexpr_array(mo,false,o,&d,&l,1,NULL) :
    		avo_assign_linexpr_array(mo,false,o,&d,&l,1,NULL);
    FLAG(mo);
    check(o1); check(o2);
    if (avo_is_eq(mo,o1,o2)) RESULT('=');
    else if (avo_is_leq(mo,o1,o2)) RESULT('<');
    else if (avo_is_leq(mo,o2,o1)) RESULT('>');
    else RESULT('.');
    if (flag==exact && avo_is_neq(mo,o1,o2)) ERROR("exact flag");
    avo_free(mo,o); avo_free(mo,o1); avo_free(mo,o2);
    ap_linexpr0_free(l);
  } ENDLOOP;
}

/* ********************************* */
/*           main                    */
/* ********************************* */

void tests(int algo)
{
  int i;
  for (i=0;i<AP_FUNID_SIZE;i++) {
    mo->option.funopt[i].algorithm = algo;
  }
  printf("\nstarting tests with algo=%i\n",algo);
  /* tests */
  test_misc();
  test_serialize();
  test_closure();
  test_incremental_closure();
  test_box_conversion();
  test_dimadd();
  test_dimrem();
  test_permute();
  test_widening();
  test_widening_thrs();
  test_narrowing();
  test_par_assign2(0,expr_unary);
  test_par_assign2(0,expr_lin);
  test_par_assign2(0,expr_interv);
  test_par_assign2(1,expr_unary);
  test_par_assign2(1,expr_lin);
  test_par_assign2(1,expr_interv);
}

int main(int argc, const char** argv)
{
  long int seed;
  int i;

  seed = time(NULL);
  for (i=1;i<argc;i++) {
    if (argv[i][0]=='+') N = atol(argv[i]+1);
    else seed = atol(argv[i]);
  }
  printf("seed = %ld, N= %i\n", seed, N);

  assert(N<MAXN);

  /* init */
  srand48(seed);
  mo = avo_manager_alloc();
  if (!mo) return 1;
  for (i=0;i<AP_FUNID_SIZE;i++) {
    mo->option.funopt[i].flag_exact_wanted = true;
    mo->option.funopt[i].flag_best_wanted = true;
  }
  for (i=0;i<AP_EXC_SIZE;i++){
    mo->option.abort_if_exception[i] = true;
  }
  pr = avo_init_from_manager(mo,0,0);
  info();

  tests(0);
  tests(-1);

  /* quit */
  ap_manager_free(mo);
  if (error_) printf("\n%i error(s)!\n",error_);
  else printf("\nall tests passed\n");
  return 0;
}
