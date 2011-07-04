/*
 * ppl_grid.hh
 *
 * APRON Library / PPL library wrapper
 *
 * Wrappers specific to the Grid PPL classes.
 *
 * Copyright (C) Antoine Mine' 2006
 *
 */

/* 
 * This file is part of the APRON Library, released under GPL license.
 * Please read the COPYING file packaged in the distribution.
 */

#ifndef __PPL_GRID_HH
#define __PPL_GRID_HH

#include "ppl.hh"

using namespace std;
using namespace Parma_Polyhedra_Library;

class PPL_Grid {
public:
  Grid* p;
  size_t intdim;

  PPL_Grid(const PPL_Grid& x);
  PPL_Grid(size_t intdim, size_t realdim, Degenerate_Element kind);
  ~PPL_Grid();

  /* enforce integer constraints */
  void reduce();
  void forget_dim(size_t dim);
};
#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================ */
/* A. Constructor for APRON manager (to be freed with ap_manager_free). */
/* ============================================================ */

ap_manager_t* ap_ppl_grid_manager_alloc();
  /* Allocate a PPL manager for grids. */

/* ********************************************************************** */
/* I. General management */
/* ********************************************************************** */

/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

PPL_Grid* ap_ppl_grid_copy(ap_manager_t* man, PPL_Grid* a);
  /* Return a copy of an abstract value, on
     which destructive update does not affect the initial value. */

void ap_ppl_grid_free(ap_manager_t* man, PPL_Grid* a);
  /* Free all the memory used by the abstract value */

size_t ap_ppl_grid_size(ap_manager_t* man, PPL_Grid* a);
  /* Return the abstract size of an abstract value, which is the number of
     coefficients of its current representation, possibly redundant. */


/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */

void ap_ppl_grid_minimize(ap_manager_t* man, PPL_Grid* a);
  /* Same as ap_ppl_grid_canonicalize */

void ap_ppl_grid_canonicalize(ap_manager_t* man, PPL_Grid* a);
  /* Put the polyhedron with minimized constraints and frames.  If in addition
     the integer man->option->canonicalize.algorithm is strictly positive,
     normalize equalities and lines, and also strict constraints */

int ap_ppl_grid_hash(ap_manager_t* man, PPL_Grid* a);

void ap_ppl_grid_approximate(ap_manager_t* man, PPL_Grid* a, int algorithm);
  /* Do nothing */

/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */

void ap_ppl_grid_fprint(FILE* stream,
			ap_manager_t* man,
			PPL_Grid* a,
			char** name_of_dim);
  /* Print the abstract value in a pretty way, using function
     name_of_dim to name dimensions */

void ap_ppl_grid_fprintdiff(FILE* stream,
			    ap_manager_t* man,
			    PPL_Grid* a1, PPL_Grid* a2,
			    char** name_of_dim);
  /* Print the difference between a1 (old value) and a2 (new value),
     using function name_of_dim to name dimensions.
     The meaning of difference is library dependent. 

     Not implemented */

void ap_ppl_grid_fdump(FILE* stream, ap_manager_t* man, PPL_Grid* a);
  /* Dump the internal representation of an abstract value,
     for debugging purposes */


/* ============================================================ */
/* I.4 Serialization */
/* ============================================================ */

ap_membuf_t ap_ppl_grid_serialize_raw(ap_manager_t* man, PPL_Grid* a);
/* Allocate a memory buffer (with malloc), output the abstract value in raw
   binary format to it and return a pointer on the memory buffer and the size
   of bytes written.  It is the user responsability to free the memory
   afterwards (with free).  

   Not implemented */

PPL_Grid* ap_ppl_grid_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size);
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

PPL_Grid* ap_ppl_grid_bottom(ap_manager_t* man, size_t intdim, size_t realdim);
  /* Create a bottom (empty) value */

PPL_Grid* ap_ppl_grid_top(ap_manager_t* man, size_t intdim, size_t realdim);
  /* Create a top (universe) value */


PPL_Grid* ap_ppl_grid_of_box(ap_manager_t* man,
			     size_t intdim, size_t realdim,
			     ap_interval_t** tinterval);
  /* Abstract an hypercube defined by the array of intervals
     of size intdim+realdim */

PPL_Grid* ap_ppl_grid_of_lincons_array(ap_manager_t* man,
				       size_t intdim, size_t realdim,
				       ap_lincons0_array_t* array);
  /* Abstract a convex polyhedra defined by the array of linear constraints
     of size size */

PPL_Grid* ap_ppl_grid_of_tcons_array(ap_manager_t* man,
			size_t intdim, size_t realdim,
			ap_tcons0_array_t* array);
  /* Abstract a conjunction of tree expressions constraints
     of size size */

/* ============================================================ */
/* II.2 Accessors */
/* ============================================================ */

ap_dimension_t ap_ppl_grid_dimension(ap_manager_t* man, PPL_Grid* a);
/* Return the total number of dimensions of the abstract values */

/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */

bool ap_ppl_grid_is_bottom(ap_manager_t* man, PPL_Grid* a);
  /* Emptiness test */
bool ap_ppl_grid_is_top(ap_manager_t* man, PPL_Grid* a);
  /* Universe test */

bool ap_ppl_grid_is_leq(ap_manager_t* man, PPL_Grid* a1, PPL_Grid* a2);
  /* Inclusion test */
  
bool ap_ppl_grid_is_eq(ap_manager_t* man, PPL_Grid* a1, PPL_Grid* a2);
  /* Equality test */
 
bool ap_ppl_grid_sat_lincons(ap_manager_t* man, PPL_Grid* a, ap_lincons0_t* lincons);
  /* Satisfiability of a linear constraint */

bool ap_ppl_grid_sat_tcons(ap_manager_t* man, PPL_Grid* a, ap_tcons0_t* cons);
  /* Satisfiability of a tree expression constraint. */

bool ap_ppl_grid_sat_interval(ap_manager_t* man, PPL_Grid* a,
			      ap_dim_t dim, ap_interval_t* interval);
  /* Inclusion of a dimension in an interval */

bool ap_ppl_grid_is_dimension_unconstrained(ap_manager_t* man, PPL_Grid* po,
					    ap_dim_t dim);
  /* Is a dimension unconstrained ? */

/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */

ap_interval_t* ap_ppl_grid_bound_linexpr(ap_manager_t* man,
					 PPL_Grid* a, ap_linexpr0_t* expr);
  /* Returns the interval taken by a linear expression
     over the abstract value. */

ap_interval_t* ap_ppl_grid_bound_texpr(ap_manager_t* man,
			      PPL_Grid* a, ap_texpr0_t* expr);
  /* Returns the interval taken by a tree expression
     over the abstract value. */

ap_interval_t* ap_ppl_grid_bound_dimension(ap_manager_t* man,
					   PPL_Grid* a, ap_dim_t dim);
  /* Returns the interval taken by the dimension
     over the abstract value. */

ap_lincons0_array_t ap_ppl_grid_to_lincons_array(ap_manager_t* man, PPL_Grid* a);
  /* Converts an abstract value to a gridhedra
     (conjunction of linear constraints). */

ap_tcons0_array_t ap_ppl_grid_to_tcons_array(ap_manager_t* man, PPL_Grid* a);
  /* Converts an abstract value to a 
     conjunction of tree expressions constraints. */

ap_interval_t** ap_ppl_grid_to_box(ap_manager_t* man, PPL_Grid* a);
  /* Converts an abstract value to an interval/hypercube.
     The size of the resulting array is ap_ppl_grid_dimension(man,a). */

ap_generator0_array_t ap_ppl_grid_to_generator_array(ap_manager_t* man, PPL_Grid* a);
  /* Converts an abstract value to a system of generators. */


/* ********************************************************************** */
/* III. Operations */
/* ********************************************************************** */

/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */

PPL_Grid* ap_ppl_grid_meet(ap_manager_t* man, bool destructive, PPL_Grid* a1, PPL_Grid* a2);
PPL_Grid* ap_ppl_grid_join(ap_manager_t* man, bool destructive, PPL_Grid* a1, PPL_Grid* a2);
  /* Meet and Join of 2 abstract values */

PPL_Grid* ap_ppl_grid_meet_array(ap_manager_t* man, PPL_Grid** tab, size_t size);
PPL_Grid* ap_ppl_grid_join_array(ap_manager_t* man, PPL_Grid** tab, size_t size);
  /* Meet and Join of a non-empty array of abstract values.
     (no way to define the dimensionality of the result in such a case */

PPL_Grid* ap_ppl_grid_meet_lincons_array(ap_manager_t* man,
			    bool destructive, PPL_Grid* a,
			    ap_lincons0_array_t* array);
  /* Meet of an abstract value with a set of constraints
     (generalize ap_ppl_grid_of_lincons_array) */

PPL_Grid* ap_ppl_grid_meet_tcons_array(ap_manager_t* man,
			  bool destructive, PPL_Grid* a,
			  ap_tcons0_array_t* array);
  /* Meet of an abstract value with a set of tree expressionsconstraints. */

PPL_Grid* ap_ppl_grid_add_ray_array(ap_manager_t* man,
		       bool destructive, PPL_Grid* a,
		       ap_generator0_array_t* array);
  /* Generalized time elapse operator */

/* ============================================================ */
/* III.2 Assignement and Substitutions */
/* ============================================================ */

PPL_Grid* ap_ppl_grid_assign_linexpr(ap_manager_t* man,
			bool destructive, PPL_Grid* a,
			ap_dim_t dim, ap_linexpr0_t* expr,
			PPL_Grid* dest);
PPL_Grid* ap_ppl_grid_substitute_linexpr(ap_manager_t* man,
			    bool destructive, PPL_Grid* a,
			    ap_dim_t dim, ap_linexpr0_t* expr,
			    PPL_Grid* dest);
  /* Assignement and Substitution of a single dimension by resp.
     a linear expression and a interval linear expression */

PPL_Grid* ap_ppl_grid_assign_linexpr_array(ap_manager_t* man,
			      bool destructive, PPL_Grid* a,
			      ap_dim_t* tdim,
			      ap_linexpr0_t** texpr,
			      size_t size,
			      PPL_Grid* dest);
PPL_Grid* ap_ppl_grid_substitute_linexpr_array(ap_manager_t* man,
				  bool destructive, PPL_Grid* a,
				  ap_dim_t* tdim,
				  ap_linexpr0_t** texpr,
				  size_t size,
				  PPL_Grid* dest);
  /* Parallel Assignement and Substitution of several dimensions by
     linear expressons. */

PPL_Grid* ap_ppl_grid_assign_texpr(ap_manager_t* man,
		      bool destructive, PPL_Grid* a,
		      ap_dim_t dim, ap_texpr0_t* expr,
		      PPL_Grid* dest);
PPL_Grid* ap_ppl_grid_substitute_texpr(ap_manager_t* man,
			  bool destructive, PPL_Grid* a,
			  ap_dim_t dim, ap_texpr0_t* expr,
			  PPL_Grid* dest);
  /* Assignement and Substitution of a single dimension by
     a tree expression */

PPL_Grid* ap_ppl_grid_assign_texpr_array(ap_manager_t* man,
			    bool destructive, PPL_Grid* a,
			    ap_dim_t* tdim,
			    ap_texpr0_t** texpr,
			    size_t size,
			    PPL_Grid* dest);
PPL_Grid* ap_ppl_grid_substitute_texpr_array(ap_manager_t* man,
				bool destructive, PPL_Grid* a,
				ap_dim_t* tdim,
				ap_texpr0_t** texpr,
				size_t size,
				PPL_Grid* dest);
  /* Parallel Assignement and Substitution of several dimensions by
     tree expressions. */

/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

PPL_Grid* ap_ppl_grid_forget_array(ap_manager_t* man,
		      bool destructive, PPL_Grid* a,
		      ap_dim_t* tdim, size_t size,
		      bool project);

/* ============================================================ */
/* III.4 Change and permutation of dimensions */
/* ============================================================ */

PPL_Grid* ap_ppl_grid_add_dimensions(ap_manager_t* man,
			bool destructive, PPL_Grid* a,
			ap_dimchange_t* dimchange,
			bool project);

PPL_Grid* ap_ppl_grid_remove_dimensions(ap_manager_t* man,
			   bool destructive, PPL_Grid* a,
			   ap_dimchange_t* dimchange);
PPL_Grid* ap_ppl_grid_permute_dimensions(ap_manager_t* man,
			    bool destructive,
			    PPL_Grid* a,
			    ap_dimperm_t* permutation);

/* ============================================================ */
/* III.5 Expansion and folding of dimensions */
/* ============================================================ */

PPL_Grid* ap_ppl_grid_expand(ap_manager_t* man,
		bool destructive, PPL_Grid* a,
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

PPL_Grid* ap_ppl_grid_fold(ap_manager_t* man,
	      bool destructive, PPL_Grid* a,
	      ap_dim_t* tdim,
	      size_t size);
  /* Fold the dimensions in the array tdim of size n>=1 and put the result
     in the first dimension in the array. The other dimensions of the array
     are then removed (using ap_ppl_grid_permute_remove_dimensions). */

/* ============================================================ */
/* III.6 Widening */
/* ============================================================ */

/* Widening */

PPL_Grid* ap_ppl_grid_widening(ap_manager_t* man, PPL_Grid* a1, PPL_Grid* a2);

/* ============================================================ */
/* III.7 Closure operation */
/* ============================================================ */

/* Returns the topological closure of a possibly opened abstract value */

PPL_Grid* ap_ppl_grid_closure(ap_manager_t* man, bool destructive, PPL_Grid* a);


#ifdef __cplusplus
}
#endif

#endif
