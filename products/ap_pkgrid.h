/* ************************************************************************* */
/* ap_poly_grid.h: reduced product of NewPolka polyhedra and PPL grids */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under GPL license.  Please
   read the COPYING file packaged in the distribution */

#ifndef _AP_PKGRID_H_
#define _AP_PKGRID_H_

#include "ap_reducedproduct.h"

#ifdef __cplusplus
extern "C" {
#endif

ap_manager_t* ap_pkgrid_manager_alloc(ap_manager_t* manpk, ap_manager_t* manpplgrid);
  /* Allocates a product managaer (see ap_reducedproduct.h header file

    Returns NULL if manpk is not a polka manager for loose or strict
    polyhedra, or if manpplgrid is not a PPL manager for grids.

    The given managers are copied (reference count incremented) in the
    result.  So, if the argument managers are not needed any more, they
    should be freed with ap_manager_free.
  */
void ap_pkgrid_reduce(ap_manager_t* manager,
		      ap_reducedproduct_t* a);
  /* Reduction function between the two domains */

void ap_pkgrid_approximate(ap_manager_t* manager,
			   ap_reducedproduct_t* a,
			   int n);
  /* Approximation function.

     It consists in apply approximate to the Polka polyhedron, with the argument n.
  */

#ifdef __cplusplus
}
#endif

#endif
