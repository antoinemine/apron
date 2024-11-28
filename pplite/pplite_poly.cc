/*
 * pplite_poly.cc
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

#include "ap_generic.h"
#include "ap_linearize.h"
#include "ap_pplite.h"
#include "pplite_user.hh"
#include "pplite_poly.hh"

#include <cassert>

namespace pplite {
namespace apron {

/* Internal structure in managers */
struct pplite_internal {
  using Abs_Poly = dynamic::Abs_Poly;

  bool strict; /* strict => use NNC polyhedra */
  Abs_Poly::Kind kind; /* use this kind of polyhedra */
  Widen_Spec widen_spec; /* use this widening specification */
  itv_internal_t* itv;

  explicit pplite_internal(bool s)
    : strict(s),
      kind(Abs_Poly::Kind::POLY),
      widen_spec(Widen_Spec::SAFE),
      itv(itv_internal_alloc())
  {}
  ~pplite_internal() {
    itv_internal_free(itv);
    // FLINT uses custom allocator: clean up its cache.
    if (using_FLINT())
      flint_cleanup();
  }

  pplite_internal(const pplite_internal&) = delete;
  pplite_internal(pplite_internal&&) = delete;
  pplite_internal& operator=(const pplite_internal&) = delete;
  pplite_internal& operator=(pplite_internal&&) = delete;
};

extern "C" void
ap_pplite_internal_free(void* internal) {
  delete static_cast<pplite_internal*>(internal);
}

namespace {

inline pplite_internal*
get_internal(ap_manager_t* man) {
  return static_cast<pplite_internal*>(man->internal);
}

inline void
set_flags(ap_manager_t* man, bool flag) {
  man->result.flag_exact = flag;
  man->result.flag_best = flag;
}

/* helper to force use of mpq scalars */
inline ap_interval_t*
ap_MPQ_interval_alloc() {
  ap_interval_t* r = ap_interval_alloc();
  ap_interval_reinit(r, AP_SCALAR_MPQ);
  return r;
}

/* helper to force use of mpq scalars */
inline ap_interval_t**
ap_MPQ_interval_array_alloc(dim_type dim) {
  ap_interval_t** r = ap_interval_array_alloc(dim);
  for (int i = 0; i < dim; ++i)
    ap_interval_reinit(r[i], AP_SCALAR_MPQ);
  return r;
}

inline void
to_ap_interval(ap_interval_t* r, const Itv& itv) {
  if (itv.is_empty()) {
    ap_interval_set_bottom(r);
    return;
  }
  if (itv.inf_ub())
    ap_scalar_set_infty(r->sup, 1);
  else
    to_scalar(r->sup, itv.ub);
  if (itv.inf_lb())
    ap_scalar_set_infty(r->inf, -1);
  else
    to_scalar(r->inf, itv.lb);
}

inline void
compute_ap_interval(ap_interval_t* r, pplite_poly* a, int dim) {
  Itv itv = a->p->get_bounds(Var(dim));
  if (!itv.is_empty() && dim < static_cast<dim_type>(a->intdim))
    itv.refine_as_integral();
  to_ap_interval(r, itv);
}

// Note: returns <bool, ptr> where first element is true if either
// the polyhedron is empty or it would become empty by integral refinement;
// in those cases the itv array is NOT allocated (i.e., ptr is nullptr).
std::pair<bool, itv_t*>
to_itv_array(pplite_poly* a) {
  BBox bbox = a->p->get_bounding_box();
  bbox.refine_as_integral(0, a->intdim);
  if (bbox.is_empty())
    return { true, nullptr };
  auto dim = bbox.space_dim();
  auto env = itv_array_alloc(dim);
  mpq_t temp;
  mpq_init(temp);
  for (auto i = 0; i != dim; ++i) {
    if (bbox.inf_ub(i))
      bound_set_infty(env[i]->sup, 1);
    else {
      mpq_set_Rational(temp, bbox.ub(i));
      bound_set_int(env[i]->sup, 0);
      numrat_set_mpq(env[i]->sup, temp);
    }
    if (bbox.inf_lb(i))
      bound_set_infty(env[i]->inf, -1);
    else {
      mpq_set_Rational(temp, bbox.lb(i));
      bound_set_int(env[i]->inf, 0);
      numrat_set_mpq(env[i]->inf, temp);
      numrat_neg(env[i]->inf, env[i]->inf);
    }
  }
  mpq_clear(temp);
  return { false, env };
}

/* Helper to be used in catch clauses: whole universe as a generator system */
ap_generator0_array_t
generator_universe(dim_type dim) {
  auto res = ap_generator0_array_make(dim+1);
  /* origin vertex */
  res.p[0] = ap_generator0_make(AP_GEN_VERTEX,
                                ap_linexpr0_alloc(AP_LINEXPR_SPARSE, 0));
  /* one line for each dimension */
  for (auto i = 0; i < dim; ++i) {
    ap_linexpr0_t* e = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, 1);
    e->p.linterm[0].dim = i;
    ap_coeff_set_scalar_int(&e->p.linterm[0].coeff, 1);
    res.p[i+1] = ap_generator0_make(AP_GEN_LINE, e);
  }
  return res;
}

/* Helper to be used in catch clauses: whole universe as a box */
ap_interval_t**
box_universe(dim_type dim) {
  auto res = ap_MPQ_interval_array_alloc(dim);
  for (auto i = 0; i < dim; ++i)
    ap_interval_set_top(res[i]);
  return res;
}

inline Widen_Impl
ap_pplite_select_widening(ap_manager_t* man) {
  int algo = man->option.funopt[AP_FUNID_WIDENING].algorithm;
  switch (algo) {
  case 0:
    return Widen_Impl::H79;
  case 1:
    return Widen_Impl::BHRZ03;
  case 2:
    return Widen_Impl::BOXED_H79;
  default:
    return Widen_Impl::H79;
  }
}

} // namespace

pplite_poly::pplite_poly(ap_manager_t* man,
                         size_t intdim, size_t realdim,
                         Spec_Elem spec)
  : p(nullptr), intdim(intdim) {
  auto internal = get_internal(man);
  auto kind = internal->kind;
  auto topol = internal->strict ? Topol::NNC : Topol::CLOSED;
  p.reset(pplite::dynamic::make_poly(kind, intdim + realdim, spec, topol));
}

/* returns a polyhedron, of specified size if possible */
#define CATCH_WITH_DIM(funid, intdim, realdim)                          \
  catch (const cannot_convert& w) {                                     \
    /* bailing out, not an error */                                     \
    set_flags(man, false);                                              \
    return new pplite_poly(man, intdim, realdim);                       \
  }                                                                     \
  catch (const std::logic_error& e) {                                   \
    /* actual error */                                                  \
    ap_manager_raise_exception(man, AP_EXC_INVALID_ARGUMENT,            \
                               funid, e.what());                        \
    return new pplite_poly(man, intdim, realdim);                       \
  }

/* returns a polyhedron, with size compatible with poly if possible */
#define CATCH_WITH_POLY(funid, poly)                                    \
  CATCH_WITH_DIM(funid, (poly)->intdim,                                 \
                 (poly)->p->space_dim() - (poly)->intdim)

/* returns v */
#define CATCH_WITH_VAL(funid, v)                                        \
  catch (const cannot_convert&) {                                       \
    /* bailing out, not an error */                                     \
    set_flags(man, false);                                              \
    return v;                                                           \
  }                                                                     \
  catch (const std::logic_error& e) {                                   \
    /* actual error */                                                  \
    ap_manager_raise_exception(man, AP_EXC_INVALID_ARGUMENT,            \
                               funid, e.what());                        \
    return v;                                                           \
  }

/* returns nothing */
#define CATCH_WITH_VOID(funid) CATCH_WITH_VAL(funid, )

/* prints message */
#define CATCH_WITH_MSG(funid)                                           \
  catch (const cannot_convert&) {                                       \
    /* bailing out, not an error */                                     \
    fprintf(stream, "!exception!");                                     \
  }                                                                     \
  catch (const std::logic_error& e) {                                   \
    /* actual error */                                                  \
    ap_manager_raise_exception(man, AP_EXC_INVALID_ARGUMENT,            \
                               funid, e.what());                        \
    fprintf(stream, "!exception!");                                     \
  }


/* Implementation of APRON's library functions */

extern "C" pplite_poly*
ap_pplite_poly_copy(ap_manager_t* man, pplite_poly* a) {
  set_flags(man, true);
  try {
    return new pplite_poly(man, a);
  }
  CATCH_WITH_POLY(AP_FUNID_COPY, a);
}

extern "C" void
ap_pplite_poly_free(ap_manager_t* man, pplite_poly* a) {
  set_flags(man, true);
  delete a;
}

extern "C" size_t
ap_pplite_poly_size(ap_manager_t* man, pplite_poly* a) {
  set_flags(man, true);
  try {
    return a->p->get_memory_in_bytes();
  }
  CATCH_WITH_VAL(AP_FUNID_ASIZE, 0);
}

extern "C" void
ap_pplite_poly_minimize(ap_manager_t* man, pplite_poly* a) {
  set_flags(man, true);
  try {
    a->p->minimize();
  }
  CATCH_WITH_VOID(AP_FUNID_MINIMIZE);
}

extern "C" void
ap_pplite_poly_canonicalize(ap_manager_t* man, pplite_poly* a) {
  set_flags(man, true);
  try {
    a->p->minimize();
  }
  CATCH_WITH_VOID(AP_FUNID_CANONICALIZE);
}

extern "C" int
ap_pplite_poly_hash(ap_manager_t* man, pplite_poly* a) {
  set_flags(man, true);
  try {
    // FIXME: forcing a size_t into an int
    return a->p->hash();
  }
  CATCH_WITH_VAL(AP_FUNID_HASH, 0);
}

extern "C" void
ap_pplite_poly_approximate(ap_manager_t* man, pplite_poly*, int) {
  // Do noting
  set_flags(man, true);
  return;
}

extern "C" void
ap_pplite_poly_fprint(FILE* stream,
                      ap_manager_t* man, pplite_poly* a,
                      char** name_of_dim) {
  set_flags(man, true);
  try {
    if (a->p->is_empty()) {
      ap_dimension_t dim = ap_pplite_poly_dimension(man, a);
      fprintf(stream,"empty polyhedron of dim (%lu,%lu)\n",
              static_cast<unsigned long>(dim.intdim),
              static_cast<unsigned long>(dim.realdim));
    } else {
      a->p->minimize();
      auto num_disj = a->p->num_disjuncts();
      assert(num_disj > 0);
      auto ar = to_lincons_array(a->p->disjunct_cons(0));
      fprintf(stream, "{\n");
      ap_lincons0_array_fprint(stream, &ar, name_of_dim);
      fprintf(stream, "}\n");
      ap_lincons0_array_clear(&ar);
      for (auto i = 1; i != num_disj; ++i) {
        ar = to_lincons_array(a->p->disjunct_cons(i));
        fprintf(stream, "  ||\n{\n");
        ap_lincons0_array_fprint(stream, &ar, name_of_dim);
        fprintf(stream, "}\n");
        ap_lincons0_array_clear(&ar);
      }
    }
  }
  CATCH_WITH_MSG(AP_FUNID_FPRINT);
}

/* NOT IMPLEMENTED! */
extern "C" void
ap_pplite_poly_fprintdiff(FILE* stream,
                          ap_manager_t* man,
                          pplite_poly* a1, pplite_poly* a2,
                          char** name_of_dim) {
  (void) stream; (void) a1; (void) a2; (void) name_of_dim;
  ap_manager_raise_exception(man, AP_EXC_NOT_IMPLEMENTED,
                             AP_FUNID_FPRINTDIFF, "not implemented");
}

/* fdump will print the unminimized contraint & generator systems */
extern "C" void
ap_pplite_poly_fdump(FILE* stream, ap_manager_t* man, pplite_poly* a) {
  set_flags(man, true);
  try {
    auto is_disj = a->p->is_disjunctive();
    auto num_disj = a->p->num_disjuncts();
    if (is_disj) {
      fprintf(stream, "powerset element having %d disjuncts\n", num_disj);
      for (auto d = 0; d != num_disj; ++d) {
        auto ar_d = to_lincons_array(a->p->disjunct_cons(d));
        /* dump only constraints */
        fprintf(stream, "disjunct %d constraints: ", d);
        ap_lincons0_array_fprint(stream, &ar_d, nullptr);
        ap_lincons0_array_clear(&ar_d);
      }
    } else {
      /* dump constraints */
      fprintf(stream, "constraints: ");
      auto ar = to_lincons_array(a->p->cons());
      ap_lincons0_array_fprint(stream, &ar, nullptr);
      ap_lincons0_array_clear(&ar);
      /* dump generators */
      fprintf(stream, "generators: ");
      bool exact = true;
      auto ar2 = to_generator_array(a->p->gens(), exact);
      if (!exact)
        fprintf(stream, "(inexact) ");
      ap_generator0_array_fprint(stream, &ar2, nullptr);
      ap_generator0_array_clear(&ar2);
    }
  }
  CATCH_WITH_MSG(AP_FUNID_FDUMP);
}

/* NOT IMPLEMENTED! */
extern "C" ap_membuf_t
ap_pplite_poly_serialize_raw(ap_manager_t* man, pplite_poly* a) {
  ap_membuf_t membuf;
  membuf.ptr = nullptr;
  membuf.size = 0;
  (void) a;
  ap_manager_raise_exception(man, AP_EXC_NOT_IMPLEMENTED,
                             AP_FUNID_SERIALIZE_RAW, "not implemented");
  return membuf;
}

/* NOT IMPLEMENTED! */
extern "C" pplite_poly*
ap_pplite_poly_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size) {
  (void) ptr; (void) size;
  ap_manager_raise_exception(man, AP_EXC_NOT_IMPLEMENTED,
                             AP_FUNID_DESERIALIZE_RAW, "not implemented");
  return nullptr;
}

extern "C" pplite_poly*
ap_pplite_poly_bottom(ap_manager_t* man, size_t intdim, size_t realdim) {
  set_flags(man, true);
  try {
    return new pplite_poly(man, intdim, realdim, Spec_Elem::EMPTY);
  }
  CATCH_WITH_DIM(AP_FUNID_BOTTOM, intdim, realdim);
}

extern "C" pplite_poly*
ap_pplite_poly_top(ap_manager_t* man, size_t intdim, size_t realdim) {
  set_flags(man, true);
  try {
    return new pplite_poly(man, intdim, realdim, Spec_Elem::UNIVERSE);
  }
  CATCH_WITH_DIM(AP_FUNID_TOP, intdim, realdim);
}

extern "C" pplite_poly*
ap_pplite_poly_of_box(ap_manager_t* man,
                      size_t intdim, size_t realdim,
                      ap_interval_t** tinterval) {
  set_flags(man, (intdim == 0));
  try {
    auto a = new pplite_poly(man, intdim, realdim);
    bool exact = true;
    Cons cs = to_Cons(tinterval, intdim, realdim, exact);
    if (!exact)
      set_flags(man, false);
    a->p->add_cons(cs);
    return a;
  }
  CATCH_WITH_DIM(AP_FUNID_OF_BOX, intdim, realdim);
}

extern "C" ap_dimension_t
ap_pplite_poly_dimension(ap_manager_t* man, pplite_poly* a) {
  set_flags(man, true);
  ap_dimension_t d = { 0, 0 };
  try {
    d.intdim = a->intdim;
    d.realdim = a->p->space_dim() - a->intdim;
    return d;
  }
  CATCH_WITH_VAL(AP_FUNID_DIMENSION, d);
}

extern "C" bool
ap_pplite_poly_is_bottom(ap_manager_t* man, pplite_poly* a) {
  set_flags(man, (a->intdim == 0));
  try {
    return a->p->is_empty();
  }
  CATCH_WITH_VAL(AP_FUNID_IS_BOTTOM, false);
}

extern "C" bool
ap_pplite_poly_is_top(ap_manager_t* man, pplite_poly* a) {
  set_flags(man, true);
  try {
    return a->p->is_universe();
  }
  CATCH_WITH_VAL(AP_FUNID_IS_TOP, false);
}

extern "C" bool
ap_pplite_poly_is_leq(ap_manager_t* man,
                      pplite_poly* a1, pplite_poly* a2) {
  set_flags(man, (a1->intdim == 0));
  try {
    return a2->p->contains(*a1->p);
  }
  CATCH_WITH_VAL(AP_FUNID_IS_LEQ, false);
}

extern "C" bool
ap_pplite_poly_is_eq(ap_manager_t* man,
                     pplite_poly* a1, pplite_poly* a2) {
  set_flags(man, (a1->intdim == 0));
  try {
    return a1->p->equals(*a2->p);
  }
  CATCH_WITH_VAL(AP_FUNID_IS_EQ, false);
}

extern "C" bool
ap_pplite_poly_sat_lincons(ap_manager_t* man, pplite_poly* a,
                           ap_lincons0_t* lincons0) {
  auto intern = get_internal(man);

  try {
    if (a->p->is_empty()) {
      set_flags(man, true);
      return true;
    }

    if (lincons0->constyp == AP_CONS_DISEQ) {
      // FIXME: special cases.
      set_flags(man, false);
      return false;
    }
    if (lincons0->constyp == AP_CONS_EQ
        && not ap_linexpr0_is_linear(lincons0->linexpr0)) {
      set_flags(man, false);
      return false;
    }
    if (lincons0->constyp == AP_CONS_EQMOD) {
      if (ap_scalar_sgn(lincons0->scalar) != 0
          || not ap_linexpr0_is_linear(lincons0->linexpr0)) {
        set_flags(man, false);
        return false;
      }
    }

    itv_lincons_t lincons;
    itv_lincons_init(&lincons);
    bool exact = itv_lincons_set_ap_lincons0(intern->itv, &lincons, lincons0);
    if (itv_sat_lincons_is_false(intern->itv, &lincons)) {
      itv_lincons_clear(&lincons);
      set_flags(man, exact);
      return false;
    }
    if (not itv_lincons_is_quasilinear(&lincons)) {
      auto p = to_itv_array(a);
      bool empty = p.first;
      const auto& env = p.second;
      if (empty) {
        assert(env == nullptr);
        // found empty after integral refinement
        assert(a->intdim > 0);
        set_flags(man, true);
        return true;
      }
      bool exact2 = itv_quasilinearize_lincons(intern->itv, &lincons,
                                               env, false);
      exact = exact && exact2;
      itv_array_free(env, a->p->space_dim());
    }
    if (itv_sat_lincons_is_false(intern->itv, &lincons)) {
      itv_lincons_clear(&lincons);
      set_flags(man, true);
      return false;
    }

    auto type = from_ap_constyp(lincons.constyp);
    if (!intern->strict && type == Con::STRICT_INEQUALITY) {
      exact = false;
      type = Con::NONSTRICT_INEQUALITY;
    }
    assert(!bound_infty(lincons.linexpr.cst->inf));
    auto ae = to_scaled_Affine_Expr(&lincons.linexpr, -1);
    itv_lincons_clear(&lincons);
    auto con = Con(std::move(ae.expr), std::move(ae.inhomo), type);
    Poly_Con_Rel relation = a->p->relation_with(con);
    set_flags(man, exact);
    return relation.implies(Poly_Con_Rel::is_included());
  }
  CATCH_WITH_VAL(AP_FUNID_SAT_LINCONS, false);
}

extern "C" bool
ap_pplite_poly_sat_tcons(ap_manager_t* man, pplite_poly* a,
                         ap_tcons0_t* cons) {
  return ap_generic_sat_tcons(man, a, cons, AP_SCALAR_MPQ, true);
}

extern "C" bool
ap_pplite_poly_sat_interval(ap_manager_t* man, pplite_poly* a,
                            ap_dim_t dim, ap_interval_t* i) {
  try {
    if (a->p->is_empty()) {
      set_flags(man, true);
      return true;
    }
  }
  CATCH_WITH_VAL(AP_FUNID_SAT_INTERVAL, false);
  set_flags(man, false);
  bool res;
  auto p_i = ap_MPQ_interval_alloc();
  try {
    compute_ap_interval(p_i, a, dim);
    res = ap_interval_is_leq(p_i, i);
    ap_interval_free(p_i);
  }
  CATCH_WITH_VAL(AP_FUNID_SAT_INTERVAL, (ap_interval_free(p_i), false));
  return res;
}

extern "C" bool
ap_pplite_poly_is_dimension_unconstrained(ap_manager_t* man,
                                          pplite_poly* a,
                                          ap_dim_t dim) {
  set_flags(man, true);
  try {
    return not a->p->constrains(Var(dim));
  }
  CATCH_WITH_VAL(AP_FUNID_IS_DIMENSION_UNCONSTRAINED, false);
}

extern "C" ap_interval_t*
ap_pplite_poly_bound_linexpr(ap_manager_t* man,
                             pplite_poly* a,
                             ap_linexpr0_t* expr) {
  set_flags(man, (a->intdim == 0));
  ap_interval_t* r = ap_MPQ_interval_alloc();
  try {
    /* check for empty case */
    if (a->p->is_empty()) {
      ap_interval_set_bottom(r);
      set_flags(man, true);
      return r;
    }

    /* not empty */
    auto intern = get_internal(man);
    itv_linexpr_t linexpr;
    itv_linexpr_init(&linexpr, 0);
    bool exact = itv_linexpr_set_ap_linexpr0(intern->itv, &linexpr, expr);

    if (linexpr.size == 0) {
      ap_interval_set_itv(intern->itv, r, linexpr.cst);
    } else {
      Itv itv_res;
      // Copy into itv_cst the constant of linexpr.
      Itv itv_cst = to_Itv(linexpr.cst);
      // Reset to zero the constant of linexpr.
      itv_set_int(linexpr.cst, 0);
      if (itv_linexpr_is_scalar(&linexpr)) {
        // Scalar: covert it to a Linear_Expr.
        Linear_Expr le;
        Integer inhomo, den;
        from_itv_linexpr(le, inhomo, den, &linexpr, 1);
        assert(inhomo == 0);
        itv_res = a->p->get_bounds(Affine_Expr(le, inhomo));
        if (not den.is_one())
          itv_res.mul_assign(Rational(1, den));
      } else {
        // Not scalar: use the itvs get_bounds method
        Itv_Expr ie = to_Itv_Expr_no_inhomo(&linexpr);
        itv_res = a->p->get_bounds(ie);
      }
      assert(!itv_res.is_empty());
      // Combine itv_res and itv_cst.
      itv_res.add_assign(itv_cst);
      // Set result upper bound
      if (itv_res.inf_ub())
        ap_scalar_set_infty(r->sup, 1);
      else
        to_scalar(r->sup, itv_res.ub);
      // Set lower bound
      if (itv_res.inf_lb())
        ap_scalar_set_infty(r->inf, -1);
      else
        to_scalar(r->inf, itv_res.lb);
    }

    itv_linexpr_clear(&linexpr);
    if (!exact)
      set_flags(man, false);
  }
  CATCH_WITH_VAL(AP_FUNID_BOUND_LINEXPR,(ap_interval_set_top(r),r));
  return r;
}

extern "C" ap_interval_t*
ap_pplite_poly_bound_texpr(ap_manager_t* man,
                           pplite_poly* a, ap_texpr0_t* expr) {
  ap_abstract0_t a0;
  a0.value = a;
  a0.man = man;

  bool exact;
  ap_linexpr0_t* le0
    = ap_intlinearize_texpr0(man, &a0, expr, &exact, AP_SCALAR_MPQ, false);
  ap_interval_t* r = ap_pplite_poly_bound_linexpr(man, a, le0);
  ap_linexpr0_free(le0);
  if (!exact)
    set_flags(man, false);
  return r;
}

extern "C" ap_interval_t*
ap_pplite_poly_bound_dimension(ap_manager_t* man,
                               pplite_poly* a, ap_dim_t dim) {
  set_flags(man, (a->intdim == 0));
  ap_interval_t* r = ap_MPQ_interval_alloc();
  try {
    compute_ap_interval(r, a, dim);
    if (ap_interval_is_bottom(r))
      set_flags(man, true);
    return r;
  }
  CATCH_WITH_VAL(AP_FUNID_BOUND_DIMENSION, (ap_interval_set_top(r),r));
}

extern "C" ap_lincons0_array_t
ap_pplite_poly_to_lincons_array(ap_manager_t* man, pplite_poly* a) {
  set_flags(man, (a->intdim == 0));
  try {
    a->p->minimize();
    auto nd = a->p->num_disjuncts();
    if (nd > 1)
      throw std::invalid_argument("to_lincons: powerset has many disjuncts");
    return to_lincons_array(a->p->cons());
  }
  CATCH_WITH_VAL(AP_FUNID_TO_LINCONS_ARRAY, ap_lincons0_array_make(0));
}

extern "C" ap_tcons0_array_t
ap_pplite_poly_to_tcons_array(ap_manager_t* man, pplite_poly* a) {
  return ap_generic_to_tcons_array(man, a);
}

extern "C" ap_interval_t**
ap_pplite_poly_to_box(ap_manager_t* man, pplite_poly* a) {
  set_flags(man, (a->intdim == 0));
  auto dim = a->p->space_dim();
  try {
    auto res = ap_MPQ_interval_array_alloc(dim);
    BBox bbox = a->p->get_bounding_box();
    bbox.refine_as_integral(0, a->intdim);
    if (bbox.is_empty()) {
      for (auto i = 0; i < dim; ++i)
        ap_interval_set_bottom(res[i]);
      set_flags(man, true);
      return res;
    }
    for (auto i = 0; i < dim; ++i)
      to_ap_interval(res[i], bbox.get_bounds(Var(i)));
    return res;
  }
  CATCH_WITH_VAL(AP_FUNID_TO_BOX, box_universe(dim));
}

extern "C" ap_generator0_array_t
ap_pplite_poly_to_generator_array(ap_manager_t* man,
                                  pplite_poly* a) {
  set_flags(man, (a->intdim == 0));
  try {
    bool exact = true;
    auto r = to_generator_array(a->p->gens(), exact);
    if (!exact)
      set_flags(man, false);
    return r;
  }
  CATCH_WITH_VAL(AP_FUNID_TO_GENERATOR_ARRAY,
                 generator_universe(a->p->space_dim()));
}

extern "C" pplite_poly*
ap_pplite_poly_meet(ap_manager_t* man, bool destructive,
                    pplite_poly* a1, pplite_poly* a2) {
  set_flags(man, (a1->intdim == 0));
  try {
    pplite_poly* r = destructive ? a1 : new pplite_poly(man, a1);
    r->p->intersection_assign(*a2->p);
    return r;
  }
  CATCH_WITH_POLY(AP_FUNID_MEET, a1);
}

extern "C" pplite_poly*
ap_pplite_poly_join(ap_manager_t* man, bool destructive,
                    pplite_poly* a1, pplite_poly* a2) {
  man->result.flag_exact = (a1->p->is_disjunctive());
  man->result.flag_best = (a1->intdim == 0);
  try {
    pplite_poly* r = destructive ? a1 : new pplite_poly(man, a1);
    r->p->join_assign(*a2->p);
    return r;
  }
  CATCH_WITH_POLY(AP_FUNID_JOIN, a1);
}

extern "C" pplite_poly*
ap_pplite_poly_meet_array(ap_manager_t* man,
                          pplite_poly** tab, size_t size) {
  assert(size >= 1);
  set_flags(man, (tab[0]->intdim == 0));
  try {
    auto r = new pplite_poly(man, tab[0]);
    for (size_t i = 1; i < size; ++i)
      r->p->intersection_assign(*tab[i]->p);
    return r;
  }
  CATCH_WITH_POLY(AP_FUNID_MEET_ARRAY, tab[0]);
}

extern "C" pplite_poly*
ap_pplite_poly_join_array(ap_manager_t* man,
                          pplite_poly** tab, size_t size) {
  assert(size >= 1);
  man->result.flag_exact = (size == 1) || (tab[0]->p->is_disjunctive());
  man->result.flag_best = (tab[0]->intdim == 0);
  try {
    auto r = new pplite_poly(man, tab[0]);
    for (size_t i = 1; i < size; ++i)
      r->p->join_assign(*tab[i]->p);
    return r;
  }
  CATCH_WITH_POLY(AP_FUNID_JOIN_ARRAY, tab[0]);
}

extern "C" pplite_poly*
ap_pplite_poly_meet_lincons_array(ap_manager_t* man,
                                  bool destructive,
                                  pplite_poly* a,
                                  ap_lincons0_array_t* array) {
  set_flags(man, (a->intdim == 0));
  try {
    pplite_poly* r = destructive ? a : new pplite_poly(man, a);
    if (a->p->is_empty()) {
      set_flags(man, true);
      return r;
    }

    itv_lincons_array_t array2;
    itv_lincons_array_init(&array2,array->size);
    auto intern = get_internal(man);
    bool exact = itv_lincons_array_set_ap_lincons0_array(intern->itv,
                                                         &array2, array);
    if (not itv_lincons_array_is_quasilinear(&array2)) {
      auto p = to_itv_array(a);
      bool empty = p.first;
      const auto& env = p.second;
      if (empty) {
        assert(env == nullptr);
        // found empty after refining integral bounds
        assert(a->intdim > 0);
        r->p->set_empty();
        set_flags(man, true);
        return r;
      }
      exact = itv_quasilinearize_lincons_array(intern->itv, &array2,
                                               env, true)
        && exact;
      itv_array_free(env, a->p->space_dim());
    }
    itv_linearize_lincons_array(intern->itv, &array2, true);
    itv_lincons_array_reduce_integer(intern->itv, &array2, a->intdim);
    bool cs_exact = true;
    Cons cs = to_Cons(&array2, intern->strict, cs_exact);
    exact = exact && cs_exact;
    if (!exact)
      set_flags(man, false);
    itv_lincons_array_clear(&array2);
    r->p->add_cons(cs);
    return r;
  }
  CATCH_WITH_POLY(AP_FUNID_MEET_LINCONS_ARRAY, a);
}

extern "C" pplite_poly*
ap_pplite_poly_meet_tcons_array(ap_manager_t* man,
                                bool destructive,
                                pplite_poly* a,
                                ap_tcons0_array_t* array) {
  // Avoid calling the generic implementation (which also causes the
  // computation of a bounding box for the polyhedron) when all the
  // constraints are linear and have integer (hence, scalar) coefficients.
  if (has_precise_conversion(array)) {
    auto intern = get_internal(man);
    bool exact = true;
    Cons cs = to_Cons(intern->itv, array, a->intdim,
                      intern->strict, exact);
    man->result.flag_exact = exact;
    man->result.flag_best = true;
    auto r = destructive ? a : new pplite_poly(man, a);
    r->p->add_cons(cs);
    return r;
  }
  /* If not precise, fall through to generic implementation. */
  using FunPtr = void* (*)(ap_manager_t*, bool, void*, ap_lincons0_array_t*);
  auto fun = reinterpret_cast<FunPtr>(&ap_pplite_poly_meet_lincons_array);
  auto r = ap_generic_meet_intlinearize_tcons_array
    (man, destructive, a, array, AP_SCALAR_MPQ, AP_LINEXPR_LINEAR, fun);
  return static_cast<pplite_poly*>(r);
}

extern "C" pplite_poly*
ap_pplite_poly_add_ray_array(ap_manager_t* man,
                             bool destructive,
                             pplite_poly* a,
                             ap_generator0_array_t* array) {
  set_flags(man, (a->intdim == 0));
  try {
    pplite_poly* r = destructive ? a : new pplite_poly(man, a);
    // Note: removing the check for emptyness, as we want to use
    // this method (e.g., in PAGAI) on an initially empty polyhedron.
    auto intern = get_internal(man);
    bool exact = true;
    Gens gs = to_Gens(intern->itv, array, exact);
    if (!exact)
      set_flags(man, false);
    r->p->add_gens(gs);
    return r;
  }
  CATCH_WITH_POLY(AP_FUNID_ADD_RAY_ARRAY, a);
}

extern "C" pplite_poly*
ap_pplite_poly_assign_linexpr_array(ap_manager_t* man,
                                    bool destructive,
                                    pplite_poly* org,
                                    ap_dim_t* tdim,
                                    ap_linexpr0_t** texpr,
                                    size_t size,
                                    pplite_poly* dest) {
  auto intern = get_internal(man);
  bool exact = true;
  for (size_t i = 0; i < size; ++i) {
    if (!ap_linexpr0_is_linear(texpr[i])) {
      exact = false;
      break;
    }
  }
  if (!exact || size > 1) {
    auto r = ap_generic_assign_linexpr_array(man, destructive, org,
                                             tdim, texpr, size, dest);
    return static_cast<pplite_poly*>(r);
  }

  set_flags(man, (org->intdim == 0));
  try {
    pplite_poly* r = destructive ? org : new pplite_poly(man, org);
    itv_linexpr_t linexpr;
    itv_linexpr_init(&linexpr, 0);
    itv_linexpr_set_ap_linexpr0(intern->itv, &linexpr, texpr[0]);
    Linear_Expr le;
    Integer inhomo, den;
    from_itv_linexpr(le, inhomo, den, &linexpr, 1);
    itv_linexpr_clear(&linexpr);
    r->p->affine_image(Var(tdim[0]), le, inhomo, den);
    if (dest)
      r->p->intersection_assign(*dest->p);
    return r;
  }
  CATCH_WITH_POLY(AP_FUNID_ASSIGN_LINEXPR_ARRAY, org);
}

extern "C" pplite_poly*
ap_pplite_poly_substitute_linexpr_array(ap_manager_t* man,
                                        bool destructive,
                                        pplite_poly* org,
                                        ap_dim_t* tdim,
                                        ap_linexpr0_t** texpr,
                                        size_t size,
                                        pplite_poly* dest) {
  bool exact = true;
  for (size_t i = 0; i < size; ++i) {
    if (!ap_linexpr0_is_linear(texpr[i])) {
      exact = false;
      break;
    }
  }

  if (!exact || size > 1) {
    auto r = ap_generic_substitute_linexpr_array(man, destructive, org,
                                                 tdim, texpr, size, dest);
    return static_cast<pplite_poly*>(r);
  }

  set_flags(man, (org->intdim == 0));
  try {
    auto intern = get_internal(man);
    pplite_poly* r = destructive ? org : new pplite_poly(man, org);
    itv_linexpr_t linexpr;
    itv_linexpr_init(&linexpr, 0);
    itv_linexpr_set_ap_linexpr0(intern->itv, &linexpr, texpr[0]);
    Linear_Expr le;
    Integer inhomo, den;
    from_itv_linexpr(le, inhomo, den, &linexpr, 1);
    itv_linexpr_clear(&linexpr);
    r->p->affine_preimage(Var(tdim[0]), le, inhomo, den);
    if (dest)
      r->p->intersection_assign(*dest->p);
    return r;
  }
  CATCH_WITH_POLY(AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY, org);
}

extern "C" pplite_poly*
ap_pplite_poly_assign_texpr_array(ap_manager_t* man,
                                  bool destructive,
                                  pplite_poly* org,
                                  ap_dim_t* tdim,
                                  ap_texpr0_t** texpr,
                                  size_t size,
                                  pplite_poly* dest) {
  bool exact = true;
  for (size_t i = 0; i < size; ++i) {
    if (not (ap_texpr0_is_interval_linear(texpr[i])
             &&
             ap_texpr0_is_scalar(texpr[i]))
        ) {
      exact = false;
      break;
    }
  }

  if (!exact) {
    auto r = ap_generic_assign_texpr_array(man, destructive, org,
                                           tdim, texpr, size, dest);
    return static_cast<pplite_poly*>(r);
  }

  set_flags(man, (org->intdim == 0));
  try {
    pplite_poly* r = destructive ? org : new pplite_poly(man, org);

    ap_abstract0_t a0;
    a0.value = org;
    a0.man = man;
    auto intern = get_internal(man);

    std::vector<Var> vars(size, Var(0));
    std::vector<Linear_Expr> exprs(size);
    std::vector<Integer> inhomos(size);
    std::vector<Integer> dens(size);

    itv_linexpr_t itv_expr;
    for (size_t i = 0; i < size; ++i) {
      ap_texpr0_t* texpr_i = texpr[i];
      ap_linexpr0_t* linexpr0
        = ap_intlinearize_texpr0(man, &a0, texpr_i, &exact,
                                 AP_SCALAR_MPQ, false);
      itv_linexpr_init(&itv_expr, 0);
      itv_linexpr_set_ap_linexpr0(intern->itv, &itv_expr, linexpr0);
      ap_linexpr0_free(linexpr0);
      vars[i] = Var(tdim[i]);
      from_itv_linexpr(exprs[i], inhomos[i], dens[i], &itv_expr, 1);
      itv_linexpr_clear(&itv_expr);
    }
    r->p->parallel_affine_image(vars, exprs, inhomos, dens);
    if (dest)
      r->p->intersection_assign(*dest->p);
    return r;
  }
  CATCH_WITH_POLY(AP_FUNID_ASSIGN_TEXPR_ARRAY, org);
}

extern "C" pplite_poly*
ap_pplite_poly_substitute_texpr_array(ap_manager_t* man,
                                      bool destructive,
                                      pplite_poly* org,
                                      ap_dim_t* tdim,
                                      ap_texpr0_t** texpr,
                                      size_t size,
                                      pplite_poly* dest) {
  auto r = ap_generic_substitute_texpr_array(man, destructive, org,
                                             tdim, texpr, size, dest);
  return static_cast<pplite_poly*>(r);
}

extern "C" pplite_poly*
ap_pplite_poly_forget_array(ap_manager_t* man,
                            bool destructive,
                            pplite_poly* a,
                            ap_dim_t* tdim, size_t size,
                            bool project) {
  set_flags(man, (a->intdim == 0));
  try {
    pplite_poly* r = destructive ? a : new pplite_poly(man, a);
    if (r->p->is_empty()) {
      set_flags(man, true);
      return r;
    }
    Index_Set vars;
    for (size_t i = 0; i < size; ++i)
      vars.set(tdim[i]);
    r->p->unconstrain(vars);
    if (project) {
      Cons cs;
      cs.reserve(size);
      for (auto i : vars)
        cs.push_back(Var(i) == 0);
      r->p->add_cons(cs);
    }
    return r;
  }
  CATCH_WITH_POLY(AP_FUNID_FORGET_ARRAY, a);
}

/* partial functions used in map_space_dims */
class ap_pplite_map {
protected:
  Dims pfunc;
public:
  ap_pplite_map(dim_type dim) : pfunc(dim, not_a_dim()) {}
  void set(size_t i, size_t j) {
    assert(i < pfunc.size());
    pfunc[i] = j;
  }
  void do_map(pplite_poly* r) const {
    r->p->map_space_dims(pfunc);
  }
};

extern "C" pplite_poly*
ap_pplite_poly_add_dimensions(ap_manager_t* man,
                              bool destructive,
                              pplite_poly* a,
                              ap_dimchange_t* dimchange,
                              bool project) {
  set_flags(man, true);
  size_t adddim = dimchange->intdim + dimchange->realdim;
  size_t olddim = a->p->space_dim();
  try {
    pplite_poly* r = destructive ? a : new pplite_poly(man, a);
    r->p->add_space_dims(adddim, project);
    /* reorder dimensions */
    ap_pplite_map map = ap_pplite_map(olddim + adddim);
    size_t i = 0, j = 0;
    for ( ; i < adddim; ++i) {
      for ( ; j < dimchange->dim[i]; ++j)
        map.set(j, i+j);
      map.set(olddim+i, i+j);
    }
    for ( ; j < olddim; ++j)
      map.set(j, i+j);
    map.do_map(r);
    r->intdim += dimchange->intdim;
    return r;
  }
  CATCH_WITH_DIM(AP_FUNID_ADD_DIMENSIONS,
                 a->intdim + dimchange->intdim,
                 olddim + dimchange->realdim - a->intdim);
}

extern "C" pplite_poly*
ap_pplite_poly_remove_dimensions(ap_manager_t* man,
                                 bool destructive,
                                 pplite_poly* a,
                                 ap_dimchange_t* dimchange) {
  set_flags(man, (a->intdim == 0));
  size_t deldim = dimchange->intdim + dimchange->realdim;
  size_t olddim = a->p->space_dim();
  try {
    pplite_poly* r = destructive ? a : new pplite_poly(man, a);
    /* reorder & drop dimensions */
    ap_pplite_map map = ap_pplite_map(olddim);
    for (size_t j = 0, i = 0; j < olddim; ++j)
      if (i < deldim && j == dimchange->dim[i])
        ++i;
      else
        map.set(j, j-i);
    map.do_map(r);
    r->intdim -= dimchange->intdim;
    return r;
  }
  CATCH_WITH_DIM(AP_FUNID_REMOVE_DIMENSIONS,
                 a->intdim - dimchange->intdim,
                 olddim - dimchange->realdim - a->intdim);
}

extern "C" pplite_poly*
ap_pplite_poly_permute_dimensions(ap_manager_t* man,
                                  bool destructive,
                                  pplite_poly* a,
                                  ap_dimperm_t* perm) {
  set_flags(man, true);
  try {
    pplite_poly* r = destructive ? a : new pplite_poly(man, a);
    ap_pplite_map map = ap_pplite_map(perm->size);
    for (size_t i = 0; i < perm->size; ++i)
      map.set(i, perm->dim[i]);
    map.do_map(r);
    return r;
  }
  CATCH_WITH_POLY(AP_FUNID_PERMUTE_DIMENSIONS, a);
}

extern "C" pplite_poly*
ap_pplite_poly_expand(ap_manager_t* man,
                      bool destructive,
                      pplite_poly* a,
                      ap_dim_t dim,
                      size_t n) {
  set_flags(man, true);
  try {
    size_t olddim = a->p->space_dim();
    pplite_poly* r = destructive ? a : new pplite_poly(man, a);
    r->p->expand_space_dim(Var(dim), n);
    if (dim < r->intdim) {
      /* expanded an integer dimension => needs some reordering */
      ap_pplite_map map = ap_pplite_map(olddim + n);
      for (size_t i = 0; i < r->intdim; ++i)
        map.set(i, i);
      for (size_t i = 0; i < n; ++i)
        map.set(olddim+i, r->intdim+i);
      for (size_t i = r->intdim; i < olddim; ++i)
        map.set(i, i+n);
      map.do_map(r);
      r->intdim += n;
    }
    return r;
  }
  CATCH_WITH_DIM(AP_FUNID_EXPAND, a->intdim,
                 a->p->space_dim() - a->intdim + n);
}

extern "C" pplite_poly*
ap_pplite_poly_fold(ap_manager_t* man, bool destructive,
                    pplite_poly* a, ap_dim_t* tdim, size_t size) {
  man->result.flag_exact = false;
  man->result.flag_best = (a->intdim == 0);
  try {
    pplite_poly* r = destructive ? a : new pplite_poly(man, a);
    Index_Set iset;
    assert(size > 0);
    for (size_t i = 1; i < size; ++i)
      iset.set(tdim[i]);
    r->p->fold_space_dims(iset, Var(tdim[0]));
    if (tdim[0] < a->intdim)
      r->intdim -= size - 1;
    return r;
  }
  CATCH_WITH_DIM(AP_FUNID_FOLD, a->intdim,
                 a->p->space_dim() - a->intdim - size);
}

extern "C" pplite_poly*
ap_pplite_poly_widening(ap_manager_t* man,
                        pplite_poly* a1, pplite_poly* a2) {
  set_flags(man, false);
  try {
    auto w_impl = ap_pplite_select_widening(man);
    auto w_spec = get_internal(man)->widen_spec;
    auto r = new pplite_poly(man, a2);
    r->p->widening_assign(*a1->p, w_impl, w_spec);
    return r;
  }
  CATCH_WITH_POLY(AP_FUNID_WIDENING, a1);
}


extern "C" pplite_poly*
ap_pplite_poly_closure(ap_manager_t* man, bool destructive, pplite_poly* a) {
  set_flags(man, true);
  try {
    pplite_poly* r = destructive ? a : new pplite_poly(man, a);
    r->p->topological_closure_assign();
    return r;
  }
  CATCH_WITH_POLY(AP_FUNID_CLOSURE,a);
}


/****** Special PPLite's manager functions not in Apron ******/

extern "C" void
ap_pplite_manager_set_kind(ap_manager_t* man, const char* name) {
  using namespace pplite::dynamic;
  auto kind = Abs_Poly::Kind::POLY;
  bool requires_stats = false;
  if (abs_poly_name_to_kind(name, kind, requires_stats)) {
    get_internal(man)->kind = kind;
    if (requires_stats)
      set_noisy_stats(true);
  } else
    std::cerr << "Unknown Abs_Poly::Kind name '" << name << "'\n";
}

extern "C" const char*
ap_pplite_manager_get_kind(ap_manager_t* man) {
  return pplite::dynamic::abs_poly_kind_to_name(get_internal(man)->kind);
}

extern "C" void
ap_pplite_manager_set_widen_spec(ap_manager_t* man, const char* name) {
  if (strcmp(name, "safe") == 0)
    get_internal(man)->widen_spec = Widen_Spec::SAFE;
  else if (strcmp(name, "risky") == 0)
    get_internal(man)->widen_spec = Widen_Spec::RISKY;
  else
    std::cerr << "Unknown widening spec '" << name << "'\n"
              << "(allowed values are: safe, risky).\n";
}

extern "C" const char*
ap_pplite_manager_get_widen_spec(ap_manager_t* man) {
  auto spec = get_internal(man)->widen_spec;
  return (spec == Widen_Spec::SAFE) ? "safe" : "risky";
}

/****** Extra functions not in APRON abstract0 interface */

/* Widening with thresholds */

namespace {

pplite_poly*
ap_pplite_poly_widening_threshold(ap_manager_t* man,
                                  pplite_poly* a1, pplite_poly* a2,
                                  ap_lincons0_array_t* array) {
  set_flags(man, false);
  try {
    auto intern = get_internal(man);
    bool exact = true;
    Cons cs = to_Cons(intern->itv, array, intern->strict, exact);
    auto w_impl = ap_pplite_select_widening(man);
    auto w_spec = intern->widen_spec;
    auto r = new pplite_poly(man, a2);
    r->p->widening_assign(*a1->p, cs, w_impl, w_spec);
    return r;
  }
  CATCH_WITH_POLY(AP_FUNID_WIDENING, a1);
}

} // namespace

extern "C" ap_abstract0_t*
ap_pplite_abstract0_widening_thresholds(ap_manager_t* man,
                                        ap_abstract0_t* a1,
                                        ap_abstract0_t* a2,
                                        ap_lincons0_array_t* array) {
  arg_assert(man->library == a1->man->library &&
             man->library == a2->man->library,
             return ap_pplite_invalid_abstract0(man, a1);,
             AP_FUNID_WIDENING);
  set_flags(man, false);
  auto ph1 = static_cast<pplite_poly*>(a1->value);
  auto ph2 = static_cast<pplite_poly*>(a2->value);
  auto res = ap_pplite_poly_widening_threshold(man, ph1, ph2, array);
  return ap_pplite_make_abstract0(man, res);
}

/* Splitting polyhedra on a linear constraint */

namespace {

pplite_poly*
ap_pplite_poly_split(ap_manager_t* man, pplite_poly* a,
                     ap_lincons0_t* c, bool integral, bool strict) {
  set_flags(man, (a->intdim == 0));
  try {
    auto intern = get_internal(man);
    if (strict && not intern->strict)
      throw std::invalid_argument("strict split with lose polyhedra");
    bool exact = true;
    Con con = to_Con(intern->itv, c, strict, exact);
    if (not exact)
      throw std::invalid_argument("split with invalid constraint");
    Topol t = strict ? Topol::NNC : Topol::CLOSED;
    auto res = new pplite_poly(man, 0, 0, Spec_Elem::EMPTY);
    if (integral)
      res->p.reset(a->p->integral_split(con));
    else
      res->p.reset(a->p->split(con, t));
    res->intdim = a->intdim;
    return res;
  }
  CATCH_WITH_POLY(AP_FUNID_MEET_LINCONS_ARRAY, a);
}

} // namespace

extern "C" ap_abstract0_t*
ap_pplite_abstract0_split(ap_manager_t* man, ap_abstract0_t* a,
                          ap_lincons0_t* c, bool integral, bool strict) {
  arg_assert(man->library == a->man->library,
             return ap_pplite_invalid_abstract0(man, a);,
             AP_FUNID_COPY);
  auto ph = static_cast<pplite_poly*>(a->value);
  auto res = ap_pplite_poly_split(man, ph, c, integral, strict);
  return ap_pplite_make_abstract0(man, res);
}

/* Resetting integer dimensions (for debugging only) */

extern "C" void
ap_pplite_abstract0_set_intdim(ap_manager_t* man,
                               ap_abstract0_t* a, int intdim) {
  auto ap = static_cast<pplite_poly*>(a->value);
  set_flags(man, false);
  assert(ap->p->space_dim() >= intdim);
  ap->intdim = intdim;
}

/* Extra functions for polyhedra powerset
   (trivial implementations for other domains) */

extern "C" bool
ap_pplite_abstract0_is_disjunctive(ap_manager_t* man, ap_abstract0_t* a) {
  auto ap = static_cast<pplite_poly*>(a->value);
  set_flags(man, true);
  return ap->p->is_disjunctive();
}

extern "C" int
ap_pplite_abstract0_num_disjuncts(ap_manager_t* man, ap_abstract0_t* a) {
  auto ap = static_cast<pplite_poly*>(a->value);
  set_flags(man, true);
  return ap->p->num_disjuncts();
}

extern "C" ap_lincons0_array_t
ap_pplite_abstract0_disj_to_lincons_array(ap_manager_t* man,
                                          ap_abstract0_t* a, int n) {
  auto ap = static_cast<pplite_poly*>(a->value);
  set_flags(man, (ap->intdim == 0));
  try {
    return to_lincons_array(ap->p->disjunct_cons(n));
  }
  CATCH_WITH_VAL(AP_FUNID_TO_LINCONS_ARRAY, ap_lincons0_array_make(0));
}

extern "C" ap_tcons0_array_t
ap_pplite_abstract0_disj_to_tcons_array(ap_manager_t* man,
                                        ap_abstract0_t* a, int n) {
  auto ap = static_cast<pplite_poly*>(a->value);
  set_flags(man, (ap->intdim == 0));
  try {
    ap_lincons0_array_t array = to_lincons_array(ap->p->disjunct_cons(n));
    ap_tcons0_array_t res = ap_tcons0_array_make(array.size);
    for (size_t i = 0; i < array.size; ++i)
      res.p[i] = ap_tcons0_from_lincons0(&array.p[i]);
    ap_lincons0_array_clear(&array);
    return res;
  }
  CATCH_WITH_VAL(AP_FUNID_TO_TCONS_ARRAY, ap_tcons0_array_make(0));
}

extern "C" bool
ap_pplite_abstract0_geom_subseteq(ap_manager_t* man,
                                  ap_abstract0_t* a1, ap_abstract0_t* a2) {
  arg_assert(man->library == a1->man->library &&
             man->library == a2->man->library,
             return ap_pplite_invalid_abstract0(man, a1);,
             AP_FUNID_IS_LEQ);
  auto ap1 = static_cast<pplite_poly*>(a1->value);
  auto ap2 = static_cast<pplite_poly*>(a2->value);
  set_flags(man, (ap1->intdim == 0));
  try {
    return ap2->p->geom_covers(*ap1->p);
  }
  CATCH_WITH_VAL(AP_FUNID_IS_LEQ, false);
}

extern "C" void
ap_pplite_abstract0_collapse(ap_manager_t* man, ap_abstract0_t* a, int n) {
  auto ap = static_cast<pplite_poly*>(a->value);
  set_flags(man, false);
  ap->p->collapse(n);
}


/* PPLite's manager allocation and setup */

extern "C" ap_manager_t*
ap_pplite_manager_alloc(bool strict) {
  auto intern = new pplite_internal(strict);
  const char* name = (strict ?
                      "PPLite polyhedra, strict mode" :
                      "PPLite polyhedra, loose mode");
  ap_manager_t* man = ap_manager_alloc(name, PPLITE_PACKAGE_VERSION,
                                       intern, &ap_pplite_internal_free);
  assert(man);

  man->funptr[AP_FUNID_COPY]
    = reinterpret_cast<void*>(ap_pplite_poly_copy);
  man->funptr[AP_FUNID_FREE]
    = reinterpret_cast<void*>(ap_pplite_poly_free);
  man->funptr[AP_FUNID_ASIZE]
    = reinterpret_cast<void*>(ap_pplite_poly_size);
  man->funptr[AP_FUNID_MINIMIZE]
    = reinterpret_cast<void*>(ap_pplite_poly_minimize);
  man->funptr[AP_FUNID_CANONICALIZE]
    = reinterpret_cast<void*>(ap_pplite_poly_canonicalize);
  man->funptr[AP_FUNID_HASH]
    = reinterpret_cast<void*>(ap_pplite_poly_hash);
  man->funptr[AP_FUNID_APPROXIMATE]
    = reinterpret_cast<void*>(ap_pplite_poly_approximate);
  man->funptr[AP_FUNID_FPRINT]
    = reinterpret_cast<void*>(ap_pplite_poly_fprint);
  man->funptr[AP_FUNID_FPRINTDIFF]
    = reinterpret_cast<void*>(ap_pplite_poly_fprintdiff);
  man->funptr[AP_FUNID_FDUMP]
    = reinterpret_cast<void*>(ap_pplite_poly_fdump);
  man->funptr[AP_FUNID_SERIALIZE_RAW]
    = reinterpret_cast<void*>(ap_pplite_poly_serialize_raw);
  man->funptr[AP_FUNID_DESERIALIZE_RAW]
    = reinterpret_cast<void*>(ap_pplite_poly_deserialize_raw);
  man->funptr[AP_FUNID_BOTTOM]
    = reinterpret_cast<void*>(ap_pplite_poly_bottom);
  man->funptr[AP_FUNID_TOP]
    = reinterpret_cast<void*>(ap_pplite_poly_top);
  man->funptr[AP_FUNID_OF_BOX]
    = reinterpret_cast<void*>(ap_pplite_poly_of_box);
  man->funptr[AP_FUNID_DIMENSION]
    = reinterpret_cast<void*>(ap_pplite_poly_dimension);
  man->funptr[AP_FUNID_IS_BOTTOM]
    = reinterpret_cast<void*>(ap_pplite_poly_is_bottom);
  man->funptr[AP_FUNID_IS_TOP]
    = reinterpret_cast<void*>(ap_pplite_poly_is_top);
  man->funptr[AP_FUNID_IS_LEQ]
    = reinterpret_cast<void*>(ap_pplite_poly_is_leq);
  man->funptr[AP_FUNID_IS_EQ]
    = reinterpret_cast<void*>(ap_pplite_poly_is_eq);
  man->funptr[AP_FUNID_IS_DIMENSION_UNCONSTRAINED]
    = reinterpret_cast<void*>(ap_pplite_poly_is_dimension_unconstrained);
  man->funptr[AP_FUNID_SAT_INTERVAL]
    = reinterpret_cast<void*>(ap_pplite_poly_sat_interval);
  man->funptr[AP_FUNID_SAT_LINCONS]
    = reinterpret_cast<void*>(ap_pplite_poly_sat_lincons);
  man->funptr[AP_FUNID_SAT_TCONS]
    = reinterpret_cast<void*>(ap_pplite_poly_sat_tcons);
  man->funptr[AP_FUNID_BOUND_DIMENSION]
    = reinterpret_cast<void*>(ap_pplite_poly_bound_dimension);
  man->funptr[AP_FUNID_BOUND_LINEXPR]
    = reinterpret_cast<void*>(ap_pplite_poly_bound_linexpr);
  man->funptr[AP_FUNID_BOUND_TEXPR]
    = reinterpret_cast<void*>(ap_pplite_poly_bound_texpr);
  man->funptr[AP_FUNID_TO_BOX]
    = reinterpret_cast<void*>(ap_pplite_poly_to_box);
  man->funptr[AP_FUNID_TO_LINCONS_ARRAY]
    = reinterpret_cast<void*>(ap_pplite_poly_to_lincons_array);
  man->funptr[AP_FUNID_TO_TCONS_ARRAY]
    = reinterpret_cast<void*>(ap_pplite_poly_to_tcons_array);
  man->funptr[AP_FUNID_TO_GENERATOR_ARRAY]
    = reinterpret_cast<void*>(ap_pplite_poly_to_generator_array);
  man->funptr[AP_FUNID_MEET]
    = reinterpret_cast<void*>(ap_pplite_poly_meet);
  man->funptr[AP_FUNID_MEET_ARRAY]
    = reinterpret_cast<void*>(ap_pplite_poly_meet_array);
  man->funptr[AP_FUNID_MEET_LINCONS_ARRAY]
    = reinterpret_cast<void*>(ap_pplite_poly_meet_lincons_array);
  man->funptr[AP_FUNID_MEET_TCONS_ARRAY]
    = reinterpret_cast<void*>(ap_pplite_poly_meet_tcons_array);
  man->funptr[AP_FUNID_JOIN]
    = reinterpret_cast<void*>(ap_pplite_poly_join);
  man->funptr[AP_FUNID_JOIN_ARRAY]
    = reinterpret_cast<void*>(ap_pplite_poly_join_array);
  man->funptr[AP_FUNID_ADD_RAY_ARRAY]
    = reinterpret_cast<void*>(ap_pplite_poly_add_ray_array);
  man->funptr[AP_FUNID_ASSIGN_LINEXPR_ARRAY]
    = reinterpret_cast<void*>(ap_pplite_poly_assign_linexpr_array);
  man->funptr[AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY]
    = reinterpret_cast<void*>(ap_pplite_poly_substitute_linexpr_array);
  man->funptr[AP_FUNID_ASSIGN_TEXPR_ARRAY]
    = reinterpret_cast<void*>(ap_pplite_poly_assign_texpr_array);
  man->funptr[AP_FUNID_SUBSTITUTE_TEXPR_ARRAY]
    = reinterpret_cast<void*>(ap_pplite_poly_substitute_texpr_array);
  man->funptr[AP_FUNID_ADD_DIMENSIONS]
    = reinterpret_cast<void*>(ap_pplite_poly_add_dimensions);
  man->funptr[AP_FUNID_REMOVE_DIMENSIONS]
    = reinterpret_cast<void*>(ap_pplite_poly_remove_dimensions);
  man->funptr[AP_FUNID_PERMUTE_DIMENSIONS]
    = reinterpret_cast<void*>(ap_pplite_poly_permute_dimensions);
  man->funptr[AP_FUNID_FORGET_ARRAY]
    = reinterpret_cast<void*>(ap_pplite_poly_forget_array);
  man->funptr[AP_FUNID_EXPAND]
    = reinterpret_cast<void*>(ap_pplite_poly_expand);
  man->funptr[AP_FUNID_FOLD]
    = reinterpret_cast<void*>(ap_pplite_poly_fold);
  man->funptr[AP_FUNID_WIDENING]
    = reinterpret_cast<void*>(ap_pplite_poly_widening);
  man->funptr[AP_FUNID_CLOSURE]
    = reinterpret_cast<void*>(ap_pplite_poly_closure);

  /* Make sure we do not proceed in case of exceptions */
  for (auto i = 0; i < AP_EXC_SIZE; ++i)
    ap_manager_set_abort_if_exception(man, static_cast<ap_exc_t>(i), true);

  return man;
}

} // namespace apron
} // namespace pplite
