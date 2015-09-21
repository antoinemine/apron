/* -*- C++ -*-
 * apxx_var_inline.hh
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


/* ================================= */
/* var                               */
/* ================================= */


/* constructors */
/* ============ */

inline var::var(const void* x)
{
  v = ap_var_operations->copy(const_cast<void*>(x));
}
 
inline var::var(const std::string& x)
{
  v = ap_var_operations->copy(const_cast<char*>(x.c_str()));
}
 
inline var::var(const var& x)
{
  v = ap_var_operations->copy(x.v);
}


/* destructor */
/* ========== */

inline var::~var()
{
  ap_var_operations->free(v);
}


/* assignments */
/* =========== */

inline var& var::operator=(const var& x)
{
  if (x.v!=v) {
    ap_var_t vv = ap_var_operations->copy(x.v);
    ap_var_operations->free(v);
    v = vv;
  }
  return *this;
}

inline var& var::operator=(const void* x)
{
  ap_var_operations->free(v);
  v = ap_var_operations->copy(const_cast<void*>(x));
  return *this;
}

inline var& var::operator=(const std::string& x)
{
  ap_var_operations->free(v);
  v = ap_var_operations->copy(const_cast<char*>(x.c_str()));
  return *this;
}


/* conversions */
/* =========== */

inline var::operator char*() const
{
  return ap_var_operations->to_string(v);
}

inline var::operator std::string() const
{
  char* c = ap_var_operations->to_string(v);
  std::string s = c;
  free(c);
  return s;
}


/* comparisons */
/* =========== */

inline int  compare(const var& x, const var& y)
{
  return ap_var_operations->compare(x.v,y.v);
}

inline bool operator==(const var& x, const var& y)
{
  return ap_var_operations->compare(x.v,y.v)==0;
}

inline bool operator!=(const var& x, const var& y)
{
  return ap_var_operations->compare(x.v,y.v)!=0;
}

inline bool operator>=(const var& x, const var& y)
{
  return ap_var_operations->compare(x.v,y.v)>=0;
}

inline bool operator<=(const var& x, const var& y)
{
  return ap_var_operations->compare(x.v,y.v)<=0;
}

inline bool operator>(const var& x, const var& y)
{
  return ap_var_operations->compare(x.v,y.v)>0;
}

inline bool operator<(const var& x, const var& y)
{
  return ap_var_operations->compare(x.v,y.v)<0;
}

/* print */
/* ===== */

inline std::ostream& operator<< (std::ostream& os, const var& s)
{
  std::string ss = s;
  return os << ss;
}

inline void var::print(FILE* stream) const
{
  char* x = ap_var_operations->to_string(const_cast<void*>(v));
  fprintf(stream,"%s",x);
  free(x);
}



/* C-level compatibility */
/* ===================== */

inline const ap_var_t& var::get_ap_var_t() const
{
  return v;
}

inline ap_var_t& var::get_ap_var_t()
{
  return v;
}


