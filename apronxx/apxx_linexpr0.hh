/* -*- C++ -*-
 * apxx_linexpr0.hh
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

#ifndef __APXX_LINEXPR0_HH
#define __APXX_LINEXPR0_HH

#include "ap_linexpr0.h"
#include "ap_linearize.h"
#include "apxx_coeff.hh"
#include "apxx_dimension.hh"


namespace apron {



/* ================================= */
/* linexpr0                          */
/* ================================= */


/*! \brief Level 0 linear expression (ap_linexpr0_t wrapper).
 *
 * A linexpr0 object represents a linear expression with scalar or interval coefficients (coeff).
 * They have a constant coefficient, plus a coefficient for each represented dimension.
 * An expression can be either sparse (\c AP_LINEXPR_SPARSE) or dense (\c AP_LINEXPR_DENSE).
 * Dense expressions use an array of coefficients to represent dimensions from 0 up to the 
 * expression size.
 * Sparse expressions can have 'holes' between coefficients and grow automatically when 
 * a coefficient for a non-existing dimension is accessed.
 */
class linexpr0 : public use_malloc {

protected:
  
  ap_linexpr0_t l; //!<  Structure managed by APRON.

  //! Internal use only. Shallow copy of structure followed by a free to take ownership of expression.
  linexpr0(ap_linexpr0_t* p);

  friend class texpr0_tmp;
  friend class texpr0;

public:


  /* constructors */
  /* ============ */

  /** @name Constructors */
  //@{

  /*! \brief Creates a new expression.
   *
   * \arg \c discr whether the expression is sparse (\c AP_LINEXPR_SPARSE) or dense (\c AP_LINEXPR_DENSE).
   * \arg \c size is the initial number of dimensions.
   */
  linexpr0(ap_linexpr_discr_t discr=AP_LINEXPR_SPARSE, size_t size=0);

  //! Makes a (deep) copy.
  linexpr0(const linexpr0& x);

  //! Makes a (deep) copy, and adds some dimensions (shifting coefficients if needed).
  linexpr0(const linexpr0& x, const dimchange& d);

  //! Makes a (deep) copy, and applies a permutation on coefficients.
  linexpr0(const linexpr0& x, const dimperm& d);

  /*! \brief Makes a new linear expression with the given size and coefficients (copied).
   *
   * \arg \c discr whether the expression is sparse (\c AP_LINEXPR_SPARSE) or dense (\c AP_LINEXPR_DENSE).
   * \arg \c coeffs has size size, coeffs[i] corresponds to the coefficient for dimension i.
   * \arg \c cst corresponds to the constant coefficient.
   */
  linexpr0(size_t size, const coeff coeffs[], const coeff& cst, ap_linexpr_discr_t discr=AP_LINEXPR_SPARSE);

  /*! \brief Makes a new linear expression with the given coefficients (copied).
   *
   * \arg \c discr whether the expression is sparse (\c AP_LINEXPR_SPARSE) or dense (\c AP_LINEXPR_DENSE).
   * \arg \c coeffs vector of coefficients, coeffs[i] corresponds to the coefficient for dimension i.
   * \arg \c cst corresponds to the constant coefficient.
   */
  linexpr0(const std::vector<coeff>& coeffs, const coeff& cst, ap_linexpr_discr_t discr=AP_LINEXPR_SPARSE);

  /*! \brief Makes a new linear expression with the given size and coefficients (copied).
   *
   * The expression is always sparse.
   * \arg \c coeffs has size size, coeffs[i] corresponds to the coefficient for dimension dims[i];
   * \arg \c dims is a list of size dimensions (sorted in increasing order)
   * \arg \c cst corresponds to the constant coefficient.
   */
  linexpr0(size_t size, const coeff coeffs[], const ap_dim_t dims[], const coeff& cst);

  //@}


  /* destructor */
  /* ========== */

  /** @name Destructor */
  //@{

  //! Frees all space for the expression and coefficients.
  ~linexpr0();

  //@}


  /* assignment */
  /* ========== */

  /** @name Assignment */
  //@{

  //! Makes a (deep) copy.
  linexpr0& operator= (const linexpr0& x);

  //@}


  /* dimension operations */
  /* ==================== */

  /** @name Dimension operations */
  //@{

  //! Changes the number of coefficients in the expression. (Useful only for dense expressions.)
  void resize(size_t size);

  //! Adds some dimensions, shifting coefficients if needed.
  void add_dimensions(const dimchange& d);

  //! Applies a permutation on coefficients.
  void permute_dimensions(const dimperm& d);

  //@}

  
  /* access */
  /* ====== */

  /** @name Accesses, size */
  //@{

  /* size */

  //! Returns the number of coefficients in the expression.
  size_t size() const;


  /* get */
 
  /*! \brief Returns the expression type.
   *
   * \return Either \c AP_LINEXPR_SPARSE or \c AP_LINEXPR_DENSE.
   */
  ap_linexpr_discr_t get_discr() const;

  //! Returns a (modifiable) reference to the constant coefficient.
  coeff& get_cst();

  //! Returns a reference to the constant coefficient.
  const coeff& get_cst() const;

  /*! \brief Returns a (modifiable) reference to the coefficient corresponding to the given dimension.
   *
   * Always succeeds if the expression is sparse: if the dimension has no coefficient yet, it is
   * created and returned (linear time in the worst case).
   * \throw std::out_of_range if the expression is dense and the dimension exceeds the size
   * of the expression.
   * \warning as a side-effect, the method renders previous references to coefficients within the
   * same expression invalid. For instance, \code l.get_coeff(0) = l.get_coeff(1) = 0; \endcode is invalid.
   */
  coeff& operator[](ap_dim_t dim);
 
  //! \brief Returns a reference to the coefficient corresponding to the given dimension.
  const coeff& operator[](ap_dim_t dim) const;

  //@}


  /* print */
  /* ===== */

  /** @name Printing */
  //@{

  /*! \brief Printing.
   *
   * Variable naming can be configured through the varname stream modifier.
   */
  friend std::ostream& operator<< (std::ostream& os, const linexpr0& s);

  //! Prints to a C stream.
  void print(char** name_of_dim = NULL, FILE* stream=stdout) const;

  //@}


  /* tests */
  /* ===== */

  /** @name Tests */
  //@{

  //! Whether only dimensions greater than \c  intdim have a non-zero coefficient.
  bool is_integer(size_t intdim) const;

  //! Whether only dimensions strictly smaller than \c intdim have a non-zero coefficient.
  bool is_real(size_t intdim) const;

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

  //! Lexicography ordering, terminating with constant coefficients.
  friend int compare(const linexpr0& x, const linexpr0& y);

  //! Structural, syntactical equality.
  friend bool equal(const linexpr0& x, const linexpr0& y);

#if 0 // overloaded to make constraints
  //! Whether compare(x,y)>=0.
  friend bool operator>= (const linexpr0& x, const linexpr0& y);

  //! Whether compare(x,y)<=0.
  friend bool operator<= (const linexpr0& x, const linexpr0& y);

  //! Whether compare(x,y)>0.
  friend bool operator> (const linexpr0& x, const linexpr0& y);

  //! Whether compare(x,y)<0.
  friend bool operator< (const linexpr0& x, const linexpr0& y);

  //! Whether both expressions have (dimension-wise) equal coefficients.
  friend bool operator== (const linexpr0& x, const linexpr0& y);

  //! Whether some coefficients are different.
  friend bool operator!= (const linexpr0& x, const linexpr0& y);
#endif

  //@}
 
  /* iterators */
  /* ========= */


  /** @name Iterator */
  //@{


  /*! \brief Iterator to traverse a constant linexpr0.
   *
   * At each position, there is dimension and its corresponding coefficients.
   * For sparse expressions, there can be a gap between the dimension at two successive position
   * (holes are skipped.)
   * In all cases, the expression is traversed in increasing dimension order.
   *
   * To mutate a linear expression, use the iterator class instead.
   *
   * Sample code:
   * \code for (linexpr0::const_iterator i = m.begin();i.valid();++i) cout << i.get_coeff() << " "; \endcode
   */
  class const_iterator {

    /* TODO:
       - reverse traversal (operator--).
     */

  protected:

    ap_linexpr0_t* l; //!< Internal use only. Pointer to the underlying APRON structure.
    ap_dim_t pos;     //!< Internal use only. Current index.

    //! Internal use only. Skips free coefficients in sparse expressions.
    void skip_AP_DIM_MAX();

    //! Internal use only.
    const_iterator(ap_linexpr0_t* l);

    friend class linexpr0;

  public:
   
    //! Starts a new iterator to traverse the linear expression.
    const_iterator(const linexpr0& e);

    //! Duplicates the iterator.
    const_iterator(const const_iterator& i);

    //! Assigns the iterator.
    const_iterator& operator=(const const_iterator& i);

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
   * As const_iterator, but for non-constant linexpr0.
   *
   * Caution: do not modify the expression during traversal, except through the iterator.
   *
   * Sample code:
   * \code for (linexpr0::iterator i = m.begin();i.valid();++i) i.get_coeff().neg(); \endcode
   */
  class iterator : public const_iterator {

    /* TODO:
       - removing/inserting coefficients at iterator position for sparse expressions.
     */

  protected:

    //! Internal use only.
    iterator(ap_linexpr0_t* l);

    friend class linexpr0;


  public:
   
    //! Starts a new iterator to traverse the linear expression.
    iterator(linexpr0& e);

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

  //! Returns a hash-code.
  long hash() const;

  //@}


  /* TODO: evaluation, linearization, intelligent constructors */


  /* C-level compatibility */
  /* ===================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object stored in *this.
  const ap_linexpr0_t* get_ap_linexpr0_t() const;

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_linexpr0_t* get_ap_linexpr0_t();

  //@}

};

#include "apxx_linexpr0_inline.hh"

}

#endif /* __APXX_LINEXPR0_HH */
