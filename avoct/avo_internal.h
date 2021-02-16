/*
 * avo_internal.h
 *
 * Private definitions, access to internal structures and algorithms.
 * Use with care.
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

#ifndef __AVO_INTERNAL_H
#define __AVO_INTERNAL_H

#include "num.h"
#include "bound.h"
#include "avo_fun.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int abs_print_hint_flag; 
  /* print 11x to denote abs(x), i.e., |x| */
extern int avo_closure_alg;
/* 0 (default): WeakCloVia1Sign(i.e., enumerating the signs of 1 variables each time) */
/* 1: WeakCloVia3Sign(i.e., enumerating the signs of 3 variables each time)           */
/* 2: StrongCloViaAllSigns(i.e., enumerating the signs of all variables each time)    */

/* ********************************************************************** */
/* I. Manager */
/* ********************************************************************** */

/* manager-local data specific to avOctagons */
struct _avo_internal_t {

  /* current function */
  ap_funid_t funid;

  /* local parameters for current function */
  ap_funopt_t* funopt;

  /* growing temporary buffer */
  bound_t* tmp;
  void* tmp2;
  size_t tmp_size;

  /* raised when a conversion from/to a user type resulted in an 
     overapproximation 
  */
  bool conv;

  /* back-pointer */
  ap_manager_t* man;
};


/* called by each function to setup and get manager-local data */
static inline avo_internal_t*
avo_init_from_manager(ap_manager_t* man, ap_funid_t id, size_t size)
{
  avo_internal_t* pr = (avo_internal_t*) man->internal;
  pr->funid = id;
  pr->funopt = man->option.funopt+id;
  man->result.flag_exact = man->result.flag_best = true;
  pr->conv = false;
  if (pr->tmp_size<size) {
    bound_clear_array(pr->tmp,pr->tmp_size);
    pr->tmp = (bound_t*)realloc(pr->tmp,sizeof(bound_t)*size);
    assert(pr->tmp);
    pr->tmp_size = size;
    bound_init_array(pr->tmp,pr->tmp_size);
    pr->tmp2 = realloc(pr->tmp2,sizeof(long)*size);
    assert(pr->tmp2);
  }
  return pr;
}



/* loss of precision can be due to one of the following
   1) the algorithm is incomplete or
      the algorithm is incomplete on Z and we have intdim > 0
      or the numerical type induces overapproximation (NUMINT or NUMFLOAT)
      => no solution at run-time, you need to recompile the library 
         with another NUM base type
   2) the user disabled closure (algorithm<0)
      => solution: raise algorithm
   3) approximation in the conversion from / to user type
      => use another user type
 */

#define flag_incomplete						\
  man->result.flag_exact = man->result.flag_best = false

#define flag_algo flag_incomplete

#define flag_conv flag_incomplete


  /* invalid argument exception */
#define arg_assert(cond,action)						\
  do { if (!(cond)) {							\
      char buf_[1024];							\
      snprintf(buf_,sizeof(buf_),					\
	       "assertion (%s) failed in %s at %s:%i",			\
	       #cond, __func__, __FILE__, __LINE__);		\
      ap_manager_raise_exception(pr->man,AP_EXC_INVALID_ARGUMENT,	\
				 pr->funid,buf_);			\
      action }								\
  } while(0)

  /* malloc with safe-guard */
#define checked_malloc(ptr,t,nb,action)					\
  do {									\
    (ptr) = (t*)malloc(sizeof(t)*(nb));					\
    if (!(ptr)) {							\
      char buf_[1024];							\
      snprintf(buf_,sizeof(buf_),					\
	       "cannot allocate %s[%lu] for %s in %s at %s:%i",		\
	       #t, (long unsigned)(nb), #ptr,				\
	       __func__, __FILE__, __LINE__);				\
      ap_manager_raise_exception(pr->man,AP_EXC_OUT_OF_SPACE,		\
				 pr->funid,buf_);			\
      action }								\
  } while(0)


/* ********************************************************************** */
/* II. Half-matrices */
/* ********************************************************************** */

/* ============================================================ */
/* II.1 Basic Management */
/* ============================================================ */

/* see avo_hmat.c */

bound_t* avo_hmat_alloc       (avo_internal_t* pr, size_t dim);
void     avo_hmat_free        (avo_internal_t* pr, bound_t* m, size_t dim);
bound_t* avo_hmat_alloc_zero  (avo_internal_t* pr, size_t dim);
bound_t* avo_hmat_alloc_top   (avo_internal_t* pr, size_t dim);
bound_t* avo_hmat_alloc_top_nsc   (avo_internal_t* pr, size_t dim);
bound_t* avo_hmat_copy        (avo_internal_t* pr, bound_t* m, size_t dim);
void     avo_hmat_fdump       (FILE* stream, bound_t* m, size_t dim);



//* ============================================================ */
/* II.2 Access */
/* ============================================================ */

static inline size_t avo_matsize(size_t dim)
{
  return 2 * dim * (4*dim+2);
}
static inline size_t avo_octmatsize(size_t dim)
{
  return dim * (2*dim+2);
}
/* position of (i,j) element, assuming j/2 <= i/2 */
static inline size_t avo_matpos(size_t i, size_t j)
{
  return j + ((i+1)*(i+1))/2;
}

/* position of (i,j) element, no assumption */
static inline size_t avo_matpos2(size_t i, size_t j)
{
  if (j>i) return avo_matpos(j^1,i^1);
  else return avo_matpos(i,j);
}


/* ============================================================ */
/* II.3 Closure Algorithms */
/* ============================================================ */

/* see avo_closure.c */
bool avo_hmat_s_step(bound_t* m, bound_t* nsc, size_t dim);
bool avo_hmat_close(bound_t* m, bound_t* nsc, size_t dim);
bool avo_hmat_close_incremental(bound_t* m, bound_t* nsc, size_t dim, size_t v);
bool avo_hmat_check_closed(bound_t* m,bound_t* nsc, size_t dim);


/* ============================================================ */
/* II.4  Constraints and generators */
/* ============================================================ */

/* see avo_transfer.c */
  
bool avo_hmat_add_lincons(avo_internal_t* pr, bound_t* b, bound_t * nsc,size_t dim,
		      ap_lincons0_array_t* ar, bool* exact,
		      bool* respect_closure);

void avo_hmat_add_generators(avo_internal_t* pr, bound_t* b, size_t dim,
			 ap_generator0_array_t* ar);


/* ============================================================ */
/* II.5 Resze */
/* ============================================================ */

/* see avo_reize.c */

void avo_hmat_addrem_dimensions(bound_t* dst, bound_t* src,
			    ap_dim_t* pos, size_t nb_pos,
			    size_t mult, size_t dim, bool add);

void avo_hmat_permute(bound_t* dst, bound_t* src,
		  size_t dst_dim, size_t src_dim,
		  ap_dim_t* permutation);

/* ********************************************************************** */
/* III. Numbers */
/* ********************************************************************** */

/* To perform soundly, we suppose that all conversions beteween num and
   base types (double, int, mpz, mpq, etc.) always over-approximate the
   result (as long as the fits function returns true).
 */

static inline void bound_bmin(bound_t dst, bound_t arg)
{ bound_min(dst,dst,arg); }

static inline void bound_badd(bound_t dst, bound_t arg)
{ bound_add(dst,dst,arg); }


/* ============================================================ */
/* III.1 Properties on num_t */
/* ============================================================ */

/*
  num_name              unique type description
  num_incomplete        does the type make algorithms incomplete
  num_safe              is the type safe in case of overflow
  num_fpu               num requires init_fpu to be called
  num_export_double     constraints are output using double instead of mpq
*/


#if   defined ( NUM_LONGINT )
/* overflows produce unsound results, type not closed by / 2 */
#define num_name          "NUM_LONGINT"
#define num_incomplete    1    
#define num_safe          0
#define num_fpu              0
#define num_export_double    0
#elif defined ( NUM_LONGLONGINT )
#define num_name             "NUM_LONGLONGINT"
#define num_incomplete       1
#define num_safe             0
#define num_fpu              0
#define num_export_double    0

#elif defined ( NUM_MPZ )
/* no overflow, type not closed by / 2 */
#define num_name             "NUM_MPZ"
#define num_incomplete       1
#define num_safe             1
#define num_fpu              0
#define num_export_double    0

#elif defined ( NUM_LONGRAT )
/* complete algorithms, but overflows produce unsound results */
#define num_name             "NUM_LONGRAT"
#define num_incomplete       0
#define num_safe             0
#define num_fpu              0
#define num_export_double    0
#elif defined ( NUM_LONGLONGRAT )
#define num_name             "NUM_LONGLONGRAT"
#define num_incomplete       0
#define num_safe             0
#define num_fpu              0
#define num_export_double    0

#elif defined ( NUM_MPQ )
/* the "perfect" type */
#define num_name             "NUM_MPQ"
#define num_incomplete       0
#define num_safe             1
#define num_fpu              0
#define num_export_double    0

#elif defined ( NUM_DOUBLE )
/* overflow are ok (stick to +oo), type not closed by + and / 2 */
#define num_name            "NUM_DOUBLE"
#define num_incomplete       1
#define num_safe             1
#define num_fpu              1
#define num_export_double    1
#elif defined ( NUM_LONGDOUBLE )
#define num_name             "NUM_LONGDOUBLE"
#define num_incomplete       1
#define num_safe             1
#define num_fpu              1
#define num_export_double    1
#elif defined ( NUM_MPFR )
#define num_name             "NUM_MPFR"
#define num_incomplete       1
#define num_safe             1
#define num_fpu              1
#define num_export_double    1

/* duh */
#else
#error "No numerical type defined"
#endif


/* ============================================================ */
/* III.2 Conversions from user types */
/* ============================================================ */

/* sound conversion from a scalar to a bound_t
   optional negation and multiplication by 2
   if negation, lower approximation, otherwise, upper approximation 
   pr->conv is set if the conversion is not exact
 */
static inline void avo_bound_of_scalar(avo_internal_t* pr,
				   bound_t r, ap_scalar_t* t,
				   bool neg, bool mul2)
{
  switch (t->discr) {
  case AP_SCALAR_DOUBLE:
    {
      double d = t->val.dbl;
      if (neg) d = -d;
      if (mul2) d *= 2;
      if (d==1/0.) bound_set_infty(r,1);
      else if (double_fits_num(d)) { 
	if (!num_set_double(bound_numref(r),d) ||  mul2) pr->conv = true;
	bound_set_num(r,bound_numref(r));
      }
      else { pr->conv = true; bound_set_infty(r,1); }
    }
    break;
  case AP_SCALAR_MPQ:
    /* negate _before_ converting */
    if (neg) mpq_neg(t->val.mpq,t->val.mpq);
    if (ap_scalar_infty(t)==1) bound_set_infty(r,1);
    else if (mpq_fits_num(t->val.mpq)) {
      if (!num_set_mpq(bound_numref(r),t->val.mpq))  pr->conv = true;
      bound_set_num(r,bound_numref(r));
      if (mul2) bound_mul_2(r,r);
    }
    else { pr->conv = true; bound_set_infty(r,1); }
    /* undo change in argument */
    if (neg) mpq_neg(t->val.mpq,t->val.mpq);
    break;
  default: arg_assert(0,return;);
  }
}


/* both bounds of an interval, the lower bound is negated 
   pr->conv is set if the conversion is not exact
   returns true if the interval is empty
*/
static inline bool avo_bounds_of_interval(avo_internal_t* pr,
				      bound_t minf, bound_t sup,
				      ap_interval_t* i,
				      bool mul2)
{
  avo_bound_of_scalar(pr,minf,i->inf,true,mul2);
  avo_bound_of_scalar(pr,sup,i->sup,false,mul2);
  return ap_scalar_cmp(i->inf,i->sup)>0;
}

/* as above, for a coeff_t */
static inline bool avo_bounds_of_coeff(avo_internal_t* pr,
				   bound_t minf, bound_t sup,
				   ap_coeff_t c,
				   bool mul2)
{
  switch (c.discr) {
  case AP_COEFF_SCALAR:
    avo_bound_of_scalar(pr,minf,c.val.scalar,true,mul2);
    avo_bound_of_scalar(pr,sup,c.val.scalar,false,mul2);
    return false;
  case AP_COEFF_INTERVAL:
    avo_bound_of_scalar(pr,minf,c.val.interval->inf,true,mul2);
    avo_bound_of_scalar(pr,sup,c.val.interval->sup,false,mul2);
    return ap_scalar_cmp(c.val.interval->inf,c.val.interval->sup)>0;
  default: arg_assert(0,return false;);
  }
}

static void avo_bounds_of_generator(avo_internal_t* pr, bound_t* dst,
				ap_linexpr0_t* e, size_t dim)
{
  size_t i;
  switch (e->discr) {
  case AP_LINEXPR_DENSE:
    arg_assert(e->size<=dim,return;);
    for (i=0;i<e->size;i++) {
      avo_bounds_of_coeff(pr,dst[2*i],dst[2*i+1],e->p.coeff[i],false);
    }
    for (;i<dim;i++) {
      bound_set_int(dst[2*i],0);
      bound_set_int(dst[2*i+1],0);
    }
    break;
  case AP_LINEXPR_SPARSE:
    for (i=0;i<dim;i++) {
      bound_set_int(dst[2*i],0);
      bound_set_int(dst[2*i+1],0);
    }
    for (i=0;i<e->size;i++) {
      size_t d = e->p.linterm[i].dim;
      arg_assert(d<dim,return;);
      avo_bounds_of_coeff(pr,dst[2*d],dst[2*d+1],e->p.linterm[i].coeff,false);
    }
    break;
  default: arg_assert(0,return;);
  }
}

/* ============================================================ */
/* III.3 Conversions to user types */
/* ============================================================ */

/* upper bound => scalar, with optional division by 2
   pr->conv is set if the conversion is not exact
*/
static inline void avo_scalar_of_upper_bound(avo_internal_t* pr,
					 ap_scalar_t* r,
					 bound_t b,
					 bool div2)
{
  if (bound_infty(b)) ap_scalar_set_infty(r,1);
  else {
#if num_export_double
    /* use double */
    ap_scalar_reinit(r,AP_SCALAR_DOUBLE);
    if (!double_set_num(&r->val.dbl,bound_numref(b)) || div2) pr->conv = 1;
    if (div2) r->val.dbl /= 2;
#else
    /* use mpq */
    ap_scalar_reinit(r,AP_SCALAR_MPQ);
    if (!mpq_set_num(r->val.mpq,bound_numref(b)) || div2) pr->conv = 1;
    if (div2) mpq_div_2exp(r->val.mpq,r->val.mpq,1);
#endif
  }
}

/* opposite of lower bound => scalar, with optional division by 2
   pr->conv is set if the conversion is not exact  
*/
static inline void avo_scalar_of_lower_bound(avo_internal_t* pr,
					 ap_scalar_t* r,
					 bound_t b,
					 bool div2)
{
  if (bound_infty(b)) ap_scalar_set_infty(r,-1);
  else {
#if num_export_double
    /* use double */
    ap_scalar_reinit(r,AP_SCALAR_DOUBLE);
    if (!double_set_num(&r->val.dbl,bound_numref(b)) || div2) pr->conv = 1;
    if (div2) r->val.dbl /= 2;
    r->val.dbl = -r->val.dbl;
#else
    /* use mpq */
    ap_scalar_reinit(r,AP_SCALAR_MPQ);
    if (!mpq_set_num(r->val.mpq,bound_numref(b)) || div2)  pr->conv = 1;
    if (div2) mpq_div_2exp(r->val.mpq,r->val.mpq,1);
    mpq_neg(r->val.mpq,r->val.mpq);
#endif
  }
}


/* makes an interval from [-minf,sup], with sound approximations
   pr->conv is set if the conversion is not exact
   note: may output an empty interval
*/
static inline void avo_interval_of_bounds(avo_internal_t* pr,
				      ap_interval_t* i,
				      bound_t minf, bound_t sup,
				      bool div2)
{
  avo_scalar_of_upper_bound(pr,i->sup, sup,div2);
  avo_scalar_of_lower_bound(pr,i->inf,minf,div2);
}


/* ============================================================ */
/* III.4 Bound manipulations */
/* ============================================================ */

/* [-r_inf,r_sup] = [-a_inf,a_sup] * [-b_inf,b_sup]
   where 0 * oo = oo * 0 = 0
 */
static inline void bounds_mul(bound_t r_inf, bound_t r_sup,
			      bound_t a_inf, bound_t a_sup,
			      bound_t b_inf, bound_t b_sup,
			      bound_t tmp[8])
{
  bound_mul(tmp[0],a_sup,b_sup);
  bound_neg(tmp[4],a_sup); bound_mul(tmp[4],tmp[4],b_sup);

  bound_mul(tmp[1],a_inf,b_inf);
  bound_neg(tmp[5],a_inf);  bound_mul(tmp[5],tmp[5],b_inf);

  bound_mul(tmp[6],a_sup,b_inf);
  bound_neg(tmp[2],a_sup);  bound_mul(tmp[2],tmp[2],b_inf);

  bound_mul(tmp[7],a_inf,b_sup);
  bound_neg(tmp[3],a_inf);  bound_mul(tmp[3],tmp[3],b_sup);

  bound_max(r_sup,tmp[0],tmp[1]);
  bound_max(r_sup,r_sup,tmp[2]);
  bound_max(r_sup,r_sup,tmp[3]);

  bound_max(r_inf,tmp[4],tmp[5]);
  bound_max(r_inf,r_inf,tmp[6]);
  bound_max(r_inf,r_inf,tmp[7]);
}

/* ========================================================================== */
/* Operations on nsc (not neccesarily closed, i.e., possibly <= or < ) bounds */
/* ========================================================================== */

static inline void bound_add_nsc(bound_t *des_m, bound_t *des_nsc, bound_t m1, bound_t m2, bound_t nsc1, bound_t nsc2)
{
	bound_t temp, nsc_temp;
	bound_init(temp); bound_init(nsc_temp);

	bound_add(temp,m1,m2);
	if(bound_infty(temp)==true)
		 bound_set_infty(nsc_temp,1);
	else bound_min(nsc_temp,nsc1,nsc2);

    int order = bound_cmp(temp,*des_m);
	if(order<0 || (order==0 && bound_cmp(nsc_temp,*des_nsc)<0)){
		 bound_set(*des_m,temp);
		 bound_set(*des_nsc,nsc_temp);
	}

	bound_clear(temp); bound_clear(nsc_temp);
}

static inline void bound_bmin_nsc(bound_t *des_m, bound_t *des_nsc, bound_t m1, bound_t nsc1)
{
	if(bound_infty(m1)==true) return;
	int order =  bound_cmp(m1,*des_m);
	if(order < 0 || (order==0 && bound_cmp(nsc1,*des_nsc)<0)){
		bound_set(*des_m,m1);
		bound_set(*des_nsc,nsc1);
	}
}

static inline void bound_bmax_nsc(bound_t *des_m, bound_t *des_nsc, bound_t m1, bound_t nsc1)
{
	int order =  bound_cmp(*des_m,m1);
	if(order < 0 || (order==0 && bound_cmp(*des_nsc,nsc1)<0)){
		bound_set(*des_m,m1);
		bound_set(*des_nsc,nsc1);
	}
}

/* ============================================================ */
/* III.5 Conversion to constraints */
/* ============================================================ */

/* constraint at line i, column j, with upper bound m */
static inline ap_lincons0_t avo_lincons_of_bound(avo_internal_t* pr,
					     size_t i, size_t j, 
					     bound_t m,bound_t m1,size_t dim)
{
  if (abs_print_hint_flag==0){
	fprintf(stdout, "/* Note that we use 11x to denote abs(x), i.e., |x| */\n"); 
	abs_print_hint_flag=1;
  }
  ap_linexpr0_t* e;
  if (i==j) {
    /* zeroary constraint */
    e = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, 0);
    avo_scalar_of_upper_bound(pr,e->cst.val.scalar,m,true);
  }
  else if (i==(j^1)) {
    /* unary constraint */
    e = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, 1);

    if(i < 2 * dim )
    	ap_scalar_set_int(e->p.linterm[0].coeff.val.scalar,(i&1) ? -1 : 1);
    else
    {
    	ap_scalar_set_int(e->p.linterm[0].coeff.val.scalar,(i&1) ? -11 : 11); /* 11x to denote abs(x)*/
    	i = i - 2 * dim;
    }

    e->p.linterm[0].dim = i/2;
    avo_scalar_of_upper_bound(pr,e->cst.val.scalar,m,true);
  }
  else {
    /* binary constraint */
    e = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, 2);

    if(j < 2 * dim)
    	ap_scalar_set_int(e->p.linterm[0].coeff.val.scalar,(j&1) ?  1 : -1);
    else
    {
    	ap_scalar_set_int(e->p.linterm[0].coeff.val.scalar,(j&1) ?  11 : -11); /* 11x to denote abs(x)*/
    	j = j - 2 * dim;
    }
    if(i < 2 * dim)
    	ap_scalar_set_int(e->p.linterm[1].coeff.val.scalar,(i&1) ? -1 :  1);
    else
    {
    	ap_scalar_set_int(e->p.linterm[1].coeff.val.scalar,(i&1) ? -11 :  11); /* 11x to denote abs(x)*/
    	i = i - 2 * dim;
    }
    e->p.linterm[0].dim = j/2;
       e->p.linterm[1].dim = i/2;
    avo_scalar_of_upper_bound(pr,e->cst.val.scalar,m,false);
  }
  if(bound_cmp_int(m1,1) > 0)  return ap_lincons0_make(AP_CONS_SUPEQ,e,NULL);
  else	  return ap_lincons0_make(AP_CONS_SUP,e,NULL);
}

/* ============================================================ */
/* III.5 Expression classification */
/* ============================================================ */

/* see avo_transfer.c */

typedef struct {
  enum { 
    EMPTY,    /* empty domain */
    ZERO,     /* 0 */
    UNARY,    /* unary unit expression */
    BINARY,   /* binary unit expression */
    OTHER,
  } type;

  /* index and coefficient for unary / binary unit expressions */
  size_t i,j;
  int coef_i,coef_j; /* -1 or 1 */

} uexpr;

/* convert expression to bounds, look for unit unary or binary form */
uexpr avo_uexpr_of_linexpr(avo_internal_t* pr, bound_t* dst,
			   ap_linexpr0_t* e, size_t dim);


/* ********************************************************************** */
/* IV. avOctagons */
/* ********************************************************************** */


/* ============================================================ */
/* IV.1 Internal Representation */
/* ============================================================ */

struct _avo_t {
  bound_t* m;      /* contraint half-matrix (or NULL) */
  bound_t* closed; /* closed version of m (or NULL for not available) */
  bound_t* nsc;    /* half-matrix describing not necessarily strictness (closeness), e.g., x -y < c */
  	  	  	  	   /* TODO: replaced as (char* nsc)  */
  size_t dim;      /* total number of variables */
  size_t intdim;   /* the first intdim variables are integer ones */
};

/* several cases are possible
   m==NULL closed==NULL -- definitively empty avOctagon
   m!=NULL closed==NULL -- empty or non-empty avOctagon, closure not available
   m==NULL closed!=NULL \_ definitively non-empty avOctagon, closure available
   m!=NULL closed!=NULL /
*/


/* ============================================================ */
/* IV.2 Management */
/* ============================================================ */

avo_t* avo_alloc_internal (avo_internal_t* pr, size_t dim, size_t intdim);
void   avo_free_internal  (avo_internal_t* pr, avo_t* o);
avo_t* avo_copy_internal  (avo_internal_t* pr, avo_t* o);
void   avo_cache_closure  (avo_internal_t* pr, avo_t* a);
void   avo_close          (avo_internal_t* pr, avo_t* a);
avo_t* avo_set_mat_nsc        (avo_internal_t* pr, avo_t* a, bound_t* m,
			   bound_t* closed,bound_t * nsc, bool destructive);
avo_t* avo_alloc_top      (avo_internal_t* pr, size_t dim, size_t intdim);

#ifdef __cplusplus
}
#endif

#endif /* __avo_INTERNAL_H */
