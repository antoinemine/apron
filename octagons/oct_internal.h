/*
 * oct_internal.h
 *
 * Private definitions, access to internal structures and algorithms.
 * Use with care.
 *
 * APRON Library / Octagonal Domain
 *
 * Copyright (C) Antoine Mine' 2006
 *
 */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution.
*/

#ifndef __OCT_INTERNAL_H
#define __OCT_INTERNAL_H

#include "oct_fun.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* I. Manager */
/* ********************************************************************** */

/* manager-local data specific to octagons */
struct _oct_internal_t {

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
static inline oct_internal_t*
oct_init_from_manager(ap_manager_t* man, ap_funid_t id, size_t size)
{
  oct_internal_t* pr = (oct_internal_t*) man->internal;
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

/* see oct_hmat.c */

bound_t* hmat_alloc       (oct_internal_t* pr, size_t dim);
void     hmat_free        (oct_internal_t* pr, bound_t* m, size_t dim);
bound_t* hmat_alloc_zero  (oct_internal_t* pr, size_t dim);
bound_t* hmat_alloc_top   (oct_internal_t* pr, size_t dim);
bound_t* hmat_copy        (oct_internal_t* pr, bound_t* m, size_t dim);
void     hmat_fdump       (FILE* stream, oct_internal_t* pr,
			   bound_t* m, size_t dim);



//* ============================================================ */
/* II.2 Access */
/* ============================================================ */

static inline size_t matsize(size_t dim)
{
  return 2 * dim * (dim+1);
}

/* position of (i,j) element, assuming j/2 <= i/2 */
static inline size_t matpos(size_t i, size_t j)
{
  return j + ((i+1)*(i+1))/2;
}

/* position of (i,j) element, no assumption */
static inline size_t matpos2(size_t i, size_t j)
{
  if (j>i) return matpos(j^1,i^1);
  else return matpos(i,j);
}


/* ============================================================ */
/* II.3 Closure Algorithms */
/* ============================================================ */

/* see oct_closure.c */

bool hmat_s_step(bound_t* m, size_t dim);
bool hmat_close(bound_t* m, size_t dim);
bool hmat_close_incremental(bound_t* m, size_t dim, size_t v);
bool hmat_check_closed(bound_t* m, size_t dim);


/* ============================================================ */
/* II.4  Constraints and generators */
/* ============================================================ */

/* see oct_transfer.c */

bool hmat_add_lincons(oct_internal_t* pr, bound_t* b, size_t intdim, size_t dim,
		      ap_lincons0_array_t* ar, bool* exact,
		      bool* respect_closure);

void hmat_add_generators(oct_internal_t* pr, bound_t* b, size_t dim,
			 ap_generator0_array_t* ar);


/* ============================================================ */
/* II.5 Resze */
/* ============================================================ */

/* see oct_reize.c */

void hmat_addrem_dimensions(bound_t* dst, bound_t* src,
			    ap_dim_t* pos, size_t nb_pos,
			    size_t mult, size_t dim, bool add);

void hmat_permute(bound_t* dst, bound_t* src,
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
  num_incomplete  does the type make algorithms incomplete
  num_safe        is the type safe in case of overflow
*/


#if defined(NUM_LONGINT) || defined(NUM_LONGLONGINT)
/* overflows produce unsound results, type not closed by / 2 */
#define num_incomplete     1
#define num_safe           0

#elif defined ( NUM_MPZ )
/* no overflow, type not closed by / 2 */
#define num_incomplete     1
#define num_safe           1

#elif defined(NUM_LONGRAT) || defined(NUM_LONGLONGRAT)
/* complete algorithms, but overflows produce unsound results */
#define num_incomplete     0
#define num_safe           0

#elif defined(NUM_MPQ)
/* the "perfect" type */
#define num_incomplete     0
#define num_safe           1

#elif defined(NUM_DOUBLE) || defined(NUM_LONGDOUBLE) || defined(NUM_MPFR)
/* overflow are ok (stick to +oo), type not closed by + and / 2 */
#define num_incomplete     1
#define num_safe           1

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
static inline void bound_of_scalar(oct_internal_t* pr,
				   bound_t r, ap_scalar_t* t,
				   bool neg, bool mul2)
{
  if (neg) ap_scalar_neg(t,t);
  if (!bound_set_ap_scalar(r,t)) pr->conv = true;
  if (mul2) {
    bound_mul_2(r,r);
    pr->conv = true;
  }
  if (neg) ap_scalar_neg(t,t);
}


/* both bounds of an interval, the lower bound is negated
   pr->conv is set if the conversion is not exact
   returns true if the interval is empty
*/
static inline bool bounds_of_interval(oct_internal_t* pr,
				      bound_t minf, bound_t sup,
				      ap_interval_t* i,
				      bool mul2)
{
  bound_of_scalar(pr,minf,i->inf,true,mul2);
  bound_of_scalar(pr,sup,i->sup,false,mul2);
  return ap_scalar_cmp(i->inf,i->sup)>0;
}

/* as above, for a coeff_t */
static inline bool bounds_of_coeff(oct_internal_t* pr,
				   bound_t minf, bound_t sup,
				   ap_coeff_t c,
				   bool mul2)
{
  switch (c.discr) {
  case AP_COEFF_SCALAR:
    bound_of_scalar(pr,minf,c.val.scalar,true,mul2);
    bound_of_scalar(pr,sup,c.val.scalar,false,mul2);
    return false;
  case AP_COEFF_INTERVAL:
    bound_of_scalar(pr,minf,c.val.interval->inf,true,mul2);
    bound_of_scalar(pr,sup,c.val.interval->sup,false,mul2);
    return ap_scalar_cmp(c.val.interval->inf,c.val.interval->sup)>0;
  default: arg_assert(0,return false;);
  }
}

static void bounds_of_generator(oct_internal_t* pr, bound_t* dst,
				ap_linexpr0_t* e, size_t dim)
{
  size_t i;
  switch (e->discr) {
  case AP_LINEXPR_DENSE:
    arg_assert(e->size<=dim,return;);
    for (i=0;i<e->size;i++) {
      bounds_of_coeff(pr,dst[2*i],dst[2*i+1],e->p.coeff[i],false);
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
      ap_dim_t d = e->p.linterm[i].dim;
      if (d==AP_DIM_MAX) continue;
      arg_assert(d<dim,return;);
      bounds_of_coeff(pr,dst[2*d],dst[2*d+1],e->p.linterm[i].coeff,false);
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
static inline void scalar_of_upper_bound(oct_internal_t* pr,
					 ap_scalar_t* r,
					 bound_t b,
					 bool div2)
{
  ap_scalar_reinit(r,NUM_AP_SCALAR);
  if (bound_infty(b)) ap_scalar_set_infty(r,1);
  else {
    switch (NUM_AP_SCALAR) {
    case AP_SCALAR_DOUBLE:
      if (!double_set_num(&r->val.dbl,bound_numref(b)) || div2) pr->conv = 1;
      if (div2) r->val.dbl /= 2;
      break;
    case AP_SCALAR_MPQ:
      if (!mpq_set_num(r->val.mpq,bound_numref(b)) || div2) pr->conv = 1;
      if (div2) mpq_div_2exp(r->val.mpq,r->val.mpq,1);
      break;
    case AP_SCALAR_MPFR:
      if (!mpfr_set_num(r->val.mpfr,bound_numref(b)) || div2) pr->conv = 1;
      if (div2) mpfr_div_2ui(r->val.mpfr,r->val.mpfr,1,GMP_RNDU);
      break;
    default:
      abort();
    }
  }
}

/* opposite of lower bound => scalar, with optional division by 2
   pr->conv is set if the conversion is not exact
*/
static inline void scalar_of_lower_bound(oct_internal_t* pr,
					 ap_scalar_t* r,
					 bound_t b,
					 bool div2)
{
  ap_scalar_reinit(r,NUM_AP_SCALAR);
  if (bound_infty(b)) ap_scalar_set_infty(r,-1);
  else {
    switch (NUM_AP_SCALAR) {
    case AP_SCALAR_DOUBLE:
      if (!double_set_num(&r->val.dbl,bound_numref(b)) || div2) pr->conv = 1;
      if (div2) r->val.dbl /= 2;
      r->val.dbl = -r->val.dbl;
      break;
    case AP_SCALAR_MPQ:
      if (!mpq_set_num(r->val.mpq,bound_numref(b)) || div2)  pr->conv = 1;
      if (div2) mpq_div_2exp(r->val.mpq,r->val.mpq,1);
      mpq_neg(r->val.mpq,r->val.mpq);
      break;
    case AP_SCALAR_MPFR:
      if (!mpfr_set_num(r->val.mpfr,bound_numref(b)) || div2) pr->conv = 1;
      if (div2) mpfr_div_2ui(r->val.mpfr,r->val.mpfr,1,GMP_RNDU);
      mpfr_neg(r->val.mpfr,r->val.mpfr,GMP_RNDD);
      break;
    default:
      abort();
    }
  }
}


/* makes an interval from [-minf,sup], with sound approximations
   pr->conv is set if the conversion is not exact
   note: may output an empty interval
*/
static inline void interval_of_bounds(oct_internal_t* pr,
				      ap_interval_t* i,
				      bound_t minf, bound_t sup,
				      bool div2)
{
  scalar_of_upper_bound(pr,i->sup, sup,div2);
  scalar_of_lower_bound(pr,i->inf,minf,div2);
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


/* ============================================================ */
/* III.5 Conversion to constraints */
/* ============================================================ */

/* constraint at line i, column j, with upper bound m */
static inline ap_lincons0_t lincons_of_bound(oct_internal_t* pr,
					     size_t i, size_t j,
					     bound_t m)
{
  ap_linexpr0_t* e;
  if (i==j) {
    /* zeroary constraint */
    e = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, 0);
    scalar_of_upper_bound(pr,e->cst.val.scalar,m,true);
  }
  else if (i==(j^1)) {
    /* unary constraint */
    e = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, 1);
    e->p.linterm[0].dim = i/2;
    ap_scalar_set_int(e->p.linterm[0].coeff.val.scalar,(i&1) ? -1 : 1);
    scalar_of_upper_bound(pr,e->cst.val.scalar,m,true);
  }
  else {
    /* binary constraint */
    e = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, 2);
    e->p.linterm[0].dim = j/2;
    e->p.linterm[1].dim = i/2;
    ap_scalar_set_int(e->p.linterm[0].coeff.val.scalar,(j&1) ?  1 : -1);
    ap_scalar_set_int(e->p.linterm[1].coeff.val.scalar,(i&1) ? -1 :  1);
    scalar_of_upper_bound(pr,e->cst.val.scalar,m,false);
  }
  return ap_lincons0_make(AP_CONS_SUPEQ,e,NULL);
}


/* ============================================================ */
/* III.5 Expression classification */
/* ============================================================ */

/* see oct_transfer.c */

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

  /* expression has integer value */
  int is_int;

} uexpr;

/* convert expression to bounds, look for unit unary or binary form */
uexpr oct_uexpr_of_linexpr(oct_internal_t* pr, bound_t* dst,
			   ap_linexpr0_t* e, size_t intdim, size_t dim);


/* ********************************************************************** */
/* IV. Octagons */
/* ********************************************************************** */


/* ============================================================ */
/* IV.1 Internal Representation */
/* ============================================================ */

struct _oct_t {
  bound_t* m;      /* contraint half-matrix (or NULL) */
  bound_t* closed; /* closed version of m (or NULL for not available) */
  size_t dim;      /* total number of variables */
  size_t intdim;   /* the first intdim variables are integer ones */
};

/* several cases are possible
   m==NULL closed==NULL -- definitively empty octagon
   m!=NULL closed==NULL -- empty or non-empty octagon, closure not available
   m==NULL closed!=NULL \_ definitively non-empty octagon, closure available
   m!=NULL closed!=NULL /
*/


/* ============================================================ */
/* IV.2 Management */
/* ============================================================ */

oct_t* oct_alloc_internal (oct_internal_t* pr, size_t dim, size_t intdim);
void   oct_free_internal  (oct_internal_t* pr, oct_t* o);
oct_t* oct_copy_internal  (oct_internal_t* pr, oct_t* o);
void   oct_cache_closure  (oct_internal_t* pr, oct_t* a);
void   oct_close          (oct_internal_t* pr, oct_t* a);
oct_t* oct_set_mat        (oct_internal_t* pr, oct_t* a, bound_t* m,
			   bound_t* closed, bool destructive);
oct_t* oct_alloc_top      (oct_internal_t* pr, size_t dim, size_t intdim);


#ifdef __cplusplus
}
#endif

#endif /* __OCT_INTERNAL_H */
