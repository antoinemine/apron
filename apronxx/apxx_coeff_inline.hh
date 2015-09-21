/* -*- C++ -*-
 * apxx_coeff_inline.hh
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
/* coeff                             */
/* ================================= */


/* constructors */
/* ============ */

inline coeff::coeff()
{ 
  ap_coeff_init(&c, AP_COEFF_SCALAR); 
}

inline coeff::coeff(const coeff& x)
{ 
  ap_coeff_init(&c, x.c.discr);
  ap_coeff_set(&c, const_cast<ap_coeff_t*>(x.get_ap_coeff_t())); 
}


/* scalars */

inline coeff::coeff(const scalar& x)
{ 
  ap_coeff_init(&c, AP_COEFF_SCALAR); 
  ap_coeff_set_scalar(&c, const_cast<ap_scalar_t*>(x.get_ap_scalar_t())); 
}

inline coeff::coeff(int x)   
{ 
  ap_coeff_init(&c, AP_COEFF_SCALAR); 
  ap_coeff_set_scalar_int(&c, x); 
}

inline coeff::coeff(long x)
{ 
  ap_coeff_init(&c, AP_COEFF_SCALAR); 
  ap_coeff_set_scalar_int(&c, x); 
}

inline coeff::coeff(double x) 
{ 
  ap_coeff_init(&c, AP_COEFF_SCALAR); 
  ap_coeff_set_scalar_double(&c, x); 
}

inline coeff::coeff(const frac& x)
{ 
  ap_coeff_init(&c, AP_COEFF_SCALAR); 
  ap_coeff_set_scalar_frac(&c, x.num, x.den); 
}

inline coeff::coeff(const mpq_class& x) 
{ 
  ap_coeff_init(&c, AP_COEFF_SCALAR); 
  ap_coeff_set_scalar_mpq(&c, const_cast<mpq_class&>(x).get_mpq_t());
}
  
inline coeff::coeff(mpfr_t x) 
{ 
  ap_coeff_init(&c, AP_COEFF_SCALAR); 
  ap_coeff_set_scalar_mpfr(&c,x);
}
  

/* intervals */

inline coeff::coeff(const interval& x)
{ 
  ap_coeff_init(&c, AP_COEFF_INTERVAL); 
  ap_coeff_set_interval(&c, 
			const_cast<ap_interval_t*>(x.get_ap_interval_t())); 
}

inline coeff::coeff(const scalar& inf, const scalar& sup)
{ 
  ap_coeff_init(&c, AP_COEFF_INTERVAL);
  ap_coeff_set_interval_scalar(&c, 
			       const_cast<ap_scalar_t*>(inf.get_ap_scalar_t()),
			       const_cast<ap_scalar_t*>(sup.get_ap_scalar_t()));
}

inline coeff::coeff(int inf, int sup)
{ 
  ap_coeff_init(&c, AP_COEFF_INTERVAL); 
  ap_coeff_set_interval_int(&c, inf, sup); 
}

inline coeff::coeff(long inf, long sup)
{ 
  ap_coeff_init(&c, AP_COEFF_INTERVAL); 
  ap_coeff_set_interval_int(&c, inf, sup);
}

inline coeff::coeff(double inf, double sup)
{ 
  ap_coeff_init(&c, AP_COEFF_INTERVAL); 
  ap_coeff_set_interval_double(&c, inf, sup);  
}

inline coeff::coeff(const frac& inf, const frac& sup)
{ 
  ap_coeff_init(&c, AP_COEFF_INTERVAL); 
  ap_coeff_set_interval_frac(&c, inf.num, inf.den, sup.num, sup.den); 
}
  
inline coeff::coeff(const mpq_class& inf, const mpq_class& sup) 
{
  ap_coeff_init(&c, AP_COEFF_INTERVAL);
  ap_coeff_set_interval_mpq(&c,
			    const_cast<mpq_class&>(inf).get_mpq_t(),
			    const_cast<mpq_class&>(sup).get_mpq_t());
}

inline coeff::coeff(mpfr_t inf, mpfr_t sup)
{
  ap_coeff_init(&c, AP_COEFF_INTERVAL);
  ap_coeff_set_interval_mpfr(&c,inf,sup);
}

inline coeff::coeff(top t)
{
  ap_coeff_init(&c, AP_COEFF_INTERVAL);
  ap_coeff_set_interval_top(&c);
}

inline coeff::coeff(bottom t)
{
  ap_coeff_init(&c, AP_COEFF_INTERVAL);
  ap_coeff_set_interval_int(&c, 1, -1);
}


/* destructor */
/* ========== */

inline coeff::~coeff()
{ 
  ap_coeff_clear(&c); 
}


/* assignments */
/* =========== */

inline coeff& coeff::operator= (const coeff& x)
{ 
  ap_coeff_set(&c, const_cast<ap_coeff_t*>(x.get_ap_coeff_t())); 
  return *this; 
}
  
inline coeff& coeff::operator= (const scalar& x)
{ 
  ap_coeff_set_scalar(&c, const_cast<ap_scalar_t*>(x.get_ap_scalar_t())); 
  return *this;
}

inline coeff& coeff::operator= (int x)   
{ 
  ap_coeff_set_scalar_int(&c, x); 
  return *this;
}

inline coeff& coeff::operator= (long x)
{ 
  ap_coeff_set_scalar_int(&c, x); 
  return *this; 
}

inline coeff& coeff::operator= (double x) 
{ 
  ap_coeff_set_scalar_double(&c, x); 
  return *this; 
}

inline coeff& coeff::operator= (const frac& x)
{ 
  ap_coeff_set_scalar_frac(&c, x.num, x.den); 
  return *this; 
}

inline coeff& coeff::operator= (const mpq_class& x) 
{ 
  ap_coeff_set_scalar_mpq(&c, const_cast<mpq_class&>(x).get_mpq_t()); 
  return *this; 
}
  
inline coeff& coeff::operator= (mpfr_t x) 
{ 
  ap_coeff_set_scalar_mpfr(&c, x);
  return *this; 
}
  
inline coeff& coeff::operator= (const interval& x)
{ 
  ap_coeff_set_interval(&c, const_cast<ap_interval_t*>(x.get_ap_interval_t())); 
  return *this; 
}

inline coeff& coeff::operator= (top t)
{ 
  ap_coeff_set_interval_top(&c);
  return *this; 
}

inline coeff& coeff::operator= (bottom t)
{ 
  ap_coeff_set_interval_int(&c, 1, -1);
  return *this; 
}



/* access */
/* ====== */

inline ap_coeff_discr_t coeff::get_discr() const
{ 
  return c.discr; 
}

inline scalar& coeff::get_scalar()
{
  if (c.discr!=AP_COEFF_SCALAR) throw(bad_discriminant("coeff::get_scalar"));
  return reinterpret_cast<scalar&>(*c.val.scalar);
}

inline const scalar& coeff::get_scalar() const
{
  if (c.discr!=AP_COEFF_SCALAR) throw(bad_discriminant("coeff::get_scalar"));
  return reinterpret_cast<const scalar&>(*c.val.scalar);
}

inline interval& coeff::get_interval()
{
  if (c.discr!=AP_COEFF_INTERVAL) throw(bad_discriminant("coeff::get_interval"));
  return reinterpret_cast<interval&>(*c.val.interval);
}

inline const interval& coeff::get_interval() const
{
  if (c.discr!=AP_COEFF_INTERVAL) throw(bad_discriminant("coeff::get_interval"));
  return reinterpret_cast<const interval&>(*c.val.interval);
}


inline coeff&coeff::set(const coeff& x)
{ 
  ap_coeff_set(&c, const_cast<ap_coeff_t*>(x.get_ap_coeff_t())); 
  return *this;
}

inline coeff&coeff::set(const scalar& x)
{
  ap_coeff_set_scalar(&c, const_cast<ap_scalar_t*>(x.get_ap_scalar_t()));  
  return *this;
}

inline coeff&coeff::set(int x)   
{
  ap_coeff_set_scalar_int(&c, x); 
  return *this;
}

inline coeff&coeff::set(long x)
{
  ap_coeff_set_scalar_int(&c, x); 
  return *this;
}

inline coeff&coeff::set(double x) 
{
  ap_coeff_set_scalar_double(&c, x); 
  return *this;
}

inline coeff&coeff::set(const frac& x)
{
  ap_coeff_set_scalar_frac(&c, x.num, x.den); 
  return *this;
}

inline coeff&coeff::set(const mpq_class& x) 
{ 
  ap_coeff_set_scalar_mpq(&c, const_cast<mpq_class&>(x).get_mpq_t()); 
  return *this;
}
  
inline coeff&coeff::set(mpfr_t x) 
{ 
  ap_coeff_set_scalar_mpfr(&c, x);
  return *this;
}
  

inline coeff&coeff::set(const interval& x)
{ 
  ap_coeff_set_interval(&c, const_cast<ap_interval_t*>(x.get_ap_interval_t())); 
  return *this;
}

inline coeff&coeff::set(const scalar& inf, const scalar& sup)
{ 
  ap_coeff_set_interval_scalar(&c, 
			       const_cast<ap_scalar_t*>(inf.get_ap_scalar_t()),
			       const_cast<ap_scalar_t*>(sup.get_ap_scalar_t()));
  return *this;
}

inline coeff&coeff::set(int inf, int sup)
{
  ap_coeff_set_interval_int(&c, inf, sup); 
  return *this;
}

inline coeff&coeff::set(long inf, long sup)
{
  ap_coeff_set_interval_int(&c, inf, sup); 
  return *this;
}

inline coeff&coeff::set(double inf, double sup)
{
  ap_coeff_set_interval_double(&c, inf, sup);  
  return *this;
}

inline coeff&coeff::set(const frac& inf, const frac& sup)
{
  ap_coeff_set_interval_frac(&c, inf.num, inf.den, sup.num, sup.den); 
  return *this;
}
  
inline coeff&coeff::set(const mpq_class& inf, const mpq_class& sup) 
{
  ap_coeff_set_interval_mpq(&c,
			    const_cast<mpq_class&>(inf).get_mpq_t(),
			    const_cast<mpq_class&>(sup).get_mpq_t());
  return *this;
}

inline coeff&coeff::set(mpfr_t inf, mpfr_t sup)
{
  ap_coeff_set_interval_mpfr(&c, inf, sup);
  return *this;
}

inline coeff&coeff::set(top t)
{ 
  ap_coeff_set_interval_top(&c);
  return *this;
}

inline coeff&coeff::set(bottom t)
{ 
  ap_coeff_set_interval_int(&c, 1, -1);
  return *this;
}


/* swap */

inline void swap(coeff& a, coeff &b)
{
  ap_coeff_swap(&a.c, &b.c); 
}



/* print */
/* ===== */

inline std::ostream& operator<< (std::ostream& os, const coeff& s)
{
  switch (s.c.discr) {
  case AP_COEFF_SCALAR:   return os << *(reinterpret_cast<scalar*>(s.c.val.scalar));
  case AP_COEFF_INTERVAL: return os << *(reinterpret_cast<interval*>(s.c.val.interval));
  default: return os;
  }
}

inline void coeff::print(FILE* stream) const 
{ 
  ap_coeff_fprint(stream, const_cast<ap_coeff_t*>(&c)); 
}
 

/* tests */
/* ===== */

inline bool coeff::is_zero() const
{ 
  return ap_coeff_zero(const_cast<ap_coeff_t*>(&c)); 
}

inline int cmp(const coeff& a, const coeff& b)
{ 
  return ap_coeff_cmp(const_cast<ap_coeff_t*>(&a.c), const_cast<ap_coeff_t*>(&b.c)); 
}

/*
inline bool operator<= (const coeff& a, const coeff& b)
{ 
  int x = ap_coeff_cmp(const_cast<ap_coeff_t*>(&a.c), const_cast<ap_coeff_t*>(&b.c));
  printf("%i\n",x);
  return x>=-1 && x<=0;
}

inline bool operator>= (const coeff& a, const coeff& b)
{ 
  int x = ap_coeff_cmp(const_cast<ap_coeff_t*>(&a.c), const_cast<ap_coeff_t*>(&b.c));
  return x>=0 && x<=1;
}

inline bool operator< (const coeff& a, const coeff& b)
{
  return ap_coeff_cmp(const_cast<ap_coeff_t*>(&a.c), const_cast<ap_coeff_t*>(&b.c)) == -1; 
}

inline bool operator> (const coeff& a, const coeff& b)
{ 
  return ap_coeff_cmp(const_cast<ap_coeff_t*>(&a.c), const_cast<ap_coeff_t*>(&b.c)) == 1; 
}
*/

inline bool operator== (const coeff& a, const coeff& b)
{
  return ap_coeff_equal(const_cast<ap_coeff_t*>(&a.c), const_cast<ap_coeff_t*>(&b.c)); 
}

inline bool operator!= (const coeff& a, const coeff& b)
{
  return !ap_coeff_equal(const_cast<ap_coeff_t*>(&a.c), const_cast<ap_coeff_t*>(&b.c)); 
}




/* other operators */
/* =============== */

inline void coeff::reduce()
{
  ap_coeff_reduce(&c); 
}

inline void coeff::neg()
{
  ap_coeff_neg(&c, &c); 
}

inline coeff coeff::operator- () const
{
  coeff r = *this; r.neg(); return r; 
}

inline long coeff::hash() const
{
  return ap_coeff_hash(const_cast<ap_coeff_t*>(&c)); 
}


/* C-level compatibility */

inline const ap_coeff_t* coeff::get_ap_coeff_t() const
{
  return &c; 
}

inline ap_coeff_t* coeff::get_ap_coeff_t()
{
  return &c; 
}
