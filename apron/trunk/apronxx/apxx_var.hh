/* -*- C++ -*-
 * apxx_var.hh
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

#ifndef __APXX_VAR_HH
#define __APXX_VAR_HH

#include "ap_var.h"


namespace apron {


/* ================================= */
/* var                               */
/* ================================= */


/*! \brief Variable name (ap_var_t wrapper).
 *
 * A var object designates a variable name, using a void* pointer (i.e., ap_var_t).
 * Variable names replace dimensions in all level 1 operations.
 * Variable names can be copied, freed, compared, and converted to string.
 * The actual implementation of these operators is defined using the global pointer ap_var_operations.
 * By default, ap_var_operations implements dynamically allocated C-style 
 * NULL-terminated strings.
 */
class var {
  
protected:
  ap_var_t v; //! Variable name.
  
public:
  
  /* constructors */
  /* ============ */

  /** @name Constructors */
  //@{

  //! Makes a variable name from a pointer (copied with ap_var_operations->copy).
  var(const void* x);
  
   //! Makes a variable name from a string (copied with ap_var_operations->copy).
  var(const std::string& x);
  
  //! Copies the variable name (calls ap_var_operations->copy).
  var(const var& x);

  //@}


  /* destructor */
  /* ========== */

  /** @name Destructor */
  //@{
  
  //! Frees the variable name (calls ap_var_operations->free).
  ~var();
  
  //@}


  /* assignments */
  /* =========== */

  /** @name Assignments */
  //@{

  //! Assigns the variable name (calls ap_var_operations->copy, ->free).
  var& operator=(const var& x);
  
  //! Assigns the variable name (calls ap_var_operations->copy, ->free).
  var& operator=(const std::string& x);
  
  //! Assigns the variable name (calls ap_var_operations->copy, ->free).
  var& operator=(const void* x);
  
  //@}


  /* conversions */
  /* =========== */

  /** @name Conversions */
  //@{

  /*! \brief Converts to a C string (calls ap_var_operations->to_string).
   *
   * The C string should be freed with malloc by the caller.
   */
  operator char*() const;

  //! \brief Converts to a C++ string (calls ap_var_operations->to_string).
  operator std::string() const;

  //@}


  /* comparisons */
  /* =========== */

  /** @name Comparisons */
  //@{

  //! \brief Compares two variable names (calls ap_var_operations->to_compare).
  friend int  compare(const var& x, const var& y);

  //! \brief Compares two variables names (calls ap_var_operations->to_compare).
  friend bool operator==(const var& x, const var& y);

  //! \brief Compares two variable names (calls ap_var_operations->to_compare).
  friend bool operator!=(const var& x, const var& y);
 
  //! \brief Compares two variable names (calls ap_var_operations->to_compare).
  friend bool operator>=(const var& x, const var& y);

  //! \brief Compares two variable names (calls ap_var_operations->to_compare).
  friend bool operator<=(const var& x, const var& y);

  //! \brief Compares two variable names (calls ap_var_operations->to_compare).
  friend bool operator>(const var& x, const var& y);

  //! \brief Compares two variable names (calls ap_var_operations->to_compare).
  friend bool operator<(const var& x, const var& y);

  //@}


  /* print */
  /* ===== */

  /** @name Printing */
  //@{

  //! Printing.
  friend std::ostream& operator<< (std::ostream& os, const var& s);

  //! Prints to a C stream.
  void print(FILE* stream=stdout) const;

  //@}


  /* C-level compatibility */
  /* ===================== */

  /** @name C API compatibility */
  //@{

 //! \brief Returns a reference to the APRON object wrapped (no copy).
  const ap_var_t& get_ap_var_t() const;

  //! \brief Returns a (modifiable) reference to the APRON object wrapped (no copy).
  ap_var_t& get_ap_var_t();

  //@}

};

#include "apxx_var_inline.hh"

}


#endif /* __APXX_VAR_HH */
