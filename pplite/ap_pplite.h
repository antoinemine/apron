/*
 * ap_pplite.h
 *
 * APRON Library / PPLite library wrapper
 *
 * This is the (only) one .h to include to access all PPLite domains.
 *
 * Copyright (C) Antoine Mine' 2006
 * Copyright (C) 2018-2023 Enea Zaffanella <enea.zaffanella@unipr.it>
 *
 */

/*
 * This file is part of the APRON Library, released under GPL license.
 * Please read the COPYING file packaged in the distribution.
 */

#ifndef __APRON_PPLITE_H
#define __APRON_PPLITE_H

#include "ap_global0.h"

#ifdef __cplusplus
extern "C" {
#endif

ap_manager_t*
ap_pplite_manager_alloc(bool strict);
  /*
    Allocate an APRON manager for PPLite's domains

    If the Boolean parameter is true, abstract values generated with the
    manager can have strict constraints (like x > 0); otherwise, they
    are defined using only non-strict constraints. Managers and abstract
    values in strict and non-strict mode are incompatible.

    See also ap_pplite_manager_set_kind.
  */

  /* extra functions not in APRON managers */

void ap_pplite_manager_set_kind(ap_manager_t* man, const char* name);
  /* Sets specific implementation of polyhedra domain; this should
     be called before creating abstract0_t objects, since the
     different implementations are incompatible. Accepted values:
       "Poly" (default), "F_Poly", "U_Poly", "UF_Poly", "P_Set", "FP_Set"
     Adding suffix "_Stats" to the values above will trigger the
     gathering (and printing on program exit) of statistics on
     the abstract operators invoked.
  */
const char* ap_pplite_manager_get_kind(ap_manager_t* man);

void ap_pplite_manager_set_widen_spec(ap_manager_t* man, const char* name);
  /* Sets the widening specification used by domains; legal values are:
       - "safe": the Cousot&Cousot POPL77 specification, which does not
          assume the inclusion between arguments;
       - "risky": the Cousot&Cousot PLILP92 footnote 6 alternative
          specification, assuming the inclusion between arguments.
  */
const char* ap_pplite_manager_get_widen_spec(ap_manager_t* man);

  /* extra functions not in APRON abstract0 interface */

void
ap_pplite_abstract0_set_intdim(ap_manager_t* man,
                               ap_abstract0_t* a, int intdim);
  /* Helper method only used for debugging purposes.
     It changes the number of space dimensions in a
     that are considered integral dimensions.
  */

ap_abstract0_t*
ap_pplite_abstract0_split(ap_manager_t* man, ap_abstract0_t* a,
                          ap_lincons0_t* c, bool integral, bool strict);
  /* Splits abstract element [[a]] using the inequality constraint
     [[c]].  Let [[pos_c]] and [[neg_c]] be the positive and negative
     splitting components of constraint [[c]] (defined below); then
     the function returns a new abstract element [[res]] obtained by
     adding the negative component [[neg_c]] to input element [[a]];
     at the same time, it updates element [[a]] by adding the positive
     component [[pos_c]].

     If parameter [[integral]] is true, then the constraint [[c]] is
     assumed to evaluate to an integral value and [[pos_c]], [[neg_c]]
     are refined accordingly; e.g., c := (x < 5) is refined into
     pos_c := (x <= 4) and neg_c := (x >= 5).

     If parameter [[integral]] is false, then a rational split is computed,
     meaning that [[pos_c]] is equal to [[c]]; in this case, strict
     constraints are supported only if parameter [[strict]] is true
     and [[man]] supports NNC polyhedra; otherwise, both [[pos_c]] and
     [[neg_c]] are non-strict.

     Note: interval constraints are NOT supported.
  */

ap_abstract0_t*
ap_pplite_abstract0_widening_thresholds(ap_manager_t* man,
                                        ap_abstract0_t* a1,
                                        ap_abstract0_t* a2,
                                        ap_lincons0_array_t* array);

bool
ap_pplite_abstract0_is_disjunctive(ap_manager_t* man, ap_abstract0_t* a);
  /* Returns true if the abstract element is able to encode
     disjunctions (i.e., if it is a finite powerset of polyhedra).
  */

int
ap_pplite_abstract0_num_disjuncts(ap_manager_t* man, ap_abstract0_t* a);
  /* Returns the number of disjuncts in abstract value a
     (result can be greater than 1 only for powersets of polyhedra).
  */

ap_lincons0_array_t
ap_pplite_abstract0_disj_to_lincons_array(ap_manager_t* man,
                                          ap_abstract0_t* a, int n);
  /* Returns an array of linear constraints for the n-th disjunct */

ap_tcons0_array_t
ap_pplite_abstract0_disj_to_tcons_array(ap_manager_t* man,
                                        ap_abstract0_t* a, int n);
  /* Returns an array of tree constraints for the n-th disjunct */

bool
ap_pplite_abstract0_geom_subseteq(ap_manager_t* man,
                                  ap_abstract0_t* a1, ap_abstract0_t* a2);
  /* Geometric inclusion test; exact for powersets */

void
ap_pplite_abstract0_collapse(ap_manager_t* man, ap_abstract0_t* a, int n);
  /* Modifies a to have at most n disjuncts. */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __APRON_PPLITE_H */
