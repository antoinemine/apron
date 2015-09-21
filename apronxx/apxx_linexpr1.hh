/* -*- C++ -*-
 * apxx_linexpr1.hh
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

#ifndef __APXX_LINEXPR1_HH
#define __APXX_LINEXPR1_HH

#include "ap_linexpr1.h"
#include "apxx_environment.hh"
#include "apxx_linexpr0.hh"


namespace apron {


/* ================================= */
/* linexpr1                          */
/* ================================= */


/*! \brief Level 1 linear expression (ap_linexpr1_t wrapper).
 *
 * Level 1 version of linear expressions with scalar or interval coefficients (coeff).
 * Terms are indexed using variable names (var) instead of dimensions (ap_dim_t).
 * Internally, a linexpr1 wraps together a linexpr0 (memory managed) and an environment (holding a
 * reference count).
 */
class linexpr1 : public use_malloc {

protected:
  
  ap_linexpr1_t l; //!<  Structure managed by APRON.

  //! Internal use only. Shallow copy (no copy of linexpr0 or environment).
  linexpr1(ap_linexpr1_t& p);


public:


  /* constructors */
  /* ============ */

  /** @name Constructors */
  //@{

  /*! \brief Creates a new expression from a linexpr0 (copied) and an environment (reference count
   * incremented) associating names to dimensions in linexpr0.
   */
  linexpr1(const environment& e, const linexpr0& x);

  /*! \brief Creates a new null expression.
   *
   * \arg \c e associates a variable name to each dimension.
   * \arg \c discr whether the expression is sparse (\c AP_LINEXPR_SPARSE) or dense (\c AP_LINEXPR_DENSE).
   * \arg \c size, for \c AP_LINEXPR_SPARSE expressions, is the initial number of dimensions in the
   * underlying linexpr0 (new dimensions being added as needed); ignored for \c AP_LINEXPR_DENSE
   * expressions (the size being always the size of the environment).
   */
  linexpr1(const environment& e, ap_linexpr_discr_t lin_discr=AP_LINEXPR_SPARSE, size_t size=0);
  
  //! Makes a (deep) copy.
  linexpr1(const linexpr1& x);

  /*! \brief Makes a (deep) copy of x and extends its environment.
   *
   * \throw std::invalid_argument if e is not a super-environment of that of x.
   */
  linexpr1(const linexpr1& x, const environment& e);

  //@}


  /* destructor */
  /* ========== */

  /** @name Destructor */
  //@{

  /*! \brief Frees all space for the expression and coefficients, and decrements the reference count 
   * of the environment.
   */  
  ~linexpr1();

  //@}


  /* assignment */
  /* ========== */

  /** @name Assignment */
  //@{

  //! Makes a (deep) copy.
  linexpr1& operator= (const linexpr1& x);

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

  //! Returns the environment of the expression (with incremented reference count).
  environment get_environment() const;

  //! Returns a reference to the underlying linexpr0.
  const linexpr0& get_linexpr0() const;

  //! Returns a modifiable reference to the underlying linexpr0.
  linexpr0& get_linexpr0();

  //! Returns the number of coefficients in the expression.
  size_t size() const;

  /*! \brief Returns the type of underlying linexpr0.
   *
   * \return Either \c AP_LINEXPR_SPARSE or \c AP_LINEXPR_DENSE.
   */
  ap_linexpr_discr_t get_discr() const;

  //! Returns a (modifiable) reference to the constant coefficient.
  coeff& get_cst();

  //! Returns a reference to the constant coefficient.
  const coeff& get_cst() const;

  /*! \brief Returns a (modifiable) reference to the coefficient corresponding to the given variable name.
   *
   * \throw std::invalid_argument if the variable name is not present in the environment.
   */
  coeff& operator[](const var& v);
  
  /*! \brief Returns a reference to the coefficient corresponding to the given variable name.
   *
   * \throw std::invalid_argument if the variable name is not present in the environment.
   */
  const coeff& operator[](const var& v) const;

  //@}


  /* print */
  /* ===== */

  /** @name Printing */
  //@{

  //! Printing.
  friend std::ostream& operator<< (std::ostream& os, const linexpr1& s);

  //! Prints to a C stream.
  void print(FILE* stream=stdout) const;

  //@}


  /* tests */
  /* ===== */

  /** @name Tests */
  //@{

  //! Whether the expression only depends on integer variables
  bool is_integer() const;

  //! Whether the expression only depends on real variables
  bool is_real() const;

  /*! \brief Gets the type of the linear expression.
   *
   * \return Either \c AP_LINEXPR_INTLINEAR (coefficients are interval), 
   * \c AP_LINEXPR_QUASILINEAR (coefficients are scalar except the constant one), or 
   * \c AP_LINEXPR_LINEAR (all coefficients are scalar).
   */
  ap_linexpr_type_t get_type() const;

  //! Whether all coefficients are scalar.
  bool is_linear() const;

  //! Whether all coefficients are scalar, except maybe the constant one.
  bool is_quasilinear() const;


  // TODO: equal, compare (currently not in ap_linexpr1.h) ???


  //@}
 
  /* iterators */
  /* ========= */

  /** @name Iterator */
  //@{

 /*! \brief Iterator to traverse a constant linexpr1.
   *
   * At each position, there is variable name and its corresponding coefficients.
   * The expression is traversed in increasing order of integer variable names followed by
   * real variable names also in increasing order.
   *
   * To mutate a linear expression, use the iterator class instead.
   *
   * Sample code:
   * \code for (linexpr1::const_iterator i = m.begin();i.valid();++i) cout << i.get_coeff() << " "; \endcode
   */
  class const_iterator {

    /* TODO:
       - reverse traversal (operator--).
     */

    friend class linexpr1;
    
  protected:

    ap_linexpr1_t* l; //!< Internal use only. Pointer to the underlying APRON structure.
    ap_dim_t pos;     //!< Internal use only. Current index.

    //! Internal use only. Skips free coefficients in sparse expressions.
    void skip_AP_DIM_MAX();

    //! Internal use only.
    const_iterator(ap_linexpr1_t* l);

  public:
    
    //! Starts a new iterator to traverse the linear expression.
    const_iterator(const linexpr1& e);

    //! Duplicates the iterator.
    const_iterator(const const_iterator& i);

    //! Assigns the iterator.
    const_iterator& operator=(const const_iterator& i);

    /*! \brief Returns the variable name of the coefficient at the current iterator position.
     *
     * \throw std::out_of_range if valid() returns false (we are past the last position).
     */
    const var& get_var() const;

    /*! \brief Returns the dimension of the coefficient at the current iterator position.
     *
     * \throw std::out_of_range if valid() returns false (we are past the last position).
     */
    ap_dim_t get_dim() const;

    /*! \brief Returns a reference to the coefficient at the current iterator position.
     *
     * \throw std::out_of_range if valid() returns false (we are past the last position).
     */
    const coeff& get_coeff() const;

    //! Moves the iterator to the following position.
    void next();

    /*! \brief Moves the iterator to the following position.
     *
     * (Identical to next()).
     */
    void operator++();

    //! Whether we are at a valid position (true) or past the last iterator position (false).
    bool valid() const;

  };

  /*! \brief Iterator to traverse and mutate a linear expression.
   *
   * As const_iterator, but for non-constant linexpr1.
   *
   * Caution: do not modify the expression during traversal, except through the iterator.
   *
   * Sample code:
   * \code for (linexpr1::iterator i = m.begin();i.valid();++i) i.get_coeff().neg(); \endcode
   */
  class iterator : public const_iterator {

    /* TODO:
       - removing/inserting coefficients at iterator position for sparse expressions.
     */

  protected:
    
    friend class linexpr1;
    
    //! Internal use only.
    iterator(ap_linexpr1_t* l);

  public:
   
    //! Starts a new iterator to traverse the linear expression.
    iterator(linexpr1& e);

    //! Duplicates the iterator.
    iterator(const iterator& i);

    //! Assigns the iterator.
    iterator& operator=(const iterator& i);

    /*! \brief Returns a (modifiable) reference to the coefficient at the current iterator position.
     *
     * \throw std::out_of_range if valid() returns false (we are past the last position).
     */
    coeff& get_coeff() const;

  };

  //! Returns a new iterator to traverse and mutate the linear expression.
  iterator begin();
  
  //! Returns a new constant iterator to traverse the linear expression.
  const_iterator begin() const;
  
  //@}


  /* other operators */
  /* =============== */

  /** @name Operators */
  //@{

  /*! \brief Minimizes all coefficients.
   * 
   * In sparse expressions, also remove zero and unused (hole) coefficients to save space.
   */
  void minimize();

  //@}


  /* TODO: evaluation, linearization, intelligent constructors */


  /* C-level compatibility */
  /* ===================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object stored in *this.
  const ap_linexpr1_t* get_ap_linexpr1_t() const;

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_linexpr1_t* get_ap_linexpr1_t();

  //@}

};

#include "apxx_linexpr1_inline.hh"

}

#endif /* __APXX_LINEXPR1_HH */
