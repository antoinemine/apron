/* -*- C++ -*-
 * apxx_abstract1.hh
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

#ifndef __APXX_ABSTRACT1_HH
#define __APXX_ABSTRACT1_HH

#include "ap_abstract1.h"
#include "apxx_abstract0.hh"
#include "apxx_linexpr1.hh"
#include "apxx_lincons1.hh"
#include "apxx_generator1.hh"


namespace apron {


/* ================================= */
/* abstract1                         */
/* ================================= */


/*! \brief Level 1 abstract value (ap_abstract1_t wrapper).
 *
 * Level 1 version of abstract values.
 * Variable names (var) are used in place of dimensions (ap_dim_t).
 * Internally, an abstract1 wraps together an abstract0 (memory managed) and an environment
 * (holding a reference count).
 */

class abstract1 : public use_malloc {

protected:

  ap_abstract1_t a; //!< Structure managed by APRON.

  //! NULL abstract element, to be used only as default argument in assign and substitute.
  static const abstract1 null;

  //! Internal use only. Shallow copy of structure.
  abstract1(ap_abstract1_t x);

  friend class manager;

public:


  /* constructors */
  /* ============ */

  /** @name Constructors */
  //@{

  //! Creates an abstract element representing the whole space.
  abstract1(manager &m, const environment& e, top x);

  //! Creates an abstract element representing the empty set.
  abstract1(manager &m, const environment& e, bottom x);

  //! Creates a (deep) copy of the abstract element.
  abstract1(manager &m, const abstract1& t);

  //! Creates a (deep) copy of the abstract element and associates an environment (reference count incremented).
  abstract1(manager &m, const environment& e, const abstract0& t);

  /*! \brief Creates an abstract element from a box.
   *
   * x[i] is the bound for the variable v[i].
   * Variables not in v have unconstrained bounds.
   */
  abstract1(manager &m, const environment& e, const var v[], const interval_array& x);

  /*! \brief Creates an abstract element from a box.
   *
   * x[i] is the bound for all the variables in v.
   * Variables not in v have unconstrained bounds.
   */
  abstract1(manager &m, const environment& e, const std::vector<var>& v, const interval_array& x);

  //! Creates an abstract element from a conjunction of linear constraints.
  abstract1(manager &m, const lincons1_array& x);

  //! Creates an abstract element from a conjunction of arbitrary constraints.
  abstract1(manager &m, const tcons1_array& x);

  /*! \brief Creates a (deep) copy of the abstract element.
   *
   * Implicitly uses the manager used to create *this.
   */
  abstract1(const abstract1& t);

  //@}


  /* destructor */
  /* ========== */

  /** @name Destructors */
  //@{

  /*! \brief Destroys the abstract element.
   *
   * Implicitly uses the manager used to create *this.
   */
  ~abstract1();

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
  abstract1& operator=(const abstract1& t);

  /*! \brief Assigns the full space to *this.
   *
   * Implicitly uses the manager used to create *this.
   */
  abstract1& operator=(top t);

  /*! \brief Assigns the empty set to *this.
   *
   * Implicitly uses the manager used to create *this.
   * Does not change the environment.
   */
  abstract1& operator=(bottom t);

  /*! \brief Assigns a box to *this.
   *
   * Implicitly uses the manager used to create *this.
   * Does not change the environment.
   *
   * \throw std::invalid_argument if the array has insufficient size.
   */
  abstract1& operator=(const interval_array& x);

  /*! \brief Assigns a conjunction of linear constraints to *this.
   *
   * Implicitly uses the manager used to create *this.
   * Does not change the environment.
   */
  abstract1& operator=(const lincons1_array& x);

  /*! \brief Assigns a conjunction of arbitrary constraints to *this.
   *
   * Implicitly uses the manager used to create *this.
   */
  abstract1& operator=(const tcons1_array& x);

  /*! \brief Replaces *this with a copy of x.
   *
   * \return a reference to *this.
   */
  abstract1& set(manager& m, const abstract1& x);

  /*! \brief Replaces *this with the full space.
   *
   * Does not change the environment.
   *
   * \return a reference to *this.
   */
  abstract1& set(manager& m, top t);

  /*! \brief Replaces *this with the full space.
   *
   * \return a reference to *this.
   */
  abstract1& set(manager& m, const environment& e, top t);

  /*! \brief Replaces *this with the empty set.
   *
   * Does not change the environment.
   *
   * \return a reference to *this.
   */
  abstract1& set(manager& m, bottom t);

  /*! \brief Replaces *this with the empty set.
   *
   * \return a reference to *this.
   */
  abstract1& set(manager& m, const environment& e, bottom t);

  /*! \brief Replaces *this with a box.
   *
   * Does not change the environment.
   *
   * \return a reference to *this.
   */
  abstract1& set(manager& m, const interval_array& x);


  /*! \brief Replaces *this with a box.
   *
   * \return a reference to *this.
   */
  abstract1& set(manager &m, const environment& e, const var v[], const interval_array& x);

  /*! \brief Replaces *this with a box.
   *
   * \return a reference to *this.
   */
  abstract1& set(manager &m, const environment& e, const std::vector<var>& v, const interval_array& x);

  /*! \brief Replaces *this with a conjunction of linear constraints.
   *
   * \return a reference to *this.
   */
  abstract1& set(manager& m, const lincons1_array& x);

  /*! \brief Replaces *this with a conjunction of arbitrary constraints.
   *
   * \return a reference to *this.
   */
  abstract1& set(manager& m, const tcons1_array& x);


  //@}


  /* representation */
  /* ============== */

  /** @name Control of internal representation */
  //@{

  /*! \brief Minimizes the size of the representation, to save memory.
   *
   * \return a reference to *this.
   */
  abstract1& minimize(manager& m);

  /*! \brief Puts the abstract element in canonical form (if such a notion exists).
   *
   * \return a reference to *this.
   */
  abstract1& canonicalize(manager& m);

  /*! \brief Simplifies the abstract element, potentially loosing precision.
   *
   * \return a reference to *this.
   */
  abstract1& approximate(manager& m, int algorithm);

  //@}


  /* printing */
  /* ======== */

  /** @name Printing */
  //@{

  //! Pretty-printing to a C stream.
  void print(manager& m, FILE* stream=stdout) const;

  //! Pretty-printing the difference between x and y to a C stream.
  friend void printdiff(manager& m, const abstract1& x, const abstract1& y, FILE* stream);

  //! Raw printing to a C stream (mainly for debug purposes).
  void dump(manager& m, FILE* stream=stdout) const;

  //! Prints in constraint form.
  friend std::ostream& operator<< (std::ostream& os, const abstract1& s);

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
  friend abstract1& deserialize(manager& m, abstract1& dst, const std::string& s, size_t* eaten);

  //@}


  /* access */
  /* ====== */

  /** @name Access */
  //@{

  //! Returns the manager the abstract element was created with (with reference count incremented).
  manager get_manager() const;

   //! Returns the environment of the abstract element (with reference count incremented).
  environment get_environment() const;

  //! Returns a (modifiable) reference to the underlying abstract0.
  abstract0& get_abstract0();

  //! Returns a reference to the underlying abstract0.
  const abstract0& get_abstract0() const;

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
  bool is_eq(manager& m, const abstract1& x) const;

  //! Whether *this is included in x (set-wise).
  bool is_leq(manager& m, const abstract1& x) const;

  //! Whether all points in *this satisfy a linear constraint.
  bool sat(manager& m, const lincons1& l) const;

  //! Whether all points in *this satisfy an arbitrary constraint.
  bool sat(manager& m, const tcons1& l) const;

  /*! \brief Whether the component v of all points in *this is included
   * in the given interval.
   */
  bool sat(manager& m, const var& v, const interval& i) const;

  //! \brief Whether the points in *this are unbounded in the given variable.
  bool is_variable_unconstrained(manager& m, const var& v) const;

  /*! \brief Whether x and y represent the same set.
   *
   * The manager for the left argument is used implicitly.
   */
  friend bool operator== (const abstract1& x, const abstract1& y);

  /*! \brief Whether x and y represent different sets.
   *
   * The manager for the left argument is used implicitly.
   */
  friend bool operator!= (const abstract1& x, const abstract1& y);

  /*! \brief Whether x is included within y (set-wise).
   *
   * The manager for the left argument is used implicitly.
   */
  friend bool operator<= (const abstract1& x, const abstract1& y);

  /*! \brief Whether x contains y (set-wise).
   *
   * The manager for the left argument is used implicitly.
   */
  friend bool operator>= (const abstract1& x, const abstract1& y);

  /*! \brief Whether x strictly contains y (set-wise).
   *
   * The manager for the left argument is used implicitly.
   */
  friend bool operator> (const abstract1& x, const abstract1& y);

  /*! \brief Whether x is strictly included within y (set-wise).
   *
   * The manager for the left argument is used implicitly.
   */
  friend bool operator< (const abstract1& x, const abstract1& y);



  /* Properties */
  /* ========== */

  /** @name Property extraction */
  //@{

  //! Returns some bounds for a linear expression evaluated in all points in the abstract element.
  interval bound(manager& m, const linexpr1& l) const;

  //! Returns some bounds for an arbitrary expression evaluated in all points in the abstract element.
  interval bound(manager& m, const texpr1& l) const;

  //! Returns some bounds for the given variable on all points in the abstract element.
  interval bound(manager& m, const var& v) const;

  //! Returns a bounding box for the set represented by the abstract element.
  interval_array to_box(manager& m) const;

  /*! \brief Returns a generator representation of (an over-approximation of) the
   * set represented by the abstract element.
   */
  generator1_array to_generator_array(manager& m) const;

  /*! \brief Returns a linear constraint representation of (an over-approximation of) the
   * set represented by the abstract element.
   */
  lincons1_array to_lincons_array(manager& m) const;

  /*! \brief Returns a constraint representation of (an over-approximation of) the
   * set represented by the abstract element.
   */
  tcons1_array to_tcons_array(manager& m) const;

  //@}


  /* Meet and unification */
  /* ==================== */

  /** @name Meet and unification */
  //@{

  /*! \brief Replaces *this with the meet of *this and the abstract element y.
   *
   * \return a reference to *this.
   */
  abstract1& meet(manager& m, const abstract1& y);

  /*! \brief Replaces dst with the meet of x and y.
   *
   * \return a reference to dst.
   */
  friend abstract1& meet(manager& m, abstract1& dst, const abstract1& x, const abstract1& y);


  /*! \brief Replaces *this with the meet of *this and the abstract element y.
   *
   * *this and y can have different environment.
   * They are first embedded into the least common environment before the meet is computed.
   *
   * \return a reference to *this.
   */
  abstract1& unify(manager& m, const abstract1& y);

  /*! \brief Replaces dst with the meet of x and y.
   *
   * x and y can have different environment.
   * They are first embedded into the least common environment before the meet is computed.
   *
   * \return a reference to dst.
   */
  friend abstract1& unify(manager& m, abstract1& dst, const abstract1& x, const abstract1& y);


  /*! \brief Replaces dst with the meet of all abstract elements in x.
   *
   * \return a reference to dst.
   */
  friend abstract1& meet(manager& m, abstract1& dst, const std::vector<const abstract1*>& x);

  /*! \brief Replaces dst with the meet of all size abstract elements in x.
   *
   * \return a reference to dst.
   */
  friend abstract1& meet(manager& m, abstract1& dst, size_t size, const abstract1 * const x[]);

  /*! \brief Adds some linear constraints to *this (modified in-place).
   *
   * \return a reference to *this.
   */
  abstract1& meet(manager& m, const lincons1_array& y);

  /*! \brief Replaces dst with the meet of x and some linear constraints.
   *
   * \return a reference to dst.
   */
  friend abstract1& meet(manager& m, abstract1& dst, const abstract1& x, const lincons1_array& y);

  /*! \brief Adds some arbitrary constraints to *this (modified in-place).
   *
   * \return a reference to *this.
   */
  abstract1& meet(manager& m, const tcons1_array& y);

  /*! \brief Replaces dst with the meet of x and some arbitrary constraints.
   *
   * \return a reference to dst.
   */
  friend abstract1& meet(manager& m, abstract1& dst, const abstract1& x, const tcons1_array& y);


  /*! \brief Replaces *this with the meet of *this and the abstract element y.
   *
   * Implicitly uses the manager used to create *this.
   *
   * \return a reference to *this.
   */
  abstract1& operator*=(const abstract1& y);


  /*! \brief Adds some linear constraints to *this (modified in-place).
   *
   * Implicitly uses the manager used to create *this.
   *
   * \return a reference to *this.
   */
  abstract1& operator*=(const lincons1_array& y);

  /*! \brief Adds some arbitrary constraints to *this (modified in-place).
   *
   * Implicitly uses the manager used to create *this.
   *
   * \return a reference to *this.
   */
  abstract1& operator*=(const tcons1_array& y);

  //@}




  /* Join */
  /* ==== */

  /** @name Join */
  //@{

  /*! \brief Replaces *this with the join of *this and the abstract element y.
   *
   * \return a reference to *this.
   */
  abstract1& join(manager& m, const abstract1& y);

  /*! \brief Replaces dst with the join of x and y.
   *
   * \return a reference to dst.
   */
  friend abstract1& join(manager& m, abstract1& dst, const abstract1& x, const abstract1& y);

  /*! \brief Replaces dst with the join of all abstract elements in x.
   *
   * \return a reference to dst.
   */
  friend abstract1& join(manager& m, abstract1& dst, const std::vector<const abstract1*>& x);

  /*! \brief Replaces dst with the join of all size abstract elements in x.
   *
   * \return a reference to dst.
   */
  friend abstract1& join(manager& m, abstract1& dst, size_t size, const abstract1 * const x[]);

  /*! \brief Adds some rays to *this (modified in-place).
   *
   * \arg \c y can only contain rays and lines, not vertexes.
   *
   * \return a reference to *this.
   */
  abstract1& add_rays(manager& m, const generator1_array& y);

  /*! \brief Replaces dst with x with some rays added.
   *
   * \arg \c y can only contain rays and lines, not vertexes.
   *
   * \return a reference to *this.
   */
  friend abstract1& add_rays(manager& m, abstract1& dst, const abstract1& x, const generator1_array& y);

  /*! \brief Replaces *this with the join of *this and the abstract element y.
   *
   * Implicitly uses the manager used to create *this.
   *
   * \return a reference to *this.
   */
  abstract1& operator+=(const abstract1& y);

  /*! \brief Adds some rays to *this (modified in-place).
   *
   * Implicitly uses the manager used to create *this.
   *
   * \arg \c y can only contain rays and lines, not vertexes.
   *
   * \return a reference to *this.
   */
  abstract1& operator+=(const generator1_array& y);

  //@}


  /* Assignments */
  /* =========== */

  /** @name Assignment */
  //@{

  /*! \brief In-place assignment of linear expression.
   *
   * *this is modified in-place to reflect the effect of assigning l to variable v.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   */
  abstract1& assign(manager& m, const var& v, const linexpr1& l, const abstract1& inter = null);

  /*! \brief In-place parallel assignment of linear expressions.
   *
   * *this is modified in-place to reflect the effect of assigning l[i] to variable v[i], for
   * i from 0 to size-1.
   * Assignments are performed in parallel.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   */
  abstract1& assign(manager& m,  size_t size, const var v[], const linexpr1 * const l[], const abstract1& inter = null);


  /*! \brief In-place parallel assignment of linear expressions.
   *
   * *this is modified in-place to reflect the effect of assigning l[i] to variable v[i].
   * Assignments are performed in parallel.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   *
   * \throw std::invalid_argument if the vectors have different size.
   */
  abstract1& assign(manager& m, const std::vector<var>& v, const std::vector<const linexpr1*>& l, const abstract1& inter = null);

  /*! \brief Assignment of linear expression.
   *
   * dst is replaced with the effect of assigning l to variable v in src.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   */
  friend abstract1& assign(manager& m, abstract1& dst, const abstract1& src, const var& v, const linexpr1& l, const abstract1& inter);

  /*! \brief Parallel assignment of linear expressions.
   *
   * dst is replaced with the effect of assigning l[i] to variable v[i] in src,
   * for i from 0 to size-1.
   * Assignments are performed in parallel.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   */
  friend abstract1& assign(manager& m, abstract1& dst, const abstract1& src,  size_t size, const var v[], const linexpr1 * const l[], const abstract1& inter);

  /*! \brief Parallel assignment of linear expressions.
   *
   * dst is replaced with the effect of assigning l[i] to variable v[i] in src.
   * Assignments are performed in parallel.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   *
   * \throw std::invalid_argument if the vectors have different size.
   */
  friend abstract1& assign(manager& m, abstract1& dst, const abstract1& src, const std::vector<var>& v, const std::vector<const linexpr1*>& l, const abstract1& inter);




  /*! \brief In-place assignment of arbitrary expression.
   *
   * *this is modified in-place to reflect the effect of assigning l to variable v.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   */
  abstract1& assign(manager& m, const var& v, const texpr1& l, const abstract1& inter = null);

  /*! \brief In-place parallel assignment of arbitrary expressions.
   *
   * *this is modified in-place to reflect the effect of assigning l[i] to variable v[i], for
   * i from 0 to size-1.
   * Assignments are performed in parallel.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   */
  abstract1& assign(manager& m, size_t size, const var v[], const texpr1 * const l[], const abstract1& inter = null);


  /*! \brief In-place parallel assignment of arbitrary expressions.
   *
   * *this is modified in-place to reflect the effect of assigning l[i] to variable v[i].
   * Assignments are performed in parallel.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   *
   * \throw std::invalid_argument if the vectors have different size.
   */
  abstract1& assign(manager& m, const std::vector<var>& v, const std::vector<const texpr1*>& l, const abstract1& inter = null);

  /*! \brief Assignment of arbitrary expression.
   *
   * dst is replaced with the effect of assigning l to variable v in src.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   */
  friend abstract1& assign(manager& m, abstract1& dst, const abstract1& src, const var& v, const texpr1& l, const abstract1& inter);

  /*! \brief Parallel assignment of arbitrary expressions.
   *
   * dst is replaced with the effect of assigning l[i] to variable v[i] in src,
   * for i from 0 to size-1.
   * Assignments are performed in parallel.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   */
  friend abstract1& assign(manager& m, abstract1& dst, const abstract1& src, size_t size, const var v[], const texpr1 * const l[], const abstract1& inter);

  /*! \brief Parallel assignment of arbitrary expressions.
   *
   * dst is replaced with the effect of assigning l[i] to variable v[i] in src.
   * Assignments are performed in parallel.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   *
   * \throw std::invalid_argument if the vectors have different size.
   */
  friend abstract1& assign(manager& m, abstract1& dst, const abstract1& src, const std::vector<var>& v, const std::vector<const texpr1*>& l, const abstract1& inter);

  //@}


  /* Substitutions */
  /* ============== */

  /** @name Substitution */
  //@{

  /*! \brief In-place substitution (backward assignment) of linear expression.
   *
   * *this is modified in-place to reflect the effect of substituting l to variable v.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   */
  abstract1& substitute(manager& m, const var& v, const linexpr1& l, const abstract1& inter = null);

  /*! \brief In-place parallel substitution (backward assignment) of linear expressions.
   *
   * *this is modified in-place to reflect the effect of substituting l[i] to variable v[i], for
   * i from 0 to size-1.
   * Substitutions are performed in parallel.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   */
  abstract1& substitute(manager& m, size_t size, const var v[], const linexpr1 * const l[], const abstract1& inter = null);


  /*! \brief In-place parallel substitution (backward assignment) of linear expressions.
   *
   * *this is modified in-place to reflect the effect of substituting l[i] to variable v[i].
   * Substitutions are performed in parallel.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   *
   * \throw std::invalid_argument if the vectors have different size.
   */
  abstract1& substitute(manager& m, const std::vector<var>& v, const std::vector<const linexpr1*>& l, const abstract1& inter = null);

  /*! \brief Substitution (backward assignment) of linear expression.
   *
   * dst is replaced with the effect of substituting l to variable v in src.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   */
  friend abstract1& substitute(manager& m, abstract1& dst, const abstract1& src, const var& v, const linexpr1& l, const abstract1& inter);

  /*! \brief Parallel substitution (backward assignment) of linear expressions.
   *
   * dst is replaced with the effect of substituting l[i] to variable v[i] in src,
   * for i from 0 to size-1.
   * Substitutions are performed in parallel.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   */
  friend abstract1& substitute(manager& m, abstract1& dst, const abstract1& src,  size_t size, const var v[], const linexpr1 * const l[], const abstract1& inter);

  /*! \brief Parallel substitution (backward assignment) of linear expressions.
   *
   * dst is replaced with the effect of substituting l[i] to variable v[i] in src.
   * Substitutions are performed in parallel.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   *
   * \throw std::invalid_argument if the vectors have different size.
   */
  friend abstract1& substitute(manager& m, abstract1& dst, const abstract1& src, const std::vector<var>& v, const std::vector<const linexpr1*>& l, const abstract1& inter);




  /*! \brief In-place substitution (backward assignment) of arbitrary expression.
   *
   * *this is modified in-place to reflect the effect of substituting l to variable v.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   */
  abstract1& substitute(manager& m, const var& v, const texpr1& l, const abstract1& inter = null);

  /*! \brief In-place parallel substitution (backward assignment) of arbitrary expressions.
   *
   * *this is modified in-place to reflect the effect of substituting l[i] to variable v[i], for
   * i from 0 to size-1.
   * Substitutions are performed in parallel.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   */
  abstract1& substitute(manager& m,  size_t size, const var v[], const texpr1 * const l[], const abstract1& inter = null);


  /*! \brief In-place parallel substitution (backward assignment) of arbitrary expressions.
   *
   * *this is modified in-place to reflect the effect of substituting l[i] to variable v[i].
   * Substitutions are performed in parallel.
   * If inter is specified, *this is then intersected with it.
   *
   * \return a reference to *this.
   *
   * \throw std::invalid_argument if the vectors have different size.
   */
  abstract1& substitute(manager& m, const std::vector<var>& v, const std::vector<const texpr1*>& l, const abstract1& inter = null);

  /*! \brief Substitution (backward assignment) of arbitrary expression.
   *
   * dst is replaced with the effect of substituting l to variable v in src.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   */
  friend abstract1& substitute(manager& m, abstract1& dst, const abstract1& src, const var& v, const texpr1& l, const abstract1& inter);

  /*! \brief Parallel substitution (backward assignment) of arbitrary expressions.
   *
   * dst is replaced with the effect of substituting l[i] to variable v[i] in src,
   * for i from 0 to size-1.
   * Substitutions are performed in parallel.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   */
  friend abstract1& substitute(manager& m, abstract1& dst, const abstract1& src,  size_t size, const var v[], const texpr1 * const l[], const abstract1& inter);

  /*! \brief Parallel substitution (backward assignment) of arbitrary expressions.
   *
   * dst is replaced with the effect of substituting l[i] to variable v[i] in src.
   * Substitutions are performed in parallel.
   * If inter is specified, dst is then intersected with it.
   *
   * \return a reference to dst.
   *
   * \throw std::invalid_argument if the vectors have different size.
   */
  friend abstract1& substitute(manager& m, abstract1& dst, const abstract1& src, const std::vector<var>& v, const std::vector<const texpr1*>& l, const abstract1& inter);

  //@}



  /* Projection */
  /* ========== */

  /** @name Projection, forget */
  //@{

  /*! \brief Forgets about the value of variable v in *this.
   *
   * \arg \c project whether to reset the variable to 0 (if true), or leave it undefined (if false).
   *
   * \return a reference to *this.
   */
  abstract1& forget(manager& m, const var& v, bool project = false);

  /*! \brief Forgets about the value of variables v[0] to v[size-1] in *this.
   *
   * \arg \c project whether to reset the variables to 0 (if true), or it them undefined (if false).
   *
   * \return a reference to *this.
   */
  abstract1& forget(manager& m, size_t size, const var v[], bool project = false);

  /*! \brief Forgets about the value of all the variables in v in *this.
   *
   * \arg \c project whether to reset the variables to 0 (if true), or leave them undefined (if false).
   *
   * \return a reference to *this.
   */
  abstract1& forget(manager& m, const std::vector<var>& v, bool project = false);

  /*! \brief Stores in dst the result of forgetting the value of variable v in src.
   *
   * \arg \c project if true, resets the variable to 0 (if true).
   *
   * \return a reference to dst.
   */
  friend abstract1& forget(manager& m, abstract1& dst, const abstract1& src, const var& v, bool project);

  /*! \brief Stores in dst the result of forgetting the value of variables v[0] to
   * v[size-1] in src.
   *
   * \arg \c project whether to reset the variables to 0 (if true), or leave them undefined (if false).
   *
   * \return a reference to dst.
   */
  friend abstract1& forget(manager& m, abstract1& dst, const abstract1& src, size_t size, const var v[], bool project);

  /*! \brief Stores in dst the result of forgetting the value of all the variables in v in src.
   *
   * \arg \c project whether to reset the variables to 0 (if true), or leave them undefined (if false).
   *
   * \return a reference to dst.
   */
  friend abstract1& forget(manager& m, abstract1& dst, const abstract1& src, const std::vector<var>& v, bool project);

  //@}


  /* Change of environment */
  /* ====================== */

  /** @name Change of environment */
  //@{

  /*! \brief Modifies the environment of *this.
   *
   * \arg \c project whether new variables are initialized to 0 (if true), or undefined (if false).
   *
   * \return a reference to *this.
   */
   abstract1& change_environment(manager& m, const environment& e, bool project = false);

  /*! \brief Replaces dst with src and changes its environment.
   *
   * \arg \c project whether new variables are initialized to 0 (if true), or undefined (if false).
   *
   * \return a reference to dst.
   */
  friend abstract1& change_environment(manager& m, abstract1& dst, const abstract1& src, const environment& e, bool project);

  /*! \brief Removes from *this the variables that are unconstrained.
   *
   * \return a reference to *this.
   */
  abstract1& minimize_environment(manager& m);

  /*! \brief Replaces dst with src and removes the variables that are unconstrained.
   *
   * \return a reference to dst.
   */
  friend abstract1& minimize_environment(manager& m, abstract1& dst, const abstract1& src);

  /*! \brief Renames oldv[i] into newv[i] in *this.
   *
   * \return a reference to *this.
   */
  abstract1& rename(manager& m, size_t size, const var oldv[], const var newv[]);

  /*! \brief Renames oldv[i] into newv[i] in *this.
   *
   * \return a reference to *this.
   */
  abstract1& rename(manager& m, const std::vector<var>& oldv, const std::vector<var>& newv);

  /*! \brief Replaces dst with src and renames oldv[i] into newv[i].
   *
   * \return a reference to dst.
   */
  friend abstract1& rename(manager& m, abstract1& dst, const abstract1& src, size_t size, const var oldv[], const var newv[]);

  /*! \brief Replaces dst with src and renames oldv[i] into newv[i].
   *
   * \return a reference to dst.
   */
  friend abstract1& rename(manager& m, abstract1& dst, const abstract1& src, const std::vector<var>& oldv, const std::vector<var>& newv);

  //@}


  /* Expansion and folding */
  /* ===================== */

  /** @name Expansion and folding */
  //@{

  /*! \brief Duplicates variable v into size copies in *this (modified in-place).
   *
   * New variables are named vv[0] to vv[size-1].
   *
   * \return a reference to *this.
   */
  abstract1& expand(manager& m, const var& v, size_t size, const var vv[]);

  /*! \brief Duplicates variable v in *this (modified in-place).
   *
   * The i-th new variable is named vv[i].
   *
   * \return a reference to *this.
   */
  abstract1& expand(manager& m, const var& v, const std::vector<var>& vv);

  /*! \brief Replaces dst with a copy of src and duplicates variable v into size copies.
   *
   * New variables are named vv[0] to vv[size-1].
   *
   * \return a reference to dst.
   */
  friend abstract1& expand(manager& m, abstract1& dst, const abstract1& src, const var& v, size_t size, const var vv[]);

  /*! \brief Replaces dst with a copy of src and duplicates variable v.
   *
   * The i-th new variables is named vv[i].
   *
   * \return a reference to dst.
   */
  friend abstract1& expand(manager& m, abstract1& dst, const abstract1& src, const var& v, const std::vector<var>& vv);

  /*! \brief Folds variables v[0] to v[size-1] in *this (modified in-place).
   *
   * After folding, only v[0] is kept and other variables are removed.
   *
   * \return a reference to *this.
   */
  abstract1& fold(manager& m,  size_t size, const var v[]);

  /*! \brief Folds all variables v in *this (modified in-place).
   *
   * After folding, only v[0] is kept and other variables are removed.
   *
   * \return a reference to *this.
   */
  abstract1& fold(manager& m, const std::vector<var>& v);

  /*! \brief Replaces dst with a copy of src and folds variables v[0] to v[size-1].
   *
   * After folding, only v[0] is kept and other variables are removed.
   *
   * \return a reference to dst.
   */
  friend abstract1& fold(manager& m, abstract1& dst, const abstract1& src,  size_t size, const var v[]);

  /*! \brief Replaces dst with a copy of src and folds all variables in v.
   *
   * After folding, only v[0] is kept and other variables are removed.
   *
   * \return a reference to dst.
   */
  friend abstract1& fold(manager& m, abstract1& dst, const abstract1& src, const std::vector<var>& v);

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
  friend abstract1& widening(manager& m, abstract1& dst, const abstract1& x, const abstract1& y);

  /*! \brief Stores in dst the result of x widened with y, using some widening thresholds.
   *
   * \warning y must be a superset of x.
   *
   * \return a reference to dst.
   */
  friend abstract1& widening(manager& m, abstract1& dst, const abstract1& x, const abstract1& y, const lincons1_array& l);

  //@}


  /* Closure */
  /* ======= */

  /** @name Closure */
  //@{


  /*! \brief Replaces *this with its topological closure.
   *
   * \return a reference to *this.
   */
  abstract1& closure(manager& m);

  /*! \brief Stores in dst the topological closure of src.
   *
   * \return a reference to dst.
   */
  friend abstract1& closure(manager& m, abstract1& dst, const abstract1& src);

  //@}



  /* C-level compatibility */
  /* ===================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_abstract1_t* get_ap_abstract1_t();

  //! Returns a pointer to the internal APRON object stored in *this.
  const ap_abstract1_t* get_ap_abstract1_t() const;

  //@}

};

#include "apxx_abstract1_inline.hh"

}

#endif /* __APXX_ABSTRACT1_HH */
