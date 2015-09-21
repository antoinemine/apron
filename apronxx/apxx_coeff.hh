/* -*- C++ -*-
 * apxx_coeff.hh
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

#ifndef __APXX_COEFF_HH
#define __APXX_COEFF_HH

#include "ap_coeff.h"
#include "apxx_scalar.hh"
#include "apxx_interval.hh"


namespace apron {


/* ================================= */
/* coeff                             */
/* ================================= */


/*! \brief Coefficient (ap_coeff_t wrapper).
 *
 * A coeff is either a scalar or an interval.
 */
class coeff : public use_malloc {

protected:

  ap_coeff_t c;  //!< Structure managed by APRON.

public:


  /* constructors */
  /* ============ */

  /** @name Constructors */
  //@{

  //! Makes a double scalar coeff equal to 0.0.
  coeff();

  //! Makes a copy of a coeff.
  coeff(const coeff& x);

  //! Makes a scalar coeff from a scalar (copied).
  coeff(const scalar& x);

  //! Makes a MPQ scalar coeff from a native integer.
  coeff(int x);

  //! Makes a MPQ scalar coeff from a native integer.
  coeff(long x);

  //! Makes a double scalar coeff from a double.
  coeff(double x);

  //! Makes a MPQ scalar coeff from a fraction with native integer coefficients.
  coeff(const frac& x);

  //! Makes a MPQ scalar coeff from a MPQ (copied).
  coeff(const mpq_class& x);
  
  //! Makes a MPFR scalar coeff from a MPFR (copied).
  coeff(mpfr_t x);
  
  //! Makes an interval coeff from an interval (copied).
  coeff(const interval& x);

  //! Makes an interval coeff from two scalar bounds (copied).
  coeff(const scalar& inf, const scalar& sup);

  //! Makes an interval coeff with MPQ bounds from native integer bounds.
  coeff(int inf, int sup);

  //! Makes an interval coeff with MPQ bounds from native integer bounds.
  coeff(long inf, long sup);

  //! Makes an interval coeff with double bounds from double bounds.
  coeff(double inf, double sup);

  //! Makes an interval coeff with MPQ bounds from fractions with native integer coefficients.
  coeff(const frac& inf, const frac& sup);

  //! Makes an interval coeff with MPQ bounds from MPQ bounds (copied).
  coeff(const mpq_class& inf, const mpq_class& sup);

  //! Makes an interval coeff with MPFR bounds from MPFR bounds (copied).
  coeff(mpfr_t inf, mpfr_t sup);

  //! Makes an interval coeff representing ]-oo,+oo[.
  coeff(top t);

  //! Makes an empty interval coeff [+1;-1].
  coeff(bottom t);

  //@}


  /* destructor */
  /* ========== */

  /** @name Destructor */
  //@{

  ~coeff();

  //@}


  /* assignments */
  /* =========== */

  /** @name Assignments */
  //@{

  //! Copies the coeff into *this (setting its type and value).
  coeff& operator= (const coeff& x);
  
  //! Copies the scalar into *this, setting its type to scalar coeff.
  coeff& operator= (const scalar& x);

  //! Assigns a native integer to *this, setting its type to MPQ scalar coeff.
  coeff& operator= (int x);

  //! Assigns a native integer to *this, setting its type to MPQ scalar coeff.
  coeff& operator= (long x);

  //! Assigns a double to *this, setting its type to double scalar coeff.
  coeff& operator= (double x);

  //! Assigns a native integer fraction to *this, setting its type to MPQ scalar coeff.
  coeff& operator= (const frac& x);

  //! Assigns a MPQ to *this, setting its type to MPQ scalar coeff and copying the MPQ.
  coeff& operator= (const mpq_class& x);

  //! Assigns a MPFR to *this, setting its type to MPFR scalar coeff and copying the MPFR.
  coeff& operator= (mpfr_t x);

  //! Copies the interval into *this, setting its type to interval coeff.
  coeff& operator= (const interval& x);

  //! Set *this to the top interval (]-oo;+oo[), setting its type to interval coeff.
  coeff& operator= (top t);

  //! Set *this to the empty interval ([+1;-1])., setting its type to interval coeff.
  coeff& operator= (bottom t);

 //! Copies the coeff into *this (setting its type and value). \return *this.
  coeff& set(const coeff& x);

  //! Copies the scalar into *this, setting its type to scalar coeff. \return *this.
  coeff& set(const scalar& x);

  //! Assigns a native integer to *this, setting its type to MPQ scalar coeff. \return *this.
  coeff& set(int x);

  //! Assigns a native integer to *this, setting its type to MPQ scalar coeff. \return *this.
  coeff& set(long x);

  //! Assigns a double to *this, setting its type to double scalar coeff. \return *this.
  coeff& set(double x) ;

  //! Assigns a native integer fraction to *this, setting its type to MPQ scalar coeff. \return *this.
  coeff& set(const frac& x);

  //! Assigns a MPQ to *this, setting its type to MPQ scalar coeff and copying the MPQ. \return *this.
  coeff& set(const mpq_class& x);
  
  //! Assigns a MPFR to *this, setting its type to MPFR scalar coeff and copying the MPFR. \return *this.
  coeff& set(mpfr_t x);
  
  //! Copies the interval into *this, setting its type to interval coeff. \return *this.
  coeff& set(const interval& x);

  //! Sets *this to an interval coeff with the given scalar bounds (copied). \return *this.
  coeff& set(const scalar& inf, const scalar& sup);

  //! Sets *this to an interval coeff with MPQ scalar bounds. \return *this.
  coeff& set(int inf, int sup);

  //! Sets *this to an interval coeff with MPQ scalar bounds. \return *this.
  coeff& set(long inf, long sup);

  //! Sets *this to an interval coeff with double scalar bounds. \return *this.
  coeff& set(double inf, double sup);

  //! Sets *this to an interval coeff with MPQ scalar bounds. \return *this.
  coeff& set(const frac& inf, const frac& sup);
  
   //! Sets *this to an interval coeff with MPQ scalar bounds (copied). \return *this.
  coeff& set(const mpq_class& inf, const mpq_class& sup);

   //! Sets *this to an interval coeff with MPFR scalar bounds (copied). \return *this.
  coeff& set(mpfr_t inf, mpfr_t sup);

  //! Sets *this to the top interval (]-oo;+oo[), setting its type to interval coeff. \return *this.
  coeff& set(top t);

  //! Sets *this to the empty interval ([+1;-1])., setting its type to interval coeff. \return *this.
  coeff& set(bottom t);


  //! Swaps the contents and type of both coeff.
  friend void swap(coeff& a, coeff &b);

  //@}


  /* access */
  /* ====== */
  
  /** @name Accesses */
  //@{

  /*! \brief Whether the coeff is a scalar or an interval.
   *
   * \return Either \c AP_COEFF_SCALAR or \c AP_COEFF_INTERVAL.
   */
  ap_coeff_discr_t get_discr() const;

  /*! \brief Returns a (modifiable) reference to the scalar contained in the coeff.
   *
   * \throw bad_discriminant when the coeff is not of scalar type
   */
  scalar& get_scalar();

  /*! \brief Returns a reference to the scalar contained in the coeff.
   *
   * \throw bad_discriminant when the coeff is not of scalar type
   */
  const scalar& get_scalar() const;


  /*! \brief Returns a (modifiable) reference to the interval contained in the coeff.
   *
   * \throw bad_discriminant when the coeff is not of interval type
   */  
  interval& get_interval();

  /*! \brief Returns a reference to the interval contained in the coeff.
   *
   * \throw bad_discriminant when the coeff is not of interval type
   */
  const interval& get_interval() const;

  //@}


  /* print */
  /* ===== */

  /** @name Printing */
  //@{

 //! Printing.
  friend std::ostream& operator<< (std::ostream& os, const coeff& s);

  //! Prints to a C stream.
  void print(FILE* stream=stdout) const;
 
  //@}


  /* tests */
  /* ===== */

  /** @name Tests */
  //@{

  //! Whether the coeff is the scalar 0 or the interval [0;0].
  bool is_zero() const;

  //! Non-total comparison.
  friend int cmp(const coeff& a, const coeff& b);

  /*
  //! Set inclusion.
  friend bool operator<= (const coeff& a, const coeff& b);

  //! Set inclusion.
  friend bool operator>= (const coeff& a, const coeff& b);
  
  //! Strict set inclusion.
  friend bool operator< (const coeff& a, const coeff& b);
  
  //! Strict set inclusion.
  friend bool operator> (const coeff& a, const coeff& b);
  */

  //! Set equality.
  friend bool operator== (const coeff& a, const coeff& b);
  
  //! Set inequality.
  friend bool operator!= (const coeff& a, const coeff& b);

  //@}


  /* other operators */
  /* =============== */

  /** @name Operators */
  //@{

  //! Converts a singleton intervals to scalar.
  void reduce();

  //! Negates *this.
  void neg();

  //! Returns the opposite of *this.
  coeff operator- () const;

  //! Returns a hash code.
  long hash() const;

  //@}


  /* C-level compatibility */
  /* ===================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object stored in *this.
  const ap_coeff_t* get_ap_coeff_t() const;

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_coeff_t* get_ap_coeff_t();

  //@}
};

#include "apxx_coeff_inline.hh"

}

#endif /* __APXX_COEFF_HH */
