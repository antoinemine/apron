/* ********************************************************************** */
/* pk_meetjoin.h: Meet and join operations */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#ifndef _PK_MEETJOIN_H_
#define _PK_MEETJOIN_H_

#include "pk_config.h"
#include "pk.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* I. Meet/Join */
/* ********************************************************************** */

/* In all the following functions, for the structure poly_t:

   - If meet is true, standard meaning of everything
   - If meet is false,
     - matrices C and F, satC and satF have been exchanged,
     - nbeq and nbline have been exchanged
     - in status, nothing has changed
*/

/* Meet/Join of a polyhedronwith an array of constraints/generators.

  The polyhedron is supposed:
   - to have constraints,
   - if lazy is false, to be minimized, and to have satC
   - possible emptiness not detected,

   Matrix of constraints is supposed:
   - to be sorted
   - to be canonical too.

   Return true if exception
 */
bool poly_meet_matrix(bool meet, bool lazy,
		      ap_manager_t* man,
		      pk_t* po, 
		      pk_t* pa, pk_matrix_t* mat);

void poly_meet_itv_lincons_array(bool lazy,
				 ap_manager_t* man,
				 pk_t* po, pk_t* pa, 
				 itv_lincons_array_t* array);

void poly_meet(bool meet,
	       bool lazy,
	       ap_manager_t* man,
	       pk_t* po, pk_t* pa, pk_t* pb);

#ifdef __cplusplus
}
#endif

#endif
