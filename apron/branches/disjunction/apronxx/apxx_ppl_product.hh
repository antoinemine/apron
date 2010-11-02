/* -*- C++ -*-
 * apxx_ppl_product.hh
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

#ifndef __APXX_PPL_PRODUCT_HH
#define __APXX_PPL_PRODUCT_HH

#include "apxx_manager.hh"

#include "ap_pkgrid.h"

namespace apron {

//! Manager factory for reduced product domain of NewPolka polyhedra and PPL grids.
class pkgrid_manager : public manager {

public:

  /*! \brief Creates a new manager.
   *
   * \arg \c manpk should be a manager for (loose or strict) polyhedra
   * \arg \c manpplgrid should be a manager for PPL grids
   *
   * The argument managers are copied into the new manager.
   */
  pkgrid_manager(const polka_manager& manpk, const ppl_grid_manager& manpplgrid);  

  /*! \brief Creates a new manager.
   *
   * \arg \c strict whether to allow strict inequalities as well as non-strict inequalities in polyhedra.
   *
   * Note that abstract values created with strict and non-strict managers 
   * are not compatible.
   */
  pkgrid_manager(bool strict = false);


  //! Copy operator.
  manager& operator=(const manager&);
};

#include "apxx_ppl_product_inline.hh"

}

#endif /* __APXX_PPL_PRODUCT_HH */
