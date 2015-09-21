/* -*- C++ -*-
 * apxx_generator1.hh
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

#ifndef __APXX_GENERATOR1_HH
#define __APXX_GENERATOR1_HH

#include "ap_generator1.h"
#include "apxx_environment.hh"
#include "apxx_generator0.hh"
#include "apxx_linexpr1.hh"

namespace apron {



/* ================================= */
/* generator1                        */
/* ================================= */


/*! \brief Level 1 generator (ap_generator1_t wrapper).
 *
 * Level 1 version of linear generators with scalar coefficients (coeff).
 * Terms are indexed using variable names (var) instead of dimensions (ap_dim_t).
 * Internally, a generator1 wraps together a generator0 (memory managed) and an environment (holding a
 * reference count).
 */
class generator1 : public use_malloc {

protected:
  
  ap_generator1_t l; //!<  Structure managed by APRON.

  //! Internal use only. Shallow copy (no copy of generator0 or environment).
  generator1(ap_generator1_t p);

  friend class generator1_array;

public:


  /* constructors */
  /* ============ */

  /** @name Constructors */
  //@{

  /*! \brief Creates a new generator from a generator0 (copied) and an environment (reference count
   * incremented) associating names to dimensions in generator0.
   */
  generator1(const environment& e, const generator0& x);

  /*! \brief Creates a new generator from an empty linear expression.
   *
   * The linear expression is created sparse and empty (has_linexpr returns true).
   * \arg \c e associates a variable name to each dimension (reference counter incremented).
   * \arg \c gentyp can be AP_GEN_LINE, AP_GEN_RAY, AP_GEN_VERTEX,  AP_GEN_LINEMOD,
   * or AP_GEN_RAYMOD.
   */
  generator1(const environment& e, ap_gentyp_t gentyp=AP_GEN_RAY);

  /*! \brief Creates a new generator from a linear expression (copied).
   *
   * \arg \c e associates a variable name to each dimension (reference counter incremented).
   * \arg \c gentyp can be AP_GEN_LINE, AP_GEN_RAY, AP_GEN_VERTEX,  AP_GEN_LINEMOD,
   * or AP_GEN_RAYMOD.
   *
   * \warning \c lin should have only scalar (non-interval) coefficients (unchecked).
   */
  generator1(ap_gentyp_t gentyp, const linexpr1& lin);

  //! (Deep) copy of a generator.
  generator1(const generator1& x);

  /*! \brief Makes a (deep) copy of x and extends its environment.
   *
   * \throw std::invalid_argument if e is not a super-environment of that of x.
   */
  generator1(const generator1& x, const environment& e);

  //@}


  /* destructor */
  /* ========== */

  /** @name Destructor */
  //@{

  /*! \brief Frees all space for the generator, and decrements the reference count 
   * of the environment.
   */  
  ~generator1();

  //@}


  /* assignment */
  /* ========== */

  /** @name Assignment */
  //@{

  //! Makes a (deep) copy.
  generator1& operator= (const generator1& x);

  /*! \brief Sets the underlying linear expression to c (copied).
   *
   * Does not fail as get_linexpr can: if the generator was created without an underlying expression, 
   * it is created.
   *
   * \warning \c c should have only scalar (non-interval) coefficients (unchecked).
   * \warning assumes that c and *this have equal environments (unchecked).
   */
  void set_linexpr(const linexpr1& c);

  //@}


  /* dimension operations */
  /* ==================== */

  /** @name Dimension operations */
  //@{

  /*! \brief Extends the environment of the generator.
   *
   * \throw std::invalid_argument if e is not a super-environment of that of *this.
   */
  void extend_environment(const environment& e);

  //@}

  
  /* access */
  /* ====== */

  /** @name Accesses, size */
  //@{

  //! Returns the environment of the generator (with incremented reference count).
  environment get_environment() const;

  //! Returns a reference to the underlying generator0.
  const generator0& get_generator0() const;

  //! Returns a (modifiable) reference to the underlying generator0.
  generator0& get_generator0();

  /*! \brief Returns the size of the underlying linear expression.
   *
   * \throw std::invalid_argument if no valid linear expression has been defined.
   */
  size_t size() const;


  /* get */
  
  /*! \brief Returns a (modifiable) reference to the constraint type.
   *
   * \return either AP_GEN_LINE, AP_GEN_RAY, AP_GEN_VERTEX, AP_GEN_LINEMOD, or AP_GEN_RAYMOD.
   */
  ap_gentyp_t& get_gentyp();
  
  /*! \brief Returns a reference to the constraint type.
   *
   * \return either AP_GEN_LINE, AP_GEN_RAY, AP_GEN_VERTEX, AP_GEN_LINEMOD, or AP_GEN_RAYMOD.
   */
  const ap_gentyp_t& get_gentyp() const;
  
  /*! \brief Whether the generator has a valid linear expression.
   *
   * \note The only way the linear expression may be invalid is when accessing fields of uninitialised 
   * (or enlarged) generator1_array.
   */
  bool has_linexpr() const;

  /*! \brief Returns a copy of the underlying linear expression.
   *
   * \throw std::invalid_argument if no valid linear expression has been defined.
   */
  linexpr1 get_linexpr() const;

  /*! \brief Returns a (modifiable) reference to the constant coefficient.
   *
   * \throw std::invalid_argument if no valid linear expression has been defined.
   */
  coeff& get_cst();

  /*! \brief Returns a reference to the constant coefficient.
   *
   * \throw std::invalid_argument if no valid linear expression has been defined.
   */
  const coeff& get_cst() const;

  /*! \brief Returns a (modifiable) reference to the coefficient corresponding to the given variable name.
   *
   * \throw std::invalid_argument if the variable name is not present in the environment.
   * \throw std::invalid_argument if no valid linear expression has been defined.
   */
  coeff& operator[](const var& v);
  
  /*! \brief Returns a reference to the coefficient corresponding to the given variable name.
   *
   * \throw std::invalid_argument if the variable name is not present in the environment.
   * \throw std::invalid_argument if no valid linear expression has been defined.
   */
  const coeff& operator[](const var& v) const;

  //@}


  /* print */
  /* ===== */

  /** @name Printing */
  //@{

  //! Printing.
  friend std::ostream& operator<< (std::ostream& os, const generator1& s);

  //! Prints to a C stream.
  void print(FILE* stream=stdout) const;

  //@}


  /* TODO: intelligent constructors */


  /* C-level compatibility */
  /* ===================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object stored in *this.
  const ap_generator1_t* get_ap_generator1_t() const;

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_generator1_t* get_ap_generator1_t();

  //@}
};



/* ================================= */
/* generator1_array                  */
/* ================================= */


/*! \brief Array of generators (ap_generator1_array_t wrapper).
 *
 * Level 1 version of linear generator arrays.
 * Internally, a generator1_array wraps together a generator0_array (memory managed) and an environment 
 * (holding a reference count).
 * Please note that all generators share the same environment!
 */
class generator1_array : public use_malloc {

protected:

  ap_generator1_array_t a;  //!< Structure managed by APRON.

  //! Internal use only. Shallow copy (no copy of generator0_array or environment).
  generator1_array(ap_generator1_array_t& a);

  friend class abstract1;

public:
  
  /* constructors */
  /* ============ */

  /** @name Constructors */
  //@{

  /*! \brief Creates a new generator array from a generator0_array (copied) and an environment (reference count
   * incremented) associating names to dimensions.
   */
  generator1_array(const environment& e, const generator0_array& x);

  /*! \brief Creates a new array of the given size containing uninitialized generators.
   *
   * has_linexpr will return false on all elements of the array.
   * \arg \c e associates a variable name to each dimension (reference count incremented).
   */
  generator1_array(const environment& e, size_t size);

  //! (Deep) copy.
  generator1_array(const generator1_array& x);

  /*! \brief Makes a (deep) copy of x and extends its environment.
   *
   * \throw std::invalid_argument if e is not a super-environment of that of x.
   */
  generator1_array(const generator1_array& x, const environment& e);

  /*! \brief Creates a generator1_array from an array (of size >0) of generators of the given size (copied).
   *
   * \warning assumes that all generators have the same environment (unchecked).
   * \throw std::invalid_argument if size<1.
   */
  generator1_array(size_t size, const generator1 x[]);

  /*! \brief Creates a generator1_array from an vector (of size >0) of generators of the given size (copied).
   *
   * \warning assumes that all generators have the same environment (unchecked).
   * \throw std::invalid_argument if vector size<1.
   */
  generator1_array(const std::vector<generator1>& x);

  //@}


  /* destructor */
  /* ========== */

  /** @name Destructor */
  //@{

  /*! \brief Frees the space used by the array and all its generators, and decrements the 
   * reference count of the environment.
   */
  ~generator1_array();

  //@}


  /* assignment */
  /* ========== */

  /** @name Assignments */
  //@{

  //! (Deep) copy.
  generator1_array& operator= (const generator1_array& x);

  /*! \brief Copies the generators from the array into *this.
   *
   * \warning assumes that all generators have the same environment (unchecked).
   * \arg x should contain (at least) size elements.
   */
  generator1_array& operator= (const generator1 x[]);

  /*! Copies the generators from the vector into the array, changing its size if needed.
   *
   * \warning assumes that all generators have the same environment (unchecked).
   */
  generator1_array& operator= (const std::vector<generator1>& x);

  //@}


  /* dimension operations */
  /* ==================== */

  /** @name Dimension operations */
  //@{

  //! Resizes the array.
  void resize(size_t size);

  /*! \brief Extends the environment of all generators in the array.
   *
   * \throw std::invalid_argument if e is not a super-environment of that of *this.
   */
  void extend_environment(const environment& e);

  //@}


  /* access */
  /* ====== */

  /** @name Accesses */
  //@{

  //! Returns the size of the array.
  size_t size() const;

  //! Returns the environment shared by all generators (with incremented reference count).
  environment get_environment() const;

  //! Returns a reference to the underlying generator0_array.
  const generator0_array& get_generator0_array() const;

  //! Returns a (modifiable) reference to the underlying generator0_array.
  generator0_array& get_generator0_array();

  /*! \brief Returns a copy of the generator at index i.
   *
   * \throw std::out_of_range if i exceeds the dimension of the array.
   */
  generator1 get(size_t i) const;

  /*! \brief Changes the generator at index i.
   *
   * \warning assumes that x and *this have equal environments.
   * \throw std::out_of_range if i exceeds the dimension of the array.
   */
  void set(size_t i, const generator1& x);

  //@}


  /* conversion */
  /* ========== */
  
  /** @name Conversion */
  //@{

  //! Returns a copy of the linear generators in the form of a vector.
  operator std::vector<generator1>() const;

  //@}


  /* print */
  /* ===== */

  /** @name Printing */
  //@{

  /*! \brief Printing.
   *
   * \throw std::invalid_argument an underlying expression is missing, or an
   * auxiliary scalar is missing (for modulo generator).
   */
  friend std::ostream& operator<< (std::ostream& os, const generator1_array& s);

  //! Prints to a C stream.
  void print(FILE* stream=stdout) const;

  //@}


  /* C-level compatibility */
  /* ===================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object stored in *this.
  const ap_generator1_array_t* get_ap_generator1_array_t() const;

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_generator1_array_t* get_ap_generator1_array_t();

  //@}

};

#include "apxx_generator1_inline.hh"

}

#endif /* __APXX_GENERATOR1_HH */
