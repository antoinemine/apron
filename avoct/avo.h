/*
 * avo.h
 *
 * Public definitions.
 * The only file you need to include to use AV octagons.
 *
 *
 * APRON Library / Absolute Value Octagonal (AVO) Domain
 *
 * Copyright (C) Liqian Chen & Jiangchao Liu' 2014-
 *
 */

/* This file is part of the APRON Library, released under LGPL license.  
   Please read the COPYING file packaged in the distribution.
*/

#ifndef __AVO_H
#define __AVO_H


/* dependencies */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "ap_coeff.h"
#include "ap_dimension.h"
#include "ap_expr0.h"
#include "ap_manager.h"
#include "ap_abstract0.h"

#ifdef __cplusplus
extern "C" {
#endif


ap_manager_t* avo_manager_alloc(void);
/* Creates a new manager for the AV octagon library. */


/* ============================================================ */
/* Supplementary functions & options */
/* ============================================================ */

ap_abstract0_t* 
ap_abstract0_avo_of_generator_array(ap_manager_t* man,
				    size_t intdim, size_t realdim,
				    ap_generator0_array_t* array);
  
ap_abstract0_t* 
ap_abstract0_avo_widening_thresholds(ap_manager_t* man,
				     ap_abstract0_t* a1,
				     ap_abstract0_t* a2,
				     ap_scalar_t** arr,
				     size_t nb );
  /* Widening with threshold.
     array is assumed to contain nb thresholds, sorted in increasing order. */


ap_abstract0_t* 
ap_abstract0_avo_narrowing( ap_manager_t* man,
			    ap_abstract0_t* a1,
			    ap_abstract0_t* a2 );
  /* Standard narrowing: refine only +oo constraint */


static const int avo_pre_widening = 99;
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

ap_abstract0_t* 
ap_abstract0_avo_add_epsilon(ap_manager_t* man,
			     ap_abstract0_t* a, 
			     ap_scalar_t* epsilon);
  /* Enlarge each bound by epsilon times the maximum finite bound in 
     the avOctagon */

ap_abstract0_t* 
ap_abstract0_avo_add_epsilon_bin(ap_manager_t* man,
				 ap_abstract0_t* a1, 
				 ap_abstract0_t* a2, 
				 ap_scalar_t* epsilon);
  /* Enlarge each bound from a1 by epsilon times the maximum finite bound in 
     a2. Only those bounds in a1 that are not stable in a2 are enlared. */

#ifdef __cplusplus
}
#endif

#endif /* __avo_H */
