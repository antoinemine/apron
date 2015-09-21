/* -*- C++ -*-
 * apxx_oct.hh
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

#ifndef __APXX_OCT_HH
#define __APXX_OCT_HH

#include "apxx_manager.hh"

#include "oct.h"

namespace apron {

//! Manager factory for the Octagon domain library.
class oct_manager : public manager {

public:

  //! \brief Creates a new manager.
  oct_manager();

  //! Copy operator.
  manager& operator=(const manager&);
};

#include "apxx_oct_inline.hh"

}

#endif /* __APXX_OCT_HH */
