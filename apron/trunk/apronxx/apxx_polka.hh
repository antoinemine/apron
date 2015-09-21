/* -*- C++ -*-
 * apxx_polka.hh
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

#ifndef __APXX_POLKA_HH
#define __APXX_POLKA_HH

#include "apxx_manager.hh"

#include "pk.h"

namespace apron {

//! Manager factory for the NewPolka polyhedra library.
class polka_manager : public manager {

public:

  /*! \brief Creates a new manager.
   *
   * \arg \c strict whether to allow strict inequalities as well as non-strict inequalities.
   *
   * Note that abstract values created with strict and non-strict managers 
   * are not compatible.
   */
  polka_manager(bool strict = false);

  //! Copy operator.
  manager& operator=(const manager&);
};

#include "apxx_polka_inline.hh"

}

#endif /* __APXX_POLKA_HH */
