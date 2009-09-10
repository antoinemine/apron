/*
 * ppl_user.cc
 *
 * APRON Library / PPL library wrapper
 *
 * Conversion between APRON user types and PPL classes.
 * These are used internally by all PPL wrappers, but not exported to the user.
 *
 * Copyright (C) Antoine Mine' 2006
 *
 */

/* 
 * This file is part of the APRON Library, released under GPL license.
 * Please read the COPYING file packaged in the distribution.
 */

/* APRON includes */
#include "ap_ppl.h"
#include "ppl_poly.hh"

#define NUM_MPQ
#include "num.h"
#include "numint.h"
#include "numrat.h"
#include "bound.h"
#include "itv.h"
#include "itv_linexpr.h"
#include "itv_linearize.h"

/* PPL includes */
#include "ppl.hh"
/*
using namespace std;
using namespace Parma_Polyhedra_Library;
*/

#ifndef PPL_VERSION_MAJOR 
#error "no version information"
#endif

#if (PPL_VERSION_MAJOR==0) && (PPL_VERSION_MINOR <= 9)
#define PPL_0_9
#endif


/* ********************************************************************** */
/* Types and exceptions */
/* ********************************************************************** */

/* Internal structure in managers */
typedef struct ppl_internal_t {
  bool strict; /* strict or loose mode for polyhedra */
  struct itv_internal_t* itv; /* Used for linearisation */ 
} ppl_internal_t;

extern "C" ppl_internal_t* ap_ppl_internal_alloc(bool strict);
extern "C" void ap_ppl_internal_free(void*);

/* Indicates that a an object cannot be exactly converted into another one
   should be caught in transfer functions
*/
class cannot_convert: public exception {
public: 
  cannot_convert() {};
};


/* ********************************************************************** */
/* Number conversions */
/* ********************************************************************** */

/* GMP => scalar (exact) */
static inline void ap_ppl_mpz_to_scalar(ap_scalar_t* s, const mpz_class& i)
{
  ap_scalar_reinit(s,AP_SCALAR_MPQ);
  mpz_set(mpq_numref(s->val.mpq), i.get_mpz_t());
  mpz_set_ui(mpq_denref(s->val.mpq), 1);
}

/* GMP / GMP => scalar (exact) */
static inline void ap_ppl_mpz2_to_scalar(ap_scalar_t* s, 
					 const mpz_class& n, const mpz_class& d)
{
  ap_scalar_reinit(s,AP_SCALAR_MPQ);
  mpz_set(mpq_numref(s->val.mpq), n.get_mpz_t());
  mpz_set(mpq_denref(s->val.mpq), d.get_mpz_t());
  mpq_canonicalize(s->val.mpq);
}

/* coeff => GMP */
/* fail if the coeff cannot be exactely represented as a finite mpq */
static inline void ap_ppl_mpq_of_coef(mpq_class& res, const ap_coeff_t coef)
{

  switch (coef.discr) {
  case AP_COEFF_SCALAR:
    if (ap_scalar_infty(coef.val.scalar) ||
	ap_mpq_set_scalar(res.get_mpq_t(),coef.val.scalar,GMP_RNDU))
      throw cannot_convert();
    break;
  case AP_COEFF_INTERVAL:
    {
      mpq_class tmp;
      if (ap_scalar_infty(coef.val.interval->sup) ||
	  ap_scalar_infty(coef.val.interval->inf) ||
	  ap_mpq_set_scalar(res.get_mpq_t(),coef.val.interval->sup,GMP_RNDU) ||
	  ap_mpq_set_scalar(tmp.get_mpq_t(),coef.val.interval->inf,GMP_RNDD) ||
	  tmp!=res) 
	throw cannot_convert();
    }
    break;
  default: 
    throw invalid_argument("Coefficient type ap_ppl_mpq_of_coef");
  }
}


/* ********************************************************************** */
/* Conversions from PPL to APRON */
/* ********************************************************************** */

/* some of these may raise cannot_convert */
/* returned booleans are true when inexact */
/* reference booleans are set to true when inexact, left unchanged otherwise */

extern ap_lincons0_t ap_ppl_to_lincons(const Constraint& c);
extern ap_lincons0_t ap_ppl_to_lincons(const Congruence& c);
extern ap_lincons0_array_t ap_ppl_to_lincons_array(const Constraint_System& c);
extern ap_lincons0_array_t ap_ppl_to_lincons_array(const Congruence_System& c);
extern ap_generator0_t ap_ppl_to_generator(const Generator& c, bool& exact);
extern ap_generator0_t ap_ppl_to_generator(const Grid_Generator& c);
extern ap_generator0_array_t ap_ppl_to_generator_array(const Generator_System& c, bool& exact);
extern ap_generator0_array_t ap_ppl_to_generator_array(const Grid_Generator_System& c);
extern ap_generator0_array_t ap_ppl_generator_universe(size_t dim);
extern void ap_ppl_box_universe(ap_interval_t** i,size_t nb);

/* ********************************************************************** */
/* Conversions from ITV to PPL */
/* ********************************************************************** */

/* some of these may raise cannot_convert */
/* returned booleans are true when exact */
/* reference booleans are set to true when exact, false otherwise */

/* Linear expressions */
extern bool ap_ppl_of_itv_linexpr(Linear_Expression& r, 
				  mpz_class& den,
				  itv_linexpr_t* c,
				  int mode);

/* Linear constraints */
extern bool ap_ppl_of_itv_lincons(Constraint& r, mpz_class& den, itv_lincons_t* c, bool allow_strict);
extern bool ap_ppl_of_itv_lincons(Congruence& r, mpz_class& den, itv_lincons_t* c);
extern bool ap_ppl_of_itv_lincons_array(Constraint_System& r, mpz_class& den,itv_lincons_array_t* a,bool allow_strict);
extern bool ap_ppl_of_itv_lincons_array(Congruence_System& r, mpz_class& den,itv_lincons_array_t* a);

/* ********************************************************************** */
/* Conversions from APRON to PPL */
/* ********************************************************************** */

/* some of these may raise cannot_convert */
/* returned booleans are true when exact */
/* reference booleans are set to true when exact, false otherwise */

/* Generators */
extern bool ap_ppl_of_generator(itv_internal_t* intern, Generator& r,ap_generator0_t* c);
extern bool ap_ppl_of_generator(itv_internal_t* intern, Grid_Generator& r,ap_generator0_t* c);
extern bool ap_ppl_of_generator_array(itv_internal_t* intern, Generator_System& r,ap_generator0_array_t* a);
extern bool ap_ppl_of_generator_array(itv_internal_t* intern, Grid_Generator_System& r,ap_generator0_array_t* a);

/* Boxes */ 
extern bool ap_ppl_of_box(Constraint_System& r, ap_interval_t** a, size_t intdim, size_t realdim);
extern bool ap_ppl_of_box(Congruence_System& r, ap_interval_t** a, size_t intdim, size_t realdim);

/* Linear expressions */
extern void ap_ppl_of_linexpr(itv_internal_t* intern,
			      Linear_Expression& r, mpz_class& den,
			      ap_linexpr0_t* c,
			      int mode);

/* Linear constraints */
extern bool ap_ppl_of_lincons(itv_internal_t* intern, Constraint& r, PPL_Poly& abs, ap_lincons0_t* c,bool allow_strict);
extern bool ap_ppl_of_lincons(itv_internal_t* intern, Congruence& r,ap_lincons0_t* c);
extern bool ap_ppl_of_lincons_array(itv_internal_t* intern, Constraint_System& r,ap_lincons0_array_t* a,bool allow_strict);
extern bool ap_ppl_of_lincons_array(itv_internal_t* intern, Congruence_System& r,ap_lincons0_array_t* a);

/* ********************************************************************** */
/* Others */
/* ********************************************************************** */

static ap_abstract0_t* ap_ppl_make_abstract0(ap_manager_t* man, void* v)
{
  ap_abstract0_t* r = (ap_abstract0_t*)malloc(sizeof(ap_abstract0_t));
  assert(r);
  r->value = v;
  r->man = ap_manager_copy(man);
  return r;
}

/* returns an element with the correct manager and, if possible, size */
static ap_abstract0_t* ap_ppl_invalid_abstract0(ap_manager_t* man, ap_abstract0_t* org = NULL)
{
  if (org) {
    ap_dimension_t d = ap_abstract0_dimension(org->man,org);
    return ap_abstract0_top(man,d.intdim,d.realdim);
  }
  else 
    return ap_abstract0_top(man,0,1);
}

#define arg_assert(cond,action,funid)					\
  do { if (!(cond)) {							\
      char buf_[1024];							\
      snprintf(buf_,sizeof(buf_),					\
	       "assertion (%s) failed in %s at %s:%i",			\
	       #cond, __func__, __FILE__, __LINE__);			\
      ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT, funid,buf_); \
      action }								\
  } while(0)
