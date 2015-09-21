/* -*- C++ -*-
 * apxx_lincons1_inline.hh
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
/* lincons1                          */
/* ================================= */


/* constructors */
/* ============ */

inline lincons1::lincons1(ap_lincons1_t p) : l(p)
{}

inline lincons1::lincons1(const environment& e, const lincons0& x)
{
  l = ap_lincons1_of_lincons0(const_cast<ap_environment_t*>(e.get_ap_environment_t()),
			      ap_lincons0_copy(const_cast<ap_lincons0_t*>(x.get_ap_lincons0_t())));
}

inline lincons1::lincons1(const environment& e, ap_constyp_t constyp)
{
  ap_linexpr1_t llin = 
    ap_linexpr1_make(const_cast<ap_environment_t*>(e.get_ap_environment_t()), AP_LINEXPR_SPARSE, 0);
  l = ap_lincons1_make(constyp, &llin, NULL);
}

inline lincons1::lincons1(ap_constyp_t constyp, const linexpr1& lin)
{
  ap_linexpr1_t llin = ap_linexpr1_copy(const_cast<ap_linexpr1_t*>(lin.get_ap_linexpr1_t()));
  l = ap_lincons1_make(constyp, &llin, NULL);
}

inline lincons1::lincons1(ap_constyp_t constyp, const linexpr1& lin, const scalar& modulo)
{
  ap_linexpr1_t llin = ap_linexpr1_copy(const_cast<ap_linexpr1_t*>(lin.get_ap_linexpr1_t()));
  ap_scalar_t* mmodulo = ap_scalar_alloc_set(const_cast<ap_scalar_t*>(modulo.get_ap_scalar_t()));
  l = ap_lincons1_make(constyp, &llin, mmodulo);
}

inline lincons1::lincons1(const environment& e, unsat x)
{
  l = ap_lincons1_make_unsat(const_cast<ap_environment_t*>(e.get_ap_environment_t()));
}

inline lincons1::lincons1(const lincons1& x)
{
  l = ap_lincons1_copy(const_cast<ap_lincons1_t*>(&x.l));
}

inline lincons1::lincons1(const lincons1& x, const environment& e)
{
  if (!x.has_linexpr())
    throw std::invalid_argument("apron::lincons1::lincons1(const lincons1&, const environment&) empty expression");
  bool r =
    ap_lincons1_extend_environment(&l,
				   const_cast<ap_lincons1_t*>(&x.l),
				   const_cast<ap_environment_t*>(e.get_ap_environment_t()));
  if (r) throw std::invalid_argument("apron::lincons1::lincons1(const lincons1&, const environment&) not a super-environment");
}


/* destructor */
/* ========== */

inline lincons1::~lincons1()
{
  ap_lincons1_clear(&l);
}


/* assignment */
/* ========== */

inline lincons1& lincons1::operator= (const lincons1& x)
{
  ap_lincons1_t ll  = ap_lincons1_copy(const_cast<ap_lincons1_t*>(&x.l));
  ap_lincons1_clear(&l);
  l = ll;
  return *this;
}

inline lincons1& lincons1::operator= (unsat x)
{
  ap_lincons1_t ll = ap_lincons1_make_unsat(ap_lincons1_envref(&l));
  ap_lincons1_clear(&l);
  l = ll;
  return *this;
}

inline void lincons1::set_modulo(const scalar& c)
{
  get_lincons0().set_modulo(c);
}

inline void lincons1::set_linexpr(const linexpr1& c)
{
  get_lincons0().set_linexpr(c.get_linexpr0());
}


/* dimension operations */
/* ==================== */

inline void lincons1::extend_environment(const environment& e)
{
  if (!has_linexpr())
    throw std::invalid_argument("apron::lincons1::extend_environment(const environment&) empty expression");
  bool r =
    ap_lincons1_extend_environment_with(&l,
					const_cast<ap_environment_t*>(e.get_ap_environment_t()));
  if (r) throw std::invalid_argument("apron::lincons1::extend_environment(const environment&) not a super-environment");
  
}

  
/* access */
/* ====== */

inline environment lincons1::get_environment() const
{
  return (ap_environment_copy(ap_lincons1_envref(const_cast<ap_lincons1_t*>(&l))));  
}

inline const lincons0& lincons1::get_lincons0() const
{
  return reinterpret_cast<lincons0&>(*ap_lincons1_lincons0ref(const_cast<ap_lincons1_t*>(&l)));
}

inline lincons0& lincons1::get_lincons0()
{
  return reinterpret_cast<lincons0&>(*ap_lincons1_lincons0ref(&l));
}

inline size_t lincons1::size() const
{
  return get_lincons0().size();
}

inline ap_constyp_t& lincons1::get_constyp()
{
  return get_lincons0().get_constyp();
}

inline const ap_constyp_t& lincons1::get_constyp() const
{
  return get_lincons0().get_constyp();
}

inline bool lincons1::has_modulo() const
{
  return get_lincons0().has_modulo();
}

inline bool lincons1::has_linexpr() const
{
  return get_lincons0().has_linexpr();
}

inline scalar& lincons1::get_modulo()
{
  if (!has_modulo()) 
    throw std::invalid_argument("apron::lincons1::get_modulo() empty scalar");
  return get_lincons0().get_modulo();
}

inline const scalar& lincons1::get_modulo() const
{
  if (!has_modulo()) 
    throw std::invalid_argument("apron::lincons1::get_modulo() empty scalar");
  return get_lincons0().get_modulo();
}

inline linexpr1 lincons1::get_linexpr() const
{
  if (!has_linexpr()) 
    throw std::invalid_argument("apron::lincons1::get_linexpr() empty expression");
  linexpr0 ll = get_lincons0().get_linexpr();
  return linexpr1(get_environment(),ll);
}

inline coeff& lincons1::get_cst()
{ 
  if (!has_linexpr()) 
    throw std::invalid_argument("apron::lincons1::get_cst() empty expression");
  return reinterpret_cast<coeff&>(*ap_lincons1_cstref(&l));
}
 
inline const coeff& lincons1::get_cst() const
{ 
  if (!has_linexpr()) 
    throw std::invalid_argument("apron::lincons1::get_cst() empty expression");
  return reinterpret_cast<coeff&>(*ap_lincons1_cstref(const_cast<ap_lincons1_t*>(&l))); 
}

inline coeff& lincons1::operator[](const var& var)
{ 
  if (!has_linexpr()) 
    throw std::invalid_argument("apron::lincons1::operator[](const var&) empty expression");
  ap_coeff_t* x = ap_lincons1_coeffref(&l, 
				       const_cast<ap_var_t>(var.get_ap_var_t()));
  if (!x) 
    throw std::invalid_argument("apron::lincons1::operator[](const var&) variable not in environment");
  return reinterpret_cast<coeff&>(*x); 
}
  
inline const coeff& lincons1::operator[](const var& var) const
{ 
  if (!has_linexpr()) 
    throw std::invalid_argument("apron::lincons1::operator[](const var&) empty expression");
  ap_coeff_t* x = ap_lincons1_coeffref(const_cast<ap_lincons1_t*>(&l), 
				       const_cast<ap_var_t>(var.get_ap_var_t()));
  if (!x) 
    throw std::invalid_argument("apron::lincons1::operator[](const var&) variable not in environment");
  return reinterpret_cast<coeff&>(*x); 
}


/* print */
/* ===== */

inline std::ostream& operator<< (std::ostream& os, const lincons1& s)
{
  os << s.get_linexpr();
  switch (s.get_constyp()) {
  case AP_CONS_EQ:    return os << " = 0";
  case AP_CONS_SUPEQ: return os << " >= 0";
  case AP_CONS_SUP:   return os << " > 0";
  case AP_CONS_EQMOD: return os << " = 0 mod " << s.get_modulo();
  case AP_CONS_DISEQ: return os << " != 0";
  default: throw std::invalid_argument("apron::operator<<(ostream&, const lincons1&) invalid constraint type");
  }
}

inline void lincons1::print(FILE* stream) const
{
  ap_lincons1_fprint(stream, const_cast<ap_lincons1_t*>(&l));  
}


/* tests */
/* ===== */

inline bool lincons1::is_unsat() const
{
  return ap_lincons1_is_unsat(const_cast<ap_lincons1_t*>(&l));  
}
  
inline bool lincons1::is_linear() const
{
  return get_lincons0().is_linear();
}

inline bool lincons1::is_quasilinear() const
{
  return get_lincons0().is_quasilinear();
}


/* C-level compatibility */
/* ===================== */

inline const ap_lincons1_t* lincons1::get_ap_lincons1_t() const
{
  return &l;
}

inline ap_lincons1_t* lincons1::get_ap_lincons1_t()
{
  return &l;
}




/* ================================= */
/* lincons1_array                    */
/* ================================= */



/* constructors */
/* ============ */

inline lincons1_array::lincons1_array(ap_lincons1_array_t& a) : a(a) 
{}

inline lincons1_array::lincons1_array(const environment& e, const lincons0_array& x)
{
  size_t sz = x.size();
  a = ap_lincons1_array_make(const_cast<ap_environment_t*>(e.get_ap_environment_t()), sz);
  for (size_t i=0; i<sz; i++) 
    a.lincons0_array.p[i] = ap_lincons0_copy(&x.a.p[i]);
}

inline lincons1_array::lincons1_array(const environment& e, size_t size)
{
  a = ap_lincons1_array_make(const_cast<ap_environment_t*>(e.get_ap_environment_t()), size);
}

inline lincons1_array::lincons1_array(const lincons1_array& x)
{
  size_t sz = x.size();
  a = ap_lincons1_array_make(x.get_environment().get_ap_environment_t(), sz);
  for (size_t i=0; i<sz; i++) 
    a.lincons0_array.p[i] = ap_lincons0_copy(&x.a.lincons0_array.p[i]);
}

inline lincons1_array::lincons1_array(const lincons1_array& x, const environment& e)
{
  bool r = 
    ap_lincons1_array_extend_environment(&a,
					 const_cast<ap_lincons1_array_t*>(&x.a),
					 const_cast<ap_environment_t*>(e.get_ap_environment_t()));
  if (r) throw std::invalid_argument("apron::lincons1_array::lincons1_array(const lincons1_array, const environment&) not a super-environment");
}

inline lincons1_array::lincons1_array(size_t sz, const lincons1 x[])
{
  if (sz<1) throw std::invalid_argument("apron::lincons1_array::lincons1_array(size_t, const lincons1) null size");
  a = ap_lincons1_array_make(x[0].get_environment().get_ap_environment_t(), sz);
  for (size_t i=0; i<sz; i++) 
    a.lincons0_array.p[i] = ap_lincons0_copy(const_cast<ap_lincons0_t*>
					     (x[i].get_lincons0().get_ap_lincons0_t()));
}

inline lincons1_array::lincons1_array(const std::vector<lincons1>& x)
{
  size_t sz = x.size();
  if (sz<1) throw std::invalid_argument("apron::lincons1_array::lincons1_array(const vector<lincons1>&) null size");
  a = ap_lincons1_array_make(x[0].get_environment().get_ap_environment_t(), sz);
  for (size_t i=0; i<sz; i++) 
    a.lincons0_array.p[i] = ap_lincons0_copy(const_cast<ap_lincons0_t*>
					     (x[i].get_lincons0().get_ap_lincons0_t()));
}


/* destructor */
/* ========== */

inline lincons1_array::~lincons1_array()
{
  ap_lincons1_array_clear(&a);
}


/* assignment */
/* ========== */

inline lincons1_array& lincons1_array::operator= (const lincons1_array& x)
{
  if (this!=&x) {
    size_t sz = x.size();
    ap_lincons1_array_clear(&a);
    a = ap_lincons1_array_make(x.get_environment().get_ap_environment_t(), sz);
    for (size_t i=0; i<sz; i++) 
      a.lincons0_array.p[i] = ap_lincons0_copy(&x.a.lincons0_array.p[i]);
  }
  return *this;
}

inline lincons1_array& lincons1_array::operator= (const lincons1 x[])
{
  size_t sz = size();
  for (size_t i=0; i<sz; i++) {
    ap_lincons0_clear(&a.lincons0_array.p[i]);
    a.lincons0_array.p[i] = ap_lincons0_copy(const_cast<ap_lincons0_t*>
					     (x[i].get_lincons0().get_ap_lincons0_t()));
  }
  return *this;
}

inline lincons1_array& lincons1_array::operator= (const std::vector<lincons1>& x)
{
  size_t size = x.size();
  if (size<1) {
    ap_lincons1_array_t aa = ap_lincons1_array_make(a.env,0);
    ap_lincons1_array_clear(&a);
    a = aa;
    return *this;
  }
  ap_lincons1_array_clear(&a);
  a = ap_lincons1_array_make(x[0].get_environment().get_ap_environment_t(), size);
  for (size_t i=0; i<size; i++) 
    a.lincons0_array.p[i] = ap_lincons0_copy(const_cast<ap_lincons0_t*>
					     (x[i].get_lincons0().get_ap_lincons0_t()));
  return *this;
}


/* dimension operations */
/* ==================== */

inline void lincons1_array::resize(size_t size)
{
  ap_lincons0_array_resize(&a.lincons0_array, size);
}

inline void lincons1_array::extend_environment(const environment& e)
{
  bool r =
    ap_lincons1_array_extend_environment_with(&a,
					      const_cast<ap_environment_t*>(e.get_ap_environment_t()));
  if (r) throw std::invalid_argument("apron::lincons1_array::extend_environment(const environment&) not a super-environment");
}


/* access */
/* ====== */

inline size_t lincons1_array::size() const
{
  return ap_lincons1_array_size(const_cast<ap_lincons1_array_t*>(&a));
}

inline environment lincons1_array::get_environment() const
{
  return (ap_environment_copy(ap_lincons1_array_envref(const_cast<ap_lincons1_array_t*>(&a))));
}

inline const lincons0_array& lincons1_array::get_lincons0_array() const
{
  return reinterpret_cast<lincons0_array&>(const_cast<ap_lincons0_array_t&>(a.lincons0_array));
}

inline lincons0_array& lincons1_array::get_lincons0_array()
{
  return reinterpret_cast<lincons0_array&>(a.lincons0_array);
}

inline lincons1 lincons1_array::get(size_t i) const
{
  if (i>=size()) throw std::out_of_range("apron::lincons1_array::get(size_t)");
  ap_lincons1_t c = ap_lincons1_array_get(const_cast<ap_lincons1_array_t*>(&a),i);
  return lincons1(ap_lincons1_copy(&c));
}

inline void lincons1_array::set(size_t i, const lincons1& x)
{
  if (i>=size()) throw std::out_of_range("apron::lincons1_array::set(size_t, const lincons1&)");
  ap_lincons0_clear(&a.lincons0_array.p[i]);
  a.lincons0_array.p[i] = ap_lincons0_copy(const_cast<ap_lincons0_t*>
					   (x.get_lincons0().get_ap_lincons0_t()));
}


/* conversion */
/* ========== */
  
inline lincons1_array::operator std::vector<lincons1>() const
{
  size_t sz = size();
  lincons1 dummy(get_environment(),unsat());
  std::vector<lincons1> v(sz,dummy);
  for (size_t i=0;i<sz;i++) {
    ap_lincons1_t c = ap_lincons1_array_get(const_cast<ap_lincons1_array_t*>(&a),i);
    v[i] = ap_lincons1_copy(&c);
  }
  return v;
}


/* print */
/* ===== */

inline std::ostream& operator<< (std::ostream& os, const lincons1_array& s)
{
  size_t sz = s.size();
  os << "{ ";
  for (size_t i=0;i<sz;i++)
    os << s.get(i) << "; ";
  return os << "}";
}

inline void lincons1_array:: print(FILE* stream) const
{
  ap_lincons1_array_fprint(stream, const_cast<ap_lincons1_array_t*>(&a));
}


/* C-level compatibility */
/* ===================== */

inline const ap_lincons1_array_t* lincons1_array::get_ap_lincons1_array_t() const
{
  return &a;
}

inline ap_lincons1_array_t* lincons1_array::get_ap_lincons1_array_t()
{
  return &a;
}

