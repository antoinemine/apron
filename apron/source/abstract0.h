#line 10 "/udd/bjeannet/dev/commoninterface/abstract0.nw"
/* ************************************************************************* */
/* abstract0.h: generic operations on numerical abstract values */
/* ************************************************************************* */

/* GENERATED FROM abstract0.nw: DOT NOT MODIFY ! */


#ifndef _ABSTRACT0_H_
#define _ABSTRACT0_H_

#include "manager.h"
#include "expr0.h"

/* Generic abstract value at level 0 */
typedef struct abstract0_t {
  void* value;    /* Abstract value of the underlying library */
  manager_t* man; /* Used to identify the effective type of value */
} abstract0_t;

/* Dimensionality of an abstract value */
typedef struct dimension_t {
  size_t intdim;
  size_t realdim;
} dimension_t;


#line 44 "/udd/bjeannet/dev/commoninterface/abstract0.nw"
/* ********************************************************************** */
/* I. General management */
/* ********************************************************************** */

#line 57 "/udd/bjeannet/dev/commoninterface/abstract0.nw"
/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

abstract0_t* abstract0_copy(manager_t* man, const abstract0_t* a);
  /* Return a copy of an abstract value, on
     which destructive update does not affect the initial value. */

void abstract0_free(manager_t* man, abstract0_t* a);
  /* Free all the memory used by the abstract value */

size_t abstract0_size(manager_t* man, const abstract0_t* a);
  /* Return the abstract size of an abstract value (see manager_t) */


#line 78 "/udd/bjeannet/dev/commoninterface/abstract0.nw"
/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */


void abstract0_minimize(manager_t* man, const abstract0_t* a);
  /* Minimize the size of the representation of a.
     This may result in a later recomputation of internal information.
  */


void abstract0_canonicalize(manager_t* man, const abstract0_t* a);
  /* Put the abstract value in canonical form. (not yet clear definition) */


void abstract0_approximate(manager_t* man, abstract0_t* a, int algorithm);
  /* Perform some transformation on the abstract value, guided by the
     field algorithm.

     The transformation may lose information.  The argument "algorithm"
     overrides the field algorithm of the structure of type foption_t
     associated to abstract0_approximate (commodity feature). */


tbool_t abstract0_is_minimal(manager_t* man, const abstract0_t* a);

tbool_t abstract0_is_canonical(manager_t* man, const abstract0_t* a);

#line 117 "/udd/bjeannet/dev/commoninterface/abstract0.nw"
/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */


void abstract0_fprint(FILE* stream,
                    manager_t* man,
                    const abstract0_t* a,
                    const char** name_of_dim);
  /* Print the abstract value in a pretty way, using function
     name_of_dim to name dimensions */


void abstract0_fprintdiff(FILE* stream,
                        manager_t* man,
                        const abstract0_t* a1, const abstract0_t* a2,
                        const char** name_of_dim);
  /* Print the difference between a1 (old value) and a2 (new value),
     using function name_of_dim to name dimensions.
     The meaning of difference is library dependent. */


void abstract0_fdump(FILE* stream, manager_t* man, const abstract0_t* a);
  /* Dump the internal representation of an abstract value,
     for debugging purposes */

#line 172 "/udd/bjeannet/dev/commoninterface/abstract0.nw"
/* ============================================================ */
/* I.4 Serialization */
/* ============================================================ */


membuf_t abstract0_serialize_raw(manager_t* man, const abstract0_t* a);
/* Allocate a memory buffer (with malloc), output the abstract value in raw
   binary format to it and return a pointer on the memory buffer and the size
   of bytes written.  It is the user responsability to free the memory
   afterwards (with free). */


abstract0_t* abstract0_deserialize_raw(manager_t* man, void* ptr);
/* Return the abstract value read in raw binary format from the input stream
   and store in size the number of bytes read */

#line 200 "/udd/bjeannet/dev/commoninterface/abstract0.nw"
/* ********************************************************************** */
/* II. Constructor, accessors, tests and property extraction */
/* ********************************************************************** */

/* ============================================================ */
/* II.1 Basic constructors */
/* ============================================================ */

/* We assume that dimensions [0..intdim-1] correspond to integer variables, and
   dimensions [intdim..intdim+realdim-1] to real variables */

abstract0_t* abstract0_bottom(manager_t* man, size_t intdim, size_t realdim);
  /* Create a bottom (empty) value */


abstract0_t* abstract0_top(manager_t* man, size_t intdim, size_t realdim);
  /* Create a top (universe) value */

#line 224 "/udd/bjeannet/dev/commoninterface/abstract0.nw"
abstract0_t* abstract0_of_box(manager_t* man,
                              size_t intdim, size_t realdim,
                              const interval_t** tinterval);
  /* Abstract an hypercube defined by the array of intervals
     of size intdim+realdim */


abstract0_t* abstract0_of_lincons_array(manager_t* man,
                                        size_t intdim, size_t realdim,
                                        const lincons0_array_t* array);
  /* Abstract a convex polyhedra defined by the array of (interval) linear
     constraints of size size */

#line 252 "/udd/bjeannet/dev/commoninterface/abstract0.nw"
/* ============================================================ */
/* II.2 Accessors */
/* ============================================================ */

dimension_t abstract0_dimension(manager_t* man, const abstract0_t* a);
  /* Return the dimensionality of the abstract values */


#line 267 "/udd/bjeannet/dev/commoninterface/abstract0.nw"
/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */

/* If any of the following functions returns tbool_top, this means that
   an exception has occured, or that the exact computation was
   considered too expensive to be performed (according to the options).
   The flag exact and best should be cleared in such a case. */


tbool_t abstract0_is_bottom(manager_t* man, const abstract0_t* a);

tbool_t abstract0_is_top(manager_t* man, const abstract0_t* a);


tbool_t abstract0_is_leq(manager_t* man, const abstract0_t* a1, const abstract0_t* a2);
  /* inclusion check */


tbool_t abstract0_is_eq(manager_t* man, const abstract0_t* a1, const abstract0_t* a2);
  /* equality check */


tbool_t abstract0_sat_lincons(manager_t* man, const abstract0_t* a, const lincons0_t* lincons);
  /* does the abstract value satisfy the (interval) linear constraint ? */


tbool_t abstract0_sat_interval(manager_t* man, const abstract0_t* a,
                              dim_t dim, const interval_t* interval);
  /* is the dimension included in the interval in the abstract value ? */


tbool_t abstract0_is_dimension_unconstrained(manager_t* man, const abstract0_t*
                                             a, dim_t dim); /* is the dimension
                                             unconstrained in the abstract
                                             value ?  If it is the case, we
                                             have forget(man,a,dim) == a */

#line 337 "/udd/bjeannet/dev/commoninterface/abstract0.nw"
/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */


interval_t* abstract0_bound_linexpr(manager_t* man,
                                    const abstract0_t* a, const linexpr0_t* expr);
  /* Returns the interval taken by a linear expression
     over the abstract value */


interval_t* abstract0_bound_dimension(manager_t* man,
                                      const abstract0_t* a, dim_t dim);
  /* Returns the interval taken by the dimension
     over the abstract value */


lincons0_array_t abstract0_to_lincons_array(manager_t* man, const abstract0_t* a);
  /* Converts an abstract value to a polyhedra
     (conjunction of linear constraints).

     The constraints are normally guaranteed to be really linear (without intervals) */


interval_t** abstract0_to_box(manager_t* man, const abstract0_t* a);
  /* Converts an abstract value to an interval/hypercube.
     The size of the resulting array is abstract0_dimension(man,a).  This
     function can be reimplemented by using abstract0_bound_linexpr */


generator0_array_t abstract0_to_generator_array(manager_t* man, const abstract0_t* a);
  /* Converts an abstract value to a system of generators. */


#line 391 "/udd/bjeannet/dev/commoninterface/abstract0.nw"
/* ********************************************************************** */
/* III. Operations: functional version */
/* ********************************************************************** */

/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */


abstract0_t* abstract0_meet(manager_t* man, bool destructive, abstract0_t* a1, const abstract0_t* a2);

abstract0_t* abstract0_join(manager_t* man, bool destructive, abstract0_t* a1, const abstract0_t* a2);
  /* Meet and Join of 2 abstract values */


abstract0_t* abstract0_meet_array(manager_t* man, const abstract0_t** tab, size_t size);

abstract0_t* abstract0_join_array(manager_t* man, const abstract0_t** tab, size_t size);
  /* Meet and Join of an array of abstract values.
     Raises an [[exc_invalid_argument]] exception if [[size==0]]
     (no way to define the dimensionality of the result in such a case */


abstract0_t* abstract0_meet_lincons_array(manager_t* man,
                                          bool destructive,
                                          abstract0_t* a,
                                          const lincons0_array_t* array);
  /* Meet of an abstract value with a set of constraints */


abstract0_t* abstract0_add_ray_array(manager_t* man,
                                     bool destructive,
                                     abstract0_t* a,
                                     const generator0_array_t* array);
  /* Generalized time elapse operator */

#line 453 "/udd/bjeannet/dev/commoninterface/abstract0.nw"
/* ============================================================ */
/* III.2 Assignement and Substitutions */
/* ============================================================ */


abstract0_t* abstract0_assign_linexpr(manager_t* man,
                                      bool destructive,
                                      abstract0_t* org,
                                      dim_t dim, const linexpr0_t* expr,
                                      const abstract0_t* dest);

abstract0_t* abstract0_substitute_linexpr(manager_t* man,
                                          bool destructive,
                                          abstract0_t* org,
                                          dim_t dim, const linexpr0_t* expr,
                                          const abstract0_t* dest);
  /* Assignement and Substitution of a single dimension by a (interval)
     linear expression in abstract value org. 
     
     dest is an optional argument. If not NULL, semantically speaking,
     the result of the transformation is intersected with dest. This is
     useful for precise backward transformations in lattices like intervals or 
     octagons.
 */


abstract0_t* abstract0_assign_linexpr_array(manager_t* man,
                                            bool destructive,
                                            abstract0_t* org,
                                            const dim_t* tdim,
                                            const linexpr0_t** texpr,
                                            size_t size,
                                            const abstract0_t* dest);

abstract0_t* abstract0_substitute_linexpr_array(manager_t* man,
                                                bool destructive,
                                                abstract0_t* org,
                                                const dim_t* tdim,
                                                const linexpr0_t** texpr,
                                                size_t size,
                                                const abstract0_t* dest);
  /* Parallel Assignement and Substitution of several dimensions by
     linear expressions in abstract value org. 
     
     dest is an optional argument. If not NULL, semantically speaking,
     the result of the transformation is intersected with dest. This is
     useful for precise backward transformations in lattices like intervals or 
     octagons. */

#line 508 "/udd/bjeannet/dev/commoninterface/abstract0.nw"
/* ============================================================ */
/* III.3 Projections */
/* ============================================================ */

abstract0_t* abstract0_forget_array(manager_t* man,
                                    bool destructive,
                                    abstract0_t* a, const dim_t* tdim, size_t size,
                                    bool project);

#line 546 "/udd/bjeannet/dev/commoninterface/abstract0.nw"
/* ============================================================ */
/* III.4 Change and permutation of dimensions */
/* ============================================================ */

abstract0_t* abstract0_add_dimensions(manager_t* man,
                                      bool destructive,
                                      abstract0_t* a,
                                      dimchange_t* dimchange);
abstract0_t* abstract0_remove_dimensions(manager_t* man,
                                         bool destructive,
                                         abstract0_t* a,
                                         dimchange_t* dimchange);
abstract0_t* abstract0_permute_dimensions(manager_t* man,
                                          bool destructive,
                                          abstract0_t* a,
                                          const dim_t* permutation);

#line 584 "/udd/bjeannet/dev/commoninterface/abstract0.nw"
/* ============================================================ */
/* III.5 Expansion and folding of dimensions */
/* ============================================================ */


abstract0_t* abstract0_expand(manager_t* man,
                              bool destructive,
                              abstract0_t* a,
                              dim_t dim,
                              size_t n);
  /* Expand the dimension dim into itself + n additional dimensions.
     It results in (n+1) unrelated dimensions having same
     relations with other dimensions. The (n+1) dimensions are put as follows:

     - original dimension dim

     - if the dimension is integer, the n additional dimensions are put at the
       end of integer dimensions; if it is real, at the end of the real
       dimensions.
  */


abstract0_t* abstract0_fold(manager_t* man,
                            bool destructive,
                            abstract0_t* a,
                            const dim_t* tdim,
                            size_t size);
  /* Fold the dimensions in the array tdim of size n>=1 and put the result
     in the first dimension in the array. The other dimensions of the array
     are then removed (using abstract0_permute_remove_dimensions). */

#line 621 "/udd/bjeannet/dev/commoninterface/abstract0.nw"
/* ============================================================ */
/* III.6 Widening */
/* ============================================================ */

abstract0_t* abstract0_widening(manager_t* man,
                                const abstract0_t* a1, const abstract0_t* a2);

#line 639 "/udd/bjeannet/dev/commoninterface/abstract0.nw"
/* ============================================================ */
/* III.7 Closure operation */
/* ============================================================ */

/* Returns the topological closure of a possibly opened abstract value */


abstract0_t* abstract0_closure(manager_t* man, bool destructive, abstract0_t* a);
#line 658 "/udd/bjeannet/dev/commoninterface/abstract0.nw"
/* ********************************************************************** */
/* ********************************************************************** */
/* Additional functions */
/* ********************************************************************** */
/* ********************************************************************** */
static inline 
manager_t* abstract0_manager(const abstract0_t* a)
  { return a->man; }
  /* Return a reference to the manager contained in the abstract value.
     The reference should not be freed */

/* Widening with threshold */

abstract0_t* abstract0_widening(manager_t* man,
                                const abstract0_t* a1, const abstract0_t* a2,
                                const lincons0_array_t* array);

linexpr0_t* abstract0_quasilinear_of_intervallinear(manager_t* man,
                                                    const abstract0_t* a,
                                                    linexpr0_t* expr);
  /* Evaluate a interval linear expression on the abstract 
     value such as to transform it into a quasilinear expression. 

     This implies calls to abstract0_bound_dimension.

     NOT YET IMPLEMENTED
  */
#line 690 "/udd/bjeannet/dev/commoninterface/abstract0.nw"
#endif
