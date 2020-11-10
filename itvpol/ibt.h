
/* ********************************************************************** */
/* ibt.h: bound tightening functions */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#ifndef _IBT_H_
#define _IBT_H_

#include "itvp.h"
#include "itvp_internal.h"

#ifdef __cplusplus
extern "C" {
#endif
itvp_t *bt_kthvar_byExactILP(itvp_internal_t* pr, bool destructive, itvp_t *itvp,int k);
itvp_t* bt_kVars_byExactILP(itvp_internal_t* pr, bool destructive, itvp_t *itvp,unsigned k0,unsigned k1);
itvp_t* bt_byExactILP(itvp_internal_t* pr, bool destructive, itvp_t *itvp, int *rvars, unsigned nvars);
#ifdef __cplusplus
}
#endif

#endif
