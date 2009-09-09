/* -*- C++ -*-
 * apxx_ppl_product_inline.hh
 *
 * APRON Library / C++ inline functions
 *
 * DO NOT INCLUDE THIS FILE DIRECTLY
 *
 * Copyright (C) Antoine Mine' 2007
 *
 */
/* This file is part of the APRON Library.
   As it includes both APRON and PPL files, it is released under GPL license.  
   Please read the ppl/COPYING file packaged in the distribution.
*/

inline pkgrid_manager::pkgrid_manager(bool strict) 
  : manager(ap_pkgrid_manager_alloc(strict))
{}

inline manager& pkgrid_manager::operator=(const manager& m)
{ 
  return manager::operator=(m); 
}
