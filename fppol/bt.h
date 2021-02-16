/*
 * bt.h
 *
 * Functions for bound tightenin.
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

#ifndef _BT_H_
#define _BT_H_

#include "fpp.h"
#include "fpp_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

fpp_t* bt_byRLP(fpp_internal_t* pr, bool destructive, fpp_t *fpp, int *rvars, unsigned nvars);
fpp_t *bt_kthvar_byRLP(fpp_internal_t* pr, bool destructive, fpp_t *fpp,int k);
fpp_t *bt_kthvar_by_bound_propagation(fpp_internal_t* pr, bool destructive, fpp_t *fpp,int k);
numdbl_t bt_half_kthvar_by_bound_propagation(fpp_t *fpp,unsigned k,bool upbd);
fpp_t* bt_kVars_byRLP(fpp_internal_t* pr, bool destructive, fpp_t *fpp,unsigned k0,unsigned k1);
fpp_t* bt_byPureRLP(fpp_internal_t* pr, bool destructive, fpp_t *fpp, int *rvars, unsigned nvars);

fpp_t *bt_kthvar_byExactLP(fpp_internal_t* pr, bool destructive, fpp_t *fpp,int k);
fpp_t* bt_kVars_byExactLP(fpp_internal_t* pr, bool destructive, fpp_t *fpp,unsigned k0,unsigned k1);
fpp_t* bt_byExactLP(fpp_internal_t* pr, bool destructive, fpp_t *fpp, int *rvars, unsigned nvars);
#ifdef __cplusplus
}
#endif

#endif
