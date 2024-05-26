/*
 * pplite_user.cc
 *
 * APRON Library / PPLite library wrapper
 *
 * Copyright (C) Antoine Mine' 2006
 * Copyright (C) 2018-2024 Enea Zaffanella <enea.zaffanella@unipr.it>
 *
 */

/*
 * This file is part of the APRON Library, released under GPL license.
 * Please read the COPYING file packaged in the distribution.
 */

#include "pplite_user.hh"

#include <cassert>
#include <stdexcept>

namespace pplite {
namespace apron {

/************************************/
/* Conversions from PPLite to Apron */
/************************************/

/* Con => ap_lincons0_t */
ap_lincons0_t
to_lincons(const Con& c) {
  if (c.is_inconsistent())
    return ap_lincons0_make_unsat();
  auto t = to_ap_constyp(c.type());
  auto n = c.space_dim();
  ap_linexpr0_t* e = ap_linexpr0_alloc(AP_LINEXPR_DENSE, n);
  for (auto i = 0; i < n; ++i)
    to_scalar(e->p.coeff[i].val.scalar, c.coeff(Var(i)));
  to_scalar(e->cst.val.scalar, c.inhomo_term());
  return ap_lincons0_make(t, e, nullptr);
}

/* Gen => ap_generator0_t
   exact set to false for closure points */
ap_generator0_t
to_generator(const Gen& g, bool& exact) {
  exact = !g.is_closure_point();
  auto t = to_ap_gentyp(g.type());
  auto n = g.space_dim();
  auto e = ap_linexpr0_alloc(AP_LINEXPR_DENSE, n);
  ap_linexpr0_set_cst_scalar_int(e, 0);
  if (t == AP_GEN_VERTEX) {
    /* Has divisor */
    const auto& div = g.divisor();
    for (auto i = 0; i < n; ++i)
      to_scalar(e->p.coeff[i].val.scalar, g.coeff(Var(i)), div);
  } else {
    /* No divisor */
    for (auto i = 0; i < n; ++i)
      to_scalar(e->p.coeff[i].val.scalar, g.coeff(Var(i)));
  }
  return ap_generator0_make(t, e);
}

/* ap_interval_t box => Cons
   may set exact to false */
Cons
to_Cons(ap_interval_t** a, size_t intdim, size_t realdim, bool& exact) {
  exact = true;
  size_t nb = intdim+realdim;
  mpq_t temp;
  mpq_init(temp);
  mpz_ptr temp_num = mpq_numref(temp);
  mpz_ptr temp_den = mpq_denref(temp);

  Cons r;
  for (size_t i = 0; i < nb; ++i) {
    /* inf */
    switch (ap_scalar_infty(a[i]->inf)) {
    case 0:
      exact = !ap_mpq_set_scalar(temp, a[i]->inf, GMP_RNDD) && exact;
      if (i < intdim) {
        mpz_fdiv_q(temp_num, temp_num, temp_den);
        mpz_set_ui(temp_den, 1);
      }
      r.push_back(Integer(temp_den) * Var(i) >= Integer(temp_num));
      break;
    case -1:
      break;
    case 1:
      r.clear();
      r.push_back(Con::zero_dim_false());
      exact = true;
      mpq_clear(temp);
      return r;
    default:
      assert(false);
    }
    /* sup */
    switch (ap_scalar_infty(a[i]->sup)) {
    case 0:
      exact = !ap_mpq_set_scalar(temp, a[i]->sup, GMP_RNDU) && exact;
      if (i<intdim){
        mpz_fdiv_q(temp_num, temp_num, temp_den);
        mpz_set_ui(temp_den, 1);
      }
      r.push_back(Integer(temp_den) * Var(i) <= Integer(temp_num));
      break;
    case 1:
      break;
    case -1:
      r.clear();
      r.push_back(Con::zero_dim_false());
      exact = true;
      mpq_clear(temp);
      return r;
    default:
      assert(false);
    }
  }
  mpq_clear(temp);
  return r;
}

/************************************/
/* Conversions from Apron to PPLite */
/************************************/

/* Assumes a quasilinear expression,
   with the selected bound of constant coefficient different from +oo
   linearize if mode != 0
*/
void
from_itv_linexpr(Linear_Expr& le, Integer& inhomo, Integer& den,
                 itv_linexpr_t* linexpr, int mode) {
  le = Linear_Expr();
  inhomo = Integer::zero();
  den = Integer::one();

  assert(mode != 0);
  if (mode > 0) {
    if (bound_infty(linexpr->cst->sup))
      throw cannot_convert();
    if (bound_sgn(linexpr->cst->sup))
      den = Integer(numrat_denref(bound_numref(linexpr->cst->sup)));
  } else {
    if (bound_infty(linexpr->cst->inf))
      throw cannot_convert();
    if (bound_sgn(linexpr->cst->inf))
      den = Integer(numrat_denref(bound_numref(linexpr->cst->inf)));
  }

  size_t i;
  ap_dim_t dim;
  itv_ptr pitv;
  bool* peq;
  /* compute lcm of denominators in den */
  itv_linexpr_ForeachLinterm(linexpr, i, dim, pitv, peq){
    assert(*peq);
    (void) peq; // silence warning;
    lcm_assign(den, den, Integer(numrat_denref(bound_numref(pitv->sup))));
  }
  /* add variable coefficients * den */
  Integer tmp_num;
  Integer tmp_den;
  itv_linexpr_ForeachLinterm(linexpr, i, dim, pitv, peq){
    tmp_num = numrat_numref(bound_numref(pitv->sup));
    tmp_den = numrat_denref(bound_numref(pitv->sup));
    exact_div_assign(tmp_den, den, tmp_den);
    tmp_num *= tmp_den;
    add_mul_assign(le, tmp_num, Var(dim));
  }
  /* add constant coefficient * den */
  if (mode > 0) {
    tmp_num = numrat_numref(bound_numref(linexpr->cst->sup));
    tmp_den = numrat_denref(bound_numref(linexpr->cst->sup));
  } else {
    tmp_num = numrat_numref(bound_numref(linexpr->cst->inf));
    tmp_den = numrat_denref(bound_numref(linexpr->cst->inf));
    neg_assign(tmp_num);
  }
  exact_div_assign(tmp_den, den, tmp_den);
  tmp_num *= tmp_den;
  inhomo = std::move(tmp_num);
}

Itv
to_Itv(itv_t itv) {
  Itv res;
  if (itv_is_top(itv))
    return res;
  if (itv_is_canonical_bottom(itv)) {
    res.set_empty();
    return res;
  }
  if (not bound_infty(itv->sup)) {
    Rational ub(numrat_numref(bound_numref(itv->sup)),
                numrat_denref(bound_numref(itv->sup)));
    res.set_ub(ub);
  }
  if (not bound_infty(itv->inf)) {
    Rational lb(numrat_numref(bound_numref(itv->inf)),
                numrat_denref(bound_numref(itv->inf)));
    neg_assign(lb);
    res.set_lb(lb);
  }
  return res;
}

Itv_Expr
to_Itv_Expr_no_inhomo(itv_linexpr_t* linexpr) {
  Itv_Expr res;
  auto& res_vars = res.first;
  auto& res_itvs = res.second;

  size_t i;
  ap_dim_t d;
  itv_ptr pitv;
  bool* peq;
  itv_linexpr_ForeachLinterm(linexpr, i, d, pitv, peq) {
    (void) peq; // silence warning;
    res_vars.push_back(Var(d));
    res_itvs.push_back(to_Itv(pitv));
  }
  return res;
}

/* Same as from_itv_linexpr, but returns by value and discards den.
   NOTE: scaled ==> den will be discarded */
Affine_Expr
to_scaled_Affine_Expr(itv_linexpr_t* linexpr, int mode) {
  Affine_Expr res;
  auto& le = res.expr;
  auto& inhomo = res.inhomo;
  Integer den; // will be discarded
  from_itv_linexpr(le, inhomo, den, linexpr, mode);
  return res;
}

/* ap_linexpr0_t => Affine_Expr
   linearize if mode != 0
   NOTE: scaled ==> denominator will be discarded */
Affine_Expr
to_scaled_Affine_Expr(itv_internal_t* intern, ap_linexpr0_t* c, int mode) {
  itv_linexpr_t linexpr;
  itv_linexpr_init(&linexpr, 0);
  itv_linexpr_set_ap_linexpr0(intern, &linexpr, c);
  auto res = to_scaled_Affine_Expr(&linexpr, mode);
  itv_linexpr_clear(&linexpr);
  return res;
}

/* itv_lincons_t => Con
   may raise cannot_convert, sets exact */
Con
to_Con(itv_lincons_t* lincons, bool allow_strict, bool& exact) {
  if (lincons->constyp == AP_CONS_EQMOD && numrat_sgn(lincons->num) != 0)
    throw cannot_convert();
  Con::Type type = from_ap_constyp(lincons->constyp);
  exact = true;
  if (!allow_strict && type == Con::STRICT_INEQUALITY) {
    exact = false;
    type = Con::NONSTRICT_INEQUALITY;
  }
  auto ae = to_scaled_Affine_Expr(&lincons->linexpr, 1);
  return Con(std::move(ae.expr), std::move(ae.inhomo), type);
}

/* itv_lincons_t => Con
   may raise cannot_convert, sets exact */
Con
to_Con(itv_internal_t* intern, ap_lincons0_t* c,
       bool allow_strict, bool& exact) {
  itv_lincons_t lincons;
  itv_lincons_init(&lincons);
  bool c_exact = itv_lincons_set_ap_lincons0(intern, &lincons, c);
  Con res = to_Con(&lincons, allow_strict, exact);
  exact = exact && c_exact;
  itv_lincons_clear(&lincons);
  return res;
}

/* itv_lincons0_array_t => Cons
   may set exact to false (constraints may be dropped or approximated) */
Cons
to_Cons(itv_lincons_array_t* a, bool allow_strict, bool& exact) {
  Cons res;
  exact = true;
  for (size_t i = 0; i < a->size; ++i) {
    try {
      bool i_exact = true;
      res.push_back(to_Con(&a->p[i], allow_strict, i_exact));
      exact = exact && i_exact;
    }
    catch (const cannot_convert&) {
      exact = false;
    }
  }
  return res;
}

/* ap_lincons0_array_t => Cons
   may set exact to false (constraints may be dropped or approximated) */
Cons
to_Cons(itv_internal_t* intern, ap_lincons0_array_t* a,
        bool allow_strict, bool& exact) {
  itv_lincons_array_t array;
  itv_lincons_array_init(&array, a->size);
  bool a_exact = itv_lincons_array_set_ap_lincons0_array(intern, &array, a);
  Cons res = to_Cons(&array, allow_strict, exact);
  exact = exact && a_exact;
  itv_lincons_array_clear(&array);
  return res;
}

bool
has_precise_conversion(ap_tcons0_array_t* t_array) {
  for (size_t i = 0; i < t_array->size; ++i) {
    ap_texpr0_t* expr = t_array->p[i].texpr0;
    if (!(ap_texpr0_is_interval_linear(expr) && ap_texpr0_is_scalar(expr)))
      return false;
    // FIXME: check constraint type.
  }
  return true;
}

/* ap_tcons0_array ==> Cons
   may set exact to false (constraints may be dropped or approximated) */
Cons
to_Cons(itv_internal_t* intern, ap_tcons0_array_t* t_array, size_t intdim,
        bool allow_strict, bool& exact) {
  assert(has_precise_conversion(t_array));
  ap_lincons0_array_t lin_array = ap_lincons0_array_make(t_array->size);
  itv_lincons_t itv;
  itv_lincons_init(&itv);
  for (size_t i = 0; i < t_array->size; ++i) {
    itv_intlinearize_ap_tcons0_intlinear(intern, &itv, &t_array->p[i]);
    itv_lincons_reduce_integer(intern, &itv, intdim);
    ap_lincons0_set_itv_lincons(intern, &lin_array.p[i], &itv);
  }
  itv_lincons_clear(&itv);
  Cons res = to_Cons(intern, &lin_array, allow_strict, exact);
  ap_lincons0_array_clear(&lin_array);
  return res;
}

/* ap_generator0_t => Gen
   may raise cannot_convert, sets exact */
Gen
to_Gen(itv_internal_t* intern, ap_generator0_t* g, bool& exact) {
  auto t = g->gentyp;
  if (t == AP_GEN_RAYMOD || t == AP_GEN_LINEMOD)
    throw cannot_convert();
  auto le0 = g->linexpr0;
  if (!ap_linexpr0_is_linear(le0))
    throw cannot_convert();

  exact = true;
  auto type = from_ap_gentyp(t);
  if (t == AP_GEN_VERTEX) {
    // vertices have a divisor
    itv_linexpr_t linexpr;
    itv_linexpr_init(&linexpr, 0);
    itv_linexpr_set_ap_linexpr0(intern, &linexpr, le0);
    Affine_Expr ae;
    Integer den;
    from_itv_linexpr(ae.expr, ae.inhomo, den, &linexpr, 1);
    itv_linexpr_clear(&linexpr);
    return Gen(type, std::move(ae.expr), std::move(den));
  } else {
    // rays and line do not have a divisor
    auto ae = to_scaled_Affine_Expr(intern, le0, 1);
    return Gen(type, std::move(ae.expr), 0);
  }
}

/* ap_generator0_array_t => Gens
   may raise cannot_convert, sets exact
*/
Gens
to_Gens(itv_internal_t* intern, ap_generator0_array_t* a, bool& exact) {
  // Checks that a ray/line has a non-zero coefficient
  auto is_valid_gen = [](ap_generator0_t* g) {
    if (g->gentyp == AP_GEN_VERTEX)
      return true;
    ap_linexpr0_t* e = g->linexpr0;
    size_t i;
    ap_dim_t dim;
    ap_coeff_t* coeff;
    ap_linexpr0_ForeachLinterm(e, i, dim, coeff) {
      if (not ap_coeff_zero(coeff))
        return true;
    }
    return false;
  };

  exact = true;
  Gens res;
  for (size_t i = 0; i < a->size; ++i) {
    if (is_valid_gen(&a->p[i])) {
      bool exact2 = true;
      res.push_back(to_Gen(intern, &a->p[i], exact2));
      exact &= exact2;
    }
  }
  return res;
}

} // namespace apron
} // namespace pplite
