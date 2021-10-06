/*
 * fpp_user.h
 *
 * Functions for conversions with interface datatypes.
 *
 * APRON Library / Fppol Domain
 *
 * Copyright (C) Liqian CHEN' 2008-
 *
 */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution.
*/

#ifndef __FPP_USER_H__
#define __FPP_USER_H__

#include "fpp.h"
#include "fpp_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

ap_lincons0_t fpp_lincons0_of_vector(numdbl_t* q, size_t size);
numdbl_t* fpp_bnds_to_cons(fpp_internal_t* fpp, numdbl_t* bnds, size_t dim, size_t* ncons);
numdbl_t* fpp_vector_of_lincons0(ap_lincons0_t p, unsigned dimension,unsigned intdim, unsigned* n);
void fpp_vector_of_linexp0(numdbl_t* q, ap_linexpr0_t* p);
void fpp_init_bounds(numdbl_t* bnds, size_t dim);
fpp_t* fpp_normalize(fpp_internal_t* pr,bool destructive,fpp_t* fpp);
int* fpp_vars_related_to_last_kthConstraints(fpp_t* fpp1,int k, int* nvars);
fpp_t* fpp_remove_kthCons(fpp_t* fpp,int k);
#ifdef __cplusplus
}
#endif

#endif
