/* -*- C++ -*-
 * apxx_lincons1.hh
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

#ifndef __APXX_LINCONS1_HH
#define __APXX_LINCONS1_HH

#include "ap_lincons1.h"
#include "apxx_environment.hh"
#include "apxx_lincons0.hh"
#include "apxx_linexpr1.hh"


namespace apron {


/* ================================= */
/* lincons1                          */
/* ================================= */


/*! \brief Level 1 linear constraint (ap_lincons1_t wrapper).
 *
 * Level 1 version of linear constraints with scalar or interval coefficients (coeff).
 * Terms are indexed using variable names (var) instead of dimensions (ap_dim_t).
 * Internally, a lincons1 wraps together a lincons0 (memory managed) and an environment (holding a
 * reference count).
 */
class lincons1 : public use_malloc {

protected:
  
  ap_lincons1_t l; //!<  Structure managed by APRON.

  //! Internal use only. Shallow copy (no copy of lincons0 or environment).
  lincons1(ap_lincons1_t p);


  friend class lincons1_array;

public:


  /* constructors */
  /* ============ */

  /** @name Constructors */
  //@{

  /*! \brief Creates a new constraint from a lincons0 (copied) and an environment (reference count
   * incremented) associating names to dimensions in lincons0.
   */
  lincons1(const environment& e, const lincons0& x);

  /*! \brief Creates a new (non-modulo) constraint from an empty linear expression.
   *
   * The auxiliary scalar is not created (has_modulo returns false).
   * The linear expression is created sparse and empty (has_linexpr returns true).
   * \arg \c e associates a variable name to each dimension (reference counter incremented).
   * \arg \c constyp can be \c AP_CONS_EQ, \c AP_CONS_SUPEQ, \c AP_CONS_SUP, or \c AP_CONS_DISEQ (but not \c AP_CONS_EQMOD).
   */
  lincons1(const environment& e, ap_constyp_t constyp=AP_CONS_SUPEQ);

  /*! \brief Creates a new (non-modulo) constraint from a linear expression (copied).
   *
   * The auxiliary scalar is not created (has_modulo returns false).
   * \arg \c constyp can be \c AP_CONS_EQ, \c AP_CONS_SUPEQ, \c AP_CONS_SUP, or \c AP_CONS_DISEQ (but not \c AP_CONS_EQMOD).
   */
  lincons1(ap_constyp_t constyp, const linexpr1& lin);

  /*! \brief Creates a new constraint from a linear expression and a modulo scalar (both copied).
   *
   * \arg \c constyp can be \c AP_CONS_EQ, \c AP_CONS_SUPEQ, \c AP_CONS_SUP, \c AP_CONS_EQMOD, or \c AP_CONS_DISEQ.
   */
  lincons1(ap_constyp_t constyp, const linexpr1& lin, const scalar& modulo);

  //! Makes an unsatisfiable constraint (-1>=0).
  lincons1(const environment& e, unsat x);

  //! (Deep) copy of a constraint.
  lincons1(const lincons1& x);

  /*! \brief Makes a (deep) copy of x and extends its environment.
   *
   * \throw std::invalid_argument if e is not a super-environment of that of x.
   */
  lincons1(const lincons1& x, const environment& e);

  //@}


  /* destructor */
  /* ========== */

  /** @name Destructor */
  //@{

  /*! \brief Frees all space for the expression and coefficients, and decrements the reference count 
   * of the environment.
   */  
  ~lincons1();

  //@}


  /* assignment */
  /* ========== */

  /** @name Assignment */
  //@{

  //! Makes a (deep) copy.
  lincons1& operator= (const lincons1& x);

  //! Assigns an unsatisfiable constraint to *this (-1>=0).
  lincons1& operator= (unsat x);

  /*! \brief Sets the auxiliary scalar modulo to c (copied).
   *
   * Does not fail as get_modulo can: if the constraint was created without an auxiliary scalar, 
   * it is created.
   */
  void set_modulo(const scalar& c);  

  /*! \brief Sets the underlying linear expression to c (copied).
   *
   * Does not fail as get_linexpr can: if the constraint was created without an underlying expression, 
   * it is created.
   *
   * \warning assumes that c and *this have equal environments (unchecked).
   */
  void set_linexpr(const linexpr1& c);

  //@}


  /* dimension operations */
  /* ==================== */

  /** @name Dimension operations */
  //@{

  /*! \brief Extends the environment of the expression.
   *
   * \throw std::invalid_argument if e is not a super-environment of that of *this.
   */
  void extend_environment(const environment& e);

  //@}

  
  /* access */
  /* ====== */

  /** @name Accesses, size */
  //@{

  //! Returns the environment of the constraint (with incremented reference count).
  environment get_environment() const;

  //! Returns a reference to the underlying lincons0.
  const lincons0& get_lincons0() const;

  //! Returns a (modifiable) reference to the underlying lincons0.
  lincons0& get_lincons0();

  /*! \brief Returns the size of the underlying linear expression.
   *
   * \throw std::invalid_argument if no valid linear expression has been defined.
   */
  size_t size() const;

  /*! \brief Returns a (modifiable) reference to the constraint type.
   *
   * \return either \c AP_CONS_EQ, \c AP_CONS_SUPEQ, \c AP_CONS_SUP, \c AP_CONS_EQMOD, or \c AP_CONS_DISEQ.
   */
  ap_constyp_t& get_constyp();
  
  /*! \brief Returns a reference to the constraint type.
   *
   * \return either \c AP_CONS_EQ, \c AP_CONS_SUPEQ, \c AP_CONS_SUP, \c AP_CONS_EQMOD, or \c AP_CONS_DISEQ.
   */
  const ap_constyp_t& get_constyp() const;

  //! Whether the constraint has a valid auxiliary scalar (used in modulo constraints).
  bool has_modulo() const;

  /*! \brief Whether the constraint has a valid linear expression.
   *
   * \note The only way the linear expression may be invalid is when accessing fields of uninitialised 
   * (or enlarged) lincons1_array.
   */
  bool has_linexpr() const;

  /*! \brief Returns a (modifiable) reference to the auxiliary scalar.
   *
   * \throw std::invalid_argument if no valid auxiliary scalar has been defined.
   */
  scalar& get_modulo();
 
  /*! \brief Returns a reference to the auxiliary scalar.
   *
   * \throw std::invalid_argument if no valid auxiliary scalar has been defined.
   */
  const scalar& get_modulo() const;

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
  friend std::ostream& operator<< (std::ostream& os, const lincons1& s);

  //! Prints to a C stream.
  void print(FILE* stream=stdout) const;

  //@}


  /* tests */
  /* ===== */

  /** @name Tests */
  //@{

  /*! \brief Whether the constraint is unsatisfiable.
   *
   * \throw std::invalid_argument if no valid linear expression has been defined.
   */

  bool is_unsat() const;
  
  /*! \brief Whether the underlying linear expression has only scalar coefficients.
   *
   * \throw std::invalid_argument if no valid linear expression has been defined.
   */
  bool is_linear() const;

  /*! \brief Whether the underlying linear expression has only scalar coefficients, 
   * except maybe for the constant one.
   *
   * \throw std::invalid_argument if no valid linear expression has been defined.
   */
  bool is_quasilinear() const;

  // TODO: equal, compare (currently not in ap_lincons1.h) ???

  //@}


  /* TODO: evaluation, linearization, intelligent constructors */


  /* C-level compatibility */
  /* ===================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object stored in *this.
  const ap_lincons1_t* get_ap_lincons1_t() const;

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_lincons1_t* get_ap_lincons1_t();

  //@}
};



/* ================================= */
/* lincons1_array                    */
/* ================================= */


/*! \brief Array of linear constraints (ap_lincons1_array_t wrapper).
 *
 * Level 1 version of linear constraint arrays.
 * Internally, a lincons1_array wraps together a lincons0_array (memory managed) and an environment 
 * (holding a reference count).
 * Please note that all constraints share the same environment!
 */
class lincons1_array : public use_malloc {

protected:

  ap_lincons1_array_t a;  //!< Structure managed by APRON.

  //! Internal use only. Shallow copy (no copy of lincons0_array or environment).
  lincons1_array(ap_lincons1_array_t& a);

  friend class abstract1;

public:
  
  /* constructors */
  /* ============ */

  /** @name Constructors */
  //@{

  /*! \brief Creates a new constraint array from a lincons0_array (copied) and an environment (reference count
   * incremented) associating names to dimensions.
   */
  lincons1_array(const environment& e, const lincons0_array& x);

  /*! \brief Creates a new array of the given size containing uninitialized constraints.
   *
   * has_modulo and has_linexpr will return false on all elements of the array.
   * \arg \c e associates a variable name to each dimension (reference count incremented).
   */
  lincons1_array(const environment& e, size_t size);

  //! (Deep) copy.
  lincons1_array(const lincons1_array& x);

  /*! \brief Makes a (deep) copy of x and extends its environment.
   *
   * \throw std::invalid_argument if e is not a super-environment of that of x.
   */
  lincons1_array(const lincons1_array& x, const environment& e);

  /*! \brief Creates a lincons1_array from an array (of size >0) of constraints of the given size (copied).
   *
   * \warning assumes that all constraints have the same environment (unchecked).
   * \throw std::invalid_argument if size<1.
   */
  lincons1_array(size_t size, const lincons1 x[]);

  /*! \brief Creates a lincons1_array from an vector (of size >0) of constraints of the given size (copied).
   *
   * \warning assumes that all constraints have the same environment (unchecked).
   * \throw std::invalid_argument if vector size<1.
   */
  lincons1_array(const std::vector<lincons1>& x);

  //@}


  /* destructor */
  /* ========== */

  /** @name Destructor */
  //@{

  /*! \brief Frees the space used by the array and all its constraints, and decrements the 
   * reference count of the environment.
   */
  ~lincons1_array();

  //@}


  /* assignment */
  /* ========== */

  /** @name Assignments */
  //@{

  //! (Deep) copy.
  lincons1_array& operator= (const lincons1_array& x);

  /*! \brief Copies the constraints from the array into *this.
   *
   * \warning assumes that all constraints have the same environment (unchecked).
   * \arg x should contain (at least) size elements.
   */
  lincons1_array& operator= (const lincons1 x[]);

  /*! Copies the constraints from the vector into the array, changing its size if needed.
   *
   * \warning assumes that all constraints have the same environment (unchecked).
   */
  lincons1_array& operator= (const std::vector<lincons1>& x);

  //@}


  /* dimension operations */
  /* ==================== */

  /** @name Dimension operations */
  //@{

  //! Resizes the array.
  void resize(size_t size);

  /*! \brief Extends the environment of all expressions in array.
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

  //! Returns the environment shared by all constraints (with incremented reference count).
  environment get_environment() const;

  //! Returns a reference to the underlying lincons0_array.
  const lincons0_array& get_lincons0_array() const;

  //! Returns a (modifiable) reference to the underlying lincons0_array.
  lincons0_array& get_lincons0_array();

  /*! \brief Returns a copy of the constraint at index i.
   *
   * \throw std::out_of_range if i exceeds the dimension of the array.
   */
  lincons1 get(size_t i) const;

  /*! \brief Changes the constraint at index i.
   *
   * \warning assumes that x and *this have equal environments.
   * \throw std::out_of_range if i exceeds the dimension of the array.
   */
  void set(size_t i, const lincons1& x);

  //@}


  /* conversion */
  /* ========== */
  
  /** @name Conversion */
  //@{

  //! Returns a copy of the linear constraints in the form of a vector.
  operator std::vector<lincons1>() const;

  //@}


  /* print */
  /* ===== */

  /** @name Printing */
  //@{

  /*! \brief Printing.
   *
   * \throw std::invalid_argument an underlying expression is missing, or an
   * auxiliary scalar is missing (for modulo constraint).
   */
  friend std::ostream& operator<< (std::ostream& os, const lincons1_array& s);

  //! Prints to a C stream.
  void print(FILE* stream=stdout) const;

  //@}


  /* C-level compatibility */
  /* ===================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object stored in *this.
  const ap_lincons1_array_t* get_ap_lincons1_array_t() const;

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_lincons1_array_t* get_ap_lincons1_array_t();

  //@}

};


#include "apxx_lincons1_inline.hh"

}

#endif /* __APXX_LINCONS1_HH */
