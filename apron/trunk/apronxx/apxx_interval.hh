/* -*- C++ -*-
 * apxx_interval.hh
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

#ifndef __APXX_INTERVAL_HH
#define __APXX_INTERVAL_HH

#include <vector>
#include "ap_interval.h"
#include "apxx_scalar.hh"


namespace apron {

//! Full interval (]-oo,+oo[) or domain, to simplify initialisations and assignments.
struct top { 
  top() {} 
};


//! Empty interval or domain, to simplify initialisations and assignments.
struct bottom {
  bottom() {} 
};


/* ================================= */
/* interval                          */
/* ================================= */


/*! \brief Interval (ap_interval_t wrapper).
 *
 * An interval is represented using two scalar bounds (of double or MPQ type).
 */
class interval : public use_malloc {

protected:
  ap_interval_t c; //!< Structure managed by APRON.

  //! Internal initialisation.
  void init();

  //! Internal use only. Take ownership of an object created by ap_interval_alloc.
  interval(ap_interval_t* i);

  friend class abstract0;
  friend class abstract1;
  friend class linexpr0;
  friend class texpr0;

public:
  
  //! Returned by ordering functions.
  enum order { 
    INCLUDED = -1, //!< Means i1 is included in i2 
    EQUAL    =  0, //!< Means i1 equals i2 
    CONTAINS =  1, //!< Means i2 is included in i1
    LESS     = -2, //!< Means min i1 <= min i2
    GREATER  =  2, //!< Means min i1 >= min i2
  };


  /* constructors */
  /* ============ */

  /** @name Constructors */
  //@{

  //! Makes a new interval [0,0] with double bounds.
  interval();
  
  //! Makes a copy of an interval (bounds are copied).
  interval(const interval& x);

  //! Makes a new interval with the specified scalar bounds (copied).
  interval(const scalar& inf, const scalar& sup);

  //! Makes a new interval with the specified integer bounds, using MPQ scalars.
  interval(int inf, int sup);

  //! Makes a new interval with the specified integer bounds, using MPQ scalars.
  interval(long inf, long sup);

  //! Makes a new interval with the specified double bounds, using double scalars.
  interval(double inf, double sup);

  //! Makes a new interval with the specified fraction bounds, using MPQ scalars.
  interval(const frac& inf, const frac& sup);

  //! Makes a new interval with the specified MPQ bounds (copied), using MPQ scalars.
  interval(const mpq_class& inf, const mpq_class& sup);

  //! Makes a new interval with the specified MPFR bounds (copied), using MPFR scalars.
  interval(mpfr_t inf, mpfr_t sup);

  //! Makes a new interval representing ]-oo,+oo[.
  interval(top t);

  //! Makes a new empty interval [+1;-1].
  interval(bottom t);

  //@}


  /* destructor */
  /* ========== */

  /** @name Destructor */
  //@{

  ~interval();

  //@}


  /* assignments */
  /* =========== */

  /** @name Assignments */
  //@{

  /* = */

  //! Copies an interval into *this
  interval& operator= (const interval& x);

  //! Sets *this to top (]-oo;+oo[), no change in scalar types.
  interval& operator= (top t);

  //! Sets *this to empty ([+1;-1]), no change in scalar types.
  interval& operator= (bottom t);


  /* set */

  //! Copies an interval into *this. \return *this.
  interval& set(const interval& x);

  //! Sets *this to [inf;sup], copying both scalar bounds. \return *this.
  interval& set(const scalar& inf, const scalar& sup);

  //! Sets *this to [inf;sup], using MPQ scalar bounds. \return *this.
  interval& set(int inf, int sup);

  //! Sets *this to [inf;sup], using MPQ scalar bounds. \return *this.
  interval& set(long inf, long sup);

  //! Sets *this to [inf;sup], using double scalar bounds. \return *this.
  interval& set(double inf, double sup);

  //! Sets *this to [inf;sup], using MPQ scalar bounds. \return *this.
  interval& set(const frac& inf, const frac& sup);

  //! Sets *this to [inf;sup], using MPQ scalar bounds (copied). \return *this.
  interval& set(const mpq_class& inf, const mpq_class& sup);

  //! Sets *this to [inf;sup], using MPFR scalar bounds (copied). \return *this.
  interval& set(mpfr_t inf, mpfr_t sup);

  //! Sets *this to top (]-oo;+oo[). \return *this.
  interval& set(top t);

  //! Sets *this to empty ([+1;-1]). \return *this.
  interval& set(bottom t);


  /* swap */

  //! Swaps the contents of two intervals.
  friend void swap(interval& a, interval &b);

  //@}


  /* access */
  /* ====== */

  /** @name Accesses */
  //@{

  //! Gets a (modifiable) reference to the lower bound.
  scalar& get_inf();
  
  //! Gets a (modifiable) reference to the upper bound.
  scalar& get_sup();

  //! Gets a reference to the lower bound.
  const scalar& get_inf() const;
  
  //! Gets a reference to the upper bound.
  const scalar& get_sup() const;

  //@}


  /* print */
  /* ===== */

  /** @name Printing */
  //@{

  //! Printing.
  friend std::ostream& operator<< (std::ostream& os, const interval& s);

  //! Prints to a C stream.
  void print(FILE* stream=stdout) const;

  //@}


  /* tests */
  /* ===== */
 
  /** @name Tests */
  //@{

  //! Whether *this equals ]-oo;+oo[.
  bool is_top() const;

  //! Whether *this represents an empty interval.
  bool is_bottom() const;

  //! Set inclusion.
  friend bool operator<= (const interval&a, const interval &b);

  //! Set inclusion.
  friend bool operator>= (const interval&a, const interval &b);

  //! Strict set inclusion.
  friend bool operator< (const interval&a, const interval &b);

  //! Strict set inclusion.
  friend bool operator> (const interval&a, const interval &b);

  //! Set equality.
  friend bool operator== (const interval&a, const interval &b);

  //! Set disequality.
  friend bool operator!= (const interval&a, const interval &b);

  //! Interval ordering.
  friend order cmp (const interval&a, const interval &b);

  //@}

 
  /* other operators */
  /* =============== */

  /** @name Operators */
  //@{

  //! Negates *this.
  void neg();

  //! Returns a new interval which is the opposite of *this.
  interval operator- () const;

  //! Returns a hash code.
  long hash() const;

  //@}


  /* C API compatibility */
  /* =================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object stored in *this.
  const ap_interval_t* get_ap_interval_t() const;

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_interval_t* get_ap_interval_t();

  //@}
};




/* ================================= */
/* interval_array                    */
/* ================================= */


/*! \brief array of interval(s).
 */
class interval_array : public use_malloc {

protected:

  size_t sz;           //!< Array size.
  ap_interval_t** c;   //!< Array of pointers to intervals.

  //! Internal use only. Reference an array created with ap_interval_array_alloc.
  interval_array(size_t size, ap_interval_t** c);

  friend class abstract0;
  friend class abstract1;

public:
  
  /* constructors */
  /* ============ */

  /** @name Constructors */
  //@{

  /*! \brief Creates a new interval array.
   *
   * All elements are initially [0;0] intervals with double scalar bounds.
   */
  interval_array(size_t size);

  //! Makes a copy of an interval array (copying all elements).
  interval_array(const interval_array &x);

  //! Makes a interval array from an interval vector (copying all elements).
  interval_array(const std::vector<interval>& x);

  //! Makes a interval array from an interval array of give size (copying all elements).
  interval_array(size_t size, const interval x[]);

  //@}


  /* destructor */
  /* ========== */

  /** @name Destructor */
  //@{

  //! Frees the space occupied by the array and all its elements.
  ~interval_array();

  //@}


  /* assignment */
  /* ========== */

  /** @name Assignments */
  //@{

  /*! \brief Copies an interval array into *this.
   *
   * All elements are copied and the array size is updated if necessary.
   */
  interval_array& operator= (const interval_array &x);

  /*! \brief Copies an interval vector into *this.
   *
   * All elements are copied and the array size is updated if necessary.
   */
  interval_array& operator= (const std::vector<interval>& x);

 /*! \brief Copies an interval array into *this.
   *
   * The size of the interval_array is not changed.
   * \c x should have enough elements to fill the interval_array.
   */
  interval_array& operator= (const interval x[]);

  //@}


  /* conversion */
  /* ========== */

  /** @name Conversion */
  //@{

  //! \brief Makes an interval vector from an interval array (copying all elements).
  operator std::vector<interval>() const;

  //@}


  /* print */
  /* ===== */

  /** @name Printing */
  //@{

  //! Printing.
  friend std::ostream& operator<< (std::ostream& os, const interval_array& s);

  //! Prints to a C stream.
  void print(FILE* stream=stdout) const;

  //@}


  /* access */
  /* ====== */

  /** @name Accesses */
  //@{

  //! Returns the array size.
  size_t size() const;

  //! Returns a pointer to the start of the array of elements used internally.
  interval** contents();

  //! Returns a (modifiable) reference to an element, no bound checking.
  interval& operator[](size_t i);

  //! Returns a reference to an element, no bound checking.
  const interval& operator[](size_t i) const;

  /*! Returns a (modifiable) reference to an element (bound-checked).
   *
   * \throw std::out_of_range if the index is invalid.
   */
  interval& get(size_t i);
 
  /*! Returns a reference to an element (bound-checked).
   *
   * \throw std::out_of_range if the index is invalid.
   */
  const interval& get(size_t i) const;

  //@}


  /* C-level compatibility */
  /* ===================== */

  /** @name C API compatibility */
  //@{

 //! Returns a pointer to the internal APRON object stored in *this.
  const ap_interval_t * const * get_ap_interval_t_array() const;

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_interval_t** get_ap_interval_t_array();

  //@}
};


#include "apxx_interval_inline.hh"

}

#endif /* __APXX_INTERVAL_HH */
