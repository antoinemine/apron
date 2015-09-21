/* -*- C++ -*-
 * apxx_tcons1_inline.hh
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
/* tcons1                            */
/* ================================= */


/* constructors */
/* ============ */

inline tcons1::tcons1(ap_tcons1_t l) : l(l) 
{}

inline tcons1::tcons1(const environment& env, ap_constyp_t constyp)
{
  l.tcons0 = ap_tcons0_make(constyp, NULL, NULL);
  l.env = ap_environment_copy(const_cast<ap_environment_t*>(env.get_ap_environment_t()));
}

inline tcons1::tcons1(ap_constyp_t constyp, const texpr1::builder& t)
{
  ap_texpr1_t* x = ap_texpr1_copy(const_cast<ap_texpr1_t*>(t.get_ap_texpr1_t()));
  l = ap_tcons1_make(constyp, x, NULL);
  free(x);
}

inline tcons1::tcons1(ap_constyp_t constyp, const texpr1::builder& t, const scalar& modulo)
{
  ap_scalar_t* mmodulo = ap_scalar_alloc_set(const_cast<ap_scalar_t*>(modulo.get_ap_scalar_t()));
  ap_texpr1_t* x = ap_texpr1_copy(const_cast<ap_texpr1_t*>(t.get_ap_texpr1_t()));
  l = ap_tcons1_make(constyp, x, mmodulo);
  free(x);
}

inline tcons1::tcons1(const tcons1& x)
  : l(ap_tcons1_copy(const_cast<ap_tcons1_t*>(&x.l)))
{}

inline tcons1::tcons1(const tcons1& x, const environment& e)
{
  if (!x.l.tcons0.texpr0)
    throw std::invalid_argument("apron::tcons1::tcons1(const tcons1&, const enviroment&) empty expression");
  bool r =
    ap_tcons1_extend_environment(&l,
				 const_cast<ap_tcons1_t*>(&x.l),
				 const_cast<ap_environment_t*>(e.get_ap_environment_t()));
  if (r)
    throw std::invalid_argument("apron::tcons1::tcons1(const tcons1&, const enviroment&) not a super-environment");
}

inline tcons1::tcons1(const environment& e, unsat x)
{
  l.tcons0 = ap_tcons0_make_unsat();
  l.env = ap_environment_copy(const_cast<ap_environment_t*>(e.get_ap_environment_t()));  
}

inline tcons1::tcons1(const lincons1& x)
  : l(ap_tcons1_from_lincons1(const_cast<ap_lincons1_t*>(x.get_ap_lincons1_t())))
{}


/* destructor */
/* ========== */

inline tcons1::~tcons1()
{
  ap_tcons1_clear(&l);
}


/* 'Intelligent' constructors */
/* ========================== */

inline tcons1 operator>=(const texpr1::builder& a, const texpr1::builder& b)
{
  if (b.is_zero())      return tcons1(AP_CONS_SUPEQ,a);
  else if (a.is_zero()) return tcons1(AP_CONS_SUPEQ,-b);
  else         	        return tcons1(AP_CONS_SUPEQ,a-b);
}

inline tcons1 operator<=(const texpr1::builder& a, const texpr1::builder& b)
{
  if (b.is_zero())      return tcons1(AP_CONS_SUPEQ,-a);
  else if (a.is_zero()) return tcons1(AP_CONS_SUPEQ,b);
  else         	        return tcons1(AP_CONS_SUPEQ,b-a);
}

inline tcons1 operator> (const texpr1::builder& a, const texpr1::builder& b)
{
  if (b.is_zero())      return tcons1(AP_CONS_SUP,a);
  else if (a.is_zero()) return tcons1(AP_CONS_SUP,-b);
  else         	        return tcons1(AP_CONS_SUP,a-b);
}

inline tcons1 operator< (const texpr1::builder& a, const texpr1::builder& b)
{
  if (b.is_zero())      return tcons1(AP_CONS_SUP,-a);
  else if (a.is_zero()) return tcons1(AP_CONS_SUP,b);
  else         	        return tcons1(AP_CONS_SUP,b-a);
}

inline tcons1 operator==(const texpr1::builder& a, const texpr1::builder& b)
{
  if (b.is_zero())      return tcons1(AP_CONS_EQ,a);
  else if (a.is_zero()) return tcons1(AP_CONS_EQ,b);
  else         	        return tcons1(AP_CONS_EQ,a-b);
}

inline tcons1 operator!=(const texpr1::builder& a, const texpr1::builder& b)
{
  if (b.is_zero())      return tcons1(AP_CONS_DISEQ,a);
  else if (a.is_zero()) return tcons1(AP_CONS_DISEQ,b);
  else         	        return tcons1(AP_CONS_DISEQ,a-b);
}


/* assignment */
/* ========== */

inline tcons1& tcons1::operator= (const tcons1& x)
{
  if (&x!=this) {
    ap_tcons1_clear(&l);
    l = ap_tcons1_copy(const_cast<ap_tcons1_t*>(&x.l)); 
  }
  return *this;
}

inline tcons1& tcons1::operator= (unsat x)
{
  ap_tcons0_clear(&l.tcons0);
  l.tcons0 = ap_tcons0_make_unsat();
  return *this;
}

inline tcons1& tcons1::operator= (const lincons1& x)
{
  ap_tcons1_clear(&l); 
  l = ap_tcons1_from_lincons1(const_cast<ap_lincons1_t*>(x.get_ap_lincons1_t()));
  return *this;
}

inline void tcons1::set_modulo(const scalar& c)
{
  get_tcons0().set_modulo(c);
}

inline void tcons1::set_texpr(const texpr1::builder& c)
{
  ap_texpr1_t* cc = ap_texpr1_copy(const_cast<ap_texpr1_t*>(c.get_ap_texpr1_t()));
  if (l.tcons0.texpr0) ap_texpr0_free(l.tcons0.texpr0);
  ap_environment_free(l.env);
  l.tcons0.texpr0 = cc->texpr0;
  l.env = cc->env;
  free(cc);
}


/* dimension operations */
/* ==================== */

inline void tcons1::extend_environment(const environment& e)
{
  bool r = 
    ap_tcons1_extend_environment_with(&l,
				      const_cast<ap_environment_t*>(e.get_ap_environment_t()));
  if (r)
    throw std::invalid_argument("apron::tcons1::extend_environment(const environment&) not a super-environment");
}


/* access */
/* ====== */

inline environment tcons1::get_environment() const
{
  return ap_environment_copy(l.env);
}

inline const tcons0& tcons1::get_tcons0() const
{
  return reinterpret_cast<const tcons0&>(l.tcons0);
}

inline tcons0& tcons1::get_tcons0()
{
  return reinterpret_cast<tcons0&>(l.tcons0);
}

inline ap_constyp_t& tcons1::get_constyp()
{
  return get_tcons0().get_constyp();
}

inline const ap_constyp_t& tcons1::get_constyp() const
{
  return get_tcons0().get_constyp();
}

inline bool tcons1::has_modulo() const
{
  return get_tcons0().has_modulo();
}

inline bool tcons1::has_texpr() const
{
  return get_tcons0().has_texpr();
}

inline scalar& tcons1::get_modulo()
{
  if (!get_tcons0().has_modulo()) throw std::invalid_argument("apron::tcons1::get_modulo() empty scalar");
  return get_tcons0().get_modulo();
}
 
inline const scalar& tcons1::get_modulo() const
{
  if (!get_tcons0().has_modulo()) throw std::invalid_argument("apron::tcons1::get_modulo() empty scalar");
  return get_tcons0().get_modulo();
}

inline texpr1::iterator tcons1::get_texpr()
{
  if (!l.tcons0.texpr0) throw std::invalid_argument("apron::tcons1::get_texpr() empty expression");
  return texpr1::iterator(l.tcons0.texpr0,l.env);
}

inline texpr1::const_iterator tcons1::get_texpr() const
{
  if (!l.tcons0.texpr0) throw std::invalid_argument("apron::tcons1::get_texpr() empty expression");
  return texpr1::const_iterator(l.tcons0.texpr0,l.env);
}

 

/* print */
/* ===== */

inline std::ostream& operator<< (std::ostream& os, const tcons1& s)
{
  os << s.get_texpr();
  switch (s.get_constyp()) {
  case AP_CONS_EQ:    return os << " = 0";
  case AP_CONS_SUPEQ: return os << " >= 0";
  case AP_CONS_SUP:   return os << " > 0";
  case AP_CONS_EQMOD: return os << " = 0 mod " << s.get_modulo();
  case AP_CONS_DISEQ: return os << " != 0";
  default: throw std::invalid_argument("apron::operator<<(ostream&, const tcons1&) invalid constraint type");
  }
}

inline void tcons1::print(FILE* stream) const
{
  ap_tcons1_fprint(stream, const_cast<ap_tcons1_t*>(&l));
}

 
/* tests */
/* ===== */

inline bool tcons1::is_interval_cst() const
{
  return get_texpr().is_interval_cst();
}

inline bool tcons1::is_interval_linear() const
{
  return get_texpr().is_interval_linear();
}

inline bool tcons1::is_interval_polynomial() const
{
  return get_texpr().is_interval_polynomial();
}

inline bool tcons1::is_interval_polyfrac() const
{
  return get_texpr().is_interval_polyfrac();
}

inline bool tcons1::is_scalar() const
{
  return get_texpr().is_scalar();
}


/* C-level compatibility */
/* ===================== */

inline const ap_tcons1_t* tcons1::get_ap_tcons1_t() const
{
  return &l;
}

inline ap_tcons1_t* tcons1::get_ap_tcons1_t()
{
  return &l;
}





/* ================================= */
/* tcons1_array                    */
/* ================================= */



/* constructors */
/* ============ */

inline tcons1_array::tcons1_array(ap_tcons1_array_t& a) : a(a) 
{}

inline tcons1_array::tcons1_array(const environment& e, const tcons0_array& x)
{
  size_t sz = x.size();
  a = ap_tcons1_array_make(const_cast<ap_environment_t*>(e.get_ap_environment_t()), sz);
  for (size_t i=0; i<sz; i++) 
    a.tcons0_array.p[i] = ap_tcons0_copy(&x.a.p[i]);
}

inline tcons1_array::tcons1_array(const environment& e, size_t size)
{
  a = ap_tcons1_array_make(const_cast<ap_environment_t*>(e.get_ap_environment_t()), size);
}

inline tcons1_array::tcons1_array(const tcons1_array& x)
{
  size_t sz = x.size();
  a = ap_tcons1_array_make(x.get_environment().get_ap_environment_t(), sz);
  for (size_t i=0; i<sz; i++) 
    a.tcons0_array.p[i] = ap_tcons0_copy(&x.a.tcons0_array.p[i]);
}

inline tcons1_array::tcons1_array(const tcons1_array& x, const environment& e)
{
  bool r = 
    ap_tcons1_array_extend_environment(&a,
					 const_cast<ap_tcons1_array_t*>(&x.a),
					 const_cast<ap_environment_t*>(e.get_ap_environment_t()));
  if (r) throw std::invalid_argument("apron::tcons1_array::tcons1_array(const tcons1_array, const environment&) not a super-environment");
}

inline tcons1_array::tcons1_array(size_t sz, const tcons1 x[])
{
  if (sz<1) throw std::invalid_argument("apron::tcons1_array::tcons1_array(size_t, const tcons1) null size");
  a = ap_tcons1_array_make(x[0].get_environment().get_ap_environment_t(), sz);
  for (size_t i=0; i<sz; i++) 
    a.tcons0_array.p[i] = ap_tcons0_copy(const_cast<ap_tcons0_t*>
					     (x[i].get_tcons0().get_ap_tcons0_t()));
}

inline tcons1_array::tcons1_array(const std::vector<tcons1>& x)
{
  size_t sz = x.size();
  if (sz<1) throw std::invalid_argument("apron::tcons1_array::tcons1_array(const vector<tcons1>&) null size");
  a = ap_tcons1_array_make(x[0].get_environment().get_ap_environment_t(), sz);
  for (size_t i=0; i<sz; i++) 
    a.tcons0_array.p[i] = ap_tcons0_copy(const_cast<ap_tcons0_t*>
					     (x[i].get_tcons0().get_ap_tcons0_t()));
}


/* destructor */
/* ========== */

inline tcons1_array::~tcons1_array()
{
  ap_tcons1_array_clear(&a);
}


/* assignment */
/* ========== */

inline tcons1_array& tcons1_array::operator= (const tcons1_array& x)
{
  if (this!=&x) {
    size_t sz = x.size();
    ap_tcons1_array_clear(&a);
    a = ap_tcons1_array_make(x.get_environment().get_ap_environment_t(), sz);
    for (size_t i=0; i<sz; i++) 
      a.tcons0_array.p[i] = ap_tcons0_copy(&x.a.tcons0_array.p[i]);
  }
  return *this;
}

inline tcons1_array& tcons1_array::operator= (const tcons1 x[])
{
  size_t sz = size();
  for (size_t i=0; i<sz; i++) {
    ap_tcons0_clear(&a.tcons0_array.p[i]);
    a.tcons0_array.p[i] = ap_tcons0_copy(const_cast<ap_tcons0_t*>
					     (x[i].get_tcons0().get_ap_tcons0_t()));
  }
  return *this;
}

inline tcons1_array& tcons1_array::operator= (const std::vector<tcons1>& x)
{
  size_t size = x.size();
  if (size<1) {
    ap_tcons1_array_t aa = ap_tcons1_array_make(a.env,0);
    ap_tcons1_array_clear(&a);
    a = aa;
    return *this;
  }
  ap_tcons1_array_clear(&a);
  a = ap_tcons1_array_make(x[0].get_environment().get_ap_environment_t(), size);
  for (size_t i=0; i<size; i++) 
    a.tcons0_array.p[i] = ap_tcons0_copy(const_cast<ap_tcons0_t*>
					     (x[i].get_tcons0().get_ap_tcons0_t()));
  return *this;
}


/* dimension operations */
/* ==================== */

inline void tcons1_array::resize(size_t size)
{
  ap_tcons0_array_resize(&a.tcons0_array, size);
}

inline void tcons1_array::extend_environment(const environment& e)
{
  bool r =
    ap_tcons1_array_extend_environment_with(&a,
					      const_cast<ap_environment_t*>(e.get_ap_environment_t()));
  if (r) throw std::invalid_argument("apron::tcons1_array::extend_environment(const environment&) not a super-environment");
}


/* access */
/* ====== */

inline size_t tcons1_array::size() const
{
  return ap_tcons1_array_size(const_cast<ap_tcons1_array_t*>(&a));
}

inline environment tcons1_array::get_environment() const
{
  return (ap_environment_copy(ap_tcons1_array_envref(const_cast<ap_tcons1_array_t*>(&a))));
}

inline const tcons0_array& tcons1_array::get_tcons0_array() const
{
  return reinterpret_cast<tcons0_array&>(const_cast<ap_tcons0_array_t&>(a.tcons0_array));
}

inline tcons0_array& tcons1_array::get_tcons0_array()
{
  return reinterpret_cast<tcons0_array&>(a.tcons0_array);
}

inline tcons1 tcons1_array::get(size_t i) const
{
  if (i>=size()) throw std::out_of_range("apron::tcons1_array::get(size_t)");
  ap_tcons1_t c = ap_tcons1_array_get(const_cast<ap_tcons1_array_t*>(&a),i);
  ap_tcons1_t cc = ap_tcons1_copy(&c);
  return tcons1(cc);
}

inline void tcons1_array::set(size_t i, const tcons1& x)
{
  if (i>=size()) throw std::out_of_range("apron::tcons1_array::set(size_t, const tcons1&)");
  ap_tcons0_clear(&a.tcons0_array.p[i]);
  a.tcons0_array.p[i] = ap_tcons0_copy(const_cast<ap_tcons0_t*>
				       (x.get_tcons0().get_ap_tcons0_t()));
}


/* conversion */
/* ========== */
  
inline tcons1_array::operator std::vector<tcons1>() const
{
  size_t sz = size();
  tcons1 dummy(get_environment(),unsat());
  std::vector<tcons1> v(sz,dummy);
  for (size_t i=0;i<sz;i++) {
    ap_tcons1_t c = ap_tcons1_array_get(const_cast<ap_tcons1_array_t*>(&a),i);
    v[i] = ap_tcons1_copy(&c);
  }
  return v;
}


/* print */
/* ===== */

inline std::ostream& operator<< (std::ostream& os, const tcons1_array& s)
{
  size_t sz = s.size();
  os << "{ ";
  for (size_t i=0;i<sz;i++)
    os << s.get(i) << "; ";
  return os << "}";
}

inline void tcons1_array:: print(FILE* stream) const
{
  ap_tcons1_array_fprint(stream, const_cast<ap_tcons1_array_t*>(&a));
}


/* C-level compatibility */
/* ===================== */

inline const ap_tcons1_array_t* tcons1_array::get_ap_tcons1_array_t() const
{
  return &a;
}

inline ap_tcons1_array_t* tcons1_array::get_ap_tcons1_array_t()
{
  return &a;
}

