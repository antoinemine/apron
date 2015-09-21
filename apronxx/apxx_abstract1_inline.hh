/* -*- C++ -*-
 * apxx_abstract1_inline.hh
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
/* abstract1                         */
/* ================================= */


/* constructors */
/* ============ */

inline abstract1::abstract1(ap_abstract1_t x) : a(x)
{}

inline abstract1::abstract1(manager &m, const environment& e, top x)
{
  a = ap_abstract1_top(m.get_ap_manager_t(),
		       const_cast<ap_environment_t*>(e.get_ap_environment_t()));
  m.raise("apron::abstract1::abstract1(manager &, const environment&, top)",a);
}

inline abstract1::abstract1(manager &m, const environment& e, bottom x)
{
  a = ap_abstract1_bottom(m.get_ap_manager_t(),
			  const_cast<ap_environment_t*>(e.get_ap_environment_t()));
  m.raise("apron::abstract1::abstract1(manager &, const environment&, bottom)",a);
}

inline abstract1::abstract1(manager &m, const abstract1& t)
{
  a = ap_abstract1_copy(m.get_ap_manager_t(), const_cast<ap_abstract1_t*>(&t.a));
  m.raise("apron::abstract1::abstract1(manager &, const abstract1&)",a);
}

inline abstract1::abstract1(manager &m, const environment& e, const abstract0& t)
{
  a.env = ap_environment_copy(const_cast<ap_environment_t*>(e.get_ap_environment_t()));
  a.abstract0 = ap_abstract0_copy(m.get_ap_manager_t(),
			       const_cast<ap_abstract0_t*>(t.get_ap_abstract0_t()));
  m.raise("apron::abstract1::abstract1(manager &, const abstract1&)",a);
}

inline abstract1::abstract1(manager &m, const environment& e, const var v[], const interval_array& x)
{
  a = ap_abstract1_of_box(m.get_ap_manager_t(),
			  const_cast<ap_environment_t*>(e.get_ap_environment_t()),
			  reinterpret_cast<ap_var_t*>(const_cast<var*>(v)),
			  const_cast<ap_interval_t**>(x.get_ap_interval_t_array()),
			  x.size());
  m.raise("apron::abstract1::abstract1(manager &, const environment&, const var[], const interval_array&)",a);
}

inline abstract1::abstract1(manager &m, const environment& e, const std::vector<var>& v, const interval_array& x)
{
  if (v.size()!=x.size())
    throw std::invalid_argument("apron::abstract1::abstract1(manager &, const environment&, const std::vector<var>&, const interval_array&): different array sizes");
  ap_var_t vv[v.size()];
  for (size_t i=0;i<v.size();i++) vv[i] = v[i].get_ap_var_t();
  a = ap_abstract1_of_box(m.get_ap_manager_t(),
			  const_cast<ap_environment_t*>(e.get_ap_environment_t()),
			  vv,
			  const_cast<ap_interval_t**>(x.get_ap_interval_t_array()),
			  v.size());
  m.raise("apron::abstract1::abstract1(manager &, const environment&, const std::vector<var>&, const interval_array&)",a);
}

inline abstract1::abstract1(manager &m, const lincons1_array& x)
{
  a = ap_abstract1_of_lincons_array(m.get_ap_manager_t(),
				    const_cast<ap_environment_t*>(x.get_environment().get_ap_environment_t()),
				    const_cast<ap_lincons1_array_t*>(x.get_ap_lincons1_array_t()));
  m.raise("apron::abstract1::abstract1(manager &, const lincons1_array&)",a);
}

inline abstract1::abstract1(manager &m, const tcons1_array& x)
{
  a = ap_abstract1_of_tcons_array(m.get_ap_manager_t(),
				  const_cast<ap_environment_t*>(x.get_environment().get_ap_environment_t()),
				  const_cast<ap_tcons1_array_t*>(x.get_ap_tcons1_array_t()));
  m.raise("apron::abstract1::abstract1(manager &, const tcons1_array&)",a);
}

inline abstract1::abstract1(const abstract1& t)
{
  a = ap_abstract1_copy(t.a.abstract0->man, const_cast<ap_abstract1_t*>(&t.a));
  manager::raise(a.abstract0->man, "apron::abstract1::abstract1(abstract1&)",a);
}


/* destructor */
/* ========== */

inline abstract1::~abstract1()
{
  if (a.abstract0) ap_abstract1_clear(a.abstract0->man, &a);
}


inline void abstract1::free(manager& m)
{
  if (a.abstract0) ap_abstract1_clear(m.get_ap_manager_t(), &a);
}



/* assignments */
/* =========== */

inline abstract1& abstract1::operator=(const abstract1& t)
{
  if (&t!=this) {
    ap_abstract1_t r = ap_abstract1_copy(a.abstract0->man,
					 const_cast<ap_abstract1_t*>(&t.a));
    manager::raise(a.abstract0->man, "apron::abstract1::operator=(const abstract1&)",r);
    ap_abstract1_clear(a.abstract0->man, &a);
    a = r;
  }
  return *this;
}

inline abstract1& abstract1::operator=(top t)
{
  ap_abstract1_t r = ap_abstract1_top(a.abstract0->man, a.env);
  manager::raise(a.abstract0->man, "apron::abstract1::operator=(top)",r);
  ap_abstract1_clear(a.abstract0->man, &a);
  a = r;
  return *this;
}

inline abstract1& abstract1::operator=(bottom t)
{
  ap_abstract1_t r = ap_abstract1_bottom(a.abstract0->man, a.env);
  manager::raise(a.abstract0->man, "apron::abstract1::operator=(bottom)",r);
  ap_abstract1_clear(a.abstract0->man, &a);
  a = r;
  return *this;
}

inline abstract1& abstract1::operator=(const interval_array& x)
{
  ap_dimension_t d = ap_abstract0_dimension(a.abstract0->man, a.abstract0);
  if (x.size()<d.intdim+d.realdim)
    throw std::invalid_argument("apron::abstract1::operator=(const interval_array&) array too short");
  ap_abstract0_t* r = ap_abstract0_of_box(a.abstract0->man, d.intdim, d.realdim,
					  const_cast<ap_interval_t**>(x.get_ap_interval_t_array()));
  manager::raise(a.abstract0->man, "apron::abstract1::operator=(const interval_array&)",r);
  ap_abstract0_free(a.abstract0->man, a.abstract0);
  a.abstract0 = r;
  return *this;
}

inline abstract1& abstract1::operator=(const lincons1_array& x)
{
  ap_abstract1_t r =
    ap_abstract1_of_lincons_array(a.abstract0->man,
				  const_cast<ap_environment_t*>(x.get_environment().get_ap_environment_t()),
				  const_cast<ap_lincons1_array_t*>(x.get_ap_lincons1_array_t()));
  manager::raise(a.abstract0->man, "apron::abstract1::operator=(const lincons1_array&)",r);
  ap_abstract1_clear(a.abstract0->man, &a);
  a = r;
  return *this;
}

inline abstract1& abstract1::operator=(const tcons1_array& x)
{
  ap_abstract1_t r =
    ap_abstract1_of_tcons_array(a.abstract0->man,
				const_cast<ap_environment_t*>(x.get_environment().get_ap_environment_t()),
				const_cast<ap_tcons1_array_t*>(x.get_ap_tcons1_array_t()));
  manager::raise(a.abstract0->man, "apron::abstract1::operator=(const tcons1_array&)",r);
  ap_abstract1_clear(a.abstract0->man, &a);
  a = r;
  return *this;
}


inline abstract1& abstract1::set(manager& m, const abstract1& x)
{
  if (&x!=this) {
    ap_abstract1_t r = ap_abstract1_copy(m.get_ap_manager_t(),
					 const_cast<ap_abstract1_t*>(&x.a));
    m.raise("apron::abstract1::set(manager&, abstract1&)",r);
    ap_abstract1_clear(m.get_ap_manager_t(), &a);
    a = r;

  }
  return *this;
}

inline abstract1& abstract1::set(manager& m, top t)
{
  ap_abstract1_t r = ap_abstract1_top(m.get_ap_manager_t(), a.env);
  m.raise("apron::abstract1::set(manager&, top)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &a);
  a = r;
  return *this;
}

inline abstract1& abstract1::set(manager& m, const environment& e, top t)
{
  ap_abstract1_t r = ap_abstract1_top(m.get_ap_manager_t(),
				      const_cast<ap_environment_t*>(e.get_ap_environment_t()));
  m.raise("apron::abstract1::set(manager&, const environment&, top)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &a);
  a = r;
  return *this;
}

inline abstract1& abstract1::set(manager& m, bottom t)
{
  ap_abstract1_t r = ap_abstract1_bottom(m.get_ap_manager_t(), a.env);
  m.raise("apron::abstract1::set(manager&, bottom)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &a);
  a = r;
  return *this;
}

inline abstract1& abstract1::set(manager& m, const environment& e, bottom t)
{
  ap_abstract1_t r = ap_abstract1_bottom(m.get_ap_manager_t(),
					 const_cast<ap_environment_t*>(e.get_ap_environment_t()));
  m.raise("apron::abstract1::set(manager&, const environment&, bottom)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &a);
  a = r;
  return *this;
}

inline abstract1& abstract1::set(manager& m, const interval_array& x)
{
  ap_dimension_t d = ap_abstract0_dimension(m.get_ap_manager_t(), a.abstract0);
  if (x.size()<d.intdim+d.realdim)
    throw std::invalid_argument("apron::abstract1::set(manager&, const interval_array&) array too short");
  ap_abstract0_t* r = ap_abstract0_of_box(m.get_ap_manager_t(), d.intdim, d.realdim,
					  const_cast<ap_interval_t**>(x.get_ap_interval_t_array()));
  m.raise("apron::abstract1::operator=(const interval_array&)",r);
  ap_abstract0_free(m.get_ap_manager_t(), a.abstract0);
  a.abstract0 = r;
  return *this;
}


inline abstract1& abstract1::set(manager &m, const environment& e, const var v[], const interval_array& x)
{
  ap_abstract1_t r =
    ap_abstract1_of_box(m.get_ap_manager_t(),
			const_cast<ap_environment_t*>(e.get_ap_environment_t()),
			reinterpret_cast<ap_var_t*>(const_cast<var*>(v)),
			const_cast<ap_interval_t**>(x.get_ap_interval_t_array()),
			x.size());
  m.raise("apron::abstract1::set(manager &, const environment&, const var[], const interval_array&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(),&a);
  a = r;
  return *this;
}

inline abstract1& abstract1::set(manager &m, const environment& e, const std::vector<var>& v, const interval_array& x)
{
  if (v.size()!=x.size())
    throw std::invalid_argument("apron::abstract1::abstract1(manager &, const environment&, const std::vector<var>&, const interval_array&): different array sizes");
  ap_var_t vv[v.size()];
  for (size_t i=0;i<v.size();i++) vv[i] = v[i].get_ap_var_t();
  ap_abstract1_t r =
    ap_abstract1_of_box(m.get_ap_manager_t(),
			const_cast<ap_environment_t*>(e.get_ap_environment_t()),
			vv,
			const_cast<ap_interval_t**>(x.get_ap_interval_t_array()),
			v.size());
  m.raise("apron::abstract1::set(manager &, const environment&, const std::vector<var>&, const interval_array&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &a);
  a = r;
  return *this;
}

inline abstract1& abstract1::set(manager& m, const lincons1_array& x)
{
  ap_abstract1_t r =
    ap_abstract1_of_lincons_array(m.get_ap_manager_t(),
				  const_cast<ap_environment_t*>(x.get_environment().get_ap_environment_t()),
				  const_cast<ap_lincons1_array_t*>(x.get_ap_lincons1_array_t()));
  m.raise("apron::abstract1::set(manager &, const lincons1_array&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &a);
  a = r;
  return *this;
}

inline abstract1& abstract1::set(manager& m, const tcons1_array& x)
{
  ap_abstract1_t r =
    ap_abstract1_of_tcons_array(m.get_ap_manager_t(),
				const_cast<ap_environment_t*>(x.get_environment().get_ap_environment_t()),
				const_cast<ap_tcons1_array_t*>(x.get_ap_tcons1_array_t()));
  m.raise("apron::abstract1::set(manager &, const tcons1_array&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &a);
  a = r;
  return *this;
}


/* representation */
/* ============== */

inline abstract1& abstract1::minimize(manager& m)
{
  ap_abstract1_minimize(m.get_ap_manager_t(), &a);
  m.raise("apron::abstract1::minimize(manager &)");
  return *this;
}

inline abstract1& abstract1::canonicalize(manager& m)
{
  ap_abstract1_canonicalize(m.get_ap_manager_t(), &a);
  m.raise("apron::abstract1::canonicalize(manager &)");
  return *this;
}

inline abstract1& abstract1::approximate(manager& m, int algorithm)
{
  ap_abstract1_approximate(m.get_ap_manager_t(), &a, algorithm);
  m.raise("apron::abstract1::approximate(manager &, int)");
  return *this;
}


/* printing */
/* ======== */

inline void abstract1::print(manager& m, FILE* stream) const
{
  ap_abstract1_fprint(stream, m.get_ap_manager_t(), const_cast<ap_abstract1_t*>(&a));
  m.raise("apron::abstract1::print(manager&, FILE*)");
}

inline void printdiff(manager& m, const abstract1& x, const abstract1& y, FILE* stream = stdout)
{
  ap_abstract1_fprintdiff(stream, m.get_ap_manager_t(),
			  const_cast<ap_abstract1_t*>(&x.a),
			  const_cast<ap_abstract1_t*>(&y.a));
  m.raise("apron::printdiff(manager&, const abstract1&, const abstract1&, FILE*)");
}

inline void abstract1::dump(manager& m, FILE* stream) const
{
  ap_abstract1_fdump(stream, m.get_ap_manager_t(), const_cast<ap_abstract1_t*>(&a));
  m.raise("apron::abstract1::dump(manager&, FILE*)");
}

inline std::ostream& operator<< (std::ostream& os, const abstract1& s)
{
  manager m = s.get_manager();
  if (s.is_bottom(m)) return os << "bottom";
  if (s.is_top(m)) return os << "top";
  return os << s.to_lincons_array(m);
}


/* serialisation */
/* ============= */

inline std::string* abstract1::serialize(manager& m) const
{
  ap_membuf_t x = ap_abstract1_serialize_raw(m.get_ap_manager_t(),
					     const_cast<ap_abstract1_t*>(&a));
  m.raise("apron::abstract1::serialize_raw(manager&)");
  std::string* s = new std::string((char*)x.ptr, x.size);
  ::free(x.ptr);
  return s;
}

inline abstract1& deserialize(manager& m, abstract1& dst, const std::string& s, size_t* eaten = NULL)
{
  size_t x = s.size();
  ap_abstract1_t r =
    ap_abstract1_deserialize_raw(m.get_ap_manager_t(), const_cast<char*>(s.data()), &x);
  m.raise("apron::deserialize_raw(manager&, abstract1&, const std::string&, size_t*)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  if (eaten) *eaten = x;
  return dst;
}


/* access */
/* ====== */

inline manager abstract1::get_manager() const
{
  return ap_manager_copy(ap_abstract0_manager(const_cast<ap_abstract0_t*>(a.abstract0)));
}

inline environment abstract1::get_environment() const
{
  return ap_environment_copy(const_cast<ap_environment_t*>(a.env));
}

inline abstract0& abstract1::get_abstract0()
{
  return reinterpret_cast<abstract0&>(a.abstract0);
}

inline const abstract0& abstract1::get_abstract0() const
{
  return reinterpret_cast<const abstract0&>(a.abstract0);
}

inline size_t abstract1::size(manager& m) const
{
  size_t sz = ap_abstract1_size(m.get_ap_manager_t(),
				const_cast<ap_abstract1_t*>(&a));
  m.raise("apron::abstract1::get_size(manager&)");
  return sz;
}


/* predicates */
/* ========== */



inline bool abstract1::is_bottom(manager& m) const
{
  bool r = ap_abstract1_is_bottom(m.get_ap_manager_t(),
				  const_cast<ap_abstract1_t*>(&a));
  m.raise("apron::abstract1::is_bottom(manager&)");
  return r;
}

inline bool abstract1::is_top(manager& m) const
{
  bool r = ap_abstract1_is_top(m.get_ap_manager_t(),
			       const_cast<ap_abstract1_t*>(&a));
  m.raise("apron::abstract1::is_top(manager&)");
  return r;
}

inline bool abstract1::is_eq(manager& m, const abstract1& x) const
{
  bool r = ap_abstract1_is_eq(m.get_ap_manager_t(),
			      const_cast<ap_abstract1_t*>(&a),
			      const_cast<ap_abstract1_t*>(&x.a));
  m.raise("apron::abstract1::is_eq(manager&, const abstract&)");
  return r;
}

inline bool abstract1::is_leq(manager& m, const abstract1& x) const
{
  bool r = ap_abstract1_is_leq(m.get_ap_manager_t(),
			       const_cast<ap_abstract1_t*>(&a),
			       const_cast<ap_abstract1_t*>(&x.a));
  m.raise("apron::abstract1::is_leq(manager&, const abstract&)");
  return r;
}

inline bool abstract1::sat(manager& m, const lincons1& l) const
{
  bool r = ap_abstract1_sat_lincons(m.get_ap_manager_t(),
				    const_cast<ap_abstract1_t*>(&a),
				    const_cast<ap_lincons1_t*>(l.get_ap_lincons1_t()));
  m.raise("apron::abstract1::sat(manager&, const lincons1&)");
  return r;
}

inline bool abstract1::sat(manager& m, const tcons1& l) const
{
  bool r = ap_abstract1_sat_tcons(m.get_ap_manager_t(),
				  const_cast<ap_abstract1_t*>(&a),
				  const_cast<ap_tcons1_t*>(l.get_ap_tcons1_t()));
  m.raise("apron::abstract1::sat(manager&, const tcons1&)");
  return r;
}

inline bool abstract1::sat(manager& m, const var& v, const interval& i) const
{
  bool r = ap_abstract1_sat_interval(m.get_ap_manager_t(),
				     const_cast<ap_abstract1_t*>(&a),
				     v.get_ap_var_t(),
				     const_cast<ap_interval_t*>(i.get_ap_interval_t()));
  m.raise("apron::abstract1::sat(manager&, const var&, const interval&)");
  return r;
}

inline bool abstract1::is_variable_unconstrained(manager& m, const var& v) const
{
  bool r = ap_abstract1_is_variable_unconstrained(m.get_ap_manager_t(),
						  const_cast<ap_abstract1_t*>(&a),
						  v.get_ap_var_t());
  m.raise("apron::abstract1::is_variable_unconstrained(manager&, const var&)");
  return r;
}


inline bool operator== (const abstract1& x, const abstract1& y)
{
  bool r = ap_abstract1_is_eq(x.a.abstract0->man,
			      const_cast<ap_abstract1_t*>(&x.a),
			      const_cast<ap_abstract1_t*>(&y.a));
  manager::raise(x.a.abstract0->man, "apron::operator==(const abstract1&, const abstract1&)");
  return r;
}

inline bool operator!= (const abstract1& x, const abstract1& y)
{
  return !(x==y);
}

inline bool operator<= (const abstract1& x, const abstract1& y)
{
  bool r = ap_abstract1_is_leq(x.a.abstract0->man,
			       const_cast<ap_abstract1_t*>(&x.a),
			       const_cast<ap_abstract1_t*>(&y.a));
  manager::raise(x.a.abstract0->man, "apron::operator<=(const abstract1&, const abstract1&)");
  return r;
}

inline bool operator>= (const abstract1& x, const abstract1& y)
{
  return y<=x;
}

inline bool operator> (const abstract1& x, const abstract1& y)
{
  return !(x<=y);
}

inline bool operator< (const abstract1& x, const abstract1& y)
{
  return !(y<=x);
}


/* Properties */
/* ========== */


inline interval abstract1::bound(manager& m, const linexpr1& l) const
{
  ap_interval_t* r =
    ap_abstract1_bound_linexpr(m.get_ap_manager_t(),
			       const_cast<ap_abstract1_t*>(&a),
			       const_cast<ap_linexpr1_t*>(l.get_ap_linexpr1_t()));
  m.raise("apron::abstract1::bound(manager&, const linexpr1&)");
  return r;
}

inline interval abstract1::bound(manager& m, const texpr1& l) const
{
  ap_interval_t* r =
    ap_abstract1_bound_texpr(m.get_ap_manager_t(),
			     const_cast<ap_abstract1_t*>(&a),
			     const_cast<ap_texpr1_t*>(l.get_ap_texpr1_t()));
  if (m.exception_raised()) ap_interval_free(r);
  m.raise("apron::abstract1::bound(manager&, const texpr1&)");
  return r;
}

inline interval abstract1::bound(manager& m, const var& d) const
{
  ap_interval_t* r =
    ap_abstract1_bound_variable(m.get_ap_manager_t(),
				const_cast<ap_abstract1_t*>(&a),
				d.get_ap_var_t());
  if (m.exception_raised()) ap_interval_free(r);
  m.raise("apron::abstract1::bound(manager&, ap_dim_t)");
  return r;
}

inline interval_array abstract1::to_box(manager& m) const
{
  ap_box1_t r =
    ap_abstract1_to_box(m.get_ap_manager_t(),
			const_cast<ap_abstract1_t*>(&a));
  if (m.exception_raised()) ap_box1_clear(&r);
  m.raise("apron::abstract1::to_box(manager&)");
  size_t sz = r.env->intdim + r.env->realdim;
  ap_environment_free(r.env);
  return interval_array(sz, r.p);
}

inline lincons1_array abstract1::to_lincons_array(manager& m) const
{
  ap_lincons1_array_t r =
    ap_abstract1_to_lincons_array(m.get_ap_manager_t(),
				  const_cast<ap_abstract1_t*>(&a));
  if (m.exception_raised()) ap_lincons1_array_clear(&r);
  m.raise("apron::abstract1::to_lincons_array(manager&)");
  return r;
}

inline tcons1_array abstract1::to_tcons_array(manager& m) const
{
  ap_tcons1_array_t r =
    ap_abstract1_to_tcons_array(m.get_ap_manager_t(),
				const_cast<ap_abstract1_t*>(&a));
  if (m.exception_raised()) ap_tcons1_array_clear(&r);
  m.raise("apron::abstract1::to_tcons_array(manager&)");
  return r;
}

inline generator1_array abstract1::to_generator_array(manager& m) const
{
  ap_generator1_array_t r =
    ap_abstract1_to_generator_array(m.get_ap_manager_t(),
				    const_cast<ap_abstract1_t*>(&a));
  if (m.exception_raised()) ap_generator1_array_clear(&r);
  m.raise("apron::abstract1::to_generator_array(manager&)");
  return r;
}


/* Meet, join, unification */
/* ======================= */

inline abstract1& abstract1::unify(manager& m, const abstract1& y)
{
  a = ap_abstract1_unify(m.get_ap_manager_t(), true,
			 const_cast<ap_abstract1_t*>(&a),
			 const_cast<ap_abstract1_t*>(&y.a));
  m.raise("apron::abstract1::unify(manager&, const abstract1&)");
  return *this;
}

inline abstract1& unify(manager& m, abstract1& dst, const abstract1& x, const abstract1& y)
{
  ap_abstract1_t r =
    ap_abstract1_unify(m.get_ap_manager_t(), false,
		       const_cast<ap_abstract1_t*>(&x.a),
		       const_cast<ap_abstract1_t*>(&y.a));
  m.raise("apron::unify(manager&, abstract1&, const abstract1&, const abstract1&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}

inline abstract1& abstract1::meet(manager& m, const abstract1& y)
{
  a = ap_abstract1_meet(m.get_ap_manager_t(), true,
			const_cast<ap_abstract1_t*>(&a),
			const_cast<ap_abstract1_t*>(&y.a));
  m.raise("apron::abstract1::meet(manager&, const abstract1&)");
  return *this;
}

inline abstract1& meet(manager& m, abstract1& dst, const abstract1& x, const abstract1& y)
{
  ap_abstract1_t r =
    ap_abstract1_meet(m.get_ap_manager_t(), false,
		      const_cast<ap_abstract1_t*>(&x.a),
		      const_cast<ap_abstract1_t*>(&y.a));
  m.raise("apron::meet(manager&, abstract1&, const abstract1&, const abstract1&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}

inline abstract1& meet(manager& m, abstract1& dst, const std::vector<const abstract1*>& x)
{
  ap_abstract1_t xx[x.size()];
  for (size_t i=0;i<x.size();i++)
    xx[i] = *(x[i]->get_ap_abstract1_t());
  ap_abstract1_t r =
    ap_abstract1_meet_array(m.get_ap_manager_t(), xx, x.size());
  m.raise("apron::meet(manager&, abstract1&, const std::vector<const abstract1*>&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}

inline abstract1& meet(manager& m, abstract1& dst, size_t sz, const abstract1 * const x[])
{
  ap_abstract1_t xx[sz];
  for (size_t i=0;i<sz;i++)
    xx[i] = *(x[i]->get_ap_abstract1_t());
  ap_abstract1_t r =
    ap_abstract1_meet_array(m.get_ap_manager_t(), xx, sz);
  m.raise("apron::meet(manager&, abstract1&, size_t, const abstract1 * const [])",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}

inline abstract1& abstract1::join(manager& m, const abstract1& y)
{
  a = ap_abstract1_join(m.get_ap_manager_t(), true,
			const_cast<ap_abstract1_t*>(&a),
			const_cast<ap_abstract1_t*>(&y.a));
  m.raise("apron::abstract1::join(manager&, const abstract1&)");
  return *this;
}

inline abstract1& join(manager& m, abstract1& dst, const abstract1& x, const abstract1& y)
{
  ap_abstract1_t r =
    ap_abstract1_join(m.get_ap_manager_t(), false,
		      const_cast<ap_abstract1_t*>(&x.a),
		      const_cast<ap_abstract1_t*>(&y.a));
  m.raise("apron::join(manager&, abstract1&, const abstract1&, const abstract1&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}

inline abstract1& join(manager& m, abstract1& dst, size_t sz, const abstract1 * const x[])
{
  ap_abstract1_t xx[sz];
  for (size_t i=0;i<sz;i++)
    xx[i] = *(x[i]->get_ap_abstract1_t());
  ap_abstract1_t r =
    ap_abstract1_join_array(m.get_ap_manager_t(), xx, sz);
  m.raise("apron::join(manager&, abstract1&, size_t, const abstract1 * const [])",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}

inline abstract1& join(manager& m, abstract1& dst, const std::vector<const abstract1*>& x)
{
  ap_abstract1_t xx[x.size()];
  for (size_t i=0;i<x.size();i++)
    xx[i] = *(x[i]->get_ap_abstract1_t());
  ap_abstract1_t r =
    ap_abstract1_join_array(m.get_ap_manager_t(), xx, x.size());
  m.raise("apron::join(manager&, abstract1&, const std::vector<const abstract1*>&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}

inline abstract1& abstract1::meet(manager& m, const lincons1_array& y)
{
  a =
    ap_abstract1_meet_lincons_array(m.get_ap_manager_t(), true,
				    const_cast<ap_abstract1_t*>(&a),
				    const_cast<ap_lincons1_array_t*>(y.get_ap_lincons1_array_t()));
  m.raise("apron::abstract1::meet(manager&, const lincons1_array&)");
  return *this;
}

inline abstract1& meet(manager& m, abstract1& dst, const abstract1& x, const lincons1_array& y)
{
  ap_abstract1_t r =
    ap_abstract1_meet_lincons_array(m.get_ap_manager_t(), false,
				    const_cast<ap_abstract1_t*>(&x.a),
				    const_cast<ap_lincons1_array_t*>(y.get_ap_lincons1_array_t()));
  m.raise("apron::meet(manager&, abstract1&, const abstract1&, const lincons1_array&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}


inline abstract1& abstract1::meet(manager& m, const tcons1_array& y)
{
  a = ap_abstract1_meet_tcons_array(m.get_ap_manager_t(), true,
				    const_cast<ap_abstract1_t*>(&a),
				    const_cast<ap_tcons1_array_t*>(y.get_ap_tcons1_array_t()));
  m.raise("apron::abstract1::meet(manager&, const tcons1_array&)");
  return *this;
}

inline abstract1& meet(manager& m, abstract1& dst, const abstract1& x, const tcons1_array& y)
{
  ap_abstract1_t r =
    ap_abstract1_meet_tcons_array(m.get_ap_manager_t(), false,
				  const_cast<ap_abstract1_t*>(&x.a),
				  const_cast<ap_tcons1_array_t*>(y.get_ap_tcons1_array_t()));
  m.raise("apron::meet(manager&, abstract1&, const abstract1&, const tcons1_array&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}



inline abstract1& abstract1::add_rays(manager& m, const generator1_array& y)
{
  a =
    ap_abstract1_add_ray_array(m.get_ap_manager_t(), true,
			       const_cast<ap_abstract1_t*>(&a),
			       const_cast<ap_generator1_array_t*>(y.get_ap_generator1_array_t()));
  m.raise("apron::abstract1::add_rays(manager&, const generator1_array&)");
  return *this;
}

inline abstract1& add_rays(manager& m, abstract1& dst, const abstract1& x, const generator1_array& y)
{
  ap_abstract1_t r =
    ap_abstract1_add_ray_array(m.get_ap_manager_t(), false,
			       const_cast<ap_abstract1_t*>(&x.a),
			       const_cast<ap_generator1_array_t*>(y.get_ap_generator1_array_t()));
  m.raise("apron::add_rays(manager&, abstract1&, const abstract1&, const generator1_array&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}



inline abstract1& abstract1::operator*=(const abstract1& y)
{
  a =
    ap_abstract1_meet(a.abstract0->man, true,
		      const_cast<ap_abstract1_t*>(&a),
		      const_cast<ap_abstract1_t*>(&y.a));
  manager::raise(a.abstract0->man, "apron::abstract1::operator*=(const abstract1&)");
  return *this;
}

inline abstract1& abstract1::operator+=(const abstract1& y)
{
  a =
    ap_abstract1_join(a.abstract0->man, true,
		      const_cast<ap_abstract1_t*>(&a),
		      const_cast<ap_abstract1_t*>(&y.a));
  manager::raise(a.abstract0->man, "apron::abstract1::operator+=(const abstract1&)");
  return *this;
}

inline abstract1& abstract1::operator*=(const lincons1_array& y)
{
  a =
    ap_abstract1_meet_lincons_array(a.abstract0->man, true,
				    const_cast<ap_abstract1_t*>(&a),
				    const_cast<ap_lincons1_array_t*>(y.get_ap_lincons1_array_t()));
  manager::raise(a.abstract0->man, "apron::abstract1::operator*=(const lincons1_array&)");
  return *this;
}

inline abstract1& abstract1::operator*=(const tcons1_array& y)
{
  a =
    ap_abstract1_meet_tcons_array(a.abstract0->man, true,
				  const_cast<ap_abstract1_t*>(&a),
				  const_cast<ap_tcons1_array_t*>(y.get_ap_tcons1_array_t()));
  manager::raise(a.abstract0->man, "apron::abstract1::operator*=(const tcons1_array&)");
  return *this;
}

inline abstract1& abstract1::operator+=(const generator1_array& y)
{
  a =
    ap_abstract1_add_ray_array(a.abstract0->man, true,
			       const_cast<ap_abstract1_t*>(&a),
			       const_cast<ap_generator1_array_t*>(y.get_ap_generator1_array_t()));
  manager::raise(a.abstract0->man, "apron::abstract1::operator+=(const generator1_array&)");
  return *this;
}



/* Assignments */
/* =========== */

static inline ap_abstract1_t* ap_abstract1_t_or_null(const abstract1& i)
{
  ap_abstract1_t* r = const_cast<ap_abstract1_t*>(i.get_ap_abstract1_t());
  if (r->abstract0) return r;
  return NULL;
}

inline abstract1& abstract1::assign(manager& m, const var& v, const linexpr1& l, const abstract1& inter)
{
  a = ap_abstract1_assign_linexpr_array(m.get_ap_manager_t(), true, &a,
					reinterpret_cast<ap_var_t*>(const_cast<var*>(&v)),
					const_cast<ap_linexpr1_t*>(l.get_ap_linexpr1_t()),
					1,
					ap_abstract1_t_or_null(inter));
  m.raise("apron::abstract1::assign(manager&, size_t size, const var&, const linexpr1&, const abstract1&)");
  return *this;
}

inline abstract1& abstract1::assign(manager& m, size_t size, const var v[], const linexpr1 * const l[], const abstract1& inter)
{
  ap_linexpr1_t ll[size];
  for (size_t i=0;i<size;i++) ll[i] = *(l[i]->get_ap_linexpr1_t());
  a =
    ap_abstract1_assign_linexpr_array(m.get_ap_manager_t(), true, &a,
				      reinterpret_cast<ap_var_t*>(const_cast<var*>(v)),
				      ll, size,
				      ap_abstract1_t_or_null(inter));
  m.raise("apron::abstract1::assign(manager&, size_t size, const var[], const linexpr1 * const [], const abstract1&)");
  return *this;
}


inline abstract1& abstract1::assign(manager& m, const std::vector<var>& v, const std::vector<const linexpr1*>& l, const abstract1& inter)
{
  if (l.size()!=v.size())
    throw std::invalid_argument("apron::abstract1::assign(manager&, const std::vector<var>&, const std::vector<const linexpr1*>&, const abstract1&) vectors have different size");
  ap_linexpr1_t ll[l.size()];
  ap_var_t vv[v.size()];
  for (size_t i=0;i<l.size();i++) {
    ll[i] = *(l[i]->get_ap_linexpr1_t());
    vv[i] = v[i].get_ap_var_t();
  }
  a =
    ap_abstract1_assign_linexpr_array(m.get_ap_manager_t(), true, &a,
				      vv, ll, l.size(),
				      ap_abstract1_t_or_null(inter));
  m.raise("apron::abstract1::assign(manager&, const std::vector<var>&, const std::vector<const linexpr1*>&, const abstract1&)");
  return *this;
}


inline abstract1& assign(manager& m, abstract1& dst, const abstract1& src, const var& v, const linexpr1& l, const abstract1& inter = abstract1::null)
{
  ap_abstract1_t r =
    ap_abstract1_assign_linexpr_array(m.get_ap_manager_t(), false,
				      const_cast<ap_abstract1_t*>(&src.a),
				      reinterpret_cast<ap_var_t*>(const_cast<var*>(&v)),
				      const_cast<ap_linexpr1_t*>(l.get_ap_linexpr1_t()),
				      1,
				      ap_abstract1_t_or_null(inter));
  m.raise("apron::assign(manager&, abstract1&, const abstract1&, const var&, const linexpr1&, const abstract1&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}

inline abstract1& assign(manager& m, abstract1& dst, const abstract1& src, size_t size, const var v[], const linexpr1 * const l[], const abstract1& inter = abstract1::null)
{
  ap_linexpr1_t ll[size];
  for (size_t i=0;i<size;i++) ll[i] = *(l[i]->get_ap_linexpr1_t());
  ap_abstract1_t r =
    ap_abstract1_assign_linexpr_array(m.get_ap_manager_t(), false,
				      const_cast<ap_abstract1_t*>(&src.a),
				      reinterpret_cast<ap_var_t*>(const_cast<var*>(v)),
				      ll, size,
				      ap_abstract1_t_or_null(inter));
  m.raise("apron::assign((manager&, abstract1&, const abstract1&, size_t size, const var[], const linexpr1 * const [], const abstract1&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}

inline abstract1& assign(manager& m, abstract1& dst, const abstract1& src, const std::vector<var>& v, const std::vector<const linexpr1*>& l, const abstract1& inter = abstract1::null)
{
  if (l.size()!=v.size())
    throw std::invalid_argument("apron::assign(manager&, abstract1, const abstract1&, const std::vector<var>&, const std::vector<const linexpr1*>&, const abstract1&) vectors have different size");
  ap_linexpr1_t ll[l.size()];
  ap_var_t vv[v.size()];
  for (size_t i=0;i<l.size();i++) {
    ll[i] = *(l[i]->get_ap_linexpr1_t());
    vv[i] = v[i].get_ap_var_t();
  }
  ap_abstract1_t r =
    ap_abstract1_assign_linexpr_array(m.get_ap_manager_t(), false,
				      const_cast<ap_abstract1_t*>(&src.a),
				      vv, ll, l.size(),
				      ap_abstract1_t_or_null(inter));
  m.raise("apron::assign(manager&, abstract1, const abstract1&, const std::vector<var>&, const std::vector<const linexpr1*>&, const abstract1&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}





inline abstract1& abstract1::assign(manager& m, const var& v, const texpr1& l, const abstract1& inter)
{
  a = ap_abstract1_assign_texpr_array(m.get_ap_manager_t(), true, &a,
				      reinterpret_cast<ap_var_t*>(const_cast<var*>(&v)),
				      const_cast<ap_texpr1_t*>(l.get_ap_texpr1_t()),
				      1,
				      ap_abstract1_t_or_null(inter));
  m.raise("apron::abstract1::assign(manager&, size_t size, const var&, const texpr1&, const abstract1&)");
  return *this;
}

inline abstract1& abstract1::assign(manager& m, size_t size, const var v[], const texpr1 * const l[], const abstract1& inter)
{
  ap_texpr1_t ll[size];
  for (size_t i=0;i<size;i++) ll[i] = *(l[i]->get_ap_texpr1_t());
  a = ap_abstract1_assign_texpr_array(m.get_ap_manager_t(), true, &a,
				      reinterpret_cast<ap_var_t*>(const_cast<var*>(v)),
				      ll, size,
				      ap_abstract1_t_or_null(inter));
  m.raise("apron::abstract1::assign(manager&, size_t size, const var[], const texpr1 * const [], const abstract1&)");
  return *this;
}

inline abstract1& abstract1::assign(manager& m, const std::vector<var>& v, const std::vector<const texpr1*>& l, const abstract1& inter)
{
  if (l.size()!=v.size())
    throw std::invalid_argument("apron::abstract1::assign(manager&, const std::vector<var>&, const std::vector<const texpr1*>&, const abstract1&) vectors have different size");
  ap_texpr1_t ll[l.size()];
  ap_var_t vv[v.size()];
  for (size_t i=0;i<l.size();i++) {
    ll[i] = *(l[i]->get_ap_texpr1_t());
    vv[i] = v[i].get_ap_var_t();
  }
  a = ap_abstract1_assign_texpr_array(m.get_ap_manager_t(), true, &a,
				      vv, ll, l.size(),
				      ap_abstract1_t_or_null(inter));
  m.raise("apron::abstract1::assign(manager&, const std::vector<var>&, const std::vector<const texpr1*>&, const abstract1&) vectors have different size");
  return *this;
}

inline abstract1& assign(manager& m, abstract1& dst, const abstract1& src, const var& v, const texpr1& l, const abstract1& inter = abstract1::null)
{
  ap_abstract1_t r =
    ap_abstract1_assign_texpr_array(m.get_ap_manager_t(), false,
				    const_cast<ap_abstract1_t*>(&src.a),
				    reinterpret_cast<ap_var_t*>(const_cast<var*>(&v)),
				    const_cast<ap_texpr1_t*>(l.get_ap_texpr1_t()),
				    1,
				    ap_abstract1_t_or_null(inter));
  m.raise("apron::assign(manager&, abstract1&, const abstract1&, const var&, const texpr1&, const abstract1&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}

inline abstract1& assign(manager& m, abstract1& dst, const abstract1& src, size_t size, const var v[], const texpr1 * const l[], const abstract1& inter = abstract1::null)
{
  ap_texpr1_t ll[size];
  for (size_t i=0;i<size;i++) ll[i] = *(l[i]->get_ap_texpr1_t());
  ap_abstract1_t r =
    ap_abstract1_assign_texpr_array(m.get_ap_manager_t(), false,
				    const_cast<ap_abstract1_t*>(&src.a),
				    reinterpret_cast<ap_var_t*>(const_cast<var*>(v)),
				    ll, size,
				    ap_abstract1_t_or_null(inter));
  m.raise("apron::assign((manager&, abstract1&, const abstract1&, size_t size, const var[], const texpr1 * const [], const abstract1&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}

inline abstract1& assign(manager& m, abstract1& dst, const abstract1& src, const std::vector<var>& v, const std::vector<const texpr1*>& l, const abstract1& inter = abstract1::null)
{
  if (l.size()!=v.size())
    throw std::invalid_argument("apron::assign(manager&, abstract1, const abstract1&, const std::vector<var>&, const std::vector<const texpr1*>&, const abstract1&) vectors have different size");
  ap_texpr1_t ll[l.size()];
  ap_var_t vv[v.size()];
  for (size_t i=0;i<l.size();i++) {
    ll[i] = *(l[i]->get_ap_texpr1_t());
    vv[i] = v[i].get_ap_var_t();
  }
  ap_abstract1_t r =
    ap_abstract1_assign_texpr_array(m.get_ap_manager_t(), false,
				    const_cast<ap_abstract1_t*>(&src.a),
				    vv, ll, l.size(),
				    ap_abstract1_t_or_null(inter));
  m.raise("apron::assign(manager&, abstract1, const abstract1&, const std::vector<var>&, const std::vector<const texpr1*>&, const abstract1&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}



/* Substitutions */
/* =========== = */


inline abstract1& abstract1::substitute(manager& m, const var& v, const linexpr1& l, const abstract1& inter)
{
  a = ap_abstract1_substitute_linexpr_array(m.get_ap_manager_t(), true, &a,
					    reinterpret_cast<ap_var_t*>(const_cast<var*>(&v)),
					    const_cast<ap_linexpr1_t*>(l.get_ap_linexpr1_t()),
					    1,
					    ap_abstract1_t_or_null(inter));
  m.raise("apron::abstract1::substitute(manager&, size_t size, const var&, const linexpr1&, const abstract1&)");
  return *this;
}

inline abstract1& abstract1::substitute(manager& m, size_t size, const var v[], const linexpr1 * const l[], const abstract1& inter)
{
  ap_linexpr1_t ll[size];
  for (size_t i=0;i<size;i++) ll[i] = *(l[i]->get_ap_linexpr1_t());
  a =
    ap_abstract1_substitute_linexpr_array(m.get_ap_manager_t(), true, &a,
					  reinterpret_cast<ap_var_t*>(const_cast<var*>(v)),
					  ll, size,
					  ap_abstract1_t_or_null(inter));
  m.raise("apron::abstract1::substitute(manager&, size_t size, const var[], const linexpr1 * const [], const abstract1&)");
  return *this;
}


inline abstract1& abstract1::substitute(manager& m, const std::vector<var>& v, const std::vector<const linexpr1*>& l, const abstract1& inter)
{
  if (l.size()!=v.size())
    throw std::invalid_argument("apron::abstract1::substitute(manager&, const std::vector<var>&, const std::vector<const linexpr1*>&, const abstract1&) vectors have different size");
  ap_linexpr1_t ll[l.size()];
  ap_var_t vv[v.size()];
  for (size_t i=0;i<l.size();i++) {
    ll[i] = *(l[i]->get_ap_linexpr1_t());
    vv[i] = v[i].get_ap_var_t();
  }
  a =
    ap_abstract1_substitute_linexpr_array(m.get_ap_manager_t(), true, &a,
					  vv, ll, l.size(),
					  ap_abstract1_t_or_null(inter));
  m.raise("apron::abstract1::substitute(manager&, const std::vector<var>&, const std::vector<const linexpr1*>&, const abstract1&)");
  return *this;
}


inline abstract1& substitute(manager& m, abstract1& dst, const abstract1& src, const var& v, const linexpr1& l, const abstract1& inter = abstract1::null)
{
  ap_abstract1_t r =
    ap_abstract1_substitute_linexpr_array(m.get_ap_manager_t(), false,
					  const_cast<ap_abstract1_t*>(&src.a),
					  reinterpret_cast<ap_var_t*>(const_cast<var*>(&v)),
					  const_cast<ap_linexpr1_t*>(l.get_ap_linexpr1_t()),
					  1,
					  ap_abstract1_t_or_null(inter));
  m.raise("apron::substitute(manager&, abstract1&, const abstract1&, const var&, const linexpr1&, const abstract1&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}

inline abstract1& substitute(manager& m, abstract1& dst, const abstract1& src, size_t size, const var v[], const linexpr1 * const l[], const abstract1& inter = abstract1::null)
{
  ap_linexpr1_t ll[size];
  for (size_t i=0;i<size;i++) ll[i] = *(l[i]->get_ap_linexpr1_t());
  ap_abstract1_t r =
    ap_abstract1_substitute_linexpr_array(m.get_ap_manager_t(), false,
					  const_cast<ap_abstract1_t*>(&src.a),
					  reinterpret_cast<ap_var_t*>(const_cast<var*>(v)),
					  ll, size,
					  ap_abstract1_t_or_null(inter));
  m.raise("apron::substitute((manager&, abstract1&, const abstract1&, size_t size, const var[], const linexpr1 * const [], const abstract1&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}

inline abstract1& substitute(manager& m, abstract1& dst, const abstract1& src, const std::vector<var>& v, const std::vector<const linexpr1*>& l, const abstract1& inter = abstract1::null)
{
  if (l.size()!=v.size())
    throw std::invalid_argument("apron::substitute(manager&, abstract1, const abstract1&, const std::vector<var>&, const std::vector<const linexpr1*>&, const abstract1&) vectors have different size");
  ap_linexpr1_t ll[l.size()];
  ap_var_t vv[v.size()];
  for (size_t i=0;i<l.size();i++) {
    ll[i] = *(l[i]->get_ap_linexpr1_t());
    vv[i] = v[i].get_ap_var_t();
  }
  ap_abstract1_t r =
    ap_abstract1_substitute_linexpr_array(m.get_ap_manager_t(), false,
					  const_cast<ap_abstract1_t*>(&src.a),
					  vv, ll, l.size(),
					  ap_abstract1_t_or_null(inter));
  m.raise("apron::substitute(manager&, abstract1, const abstract1&, const std::vector<var>&, const std::vector<const linexpr1*>&, const abstract1&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}





inline abstract1& abstract1::substitute(manager& m, const var& v, const texpr1& l, const abstract1& inter)
{
  a = ap_abstract1_substitute_texpr_array(m.get_ap_manager_t(), true, &a,
					  reinterpret_cast<ap_var_t*>(const_cast<var*>(&v)),
					  const_cast<ap_texpr1_t*>(l.get_ap_texpr1_t()),
					  1,
					  ap_abstract1_t_or_null(inter));
  m.raise("apron::abstract1::substitute(manager&, size_t size, const var&, const texpr1&, const abstract1&)");
  return *this;
}

inline abstract1& abstract1::substitute(manager& m, size_t size, const var v[], const texpr1 * const l[], const abstract1& inter)
{
  ap_texpr1_t ll[size];
  for (size_t i=0;i<size;i++) ll[i] = *(l[i]->get_ap_texpr1_t());
  a = ap_abstract1_substitute_texpr_array(m.get_ap_manager_t(), true, &a,
					  reinterpret_cast<ap_var_t*>(const_cast<var*>(v)),
					  ll, size,
					  ap_abstract1_t_or_null(inter));
  m.raise("apron::abstract1::substitute(manager&, size_t size, const var[], const texpr1 * const [], const abstract1&)");
  return *this;
}

inline abstract1& abstract1::substitute(manager& m, const std::vector<var>& v, const std::vector<const texpr1*>& l, const abstract1& inter)
{
  if (l.size()!=v.size())
    throw std::invalid_argument("apron::abstract1::substitute(manager&, const std::vector<var>&, const std::vector<const texpr1*>&, const abstract1&) vectors have different size");
  ap_texpr1_t ll[l.size()];
  ap_var_t vv[v.size()];
  for (size_t i=0;i<l.size();i++) {
    ll[i] = *(l[i]->get_ap_texpr1_t());
    vv[i] = v[i].get_ap_var_t();
  }
  a = ap_abstract1_substitute_texpr_array(m.get_ap_manager_t(), true, &a,
					  vv, ll, l.size(),
					  ap_abstract1_t_or_null(inter));
  m.raise("apron::abstract1::substitute(manager&, const std::vector<var>&, const std::vector<const texpr1*>&, const abstract1&) vectors have different size");
  return *this;
}

inline abstract1& substitute(manager& m, abstract1& dst, const abstract1& src, const var& v, const texpr1& l, const abstract1& inter = abstract1::null)
{
  ap_abstract1_t r =
    ap_abstract1_substitute_texpr_array(m.get_ap_manager_t(), false,
					const_cast<ap_abstract1_t*>(&src.a),
					reinterpret_cast<ap_var_t*>(const_cast<var*>(&v)),
					const_cast<ap_texpr1_t*>(l.get_ap_texpr1_t()),
					1,
					ap_abstract1_t_or_null(inter));
  m.raise("apron::substitute(manager&, abstract1&, const abstract1&, const var&, const texpr1&, const abstract1&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}

inline abstract1& substitute(manager& m, abstract1& dst, const abstract1& src, size_t size, const var v[], const texpr1 * const l[], const abstract1& inter = abstract1::null)
{
  ap_texpr1_t ll[size];
  for (size_t i=0;i<size;i++) ll[i] = *(l[i]->get_ap_texpr1_t());
  ap_abstract1_t r =
    ap_abstract1_substitute_texpr_array(m.get_ap_manager_t(), false,
					const_cast<ap_abstract1_t*>(&src.a),
					reinterpret_cast<ap_var_t*>(const_cast<var*>(v)),
					ll, size,
					ap_abstract1_t_or_null(inter));
  m.raise("apron::substitute((manager&, abstract1&, const abstract1&, size_t size, const var[], const texpr1 * const [], const abstract1&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}

inline abstract1& substitute(manager& m, abstract1& dst, const abstract1& src, const std::vector<var>& v, const std::vector<const texpr1*>& l, const abstract1& inter = abstract1::null)
{
  if (l.size()!=v.size())
    throw std::invalid_argument("apron::substitute(manager&, abstract1, const abstract1&, const std::vector<var>&, const std::vector<const texpr1*>&, const abstract1&) vectors have different size");
  ap_texpr1_t ll[l.size()];
  ap_var_t vv[v.size()];
  for (size_t i=0;i<l.size();i++) {
    ll[i] = *(l[i]->get_ap_texpr1_t());
    vv[i] = v[i].get_ap_var_t();
  }
  ap_abstract1_t r =
    ap_abstract1_substitute_texpr_array(m.get_ap_manager_t(), false,
					const_cast<ap_abstract1_t*>(&src.a),
					vv, ll, l.size(),
					ap_abstract1_t_or_null(inter));
  m.raise("apron::substitute(manager&, abstract1, const abstract1&, const std::vector<var>&, const std::vector<const texpr1*>&, const abstract1&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}





/* Projection */
/* ========== */

inline abstract1& abstract1::forget(manager& m, const var& v, bool project)
{
  return forget(m,1,&v,project);
}

inline abstract1& abstract1::forget(manager& m, size_t size, const var v[], bool project)
{
  a = ap_abstract1_forget_array(m.get_ap_manager_t(), true, &a,
				reinterpret_cast<ap_var_t*>(const_cast<var*>(v)),
				size, project);
  m.raise("apron::abstract1::forget(manager&, size_t, const var[], bool)");
  return *this;
}

inline abstract1& abstract1::forget(manager& m, const std::vector<var>& v, bool project)
{
  ap_var_t vv[v.size()];
  for (size_t i=0;i<v.size();i++) vv[i] = v[i].get_ap_var_t();
  a = ap_abstract1_forget_array(m.get_ap_manager_t(), true, &a,	vv, v.size(), project);
  m.raise("apron::abstract1::forget(manager&,  const std::vector<var>&, bool)");
  return *this;
}

inline abstract1& forget(manager& m, abstract1& dst, const abstract1& src, const var& v, bool project = false)
{
  return forget(m,dst,src,1,&v,project);
}

inline abstract1& forget(manager& m, abstract1& dst, const abstract1& src, size_t size, const var v[], bool project = false)
{
  ap_abstract1_t r = ap_abstract1_forget_array(m.get_ap_manager_t(), false,
					       const_cast<ap_abstract1_t*>(&src.a),
					       reinterpret_cast<ap_var_t*>(const_cast<var*>(v)),
					       size, project);
  m.raise("apron::forget(manager&, abstract1&, const abstract1&, size_t, const var[], bool)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}

inline abstract1& forget(manager& m, abstract1& dst, const abstract1& src, const std::vector<var>& v, bool project = false)
{
  ap_var_t vv[v.size()];
  for (size_t i=0;i<v.size();i++) vv[i] = v[i].get_ap_var_t();
  ap_abstract1_t r = ap_abstract1_forget_array(m.get_ap_manager_t(), false,
					       const_cast<ap_abstract1_t*>(&src.a),
					       vv, v.size(), project);
  m.raise("apron::forget(manager&, abstract1&, const abstract1&, const std::vector<var>&, bool)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}



/* Change of environment */
/* ====================== */

inline abstract1& abstract1::change_environment(manager& m, const environment& e, bool project)
{
  a = ap_abstract1_change_environment(m.get_ap_manager_t(), true, &a,
				      const_cast<ap_environment_t*>(e.get_ap_environment_t()),
				      project);
  m.raise("apron::abstract1::change_environment(manager&, const environment&, bool)");
  return *this;
}

inline abstract1& change_environment(manager& m, abstract1& dst, const abstract1& src, const environment& e, bool project = false)
{
  ap_abstract1_t r =
    ap_abstract1_change_environment(m.get_ap_manager_t(), false,
				    const_cast<ap_abstract1_t*>(&src.a),
				    const_cast<ap_environment_t*>(e.get_ap_environment_t()),
				    project);
  m.raise("apron::change_environment(manager&, abstrct1&, const abstract1&, const environment&, bool)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}

inline abstract1& abstract1::minimize_environment(manager& m)
{
  a = ap_abstract1_minimize_environment(m.get_ap_manager_t(), true, &a);
  m.raise("apron::abstract1::minimize_environment(manager&)");
  return *this;
}

inline abstract1& minimize_environment(manager& m, abstract1& dst, const abstract1& src)
{
  ap_abstract1_t r =
    ap_abstract1_minimize_environment(m.get_ap_manager_t(), false,
				      const_cast<ap_abstract1_t*>(&src.a));
  m.raise("apron::minimize_environment(manager&,  abstract1&, const abstract1&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}

inline abstract1& abstract1::rename(manager& m, size_t size, const var oldv[], const var newv[])
{
  a = ap_abstract1_rename_array(m.get_ap_manager_t(), true, &a,
				reinterpret_cast<ap_var_t*>(const_cast<var*>(oldv)),
				reinterpret_cast<ap_var_t*>(const_cast<var*>(newv)),
				size);
  m.raise("apron::abstract1::rename(manager&, size_t, const var[], const var[])");
  return *this;
}

inline abstract1& abstract1::rename(manager& m, const std::vector<var>& oldv, const std::vector<var>& newv)
{
  if (oldv.size()!=newv.size())
    throw std::invalid_argument("apron::abstract1::rename(manager&, const std::vector<var>&, const std::vector<var>&) vector have different size");
  ap_var_t oldvv[oldv.size()];
  ap_var_t newvv[newv.size()];
  for (size_t i=0;i<oldv.size();i++) {
    oldvv[i] = oldv[i].get_ap_var_t();
    newvv[i] = newv[i].get_ap_var_t();
  }
  a = ap_abstract1_rename_array(m.get_ap_manager_t(), true, &a,
				oldvv, newvv, oldv.size());
  m.raise("apron::abstract1::rename(manager&, const std::vector<var>&, const std::vector<var>&)");
  return *this;
}

inline abstract1& rename(manager& m, abstract1& dst, const abstract1& src, size_t size, const var oldv[], const var newv[])
{
  ap_abstract1_t r =
    ap_abstract1_rename_array(m.get_ap_manager_t(), false,
			      const_cast<ap_abstract1_t*>(&src.a),
			      reinterpret_cast<ap_var_t*>(const_cast<var*>(oldv)),
			      reinterpret_cast<ap_var_t*>(const_cast<var*>(newv)),
				size);
  m.raise("apron::rename(manager&, abstract1&, const abstract1&, size_t, const var[], const var[])",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}

inline abstract1& rename(manager& m, abstract1& dst, const abstract1& src, const std::vector<var>& oldv, const std::vector<var>& newv)
{
  if (oldv.size()!=newv.size())
    throw std::invalid_argument("apron::abstract1::rename(manager&, abstract1&, const abstract1&, const std::vector<var>&, const std::vector<var>&) vector have different size");
  ap_var_t oldvv[oldv.size()];
  ap_var_t newvv[newv.size()];
  for (size_t i=0;i<oldv.size();i++) {
    oldvv[i] = oldv[i].get_ap_var_t();
    newvv[i] = newv[i].get_ap_var_t();
  }
  ap_abstract1_t r =
    ap_abstract1_rename_array(m.get_ap_manager_t(), false,
			      const_cast<ap_abstract1_t*>(&src.a),
			      oldvv, newvv, oldv.size());
  m.raise("apron::rename(manager&, abstract1&, const abstract1&, const std::vector<var>&, const std::vector<var>&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}



/* Expansion and folding */
/* ===================== */

inline abstract1& abstract1::expand(manager& m, const var& v, size_t size, const var vv[])
{
  a = ap_abstract1_expand(m.get_ap_manager_t(), true, &a,
			  const_cast<ap_var_t>(v.get_ap_var_t()),
			  reinterpret_cast<ap_var_t*>(const_cast<var*>(vv)),
			  size);
  m.raise("apron::abstract1::expand(manager&, const var&, size_t, const var[])");
  return *this;
}

inline abstract1& abstract1::expand(manager& m, const var& v, const std::vector<var>& vv)
{
  ap_var_t ww[vv.size()];
  for (size_t i=0;i<vv.size();i++) ww[i] = vv[i].get_ap_var_t();
  a = ap_abstract1_expand(m.get_ap_manager_t(), true, &a,
			  const_cast<ap_var_t>(v.get_ap_var_t()),
			  ww, vv.size());
  m.raise("apron::abstract1::expand(manager&, const var&, const std::vector<var>&)");
  return *this;
}

inline abstract1& expand(manager& m, abstract1& dst, const abstract1& src, const var& v, size_t size, const var vv[])
{
  ap_abstract1_t r =
    ap_abstract1_expand(m.get_ap_manager_t(), false,
			const_cast<ap_abstract1_t*>(&src.a),
			const_cast<ap_var_t>(v.get_ap_var_t()),
			reinterpret_cast<ap_var_t*>(const_cast<var*>(vv)),
			size);
  m.raise("apron::expand(manager&, abstract1&, const abstract1&, const var&, size_t, const var[])",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}

inline abstract1& expand(manager& m, abstract1& dst, const abstract1& src, const var& v, const std::vector<var>& vv)
{
  ap_var_t ww[vv.size()];
  for (size_t i=0;i<vv.size();i++) ww[i] = vv[i].get_ap_var_t();
  ap_abstract1_t r =
    ap_abstract1_expand(m.get_ap_manager_t(), false,
			const_cast<ap_abstract1_t*>(&src.a),
			const_cast<ap_var_t>(v.get_ap_var_t()),
			ww, vv.size());
  m.raise("apron::expand(manager&, abstract1&, const abstract1&, const var&, const std::vector<var>&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}



inline abstract1& abstract1::fold(manager& m, size_t size, const var vv[])
{
  a = ap_abstract1_fold(m.get_ap_manager_t(), true, &a,
			reinterpret_cast<ap_var_t*>(const_cast<var*>(vv)),
			size);
  m.raise("apron::abstract1::fold(manager&, const var&, size_t, const var[])");
  return *this;
}

inline abstract1& abstract1::fold(manager& m, const std::vector<var>& vv)
{
  ap_var_t ww[vv.size()];
  for (size_t i=0;i<vv.size();i++) ww[i] = vv[i].get_ap_var_t();
  a = ap_abstract1_fold(m.get_ap_manager_t(), true, &a,	ww, vv.size());
  m.raise("apron::abstract1::fold(manager&, const std::vector<var>&)");
  return *this;
}

inline abstract1& fold(manager& m, abstract1& dst, const abstract1& src, size_t size, const var vv[])
{
  ap_abstract1_t r =
    ap_abstract1_fold(m.get_ap_manager_t(), false,
		      const_cast<ap_abstract1_t*>(&src.a),
		      reinterpret_cast<ap_var_t*>(const_cast<var*>(vv)),
		      size);
  m.raise("apron::fold(manager&, abstract1&, const abstract1&, size_t, const var[])",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}

inline abstract1& fold(manager& m, abstract1& dst, const abstract1& src, const std::vector<var>& vv)
{
  ap_var_t ww[vv.size()];
  for (size_t i=0;i<vv.size();i++) ww[i] = vv[i].get_ap_var_t();
  ap_abstract1_t r =
    ap_abstract1_fold(m.get_ap_manager_t(), false,
		      const_cast<ap_abstract1_t*>(&src.a),
		      ww, vv.size());
  m.raise("apron::fold(manager&, abstract1&, const abstract1&, const std::vector<var>&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}


/* Widenings */
/* ========= */

inline abstract1& widening(manager& m, abstract1& dst, const abstract1& x, const abstract1& y)
{
  ap_abstract1_t r =
    ap_abstract1_widening(m.get_ap_manager_t(),
			  const_cast<ap_abstract1_t*>(&x.a),
			  const_cast<ap_abstract1_t*>(&y.a));
  m.raise("apron::widening(manager&, abstract1&, const abstract1&, const abstract1&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}

inline abstract1& widening(manager& m, abstract1& dst, const abstract1& x, const abstract1& y, const lincons1_array& l)
{
  ap_abstract1_t r =
    ap_abstract1_widening_threshold(m.get_ap_manager_t(),
				    const_cast<ap_abstract1_t*>(&x.a),
				    const_cast<ap_abstract1_t*>(&y.a),
				    const_cast<ap_lincons1_array_t*>(l.get_ap_lincons1_array_t()));
  m.raise("apron::widening(manager&, abstract1&, const abstract1&, const abstract1&, const lincons1_array&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}


/* Closure */
/* ======= */


inline abstract1& abstract1::closure(manager& m)
{
  a = ap_abstract1_closure(m.get_ap_manager_t(), true, &a);
  m.raise("apron::abstract1::closured(manager&)");
  return *this;
}

inline abstract1& closure(manager& m, abstract1& dst, const abstract1& src)
{
  ap_abstract1_t r =
    ap_abstract1_closure(m.get_ap_manager_t(), false, const_cast<ap_abstract1_t*>(&src.a));
  m.raise("apron::closure(manager&, abstract1&, const abstract1&)",r);
  ap_abstract1_clear(m.get_ap_manager_t(), &dst.a);
  dst.a = r;
  return dst;
}


/* C-level compatibility */
/* ===================== */

inline ap_abstract1_t* abstract1::get_ap_abstract1_t()
{ return &a; }


inline const ap_abstract1_t* abstract1::get_ap_abstract1_t() const
{ return &a; }
