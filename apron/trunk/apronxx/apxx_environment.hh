/* -*- C++ -*-
 * apxx_environment.hh
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

#ifndef __APXX_ENVIRONMENT_HH
#define __APXX_ENVIRONMENT_HH

#include <string>
#include <vector>

#include "ap_environment.h"
#include "apxx_dimension.hh"
#include "apxx_var.hh"


namespace apron {


/* ================================= */
/* environment                       */
/* ================================= */

/*! \brief Level 1 environment (ap_environment_t wrapper).
 *
 * An environment associates dimensions to integer and real variable names (var).
 * It stores a number intdim of integer dimensions, a number realdim of real dimensions, and
 * an array of size intdim+realdim variable names indexed by dimensions.
 * Dimensions 0 to intdim-1 are associated to integer variables while dimensions intdim to intdim+realdim-1
 * are associated to real variables.
 * Variables in both slices [0;intdim-1] and [intdim;intdim+realdim-1] are always sorted in
 * strictly increasing order.
 * (Recall that variables have the following user-specified functions: copy, deallocation, comparison, conversion to string.)
 * This invariant is automatically enforced by the library by performing dimension permutations after
 * variable name addition, deletion, or renaming. Thus, such an operation can change the dimension associated to existing
 * variable names.
 *
 * Environments are reference-counted and the API is fully functional (i.e., functions do not change *this but return
 * a new object instead).
 */
class environment : public use_malloc {

protected:

  ap_environment_t* e;

  //! Used internally only. Wraps the APRON object in an environment object (no copy, no change in reference counter).
  environment(ap_environment_t* x) : e(x) {}

  friend class linexpr1;
  friend class lincons1;
  friend class lincons1_array;
  friend class generator1;
  friend class generator1_array;
  friend class texpr1;
  friend class tcons1;
  friend class tcons1_array;
  friend class abstract1;

public:


  /* constructors */
  /* ============ */


  /** @name Constructors */
  //@{

  //! Creates a new empty environment.
  environment();

  /*! \brief Creates a new environment with given integer and real variable names (copied).
   *
   * \throw std::invalid_argument if variable names are not distinct.
   */
  environment(const std::vector<var>& intdim, const std::vector<var>& realdim);

  /*! \brief Creates a new environment with given integer and real variable names (copied).
   *
   * \throw std::invalid_argument if variable names are not distinct.
   */
  environment(const var* intdim, size_t intdim_size, const var* realdim, size_t realdim_size);

  //! Creates a copy (i.e., increments the reference counter).
  environment(const environment& x);

  //@}


  /** @name Destructor */
  //@{

  //! Decrements the reference counter and, if null, actually frees the environment.
  ~environment();

  //@}


  /* assignment */
  /* ========== */

  /** @name Assignment */
  //@{

  //! Assignment (reference counter manipulation).
  const environment& operator=(const environment& x);

  //@}


  /* operations */
  /* ========== */

  /** @name Operations */
  //@{

  /*! \brief Returns a copy of the environment with some integer and/or real variable names added.
   *
   * \throw std::invalid_argument if the resulting environment has duplicate variable names.
   */
  environment add(const std::vector<var>& intdim, const std::vector<var>& realdim) const;

  /*! \brief Returns a copy of the environment with intdim_size integer and realdim_size real variable names added.
   *
   * \throw std::invalid_argument if the resulting environment has duplicate variable names.
   */
  environment add(const var* intdim, size_t intdim_size, const var* realdim, size_t realdim_size) const;

  /*! \brief Returns a copy of the environment with some integer and/or real variable names added.
   *
   * Sets perm to the dimension permutation that has been performed after appending variable names at the end of
   * integer (resp. real) dimensions to get them sorted again.
   *
   * \throw std::invalid_argument if the resulting environment has duplicate variable names.
   */
  environment add(const std::vector<var>& intdim, const std::vector<var>& realdim, dimperm& perm) const;

  /*! \brief Returns a copy of the environment with intdim_size integer and realdim_size real variable names added.
   *
   * Sets perm to the dimension permutation that has been performed after appending variable names at the end of
   * integer (resp. real) dimensions to get them sorted again.
   *
   * \throw std::invalid_argument if the resulting environment has duplicate variable names.
   */
  environment add(const var* intdim, size_t intdim_size, const var* realdim, size_t realdim_size, dimperm& perm) const;

  /*! \brief Returns a copy of the environment with some variable names removed.
   *
   * \throw std::invalid_argument if some variable name does not exist.
   */
  environment remove(const std::vector<var>& dims) const;

  /*! \brief Returns a copy of the environment with dims_size variable names removed.
   *
   * \throw std::invalid_argument if some variable name does not exist.
   */
  environment remove(const var* dims, size_t dims_size) const;

  /*! \brief Returns a copy of the environment with some variable names substituted.
   *
   * dims[i].first is replaced with dims[i].second.
   *
   * \throw std::invalid_argument if some variable name does not exist.
   */
  environment rename(const std::vector<std::pair<var,var> >& dims) const;

  /*! \brief Returns a copy of the environment with dims_size variable names substituted.
   *
   * before[i] is replaced with after[i]. Both array should contain dims_size elements.
   *
   * \throw std::invalid_argument if some variable name does not exist.
   */
  environment rename(const var* before, const var* after, size_t dims_size) const;

  /*! \brief Returns a copy of the environment with some variable names substituted.
   *
   * dims[i].first is replaced with dims[i].second.
   * Sets perm to the dimension permutation that has been performed after renaming the variable names in-place to get them 
   * sorted again.
   *
   * \throw std::invalid_argument if some variable name does not exist.
   */
  environment rename(const std::vector<std::pair<var,var> >& dims, dimperm& perm) const;

  /*! \brief Returns a copy of the environment with dims_size variable names substituted.
   *
   * before[i] is replaced with after[i]. Both array should contain dims_size elements.
   * Sets perm to the dimension permutation that has been performed after renaming the variable names in-place to get them 
   * sorted again.
   *
   * \throw std::invalid_argument if some variable name does not exist.
   */
  environment rename(const var* before, const var* after, size_t dims_size, dimperm& perm) const;

  /*! \brief Returns the least common environment of two environments.
   *
   * \throw std::invalid_argument if some variable name has different type in both environments
   */
  friend environment lce(const environment& x, const environment& y);

  /*! \brief Returns the least common environment of two environments.
   *
   * Sets chgx (resp. chgy) to the dimension transformation that has been performed after appending the required 
   * variable names to x (resp. y) to keep them sorted.
   *
   * \throw std::invalid_argument if some variable name has different type in both environments
   */
  friend environment lce(const environment& x, const environment& y, dimchange& chgx, dimchange& chgy);

  /*! \brief Returns the least common environment of a vector of environments.
   *
   * \throw std::invalid_argument if some variable name has different type in different environments
   */
  friend environment lce(const std::vector<environment>& x);

  /*! \brief Returns the least common environment of an array env of env_size environments.
   *
   * \throw std::invalid_argument if some variable name has different type in different environments
   */
  friend environment lce(const environment* env, size_t env_size);

  /*! \brief Returns the least common environment of a vector of environments.
   *
   * Sets chg[i] to the dimension transformation that has been performed after appending the required variable names
   * to x[i] to keep them sorted.
   * chg must have space for at least env_size elements.
   *
   * \throw std::invalid_argument if some variable name has different type in different environments
   */
  friend environment lce(const std::vector<environment>& x, std::vector<dimchange>& chg);

  /*! \brief Returns the least common environment of an array env of env_size environments.
   *
   * Sets chg[i] to the dimension transformation that has been performed after appending the required variable names
   * to x[i] to keep them sorted.
   * chg must have space for at least env_size elements.
   *
   * \throw std::invalid_argument if some variable name has different type in different environments
   */
  friend environment lce(const environment* env, dimchange* chg, size_t env_size);

  /*! \brief Returns the dimension change to transform an environment x into a super-environment y.
   *
   * \throw std::invalid_argument if x is not a subset of y, or if x and chg have different size.
   */
  friend dimchange get_dimchange(const environment& x, const environment& y);

  //@}


  /* access */
  /* ====== */

  /** @name Accesses */
  //@{

  //! Returns the number of integer dimensions in the environment.
  size_t intdim() const;

  //! Returns the number of real dimensions in the environment.
  size_t realdim() const;

  //! Whether the environment contains the variable name x.
  bool contains(const var& x) const;

  /*! \brief Returns the dimension associated with the variable name.
   *
   * \throw std::invalid_argument if there is no such variable name in the environment.
   */
  ap_dim_t operator[] (const var& x) const;

  //! \brief Returns the variable name at a given dimension (no bound-check).
  const var& operator[] (ap_dim_t d) const;

  /*! \brief Returns the dimension associated with a variable name.
   *
   * \throw std::invalid_argument if there is no such variable name in the environment.
   */
  ap_dim_t get_dim(const var& x) const;

  /*! \brief Returns the variable name at a given dimension (bound-checked).
   *
   * \throw std::out_of_range if the dimension is larger than the environment size.
   */
  const var& get_var(ap_dim_t d) const;

  //! Returns (a copy of) the full map from dimensions to variable names.
  std::vector<var> get_vars() const;


  //@}


  /* Tests */
  /* ===== */

  /** @name Tests */
  //@{

  //! Equality testing.
  friend bool operator==(const environment& x, const environment& y);

  //! Disequality testing.
  friend bool operator!=(const environment& x, const environment& y);

  //! Inclusion testing.
  friend bool operator<=(const environment& x, const environment& y);

  //! Inclusion testing.
  friend bool operator>=(const environment& x, const environment& y);

  /*! \brief Environment comparison.
   *
   * \return 
   *  - -2 if some variable name has different type in both environments
   *  - -1 if x is a sub-environment of y
   *  -  0 if x equals y
   *  - +1 if x is a super-environment of y
   *  - +2 if x and y a not comparable but there exists a lowest upper bound
   */
  friend int cmp(const environment& x, const environment& y);

  //@}


  /* print */
  /* ===== */

  /** @name Printing */
  //@{

   //! Printing.
  friend std::ostream& operator<< (std::ostream& os, const environment& s); 

  //! Prints to a C stream.  
  void print(FILE* stream=stdout) const; 

  //@}


  /* C API compatibility */
  /* =================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object pointed by *this.
  const ap_environment_t* get_ap_environment_t() const;

  //! Returns a pointer to the internal APRON object pointed by *this.
  ap_environment_t* get_ap_environment_t();

  //@}

};

#include "apxx_environment_inline.hh"

}

#endif /* __APXX_ENVIRONMENT_HH */
