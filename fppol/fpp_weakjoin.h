/*
 * join.h
 *
 * Functions for strong join & weak joins
 *
 * APRON Library / FPPol Domain
 *
 * Copyright (C) Liqian CHEN' 2008-
 *
 */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution.
*/

#ifndef _JOIN_H_
#define _JOIN_H_

#include <glpk.h> /* linear programming lib */
#include "fpp.h"
#include "fpp_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

fpp_t *fppol_weak_join_template(fpp_internal_t* pr, fpp_t *fpp01, fpp_t *fpp02);
fpp_t *fppol_weak_join_envbnds(fpp_internal_t* pr, fpp_t *fpp01, fpp_t *fpp02);
fpp_t *fppol_weak_join_nenvbnds(fpp_internal_t* pr, fpp_t *fpp01, fpp_t *fpp02);
numdbl_t *fppol_convex_hull_new_cons(fpp_internal_t* pr, fpp_t *fpp01, fpp_t *fpp02, numdbl_t *env, unsigned nenv, numdbl_t *chbnds, unsigned nchbnds, unsigned *nNewCons);
void fppol_tighten_tmplts(fpp_internal_t* pr, fpp_t *fpp01,  fpp_t *fpp02, unsigned nTmplts, numdbl_t *tmplts);
fpp_t *fppol_weak_join_envTVPI(fpp_internal_t* pr, fpp_t *fpp01, fpp_t *fpp02);
fpp_t *fppol_weak_join_each_overapprox(fpp_internal_t* pr, fpp_t *fpp01, fpp_t *fpp02);
fpp_t *fppol_weak_joins(fpp_internal_t* pr, fpp_t *fpp01, fpp_t *fpp02);
numdbl_t fppol_sparse_factor(fpp_t *fpp);

#ifdef __cplusplus
}
#endif

#endif
