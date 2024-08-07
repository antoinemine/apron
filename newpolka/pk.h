/* ********************************************************************** */
/* pk.h: Interface of the polka linear relation library  */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.
   
   Please read the COPYING file packaged in the distribution */

#ifndef __PK_H__
#define __PK_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "ap_global0.h"

/* The invariant of the representation of a polyhedron is the following: if the
   polyhedron is empty, then C==F==satC==satF==0. Otherwise, we have
   (C || F) && (satC || satF || !(C && F)).
   This means that a non-empty polyhedron has a minimal representation minimal
   if and only if C && F if and only if satC || satF. */

typedef enum pk_status_t {
  pk_status_conseps=0x1,
  pk_status_consgauss=0x2,
  pk_status_gengauss=0x4,
  pk_status_minimaleps=0x8
} pk_status_t;

struct pk_t {
  /* private data: do not use directly ! */
  struct pk_matrix_t* C;
  struct pk_matrix_t* F;
  struct satmat_t* satC;
  struct satmat_t* satF;
  size_t intdim;
  size_t realdim;
  size_t nbeq;
  size_t nbline;
  pk_status_t status;
};

typedef struct pk_t pk_t;
typedef struct pk_internal_t pk_internal_t;

/*

  Important remark: the newpolka library is normally intended to be accessed
  through the APRON interface, i.e., through abstract0_XX and abstract1_XX
  functions. If it is accessed directly with pk_XXX functions, many checks on
  arguments will not be performed.

*/


/* ============================================================ */
/* A. Constructor for APRON manager (to be freed with ap_manager_free). */
/* ============================================================ */

ap_manager_t* pk_manager_alloc(bool strict);
  /* Allocate a NewPolka manager for convex polyhedra.

     If the Boolean parameter is true, abstract values generated with the
     manager can have strict constraints (like x>0). Otherwise they are defined
     using only loose constraints. Managers and abstract values in strict or
     loose mode are incompatible.
  */

/* ============================================================ */
/* B. Options */
/* ============================================================ */

pk_internal_t* pk_manager_get_internal(ap_manager_t* man);

/* For setting options when one has a ap_manager_t object, one can use the
   APRON function ap_manager_get_internal with a cast. */

void pk_set_max_coeff_size(pk_internal_t* pk, size_t size);
void pk_set_approximate_max_coeff_size(pk_internal_t* pk, size_t size);
size_t pk_get_max_coeff_size(pk_internal_t* pk);
size_t pk_get_approximate_max_coeff_size(pk_internal_t* pk);
void pk_print(ap_manager_t* man, pk_t* po, char** name_of_dim);

/* ============================================================ */
/* D. Conversions */
/* ============================================================ */

pk_t* pk_of_abstract0(ap_abstract0_t* abstract);
  /* Extract from an abstract value the underlying NewPolka polyhedron.  There
     is no copy, and only the argument should be freed. */

ap_abstract0_t* pk_to_abstract0(ap_manager_t* man, pk_t* poly);
  /* Create an abstract value from the manager and the underlying NewPolka
     polyhedron. There is no copy, and only the result should be freed
  */

/* ============================================================ */
/* D. Constructor and destructor for internal manager */
/* ============================================================ */

/* Allocates pk and initializes it with a default size */
struct pk_internal_t* pk_internal_alloc(bool strict);
/* Clear and free pk */
void pk_internal_free(pk_internal_t* pk);

/* ********************************************************************** */
/* I. General management */
/* ********************************************************************** */


/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

pk_t* pk_copy(ap_manager_t* man, pk_t* a);
  /* Return a copy of an abstract value, on
     which destructive update does not affect the initial value. */

void pk_free(ap_manager_t* man, pk_t* a);
  /* Free all the memory used by the abstract value */

size_t pk_size(ap_manager_t* man, pk_t* a);
  /* Return the abstract size of a polyhedron, which is the number of
     coefficients of its current representation, possibly redundant. */


/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */

void pk_minimize(ap_manager_t* man, pk_t* a);
  /* Minimize the size of the representation of a.
     This may result in a later recomputation of internal information.
  */

void pk_canonicalize(ap_manager_t* man, pk_t* a);
  /* Put the polyhedron with minimized constraints and frames.  If in addition
     the integer man->option->canonicalize.algorithm is strictly positive,
     normalize equalities and lines, and also strict constraints */

int pk_hash(ap_manager_t* man, pk_t* a);
  /* Return an hash value for the abstract value.  Two abstract values in
     canonical from (according to @code{ap_abstract1_canonicalize}) and
     considered as equal by the function ap_abstract0_is_eq are given the
     same hash value (this implies more or less a canonical form).
  */

void pk_approximate(ap_manager_t* man, pk_t* a, int algorithm);
  /* Perform some transformation on the abstract value, guided by the
     field algorithm.

     Approximation:

     - algorithm==0: do nothing

     - algorithm==-1: normalize integer minimal constraints (induces a smaller
		      polyhedron)

     - algorithm==1: remove constraints with coefficients of size greater than
		     max_coeff_size, if max_coeff_size > 0
     - algorithm==2: in addition, keep same bounding box (more precise)
     - algorithm==3: in addition, keep same bounding octagon (even more
		     precise)

     - algorithm==10: round constraints with too big coefficients,
		      of size greater than
		      approximate_max_coeff_size, if
		      approximate_max_coeff_size>0.  Rounding is
		      done by truncation towards zero.
     - algorithm==11: same, but if the constraint truncated
                      towards zero is removed, try with the
                      constraint truncated towards -oo or +oo.
     - algorithm==12: consider both inner and outer truncation
                      (may increase the number of constraint
*/

/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */

void pk_fprint(FILE* stream,
	       ap_manager_t* man,
	       pk_t* a,
	       char** name_of_dim);
  /* Print the abstract value in a pretty way, using function
     name_of_dim to name dimensions */

void pk_fprintdiff(FILE* stream,
		   ap_manager_t* man,
		   pk_t* a1, pk_t* a2,
		   char** name_of_dim);
  /* Print the difference between a1 (old value) and a2 (new value),
     using function name_of_dim to name dimensions.
     The meaning of difference is library dependent.

     Not implemented */

void pk_fdump(FILE* stream, ap_manager_t* man, pk_t* a);
  /* Dump the internal representation of an abstract value,
     for debugging purposes */


/* ============================================================ */
/* I.4 Serialization */
/* ============================================================ */

ap_membuf_t pk_serialize_raw(ap_manager_t* man, pk_t* a);
/* Allocate a memory buffer (with malloc), output the abstract value in raw
   binary format to it and return a pointer on the memory buffer and the size
   of bytes written.  It is the user responsability to free the memory
   afterwards (with free).
   Not implemented */

pk_t* pk_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size);
/* Return the abstract value read in raw binary format from the input stream
   and store in size the number of bytes read
   Not implemented */

/* ********************************************************************** */
/* II. Constructor, accessors, tests and property extraction */
/* ********************************************************************** */

/* ============================================================ */
/* II.1 Basic constructors */
/* ============================================================ */

/* We assume that dimensions [0..intdim-1] correspond to integer variables, and
   dimensions [intdim..intdim+realdim-1] to real variables */

pk_t* pk_bottom(ap_manager_t* man, size_t intdim, size_t realdim);
  /* Create a bottom (empty) value */

pk_t* pk_top(ap_manager_t* man, size_t intdim, size_t realdim);
  /* Create a top (universe) value */


pk_t* pk_of_box(ap_manager_t* man,
		size_t intdim, size_t realdim,
		ap_interval_t** tinterval);
  /* Abstract an hypercube defined by the array of intervals
     of size intdim+realdim */

/* ============================================================ */
/* II.2 Accessors */
/* ============================================================ */

ap_dimension_t pk_dimension(ap_manager_t* man, pk_t* a);
/* Return the total number of dimensions of the abstract values */

/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */

bool pk_is_bottom(ap_manager_t* man, pk_t* a);
  /* Emptiness test
     algorithm >= 0: strict behaviour, compute canonical form if necessary
     algorithm < 0: lazy behaviour, always cheap
  */
bool pk_is_top(ap_manager_t* man, pk_t* a);
  /* Universe test
     algorithm >= 0: strict behaviour, compute canonical form if necessary
     algorithm < 0: lazy behaviour, always cheap
  */

bool pk_is_leq(ap_manager_t* man, pk_t* a1, pk_t* a2);
  /* Inclusion test:
     Is always strict
     algorithm > 0: (nearly always) compute canonical forms
     algorithm <= 0: compute dual representations only if necessary
  */

bool pk_is_eq(ap_manager_t* man, pk_t* a1, pk_t* a2);
  /* Equality test:
     Is always strict
     Use algorithm field of is_leq.
  */

bool pk_sat_lincons(ap_manager_t* man, pk_t* a, ap_lincons0_t* lincons);
  /* Satisfiability of a linear constraint
     Is always strict
     algorithm > 0: (nearly always) compute canonical form
     algorithm <= 0: compute dual representation only if necessary
  */

bool pk_sat_tcons(ap_manager_t* man, pk_t* a, ap_tcons0_t* cons);
  /* Satisfiability of a tree expression constraint. */

bool pk_sat_interval(ap_manager_t* man, pk_t* a,
		     ap_dim_t dim, ap_interval_t* interval);
  /* Inclusion of a dimension in an interval
     Is always strict
     algorithm > 0: (nearly always) compute canonical form
     algorithm <= 0: compute dual representation only if necessary
  */

bool pk_is_dimension_unconstrained(ap_manager_t* man, pk_t* po,
				   ap_dim_t dim);
  /* Is a dimension unconstrained ?
     Is always strict
     algorithm > 0: compute canonical form
     algorithm <= 0: compute dual representation only if necessary
  */

/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */

ap_interval_t* pk_bound_linexpr(ap_manager_t* man,
				pk_t* a, ap_linexpr0_t* expr);
  /* Returns the interval taken by a linear expression
     over the abstract value.

     algorithm > 0: compute canonical form
     algorithm <= 0: compute dual representation only if necessary
  */

ap_interval_t* pk_bound_texpr(ap_manager_t* man,
			      pk_t* a, ap_texpr0_t* expr);
  /* Returns the interval taken by a tree expression
     over the abstract value. */

ap_interval_t* pk_bound_dimension(ap_manager_t* man,
				  pk_t* a, ap_dim_t dim);
  /* Returns the interval taken by the dimension
     over the abstract value

     algorithm > 0: compute canonical form
     algorithm <= 0: compute dual representation only if necessary
  */

ap_lincons0_array_t pk_to_lincons_array(ap_manager_t* man, pk_t* a);
  /* Converts an abstract value to a polyhedra
     (conjunction of linear constraints).

     Always consider canonical form */

ap_tcons0_array_t pk_to_tcons_array(ap_manager_t* man, pk_t* a);
  /* Converts an abstract value to a
     conjunction of tree expressions constraints. */

ap_interval_t** pk_to_box(ap_manager_t* man, pk_t* a);
  /* Converts an abstract value to an interval/hypercube.
     The size of the resulting array is pk_dimension(man,a).  This
     function can be reimplemented by using pk_bound_linexpr

     algorithm >= 0: compute canonical form
     algorithm < 0: compute dual representation only if necessary
  */

ap_generator0_array_t pk_to_generator_array(ap_manager_t* man, pk_t* a);
  /* Converts an abstract value to a system of generators.
     Always consider canonical form. */


/* ********************************************************************** */
/* III. Operations */
/* ********************************************************************** */

/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */

pk_t* pk_meet(ap_manager_t* man, bool destructive, pk_t* a1, pk_t* a2);
pk_t* pk_join(ap_manager_t* man, bool destructive, pk_t* a1, pk_t* a2);
  /* Meet and Join of 2 abstract values */

pk_t* pk_meet_array(ap_manager_t* man, pk_t** tab, size_t size);
pk_t* pk_join_array(ap_manager_t* man, pk_t** tab, size_t size);
  /* Meet and Join of a non empty array of abstract values. */

void* pk_meet_lincons_array(ap_manager_t* man,
			    bool destructive, void* a,
			    ap_lincons0_array_t* array);
pk_t* pk_meet_tcons_array(ap_manager_t* man,
			  bool destructive, pk_t* a,
			  ap_tcons0_array_t* array);
  /* Meet of an abstract value with a set of constraints. */

pk_t* pk_add_ray_array(ap_manager_t* man,
		       bool destructive, pk_t* a,
		       ap_generator0_array_t* array);
  /* Generalized time elapse operator */

/* ============================================================ */
/* III.2 Assignement and Substitutions */
/* ============================================================ */

pk_t* pk_assign_linexpr_array(ap_manager_t* man,
			      bool destructive, pk_t* a,
			      ap_dim_t* tdim,
			      ap_linexpr0_t** texpr,
			      size_t size,
			      pk_t* dest);
pk_t* pk_substitute_linexpr_array(ap_manager_t* man,
				  bool destructive, pk_t* a,
				  ap_dim_t* tdim,
				  ap_linexpr0_t** texpr,
				  size_t size,
				  pk_t* dest);
pk_t* pk_assign_texpr_array(ap_manager_t* man,
			    bool destructive, pk_t* a,
			    ap_dim_t* tdim,
			    ap_texpr0_t** texpr,
			    size_t size,
			    pk_t* dest);
pk_t* pk_substitute_texpr_array(ap_manager_t* man,
				bool destructive, pk_t* a,
				ap_dim_t* tdim,
				ap_texpr0_t** texpr,
				size_t size,
				pk_t* dest);
  /* Parallel Assignement and Substitution of several dimensions by interval
     expressons. */

/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

pk_t* pk_forget_array(ap_manager_t* man,
		      bool destructive, pk_t* a,
		      ap_dim_t* tdim, size_t size,
		      bool project);

/* ============================================================ */
/* III.4 Change and permutation of dimensions */
/* ============================================================ */

pk_t* pk_add_dimensions(ap_manager_t* man,
			bool destructive, pk_t* a,
			ap_dimchange_t* dimchange,
			bool project);

pk_t* pk_remove_dimensions(ap_manager_t* man,
			   bool destructive, pk_t* a,
			   ap_dimchange_t* dimchange);
pk_t* pk_permute_dimensions(ap_manager_t* man,
			    bool destructive,
			    pk_t* a,
			    ap_dimperm_t* permutation);

/* ============================================================ */
/* III.5 Expansion and folding of dimensions */
/* ============================================================ */

pk_t* pk_expand(ap_manager_t* man,
		bool destructive, pk_t* a,
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

pk_t* pk_fold(ap_manager_t* man,
	      bool destructive, pk_t* a,
	      ap_dim_t* tdim,
	      size_t size);
  /* Fold the dimensions in the array tdim of size n>=1 and put the result
     in the first dimension in the array. The other dimensions of the array
     are then removed (using pk_permute_remove_dimensions). */

/* ============================================================ */
/* III.6 Widening */
/* ============================================================ */

/* Widening */

pk_t* pk_widening(ap_manager_t* man, pk_t* a1, pk_t* a2);

/* ============================================================ */
/* III.7 Closure operation */
/* ============================================================ */

/* Returns the topological closure of a possibly opened abstract value */

pk_t* pk_closure(ap_manager_t* man, bool destructive, pk_t* a);

#ifdef __cplusplus
}
#endif

#endif
