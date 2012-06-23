/* ********************************************************************** */
/* box.h: abstract lattice of intervals */
/* ********************************************************************** */

#ifndef _BOX_H_
#define _BOX_H_

#include <stdio.h>
#include "ap_global0.h"

#ifdef __cplusplus
extern "C" {
#endif

/* OCaml interface */
ap_manager_t* box_manager_alloc(void);

typedef struct box_t box_t;

/* ********************************************************************** */
/* I. General management */
/* ********************************************************************** */

/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

box_t* box_copy(ap_manager_t* man, box_t* a);
  /* Return a copy of a box value, on
     which destructive update does not affect the initial value. */

void box_free(ap_manager_t* man, box_t* a);
  /* Free all the memory used by the box value */

size_t box_size(ap_manager_t* man, box_t* a);
  /* Return the itv size of a box value (see ap_manager_t) */

/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */

void box_minimize(ap_manager_t* man, box_t* a);
  /* Minimize the size of the representation of a.
     This may result in a later recomputation of internal information.
  */

void box_canonicalize(ap_manager_t* man, box_t* a);
  /* Put the box value in canonical form. (not yet clear definition) */
int box_hash(ap_manager_t* man, box_t* a);
  /* Return an hash code */

void box_approximate(ap_manager_t* man, box_t* a, int algorithm);
  /* Perform some transformation on the box value, guided by the
     field algorithm.

     The transformation may lose information. */

bool box_is_minimal(ap_manager_t* man, box_t* a);
bool box_is_canonical(ap_manager_t* man, box_t* a);

/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */

void box_fprint(FILE* stream,
	       ap_manager_t* man,
	       box_t* a,
	       char** name_of_dim);
  /* Print the box value in a pretty way, using function
     name_of_dim to name dimensions */

void box_fprintdiff(FILE* stream,
		   ap_manager_t* man,
		   box_t* a1, box_t* a2,
		   char** name_of_dim);
/* Print the difference between a1 (old value) and a2 (new value),
     using function name_of_dim to name dimensions.
     The meaning of difference is library dependent. */

void box_fdump(FILE* stream, ap_manager_t* man, box_t* a);
  /* Dump the internal representation of a box value,
     for debugging purposes */

/* ============================================================ */
/* I.4 Serialization */
/* ============================================================ */

ap_membuf_t box_serialize_raw(ap_manager_t* man, box_t* a);
/* Allocate a memory bfer (with malloc), output the box value in raw
   binary format to it and return a pointer on the memory buffer and the size
   of bytes written.  It is the user responsability to free the memory
   afterwards (with free). */

box_t* box_deserialize_raw(ap_manager_t* man, void* ptr);
/* Return the box value read in raw binary format from the input stream
   and store in size the number of bytes read */
/* ********************************************************************** */
/* II. Constructor, accessors, tests and property extraction */
/* ********************************************************************** */

/* ============================================================ */
/* II.1 Basic constructors */
/* ============================================================ */

/* We assume that dimensions [0..intdim-1] correspond to integer variables, and
   dimensions [intdim..intdim+realdim-1] to real variables */

box_t* box_bottom(ap_manager_t* man, size_t intdim, size_t realdim);
  /* Create a bottom (empty) value */

box_t* box_top(ap_manager_t* man, size_t intdim, size_t realdim);
  /* Create a top (universe) value */

box_t* box_of_box(ap_manager_t* man,
		  size_t intdim, size_t realdim,
		  ap_interval_t** tinterval);
  /* Abstract an hypercube defined by the array of intervals
     of size intdim+realdim */

/* ============================================================ */
/* II.2 Accessors */
/* ============================================================ */

ap_dimension_t box_dimension(ap_manager_t* man, box_t* a);
  /* Return the dimensionality of the box values */

/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */

bool box_is_bottom(ap_manager_t* man, box_t* a);
bool box_is_top(ap_manager_t* man, box_t* a);

bool box_is_leq(ap_manager_t* man, box_t* a1, box_t* a2);
  /* inclusion check */

bool box_is_eq(ap_manager_t* man, box_t* a1, box_t* a2);
  /* equality check */

bool box_sat_lincons(ap_manager_t* man, box_t* a, ap_lincons0_t* cons);
  /* does the box value satisfy the linear constraint ? */
bool box_sat_tcons(ap_manager_t* man, box_t* a, ap_tcons0_t* tcons);
  /* does the box value satisfy the tree expression constraint ? */

bool box_sat_interval(ap_manager_t* man,
		      box_t* a,
		      ap_dim_t dim, ap_interval_t* interval);
  /* is the dimension included in the interval in the box value ? */

bool box_is_dimension_unconstrained(ap_manager_t* man,
				    box_t* a, ap_dim_t dim);
  /* is the dimension unconstrained in the box value ?  If it
     is the case, we have forget(man,a,dim) == a */

/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */

ap_interval_t* box_bound_dimension(ap_manager_t* man,
				box_t* a, ap_dim_t dim);
  /* Returns the interval taken by a linear expression
     over the box value */

ap_interval_t* box_bound_linexpr(ap_manager_t* man,
				 box_t* a, ap_linexpr0_t* expr);
  /* Returns the interval taken by a linear expression
     over the box value */

ap_interval_t* box_bound_texpr(ap_manager_t* man,
			       box_t* a, ap_texpr0_t* expr);
  /* Returns the interval taken by a tree expression
     over the box value */

ap_interval_t** box_to_box(ap_manager_t* man, box_t* a);
  /* Converts a box value to an interval/hypercube.
     The size of the resulting array is box_dimension(man,a).  This
     function can be reimplemented by using box_bound_linexpr */

ap_lincons0_array_t box_to_lincons_array(ap_manager_t* man, box_t* a);
  /* Converts a box value to a polyhedra
     (conjunction of linear constraints).
     The size of the returned array is stored in size. */

ap_tcons0_array_t box_to_tcons_array(ap_manager_t* man, box_t* a);
  /* Converts an box value to conjunction of tree expressions constraints.

     The constraints are normally guaranteed to be scalar (without intervals) */

ap_generator0_array_t box_to_generator_array(ap_manager_t* man, box_t* a);
  /* Converts an box value to a system of generators. */

/* ********************************************************************** */
/* III. Operations: functional version */
/* ********************************************************************** */

/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */


box_t* box_meet(ap_manager_t* man, bool destructive, box_t* a1, box_t* a2);

box_t* box_join(ap_manager_t* man, bool destructive, box_t* a1, box_t* a2);
  /* Meet and Join of 2 box values */


box_t* box_meet_array(ap_manager_t* man, box_t** tab, size_t size);

box_t* box_join_array(ap_manager_t* man, box_t** tab, size_t size);
  /* Meet and Join of an array of box values.
     Raises an [[exc_invalid_argument]] exception if [[size==0]]
     (no way to define the dimensionality of the result in such a case */


box_t* box_meet_lincons_array(ap_manager_t* man,
			      bool destructive,
			      box_t* a,
			      ap_lincons0_array_t* array);
  /* Meet of an box value with a set of constraints */

box_t*
box_meet_tcons_array(ap_manager_t* man,
		     bool destructive, box_t* a, ap_tcons0_array_t* array);
  /* Meet of an box value with a set of tree expressions constraints.
     The algorithm field allows selecting between different algorithms
     and levels of precision:

     - between 0 and 99: legacy algorithm based on linearization.
     The algorithm value corresponds to the number of local iterations
     (0 selects the default number of iterations).

     - between 100 and 199: algorithm based on forward-backward propagation in
     expression trees, similar to the HC4 algorithm used in constraint
     programming. The algorithm value corresponds to the number of iterations
     plus 100 (the value 100 selects the default number of iterations).
   */

box_t* box_add_ray_array(ap_manager_t* man,
			 bool destructive,
			 box_t* a,
			 ap_generator0_array_t* array);
  /* Generalized time elapse operator */

/* ============================================================ */
/* III.2 Assignement and Substitutions */
/* ============================================================ */

box_t* box_assign_linexpr_array(ap_manager_t* man,
				bool destructive,
				box_t* a,
				ap_dim_t* tdim,
				ap_linexpr0_t** texpr,
				size_t size,
				box_t* dest);

box_t* box_substitute_linexpr_array(ap_manager_t* man,
				    bool destructive,
				    box_t* org,
				    ap_dim_t* tdim,
				    ap_linexpr0_t** texpr,
				    size_t size,
				    box_t* dest);
box_t*
box_assign_texpr_array(ap_manager_t* man,
		       bool destructive,
		       box_t* org,
		       ap_dim_t* tdim, ap_texpr0_t** texpr, size_t size,
		       box_t* dest);

box_t*
box_substitute_texpr_array(ap_manager_t* man,
			   bool destructive,
			   box_t* org,
			   ap_dim_t* tdim, ap_texpr0_t** texpr, size_t size,
			   box_t* dest);
  /* Parallel Assignement and Substitution of several dimensions by
     expressions in box value org.

     dest is an optional argument. If not NULL, semantically speaking,
     the result of the transformation is intersected with dest. This is
     useful for precise backward transformations in lattices like intervals or
     octagons. */

/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

box_t* box_forget_array(ap_manager_t* man,
			bool destructive,
			box_t* a, ap_dim_t* tdim, size_t size,
			bool project);

/* ============================================================ */
/* III.4 Change and permutation of dimensions */
/* ============================================================ */

box_t* box_add_dimensions(ap_manager_t* man,
			  bool destructive,
			  box_t* a,
			  ap_dimchange_t* dimchange,
			  bool project);
box_t* box_remove_dimensions(ap_manager_t* man,
			     bool destructive,
			     box_t* a,
			     ap_dimchange_t* dimchange);
/* Size of the permutation is supposed to be equal to
   the dimension of the box value */
box_t* box_permute_dimensions(ap_manager_t* man,
			      bool destructive,
			      box_t* a,
			      ap_dimperm_t* perm);

/* ============================================================ */
/* III.5 Expansion and folding of dimensions */
/* ============================================================ */


box_t* box_expand(ap_manager_t* man,
		  bool destructive,
		  box_t* a,
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


box_t* box_fold(ap_manager_t* man,
		bool destructive,
		box_t* a,
		ap_dim_t* tdim,
		size_t size);
  /* Fold the dimensions in the array tdim of size n>=1 and put the result
     in the first dimension in the array. The other dimensions of the array
     are then removed (using box_permute_remove_dimensions). */

/* ============================================================ */
/* III.6 Widening */
/* ============================================================ */

box_t* box_widening(ap_manager_t* man,
		    box_t* a1, box_t* a2);

/* ============================================================ */
/* III.7 Closure operation */
/* ============================================================ */

/* Returns the topological closure of a possibly opened box value */

box_t* box_closure(ap_manager_t* man, bool destructive, box_t* a);

#ifdef __cplusplus
}
#endif

#endif
