/* -*- C++ -*-
 * apxx_box.hh
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

#ifndef __APXX_BOX_HH
#define __APXX_BOX_HH

#include "apxx_manager.hh"

#include "box.h"

namespace apron {

//! Manager factory for the Box interval-domain library.
class box_manager : public manager {

public:

  //! \brief Creates a new manager.
  box_manager();

  //! Copy operator.
  manager& operator=(const manager&);
};

#include "apxx_box_inline.hh"

}

#endif /* __APXX_BOX_HH */
