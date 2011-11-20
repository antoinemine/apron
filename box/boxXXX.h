/* ********************************************************************** */
/* boxXXX.h: abstract lattice of intervals */
/* ********************************************************************** */

#ifndef _BOX_XXX_H_
#define _BOX_XXX_H_

#include <stdio.h>
#include "ap_global0.h"
#include "ap_linexprXXX.h"
#include "box.h"

#ifdef __cplusplus
extern "C" {
#endif

struct boxXXX_t {
  ap_linexprXXX_t e;
  ap_dimension_t dim;
};
typedef struct boxXXX_t boxXXX_t;

ap_manager_t* boxXXX_manager_alloc(void);

/* ********************************************************************** */
/* I. General management */
/* ********************************************************************** */

/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

boxXXX_t* boxXXX_copy(ap_manager_t* man, boxXXX_t* a);
  /* Return a copy of a box value, on
     which destructive update does not affect the initial value. */

void boxXXX_free(ap_manager_t* man, boxXXX_t* a);
  /* Free all the memory used by the box value */

size_t boxXXX_size(ap_manager_t* man, boxXXX_t* a);
  /* Return the itv size of a box value (see ap_manager_t) */

/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */

void boxXXX_minimize(ap_manager_t* man, boxXXX_t* a);
  /* Minimize the size of the representation of a.
     This may result in a later recomputation of internal information.
  */

void boxXXX_canonicalize(ap_manager_t* man, boxXXX_t* a);
  /* Put the box value in canonical form. (not yet clear definition) */
int boxXXX_hash(ap_manager_t* man, boxXXX_t* a);
  /* Return an hash code */

void boxXXX_approximate(ap_manager_t* man, boxXXX_t* a, int algorithm);
  /* Perform some transformation on the box value, guided by the
     field algorithm.

     The transformation may lose information. */

bool boxXXX_is_minimal(ap_manager_t* man, boxXXX_t* a);
bool boxXXX_is_canonical(ap_manager_t* man, boxXXX_t* a);

/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */

void boxXXX_fprint(FILE* stream,
	       ap_manager_t* man,
	       boxXXX_t* a,
	       char** name_of_dim);
  /* Print the box value in a pretty way, using function
     name_of_dim to name dimensions */

void boxXXX_fprintdiff(FILE* stream,
		   ap_manager_t* man,
		   boxXXX_t* a1, boxXXX_t* a2,
		   char** name_of_dim);
/* Print the difference between a1 (old value) and a2 (new value),
     using function name_of_dim to name dimensions.
     The meaning of difference is library dependent. */

void boxXXX_fdump(FILE* stream, ap_manager_t* man, boxXXX_t* a);
  /* Dump the internal representation of a box value,
     for debugging purposes */

/* ============================================================ */
/* I.4 Serialization */
/* ============================================================ */

ap_membuf_t boxXXX_serialize_raw(ap_manager_t* man, boxXXX_t* a);
/* Allocate a memory bfer (with malloc), output the box value in raw
   binary format to it and return a pointer on the memory buffer and the size
   of bytes written.  It is the user responsability to free the memory
   afterwards (with free). */

boxXXX_t* boxXXX_deserialize_raw(ap_manager_t* man, void* ptr);
/* Return the box value read in raw binary format from the input stream
   and store in size the number of bytes read */
/* ********************************************************************** */
/* II. Constructor, accessors, tests and property extraction */
/* ********************************************************************** */

/* ============================================================ */
/* II.1 Basic constructors */
/* ============================================================ */

/* We assume that dimensions [0..dim.intd-1] correspond to integer variables,
   and dimensions [dim.intd..dim.intd+dim.reald-1] to real variables */

boxXXX_t* boxXXX_bottom(ap_manager_t* man, ap_dimension_t dim);
  /* Create a bottom (empty) value */

boxXXX_t* boxXXX_top(ap_manager_t* man, ap_dimension_t dim);
  /* Create a top (universe) value */

boxXXX_t* boxXXX_of_box(ap_manager_t* man,
			ap_dimension_t dim,
			ap_linexpr0_t box);
  /* Abstract an hypercube */

/* ============================================================ */
/* II.2 Accessors */
/* ============================================================ */

ap_dimension_t boxXXX_dimension(ap_manager_t* man, boxXXX_t* a);
  /* Return the dimensionality of the box values */

/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */

bool boxXXX_is_bottom(ap_manager_t* man, boxXXX_t* a);
bool boxXXX_is_top(ap_manager_t* man, boxXXX_t* a);

bool boxXXX_is_leq(ap_manager_t* man, boxXXX_t* a1, boxXXX_t* a2);
  /* inclusion check */

bool boxXXX_is_eq(ap_manager_t* man, boxXXX_t* a1, boxXXX_t* a2);
  /* equality check */

bool boxXXX_sat_lincons(ap_manager_t* man, boxXXX_t* a, ap_lincons0_t cons);
  /* does the box value satisfy the linear constraint ? */
bool boxXXX_sat_tcons(ap_manager_t* man, boxXXX_t* a, ap_tcons0_t* tcons);
  /* does the box value satisfy the tree expression constraint ? */

bool boxXXX_sat_interval(ap_manager_t* man,
		      boxXXX_t* a,
		      ap_dim_t dim, ap_coeff_t interval);
  /* is the dimension included in the interval in the box value ? */

bool boxXXX_is_dimension_unconstrained(ap_manager_t* man,
				       boxXXX_t* a, ap_dim_t dim);
  /* is the dimension unconstrained in the box value ?  If it
     is the case, we have forget(man,a,dim) == a */

/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */

void boxXXX_bound_dimension(ap_manager_t* man,
			    ap_coeff_t interval,
			    boxXXX_t* a, ap_dim_t dim);
  /* Returns the interval taken by a linear expression
     over the box value */

void boxXXX_bound_linexpr(ap_manager_t* man,
			  ap_coeff_t interval,
			  boxXXX_t* a, ap_linexpr0_t expr);
  /* Returns the interval taken by a linear expression
     over the box value */

void boxXXX_bound_texpr(ap_manager_t* man,
			ap_coeff_t interval,
			boxXXX_t* a, ap_texpr0_t* expr);
  /* Returns the interval taken by a tree expression
     over the box value */

void boxXXX_to_box(ap_manager_t* man, ap_linexpr0_t box, boxXXX_t* a);
  /* Converts a box value to an interval/hypercube.
     The size of the resulting array is boxXXX_dimension(man,a).  This
     function can be reimplemented by using boxXXX_bound_linexpr */

void boxXXX_to_lincons_array(ap_manager_t* man, ap_lincons0_array_t array, boxXXX_t* a);
  /* Converts a box value to a polyhedra
     (conjunction of linear constraints).
     The size of the returned array is stored in size. */

ap_tcons0_array_t boxXXX_to_tcons_array(ap_manager_t* man, boxXXX_t* a);
  /* Converts an box value to conjunction of tree expressions constraints.

     The constraints are normally guaranteed to be scalar (without intervals) */

void boxXXX_to_lingen_array(ap_manager_t* man, ap_lingen0_array_t array, boxXXX_t* a);
  /* Converts an box value to a system of generators. */

/* ********************************************************************** */
/* III. Operations: functional version */
/* ********************************************************************** */

/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */


boxXXX_t* boxXXX_meet(ap_manager_t* man, bool destructive, boxXXX_t* a1, boxXXX_t* a2);

boxXXX_t* boxXXX_join(ap_manager_t* man, bool destructive, boxXXX_t* a1, boxXXX_t* a2);
  /* Meet and Join of 2 box values */


boxXXX_t* boxXXX_meet_array(ap_manager_t* man, boxXXX_t** tab, size_t size);

boxXXX_t* boxXXX_join_array(ap_manager_t* man, boxXXX_t** tab, size_t size);
  /* Meet and Join of an array of box values.
     Raises an [[exc_invalid_argument]] exception if [[size==0]]
     (no way to define the dimensionality of the result in such a case */


boxXXX_t* boxXXX_meet_lincons_array(ap_manager_t* man,
			      bool destructive,
			      boxXXX_t* a,
			      ap_lincons0_array_t array);
  /* Meet of an box value with a set of constraints */

boxXXX_t*
boxXXX_meet_tcons_array(ap_manager_t* man,
		     bool destructive, boxXXX_t* a, ap_tcons0_array_t* array);
  /* Meet of an box value with a set of tree expressions constraints */

boxXXX_t* boxXXX_add_ray_array(ap_manager_t* man,
			       bool destructive,
			       boxXXX_t* a,
			       ap_lingen0_array_t array);
  /* Generalized time elapse operator */

/* ============================================================ */
/* III.2 Assignement and Substitutions */
/* ============================================================ */

boxXXX_t* boxXXX_assign_linexpr_array(ap_manager_t* man,
				bool destructive,
				boxXXX_t* a,
				ap_dim_t* tdim,
				ap_linexpr0_array_t texpr,
				boxXXX_t* dest);

boxXXX_t* boxXXX_substitute_linexpr_array(ap_manager_t* man,
				    bool destructive,
				    boxXXX_t* org,
				    ap_dim_t* tdim,
				    ap_linexpr0_array_t texpr,
				    boxXXX_t* dest);
boxXXX_t*
boxXXX_assign_texpr_array(ap_manager_t* man,
			  bool destructive,
			  boxXXX_t* org,
			  ap_dim_t* tdim, ap_texpr0_array_t* array,
			  boxXXX_t* dest);

boxXXX_t*
boxXXX_substitute_texpr_array(ap_manager_t* man,
			      bool destructive,
			      boxXXX_t* org,
			      ap_dim_t* tdim, ap_texpr0_array_t* array,
			      boxXXX_t* dest);
  /* Parallel Assignement and Substitution of several dimensions by
     expressions in box value org.

     dest is an optional argument. If not NULL, semantically speaking,
     the result of the transformation is intersected with dest. This is
     useful for precise backward transformations in lattices like intervals or
     octagons. */

/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

boxXXX_t* boxXXX_forget_array(ap_manager_t* man,
			      bool destructive,
			      boxXXX_t* a, ap_dim_t* tdim, size_t size,
			      bool project);

/* ============================================================ */
/* III.4 Change and permutation of dimensions */
/* ============================================================ */

boxXXX_t* boxXXX_add_dimensions(ap_manager_t* man,
			  bool destructive,
			  boxXXX_t* a,
			  ap_dimchange_t* dimchange,
			  bool project);
boxXXX_t* boxXXX_remove_dimensions(ap_manager_t* man,
			     bool destructive,
			     boxXXX_t* a,
			     ap_dimchange_t* dimchange);
/* Size of the permutation is supposed to be equal to
   the dimension of the box value */
boxXXX_t* boxXXX_permute_dimensions(ap_manager_t* man,
			      bool destructive,
			      boxXXX_t* a,
			      ap_dimperm_t* perm);

/* ============================================================ */
/* III.5 Expansion and folding of dimensions */
/* ============================================================ */


boxXXX_t* boxXXX_expand(ap_manager_t* man,
		  bool destructive,
		  boxXXX_t* a,
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


boxXXX_t* boxXXX_fold(ap_manager_t* man,
		bool destructive,
		boxXXX_t* a,
		ap_dim_t* tdim,
		size_t size);
  /* Fold the dimensions in the array tdim of size n>=1 and put the result
     in the first dimension in the array. The other dimensions of the array
     are then removed (using boxXXX_permute_remove_dimensions). */

/* ============================================================ */
/* III.6 Widening */
/* ============================================================ */

boxXXX_t* boxXXX_widening(ap_manager_t* man,
		    boxXXX_t* a1, boxXXX_t* a2);

/* ============================================================ */
/* III.7 Closure operation */
/* ============================================================ */

/* Returns the topological closure of a possibly opened box value */

boxXXX_t* boxXXX_closure(ap_manager_t* man, bool destructive, boxXXX_t* a);

#ifdef __cplusplus
}
#endif

#endif
