
/* ********************************************************************** */
/* ilp.h: interval linear programming functions */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#ifndef _ILP_H_
#define _ILP_H_

#include <glpk.h> /* linear programming lib */
#include "itvp.h"
#include "itvp_internal.h"

#ifdef __cplusplus
extern "C" {
#endif
/* rigorous ILP */
numdbl_t ilp_rig(itvp_t *itvp, unsigned objType, numdbl_t *objVector);
numdbl_t ilp_orthants_enumerate_rig(itvp_t *itvp, unsigned objType, numdbl_t *objVector, char *split, int index);
numdbl_t ilp_orthant_rig(itvp_t *itvp, unsigned objType, numdbl_t *objVector, char *split);

/*  ILP with standard FP LP */
numdbl_t ilp_std(itvp_t *itvp, unsigned objType, numdbl_t *objVector);
numdbl_t ilp_orthants_enumerate_std(itvp_t *itvp, unsigned objType, numdbl_t *objVector, char *split, int index);
numdbl_t ilp_orthant_std(itvp_t *itvp, unsigned objType, numdbl_t *objVector, char *split);
#ifdef __cplusplus
}
#endif

#endif