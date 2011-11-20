/*
 * oct.h
 *
 * Public definitions.
 * The only file you need to include to use octagons.
 *
 * APRON Library / Octagonal Domain
 *
 * Copyright (C) Antoine Mine' 2006
 *
 */

/* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution.
*/

#ifndef __OCT_H
#define __OCT_H

#include "ap_global0.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum oct_widening_kind {
  oct_widening,
  oct_pre_widening
  /* Not a widening, but a degenerate hull (precisely, a hull without
     closure of the left argument).
     It is tantalizing to interleave widenings and hulls to improve the
     precision of fix-point computations but, unfortunately, this destroys
     the converge property and makes analyses loop forever.
     This PRE-WIDENING is a middle-ground.
     It does not ensure convergence by itself, but can be safely interleaved
     with widenings.
     As long as proper widenings occur infinitely often, the interleaved
     sequence will converge. Also, it converges more slowly, and so, gives
     a better precision. */
} oct_widening_kind;

typedef struct oct_option_t {
  bool print_closure;
  bool printdiff_closure;
  bool meet_lincons_array_closure;
  bool add_ray_array_closure;
  bool assign_closure;
  bool substitute_closure;
  bool join_closure;
  bool narrowing_closure;
  bool widening_closure;
  bool comparison_test_closure;
  bool sat_test_closure;
  bool bound_closure;
  bool extract_closure;
  bool project_closure;
  bool fold_closure;
  oct_widening_kind widening_kind;
} oct_option_t;

ap_manager_t* oct_manager_alloc(num_discr_t discr);
/* Creates a new manager for the octagon library. */

oct_option_t* oct_manager_option_ref(ap_manager_t* man);
/* Returns a reference to option structure */

/* ============================================================ */
/* Supplementary functions & options */
/* ============================================================ */

ap_abstract0_t*
ap_abstract0_oct_of_lingen_array(ap_manager_t* man,
				 size_t intdim, size_t realdim,
				 ap_lingen0_array_t array);

ap_abstract0_t*
ap_abstract0_oct_widening_thresholds(ap_manager_t* man,
				     ap_abstract0_t* a1,
				     ap_abstract0_t* a2,
				     ap_coeff_t* arr,
				     size_t nb );
  /* Widening with threshold.
     array is assumed to contain nb thresholds, sorted in increasing order. */


ap_abstract0_t*
ap_abstract0_oct_narrowing( ap_manager_t* man,
			    ap_abstract0_t* a1,
			    ap_abstract0_t* a2 );
  /* Standard narrowing: refine only +oo constraint */



ap_abstract0_t*
ap_abstract0_oct_add_epsilon(ap_manager_t* man,
			     ap_abstract0_t* a,
			     ap_coeff_t epsilon);
  /* Enlarge each bound by epsilon times the maximum finite bound in
     the octagon */

ap_abstract0_t*
ap_abstract0_oct_add_epsilon_bin(ap_manager_t* man,
				 ap_abstract0_t* a1,
				 ap_abstract0_t* a2,
				 ap_coeff_t epsilon);
  /* Enlarge each bound from a1 by epsilon times the maximum finite bound in
     a2. Only those bounds in a1 that are not stable in a2 are enlared. */

#ifdef __cplusplus
}
#endif

#endif /* __OCT_H */
