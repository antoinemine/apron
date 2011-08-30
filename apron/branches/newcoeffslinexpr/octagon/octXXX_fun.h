/*
 * octXXX_fun.h
 *
 * Direct access to hi-level octagonal functions bypassing the manager.
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

#ifndef __OCTXXX_FUN_H
#define __OCTXXX_FUN_H


/* dependencies */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "numXXX.h"
#include "boundXXX.h"
#include "ap_global0.h"
#include "octXXX.h"

#ifdef __cplusplus
extern "C" {
#endif



/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

struct _octXXX_t;
typedef struct _octXXX_t octXXX_t;
  /* Abstract data type of octagons (defined in octXXX_private.h). */

octXXX_t* octXXX_copy(ap_manager_t* man, octXXX_t* a);
  /* Return a copy of an abstract value, on
     which destructive update does not affect the initial value. */

void octXXX_free(ap_manager_t* man, octXXX_t* a);
  /* Free all the memory used by the abstract value */

size_t octXXX_size(ap_manager_t* man, octXXX_t* a);
  /* Return the abstract size of an abstract value (see ap_manager_t) */


struct _octXXX_internal_t;
typedef struct _octXXX_internal_t octXXX_internal_t;
  /* Abstract data type of library-specific manager options. */


octXXX_t*          octXXX_of_abstract0(ap_abstract0_t* a);
ap_abstract0_t* abstract0_of_oct(ap_manager_t* man, octXXX_t* oct);
  /* Wrapping / unwrapping of octXXX_t in ap_abstract0_t (no copy) */


/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */

/* NOT IMPLEMENTED */

void octXXX_minimize(ap_manager_t* man, octXXX_t* a);

void octXXX_canonicalize(ap_manager_t* man, octXXX_t* a);

int octXXX_hash(ap_manager_t* man, octXXX_t* a);

void octXXX_approximate(ap_manager_t* man, octXXX_t* a, int algorithm);

bool octXXX_is_minimal(ap_manager_t* man, octXXX_t* a);

bool octXXX_is_canonical(ap_manager_t* man, octXXX_t* a);



/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */

void octXXX_fprint(FILE* stream,
		ap_manager_t* man,
		octXXX_t* a,
		char** name_of_dim);
  /* Print the abstract value in a pretty way, using function
     name_of_dim to name dimensions */

void octXXX_fprintdiff(FILE* stream,
		    ap_manager_t* man,
		    octXXX_t* a1, octXXX_t* a2,
		    char** name_of_dim);
  /* Print the difference between a1 (old value) and a2 (new value),
     using function name_of_dim to name dimensions.
     The meaning of difference is library dependent. */

void octXXX_fdump(FILE* stream, ap_manager_t* man, octXXX_t* a);
  /* Dump the internal representation of an abstract value,
     for debugging purposes */


/* ============================================================ */
/* I.4 Serialization */
/* ============================================================ */

ap_membuf_t octXXX_serialize_raw(ap_manager_t* man, octXXX_t* a);
/* Allocate a memory buffer (with malloc), output the abstract value in raw
   binary format to it and return a pointer on the memory buffer and the size
   of bytes written.  It is the user responsability to free the memory
   afterwards (with free). */

octXXX_t* octXXX_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size);
/* Return the abstract value read in raw binary format from the input stream
   and store in size the number of bytes read */


/* ********************************************************************** */
/* II. Constructor, accessors, tests and property extraction */
/* ********************************************************************** */

/* ============================================================ */
/* II.1 Basic constructors */
/* ============================================================ */

octXXX_t* octXXX_bottom(ap_manager_t* man, ap_dimension_t dimension);
  /* Create a bottom (empty) value */

octXXX_t* octXXX_top(ap_manager_t* man, ap_dimension_t dimension);
  /* Create a top (universe) value */

octXXX_t* octXXX_of_box(ap_manager_t* man,
			ap_dimension_t dimension,
			ap_linexpr0_t box);
  /* Abstract an hypercube */

/* ============================================================ */
/* II.2 Accessors */
/* ============================================================ */

ap_dimension_t octXXX_dimension(ap_manager_t* man, octXXX_t* a);
/* Return the total number of dimensions of the abstract values */


/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */

bool octXXX_is_bottom(ap_manager_t* man, octXXX_t* a);
bool octXXX_is_top(ap_manager_t* man, octXXX_t* a);

bool octXXX_is_leq(ap_manager_t* man, octXXX_t* a1, octXXX_t* a2);
  /* inclusion check */

bool octXXX_is_eq(ap_manager_t* man, octXXX_t* a1, octXXX_t* a2);
  /* equality check */

/* NOT IMPLEMENTED */
bool octXXX_sat_lincons(ap_manager_t* man, octXXX_t* a, ap_lincons0_t cons);
  /* does the abstract value satisfy the linear constraint ? */

bool octXXX_sat_tcons(ap_manager_t* man, octXXX_t* a, ap_tcons0_t* cons);
  /* does the abstract value satisfy the tree expression constraint ? */

bool octXXX_sat_interval(ap_manager_t* man, octXXX_t* a,
			 ap_dim_t dim, ap_coeff_t interval);
  /* is the dimension included in the interval in the abstract value ? */

bool octXXX_is_dimension_unconstrained(ap_manager_t* man, octXXX_t* a,
				    ap_dim_t dim);
  /* is the dimension unconstrained ? */

/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */

void octXXX_bound_linexpr(ap_manager_t* man,
			  ap_coeff_t i, octXXX_t* a, ap_linexpr0_t expr);
  /* Returns the interval taken by a linear expression
     over the abstract value */

void octXXX_bound_texpr(ap_manager_t* man,
			ap_coeff_t i, octXXX_t* a, ap_texpr0_t* expr);
  /* Returns the interval taken by a tree expression
     over the abstract value */

void octXXX_bound_dimension(ap_manager_t* man,
			    ap_coeff_t i, octXXX_t* a, ap_dim_t dim);
  /* Returns the interval taken by the dimension
     over the abstract value */

void octXXX_to_lincons_array(ap_manager_t* man, ap_lincons0_array_t array, octXXX_t* a);
  /* Converts an abstract value to a polyhedra
     (conjunction of linear constraints). */

ap_tcons0_array_t octXXX_to_tcons_array(ap_manager_t* man, octXXX_t* a);
  /* Converts an abstract value to a conjunction
     of tree expressions constraints */

void octXXX_to_box(ap_manager_t* man, ap_linexpr0_t box, octXXX_t* a);
  /* Converts an abstract value to an interval/hypercube.
     The size of the resulting array is octXXX_dimension(man,a).  This
     function can be reimplemented by using octXXX_bound_linexpr */

void octXXX_to_lingen_array(ap_manager_t* man, ap_lingen0_array_t array, octXXX_t* a);
  /* Converts an abstract value to a system of generators. */


/* ********************************************************************** */
/* III. Operations */
/* ********************************************************************** */

/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */

octXXX_t* octXXX_meet(ap_manager_t* man, bool destructive, octXXX_t* a1, octXXX_t* a2);
octXXX_t* octXXX_join(ap_manager_t* man, bool destructive, octXXX_t* a1, octXXX_t* a2);
  /* Meet and Join of 2 abstract values */

octXXX_t* octXXX_meet_array(ap_manager_t* man, octXXX_t** tab, size_t size);
octXXX_t* octXXX_join_array(ap_manager_t* man, octXXX_t** tab, size_t size);
  /* Meet and Join of an array of abstract values.
     Raises an [[exc_invalid_argument]] exception if [[size==0]]
     (no way to define the dimensionality of the result in such a case */

octXXX_t* octXXX_meet_lincons_array(ap_manager_t* man,
			      bool destructive, octXXX_t* a,
			      ap_lincons0_array_t array);
  /* Meet of an abstract value with a set of constraints
     (generalize octXXX_of_lincons_array) */

octXXX_t* octXXX_meet_tcons_array(ap_manager_t* man,
				  bool destructive, octXXX_t* a,
				  ap_tcons0_array_t* array);
  /* Meet of an abstract value with a set of tree expressions constraints.
     (generalize octXXX_of_tcons_array) */

octXXX_t* octXXX_add_ray_array(ap_manager_t* man,
			       bool destructive, octXXX_t* a,
			       ap_lingen0_array_t array);
  /* Generalized time elapse operator.
     Note: this is not like adding arbitrary generators because:
     - octXXX_add_ray_array is strict
     - array can only contain rays and lines, not vertices
   */

/* ============================================================ */
/* III.2 Assignement and Substitutions */
/* ============================================================ */

octXXX_t* octXXX_assign_linexpr_array(ap_manager_t* man,
				      bool destructive, octXXX_t* a,
				      ap_dim_t* tdim,
				      ap_linexpr0_array_t array,
				      octXXX_t* dest);
octXXX_t* octXXX_substitute_linexpr_array(ap_manager_t* man,
				    bool destructive, octXXX_t* a,
				    ap_dim_t* tdim,
				    ap_linexpr0_array_t array,
				    octXXX_t* dest);
octXXX_t* octXXX_assign_texpr_array(ap_manager_t* man,
				    bool destructive, octXXX_t* a,
				    ap_dim_t* tdim,
				    ap_texpr0_array_t* array,
				    octXXX_t* dest);
octXXX_t* octXXX_substitute_texpr_array(ap_manager_t* man,
					bool destructive, octXXX_t* a,
					ap_dim_t* tdim,
					ap_texpr0_array_t* array,
					octXXX_t* dest);


  /* Parallel Assignement and Substitution of several dimensions by
     expressions in abstract value org.

     dest is an optional argument. If not NULL, semantically speaking,
     the result of the transformation is intersected with dest. This is
     useful for precise backward transformations in lattices like intervals or
     octagons. */

/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

octXXX_t* octXXX_forget_array(ap_manager_t* man,
			      bool destructive, octXXX_t* a,
			      ap_dim_t* tdim, size_t size,
			      bool project);

/* ============================================================ */
/* III.4 Change and permutation of dimensions */
/* ============================================================ */

octXXX_t* octXXX_add_dimensions(ap_manager_t* man,
				bool destructive, octXXX_t* a,
				ap_dimchange_t* dimchange,
				bool project);

octXXX_t* octXXX_remove_dimensions(ap_manager_t* man,
				   bool destructive, octXXX_t* a,
				   ap_dimchange_t* dimchange);

octXXX_t* octXXX_permute_dimensions(ap_manager_t* man,
				    bool destructive,
				    octXXX_t* a,
				    ap_dimperm_t* permutation);

/* ============================================================ */
/* III.5 Expansion and folding of dimensions */
/* ============================================================ */

octXXX_t* octXXX_expand(ap_manager_t* man,
			bool destructive, octXXX_t* a,
			ap_dim_t dim,
			size_t n);
  /* Expand the dimension dim into itself + n additional dimensions.
     It results in (n+1) unrelated dimensions having same
     relations with other dimensions. The (n+1) dimensions are put as follows:

     - original dimension dim

     - if the dimension is integer, the n additional dimensions are put at the
       end of integer dimensions; if it is real, at the end of the real
       dimensions.
  */

octXXX_t* octXXX_fold(ap_manager_t* man,
		      bool destructive, octXXX_t* a,
		      ap_dim_t* tdim,
		      size_t size);
  /* Fold the dimensions in the array tdim of size n>=1 and put the result
     in the first dimension in the array. The other dimensions of the array
     are then removed. */

/* ============================================================ */
/* III.6 Widening, Narrowing */
/* ============================================================ */

octXXX_t* octXXX_widening(ap_manager_t* man, octXXX_t* a1, octXXX_t* a2);
  /* Standard widening: set unstable constraints to +oo */

octXXX_t* octXXX_widening_thresholds(ap_manager_t* man,
				     octXXX_t* a1, octXXX_t* a2,
				     ap_coeff_t* array,
				     size_t nb);
  /* Widening with threshold.
     array is assumed to contain nb thresholds, sorted in increasing order. */

octXXX_t* octXXX_narrowing(ap_manager_t* man, octXXX_t* a1, octXXX_t* a2);
  /* Standard narrowing: refine only +oo constraint */

octXXX_t* octXXX_add_epsilon(ap_manager_t* man, octXXX_t* a, ap_coeff_t epsilon);
  /* Enlarge each bound by epsilon times the maximum finite bound in
     the octagon */

octXXX_t* octXXX_add_epsilon_bin(ap_manager_t* man, octXXX_t* a1, octXXX_t* a2,
				 ap_coeff_t epsilon);
  /* Enlarge each bound from a1 by epsilon times the maximum finite bound in
     a2. Only bounds in a1 that are not stable in a2 are enlarged. */


/* ============================================================ */
/* III.7 Topological closure operation */
/* ============================================================ */

octXXX_t* octXXX_closure(ap_manager_t* man, bool destructive, octXXX_t* a);
  /* Returns the topological closure of a possibly opened abstract value */


#ifdef __cplusplus
}
#endif

#endif /* __OCTXXX_FUN_H */
