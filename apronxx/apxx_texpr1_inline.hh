/* -*- C++ -*-
 * apxx_texpr1_inline.hh
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

inline void texpr1::init_from(ap_environment_t* e, ap_texpr0_t* x)
{
  l.env = e;
  l.texpr0 = x;
}

inline void texpr1::init_from(ap_texpr1_t* x)
{
  l = *x;
  free(x);
}

inline texpr1::texpr1(ap_texpr1_t* x)
{
  init_from(x);
}

inline texpr1::texpr1(const texpr1& x)
{
  init_from(ap_texpr1_copy(const_cast<ap_texpr1_t*>(&x.l)));
}

inline texpr1::texpr1(const builder& x)
{
  init_from(ap_texpr1_copy(const_cast<ap_texpr1_t*>(x.get_ap_texpr1_t())));
}

inline texpr1::texpr1(const const_iterator& x)
{
  init_from(ap_texpr1_copy(const_cast<ap_texpr1_t*>(x.get_ap_texpr1_t())));
}

inline texpr1::texpr1(const environment& e, const var& v)
{
  ap_texpr1_t* ll =
    ap_texpr1_var(const_cast<ap_environment_t*>(e.get_ap_environment_t()), v.get_ap_var_t());
  if (!ll) throw(std::invalid_argument("apron::texpr1::texpr1(const environment&, const var&) variable not in environment"));
  init_from(ll);
}

inline texpr1::texpr1(const environment& e, const texpr0& x)
{
  init_from(ap_environment_copy(const_cast<ap_environment_t*>(e.get_ap_environment_t())),
	    ap_texpr0_copy(const_cast<ap_texpr0_t*>(x.get_ap_texpr0_t())));
}

inline texpr1::texpr1(const environment& e, const texpr0::const_iterator& x)
{
  init_from(ap_environment_copy(const_cast<ap_environment_t*>(e.get_ap_environment_t())),
	    ap_texpr0_copy(const_cast<ap_texpr0_t*>(x.get_ap_texpr0_t())));
}

inline texpr1::texpr1(const environment& e, const texpr0::builder& c)
{
  init_from(ap_environment_copy(const_cast<ap_environment_t*>(e.get_ap_environment_t())),
	    ap_texpr0_copy(const_cast<ap_texpr0_t*>(c.get_ap_texpr0_t())));
}


inline texpr1::texpr1(const linexpr1& l)
{
  init_from(ap_texpr1_from_linexpr1(const_cast<ap_linexpr1_t*>(l.get_ap_linexpr1_t())));
}

inline texpr1::texpr1(const texpr1& x, const environment& e)
{
  ap_texpr1_t* ll =
    ap_texpr1_extend_environment(const_cast<ap_texpr1_t*>(&x.l),
				 const_cast<ap_environment_t*>(e.get_ap_environment_t()));
  if (!ll)
    throw std::invalid_argument("apron::texpr1::texpr1(const texpr1&, const environment&) not a super-environment");
  init_from(ll);
}

inline texpr1::texpr1(const texpr1& x, const var& src, const texpr1& dst)
{
  ap_texpr1_t* ll =
    ap_texpr1_substitute(const_cast<ap_texpr1_t*>(&x.l),
			 const_cast<ap_var_t>(src.get_ap_var_t()),
			 const_cast<ap_texpr1_t*>(&dst.l));
  if (!ll)
    throw std::invalid_argument("apron::texpr1::texpr1(const texpr1&, const var&, const texpr1&)");
  init_from(ll);
}


/* destructor */
/* ========== */

inline texpr1::~texpr1()
{
  ap_environment_free(l.env);
  ap_texpr0_free(l.texpr0);
}


/* assignment */
/* ========== */

inline texpr1& texpr1::operator=(const texpr1& x)
{
  ap_texpr1_t* c = ap_texpr1_copy(const_cast<ap_texpr1_t*>(&x.l));
  ap_environment_free(l.env);
  ap_texpr0_free(l.texpr0);
  init_from(c);
  return *this;
}

inline texpr1& texpr1::operator=(const builder& x)
{
  ap_texpr1_t* c = ap_texpr1_copy(const_cast<ap_texpr1_t*>(x.get_ap_texpr1_t()));
  ap_environment_free(l.env);
  ap_texpr0_free(l.texpr0);
  init_from(c);
  return *this;
}

inline texpr1& texpr1::operator=(const const_iterator& x)
{
  ap_texpr1_t* c = ap_texpr1_copy(const_cast<ap_texpr1_t*>(x.get_ap_texpr1_t()));
  ap_environment_free(l.env);
  ap_texpr0_free(l.texpr0);
  init_from(c);
  return *this;
}

inline texpr1& texpr1::operator= (const linexpr1& x)
{
  ap_environment_free(l.env);
  ap_texpr0_free(l.texpr0);
  init_from(ap_texpr1_from_linexpr1(const_cast<ap_linexpr1_t*>(x.get_ap_linexpr1_t())));
  return *this;
}



/* print */
/* ===== */

extern void apxx_texpr0_env_ostream(std::ostream& os, ap_environment_t* env, ap_texpr0_t* a);

inline std::ostream& operator<< (std::ostream& os, const texpr1& s)
{
  return os << s.root();
}

inline void texpr1::print(FILE* stream) const
{
  root().print(stream);
}



/* dimension operations */
/* ==================== */

inline void texpr1::extend_environment(const environment& e)
{
  bool r =
    ap_texpr1_extend_environment_with(&l,
				      const_cast<ap_environment_t*>(e.get_ap_environment_t()));
  if (r)
    throw std::invalid_argument("apron::texpr1::extend_environment(const environment&) not a super-environment");
}


/* access */
/* ====== */

inline environment texpr1::get_environment() const
{
  return root().get_environment();
}

inline texpr0::const_iterator texpr1::get_texpr0() const
{
  return root().get_texpr0();
}

inline texpr0::iterator texpr1::get_texpr0()
{
  return root().get_texpr0();
}

inline bool texpr1::equal(const texpr1& x) const
{
  return root().equal(x);
}

inline size_t texpr1::depth() const
{
  return root().depth();
}

inline bool texpr1::is_zero() const
{
  return root().is_zero();
}

inline size_t texpr1::size() const
{
  return root().size();
}

inline bool texpr1::has_var(const var& v) const
{
  return root().has_var(v);
}

inline bool texpr1::is_interval_cst() const
{
  return root().is_interval_cst();
}

inline bool texpr1::is_interval_linear() const
{
  return root().is_interval_linear();
}

inline bool texpr1::is_interval_polynomial() const
{
  return root().is_interval_polynomial();
}

inline bool texpr1::is_interval_polyfrac() const
{
  return root().is_interval_polyfrac();
}

inline bool texpr1::is_scalar() const
{
  return root().is_scalar();
}


/* operations */
/* ========== */

inline void texpr1::substitute(const var& src, const texpr1& dst)
{
  bool r =
    ap_texpr1_substitute_with(const_cast<ap_texpr1_t*>(&l),
			      const_cast<ap_var_t>(src.get_ap_var_t()),
			      const_cast<ap_texpr1_t*>(&dst.l));
  if (r)
    throw std::invalid_argument("apron::texpr1::substitute(const var&, const texpr1&)");
}


/* C-level compatibility */
/* ===================== */


inline ap_texpr1_t* texpr1::get_ap_texpr1_t()
{
  return &l;
}

inline const ap_texpr1_t* texpr1::get_ap_texpr1_t() const
{
  return &l;
}






/* ========= */
/* iterators */
/* ========= */



/* constructors */
/* ============ */

inline texpr1::const_iterator texpr1::root() const
{
  return const_iterator(const_cast<ap_texpr1_t*>(&l));
}

inline texpr1::const_iterator::const_iterator(ap_texpr0_t* ll, ap_environment_t* ee)
{
  l.texpr0 = ll;
  l.env = ee;
}

inline texpr1::const_iterator::const_iterator(ap_texpr1_t* l)
  : l(*l)
{}

inline texpr1::const_iterator::const_iterator(const texpr1& e)
  : l(*e.get_ap_texpr1_t())
{}

inline texpr1::const_iterator::const_iterator(const const_iterator& i)
  : l(i.l)
{}


/* access */
/* ====== */

inline environment texpr1::const_iterator::get_environment() const
{
  return ap_environment_copy(l.env);
}

inline texpr0::const_iterator texpr1::const_iterator::get_texpr0() const
{
  return l.texpr0;
}

inline ap_texpr_discr_t texpr1::const_iterator::get_discr() const
{
  return l.texpr0->discr;
}

inline const coeff& texpr1::const_iterator::get_coeff() const
{
  if (l.texpr0->discr!=AP_TEXPR_CST) throw(bad_discriminant("apron::texpr1::const_iterator::get_coeff()"));
  return reinterpret_cast<coeff&>(l.texpr0->val.cst);
}

inline const var& texpr1::const_iterator::get_var() const
{
  if (l.texpr0->discr!=AP_TEXPR_DIM) throw(bad_discriminant("apron::texpr1::const_iterator::get_var()"));
  return *reinterpret_cast<var*>(&l.env->var_of_dim[l.texpr0->val.dim]);
}

inline ap_dim_t texpr1::const_iterator::get_dim() const
{
  if (l.texpr0->discr!=AP_TEXPR_DIM) throw(bad_discriminant("apron::texpr1::const_iterator::get_dim()"));
  return l.texpr0->val.dim;
}

inline ap_texpr_op_t texpr1::const_iterator::get_op() const
{
  if (l.texpr0->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr1::const_iterator::get_op()"));
  return l.texpr0->val.node->op;
}

inline ap_texpr_rtype_t texpr1::const_iterator::get_rtype() const
{
  if (l.texpr0->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr1::const_iterator::get_type()"));
  return l.texpr0->val.node->type;
}

inline ap_texpr_rdir_t texpr1::const_iterator::get_rdir() const
{
  if (l.texpr0->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr1::const_iterator::get_dir()"));
  return l.texpr0->val.node->dir;
}


/* traversal */
/* ========= */

inline texpr1::const_iterator& texpr1::const_iterator::operator=(const const_iterator& i)
{
  l = i.l;
  return *this;
}

inline texpr1::const_iterator texpr1::const_iterator::child() const
{
  if (l.texpr0->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr1::const_iterator::child()"));
  if (!l.texpr0->val.node->exprA) throw(std::out_of_range("apron::texpr1::const_iterator::child()"));
  return const_iterator(l.texpr0->val.node->exprA, l.env);
}

inline texpr1::const_iterator texpr1::const_iterator::left() const
{
  if (l.texpr0->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr1::const_iterator::left()"));
  if (!l.texpr0->val.node->exprA) throw(std::out_of_range("apron::texpr1::const_iterator::left()"));
  return const_iterator(l.texpr0->val.node->exprA, l.env);
}

inline texpr1::const_iterator texpr1::const_iterator::right() const
{
  if (l.texpr0->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr1::const_iterator::right()"));
  if (!l.texpr0->val.node->exprB) throw(std::out_of_range("apron::texpr1::const_iterator::right()"));
  return const_iterator(l.texpr0->val.node->exprB, l.env);
}




/* print */
/* ===== */

inline std::ostream& operator<< (std::ostream& os, const texpr1::const_iterator& s)
{
  ap_texpr1_t* x = const_cast<ap_texpr1_t*>(s.get_ap_texpr1_t());
  apxx_texpr0_env_ostream(os, x->env, x->texpr0);
  return os;
}

inline void texpr1::const_iterator::print(FILE* stream) const
{
  ap_texpr1_fprint(stream, const_cast<ap_texpr1_t*>(&l));
}


/* tests, size */
/* =========== */

inline bool texpr1::const_iterator::equal(const texpr1& x) const
{
  return ap_texpr1_equal(const_cast<ap_texpr1_t*>(&l),
			 const_cast<ap_texpr1_t*>(x.get_ap_texpr1_t()));
}

inline size_t texpr1::const_iterator::depth() const
{
  return ap_texpr0_depth(const_cast<ap_texpr0_t*>(l.texpr0));
}

inline bool texpr1::const_iterator::is_zero() const
{
  return l.texpr0->discr==AP_TEXPR_CST && ap_coeff_zero(&const_cast<ap_texpr0_t*>(l.texpr0)->val.cst);
}

inline size_t texpr1::const_iterator::size() const
{
  return ap_texpr0_size(const_cast<ap_texpr0_t*>(l.texpr0));
}

inline bool texpr1::const_iterator::has_var(const var& v) const
{
  return ap_texpr1_has_var(const_cast<ap_texpr1_t*>(&l),
			   const_cast<ap_var_t>(v.get_ap_var_t()));
}

inline bool texpr1::const_iterator::is_interval_cst() const
{
  return ap_texpr1_is_interval_cst(const_cast<ap_texpr1_t*>(&l));
}

inline bool texpr1::const_iterator::is_interval_linear() const
{
  return ap_texpr1_is_interval_linear(const_cast<ap_texpr1_t*>(&l));
}

inline bool texpr1::const_iterator::is_interval_polynomial() const
{
  return ap_texpr1_is_interval_polynomial(const_cast<ap_texpr1_t*>(&l));
}

inline bool texpr1::const_iterator::is_interval_polyfrac() const
{
  return ap_texpr1_is_interval_polyfrac(const_cast<ap_texpr1_t*>(&l));
}

inline bool texpr1::const_iterator::is_scalar() const
{
  return ap_texpr1_is_scalar(const_cast<ap_texpr1_t*>(&l));
}


/* C-level compatibility */
/* ===================== */

inline ap_texpr1_t* texpr1::const_iterator::get_ap_texpr1_t()
{
  return &l;
}

inline const ap_texpr1_t* texpr1::const_iterator::get_ap_texpr1_t() const
{
  return &l;
}


/* constructors */
/* ============ */

inline texpr1::iterator texpr1::root()
{
  return iterator(&l);
}

inline texpr1::iterator::iterator(ap_texpr0_t* l, ap_environment_t* e)
  : texpr1::const_iterator(l,e)
{}

inline texpr1::iterator::iterator(ap_texpr1_t* l)
  : texpr1::const_iterator(l)
{}


inline texpr1::iterator::iterator(texpr1& e)
  : texpr1::const_iterator(e.get_ap_texpr1_t())
{}

inline texpr1::iterator::iterator(const iterator& i)
  : texpr1::const_iterator(i)
{}



/* substitution */
/* ============ */

inline texpr1::iterator& texpr1::iterator::operator= (const builder& c)
{
  ap_texpr0_t* cc = ap_texpr0_copy(c.get_ap_texpr1_t()->texpr0);
  ap_texpr0_clear(l.texpr0);
  *l.texpr0 = *cc;
  free(cc);
  return *this;
}


/* access */
/* ====== */

inline texpr0::iterator texpr1::iterator::get_texpr0() const
{
  return l.texpr0;
}


inline coeff& texpr1::iterator::get_coeff() const
{
  if (l.texpr0->discr!=AP_TEXPR_CST) throw(bad_discriminant("apron::texpr1::iterator::get_coeff()"));
  return reinterpret_cast<coeff&>(l.texpr0->val.cst);
}

inline void texpr1::iterator::set_var(const var& v) const
{
  if (l.texpr0->discr!=AP_TEXPR_DIM) throw(bad_discriminant("apron::texpr1::iterator::set_var(const var&)"));
  ap_dim_t d = ap_environment_dim_of_var(l.env, v.get_ap_var_t());
  if (d==AP_DIM_MAX) throw(std::invalid_argument("apron::texpr1::iterator::set_var(const var&) variable not in environment"));
  l.texpr0->val.dim = d;
}

inline ap_dim_t& texpr1::iterator::get_dim() const
{
  if (l.texpr0->discr!=AP_TEXPR_DIM) throw(bad_discriminant("apron::texpr1::iterator::get_dim()"));
  return l.texpr0->val.dim;
}

inline ap_texpr_op_t& texpr1::iterator::get_op() const
{
  if (l.texpr0->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr1::iterator::get_op()"));
  return l.texpr0->val.node->op;
}

inline ap_texpr_rtype_t& texpr1::iterator::get_rtype() const
{
  if (l.texpr0->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr1::iterator::get_type()"));
  return l.texpr0->val.node->type;
}

inline ap_texpr_rdir_t& texpr1::iterator::get_rdir() const
{
  if (l.texpr0->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr1::iterator::get_dir()"));
  return l.texpr0->val.node->dir;
}


/* traversal */
/* ========= */

inline texpr1::iterator& texpr1::iterator::operator=(const iterator& i)
{
  l = i.l;
  return *this;
}

inline texpr1::iterator texpr1::iterator::child() const
{
  if (l.texpr0->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr1::iterator::child()"));
  if (!l.texpr0->val.node->exprA) throw(std::out_of_range("apron::texpr1::iterator::child()"));
  return iterator(l.texpr0->val.node->exprA, l.env);
}

inline texpr1::iterator texpr1::iterator::left() const
{
  if (l.texpr0->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr1::iterator::left()"));
  if (!l.texpr0->val.node->exprA) throw(std::out_of_range("apron::texpr1::iterator::left()"));
  return iterator(l.texpr0->val.node->exprA, l.env);
}

inline texpr1::iterator texpr1::iterator::right() const
{
  if (l.texpr0->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr1::iterator::right()"));
  if (!l.texpr0->val.node->exprB) throw(std::out_of_range("apron::texpr1::iterator::right()"));
  return iterator(l.texpr0->val.node->exprB, l.env);
}


/* C-level compatibility */
/* ===================== */

inline ap_texpr1_t* texpr1::iterator::get_ap_texpr1_t()
{
  return &l;
}

inline const ap_texpr1_t* texpr1::iterator::get_ap_texpr1_t() const
{
  return &l;
}


/* ================================= */
/* builder                           */
/* ================================= */

inline void texpr1::builder::init_from(ap_texpr1_t* x)
{
  l.env = x->env;
  switch (x->texpr0->discr) {
  case AP_TEXPR_DIM: l.texpr0 = ap_texpr0_dim(x->texpr0->val.dim); break;
  case AP_TEXPR_CST: l.texpr0 = ap_texpr0_cst(const_cast<ap_coeff_t*>(&x->texpr0->val.cst)); break;
  case AP_TEXPR_NODE:
    l.texpr0 = ap_texpr0_node(x->texpr0->val.node->op,
			      x->texpr0->val.node->type, x->texpr0->val.node->dir,
			      x->texpr0->val.node->exprA, x->texpr0->val.node->exprB);
    break;
  default: throw std::invalid_argument("apron::texpr1::builder::init_from(ap_texpr1_t*) invalid discriminant");
  }
}

inline void texpr1::builder::init_from(ap_environment_t* e, ap_texpr0_t* x)
{
  ap_texpr1_t ll;
  ll.texpr0 = x;
  ll.env = e;
  init_from(&ll);
}

inline texpr1::builder::builder(ap_texpr1_t* x)
{
  init_from(x);
}

inline texpr1::builder::builder(const builder& x)
{
  init_from(const_cast<ap_texpr1_t*>(&x.l));
}

inline texpr1::builder::builder(const const_iterator& x)
{
  init_from(const_cast<ap_texpr1_t*>(x.get_ap_texpr1_t()));
}

inline texpr1::builder::builder(const texpr1& x)
{
  init_from(const_cast<ap_texpr1_t*>(x.get_ap_texpr1_t()));
}

inline texpr1::builder::builder(const environment& e, const texpr0::builder& b)
{
  init_from(const_cast<ap_environment_t*>(e.get_ap_environment_t()),
	    const_cast<ap_texpr0_t*>(b.get_ap_texpr0_t()));
}

inline texpr1::builder::builder(const environment& e, const texpr0::const_iterator& b)
{
  init_from(const_cast<ap_environment_t*>(e.get_ap_environment_t()),
	    const_cast<ap_texpr0_t*>(b.get_ap_texpr0_t()));
}

inline texpr1::builder::builder(const environment& e, const texpr0& b)
{
  init_from(const_cast<ap_environment_t*>(e.get_ap_environment_t()),
	    const_cast<ap_texpr0_t*>(b.get_ap_texpr0_t()));
}

inline texpr1::builder::builder(const environment& e, const coeff& x)
{
  l.env = const_cast<ap_environment_t*>(e.get_ap_environment_t());
  l.texpr0 = ap_texpr0_cst(const_cast<ap_coeff_t*>(x.get_ap_coeff_t()));
}

inline texpr1::builder::builder(const environment& e, const scalar& x)
{
  l.env = const_cast<ap_environment_t*>(e.get_ap_environment_t());
  l.texpr0 = ap_texpr0_cst_scalar(const_cast<ap_scalar_t*>(x.get_ap_scalar_t()));
}

inline texpr1::builder::builder(const environment& e, const mpq_class& x)
{
  l.env = const_cast<ap_environment_t*>(e.get_ap_environment_t());
  l.texpr0 = ap_texpr0_cst_scalar_mpq(const_cast<mpq_class&>(x).get_mpq_t());
}

inline texpr1::builder::builder(const environment& e, mpfr_t x)
{
  l.env = const_cast<ap_environment_t*>(e.get_ap_environment_t());
  l.texpr0 = ap_texpr0_cst_scalar_mpfr(x);
}

inline texpr1::builder::builder(const environment& e, int x)
{
  l.env = const_cast<ap_environment_t*>(e.get_ap_environment_t());
  l.texpr0 = ap_texpr0_cst_scalar_int(x);
}

inline texpr1::builder::builder(const environment& e, long x)
{
  l.env = const_cast<ap_environment_t*>(e.get_ap_environment_t());
  l.texpr0 = ap_texpr0_cst_scalar_int(x);
}

inline texpr1::builder::builder(const environment& e, double x)
{
  l.env = const_cast<ap_environment_t*>(e.get_ap_environment_t());
  l.texpr0 = ap_texpr0_cst_scalar_double(x);
}

inline texpr1::builder::builder(const environment& e, const frac& x)
{
  l.env = const_cast<ap_environment_t*>(e.get_ap_environment_t());
  l.texpr0 = ap_texpr0_cst_scalar_frac(x.num, x.den);
}

inline texpr1::builder::builder(const environment& e, const interval& x)
{
  l.env = const_cast<ap_environment_t*>(e.get_ap_environment_t());
  l.texpr0 = ap_texpr0_cst_interval(const_cast<ap_interval_t*>(x.get_ap_interval_t()));
}

inline texpr1::builder::builder(const environment& e, const scalar& inf, const scalar& sup)
{
  l.env = const_cast<ap_environment_t*>(e.get_ap_environment_t());
  l.texpr0 = ap_texpr0_cst_interval_scalar(const_cast<ap_scalar_t*>(inf.get_ap_scalar_t()),
					  const_cast<ap_scalar_t*>(sup.get_ap_scalar_t()));
}

inline texpr1::builder::builder(const environment& e, const mpq_class& inf, const mpq_class& sup)
{
  l.env = const_cast<ap_environment_t*>(e.get_ap_environment_t());
  l.texpr0 = ap_texpr0_cst_interval_mpq(const_cast<mpq_class&>(inf).get_mpq_t(),
				       const_cast<mpq_class&>(sup).get_mpq_t());
}

inline texpr1::builder::builder(const environment& e, mpfr_t inf, mpfr_t sup)
{
  l.env = const_cast<ap_environment_t*>(e.get_ap_environment_t());
  l.texpr0 = ap_texpr0_cst_interval_mpfr(inf,sup);
}

inline texpr1::builder::builder(const environment& e, int inf, int sup)
{
  l.env = const_cast<ap_environment_t*>(e.get_ap_environment_t());
  l.texpr0 = ap_texpr0_cst_interval_int(inf, sup);
}

inline texpr1::builder::builder(const environment& e, long inf, long sup)
{
  l.env = const_cast<ap_environment_t*>(e.get_ap_environment_t());
  l.texpr0 = ap_texpr0_cst_interval_int(inf, sup);
}

inline texpr1::builder::builder(const environment& e, double inf, double sup)
{
  l.env = const_cast<ap_environment_t*>(e.get_ap_environment_t());
  l.texpr0 = ap_texpr0_cst_interval_double(inf, sup);
}

inline texpr1::builder::builder(const environment& e, const frac& inf, const frac& sup)
{
  l.env = const_cast<ap_environment_t*>(e.get_ap_environment_t());
  l.texpr0 = ap_texpr0_cst_interval_frac(inf.num, inf.den, sup.num, sup.den);
}

inline texpr1::builder::builder(const environment& e, top t)
{
  l.env = const_cast<ap_environment_t*>(e.get_ap_environment_t());
  l.texpr0 = ap_texpr0_cst_interval_top();
}

inline texpr1::builder::builder(const environment& e, const var& v)
{
  l.env = const_cast<ap_environment_t*>(e.get_ap_environment_t());
  ap_dim_t d = ap_environment_dim_of_var(l.env, v.get_ap_var_t());
  if (d==AP_DIM_MAX)
    throw std::invalid_argument("texpr1::builder::builder(const environment&, const var&) variable not in environment");
  l.texpr0 = ap_texpr0_dim(d);
}

inline texpr1::builder::builder(ap_texpr_op_t op, const builder& argA, ap_texpr_rtype_t rtype, ap_texpr_rdir_t rdir)
{
  if (!ap_texpr_is_unop(op))
    throw std::invalid_argument("apron::texpr1::builder::builder(ap_texpr_op_t, const builder&, ap_texpr_rtype_t, ap_texpr_rdir_t) not a unary operator");
  l.env = argA.l.env;
  l.texpr0 =  ap_texpr0_unop(op,
			     const_cast<ap_texpr0_t*>(argA.l.texpr0),
			     rtype, rdir);
}

inline texpr1::builder::builder(ap_texpr_op_t op, const builder& argA, const builder& argB, ap_texpr_rtype_t rtype, ap_texpr_rdir_t rdir)
{
  if (!ap_texpr_is_binop(op))
    throw std::invalid_argument("apron::texpr1::builder::builder(ap_texpr_op_t, const builder&, const builder&, ap_texpr_rtype_t, ap_texpr_rdir_t) not a binary operator");
  l.env = argA.l.env;
  l.texpr0 =  ap_texpr0_binop(op,
			     const_cast<ap_texpr0_t*>(argA.l.texpr0),
			     const_cast<ap_texpr0_t*>(argB.l.texpr0),
			     rtype, rdir);
}


/* 'intelligent' constructors */

inline texpr1::builder unary(ap_texpr_op_t op, const texpr1::builder& a,
			     ap_texpr_rtype_t rtype = AP_RTYPE_REAL,
			     ap_texpr_rdir_t rdir = AP_RDIR_NEAREST)
{
  return texpr1::builder(op, a, rtype, rdir);
}

inline texpr1::builder binary(ap_texpr_op_t op, const texpr1::builder& a, const texpr1::builder& b,
			      ap_texpr_rtype_t rtype = AP_RTYPE_REAL,
			      ap_texpr_rdir_t rdir = AP_RDIR_NEAREST)
{
  return texpr1::builder(op, a, b, rtype, rdir);
}


inline texpr1::builder add(const texpr1::builder& a, const texpr1::builder& b,
			   ap_texpr_rtype_t rtype = AP_RTYPE_REAL,
			   ap_texpr_rdir_t rdir = AP_RDIR_NEAREST)
{
  return texpr1::builder(AP_TEXPR_ADD, a, b, rtype, rdir);
}

inline texpr1::builder sub(const texpr1::builder& a, const texpr1::builder& b,
			   ap_texpr_rtype_t rtype = AP_RTYPE_REAL,
			   ap_texpr_rdir_t rdir = AP_RDIR_NEAREST)
{
  return texpr1::builder(AP_TEXPR_SUB, a, b, rtype, rdir);
}

inline texpr1::builder mul(const texpr1::builder& a, const texpr1::builder& b,
			   ap_texpr_rtype_t rtype = AP_RTYPE_REAL,
			   ap_texpr_rdir_t rdir = AP_RDIR_NEAREST)
{
  return texpr1::builder(AP_TEXPR_MUL, a, b, rtype, rdir);
}

inline texpr1::builder div(const texpr1::builder& a, const texpr1::builder& b,
			   ap_texpr_rtype_t rtype = AP_RTYPE_REAL,
			   ap_texpr_rdir_t rdir = AP_RDIR_NEAREST)
{
  return texpr1::builder(AP_TEXPR_DIV, a, b, rtype, rdir);
}

inline texpr1::builder mod(const texpr1::builder& a, const texpr1::builder& b,
			   ap_texpr_rtype_t rtype = AP_RTYPE_REAL,
			   ap_texpr_rdir_t rdir = AP_RDIR_NEAREST)
{
  return texpr1::builder(AP_TEXPR_MOD, a, b, rtype, rdir);
}

inline texpr1::builder pow(const texpr1::builder& a, const texpr1::builder& b,
			   ap_texpr_rtype_t rtype = AP_RTYPE_REAL,
			   ap_texpr_rdir_t rdir = AP_RDIR_NEAREST)
{
  return texpr1::builder(AP_TEXPR_POW, a, b, rtype, rdir);
}

inline texpr1::builder neg(const texpr1::builder& a,
			   ap_texpr_rtype_t rtype = AP_RTYPE_REAL,
			   ap_texpr_rdir_t rdir = AP_RDIR_NEAREST)
{
  return texpr1::builder(AP_TEXPR_NEG, a, rtype, rdir);
}

inline texpr1::builder cast(const texpr1::builder& a, ap_texpr_rtype_t rtype,
			    ap_texpr_rdir_t rdir = AP_RDIR_NEAREST)
{
  return texpr1::builder(AP_TEXPR_CAST, a, rtype, rdir);
}

inline texpr1::builder floor(const texpr1::builder& a)
{
  return texpr1::builder(AP_TEXPR_CAST, a, AP_RTYPE_INT, AP_RDIR_DOWN);
}

inline texpr1::builder ceil(const texpr1::builder& a)
{
  return texpr1::builder(AP_TEXPR_CAST, a, AP_RTYPE_INT, AP_RDIR_UP);
}

inline texpr1::builder trunc(const texpr1::builder& a)
{
  return texpr1::builder(AP_TEXPR_CAST, a, AP_RTYPE_INT, AP_RDIR_ZERO);
}

inline texpr1::builder sqrt(const texpr1::builder& a,
			    ap_texpr_rtype_t rtype = AP_RTYPE_REAL,
			    ap_texpr_rdir_t rdir = AP_RDIR_NEAREST)
{
  return texpr1::builder(AP_TEXPR_SQRT, a, rtype, rdir);
}

inline texpr1::builder operator+(const texpr1::builder& a)
{
  return a;
}

inline texpr1::builder operator-(const texpr1::builder& a)
{
  return neg(a);
}

inline texpr1::builder operator+(const texpr1::builder& a, const texpr1::builder& b)
{
  return add(a,b);
}

inline texpr1::builder operator-(const texpr1::builder& a, const texpr1::builder& b)
{
  return sub(a,b);
}

inline texpr1::builder operator*(const texpr1::builder& a, const texpr1::builder& b)
{
  return mul(a,b);
}

inline texpr1::builder operator/(const texpr1::builder& a, const texpr1::builder& b)
{
  return div(a,b);
}

inline texpr1::builder operator%(const texpr1::builder& a, const texpr1::builder& b)
{
  return mod(a,b);
}

inline texpr1::builder operator^(const texpr1::builder& a, const texpr1::builder& b)
{
  return pow(a,b);
}


/* tests */
/* ===== */

inline bool texpr1::builder::is_zero() const
{
  return l.texpr0->discr==AP_TEXPR_CST && ap_coeff_zero(const_cast<ap_coeff_t*>(&l.texpr0->val.cst));
}


/* destructor */
/* ========== */

inline texpr1::builder::~builder()
{
  switch(l.texpr0->discr){
  case AP_TEXPR_CST:  ap_coeff_clear(&l.texpr0->val.cst); break;
  case AP_TEXPR_DIM:  break;
  case AP_TEXPR_NODE: free(l.texpr0->val.node); break;
  default: assert(false);
  }
  free(l.texpr0);
}


/* C-level compatibility */
/* ===================== */

inline ap_texpr1_t* texpr1::builder::get_ap_texpr1_t()
{
  return &l;
}

inline const ap_texpr1_t* texpr1::builder::get_ap_texpr1_t() const
{
  return &l;
}
