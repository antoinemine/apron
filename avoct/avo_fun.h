/*
 * avo_fun.h
 *
 * Direct access to hi-level AV octagonal functions bypassing the manager.
 * The only file you need to include to use AV octagons.
 *
 * APRON Library / Absolute Value Octagonal (AVO) Domain
 *
 * Copyright (C) Liqian Chen & Jiangchao Liu' 2014-
 *
 */

/* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution.
*/

#ifndef __AVO_FUN_H
#define __AVO_FUN_H


/* dependencies */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "num.h"
#include "bound.h"

#include "ap_coeff.h"
#include "ap_dimension.h"
#include "ap_expr0.h"
#include "ap_manager.h"
#include "avo.h"

#ifdef __cplusplus
extern "C" {
#endif



/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

struct _avo_t;
typedef struct _avo_t avo_t;
  /* Abstract data type of avOctagons (defined in avo_private.h). */

avo_t* avo_copy(ap_manager_t* man, avo_t* a);
  /* Return a copy of an abstract value, on
     which destructive update does not affect the initial value. */

void avo_free(ap_manager_t* man, avo_t* a);
  /* Free all the memory used by the abstract value */

size_t avo_size(ap_manager_t* man, avo_t* a);
  /* Return the abstract size of an abstract value (see ap_manager_t) */


struct _avo_internal_t;
typedef struct _avo_internal_t avo_internal_t;
  /* Abstract data type of library-specific manager options. */


avo_t*          avo_of_abstract0(ap_abstract0_t* a);
ap_abstract0_t* abstract0_of_avo(ap_manager_t* man, avo_t* avo);
  /* Wrapping / unwrapping of avo_t in ap_abstract0_t (no copy) */

/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */

/* NOT IMPLEMENTED */

void avo_minimize(ap_manager_t* man, avo_t* a);

void avo_canonicalize(ap_manager_t* man, avo_t* a);

int avo_hash(ap_manager_t* man, avo_t* a);

void avo_approximate(ap_manager_t* man, avo_t* a, int algorithm);

bool avo_is_minimal(ap_manager_t* man, avo_t* a);

bool avo_is_canonical(ap_manager_t* man, avo_t* a);



/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */

void avo_fprint(FILE* stream,
		ap_manager_t* man,
		avo_t* a,
		char** name_of_dim);
  /* Print the abstract value in a pretty way, using function
     name_of_dim to name dimensions */

void avo_fprintdiff(FILE* stream,
		    ap_manager_t* man,
		    avo_t* a1, avo_t* a2,
		    char** name_of_dim);
  /* Print the difference between a1 (old value) and a2 (new value),
     using function name_of_dim to name dimensions.
     The meaning of difference is library dependent. */

void avo_fdump(FILE* stream, ap_manager_t* man, avo_t* a);
  /* Dump the internal representation of an abstract value,
     for debugging purposes */


/* ============================================================ */
/* I.4 Serialization */
/* ============================================================ */

ap_membuf_t avo_serialize_raw(ap_manager_t* man, avo_t* a);
/* Allocate a memory buffer (with malloc), output the abstract value in raw
   binary format to it and return a pointer on the memory buffer and the size
   of bytes written.  It is the user responsability to free the memory
   afterwards (with free). */

avo_t* avo_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size);
/* Return the abstract value read in raw binary format from the input stream
   and store in size the number of bytes read */


/* ********************************************************************** */
/* II. Constructor, accessors, tests and property extraction */
/* ********************************************************************** */

/* ============================================================ */
/* II.1 Basic constructors */
/* ============================================================ */

/* We assume that dimensions [0..intdim-1] correspond to integer variables, and
   dimensions [intdim..intdim+realdim-1] to real variables */

avo_t* avo_bottom(ap_manager_t* man, size_t intdim, size_t realdim);
  /* Create a bottom (empty) value */

avo_t* avo_top(ap_manager_t* man, size_t intdim, size_t realdim);
  /* Create a top (universe) value */


avo_t* avo_of_box(ap_manager_t* man,
		  size_t intdim, size_t realdim,
		  ap_interval_t** tinterval);
  /* Abstract an hypercube defined by the array of intervals
     of size intdim+realdim */

avo_t* avo_of_lincons_array(ap_manager_t* man,
			      size_t intdim, size_t realdim,
			      ap_lincons0_array_t* array);
  /* Abstract a convex polyhedra defined by an array of linear constraints */

avo_t* avo_of_tcons_array(ap_manager_t* man,
			  size_t intdim, size_t realdim,
			  ap_tcons0_array_t* array);
  /* Abstract a conjunction of tree expressions constraints */

avo_t* avo_of_generator_array(ap_manager_t* man,
			      size_t intdim, size_t realdim,
			      ap_generator0_array_t* array);
  /* Abstract a convex polyhedra defined an array of generators */


/* ============================================================ */
/* II.2 Accessors */
/* ============================================================ */

ap_dimension_t avo_dimension(ap_manager_t* man, avo_t* a);
/* Return the total number of dimensions of the abstract values */


/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */

bool avo_is_bottom(ap_manager_t* man, avo_t* a);
bool avo_is_top(ap_manager_t* man, avo_t* a);

bool avo_is_leq(ap_manager_t* man, avo_t* a1, avo_t* a2);
  /* inclusion check */

bool avo_is_eq(ap_manager_t* man, avo_t* a1, avo_t* a2);
  /* equality check */

/* NOT IMPLEMENTED */
bool avo_sat_lincons(ap_manager_t* man, avo_t* a, ap_lincons0_t* cons);
  /* does the abstract value satisfy the linear constraint ? */

bool avo_sat_tcons(ap_manager_t* man, avo_t* a, ap_tcons0_t* cons);
  /* does the abstract value satisfy the tree expression constraint ? */

bool avo_sat_interval(ap_manager_t* man, avo_t* a,
		      ap_dim_t dim, ap_interval_t* interval);
  /* is the dimension included in the interval in the abstract value ? */

bool avo_is_dimension_unconstrained(ap_manager_t* man, avo_t* a,
				    ap_dim_t dim);
  /* is the dimension unconstrained ? */

/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */

ap_interval_t* avo_bound_linexpr(ap_manager_t* man,
				 avo_t* a, ap_linexpr0_t* expr);
  /* Returns the interval taken by a linear expression
     over the abstract value */

ap_interval_t* avo_bound_texpr(ap_manager_t* man,
			       avo_t* a, ap_texpr0_t* expr);
  /* Returns the interval taken by a tree expression
     over the abstract value */

ap_interval_t* avo_bound_dimension(ap_manager_t* man,
				   avo_t* a, ap_dim_t dim);
  /* Returns the interval taken by the dimension
     over the abstract value */

ap_lincons0_array_t avo_to_lincons_array(ap_manager_t* man, avo_t* a);
  /* Converts an abstract value to a polyhedra
     (conjunction of linear constraints). */

ap_tcons0_array_t avo_to_tcons_array(ap_manager_t* man, avo_t* a);
  /* Converts an abstract value to a conjunction
     of tree expressions constraints */

ap_interval_t** avo_to_box(ap_manager_t* man, avo_t* a);
  /* Converts an abstract value to an interval/hypercube.
     The size of the resulting array is avo_dimension(man,a).  This
     function can be reimplemented by using avo_bound_linexpr */

ap_generator0_array_t avo_to_generator_array(ap_manager_t* man, avo_t* a);
  /* Converts an abstract value to a system of generators. */


/* ********************************************************************** */
/* III. Operations */
/* ********************************************************************** */

/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */

avo_t* avo_meet(ap_manager_t* man, bool destructive, avo_t* a1, avo_t* a2);
avo_t* avo_join(ap_manager_t* man, bool destructive, avo_t* a1, avo_t* a2);
  /* Meet and Join of 2 abstract values */

avo_t* avo_meet_array(ap_manager_t* man, avo_t** tab, size_t size);
avo_t* avo_join_array(ap_manager_t* man, avo_t** tab, size_t size);
  /* Meet and Join of an array of abstract values.
     Raises an [[exc_invalid_argument]] exception if [[size==0]]
     (no way to define the dimensionality of the result in such a case */

avo_t* avo_meet_lincons_array(ap_manager_t* man,
			      bool destructive, avo_t* a,
			      ap_lincons0_array_t* array);
  /* Meet of an abstract value with a set of constraints
     (generalize avo_of_lincons_array) */

avo_t* avo_meet_tcons_array(ap_manager_t* man,
			    bool destructive, avo_t* a,
			    ap_tcons0_array_t* array);
  /* Meet of an abstract value with a set of tree expressions constraints.
     (generalize avo_of_tcons_array) */

avo_t* avo_add_ray_array(ap_manager_t* man,
			 bool destructive, avo_t* a,
			 ap_generator0_array_t* array);
  /* Generalized time elapse operator.
     Note: this is not like adding arbitrary generators because:
     - avo_add_ray_array is strict
     - array can only contain rays and lines, not vertices
   */

/* ============================================================ */
/* III.2 Assignement and Substitutions */
/* ============================================================ */

avo_t* avo_assign_linexpr_array(ap_manager_t* man,
				bool destructive, avo_t* a,
				ap_dim_t* tdim,
				ap_linexpr0_t** texpr,
				size_t size,
				avo_t* dest);
avo_t* avo_substitute_linexpr_array(ap_manager_t* man,
				    bool destructive, avo_t* a,
				    ap_dim_t* tdim,
				    ap_linexpr0_t** texpr,
				    size_t size,
				    avo_t* dest);
avo_t* avo_assign_texpr_array(ap_manager_t* man,
			      bool destructive, avo_t* a,
			      ap_dim_t* tdim,
			      ap_texpr0_t** texpr,
			      size_t size,
			      avo_t* dest);
avo_t* avo_substitute_texpr_array(ap_manager_t* man,
				  bool destructive, avo_t* a,
				  ap_dim_t* tdim,
				  ap_texpr0_t** texpr,
				  size_t size,
				  avo_t* dest);
  

  /* Parallel Assignement and Substitution of several dimensions by
     expressions in abstract value org.

     dest is an optional argument. If not NULL, semantically speaking,
     the result of the transformation is intersected with dest. This is
     useful for precise backward transformations in lattices like intervals or
     avOctagons. */

/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

avo_t* avo_forget_array(ap_manager_t* man,
			bool destructive, avo_t* a,
			ap_dim_t* tdim, size_t size,
			bool project);

/* ============================================================ */
/* III.4 Change and permutation of dimensions */
/* ============================================================ */

avo_t* avo_add_dimensions(ap_manager_t* man,
			  bool destructive, avo_t* a,
			  ap_dimchange_t* dimchange,
			  bool project);

avo_t* avo_remove_dimensions(ap_manager_t* man,
			     bool destructive, avo_t* a,
			     ap_dimchange_t* dimchange);

avo_t* avo_permute_dimensions(ap_manager_t* man,
			      bool destructive,
			      avo_t* a,
			      ap_dimperm_t* permutation);

/* ============================================================ */
/* III.5 Expansion and folding of dimensions */
/* ============================================================ */

avo_t* avo_expand(ap_manager_t* man,
		  bool destructive, avo_t* a,
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

avo_t* avo_fold(ap_manager_t* man,
		bool destructive, avo_t* a,
		ap_dim_t* tdim,
		size_t size);
  /* Fold the dimensions in the array tdim of size n>=1 and put the result
     in the first dimension in the array. The other dimensions of the array
     are then removed. */

/* ============================================================ */
/* III.6 Widening, Narrowing */
/* ============================================================ */

avo_t* avo_widening(ap_manager_t* man, avo_t* a1, avo_t* a2);
  /* Standard widening: set unstable constraints to +oo */

avo_t* avo_widening_thresholds(ap_manager_t* man,
			       avo_t* a1, avo_t* a2,
			       ap_scalar_t** array,
			       size_t nb);
  /* Widening with threshold.
     array is assumed to contain nb thresholds, sorted in increasing order. */

avo_t* avo_narrowing(ap_manager_t* man, avo_t* a1, avo_t* a2);
  /* Standard narrowing: refine only +oo constraint */

avo_t* avo_add_epsilon(ap_manager_t* man, avo_t* a, ap_scalar_t* epsilon);
  /* Enlarge each bound by epsilon times the maximum finite bound in
     the avOctagon */

avo_t* avo_add_epsilon_bin(ap_manager_t* man, avo_t* a1, avo_t* a2,
			   ap_scalar_t* epsilon);
  /* Enlarge each bound from a1 by epsilon times the maximum finite bound in
     a2. Only bounds in a1 that are not stable in a2 are enlared. */


/* ============================================================ */
/* III.7 Topological closure operation */
/* ============================================================ */

avo_t* avo_closure(ap_manager_t* man, bool destructive, avo_t* a);
  /* Returns the topological closure of a not-necessarily closed abstract value (allowing strict inequalities)  */


#ifdef __cplusplus
}
#endif

#endif /* __AVOsssss_FUN_H */
