/*
 * fm.h
 *
 * Functions for sound Fourier-Motzkin elimination in floating point arithemetic.
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

#ifndef _FM_H_
#define _FM_H_

#include "fpp.h"
#include "fpp_internal.h"
#include "bv.h"

#ifdef __cplusplus
extern "C" {
#endif

fpp_t *fm_elimination(fpp_internal_t* pr, bool destructive, fpp_t *fpp, unsigned varIndex, bit_vectors_t* bv,bool *subset);
fpp_t *redundancy_removal(fpp_internal_t* pr, bool destructive,fpp_t *fpp, bool eps);
fpp_t *redundancy_removal_firstKs(fpp_internal_t* pr,bool destructive, fpp_t *fpp, unsigned k,bit_vectors_t *bv, bool eps);
fpp_t *redundancy_removal_lastKs(fpp_internal_t* pr,bool destructive, fpp_t *fpp,unsigned k,bool eps);
fpp_t *redundancy_removal_RLP(fpp_internal_t* pr,bool destructive, fpp_t *fpp,bool eps);
fpp_t *redundancy_removal_RLP_firstKs(fpp_internal_t* pr,bool destructive, fpp_t *fpp,unsigned k,bool eps);
fpp_t *redundancy_removal_RLP_lastKs(fpp_internal_t* pr,bool destructive, fpp_t *fpp,unsigned k,bool eps);
fpp_t *redundancy_removal_quasi_syntactic(fpp_internal_t* pr,bool destructive, fpp_t *fpp);
fpp_t *redundancy_removal_quasi_syntactic_firstKs(fpp_internal_t* pr,bool destructive, fpp_t *fpp,unsigned k,bit_vectors_t *bv);
fpp_t *redundancy_removal_quasi_syntactic_lastKs(fpp_internal_t* pr,bool destructive, fpp_t *fpp, unsigned k);
fpp_t *redundancy_removal_SLP(fpp_internal_t* pr,bool destructive, fpp_t *fpp,bool eps);
fpp_t *redundancy_removal_SLP_firstKs(fpp_internal_t* pr,bool destructive, fpp_t *fpp,unsigned k,bit_vectors_t *bv,  bool eps);
fpp_t *redundancy_removal_SLP_lastKs(fpp_internal_t* pr,bool destructive, fpp_t *fpp,unsigned k, bool eps);

unsigned fm_select(fpp_t *fpp, unsigned startI, unsigned endI);
fpp_t *redundancy_removal_SLP_firstKs_withEnv(fpp_internal_t* pr,bool destructive, fpp_t *fpp,unsigned k,numdbl_t *env,unsigned nenvs,unsigned envdim, bit_vectors_t *bv, bool eps);
fpp_t *redundancy_removal_SLP_firstKs_withSigma(fpp_internal_t* pr,bool destructive, fpp_t *fpp,unsigned k,bit_vectors_t *bv, bool eps);
fpp_t *redundancy_removal_firstKs_byBounds(fpp_internal_t* pr,bool destructive, fpp_t *fpp, numdbl_t *bounds,unsigned k,bit_vectors_t *bv);


fpp_t *fm_elimination_rational(fpp_internal_t* pr, bool destructive, fpp_t *fpp, unsigned varIndex, bit_vectors_t* bv,bool *subset);

fpp_t *fm_elimination_withNenv(fpp_internal_t* pr, bool destructive, fpp_t *fpp, unsigned varIndex, bit_vectors_t* bv,char *nenv1bv,char *nenv2bv,char *sigmabv,bool *subset);

void fm_elimination_combination(fpp_internal_t* pr, fpp_t *fpp,unsigned ncons1,fm_pair_t *fmp,unsigned varIndex,unsigned *nitvcons,unsigned *nsigcons,numdbl_t *itvcons,numdbl_t *sigcons);
int quasi_syntactic_cmp(numdbl_t *l1,numdbl_t *l2,unsigned dim, bool itv);
unsigned itv_quasi_syntactic_redundancy_removal(fpp_internal_t* pr,bit_vectors_t* bv1,unsigned dim,unsigned nitv,unsigned nsig,numdbl_t *itvcons,numdbl_t *sigcons);
fpp_t *fm_elimination_lazy(fpp_internal_t* pr, bool destructive, fpp_t *fpp, unsigned varIndex, bit_vectors_t* bv,unsigned nelimv,bool *subset);
#ifdef __cplusplus
}
#endif

#endif
