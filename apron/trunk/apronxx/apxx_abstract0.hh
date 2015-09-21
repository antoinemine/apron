/* -*- C++ -*-
 * apxx_abstract0.hh
 *
 * APRON Library / C++ class wrappers
 *
 * Copyright (C) Antoine Mine' 2007
 *
 */
/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution.
*/

#ifndef __APXX_ABSTRACT0_HH
#define __APXX_ABSTRACT0_HH

#include <string>
#include "ap_abstract0.h"
#include "apxx_linexpr0.hh"
#include "apxx_lincons0.hh"
#include "apxx_generator0.hh"
#include "apxx_manager.hh"


namespace apron {


/* ================================= */
/* abstract0                         */
/* ================================= */

/*! \brief Level 0 abstract value (ap_abstract0_t* wrapper).
 *
 * An abstract0 object stores an abstract value.
 * It represents a set of points in a numerical space with some integer-valued dimensions and some
 * real-valued dimensions.
 *
 * \warning all the abstract operators are guaranteed to be sound, but not complete, that is:
 * - returned abstract elements and intervals are over-approximations of the actual result on sets,
 * - returned constraint conjunction may satisfy more points than the original abstract element,
 * - returned generators may span more points than the original abstract element,
 * - predicates return \c true if the predicate is definitively true,
 * and \c false if either the predicate is false (flag_exact is then true), or
 * due to abstraction, the domain cannot conclude (flag_exact is then false).
 *
 * An abstract0 is always created with a manager that indicates the underlying library. This
 * defines its internal representation and algorithms, which in turns defines the expressiveness and
 * cost vs. precision trade-off.
 * Most operations to manipulate an abstract0 also take a manager as first argument.
 * This must be a manager compatible with the one of all abstract0 arguments (including this),
 * that is, a manger created by the same library (e.g., NewPolka polyhedra) and using the same
 * parameter values, if any (e.g., strictness).
 * It need not be the very same manager the abstract0 was created with.
 *
 * Overloaded arithmetic, assignment and copy operators that cannot take an extra manager
 * argument will implicitly use the manager used to create the first argument.
 *
 * Additionally, for binary or n-aray operators, all abstract0 must have the same number of
 * integer and real dimensions.
 *
 * Many operations exist in two kinds:
 * - an imperative class function that modifies the this abstract0,
 * - a more "functional" global function that takes as argument both one or several constant
 * source abstract0 and one destination abstract0 that will be overridden with the result.
 *
 * Both kinds will return a reference to the abstract0 that holds the result.
 *
 * Most functions can throw a variety of exceptions:
 * - std::invalid_argument, when arguments have incompatible managers, types, or dimensions,
 * - std::length_error, when running out of memory or exceeding the \c max_object_size
 * value set by the user in the manager,
 * - std::overflow_error, when a numerical overflow occurs,
 * - not_implemented, when some function is not available,
 * - timeout, when exceeding the \c timout value set by the user in the manager.
 */
class abstract0 : public use_malloc {

protected:

  ap_abstract0_t* a; //!< Pointer managed by APRON.

  //! Internal use only. Wraps an abstract0 around the pointer x, taking ownership of the object.
  abstract0(ap_abstract0_t* x);

  //! NULL abstract element, to be used only as default argument in assign and substitute.
  static const abstract0 null;

  friend class manager;

public:


  /* constructors */
  /* ============ */

  /** @name Constructors */
  //@{

  //! Creates an abstract element representing the whole space.
  abstract0(manager &m, size_t intdim, size_t realdim, top x);

  //! Creates an abstract element representing the empty set.
  abstract0(manager &m, size_t intdim, size_t realdim, bottom x);

  //! Creates a (deep) copy of the abstract element.
  abstract0(manager &m, const abstract0& t);

  /*! \brief Creates an abstract element from a box.
   *
   * \throw std::invalid_argument if x has less than intdim+realdim dimensions.
   */
  abstract0(manager &m, size_t intdim, size_t realdim, const interval_array& x);

  //! Creates an abstract element from a conjunction of linear constraints.
  abstract0(manager &m, size_t intdim, size_t realdim, const lincons0_array& x);

  //! Creates an abstract element from a conjunction of arbitrary constraints.
  abstract0(manager &m, size_t intdim, size_t realdim, const tcons0_array& x);

  /*! \brief Creates a (deep) copy of the abstract element.
   *
   * Implicitly uses the manager used to create *this.
   */
  abstract0(const abstract0& t);

  //@}


  /* destructor */
  /* ========== */

  /** @name Destructors */
  //@{

  /*! \brief Destroys the abstract element.
   *
   * Implicitly uses the manager used to create *this.
   */
  ~abstract0();

  /*! \brief Destroys the abstract element using the given manager.
   *
   * The abstract element cannot be used after being freed.
   * However, the standard destructor can be safely be called (resulting in a no-op).
   */
  void free(manager& m);

  //@}


  /* assignments */
  /* =========== */

  /** @name Copies and conversions to abstract elements */
  //@{

  /*! \brief Assigns a copy of t to *this.
   *
   * Implicitly uses the manager used to create *this.
   */
  abstract0& operator=(const abstract0& t);

  /*! \brief Assigns the full space to *this.
   *
   * Implicitly uses the manager used to create *this.
   */
  abstract0& operator=(top t);

  /*! \brief Assigns the empty set to *this.
   *
   * Implicitly uses the manager used to create *this.
   */
  abstract0& operator=(bottom t);

  /*! \brief Assigns a box to *this.
   *
   * Implicitly uses the manager used to create *this.
   *
   * \throw std::invalid_argument if the array has insufficient size.
   */
  abstract0& operator=(const interval_array& x);

  /*! \brief Assigns a conjunction of linear constraints to *this.
   *
   * Implicitly uses the manager used to create *this.
   */
  abstract0& operator=(const lincons0_array& x);

  /*! \brief Assigns a conjunction of arbitrary constraints to *this.
   *
   * Implicitly uses the manager used to create *this.
   */
  abstract0& operator=(const tcons0_array& x);

  /*! \brief Replaces *this with a copy of x.
   *
   * \return a reference to *this.
   */
  abstract0& set(manager& m, const abstract0& x);

  /*! \brief Replaces *this with the full space.
   *
   * You can either specify new intdim and realdim values or keep those of *this
   * (if unspecified, i.e., left to \c AP_DIM_MAX).
   *
   * \return a reference to *this.
   */
  abstract0& set(manager& m, top t, size_t intdim = AP_DIM_MAX, size_t realdim = AP_DIM_MAX);

  /*! \brief Replaces *this with the empty set.
   *
   * You can either specify new intdim and realdim values or keep those of *this
   * (if unspecified, i.e., left to \c AP_DIM_MAX).
   *
   * \return a reference to *this.
   */
  abstract0& set(manager& m, bottom t, size_t intdim = AP_DIM_MAX, size_t realdim = AP_DIM_MAX);

  /*! \brief Replaces *this with a box.
   *
   * You can either specify new intdim and realdim values or keep those of *this
   * (if unspecified, i.e., left to \c AP_DIM_MAX).
   *
   * \return a reference to *this.
   */
  abstract0& set(manager& m, const interval_array& x, size_t intdim = AP_DIM_MAX, size_t realdim = AP_DIM_MAX);

  /*! \brief Replaces *this with a conjunction of linear constraints.
   *
   * You can either specify new intdim and realdim values or keep those of *this
   * (if unspecified, i.e., left to \c AP_DIM_MAX).
   *
   * \return a reference to *this.
   */
  abstract0& set(manager& m, const lincons0_array& x, size_t intdim = AP_DIM_MAX, size_t realdim = AP_DIM_MAX);

  /*! \brief Replaces *this with a conjunction of arbitrary constraints.
   *
   * You can either specify new intdim and realdim values or keep those of *this
   * (if unspecified, i.e., left to \c AP_DIM_MAX).
   *
   * \return a reference to *this.
   */
  abstract0& set(manager& m, const tcons0_array& x, size_t intdim = AP_DIM_MAX, size_t realdim = AP_DIM_MAX);


  //@}


  /* representation */
  /* ============== */

  /** @name Control of internal representation */
  //@{

  /*! \brief Minimizes the size of the representation, to save memory.
   *
   * \return a reference to *this.
   */
  abstract0& minimize(manager& m);

  /*! \brief Puts the abstract element in canonical form (if such a notion exists).
   *
   * \return a reference to *this.
   */
  abstract0& canonicalize(manager& m);

  /*! \brief Simplifies the abstract element, potentially loosing precision.
   *
   * \return a reference to *this.
   */
  abstract0& approximate(manager& m, int algorithm);

  //@}


  /* printing */
  /* ======== */

  /** @name Printing */
  //@{

  //! Pretty-printing to a C stream.
  void print(manager& m, char** name_of_dim=NULL, FILE* stream=stdout) const;

  //! Pretty-printing the difference between x and y to a C stream.
  friend void printdiff(manager& m, const abstract0& x, const abstract0& y, char** name_of_dim, FILE* stream);

  //! Raw printing to a C stream (mainly for debug purposes).
  void dump(manager& m, FILE* stream=stdout) const;

  //! Prints in constraint form.
  friend std::ostream& operator<< (std::ostream& os, const abstract0& s);

  //@}


  /* serialisation */
  /* ============= */

  /** @name Serialisation */
  //@{

  /*! \brief Serializes an abstract element.
   *
   * The string can be safely stored to disk and reloaded later or transmitted across a network.
   * The format is library-specific but is generally a machine-readable byte-stream.
   *
   * \return a newly allocated string that the caller should delete it after use.
   */
  std::string* serialize(manager& m) const;

  /*! \brief Reconstruct an abstract element form a serialized byte-stream and put it into dst.
   *
   * The managers used to serialized and deserialize need not be the same, but they must
   * have been created using the same library and with the same arguments.
   *
   * \arg \c eaten, if not NULL, will be set to the actual number of bytes consumed from the
   * string.
   * \return a reference to dst.
   */
  friend abstract0& deserialize(manager& m, abstract0& dst, const std::string& s, size_t* eaten);

  //@}


  /* access */
  /* ====== */

  /** @name Access */
  //@{

  //! Returns the manager the abstract element was created with (with reference count incremented).
  manager get_manager() const;

  //! \brief Returns the number of integer and real dimensions in the abstract element.
  ap_dimension_t get_dimension(manager& m) const;

  /*! \brief Returns the (abstract) size of the abstract element.
   *
   * The unit in which size is computed is library-specific.
   * It is guaranteed to be the same as the unit for the \c max_object_size field of
   * the ap_funopt_t structure.
   */
  size_t size(manager& m) const;

  //@}


  /* predicates */
  /* ========== */

  /** @name Predicates */
  //@{

  //! \brief Whether *this represents the empty set.
  bool is_bottom(manager& m) const;

  //! Whether *this represents the full space.
  bool is_top(manager& m) const;

  //! Whether *this and x represent the same set.
  bool is_eq(manager& m, const abstract0& x) const;

  //! Whether *this is included in x (set-wise).
  bool is_leq(manager& m, const abstract0& x) const;

  //! Whether all points in *this satisfy a linear constraint.
  bool sat(manager& m, const lincons0& l) const;

  //! Whether all points in *this satisfy an arbitrary constraint.
  bool sat(manager& m, const tcons0& l) const;

  /*! \brief Whether the dimension dim of all points in *this is included
   * in the given interval.
   */
  bool sat(manager& m, ap_dim_t dim, const interval& i) const;

  //! \brief Whether the points in *this are unbounded in the given dimension.
  bool is_dimension_unconstrained(manager& m, ap_dim_t dim) const;

  /*! \brief Whether x and y represent the same set.
   *
   * The manager for the left argument is used implicitly.
   */
  friend bool operator== (const abstract0& x, const abstract0& y);

  /*! \brief Whether x and y represent different sets.
   *
   * The manager for the left argument is used implicitly.
   */
  friend bool operator!= (const abstract0& x, const abstract0& y);

  /*! \brief Whether x is included within y (set-wise).
   *
   * The manager for the left argument is used implicitly.
   */
  friend bool operator<= (const abstract0& x, const abstract0& y);

  /*! \brief Whether x contains y (set-wise).
   *
   * The manager for the left argument is used implicitly.
   */
  friend bool operator>= (const abstract0& x, const abstract0& y);

  /*! \brief Whether x strictly contains y (set-wise).
   *
   * The manager for the left argument is used implicitly.
   */
  friend bool operator> (const abstract0& x, const abstract0& y);

  /*! \brief Whether x is strictly included within y (set-wise).
   *
   * The manager for the left argument is used implicitly.
   */
  friend bool operator< (const abstract0& x, const abstract0& y);



  /* Properties */
  /* ========== */

  /** @name Property extraction */
  //@{

  //! Returns some bounds for a linear expression evaluated in all points in the abstract element.
  interval bound(manager& m, const linexpr0& l) const;

  //! Returns some bounds for an arbitrary expression evaluated in all points in the abstract element.
  interval bound(manager& m, const texpr0& l) const;

  //! Returns some bounds for the given coordinate on all points in the abstract element.
  interval bound(manager& m, ap_dim_t d) const;

  //! Returns a bounding box for the set represented by the abstract element.
  interval_array to_box(manager& m) const;

  /*! \brief Returns a generator representation of (an over-approximation of) the
   * set represented by the abstract element.
   */
  generator0_array to_generator_array(manager& m) const;

  /*! \brief Returns a linear constraint representation of (an over-approximation of) the
   * set represented by the abstract element.
   */
  lincons0_array to_lincons_array(manager& m) const;

  /*! \brief Returns a constraint representation of (an over-approximation of) the
   * set represented by the abstract element.
   */
  tcons0_array to_tcons_array(manager& m) const;

  //@}


  /* Meet */
  /* ==== */

  /** @name Meet */
  //@{

  /*! \brief Replaces *this with the meet of *this and the abstract element y.
   *
   * \return a reference to *this.
   */
  abstract0& meet(manager& m, const abstract0& y);

  /*! \brief Replaces dst with the meet of x and y.
   *
   * \return a reference to dst.
   */
  friend abstract0& meet(manager& m, abstract0& dst, const abstract0& x, const abstract0& y);

  /*! \brief Replaces dst with the meet of all abstract elements in x.
   *
   * \return a reference to dst.
   */
  friend abstract0& meet(manager& m, abstract0& dst, const std::vector<const abstract0*>& x);

  /*! \brief Replaces dst with the meet of all size abstract elements in x.
   *
   * \return a reference to dst.
   */
  friend abstract0& meet(manager& m, abstract0& dst, size_t size, const abstract0 * const x[]);

  /*! \brief Adds some linear constraints to *this (modified in-place).
   *
   * \return a reference to *this.
   */
  abstract0& meet(manager& m, const lincons0_array& y);

  /*! \brief Replaces dst with the meet of x and some linear constraints.
   *
   * \return a reference to dst.
   */
  friend abstract0& meet(manager& m, abstract0& dst, const abstract0& x, const lincons0_array& y);

  /*! \brief Adds some arbitrary constraints to *this (modified in-place).
   *
   * \return a reference to *this.
   */
  abstract0& meet(manager& m, const tcons0_array& y);

  /*! \brief Replaces dst with the meet of x and some arbitrary constraints.
   *
   * \return a reference to dst.
   */
  friend abstract0& meet(manager& m, abstract0& dst, const abstract0& x, const tcons0_array& y);


  /*! \brief Replaces *this with the meet of *this and the abstract element y.
   *
   * Implicitly uses the manager used to create *this.
   *
   * \return a reference to *this.
   */
  abstract0& operator*=(const abstract0& y);


  /*! \brief Adds some linear constraints to *this (modified in-place).
   *
   * Implicitly uses the manager used to create *this.
   *
   * \return a reference to *this.
   */
  abstract0& operator*=(const lincons0_array& y);

  /*! \brief Adds some arbitrary constraints to *this (modified in-place).
   *
   * Implicitly uses the manager used to create *this.
   *
   * \return a reference to *this.
   */
  abstract0& operator*=(const tcons0_array& y);

  //@}




  /* Join */
  /* ==== */

  /** @name Join */
  //@{

  /*! \brief Replaces *this with the join of *this and the abstract element y.
   *
   * \return a reference to *this.
   */
  abstract0& join(manager& m, const abstract0& y);

  /*! \brief Replaces dst with the join of x and y.
   *
   * \return a reference to dst.
   */
  friend abstract0& join(manager& m, abstract0& dst, const abstract0& x, const abstract0& y);

  /*! \brief Replaces dst with the join of all abstract elements in x.
   *
   * \return a reference to dst.
   */
  friend abstract0& join(manager& m, abstract0& dst, const std::vector<const abstract0*>& x);

  /*! \brief Replaces dst with the join of all size abstract elements in x.
   *
   * \return a reference to dst.
   */
  friend abstract0& join(manager& m, abstract0& dst, size_t size, const abstract0 * const x[]);

  /*! \brief Adds some rays to *this (modified in-place).
   *
   * \arg \c y can only contain rays and lines, not vertexes.
   *
   * \return a reference to *this.
   */
  abstract0& add_rays(manager& m, const generator0_array& y);

  /*! \brief Replaces dst with x with some rays added.
   *
   * \arg \c y can only contain rays and lines, not vertexes.
   *
   * \return a reference to *this.
   */
  friend abstract0& add_rays(manager& m, abstract0& dst, const abstract0& x, const generator0_array& y);

  /*! \brief Replaces *this with the join of *this and the abstract element y.
   *
   * Implicitly uses the manager used to create *this.
   *
   * \return a reference to *this.
   */
  abstract0& operator+=(const abstract0& y);

  /*! \brief Adds some rays to *this (modified in-place).
   *
   * Implicitly uses the manager used to create *this.
   *
   * \arg \c y can only contain rays and lines, not vertexes.
   *
   * \return a reference to *this.
   */
  abstract0& operator+=(const generator0_array& y);

  //@}


  /* Assignments */
  /* =========== */

  /** @name Assignment */
  //@{

  /*! \brief In-place assignment of linear expression.
   *
   * *this is modified in-place to reflect the effect of assigning l to dimension dim.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   */
  abstract0& assign(manager& m, ap_dim_t dim, const linexpr0& l, const abstract0& inter = null);

  /*! \brief In-place parallel assignment of linear expressions.
   *
   * *this is modified in-place to reflect the effect of assigning l[i] to dimension dim[i], for
   * i from 0 to size-1.
   * Assignments are performed in parallel.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   */
  abstract0& assign(manager& m,  size_t size, const ap_dim_t dim[], const linexpr0 * const l[], const abstract0& inter = null);


  /*! \brief In-place parallel assignment of linear expressions.
   *
   * *this is modified in-place to reflect the effect of assigning l[i] to dimension dim[i].
   * Assignments are performed in parallel.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   *
   * \throw std::invalid_argument if the vectors have different size.
   */
  abstract0& assign(manager& m, const std::vector<ap_dim_t>& dim, const std::vector<const linexpr0*>& l, const abstract0& inter = null);

  /*! \brief Assignment of linear expression.
   *
   * dst is replaced with the effect of assigning l to dimension dim in src.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   */
  friend abstract0& assign(manager& m, abstract0& dst, const abstract0& src, ap_dim_t dim, const linexpr0& l, const abstract0& inter);

  /*! \brief Parallel assignment of linear expressions.
   *
   * dst is replaced with the effect of assigning l[i] to dimension dim[i] in src,
   * for i from 0 to size-1.
   * Assignments are performed in parallel.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   */
  friend abstract0& assign(manager& m, abstract0& dst, const abstract0& src,  size_t size, const ap_dim_t dim[], const linexpr0 * const l[], const abstract0& inter);

  /*! \brief Parallel assignment of linear expressions.
   *
   * dst is replaced with the effect of assigning l[i] to dimension dim[i] in src.
   * Assignments are performed in parallel.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   *
   * \throw std::invalid_argument if the vectors have different size.
   */
  friend abstract0& assign(manager& m, abstract0& dst, const abstract0& src, const std::vector<ap_dim_t>& dim, const std::vector<const linexpr0*>& l, const abstract0& inter);




  /*! \brief In-place assignment of arbitrary expression.
   *
   * *this is modified in-place to reflect the effect of assigning l to dimension dim.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   */
  abstract0& assign(manager& m, ap_dim_t dim, const texpr0& l, const abstract0& inter = null);

  /*! \brief In-place parallel assignment of arbitrary expressions.
   *
   * *this is modified in-place to reflect the effect of assigning l[i] to dimension dim[i], for
   * i from 0 to size-1.
   * Assignments are performed in parallel.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   */
  abstract0& assign(manager& m, size_t size, const ap_dim_t dim[], const texpr0 * const l[], const abstract0& inter = null);


  /*! \brief In-place parallel assignment of arbitrary expressions.
   *
   * *this is modified in-place to reflect the effect of assigning l[i] to dimension dim[i].
   * Assignments are performed in parallel.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   *
   * \throw std::invalid_argument if the vectors have different size.
   */
  abstract0& assign(manager& m, const std::vector<ap_dim_t>& dim, const std::vector<const texpr0*>& l, const abstract0& inter = null);

  /*! \brief Assignment of arbitrary expression.
   *
   * dst is replaced with the effect of assigning l to dimension dim in src.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   */
  friend abstract0& assign(manager& m, abstract0& dst, const abstract0& src, ap_dim_t dim, const texpr0& l, const abstract0& inter);

  /*! \brief Parallel assignment of arbitrary expressions.
   *
   * dst is replaced with the effect of assigning l[i] to dimension dim[i] in src,
   * for i from 0 to size-1.
   * Assignments are performed in parallel.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   */
  friend abstract0& assign(manager& m, abstract0& dst, const abstract0& src, size_t size, const ap_dim_t dim[], const texpr0 * const l[], const abstract0& inter);

  /*! \brief Parallel assignment of arbitrary expressions.
   *
   * dst is replaced with the effect of assigning l[i] to dimension dim[i] in src.
   * Assignments are performed in parallel.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   *
   * \throw std::invalid_argument if the vectors have different size.
   */
  friend abstract0& assign(manager& m, abstract0& dst, const abstract0& src, const std::vector<ap_dim_t>& dim, const std::vector<const texpr0*>& l, const abstract0& inter);

  //@}


  /* Substitutions */
  /* ============== */

  /** @name Substitution */
  //@{

  /*! \brief In-place substitution (backward assignment) of linear expression.
   *
   * *this is modified in-place to reflect the effect of substituting l to dimension dim.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   */
  abstract0& substitute(manager& m, ap_dim_t dim, const linexpr0& l, const abstract0& inter = null);

  /*! \brief In-place parallel substitution (backward assignment) of linear expressions.
   *
   * *this is modified in-place to reflect the effect of substituting l[i] to dimension dim[i], for
   * i from 0 to size-1.
   * Substitutions are performed in parallel.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   */
  abstract0& substitute(manager& m, size_t size, const ap_dim_t dim[], const linexpr0 * const l[], const abstract0& inter = null);


  /*! \brief In-place parallel substitution (backward assignment) of linear expressions.
   *
   * *this is modified in-place to reflect the effect of substituting l[i] to dimension dim[i].
   * Substitutions are performed in parallel.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   *
   * \throw std::invalid_argument if the vectors have different size.
   */
  abstract0& substitute(manager& m, const std::vector<ap_dim_t>& dim, const std::vector<const linexpr0*>& l, const abstract0& inter = null);

  /*! \brief Substitution (backward assignment) of linear expression.
   *
   * dst is replaced with the effect of substituting l to dimension dim in src.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   */
  friend abstract0& substitute(manager& m, abstract0& dst, const abstract0& src, ap_dim_t dim, const linexpr0& l, const abstract0& inter);

  /*! \brief Parallel substitution (backward assignment) of linear expressions.
   *
   * dst is replaced with the effect of substituting l[i] to dimension dim[i] in src,
   * for i from 0 to size-1.
   * Substitutions are performed in parallel.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   */
  friend abstract0& substitute(manager& m, abstract0& dst, const abstract0& src,  size_t size, const ap_dim_t dim[], const linexpr0 * const l[], const abstract0& inter);

  /*! \brief Parallel substitution (backward assignment) of linear expressions.
   *
   * dst is replaced with the effect of substituting l[i] to dimension dim[i] in src.
   * Substitutions are performed in parallel.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   *
   * \throw std::invalid_argument if the vectors have different size.
   */
  friend abstract0& substitute(manager& m, abstract0& dst, const abstract0& src, const std::vector<ap_dim_t>& dim, const std::vector<const linexpr0*>& l, const abstract0& inter);




  /*! \brief In-place substitution (backward assignment) of arbitrary expression.
   *
   * *this is modified in-place to reflect the effect of substituting l to dimension dim.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   */
  abstract0& substitute(manager& m, ap_dim_t dim, const texpr0& l, const abstract0& inter = null);

  /*! \brief In-place parallel substitution (backward assignment) of arbitrary expressions.
   *
   * *this is modified in-place to reflect the effect of substituting l[i] to dimension dim[i], for
   * i from 0 to size-1.
   * Substitutions are performed in parallel.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   */
  abstract0& substitute(manager& m,  size_t size, const ap_dim_t dim[], const texpr0 * const l[], const abstract0& inter = null);


  /*! \brief In-place parallel substitution (backward assignment) of arbitrary expressions.
   *
   * *this is modified in-place to reflect the effect of substituting l[i] to dimension dim[i].
   * Substitutions are performed in parallel.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   *
   * \throw std::invalid_argument if the vectors have different size.
   */
  abstract0& substitute(manager& m, const std::vector<ap_dim_t>& dim, const std::vector<const texpr0*>& l, const abstract0& inter = null);

  /*! \brief Substitution (backward assignment) of arbitrary expression.
   *
   * dst is replaced with the effect of substituting l to dimension dim in src.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   */
  friend abstract0& substitute(manager& m, abstract0& dst, const abstract0& src, ap_dim_t dim, const texpr0& l, const abstract0& inter);

  /*! \brief Parallel substitution (backward assignment) of arbitrary expressions.
   *
   * dst is replaced with the effect of substituting l[i] to dimension dim[i] in src,
   * for i from 0 to size-1.
   * Substitutions are performed in parallel.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   */
  friend abstract0& substitute(manager& m, abstract0& dst, const abstract0& src,  size_t size, const ap_dim_t dim[], const texpr0 * const l[], const abstract0& inter);

  /*! \brief Parallel substitution (backward assignment) of arbitrary expressions.
   *
   * dst is replaced with the effect of substituting l[i] to dimension dim[i] in src.
   * Substitutions are performed in parallel.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   *
   * \throw std::invalid_argument if the vectors have different size.
   */
  friend abstract0& substitute(manager& m, abstract0& dst, const abstract0& src, const std::vector<ap_dim_t>& dim, const std::vector<const texpr0*>& l, const abstract0& inter);

  //@}



  /* Projection */
  /* ========== */

  /** @name Projection, forget */
  //@{

  /*! \brief Forgets about the value of dimension dim in *this.
   *
   * \arg \c project whether to reset the dimension to 0 (if true), or leave it undefined (if false).
   *
   * \return a reference to *this.
   */
  abstract0& forget(manager& m, ap_dim_t dim, bool project = false);

  /*! \brief Forgets about the value of dimensions dims[0] to dims[size-1] in *this.
   *
   * \arg \c project whether to reset the dimensions to 0 (if true), or leave them undefined (if false).
   *
   * \return a reference to *this.
   */
  abstract0& forget(manager& m, size_t size, const ap_dim_t dim[], bool project = false);

  /*! \brief Forgets about the value of all the dimensions in dim in *this.
   *
   * \arg \c project whether to reset the dimensions to 0 (if true), or leave them undefined (if false).
   *
   * \return a reference to *this.
   */
  abstract0& forget(manager& m, const std::vector<ap_dim_t> dim, bool project = false);

  /*! \brief Stores in dst the result of forgetting the value of dimension dim in src.
   *
   * \arg \c project if true, resets the dimension to 0 (if true).
   *
   * \return a reference to dst.
   */
  friend abstract0& forget(manager& m, abstract0& dst, const abstract0& src, ap_dim_t dim, bool project);

  /*! \brief Stores in dst the result of forgetting the value of dimensions dim[0] to
   * dim[size-1] in src.
   *
   * \arg \c project whether to reset the dimensions to 0 (if true), or leave them undefined (if false).
   *
   * \return a reference to dst.
   */
  friend abstract0& forget(manager& m, abstract0& dst, const abstract0& src, size_t size, const ap_dim_t dim[], bool project);

  /*! \brief Stores in dst the result of forgetting the value of all the dimensions in dim in src.
   *
   * \arg \c project whether to reset the dimensions to 0 (if true), or leave them undefined (if false).
   *
   * \return a reference to dst.
   */
  friend abstract0& forget(manager& m, abstract0& dst, const abstract0& src, const std::vector<ap_dim_t> dim, bool project);

  //@}


  /* Change of dimension */
  /* =================== */

  /** @name Change of dimension */
  //@{


  /*! \brief Adds some dimensions to *this.
   *
   * \arg \c project whether to set the new dimensions to 0 (if true), or leave them undefined (if false).
   *
   * \return a reference to *this.
   */
  abstract0& add_dimensions(manager& m, const dimchange& d, bool project = false);

  /*! \brief Removes some dimensions from *this.
   *
   * \return a reference to *this.
   */
  abstract0& remove_dimensions(manager& m, const dimchange& d);

  /*! \brief Permutes some dimensions in *this.
   *
   * \return a reference to *this.
   */
  abstract0& permute_dimensions(manager& m, const dimperm& d);


  /*! \brief Copies src into dst and adds some dimensions.
   *
   * \arg \c project whether to set the new dimensions to 0 (if true), or leave them undefined (if false).
   *
   * \return a reference to dst.
   */
  friend abstract0& add_dimensions(manager& m, abstract0& dst, const abstract0& src, const dimchange& d, bool project);

  /*! \brief Copies src into dst and removes some dimensions.
   *
   * \return a reference to dst.
   */
  friend abstract0& remove_dimensions(manager& m, abstract0& dst, const abstract0& src, const dimchange& d);

  /*! \brief Copies src into dst and permute some dimensions.
   *
   * \return a reference to dst.
   */
  friend abstract0& permute_dimensions(manager& m, abstract0& dst, const abstract0& src, const dimperm& d);

  //@}


  /* Expansion and folding */
  /* ===================== */

  /** @name Expansion and folding */
  //@{

  /*! \brief Duplicates dimension dim into n copies in *this (modified in-place).
   *
   * New dimensions are appended after the last integer or real dimension of *this.
   *
   * \return a reference to *this.
   */
  abstract0& expand(manager& m, ap_dim_t dim, size_t n = 1);

  /*! \brief Replaces dst with a copy of src and duplicates dimension dim into n copies.
   *
   * New dimensions are appended after the last integer or real dimension of dst.
   *
   * \return a reference to dst.
   */
  friend abstract0& expand(manager& m, abstract0& dst, const abstract0& src, ap_dim_t dim, size_t n);

  /*! \brief Folds dimensions dim[0] to dim[size-1] in *this (modified in-place).
   *
   * dim must be sorted and contain variables of the same type (integer or real).
   * After folding, only dim[0] is kept and other dimensions are removed.
   *
   * \return a reference to *this.
   */
  abstract0& fold(manager& m,  size_t size, const ap_dim_t dim[]);

  /*! \brief Folds all dimensions dims in *this (modified in-place).
   *
   * dim must be sorted and contain variables of the same type (integer or real).
   * After folding, only dim[0] is kept and other dimensions are removed.
   *
   * \return a reference to *this.
   */
  abstract0& fold(manager& m, const std::vector<ap_dim_t>& dim);

  /*! \brief Replaces dst with a copy of src and folds dimensions dims[0] to dim[size-1].
   *
   * dim must be sorted and contain variables of the same type (integer or real).
   * After folding, only dim[0] is kept and other dimensions are removed.
   *
   * \return a reference to dst.
   */
  friend abstract0& fold(manager& m, abstract0& dst, const abstract0& src,  size_t size, const ap_dim_t dim[]);

  /*! \brief Replaces dst with a copy of src and folds all dimensions in dims.
   *
   * dim must be sorted and contain variables of the same type (integer or real).
   * After folding, only dim[0] is kept and other dimensions are removed.
   *
   * \return a reference to dst.
   */
  friend abstract0& fold(manager& m, abstract0& dst, const abstract0& src, const std::vector<ap_dim_t>& dim);

  //@}


  /* Widenings */
  /* ========= */

  /** @name Widening */
  //@{

  /*! \brief Stores in dst the result of x widened with y.
   *
   * \warning y must be a superset of x.
   *
   * \return a reference to dst.
   */
  friend abstract0& widening(manager& m, abstract0& dst, const abstract0& x, const abstract0& y);

  /*! \brief Stores in dst the result of x widened with y, using some widening thresholds.
   *
   * \warning y must be a superset of x.
   *
   * \return a reference to dst.
   */
  friend abstract0& widening(manager& m, abstract0& dst, const abstract0& x, const abstract0& y, const lincons0_array& l);

  //@}


  /* Closure */
  /* ======= */

  /** @name Closure */
  //@{


  /*! \brief Replaces *this with its topological closure.
   *
   * \return a reference to *this.
   */
  abstract0& closure(manager& m);

  /*! \brief Stores in dst the topological closure of src.
   *
   * \return a reference to dst.
   */
  friend abstract0& closure(manager& m, abstract0& dst, const abstract0& src);

  //@}



  /* C-level compatibility */
  /* ===================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_abstract0_t* get_ap_abstract0_t();

  //! Returns a pointer to the internal APRON object stored in *this.
  const ap_abstract0_t* get_ap_abstract0_t() const;

  //@}

};

#include "apxx_abstract0_inline.hh"

}

#endif /* __APXX_ABSTRACT0_HH */
