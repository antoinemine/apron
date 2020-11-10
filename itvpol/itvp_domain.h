
/* ********************************************************************** */
/* itvp_domain.h: interval polyhedral domain operations for itvpol  */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#ifndef _ITVP_DOMAIN_H_
#define _ITVP_DOMAIN_H_

#include "itvp.h"
#include "itvp_internal.h"
#ifdef __cplusplus
extern "C" {
#endif

itvp_t *itvpol_weak_join(itvp_internal_t* pr, itvp_t *itvp01, itvp_t *itvp02);
numdbl_t *itvpol_envelope(itvp_t *itvp2, itvp_t *itvp1, unsigned *NbEnvs, int *superset,char *env2Index,char *env1Index);
bool is_universal_constraint(numdbl_t *l, int dim);
numdbl_t *interval_combination(numdbl_t *l1, numdbl_t *l2, int dim);
numdbl_t *interval_combination_multi(numdbl_t *l1, numdbl_t *l2, int dim);
numdbl_t *itvpol_nonenv_interval_combination(itvp_t *itvp1, itvp_t *itvp2, char *env1Index,char *env2Index, unsigned *nCons);
int itvpol_vector_cmp(numdbl_t *l1, numdbl_t *l2, int len);
int itvpol_infeasibility_byILP(itvp_internal_t* pr, itvp_t *itvp);
itvp_t *itvpol_intersection(itvp_internal_t* pr, bool destructive, itvp_t *itvp1, itvp_t *itvp2);
numdbl_t *itvpol_envelope_half(itvp_t *itvp1, itvp_t *itvp2, unsigned *NbEnvs, unsigned *NbQuasi,
char *envIndex);
bool itvpol_entailment(itvp_internal_t* pr,itvp_t *itvp, numdbl_t *aCons, bool recalBnds);
itvp_t* itvpol_widen_standard(itvp_internal_t* pr, bool destructive, itvp_t *itvp1, itvp_t *itvp2);
numdbl_t* itvpol_common_bounds(itvp_t *itvp1, itvp_t *itvp2, unsigned *NbCmbnds);
itvp_t* itvpol_widen_standard_withBound(itvp_internal_t* pr, bool destructive, itvp_t *itvp1, itvp_t *itvp2);
int itvpol_inclusion_byBound(itvp_t *itvp1, itvp_t *itvp2);
bool itvpol_is_leq_bySyntactic(itvp_t *itvp1, itvp_t *itvp2);
int itvpol_inclusion_byILP(itvp_t *itvp1, itvp_t *itvp2);
bool itvpol_is_eq_bySyntactic(itvp_t *itvp1, itvp_t *itvp2);
itvp_t *itvpol_project(itvp_internal_t* pr, bool destructive, itvp_t *itvp, int varIndex);
itvp_t *itvpol_reduction(itvp_internal_t* pr, bool destructive, itvp_t *itvp);

itvp_t *itvpol_construct_convex_hull(itvp_internal_t* pr, itvp_t *itvp1, itvp_t *itvp2, numdbl_t *env, unsigned nenvs);
itvp_t *itvpol_strong_join(itvp_internal_t* pr, itvp_t *itvp01, itvp_t *itvp02);
#ifdef __cplusplus
}
#endif

#endif
