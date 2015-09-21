/* -*- C++ -*-
 * apxx_tcons0_inline.hh
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
/* tcons0                            */
/* ================================= */

inline tcons0::tcons0(ap_tcons0_t& l) : l(l) 
{}

inline tcons0::tcons0(ap_constyp_t constyp)
{
  l = ap_tcons0_make(constyp, NULL, NULL);
}

inline tcons0::tcons0(ap_constyp_t constyp, const texpr0::builder& t, const scalar& modulo)
{
  ap_texpr0_t* lt = ap_texpr0_copy(const_cast<ap_texpr0_t*>(t.get_ap_texpr0_t()));
  ap_scalar_t* mmodulo = ap_scalar_alloc_set(const_cast<ap_scalar_t*>(modulo.get_ap_scalar_t()));
  l = ap_tcons0_make(constyp, lt, mmodulo);
}

inline tcons0::tcons0(ap_constyp_t constyp, const texpr0::builder& t)
{
  ap_texpr0_t* lt = ap_texpr0_copy(const_cast<ap_texpr0_t*>(t.get_ap_texpr0_t()));
  l = ap_tcons0_make(constyp, lt, NULL);
}

inline tcons0::tcons0(const tcons0& x)
{ 
  l = ap_tcons0_copy(const_cast<ap_tcons0_t*>(&x.l)); 
}

inline tcons0::tcons0(unsat x)
{ 
  l = ap_tcons0_make_unsat();
}

inline tcons0::tcons0(const tcons0& x, const dimchange& d, bool add)
{
  if (!x.l.texpr0) throw std::invalid_argument("apron::tcons0::tcons0(const tcons0&, const dimchange&, bool) empty expression");
  if (add)
    l = ap_tcons0_add_dimensions(const_cast<ap_tcons0_t*>(&x.l), 
				 const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t()));
  else
    l = ap_tcons0_remove_dimensions(const_cast<ap_tcons0_t*>(&x.l), 
				    const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t()));
}

inline tcons0::tcons0(const tcons0& x, const dimperm& d)
{ 
  if (!x.l.texpr0) throw std::invalid_argument("apron::tcons0::tcons0(const tcons0&, const dimperm&) empty expression");
  l = ap_tcons0_permute_dimensions(const_cast<ap_tcons0_t*>(&x.l), 
				   const_cast<ap_dimperm_t*>(d.get_ap_dimperm_t()));
}
  
inline tcons0::tcons0(const lincons0& x)
{ 
  l = ap_tcons0_from_lincons0(const_cast<ap_lincons0_t*>(x.get_ap_lincons0_t()));
}


/* destructor */
/* ========== */

inline tcons0::~tcons0()
{ 
  ap_tcons0_clear(const_cast<ap_tcons0_t*>(&l)); 
}
    

/* 'Intelligent' constructors */
/* ========================== */

inline tcons0 operator>=(const texpr0::builder& a, const texpr0::builder& b)
{
  if (b.is_zero())      return tcons0(AP_CONS_SUPEQ,a);
  else if (a.is_zero()) return tcons0(AP_CONS_SUPEQ,-b);
  else         	        return tcons0(AP_CONS_SUPEQ,a-b);
}

inline tcons0 operator<=(const texpr0::builder& a, const texpr0::builder& b)
{
  if (b.is_zero())      return tcons0(AP_CONS_SUPEQ,-a);
  else if (a.is_zero()) return tcons0(AP_CONS_SUPEQ,b);
  else         	        return tcons0(AP_CONS_SUPEQ,b-a);
}

inline tcons0 operator> (const texpr0::builder& a, const texpr0::builder& b)
{
  if (b.is_zero())      return tcons0(AP_CONS_SUP,a);
  else if (a.is_zero()) return tcons0(AP_CONS_SUP,-b);
  else         	        return tcons0(AP_CONS_SUP,a-b);
}

inline tcons0 operator< (const texpr0::builder& a, const texpr0::builder& b)
{
  if (b.is_zero())      return tcons0(AP_CONS_SUP,-a);
  else if (a.is_zero()) return tcons0(AP_CONS_SUP,b);
  else         	        return tcons0(AP_CONS_SUP,b-a);
}

inline tcons0 operator==(const texpr0::builder& a, const texpr0::builder& b)
{
  if (b.is_zero())      return tcons0(AP_CONS_EQ,a);
  else if (a.is_zero()) return tcons0(AP_CONS_EQ,b);
  else         	        return tcons0(AP_CONS_EQ,a-b);
}

inline tcons0 operator!=(const texpr0::builder& a, const texpr0::builder& b)
{
  if (b.is_zero())      return tcons0(AP_CONS_DISEQ,a);
  else if (a.is_zero()) return tcons0(AP_CONS_DISEQ,b);
  else         	        return tcons0(AP_CONS_DISEQ,a-b);
}


/* assignment */
/* ========== */

inline tcons0& tcons0::operator= (const tcons0& x)
{ 
  if (&x!=this) {
    ap_tcons0_clear(&l);
    l = ap_tcons0_copy(const_cast<ap_tcons0_t*>(&x.l)); 
  }
  return *this;
}

inline tcons0& tcons0::operator= (unsat x)
{ 
  ap_tcons0_clear(&l); 
  l = ap_tcons0_make_unsat();  
  return *this;
}

inline tcons0& tcons0::operator= (const lincons0& x)
{ 
  ap_tcons0_clear(&l); 
  l = ap_tcons0_from_lincons0(const_cast<ap_lincons0_t*>(x.get_ap_lincons0_t()));
  return *this;
}


/* dimension operations */
/* ==================== */

inline void tcons0::add_dimensions(const dimchange& d)
{ 
  if (!l.texpr0) throw std::invalid_argument("apron::tcons0::add_dimensions(const dimchange&) empty expression");
  ap_tcons0_add_dimensions_with(&l, const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t()));
}

inline void tcons0::remove_dimensions(const dimchange& d)
{ 
  if (!l.texpr0) throw std::invalid_argument("apron::tcons0::remove_dimensions(const dimchange&) empty expression");
  ap_tcons0_remove_dimensions_with(&l, const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t()));
}

inline void tcons0::permute_dimensions(const dimperm& d)
{ 
  if (!l.texpr0) throw std::invalid_argument("apron::tcons0::permute_dimensions(dimperm&) empty expression");
  ap_tcons0_permute_dimensions_with(&l, const_cast<ap_dimperm_t*>(d.get_ap_dimperm_t())); 
}


/* access */
/* ====== */

/* get */
 
inline ap_constyp_t& tcons0::get_constyp()
{ 
  return l.constyp; 
}
  
inline const ap_constyp_t& tcons0::get_constyp() const
{
  return l.constyp; 
}

inline bool tcons0::has_modulo() const
{ 
  return l.scalar!=NULL; 
}

inline bool tcons0::has_texpr() const
{
  return l.texpr0!=NULL; 
}

inline scalar& tcons0::get_modulo()
{ 
  if (!l.scalar) throw std::invalid_argument("apron::tcons0::get_modulo() empty scalar");
  return reinterpret_cast<scalar&>(*l.scalar);
}
 
inline const scalar& tcons0::get_modulo() const
{ 
  if (!l.scalar) throw std::invalid_argument("apron::tcons0::get_modulo() empty scalar");
  return reinterpret_cast<scalar&>(*l.scalar);
}

inline void tcons0::set_modulo(const scalar& c)
{ 
  if (!l.scalar) l.scalar = ap_scalar_alloc_set(const_cast<ap_scalar_t*>(c.get_ap_scalar_t()));
  else ap_scalar_set(l.scalar, const_cast<ap_scalar_t*>(c.get_ap_scalar_t()));
}

inline texpr0::const_iterator tcons0::get_texpr() const
{ 
  if (!l.texpr0) 
    throw std::invalid_argument("apron::tcons0::get_texpr() empty expression");
  return l.texpr0;
}
 
inline texpr0::iterator tcons0::get_texpr()
{ 
  if (!l.texpr0) throw std::invalid_argument("apron::tcons0::get_texpr() empty expression");
  return l.texpr0;
}
 
inline void tcons0::set_texpr(const texpr0::builder& c)
{ 
  ap_texpr0_t* cc = ap_texpr0_copy(const_cast<ap_texpr0_t*>(c.get_ap_texpr0_t()));
  if (l.texpr0) ap_texpr0_free(l.texpr0);
  l.texpr0 = cc;
}


/* print */
/* ===== */

inline std::ostream& operator<< (std::ostream& os, const tcons0& s)
{
  os << s.get_texpr();
  switch (s.get_constyp()) {
  case AP_CONS_EQ:    return os << " = 0";
  case AP_CONS_SUPEQ: return os << " >= 0";
  case AP_CONS_SUP:   return os << " > 0";
  case AP_CONS_EQMOD: return os << " = 0 mod " << s.get_modulo();
  case AP_CONS_DISEQ: return os << " != 0";
  default: throw std::invalid_argument("apron::operator<<(ostream&, const tcons0&) invalid constraint type");
  }
}

inline void tcons0::print(char** name_of_dim, FILE* stream) const
{ 
  ap_tcons0_fprint(stream, const_cast<ap_tcons0_t*>(&l), name_of_dim);
}

 
/* tests */
/* ===== */

inline bool tcons0::is_interval_cst() const
{
  return ap_tcons0_is_interval_cst(const_cast<ap_tcons0_t*>(&l));
}

inline bool tcons0::is_interval_linear() const
{
  return ap_tcons0_is_interval_linear(const_cast<ap_tcons0_t*>(&l));
}

inline bool tcons0::is_interval_polynomial() const
{
  return ap_tcons0_is_interval_polynomial(const_cast<ap_tcons0_t*>(&l));
}

inline bool tcons0::is_interval_polyfrac() const
{
  return ap_tcons0_is_interval_polyfrac(const_cast<ap_tcons0_t*>(&l));
}

inline bool tcons0::is_scalar() const
{
  return ap_tcons0_is_scalar(const_cast<ap_tcons0_t*>(&l));
}



/* C-level compatibility */
/* ===================== */

inline const ap_tcons0_t* tcons0::get_ap_tcons0_t() const
{ 
  return &l; 
}

inline ap_tcons0_t* tcons0::get_ap_tcons0_t()
{ 
  return &l; 
}


/* ================================= */
/* tcons0_array                      */
/* ================================= */


/* constructors */
/* ============ */

inline tcons0_array::tcons0_array(size_t size) 
  : a(ap_tcons0_array_make(size)) 
{
}

inline tcons0_array::tcons0_array(const tcons0_array& x) 
  : a(ap_tcons0_array_make(x.a.size))
{ 
  for (size_t i=0; i<a.size; i++) 
    a.p[i] = ap_tcons0_copy(&x.a.p[i]);
}

inline tcons0_array::tcons0_array(size_t size, const tcons0 x[]) 
  : a(ap_tcons0_array_make(size))
{ 
  for (size_t i=0; i<size; i++) 
    a.p[i] = ap_tcons0_copy(const_cast<ap_tcons0_t*>(x[i].get_ap_tcons0_t()));
}

inline tcons0_array::tcons0_array(const std::vector<tcons0>& x) 
  : a(ap_tcons0_array_make(x.size()))
{ 
  for (size_t i=0; i<a.size; i++) 
    a.p[i] = ap_tcons0_copy(const_cast<ap_tcons0_t*>(x[i].get_ap_tcons0_t()));
}

inline tcons0_array::tcons0_array(const tcons0_array& x, const dimchange& d, bool add)
{
  if (add) 
    a = ap_tcons0_array_add_dimensions(const_cast<ap_tcons0_array_t*>(&x.a), 
				       const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t()));
  else
    a = ap_tcons0_array_remove_dimensions(const_cast<ap_tcons0_array_t*>(&x.a), 
					  const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t()));
}

inline tcons0_array::tcons0_array(const tcons0_array& x, const dimperm& d)
{ 
  a = ap_tcons0_array_permute_dimensions(const_cast<ap_tcons0_array_t*>(&x.a), 
					 const_cast<ap_dimperm_t*>(d.get_ap_dimperm_t()));
}

inline tcons0_array::tcons0_array(const lincons0_array& x)
  : a(ap_tcons0_array_make(x.size()))
{ 
  for (size_t i=0; i<a.size; i++) 
    a.p[i] = ap_tcons0_from_lincons0(const_cast<ap_lincons0_t*>(x[i].get_ap_lincons0_t()));
}


/* destructor */
/* ========== */

inline tcons0_array::~tcons0_array()
{ 
  ap_tcons0_array_clear(&a); 
}


/* assignment */
/* ========== */

inline tcons0_array& tcons0_array::operator= (const tcons0_array& x)
{ 
  if (&x!=this) {
    ap_tcons0_array_clear(&a);
    a = ap_tcons0_array_make(x.a.size);
    for (size_t i=0; i<a.size; i++) a.p[i] = ap_tcons0_copy(&x.a.p[i]);
  }
  return *this;
}

inline tcons0_array& tcons0_array::operator= (const tcons0 x[])
{
  size_t size = a.size;
  ap_tcons0_array_clear(&a);
  a = ap_tcons0_array_make(size);
  for (size_t i=0; i<size; i++) 
    a.p[i] = ap_tcons0_copy(const_cast<ap_tcons0_t*>(x[i].get_ap_tcons0_t()));
  return *this;
}

inline tcons0_array& tcons0_array::operator= (const std::vector<tcons0>& x)
{ 
  size_t size = x.size();
  ap_tcons0_array_clear(&a);
  a = ap_tcons0_array_make(size);
  for (size_t i=0; i<size; i++) 
    a.p[i] = ap_tcons0_copy(const_cast<ap_tcons0_t*>(x[i].get_ap_tcons0_t()));
  return *this;
}

inline tcons0_array& tcons0_array::operator= (const lincons0_array& x)
{ 
  size_t size = x.size();
  ap_tcons0_array_clear(&a);
  a = ap_tcons0_array_make(size);
  for (size_t i=0; i<size; i++) 
    a.p[i] = ap_tcons0_from_lincons0(const_cast<ap_lincons0_t*>(x[i].get_ap_lincons0_t()));
  return *this;
}


/* dimension operations */
/* ==================== */

inline void tcons0_array::resize(size_t size)
{ 
  ap_tcons0_array_resize(&a, size);
}

inline void tcons0_array::add_dimensions(const dimchange& d)
{ 
  ap_tcons0_array_add_dimensions_with(&a, const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t()));
}

inline void tcons0_array::remove_dimensions(const dimchange& d)
{ 
  ap_tcons0_array_remove_dimensions_with(&a, const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t()));
}

inline void tcons0_array::permute_dimensions(const dimperm& d)
{ 
  ap_tcons0_array_permute_dimensions_with(&a, const_cast<ap_dimperm_t*>(d.get_ap_dimperm_t()));
}
  

/* access */
/* ====== */

inline size_t tcons0_array::size() const
{ 
  return a.size;
}
 
inline tcons0* tcons0_array::contents()
{ 
  return reinterpret_cast<tcons0*>(a.p);
}

inline const tcons0* tcons0_array::contents() const
{ 
  return reinterpret_cast<tcons0*>(a.p);
}

inline tcons0& tcons0_array::operator[](size_t i)
{ 
  return reinterpret_cast<tcons0&>(a.p[i]); 
}

inline const tcons0& tcons0_array::operator[](size_t i) const
{ 
  return reinterpret_cast<tcons0&>(a.p[i]); 
}

inline tcons0& tcons0_array::get(size_t i)
{ 
  if (i >= a.size) throw std::out_of_range("apron::tcons0_array::get()");
  return reinterpret_cast<tcons0&>(a.p[i]); 
}

inline const tcons0& tcons0_array::get(size_t i) const
{ 
  if (i >= a.size) throw std::out_of_range("apron::tcons0_array::get()");
  return reinterpret_cast<tcons0&>(a.p[i]); 
}


/* conversion */
/* ========== */

inline tcons0_array::operator std::vector<tcons0>() const
{
  size_t sz = size();
  std::vector<tcons0> v = std::vector<tcons0>(sz);
  for (size_t i=0;i<sz;i++)
    v[i] = (*this)[i];
  return v;
}


/* print */
/* ===== */

inline std::ostream& operator<< (std::ostream& os, const tcons0_array& s)
{
  size_t size = s.size();
  os << "{ ";
  for (size_t i=0;i<size;i++)
    os << s[i] << "; ";
  return os << "}";
}

inline void tcons0_array::print(char** name_of_dim, FILE* stream) const
{ 
  ap_tcons0_array_fprint(stream, const_cast<ap_tcons0_array_t*>(&a), name_of_dim); 
}


/* tests */
/* ===== */

inline bool tcons0_array::is_interval_linear() const
{ 
  return ap_tcons0_array_is_interval_linear(const_cast<ap_tcons0_array_t*>(&a)); 
}

/* C-level compatibility */
/* ===================== */

inline const ap_tcons0_array_t* tcons0_array::get_ap_tcons0_array_t() const
{ 
  return &a;
}

inline ap_tcons0_array_t* tcons0_array::get_ap_tcons0_array_t()
{ 
  return &a;
}
