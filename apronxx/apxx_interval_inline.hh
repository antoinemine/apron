/* -*- C++ -*-
 * apxx_interval_inline.hh
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
/* interval                          */
/* ================================= */


/* constructors */
/* ============ */


inline void interval::init()
{ 
  c.inf = ap_scalar_alloc(); 
  c.sup = ap_scalar_alloc(); 
}

inline interval::interval(ap_interval_t* i) 
  : c(*i) 
{
  free(i); 
}

inline interval::interval()
{ 
  init(); 
}
  
inline interval::interval(const interval& x)
{ 
  init(); 
  ap_interval_set(&c, const_cast<ap_interval_t*>(&x.c)); 
}

inline interval::interval(const scalar& inf, const scalar& sup)
{ 
  init();
  ap_interval_set_scalar(&c, 
			 const_cast<ap_scalar_t*>(inf.get_ap_scalar_t()), 
			 const_cast<ap_scalar_t*>(sup.get_ap_scalar_t()));
}

inline interval::interval(int inf, int sup)
{ 
  init(); 
  ap_interval_set_int(&c, inf, sup); 
}

inline interval::interval(long inf, long sup)
{ 
  init(); 
  ap_interval_set_int(&c, inf, sup); 
}

inline interval::interval(double inf, double sup)
{ 
  init(); 
  ap_interval_set_double(&c, inf, sup); 
}

inline interval::interval(const frac& inf, const frac& sup)
{ 
  init(); 
  ap_interval_set_frac(&c, inf.num, inf.den, sup.num, sup.den); 
}

inline interval::interval(const mpq_class& inf, const mpq_class& sup) 
{
  init(); 
  ap_interval_set_mpq(&c,
		      const_cast<mpq_class&>(inf).get_mpq_t(),
		      const_cast<mpq_class&>(sup).get_mpq_t());
}

inline interval::interval(mpfr_t inf, mpfr_t sup) 
{
  init(); 
  ap_interval_set_mpfr(&c,inf,sup);
}

inline interval::interval(top t)
{ 
  init(); 
  ap_interval_set_top(&c); 
}

inline interval::interval(bottom t)
{ 
  init(); 
  ap_interval_set_bottom(&c); 
}


/* destructor */
/* ========== */

inline interval::~interval()
{ 
  ap_scalar_free(c.inf); 
ap_scalar_free(c.sup); 
}


/* assignments */
/* =========== */

/* = */

inline interval& interval::operator= (const interval& x)
{ 
  ap_interval_set(&c, const_cast<ap_interval_t*>(&x.c)); 
  return *this; 
}

inline interval& interval::operator= (top t)
{ 
  ap_interval_set_top(&c); 
  return *this; 
}

inline interval& interval::operator= (bottom t)
{ 
  ap_interval_set_bottom(&c); 
  return *this; 
}


/* set */

inline interval& interval::set(const interval& x)
{ 
  ap_interval_set(&c, const_cast<ap_interval_t*>(&x.c)); 
  return *this;
}

inline interval& interval::set(const scalar& inf, const scalar& sup)
{ 
  ap_interval_set_scalar(&c, 
			 const_cast<ap_scalar_t*>(inf.get_ap_scalar_t()), 
			 const_cast<ap_scalar_t*>(sup.get_ap_scalar_t()));
  return *this;
}

inline interval& interval::set(int inf, int sup)
{ 
  ap_interval_set_int(&c, inf, sup); 
  return *this;
}

inline interval& interval::set(long inf, long sup)
{ 
  ap_interval_set_int(&c, inf, sup); 
  return *this;
}

inline interval& interval::set(double inf, double sup)
{ 
  ap_interval_set_double(&c, inf, sup); 
  return *this;
}

inline interval& interval::set(const frac& inf, const frac& sup)
{ 
  ap_interval_set_frac(&c, inf.num, inf.den, sup.num, sup.den); 
  return *this;
}

inline interval& interval::set(const mpq_class& inf, const mpq_class& sup) 
{
  ap_interval_set_mpq(&c,
		      const_cast<mpq_class&>(inf).get_mpq_t(),
		      const_cast<mpq_class&>(sup).get_mpq_t());
  return *this;
}

inline interval& interval::set(mpfr_t inf, mpfr_t sup) 
{
  ap_interval_set_mpfr(&c,inf,sup);
  return *this;
}

inline interval& interval::set(top t)
{ 
  ap_interval_set_top(&c); 
  return *this;
}

inline interval& interval::set(bottom t)
{ 
  ap_interval_set_bottom(&c); 
  return *this;
}


/* swap */

inline void swap(interval& a, interval &b)
{ 
  ap_interval_swap(&a.c, &b.c); 
}


/* access */
/* ====== */

inline scalar& interval::get_inf() 
{ 
  return reinterpret_cast<scalar&>(*c.inf); 
}
  
inline scalar& interval::get_sup() 
{ 
  return reinterpret_cast<scalar&>(*c.sup); 
}

inline const scalar& interval::get_inf() const
{ 
  return reinterpret_cast<const scalar&>(*c.inf); 
}
  
inline const scalar& interval::get_sup() const
{ 
  return reinterpret_cast<const scalar&>(*c.sup); 
}


/* print */
/* ===== */

inline std::ostream& operator<< (std::ostream& os, const interval& s)
{
  return os << '[' << s.get_inf() << ',' << s.get_sup() << ']';
}

inline void interval::print(FILE* stream) const 
{ 
  ap_interval_fprint(stream, const_cast<ap_interval_t*>(&c)); 
}


/* tests */
/* ===== */
 
inline bool interval::is_top() const
{ 
  return ap_interval_is_top(const_cast<ap_interval_t*>(&c)); 
}

inline bool interval::is_bottom() const
{ 
  return ap_interval_is_bottom(const_cast<ap_interval_t*>(&c)); 
}

inline bool operator<= (const interval&a, const interval &b)
{ 
  return ap_interval_is_leq(const_cast<ap_interval_t*>(&a.c), 
			    const_cast<ap_interval_t*>(&b.c)); 
}

inline bool operator>= (const interval&a, const interval &b)
{
  return ap_interval_is_leq(const_cast<ap_interval_t*>(&b.c), 
			    const_cast<ap_interval_t*>(&a.c)); 
}

inline bool operator< (const interval&a, const interval &b)
{ 
  return ap_interval_cmp(const_cast<ap_interval_t*>(&a.c), 
			 const_cast<ap_interval_t*>(&b.c)) == -1; 
}

inline bool operator> (const interval&a, const interval &b)
{ 
  return ap_interval_cmp(const_cast<ap_interval_t*>(&a.c), 
			 const_cast<ap_interval_t*>(&b.c)) == 1; 
}

inline bool operator== (const interval&a, const interval &b)
{ 
  return ap_interval_equal(const_cast<ap_interval_t*>(&a.c), 
			   const_cast<ap_interval_t*>(&b.c)); 
}

inline bool operator!= (const interval&a, const interval &b)
{ 
  return !ap_interval_equal(const_cast<ap_interval_t*>(&a.c), 
			    const_cast<ap_interval_t*>(&b.c)); 
}

inline interval::order cmp (const interval&a, const interval &b)
{ 
  return (interval::order)ap_interval_cmp(const_cast<ap_interval_t*>(&a.c), 
					  const_cast<ap_interval_t*>(&b.c)); 
}

 
/* other operators */
/* =============== */

inline void interval::neg()
{ 
  ap_interval_neg(&c, &c); 
}

inline interval interval::operator- () const
{ 
  interval r = *this; 
  r.neg(); 
  return r; 
}

inline long interval::hash() const
{ 
  return ap_interval_hash(const_cast<ap_interval_t*>(&c)); 
}


/* C-level compatibility */

inline const ap_interval_t* interval::get_ap_interval_t() const
{ 
  return &c; 
}

inline ap_interval_t* interval::get_ap_interval_t()
{ 
  return &c; 
}





/* ================================= */
/* interval_array                    */
/* ================================= */


inline interval_array::interval_array(size_t size, ap_interval_t** c) 
  : sz(size), c(c)
{}

  
/* constructors */
/* ============ */

inline interval_array::interval_array(size_t size) 
  : sz(size), c(ap_interval_array_alloc(size))
{}

inline interval_array::interval_array(const interval_array &x)
  : sz(x.sz), c(ap_interval_array_alloc(x.sz))
{
  for (size_t i=0;i<sz;i++)
    ap_interval_set(c[i], x.c[i]);
}

inline interval_array::interval_array(const std::vector<interval>& x)
  : sz(x.size()), c(ap_interval_array_alloc(x.size()))
{
  for (size_t i=0;i<sz;i++)
    ap_interval_set(c[i], const_cast<ap_interval_t*>(x[i].get_ap_interval_t()));
}

inline interval_array::interval_array(size_t size, const interval x[])
  : sz(size), c(ap_interval_array_alloc(size))
{
  for (size_t i=0;i<size;i++)
    ap_interval_set(c[i], const_cast<ap_interval_t*>(x[i].get_ap_interval_t()));  
}



/* destructor */
/* ========== */

inline interval_array::~interval_array()
{ 
  ap_interval_array_free(c, sz); 
}


/* assignment */
/* ========== */

inline interval_array& interval_array::operator= (const interval_array &x)
{
  if (&x!=this) {
    if (sz != x.sz) {
      ap_interval_array_free(c, sz);
      sz = x.sz;
      c = ap_interval_array_alloc(sz);
    }
    for (size_t i=0;i<sz;i++)
      ap_interval_set(c[i], x.c[i]);
  }
  return *this;
}

inline interval_array& interval_array::operator= (const std::vector<interval>& x)
{
  if (sz != x.size()) {
    ap_interval_array_free(c, sz);
    sz = x.size();
    c = ap_interval_array_alloc(sz);
  }
  for (size_t i=0;i<sz;i++)
    ap_interval_set(c[i], const_cast<ap_interval_t*>(x[i].get_ap_interval_t()));
  return *this;
}

inline interval_array& interval_array::operator= (const interval x[])
{
  for (size_t i=0;i<sz;i++)
    ap_interval_set(c[i], const_cast<ap_interval_t*>(x[i].get_ap_interval_t()));
  return *this;
}


/* conversion */
/* ========== */

inline interval_array::operator std::vector<interval>() const
{
  std::vector<interval> v = std::vector<interval>(sz);
  for (size_t i=0;i<sz;i++)
    ap_interval_set(v[i].get_ap_interval_t(), c[i]);
  return v;
}


/* print */
/* ===== */

inline std::ostream& operator<< (std::ostream& os, const interval_array& s)
{
  os << "{ ";
  for (size_t i=0;i<s.sz;i++)
    os << s.get(i) << " ";
  return os << "}";
}

inline void interval_array::print(FILE* stream) const 
{ 
  fprintf(stream, "{ " );
  for (size_t i=0;i<sz;i++) {
    ap_interval_fprint(stream, const_cast<ap_interval_t*>(c[i])); 
    fprintf(stream, " ");
  }
  fprintf(stream,"}");
}


/* access */
/* ====== */

inline size_t interval_array::size() const
{ return sz; }
 
inline interval** interval_array::contents()
{ return reinterpret_cast<interval**>(c); }

inline interval& interval_array::get(size_t i)
{ 
  if (i >= sz) throw std::out_of_range("apron::interval_array::get(size_t)");
  return reinterpret_cast<interval&>(*c[i]); 
}

inline const interval& interval_array::get(size_t i) const
{ 
  if (i >= sz) throw std::out_of_range("apron::interval_array::get(size_t)");
  return reinterpret_cast<interval&>(*c[i]); 
}

inline interval& interval_array::operator[](size_t i)
{ 
  return reinterpret_cast<interval&>(*c[i]); 
}

inline const interval& interval_array::operator[](size_t i) const
{ 
  return reinterpret_cast<interval&>(*c[i]); 
}


/* C API compatibility */
/* =================== */

inline const ap_interval_t * const * interval_array::get_ap_interval_t_array() const
{ 
  return c; 
}

inline ap_interval_t** interval_array::get_ap_interval_t_array()
{ 
  return c; 
}

