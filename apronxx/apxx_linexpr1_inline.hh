/* -*- C++ -*-
 * apxx_linexpr1_inline.hh
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
/* linexpr1                          */
/* ================================= */



/* constructors */
/* ============ */

inline linexpr1::linexpr1(ap_linexpr1_t& p) : l(p)
{
}

inline linexpr1::linexpr1(const environment& e, const linexpr0& x)
{
  l = ap_linexpr1_of_linexpr0(const_cast<ap_environment_t*>(e.get_ap_environment_t()),
			      ap_linexpr0_copy(const_cast<ap_linexpr0_t*>(x.get_ap_linexpr0_t())));
}

inline linexpr1::linexpr1(const environment& e, ap_linexpr_discr_t lin_discr, size_t size)
{
  l = ap_linexpr1_make(const_cast<ap_environment_t*>(e.get_ap_environment_t()),
		       lin_discr, size);
}

inline linexpr1::linexpr1(const linexpr1& x)
{
  l = ap_linexpr1_copy(const_cast<ap_linexpr1_t*>(&x.l));
}

inline linexpr1::linexpr1(const linexpr1& x, const environment& e)
{
  bool r =
    ap_linexpr1_extend_environment(&l,
				   const_cast<ap_linexpr1_t*>(&x.l),
				   const_cast<ap_environment_t*>(e.get_ap_environment_t()));
  if (r) throw std::invalid_argument("apron::linexpr1::linexpr1(const linexpr1&, environment&) not a super-environment");
}


/* destructor */
/* ========== */


inline linexpr1::~linexpr1()
{
  ap_linexpr1_clear(&l);
}


/* assignment */
/* ========== */

inline linexpr1& linexpr1::operator=(const linexpr1& x)
{
  ap_linexpr1_t ll = ap_linexpr1_copy(const_cast<ap_linexpr1_t*>(&x.l));
  ap_linexpr1_clear(&l);
  l = ll;
  return *this;
}


/* dimension operations */
/* ==================== */

inline void linexpr1::extend_environment(const environment& e)
{
  bool r =
    ap_linexpr1_extend_environment_with(&l,
					const_cast<ap_environment_t*>(e.get_ap_environment_t()));
  if (r) throw std::invalid_argument("apron::linexpr1::extend_environment(const environment&) not a super-environment");
}


/* access */
/* ====== */


inline environment linexpr1::get_environment() const
{
  return ap_environment_copy(ap_linexpr1_envref(const_cast<ap_linexpr1_t*>(&l)));
}

inline const linexpr0& linexpr1::get_linexpr0() const
{
  return reinterpret_cast<linexpr0&>(*ap_linexpr1_linexpr0ref(const_cast<ap_linexpr1_t*>(&l)));
}

inline linexpr0& linexpr1::get_linexpr0()
{
  return reinterpret_cast<linexpr0&>(*ap_linexpr1_linexpr0ref(&l));
}

inline size_t linexpr1::size() const
{ 
  return ap_linexpr0_size(ap_linexpr1_linexpr0ref(const_cast<ap_linexpr1_t*>(&l)));
}

inline ap_linexpr_discr_t linexpr1::get_discr() const
{ 
  return ap_linexpr1_linexpr0ref(const_cast<ap_linexpr1_t*>(&l))->discr;
}

inline coeff& linexpr1::get_cst()
{ 
  return reinterpret_cast<coeff&>(*ap_linexpr1_cstref(&l));
}
 
inline const coeff& linexpr1::get_cst() const
{ 
  return reinterpret_cast<coeff&>(*ap_linexpr1_cstref(const_cast<ap_linexpr1_t*>(&l))); 
}

inline coeff& linexpr1::operator[](const var& var)
{ 
  ap_coeff_t* x = ap_linexpr1_coeffref(&l, 
				       const_cast<ap_var_t>(var.get_ap_var_t()));
  if (!x) throw std::invalid_argument("apron::linexpr1::operator[](const var&) variable not in environment");
  return reinterpret_cast<coeff&>(*x); 
}
  
inline const coeff& linexpr1::operator[](const var& var) const
{ 
  ap_coeff_t* x = ap_linexpr1_coeffref(const_cast<ap_linexpr1_t*>(&l), 
				       const_cast<ap_var_t>(var.get_ap_var_t()));
  if (!x) throw std::invalid_argument("apron::linexpr1::operator[](const var&) variable not in environment");
  return reinterpret_cast<coeff&>(*x); 
}
  

/* print */
/* ===== */

inline std::ostream& operator<<(std::ostream& os, const linexpr1& s)
{
  bool first = true;
  for (linexpr1::const_iterator i=s.begin();i.valid();++i) {
    if (print_coeff_sign(os, i.get_coeff(), first, false))
      os << i.get_var();
  }
  print_coeff_sign(os, s.get_cst(), first, true);
  if (first) os << "0";
  return os;
}

inline void linexpr1::print(FILE* stream) const
{ 
  ap_linexpr1_fprint(stream, const_cast<ap_linexpr1_t*>(&l));
}


/* tests */
/* ===== */

inline bool linexpr1::is_integer() const
{ 
  return ap_linexpr1_is_integer(const_cast<ap_linexpr1_t*>(&l));
}

inline bool linexpr1::is_real() const
{ 
  return ap_linexpr1_is_real(const_cast<ap_linexpr1_t*>(&l)); 
}

inline ap_linexpr_type_t linexpr1::get_type() const
{
  return ap_linexpr1_type(const_cast<ap_linexpr1_t*>(&l));
}

inline bool linexpr1::is_linear() const
{ 
  return ap_linexpr1_is_linear(const_cast<ap_linexpr1_t*>(&l)); 
}

inline bool linexpr1::is_quasilinear() const
{ 
  return ap_linexpr1_is_quasilinear(const_cast<ap_linexpr1_t*>(&l));
}


/* iterators */
/* ========= */

inline void linexpr1::const_iterator::skip_AP_DIM_MAX()
{ 
  if (l->linexpr0->discr == AP_LINEXPR_DENSE) return;
  while (pos < l->linexpr0->size && l->linexpr0->p.linterm[pos].dim == AP_DIM_MAX) pos++;
}

inline linexpr1::const_iterator::const_iterator(ap_linexpr1_t* e)
  : l(e), pos(0)
{
  skip_AP_DIM_MAX(); 
}

inline linexpr1::const_iterator::const_iterator(const linexpr1& e)
{
  l = const_cast<ap_linexpr1_t*>(e.get_ap_linexpr1_t());
  pos = 0;
  skip_AP_DIM_MAX(); 
}

inline linexpr1::const_iterator::const_iterator(const const_iterator& i)
  : l(i.l), pos(i.pos)
{}


inline linexpr1::iterator::iterator(ap_linexpr1_t* e)
  : linexpr1::const_iterator(e)
{}

inline linexpr1::iterator::iterator(linexpr1& e)
  : linexpr1::const_iterator(e.get_ap_linexpr1_t())
{}

inline linexpr1::iterator::iterator(const iterator& i) 
  : linexpr1::const_iterator(i.l)
{}

inline linexpr1::const_iterator& linexpr1::const_iterator::operator=(const const_iterator& i)
{
  l = i.l;
  pos = i.pos;
  return *this;
}

inline linexpr1::iterator& linexpr1::iterator::operator=(const iterator& i)
{
  l = i.l;
  pos = i.pos;
  return *this;
}

inline ap_dim_t linexpr1::const_iterator::get_dim() const
{
  if (pos >= l->linexpr0->size) throw std::out_of_range("apron::linexpr1::const_iterator::get_dim()");
  if (l->linexpr0->discr == AP_LINEXPR_DENSE) return pos;
  else return l->linexpr0->p.linterm[pos].dim;
}

inline const var& linexpr1::const_iterator::get_var() const
{
  ap_dim_t d;
  if (pos >= l->linexpr0->size) throw std::out_of_range("apron::linexpr1::const_iterator::get_var()");
  if (l->linexpr0->discr == AP_LINEXPR_DENSE) d = pos;
  else d = l->linexpr0->p.linterm[pos].dim;
  return *reinterpret_cast<var*>(&l->env->var_of_dim[d]);
}

inline const coeff& linexpr1::const_iterator::get_coeff() const
{
  if (pos >= l->linexpr0->size) throw std::out_of_range("apron::linexpr1::const_iterator::get_coeff()");
  if (l->linexpr0->discr == AP_LINEXPR_DENSE) return reinterpret_cast<coeff&>(l->linexpr0->p.coeff[pos]);
  else return reinterpret_cast<coeff&>(l->linexpr0->p.linterm[pos].coeff);
}

inline coeff& linexpr1::iterator::get_coeff() const
{
  if (pos >= l->linexpr0->size) throw std::out_of_range("apron::linexpr1::iterator::get_coeff()");
  if (l->linexpr0->discr == AP_LINEXPR_DENSE) return reinterpret_cast<coeff&>(l->linexpr0->p.coeff[pos]);
  else return reinterpret_cast<coeff&>(l->linexpr0->p.linterm[pos].coeff);
}

inline void linexpr1::const_iterator::next()
{
  pos++; 
  skip_AP_DIM_MAX(); 
}

inline void linexpr1::const_iterator::operator++()
{
  next();
}

inline bool linexpr1::const_iterator::valid() const
{ 
  return pos < l->linexpr0->size;
}

inline linexpr1::const_iterator linexpr1::begin() const
{
  return const_iterator(*this); 
}
  
inline linexpr1::iterator linexpr1::begin()
{
  return iterator(*this); 
}
  

/* other operators */
/* =============== */

inline void linexpr1::minimize()
{
  ap_linexpr1_minimize(&l);
}


/* C-level compatibility */
/* ===================== */

inline const ap_linexpr1_t* linexpr1::get_ap_linexpr1_t() const
{
  return &l; 
}

inline ap_linexpr1_t* linexpr1::get_ap_linexpr1_t()
{
  return &l; 
}
