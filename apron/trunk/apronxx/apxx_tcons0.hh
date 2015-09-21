/* -*- C++ -*-
 * apxx_tcons0.hh
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

#ifndef __APXX_TCONS0_HH
#define __APXX_TCONS0_HH

#include "ap_tcons0.h"
#include "apxx_texpr0.hh"
#include "apxx_lincons0.hh"


namespace apron {

/* ================================= */
/* tcons0                            */
/* ================================= */


/*! \brief Level 0 arbitrary constraint (ap_tcons0_t wrapper).
 *
 * A tcons0 represents a constraint of the form expr==0, expr>=0, expr>0, expr!=0, or expr==0 mod c
 * where expr is an arbitrary expression tree.
 * It stores and manages a texpr0, a constraint type (==, >=, >, !=, mod), and (for modulo constraint) 
 * an extra scalar (c).
 *
 * Constraints can be constructed using the >=, <=, >, <, ==, != operators on expression trees, or
 * dedicated constructors:
 * \code 
 * tcons0 x = dim(1) >= dim(2) * 5;
 * tcons0 y = add(sqrt(dim(1),AP_RTYPE_FLOAT), dim(0), AP_RTYPE_FLOAT) != 10;
 * tcons0 y = tcons0(AP_CONS_EQMOD, 3*dim(0)+2*dim(1), 5);
 * \endcode
 * See the documentation of the texpr0 class for more information on constructing expression trees.
 */
class tcons0 : public use_malloc {

protected:
  
  ap_tcons0_t l;  //!< Structure managed by APRON.

  //! Internal use only. Performs a shallow copy and takes ownership of the contents.
  tcons0(ap_tcons0_t& l);

  friend class abstract0;
  
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
  tcons0(ap_constyp_t constyp=AP_CONS_SUPEQ);

  /*! \brief Creates a new (non-modulo) constraint from an expression tree (copied).
   *
   * The extra scalar is not created (has_modulo returns false).
   * \arg \c constyp can be \c AP_CONS_EQ, \c AP_CONS_SUPEQ, \c AP_CONS_SUP, or \c AP_CONS_DISEQ (but not \c AP_CONS_EQMOD).
   */
  tcons0(ap_constyp_t constyp, const texpr0::builder& t);

  /*! \brief Creates a new constraint from an expression tree and a modulo scalar (both copied).
   *
   * \arg \c constyp can be \c AP_CONS_EQ, \c AP_CONS_SUPEQ, \c AP_CONS_SUP, \c AP_CONS_EQMOD, or \c AP_CONS_DISEQ.
   */
  tcons0(ap_constyp_t constyp, const texpr0::builder& t, const scalar& modulo);

  //! (Deep) copy of a constraint.
  tcons0(const tcons0& x);

  /*! \brief Makes a (deep) copy of a constraint, and applies a dimension change to the underlying expression.
   *
   * \arg \c add whether to add or remove dimensions.
   */
  tcons0(const tcons0& x, const dimchange& d, bool add=true);

  //! Makes a (deep) copy of a constraint, and applies a permutation to the underlying expression.
  tcons0(const tcons0& x, const dimperm& d);

  //! Makes an unsatisfiable constraint (-1>=0).
  tcons0(unsat x);

  /*! \brief Makes a constraint from a linear constraint (copying coefficients).
   *
   * Real-valued addition and multiplication operators are used (i.e., no rounding).
   */
  tcons0(const lincons0& x);


  //@}

  /* destructor */
  /* ========== */

  /** @name Destructor */
  //@{

  //! Frees the constraint, including the embedded expression tree and optional modulo scalar.
  ~tcons0();

  //@}

  /* 'Intelligent' constructors */
  /* ========================== */

  /** @name 'Intelligent' constructors */
  //@{

  //! Makes a constraint a-b >= 0.
  friend tcons0 operator>=(const texpr0::builder& a, const texpr0::builder& b);

  //! Makes a constraint b-a >= 0.
  friend tcons0 operator<=(const texpr0::builder& a, const texpr0::builder& b);

  //! Makes a constraint a-b > 0.
  friend tcons0 operator> (const texpr0::builder& a, const texpr0::builder& b);

  //! Makes a constraint b-a > 0.
  friend tcons0 operator< (const texpr0::builder& a, const texpr0::builder& b);

  //! Makes a constraint a-b == 0.
  friend tcons0 operator==(const texpr0::builder& a, const texpr0::builder& b);

  //! Makes a constraint a-b != 0.
  friend tcons0 operator!=(const texpr0::builder& a, const texpr0::builder& b);
  
  //@}

  /* assignment */
  /* ========== */

  /** @name Assignments */
  //@{

  //! (Deep) copy.
  tcons0& operator= (const tcons0& x);

  //! Assigns an unsatisfiable constraint to *this (-1>=0).
  tcons0& operator= (unsat x);

  /*! \brief Makes *this equal to the linear constraint x (coefficients are copied), deleting the previous value of *this.
   *
   * Real-valued addition and multiplication operators are used (i.e., no rounding).
   */
  tcons0& operator= (const lincons0& x);

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
  void set_texpr(const texpr0::builder& c);

  //@}


  /* dimension operations */
  /* ==================== */

  /** @name Dimension operations */
  //@{

  /*! \brief Adds dimensions to the underlying expression tree.
   *
   * \throw std::invalid_argument if no valid expression tree has been defined.
   */
  void add_dimensions(const dimchange& d);

  /*! \brief Removes dimensions to the underlying expression tree.
   *
   * \throw std::invalid_argument if no valid expression tree has been defined.
   */
  void remove_dimensions(const dimchange& d);

  /*! \brief Applies a permutation to the underlying expression tree.
   *
   * \throw std::invalid_argument if no valid expression tree has been defined.
   */
  void permute_dimensions(const dimperm& d);

  //@}


  /* access */
  /* ====== */

  /** @name Accesses */
  //@{

  /* get */
 
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
  texpr0::iterator get_texpr();
 
  /*! \brief Returns a const_iterator to the root of the underlying expression tree.
   *
   * \throw std::invalid_argument if no valid expression tree has been defined.
   */
  texpr0::const_iterator get_texpr() const;
 
  //@}


  /* print */
  /* ===== */

  /** @name Printing */
  //@{

  /*! \brief Printing.
   *
   * Variable naming can be configured through the varname stream modifier.
   *
   * \throw std::invalid_argument if the underlying expression is missing, or the
   * extra scalar is missing (for modulo).
   */
  friend std::ostream& operator<< (std::ostream& os, const tcons0& s);

  //! Prints to a C stream.
  void print(char** name_of_dim=NULL, FILE* stream=stdout) const;

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
  const ap_tcons0_t* get_ap_tcons0_t() const;

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_tcons0_t* get_ap_tcons0_t();
 
  //@}

};



/* ================================= */
/* tcons0_array                    */
/* ================================= */


/*! \brief Array of arbitrary constraints (ap_tcons0_array_t wrapper).
 *
 * A tcons0_array represents an array of constraints on arbitrary expressions.
 */
class tcons0_array : public use_malloc {

protected:

  ap_tcons0_array_t a;  //!< Structure managed by APRON.

  //! Internal use only. Performs a shallow copy and takes ownership of the contents.
  tcons0_array(ap_tcons0_array_t& a) : a(a) {}

  friend class abstract0;
  friend class tcons1_array;

public:
  
  /* constructors */
  /* ============ */

  /** @name Constructors */
  //@{

  /*! \brief Creates a new array of the given size containing uninitialized constraints.
   *
   * has_modulo and has_texpr will return false on all elements of the array.
   */
  tcons0_array(size_t size);

  //! (Deep) copy.
  tcons0_array(const tcons0_array& x);

  //! Makes a (deep) copy of the array and applies add_dimensions to all constraints.
  tcons0_array(const tcons0_array& x, const dimchange& d, bool add=true);

  //! Makes a (deep) copy of the array and applies permute_dimensions to all constraints.
  tcons0_array(const tcons0_array& x, const dimperm& d);

  //! Creates a tcons0_array from an array of constraints of the given size (copied).
  tcons0_array(size_t size, const tcons0 x[]);

  //! Creates a tcons0_array from a vector of constraints (copied).
  tcons0_array(const std::vector<tcons0>& x);

  /*! \brief Makes a constraint array from a linear constraint array (copying coefficients).
   *
   * Real-valued addition and multiplication operators are used (i.e., no rounding).
   */
  tcons0_array(const lincons0_array& x);

  //@}


  /* destructor */
  /* ========== */

  /** @name Destructor */
  //@{

  //! Frees the space used by the array and all its constraints.
  ~tcons0_array();

  //@}


  /* assignment */
  /* ========== */

  /** @name Assignments */
  //@{

  //! (Deep) copy.
  tcons0_array& operator= (const tcons0_array& x);

  /*! \brief Copies the constraints from the array into *this.
   *
   * \arg x should contain (at least) size elements.
   */
  tcons0_array& operator= (const tcons0 x[]);

  //! Copies the constraints from the vector into the array, changing its size if needed.
  tcons0_array& operator= (const std::vector<tcons0>& x);

  /*! \brief Makes *this equal to the linear constraint array x (coefficients are copied), deleting the previous value of *this.
   *
   * Real-valued addition and multiplication operators are used (i.e., no rounding).
   */
  tcons0_array& operator= (const lincons0_array& x);

  //@}


  /* dimension operations */
  /* ==================== */

  /** @name Dimension operations */
  //@{

  //! Resizes the array.
  void resize(size_t size);

  //! Applies add_dimensions to all constraints in the array.
  void add_dimensions(const dimchange& d);

  //! Applies remove_dimensions to all constraints in the array.
  void remove_dimensions(const dimchange& d);

  //! Applies permute_dimensions to all constraints in the array.
  void permute_dimensions(const dimperm& d);

  //@}


  /* access */
  /* ====== */

  /** @name Accesses */
  //@{

  //! Returns the size of the array.
  size_t size() const;

  //! Returns a pointer to the start of the internal array holding the constraints. 
  tcons0* contents();

  //! Returns a pointer to the start of the internal array holding the constraints. 
  const tcons0* contents() const;
  
  //! Returns a (modifiable) reference to an element, no bound checking.
  tcons0& operator[](size_t i);

  //! Returns a reference to an element, no bound checking.
  const tcons0& operator[](size_t i) const;

  /*! \brief Returns a (modifiable) reference to an element (bound-checked).
   *
   * \throw std::out_of_range if the index is invalid.
   */
  tcons0& get(size_t i);

  /*! \brief Returns a reference to an element (bound-checked).
   *
   * \throw std::out_of_range if the index is invalid.
   */
  const tcons0& get(size_t i) const;

  //@}


  /* conversion */
  /* ========== */
  
  /** @name Conversion */
  //@{

  //! Returns a copy of the constraints in the form of a vector.
  operator std::vector<tcons0>() const;

  //@}


  /* print */
  /* ===== */

  /** @name Printing */
  //@{

  
  /*! \brief Printing.
   *
   * Variable naming can be configured through the varname stream modifier.
   *
   * \throw std::invalid_argument an underlying expression is missing, or an
   * extra scalar is missing (for modulo constraint).
   */
  friend std::ostream& operator<< (std::ostream& os, const tcons0_array& s);

  //! Prints to a C stream.
  void print(char** name_of_dim = NULL, FILE* stream=stdout) const;

  //@}


  /* tests */
  /* ===== */

  /** @name Tests */
  //@{

  //! Whether all constraints are linear.
  bool is_interval_linear() const;

  //@}


  /* C-level compatibility */
  /* ===================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object stored in *this.
  const ap_tcons0_array_t* get_ap_tcons0_array_t() const;

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_tcons0_array_t* get_ap_tcons0_array_t();

  //@}
};

#include "apxx_tcons0_inline.hh"

}

#endif /* __APXX_TCONS0_HH */
