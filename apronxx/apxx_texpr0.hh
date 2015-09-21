/* -*- C++ -*-
 * apxx_texpr0.hh
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

#ifndef __APXX_TEXPR0_HH
#define __APXX_TEXPR0_HH

#include <vector>

#include "ap_texpr0.h"
#include "ap_linearize.h"

#include "apxx_linexpr0.hh"
#include "apxx_environment.hh"

namespace apron {

class abstract0;


//! Represents a dimension (i.e., variable by index) in an expression tree.
class dim {

public:

  ap_dim_t d; //!< Dimension index.

  //! Standard constructor.
  dim(ap_dim_t d);

  /*! \brief Extract dimension corresponding to variable name in given environment.
   *
   * \throw std::invalid_argument if the variable does not exist.
   */
  dim(const environment& e, const var& v);
};



//! Whether the operator is unary.
bool is_unop(ap_texpr_op_t op);

//! Whether the operator is binary.
 bool is_binop(ap_texpr_op_t op);




/* ================================= */
/* texpr0                            */
/* ================================= */


/*! \brief Level 0 arbitrary expression tree (ap_texpr0_t wrapper).
 *
 * A texpr0 represents an expression tree.
 * Leaves include dimensions (i.e., variables by index) as well as constants (coefficients).
 * Binary operators include addition, subtraction, multiplication, division, and modulo.
 * Unary operators include opposite, square root, and typecast.
 * Additionally, all operators are tagged with a destination type: perfect integers, reals,
 * or floating-point numbers (of various bit-size).
 * For integer and floating-point operators, a rounding direction can be specified
 * (as well as 'any direction').
 * The concrete semantics of all operators is to first perform the operation on perfect reals
 * (typecast being the identity operator here), and then round the result to the destination type
 * in the specified direction.
 *
 * A texpr0 manages a whole expression tree: assignment, copy, and destruction are deep (recursive).
 *
 * Complex expression trees can be constructed using the standard +, -, *, /, % operators, as well as
 * functions such as add, mul, div, etc. (allowing to set the destination type and rounding direction).
 * \code
 * texpr0 x = dim(1) + dim(2) * 5;
 * texpr0 y = add(sqrt(x), x, AP_RTYPE_FLOAT);
 * \endcode
 * Temporaries created by the operators have the texpr0::builder type.
 * See the texpr0::builder class documentation for the details on operator arguments.
 *
 * Expression trees can be visited and/or modified using the texpr0::iterator type.
 */
class texpr0 : public use_malloc {

protected:

  ap_texpr0_t l; //!< Structure managed by APRON.

  //! Internal use only. Shallow copy.
  texpr0(ap_texpr0_t* x);

  //! Internal use only. Shallow copy.
  void init_from(ap_texpr0_t* x);

public:

  class builder;
  class iterator;
  class const_iterator;


  /* constructors */
  /* ============ */

  /** @name Constructors */
  //@{

  /*! \brief Makes an expression tree from a temporary (performing a deep copy).
   *
   * A deep copy is performed so that, after the assignment, temporaries can be safely destroyed
   * and we have a private copy of all formerly aliased sub-trees.
   */
  texpr0(const builder& x);

  //! Makes a (deep) copy of the expression tree.
  texpr0(const texpr0& x);

  //! Makes a (deep) copy of the expression tree.
  texpr0(const const_iterator& x);

  /*! \brief Makes an expression tree from a linear expression (copying coefficients).
   *
   * Real-valued addition and multiplication operators are used (i.e., no rounding).
   */
  texpr0(const linexpr0& l);


  /*! \brief Makes a (deep) copy of the expression, and then add or remove some dimensions.
   *
   * \arg \c add whether to add or remove dimensions.
   */
  texpr0(const texpr0& x, const dimchange& d, bool add=true);

  //! Makes a (deep) copy of the expression, and then permutes dimensions.
  texpr0(const texpr0& x, const dimperm& d);

  //! Makes a (deep) copy of x and substitutes each occurrence of dim with (a fresh copy of) dst.
  texpr0(const texpr0& x, ap_dim_t dim, const texpr0& dst);

  //@}


  /* destructor */
  /* ========== */

  /** @name Destructor */
  //@{

  //! Frees the memory occupied by the expression (and, recursively, all sub-expressions).
  ~texpr0();

  //@}

  /* assignment */
  /* ========== */

  /** @name Assignments */
  //@{

  //! Makes a (deep) copy of the expression.
  texpr0& operator=(const texpr0& x);

  //! Makes a (deep) copy of the expression.
  texpr0& operator=(const const_iterator& x);

  //! Assigns from a temporary expression, performing a deep copy and deleting the previous value of *this.
  texpr0& operator=(const builder& x);

  /*! \brief Makes *this equal to the linear expression x (coefficients are copied), deleting the previous value of *this.
   *
   * Real-valued addition and multiplication operators are used (i.e., no rounding).
   */
  texpr0& operator= (const linexpr0& x);

  //@}



  /* iterators */
  /* ========= */


  /** @name Iterators */
  //@{

  /*! \brief Iterators to traverse a constant expression tree.
   *
   * For lack of better name, an 'iterator' object is used to reference an
   * expression node within a constant expression tree.
   * Through the 'iterator', nodes can be examined.
   * However, 'iterators' traverse the tree in a functional way: you spawn new
   * const_iterator objects for left and right children.
   *
   * Use the iterator class if you also wish to mutate an expression.
   *
   * \warning You can no longer use a const_iterator after the corresponding node has been discarded
   * (i.e., part of a destroyed expression, part of a sub-expression substituted away, etc.).
   * However, you can safely destroy the const_iterator after the node has been destroyed
   * (provided you do not use it in the meantime).
   *
   */
  class const_iterator {

  protected:

    ap_texpr0_t* l;

    //! Internal use only.
    const_iterator(ap_texpr0_t* l);

    friend class texpr0;
    friend class tcons0;
    friend class texpr1;
    friend class tcons1;

  public:

    /* constructors */
    /* ============ */

    /** @name Constructors */
    //@{

    //! Starts a new const_iterator at the root of a constant expression tree.
    const_iterator(const texpr0& e);

    //! Duplicates the const_iterator.
    const_iterator(const const_iterator& i);

    //@}


    /* access */
    /* ====== */

    /** @name Access */
    //@{


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

    /*! \brief Returns the dimension of a dimension node.
     *
     * \throw bad_discriminant if the node is not a dimension node.
     */
    ap_dim_t get_dim() const;

    /*! \brief Returns the operator kind of an operator node.
     *
     * \return either \c AP_TEXPR_ADD, \c AP_TEXPR_SUB, \c AP_TEXPR_MUL, \c AP_TEXPR_DIV, \c AP_TEXPR_MOD, \c AP_TEXPR_POW
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
    bool equal(const texpr0& x) const;

    //! Whether the expression is a single coefficient node with 0 value.
    bool is_zero() const;

    //! Returns the depth of the expression tree (counting only operator nodes).
    size_t depth() const;

    //! Returns the number of operator nodes in the expression tree.
    size_t size() const;

    //! Returns the maximal dimension occurring in the expression (or 0 if there is no variable).
    ap_dim_t max_dim() const;

    //! Whether the given dimension occurs in the expression.
    bool has_dim(ap_dim_t d) const;

    /*! \brief Returns a list of all dimensions occurring in the expression
     * (in strict increasing order)
     */
    std::vector<ap_dim_t> dimlist() const;

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

    /*! \brief Printing.
     *
     * Variable naming can be configured through the varname stream modifier.
     */
    friend std::ostream& operator<< (std::ostream& os, const const_iterator& s);

    //! Prints to a C stream.
    void print(char** name_of_dim = NULL, FILE* stream=stdout) const;

    //@}


    /* C-level compatibility */
    /* ===================== */

    /** @name C API compatibility */
    //@{

    //! Returns a pointer to the internal APRON object stored in *this.
    ap_texpr0_t* get_ap_texpr0_t();

    //! Returns a pointer to the internal APRON object stored in *this.
    const ap_texpr0_t* get_ap_texpr0_t() const;

    //@}

  };


  /*! \brief Iterators to traverse and mutate an expression tree.
   *
   * As const_iterator, but can be used to modify as well as traverse nodes.
   */
  class iterator : public const_iterator {

  protected:

    //! Internal use only.
    iterator(ap_texpr0_t* l);

    friend class tcons0;
    friend class texpr1;
    friend class tcons1;

  public:

    /* constructors */
    /* ============ */

    /** @name Constructors */
    //@{

    //! Starts a new iterator at the root of the tree.
    iterator(texpr0& e);

    //! Duplicates the iterator.
    iterator(const iterator& i);

    //@}


    /* substitution */
    /* ============ */

    /** @name Expression substitutions */
    //@{

    /*! \brief Replace the sub-expression at the iterator position with a (deep) copy of c.
     *
     * The iterator is still valid after substitution (it points to the sub-expression copy).
     * However, any iterator in the discarded part becomes invalid.
     */
    iterator& operator= (const builder& c);

    //@}


    /* access */
    /* ====== */

    /** @name Access */
    //@{

    /*! \brief Returns a (modifiable) reference to the coefficient of a constant node.
     *
     * \throw bad_discriminant if the node is not a constant leaf.
     */
    coeff& get_coeff() const;

    /*! \brief Returns a (modifiable) reference to the dimension of a dimension node.
     *
     * \throw bad_discriminant if the node is not a dimension node.
     */
    ap_dim_t& get_dim() const;

    /*! \brief Returns a (modifiable) reference to the operator kind of an operator node.
     *
     * \return either \c AP_TEXPR_ADD, \c AP_TEXPR_SUB, \c AP_TEXPR_MUL, \c AP_TEXPR_DIV, \c AP_TEXPR_MOD, \c AP_TEXPR_POW
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
    ap_texpr0_t* get_ap_texpr0_t();

    //! Returns a pointer to the internal APRON object stored in *this.
    const ap_texpr0_t* get_ap_texpr0_t() const;

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

  /*! \brief Temporary expression nodes used when constructing a texpr0.
   *
   * Builders allow constructing expression trees one node at a time using overloaded
   * operators (+, -, *, /, %) and friend functions (add, sub, mul, div, mod, neg, sqrt, cast,
   * unary, binary).
   *
   * Each builder object manages a single node which is allocated by the constructor and destroyed
   * by the destructor, without considering potential sub-nodes.
   * As their usage is temporary (they are normally destructed at the end of full expressions),
   * assignment, copy, and destruction can be shallow, which results in improved efficiency wrt.
   * using texpr0 as temporaries.
   *
   * You should not create variables of type build, nor define functions that return or take
   * builder objects as arguments.
   */
  class builder : public use_malloc {

    friend class texpr1;

  protected:

    ap_texpr0_t* l;


    //! Not to be used. (Temporaries are not to be re-assigned).
    builder& operator= (const builder& x) { assert(0); return *this; }

    //! Internal use only: makes a shallow copy, copying only the root node.
    void init_from(ap_texpr0_t* x);

    //! Internal use only: makes a shallow copy, copying only the root node.
    builder(ap_texpr0_t* x);

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
    builder(const texpr0& x);

    //! Makes a constant leaf from a scalar or interval coefficient (copied).
    builder(const coeff& x);

    //! Makes a constant scalar leaf from a scalar (copied).
    builder(const scalar& x);

    //! Makes a constant scalar leaf from a MPQ (copied).
    builder(const mpq_class& x);

    //! Makes a constant scalar leaf from a MPFR (copied).
    builder(mpfr_t x);

    //! Makes a constant scalar MPQ leaf from an integer.
    builder(int x);

    //! Makes a constant scalar MPQ leaf from an integer.
    builder(long x);

    //! Makes a constant scalar double leaf from a double.
    builder(double x);

    //! Makes a constant scalar MPQ leaf from a fraction with integer coefficients.
    builder(const frac& x);

    //! Makes a constant interval leaf from an interval (copied).
    builder(const interval& x);

    //! Makes a constant interval leaf from two scalar bounds (copied).
    builder(const scalar& inf, const scalar& sup);

    //! Makes a constant interval leaf from two MPQ bounds (copied).
    builder(const mpq_class& inf, const mpq_class& sup);

    //! Makes a constant interval leaf from two MPFR bounds (copied).
    builder(mpfr_t inf, mpfr_t sup);

    //! Makes a constant interval leaf from two integer bounds (converted to MPQ).
    builder(int inf, int sup);

    //! Makes a constant interval leaf from two integer bounds (converted to MPQ).
    builder(long inf, long sup);

    //! Makes a constant interval leaf from two double bounds.
    builder(double inf, double sup);

    //! Makes a constant interval leaf from two fraction bounds (converted to MPQ).
    builder(const frac& inf, const frac& sup);

    //! Makes a constant interval leaf equal to ]-oo;+oo[.
    builder(top t);

    //! Makes a dimension leaf (i.e., variable of specified index).
    builder(dim d);

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


    /* C-level compatibility */
    /* ===================== */

    /** @name C API compatibility */
    //@{

    //! Returns a pointer to the internal APRON object stored in *this.
    ap_texpr0_t* get_ap_texpr0_t();

    //! Returns a pointer to the internal APRON object stored in *this.
    const ap_texpr0_t* get_ap_texpr0_t() const;

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
     * multiplication (\c AP_TEXPR_MUL), division (\c AP_TEXPR_DIV), modulo (\c AP_TEXPR_MOD), or power (\c AP_TEXPR_POW).
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


    //! Whether the expression is a single coefficient node with 0 value.
    bool is_zero() const;

  };



  /* print */
  /* ===== */

  /** @name Printing */
  //@{

  /*! \brief Printing.
   *
   * Variable naming can be configured through the varname stream modifier.
   */
  friend std::ostream& operator<< (std::ostream& os, const texpr0& s);

  //! Prints to a C stream.
  void print(char** name_of_dim = NULL, FILE* stream=stdout) const;

  //@}


  /* tests, size, dimensions */
  /* ======================= */

  /** @name Tests, size, dimensions */
  //@{


  //! Whether two expressions are syntactically, structurally equal.
  bool equal(const texpr0& x) const;

  //! Whether the expression is a single coefficient node with 0 value.
  bool is_zero() const;

  //! Returns the depth of the expression tree (counting only operator nodes).
  size_t depth() const;

  //! Returns the number of operator nodes in the expression tree.
  size_t size() const;

  //! Returns the maximal dimension occurring in the expression (or 0 if there is no variable).
  ap_dim_t max_dim() const;

  //! Whether the given dimension occurs in the expression.
  bool has_dim(ap_dim_t d) const;

  /*! \brief Returns a list of all dimensions occurring in the expression
   * (in strict increasing order)
   */
  std::vector<ap_dim_t> dimlist() const;

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

  //! Substitutes each occurrence of dim with (a fresh copy of) dst.
  void substitute(ap_dim_t dim, const texpr0& dst);

#if 0
  /*! \brief Evaluates the expression given an abstract environment.
   *
   * \arg \c discr whether to evaluate using double (\c AP_SCALAR_DOUBLE), MPQ (\c AP_SCALAR_MPQ) or MPFR (\c AP_SCALAR_MPFR).
   * \arg \c pexact if not NULL, sets to true whenever the evaluation was exact
   * (i.e., not over-approximated).
   */
  interval eval(manager& m, const abstract0& a, ap_scalar_discr_t discr=AP_SCALAR_DOUBLE,
		bool* pexact=NULL) const;

  linexpr0 intlinearize(manager& m, const abstract0& a, ap_scalar_discr_t discr=AP_SCALAR_DOUBLE,
			bool quasilinearize=false, bool* pexact=NULL) const;
#endif

  //! Returns a hash code.
  long hash() const;

  //@}

  /* change of dimension */
  /* =================== */

  /** @name Changes of dimension */
  //@{

  //! Adds some dimensions, shifting dimension if needed.
  void add_dimensions(const dimchange& d);

  /*! \brief Removes some dimensions, shifting dimension if needed.
   *
   * Nodes corresponding to deleted dimensions are replaced with ]-oo;+oo[.
   */
  void remove_dimensions(const dimchange& d);

  //! Permutes dimensions.
  void permute_dimensions(const dimperm& d);

  //@}


  /* C-level compatibility */
  /* ===================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_texpr0_t* get_ap_texpr0_t();

  //! Returns a pointer to the internal APRON object stored in *this.
  const ap_texpr0_t* get_ap_texpr0_t() const;

  //@}

};

#include "apxx_texpr0_inline.hh"

}

#endif /* __APXX_TEXPR0_HH */
