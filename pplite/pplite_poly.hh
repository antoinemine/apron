/*
 * pplite_poly.hh
 *
 * APRON Library / PPLite library wrapper
 *
 * Wrappers specific to the PPLite domains.
 *
 * Copyright (C) Antoine Mine' 2006
 * Copyright (C) 2018-2024 Enea Zaffanella <enea.zaffanella@unipr.it>
 *
 */

/*
 * This file is part of the APRON Library, released under GPL license.
 * Please read the COPYING file packaged in the distribution.
 */

#ifndef __PPLITE_POLY_H
#define __PPLITE_POLY_H

#include "ap_pplite.h"
#include "pplite/pplite.hh"

namespace pplite {
namespace apron {

class pplite_poly {
public:
  using poly_type = dynamic::Abs_Poly;

  std::unique_ptr<poly_type> p;
  size_t intdim;

  pplite_poly(ap_manager_t* man,
              size_t intdim, size_t realdim,
              Spec_Elem kind = Spec_Elem::UNIVERSE);
  pplite_poly(ap_manager_t*, const pplite_poly* x_ptr)
    : pplite_poly(*x_ptr) { }

  // Use default move ctor, move assignment and dtor.
  pplite_poly(pplite_poly&& x) = default;
  pplite_poly& operator=(pplite_poly&& x) = default;
  ~pplite_poly() = default;

  // Have to specify ad-hoc copy ctor and assignment.
  pplite_poly(const pplite_poly& x) : p(nullptr), intdim(0) {
    p.reset(x.p->clone());
    intdim = x.intdim;
  }
  pplite_poly& operator=(const pplite_poly& x) {
    // First copy, then move assign.
    auto tmp = x;
    *this = std::move(tmp);
    return *this;
  }
}; // class pplite_poly


/* Allocates a pplite manager for convex polyhedra

   If the Boolean parameter is true, abstract values generated with the
   manager are NNC polyhedra (i.e., can have strict constraints such as x > 0).
   Otherwise they are topologically closed (C) polyhedra.
   Managers and abstract values in NNC and C mode are incompatible;
   different polyhedra kinds are incompatible too
   (see ap_pplite_manager_set_kind).
*/
extern "C" ap_manager_t*
ap_pplite_manager_alloc(bool strict);

/* Returns a deep copy of an abstract value */
extern "C" pplite_poly*
ap_pplite_poly_copy(ap_manager_t* man, pplite_poly* a);

/* Frees the memory used by the abstract value */
extern "C" void
ap_pplite_poly_free(ap_manager_t* man, pplite_poly* a);

/* Returns the abstract size of a polyhedron */
extern "C" size_t
ap_pplite_poly_size(ap_manager_t* man, pplite_poly* a);

/* Same as ap_pplite_poly_canonicalize */
extern "C" void
ap_pplite_poly_minimize(ap_manager_t* man, pplite_poly* a);

/* Puts the polyhedron with minimized constraints and generators */
extern "C" void
ap_pplite_poly_canonicalize(ap_manager_t* man, pplite_poly* a);

/* Computes hash */
extern "C" int
ap_pplite_poly_hash(ap_manager_t* man, pplite_poly* a);

/* Does nothing */
extern "C" void
ap_pplite_poly_approximate(ap_manager_t* man, pplite_poly* a,
                           int algorithm);

/* Prints the abstract value in a pretty way, using function
   name_of_dim to name dimensions */
extern "C" void
ap_pplite_poly_fprint(FILE* stream,
                      ap_manager_t* man,
                      pplite_poly* a,
                      char** name_of_dim);

/* Prints the difference between a1 (old value) and a2 (new value),
   using function name_of_dim to name dimensions.
   The meaning of difference is library dependent. */
extern "C" void
ap_pplite_poly_fprintdiff(FILE* stream,
                          ap_manager_t* man,
                          pplite_poly* a1, pplite_poly* a2,
                          char** name_of_dim);

/* Dumps the internal representation of an abstract value,
   for debugging purposes */
extern "C" void
ap_pplite_poly_fdump(FILE* stream, ap_manager_t* man, pplite_poly* a);

/* Allocates a memory buffer (with malloc), output the abstract value in raw
   binary format to it and return a pointer on the memory buffer and the size
   of bytes written.  It is the user responsability to free the memory
   afterwards (with free).

   Not implemented */
extern "C" ap_membuf_t
ap_pplite_poly_serialize_raw(ap_manager_t* man, pplite_poly* a);

/* Returns the abstract value read in raw binary format from the input stream
   and store in size the number of bytes read.

   Not implemented */
extern "C" pplite_poly*
ap_pplite_poly_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size);

/* Create a bottom (empty) value.
   We assume that dimensions [0..intdim-1] correspond to integer variables,
   and dimensions [intdim..intdim+realdim-1] to real variables */
extern "C" pplite_poly*
ap_pplite_poly_bottom(ap_manager_t* man, size_t intdim, size_t realdim);

/* Create a top (universe) value */
extern "C" pplite_poly*
ap_pplite_poly_top(ap_manager_t* man, size_t intdim, size_t realdim);

/* Abstracts an hypercube defined by the array of intervals
   of size intdim+realdim */
extern "C" pplite_poly*
ap_pplite_poly_of_box(ap_manager_t* man,
                      size_t intdim, size_t realdim,
                      ap_interval_t** tinterval);

/* Return the total number of dimensions of the abstract values */
extern "C" ap_dimension_t
ap_pplite_poly_dimension(ap_manager_t* man, pplite_poly* a);

/* Emptiness test */
extern "C" bool
ap_pplite_poly_is_bottom(ap_manager_t* man, pplite_poly* a);

/* Universe test */
extern "C" bool
ap_pplite_poly_is_top(ap_manager_t* man, pplite_poly* a);

/* Inclusion test */
extern "C" bool
ap_pplite_poly_is_leq(ap_manager_t* man, pplite_poly* a1, pplite_poly* a2);

/* Equality test */
extern "C" bool
ap_pplite_poly_is_eq(ap_manager_t* man, pplite_poly* a1, pplite_poly* a2);

/* Satisfiability of a linear constraint */
extern "C" bool
ap_pplite_poly_sat_lincons(ap_manager_t* man, pplite_poly* a,
                           ap_lincons0_t* lincons);

/* Satisfiability of a tree expression constraint */
extern "C" bool
ap_pplite_poly_sat_tcons(ap_manager_t* man, pplite_poly* a,
                         ap_tcons0_t* cons);

/* Inclusion of a dimension in an interval */
extern "C" bool
ap_pplite_poly_sat_interval(ap_manager_t* man, pplite_poly* a,
                            ap_dim_t dim, ap_interval_t* interval);

/* Tests if a dimension is unconstrained */
extern "C" bool
ap_pplite_poly_is_dimension_unconstrained(ap_manager_t* man,
                                          pplite_poly* a,
                                          ap_dim_t dim);

/* Returns the interval taken by the dimension
   over the abstract value */
extern "C" ap_interval_t*
ap_pplite_poly_bound_dimension(ap_manager_t* man,
                               pplite_poly* a, ap_dim_t dim);

/* Returns the interval taken by a linear expression
   over the abstract value */
extern "C" ap_interval_t*
ap_pplite_poly_bound_linexpr(ap_manager_t* man, pplite_poly* a,
                             ap_linexpr0_t* expr);

/* Returns the interval taken by a tree expression
   over the abstract value */
extern "C" ap_interval_t*
ap_pplite_poly_bound_texpr(ap_manager_t* man, pplite_poly* a,
                           ap_texpr0_t* expr);

/* Converts an abstract value to an array of linear constraints */
extern "C" ap_lincons0_array_t
ap_pplite_poly_to_lincons_array(ap_manager_t* man, pplite_poly* a);

/* Converts an abstract value to an array of tree expression constraints */
extern "C" ap_tcons0_array_t
ap_pplite_poly_to_tcons_array(ap_manager_t* man, pplite_poly* a);

/* Converts an abstract value to an array of intervals */
extern "C" ap_interval_t**
ap_pplite_poly_to_box(ap_manager_t* man, pplite_poly* a);

/* Converts an abstract value to an array of generators */
extern "C" ap_generator0_array_t
ap_pplite_poly_to_generator_array(ap_manager_t* man, pplite_poly* a);

/* Meet of two abstract values */
extern "C" pplite_poly*
ap_pplite_poly_meet(ap_manager_t* man, bool destructive,
                    pplite_poly* a1, pplite_poly* a2);
/* Join of two abstract values */
extern "C" pplite_poly*
ap_pplite_poly_join(ap_manager_t* man, bool destructive,
                    pplite_poly* a1, pplite_poly* a2);

/* Meet of a non-empty array of abstract values */
extern "C" pplite_poly*
ap_pplite_poly_meet_array(ap_manager_t* man, pplite_poly** tab, size_t size);

/* Join of a non-empty array of abstract values */
extern "C" pplite_poly*
ap_pplite_poly_join_array(ap_manager_t* man, pplite_poly** tab, size_t size);

/* Meet of an abstract value with an array of linear constraints */
extern "C" pplite_poly*
ap_pplite_poly_meet_lincons_array(ap_manager_t* man,
                                  bool destructive, pplite_poly* a,
                                  ap_lincons0_array_t* array);

/* Meet of an abstract value with an array of tree expression constraints. */
extern "C" pplite_poly*
ap_pplite_poly_meet_tcons_array(ap_manager_t* man,
                                bool destructive, pplite_poly* a,
                                ap_tcons0_array_t* array);

/* Generalized time elapse operator */
extern "C" pplite_poly*
ap_pplite_poly_add_ray_array(ap_manager_t* man,
                             bool destructive, pplite_poly* a,
                             ap_generator0_array_t* array);

/* Parallel assignment (forward image) by linear expressions */
extern "C" pplite_poly*
ap_pplite_poly_assign_linexpr_array(ap_manager_t* man,
                                    bool destructive, pplite_poly* a,
                                    ap_dim_t* tdim,
                                    ap_linexpr0_t** texpr,
                                    size_t size,
                                    pplite_poly* dest);

/* Parallel substitution (backward image) by linear expressions */
extern "C" pplite_poly*
ap_pplite_poly_substitute_linexpr_array(ap_manager_t* man,
                                        bool destructive, pplite_poly* a,
                                        ap_dim_t* tdim,
                                        ap_linexpr0_t** texpr,
                                        size_t size,
                                        pplite_poly* dest);

/* Parallel assignment (forward image) by tree expressions */
extern "C" pplite_poly*
ap_pplite_poly_assign_texpr_array(ap_manager_t* man,
                                  bool destructive, pplite_poly* a,
                                  ap_dim_t* tdim,
                                  ap_texpr0_t** texpr,
                                  size_t size,
                                  pplite_poly* dest);

/* Parallel substitution (backward image) by term expressions */
extern "C" pplite_poly*
ap_pplite_poly_substitute_texpr_array(ap_manager_t* man,
                                      bool destructive, pplite_poly* a,
                                      ap_dim_t* tdim,
                                      ap_texpr0_t** texpr,
                                      size_t size,
                                      pplite_poly* dest);

/* Forgetting (existentially quantifying) a set of dimenions,
   then assigning them to zero if project is true */
extern "C" pplite_poly*
ap_pplite_poly_forget_array(ap_manager_t* man,
                            bool destructive, pplite_poly* a,
                            ap_dim_t* tdim, size_t size,
                            bool project);

/* Adding (integer and real) unconstrained dimensions,
   them assigning them to zero if project is true */
extern "C" pplite_poly*
ap_pplite_poly_add_dimensions(ap_manager_t* man,
                              bool destructive, pplite_poly* a,
                              ap_dimchange_t* dimchange,
                              bool project);

/* Removing dimensions */
extern "C" pplite_poly*
ap_pplite_poly_remove_dimensions(ap_manager_t* man,
                                 bool destructive, pplite_poly* a,
                                 ap_dimchange_t* dimchange);

/* Permuting dimensions */
extern "C" pplite_poly*
ap_pplite_poly_permute_dimensions(ap_manager_t* man,
                                  bool destructive,
                                  pplite_poly* a,
                                  ap_dimperm_t* permutation);

/* Expands dimension dim into itself + n additional dimensions

   It results in (n+1) unrelated dimensions having same
   relations with other dimensions. The (n+1) dimensions are put as follows:
     - original dimension dim
     - if the dimension is integer, the n additional dimensions are put at the
       end of integer dimensions; if it is real, at the end of the real
       dimensions.
*/
extern "C" pplite_poly*
ap_pplite_poly_expand(ap_manager_t* man,
                      bool destructive, pplite_poly* a,
                      ap_dim_t dim, size_t n);

/* Folds the dimensions in the array tdim of size n>=1 and puts the result
   in the first dimension in the array. The other dimensions of the array
   are then removed. */
extern "C" pplite_poly*
ap_pplite_poly_fold(ap_manager_t* man,
                    bool destructive, pplite_poly* a,
                    ap_dim_t* tdim, size_t size);

/* Widening */
extern "C" pplite_poly*
ap_pplite_poly_widening(ap_manager_t* man, pplite_poly* a1, pplite_poly* a2);

/* Computes topological closure of abstract value */
extern "C" pplite_poly*
ap_pplite_poly_closure(ap_manager_t* man, bool destructive, pplite_poly* a);

} // namespace apron
} // namespace pplite

#endif // !defined(__PPLITE_POLY_H)
