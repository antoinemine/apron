/* -*- C++ -*-
 * apxx_abstract0_inline.hh
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
/* abstract0                         */
/* ================================= */


/* constructors */
/* ============ */

inline abstract0::abstract0(ap_abstract0_t* x)
  : a(x)
{}

inline abstract0::abstract0(manager &m, size_t intdim, size_t realdim, top x)
{
  a = ap_abstract0_top(m.get_ap_manager_t(), intdim, realdim);
  m.raise("apron::abstract0::abstract0(manager&, size_t, size_t, top)",a);
}

inline abstract0::abstract0(manager &m, size_t intdim, size_t realdim, bottom x)
{
  a = ap_abstract0_bottom(m.get_ap_manager_t(), intdim, realdim);
  m.raise("apron::abstract0::abstract0(manager&, size_t, size_t, bottom)",a);
}

inline abstract0::abstract0(manager &m, const abstract0& t)
{
  a = ap_abstract0_copy(m.get_ap_manager_t(), t.a);
  m.raise("apron::abstract0::abstract0::(manager&, const abstract0&)",a);
}

inline abstract0::abstract0(manager &m, size_t intdim, size_t realdim, const interval_array& x)
{
  if (x.size()<intdim+realdim)
    throw std::invalid_argument("apron::abstract0::abstract0(manager&, size_t, size_t, const interval_array&) array too short");
  a = ap_abstract0_of_box(m.get_ap_manager_t(), intdim, realdim,
			  const_cast<ap_interval_t**>(x.get_ap_interval_t_array()));
  m.raise("apron::abstract0::abstract0(manager&, size_t, size_t, const interval_array&)",a);
}

inline abstract0::abstract0(manager &m, size_t intdim, size_t realdim, const lincons0_array& x)
{
  a = ap_abstract0_of_lincons_array(m.get_ap_manager_t(), intdim, realdim,
				    const_cast<ap_lincons0_array_t*>(x.get_ap_lincons0_array_t()));
  m.raise("apron::abstract0::abstract0(manager&, size_t, size_t, const lincons0_array&)",a);
}

inline abstract0::abstract0(manager &m, size_t intdim, size_t realdim, const tcons0_array& x)
{
  a = ap_abstract0_of_tcons_array(m.get_ap_manager_t(), intdim, realdim,
				  const_cast<ap_tcons0_array_t*>(x.get_ap_tcons0_array_t()));
  m.raise("apron::abstract0::abstract0(manager&, size_t, size_t, const tcons0_array&)",a);
}

inline abstract0::abstract0(const abstract0& t)
{
  a = ap_abstract0_copy(t.a->man, t.a);
  manager::raise(a->man, "apron::abstract0::abstract0(abstract0&)",a);
}



/* destructors */
/* =========== */

inline abstract0::~abstract0()
{
  if (a && a->value) ap_abstract0_free(a->man, a);
  a = NULL;
}

inline void abstract0::free(manager& m)
{
  if (a && a->value) ap_abstract0_free(m.get_ap_manager_t(), a);
  a = NULL;
}


/* assignments */
/* =========== */

inline abstract0& abstract0::operator=(const abstract0& t)
{
  if (&t!=this) {
    ap_abstract0_t* r = ap_abstract0_copy(a->man, t.a);
    manager::raise(a->man, "apron::abstract0::operator=(const abstract0&)",r);
    ap_abstract0_free(a->man, a);
    a = r;
  }
  return *this;
}

inline abstract0& abstract0::operator=(top t)
{
  ap_dimension_t d = ap_abstract0_dimension(a->man, a);
  ap_abstract0_t* r = ap_abstract0_top(a->man, d.intdim, d.realdim);
  manager::raise(a->man, "apron::abstract0::operator=(top)",r);
  ap_abstract0_free(a->man, a);
  a = r;
  return *this;
}

inline abstract0& abstract0::operator=(bottom t)
{
  ap_dimension_t d = ap_abstract0_dimension(a->man, a);
  ap_abstract0_t* r = ap_abstract0_bottom(a->man, d.intdim, d.realdim);
  manager::raise(a->man, "apron::abstract0::operator=(bottom)",r);
  ap_abstract0_free(a->man, a);
  a = r;
  return *this;
}

inline abstract0& abstract0::operator=(const interval_array& x)
{
  ap_dimension_t d = ap_abstract0_dimension(a->man, a);
  if (x.size()<d.intdim+d.realdim)
    throw std::invalid_argument("apron::abstract0::operator=(const interval_array&) array too short");
  ap_abstract0_t* r = ap_abstract0_of_box(a->man, d.intdim, d.realdim,
					  const_cast<ap_interval_t**>(x.get_ap_interval_t_array()));
  manager::raise(a->man, "apron::abstract0::operator=(const interval_array&)",r);
  ap_abstract0_free(a->man, a);
  a = r;
  return *this;
}

inline abstract0& abstract0::operator=(const lincons0_array& x)
{
  ap_dimension_t d = ap_abstract0_dimension(a->man, a);
  ap_abstract0_t* r =
    ap_abstract0_of_lincons_array(a->man, d.intdim, d.realdim,
				  const_cast<ap_lincons0_array_t*>(x.get_ap_lincons0_array_t()));
  manager::raise(a->man, "apron::abstract0::operator=(const lincons0_array&)",r);
  ap_abstract0_free(a->man, a);
  a = r;
  return *this;
}

inline abstract0& abstract0::operator=(const tcons0_array& x)
{
  ap_dimension_t d = ap_abstract0_dimension(a->man, a);
  ap_abstract0_t* r =
    ap_abstract0_of_tcons_array(a->man, d.intdim, d.realdim,
				const_cast<ap_tcons0_array_t*>(x.get_ap_tcons0_array_t()));
  manager::raise(a->man, "apron::abstract0::operator=(const tcons0_array&)",r);
  ap_abstract0_free(a->man, a);
  a = r;
  return *this;
}

inline abstract0& abstract0::set(manager& m, const abstract0& t)
{
  if (&t!=this) {
    ap_abstract0_t* r = ap_abstract0_copy(m.get_ap_manager_t(), t.a);
    m.raise("apron::abstract0::set(manager&, abstract0&)",r);
    ap_abstract0_free(m.get_ap_manager_t(), a);
    a = r;
  }
  return *this;
}

inline abstract0& abstract0::set(manager& m, top t, size_t intdim, size_t realdim)
{
  ap_dimension_t d = ap_abstract0_dimension(m.get_ap_manager_t(), a);
  if (intdim!=AP_DIM_MAX) d.intdim = intdim;
  if (realdim!=AP_DIM_MAX) d.realdim = realdim;
  ap_abstract0_t* r = ap_abstract0_top(m.get_ap_manager_t(), d.intdim, d.realdim);
  m.raise("apron::abstract0::set(manager&, top, size_t, size_t)",r);
  ap_abstract0_free(m.get_ap_manager_t(), a);
  a = r;
  return *this;
}

inline abstract0& abstract0::set(manager& m, bottom t, size_t intdim, size_t realdim)
{
  ap_dimension_t d = ap_abstract0_dimension(m.get_ap_manager_t(), a);
  if (intdim!=AP_DIM_MAX) d.intdim = intdim;
  if (realdim!=AP_DIM_MAX) d.realdim = realdim;
  ap_abstract0_t* r = ap_abstract0_bottom(m.get_ap_manager_t(), d.intdim, d.realdim);
  m.raise("apron::abstract0::set(manager&, bottom, size_t, size_t)",r);
  ap_abstract0_free(m.get_ap_manager_t(), a);
  a = r;
  return *this;
}

inline abstract0& abstract0::set(manager& m, const interval_array& x, size_t intdim, size_t realdim)
{
  ap_dimension_t d = ap_abstract0_dimension(m.get_ap_manager_t(), a);
  if (intdim!=AP_DIM_MAX) d.intdim = intdim;
  if (realdim!=AP_DIM_MAX) d.realdim = realdim;
  if (x.size()<d.intdim+d.realdim)
    throw std::invalid_argument("apron::abstract0::set(manager&, const interval_array&, size_t, size_t) array too short");
  ap_abstract0_t* r = ap_abstract0_of_box(m.get_ap_manager_t(), d.intdim, d.realdim,
					  const_cast<ap_interval_t**>(x.get_ap_interval_t_array()));
  m.raise("apron::abstract0::set(manager&, const interval_array&, size_t, size_t)",r);
  ap_abstract0_free(m.get_ap_manager_t(), a);
  a = r;
  return *this;
}

inline abstract0& abstract0::set(manager& m, const lincons0_array& x, size_t intdim, size_t realdim)
{
  ap_dimension_t d = ap_abstract0_dimension(m.get_ap_manager_t(), a);
  if (intdim!=AP_DIM_MAX) d.intdim = intdim;
  if (realdim!=AP_DIM_MAX) d.realdim = realdim;
  ap_abstract0_t* r =
    ap_abstract0_of_lincons_array(m.get_ap_manager_t(), d.intdim, d.realdim,
				  const_cast<ap_lincons0_array_t*>(x.get_ap_lincons0_array_t()));
  m.raise("apron::abstract0::set(manager&, const lincons0_array&, size_t, size_t)",r);
  ap_abstract0_free(m.get_ap_manager_t(), a);
  a = r;
  return *this;
}

inline abstract0& abstract0::set(manager& m, const tcons0_array& x, size_t intdim, size_t realdim)
{
  ap_dimension_t d = ap_abstract0_dimension(m.get_ap_manager_t(), a);
  if (intdim!=AP_DIM_MAX) d.intdim = intdim;
  if (realdim!=AP_DIM_MAX) d.realdim = realdim;
  ap_abstract0_t* r =
    ap_abstract0_of_tcons_array(m.get_ap_manager_t(), d.intdim, d.realdim,
				const_cast<ap_tcons0_array_t*>(x.get_ap_tcons0_array_t()));
  m.raise("apron::abstract0::set(manager&, const tcons0_array&, size_t, size_t)",r);
  ap_abstract0_free(m.get_ap_manager_t(), a);
  a = r;
  return *this;
}


/* representation */
/* ============== */

inline abstract0& abstract0::minimize(manager& m)
{
  ap_abstract0_minimize(m.get_ap_manager_t(), a);
  m.raise("abstract0::minimize(manager&)");
  return *this;
}

inline abstract0& abstract0::canonicalize(manager& m)
{
  ap_abstract0_canonicalize(m.get_ap_manager_t(), a);
  m.raise("apron::abstract0::canonicalize(manager&)");
  return *this;
}

inline abstract0& abstract0::approximate(manager& m, int algorithm)
{
  ap_abstract0_approximate(m.get_ap_manager_t(), a, algorithm);
  m.raise("apron::abstract0::approximate(manager&, int)");
  return *this;
}


/* printing */
/* ======== */

inline void abstract0::print(manager& m, char** name_of_dim, FILE* stream) const
{
  ap_abstract0_fprint(stream, m.get_ap_manager_t(), a, name_of_dim);
  m.raise("apron::abstract0::print(manager&, char**, FILE*)");
}

inline void printdiff(manager& m, const abstract0& x, const abstract0& y, char** name_of_dim = NULL, FILE* stream = stdout)
{
  ap_abstract0_fprintdiff(stream, m.get_ap_manager_t(), x.a, y.a, name_of_dim);
  m.raise("apron::printdiff(manager&, const abstract0&, const abstract0&, char**, FILE*)");
}

inline void abstract0::dump(manager& m, FILE* stream) const
{
  ap_abstract0_fdump(stream, m.get_ap_manager_t(), a);
  m.raise("apron::abstract0::dump(manager&, FILE*)");
}

inline std::ostream& operator<< (std::ostream& os, const abstract0& s)
{
  manager m = s.get_manager();
  if (s.is_bottom(m)) return os << "bottom";
  if (s.is_top(m)) return os << "top";
  return os << s.to_lincons_array(m);
}


/* serialisation */
/* ============= */

inline std::string* abstract0::serialize(manager& m) const
{
  ap_membuf_t x = ap_abstract0_serialize_raw(m.get_ap_manager_t(), a);
  m.raise("apron::abstract0::serialize_raw(manager&)");
  std::string* s = new std::string((char*)x.ptr, x.size);
  ::free(x.ptr);
  return s;
}

inline abstract0& deserialize(manager& m, abstract0& dst, const std::string& s, size_t* eaten = NULL)
{
  size_t x = s.size();
  ap_abstract0_t* r = ap_abstract0_deserialize_raw(m.get_ap_manager_t(), const_cast<char*>(s.data()), &x);
  m.raise("apron::deserialize_raw(manager&, abstract0&, const std::string&, size_t*)",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  if (eaten) *eaten = x;
  return dst;
}


/* access */
/* ====== */

inline manager abstract0::get_manager() const
{
  return ap_manager_copy(ap_abstract0_manager(a));
}

inline ap_dimension_t abstract0::get_dimension(manager& m) const
{
  ap_dimension_t d = ap_abstract0_dimension(m.get_ap_manager_t(), a);
  m.raise("apron::abstract0::get_dimension(manager&)");
  return d;
}

inline size_t abstract0::size(manager& m) const
{
  size_t sz = ap_abstract0_size(m.get_ap_manager_t(), a);
  m.raise("apron::abstract0::get_size(manager&)");
  return sz;
}



/* predicates */
/* ========== */

inline bool abstract0::is_bottom(manager& m) const
{
  bool r = ap_abstract0_is_bottom(m.get_ap_manager_t(), a);
  m.raise("apron::abstract0::is_bottom(manager&)");
  return r;
}

inline bool abstract0::is_top(manager& m) const
{
  bool r = ap_abstract0_is_top(m.get_ap_manager_t(), a);
  m.raise("apron::abstract0::is_top(manager&)");
  return r;
}

inline bool abstract0::is_eq(manager& m, const abstract0& x) const
{
  bool r = ap_abstract0_is_eq(m.get_ap_manager_t(), a, x.a);
  m.raise("apron::abstract0::is_eq(manager&, const abstract&)");
  return r;
}

inline bool abstract0::is_leq(manager& m, const abstract0& x) const
{
  bool r = ap_abstract0_is_leq(m.get_ap_manager_t(), a, x.a);
  m.raise("apron::abstract0::is_leq(manager&, const abstract&)");
  return r;
}

inline bool abstract0::sat(manager& m, const lincons0& l) const
{
  bool r = ap_abstract0_sat_lincons(m.get_ap_manager_t(), a, const_cast<ap_lincons0_t*>(l.get_ap_lincons0_t()));
  m.raise("apron::abstract0::sat(manager&, const lincons0&)");
  return r;
}

inline bool abstract0::sat(manager& m, const tcons0& l) const
{
  bool r = ap_abstract0_sat_tcons(m.get_ap_manager_t(), a, const_cast<ap_tcons0_t*>(l.get_ap_tcons0_t()));
  m.raise("apron::abstract0::sat(manager&, const tcons0&)");
  return r;
}

inline bool abstract0::sat(manager& m, ap_dim_t dim, const interval& i) const
{
  bool r = ap_abstract0_sat_interval(m.get_ap_manager_t(), a, dim, const_cast<ap_interval_t*>(i.get_ap_interval_t()));
  m.raise("apron::abstract0::sat(manager&, ap_dim_t, const interval&)");
  return r;
}

inline bool abstract0::is_dimension_unconstrained(manager& m, ap_dim_t dim) const
{
  bool r = ap_abstract0_is_dimension_unconstrained(m.get_ap_manager_t(), a, dim);
  m.raise("apron::abstract0::is_dimension_unconstrained(manager&, ap_dim_t)");
  return r;
}


inline bool operator== (const abstract0& x, const abstract0& y)
{
  bool r = ap_abstract0_is_eq(x.a->man, x.a, y.a);
  manager::raise(x.a->man, "apron::operator==(const abstract0&, const abstract0&)");
  return r;
}

inline bool operator!= (const abstract0& x, const abstract0& y)
{
  return !(x==y);
}

inline bool operator<= (const abstract0& x, const abstract0& y)
{
  bool r = ap_abstract0_is_leq(x.a->man, x.a, y.a);
  manager::raise(x.a->man, "apron::operator<=(const abstract0&, const abstract0&)");
  return r;
}

inline bool operator>= (const abstract0& x, const abstract0& y)
{
  return y<=x;
}

inline bool operator> (const abstract0& x, const abstract0& y)
{
  return !(x<=y);
}

inline bool operator< (const abstract0& x, const abstract0& y)
{
  return !(y<=x);
}


/* Properties */
/* ========== */

inline interval abstract0::bound(manager& m, const linexpr0& l) const
{
  ap_interval_t* r =
    ap_abstract0_bound_linexpr(m.get_ap_manager_t(), a,
			       const_cast<ap_linexpr0_t*>(l.get_ap_linexpr0_t()));
  if (m.exception_raised()) ap_interval_free(r);
  m.raise("apron::abstract0::bound(manager&, const linexpr0&)");
  return r;
}

inline interval abstract0::bound(manager& m, const texpr0& l) const
{
  ap_interval_t* r =
    ap_abstract0_bound_texpr(m.get_ap_manager_t(), a,
			     const_cast<ap_texpr0_t*>(l.get_ap_texpr0_t()));
  if (m.exception_raised()) ap_interval_free(r);
  m.raise("apron::abstract0::bound(manager&, const texpr0&)");
  return r;
}

inline interval abstract0::bound(manager& m, ap_dim_t d) const
{
  ap_interval_t* r = ap_abstract0_bound_dimension(m.get_ap_manager_t(), a, d);
  if (m.exception_raised()) ap_interval_free(r);
  m.raise("apron::abstract0::bound(manager&, ap_dim_t)");
  return r;
}

inline interval_array abstract0::to_box(manager& m) const
{
  ap_dimension_t d = ap_abstract0_dimension(m.get_ap_manager_t(), a);
  ap_interval_t** r = ap_abstract0_to_box(m.get_ap_manager_t(), a);
  m.raise("apron::abstract0::to_box(manager&)");
  return interval_array(d.intdim+d.realdim, r);
}

inline lincons0_array abstract0::to_lincons_array(manager& m) const
{
  ap_lincons0_array_t r = ap_abstract0_to_lincons_array(m.get_ap_manager_t(), a);
  if (m.exception_raised()) ap_lincons0_array_clear(&r);
  m.raise("apron::abstract0::to_lincons_array(manager&)");
  return r;
}

inline tcons0_array abstract0::to_tcons_array(manager& m) const
{
  ap_tcons0_array_t r = ap_abstract0_to_tcons_array(m.get_ap_manager_t(), a);
  if (m.exception_raised()) ap_tcons0_array_clear(&r);
  m.raise("apron::abstract0::to_tcons_array(manager&)");
  return r;
}

inline generator0_array abstract0::to_generator_array(manager& m) const
{
  ap_generator0_array_t r = ap_abstract0_to_generator_array(m.get_ap_manager_t(), a);
  if (m.exception_raised()) ap_generator0_array_clear(&r);
  m.raise("apron::abstract0::to_generator_array(manager&)");
  return r;
}


/* Meet and join */
/* ============= */

inline abstract0& abstract0::meet(manager& m, const abstract0& y)
{
  a = ap_abstract0_meet(m.get_ap_manager_t(), true, a, y.a);
  m.raise("apron::abstract0::meet(manager&, const abstract0&)");
  return *this;
}

inline abstract0& meet(manager& m, abstract0& dst, const abstract0& x, const abstract0& y)
{
  ap_abstract0_t* r = ap_abstract0_meet(m.get_ap_manager_t(), false, x.a, y.a);
  m.raise("apron::meet(manager&, abstract0&, const abstract0&, const abstract0&)",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  return dst;
}

inline abstract0& meet(manager& m, abstract0& dst, const std::vector<const abstract0*>& x)
{
  ap_abstract0_t* xx[x.size()];
  for (size_t i=0;i<x.size();i++)
    xx[i] = const_cast<abstract0*>(x[i])->get_ap_abstract0_t();
  ap_abstract0_t* r =
    ap_abstract0_meet_array(m.get_ap_manager_t(), xx, x.size());
  m.raise("apron::meet(manager&, abstract0&, const std::vector<const abstract0*>&)",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  return dst;
}

inline abstract0& meet(manager& m, abstract0& dst, size_t sz, const abstract0 * const x[])
{
  ap_abstract0_t* xx[sz];
  for (size_t i=0;i<sz;i++)
    xx[i] = const_cast<abstract0*>(x[i])->get_ap_abstract0_t();
  ap_abstract0_t* r =
    ap_abstract0_meet_array(m.get_ap_manager_t(), xx, sz);
  m.raise("apron::meet(manager&, abstract0&, size_t, const abstract0 * const [])",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  return dst;
}

inline abstract0& abstract0::join(manager& m, const abstract0& y)
{
  a = ap_abstract0_join(m.get_ap_manager_t(), true, a, y.a);
  m.raise("apron::abstract0::join(manager&, const abstract0&)");
  return *this;
}

inline abstract0& join(manager& m, abstract0& dst, const abstract0& x, const abstract0& y)
{
  ap_abstract0_t* r = ap_abstract0_join(m.get_ap_manager_t(), false, x.a, y.a);
  m.raise("apron::join(manager&, abstract0&, const abstract0&, const abstract0&)",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  return dst;
}

inline abstract0& join(manager& m, abstract0& dst, size_t sz, const abstract0 * const x[])
{
  ap_abstract0_t* xx[sz];
  for (size_t i=0;i<sz;i++)
    xx[i] = const_cast<abstract0*>(x[i])->get_ap_abstract0_t();
  ap_abstract0_t* r =
    ap_abstract0_join_array(m.get_ap_manager_t(), xx, sz);
  m.raise("apron::join(manager&, abstract0&, size_t, const abstract0 * const [])",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  return dst;
}

inline abstract0& join(manager& m, abstract0& dst, const std::vector<const abstract0*>& x)
{
  ap_abstract0_t* xx[x.size()];
  for (size_t i=0;i<x.size();i++)
    xx[i] = const_cast<abstract0*>(x[i])->get_ap_abstract0_t();
  ap_abstract0_t* r =
    ap_abstract0_join_array(m.get_ap_manager_t(), xx, x.size());
  m.raise("apron::join(manager&, abstract0&, const std::vector<const abstract0*>&)",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  return dst;
}

inline abstract0& abstract0::meet(manager& m, const lincons0_array& y)
{
  a = ap_abstract0_meet_lincons_array(m.get_ap_manager_t(), true, a,
				      const_cast<ap_lincons0_array_t*>(y.get_ap_lincons0_array_t()));
  m.raise("apron::abstract0::meet(manager&, const lincons0_array&)");
  return *this;
}

inline abstract0& meet(manager& m, abstract0& dst, const abstract0& x, const lincons0_array& y)
{
  ap_abstract0_t* r =
    ap_abstract0_meet_lincons_array(m.get_ap_manager_t(), false, x.a,
				    const_cast<ap_lincons0_array_t*>(y.get_ap_lincons0_array_t()));
  m.raise("apron::meet(manager&, abstract0&, const abstract0&, const lincons0_array&)",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  return dst;
}


inline abstract0& abstract0::meet(manager& m, const tcons0_array& y)
{
  a = ap_abstract0_meet_tcons_array(m.get_ap_manager_t(), true, a,
				    const_cast<ap_tcons0_array_t*>(y.get_ap_tcons0_array_t()));
  m.raise("apron::abstract0::meet(manager&, const tcons0_array&)");
  return *this;
}

inline abstract0& meet(manager& m, abstract0& dst, const abstract0& x, const tcons0_array& y)
{
  ap_abstract0_t* r =
    ap_abstract0_meet_tcons_array(m.get_ap_manager_t(), false, x.a,
				  const_cast<ap_tcons0_array_t*>(y.get_ap_tcons0_array_t()));
  m.raise("apron::meet(manager&, abstract0&, const abstract0&, const tcons0_array&)",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  return dst;
}



inline abstract0& abstract0::add_rays(manager& m, const generator0_array& y)
{
  a = ap_abstract0_add_ray_array(m.get_ap_manager_t(), true, a,
				 const_cast<ap_generator0_array_t*>(y.get_ap_generator0_array_t()));
  m.raise("apron::abstract0::add_rays(manager&, const generator0_array&)");
  return *this;
}

inline abstract0& add_rays(manager& m, abstract0& dst, const abstract0& x, const generator0_array& y)
{
  ap_abstract0_t* r =
    ap_abstract0_add_ray_array(m.get_ap_manager_t(), false, x.a,
			       const_cast<ap_generator0_array_t*>(y.get_ap_generator0_array_t()));
  m.raise("apron::add_rays(manager&, abstract0&, const abstract0&, const generator0_array&)",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  return dst;
}



inline abstract0& abstract0::operator*=(const abstract0& y)
{
  a = ap_abstract0_meet(a->man, true, a, y.a);
  manager::raise(a->man, "apron::abstract0::operator*=(const abstract0&)");
  return *this;
}

inline abstract0& abstract0::operator+=(const abstract0& y)
{
  a = ap_abstract0_join(a->man, true, a, y.a);
  manager::raise(a->man, "apron::abstract0::operator+=(const abstract0&)");
  return *this;
}

inline abstract0& abstract0::operator*=(const lincons0_array& y)
{
  a = ap_abstract0_meet_lincons_array(a->man, true, a,
				      const_cast<ap_lincons0_array_t*>(y.get_ap_lincons0_array_t()));
  manager::raise(a->man, "apron::abstract0::operator*=(const lincons0_array&)");
  return *this;
}

inline abstract0& abstract0::operator*=(const tcons0_array& y)
{
  a = ap_abstract0_meet_tcons_array(a->man, true, a,
				    const_cast<ap_tcons0_array_t*>(y.get_ap_tcons0_array_t()));
  manager::raise(a->man, "apron::abstract0::operator*=(const tcons0_array&)");
  return *this;
}

inline abstract0& abstract0::operator+=(const generator0_array& y)
{
  a = ap_abstract0_add_ray_array(a->man, true, a,
				 const_cast<ap_generator0_array_t*>(y.get_ap_generator0_array_t()));
  manager::raise(a->man, "apron::abstract0::operator+=(const generator0_array&)");
  return *this;
}


/* Assignments */
/* =========== */

inline abstract0& abstract0::assign(manager& m, ap_dim_t dim, const linexpr0& l, const abstract0& inter)
{
  const linexpr0* ll = &l;
  return assign(m, 1, &dim, &ll, inter);
}

inline abstract0& abstract0::assign(manager& m, size_t size, const ap_dim_t dim[], const linexpr0 * const l[], const abstract0& inter)
{
  a = ap_abstract0_assign_linexpr_array(m.get_ap_manager_t(), true, a,
					const_cast<ap_dim_t*>(dim),
					reinterpret_cast<ap_linexpr0_t**>(const_cast<linexpr0**>(l)),
					size, inter.a);
  m.raise("apron::abstract0::assign(manager&, size_t size, const ap_dim_t[], const linexpr0 * const [], const abstract0&)");
  return *this;
}

inline abstract0& abstract0::assign(manager& m, const std::vector<ap_dim_t>& dim, const std::vector<const linexpr0*>& l, const abstract0& inter)
{
  if (l.size()!=dim.size())
    throw std::invalid_argument("apron::abstract0::assign(manager&, const std::vector<ap_dim_t>&, const std::vector<const linexpr0*>&, const abstract0&) vectors have different size");
  const linexpr0* ll[l.size()];
  ap_dim_t dd[dim.size()];
  for (size_t i=0;i<l.size();i++) {
    ll[i] = l[i];
    dd[i] = dim[i];
  }
  return assign(m, l.size(), dd, ll, inter);
}

inline abstract0& assign(manager& m, abstract0& dst, const abstract0& src, ap_dim_t dim, const linexpr0& l, const abstract0& inter = abstract0::null)
{
  const linexpr0* ll = &l;
  return assign(m, dst, src, 1, &dim, &ll, inter);
}

inline abstract0& assign(manager& m, abstract0& dst, const abstract0& src, size_t size, const ap_dim_t dim[], const linexpr0 * const l[], const abstract0& inter = abstract0::null)
{
  ap_abstract0_t* r =
    ap_abstract0_assign_linexpr_array(m.get_ap_manager_t(), false, src.a,
				      const_cast<ap_dim_t*>(dim),
				      reinterpret_cast<ap_linexpr0_t**>(const_cast<linexpr0**>(l)),
				      size, inter.a);
  m.raise("apron::assign((manager&, abstract0&, const abstract0&, size_t size, const ap_dim_t[], const linexpr0 * const [], const abstract0&)",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  return dst;
}

inline abstract0& assign(manager& m, abstract0& dst, const abstract0& src, const std::vector<ap_dim_t>& dim, const std::vector<const linexpr0*>& l, const abstract0& inter = abstract0::null)
{
  if (l.size()!=dim.size())
    throw std::invalid_argument("apron::assign(manager&, abstract0, const abstract0&, const std::vector<ap_dim_t>&, const std::vector<const linexpr0*>&, const abstract0&) vectors have different size");
  const linexpr0* ll[l.size()];
  ap_dim_t dd[dim.size()];
  for (size_t i=0;i<l.size();i++) {
    ll[i] = l[i];
    dd[i] = dim[i];
  }
  return assign(m, dst, src, l.size(), dd, ll, inter);
}





inline abstract0& abstract0::assign(manager& m, ap_dim_t dim, const texpr0& l, const abstract0& inter)
{
  const texpr0* ll = &l;
  return assign(m, 1, &dim, &ll, inter);
}

inline abstract0& abstract0::assign(manager& m, size_t size, const ap_dim_t dim[], const texpr0 * const l[], const abstract0& inter)
{
  a = ap_abstract0_assign_texpr_array(m.get_ap_manager_t(), true, a,
				      const_cast<ap_dim_t*>(dim),
				      reinterpret_cast<ap_texpr0_t**>(const_cast<texpr0**>(l)),
				      size, inter.a);
  m.raise("apron::abstract0::assign(manager&, size_t size, const ap_dim_t[], const texpr0 * const [], const abstract0&)");
  return *this;
}

inline abstract0& abstract0::assign(manager& m, const std::vector<ap_dim_t>& dim, const std::vector<const texpr0*>& l, const abstract0& inter)
{
  if (l.size()!=dim.size())
    throw std::invalid_argument("apron::abstract0::assign(manager&, const std::vector<ap_dim_t>&, const std::vector<const texpr0*>&, const abstract0&) vectors have different size");
  const texpr0* ll[l.size()];
  ap_dim_t dd[dim.size()];
  for (size_t i=0;i<l.size();i++) {
    ll[i] = l[i];
    dd[i] = dim[i];
  }
  return assign(m, l.size(), dd, ll, inter);
}

inline abstract0& assign(manager& m, abstract0& dst, const abstract0& src, ap_dim_t dim, const texpr0& l, const abstract0& inter = abstract0::null)
{
  const texpr0* ll = &l;
  return assign(m, dst, src, 1, &dim, &ll, inter);
}

inline abstract0& assign(manager& m, abstract0& dst, const abstract0& src, size_t size, const ap_dim_t dim[], const texpr0 * const l[], const abstract0& inter = abstract0::null)
{
  ap_abstract0_t* r =
    ap_abstract0_assign_texpr_array(m.get_ap_manager_t(), false, src.a,
				      const_cast<ap_dim_t*>(dim),
				      reinterpret_cast<ap_texpr0_t**>(const_cast<texpr0**>(l)),
				      size, inter.a);
  m.raise("apron::assign((manager&, abstract0&, const abstract0&, size_t size, const ap_dim_t[], const texpr0 * const [], const abstract0&)",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  return dst;
}

inline abstract0& assign(manager& m, abstract0& dst, const abstract0& src, const std::vector<ap_dim_t>& dim, const std::vector<const texpr0*>& l, const abstract0& inter = abstract0::null)
{
  if (l.size()!=dim.size())
    throw std::invalid_argument("apron::assign(manager&, abstract0, const abstract0&, const std::vector<ap_dim_t>&, const std::vector<const texpr0*>&, const abstract0&) vectors have different size");
  const texpr0* ll[l.size()];
  ap_dim_t dd[dim.size()];
  for (size_t i=0;i<l.size();i++) {
    ll[i] = l[i];
    dd[i] = dim[i];
  }
  return assign(m, dst, src, l.size(), dd, ll, inter);
}



/* Substitutions */
/* =========== = */

inline abstract0& abstract0::substitute(manager& m, ap_dim_t dim, const linexpr0& l, const abstract0& inter)
{
  const linexpr0* ll = &l;
  return substitute(m, 1, &dim, &ll, inter);
}

inline abstract0& abstract0::substitute(manager& m, size_t size, const ap_dim_t dim[], const linexpr0 * const l[], const abstract0& inter)
{
  a = ap_abstract0_substitute_linexpr_array(m.get_ap_manager_t(), true, a,
					const_cast<ap_dim_t*>(dim),
					reinterpret_cast<ap_linexpr0_t**>(const_cast<linexpr0**>(l)),
					size, inter.a);
  m.raise("apron::abstract0::substitute(manager&, size_t size, const ap_dim_t[], const linexpr0 * const [], const abstract0&)");
  return *this;
}

inline abstract0& abstract0::substitute(manager& m, const std::vector<ap_dim_t>& dim, const std::vector<const linexpr0*>& l, const abstract0& inter)
{
  if (l.size()!=dim.size())
    throw std::invalid_argument("apron::abstract0::substitute(manager&, const std::vector<ap_dim_t>&, const std::vector<const linexpr0*>&, const abstract0&) vectors have different size");
  const linexpr0* ll[l.size()];
  ap_dim_t dd[dim.size()];
  for (size_t i=0;i<l.size();i++) {
    ll[i] = l[i];
    dd[i] = dim[i];
  }
  return substitute(m, l.size(), dd, ll, inter);
}

inline abstract0& substitute(manager& m, abstract0& dst, const abstract0& src, ap_dim_t dim, const linexpr0& l, const abstract0& inter = abstract0::null)
{
  const linexpr0* ll = &l;
  return substitute(m, dst, src, 1, &dim, &ll, inter);
}

inline abstract0& substitute(manager& m, abstract0& dst, const abstract0& src, size_t size, const ap_dim_t dim[], const linexpr0 * const l[], const abstract0& inter = abstract0::null)
{
  ap_abstract0_t* r =
    ap_abstract0_substitute_linexpr_array(m.get_ap_manager_t(), false, src.a,
				      const_cast<ap_dim_t*>(dim),
				      reinterpret_cast<ap_linexpr0_t**>(const_cast<linexpr0**>(l)),
				      size, inter.a);
  m.raise("apron::substitute((manager&, abstract0&, const abstract0&, size_t size, const ap_dim_t[], const linexpr0 *const [], const abstract0&)",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  return dst;
}

inline abstract0& substitute(manager& m, abstract0& dst, const abstract0& src, const std::vector<ap_dim_t>& dim, const std::vector<const linexpr0*>& l, const abstract0& inter = abstract0::null)
{
  if (l.size()!=dim.size())
    throw std::invalid_argument("apron::substitute(manager&, abstract0, const abstract0&, const std::vector<ap_dim_t>&, const std::vector<const linexpr0*>&, const abstract0&) vectors have different size");
  const linexpr0* ll[l.size()];
  ap_dim_t dd[dim.size()];
  for (size_t i=0;i<l.size();i++) {
    ll[i] = l[i];
    dd[i] = dim[i];
  }
  return substitute(m, dst, src, l.size(), dd, ll, inter);
}





inline abstract0& abstract0::substitute(manager& m, ap_dim_t dim, const texpr0& l, const abstract0& inter)
{
  const texpr0* ll = &l;
  return substitute(m, 1, &dim, &ll, inter);
}

inline abstract0& abstract0::substitute(manager& m, size_t size, const ap_dim_t dim[], const texpr0 * const l[], const abstract0& inter)
{
  a = ap_abstract0_substitute_texpr_array(m.get_ap_manager_t(), true, a,
					const_cast<ap_dim_t*>(dim),
					reinterpret_cast<ap_texpr0_t**>(const_cast<texpr0**>(l)),
					size, inter.a);
  m.raise("apron::abstract0::substitute(manager&, size_t size, const ap_dim_t[], const texpr0 * const [], const abstract0&)");
  return *this;
}

inline abstract0& abstract0::substitute(manager& m, const std::vector<ap_dim_t>& dim, const std::vector<const texpr0*>& l, const abstract0& inter)
{
  if (l.size()!=dim.size())
    throw std::invalid_argument("apron::abstract0::substitute(manager&, const std::vector<ap_dim_t>&, const std::vector<const texpr0*>&, const abstract0&) vectors have different size");
  const texpr0* ll[l.size()];
  ap_dim_t dd[dim.size()];
  for (size_t i=0;i<l.size();i++) {
    ll[i] = l[i];
    dd[i] = dim[i];
  }
  return substitute(m, l.size(), dd, ll, inter);
}

inline abstract0& substitute(manager& m, abstract0& dst, const abstract0& src, ap_dim_t dim, const texpr0& l, const abstract0& inter = abstract0::null)
{
  const texpr0* ll = &l;
  return substitute(m, dst, src, 1, &dim, &ll, inter);
}

inline abstract0& substitute(manager& m, abstract0& dst, const abstract0& src, size_t size, const ap_dim_t dim[], const texpr0 * const l[], const abstract0& inter = abstract0::null)
{
  ap_abstract0_t* r =
    ap_abstract0_substitute_texpr_array(m.get_ap_manager_t(), false, src.a,
				      const_cast<ap_dim_t*>(dim),
				      reinterpret_cast<ap_texpr0_t**>(const_cast<texpr0**>(l)),
				      size, inter.a);
  m.raise("apron::substitute((manager&, abstract0&, const abstract0&, size_t size, const ap_dim_t[], const texpr0 * const [], const abstract0&)",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  return dst;
}

inline abstract0& substitute(manager& m, abstract0& dst, const abstract0& src, const std::vector<ap_dim_t>& dim, const std::vector<const texpr0*>& l, const abstract0& inter = abstract0::null)
{
  if (l.size()!=dim.size())
    throw std::invalid_argument("apron::substitute(manager&, abstract0, const abstract0&, const std::vector<ap_dim_t>&, const std::vector<const texpr0*>&, const abstract0&) vectors have different size");
  const texpr0* ll[l.size()];
  ap_dim_t dd[dim.size()];
  for (size_t i=0;i<l.size();i++) {
    ll[i] = l[i];
    dd[i] = dim[i];
  }
  return substitute(m, dst, src, l.size(), dd, ll, inter);
}




/* Projection */
/* ========== */

inline abstract0& abstract0::forget(manager& m, ap_dim_t dim, bool project)
{
  return forget(m, 1, &dim, project);
}

inline abstract0& abstract0::forget(manager& m, size_t size, const ap_dim_t dim[], bool project)
{
  a = ap_abstract0_forget_array(m.get_ap_manager_t(), true, a,
				const_cast<ap_dim_t*>(dim),
				size, project);
  m.raise("apron::abstract0::substitute(orget(manager&, size_t size, const ap_dim_t[], bool)");
  return *this;
}

inline abstract0& abstract0::forget(manager& m, const std::vector<ap_dim_t> dim, bool project)
{
  ap_dim_t dd[dim.size()];
  for (size_t i=0;i<dim.size();i++) dd[i] = dim[i];
  return forget(m, dim.size(), dd, project);
}

inline abstract0& forget(manager& m, abstract0& dst, const abstract0& src, ap_dim_t dim, bool project = false)
{
  return forget(m, dst, src, 1, &dim, project);
}

inline abstract0& forget(manager& m, abstract0& dst, const abstract0& src, size_t size, const ap_dim_t dim[], bool project = false)
{
  ap_abstract0_t* r =
    ap_abstract0_forget_array(m.get_ap_manager_t(), false, src.a,
			      const_cast<ap_dim_t*>(dim),
			      size, project);
  m.raise("apron::forget(manager&, abstract0&, const abstract0&, size_t size, const ap_dim_t[], bool)",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  return dst;
}

inline abstract0& forget(manager& m, abstract0& dst, const abstract0& src, const std::vector<ap_dim_t> dim, bool project = false)
{
  ap_dim_t dd[dim.size()];
  for (size_t i=0;i<dim.size();i++) dd[i] = dim[i];
  return forget(m, dst, src, dim.size(), dd, project);
}


/* Change of dimension */
/* =================== */

inline abstract0& abstract0::add_dimensions(manager& m, const dimchange& d, bool project)
{
  a = ap_abstract0_add_dimensions(m.get_ap_manager_t(), true, a,
				  const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t()),
				  project);
  m.raise("apron::abstract0::add_dimensions(manager&, const dimchange&, bool)");
  return *this;
}


inline abstract0& abstract0::remove_dimensions(manager& m, const dimchange& d)
{
  a = ap_abstract0_remove_dimensions(m.get_ap_manager_t(), true, a,
				     const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t()));
  m.raise("apron::abstract0::remove_dimensions(manager&, const dimchange&)");
  return *this;
}

inline abstract0& abstract0::permute_dimensions(manager& m, const dimperm& d)
{
  a = ap_abstract0_permute_dimensions(m.get_ap_manager_t(), true, a,
				      const_cast<ap_dimperm_t*>(d.get_ap_dimperm_t()));
  m.raise("apron::abstract0::permute_dimensions(manager&, const dimperm&)");
  return *this;
}

inline abstract0& add_dimensions(manager& m, abstract0& dst, const abstract0& src, const dimchange& d, bool project = false)
{
  ap_abstract0_t* r =
    ap_abstract0_add_dimensions(m.get_ap_manager_t(), false, src.a,
				const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t()),
				project);
  m.raise("apron::add_dimensions(manager&, abstract0&, const abstract0&, const dimchange&, bool)",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  return dst;
}

inline abstract0& remove_dimensions(manager& m, abstract0& dst, const abstract0& src, const dimchange& d)
{
  ap_abstract0_t* r =
    ap_abstract0_remove_dimensions(m.get_ap_manager_t(), false, src.a,
				   const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t()));
  m.raise("apron::remove_dimensions(manager&, abstract0&, const abstract0&, const dimchange&)",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  return dst;
}

inline abstract0& permute_dimensions(manager& m, abstract0& dst, const abstract0& src, const dimperm& d)
{
  ap_abstract0_t* r =
    ap_abstract0_permute_dimensions(m.get_ap_manager_t(), false, src.a,
				    const_cast<ap_dimperm_t*>(d.get_ap_dimperm_t()));
  m.raise("apron::permute_dimensions(manager&, abstract0&, const abstract0&, const dimperm&)",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  return dst;
}



/* Expansion and folding */
/* ===================== */

inline abstract0& abstract0::expand(manager& m, ap_dim_t dim, size_t n)
{
  a = ap_abstract0_expand(m.get_ap_manager_t(), true, a, dim, n);
  m.raise("apron::abstract0::expand(manager&, ap_dim_t, size_t)");
  return *this;
}

inline abstract0& expand(manager& m, abstract0& dst, const abstract0& src, ap_dim_t dim, size_t n = 1)
{
  ap_abstract0_t* r = ap_abstract0_expand(m.get_ap_manager_t(), false, src.a, dim, n);
  m.raise("apron::expand(manager&, abstract0&, const abstract0&, ap_dim_t, size_t)",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  return dst;
}

inline abstract0& abstract0::fold(manager& m, size_t size, const ap_dim_t dim[])
{
  a = ap_abstract0_fold(m.get_ap_manager_t(), true, a, const_cast<ap_dim_t*>(dim), size);
  m.raise("apron::abstract0::fold(manager&, size_t size, const ap_dim_t[])");
  return *this;
}

inline abstract0& abstract0::fold(manager& m, const std::vector<ap_dim_t>& dim)
{
  ap_dim_t dd[dim.size()];
  for (size_t i=0;i<dim.size();i++) dd[i] = dim[i];
  return fold(m, dim.size(), dd);
}

inline abstract0& fold(manager& m, abstract0& dst, const abstract0& src, size_t size, const ap_dim_t dim[])
{
  ap_abstract0_t* r =
    ap_abstract0_fold(m.get_ap_manager_t(), false, src.a, const_cast<ap_dim_t*>(dim), size);
  m.raise("apron::fold(manager&, abstract0&, const abstract0&, size_t size, const ap_dim_t[])",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  return dst;
}

inline abstract0& fold(manager& m, abstract0& dst, const abstract0& src, const std::vector<ap_dim_t>& dim)
{
  ap_dim_t dd[dim.size()];
  for (size_t i=0;i<dim.size();i++) dd[i] = dim[i];
  return fold(m, dst, src, dim.size(), dd);
}


/* Widenings */
/* ========= */

inline abstract0& widening(manager& m, abstract0& dst, const abstract0& x, const abstract0& y)
{
  ap_abstract0_t* r = ap_abstract0_widening(m.get_ap_manager_t(), x.a, y.a);
  m.raise("apron::wideningg(manager&, abstract0&, const abstract0&, const abstract0&)",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  return dst;
}

inline abstract0& widening(manager& m, abstract0& dst, const abstract0& x, const abstract0& y, const lincons0_array& l)
{
  ap_abstract0_t* r =
    ap_abstract0_widening_threshold(m.get_ap_manager_t(), x.a, y.a,
				    const_cast<ap_lincons0_array_t*>(l.get_ap_lincons0_array_t()));
  m.raise("apron::widening(manager&, abstract0&, const abstract0&, const abstract0&, const lincons0_array&)",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  return dst;
}


/* Closure */
/* ======= */

inline abstract0& abstract0::closure(manager& m)
{
  a = ap_abstract0_closure(m.get_ap_manager_t(), true, a);
  m.raise("apron::abstract0::closured(manager&)");
  return *this;
}

inline abstract0& closure(manager& m, abstract0& dst, const abstract0& src)
{
  ap_abstract0_t* r = ap_abstract0_closure(m.get_ap_manager_t(), false, src.a);
  m.raise("apron::closure(manager&, abstract0&, const abstract0&)",r);
  ap_abstract0_free(m.get_ap_manager_t(), dst.a);
  dst.a = r;
  return dst;
}



/* C-level compatibility */
/* ===================== */

inline ap_abstract0_t* abstract0::get_ap_abstract0_t()
{
  return a;
}

inline const ap_abstract0_t* abstract0::get_ap_abstract0_t() const
{
  return a;
}
