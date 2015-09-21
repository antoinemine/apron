/* -*- C++ -*-
 * apxx_manager_inline.hh
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


/* =============== */
/* tbool_t         */
/* =============== */

inline tbool::tbool(bool a) : x(a ? tbool_true : tbool_false) 
{
}

inline tbool::tbool(tbool_t a) : x(a)
{
}

inline tbool::operator tbool_t()
{
  return x;
}

inline tbool::operator bool()
{ 
  return x==tbool_true; 
}

inline tbool operator|| (tbool a, tbool b)
{ 
  return tbool(tbool_or(a.x,b.x)); 
}
  
inline tbool operator&& (tbool a, tbool b)
{
  return tbool(tbool_and(a.x,b.x)); 
}

inline tbool operator! (tbool a)
{
  return tbool(tbool_not(a.x)); 
}

inline std::ostream& operator<<(std::ostream& os, tbool x)
{
  switch (x.x) {
  case tbool_true:  return os << "true";
  case tbool_false: return os << "false";
  case tbool_top:   return os << "top";
  default: throw std::invalid_argument("apron::operator<<(ostream&, tbool) invalid boolean");
  }
}



/* =============== */
/* manager         */
/* =============== */



inline manager::manager(ap_manager_t* m) : m(m)
{
  if (!m) throw std::invalid_argument("apron::manager::manager(ap_manager_t* m) m is NULL");
  // disable aborting as we manually check exception flags and throw C++ exceptions (see raise)
  for (size_t i=0; i<AP_EXC_SIZE; i++)
    m->option.abort_if_exception[i] = 0;
}

inline bool manager::exception_raised()
{
  return (m->result.exclog && m->result.exclog->exn!=AP_EXC_NONE);
}

// called at the end of every abstract function
inline void manager::raise(ap_manager_t* m, const char* msg, ap_abstract0_t* a)
{
  if (!m->result.exclog) return;

  std::string s = std::string(msg)+" : "+m->result.exclog->msg;
 
  switch (m->result.exclog->exn) {
    
  case AP_EXC_NONE:
    break;
    
  case AP_EXC_TIMEOUT:
    ap_manager_clear_exclog(m);
    if (a) ap_abstract0_free(m,a);
    throw timeout(s);
    break;
    
  case AP_EXC_OUT_OF_SPACE:
    ap_manager_clear_exclog(m);
    if (a) ap_abstract0_free(m,a);
    throw std::length_error(s);
    break;
    
  case AP_EXC_OVERFLOW:
    ap_manager_clear_exclog(m);
    if (a) ap_abstract0_free(m,a);
    throw std::overflow_error(s);
    break;
    
  case AP_EXC_INVALID_ARGUMENT:
    ap_manager_clear_exclog(m);
    if (a) ap_abstract0_free(m,a);
    throw std::invalid_argument(s);
    break;
    
  case AP_EXC_NOT_IMPLEMENTED:
    ap_manager_clear_exclog(m);
    if (a) ap_abstract0_free(m,a);
    throw not_implemented(s);
    break;
    
  default:
    ap_manager_clear_exclog(m);
    if (a) ap_abstract0_free(m,a);
    throw std::range_error(s+" (unknwon exception type)");
    break;
  }   
}

inline void manager::raise(const char* msg, ap_abstract0_t* a)
{ 
  manager::raise(m, msg, a); 
} 

inline void manager::raise(ap_manager_t* m, const char* msg, ap_abstract1_t a)
{
  if (!m->result.exclog || m->result.exclog->exn==AP_EXC_NONE) return;
  ap_environment_free(a.env);
  raise(m,msg,a.abstract0);
  
}

inline void manager::raise(const char* msg, ap_abstract1_t a)
{ 
  manager::raise(m, msg, a); 
} 

inline manager::manager(const manager& x) 
  : m(ap_manager_copy(x.m)) 
{
}

inline manager::~manager() 
{ 
  ap_manager_free(m); 
}

inline manager& manager::operator= (const manager& x)
{
  ap_manager_t* mm = ap_manager_copy(x.m);
  ap_manager_free(m);
  m = mm;
  return *this;
}

inline std::string manager::get_library() const 
{
  return std::string(ap_manager_get_library(m)); 
}

inline std::string manager::get_version() const 
{ 
  return std::string(ap_manager_get_version(m)); 
}

inline ap_funopt_t& manager::get_funopt(ap_funid_t funid)
{ 
  if (funid<=AP_FUNID_UNKNOWN || funid>=AP_FUNID_SIZE) 
    throw std::out_of_range("apron::manager::get_funopt(ap_funid_t) unknown funid");
  return m->option.funopt[funid]; 
}

inline ap_scalar_discr_t& manager::get_scalar_discr()
{ 
  return m->option.scalar_discr; 
}

inline bool manager::get_flag_exact()
{
  return ap_manager_get_flag_exact(m);
}

inline bool manager::get_flag_best()
{
  return ap_manager_get_flag_best(m);
}


inline ap_manager_t* manager::get_ap_manager_t()
{ 
  return m; 
}

inline const ap_manager_t* manager::get_ap_manager_t() const
{ 
  return m; 
}

inline void manager::fpu_init()
{
  if (!ap_fpu_init())
    throw std::runtime_error("apron::manager::fpu_init() failed");
}

inline std::ostream& operator<< (std::ostream& os, const manager& s)
{
  return os << ap_manager_get_library(s.m) << ", " << ap_manager_get_version(s.m);
}
