/* -*- C++ -*-
 * apxx_tcons1.hh
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

#ifndef __APXX_TCONS1_HH
#define __APXX_TCONS1_HH

#include "ap_tcons1.h"
#include "apxx_tcons0.hh"
#include "apxx_texpr1.hh"


namespace apron {


/* ================================= */
/* tcons1                            */
/* ================================= */


/*! \brief Level 1 arbitrary constraint (ap_tcons1_t wrapper).
 *
 * Level 1 version of arbitrary constraints using expression trees.
 * Variable names (var) are used instead of dimensions (ap_dim_t).
 * Internally, a tcons1 wraps together a tcons0 (memory managed) and an environment (holding a
 * reference count).
 */
class tcons1 : public use_malloc {

protected:
  
  ap_tcons1_t l;  //!< Structure managed by APRON.

  //! Internal use only. Performs a shallow copy and takes ownership of the contents.
  tcons1(ap_tcons1_t l);

  friend class abstract0;
  friend class tcons1_array;

public:

  
  /* constructors */
  /* ============ */

  /** @name Constructors */
  //@{

  /*! \brief Creates an empty constraint.
   *
   * Neither the expression tree nor the extra scalar are not created
   * (has_texpr and has_modulo both return false).
   * \arg \c constyp can be \c AP_CONS_EQ, \c AP_CONS_SUPEQ, \c AP_CONS_SUP, or \c AP_CONS_DISEQ (but not \c AP_CONS_EQMOD).
   */
  tcons1(const environment& env, ap_constyp_t constyp=AP_CONS_SUPEQ);

  /*! \brief Creates a new (non-modulo) constraint from an expression tree (copied).
   *
   * The extra scalar is not created (has_modulo returns false).
   * \arg \c constyp can be \c AP_CONS_EQ, \c AP_CONS_SUPEQ, \c AP_CONS_SUP, or \c AP_CONS_DISEQ (but not \c AP_CONS_EQMOD).
   */
  tcons1(ap_constyp_t constyp, const texpr1::builder& t);

  /*! \brief Creates a new constraint from an expression tree and a modulo scalar (both copied).
   *
   * \arg \c constyp can be \c AP_CONS_EQ, \c AP_CONS_SUPEQ, \c AP_CONS_SUP, \c AP_CONS_EQMOD, or \c AP_CONS_DISEQ.
   */
  tcons1(ap_constyp_t constyp, const texpr1::builder& t, const scalar& modulo);

  //! (Deep) copy of a constraint.
  tcons1(const tcons1& x);


  /*! \brief Makes a (deep) copy of x and extends its environment.
   *
   * \throw std::invalid_argument if e is not a super-environment of that of x.
   */
  tcons1(const tcons1& x, const environment& e);

  //! Makes an unsatisfiable constraint (-1>=0).
  tcons1(const environment& e, unsat x);

  /*! \brief Makes a constraint from a linear constraint (copying coefficients).
   *
   * Real-valued addition and multiplication operators are used (i.e., no rounding).
   */
  tcons1(const lincons1& x);


  //@}

  /* destructor */
  /* ========== */

  /** @name Destructor */
  //@{

  //! Frees the constraint, including the embedded expression tree and optional modulo scalar.
  ~tcons1();

  //@}

  /* 'Intelligent' constructors */
  /* ========================== */

  /** @name 'Intelligent' constructors */
  //@{

  //! Makes a constraint a-b >= 0.
  friend tcons1 operator>=(const texpr1::builder& a, const texpr1::builder& b);

  //! Makes a constraint b-a >= 0.
  friend tcons1 operator<=(const texpr1::builder& a, const texpr1::builder& b);

  //! Makes a constraint a-b > 0.
  friend tcons1 operator> (const texpr1::builder& a, const texpr1::builder& b);

  //! Makes a constraint b-a > 0.
  friend tcons1 operator< (const texpr1::builder& a, const texpr1::builder& b);

  //! Makes a constraint a-b == 0.
  friend tcons1 operator==(const texpr1::builder& a, const texpr1::builder& b);

  //! Makes a constraint a-b != 0.
  friend tcons1 operator!=(const texpr1::builder& a, const texpr1::builder& b);
  
  //@}

  /* assignment */
  /* ========== */

  /** @name Assignments */
  //@{

  //! (Deep) copy.
  tcons1& operator= (const tcons1& x);

  //! Assigns an unsatisfiable constraint to *this (-1>=0).
  tcons1& operator= (unsat x);

  /*! \brief Makes *this equal to the linear constraint x (coefficients are copied), deleting the previous value of *this.
   *
   * Real-valued addition and multiplication operators are used (i.e., no rounding).
   */
  tcons1& operator= (const lincons1& x);

  /*! \brief Sets the extra scalar modulo to c (copied).
   *
   * Does not fail as get_modulo can: if the constraint was created without an extra scalar, 
   * it is created.
   */
  void set_modulo(const scalar& c);  

  /*! \brief Sets the underlying expression tree to c (copied).
   *
   * Does not fail as get_texpr can: if the constraint was created without an underlying expression, 
   * it is created.
   */
  void set_texpr(const texpr1::builder& c);

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

  /** @name Accesses */
  //@{

  /* get */
 
  //! Returns the environment of the expression (with incremented reference count).
  environment get_environment() const;

  //! Returns a reference to the underlying tcons0.
  const tcons0& get_tcons0() const;

  //! Returns a (modifiable) reference to the underlying tcons0.
  tcons0& get_tcons0();

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

  //! Returns whether the constraint has a valid extra scalar (used in modulo constraints).
  bool has_modulo() const;

  //! \brief Whether the constraint contains a valid expression tree.
  bool has_texpr() const;

  /*! \brief Returns a (modifiable) reference to the extra scalar.
   *
   * \throw std::invalid_argument if no valid extra scalar has been defined.
   */
  scalar& get_modulo();
 
  /*! \brief Returns a reference to the extra scalar.
   *
   * \throw std::invalid_argument if no valid extra scalar has been defined.
   */
  const scalar& get_modulo() const;

  /*! \brief Returns an iterator to the root of the underlying expression tree.
   *
   * \throw std::invalid_argument if no valid expression tree has been defined.
   */
  texpr1::iterator get_texpr();
 
  /*! \brief Returns a const_iterator to the root of the underlying expression tree.
   *
   * \throw std::invalid_argument if no valid expression tree has been defined.
   */
  texpr1::const_iterator get_texpr() const;
 
  //@}


  /* print */
  /* ===== */

  /** @name Printing */
  //@{

  /*! \brief Printing.
   *
   * \throw std::invalid_argument if the underlying expression is missing, or the
   * extra scalar is missing (for modulo).
   */
  friend std::ostream& operator<< (std::ostream& os, const tcons1& s);

  //! Prints to a C stream.
  void print(FILE* stream=stdout) const;

  //@}
 
  /* tests */
  /* ===== */

  /** @name Tests */
  //@{

  /*! \brief Whether the expression is constant (i.e., has no dimension leaves).
   *
   * \throw std::invalid_argument if no valid expression tree has been defined.
   */
  bool is_interval_cst() const;

  /*!\brief  Whether the expression is linear and there is no rounding.
   *
   * \throw std::invalid_argument if no valid expression tree has been defined.
   */
  bool is_interval_linear() const;

  /*! \brief Whether the expression is polynomial and there is no rounding.
   *
   * \throw std::invalid_argument if no valid expression tree has been defined.
   */
  bool is_interval_polynomial() const;

  /*! \brief Whether the expression is a polynomial fraction and there is no rounding.
   *
   * \throw std::invalid_argument if no valid expression tree has been defined.
   */
  bool is_interval_polyfrac() const;

  /*! \brief Whether all occurring constants are scalar.
   *
   * \throw std::invalid_argument if no valid expression tree has been defined.
   */
  bool is_scalar() const;

  //@}


  /* C-level compatibility */
  /* ===================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object stored in *this.
  const ap_tcons1_t* get_ap_tcons1_t() const;

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_tcons1_t* get_ap_tcons1_t();
 
  //@}

};



/* ================================= */
/* tcons1_array                    */
/* ================================= */


/*! \brief Array of arbitrary constraints (ap_tcons1_array_t wrapper).
 *
 * A tcons1_array represents an array of constraints on arbitrary expressions.
 * An constraints share the same environment.
 */
class tcons1_array : public use_malloc {

protected:

  ap_tcons1_array_t a;  //!< Structure managed by APRON.

  //! Internal use only. Performs a shallow copy and takes ownership of the contents.
  tcons1_array(ap_tcons1_array_t& a);

  friend class abstract0;
  friend class abstract1;

public:
  
  /* constructors */
  /* ============ */

  /** @name Constructors */
  //@{

  /*! \brief Creates a new constraint array from a tcons0_array (copied) and an environment (reference count
   * incremented) associating names to dimensions.
   */
  tcons1_array(const environment& e, const tcons0_array& x);

  /*! \brief Creates a new array of the given size containing uninitialized constraints.
   *
   * has_modulo and has_texpr will return false on all elements of the array.
   */
  tcons1_array(const environment& e, size_t size);

  //! (Deep) copy.
  tcons1_array(const tcons1_array& x);

  /*! \brief Makes a (deep) copy of the array and extends the environment.
   *
   * \throw std::invalid_argument if e is not a super-environment of that of x.
   */
  tcons1_array(const tcons1_array& x, const environment& e);


  /*! \brief Creates a tcons1_array from an array (of size >0) of constraints of the given size (copied).
   *
   * \warning assumes that all constraints have the same environment (unchecked).
   * \throw std::invalid_argument if size<1.
   */
  tcons1_array(size_t size, const tcons1 x[]);

  /*! \brief Creates a tcons1_array from an vector (of size >0) of constraints of the given size (copied).
   *
   * \warning assumes that all constraints have the same environment (unchecked).
   * \throw std::invalid_argument if vector size<1.
   */
  tcons1_array(const std::vector<tcons1>& x);

  //@}


  /* destructor */
  /* ========== */

  /** @name Destructor */
  //@{

  //! Frees the space used by the array and all its constraints.
  ~tcons1_array();

  //@}


  /* assignment */
  /* ========== */

  /** @name Assignments */
  //@{

  //! (Deep) copy.
  tcons1_array& operator= (const tcons1_array& x);

  /*! \brief Copies the constraints from the array into *this.
   *
   * \warning assumes that all constraints have the same environment (unchecked).
   * \arg x should contain (at least) size elements.
   */
  tcons1_array& operator= (const tcons1 x[]);

  /*! Copies the constraints from the vector into the array, changing its size if needed.
   *
   * \warning assumes that all constraints have the same environment (unchecked).
   */
  tcons1_array& operator= (const std::vector<tcons1>& x);

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

  //! Returns a reference to the underlying tcons0_array.
  const tcons0_array& get_tcons0_array() const;

  //! Returns a (modifiable) reference to the underlying tcons0_array.
  tcons0_array& get_tcons0_array();

  /*! \brief Returns a copy of the constraint at index i.
   *
   * \throw std::out_of_range if i exceeds the dimension of the array.
   */
  tcons1 get(size_t i) const;

  /*! \brief Changes the constraint at index i.
   *
   * \warning assumes that x and *this have equal environments.
   * \throw std::out_of_range if i exceeds the dimension of the array.
   */
  void set(size_t i, const tcons1& x);

  //@}


  /* conversion */
  /* ========== */
  
  /** @name Conversion */
  //@{

  //! Returns a copy of the constraints in the form of a vector.
  operator std::vector<tcons1>() const;

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
  friend std::ostream& operator<< (std::ostream& os, const tcons1_array& s);

  //! Prints to a C stream.
  void print(FILE* stream=stdout) const;

  //@}


  /* C-level compatibility */
  /* ===================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object stored in *this.
  const ap_tcons1_array_t* get_ap_tcons1_array_t() const;

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_tcons1_array_t* get_ap_tcons1_array_t();

  //@}

};

#include "apxx_tcons1_inline.hh"

}

#endif /* __APXX_TCONS1_HH */
