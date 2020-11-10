
/* ********************************************************************** */
/* ifm.h: Interval Fourier-Motzkin Elimination   */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#ifndef _IFM_H_
#define _IFM_H_

#include "itvp.h"
#include "itvp_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

itvp_t *ifm_elimination(itvp_internal_t* pr, bool destructive, itvp_t *itvp0, unsigned varIndex);
itvp_t *itvp_redundancy_removal_quasi_syntactic(itvp_internal_t* pr,bool destructive, itvp_t *itvp);
itvp_t *redundancy_removal_ILP_backward(itvp_internal_t* pr,bool destructive, itvp_t *itvp, bool eps);
itvp_t *redundancy_removal_backward(itvp_internal_t* pr, bool destructive,itvp_t *itvp, bool eps);
unsigned ifm_select(itvp_t *itvp, unsigned startI, unsigned endI);
#ifdef __cplusplus
}
#endif

#endif
