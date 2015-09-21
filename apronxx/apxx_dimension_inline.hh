/* -*- C++ -*-
 * apxx_dimension_inline.hh
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
/* varname                           */
/* ================================= */

inline varname::varname(const std::vector<std::string>& names)
  : names(names)
{}

template<class charT, class Traits>
inline
std::basic_ostream<charT,Traits>& 
operator<<(std::basic_ostream<charT,Traits>& os, const varname& v)
{
  std::vector<std::string>*& vv = 
    (std::vector<std::string>*&)os.pword(varname::xindex);
  if (vv) delete vv;
  if (v.names.size()) vv = new std::vector<std::string>(v.names);
  else vv = NULL;
  return os;
}

template<class charT, class Traits>
inline
std::vector<std::string>* get_varname(std::basic_ostream<charT,Traits>& os)
{
  return (std::vector<std::string>*)os.pword(varname::xindex);
}

  

/* ================================= */
/* dimchange                         */
/* ================================= */


/* constructor */
/* =========== */
  
inline dimchange::dimchange(size_t intdim, size_t realdim)
{ 
  ap_dimchange_init(&c, intdim, realdim);
}

inline dimchange::dimchange(size_t intdim, size_t realdim, const ap_dim_t d[])
{ 
  ap_dimchange_init(&c, intdim, realdim);
  memcpy(c.dim, d, sizeof(ap_dim_t) * (intdim + realdim));
}

inline dimchange::dimchange(const dimchange& x, bool inv)
{
  ap_dimchange_init(&c, x.c.intdim, x.c.realdim);
  memcpy(c.dim, x.c.dim, sizeof(ap_dim_t) * (x.c.intdim + x.c.realdim));
  if (inv) ap_dimchange_add_invert(&c);
}

inline dimchange::dimchange(size_t intdim, size_t realdim, const std::vector<ap_dim_t>& d)
{
  if (d.size()<intdim+realdim)
    throw std::invalid_argument("apron::dimchange::dimchange(size_t, size_t, const vector<ap_dim_t>&) vector too short");
  ap_dimchange_init(&c, intdim, realdim);
  for (size_t i=0; i<intdim+realdim; i++)
    c.dim[i] = d[i];
}


/* destructor */
/* ========== */

inline dimchange::~dimchange()
{ 
  ap_dimchange_clear(&c); 
}
  

/* assignment */
/* ========== */

inline dimchange& dimchange::operator= (const dimchange& x)
{
  if (&x!=this) {
    ap_dimchange_clear(&c);
    ap_dimchange_init(&c, x.c.intdim, x.c.realdim);
    memcpy(c.dim, x.c.dim, sizeof(ap_dim_t) * (x.c.intdim + x.c.realdim));
  }
  return *this;
}

inline dimchange& dimchange::operator= (const ap_dim_t d[])
{ 
  memcpy(c.dim, d, sizeof(ap_dim_t) * (c.intdim + c.realdim));
  return *this;
}

inline dimchange& dimchange::operator= (const std::vector<ap_dim_t>& d)
{
  if (d.size()<c.intdim+c.realdim)
    throw std::invalid_argument("apron::dimchange::operator=(const vector<ap_dim_t>&) vector too short");
  for (size_t i=0; i<c.intdim+c.realdim; i++)
    c.dim[i] = d[i];
  return *this;
}

/* access */
/* ====== */

inline size_t dimchange::get_intdim() const
{ 
  return c.intdim; 
}

inline size_t dimchange::get_realdim() const
{ 
  return c.realdim; 
}

inline ap_dim_t& dimchange::get(size_t dim)
{
  if (dim >= c.intdim + c.realdim) throw std::out_of_range("apron::dimchange::get(size_t)");
  return c.dim[dim];
}
  
inline const ap_dim_t& dimchange::get(size_t dim) const
{
  if (dim >= c.intdim + c.realdim) throw std::out_of_range("apron::dimchange::get(size_t)");
  return c.dim[dim];
}
  
inline ap_dim_t& dimchange::operator[](size_t dim)
{
  return c.dim[dim];
}
  
inline const ap_dim_t& dimchange::operator[](size_t dim) const
{
  return c.dim[dim];
}
  

/* print */
/* ===== */
  
inline std::ostream& operator<< (std::ostream& os, const dimchange& s)
{
  std::vector<std::string>* names = get_varname(os);
  os << "dimchange: intdim=" << s.c.intdim << ", realdim=" << s.c.realdim << std::endl;
  if (names) {
    size_t sz = (*names).size();
    for (size_t i=0;i<s.c.intdim+s.c.realdim;i++)
      if (s.c.dim[i] < sz) os << (*names)[s.c.dim[i]] << " ";
      else os << "x" << s.c.dim[i] << " ";
  }
  else {
    for (size_t i=0;i<s.c.intdim+s.c.realdim;i++)
      os << "x" << s.c.dim[i] << " ";
  }
  return os << std::endl;
}

inline void dimchange::print(FILE* stream) const
{ 
  ap_dimchange_fprint(stream, const_cast<ap_dimchange_t*>(&c)); 
}


/* other operators */
/* =============== */

inline void dimchange::add_invert()
{ 
  ap_dimchange_add_invert(&c);
}

inline dimchange dimchange::operator- () const
{ 
  dimchange r = *this; ap_dimchange_add_invert(&r.c);
  return r;
}



/* C-level compatibility */
/* ===================== */

inline const ap_dimchange_t* dimchange::get_ap_dimchange_t() const
{
  return &c;
}

inline ap_dimchange_t* dimchange::get_ap_dimchange_t()
{ 
  return &c;
}


/* ================================= */
/* dimperm                           */
/* ================================= */


/* constructor */
/* =========== */
  
inline dimperm::dimperm(size_t size)
{ 
  ap_dimperm_init(&c, size); 
}

inline dimperm::dimperm(size_t size, const ap_dim_t d[])
{ 
  ap_dimperm_init(&c, size);
  memcpy(c.dim, d, sizeof(ap_dim_t) * (size));
}

inline dimperm::dimperm(id t)
{ 
  ap_dimperm_init(&c, t.size); 
  ap_dimperm_set_id(&c);
}

inline dimperm::dimperm(const dimperm& x, bool inv)
{
  ap_dimperm_init(&c, x.c.size);
  if (inv) ap_dimperm_invert(&c, const_cast<ap_dimperm_t*>(&x.c));
  else memcpy(c.dim, x.c.dim, sizeof(ap_dim_t) * (x.c.size));
}

inline dimperm::dimperm(const dimperm& x, const dimperm& y)
{
  ap_dimperm_init(&c, x.c.size);
  ap_dimperm_compose( &c, const_cast<ap_dimperm_t*>(&x.c), const_cast<ap_dimperm_t*>(&y.c) );
}

inline dimperm::dimperm(const std::vector<ap_dim_t>& d)
{
  ap_dimperm_init(&c, d.size()); 
  for (size_t i=0; i<d.size(); i++)
    c.dim[i] = d[i];
}


/* destructor */
/* ========== */

inline dimperm::~dimperm()
{ 
  ap_dimperm_clear(&c);
}
  

/* assignment */
/* ========== */

inline dimperm& dimperm::operator= (const dimperm& x)
{
  if (&x!=this) {
    if (x.c.size!=c.size) {
      ap_dimperm_clear(&c);
      ap_dimperm_init(&c, x.c.size);
    }
    memcpy(c.dim, x.c.dim, sizeof(ap_dim_t) * c.size);
  }
  return *this;
}

inline dimperm& dimperm::operator= (id t)
{ 
  if (t.size!=c.size) {
    ap_dimperm_clear(&c);
    ap_dimperm_init(&c, t.size);
  }
  ap_dimperm_set_id(&c); 
  return *this;
}
  
inline dimperm& dimperm::operator= (const ap_dim_t d[])
{ 
  memcpy(c.dim, d, sizeof(ap_dim_t) * c.size);
  return *this;
}

inline dimperm& dimperm::operator= (const std::vector<ap_dim_t>& d)
{
  if (c.size!=d.size()) {
    ap_dimperm_clear(&c);
    ap_dimperm_init(&c, d.size());
  }
  for (size_t i=0; i<c.size; i++) c.dim[i] = d[i];
  return *this;
}


/* access */
/* ====== */

inline size_t dimperm::size() const
{
  return c.size; 
}

inline ap_dim_t& dimperm::operator[](size_t dim)
{
  return c.dim[dim];
}
  
inline const ap_dim_t& dimperm::operator[](size_t dim) const
{
  return c.dim[dim];
}
  
inline ap_dim_t& dimperm::get(size_t dim)
{
  if (dim >= c.size) throw std::out_of_range("apron::dimperm::get(size_t)");
  return c.dim[dim];
}
  
inline const ap_dim_t& dimperm::get(size_t dim) const
{
  if (dim >= c.size) throw std::out_of_range("apron::dimperm::get(size_t)");
  return c.dim[dim];
}
  

/* print */
/* ===== */
  
inline std::ostream& operator<< (std::ostream& os, const dimperm& s)
{
  std::vector<std::string>* names = get_varname(os);
  os << "dimperm: size=" << s.c.size << std::endl;
  if (names) {
    size_t sz = (*names).size();
    for (size_t i=0;i<s.c.size;i++) {
      if (i<sz) os << (*names)[i];
      else os << "x" << i;
      os << " -> ";
      if (s.c.dim[i]<sz) os << (*names)[s.c.dim[i]];
      else os << "x" << s.c.dim[i];
      os << std::endl;
    }
  }
  else {
    for (size_t i=0;i<s.c.size;i++)
      os << "x" << i << " -> " << "x" << s.c.dim[i] << std::endl;
  }
  return os;
}

inline void dimperm::print(FILE* stream) const
{ 
  ap_dimperm_fprint(stream, const_cast<ap_dimperm_t*>(&c)); 
}


/* other operators */
/* =============== */

/* composition */

inline dimperm& dimperm::operator*= (const dimperm& y)
{ 
  if (c.size!=y.c.size)
    throw std::invalid_argument("apron::dimperm::operator*= (const dimperm&) size mismatch");
  if (&y==this) {
    dimperm tmp = y;
    ap_dimperm_compose(&c, &c, const_cast<ap_dimperm_t*>(&tmp.c));
  }
  else
    ap_dimperm_compose(&c, &c, const_cast<ap_dimperm_t*>(&y.c));
  return *this; 
}

inline dimperm operator* (const dimperm& x, const dimperm& y)
{ 
  if (x.c.size!=y.c.size)
    throw std::invalid_argument("apron::dimperm::operator* (const dimperm&, const dimperm&) size mismatch");
  dimperm r = x.c.size;
  ap_dimperm_compose(&r.c, const_cast<ap_dimperm_t*>(&x.c), const_cast<ap_dimperm_t*>(&y.c)); 
  return r;
}


/* inversion */

inline void dimperm::invert()
{ 
  dimperm tmp = *this; 
  ap_dimperm_invert(&c, &tmp.c);
}

inline dimperm dimperm::operator- () const
{
  dimperm r = c.size;
  ap_dimperm_invert(&r.c, const_cast<ap_dimperm_t*>(&c));
  return r;
}



/* C-level compatibility */
/* ===================== */

inline const ap_dimperm_t* dimperm::get_ap_dimperm_t() const
{
  return &c; 
}

inline ap_dimperm_t* dimperm::get_ap_dimperm_t()
{ 
  return &c; 
}
