/* -*- C++ -*-
 * apxx_generator0.hh
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

#ifndef __APXX_GENERATOR0_HH
#define __APXX_GENERATOR0_HH

#include <vector>
#include "ap_generator0.h"
#include "apxx_linexpr0.hh"


namespace apron {



/* ================================= */
/* generator0                        */
/* ================================= */


/*! \brief Level 0 generator (ap_generator0_t wrapper).
 *
 * A generator0 object represents a generator: vertex, ray, line, modular line, or modular ray.
 * It stores and manages a linexpr0 (linear expression with scalar coefficients only), 
 * and a generator type (vertex, ray , etc).
 */
class generator0 : public use_malloc {

protected:
  
  ap_generator0_t l; //!< Structure managed by APRON.

  //! Internal use only. Performs a shallow copy and takes ownership of the contents.
  generator0(ap_generator0_t& l) : l(l) {}

  friend class abstract0;

public:


  /* constructors */
  /* ============ */

  /*! \brief Creates a new generator from an empty linear expression.
   *
   * The linear expression is created sparse and empty (has_linexpr returns true).
   * \arg \c gentyp can be \c AP_GEN_LINE, \c AP_GEN_RAY, \c AP_GEN_VERTEX,  \c AP_GEN_LINEMOD,
   * or \c AP_GEN_RAYMOD.
   */
  generator0(ap_gentyp_t gentyp=AP_GEN_RAY);

  /*! \brief Creates a new generator constraint from a linear expression (copied).
   *
   * \arg \c gentyp can be \c AP_GEN_LINE, \c AP_GEN_RAY, \c AP_GEN_VERTEX,  \c AP_GEN_LINEMOD,
   * or \c AP_GEN_RAYMOD.
   *
   * \warning \c lin should have only scalar (non-interval) coefficients (unchecked).
   */
  generator0(ap_gentyp_t gentyp, const linexpr0& lin);

  //! (Deep) copy of a generator.
  generator0(const generator0& x);

  //! Makes a (deep) copy of a generator, and applies a dimension change to the underlying linear expression.
  generator0(const generator0& x, const dimchange& d);

  //! Makes a (deep) copy of a generator, and applies a permutation to the underlying linear expression.
  generator0(const generator0& x, const dimperm& d);


  /* destructor */
  /* ========== */

  //! Frees the generator, including the embedded linear expression.
  ~generator0();


  /* assignment */
  /* ========== */

  //! (Deep) copy.
  generator0& operator= (const generator0& x);


  /* dimension operations */
  /* ==================== */

  /*! \brief Resizes the underlying linear expression.
   *
   * \throw std::invalid_argument if no valid linear expression has been defined.
   */
  void resize(size_t size);

  /*! \brief Changes the dimension of the underlying linear expression.
   *
   * \throw std::invalid_argument if no valid linear expression has been defined.
   */
  void add_dimensions(const dimchange& d);

  /*! \brief Applies a permutation to the underlying linear expression.
   *
   * \throw std::invalid_argument if no valid linear expression has been defined.
   */
  void permute_dimensions(const dimperm& d);
  

  /* access */
  /* ====== */

  /* size */

  /*! \brief Returns the size of the underlying linear expression.
   *
   * \throw std::invalid_argument if no valid linear expression has been defined.
   */
  size_t size() const;


  /* get */
  
  /*! \brief Returns a (modifiable) reference to the constraint type.
   *
   * \return either \c AP_GEN_LINE, \c AP_GEN_RAY, \c AP_GEN_VERTEX, \c AP_GEN_LINEMOD, or \c AP_GEN_RAYMOD.
   */
  ap_gentyp_t& get_gentyp();
  
  /*! \brief Returns a reference to the constraint type.
   *
   * \return either \c AP_GEN_LINE, \c AP_GEN_RAY, \c AP_GEN_VERTEX, \c AP_GEN_LINEMOD, or \c AP_GEN_RAYMOD.
   */
  const ap_gentyp_t& get_gentyp() const;
  
  /*! \brief Returns whether the generator has a valid linear expression.
   *
   * \note The only way the linear expression may be invalid is when accessing fields of uninitialised 
   * (or enlarged) generator0_array.
   */
  bool has_linexpr() const;

  /*! \brief Returns a (modifiable) reference to the underlying linear expression.
   *
   * \throw std::invalid_argument if no valid linear expression has been defined.
   */
  linexpr0& get_linexpr();
 
  /*! \brief Returns a reference to the underlying linear expression.
   *
   * \throw std::invalid_argument if no valid linear expression has been defined.
   */
  const linexpr0& get_linexpr() const;

  /*! \brief Sets the underlying linear expression to c (copied).
   *
   * Does not fail as get_linexpr can: if the generator was created without an underlying expression, 
   * it is created.
   * \warning \c c should have only scalar (non-interval) coefficients (unchecked).
   */
  void set_linexpr(const linexpr0& c);


  /* print */
  /* ===== */

  /*! \brief Printing.
   *
   * Variable naming can be configured through the varname stream modifier.
   *
   * \throw std::invalid_argument if the underlying expression is missing.
   */
  friend std::ostream& operator<< (std::ostream& os, const generator0& s);

  //! Prints to a C stream.
  void print(char** name_of_dim=NULL, FILE* stream=stdout) const;


  /* TODO: intelligent constructors */

 
  /* C-level compatibility */
  /* ===================== */

  //! Returns a pointer to the internal APRON object stored in *this.
  const ap_generator0_t* get_ap_generator0_t() const;

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_generator0_t* get_ap_generator0_t();

};




/* ================================= */
/* generator0_array                  */
/* ================================= */


/*! \brief Array of generators (ap_generator0_array_t wrapper).
 *
 * A generator0_array represents an array of generator(s).
 */
class generator0_array : public use_malloc {

protected:

  ap_generator0_array_t a;  //!< Structure managed by APRON.

  //! Internal use only. Performs a shallow copy and takes ownership of the contents.
  generator0_array(ap_generator0_array_t& a) : a(a) {}

  friend class abstract0;
  friend class generator1_array;

public:
  
  /* constructors */
  /* ============ */

  /** @name Constructors */
  //@{

  /*! \brief Creates a new array of the given size containing uninitialized generators.
   *
   * has_linexpr will return false on all elements of the array.
   */
  generator0_array(size_t size);

  //! (Deep) copy.
  generator0_array(const generator0_array& x);

  //! Makes a (deep) copy of the array and applies add_dimensions to all generators.
  generator0_array(const generator0_array& x, const dimchange& d);

  //! Makes a (deep) copy of the array and applies permute_dimensions to all generators.
  generator0_array(const generator0_array& x, const dimperm& d);

  //! Creates a generator0_array from an array of generators of the given size (copied).
  generator0_array(size_t size, const generator0 x[]);

  //! Creates a generator0_array from a vector of generators (copied).
  generator0_array(const std::vector<generator0>& x);

  //@}


  /* destructor */
  /* ========== */

   /** @name Destructor */
  //@{

 //! Frees the space used by the array and all its generators.
  ~generator0_array();

  //@}


  /* assignment */
  /* ========== */

  /** @name Assignments */
  //@{

  //! (Deep) copy.
  generator0_array& operator= (const generator0_array& x);

  /*! \brief Copies the generators from the array into *this.
   *
   * \arg x should contain (at least) size elements.
   */
  generator0_array& operator= (const generator0 x[]);

  //! Copies the generators from the vector into the array, changing its size if needed.
  generator0_array& operator= (const std::vector<generator0>& x);

  //@}


  /* dimension operations */
  /* ==================== */

  /** @name Dimension operations */
  //@{

  //! Resizes the array.
  void resize(size_t size);

  //! Applies add_dimensions to all generators in the array.
  void add_dimensions(const dimchange& d);

  //! Applies permute_dimensions to all generators in the array.
  void permute_dimensions(const dimperm& d);

  //@}


  /* access */
  /* ====== */

  /** @name Access */
  //@{

  //! Returns the size of the array.
  size_t size() const;
 
  //! Returns a pointer to the start of the internal array holding the generators.
  generator0* contents();

  //! Returns a pointer to the start of the internal array holding the generators.
  const generator0* contents() const;

  //! Returns a (modifiable) reference to an element, no bound checking.
  generator0& operator[](size_t i);

  //! Returns a reference to an element, no bound checking.
  const generator0& operator[](size_t i) const;

  /*! \brief Returns a (modifiable) reference to an element (bound-checked).
   *
   * \throw std::out_of_range if the index is invalid.
   */
  generator0& get(size_t i);

  /*! \brief Returns a reference to an element (bound-checked).
   *
   * \throw std::out_of_range if the index is invalid.
   */
  const generator0& get(size_t i) const;

  //@}


  /* conversion */
  /* ========== */
  
  /** @name Conversion */
  //@{

  //! Returns a copy of the generators in the form of a vector.
  operator std::vector<generator0>() const;

  //@}


  /* print */
  /* ===== */

  /** @name Printing */
  //@{

  /*! \brief Printing.
   *
   * Variable naming can be configured through the varname stream modifier.
   *
   * \throw std::invalid_argument an underlying expression is missing.
   */
  friend std::ostream& operator<< (std::ostream& os, const generator0_array& s);

  //! Prints to a C stream.
  void print(char** name_of_dim=NULL, FILE* stream=stdout) const;

  //@}


  /* C-level compatibility */
  /* ===================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object stored in *this.
  const ap_generator0_array_t* get_ap_generator0_array_t() const;

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_generator0_array_t* get_ap_generator0_array_t();

  //@}  
};


#include "apxx_generator0_inline.hh"

}

#endif /* __APXX_GENERATOR0_HH */
