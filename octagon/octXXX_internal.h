/*
 * octXXX_internal.h
 *
 * Private definitions, access to internal structures and algorithms.
 * Use with care.
 *
 * APRON Library / Octagonal Domain
 *
 * Copyright (C) Antoine Mine' 2006
 *
 */

/* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution.
*/

#ifndef __OCTXXX_INTERNAL_H
#define __OCTXXX_INTERNAL_H

#include "octXXX_fun.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* I. Manager */
/* ********************************************************************** */

/* manager-local data specific to octagons */
struct _octXXX_internal_t {
  /* Pointer to num workspace */
  num_internal_ptr num;

  /* Options */
  oct_option_t option;

  /* current function */
  ap_funid_t funid;

  /* local parameters for current function */
  ap_funopt_t* funopt;

  /* temporary value */
  eitvXXX_t eitv;

  /* growing temporary buffer */
  boundXXX_t* tmp;
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
octXXX_internal_t*
octXXX_init_from_manager(ap_manager_t* man, ap_funid_t id, size_t size);

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

/* see octXXX.hmat.c */

boundXXX_t* hmatXXX_alloc       (octXXX_internal_t* pr, size_t dim);
void     hmatXXX_free        (octXXX_internal_t* pr, boundXXX_t* m, size_t dim);
boundXXX_t* hmatXXX_alloc_zero  (octXXX_internal_t* pr, size_t dim);
boundXXX_t* hmatXXX_alloc_top   (octXXX_internal_t* pr, size_t dim);
boundXXX_t* hmatXXX_copy        (octXXX_internal_t* pr, boundXXX_t* m, size_t dim);
void     hmatXXX_fdump       (FILE* stream, octXXX_internal_t* pr,
			   boundXXX_t* m, size_t dim);



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

/* see octXXX_closure.c */

bool hmatXXX_s_step(boundXXX_t* m, size_t dim);
bool hmatXXX_close(boundXXX_t* m, size_t dim);
bool hmatXXX_close_incremental(boundXXX_t* m, size_t dim, size_t v);
bool hmatXXX_check_closed(boundXXX_t* m, size_t dim);


/* ============================================================ */
/* II.4  Constraints and generators */
/* ============================================================ */

/* see octXXX_transfer.c */

bool hmatXXX_add_lincons0_array(octXXX_internal_t* pr, boundXXX_t* b, size_t dim,
		      ap_lincons0_array_t ar, bool* exact,
		      bool* respect_closure);

void hmatXXX_add_lingen0_array(octXXX_internal_t* pr, boundXXX_t* b, size_t dim,
			 ap_lingen0_array_t ar);


/* ============================================================ */
/* II.5 Resze */
/* ============================================================ */

/* see octXXX_reize.c */

void hmatXXX_addrem_dimensions(boundXXX_t* dst, boundXXX_t* src,
			    ap_dim_t* pos, size_t nb_pos,
			    size_t mult, size_t dim, bool add);

void hmatXXX_permute(boundXXX_t* dst, boundXXX_t* src,
		  size_t dst_dim, size_t src_dim,
		  ap_dim_t* permutation);

/* ********************************************************************** */
/* III. Numbers */
/* ********************************************************************** */

/* To perform soundly, we suppose that all conversions beteween num and
   base types (double, int, mpz, mpq, etc.) always over-approximate the
   result (as long as the fits function returns true).
 */

static inline void boundXXX_bmin(boundXXX_t dst, boundXXX_t arg)
{ boundXXX_min(dst,dst,arg); }

static inline void boundXXX_badd(boundXXX_t dst, boundXXX_t arg)
{ boundXXX_add(dst,dst,arg); }


/* ============================================================ */
/* III.1 Properties on numXXX_t */
/* ============================================================ */

/*
  numXXX_incomplete  does the type make algorithms incomplete
  numXXX_safe        is the type safe in case of overflow
*/

#define _MARK_XXX_

#if defined(_MARK_Il_) || defined(_MARK_Ill_)
/* overflows produce unsound results, type not closed by / 2 */
#define numXXX_incomplete     1
#define numXXX_safe           0
#define numXXX_scalar         AP_SCALAR_MPQ
#elif defined ( _MARK_MPZ_ )
/* no overflow, type not closed by / 2 */
#define numXXX_incomplete     1
#define numXXX_safe           1
#define numXXX_scalar         AP_SCALAR_MPQ

#elif defined(_MARK_Rl_) || defined(_MARK_Rll_)
/* complete algorithms, but overflows produce unsound results */
#define numXXX_incomplete     0
#define numXXX_safe           0
#define numXXX_scalar         AP_SCALAR_MPQ

#elif defined(_MARK_MPQ_)
/* the "perfect" type */
#define numXXX_incomplete     0
#define numXXX_safe           1
#define numXXX_scalar         AP_SCALAR_MPQ

#elif defined(_MARK_D_) || defined(_MARK_Dl_) || defined(_MARK_MPFR_)
/* overflow are ok (stick to +oo), type not closed by + and / 2 */
#define numXXX_incomplete     1
#define numXXX_safe           1
#define numXXX_scalar         AP_SCALAR_MPFR

/* duh */
#else
#error "No numerical type defined"
#endif

#undef _MARK_XXX_

/* ============================================================ */
/* III.2 Conversions from user types */
/* ============================================================ */

/* sound conversion from a coeff to an upper boundXXX_t
   optional multiplication by 2
   pr->conv is set if the conversion is not exact
 */
void upper_boundXXX_of_coeff(octXXX_internal_t* pr,
			     boundXXX_t r, ap_coeff_t coeff,
			     bool mul2);
/* sound conversion from a coeff to [neginf,sup]/eitvXXX_t
   optional multiplication by 2
   pr->conv is set if the conversion is not exact
   returns true if the interval is empty
*/
bool boundsXXX_of_coeff(octXXX_internal_t* pr,
			boundXXX_t neginf, boundXXX_t sup,
			ap_coeff_t c,
			bool mul2);
bool eitvXXX_of_coeff(octXXX_internal_t* pr,
		      eitvXXX_t res,
		      ap_coeff_t c,
		      bool mul2);
void boundsXXX_of_lingen0(octXXX_internal_t* pr, boundXXX_t* dst,
			  ap_linexpr0_t e, size_t dim);

/* ============================================================ */
/* III.3 Conversions to user types */
/* ============================================================ */

/* upper bound => coeff, with optional division by 2
   pr->conv is set if the conversion is not exact
*/
void coeff_of_upper_boundXXX(octXXX_internal_t* pr,
			     ap_coeff_t coeff,
			     boundXXX_t m,
			     bool div2);

/* makes an interval from [neginf,sup]/eitvXXX, with sound approximations
   pr->conv is set if the conversion is not exact
   note: may output an empty interval
*/
void coeff_of_eitvXXX(octXXX_internal_t* pr,
		      ap_coeff_t res,
		      eitvXXX_t eitv,
		      bool div2);
void coeff_of_boundsXXX(octXXX_internal_t* pr,
			ap_coeff_t coeff,
			boundXXX_t neginf, boundXXX_t sup,
			bool div2);

/* ============================================================ */
/* III.4 Bound manipulations */
/* ============================================================ */

/* [-r_inf,r_sup] = [-a_inf,a_sup] * [-b_inf,b_sup]
   where 0 * oo = oo * 0 = 0
 */
void boundsXXX_mul(boundXXX_t r_inf, boundXXX_t r_sup,
		   boundXXX_t a_inf, boundXXX_t a_sup,
		   boundXXX_t b_inf, boundXXX_t b_sup,
		   boundXXX_t tmp[8]);

/* ============================================================ */
/* III.5 Conversion to constraints */
/* ============================================================ */

/* constraint at line i, column j, with upper bound m */
MACRO_FOREACH ZZZ @MainNum
void linconsZZZ_of_boundXXX(octXXX_internal_t* pr,
			    ap_linconsZZZ_t cons,
			    size_t i, size_t j,
			    boundXXX_t m);
ENDMACRO
void lincons0_of_boundXXX(octXXX_internal_t* pr,
			  ap_lincons0_t cons,
			  size_t i, size_t j,
			  boundXXX_t m);

/* ============================================================ */
/* III.5 Expression classification */
/* ============================================================ */

/* see octXXX_transfer.c */

#ifndef _oct_uexpr_
#define _oct_uexpr_
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
#endif

/* convert expression to bounds, look for unit unary or binary form */
uexpr octXXX_uexpr_of_linexpr0(octXXX_internal_t* pr, boundXXX_t* dst,
			       ap_linexpr0_t e, size_t dim);


/* ********************************************************************** */
/* IV. Octagons */
/* ********************************************************************** */


/* ============================================================ */
/* IV.1 Internal Representation */
/* ============================================================ */

struct _octXXX_t {
  boundXXX_t* m;      /* contraint half-matrix (or NULL) */
  boundXXX_t* closed; /* closed version of m (or NULL for not available) */
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

octXXX_t* octXXX_alloc_internal (octXXX_internal_t* pr, size_t dim, size_t intdim);
void   octXXX_free_internal  (octXXX_internal_t* pr, octXXX_t* o);
octXXX_t* octXXX_copy_internal  (octXXX_internal_t* pr, octXXX_t* o);
void   octXXX_cache_closure  (octXXX_internal_t* pr, octXXX_t* a);
void   octXXX_close          (octXXX_internal_t* pr, octXXX_t* a);
octXXX_t* octXXX_set_mat        (octXXX_internal_t* pr, octXXX_t* a, boundXXX_t* m,
			   boundXXX_t* closed, bool destructive);
octXXX_t* octXXX_alloc_top      (octXXX_internal_t* pr, size_t dim, size_t intdim);


#ifdef __cplusplus
}
#endif

#endif /* __OCTXXX_INTERNAL_H */
