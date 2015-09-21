/* -*- C++ -*-
 * apxx_lincons0_inline.hh
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
/* lincons0                          */
/* ================================= */

inline lincons0::lincons0(ap_lincons0_t l) : l(l) {}

inline lincons0::lincons0(ap_constyp_t constyp)
{
  ap_linexpr0_t* llin = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,0);
  l = ap_lincons0_make(constyp, llin, NULL);
}

inline lincons0::lincons0(ap_constyp_t constyp, const linexpr0& lin, const scalar& modulo)
{
  ap_linexpr0_t* llin = ap_linexpr0_copy(const_cast<ap_linexpr0_t*>(lin.get_ap_linexpr0_t()));
  ap_scalar_t* mmodulo = ap_scalar_alloc_set(const_cast<ap_scalar_t*>(modulo.get_ap_scalar_t()));
  l = ap_lincons0_make(constyp, llin, mmodulo);
}

inline lincons0::lincons0(ap_constyp_t constyp, const linexpr0& lin)
{
  ap_linexpr0_t* llin = ap_linexpr0_copy(const_cast<ap_linexpr0_t*>(lin.get_ap_linexpr0_t()));
  l = ap_lincons0_make(constyp, llin, NULL);
}

inline lincons0::lincons0(const lincons0& x)
{ 
  l = ap_lincons0_copy(const_cast<ap_lincons0_t*>(&x.l)); 
}

inline lincons0::lincons0(unsat x)
{ 
  l = ap_lincons0_make_unsat();
}

inline lincons0::lincons0(const lincons0& x, const dimchange& d)
{
  if (!x.l.linexpr0) throw std::invalid_argument("apron::lincons0::lincons0(const dimchange&) empty expression");
  l = ap_lincons0_add_dimensions(const_cast<ap_lincons0_t*>(&x.l), 
				 const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t()));
}

inline lincons0::lincons0(const lincons0& x, const dimperm& d)
{ 
  if (!x.l.linexpr0) throw std::invalid_argument("apron::lincons0::lincons0(const dimperm&) empty expression");
  l = ap_lincons0_permute_dimensions(const_cast<ap_lincons0_t*>(&x.l), 
				     const_cast<ap_dimperm_t*>(d.get_ap_dimperm_t()));
}


/* destructor */
/* ========== */

inline lincons0::~lincons0()
{ 
  ap_lincons0_clear(&l); 
}
    

/* assignment */
/* ========== */

inline lincons0& lincons0::operator= (const lincons0& x)
{ 
  if (&x!=this) {
    ap_lincons0_clear(&l);
    l = ap_lincons0_copy(const_cast<ap_lincons0_t*>(&x.l)); 
  }
  return *this;
}

inline lincons0& lincons0::operator= (unsat x)
{ 
  ap_lincons0_clear(&l); 
  l = ap_lincons0_make_unsat();  
  return *this;
}


/* dimension operations */
/* ==================== */

inline void lincons0::resize(size_t size)
{ 
  if (!l.linexpr0) throw std::invalid_argument("apron::lincons0::resize(size_t) empty expression");
  ap_linexpr0_realloc(l.linexpr0, size);
}


inline void lincons0::add_dimensions(const dimchange& d)
{ 
  if (!l.linexpr0) throw std::invalid_argument("apron::lincons0::add_dimensions(const dimchange&) empty expression");
  ap_lincons0_add_dimensions_with(&l, const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t()));
}

inline void lincons0::permute_dimensions(const dimperm& d)
{ 
  if (!l.linexpr0) throw std::invalid_argument("apron::lincons0::permute_dimensions(const dimperm&) empty expression");
  ap_lincons0_permute_dimensions_with(&l, const_cast<ap_dimperm_t*>(d.get_ap_dimperm_t())); 
}


/* access */
/* ====== */

/* size */

inline size_t lincons0::size() const
{ 
  if (!l.linexpr0) throw std::invalid_argument("apron::lincons0::size() empty expression");
  return ap_linexpr0_size(const_cast<ap_linexpr0_t*>(l.linexpr0));
}


/* get */
 
inline ap_constyp_t& lincons0::get_constyp()
{ 
  return l.constyp; 
}
  
inline const ap_constyp_t& lincons0::get_constyp() const
{
  return l.constyp; 
}

inline bool lincons0::has_modulo() const
{ 
  return l.scalar!=NULL; 
}

inline bool lincons0::has_linexpr() const
{
  return l.linexpr0!=NULL; 
}

inline scalar& lincons0::get_modulo()
{ 
  if (!l.scalar) throw std::invalid_argument("apron::lincons0::get_modulo() empty scalar");
  return reinterpret_cast<scalar&>(*l.scalar);
}
 
inline const scalar& lincons0::get_modulo() const
{ 
  if (!l.scalar) throw std::invalid_argument("apron::lincons0::get_modulo() empty scalar");
  return reinterpret_cast<scalar&>(*l.scalar);
}

inline void lincons0::set_modulo(const scalar& c)
{ 
  if (!l.scalar) l.scalar = ap_scalar_alloc_set(const_cast<ap_scalar_t*>(c.get_ap_scalar_t()));
  else ap_scalar_set(l.scalar, const_cast<ap_scalar_t*>(c.get_ap_scalar_t()));
}

inline linexpr0& lincons0::get_linexpr()
{ 
  if (!l.linexpr0) throw std::invalid_argument("apron::lincons0::get_linexpr() empty expression");
  return reinterpret_cast<linexpr0&>(*l.linexpr0); 
}
 
inline const linexpr0& lincons0::get_linexpr() const
{ 
  if (!l.linexpr0) throw std::invalid_argument("apron::lincons0::get_linexpr() empty expression");
  return reinterpret_cast<linexpr0&>(*l.linexpr0); 
}

inline void lincons0::set_linexpr(const linexpr0& c)
{ 
  if (l.linexpr0) ap_linexpr0_free(l.linexpr0);
  l.linexpr0 = ap_linexpr0_copy(const_cast<ap_linexpr0_t*>(c.get_ap_linexpr0_t()));
}

inline coeff& lincons0::get_cst()
{ return get_linexpr().get_cst(); }

inline const coeff& lincons0::get_cst() const
{ return get_linexpr().get_cst(); }

inline coeff& lincons0::operator[](ap_dim_t dim)
{ return get_linexpr()[dim]; }

inline const coeff& lincons0::operator[](ap_dim_t dim) const
{ return get_linexpr()[dim]; }


/* print */
/* ===== */

inline std::ostream& operator<< (std::ostream& os, const lincons0& s)
{
  os << s.get_linexpr();
  switch (s.get_constyp()) {
  case AP_CONS_EQ:    return os << " = 0";
  case AP_CONS_SUPEQ: return os << " >= 0";
  case AP_CONS_SUP:   return os << " > 0";
  case AP_CONS_EQMOD: return os << " = 0 mod " << s.get_modulo();
  case AP_CONS_DISEQ: return os << " != 0";
  default: throw std::invalid_argument("apron::operator<<(ostream&, const lincons0&) unknown constraint type");
  }
}

inline void lincons0::print(char** name_of_dim, FILE* stream) const
{ 
  ap_lincons0_fprint(stream, const_cast<ap_lincons0_t*>(&l), name_of_dim);
}

 
/* tests */
/* ===== */

inline bool lincons0::is_unsat() const
{ 
  if (!l.linexpr0) throw std::invalid_argument("apron::lincons0::is_unsat() empty expression");
  return ap_lincons0_is_unsat(const_cast<ap_lincons0_t*>(&l));
}
  
inline bool lincons0::is_linear() const
{ 
  if (!l.linexpr0) throw std::invalid_argument("apron::lincons0::is_linear() empty expression");
  return ap_linexpr0_is_linear(l.linexpr0);
}
  
inline bool lincons0::is_quasilinear() const
{ 
  if (!l.linexpr0) throw std::invalid_argument("apron::lincons0::is_quasilinear() empty expression");
  return ap_linexpr0_is_quasilinear(l.linexpr0);
}
  


/* C-level compatibility */
/* ===================== */

inline const ap_lincons0_t* lincons0::get_ap_lincons0_t() const
{ 
  return &l; 
}

inline ap_lincons0_t* lincons0::get_ap_lincons0_t()
{ 
  return &l; 
}


/* ================================= */
/* lincons0_array                    */
/* ================================= */


/* constructors */
/* ============ */

inline lincons0_array::lincons0_array(size_t size) 
  : a(ap_lincons0_array_make(size)) 
{
}

inline lincons0_array::lincons0_array(const lincons0_array& x) 
  : a(ap_lincons0_array_make(x.a.size))
{ 
  for (size_t i=0; i<a.size; i++) 
    a.p[i] = ap_lincons0_copy(&x.a.p[i]);
}

inline lincons0_array::lincons0_array(size_t size, const lincons0 x[]) 
  : a(ap_lincons0_array_make(size))
{ 
  for (size_t i=0; i<size; i++) 
    a.p[i] = ap_lincons0_copy(const_cast<ap_lincons0_t*>(x[i].get_ap_lincons0_t()));
}

inline lincons0_array::lincons0_array(const std::vector<lincons0>& x) 
  : a(ap_lincons0_array_make(x.size()))
{ 
  for (size_t i=0; i<a.size; i++) 
    a.p[i] = ap_lincons0_copy(const_cast<ap_lincons0_t*>(x[i].get_ap_lincons0_t()));
}

inline lincons0_array::lincons0_array(const lincons0_array& x, const dimchange& d)
{
  a = ap_lincons0_array_add_dimensions(const_cast<ap_lincons0_array_t*>(&x.a), 
				       const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t()));
}

inline lincons0_array::lincons0_array(const lincons0_array& x, const dimperm& d)
{ 
  a = ap_lincons0_array_permute_dimensions(const_cast<ap_lincons0_array_t*>(&x.a), 
					   const_cast<ap_dimperm_t*>(d.get_ap_dimperm_t()));
}


/* destructor */
/* ========== */

inline lincons0_array::~lincons0_array()
{ 
  ap_lincons0_array_clear(&a); 
}


/* assignment */
/* ========== */

inline lincons0_array& lincons0_array::operator= (const lincons0_array& x)
{ 
  if (&x!=this) {
    ap_lincons0_array_clear(&a);
    a = ap_lincons0_array_make(x.a.size);
    for (size_t i=0; i<a.size; i++) a.p[i] = ap_lincons0_copy(&x.a.p[i]);
  }
  return *this;
}

inline lincons0_array& lincons0_array::operator= (const lincons0 x[])
{
  size_t size = a.size;
  ap_lincons0_array_clear(&a);
  a = ap_lincons0_array_make(size);
  for (size_t i=0; i<size; i++) 
    a.p[i] = ap_lincons0_copy(const_cast<ap_lincons0_t*>(x[i].get_ap_lincons0_t()));
  return *this;
}

inline lincons0_array& lincons0_array::operator= (const std::vector<lincons0>& x)
{ 
  size_t size = x.size();
  ap_lincons0_array_clear(&a);
  a = ap_lincons0_array_make(size);
  for (size_t i=0; i<size; i++) 
    a.p[i] = ap_lincons0_copy(const_cast<ap_lincons0_t*>(x[i].get_ap_lincons0_t()));
  return *this;
}


/* dimension operations */
/* ==================== */

inline void lincons0_array::resize(size_t size)
{ 
  ap_lincons0_array_resize(&a, size);
}

inline void lincons0_array::add_dimensions(const dimchange& d)
{ 
  ap_lincons0_array_add_dimensions_with(&a, const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t()));
}

inline void lincons0_array::permute_dimensions(const dimperm& d)
{ 
  ap_lincons0_array_permute_dimensions_with(&a, const_cast<ap_dimperm_t*>(d.get_ap_dimperm_t()));
}
  

/* access */
/* ====== */

inline size_t lincons0_array::size() const
{ 
  return a.size;
}
 
inline lincons0* lincons0_array::contents()
{ 
  return reinterpret_cast<lincons0*>(a.p);
}

inline const lincons0* lincons0_array::contents() const
{ 
  return reinterpret_cast<lincons0*>(a.p);
}

inline lincons0& lincons0_array::operator[](size_t i)
{ 
  return reinterpret_cast<lincons0&>(a.p[i]); 
}

inline const lincons0& lincons0_array::operator[](size_t i) const
{ 
  return reinterpret_cast<lincons0&>(a.p[i]); 
}

inline lincons0& lincons0_array::get(size_t i)
{ 
  if (i >= a.size) throw std::out_of_range("apron::lincons0_array::get(size_t)");
  return reinterpret_cast<lincons0&>(a.p[i]); 
}

inline const lincons0& lincons0_array::get(size_t i) const
{ 
  if (i >= a.size) throw std::out_of_range("apron::lincons0_array::get(size_t)");
  return reinterpret_cast<lincons0&>(a.p[i]); 
}


/* conversion */
/* ========== */

inline lincons0_array::operator std::vector<lincons0>() const
{
  size_t sz = size();
  std::vector<lincons0> v = std::vector<lincons0>(sz);
  for (size_t i=0;i<sz;i++)
    v[i] = (*this)[i];
  return v;
}


/* print */
/* ===== */

inline std::ostream& operator<< (std::ostream& os, const lincons0_array& s)
{
  size_t sz = s.size();
  os << "{ ";
  for (size_t i=0;i<sz;i++)
    os << s[i] << "; ";
  return os << "}";
}

inline void lincons0_array::print(char** name_of_dim, FILE* stream) const
{ 
  ap_lincons0_array_fprint(stream, const_cast<ap_lincons0_array_t*>(&a), name_of_dim); 
}


/* tests */
/* ===== */

inline bool lincons0_array::is_linear() const
{ 
  return ap_lincons0_array_is_linear(const_cast<ap_lincons0_array_t*>(&a)); 
}

inline bool lincons0_array::is_quasilinear() const
{ 
  return ap_lincons0_array_is_quasilinear(const_cast<ap_lincons0_array_t*>(&a)); 
}



/* C-level compatibility */
/* ===================== */

inline const ap_lincons0_array_t* lincons0_array::get_ap_lincons0_array_t() const
{ 
  return &a;
}

inline ap_lincons0_array_t* lincons0_array::get_ap_lincons0_array_t()
{ 
  return &a;
}
