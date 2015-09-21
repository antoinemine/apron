/* -*- C++ -*-
 * apxx_generator1_inline.hh
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
/* generator1                        */
/* ================================= */


/* constructors */
/* ============ */

inline generator1::generator1(ap_generator1_t p) : l(p)
{}

inline generator1::generator1(const environment& e, const generator0& x)
{
  l.env = ap_environment_copy(const_cast<ap_environment_t*>(e.get_ap_environment_t()));
  l.generator0 = ap_generator0_copy(const_cast<ap_generator0_t*>(x.get_ap_generator0_t()));
}

inline generator1::generator1(const environment& e, ap_gentyp_t gentyp)
{
  ap_linexpr1_t llin = 
    ap_linexpr1_make(const_cast<ap_environment_t*>(e.get_ap_environment_t()), AP_LINEXPR_SPARSE, 0);
  l = ap_generator1_make(gentyp, &llin);
}

inline generator1::generator1(ap_gentyp_t gentyp, const linexpr1& lin)
{
  ap_linexpr1_t llin = ap_linexpr1_copy(const_cast<ap_linexpr1_t*>(lin.get_ap_linexpr1_t()));
  l = ap_generator1_make(gentyp, &llin);
}

inline generator1::generator1(const generator1& x)
{
  l = ap_generator1_copy(const_cast<ap_generator1_t*>(&x.l));
}

inline generator1::generator1(const generator1& x, const environment& e)
{
  if (!x.has_linexpr())
    throw std::invalid_argument("apron::generator1::generator1(const generator&, const environment&) empty expression");
  bool r =
    ap_generator1_extend_environment(&l,
				   const_cast<ap_generator1_t*>(&x.l),
				   const_cast<ap_environment_t*>(e.get_ap_environment_t()));
  if (r) throw std::invalid_argument("apron::generator1::generator1(const generator&, const environment&) not a super-environment");
}


/* destructor */
/* ========== */

inline generator1::~generator1()
{
  ap_generator1_clear(&l);
}


/* assignment */
/* ========== */

inline generator1& generator1::operator= (const generator1& x)
{
  ap_generator1_t ll  = ap_generator1_copy(const_cast<ap_generator1_t*>(&x.l));
  ap_generator1_clear(&l);
  l = ll;
  return *this;
}

inline void generator1::set_linexpr(const linexpr1& c)
{
  get_generator0().set_linexpr(c.get_linexpr0());
}


/* dimension operations */
/* ==================== */

inline void generator1::extend_environment(const environment& e)
{
   if (!has_linexpr())
    throw std::invalid_argument("apron::generator1::extend_environment(cconst environment&) empty expression");
 bool r =
    ap_generator1_extend_environment_with(&l,
					const_cast<ap_environment_t*>(e.get_ap_environment_t()));
  if (r) throw std::invalid_argument("apron::generator1::extend_environment(const environment&) not a super-environment");
  
}

  
/* access */
/* ====== */

inline environment generator1::get_environment() const
{
  return (ap_environment_copy(ap_generator1_envref(const_cast<ap_generator1_t*>(&l))));  
}

inline const generator0& generator1::get_generator0() const
{
  return reinterpret_cast<generator0&>(*ap_generator1_generator0ref(const_cast<ap_generator1_t*>(&l)));
}

inline generator0& generator1::get_generator0()
{
  return reinterpret_cast<generator0&>(*ap_generator1_generator0ref(&l));
}

inline size_t generator1::size() const
{
  return get_generator0().size();
}

inline ap_gentyp_t& generator1::get_gentyp()
{
  return get_generator0().get_gentyp();
}
  
inline const ap_gentyp_t& generator1::get_gentyp() const
{
  return get_generator0().get_gentyp();
}

inline bool generator1::has_linexpr() const
{
  return get_generator0().has_linexpr();
}

inline linexpr1 generator1::get_linexpr() const
{
  if (!has_linexpr())
    throw std::invalid_argument("apron::generator1::get_linexpr() empty expression"); 
  linexpr0 ll = get_generator0().get_linexpr();
  return linexpr1(get_environment(),ll);
}

inline coeff& generator1::get_cst()
{ 
  if (!has_linexpr())
    throw std::invalid_argument("apron::generator1::get_cst() empty expression"); 
  return reinterpret_cast<coeff&>(*ap_generator1_cstref(&l));
}
 
inline const coeff& generator1::get_cst() const
{ 
  if (!has_linexpr())
    throw std::invalid_argument("apron::generator1::get_cst() empty expression"); 
  return reinterpret_cast<coeff&>(*ap_generator1_cstref(const_cast<ap_generator1_t*>(&l))); 
}

inline coeff& generator1::operator[](const var& var)
{ 
  if (!has_linexpr())
    throw std::invalid_argument("apron::generator1::operator[](const var&) empty expression"); 
  ap_coeff_t* x = ap_generator1_coeffref(&l, const_cast<ap_var_t>(var.get_ap_var_t()));
  if (!x) throw std::invalid_argument("apron::generator1::operator[](const var&) variable not in environment");
  return reinterpret_cast<coeff&>(*x); 
}
  
inline const coeff& generator1::operator[](const var& var) const
{ 
  if (!has_linexpr())
    throw std::invalid_argument("apron::generator1::operator[](const var&) empty expression"); 
  ap_coeff_t* x = ap_generator1_coeffref(const_cast<ap_generator1_t*>(&l), 
				       const_cast<ap_var_t>(var.get_ap_var_t()));
  if (!x) throw std::invalid_argument("apron::generator1::operator[](const var&) variable not in environment");
  return reinterpret_cast<coeff&>(*x); 
}


/* print */
/* ===== */

inline std::ostream& operator<< (std::ostream& os, const generator1& s)
{
  if (!s.has_linexpr())
      throw std::invalid_argument("apron::operator<<(ostream&, const generator1&) empty expression");
  switch (s.get_gentyp()) {
  case AP_GEN_LINE:     os << "LINE: "; break;
  case AP_GEN_RAY:      os << "RAY: "; break;
  case AP_GEN_VERTEX:   os << "VERTEX: "; break;
  case AP_GEN_LINEMOD:  os << "LINEMOD: "; break;
  case AP_GEN_RAYMOD:   os << "RAYMOD: "; break;
  default: throw std::invalid_argument("apron::operator<<(ostream&, const generator1&) invalid generator type");
  }
  return os << s.get_linexpr();
}

inline void generator1::print(FILE* stream) const
{
  ap_generator1_fprint(stream, const_cast<ap_generator1_t*>(&l));  
}


/* C-level compatibility */
/* ===================== */

inline const ap_generator1_t* generator1::get_ap_generator1_t() const
{
  return &l;
}

inline ap_generator1_t* generator1::get_ap_generator1_t()
{
  return &l;
}




/* ================================= */
/* generator1_array                  */
/* ================================= */



/* constructors */
/* ============ */

inline generator1_array::generator1_array(ap_generator1_array_t& a) : a(a) 
{}

inline generator1_array::generator1_array(const environment& e, const generator0_array& x)
{
  size_t sz = x.size();
  a = ap_generator1_array_make(const_cast<ap_environment_t*>(e.get_ap_environment_t()), sz);
  for (size_t i=0; i<sz; i++) 
    a.generator0_array.p[i] = ap_generator0_copy(&x.a.p[i]);
}

inline generator1_array::generator1_array(const environment& e, size_t size)
{
  a = ap_generator1_array_make(const_cast<ap_environment_t*>(e.get_ap_environment_t()), size);
}

inline generator1_array::generator1_array(const generator1_array& x)
{
  size_t sz = x.size();
  a = ap_generator1_array_make(x.get_environment().get_ap_environment_t(), sz);
  for (size_t i=0; i<sz; i++) 
    a.generator0_array.p[i] = ap_generator0_copy(&x.a.generator0_array.p[i]);
}

inline generator1_array::generator1_array(const generator1_array& x, const environment& e)
{
  bool r = 
    ap_generator1_array_extend_environment(&a,
					 const_cast<ap_generator1_array_t*>(&x.a),
					 const_cast<ap_environment_t*>(e.get_ap_environment_t()));
  if (r) throw std::invalid_argument("apron::generator1_array::generator1_array(const generator1_array&, const environment&) not a super-environment");
}

inline generator1_array::generator1_array(size_t sz, const generator1 x[])
{
  if (sz<1) throw std::invalid_argument("apron::generator1_array::generator1_array(size_t sz, const generator1[]) null size");
  a = ap_generator1_array_make(x[0].get_environment().get_ap_environment_t(), sz);
  for (size_t i=0; i<sz; i++) 
    a.generator0_array.p[i] = ap_generator0_copy(const_cast<ap_generator0_t*>
					     (x[i].get_generator0().get_ap_generator0_t()));
}

inline generator1_array::generator1_array(const std::vector<generator1>& x)
{
  size_t sz = x.size();
  if (sz<1) throw std::invalid_argument("apron::generator1_array::generator1_array(const vector<generator1>&) null size");
  a = ap_generator1_array_make(x[0].get_environment().get_ap_environment_t(), sz);
  for (size_t i=0; i<sz; i++) 
    a.generator0_array.p[i] = ap_generator0_copy(const_cast<ap_generator0_t*>
					     (x[i].get_generator0().get_ap_generator0_t()));
}


/* destructor */
/* ========== */

inline generator1_array::~generator1_array()
{
  ap_generator1_array_clear(&a);
}


/* assignment */
/* ========== */

inline generator1_array& generator1_array::operator= (const generator1_array& x)
{
  if (this!=&x) {
    size_t sz = x.size();
    ap_generator1_array_clear(&a);
    a = ap_generator1_array_make(x.get_environment().get_ap_environment_t(), sz);
    for (size_t i=0; i<sz; i++) 
      a.generator0_array.p[i] = ap_generator0_copy(&x.a.generator0_array.p[i]);
  }
  return *this;
}

inline generator1_array& generator1_array::operator= (const generator1 x[])
{
  size_t sz = size();
  for (size_t i=0; i<sz; i++) {
    ap_generator0_clear(&a.generator0_array.p[i]);
    a.generator0_array.p[i] = ap_generator0_copy(const_cast<ap_generator0_t*>
					     (x[i].get_generator0().get_ap_generator0_t()));
  }
  return *this;
}

inline generator1_array& generator1_array::operator= (const std::vector<generator1>& x)
{
  size_t size = x.size();
  if (size<1) {
    ap_generator1_array_t aa = ap_generator1_array_make(a.env,0);
    ap_generator1_array_clear(&a);
    a = aa;
    return *this;
  }
  ap_generator1_array_clear(&a);
  a = ap_generator1_array_make(x[0].get_environment().get_ap_environment_t(), size);
  for (size_t i=0; i<size; i++) 
    a.generator0_array.p[i] = ap_generator0_copy(const_cast<ap_generator0_t*>
					     (x[i].get_generator0().get_ap_generator0_t()));
  return *this;
}


/* dimension operations */
/* ==================== */

inline void generator1_array::resize(size_t size)
{
  ap_generator0_array_resize(&a.generator0_array, size);
}

inline void generator1_array::extend_environment(const environment& e)
{
  bool r =
    ap_generator1_array_extend_environment_with(&a,
					      const_cast<ap_environment_t*>(e.get_ap_environment_t()));
  if (r) throw std::invalid_argument("apron::generator1_array::extend_environment(const environment&) not a super-environment");
}


/* access */
/* ====== */

inline size_t generator1_array::size() const
{
  return ap_generator1_array_size(const_cast<ap_generator1_array_t*>(&a));
}

inline environment generator1_array::get_environment() const
{
  return (ap_environment_copy(ap_generator1_array_envref(const_cast<ap_generator1_array_t*>(&a))));
}

inline const generator0_array& generator1_array::get_generator0_array() const
{
  return reinterpret_cast<generator0_array&>(const_cast<ap_generator0_array_t&>(a.generator0_array));
}

inline generator0_array& generator1_array::get_generator0_array()
{
  return reinterpret_cast<generator0_array&>(a.generator0_array);
}

inline generator1 generator1_array::get(size_t i) const
{
  if (i>=size()) throw std::out_of_range("apron::generator1_array::get(size_t)");
  ap_generator1_t c = ap_generator1_array_get(const_cast<ap_generator1_array_t*>(&a),i);
  return generator1(ap_generator1_copy(&c));
}

inline void generator1_array::set(size_t i, const generator1& x)
{
  if (i>=size()) throw std::out_of_range("apron::generator1_array::get(size_t)");
  ap_generator0_clear(&a.generator0_array.p[i]);
  a.generator0_array.p[i] = ap_generator0_copy(const_cast<ap_generator0_t*>
					   (x.get_generator0().get_ap_generator0_t()));
}


/* conversion */
/* ========== */
  
inline generator1_array::operator std::vector<generator1>() const
{
  size_t sz = size();
  generator1 dummy(get_environment(),generator0());
  std::vector<generator1> v(sz,dummy);
  for (size_t i=0;i<sz;i++) {
    ap_generator1_t c = ap_generator1_array_get(const_cast<ap_generator1_array_t*>(&a),i);
    v[i] = ap_generator1_copy(&c);
  }
  return v;
}


/* print */
/* ===== */

inline std::ostream& operator<< (std::ostream& os, const generator1_array& s)
{
  size_t sz = s.size();
  os << "{ ";
  for (size_t i=0;i<sz;i++)
    os << s.get(i) << "; ";
  return os << "}";
}

inline void generator1_array:: print(FILE* stream) const
{
  ap_generator1_array_fprint(stream, const_cast<ap_generator1_array_t*>(&a));
}


/* C-level compatibility */
/* ===================== */

inline const ap_generator1_array_t* generator1_array::get_ap_generator1_array_t() const
{
  return &a;
}

inline ap_generator1_array_t* generator1_array::get_ap_generator1_array_t()
{
  return &a;
}

