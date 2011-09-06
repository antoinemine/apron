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

/* PPL includes */
#include "ppl.hh"
using namespace std;
using namespace Parma_Polyhedra_Library;

/* APRON includes */
#include "eitvMPQ.h"
#include "ap_global0.h"
#include "ap_ppl.h"
#include "ppl_poly.hh"
#include "ppl_grid.hh"

#ifndef PPL_VERSION_MAJOR
#error "no version information"
#endif
#if (PPL_VERSION_MAJOR==0) && (PPL_VERSION_MINOR <= 9)
#error "version <=0.9 not supported any more"
#endif

/* ********************************************************************** */
/* Types and exceptions */
/* ********************************************************************** */

/* Internal structure in managers */
typedef struct ap_ppl_internal_t {
  union {
    ap_ppl_poly_option_t poly;
  } option;
  ap_linexprMPQ_t linexprMPQ;
  ap_coeff_t coeffMPQ;
  eitvMPQ_t eitvMPQ;
  mpz_class num;
  mpz_class den;
  mpq_class mpqclass;
  bool strict; /* strict or loose mode for polyhedra */
} ap_ppl_internal_t;

/* Indicates that a an object cannot be exactly converted into another one
   should be caught in transfer functions
*/
class cannot_convert: public exception {
public:
  cannot_convert() {};
};

/* partial functions used in map_space_dimensions */
class ap_ppl_map {
 protected:
  ap_dim_t dom,codom;
  ap_dim_t* tab;
  char* def;
 public:
  ap_ppl_map(size_t dom,size_t codom) : dom(dom), codom(codom)
  {
    tab = new ap_dim_t [dom];
    def = new char [dom];
    memset(def,0,dom);
  }
  ~ap_ppl_map() { delete[] tab; delete[] def; }
  void set(dimension_type i,dimension_type j) { tab[i] = j; def[i] = 1; }
  bool has_empty_codomain() const { return codom==0; }
  dimension_type max_in_codomain() const { return codom-1; }
  bool maps(dimension_type i,dimension_type& j) const { j = tab[i]; return def[i]; }
};

static inline ap_ppl_internal_t* ap_ppl_get_internal(ap_manager_t* man)
{
  return (ap_ppl_internal_t*)man->internal;
}

extern "C" ap_ppl_internal_t* ap_ppl_internal_alloc(bool strict);
extern "C" void ap_ppl_internal_free(void*);

void ap_ppl_box_set_universe(ap_linexpr0_t box0, ap_dimension_t dim);
void ap_ppl_generator_array_set_universe(ap_lingen0_array_t array,ap_dimension_t dim);

/* ********************************************************************** */
/* Number conversions */
/* ********************************************************************** */


// /* GMP => scalar (exact) */
// static inline void ap_ppl_mpz_to_scalar(ap_scalar_t* s, const mpz_class& i)
// {
//   ap_scalar_reinit(s,AP_SCALAR_MPQ);
//   mpz_set(mpq_numref(s->val.mpq), i.get_mpz_t());
//   mpz_set_ui(mpq_denref(s->val.mpq), 1);
// }

// /* GMP / GMP => scalar (exact) */
// static inline void ap_ppl_mpz2_to_scalar(ap_scalar_t* s,
//					 const mpz_class& n, const mpz_class& d)
// {
//   ap_scalar_reinit(s,AP_SCALAR_MPQ);
//   mpz_set(mpq_numref(s->val.mpq), n.get_mpz_t());
//   mpz_set(mpq_denref(s->val.mpq), d.get_mpz_t());
//   mpq_canonicalize(s->val.mpq);
// }

/* ********************************************************************** */
/* Conversions from PPL to APRON */
/* ********************************************************************** */

/* some of these may raise cannot_convert */
/* returned booleans are true when inexact */
/* reference booleans are set to true when inexact, left unchanged otherwise */

bool ap_lincons0_set_ppl_Constraint(ap_lincons0_t cons, const Constraint& c, num_internal_t num);
bool ap_lincons0_set_ppl_Congruence(ap_lincons0_t cons, const Congruence& c, num_internal_t num);
bool ap_lingen0_set_ppl_Generator(ap_lingen0_t cons, const Generator& c, num_internal_t num);
bool ap_lingen0_set_ppl_Grid_Generator(ap_lingen0_t cons, const Grid_Generator& c, num_internal_t num);

bool ap_lincons0_array_set_ppl_Constraint_System(ap_lincons0_array_t cons, const Constraint_System& c, num_internal_t num);
bool ap_lincons0_array_set_ppl_Congruence_System(ap_lincons0_array_t cons, const Congruence_System& c, num_internal_t num);
bool ap_lingen0_array_set_ppl_Generator_System(ap_lingen0_array_t cons, const Generator_System& c, num_internal_t num);
bool ap_lingen0_array_set_ppl_Grid_Generator_System(ap_lingen0_array_t cons, const Grid_Generator_System& c, num_internal_t num);

void ap_ppl_lingen0_array_set_universe(ap_lingen0_array_t array, size_t dim);

/* ********************************************************************** */
/* Conversions from APRON to PPL */
/* ********************************************************************** */

/* coeff => MPQ */
/* fail if the coeff cannot be exactely represented as a finite mpq */
void ap_ppl_mpq_set_coeff(mpq_class& res, const ap_coeff_t coeff, num_internal_t num);

/* some of these may raise cannot_convert */
/* returned booleans are true when exact */
/* reference booleans are set to true when exact, false otherwise */

bool ap_ppl_Constraint_System_set_box(
    Constraint_System& r, ap_linexpr0_t box0, ap_dimension_t dim, ap_manager_t* man
);
bool ap_ppl_Congruence_System_set_box(
    Congruence_System& r, ap_linexpr0_t box0, ap_dimension_t dim, ap_manager_t* man
);


/* Linear expressions */
bool ap_ppl_Linear_Expression_set_linexprMPQ(
    Linear_Expression& r, ap_linexprMPQ_t linexpr, int mode, ap_manager_t* man
);
bool ap_ppl_Linear_Expression_set_linexpr0(
    Linear_Expression& r, ap_linexpr0_t c, int mode, ap_manager_t* man
);
bool ap_ppl_Constraint_set_lincons0(
    Constraint& r, ap_lincons0_t lincons0, ap_manager_t* man
);
bool ap_ppl_Congruence_set_lincons0(
    Congruence& r, ap_lincons0_t lincons0, ap_manager_t* man
);
bool ap_ppl_Generator_set_lingen0(
    Generator& r, ap_lingen0_t lingen0, ap_manager_t* man
);
bool ap_ppl_Grid_Generator_set_lingen0(
    Grid_Generator& r, ap_lingen0_t lingen0, ap_manager_t* man
);
bool ap_ppl_Constraint_System_set_lincons0_array(
    Constraint_System& r, ap_lincons0_array_t array, ap_manager_t* man
);
bool ap_ppl_Congruence_System_set_lincons0_array(
    Congruence_System& r, ap_lincons0_array_t array, ap_manager_t* man
);
bool ap_ppl_Generator_System_set_lingen0_array(
    Generator_System& r, ap_lingen0_array_t array, ap_manager_t* man
);
bool ap_ppl_Grid_Generator_System_set_lingen0_array(
    Grid_Generator_System& r, ap_lingen0_array_t array, ap_manager_t* man
);

/* ********************************************************************** */
/* Others */
/* ********************************************************************** */

ap_abstract0_t* ap_ppl_make_abstract0(ap_manager_t* man, void* v);

/* returns an element with the correct manager and, if possible, size */
ap_abstract0_t* ap_ppl_invalid_abstract0(ap_manager_t* man, ap_abstract0_t* org);

#define arg_assert(cond,action,funid)					\
  do { if (!(cond)) {							\
      char buf_[1024];							\
      snprintf(buf_,sizeof(buf_),					\
	       "assertion (%s) failed in %s at %s:%i",			\
	       #cond, __func__, __FILE__, __LINE__);			\
      ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT, funid,buf_); \
      action }								\
  } while(0)
