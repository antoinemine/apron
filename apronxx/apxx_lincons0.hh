/* -*- C++ -*-
 * apxx_lincons0.hh
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

#ifndef __APXX_LINCONS0_HH
#define __APXX_LINCONS0_HH

#include <vector>
#include "ap_lincons0.h"
#include "apxx_linexpr0.hh"


namespace apron {


//! Unsatisfiable constraint, to simplify initialisations and assignments.
struct unsat {};



/* ================================= */
/* lincons0                          */
/* ================================= */


/*! \brief Level 0 linear constraint (ap_lincons0_t wrapper).
 *
 * A lincons0 object represents a linear constraint: expr==0, expr>=0, expr>0, expr!=0, or expr==0 mod c.
 * It stores and manages a linexpr0 (linear expression with scalar or interval coefficients), 
 * a constraint type (==, >=, >, !=, mod), and (for modulo constraint) an auxiliary scalar (c).
 */
class lincons0 : public use_malloc {

protected:
  
  ap_lincons0_t l;  //!< Structure managed by APRON.

  //! Internal use only. Performs a shallow copy and takes ownership of the contents.
  lincons0(ap_lincons0_t l);

  friend class abstract0;
  
public:

  
  /* constructors */
  /* ============ */

  /** @name Construtors */
  //@{

  /*! \brief Creates a new (non-modulo) constraint from an empty linear expression.
   *
   * The auxiliary scalar is not created (has_modulo returns false).
   * The linear expression is created sparse and empty (has_linexpr returns true).
   * \arg \c constyp can be \c AP_CONS_EQ, \c AP_CONS_SUPEQ, \c AP_CONS_SUP, or \c AP_CONS_DISEQ (but not \c AP_CONS_EQMOD).
   */
  lincons0(ap_constyp_t constyp=AP_CONS_SUPEQ);

  /*! \brief Creates a new (non-modulo) constraint from a linear expression (copied).
   *
   * The auxiliary scalar is not created (has_modulo returns false).
   * \arg \c constyp can be \c AP_CONS_EQ, \c AP_CONS_SUPEQ, \c AP_CONS_SUP, or \c AP_CONS_DISEQ (but not \c AP_CONS_EQMOD).
   */
  lincons0(ap_constyp_t constyp, const linexpr0& lin);

  /*! \brief Creates a new constraint from a linear expression and a modulo scalar (both copied).
   *
   * \arg \c constyp can be \c AP_CONS_EQ, \c AP_CONS_SUPEQ, \c AP_CONS_SUP, \c AP_CONS_EQMOD, or \c AP_CONS_DISEQ.
   */
  lincons0(ap_constyp_t constyp, const linexpr0& lin, const scalar& modulo);

  //! (Deep) copy of a constraint.
  lincons0(const lincons0& x);

  //! Makes a (deep) copy of a constraint, and applies a dimension change to the underlying linear expression.
  lincons0(const lincons0& x, const dimchange& d);

  //! Makes a (deep) copy of a constraint, and applies a permutation to the underlying linear expression.
  lincons0(const lincons0& x, const dimperm& d);

  //! Makes an unsatisfiable constraint (-1>=0).
  lincons0(unsat x);

  //@}


  /* destructor */
  /* ========== */

  /** @name Destructor */
  //@{

  //! Frees the constraint, including the embedded linear expression and optional modulo scalar.
  ~lincons0();

  //@}
    

  /* assignment */
  /* ========== */

  /** @name Assignments */
  //@{

  //! (Deep) copy.
  lincons0& operator= (const lincons0& x);

  //! Assigns an unsatisfiable constraint to *this (-1>=0).
  lincons0& operator= (unsat x);

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
   */
  void set_linexpr(const linexpr0& c);

  //@}


  /* dimension operations */
  /* ==================== */

  /** @name Dimension operations */
  //@{

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

  //@}


  /* access */
  /* ====== */

  /** @name Accesses, size */
  //@{

  /* size */

  /*! \brief Returns the size of the underlying linear expression.
   *
   * \throw std::invalid_argument if no valid linear expression has been defined.
   */
  size_t size() const;


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

  //! Whether the constraint has a valid auxiliary scalar (used in modulo constraints).
  bool has_modulo() const;

  /*! \brief Whether the constraint has a valid linear expression.
   *
   * \note The only way the linear expression may be invalid is when accessing fields of uninitialised 
   * (or enlarged) lincons0_array.
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

  /*! \brief Returns a (modifiable) reference to the coefficient corresponding to the given dimension.
   *
   * \throw std::out_of_range if the expression is dense and the dimension exceeds the size
   * of the expression; if the expression is sparse, it will be extended and no exception is thrown
   * \throw std::invalid_argument if no valid linear expression has been defined.
   */
  coeff& operator[](ap_dim_t dim);
  
  /*! \brief Returns a reference to the coefficient corresponding to the given dimension.
   *
   * \throw std::out_of_range if the expression is dense and the dimension exceeds the size
   * of the expression; if the expression is sparse, it will be extended and no exception is thrown
   * \throw std::invalid_argument if no valid linear expression has been defined.
   */
  const coeff& operator[](ap_dim_t dim) const;

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
   * auxiliary scalar is missing (for modulo).
   */
  friend std::ostream& operator<< (std::ostream& os, const lincons0& s);

  //! Prints to a C stream.
  void print(char** name_of_dim=NULL, FILE* stream=stdout) const;

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

  // TODO: equal, compare (currently not in ap_lincons0.h) ???

  //@}

  /* TODO: evaluation, linearization, intelligent constructors */


  /* C-level compatibility */
  /* ===================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object stored in *this.
  const ap_lincons0_t* get_ap_lincons0_t() const;

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_lincons0_t* get_ap_lincons0_t();

  //@}
};



/* ================================= */
/* lincons0_array                    */
/* ================================= */


/*! \brief Array of linear constraints (ap_lincons0_array_t wrapper).
 *
 * A lincons0_array represents an array of linear constraints.
 */
class lincons0_array : public use_malloc {

protected:

  ap_lincons0_array_t a;  //!< Structure managed by APRON.

  //! Internal use only. Performs a shallow copy and takes ownership of the contents.
  lincons0_array(ap_lincons0_array_t& a) : a(a) {}

  friend class lincons1_array;
  friend class abstract0;

public:
  
  /* constructors */
  /* ============ */

  /** @name Constructors */
  //@{

  /*! \brief Creates a new array of the given size containing uninitialized constraints.
   *
   * has_modulo and has_linexpr will return false on all elements of the array.
   */
  lincons0_array(size_t size);

  //! (Deep) copy.
  lincons0_array(const lincons0_array& x);

  //! Makes a (deep) copy of the array and applies add_dimensions to all constraints.
  lincons0_array(const lincons0_array& x, const dimchange& d);

  //! Makes a (deep) copy of the array and applies permute_dimensions to all constraints.
  lincons0_array(const lincons0_array& x, const dimperm& d);

  //! Creates a lincons0_array from an array of constraints of the given size (copied).
  lincons0_array(size_t size, const lincons0 x[]);

  //! Creates a lincons0_array from a vector of constraints (copied).
  lincons0_array(const std::vector<lincons0>& x);

  //@}


  /* destructor */
  /* ========== */

  /** @name Destructor */
  //@{

  //! Frees the space used by the array and all its constraints.
  ~lincons0_array();

  //@}


  /* assignment */
  /* ========== */

  /** @name Assignments */
  //@{

  //! (Deep) copy.
  lincons0_array& operator= (const lincons0_array& x);

  /*! \brief Copies the constraints from the array into *this.
   *
   * \arg x should contain (at least) size elements.
   */
  lincons0_array& operator= (const lincons0 x[]);

  //! Copies the constraints from the vector into the array, changing its size if needed.
  lincons0_array& operator= (const std::vector<lincons0>& x);

  //@}


  /* dimension operations */
  /* ==================== */

  /** @name Dimension operations */
  //@{

  //! Resizes the array.
  void resize(size_t size);

  //! Applies add_dimensions to all constraints in the array.
  void add_dimensions(const dimchange& d);

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
  lincons0* contents();

  //! Returns a pointer to the start of the internal array holding the constraints. 
  const lincons0* contents() const;
  
  //! Returns a (modifiable) reference to an element, no bound checking.
  lincons0& operator[](size_t i);

  //! Returns a reference to an element, no bound checking.
  const lincons0& operator[](size_t i) const;

  /*! \brief Returns a (modifiable) reference to an element (bound-checked).
   *
   * \throw std::out_of_range if the index is invalid.
   */
  lincons0& get(size_t i);

  /*! \brief Returns a reference to an element (bound-checked).
   *
   * \throw std::out_of_range if the index is invalid.
   */
  const lincons0& get(size_t i) const;

  //@}


  /* conversion */
  /* ========== */
  
  /** @name Conversion */
  //@{

  //! Returns a copy of the linear constraints in the form of a vector.
  operator std::vector<lincons0>() const;

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
   * auxiliary scalar is missing (for modulo constraint).
   */
  friend std::ostream& operator<< (std::ostream& os, const lincons0_array& s);

  //! Prints to a C stream.
  void print(char** name_of_dim = NULL, FILE* stream=stdout) const;

  //@}


  /* tests */
  /* ===== */

  /** @name Tests */
  //@{

  //! Whether all constraints are linear.
  bool is_linear() const;

  //! Whether all constraints are quasi-linear.
  bool is_quasilinear() const;

  //@}


  /* C-level compatibility */
  /* ===================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object stored in *this.
  const ap_lincons0_array_t* get_ap_lincons0_array_t() const;

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_lincons0_array_t* get_ap_lincons0_array_t();

  //@}

};

#include "apxx_lincons0_inline.hh"

}

#endif /* __APXX_LINCONS0_HH */
