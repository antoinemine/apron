/*
 * pplite_user.hh
 *
 * APRON Library / PPLite library wrapper
 *
 * Wrappers specific to the PPLite domains.
 *
 * Copyright (C) Antoine Mine' 2006
 * Copyright (C) 2018-2024 Enea Zaffanella <enea.zaffanella@unipr.it>
 *
 */

/*
 * This file is part of the APRON Library, released under GPL license.
 * Please read the COPYING file packaged in the distribution.
 */

#ifndef __PPLITE_USER_HH
#define __PPLITE_USER_HH

#include "ap_pplite.h"

#define NUM_MPQ
#include "num.h"
#include "numint.h"
#include "numrat.h"
#include "bound.h"
#include "itv.h"
#include "itv_linexpr.h"
#include "itv_linearize.h"

#include "pplite/pplite.hh"

namespace pplite {
namespace apron {

/* An object cannot be exactly converted into another one;
   this is meant to be caught in transfer functions.
*/
class cannot_convert: public std::exception {};


/**********************************************/
/* Conversions from PPLite to GMP mpz_t/mpq_t */
/* (backward compatibility helpers)           */
/**********************************************/

inline void mpz_set_Integer(mpz_t z, const Integer& i) {
#ifdef HAVE_PPLITE_0_11
  mpz_set(z, i);
#else
  i.get_mpz(z);
#endif
}

inline void mpq_set_Rational(mpq_t q, const Rational& r) {
  // No need to canonicalize
#ifdef HAVE_PPLITE_0_11
  mpq_set(q, mpq_class(r).get_mpq_t());
#else
  r.get_mpq(q);
#endif
}

/************************************/
/* Conversions from PPLite to Apron */
/************************************/

/* Integer => scalar */
inline void
to_scalar(ap_scalar_t* s, const Integer& i) {
  ap_scalar_reinit(s, AP_SCALAR_MPQ);
  mpz_set_Integer(mpq_numref(s->val.mpq), i);
  mpz_set_ui(mpq_denref(s->val.mpq), 1);
}

/* Integer / Integer => scalar */
inline void
to_scalar(ap_scalar_t* s,
          const Integer& n, const Integer& d) {
  ap_scalar_reinit(s, AP_SCALAR_MPQ);
  mpz_set_Integer(mpq_numref(s->val.mpq), n);
  mpz_set_Integer(mpq_denref(s->val.mpq), d);
  mpq_canonicalize(s->val.mpq);
}

/* Rational => scalar */
inline void
to_scalar(ap_scalar_t* s, const Rational& r) {
  ap_scalar_reinit(s, AP_SCALAR_MPQ);
  mpq_set_Rational(s->val.mpq, r);
}

/* Con::Type => ap_constyp_t */
inline ap_constyp_t
to_ap_constyp(Con::Type type) {
  switch (type) {
  case Con::NONSTRICT_INEQUALITY:
    return AP_CONS_SUPEQ;
  case Con::EQUALITY:
    return AP_CONS_EQ;
  case Con::STRICT_INEQUALITY:
    return AP_CONS_SUP;
  default:
    throw std::invalid_argument("Invalid pplite::Con::Type value");
  }
}

/* Gen::Type => ap_gentyp_t */
inline ap_gentyp_t
to_ap_gentyp(Gen::Type type) {
  switch (type) {
  case Gen::POINT:
  case Gen::CLOSURE_POINT:
    return AP_GEN_VERTEX;
  case Gen::RAY:
    return AP_GEN_RAY;
  case Gen::LINE:
    return AP_GEN_LINE;
  default:
    throw std::invalid_argument("Invalid pplite::Gen::Type value");
  }
}

/* Con => ap_lincons0_t */
ap_lincons0_t
to_lincons(const Con& c);

/* Cons => ap_lincons0_array_t */
template <typename Cons_Proxy>
ap_lincons0_array_t
to_lincons_array(const Cons_Proxy& cs) {
  ap_lincons0_array_t a = ap_lincons0_array_make(cs.size());
  int k = 0;
  for (const auto& c : cs) {
    a.p[k] = to_lincons(c);
    ++k;
  }
  return a;
}

/* Gen => ap_generator0_t
   sets exact according to possible approximation */
ap_generator0_t
to_generator(const Gen& c, bool& exact);

/* Gens => ap_generator0_array_t
   sets exact according to possible approximation */
template <typename Gens_Proxy>
ap_generator0_array_t
to_generator_array(const Gens_Proxy& gs, bool& exact) {
  exact = true;
  auto a = ap_generator0_array_make(gs.size());
  int k = 0;
  for (const auto& g : gs) {
    bool g_exact = true;
    a.p[k] = to_generator(g, g_exact);
    exact = exact && g_exact;
    ++k;
  }
  return a;
}

/************************************/
/* Conversions from APRON to PPLite */
/************************************/

void
from_itv_linexpr(Linear_Expr& le, Integer& inhomo, Integer& den,
                 itv_linexpr_t* linexpr, int mode);

Affine_Expr
to_scaled_Affine_Expr(itv_linexpr_t* linexpr, int mode);

Affine_Expr
to_scaled_Affine_Expr(itv_internal_t* intern, ap_linexpr0_t* c, int mode);

Itv to_Itv(itv_t itv);
Itv_Expr to_Itv_Expr_no_inhomo(itv_linexpr_t* linexpr);

inline Con::Type
from_ap_constyp(ap_constyp_t t) {
  switch (t) {
  case AP_CONS_SUPEQ:
    return Con::NONSTRICT_INEQUALITY;
  case AP_CONS_EQ:
    return Con::EQUALITY;
  case AP_CONS_SUP:
    // Caller has to check if strict ineqs are allowed
    return Con::STRICT_INEQUALITY;
  case AP_CONS_EQMOD:
    // Caller has to check if really convertible
    return Con::EQUALITY;
  case AP_CONS_DISEQ:
    throw cannot_convert();
  default:
    throw std::invalid_argument("Invalid ap_constyp_t value");
  }
}

Con
to_Con(itv_lincons_t* c, bool allow_strict, bool& exact);

Con
to_Con(itv_internal_t* intern, ap_lincons0_t* c,
       bool allow_strict, bool& exact);

Cons
to_Cons(itv_lincons_array_t* a, bool allow_strict, bool& exact);

Cons
to_Cons(itv_internal_t* intern, ap_lincons0_array_t* a,
        bool allow_strict, bool& exact);

bool
has_precise_conversion(ap_tcons0_array_t* t_array);

Cons
to_Cons(itv_internal_t* intern, ap_tcons0_array_t* t_array, size_t intdim,
        bool allow_strict, bool& exact);

Cons
to_Cons(ap_interval_t** a, size_t intdim, size_t realdim, bool& exact);

/* ap_gentyp_t => Gen::Type */
inline Gen::Type
from_ap_gentyp(ap_gentyp_t t) {
  switch (t) {
  case AP_GEN_VERTEX:
    return Gen::POINT;
  case AP_GEN_RAY:
    return Gen::RAY;
  case AP_GEN_LINE:
    return Gen::LINE;
  case AP_GEN_RAYMOD:
    // Caller has to check if really convertible
    return Gen::RAY;
  case AP_GEN_LINEMOD:
    // Caller has to check if really convertible
    return Gen::LINE;
  default:
    throw std::invalid_argument("Invalid ap_gentyp_t value");
  }
}

Gen
to_Gen(itv_internal_t* intern, ap_generator0_t* c, bool& exact);

Gens
to_Gens(itv_internal_t* intern, ap_generator0_array_t* a, bool& exact);

/* ********************************************************************** */
/* Others */
/* ********************************************************************** */

inline ap_abstract0_t*
ap_pplite_make_abstract0(ap_manager_t* man, void* v) {
  // Note: stick to malloc(), since it will be cleared by free().
  auto r = static_cast<ap_abstract0_t*>(malloc(sizeof(ap_abstract0_t)));
  assert(r);
  r->value = v;
  r->man = ap_manager_copy(man);
  return r;
}

inline ap_abstract0_t*
ap_pplite_invalid_abstract0(ap_manager_t* man,
                            ap_abstract0_t* org = nullptr) {
  if (org) {
    ap_dimension_t d = ap_abstract0_dimension(org->man, org);
    return ap_abstract0_top(man, d.intdim, d.realdim);
  }
  return ap_abstract0_top(man, 0, 1);
}

#define arg_assert(cond, action, funid)                                    \
  do {                                                                     \
    if (!(cond)) {                                                         \
      char buf_[1024];                                                     \
      snprintf(buf_,sizeof(buf_),                                          \
               "assertion (%s) failed in %s at %s:%i",                     \
               #cond, __func__, __FILE__, __LINE__);                       \
      ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT, funid,buf_); \
      action                                                               \
    }                                                                      \
  } while(0)

} // namespace apron
} // namespace pplite

#endif // !defined(__PPLITE_USER_HH)
