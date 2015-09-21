/* -*- C++ -*-
 * apxx_t1p.hh
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

#ifndef __APXX_T1P_HH
#define __APXX_T1P_HH

#include "apxx_manager.hh"

#include "t1p.h"

namespace apron {

//! Manager factory for the Taylor1plus zonotope library.
class t1p_manager : public manager {

public:

  /*! \brief Creates a new manager.
   *
   * \arg \c strict whether to allow strict inequalities as well as non-strict inequalities.
   *
   * Note that abstract values created with strict and non-strict managers 
   * are not compatible.
   */
  t1p_manager();

  //! Copy operator.
  manager& operator=(const manager&);
};

#include "apxx_t1p_inline.hh"

}

#endif /* __APXX_T1P_HH */
