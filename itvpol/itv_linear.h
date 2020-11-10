
/* ******************************************************************************* */
/* itv_linear.h: interval linearisation in floating point arithemetic  */
/* ******************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#ifndef _ITV_LINEAR_H_
#define _ITV_LINEAR_H_

#ifdef __cplusplus
extern "C" {
#endif

int itv_linearisation_kthVar(int dim, numdbl_t *coeffs, numdbl_t *bounds, int k);
int itv_linearisation_kthVar_toZero(int dim, numdbl_t *itvcons, numdbl_t *bnds, int k);

#ifdef __cplusplus
}
#endif

#endif
