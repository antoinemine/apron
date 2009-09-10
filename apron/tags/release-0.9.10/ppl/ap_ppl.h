/*
 * apron_ppl.h
 *
 * APRON Library / PPL library wrapper
 *
 * This is the (only) one .h to include to access all PPL domains.
 *
 * Copyright (C) Antoine Mine' 2006
 *
 */

/* 
 * This file is part of the APRON Library, released under GPL license.
 * Please read the COPYING file packaged in the distribution.
 */

#ifndef __APRON_PPL_H
#define __APRON_PPL_H

#include "ap_global0.h"

#ifdef __cplusplus
extern "C" {
#endif



ap_manager_t* ap_ppl_poly_manager_alloc(bool strict);
  /* (PPL::Polyhedron )

    Allocate an APRON manager for convex polyhedra, linked to PPL.

    If the Boolean parameter is true, abstract values generated with the
    manager can have strict constraints (like x>0). Otherwise they are defined
    using only loose constraints. Managers and abstract values in strict and
    loose mode are incompatible.
  */

ap_manager_t* ap_ppl_grid_manager_alloc(void);
  /* (PPL::Grid)

     Allocate a APRON manager for grids, linked to PPL.
  */



  /* extra functions not in APRON managers */

ap_abstract0_t*
ap_abstract0_ppl_poly_widening_thresholds(ap_manager_t* man,
					  ap_abstract0_t* a1,
					  ap_abstract0_t* a2,
					  ap_lincons0_array_t* array);

ap_abstract0_t*
ap_abstract0_ppl_grid_widening_thresholds(ap_manager_t* man,
					  ap_abstract0_t* a1,
					  ap_abstract0_t* a2,
					  ap_lincons0_array_t* array);



#ifdef __cplusplus
}
#endif

#endif /* __APRON_PPL_H */
