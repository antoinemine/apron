/* -*- C++ -*-
 * apxx_ppl_inline.hh
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

inline ppl_poly_manager::ppl_poly_manager(bool strict) 
  : manager(ap_ppl_poly_manager_alloc(strict)) 
{}

inline ppl_grid_manager::ppl_grid_manager()
  : manager(ap_ppl_grid_manager_alloc())
{}

inline manager& ppl_poly_manager::operator=(const manager& m)
{ 
  return manager::operator=(m); 
}

inline manager& ppl_grid_manager::operator=(const manager& m)
{ 
  return manager::operator=(m); 
}
