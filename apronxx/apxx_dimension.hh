/* -*- C++ -*-
 * apxx_dimension.hh
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

#ifndef __APXX_DIMENSION_HH
#define __APXX_DIMENSION_HH

#include "ap_dimension.h"


namespace apron {


/* ================================= */
/* varname                           */
/* ================================= */

/*! \brief Stream modifier to set variable names to dimensions.
 *
 * By default, all level 0 std::ostream printing functions output \c x0 to 
 * \c xn to refer to dimensions 0 to n.
 * By inserting this modifier into a stream, you can customize the variable
 * name.
 *
 * The information is local to the stream.
 *
 * The modifier is not used for level 1 printing functions as an environment
 * mapping dimensions to variable names is already available.
 * Also, the modifier does not affect the behavior of print functions
 * that take the mapping as an (optional) argument.
 */
class varname {

protected:

  //! Index to stream-local data, allocated with xalloc.
  static const int xindex;

  //! Names of variables.
  const std::vector<std::string>& names;

public:

  /*! \brief Creates a modifier to associate variable names to dimensions.
   *
   * \arg \c names[i] is the name to give to dimension i.
   *
   * If there are not enough names, printing functions will revert to
   * xi, xi+1... To disable variable names, simply pass an empty vector.
   *
   * A reference to \c names is kept by the object and will be passed to the
   * stream by the modifier. The stream will then make a deep copy and the
   * varname object and the original vector can be safely deleted.
   * 
   */
  varname(const std::vector<std::string>& names);

  //! Associates the modifier to the stream.
  template<class charT, class Traits>
  friend
  std::basic_ostream<charT,Traits>& 
  operator<<(std::basic_ostream<charT,Traits>& os, const varname& v);

  /*! \brief Gets the variable name vector associated to the stream
   *
   * \return a pointer to the vector formerly associated to the
   * stream, or NULL.
   */
  template<class charT, class Traits>
  friend
  std::vector<std::string>* get_varname(std::basic_ostream<charT,Traits>& os);

};




/* ================================= */
/* dimchange                         */
/* ================================= */


/*! \brief Dimension change object (ap_dimchange_t wrapper).
 *
 * dimchange objects are used to insert or remove dimensions at arbitrary positions in expressions, 
 * constraints, and domains.
 * A dimchange object embeds the number of integer and real dimensions to add/remove, as well as
 * an array of indices where to add/remove dimensions.
 * The array should be sorted in increasing order.
 */
class dimchange : public use_malloc {

protected:

  ap_dimchange_t c; //!< Structure managed by APRON.

public:

  /* constructor */
  /* =========== */

  /** @name Constructors */
  //@{

  //! Makes an uninitialized dimchange.
  dimchange(size_t intdim=0, size_t realdim=0);

  /*! \brief Makes a dimchange initialized using the given array of indices (copied).
   *
   * \arg \c d should contain (at least) intdim+realdim dimensions.
   */
  dimchange(size_t intdim, size_t realdim, const ap_dim_t d[]);

  /*! \brief Makes a dimchange initialized using the given vector of indices (copied).
   *
   * \exception std::invalid_argument if d contains less than intdim+realdim dimensions.
   */
  dimchange(size_t intdim, size_t realdim, const std::vector<ap_dim_t>& d);

  /*! \brief Makes a copy of a dimchange, copying the array, and optionally inverting the dimension change.
   *
   * \arg inv if inv==true, then the constructed dimchange is the inverse of x (see add_invert).
   */
  dimchange(const dimchange& x, bool inv=false);

  //@}


  /* destructor */
  /* ========== */

  /** @name Destructor */
  //@{

  ~dimchange();

  //@}
  

  /* assignment */
  /* ========== */

  /** @name Assignments */
  //@{

  //! (Deep) copy.
  dimchange& operator= (const dimchange& x);

  /*! \brief Assignment from an array of indices.
   *
   * \arg \c d should contain (at least) intdim+realdim dimensions.
   */
  dimchange& operator= (const ap_dim_t d[]);

  /*! \brief Assignment from a vector of indices (no change in size).
   *
   * \exception std::invalid_argument if \c d contains less than intdim+realdim dimensions.
   */
  dimchange& operator= (const std::vector<ap_dim_t>& d);

  //@}


  /* access */
  /* ====== */

  /** @name Accesses */
  //@{

  //! Returns the number of integer dimensions to add/remove.
  size_t get_intdim() const;

  //! Returns the number of real dimensions to add/remove.
  size_t get_realdim() const;

  /*! \brief Returns a (modifiable) reference to an index in the underlying array.
   *
   * \arg \c dim should be strictly smaller than intdim+realdim (not bound-checked).
   */
  ap_dim_t& operator[](size_t dim);

  /*! \brief Returns a  reference to a (checked) index in the underlying array.
   *
   * \arg \c dim should be strictly smaller than intdim+realdim (not bound-checked).
   */
  const ap_dim_t& operator[](size_t dim) const;

  /*! \brief Returns a (modifiable) reference to an index in the underlying array (bound-checked).
   *
   * \exception std::out_of_range is thrown if dim>=intdim+realdim.
   */
  ap_dim_t& get(size_t dim);

  /*! \brief Returns a reference to a (checked) index in the underlying array (bound-checked).
   *
   * \exception std::out_of_range is thrown if dim>=intdim+realdim.
   */
  const ap_dim_t& get(size_t dim) const;
  
  //@}


  /* print */
  /* ===== */

  /** @name Printing */
  //@{

  /*! \brief Printing.
   *
   * Variable naming can be configured through the varname stream modifier.
   */
  friend std::ostream& operator<< (std::ostream& os, const dimchange& s); 

  //! Prints to a C stream.  
  void print(FILE* stream=stdout) const;

  //@}



  /* other operators */
  /* =============== */

  /** @name Operators */
  //@{

  /*! \brief Inverts *this.
   * 
   * If the dimchange was used to add some dimensions, it can now be used to remove the
   * added dimensions (but not the converse).
   */
  void add_invert();

  /*! \brief Returns the inverse of *this.
   *
   * See \c add_invert.
   */
  dimchange operator- () const;

  //@}


  /* C-level compatibility */
  /* ===================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object stored in *this.
  const ap_dimchange_t* get_ap_dimchange_t() const;

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_dimchange_t* get_ap_dimchange_t();

  //@}
};



//! The identity permutation, to simplify initialisations and assignments.
struct id {
  size_t size; //!< Size of the permutation.

  id(size_t size) : size(size) {}
};



/* ================================= */
/* dimperm                           */
/* ================================= */


/*! \brief Dimension permutation object (ap_dimperm_t wrapper).
 *
 * dimperm objects can be used to permute dimensions in expressions, constraints, and domains.
 * They hold a map i->p[i]: [0,size-1]=>[0,size-1] using an array p of dimensions.
 * All the p[i] must be distinct and in the range [0,size-1] where size is the size of p.
 */
class dimperm : public use_malloc {

protected:

  ap_dimperm_t c;  //!< Structure managed by APRON.

public:


  /* constructor */
  /* =========== */

  /** @name Constructors */
  //@{

  //! Makes an uninitialized dimperm  of the given size.
  dimperm(size_t size=0);

  /*! \brief Makes a dimperm initialized with the array d.
   *
   * \arg \c d should contain (at least) size dimensions.
   */
  dimperm(size_t size, const ap_dim_t d[]);

  //! Makes a dimperm initialized with the vector d.
  dimperm(const std::vector<ap_dim_t>& d);


  //! Makes an identity permutation.
  dimperm(id t);

  /*! \brief Makes a copy of a permutation, optionally inverting it.
   *
   * \arg inv if inv==true, then the constructed permutation is the inverse of x.
   */
  dimperm(const dimperm& x, bool inv=false);

  //! Makes a new dimperm that is the composition of two permutations.
  dimperm(const dimperm& x, const dimperm& y);

  //@}


  /* destructor */
  /* ========== */

  /** @name Destructor */
  //@{

  ~dimperm();

  //@}
  

  /* assignment */
  /* ========== */

  /** @name Assignments */
  //@{

  //! Copies \c x.
  dimperm& operator= (const dimperm& x);

  //! Assigns the identity permutation to *this.
  dimperm& operator= (id t);

  /*! \brief Assigns the permutation from the array d to *this.
   *
   * \arg \c d should contain (at least) size dimensions.
   */
  dimperm& operator= (const ap_dim_t d[]);

  //! Assigns the permutation from the vector d to *this (changing its size).
  dimperm& operator= (const std::vector<ap_dim_t>& d);

  //@}


  /* access */
  /* ====== */

  /** @name Accesses, size */
  //@{

  //! Returns the size of the permutation.
  size_t size() const;

  /*! \brief Returns a (modifiable) reference to the image of dim.
   *
   * \arg \c dim should be strictly smaller than size (not bound-checked).
   */
  ap_dim_t& operator[](size_t dim);

  /*! \brief Returns a reference to the image of dim.
   *
   * \arg \c dim should be strictly smaller than size (not bound-checked).
   */
  const ap_dim_t& operator[](size_t dim) const;
 
  /*! \brief Returns a (modifiable) reference to the image of dim (bound-checked).
   *
   * \exception std::out_of_range is thrown if dim>=size.
   */
  ap_dim_t& get(size_t dim);
 
  /*! \brief Returns a reference to the image of dim (bound-checked).
   *
   * \exception std::out_of_range is thrown if dim>=size.
   */
  const ap_dim_t& get(size_t dim) const;

  //@}


  /* print */
  /* ===== */
  
  /** @name Printing */
  //@{

  /*! \brief Printing.
   *
   * Variable naming can be configured through the varname stream modifier.
   */
  friend std::ostream& operator<< (std::ostream& os, const dimperm& s); 

  //! Prints to a C stream.  
  void print(FILE* stream=stdout) const;

  //@}


  /* other operators */
  /* =============== */

  /** @name Operators */
  //@{

  /* composition */

  /*! \brief Composes *this with y.
   *
   * \exception std::invalid_argument if *this and y do not have the same size.
   */
  dimperm& operator*= (const dimperm& y);

  /*! \brief Returns a new dimperm that is the composition of x and y.
   *
   * \exception std::invalid_argument if x and y do not have the same size.
   */
  friend dimperm operator* (const dimperm& x, const dimperm& y);

 

  /* inversion */

  //! Inverts *this.
  void invert();

  //! Returns a new dimperm  that is the inverse of *this.
  dimperm operator- () const;

  //@}


  /* C-level compatibility */
  /* ===================== */

  /** @name C API compatibility */
  //@{

  //! Returns a pointer to the internal APRON object stored in *this.
  const ap_dimperm_t* get_ap_dimperm_t() const;

  //! Returns a pointer to the internal APRON object stored in *this.
  ap_dimperm_t* get_ap_dimperm_t();

  //@}

};

#include "apxx_dimension_inline.hh"

}

#endif /* __APXX_DIMENSION_HH */
