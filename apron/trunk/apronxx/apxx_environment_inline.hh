/* -*- C++ -*-
 * apxx_environment_inline.hh
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


// shallow copy (does not call ap_var_operations->copy)
static inline ap_var_t* apxx_environment_get_names(const std::vector<var>& n)
{
  ap_var_t* r = (ap_var_t*)malloc(sizeof(ap_var_t)*n.size());
  assert(r);
  for (size_t i=0; i<n.size(); i++) r[i] = n[i].get_ap_var_t();
  return r;
}

// shallow copy (does not call ap_var_operations->copy)
static inline void apxx_environment_get_names_pair(const std::vector<std::pair<var,var> >& n, ap_var_t** x, ap_var_t** y)
{
  *x = (ap_var_t*)malloc(sizeof(ap_var_t)*n.size());
  *y = (ap_var_t*)malloc(sizeof(ap_var_t)*n.size());
  assert(x && y);
  for (size_t i=0; i<n.size(); i++)  {
    (*x)[i] = n[i].first.get_ap_var_t();
    (*y)[i] = n[i].second.get_ap_var_t();
  }
}


/* constructors */
/* ============ */

inline environment::environment() 
  : e(ap_environment_alloc_empty())
{
}

inline environment::environment(const std::vector<var>& intdim, const std::vector<var>& realdim)
{
  ap_var_t *i = apxx_environment_get_names(intdim);
  ap_var_t *r = apxx_environment_get_names(realdim);
  e = ap_environment_alloc(i, intdim.size(), r, realdim.size());
  free(i);  free(r);
  if (!e) throw std::invalid_argument("apron::environment::environment(const vector<var>&, const vector<var>&) incompatible variable types");
}

inline environment::environment(const var* intdim, size_t intdim_size, const var* realdim, size_t realdim_size)
{
  e = ap_environment_alloc(const_cast<ap_var_t*>(reinterpret_cast<const ap_var_t*>(intdim)), intdim_size, 
			   const_cast<ap_var_t*>(reinterpret_cast<const ap_var_t*>(realdim)), realdim_size);
  if (!e) throw std::invalid_argument("apron::environment::environment(const var*, size_t, const var*, size_t) incompatible variable types");
}

inline environment::environment(const environment& x) 
  : e(ap_environment_copy(x.e))
{
}

inline environment::~environment()
{
  ap_environment_free(e);
}


/* assignment */
/* ========== */

inline const environment& environment::operator=(const environment& x)
{
  ap_environment_t* ee = ap_environment_copy(x.e);
  ap_environment_free(e);
  e = ee;
  return *this;
}


/* operations */
/* ========== */


inline environment environment::add(const std::vector<var>& intdim, const std::vector<var>& realdim) const
{
  ap_var_t *i = apxx_environment_get_names(intdim);
  ap_var_t *r = apxx_environment_get_names(realdim);
  ap_environment_t* ee = ap_environment_add(e, i, intdim.size(), r, realdim.size());
  free(i); free(r);
  if (!ee) throw std::invalid_argument("apron::environment::add(const vector<var>&, const vector<var>&) incompatible variable types");
  return ee;
}

inline environment environment::add(const var* intdim, size_t intdim_size, const var* realdim, size_t realdim_size) const
{
  ap_environment_t* ee =
    ap_environment_add(e,
		       const_cast<ap_var_t*>(reinterpret_cast<const ap_var_t*>(intdim)), intdim_size, 
		       const_cast<ap_var_t*>(reinterpret_cast<const ap_var_t*>(realdim)), realdim_size);
  if (!ee) throw std::invalid_argument("apron::environment::add(const var*, size_t, const var*, size_t) incompatible variable types");
  return ee;
}

inline environment environment::add(const std::vector<var>& intdim, const std::vector<var>& realdim, dimperm& perm) const
{
  ap_var_t *i = apxx_environment_get_names(intdim);
  ap_var_t *r = apxx_environment_get_names(realdim);
  ap_dimperm_t p;
  ap_environment_t* ee = ap_environment_add_perm(e, i, intdim.size(), r, realdim.size(), &p);
  free(i); free(r);
  if (!ee) throw std::invalid_argument("apron::environment::add(const vector<var>&, const vector<var>&, dimperm& perm) incompatible variable types");
  ap_dimperm_clear(perm.get_ap_dimperm_t());
  *perm.get_ap_dimperm_t() = p;
  return ee;
}

inline environment environment::add(const var* intdim, size_t intdim_size, const var* realdim, size_t realdim_size, dimperm& perm) const
{
  ap_dimperm_t p;
  ap_environment_t* ee = 
    ap_environment_add_perm(e,
			    const_cast<ap_var_t*>(reinterpret_cast<const ap_var_t*>(intdim)), intdim_size, 
			    const_cast<ap_var_t*>(reinterpret_cast<const ap_var_t*>(realdim)), realdim_size,
			    &p);
  if (!ee) throw std::invalid_argument("apron::environment::add(const var*, size_t, const var*, size_t, dimperm&) incompatible variable types");
  ap_dimperm_clear(perm.get_ap_dimperm_t());
  *perm.get_ap_dimperm_t() = p;
  return ee;
}

inline environment environment::remove(const std::vector<var>& dims) const
{
  ap_var_t *d = apxx_environment_get_names(dims);
  ap_environment_t* ee = ap_environment_remove(e, d, dims.size());
  free(d);
  if (!ee) throw std::invalid_argument("apron::environment::remove(const vector<var>&) variable not in environment");
  return ee;
}

inline environment environment::remove(const var* dims, size_t dims_size) const
{
  ap_environment_t* ee = 
    ap_environment_remove(e,
			  const_cast<ap_var_t*>(reinterpret_cast<const ap_var_t*>(dims)), 
			  dims_size);
  if (!ee) throw std::invalid_argument("apron::environment::remove(const var*, size_t) variable not in environment");
  return ee;
}

inline environment environment::rename(const std::vector<std::pair<var,var> >& dims) const
{
  ap_var_t *a, *b;
  ap_dimperm_t p;
  apxx_environment_get_names_pair(dims, &a, &b);
  ap_environment_t* ee = ap_environment_rename(e, a, b, dims.size(), &p);
  free(a); free(b);
  if (!ee) throw std::invalid_argument("apron::environment::rename(const vector<std::pair<var,var>>&) variable mismatch");
  ap_dimperm_clear(&p);
  return ee;
}

inline environment environment::rename(const var* before, const var* after, size_t dims_size) const
{
  ap_dimperm_t p;
  ap_environment_t* ee = 
    ap_environment_rename(e, 
			  const_cast<ap_var_t*>(reinterpret_cast<const ap_var_t*>(before)),
			  const_cast<ap_var_t*>(reinterpret_cast<const ap_var_t*>(after)), 
			  dims_size, &p);
  if (!ee) throw std::invalid_argument("apron::environment::rename(const var*, const var*, size_t) variable mismatch");
  ap_dimperm_clear(&p);  
  return ee;
}

inline environment environment::rename(const std::vector<std::pair<var,var> >& dims, dimperm& perm) const
{
  ap_var_t *a, *b;
  ap_dimperm_t p;
  apxx_environment_get_names_pair(dims, &a, &b);
  ap_dimperm_clear(perm.get_ap_dimperm_t());
  ap_environment_t* ee = ap_environment_rename(e, a, b, dims.size(), &p);
  free(a); free(b);
  if (!ee) throw std::invalid_argument("apron::environment::rename(const vector<std::pair<var,var>>&, dimperm&) variable mismatch");
  ap_dimperm_clear(perm.get_ap_dimperm_t());
  *perm.get_ap_dimperm_t() = p;
  return ee;
}

inline environment environment::rename(const var* before, const var* after, size_t dims_size, dimperm& perm) const
{
  ap_dimperm_t p;
  ap_environment_t* ee = 
    ap_environment_rename(e, 
			  const_cast<ap_var_t*>(reinterpret_cast<const ap_var_t*>(before)),
			  const_cast<ap_var_t*>(reinterpret_cast<const ap_var_t*>(after)), 
			  dims_size, &p);
  if (!ee) throw std::invalid_argument("apron::environment::rename(const var*, const var*, size_t, dimperm&) variable mismatch");
  ap_dimperm_clear(perm.get_ap_dimperm_t());
  *perm.get_ap_dimperm_t() = p;
  return ee;
}


inline environment lce(const environment& x, const environment& y)
{
  ap_dimchange_t *a = NULL, *b = NULL;
  ap_environment_t* ee = ap_environment_lce(x.e, y.e, &a, &b);
  if (a) ap_dimchange_free(a); 
  if (b) ap_dimchange_free(b);
  if (!ee) throw std::invalid_argument("apron::environment::lce(const environment&, const environment&) incompatible variable types");
  return ee;
}

inline environment lce(const environment& x, const environment& y, dimchange& chgx, dimchange& chgy)
{
  ap_dimchange_t *a = NULL, *b = NULL;
  ap_environment_t* ee = ap_environment_lce(x.e, y.e, &a, &b);
  ap_dimchange_clear(chgx.get_ap_dimchange_t());
  ap_dimchange_clear(chgy.get_ap_dimchange_t());
  if (a) { *chgx.get_ap_dimchange_t() = *a; free(a); }
  else   { ap_dimchange_init(chgx.get_ap_dimchange_t(), 0, 0); }
  if (b) { *chgy.get_ap_dimchange_t() = *b; free(b); }
  else   { ap_dimchange_init(chgy.get_ap_dimchange_t(), 0, 0); }
  if (!ee) throw std::invalid_argument("apron::environment::lce(const environment&, const environment&, dimchange&, dimchange&) incompatible variable types");
  return ee;
}

inline environment lce(const std::vector<environment>& x)
{
  ap_environment_t** ar = (ap_environment_t**)malloc(sizeof(ap_environment_t*)*x.size());
  ap_dimchange_t** r = NULL;
  assert(ar);
  for (size_t i=0; i<x.size(); i++) ar[i] = x[i].e;
  ap_environment_t* ee = ap_environment_lce_array(ar, x.size(), &r);
  free(ar);
  if (r) {
    for (size_t i=0; i<x.size(); i++)
      if (r[i]) ap_dimchange_free(r[i]);
    free(r);
  }
  if (!ee) throw std::invalid_argument("apron::environment::lce(const vector<environment>&) incompatible variable types");
  return ee;
}

inline environment lce(const environment* env, size_t env_size)
{
  ap_dimchange_t** r = NULL;
  ap_environment_t* ee = 
    ap_environment_lce_array(reinterpret_cast<ap_environment_t**>(const_cast<environment*>(env)), 
			     env_size, &r);
  if (r) {
    for (size_t i=0; i<env_size; i++)
      if (r[i]) ap_dimchange_free(r[i]);
    free(r);
  }
  if (!ee) throw std::invalid_argument("apron::environment::lce(const environment*, size_t) incompatible variable types");
  return ee;
}

inline environment lce(const std::vector<environment>& x, std::vector<dimchange>& chg)
{
  if (x.size()>chg.size()) throw std::invalid_argument("apron::environment::lce(const vector<environment>&, vector<dimchange>&) dimchange vector too short");
  ap_environment_t** ar = (ap_environment_t**)malloc(sizeof(ap_environment_t*)*x.size());
  ap_dimchange_t** r = NULL;
  assert(ar);
  for (size_t i=0; i<x.size(); i++) ar[i] = x[i].e;
  ap_environment_t* ee = ap_environment_lce_array(ar, x.size(), &r);
  free(ar);
  for (size_t i=0; i<x.size(); i++) {
    ap_dimchange_clear(chg[i].get_ap_dimchange_t());
    if (r && r[i]) { 
      *chg[i].get_ap_dimchange_t() = *(r[i]);
      free(r[i]);
    }
    else {
      ap_dimchange_init(chg[i].get_ap_dimchange_t(), 0, 0);
    }
  }
  if (r) free(r);
  if (!ee) throw std::invalid_argument("apron::environment::lce(const vector<environment>&, vector<dimchange>&) incompatible variable types");
  return ee;
}

inline environment lce(const environment* env, dimchange* chg, size_t env_size)
{
  ap_dimchange_t** r = NULL;
  ap_environment_t* ee =
    ap_environment_lce_array(reinterpret_cast<ap_environment_t**>(const_cast<environment*>(env)), 
			     env_size, &r);
  for (size_t i=0; i<env_size; i++) {
    ap_dimchange_clear(chg[i].get_ap_dimchange_t());
    if (r && r[i]) { 
      *chg[i].get_ap_dimchange_t() = *(r[i]);
      free(r[i]);
    }
    else {
      ap_dimchange_init(chg[i].get_ap_dimchange_t(), 0, 0);
    }
  }
  if (r) free(r);
  if (!ee) throw std::invalid_argument("apron::environment::lce(const environment*, dimchange*, size_t) incompatible variable types");
  return ee;
}

inline dimchange get_dimchange(const environment& x, const environment& y)
{
  ap_dimchange_t* d = ap_environment_dimchange(x.e,y.e);
  if (!d) throw std::invalid_argument("apron::environment::get_dimchange(const environment&, const environment &) not a super-type");
  dimchange c;
  *c.get_ap_dimchange_t() = *d;
  free(d);
  return c;
}


/* access */
/* ====== */

inline size_t environment::intdim() const
{
  return e->intdim;
}

inline size_t environment::realdim() const
{
  return e->realdim;
}

inline bool environment::contains(const var& x) const
{
  return ap_environment_mem_var(e, x.get_ap_var_t());
}

inline ap_dim_t environment::operator[] (const var& x) const
{
  ap_dim_t d = ap_environment_dim_of_var(e, x.get_ap_var_t());
  if (d==AP_DIM_MAX) throw std::invalid_argument("apron::environment::operator[](const var&) variable not in environment");
  return d;
}

inline const var& environment::operator[] (ap_dim_t d) const
{
  return *reinterpret_cast<var*>(&e->var_of_dim[d]);
}

inline ap_dim_t environment::get_dim(const var& x) const
{
  ap_dim_t d = ap_environment_dim_of_var(e, x.get_ap_var_t());
  if (d==AP_DIM_MAX) throw std::invalid_argument("apron::environment::get_dim(const var&) variable not in environment");
  return d;
}

inline const var& environment::get_var(ap_dim_t d) const
{
  if (d >= e->intdim+e->realdim) throw std::out_of_range("environment::get_var");
  return *reinterpret_cast<var*>(&e->var_of_dim[d]);
}

inline std::vector<var> environment::get_vars() const
{
  std::vector<var> v = std::vector<var>(e->intdim+e->realdim, "");
  for (size_t i=0; i<e->intdim+e->realdim; i++) v[i] = e->var_of_dim[i];
  return v;
}


/* Tests */
/* ===== */

inline bool operator==(const environment& x, const environment& y)
{
  return ap_environment_is_eq(x.e, y.e);
}

inline bool operator!=(const environment& x, const environment& y)
{
 return !ap_environment_is_eq(x.e, y.e);
}

inline bool operator<=(const environment& x, const environment& y)
{
 return ap_environment_is_leq(x.e, y.e);
}

inline bool operator>=(const environment& x, const environment& y)
{
 return ap_environment_is_leq(y.e, x.e);
}

inline int cmp(const environment& x, const environment& y)
{
  return ap_environment_compare(x.e, y.e);
}


/* print */

inline std::ostream& operator<< (std::ostream& os, const environment& s)
{
  for (size_t i=0; i<s.e->intdim+s.e->realdim; i++) {
    char* c = ap_var_operations->to_string(s.e->var_of_dim[i]);
    os << i << ": " << c << (i<s.e->intdim ? " (int)" : " (real)") << std::endl;
    free(c);
  }
  return os;
}

inline void environment::print(FILE* stream) const
{
  ap_environment_fdump(stream, e);
}


/* C API compatibility */
/* =================== */

inline const ap_environment_t* environment::get_ap_environment_t() const
{
  return e;
}

inline ap_environment_t*environment:: get_ap_environment_t()
{
  return e;
}

