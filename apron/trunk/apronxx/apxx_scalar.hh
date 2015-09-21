/* -*- C++ -*-
 * apxx_scalar.hh
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

#ifndef __APXX_SCALAR_HH
#define __APXX_SCALAR_HH

#include <stdlib.h>
#include <iostream>

#include "ap_scalar.h"
#include "gmpxx.h"


namespace apron {


//! Either +oo or -oo, to simplify initialisations and assignments.
struct infty {

  int sgn;  //!< Sign: >0 for +oo, <0 for -oo.

  //! \arg \c sgn: >0 for +oo, <0 for -oo.
  infty(int sgn=1) : sgn(sgn) {}; 

};


//! A fraction with native int coefficients, to simplify initialisations and assignments.
struct frac {

  long num;          //!< Numerator.
  unsigned long den; //!< Denominator.

  /*! \arg \c num: Numerator.
   *  \arg \c den: Denominator.
   */
  frac(long num, unsigned long den=1) : num(num), den(den) {}

};


//! Exception raised when an operation is incompatible with a discriminant.
class bad_discriminant : public std::logic_error {
public:
  /*! \arg \c arg: Reason of failure.
   */
  bad_discriminant(const std::string &arg) : logic_error(arg) {}
};


/* ================================= */
/* use_malloc                        */
/* ================================= */


//! Inherited by most wrappers to map new and delete to malloc and free.
struct use_malloc {
  void* operator new     (size_t sz);
  void* operator new[]   (size_t sz);
  void  operator delete  (void* p);
  void  operator delete[](void* p);
};



/* ================================= */
/* scalar                            */
/* ================================= */


/*! \brief Scalar (ap_scalar_t wrapper).
 *
 * A scalar object represents a (possibly +oo or -oo) number, using either a double
 * or a MPQ (multi-precision fraction).
 * All operations are exact unless explicitly noted.
 */
class scalar : public use_malloc {
  
protected:
  ap_scalar_t c; //!< Structure managed by APRON.
  
public:

  //! Returned by ordering functions.
  enum order { 
    LESS    = -1, //!< Means less than, negative, or -oo (depending on context).
    EQUAL   =  0, //!< Means equal or null (depending on context).
    GREATER =  1, //!< Means greater than, positive, or +oo (depending on context).
  };


  /* constructors */
  /* ============ */

  /** @name Constructors */
  //@{

  //! Makes a double scalar equal to 0.0.
  scalar();

  //! Makes a MPQ scalar from a native integer.
  scalar(int x);

  //! Makes a MPQ scalar from a native integer.
  scalar(long x);

  //! Makes a double scalar from a double.
  scalar(double x);

  //! Makes a MPQ scalar from a fraction with native integer coefficients.
  scalar(const frac& x);

  //! Makes a double scalar equal to +oo or -oo.
  scalar(infty x);
  
  //! Makes a MPQ scalar from a MPQ (copied).
  scalar(const mpq_class& x);

  //! Makes a MPFR scalar from a MPFR number (copied).
  scalar(mpfr_t x);

  //! Makes a copy of a scalar.
  scalar(const scalar& x);

  //@}


  /* destructor */
  /* ========== */
  
  /** @name Destructor */
  //@{

  ~scalar();

  //@}


  /* assignments */
  /* =========== */

  /** @name Assignments */
  //@{

  //! Assigns a native integer to *this, setting its type to MPQ.
  scalar& operator= (int x);

  //! Assigns a native integer to *this, setting its type to MPQ.
  scalar& operator= (long x);

  //! Assigns a double to *this, setting its type to double.
  scalar& operator= (double x);

  //! Assigns a fraction to *this, setting its type to MPQ.
  scalar& operator= (const frac& x);

  //! Assigns +oo or -oo to *this, no change in scalar type.
  scalar& operator= (infty x);

  //! Assigns a MPQ to *this (copied), setting its type to MPQ.
  scalar& operator= (const mpq_class& x);

  //! Assigns a MPFR to *this (copied), setting its type to MPFR.
  scalar& operator= (mpfr_t x);

  //! Sets the type and value of *this to that of x.
  scalar& operator= (const scalar& x);

  //! Swaps the contents (type and value) of two scalars.
  friend void swap(scalar& a, scalar &b);

  //@}


  /* access */
  /* ====== */

  /** @name Accesses */
  //@{

  /*! \brief  Whether the object wraps a double or a MPQ.
   *
   * \return Either \c AP_SCALAR_DOUBLE, \c AP_SCALAR_MPQ, or \c AP_SCALAR_MPFR.
   */
  ap_scalar_discr_t get_discr() const;
    
  /*! \brief Returns a (modifiable) reference to the double contained in the scalar.
   *
   * \throw bad_discriminant when the scalar is not of double type.
   */
  double& get_double();

  /*! \brief  Returns a reference to the double contained in the scalar.
   *
   *\throw bad_discriminant when the scalar is not of double type.
   */
  const double& get_double() const;

  /*! \brief  Returns a (modifiable) reference to the MPQ contained in the scalar.
   *
   * \throw bad_discriminant when the scalar is not of MPQ type.
   */
  mpq_class& get_mpq();

  /*! \brief  Return a reference to the MPQ contained in the scalar.
   *
   *\throw bad_discriminant when the scalar is not of MPQ type.
   */
  const mpq_class& get_mpq() const;

  //@}


  /* conversion */
  /* ========== */
  
  /** @name Conversions */
  //@{

  /*! \brief Returns a new MPQ containing the (possibly converted) value of the scalar.
   *
   * \arg \c round: rounding direction, may be GMP_RNDN, GMP_RNDZ, GMP_RNDU, GMP_RNDD, or GMP_RND_MAX.
   * \arg \c conv is set to either LESS, EQUAL, or GREATER, depending on whether the result is
   * less than, equal to or greater than the value of *this.
   * \note Currently round is ignored and the conversion is always exact.
   */
  mpq_class to_mpq(mp_rnd_t round, order& conv) const;

  /*! \brief Returns a double containing the (possibly converted) value of the scalar.
   *
   * \arg \c round: rounding direction, may be GMP_RNDN, GMP_RNDZ, GMP_RNDU, GMP_RNDD, or GMP_RND_MAX.
   * \arg \c conv is set to either LESS, EQUAL, or GREATER, depending on whether the result is
   * less than, equal to or greater than the value of *this.
   */
  double to_double(mp_rnd_t round, order& conv) const;

 /*! \brief Copies the (possibly converted) value of the scalar into the MPFR.
   *
   * \arg \c round: rounding direction, may be GMP_RNDN, GMP_RNDZ, GMP_RNDU, GMP_RNDD, or GMP_RND_MAX.
   * \arg \c conv is set to either LESS, EQUAL, or GREATER, depending on whether the result is
   * less than, equal to or greater than the value of *this.
   */
  void to_mpfr(mpfr_t x, mp_rnd_t round, order& conv) const;

  //! Returns a new MPQ containing the (possibly converted) value of the scalar. No rounding.
  operator mpq_class() const;

  //! Returns a double containing the (possibly converted) value of the scalar. Rounding to nearest.
  operator double() const;

  //@}


  /* print */
  /* ===== */

  /** @name Printing */
  //@{

  //! Printing.
  friend std::ostream& operator<< (std::ostream& os, const scalar& s);

  //! Prints to a C stream.
  void print(FILE* stream=stdout) const;

  //@}


  /* tests */
  /* ===== */

  /** @name Tests */
  //@{

  //! Returns LESS if -oo, GREATER if +oo, EQUAL if finite.
  order is_infty() const;

  //! Returns LESS if negative, GREATER if positive, EQUAL if null.
  order sgn () const;

  //! Returns LESS if a<b, GREATER if a>b, EQUAL if a=b (total order).
  friend order cmp(const scalar& a, const scalar& b);

  friend bool operator>=(const scalar& a, const scalar& b);
  friend bool operator<=(const scalar& a, const scalar& b);
  friend bool operator> (const scalar& a, const scalar& b);
  friend bool operator< (const scalar& a, const scalar& b);
  friend bool operator==(const scalar& a, const scalar& b);
  friend bool operator!=(const scalar& a, const scalar& b);

  friend order cmp(const scalar& a, int b);
  friend bool operator>=(const scalar& a, int b);
  friend bool operator<=(const scalar& a, int b);
  friend bool operator> (const scalar& a, int b);
  friend bool operator< (const scalar& a, int b);
  friend bool operator==(const scalar& a, int b);
  friend bool operator!=(const scalar& a, int b);

  friend order cmp(int a, const scalar& b);
  friend bool operator>=(int a, const scalar& b);
  friend bool operator<=(int a, const scalar& b);
  friend bool operator> (int a, const scalar& b);
  friend bool operator< (int a, const scalar& b);
  friend bool operator==(int a, const scalar& b);
  friend bool operator!=(int a, const scalar& b);

  //@}


  /* other operators */
  /* =============== */

  /** @name Operators */
  //@{

  //! Negates *this.
  void neg();

  //! Returns the opposite of *this.
  scalar operator- () const;

  //! Inverses *this. Not exact for double type.
  void inv();

  //! Returns the inverse of *this. Not exact for double type.
  scalar operator~ () const;

  //! Returns a hash code.
  long hash() const;

  //@}


  /* C API compatibility */
  /* =================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object stored in *this.
  const ap_scalar_t* get_ap_scalar_t() const;

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_scalar_t* get_ap_scalar_t();

  //@}

};


#include "apxx_scalar_inline.hh"

}

#endif /* __APXX_SCALAR_HH */
