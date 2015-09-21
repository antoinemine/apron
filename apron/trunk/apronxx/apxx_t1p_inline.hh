/* -*- C++ -*-
 * apxx_t1p_inline.hh
 *
 * APRON Library / C++ inline functions
 *
 * DO NOT INCLUDE THIS FILE DIRECTLY
 *
 * Copyright (C) Antoine Mine' 2007
 *
 */
/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.
  
   Please read the COPYING file packaged in the distribution.
*/

inline t1p_manager::t1p_manager() 
  : manager(t1p_manager_alloc())
{}

inline manager& t1p_manager::operator=(const manager& m)
{ 
  return manager::operator=(m); 
}
