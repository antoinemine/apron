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

#ifndef __PPL_BD_SHAPE_H
#define __PPL_BD_SHAPE_H

#include "ap_global0.h"

#ifdef __cplusplus
#include "ppl.hh"

class PPL_BD_Shape {
public:
  BD_Shape<mpq_class>* p;
  ap_dimension_t dim;

  PPL_BD_Shape(ap_manager_t* man, const PPL_BD_Shape& x);
  PPL_BD_Shape(ap_manager_t* man,
	   ap_dimension_t dim, Degenerate_Element kind);
  ~PPL_BD_Shape();
};
extern "C" {
#else
typedef struct PPL_BD_Shape* PPL_BD_Shape;
#endif

typedef enum ap_ppl_bd_shape_widening_option_t {
  ap_ppl_bd_shape_widening_H79,
  ap_ppl_bd_shape_widening_BHMZ05
} ap_ppl_bd_shape_widening_option_t;
typedef struct ap_ppl_bd_shape_option_t {
  ap_ppl_bd_shape_widening_option_t widening;
} ap_ppl_bd_shape_option_t;

/* ============================================================ */
/* A. Constructor for APRON manager (to be freed with ap_manager_free). */
/* ============================================================ */

ap_manager_t* ap_ppl_bd_shape_manager_alloc(void);
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

PPL_BD_Shape* ap_ppl_bd_shape_copy(ap_manager_t* man, PPL_BD_Shape* a);
  /* Return a copy of an abstract value, on
     which destructive update does not affect the initial value. */

void ap_ppl_bd_shape_free(ap_manager_t* man, PPL_BD_Shape* a);
  /* Free all the memory used by the abstract value */

size_t ap_ppl_bd_shape_size(ap_manager_t* man, PPL_BD_Shape* a);
  /* Return the abstract size of a polyhedron, which is the number of
     coefficients of its current representation, possibly redundant. */


/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */

void ap_ppl_bd_shape_minimize(ap_manager_t* man, PPL_BD_Shape* a);
  /* Same as ap_ppl_bd_shape_canonicalize */

void ap_ppl_bd_shape_canonicalize(ap_manager_t* man, PPL_BD_Shape* a);
  /* Put the polyhedron with minimized constraints and frames.  If in addition
     the integer man->option->canonicalize.algorithm is strictly positive,
     normalize equalities and lines, and also strict constraints */

int ap_ppl_bd_shape_hash(ap_manager_t* man, PPL_BD_Shape* a);

void ap_ppl_bd_shape_approximate(ap_manager_t* man, PPL_BD_Shape* a, int algorithm);
  /* Do nothing */

/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */

void ap_ppl_bd_shape_fprint(FILE* stream,
			ap_manager_t* man,
			PPL_BD_Shape* a,
			char** name_of_dim);
  /* Print the abstract value in a pretty way, using function
     name_of_dim to name dimensions */

void ap_ppl_bd_shape_fprintdiff(FILE* stream,
			    ap_manager_t* man,
			    PPL_BD_Shape* a1, PPL_BD_Shape* a2,
			    char** name_of_dim);
  /* Print the difference between a1 (old value) and a2 (new value),
     using function name_of_dim to name dimensions.
     The meaning of difference is library dependent.

     Not implemented */

void ap_ppl_bd_shape_fdump(FILE* stream, ap_manager_t* man, PPL_BD_Shape* a);
  /* Dump the internal representation of an abstract value,
     for debugging purposes */


/* ============================================================ */
/* I.4 Serialization */
/* ============================================================ */

ap_membuf_t ap_ppl_bd_shape_serialize_raw(ap_manager_t* man, PPL_BD_Shape* a);
/* Allocate a memory buffer (with malloc), output the abstract value in raw
   binary format to it and return a pointer on the memory buffer and the size
   of bytes written.  It is the user responsability to free the memory
   afterwards (with free).

   Not implemented */

PPL_BD_Shape* ap_ppl_bd_shape_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size);
/* Return the abstract value read in raw binary format from the input stream
   and store in size the number of bytes read.

   Not implemented */

/* ********************************************************************** */
/* II. Constructor, accessors, tests and property extraction */
/* ********************************************************************** */

/* ============================================================ */
/* II.1 Basic constructors */
/* ============================================================ */

/* We assume that dimensions [0..intd-1] correspond to integer variables, and
   dimensions [intdim..intd+realdim-1] to real variables */

PPL_BD_Shape* ap_ppl_bd_shape_bottom(ap_manager_t* man, ap_dimension_t dim);
  /* Create a bottom (empty) value */

PPL_BD_Shape* ap_ppl_bd_shape_top(ap_manager_t* man, ap_dimension_t dim);
  /* Create a top (universe) value */


PPL_BD_Shape* ap_ppl_bd_shape_of_box(ap_manager_t* man,
		ap_dimension_t dim,
		ap_linexpr0_t box);
  /* Abstract an hypercube defined by the array of intervals
     of size intdim+realdim */

/* ============================================================ */
/* II.2 Accessors */
/* ============================================================ */

ap_dimension_t ap_ppl_bd_shape_dimension(ap_manager_t* man, PPL_BD_Shape* a);
/* Return the total number of dimensions of the abstract values */

/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */

bool ap_ppl_bd_shape_is_bottom(ap_manager_t* man, PPL_BD_Shape* a);
  /* Emptiness test */
bool ap_ppl_bd_shape_is_top(ap_manager_t* man, PPL_BD_Shape* a);
  /* Universe test */

bool ap_ppl_bd_shape_is_leq(ap_manager_t* man, PPL_BD_Shape* a1, PPL_BD_Shape* a2);
  /* Inclusion test. Is always strict */

bool ap_ppl_bd_shape_is_eq(ap_manager_t* man, PPL_BD_Shape* a1, PPL_BD_Shape* a2);
  /* Equality test. Is always strict */

bool ap_ppl_bd_shape_sat_lincons(ap_manager_t* man, PPL_BD_Shape* a, ap_lincons0_t lincons);
  /* Satisfiability of a linear constraint. Is always strict */

bool ap_ppl_bd_shape_sat_tcons(ap_manager_t* man, PPL_BD_Shape* a, ap_tcons0_t* cons);
  /* Satisfiability of a tree expression constraint. */

bool ap_ppl_bd_shape_sat_interval(ap_manager_t* man, PPL_BD_Shape* a,
			ap_dim_t dim, ap_coeff_t interval);
  /* Inclusion of a dimension in an interval. Is always strict
  */

bool ap_ppl_bd_shape_is_dimension_unconstrained(ap_manager_t* man, PPL_BD_Shape* po,
				      ap_dim_t dim);
  /* Is a dimension unconstrained ? Is always strict */

/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */

void ap_ppl_bd_shape_bound_dimension(ap_manager_t* man,
			   ap_coeff_t interval, PPL_BD_Shape* a, ap_dim_t dim);
  /* Returns the interval taken by the dimension
     over the abstract value
  */

void ap_ppl_bd_shape_bound_linexpr(ap_manager_t* man,
			 ap_coeff_t interval, PPL_BD_Shape* a, ap_linexpr0_t expr);
  /* Returns the interval taken by a linear expression
     over the abstract value.
  */

void ap_ppl_bd_shape_bound_texpr(ap_manager_t* man,
		       ap_coeff_t interval, PPL_BD_Shape* a, ap_texpr0_t* expr);
  /* Returns the interval taken by a tree expression
     over the abstract value. */

void ap_ppl_bd_shape_to_box(ap_manager_t* man, ap_linexpr0_t box, PPL_BD_Shape* a);
  /* Converts an abstract value to an interval/hypercube.
     The size of the resulting array is ap_ppl_bd_shape_dimension(man,a).  This
     function can be reimplemented by using ap_ppl_bd_shape_bound_linexpr
  */

void ap_ppl_bd_shape_to_lincons_array(ap_manager_t* man, ap_lincons0_array_t array, PPL_BD_Shape* a);
  /* Converts an abstract value to a polyhedra
     (conjunction of linear constraints).

     Always consider canonical form */

ap_tcons0_array_t ap_ppl_bd_shape_to_tcons_array(ap_manager_t* man, PPL_BD_Shape* a);
  /* Converts an abstract value to a
     conjunction of tree expressions constraints. */

void ap_ppl_bd_shape_to_lingen_array(ap_manager_t* man, ap_lingen0_array_t array, PPL_BD_Shape* a);
  /* Converts an abstract value to a system of generators.
     Always consider canonical form. */

/* ********************************************************************** */
/* III. Operations */
/* ********************************************************************** */

/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */

PPL_BD_Shape* ap_ppl_bd_shape_meet(ap_manager_t* man, bool destructive, PPL_BD_Shape* a1, PPL_BD_Shape* a2);
PPL_BD_Shape* ap_ppl_bd_shape_join(ap_manager_t* man, bool destructive, PPL_BD_Shape* a1, PPL_BD_Shape* a2);
  /* Meet and Join of 2 abstract values */

PPL_BD_Shape* ap_ppl_bd_shape_meet_array(ap_manager_t* man, PPL_BD_Shape** tab, size_t size);
PPL_BD_Shape* ap_ppl_bd_shape_join_array(ap_manager_t* man, PPL_BD_Shape** tab, size_t size);
  /* Meet and Join of a non empty array of abstract values. */

PPL_BD_Shape* ap_ppl_bd_shape_meet_lincons_array(ap_manager_t* man,
				  bool destructive, PPL_BD_Shape* a,
				  ap_lincons0_array_t array);
PPL_BD_Shape* ap_ppl_bd_shape_meet_tcons_array(ap_manager_t* man,
				bool destructive, PPL_BD_Shape* a,
				ap_tcons0_array_t* array);
  /* Meet of an abstract value with a set of constraints. */

PPL_BD_Shape* ap_ppl_bd_shape_add_ray_array(ap_manager_t* man,
			     bool destructive, PPL_BD_Shape* a,
			     ap_lingen0_array_t array);
  /* Generalized time elapse operator */

/* ============================================================ */
/* III.2 Assignement and Substitutions */
/* ============================================================ */

PPL_BD_Shape* ap_ppl_bd_shape_assign_linexpr_array(ap_manager_t* man,
				    bool destructive, PPL_BD_Shape* a,
				    ap_dim_t* tdim,
				    ap_linexpr0_array_t array,
				    PPL_BD_Shape* dest);
PPL_BD_Shape* ap_ppl_bd_shape_substitute_linexpr_array(ap_manager_t* man,
					bool destructive, PPL_BD_Shape* a,
					ap_dim_t* tdim,
					ap_linexpr0_array_t array,
					PPL_BD_Shape* dest);
PPL_BD_Shape* ap_ppl_bd_shape_assign_texpr_array(ap_manager_t* man,
				  bool destructive, PPL_BD_Shape* a,
				  ap_dim_t* tdim,
				  ap_texpr0_array_t* array,
				  PPL_BD_Shape* dest);
PPL_BD_Shape* ap_ppl_bd_shape_substitute_texpr_array(ap_manager_t* man,
				      bool destructive, PPL_BD_Shape* a,
				      ap_dim_t* tdim,
				      ap_texpr0_array_t* array,
				      PPL_BD_Shape* dest);
  /* Parallel Assignement and Substitution of several dimensions by interval
     expressons. */

/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

PPL_BD_Shape* ap_ppl_bd_shape_forget_array(ap_manager_t* man,
			    bool destructive, PPL_BD_Shape* a,
			    ap_dim_t* tdim, size_t size,
			    bool project);

/* ============================================================ */
/* III.4 Change and permutation of dimensions */
/* ============================================================ */

PPL_BD_Shape* ap_ppl_bd_shape_add_dimensions(ap_manager_t* man,
			      bool destructive, PPL_BD_Shape* a,
			      ap_dimchange_t* dimchange,
			      bool project);

PPL_BD_Shape* ap_ppl_bd_shape_remove_dimensions(ap_manager_t* man,
				 bool destructive, PPL_BD_Shape* a,
				 ap_dimchange_t* dimchange);
PPL_BD_Shape* ap_ppl_bd_shape_permute_dimensions(ap_manager_t* man,
				  bool destructive,
				  PPL_BD_Shape* a,
				  ap_dimperm_t* permutation);

/* ============================================================ */
/* III.5 Expansion and folding of dimensions */
/* ============================================================ */

PPL_BD_Shape* ap_ppl_bd_shape_expand(ap_manager_t* man,
		      bool destructive, PPL_BD_Shape* a,
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

PPL_BD_Shape* ap_ppl_bd_shape_fold(ap_manager_t* man,
		    bool destructive, PPL_BD_Shape* a,
		    ap_dim_t* tdim,
		    size_t size);
  /* Fold the dimensions in the array tdim of size n>=1 and put the result
     in the first dimension in the array. The other dimensions of the array
     are then removed (using ap_ppl_bd_shape_permute_remove_dimensions). */

/* ============================================================ */
/* III.6 Widening */
/* ============================================================ */

/* Widening */

PPL_BD_Shape* ap_ppl_bd_shape_widening(ap_manager_t* man, PPL_BD_Shape* a1, PPL_BD_Shape* a2);

/* ============================================================ */
/* III.7 Closure operation */
/* ============================================================ */

/* Returns the topological closure of a possibly opened abstract value */

PPL_BD_Shape* ap_ppl_bd_shape_closure(ap_manager_t* man, bool destructive, PPL_BD_Shape* a);

#ifdef __cplusplus
}
#endif

#endif
