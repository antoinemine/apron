/*
 * fpp_fun.h
 *
 * Direct access to high-level fppol functions bypassing the manager.
 * The only file you need to include to use fppol.
 *
 * APRON Library / Fppol Domain
 *
 * Copyright (C) Liqian CHEN' 2008-
 *
 */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution.
*/

#ifndef __FPP_FUN_H
#define __FPP_FUN_H


/* dependencies */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "ap_coeff.h"
#include "ap_dimension.h"
#include "ap_expr0.h"
#include "ap_manager.h"
#include "fpp.h"

#ifdef __cplusplus
extern "C" {
#endif



/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

struct _fpp_t;
typedef struct _fpp_t fpp_t;
  /* Abstract data type of fppol (defined in fpp_private.h). */

fpp_t* fpp_copy(ap_manager_t* man, fpp_t* a);
  /* Return a copy of an abstract value, on
     which destructive update does not affect the initial value. */

void fpp_free(ap_manager_t* man, fpp_t* a);
  /* Free all the memory used by the abstract value */

size_t fpp_size(ap_manager_t* man, fpp_t* a);
  /* Return the abstract size of an abstract value (see ap_manager_t) */


struct _fpp_internal_t;
typedef struct _fpp_internal_t fpp_internal_t;
  /* Abstract data type of library-specific manager options. */


fpp_t*          fpp_of_abstract0(ap_abstract0_t* a);
ap_abstract0_t* abstract0_of_fpp(ap_manager_t* man, fpp_t* fpp);
  /* Wrapping / unwrapping of fpp_t in ap_abstract0_t (no copy) */


/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */

/* NOT IMPLEMENTED */

void fpp_minimize(ap_manager_t* man, fpp_t* a);

void fpp_canonicalize(ap_manager_t* man, fpp_t* a);

int fpp_hash(ap_manager_t* man, fpp_t* a);

void fpp_approximate(ap_manager_t* man, fpp_t* a, int algorithm);

bool fpp_is_minimal(ap_manager_t* man, fpp_t* a);

bool fpp_is_canonical(ap_manager_t* man, fpp_t* a);



/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */

void fpp_fprint(FILE* stream,
		ap_manager_t* man,
		fpp_t* a,
		char** name_of_dim);
  /* Print the abstract value in a pretty way, using function
     name_of_dim to name dimensions */

void fpp_fprintdiff(FILE* stream,
		    ap_manager_t* man,
		    fpp_t* a1, fpp_t* a2,
		    char** name_of_dim);
  /* Print the difference between a1 (old value) and a2 (new value),
     using function name_of_dim to name dimensions.
     The meaning of difference is library dependent. */

void fpp_fdump(FILE* stream, ap_manager_t* man, fpp_t* a);
  /* Dump the internal representation of an abstract value,
     for debugging purposes */


/* ============================================================ */
/* I.4 Serialization */
/* ============================================================ */

ap_membuf_t fpp_serialize_raw(ap_manager_t* man, fpp_t* a);
/* Allocate a memory buffer (with malloc), output the abstract value in raw
   binary format to it and return a pointer on the memory buffer and the size
   of bytes written.  It is the user responsability to free the memory
   afterwards (with free). */

fpp_t* fpp_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size);
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

fpp_t* fpp_bottom(ap_manager_t* man, size_t intdim, size_t realdim);
  /* Create a bottom (empty) value */

fpp_t* fpp_top(ap_manager_t* man, size_t intdim, size_t realdim);
  /* Create a top (universe) value */


fpp_t* fpp_of_box(ap_manager_t* man,
		  size_t intdim, size_t realdim,
		  ap_interval_t** tinterval);
  /* Abstract an hypercube defined by the array of intervals
     of size intdim+realdim */

fpp_t* fpp_of_lincons_array(ap_manager_t* man,
			      size_t intdim, size_t realdim,
			      ap_lincons0_array_t* array);
  /* Abstract a convex polyhedra defined by an array of linear constraints */

fpp_t* fpp_of_tcons_array(ap_manager_t* man,
			  size_t intdim, size_t realdim,
			  ap_tcons0_array_t* array);
  /* Abstract a conjunction of tree expressions constraints */

fpp_t* fpp_of_generator_array(ap_manager_t* man,
			      size_t intdim, size_t realdim,
			      ap_generator0_array_t* array);
  /* Abstract a convex polyhedra defined an array of generators */


/* ============================================================ */
/* II.2 Accessors */
/* ============================================================ */

ap_dimension_t fpp_dimension(ap_manager_t* man, fpp_t* a);
/* Return the total number of dimensions of the abstract values */


/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */

bool fpp_is_bottom(ap_manager_t* man, fpp_t* a);
bool fpp_is_top(ap_manager_t* man, fpp_t* a);

bool fpp_is_leq(ap_manager_t* man, fpp_t* a1, fpp_t* a2);
  /* inclusion check */

bool fpp_is_eq(ap_manager_t* man, fpp_t* a1, fpp_t* a2);
  /* equality check */

/* NOT IMPLEMENTED */
bool fpp_sat_lincons(ap_manager_t* man, fpp_t* a, ap_lincons0_t* cons);
  /* does the abstract value satisfy the linear constraint ? */

bool fpp_sat_tcons(ap_manager_t* man, fpp_t* a, ap_tcons0_t* cons);
  /* does the abstract value satisfy the tree expression constraint ? */

bool fpp_sat_interval(ap_manager_t* man, fpp_t* a,
		      ap_dim_t dim, ap_interval_t* interval);
  /* is the dimension included in the interval in the abstract value ? */

bool fpp_is_dimension_unconstrained(ap_manager_t* man, fpp_t* a,
				    ap_dim_t dim);
  /* is the dimension unconstrained ? */

/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */

ap_interval_t* fpp_bound_linexpr(ap_manager_t* man,
				 fpp_t* a, ap_linexpr0_t* expr);
  /* Returns the interval taken by a linear expression
     over the abstract value */

ap_interval_t* fpp_bound_texpr(ap_manager_t* man,
			       fpp_t* a, ap_texpr0_t* expr);
  /* Returns the interval taken by a tree expression
     over the abstract value */

ap_interval_t* fpp_bound_dimension(ap_manager_t* man,
				   fpp_t* a, ap_dim_t dim);
  /* Returns the interval taken by the dimension
     over the abstract value */

ap_lincons0_array_t fpp_to_lincons_array(ap_manager_t* man, fpp_t* a);
  /* Converts an abstract value to a polyhedra
     (conjunction of linear constraints). */

ap_tcons0_array_t fpp_to_tcons_array(ap_manager_t* man, fpp_t* a);
  /* Converts an abstract value to a conjunction
     of tree expressions constraints */

ap_interval_t** fpp_to_box(ap_manager_t* man, fpp_t* a);
  /* Converts an abstract value to an interval/hypercube.
     The size of the resulting array is fpp_dimension(man,a).  This
     function can be reimplemented by using fpp_bound_linexpr */

ap_generator0_array_t fpp_to_generator_array(ap_manager_t* man, fpp_t* a);
  /* Converts an abstract value to a system of generators. */


/* ********************************************************************** */
/* III. Operations */
/* ********************************************************************** */

/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */

fpp_t* fpp_meet(ap_manager_t* man, bool destructive, fpp_t* a1, fpp_t* a2);
fpp_t* fpp_join(ap_manager_t* man, bool destructive, fpp_t* a1, fpp_t* a2);
  /* Meet and Join of 2 abstract values */

fpp_t* fpp_meet_array(ap_manager_t* man, fpp_t** tab, size_t size);
fpp_t* fpp_join_array(ap_manager_t* man, fpp_t** tab, size_t size);
  /* Meet and Join of an array of abstract values.
     Raises an [[exc_invalid_argument]] exception if [[size==0]]
     (no way to define the dimensionality of the result in such a case */

fpp_t* fpp_meet_lincons_array(ap_manager_t* man,
			      bool destructive, fpp_t* a,
			      ap_lincons0_array_t* array);
  /* Meet of an abstract value with a set of constraints
     (generalize fpp_of_lincons_array) */

fpp_t* fpp_meet_tcons_array(ap_manager_t* man,
			    bool destructive, fpp_t* a,
			    ap_tcons0_array_t* array);
  /* Meet of an abstract value with a set of tree expressions constraints.
     (generalize fpp_of_tcons_array) */

fpp_t* fpp_add_ray_array(ap_manager_t* man,
			 bool destructive, fpp_t* a,
			 ap_generator0_array_t* array);
  /* Generalized time elapse operator.
     Note: this is not like adding arbitrary generators because:
     - fpp_add_ray_array is strict
     - array can only contain rays and lines, not vertices
   */

/* ============================================================ */
/* III.2 Assignement and Substitutions */
/* ============================================================ */

fpp_t* fpp_assign_linexpr_array(ap_manager_t* man,
				bool destructive, fpp_t* a,
				ap_dim_t* tdim,
				ap_linexpr0_t** texpr,
				size_t size,
				fpp_t* dest);
fpp_t* fpp_substitute_linexpr_array(ap_manager_t* man,
				    bool destructive, fpp_t* a,
				    ap_dim_t* tdim,
				    ap_linexpr0_t** texpr,
				    size_t size,
				    fpp_t* dest);
fpp_t* fpp_assign_texpr_array(ap_manager_t* man,
			      bool destructive, fpp_t* a,
			      ap_dim_t* tdim,
			      ap_texpr0_t** texpr,
			      size_t size,
			      fpp_t* dest);
fpp_t* fpp_substitute_texpr_array(ap_manager_t* man,
				  bool destructive, fpp_t* a,
				  ap_dim_t* tdim,
				  ap_texpr0_t** texpr,
				  size_t size,
				  fpp_t* dest);


/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

fpp_t* fpp_forget_array(ap_manager_t* man,
			bool destructive, fpp_t* a,
			ap_dim_t* tdim, size_t size,
			bool project);

/* ============================================================ */
/* III.4 Change and permutation of dimensions */
/* ============================================================ */

fpp_t* fpp_add_dimensions(ap_manager_t* man,
			  bool destructive, fpp_t* a,
			  ap_dimchange_t* dimchange,
			  bool project);

fpp_t* fpp_remove_dimensions(ap_manager_t* man,
			     bool destructive, fpp_t* a,
			     ap_dimchange_t* dimchange);

fpp_t* fpp_permute_dimensions(ap_manager_t* man,
			      bool destructive,
			      fpp_t* a,
			      ap_dimperm_t* permutation);

/* ============================================================ */
/* III.5 Expansion and folding of dimensions */
/* ============================================================ */

fpp_t* fpp_expand(ap_manager_t* man,
		  bool destructive, fpp_t* a,
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

fpp_t* fpp_fold(ap_manager_t* man,
		bool destructive, fpp_t* a,
		ap_dim_t* tdim,
		size_t size);
  /* Fold the dimensions in the array tdim of size n>=1 and put the result
     in the first dimension in the array. The other dimensions of the array
     are then removed. */

/* ============================================================ */
/* III.6 Widening, Narrowing */
/* ============================================================ */

fpp_t* fpp_widening(ap_manager_t* man, fpp_t* a1, fpp_t* a2);
  /* Standard widening: set unstable constraints to +oo */

fpp_t* fpp_widening_thresholds(ap_manager_t* man,
			       fpp_t* a1, fpp_t* a2,
			       ap_scalar_t** array,
			       size_t nb);
  /* Widening with threshold.
     array is assumed to contain nb thresholds, sorted in increasing order. */

fpp_t* fpp_narrowing(ap_manager_t* man, fpp_t* a1, fpp_t* a2);
  /* Standard narrowing: refine only +oo constraint */

fpp_t* fpp_add_epsilon(ap_manager_t* man, fpp_t* a, ap_scalar_t* epsilon);
  /* Enlarge each bound by epsilon times the maximum finite bound in
     the fppol */

fpp_t* fpp_add_epsilon_bin(ap_manager_t* man, fpp_t* a1, fpp_t* a2,
			   ap_scalar_t* epsilon);
  /* Enlarge each bound from a1 by epsilon times the maximum finite bound in
     a2. Only bounds in a1 that are not stable in a2 are enlared. */


/* ============================================================ */
/* III.7 Topological closure operation */
/* ============================================================ */

fpp_t* fpp_closure(ap_manager_t* man, bool destructive, fpp_t* a);
  /* Returns the topological closure of a possibly opened abstract value */


#ifdef __cplusplus
}
#endif

#endif /* __FPP_FUN_H */
