/* -*- C++ -*-
 * apxx_scalar_inline.hh
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
/* use_malloc                        */
/* ================================= */

inline void* use_malloc::operator new(size_t sz)
{ 
  void* x = malloc(sz); 
  if (!x) throw std::bad_alloc();
  return x;
}

inline void* use_malloc::operator new[](size_t sz)
{ 
  void* x = malloc(sz); 
  if (!x) throw std::bad_alloc();
  return x;
}

inline void use_malloc::operator delete(void* p)
{ 
  free(p); 
}

inline void use_malloc::operator delete[](void* p)
{ 
  free(p); 
}



/* ================================= */
/* scalar                            */
/* ================================= */


/* constructors */
/* ============ */

inline scalar::scalar() 
{ 
  ap_scalar_init(&c, AP_SCALAR_DOUBLE); 
}

inline scalar::scalar(int x)
{ 
  ap_scalar_init(&c, AP_SCALAR_MPQ); 
  ap_scalar_set_int(&c, x); 
}

inline scalar::scalar(long x)
{ 
  ap_scalar_init(&c, AP_SCALAR_MPQ); 
  ap_scalar_set_int(&c, x); 
}

inline scalar::scalar(double x)
{ 
  ap_scalar_init(&c, AP_SCALAR_DOUBLE); 
  ap_scalar_set_double(&c, x); 
}

inline scalar::scalar(const frac& x)
{ 
  ap_scalar_init(&c, AP_SCALAR_MPQ); 
  ap_scalar_set_frac(&c, x.num, x.den); 
}

inline scalar::scalar(infty x)
{ 
  ap_scalar_init(&c, AP_SCALAR_DOUBLE); 
  ap_scalar_set_infty(&c, x.sgn); 
}

inline scalar::scalar(const mpq_class& x) 
{ 
  ap_scalar_init(&c, AP_SCALAR_MPQ); 
  ap_scalar_set_mpq(&c, const_cast<mpq_class&>(x).get_mpq_t());
}
  
inline scalar::scalar(mpfr_t x) 
{ 
  ap_scalar_init(&c, AP_SCALAR_MPFR); 
  ap_scalar_set_mpfr(&c,x);
}
  
inline scalar::scalar(const scalar& x) 
{ 
  ap_scalar_init(&c, x.c.discr); 
  ap_scalar_set(&c, const_cast<ap_scalar_t*>(&x.c)); 
}


/* destructor */
/* ========== */

inline scalar::~scalar() 
{ 
  ap_scalar_clear(&c); 
}
  

/* assignments */
/* =========== */

/* = */

inline scalar& scalar::operator= (int x)
{ 
  ap_scalar_set_int(&c, x); 
  return *this; 
}

inline scalar& scalar::operator= (long x)
{ 
  ap_scalar_set_int(&c, x); 
  return *this; 
}
  
inline scalar& scalar::operator= (double x)    
{ 
  ap_scalar_set_double(&c, x); 
  return *this; 
}

inline scalar& scalar::operator= (const frac& x)
{ 
  ap_scalar_set_frac(&c, x.num, x.den); 
  return *this; 
}

inline scalar& scalar::operator= (infty x)
{ 
  ap_scalar_set_infty(&c, x.sgn); 
  return *this; 
}

inline scalar& scalar::operator= (const mpq_class& x)
{ 
  ap_scalar_set_mpq(&c, const_cast<mpq_class&>(x).get_mpq_t()); 
  return *this; 
}

inline scalar& scalar::operator= (mpfr_t x)
{ 
  ap_scalar_set_mpfr(&c, x);
  return *this; 
}

inline scalar& scalar::operator= (const scalar& x)
{ 
  ap_scalar_set(&c, const_cast<ap_scalar_t*>(&x.c)); 
  return *this; 
}


/* swap */

inline void swap(scalar& a, scalar &b)
{ 
  ap_scalar_swap(&a.c, &b.c); 
}


/* access */
/* ====== */

inline ap_scalar_discr_t scalar::get_discr() const
{ 
  return c.discr; 
}

inline double& scalar::get_double()
{
  if (c.discr!=AP_SCALAR_DOUBLE) throw(bad_discriminant("apron::scalar::get_double()"));
  return c.val.dbl;
}

inline const double& scalar::get_double() const
{
  if (c.discr!=AP_SCALAR_DOUBLE) throw(bad_discriminant("apron::scalar::get_double()"));
  return c.val.dbl;
}

inline mpq_class& scalar::get_mpq()
{
  if (c.discr!=AP_SCALAR_MPQ) throw(bad_discriminant("apron::scalar::get_mpq()"));
  return reinterpret_cast<mpq_class&>(*c.val.mpq);
}

inline const mpq_class& scalar::get_mpq() const
{
  if (c.discr!=AP_SCALAR_MPQ) throw(bad_discriminant("apron::scalar::get_mpq()"));
  return reinterpret_cast<mpq_class&>(*c.val.mpq);
}


/* conversion */
/* ========== */

inline mpq_class scalar::to_mpq(mp_rnd_t round, order& conv) const
{
  mpq_class r;
  int o = ap_mpq_set_scalar(r.get_mpq_t(), const_cast<ap_scalar_t*>(&c), round);
  conv = (o>0) ? GREATER : (o<0) ? LESS : EQUAL;
  return r;
}

inline double scalar::to_double(mp_rnd_t round, order& conv) const
{
  double r;
  int o = ap_double_set_scalar(&r, const_cast<ap_scalar_t*>(&c), round);
  conv = (o>0) ? GREATER : (o<0) ? LESS : EQUAL;
  return r;
}

inline void scalar::to_mpfr(mpfr_t r, mp_rnd_t round, order& conv) const
{
  int o = ap_mpfr_set_scalar(r, const_cast<ap_scalar_t*>(&c), round);
  conv = (o>0) ? GREATER : (o<0) ? LESS : EQUAL;
}

inline scalar::operator mpq_class() const
{
  order c;
  return to_mpq(GMP_RNDN, c);
}

inline scalar::operator double() const
{
  order c;
  return to_double(GMP_RNDN, c);
}


/* print */
/* ===== */

inline std::ostream& operator<< (std::ostream& os, const scalar& s)
{
  int i = ap_scalar_infty(const_cast<ap_scalar_t*>(&s.c));
  if (i>0) return os << "+oo";
  if (i<0) return os << "-oo";
  switch (s.c.discr) {
  case AP_SCALAR_DOUBLE: return os << s.c.val.dbl;
  case AP_SCALAR_MPQ:    return os << s.c.val.mpq;
  case AP_SCALAR_MPFR:   
    {
      double d = mpfr_get_d(s.c.val.mpfr,GMP_RNDU);
      if (!mpfr_cmp_d(s.c.val.mpfr,d)) return os << d;
      mp_exp_t e;
      char* tmp = mpfr_get_str(NULL,&e,10,os.precision(),s.c.val.mpfr,GMP_RNDU);
      if (tmp[0]=='-' || tmp[0]=='+') {
	os << tmp[0] << "." << tmp+1;
	if (e>0) os << "e+" << e;
	if (e<0) os << "e" << e;
      }
      else {
	os << "." << tmp;
	if (e>0) os << "e+" << e;
	if (e<0) os << "e" << e;
      }
      mpfr_free_str(tmp);
      return os;
    }
  default: return os;
  }
}

inline void scalar::print(FILE* stream) const 
{ 
  ap_scalar_fprint(stream, const_cast<ap_scalar_t*>(&c)); 
}


/* tests */
/* ===== */

inline scalar::order scalar::is_infty() const
{ 
  return (order) ap_scalar_infty(const_cast<ap_scalar_t*>(&c));
}

inline scalar::order scalar::sgn () const 
{ 
  return (order) ap_scalar_sgn(const_cast<ap_scalar_t*>(&c));
}


inline scalar::order cmp(const scalar& a, const scalar& b)         
{ 
  return (scalar::order) ap_scalar_cmp(const_cast<ap_scalar_t*>(&a.c), const_cast<ap_scalar_t*>(&b.c));
}

inline bool operator>=(const scalar& a, const scalar& b) 
{ 
  return ap_scalar_cmp(const_cast<ap_scalar_t*>(&a.c), const_cast<ap_scalar_t*>(&b.c))>=0;
}

inline bool operator<=(const scalar& a, const scalar& b) 
{ 
  return ap_scalar_cmp(const_cast<ap_scalar_t*>(&a.c), const_cast<ap_scalar_t*>(&b.c))<=0;
}

inline bool operator>(const scalar& a, const scalar& b) 
{ 
  return ap_scalar_cmp(const_cast<ap_scalar_t*>(&a.c), const_cast<ap_scalar_t*>(&b.c))>0;
}

inline bool operator<(const scalar& a, const scalar& b) 
{ 
  return ap_scalar_cmp(const_cast<ap_scalar_t*>(&a.c), const_cast<ap_scalar_t*>(&b.c))<0;
}

inline bool operator==(const scalar& a, const scalar& b) 
{ 
  return ap_scalar_equal(const_cast<ap_scalar_t*>(&a.c), const_cast<ap_scalar_t*>(&b.c));
}

inline bool operator!=(const scalar& a, const scalar& b) 
{ 
  return  !ap_scalar_equal(const_cast<ap_scalar_t*>(&a.c), const_cast<ap_scalar_t*>(&b.c)); 
}


inline scalar::order cmp(const scalar& a, int b)         
{ 
  return (scalar::order) ap_scalar_cmp_int(const_cast<ap_scalar_t*>(&a.c), b); 
}

inline bool operator>=(const scalar& a, int b) 
{ 
  return ap_scalar_cmp_int(const_cast<ap_scalar_t*>(&a.c), b)>=0;
}

inline bool operator<=(const scalar& a, int b) 
{ 
  return ap_scalar_cmp_int(const_cast<ap_scalar_t*>(&a.c), b)<=0;
}

inline bool operator>(const scalar& a, int b) 
{ 
  return ap_scalar_cmp_int(const_cast<ap_scalar_t*>(&a.c), b)>0;
}

inline bool operator<(const scalar& a, int b)
{ 
  return ap_scalar_cmp_int(const_cast<ap_scalar_t*>(&a.c), b)<0;
}

inline bool operator==(const scalar& a, int b) 
{ 
  return ap_scalar_equal_int(const_cast<ap_scalar_t*>(&a.c), b); 
}

inline bool operator!=(const scalar& a, int b) 
{ 
  return  !ap_scalar_equal_int(const_cast<ap_scalar_t*>(&a.c), b); 
}

inline scalar::order cmp(int a, const scalar& b)         
{ 
  return (scalar::order) ap_scalar_cmp_int(const_cast<ap_scalar_t*>(&b.c), a); 
}

inline bool operator>=(int a, const scalar& b) 
{ 
  return ap_scalar_cmp_int(const_cast<ap_scalar_t*>(&b.c), a)<=0;
}

inline bool operator<=(int a, const scalar& b) 
{ 
  return ap_scalar_cmp_int(const_cast<ap_scalar_t*>(&b.c), a)>=0;
}

inline bool operator>(int a, const scalar& b) 
{ 
  return ap_scalar_cmp_int(const_cast<ap_scalar_t*>(&b.c), a)<0;
}

inline bool operator<(int a, const scalar& b) 
{ 
  return ap_scalar_cmp_int(const_cast<ap_scalar_t*>(&b.c), a)>0;
}

inline bool operator==(int a, const scalar& b) 
{ 
  return ap_scalar_equal_int(const_cast<ap_scalar_t*>(&b.c), a); 
}

inline bool operator!=(int a, const scalar& b) 
{ 
  return  !ap_scalar_equal_int(const_cast<ap_scalar_t*>(&b.c), a); 
}


/* other operators */
/* =============== */

inline void scalar::neg()
{ 
  ap_scalar_neg(&c, &c); 
}  

inline scalar scalar::operator- () const
{ 
  scalar r = *this; 
  r.neg(); 
  return r; 
}

inline void scalar::inv() 
{ 
  ap_scalar_inv(&c, &c); 
}

inline scalar scalar::operator~ () const
{
  scalar r = *this; 
  r.inv(); 
  return r; 
}

inline long scalar::hash() const
{ 
  return ap_scalar_hash(const_cast<ap_scalar_t*>(&c)); 
}


/* C API compatibility */
/* =================== */

inline const ap_scalar_t* scalar::get_ap_scalar_t() const
{ 
  return &c; 
}

inline ap_scalar_t* scalar::get_ap_scalar_t()
{ 
  return &c; 
}
