/*
 * ppl_poly.hh
 *
 * APRON Library / PPL library wrapper
 *
 * Wrappers specific to the Polyhedron PPL classes.
 *
 * Copyright (C) Antoine Mine' 2006
 *
 */

/* 
 * This file is part of the APRON Library, released under GPL license.
 * Please read the COPYING file packaged in the distribution.
 */

#ifndef __PPL_POLY_H
#define __PPL_POLY_H

#include "ppl.hh"

using namespace std;
using namespace Parma_Polyhedra_Library;

class PPL_Poly {
public:
  Polyhedron* p;
  size_t intdim;

  PPL_Poly(ap_manager_t* man, const PPL_Poly& x);
  PPL_Poly(ap_manager_t* man, 
	   size_t intdim, size_t realdim, Degenerate_Element kind);
  ~PPL_Poly();
};

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================ */
/* A. Constructor for APRON manager (to be freed with ap_manager_free). */
/* ============================================================ */

ap_manager_t* ap_ppl_poly_manager_alloc(bool strict);
  /* Allocate a PPL manager for convex polyhedra.

     If the Boolean parameter is true, abstract values generated with the
     manager can have strict constraints (like x>0). Otherwise they are defined
     using only loose constraints. Managers and abstract values in strict or
     loose mode are incompatible.
  */

/* ********************************************************************** */
/* I. General management */
/* ********************************************************************** */

/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

PPL_Poly* ap_ppl_poly_copy(ap_manager_t* man, PPL_Poly* a);
  /* Return a copy of an abstract value, on
     which destructive update does not affect the initial value. */

void ap_ppl_poly_free(ap_manager_t* man, PPL_Poly* a);
  /* Free all the memory used by the abstract value */

size_t ap_ppl_poly_size(ap_manager_t* man, PPL_Poly* a);
  /* Return the abstract size of a polyhedron, which is the number of
     coefficients of its current representation, possibly redundant. */


/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */

void ap_ppl_poly_minimize(ap_manager_t* man, PPL_Poly* a);
  /* Same as ap_ppl_poly_canonicalize */

void ap_ppl_poly_canonicalize(ap_manager_t* man, PPL_Poly* a);
  /* Put the polyhedron with minimized constraints and frames.  If in addition
     the integer man->option->canonicalize.algorithm is strictly positive,
     normalize equalities and lines, and also strict constraints */

int ap_ppl_poly_hash(ap_manager_t* man, PPL_Poly* a);

void ap_ppl_poly_approximate(ap_manager_t* man, PPL_Poly* a, int algorithm);
  /* Do nothing */

/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */

void ap_ppl_poly_fprint(FILE* stream,
			ap_manager_t* man,
			PPL_Poly* a,
			char** name_of_dim);
  /* Print the abstract value in a pretty way, using function
     name_of_dim to name dimensions */

void ap_ppl_poly_fprintdiff(FILE* stream,
			    ap_manager_t* man,
			    PPL_Poly* a1, PPL_Poly* a2,
			    char** name_of_dim);
  /* Print the difference between a1 (old value) and a2 (new value),
     using function name_of_dim to name dimensions.
     The meaning of difference is library dependent. 

     Not implemented */

void ap_ppl_poly_fdump(FILE* stream, ap_manager_t* man, PPL_Poly* a);
  /* Dump the internal representation of an abstract value,
     for debugging purposes */


/* ============================================================ */
/* I.4 Serialization */
/* ============================================================ */

ap_membuf_t ap_ppl_poly_serialize_raw(ap_manager_t* man, PPL_Poly* a);
/* Allocate a memory buffer (with malloc), output the abstract value in raw
   binary format to it and return a pointer on the memory buffer and the size
   of bytes written.  It is the user responsability to free the memory
   afterwards (with free).  

   Not implemented */

PPL_Poly* ap_ppl_poly_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size);
/* Return the abstract value read in raw binary format from the input stream
   and store in size the number of bytes read.  

   Not implemented */

/* ********************************************************************** */
/* II. Constructor, accessors, tests and property extraction */
/* ********************************************************************** */

/* ============================================================ */
/* II.1 Basic constructors */
/* ============================================================ */

/* We assume that dimensions [0..intdim-1] correspond to integer variables, and
   dimensions [intdim..intdim+realdim-1] to real variables */

PPL_Poly* ap_ppl_poly_bottom(ap_manager_t* man, size_t intdim, size_t realdim);
  /* Create a bottom (empty) value */

PPL_Poly* ap_ppl_poly_top(ap_manager_t* man, size_t intdim, size_t realdim);
  /* Create a top (universe) value */


PPL_Poly* ap_ppl_poly_of_box(ap_manager_t* man,
			     size_t intdim, size_t realdim,
			     ap_interval_t** tinterval);
  /* Abstract an hypercube defined by the array of intervals
     of size intdim+realdim */

/* ============================================================ */
/* II.2 Accessors */
/* ============================================================ */

ap_dimension_t ap_ppl_poly_dimension(ap_manager_t* man, PPL_Poly* a);
/* Return the total number of dimensions of the abstract values */

/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */

bool ap_ppl_poly_is_bottom(ap_manager_t* man, PPL_Poly* a);
  /* Emptiness test */
bool ap_ppl_poly_is_top(ap_manager_t* man, PPL_Poly* a);
  /* Universe test */

bool ap_ppl_poly_is_leq(ap_manager_t* man, PPL_Poly* a1, PPL_Poly* a2);
  /* Inclusion test */
  
bool ap_ppl_poly_is_eq(ap_manager_t* man, PPL_Poly* a1, PPL_Poly* a2);
  /* Equality test */
 
bool ap_ppl_poly_sat_lincons(ap_manager_t* man, PPL_Poly* a, ap_lincons0_t* lincons);
  /* Satisfiability of a linear constraint */

bool ap_ppl_poly_sat_tcons(ap_manager_t* man, PPL_Poly* a, ap_tcons0_t* cons);
  /* Satisfiability of a tree expression constraint. */

bool ap_ppl_poly_sat_interval(ap_manager_t* man, PPL_Poly* a,
			      ap_dim_t dim, ap_interval_t* interval);
  /* Inclusion of a dimension in an interval */

bool ap_ppl_poly_is_dimension_unconstrained(ap_manager_t* man, PPL_Poly* po,
					    ap_dim_t dim);
  /* Is a dimension unconstrained ? */

/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */

ap_interval_t* ap_ppl_poly_bound_linexpr(ap_manager_t* man,
					 PPL_Poly* a, ap_linexpr0_t* expr);
  /* Returns the interval taken by a linear expression
     over the abstract value. */

ap_interval_t* ap_ppl_poly_bound_texpr(ap_manager_t* man,
			      PPL_Poly* a, ap_texpr0_t* expr);
  /* Returns the interval taken by a tree expression
     over the abstract value. */

ap_interval_t* ap_ppl_poly_bound_dimension(ap_manager_t* man,
					   PPL_Poly* a, ap_dim_t dim);
  /* Returns the interval taken by the dimension
     over the abstract value. */

ap_lincons0_array_t ap_ppl_poly_to_lincons_array(ap_manager_t* man, PPL_Poly* a);
  /* Converts an abstract value to a polyhedra
     (conjunction of linear constraints). */

ap_tcons0_array_t ap_ppl_poly_to_tcons_array(ap_manager_t* man, PPL_Poly* a);
  /* Converts an abstract value to a 
     conjunction of tree expressions constraints. */

ap_interval_t** ap_ppl_poly_to_box(ap_manager_t* man, PPL_Poly* a);
  /* Converts an abstract value to an interval/hypercube.
     The size of the resulting array is ap_ppl_poly_dimension(man,a). */

ap_generator0_array_t ap_ppl_poly_to_generator_array(ap_manager_t* man, PPL_Poly* a);
  /* Converts an abstract value to a system of generators. */


/* ********************************************************************** */
/* III. Operations */
/* ********************************************************************** */

/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */

PPL_Poly* ap_ppl_poly_meet(ap_manager_t* man, bool destructive, PPL_Poly* a1, PPL_Poly* a2);
PPL_Poly* ap_ppl_poly_join(ap_manager_t* man, bool destructive, PPL_Poly* a1, PPL_Poly* a2);
  /* Meet and Join of 2 abstract values */

PPL_Poly* ap_ppl_poly_meet_array(ap_manager_t* man, PPL_Poly** tab, size_t size);
PPL_Poly* ap_ppl_poly_join_array(ap_manager_t* man, PPL_Poly** tab, size_t size);
  /* Meet and Join of a non-empty array of abstract values.
     (no way to define the dimensionality of the result in such a case */

PPL_Poly* ap_ppl_poly_meet_lincons_array(ap_manager_t* man,
			    bool destructive, PPL_Poly* a,
			    ap_lincons0_array_t* array);
  /* Meet of an abstract value with a set of constraints
     (generalize ap_ppl_poly_of_lincons_array) */

PPL_Poly* ap_ppl_poly_meet_tcons_array(ap_manager_t* man,
			  bool destructive, PPL_Poly* a,
			  ap_tcons0_array_t* array);
  /* Meet of an abstract value with a set of tree expressionsconstraints. */

PPL_Poly* ap_ppl_poly_add_ray_array(ap_manager_t* man,
		       bool destructive, PPL_Poly* a,
		       ap_generator0_array_t* array);
  /* Generalized time elapse operator */

/* ============================================================ */
/* III.2 Assignement and Substitutions */
/* ============================================================ */

PPL_Poly* ap_ppl_poly_assign_linexpr_array(ap_manager_t* man,
					   bool destructive, PPL_Poly* a,
					   ap_dim_t* tdim,
					   ap_linexpr0_t** texpr,
					   size_t size,
					   PPL_Poly* dest);
PPL_Poly* ap_ppl_poly_substitute_linexpr_array(ap_manager_t* man,
					       bool destructive, PPL_Poly* a,
					       ap_dim_t* tdim,
					       ap_linexpr0_t** texpr,
					       size_t size,
					       PPL_Poly* dest);
PPL_Poly* ap_ppl_poly_assign_texpr_array(ap_manager_t* man,
					 bool destructive, PPL_Poly* a,
					 ap_dim_t* tdim,
					 ap_texpr0_t** texpr,
					 size_t size,
					 PPL_Poly* dest);
PPL_Poly* ap_ppl_poly_substitute_texpr_array(ap_manager_t* man,
					     bool destructive, PPL_Poly* a,
					     ap_dim_t* tdim,
					     ap_texpr0_t** texpr,
					     size_t size,
					     PPL_Poly* dest);
  /* Parallel Assignement and Substitution of several dimensions by
     expressons. */

/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

PPL_Poly* ap_ppl_poly_forget_array(ap_manager_t* man,
		      bool destructive, PPL_Poly* a,
		      ap_dim_t* tdim, size_t size,
		      bool project);

/* ============================================================ */
/* III.4 Change and permutation of dimensions */
/* ============================================================ */

PPL_Poly* ap_ppl_poly_add_dimensions(ap_manager_t* man,
			bool destructive, PPL_Poly* a,
			ap_dimchange_t* dimchange,
			bool project);

PPL_Poly* ap_ppl_poly_remove_dimensions(ap_manager_t* man,
			   bool destructive, PPL_Poly* a,
			   ap_dimchange_t* dimchange);
PPL_Poly* ap_ppl_poly_permute_dimensions(ap_manager_t* man,
			    bool destructive,
			    PPL_Poly* a,
			    ap_dimperm_t* permutation);

/* ============================================================ */
/* III.5 Expansion and folding of dimensions */
/* ============================================================ */

PPL_Poly* ap_ppl_poly_expand(ap_manager_t* man,
		bool destructive, PPL_Poly* a,
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

PPL_Poly* ap_ppl_poly_fold(ap_manager_t* man,
	      bool destructive, PPL_Poly* a,
	      ap_dim_t* tdim,
	      size_t size);
  /* Fold the dimensions in the array tdim of size n>=1 and put the result
     in the first dimension in the array. The other dimensions of the array
     are then removed (using ap_ppl_poly_permute_remove_dimensions). */

/* ============================================================ */
/* III.6 Widening */
/* ============================================================ */

/* Widening */

PPL_Poly* ap_ppl_poly_widening(ap_manager_t* man, PPL_Poly* a1, PPL_Poly* a2);

/* ============================================================ */
/* III.7 Closure operation */
/* ============================================================ */

/* Returns the topological closure of a possibly opened abstract value */

PPL_Poly* ap_ppl_poly_closure(ap_manager_t* man, bool destructive, PPL_Poly* a);


#ifdef __cplusplus
}
#endif

#endif
