/*
 * fppol_domain.h
 *
 * Functions for polyhdral operations   with floating point arithmetic.
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

#ifndef _FPPOL_DOMAIN_H_
#define _FPPOL_DOMAIN_H_

#include "fpp.h"
#include "fpp_internal.h"
#include "bv.h"
#ifdef __cplusplus
extern "C" {
#endif

fpp_t *fppol_joins(fpp_internal_t* pr, fpp_t *fpp01, fpp_t *fpp02);
fpp_t *fppol_strong_join(fpp_internal_t* pr, fpp_t *fpp1, fpp_t *fpp2);
fpp_t *fppol_construct_convex_hull(fpp_internal_t* pr, fpp_t *fpp1, fpp_t *fpp2, numdbl_t *env, unsigned nenvs);
numdbl_t *fppol_envelope(fpp_t *fpp1, fpp_t *fpp2, unsigned *NbEnvs, int *superset, char *env1Index,char *env2Index);
numdbl_t *fppol_envelope_half(fpp_t *fpp1, fpp_t *fpp2, unsigned *NbEnvs, unsigned *NbQuasi,
char *envIndex);

int fppol_infeasibility_byRLP(fpp_internal_t* pr, fpp_t *fpp);
int fppol_inclusion_byRLP(fpp_t *fpp1, fpp_t *fpp2);
int fppol_inclusion_byEpsilonLP(fpp_t *fpp1, fpp_t *fpp2);
fpp_t *fppol_intersection(fpp_internal_t* pr, bool destructive, fpp_t *fpp1, fpp_t *fpp2);
fpp_t *fppol_project(fpp_internal_t* pr, bool destructive, fpp_t *fpp, int varIndex);
fpp_t *fppol_assign_bySubstitution(fpp_internal_t* pr, bool destructive, fpp_t *fpp, unsigned Vj, numdbl_t *exp);
fpp_t* fppol_widen_standard(fpp_internal_t* pr, bool destructive, fpp_t *fpp1, fpp_t *fpp2);
fpp_t* fppol_widen_standard_withBound(fpp_internal_t* pr, bool destructive, fpp_t *fpp1, fpp_t *fpp2);
numdbl_t* fppol_common_bounds(fpp_t *fpp1, fpp_t *fpp2, unsigned *NbCmbnds);
int fppol_inclusion_byBound(fpp_t *fpp1, fpp_t *fpp2);
bool fppol_is_leq_bySyntactic(fpp_t *fpp1, fpp_t *fpp2);
bool fppol_is_eq_bySyntactic(fpp_t *fpp1, fpp_t *fpp2);


fpp_t *fppol_construct_convex_hull_rational(fpp_internal_t* pr, fpp_t *fpp1, fpp_t *fpp2, numdbl_t *env, unsigned nenvs);
fpp_t *fppol_strong_join_rational(fpp_internal_t* pr, fpp_t *fpp1, fpp_t *fpp2);
fpp_t *fppol_reshape_steep_ineqs(fpp_internal_t* pr, bool destructive, fpp_t *fpp, bit_vectors_t* bv);
bool fppol_factorization(fpp_internal_t* pr, fpp_t *fpp01, fpp_t *fpp02,  fpp_t *fpp1, fpp_t *fpp2);
void fppol_initialize_bitVectors(fpp_internal_t* pr,unsigned ncons1,unsigned ncons2, bit_vectors_t *bv,
 char *env1bits, char *nenv1bits,char *env2bits, char *nenv2bits,char *sigmabits, char *env1Index,char *env2Index );

bool fppol_entailment(fpp_internal_t* pr,fpp_t *fpp, numdbl_t *aCons, bool recalBnds);
int vector_cmp(numdbl_t *l1, numdbl_t *l2, int len);

fpp_t *fppol_reshape_ineqs(fpp_internal_t* pr, bool destructive, fpp_t *fpp, bit_vectors_t* bv);
#ifdef __cplusplus
}
#endif

#endif
