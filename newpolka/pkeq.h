/* ********************************************************************** */
/* pkeq.h: Interface of the polka linear equalities library  */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.
   
   Please read the COPYING file packaged in the distribution */

#ifndef __PKEQ_H__
#define __PKEQ_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "ap_global0.h"
#include "pk.h"

typedef pk_t pkeq_t;

/* 

  Important remark: the newpolka library is normally intended to be accessed
  through the APRON interface, i.e., through abstract0_XX and abstract1_XX
  functions. If it is accessed directly with poly_XXX functions, many checks on
  arguments will not be performed.

*/


/* ============================================================ */
/* A. Constructor for APRON manager (to be freed with ap_manager_free). */
/* ============================================================ */

ap_manager_t* pkeq_manager_alloc(void);
pk_internal_t* pkeq_manager_get_internal(ap_manager_t*);

/* ============================================================ */
/* D. Conversions */
/* ============================================================ */

pkeq_t* pkeq_of_abstract0(ap_abstract0_t* abstract);
  /* Extract from an abstract value the underlying NewPolka polyhedron.  There
     is no copy, so only one of the two objects should be freed. */

ap_abstract0_t* pkeq_to_abstract0(ap_manager_t* man, pkeq_t* poly);
  /* Create an abstract value from the manager and the underlying NewPolka
     polyhedron. There is no copy, and only the result should be freed
  */

/* ********************************************************************** */
/* I. General management */
/* ********************************************************************** */


/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

pkeq_t* pkeq_copy(ap_manager_t* man, pkeq_t* a);
  /* Return a copy of an abstract value, on
     which destructive update does not affect the initial value. */

void pkeq_free(ap_manager_t* man, pkeq_t* a);
  /* Free all the memory used by the abstract value */

size_t pkeq_size(ap_manager_t* man, pkeq_t* a);
  /* Return the abstract size of an abstract value (see ap_manager_t) */


/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */

void pkeq_minimize(ap_manager_t* man, pkeq_t* a);
  /* Minimize the size of the representation of a.
     This may result in a later recomputation of internal information.
  */

void pkeq_canonicalize(ap_manager_t* man, pkeq_t* a);
  /* Put the abstract value in canonical form. (not yet clear definition) */

void pkeq_hash(ap_manager_t* man, pkeq_t* a);
  /* Return an hash code */

void pkeq_approximate(ap_manager_t* man, pkeq_t* a, int algorithm);
  /* Perform some transformation on the abstract value, guided by the
     field algorithm.

     The transformation may lose information. */

/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */

void pkeq_fprint(FILE* stream,
		 ap_manager_t* man,
		 pkeq_t* a,
		 char** name_of_dim);
  /* Print the abstract value in a pretty way, using function
     name_of_dim to name dimensions */

void pkeq_fprintdiff(FILE* stream,
		     ap_manager_t* man,
		     pkeq_t* a1, pkeq_t* a2,
		     char** name_of_dim);
  /* Print the difference between a1 (old value) and a2 (new value),
     using function name_of_dim to name dimensions.
     The meaning of difference is library dependent. */

void pkeq_fdump(FILE* stream, ap_manager_t* man, pkeq_t* a);
  /* Dump the internal representation of an abstract value,
     for debugging purposes */


/* ============================================================ */
/* I.4 Serialization */
/* ============================================================ */

ap_membuf_t pkeq_serialize_raw(ap_manager_t* man, pkeq_t* a);
/* Allocate a memory buffer (with malloc), output the abstract value in raw
   binary format to it and return a pointer on the memory buffer and the size
   of bytes written.  It is the user responsability to free the memory
   afterwards (with free). */

pkeq_t* pkeq_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size);
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

pkeq_t* pkeq_bottom(ap_manager_t* man, size_t intdim, size_t realdim);
  /* Create a bottom (empty) value */

pkeq_t* pkeq_top(ap_manager_t* man, size_t intdim, size_t realdim);
  /* Create a top (universe) value */


pkeq_t* pkeq_of_box(ap_manager_t* man,
		    size_t intdim, size_t realdim,
		    ap_interval_t** tinterval);
  /* Abstract an hypercube defined by the array of intervals
     of size intdim+realdim */

pkeq_t* pkeq_of_lincons_array(ap_manager_t* man,
			      size_t intdim, size_t realdim,
			      ap_lincons0_array_t* array);
  /* Abstract a convex polyhedra defined by the array of linear constraints
     of size size */


/* ============================================================ */
/* II.2 Accessors */
/* ============================================================ */

ap_dimension_t pkeq_dimension(ap_manager_t* man, pkeq_t* a);
/* Return the total number of dimensions of the abstract values */

/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */

bool pkeq_is_bottom(ap_manager_t* man, pkeq_t* a);
bool pkeq_is_top(ap_manager_t* man, pkeq_t* a);

bool pkeq_is_leq(ap_manager_t* man, pkeq_t* a1, pkeq_t* a2);
  /* inclusion check */

bool pkeq_is_eq(ap_manager_t* man, pkeq_t* a1, pkeq_t* a2);
  /* equality check */

bool pkeq_sat_lincons(ap_manager_t* man, pkeq_t* a, ap_lincons0_t* lincons);
  /* does the abstract value satisfy the linear constraint ? */

bool pkeq_sat_interval(ap_manager_t* man, pkeq_t* a,
		       ap_dim_t dim, ap_interval_t* interval);
  /* is the dimension included in the interval in the abstract value ? */

bool pkeq_is_dimension_unconstrained(ap_manager_t* man, pkeq_t* po,
				     ap_dim_t dim);
  /* is the dimension unconstrained ? */

/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */

ap_interval_t* pkeq_bound_linexpr(ap_manager_t* man,
				      pkeq_t* a, ap_linexpr0_t* expr);
  /* Returns the interval taken by a linear expression
     over the abstract value */

ap_interval_t* pkeq_bound_dimension(ap_manager_t* man,
				    pkeq_t* a, ap_dim_t dim);
  /* Returns the interval taken by the dimension
     over the abstract value */

ap_lincons0_array_t pkeq_to_lincons_array(ap_manager_t* man, pkeq_t* a);
  /* Converts an abstract value to a polyhedra
     (conjunction of linear constraints). */

ap_interval_t** pkeq_to_box(ap_manager_t* man, pkeq_t* a);
  /* Converts an abstract value to an interval/hypercube.
     The size of the resulting array is pkeq_dimension(man,a).  This
     function can be reimplemented by using pkeq_bound_linexpr */

ap_generator0_array_t pkeq_to_generator_array(ap_manager_t* man, pkeq_t* a);
  /* Converts an abstract value to a system of generators. */


/* ********************************************************************** */
/* III. Operations */
/* ********************************************************************** */

/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */

pkeq_t* pkeq_meet(ap_manager_t* man, bool destructive, pkeq_t* a1, pkeq_t* a2);
pkeq_t* pkeq_join(ap_manager_t* man, bool destructive, pkeq_t* a1, pkeq_t* a2);
  /* Meet and Join of 2 abstract values */

pkeq_t* pkeq_meet_array(ap_manager_t* man, pkeq_t** tab, size_t size);
pkeq_t* pkeq_join_array(ap_manager_t* man, pkeq_t** tab, size_t size);
  /* Meet and Join of an array of abstract values.
     Raises an [[exc_invalid_argument]] exception if [[size==0]]
     (no way to define the dimensionality of the result in such a case */

pkeq_t* pkeq_meet_lincons_array(ap_manager_t* man,
				bool destructive, pkeq_t* a,
				ap_lincons0_array_t* array);
  /* Meet of an abstract value with a set of constraints
     (generalize pkeq_of_lincons_array) */

pkeq_t* pkeq_add_ray_array(ap_manager_t* man,
			   bool destructive, pkeq_t* a,
			   ap_generator0_array_t* array);
  /* Generalized time elapse operator */

/* ============================================================ */
/* III.2 Assignement and Substitutions */
/* ============================================================ */

pkeq_t* pkeq_assign_linexpr(ap_manager_t* man,
			    bool destructive, pkeq_t* a,
			    ap_dim_t dim, ap_linexpr0_t* expr,
			    pkeq_t* dest);
pkeq_t* pkeq_substitute_linexpr(ap_manager_t* man,
				bool destructive, pkeq_t* a,
				ap_dim_t dim, ap_linexpr0_t* expr,
				pkeq_t* dest);
  /* Assignement and Substitution of a single dimension by resp.
     a linear expression and a interval linear expression */

pkeq_t* pkeq_assign_linexpr_array(ap_manager_t* man,
				  bool destructive, pkeq_t* a,
				  ap_dim_t* tdim,
				  ap_linexpr0_t** texpr,
				  size_t size,
				  pkeq_t* dest);
pkeq_t* pkeq_substitute_linexpr_array(ap_manager_t* man,
				      bool destructive, pkeq_t* a,
				      ap_dim_t* tdim,
				      ap_linexpr0_t** texpr,
				      size_t size,
				      pkeq_t* dest);
  /* Parallel Assignement and Substitution of several dimensions by
     linear expressons. */

/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

pkeq_t* pkeq_forget_array(ap_manager_t* man,
			  bool destructive, pkeq_t* a,
			  ap_dim_t* tdim, size_t size,
			  bool project);

/* ============================================================ */
/* III.4 Change and permutation of dimensions */
/* ============================================================ */

pkeq_t* pkeq_add_dimensions(ap_manager_t* man,
			    bool destructive, pkeq_t* a,
			    ap_dimchange_t* dimchange,
			    bool project);

pkeq_t* pkeq_remove_dimensions(ap_manager_t* man,
			       bool destructive, pkeq_t* a,
			       ap_dimchange_t* dimchange);
pkeq_t* pkeq_permute_dimensions(ap_manager_t* man,
				bool destructive,
				pkeq_t* a,
				ap_dimperm_t* permutation);

/* ============================================================ */
/* III.5 Expansion and folding of dimensions */
/* ============================================================ */

pkeq_t* pkeq_expand(ap_manager_t* man,
		    bool destructive, pkeq_t* a,
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

pkeq_t* pkeq_fold(ap_manager_t* man,
		  bool destructive, pkeq_t* a,
		  ap_dim_t* tdim,
		  size_t size);
  /* Fold the dimensions in the array tdim of size n>=1 and put the result
     in the first dimension in the array. The other dimensions of the array
     are then removed (using pkeq_permute_remove_dimensions). */

/* ============================================================ */
/* III.6 Widening */
/* ============================================================ */

/* Widening */
pkeq_t* pkeq_widening(ap_manager_t* man,
		      pkeq_t* a1, pkeq_t* a2);

/* ============================================================ */
/* III.7 Closure operation */
/* ============================================================ */

/* Returns the topological closure of a possibly opened abstract value */

pkeq_t* pkeq_closure(ap_manager_t* man, bool destructive, pkeq_t* a);

#ifdef __cplusplus
}
#endif

#endif
