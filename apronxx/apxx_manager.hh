/* -*- C++ -*-
 * apxx_manager.hh
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

#ifndef __APXX_MANAGER_HH
#define __APXX_MANAGER_HH

#include <string>

#include "ap_manager.h"
#include "ap_abstract0.h"
#include "ap_abstract1.h"
#include "apxx_lincons0.hh"
#include "apxx_tcons0.hh"
#include "apxx_generator0.hh"

#include "box.h"
#include "pk.h"
#include "oct.h"

namespace apron {


/* =============== */
/* tbool           */
/* =============== */

/*! \brief 3-valued boolean (tbool_t wrapper).
 *
 * A tbool_t holds a value in 3-valued logic: \c tbool_true, \c tbool_false, 
 * or \c tbool_top (don't know).
 */
struct tbool {

public:
  
  tbool_t x; //!< Either \c tbool_false, \c tbool_true, or \c tbool_top.

  //! Converts true to \c tbool_true and false to \c tbool_false.
  tbool(bool a);

  //! Conversion from tbool_t.
  tbool(tbool_t a);

  //! Conversion to tbool_t.
  operator tbool_t();

  //! Returns true if and only if the value is \c tbool_true.
  operator bool();

  //! 3-valued logical or.
  friend tbool operator|| (tbool a, tbool b);

  //! 3-valued logical and.
  friend tbool operator&& (tbool a, tbool b);

  //! 3-valued logical not. 
  friend tbool operator! (tbool a);

  //! Printing.
  friend std::ostream& operator<<(std::ostream& os, tbool x);

  
};


/* =============== */
/* exceptions      */
/* =============== */


//! Exception raised when a timeout occurs.
class timeout : public std::logic_error {
public:
  /*! \arg \c x name of function in which the timeout occurred.
   */
  timeout(const std::string& x) : logic_error(x) {}
};


//! Exception raised when a function is not implemented.
class not_implemented : public std::logic_error {
public:
  /*! \arg \c x name of unimplemented function.
   */
  not_implemented(const std::string& x) : logic_error(x) {}
};


class abstract0;
class abstract1;


/* =============== */
/* manager         */
/* =============== */

/*! \brief Library manager (ap_manager_t wrapper).
 *
 * A manager is an instance of a numerical abstract domain library.
 * It has three roles:
 * - dispatch functions to the correct library (allowing multiple domain libraries to be used in a program),
 * - store instance-specific configuration options (such as precision/cost ratios) and feed-backs
 * (such as exactness and best flag),
 * - store instance-specific scratch space used by algorithms (allowing thread-safe operations without
 * too much allocation of function-local data).
 *
 * The following guidelines should be followed:
 * - a manager instance is required to create and manipulate abstract0 elements,
 * - an abstract0 element can be manipulated using any manager compatible with the one it 
 * was created with (that is, a manager for the same library, created using the same
 * parameter values, if any),
 * - two threads should always operate on distinct arguments, and using distinct managers
 * (if they operate concurrently).
 *
 * The manager class is an abstract base class for all managers. It has no effective 
 * constructor.
 * Instead, you should use the derived class corresponding to the numerical domain library you
 * wish to use.
 * 
 * Managers are reference-counted.
 * In particular, copying simply increment the reference counter and does not create an
 * independent manager with its own scratch space. Thus, a manager and its copy cannot be used by
 * concurrent threads.
 */
class manager : public use_malloc {
  
protected:

  ap_manager_t* m;  //!< Pointer managed by APRON.

  //! Internal use only (by subclasses). Initialise from a ap_manager_t* and take ownership (no copy).
  manager(ap_manager_t* m);
  
  friend class abstract0;
  friend class abstract1;

public:

  //! Internal use only. Translates APRON exceptions to C++ ones.
  static void raise(ap_manager_t* m, const char* msg, ap_abstract0_t* a = NULL);

  //! Internal use only. Translates APRON exceptions to C++ ones.
  static void raise(ap_manager_t* m, const char* msg, ap_abstract1_t a);
  
  //! Internal use only. Translates APRON exceptions to C++ ones.
  void raise(const char* msg, ap_abstract0_t* a = NULL);

  //! Internal use only. Translates APRON exceptions to C++ ones.
  void raise(const char* msg, ap_abstract1_t a);

  //! Internal use only. Whether APRON has raised an exception.
  bool exception_raised();


  /** @name Constructor */
  //@{

  /*! Increments the reference counter.
   *
   * \warning After the assignment, x and the new manager share the same scratch space, and thus,
   * cannot be used by concurrent threads.
   */
  manager(const manager& x);

  //@}


  /** @name Destructor */
  //@{
 
  //! Decrements the reference counter and, when reaching 0, frees the manager.
  virtual ~manager();

  //@}


  /** @name Assignment */
  //@{

  /*! \brief Assignment (actually performs some reference counter management).
   *
   * \warning After the assignment, x and *this share the same scratch space, and thus,
   * cannot be used by concurrent threads.
   */
  manager& operator= (const manager& x);

  //@}


  /** @name Accesses */
  //@{

  //! Returns the name of the library the manager comes from.
  std::string get_library() const;

  //! Returns the version name of the library the manager comes from.
  std::string get_version() const;

  /*! \brief Returns a (modifiable) reference to the options associated to some abstract function.
   *
   * \throw std::out_of_range if funid is no between \c AP_FUNID_COPY and \c AP_FUNID_CLOSURE (inclusive).
   */
  ap_funopt_t& get_funopt(ap_funid_t funid);

  /*! \brief Returns a (modifiable) reference to the user's preferred scalar type.
   *
   * \return the scalar type can be either \c AP_SCALAR_DOUBLE, \c AP_SCALAR_MPQ, or \c AP_SCALAR_MPFR.
   */
  ap_scalar_discr_t& get_scalar_discr();

  //! Returns the 'is exact' flag associated to the last computed abstract function.
  bool get_flag_exact();

  //! Returns the 'is best' flag associated to the last computed abstract function.
  bool get_flag_best();

  //@}

 
  /** @name Initialisation */
  //@{

  /*! \brief Sets the FPU rounding-mode towards +oo.
   *
   * This is needed to ensure the soundness of all double-manipulating functions in APRON
   * (coefficients or intervals with double bounds, floating-point abstract domains, etc.)
   *
   * The FPU rounding-mode change is global.
   * It affects all managers (already created or yet to be created) as well as all other floating-point
   * computations in the program.
   *
   * \throw std::runtime_error if the APRON was not able to set the rounding-mode.
   */
  static void fpu_init();

  //@}


   /** @name Printing */
  //@{
  
  //! Prints the library name and version.
  friend std::ostream& operator<< (std::ostream& os, const manager& s);
  
  //@}


  /* C-level compatibility */
  /* ===================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_manager_t* get_ap_manager_t();
  const ap_manager_t* get_ap_manager_t() const;

  //@}

};

#include "apxx_manager_inline.hh"

}

#endif /* __APXX_MANAGER_HH */
