/* ********************************************************************** */
/* itvp_user.h: conversions with interface datatypes */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

/* This header file defines operations on vectors. A vector is just an array of
   elements of type pkint_t, as a consequence functions need to be given also
   their size. */

#ifndef __ITVP_USER_H__
#define __ITVP_USER_H__

#include "itvp.h"
#include "itvp_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

ap_lincons0_t itvp_lincons0_of_vector(numdbl_t* q, size_t size);
numdbl_t* itvp_bnds_to_cons(itvp_internal_t* itvp, numdbl_t* bnds, size_t dim, size_t* ncons);
numdbl_t* itvp_vector_of_lincons0(ap_lincons0_t p, unsigned dimension,unsigned intdim, unsigned* n);
void itvp_vector_of_linexp0(numdbl_t* q, ap_linexpr0_t* p);
void itvp_init_bounds(numdbl_t* bnds, size_t dim);
itvp_t* itvp_normalize(itvp_internal_t* pr,bool destructive,itvp_t* itvp);
int* itvp_vars_related_to_last_kthConstraints(itvp_t* itvp1,int k, int* nvars);
itvp_t* itvp_remove_kthCons(itvp_t* itvp,int k);
#ifdef __cplusplus
}
#endif

#endif
