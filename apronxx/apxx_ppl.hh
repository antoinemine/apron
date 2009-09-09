/* -*- C++ -*-
 * apxx_ppl.hh
 *
 * APRON Library / C++ class wrappers
 *
 * Copyright (C) Antoine Mine' 2007
 *
 */
/* This file is part of the APRON Library.
   As it includes both APRON and PPL files, it is released under GPL license.  
   Please read the ppl/COPYING file packaged in the distribution.
*/

#ifndef __APXX_PPL_HH
#define __APXX_PPL_HH

#include "apxx_manager.hh"

#include "ap_ppl.h"

namespace apron {

//! Manager factory for polyhedra domain using the PPL library.
class ppl_poly_manager : public manager {

public:

  /*! \brief Creates a new manager.
   *
   * \arg \c strict whether to allow strict inequalities as well as non-strict inequalities.
   *
   * Note that abstract values created with strict and non-strict managers 
   * are not compatible.
   */
  ppl_poly_manager(bool strict = false);

  //! Copy operator.
  manager& operator=(const manager&);
};

//! Manager factory for grid domain using the PPL library.
class ppl_grid_manager : public manager {

public:

  //! Creates a new manager.
  ppl_grid_manager();

  //! Copy operator.
  manager& operator=(const manager&);
};

#include "apxx_ppl_inline.hh"

}

#endif /* __APXX_PPL_HH */
