/* -*- C++ -*-
 * apxx_polka_inline.hh
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

inline polka_manager::polka_manager(bool strict) 
  : manager(pk_manager_alloc(strict)) 
{}

inline manager& polka_manager::operator=(const manager& m)
{ 
  return manager::operator=(m); 
}
