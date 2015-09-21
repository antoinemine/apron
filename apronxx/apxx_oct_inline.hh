/* -*- C++ -*-
 * apxx_oct_inline.hh
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

inline oct_manager::oct_manager() 
  : manager(oct_manager_alloc()) 
{}

inline manager& oct_manager::operator=(const manager& m)
{ 
  return manager::operator=(m); 
}
