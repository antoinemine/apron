/* -*- C++ -*-
 * apxx_box_inline.hh
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

inline box_manager::box_manager() 
  : manager(box_manager_alloc()) 
{}

inline manager& box_manager::operator=(const manager& m)
{ 
  return manager::operator=(m); 
}
