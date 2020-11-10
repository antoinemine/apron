/*
 * itvp_fun.h
 *
 * Direct access to hi-level itvpol functions bypassing the manager.
 * The only file you need to include to use itvpol.
 *
 * APRON Library / itvpol Domain
 *
 * Copyright (C) Liqian CHEN' 2009
 *
 */

/* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution.
*/

#ifndef __ITVP_FUN_H
#define __ITVP_FUN_H


/* dependencies */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "ap_coeff.h"
#include "ap_dimension.h"
#include "ap_expr0.h"
#include "ap_manager.h"
#include "itvp.h"

#ifdef __cplusplus
extern "C" {
#endif



/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

struct _itvp_t;
typedef struct _itvp_t itvp_t;

struct _fpp_t0;
typedef struct _fpp_t0 fpp_t0;

  /* Abstract data type of itvpol (defined in itvp_private.h). */

itvp_t* itvp_copy(ap_manager_t* man, itvp_t* a);
  /* Return a copy of an abstract value, on
     which destructive update does not affect the initial value. */

void itvp_free(ap_manager_t* man, itvp_t* a);
  /* Free all the memory used by the abstract value */

size_t itvp_size(ap_manager_t* man, itvp_t* a);
  /* Return the abstract size of an abstract value (see ap_manager_t) */


struct _itvp_internal_t;
typedef struct _itvp_internal_t itvp_internal_t;
  /* Abstract data type of library-specific manager options. */


itvp_t*          itvp_of_abstract0(ap_abstract0_t* a);
ap_abstract0_t* abstract0_of_itvp(ap_manager_t* man, itvp_t* itvp);
  /* Wrapping / unwrapping of itvp_t in ap_abstract0_t (no copy) */


/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */

/* NOT IMPLEMENTED */

void itvp_minimize(ap_manager_t* man, itvp_t* a);

void itvp_canonicalize(ap_manager_t* man, itvp_t* a);

int itvp_hash(ap_manager_t* man, itvp_t* a);

void itvp_approximate(ap_manager_t* man, itvp_t* a, int algorithm);

bool itvp_is_minimal(ap_manager_t* man, itvp_t* a);

bool itvp_is_canonical(ap_manager_t* man, itvp_t* a);



/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */

void itvp_fprint(FILE* stream,
		ap_manager_t* man,
		itvp_t* a,
		char** name_of_dim);
  /* Print the abstract value in a pretty way, using function
     name_of_dim to name dimensions */

void itvp_fprintdiff(FILE* stream,
		    ap_manager_t* man,
		    itvp_t* a1, itvp_t* a2,
		    char** name_of_dim);
  /* Print the difference between a1 (old value) and a2 (new value),
     using function name_of_dim to name dimensions.
     The meaning of difference is library dependent. */

void itvp_fdump(FILE* stream, ap_manager_t* man, itvp_t* a);
  /* Dump the internal representation of an abstract value,
     for debugging purposes */


/* ============================================================ */
/* I.4 Serialization */
/* ============================================================ */

ap_membuf_t itvp_serialize_raw(ap_manager_t* man, itvp_t* a);
/* Allocate a memory buffer (with malloc), output the abstract value in raw
   binary format to it and return a pointer on the memory buffer and the size
   of bytes written.  It is the user responsability to free the memory
   afterwards (with free). */

itvp_t* itvp_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size);
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

itvp_t* itvp_bottom(ap_manager_t* man, size_t intdim, size_t realdim);
  /* Create a bottom (empty) value */

itvp_t* itvp_top(ap_manager_t* man, size_t intdim, size_t realdim);
  /* Create a top (universe) value */


itvp_t* itvp_of_box(ap_manager_t* man,
		  size_t intdim, size_t realdim,
		  ap_interval_t** tinterval);
  /* Abstract an hypercube defined by the array of intervals
     of size intdim+realdim */

itvp_t* itvp_of_lincons_array(ap_manager_t* man,
			      size_t intdim, size_t realdim,
			      ap_lincons0_array_t* array);
  /* Abstract a convex polyhedra defined by an array of linear constraints */

itvp_t* itvp_of_tcons_array(ap_manager_t* man,
			  size_t intdim, size_t realdim,
			  ap_tcons0_array_t* array);
  /* Abstract a conjunction of tree expressions constraints */

itvp_t* itvp_of_generator_array(ap_manager_t* man,
			      size_t intdim, size_t realdim,
			      ap_generator0_array_t* array);
  /* Abstract a convex polyhedra defined an array of generators */


/* ============================================================ */
/* II.2 Accessors */
/* ============================================================ */

ap_dimension_t itvp_dimension(ap_manager_t* man, itvp_t* a);
/* Return the total number of dimensions of the abstract values */


/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */

bool itvp_is_bottom(ap_manager_t* man, itvp_t* a);
bool itvp_is_top(ap_manager_t* man, itvp_t* a);

bool itvp_is_leq(ap_manager_t* man, itvp_t* a1, itvp_t* a2);
  /* inclusion check */

bool itvp_is_eq(ap_manager_t* man, itvp_t* a1, itvp_t* a2);
  /* equality check */

/* NOT IMPLEMENTED */
bool itvp_sat_lincons(ap_manager_t* man, itvp_t* a, ap_lincons0_t* cons);
  /* does the abstract value satisfy the linear constraint ? */

bool itvp_sat_tcons(ap_manager_t* man, itvp_t* a, ap_tcons0_t* cons);
  /* does the abstract value satisfy the tree expression constraint ? */

bool itvp_sat_interval(ap_manager_t* man, itvp_t* a,
		      ap_dim_t dim, ap_interval_t* interval);
  /* is the dimension included in the interval in the abstract value ? */

bool itvp_is_dimension_unconstrained(ap_manager_t* man, itvp_t* a,
				    ap_dim_t dim);
  /* is the dimension unconstrained ? */

/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */

ap_interval_t* itvp_bound_linexpr(ap_manager_t* man,
				 itvp_t* a, ap_linexpr0_t* expr);
  /* Returns the interval taken by a linear expression
     over the abstract value */

ap_interval_t* itvp_bound_texpr(ap_manager_t* man,
			       itvp_t* a, ap_texpr0_t* expr);
  /* Returns the interval taken by a tree expression
     over the abstract value */

ap_interval_t* itvp_bound_dimension(ap_manager_t* man,
				   itvp_t* a, ap_dim_t dim);
  /* Returns the interval taken by the dimension
     over the abstract value */

ap_lincons0_array_t itvp_to_lincons_array(ap_manager_t* man, itvp_t* a);
  /* Converts an abstract value to a polyhedra
     (conjunction of linear constraints). */

ap_tcons0_array_t itvp_to_tcons_array(ap_manager_t* man, itvp_t* a);
  /* Converts an abstract value to a conjunction
     of tree expressions constraints */

ap_interval_t** itvp_to_box(ap_manager_t* man, itvp_t* a);
  /* Converts an abstract value to an interval/hypercube.
     The size of the resulting array is itvp_dimension(man,a).  This
     function can be reimplemented by using itvp_bound_linexpr */

ap_generator0_array_t itvp_to_generator_array(ap_manager_t* man, itvp_t* a);
  /* Converts an abstract value to a system of generators. */


/* ********************************************************************** */
/* III. Operations */
/* ********************************************************************** */

/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */

itvp_t* itvp_meet(ap_manager_t* man, bool destructive, itvp_t* a1, itvp_t* a2);
itvp_t* itvp_join(ap_manager_t* man, bool destructive, itvp_t* a1, itvp_t* a2);
  /* Meet and Join of 2 abstract values */

itvp_t* itvp_meet_array(ap_manager_t* man, itvp_t** tab, size_t size);
itvp_t* itvp_join_array(ap_manager_t* man, itvp_t** tab, size_t size);
  /* Meet and Join of an array of abstract values.
     Raises an [[exc_invalid_argument]] exception if [[size==0]]
     (no way to define the dimensionality of the result in such a case */

itvp_t* itvp_meet_lincons_array(ap_manager_t* man,
			      bool destructive, itvp_t* a,
			      ap_lincons0_array_t* array);
  /* Meet of an abstract value with a set of constraints
     (generalize itvp_of_lincons_array) */

itvp_t* itvp_meet_tcons_array(ap_manager_t* man,
			    bool destructive, itvp_t* a,
			    ap_tcons0_array_t* array);
  /* Meet of an abstract value with a set of tree expressions constraints.
     (generalize itvp_of_tcons_array) */

itvp_t* itvp_add_ray_array(ap_manager_t* man,
			 bool destructive, itvp_t* a,
			 ap_generator0_array_t* array);
  /* Generalized time elapse operator.
     Note: this is not like adding arbitrary generators because:
     - itvp_add_ray_array is strict
     - array can only contain rays and lines, not vertices
   */

/* ============================================================ */
/* III.2 Assignement and Substitutions */
/* ============================================================ */

itvp_t* itvp_assign_linexpr_array(ap_manager_t* man,
				bool destructive, itvp_t* a,
				ap_dim_t* tdim,
				ap_linexpr0_t** texpr,
				size_t size,
				itvp_t* dest);
itvp_t* itvp_substitute_linexpr_array(ap_manager_t* man,
				    bool destructive, itvp_t* a,
				    ap_dim_t* tdim,
				    ap_linexpr0_t** texpr,
				    size_t size,
				    itvp_t* dest);
itvp_t* itvp_assign_texpr_array(ap_manager_t* man,
			      bool destructive, itvp_t* a,
			      ap_dim_t* tdim,
			      ap_texpr0_t** texpr,
			      size_t size,
			      itvp_t* dest);
itvp_t* itvp_substitute_texpr_array(ap_manager_t* man,
				  bool destructive, itvp_t* a,
				  ap_dim_t* tdim,
				  ap_texpr0_t** texpr,
				  size_t size,
				  itvp_t* dest);
  

/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

itvp_t* itvp_forget_array(ap_manager_t* man,
			bool destructive, itvp_t* a,
			ap_dim_t* tdim, size_t size,
			bool project);

/* ============================================================ */
/* III.4 Change and permutation of dimensions */
/* ============================================================ */

itvp_t* itvp_add_dimensions(ap_manager_t* man,
			  bool destructive, itvp_t* a,
			  ap_dimchange_t* dimchange,
			  bool project);

itvp_t* itvp_remove_dimensions(ap_manager_t* man,
			     bool destructive, itvp_t* a,
			     ap_dimchange_t* dimchange);

itvp_t* itvp_permute_dimensions(ap_manager_t* man,
			      bool destructive,
			      itvp_t* a,
			      ap_dimperm_t* permutation);

/* ============================================================ */
/* III.5 Expansion and folding of dimensions */
/* ============================================================ */

itvp_t* itvp_expand(ap_manager_t* man,
		  bool destructive, itvp_t* a,
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

itvp_t* itvp_fold(ap_manager_t* man,
		bool destructive, itvp_t* a,
		ap_dim_t* tdim,
		size_t size);
  /* Fold the dimensions in the array tdim of size n>=1 and put the result
     in the first dimension in the array. The other dimensions of the array
     are then removed. */

/* ============================================================ */
/* III.6 Widening, Narrowing */
/* ============================================================ */

itvp_t* itvp_widening(ap_manager_t* man, itvp_t* a1, itvp_t* a2);
  /* Standard widening: set unstable constraints to +oo */

itvp_t* itvp_widening_thresholds(ap_manager_t* man,
			       itvp_t* a1, itvp_t* a2,
			       ap_scalar_t** array,
			       size_t nb);
  /* Widening with threshold.
     array is assumed to contain nb thresholds, sorted in increasing order. */

itvp_t* itvp_narrowing(ap_manager_t* man, itvp_t* a1, itvp_t* a2);
  /* Standard narrowing: refine only +oo constraint */

itvp_t* itvp_add_epsilon(ap_manager_t* man, itvp_t* a, ap_scalar_t* epsilon);
  /* Enlarge each bound by epsilon times the maximum finite bound in
     the itvpol */

itvp_t* itvp_add_epsilon_bin(ap_manager_t* man, itvp_t* a1, itvp_t* a2,
			   ap_scalar_t* epsilon);
  /* Enlarge each bound from a1 by epsilon times the maximum finite bound in
     a2. Only bounds in a1 that are not stable in a2 are enlared. */


/* ============================================================ */
/* III.7 Topological closure operation */
/* ============================================================ */

itvp_t* itvp_closure(ap_manager_t* man, bool destructive, itvp_t* a);
  /* Returns the topological closure of a possibly opened abstract value */


#ifdef __cplusplus
}
#endif

#endif /* __ITVP_FUN_H */
