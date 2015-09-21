/* -*- C++ -*-
 * apxx_generator0_inline.hh
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
/* generator0                        */
/* ================================= */


/* constructors */
/* ============ */

inline generator0::generator0(ap_gentyp_t gentyp)
{
  ap_linexpr0_t* llin = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,0);
  l = ap_generator0_make(gentyp, llin);
}

inline generator0::generator0(ap_gentyp_t gentyp, const linexpr0& lin)
{
  ap_linexpr0_t* llin = ap_linexpr0_copy(const_cast<ap_linexpr0_t*>(lin.get_ap_linexpr0_t()));
  l = ap_generator0_make(gentyp, llin);
}

inline generator0::generator0(const generator0& x)
{
  l = ap_generator0_copy(const_cast<ap_generator0_t*>(&x.l)); 
}


inline generator0::generator0(const generator0& x, const dimchange& d)
{
  if (!x.l.linexpr0) throw std::invalid_argument("apron::generator0::generator0(const generator0&, cont dimchange&) empty expression");
  l = ap_generator0_add_dimensions(const_cast<ap_generator0_t*>(&x.l), 
				   const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t()));
}
  
inline generator0::generator0(const generator0& x, const dimperm& d)
{ 
  if (!x.l.linexpr0) throw std::invalid_argument("apron::generator0::generator0(const generator0&, cont dimperm&) empty expression");
  l = ap_generator0_permute_dimensions(const_cast<ap_generator0_t*>(&x.l), 
				       const_cast<ap_dimperm_t*>(d.get_ap_dimperm_t()));
}



/* destructor */
/* ========== */

inline generator0::~generator0()
{ 
  ap_generator0_clear(const_cast<ap_generator0_t*>(&l)); 
}


/* assignment */
/* ========== */

inline generator0& generator0::operator= (const generator0& x)
{ 
  if (&x!=this) {
    ap_generator0_clear(const_cast<ap_generator0_t*>(&l));
    l = ap_generator0_copy(const_cast<ap_generator0_t*>(&x.l)); 
  }
  return *this;
}


/* dimension operations */
/* ==================== */

inline void generator0::resize(size_t size)
{ 
  if (!l.linexpr0) throw std::invalid_argument("apron::generator0::resize(size_t) empty expression");
  ap_linexpr0_realloc(l.linexpr0, size);
}

inline void generator0::add_dimensions(const dimchange& d)
{
  if (!l.linexpr0) throw std::invalid_argument("apron::generator0::add_dimensions(const dimchange&) empty expression");
  ap_generator0_add_dimensions_with(&l, const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t()));
}

inline void generator0::permute_dimensions(const dimperm& d)
{ 
  if (!l.linexpr0) throw std::invalid_argument("apron::generator0::permute_dimensions(const dimperm&) empty expression");
  ap_generator0_permute_dimensions_with(&l, const_cast<ap_dimperm_t*>(d.get_ap_dimperm_t())); 
}


/* access */
/* ====== */

/* size */

inline size_t generator0::size() const
{ 
  if (!l.linexpr0) throw std::invalid_argument("apron::generator0::size() empty expression");
  return ap_linexpr0_size(const_cast<ap_linexpr0_t*>(l.linexpr0)); 
}


/* get */
 
inline ap_gentyp_t& generator0::get_gentyp()
{ 
  return l.gentyp;
}
  
inline const ap_gentyp_t& generator0::get_gentyp() const
{ 
  return l.gentyp; 
}
  
inline bool generator0::has_linexpr() const
{ 
  return l.linexpr0!=NULL; 
}

inline linexpr0& generator0::get_linexpr()
{ 
  if (!l.linexpr0) throw std::invalid_argument("apron::generator0::get_linexpr() empty expression");
  return reinterpret_cast<linexpr0&>(*l.linexpr0); 
}
 
inline const linexpr0& generator0::get_linexpr() const
{ 
  if (!l.linexpr0) throw std::invalid_argument("apron::generator0::get_linexpr() empty expression");
  return reinterpret_cast<linexpr0&>(*l.linexpr0); 
}

inline void generator0::set_linexpr(const linexpr0& c)
{ 
  if (l.linexpr0) ap_linexpr0_free(l.linexpr0);
  l.linexpr0 = ap_linexpr0_copy(const_cast<ap_linexpr0_t*>(c.get_ap_linexpr0_t()));
}


/* print */
/* ===== */

inline std::ostream& operator<< (std::ostream& os, const generator0& s)
{
  if (!s.has_linexpr())
      throw std::invalid_argument("apron::operator<<(ostream&, const generator0&) empty expression");
  switch (s.get_gentyp()) {
  case AP_GEN_LINE:     os << "LINE: "; break;
  case AP_GEN_RAY:      os << "RAY: "; break;
  case AP_GEN_VERTEX:   os << "VERTEX: "; break;
  case AP_GEN_LINEMOD:  os << "LINEMOD: "; break;
  case AP_GEN_RAYMOD:   os << "RAYMOD: "; break;
  default: throw std::invalid_argument("apron::operator<<(ostream&, const generator0&) invalid generator type");
  }
  return os << s.get_linexpr();
}


inline void generator0::print(char** name_of_dim, FILE* stream) const
{ 
  ap_generator0_fprint(stream, const_cast<ap_generator0_t*>(&l), name_of_dim);
}

 
/* C-level compatibility */
/* ===================== */

inline const ap_generator0_t* generator0::get_ap_generator0_t() const
{ 
  return &l;
}

inline ap_generator0_t* generator0::get_ap_generator0_t()
{ 
  return &l; 
}



/* ================================= */
/* generator0_array                  */
/* ================================= */


/* constructors */
/* ============ */

inline generator0_array::generator0_array(size_t size) 
  : a(ap_generator0_array_make(size)) 
{
}

inline generator0_array::generator0_array(const generator0_array& x) 
  : a(ap_generator0_array_make(x.a.size))
{ 
  for (size_t i=0; i<a.size; i++) 
    a.p[i] = ap_generator0_copy(&x.a.p[i]); 
}

inline generator0_array::generator0_array(size_t size, const generator0 x[]) 
  : a(ap_generator0_array_make(size))
{
  for (size_t i=0; i<size; i++) 
    a.p[i] = ap_generator0_copy(const_cast<ap_generator0_t*>(x[i].get_ap_generator0_t()));
}

inline generator0_array::generator0_array(const std::vector<generator0>& x) 
  : a(ap_generator0_array_make(x.size()))
{ 
  for (size_t i=0; i<a.size; i++)
    a.p[i] = ap_generator0_copy(const_cast<ap_generator0_t*>(x[i].get_ap_generator0_t())); 
}

inline generator0_array::generator0_array(const generator0_array& x, const dimchange& d)
{
  a = ap_generator0_array_add_dimensions(const_cast<ap_generator0_array_t*>(&x.a), 
					 const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t()));
}
 
inline generator0_array::generator0_array(const generator0_array& x, const dimperm& d)
{ 
  a = ap_generator0_array_permute_dimensions(const_cast<ap_generator0_array_t*>(&x.a), 
					     const_cast<ap_dimperm_t*>(d.get_ap_dimperm_t()));
}
  

/* destructor */
/* ========== */

inline generator0_array::~generator0_array()
{ 
  ap_generator0_array_clear(&a);
}


/* assignment */
/* ========== */

inline generator0_array& generator0_array::operator= (const generator0_array& x)
{ 
  if (&x!=this) {
    ap_generator0_array_clear(&a);
    a = ap_generator0_array_make(x.a.size);
    for (size_t i=0; i<a.size; i++) a.p[i] = ap_generator0_copy(&x.a.p[i]);
  }
  return *this;
}

inline generator0_array& generator0_array::operator= (const generator0 x[])
{
  size_t size = a.size;
  ap_generator0_array_clear(&a);
  a = ap_generator0_array_make(size);
  for (size_t i=0; i<size; i++) 
    a.p[i] = ap_generator0_copy(const_cast<ap_generator0_t*>(x[i].get_ap_generator0_t()));
  return *this;
}

inline generator0_array& generator0_array::operator= (const std::vector<generator0>& x)
{ 
  size_t size = x.size();
  ap_generator0_array_clear(&a);
  a = ap_generator0_array_make(size);
  for (size_t i=0; i<size; i++) 
    a.p[i] = ap_generator0_copy(const_cast<ap_generator0_t*>(x[i].get_ap_generator0_t()));
  return *this;
}


/* dimension operations */
/* ==================== */

inline void generator0_array::resize(size_t size)
{ 
  ap_generator0_array_resize(&a, size);
}

inline void generator0_array::add_dimensions(const dimchange& d)
{ 
  ap_generator0_array_add_dimensions_with(&a, const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t()));
}

inline void generator0_array::permute_dimensions(const dimperm& d)
{ 
  ap_generator0_array_permute_dimensions_with(&a, const_cast<ap_dimperm_t*>(d.get_ap_dimperm_t()));
}


/* access */
/* ====== */

inline size_t generator0_array::size() const
{ 
  return a.size;
}
 
inline generator0* generator0_array::contents()
{ 
  return reinterpret_cast<generator0*>(a.p); 
}

inline const generator0* generator0_array::contents() const
{ 
  return reinterpret_cast<generator0*>(a.p); 
}

inline generator0& generator0_array::operator[](size_t i)
{ 
  return reinterpret_cast<generator0&>(a.p[i]); 
}

inline const generator0& generator0_array::operator[](size_t i) const
{ 
  return reinterpret_cast<generator0&>(a.p[i]); 
}

inline generator0& generator0_array::get(size_t i)
{ 
  if (i >= a.size) throw std::out_of_range("apron::generator0_array::get(size_t)");
  return reinterpret_cast<generator0&>(a.p[i]);
}

inline const generator0& generator0_array::get(size_t i) const
{ 
  if (i >= a.size) throw std::out_of_range("apron::generator0_array::get(size_t)");
  return reinterpret_cast<generator0&>(a.p[i]); 
}


/* conversion */
/* ========== */

inline generator0_array::operator std::vector<generator0>() const
{
  size_t sz = size();
  std::vector<generator0> v = std::vector<generator0>(sz);
  for (size_t i=0;i<sz;i++)
    v[i] = (*this)[i];
  return v;
}


/* print */
/* ===== */

inline std::ostream& operator<< (std::ostream& os, const generator0_array& s)
{
  size_t size = s.size();
  os << "{ ";
  for (size_t i=0;i<size;i++)
    os << s[i] << "; ";
  return os << "}";
}

inline void generator0_array::print(char** name_of_dim, FILE* stream) const
{ 
  ap_generator0_array_fprint(stream, const_cast<ap_generator0_array_t*>(&a), name_of_dim);
}


/* C-level compatibility */
/* ===================== */

inline const ap_generator0_array_t* generator0_array::get_ap_generator0_array_t() const
{
  return &a;
}

inline ap_generator0_array_t* generator0_array::get_ap_generator0_array_t()
{
  return &a;
}
