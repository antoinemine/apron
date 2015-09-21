/* -*- C++ -*-
 * apxx_texpr1.hh
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

#ifndef __APXX_TEXPR1_HH
#define __APXX_TEXPR1_HH

#include "ap_texpr1.h"

#include "apxx_linexpr1.hh"
#include "apxx_texpr0.hh"


namespace apron {

/* ================================= */
/* texpr1                            */
/* ================================= */


/*! \brief Level 1 arbitrary expression tree (ap_texpr1_t wrapper).
 *
 * Level 1 version of arbitrary expression trees (texpr0).
 * Variable names (var) are used instead of dimensions (ap_dim_t).
 * Internally, a texpr1 wraps together a texpr0 (memory managed) and an environment (holding a
 * reference count).
 *
 * There are also level 1 equivalent of the helper classes: builder, iterator, and const_iterator.
 *
 */
class texpr1 : public use_malloc {

protected:

  ap_texpr1_t l; //!< Structure managed by APRON.

  //! Internal use only. Shallow copy.
  texpr1(ap_texpr1_t* x);

  //! Internal use only. Shallow copy.
  void init_from(ap_environment_t* e, ap_texpr0_t* x);

  //! Internal use only. Shallow copy.
  void init_from(ap_texpr1_t* x);

public:

  class builder;
  class iterator;
  class const_iterator;


  /* constructors */
  /* ============ */

  /** @name Constructors */
  //@{

  //! Makes an expression from a temporary.
  texpr1(const builder& c);

  //! Makes a (deep) copy of the expression tree.
  texpr1(const const_iterator& x);

  //! Makes a (deep) copy of the expression tree.
  texpr1(const texpr1& x);

  /*! \brief Makes a variable name dimension leaf.
   *
   * \throw std::invalid_argument if the environment does not contain the variable.
   */
  texpr1(const environment& e, const var& v);

  /*! \brief Makes a (deep) copy of the level 0 expression tree and associates an environment
   * (increments reference count).
   */
  texpr1(const environment& e, const texpr0::const_iterator& x);

  /*! \brief Makes a (deep) copy of the level 0 expression tree and associates an environment
   * (increments reference count).
   */
  texpr1(const environment& e, const texpr0& x);

  //! Makes an expression from a level 0 temporary.
  texpr1(const environment& e, const texpr0::builder& c);

#if 0
  //! Goes through the construction of a temporary builder.
  template<class T> texpr1(const T& x);

  //! Goes through the construction of a (level 0) temporary builder.
  template<class T> texpr1(const environment& e, const T& x);
#endif

  /*! \brief Makes an expression tree from a linear expression (copying coefficients).
   *
   * Real-valued addition and multiplication operators are used (i.e., no rounding).
   */
  texpr1(const linexpr1& l);

  /*! \brief Makes a (deep) copy of x and extends its environment.
   *
   * \throw std::invalid_argument if e is not a super-environment of that of x.
   */
  texpr1(const texpr1& x, const environment& e);

  /*! \brief Makes a (deep) copy of x and substitutes every occurrence of the src variable name with
   * a fresh copy of the dst expression.
   *
   * \throw std::invalid_argument if the environments are not equal or do not contain the variable.
   */
  texpr1(const texpr1& x, const var& src, const texpr1& dst);

  //@}


  /* destructor */
  /* ========== */

  /** @name Destructor */
  //@{

  //! Frees the memory occupied by the expression and decrements the reference count of the environment.
  ~texpr1();

  //@}

  /* assignment */
  /* ========== */

  /** @name Assignments */
  //@{

  //! Makes a (deep) copy of the expression.
  texpr1& operator=(const texpr1& x);

  //! Makes a (deep) copy of the expression.
  texpr1& operator=(const const_iterator& x);

  /*! \brief Assigns from a temporary expression, performing a deep copy and deleting the previous
   * value of *this.
   */
  texpr1& operator=(const builder& x);


#if 0
  //! Other assignments go through the use of a (level 1) temporary builder.
  template<class T> texpr1& operator=(const T& x);
#endif

  /*! \brief Makes *this equal to the linear expression x (coefficients are copied), deleting the previous value of *this.
   *
   * Real-valued addition and multiplication operators are used (i.e., no rounding).
   */
  texpr1& operator= (const linexpr1& x);

  //@}



  /* iterators */
  /* ========= */


  /** @name Iterators */
  //@{

  /*! \brief Iterators to traverse a constant expression tree.
   *
   * Use the iterator class if you also wish to mutate an expression.
   */
  class const_iterator {

  protected:

    ap_texpr1_t l;

     //! Internal use only.
    const_iterator(ap_texpr1_t* l);

    //! Internal use only.
    const_iterator(ap_texpr0_t* t, ap_environment_t* e);

    friend class texpr1;
    friend class tcons1;

  public:

    /* constructors */
    /* ============ */

    /** @name Constructors */
    //@{

    //! Starts a new const_iterator at the root of a constant expression tree.
    const_iterator(const texpr1& e);

    //! Duplicates the const_iterator.
    const_iterator(const const_iterator& i);

    //@}


    /* access */
    /* ====== */

    /** @name Access */
    //@{

    //! \brief Returns (a copy of) the environment associated with the expression.
    environment get_environment() const;

    //! Returns a constant iterator at the root of the underlying texpr0.
    texpr0::const_iterator get_texpr0() const;

   /*! \brief Returns the node kind.
     *
     * \return \c AP_TEXPR_CST for a constant node, \c AP_TEXPR_DIM for a dimension,
     * and \c AP_TEXPR_NODE for an operator node.
     */
    ap_texpr_discr_t get_discr() const;

   /*! \brief Returns a reference to the coefficient of a constant node.
     *
     * \throw bad_discriminant if the node is not a constant leaf.
     */
    const coeff& get_coeff() const;

    /*! \brief Returns a (unmodifiable) reference to the variable name of a dimension node.
     *
     * \throw bad_discriminant if the node is not a dimension node.
     */
    const var& get_var() const;

    /*! \brief Returns the dimension of a dimension node.
     *
     * \throw bad_discriminant if the node is not a dimension node.
     */
    ap_dim_t get_dim() const;

    /*! \brief Returns the operator kind of an operator node.
     *
     * \return either \c AP_TEXPR_ADD, \c AP_TEXPR_SUB, \c AP_TEXPR_MUL, \c AP_TEXPR_DIV, \c AP_TEXPR_MOD, \c AP_TEXPR_POW,
     * \c AP_TEXPR_NEG, \c AP_TEXPR_CAST, or \c AP_TEXPR_SQRT.
     * \throw bad_discriminant if the node is not an operator node.
     */
    ap_texpr_op_t get_op() const;

    /*! \brief Returns the destination type of an operator node.
     *
     * \return either \c AP_RTYPE_REAL, \c AP_RTYPE_INT, \c AP_RTYPE_SINGLE (32-bit), \c AP_RTYPE_DOUBLE (64-bit),
     *  \c AP_RTYPE_EXTENDED (80-bit), or \c AP_RTYPE_QUAD (128-bit).
     * \throw bad_discriminant if the node is not an operator node.
     */
    ap_texpr_rtype_t get_rtype() const;

    /*! \brief Returns the rounding direction of an operator node.
     *
     * \return either \c AP_RDIR_NEAREST, \c AP_RDIR_ZERO, \c AP_RDIR_UP, \c AP_RDIR_DOWN, or \c AP_RDIR_RND.
     * \throw bad_discriminant if the node is not an operator node.
     */
    ap_texpr_rdir_t get_rdir() const;

    //@}


    /* traversal */
    /* ========= */

    /** @name Traversal */
    //@{

    //! Resets the const_iterator at position i.
    const_iterator& operator=(const const_iterator& i);

    /*! \brief Constructs a const_iterator to the only or left sub-expression of an operator node.
     *
     * \throw bad_discriminant if the node is not an operator node.
     */
    const_iterator child() const;

    /*! \brief Constructs a const_iterator to the only of left sub-expression of an operator node.
     *
     * (Identical to child()).
     * \throw bad_discriminant if the node is not an operator node.
     */
    const_iterator left() const;

    /*! \brief Constructs a const_iterator to the right sub-expression argument of a binary operator node.
     *
     * \throw bad_discriminant if the node is not an operator node.
     * \throw std::out_of_range if the operator node is not binary.
     */
    const_iterator right() const;

    //@}


    /* tests, size, dimensions */
    /* ======================= */

    /** @name Tests, size, dimensions */
    //@{


    //! Whether two expressions are syntactically, structurally equal.
    bool equal(const texpr1& x) const;

    //! Whether the expression is a single coefficient node with 0 value.
    bool is_zero() const;

    //! Returns the depth of the expression tree (counting only operator nodes).
    size_t depth() const;

    //! Returns the number of operator nodes in the expression tree.
    size_t size() const;

    //! Whether the given variable name occurs in the expression.
    bool has_var(const var& v) const;

    //! Whether the expression is constant (i.e., has no dimension leaves).
    bool is_interval_cst() const;

    //! Whether the expression is linear and there is no rounding.
    bool is_interval_linear() const;

    //! Whether the expression is polynomial and there is no rounding.
    bool is_interval_polynomial() const;

    //! Whether the expression is a polynomial fraction and there is no rounding.
    bool is_interval_polyfrac() const;

    //! Whether all occurring constants are scalar.
    bool is_scalar() const;

    //@}


    /* print */
    /* ===== */

    /** @name Printing */
    //@{

    //! Printing.
    friend std::ostream& operator<< (std::ostream& os, const const_iterator& s);

    //! Prints to a C stream.
    void print(FILE* stream=stdout) const;

    //@}


    /* C-level compatibility */
    /* ===================== */

    /** @name C API compatibility */
    //@{

    //! Returns a pointer to the internal APRON object stored in *this.
    ap_texpr1_t* get_ap_texpr1_t();

    //! Returns a pointer to the internal APRON object stored in *this.
    const ap_texpr1_t* get_ap_texpr1_t() const;

    //@}

  };


  /*! \brief Iterators to traverse and mutate an expression tree.
   *
   * As const_iterator, but can be used to modify as well as traverse nodes.
   */
  class iterator : public const_iterator {

  protected:

    //! Internal use only.
    iterator(ap_texpr1_t* l);

    //! Internal use only.
    iterator(ap_texpr0_t* t, ap_environment_t* e);

    friend class texpr1;
    friend class tcons1;

  public:

    /* constructors */
    /* ============ */

    /** @name Constructors */
    //@{

    //! Starts a new iterator at the root of the tree.
    iterator(texpr1& e);

    //! Duplicates the iterator.
    iterator(const iterator& i);

    //@}


    /* substitution */
    /* ============ */

    /** @name Expression substitutions */
    //@{

    /*! \brief Replace the sub-expression at the iterator position with a (deep) copy of c.
     *
     * Assumes that the *this and have the same environment (unchecked).
     */
    iterator& operator= (const builder& x);

#if 0
    //! Assignment is through the construction of a texpr0::build.
    template<class T> iterator& operator= (const T& c);
#endif

    //@}


    /* access */
    /* ====== */

    /** @name Access */
    //@{

    //! Returns an iterator at the root of the underlying texpr0.
    texpr0::iterator get_texpr0() const;

    /*! \brief Returns a (modifiable) reference to the coefficient of a constant node.
     *
     * \throw bad_discriminant if the node is not a constant leaf.
     */
    coeff& get_coeff() const;

    /*! \brief Sets the variable name of a dimension node.
     *
     * \throw bad_discriminant if the node is not a dimension node.
     * \throw std::invalid_argument if the environment does not contain a variable.
     */
    void set_var(const var& v) const;

    /*! \brief Returns a (modifiable) reference to the dimension of a dimension node.
     *
     * \throw bad_discriminant if the node is not a dimension node.
     */
    ap_dim_t& get_dim() const;

    /*! \brief Returns a (modifiable) reference to the operator kind of an operator node.
     *
     * \return either \c AP_TEXPR_ADD, \c AP_TEXPR_SUB, \c AP_TEXPR_MUL, \c AP_TEXPR_DIV, \c AP_TEXPR_MOD, \c AP_TEXPR_POW,
     * \c AP_TEXPR_NEG, \c AP_TEXPR_CAST, or \c AP_TEXPR_SQRT.
     * \throw bad_discriminant if the node is not an operator node.
     */
    ap_texpr_op_t& get_op() const;

    /*! \brief Returns a (modifiable) reference to the destination type of an operator node.
     *
     * \return either \c AP_RTYPE_REAL, \c AP_RTYPE_INT, \c AP_RTYPE_SINGLE (32-bit), \c AP_RTYPE_DOUBLE (64-bit),
     *  \c AP_RTYPE_EXTENDED (80-bit), or \c AP_RTYPE_QUAD (128-bit).
     * \throw bad_discriminant if the node is not an operator node.
     */
    ap_texpr_rtype_t& get_rtype() const;

    /*! \brief Returns a (modifiable) reference to the rounding direction of an operator node.
     *
     * \return either \c AP_RDIR_NEAREST, \c AP_RDIR_ZERO, \c AP_RDIR_UP, \c AP_RDIR_DOWN, or \c AP_RDIR_RND.
     * \throw bad_discriminant if the node is not an operator node.
     */
    ap_texpr_rdir_t& get_rdir() const;

    //@}


    /* traversal */
    /* ========= */

    /** @name Traversal */
    //@{

    //! Resets the iterator at position i.
    iterator& operator=(const iterator& i);

    /*! \brief Constructs an iterator to the only or left sub-expression of an operator node.
     *
     * \throw bad_discriminant if the node is not an operator node.
     */
    iterator child() const;

    /*! \brief Constructs an iterator to the only of left sub-expression of an operator node.
     *
     * (Identical to child()).
     * \throw bad_discriminant if the node is not an operator node.
     */
    iterator left() const;

    /*! \brief Constructs an iterator to the right sub-expression argument of a binary operator node.
     *
     * \throw bad_discriminant if the node is not an operator node.
     * \throw std::out_of_range if the operator node is not binary.
     */
    iterator right() const;

    //@}


    /* C-level compatibility */
    /* ===================== */

    /** @name C API compatibility */
    //@{

    //! Returns a pointer to the internal APRON object stored in *this.
    ap_texpr1_t* get_ap_texpr1_t();

    //! Returns a pointer to the internal APRON object stored in *this.
    const ap_texpr1_t* get_ap_texpr1_t() const;

    //@}

  };


  //! Returns a new iterator at the root of a constant expression tree.
  const_iterator root() const;

  //! Returns a new iterator at the root of a mutable expression tree.
  iterator root();

  //@}


  /* builders */
  /* ======== */

  /** @name Builders */
  //@{

  /*! \brief Temporary expression nodes used when constructing a texpr1.
   *
   * A temporary expression is composed of a level 0 node with an associated environment.
   *
   * You should not create variables of type build, nor define functions that return or take
   * builder objects as arguments.
   */
  class builder : public use_malloc {

    friend class texpr1;

  protected:

    ap_texpr1_t l;

    //! Not to be used. (Temporaries are not to be re-assigned).
    builder& operator= (const builder& x) { assert(0); return *this; }

    //! Internal use only: makes a shallow copy, copying only the root node and referencing the environment.
    void init_from(ap_texpr1_t* x);

    //! Internal use only: makes a shallow copy, copying only the root node and referencing the environment.
    void init_from(ap_environment_t* e, ap_texpr0_t* x);

    //! Internal use only: makes a shallow copy, copying only the root node and referencing the environment.
    builder(ap_texpr1_t* x);

  public:

    /* constructors */
    /* ============ */

    /** @name Constructors */
    //@{

    //! Makes a shallow copy, copying only the root node. Sub-expressions are aliased, not copied.
    builder(const builder& x);

    //! Makes a shallow copy, copying only the root node. Sub-expressions are aliased, not copied.
    builder(const const_iterator& x);

    //! Makes a shallow copy, copying only the root node. Sub-expressions are aliased, not copied.
    builder(const texpr1& x);

    //! Converts from a level 0 builder and associates an environment.
    builder(const environment& e, const texpr0::builder& b);

    //! Converts from a level 0 expression (shallow copy) and associates an environment.
    builder(const environment& e, const texpr0::const_iterator& b);

    //! Converts from a level 0 expression (shallow copy) and associates an environment.
    builder(const environment& e, const texpr0& b);


    //! Makes a constant leaf from a scalar or interval coefficient (copied).
    builder(const environment& e, const coeff& x);

    //! Makes a constant scalar leaf from a scalar (copied).
    builder(const environment& e, const scalar& x);

    //! Makes a constant scalar leaf from a MPQ (copied).
    builder(const environment& e, const mpq_class& x);

    //! Makes a constant scalar leaf from a MPFR (copied).
    builder(const environment& e, mpfr_t x);

    //! Makes a constant scalar MPQ leaf from an integer.
    builder(const environment& e, int x);

    //! Makes a constant scalar MPQ leaf from an integer.
    builder(const environment& e, long x);

    //! Makes a constant scalar double leaf from a double.
    builder(const environment& e, double x);

    //! Makes a constant scalar MPQ leaf from a fraction with integer coefficients.
    builder(const environment& e, const frac& x);

    //! Makes a constant interval leaf from an interval (copied).
    builder(const environment& e, const interval& x);

    //! Makes a constant interval leaf from two scalar bounds (copied).
    builder(const environment& e, const scalar& inf, const scalar& sup);

    //! Makes a constant interval leaf from two MPQ bounds (copied).
    builder(const environment& e, const mpq_class& inf, const mpq_class& sup);

    //! Makes a constant interval leaf from two MPFR bounds (copied).
    builder(const environment& e, mpfr_t inf, mpfr_t sup);

    //! Makes a constant interval leaf from two integer bounds (converted to MPQ).
    builder(const environment& e, int inf, int sup);

    //! Makes a constant interval leaf from two integer bounds (converted to MPQ).
    builder(const environment& e, long inf, long sup);

    //! Makes a constant interval leaf from two double bounds.
    builder(const environment& e, double inf, double sup);

    //! Makes a constant interval leaf from two fraction bounds (converted to MPQ).
    builder(const environment& e, const frac& inf, const frac& sup);

    //! Makes a constant interval leaf equal to ]-oo;+oo[.
    builder(const environment& e, top t);

    //! Makes a variable name dimension leaf.
    builder(const environment& e, const var& v);

    //! Makes a unary expression node.
    builder(ap_texpr_op_t op, const builder& argA, ap_texpr_rtype_t rtype, ap_texpr_rdir_t rdir);

    //! Makes a binary expression node.
    builder(ap_texpr_op_t op, const builder& argA, const builder& argB, ap_texpr_rtype_t rtype, ap_texpr_rdir_t rdir);

    //@}


    /* destructor */
    /* ========== */

    /** @name Destructor */
    //@{

    //! Frees the memory occupied by the node, not its sub-expressions.
    ~builder();

    //@}


    /* tests */
    /* ===== */


    /** @name Tests */
    //@{

    //! Whether the expression is a single coefficient node with 0 value.
    bool is_zero() const;

    //@}

    /* C-level compatibility */
    /* ===================== */

    /** @name C API compatibility */
    //@{

    //! Returns a pointer to the internal APRON object stored in *this.
    ap_texpr1_t* get_ap_texpr1_t();

    //! Returns a pointer to the internal APRON object stored in *this.
    const ap_texpr1_t* get_ap_texpr1_t() const;

    //@}


    /* 'Intelligent' constructors */
    /* ========================== */

    /** @name 'Intelligent' constructors */
    //@{

    /*! \brief Makes a unary expression node.
     *
     * Creates only one node. The argument expression is aliased, not copied.
     *
     * \arg \c op should be a unary operator: negation (\c AP_TEXPR_NEG), typecast (\c AP_TEXPR_CAST),
     * or square root (\c AP_TEXPR_SQRT).
     * \arg \c argA should point to the argument expression.
     * \arg \c rtype is the destination type (for rounding): reals (\c AP_RTYPE_REAL, means no rounding),
     * perfect integers (\c AP_RTYPE_INT), 32-bit single precision (\c AP_RTYPE_SINGLE), 64-bit double precision
     * (\c AP_RTYPE_DOUBLE), 80-bit extended precision (\c AP_RTYPE_EXTENDED), or
     * 128-bit quadruple precision (\c AP_RTYPE_QUAD).
     * \arg \c rdir is the rounding direction: to nearest (\c AP_RDIR_NEAREST), truncation (\c AP_RDIR_ZERO),
     * towards +oo (\c AP_RDIR_UP), towards -oo (\c AP_RDIR_DOWN), or non-deterministic (\c AP_RDIR_RND).
     * \throw std::invalid_argument if \c op is not a unary operator.
     */
    friend builder unary(ap_texpr_op_t op, const builder& a,
			 ap_texpr_rtype_t rtype, ap_texpr_rdir_t rdir);

    /*! \brief Makes a binary expression node.
     *
     * Creates only one node. The argument expressions is aliased, not copied.
     *
     * \arg \c op should be a binary operator: addition (\c AP_TEXPR_ADD), subtraction (\c AP_TEXPR_SUB),
     * multiplication (\c AP_TEXPR_MUL), division (\c AP_TEXPR_DIV), or modulo (\c AP_TEXPR_MOD, \c AP_TEXPR_POW).
     * \arg \c argA should point to the left argument expression.
     * \arg \c argB should point to the right argument expression.
     * \arg \c rtype is the destination type (for rounding): reals (\c AP_RTYPE_REAL, means no rounding),
     * perfect integers (\c AP_RTYPE_INT), 32-bit single precision (\c AP_RTYPE_SINGLE), 64-bit double precision
     * (\c AP_RTYPE_DOUBLE), 80-bit extended precision (\c AP_RTYPE_EXTENDED).
     * \arg \c rdir is the rounding direction: to nearest (\c AP_RDIR_NEAREST), truncation (\c AP_RDIR_ZERO),
     * towards +oo (\c AP_RDIR_UP), towards -oo (\c AP_RDIR_DOWN), or non-deterministic (\c AP_RDIR_RND).
     * \throw std::invalid_argument if \c op is not a binary operator.
     */
    friend builder binary(ap_texpr_op_t op, const builder& a, const builder& b,
			  ap_texpr_rtype_t rtype, ap_texpr_rdir_t rdir);

    //! Makes an \c AP_TEXPR_ADD expression node.
    friend builder add(const builder& a, const builder& b,
		       ap_texpr_rtype_t rtype, ap_texpr_rdir_t rdir);

    //! Makes an \c AP_TEXPR_SUB expression node.
    friend builder sub(const builder& a, const builder& b,
		       ap_texpr_rtype_t rtype, ap_texpr_rdir_t rdir);

    //! Makes an \c AP_TEXPR_MUL expression node.
    friend builder mul(const builder& a, const builder& b,
		       ap_texpr_rtype_t rtype, ap_texpr_rdir_t rdir);

    //! Makes an \c AP_TEXPR_DIV expression node.
    friend builder div(const builder& a, const builder& b,
		       ap_texpr_rtype_t rtype, ap_texpr_rdir_t rdir);

    //! Makes an \c AP_TEXPR_MOD expression node.
    friend builder mod(const builder& a, const builder& b,
		       ap_texpr_rtype_t rtype, ap_texpr_rdir_t rdir);

    //! Makes an \c AP_TEXPR_POW expression node.
    friend builder pow(const builder& a, const builder& b,
		       ap_texpr_rtype_t rtype, ap_texpr_rdir_t rdir);

    //! Makes an \c AP_TEXPR_NEG expression node.
    friend builder neg(const builder& a,
		       ap_texpr_rtype_t rtype, ap_texpr_rdir_t rdir);

    //! Makes an \c AP_TEXPR_CAST expression node.
    friend builder cast(const builder& a, ap_texpr_rtype_t rtype, ap_texpr_rdir_t rdir);

    //! Makes an \c AP_TEXPR_CAST expression node to \c AP_RTYPE_INT type rounded towards \c AP_RDIR_DOWN.
    friend builder floor(const builder& a);

    //! Makes an \c AP_TEXPR_CAST expression node to \c AP_RTYPE_INT type rounded towards \c AP_RDIR_UP.
    friend builder ceil(const builder& a);

    //! Makes an \c AP_TEXPR_CAST expression node to \c AP_RTYPE_INT type rounded towards \c AP_RDIR_ZERO.
    friend builder trunc(const builder& a);

    //! Makes an \c AP_TEXPR_SQRT expression node.
    friend builder sqrt(const builder& a,
			ap_texpr_rtype_t rtype, ap_texpr_rdir_t rdir);

    //! Makes a copy of the node.
    friend builder operator+(const builder& a);

    //! Makes a \c AP_TEXPR_NEG expression node using \c AP_RTYPE_REAL type (no rounding).
    friend builder operator-(const builder& a);

    //! Makes a \c AP_TEXPR_ADD expression node using \c AP_RTYPE_REAL type (no rounding).
    friend builder operator+(const builder& a, const builder& b);

    //! Makes a \c AP_TEXPR_SUB expression node using \c AP_RTYPE_REAL type (no rounding).
    friend builder operator-(const builder& a, const builder& b);

    //! Makes a \c AP_TEXPR_MUL expression node using \c AP_RTYPE_REAL type (no rounding).
    friend builder operator*(const builder& a, const builder& b);

    //! Makes a \c AP_TEXPR_DIV expression node using \c AP_RTYPE_REAL type (no rounding).
    friend builder operator/(const builder& a, const builder& b);

    //! Makes a \c AP_TEXPR_MOD expression node using \c AP_RTYPE_REAL type (no rounding).
    friend builder operator%(const builder& a, const builder& b);

    //! Makes a \c AP_TEXPR_POW expression node using \c AP_RTYPE_REAL type (no rounding).
    friend builder operator^(const builder& a, const builder& b);

    //@}
  };


  /* print */
  /* ===== */

  /** @name Printing */
  //@{

  //! Printing.
  friend std::ostream& operator<< (std::ostream& os, const texpr1& s);

  //! Prints to a C stream.
  void print(FILE* stream=stdout) const;

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

  /** @name Accesses, tests, size */
  //@{

  //! Returns the environment of the expression (with incremented reference count).
  environment get_environment() const;

  //! Returns a constant iterator at the root of the underlying texpr0.
  texpr0::const_iterator get_texpr0() const;

  //! Returns an iterator at the root of the underlying texpr0.
  texpr0::iterator get_texpr0();

  //! Whether the expression is a single coefficient node with 0 value.
  bool is_zero() const;

  //! Whether two expressions are syntactically, structurally equal.
  bool equal(const texpr1& x) const;

  //! Returns the depth of the expression tree (counting only operator nodes).
  size_t depth() const;

  //! Returns the number of operator nodes in the expression tree.
  size_t size() const;

  //! Whether the given variable name occurs in the expression.
  bool has_var(const var& v) const;

  //! Whether the expression is constant (i.e., has no dimension leaves).
  bool is_interval_cst() const;

  //! Whether the expression is linear and there is no rounding.
  bool is_interval_linear() const;

  //! Whether the expression is polynomial and there is no rounding.
  bool is_interval_polynomial() const;

  //! Whether the expression is a polynomial fraction and there is no rounding.
  bool is_interval_polyfrac() const;

  //! Whether all occurring constants are scalar.
  bool is_scalar() const;

  //@}

  /* operations */
  /* ========== */

  /** @name Operations */
  //@{

  /*! \brief Substitutes each occurrence of src with (a fresh copy of) dst.
   *
   * \throw std::invalid_argument if the environments are not equal or do not contain src.
   */
  void substitute(const var& src, const texpr1& dst);

  //@}



  /* C-level compatibility */
  /* ===================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_texpr1_t* get_ap_texpr1_t();

  //! Returns a pointer to the internal APRON object stored in *this.
  const ap_texpr1_t* get_ap_texpr1_t() const;

  //@}

};

#include "apxx_texpr1_inline.hh"

}

#endif /* __APXX_TEXPR1_HH */
