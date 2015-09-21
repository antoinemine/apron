/* ************************************************************************* */
/* ap_disjpk.h: disjunction of NewPolka polyhedra */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license with an exception allowing the redistribution of statically linked executables.

   Please read the COPYING file packaged in the distribution */

#ifndef _AP_DISJPK_H_
#define _AP_DISJPK_H_

#include "ap_disjunction.h"

#ifdef __cplusplus
extern "C" {
#endif

ap_manager_t* ap_disjpk_manager_alloc(ap_manager_t* manpk);
ap_lincons0_array_t ap_disjpk_to_lincons_set(ap_manager_t* manager,
					      ap_abstract0_t* abs);

#ifdef __cplusplus
}
#endif

#endif
