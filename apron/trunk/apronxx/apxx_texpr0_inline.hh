/* -*- C++ -*-
 * apxx_texpr0_inline.hh
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


inline dim::dim(ap_dim_t d) : d(d)
{}

inline dim::dim(const environment& e, const var& v)
{
  ap_dim_t d = ap_environment_dim_of_var(const_cast<ap_environment_t*>(e.get_ap_environment_t()),
					 v.get_ap_var_t());
  if (d==AP_DIM_MAX)
    throw std::invalid_argument("apron::dim::dim(const environment&, const var&) invalid variable name");
}



inline bool is_unop(ap_texpr_op_t op)
{
  return ap_texpr_is_unop(op);
}

inline bool is_binop(ap_texpr_op_t op)
{
  return ap_texpr_is_binop(op);
}


/* ================================= */
/* texpr0                            */
/* ================================= */

inline void texpr0::init_from(ap_texpr0_t* x)
{
  l = *x;
  free(x);
}

inline texpr0::texpr0(ap_texpr0_t* x)
{ init_from(x); }

inline texpr0::texpr0(const builder& x)
{ init_from(ap_texpr0_copy(const_cast<ap_texpr0_t*>(x.get_ap_texpr0_t()))); }

inline texpr0::texpr0(const const_iterator& x)
{ init_from(ap_texpr0_copy(const_cast<ap_texpr0_t*>(x.get_ap_texpr0_t()))); }

inline texpr0::texpr0(const texpr0& x)
{ init_from(ap_texpr0_copy(const_cast<ap_texpr0_t*>(&x.l))); }


/* linear expression */

inline texpr0::texpr0(const linexpr0& l)
{ init_from(ap_texpr0_from_linexpr0(const_cast<ap_linexpr0_t*>(l.get_ap_linexpr0_t()))); }


/* change of dimension */

inline texpr0::texpr0(const texpr0& x, const dimchange& d, bool add)
{
  if (add)
    init_from(ap_texpr0_add_dimensions(const_cast<ap_texpr0_t*>(&x.l),
				       const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t())));
  else
    init_from(ap_texpr0_remove_dimensions(const_cast<ap_texpr0_t*>(&x.l),
					  const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t())));
}

inline texpr0::texpr0(const texpr0& x, const dimperm& d)
{
  init_from(ap_texpr0_permute_dimensions(const_cast<ap_texpr0_t*>(&x.l),
					 const_cast<ap_dimperm_t*>(d.get_ap_dimperm_t())));
}

inline texpr0::texpr0(const texpr0& x, ap_dim_t dim, const texpr0& dst)
{
  init_from(ap_texpr0_substitute(const_cast<ap_texpr0_t*>(&x.l),
				 dim, const_cast<ap_texpr0_t*>(&dst.l)));
}


/* destructor */
/* ========== */

/* (deep) destruction */

inline texpr0::~texpr0()
{
  ap_texpr0_clear(&l);
}


/* assignment */
/* ========== */

/* (deep) copy */


inline texpr0& texpr0::operator= (const builder& x)
{
  // copy first, as x.l may alias this!
  ap_texpr0_t* c = ap_texpr0_copy(const_cast<ap_texpr0_t*>(x.get_ap_texpr0_t()));
  ap_texpr0_clear(&l);
  init_from(c);
  return *this;
}

inline texpr0& texpr0::operator= (const texpr0& x)
{
  // copy first, as x.l may alias this!
  ap_texpr0_t* c = ap_texpr0_copy(const_cast<ap_texpr0_t*>(&x.l));
  ap_texpr0_clear(&l);
  init_from(c);
  return *this;
}

inline texpr0& texpr0::operator= (const const_iterator& x)
{
  // copy first, as x.l may alias this!
  ap_texpr0_t* c = ap_texpr0_copy(const_cast<ap_texpr0_t*>(x.get_ap_texpr0_t()));
  ap_texpr0_clear(&l);
  init_from(c);
  return *this;
}

inline texpr0& texpr0::operator= (const linexpr0& x)
{
  ap_texpr0_clear(&l);
  init_from(ap_texpr0_from_linexpr0(const_cast<ap_linexpr0_t*>(x.get_ap_linexpr0_t())));
  return *this;
}


/* print */
/* ===== */

inline std::ostream& operator<< (std::ostream& os, const texpr0& s)
{
  return os << s.root();
}

inline void texpr0::print(char** name_of_dim, FILE* stream) const
{
  root().print(name_of_dim,stream);
}


/* tests, size */
/* =========== */

inline bool texpr0::is_zero() const
{
  return root().is_zero();
}

inline bool texpr0::equal(const texpr0& x) const
{
  return root().equal(x);
}

inline size_t texpr0::depth() const
{
  return root().depth();
}

inline size_t texpr0::size() const
{
  return root().size();
}

inline ap_dim_t texpr0::max_dim() const
{
  return root().max_dim();
}

inline bool texpr0::has_dim(ap_dim_t d) const
{
  return root().has_dim(d);
}

inline std::vector<ap_dim_t> texpr0::dimlist() const
{
  return root().dimlist();
}

inline bool texpr0::is_interval_cst() const
{
  return root().is_interval_cst();
}

inline bool texpr0::is_interval_linear() const
{
  return root().is_interval_linear();
}

inline bool texpr0::is_interval_polynomial() const
{
  return root().is_interval_polynomial();
}

inline bool texpr0::is_interval_polyfrac() const
{
  return root().is_interval_polyfrac();
}

inline bool texpr0::is_scalar() const
{
  return root().is_scalar();
}


/* operations */
/* ========== */

inline void texpr0::substitute(ap_dim_t dim, const texpr0& dst)
{
  ap_texpr0_substitute_with(&l,dim,const_cast<ap_texpr0_t*>(&dst.l));
}

#if 0
inline interval texpr0::eval(manager& m, const abstract0& a, ap_scalar_discr_t discr,
			     bool* pexact) const
{
  bool b;
  if (!pexact) pexact = &b;
  return
    ap_eval_texpr0(m.get_ap_manager_t(),
		   const_cast<ap_abstract0_t*>(a.get_ap_abstract0_t()),
		   const_cast<ap_texpr0_t*>(l),
		   discr, pexact);
}

inline linexpr0 texpr0::intlinearize(manager& m, const abstract0& a, ap_scalar_discr_t discr,
				     bool quasilinearize, bool* pexact) const
{
  bool b;
  if (!pexact) pexact = &b;
  return
    ap_intlinearize_texpr0(m.get_ap_manager_t(),
			   const_cast<ap_abstract0_t*>(a.get_ap_abstract0_t()),
			   const_cast<ap_texpr0_t*>(l),
			   pexact, discr, quasilinearize);
}
#endif

inline long texpr0::hash() const
{
  return ap_texpr0_hash(const_cast<ap_texpr0_t*>(&l));
}



/* change of dimension */
/* =================== */


inline void texpr0::add_dimensions(const dimchange& d)
{
  ap_texpr0_add_dimensions_with(&l, const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t()));
}


inline void texpr0::remove_dimensions(const dimchange& d)
{
  ap_texpr0_remove_dimensions_with(&l, const_cast<ap_dimchange_t*>(d.get_ap_dimchange_t()));
}

inline void texpr0::permute_dimensions(const dimperm& d)
{
  ap_texpr0_permute_dimensions_with(&l, const_cast<ap_dimperm_t*>(d.get_ap_dimperm_t()));
}



/* C-level compatibility */
/* ===================== */

inline ap_texpr0_t* texpr0::get_ap_texpr0_t()
{
  return &l;
}

inline const ap_texpr0_t* texpr0::get_ap_texpr0_t() const
{
  return &l;
}





/* ================================= */
/* iterator                          */
/* ================================= */


/* constructors */
/* ============ */

inline texpr0::const_iterator texpr0::root() const
{
  return const_iterator(const_cast<ap_texpr0_t*>(&l));
}

inline texpr0::const_iterator::const_iterator(ap_texpr0_t* l) : l(l)
{}


inline texpr0::const_iterator::const_iterator(const texpr0& e)
  : l(const_cast<ap_texpr0_t*>(e.get_ap_texpr0_t()))
{}

inline texpr0::const_iterator::const_iterator(const const_iterator& i) : l(i.l)
{}


/* access */
/* ====== */

inline ap_texpr_discr_t texpr0::const_iterator::get_discr() const
{
  return l->discr;
}

inline const coeff& texpr0::const_iterator::get_coeff() const
{
  if (l->discr!=AP_TEXPR_CST) throw(bad_discriminant("apron::texpr0::const_iterator::get_coeff()"));
  return reinterpret_cast<coeff&>(l->val.cst);
}

inline ap_dim_t texpr0::const_iterator::get_dim() const
{
  if (l->discr!=AP_TEXPR_DIM) throw(bad_discriminant("apron::texpr0::const_iterator::get_dim()"));
  return l->val.dim;
}

inline ap_texpr_op_t texpr0::const_iterator::get_op() const
{
  if (l->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr0::const_iterator::get_op()"));
  return l->val.node->op;
}

inline ap_texpr_rtype_t texpr0::const_iterator::get_rtype() const
{
  if (l->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr0::const_iterator::get_type()"));
  return l->val.node->type;
}

inline ap_texpr_rdir_t texpr0::const_iterator::get_rdir() const
{
  if (l->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr0::const_iterator::get_dir()"));
  return l->val.node->dir;
}


/* traversal */
/* ========= */

inline texpr0::const_iterator& texpr0::const_iterator::operator=(const const_iterator& i)
{
  l = i.l;
  return *this;
}

inline texpr0::const_iterator texpr0::const_iterator::child() const
{
  if (l->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr0::const_iterator::child()"));
  if (!l->val.node->exprA) throw(std::out_of_range("apron::texpr0::const_iterator::child()"));
  return l->val.node->exprA;
}

inline texpr0::const_iterator texpr0::const_iterator::left() const
{
  if (l->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr0::const_iterator::left()"));
  if (!l->val.node->exprA) throw(std::out_of_range("apron::texpr0::const_iterator::left()"));
  return l->val.node->exprA;
}

inline texpr0::const_iterator texpr0::const_iterator::right() const
{
  if (l->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr0::const_iterator::right()"));
  if (!l->val.node->exprB) throw(std::out_of_range("apron::texpr0::const_iterator::right()"));
  return l->val.node->exprB;
}


/* print */
/* ===== */

extern void apxx_texpr0_ostream(std::ostream& os, ap_texpr0_t* a,
				std::vector<std::string>* names);

inline std::ostream& operator<< (std::ostream& os, const texpr0::const_iterator& s)
{
  apxx_texpr0_ostream(os, const_cast<ap_texpr0_t*>(s.l), get_varname(os));
  return os;
}

inline void texpr0::const_iterator::print(char** name_of_dim, FILE* stream) const
{
  ap_texpr0_fprint(stream, const_cast<ap_texpr0_t*>(l), name_of_dim);
}


/* tests, size */
/* =========== */

inline bool texpr0::const_iterator::is_zero() const
{
  return l->discr==AP_TEXPR_CST && ap_coeff_zero(&const_cast<ap_texpr0_t*>(l)->val.cst);
}

inline bool texpr0::const_iterator::equal(const texpr0& x) const
{
  return ap_texpr0_equal(const_cast<ap_texpr0_t*>(l),
			 const_cast<ap_texpr0_t*>(x.get_ap_texpr0_t()));
}

inline size_t texpr0::const_iterator::depth() const
{
  return ap_texpr0_depth(const_cast<ap_texpr0_t*>(l));
}

inline size_t texpr0::const_iterator::size() const
{
  return ap_texpr0_size(const_cast<ap_texpr0_t*>(l));
}

inline ap_dim_t texpr0::const_iterator::max_dim() const
{
  return ap_texpr0_max_dim(const_cast<ap_texpr0_t*>(l));
}

inline bool texpr0::const_iterator::has_dim(ap_dim_t d) const
{
  return ap_texpr0_has_dim(const_cast<ap_texpr0_t*>(l), d);
}

inline std::vector<ap_dim_t> texpr0::const_iterator::dimlist() const
{
  ap_dim_t* d = ap_texpr0_dimlist(const_cast<ap_texpr0_t*>(l));
  ap_dim_t i;
  for (i=0; d[i]!=AP_DIM_MAX; i++) ;
  std::vector<ap_dim_t> r = std::vector<ap_dim_t>(i,0);
  for (i=0; d[i]!=AP_DIM_MAX; i++) r[i] = d[i];
  free(d);
  return r;
}

inline bool texpr0::const_iterator::is_interval_cst() const
{
  return ap_texpr0_is_interval_cst(const_cast<ap_texpr0_t*>(l));
}

inline bool texpr0::const_iterator::is_interval_linear() const
{
  return ap_texpr0_is_interval_linear(const_cast<ap_texpr0_t*>(l));
}

inline bool texpr0::const_iterator::is_interval_polynomial() const
{
  return ap_texpr0_is_interval_polynomial(const_cast<ap_texpr0_t*>(l));
}

inline bool texpr0::const_iterator::is_interval_polyfrac() const
{
  return ap_texpr0_is_interval_polyfrac(const_cast<ap_texpr0_t*>(l));
}

inline bool texpr0::const_iterator::is_scalar() const
{
  return ap_texpr0_is_scalar(const_cast<ap_texpr0_t*>(l));
}


/* C-level compatibility */
/* ===================== */

inline ap_texpr0_t* texpr0::const_iterator::get_ap_texpr0_t()
{
  return l;
}

inline const ap_texpr0_t* texpr0::const_iterator::get_ap_texpr0_t() const
{
  return l;
}


/* constructors */
/* ============ */

inline texpr0::iterator texpr0::root()
{
  return iterator(*this);
}

inline texpr0::iterator::iterator(ap_texpr0_t* l)
  : texpr0::const_iterator(l)
{}


inline texpr0::iterator::iterator(texpr0& e)
  : texpr0::const_iterator(e.get_ap_texpr0_t())
{}

inline texpr0::iterator::iterator(const iterator& i)
  : texpr0::const_iterator(i)
{}



/* substitution */
/* ============ */

inline texpr0::iterator& texpr0::iterator::operator= (const builder& c)
{
  ap_texpr0_t* cc = ap_texpr0_copy(const_cast<ap_texpr0_t*>(c.get_ap_texpr0_t()));
  ap_texpr0_clear(l);
  *l = *cc;
  free(cc);
  return *this;
}


/* access */
/* ====== */

inline coeff& texpr0::iterator::get_coeff() const
{
  if (l->discr!=AP_TEXPR_CST) throw(bad_discriminant("apron::texpr0::iterator::get_coeff()"));
  return reinterpret_cast<coeff&>(l->val.cst);
}

inline ap_dim_t& texpr0::iterator::get_dim() const
{
  if (l->discr!=AP_TEXPR_DIM) throw(bad_discriminant("apron::texpr0::iterator::get_dim()"));
  return l->val.dim;
}

inline ap_texpr_op_t& texpr0::iterator::get_op() const
{
  if (l->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr0::iterator::get_op()"));
  return l->val.node->op;
}

inline ap_texpr_rtype_t& texpr0::iterator::get_rtype() const
{
  if (l->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr0::iterator::get_type()"));
  return l->val.node->type;
}

inline ap_texpr_rdir_t& texpr0::iterator::get_rdir() const
{
  if (l->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr0::iterator::get_dir()"));
  return l->val.node->dir;
}


/* traversal */
/* ========= */

inline texpr0::iterator& texpr0::iterator::operator=(const iterator& i)
{
  l = i.l;
  return *this;
}

inline texpr0::iterator texpr0::iterator::child() const
{
  if (l->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr0::iterator::child()"));
  if (!l->val.node->exprA) throw(std::out_of_range("apron::texpr0::iterator::child()"));
  return l->val.node->exprA;
}

inline texpr0::iterator texpr0::iterator::left() const
{
  if (l->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr0::iterator::left()"));
  if (!l->val.node->exprA) throw(std::out_of_range("apron::texpr0::iterator::left()"));
  return l->val.node->exprA;
}

inline texpr0::iterator texpr0::iterator::right() const
{
  if (l->discr!=AP_TEXPR_NODE) throw(bad_discriminant("apron::texpr0::iterator::right()"));
  if (!l->val.node->exprB) throw(std::out_of_range("apron::texpr0::iterator::right()"));
  return l->val.node->exprB;
}


/* C-level compatibility */
/* ===================== */

inline ap_texpr0_t* texpr0::iterator::get_ap_texpr0_t()
{
  return l;
}

inline const ap_texpr0_t* texpr0::iterator::get_ap_texpr0_t() const
{
  return l;
}



/* ================================= */
/* builder                           */
/* ================================= */


inline void texpr0::builder::init_from(ap_texpr0_t* x)
{
  switch (x->discr) {
  case AP_TEXPR_DIM:  l = ap_texpr0_dim(x->val.dim); break;
  case AP_TEXPR_CST:  l = ap_texpr0_cst(const_cast<ap_coeff_t*>(&x->val.cst)); break;
  case AP_TEXPR_NODE:
    l = ap_texpr0_node(x->val.node->op,
                       x->val.node->type, x->val.node->dir,
                       x->val.node->exprA, x->val.node->exprB);
    break;
  default: throw std::invalid_argument("apron::texpr0::builder::init_from(ap_texpr0_t*) invalid discriminant");
  }
}

inline texpr0::builder::builder(ap_texpr0_t* x)
{
  init_from(x);
}

inline texpr0::builder::builder(const builder& x)
{
  init_from(x.l);
}

inline texpr0::builder::builder(const const_iterator& x)
{
  init_from(const_cast<ap_texpr0_t*>(x.get_ap_texpr0_t()));
}

inline texpr0::builder::builder(const texpr0& x)
{
  init_from(const_cast<ap_texpr0_t*>(x.get_ap_texpr0_t()));
}

inline texpr0::builder::builder(const coeff& x)
  : l(ap_texpr0_cst(const_cast<ap_coeff_t*>(x.get_ap_coeff_t())))
{}

inline texpr0::builder::builder(const scalar& x)
  : l(ap_texpr0_cst_scalar(const_cast<ap_scalar_t*>(x.get_ap_scalar_t())))
{}

inline texpr0::builder::builder(const mpq_class& x)
  : l(ap_texpr0_cst_scalar_mpq(const_cast<mpq_class&>(x).get_mpq_t()))
{}

inline texpr0::builder::builder(mpfr_t x)
  : l(ap_texpr0_cst_scalar_mpfr(x))
{}

inline texpr0::builder::builder(int x)
  : l(ap_texpr0_cst_scalar_int(x))
{}

inline texpr0::builder::builder(long x)
  : l(ap_texpr0_cst_scalar_int(x))
{}

inline texpr0::builder::builder(double x)
  : l(ap_texpr0_cst_scalar_double(x))
{}

inline texpr0::builder::builder(const frac& x)
  : l(ap_texpr0_cst_scalar_frac(x.num, x.den))
{}

inline texpr0::builder::builder(const interval& x)
  : l(ap_texpr0_cst_interval(const_cast<ap_interval_t*>(x.get_ap_interval_t())))
{}

inline texpr0::builder::builder(const scalar& inf, const scalar& sup)
  : l(ap_texpr0_cst_interval_scalar(const_cast<ap_scalar_t*>(inf.get_ap_scalar_t()),
				    const_cast<ap_scalar_t*>(sup.get_ap_scalar_t())))
{}

inline texpr0::builder::builder(const mpq_class& inf, const mpq_class& sup)
  : l(ap_texpr0_cst_interval_mpq(const_cast<mpq_class&>(inf).get_mpq_t(),
				 const_cast<mpq_class&>(sup).get_mpq_t()))
{}

inline texpr0::builder::builder(mpfr_t inf, mpfr_t sup)
  : l(ap_texpr0_cst_interval_mpfr(inf,sup))
{}

inline texpr0::builder::builder(int inf, int sup)
  : l(ap_texpr0_cst_interval_int(inf, sup))
{}

inline texpr0::builder::builder(long inf, long sup)
  : l(ap_texpr0_cst_interval_int(inf, sup))
{}

inline texpr0::builder::builder(double inf, double sup)
  : l(ap_texpr0_cst_interval_double(inf, sup))
{}

inline texpr0::builder::builder(const frac& inf, const frac& sup)
  : l(ap_texpr0_cst_interval_frac(inf.num, inf.den, sup.num, sup.den))
{}

inline texpr0::builder::builder(top t)
  : l(ap_texpr0_cst_interval_top())
{}

inline texpr0::builder::builder(dim d)
  : l(ap_texpr0_dim(d.d))
{}

inline texpr0::builder::builder(ap_texpr_op_t op, const builder& argA, ap_texpr_rtype_t rtype, ap_texpr_rdir_t rdir)
{
  if (!ap_texpr_is_unop(op))
    throw std::invalid_argument("apron::texpr0::builder::builder(ap_texpr_op_t, const builder&, ap_texpr_rtype_t, ap_texpr_rdir_t) not a unary operator");
  l = ap_texpr0_unop(op,
		     const_cast<ap_texpr0_t*>(argA.l),
		     rtype, rdir);
}

inline texpr0::builder::builder(ap_texpr_op_t op, const builder& argA, const builder& argB, ap_texpr_rtype_t rtype, ap_texpr_rdir_t rdir)
{
  if (!ap_texpr_is_binop(op))
    throw std::invalid_argument("apron::texpr0::builder::builder(ap_texpr_op_t, const builder&, const builder&, ap_texpr_rtype_t, ap_texpr_rdir_t) not a binary operator");
  l = ap_texpr0_binop(op,
		      const_cast<ap_texpr0_t*>(argA.l),
		      const_cast<ap_texpr0_t*>(argB.l),
		      rtype, rdir);
}


/* 'intelligent' constructors */

inline texpr0::builder unary(ap_texpr_op_t op, const texpr0::builder& a,
			     ap_texpr_rtype_t rtype = AP_RTYPE_REAL,
			     ap_texpr_rdir_t rdir = AP_RDIR_NEAREST)
{
  return texpr0::builder(op, a, rtype, rdir);
}

inline texpr0::builder binary(ap_texpr_op_t op, const texpr0::builder& a, const texpr0::builder& b,
			      ap_texpr_rtype_t rtype = AP_RTYPE_REAL,
			      ap_texpr_rdir_t rdir = AP_RDIR_NEAREST)
{
  return texpr0::builder(op, a, b, rtype, rdir);
}


inline texpr0::builder add(const texpr0::builder& a, const texpr0::builder& b,
			   ap_texpr_rtype_t rtype = AP_RTYPE_REAL,
			   ap_texpr_rdir_t rdir = AP_RDIR_NEAREST)
{
  return texpr0::builder(AP_TEXPR_ADD, a, b, rtype, rdir);
}

inline texpr0::builder sub(const texpr0::builder& a, const texpr0::builder& b,
			   ap_texpr_rtype_t rtype = AP_RTYPE_REAL,
			   ap_texpr_rdir_t rdir = AP_RDIR_NEAREST)
{
  return texpr0::builder(AP_TEXPR_SUB, a, b, rtype, rdir);
}

inline texpr0::builder mul(const texpr0::builder& a, const texpr0::builder& b,
			   ap_texpr_rtype_t rtype = AP_RTYPE_REAL,
			   ap_texpr_rdir_t rdir = AP_RDIR_NEAREST)
{
  return texpr0::builder(AP_TEXPR_MUL, a, b, rtype, rdir);
}

inline texpr0::builder div(const texpr0::builder& a, const texpr0::builder& b,
			   ap_texpr_rtype_t rtype = AP_RTYPE_REAL,
			   ap_texpr_rdir_t rdir = AP_RDIR_NEAREST)
{
  return texpr0::builder(AP_TEXPR_DIV, a, b, rtype, rdir);
}

inline texpr0::builder mod(const texpr0::builder& a, const texpr0::builder& b,
			   ap_texpr_rtype_t rtype = AP_RTYPE_REAL,
			   ap_texpr_rdir_t rdir = AP_RDIR_NEAREST)
{
  return texpr0::builder(AP_TEXPR_MOD, a, b, rtype, rdir);
}

inline texpr0::builder pow(const texpr0::builder& a, const texpr0::builder& b,
			   ap_texpr_rtype_t rtype = AP_RTYPE_REAL,
			   ap_texpr_rdir_t rdir = AP_RDIR_NEAREST)
{
  return texpr0::builder(AP_TEXPR_POW, a, b, rtype, rdir);
}

inline texpr0::builder neg(const texpr0::builder& a,
			   ap_texpr_rtype_t rtype = AP_RTYPE_REAL,
			   ap_texpr_rdir_t rdir = AP_RDIR_NEAREST)
{
  return texpr0::builder(AP_TEXPR_NEG, a, rtype, rdir);
}

inline texpr0::builder cast(const texpr0::builder& a, ap_texpr_rtype_t rtype,
			    ap_texpr_rdir_t rdir = AP_RDIR_NEAREST)
{
  return texpr0::builder(AP_TEXPR_CAST, a, rtype, rdir);
}

inline texpr0::builder floor(const texpr0::builder& a)
{
  return texpr0::builder(AP_TEXPR_CAST, a, AP_RTYPE_INT, AP_RDIR_DOWN);
}

inline texpr0::builder ceil(const texpr0::builder& a)
{
  return texpr0::builder(AP_TEXPR_CAST, a, AP_RTYPE_INT, AP_RDIR_UP);
}

inline texpr0::builder trunc(const texpr0::builder& a)
{
  return texpr0::builder(AP_TEXPR_CAST, a, AP_RTYPE_INT, AP_RDIR_ZERO);
}

inline texpr0::builder sqrt(const texpr0::builder& a,
			    ap_texpr_rtype_t rtype = AP_RTYPE_REAL,
			    ap_texpr_rdir_t rdir = AP_RDIR_NEAREST)
{
  return texpr0::builder(AP_TEXPR_SQRT, a, rtype, rdir);
}

inline texpr0::builder operator+(const texpr0::builder& a)
{
  return a;
}

inline texpr0::builder operator-(const texpr0::builder& a)
{
  return neg(a);
}

inline texpr0::builder operator+(const texpr0::builder& a, const texpr0::builder& b)
{
  return add(a,b);
}

inline texpr0::builder operator-(const texpr0::builder& a, const texpr0::builder& b)
{
  return sub(a,b);
}

inline texpr0::builder operator*(const texpr0::builder& a, const texpr0::builder& b)
{
  return mul(a,b);
}

inline texpr0::builder operator/(const texpr0::builder& a, const texpr0::builder& b)
{
  return div(a,b);
}

inline texpr0::builder operator%(const texpr0::builder& a, const texpr0::builder& b)
{
  return mod(a,b);
}

inline texpr0::builder operator^(const texpr0::builder& a, const texpr0::builder& b)
{
  return pow(a,b);
}


/* destructor */
/* ========== */

inline texpr0::builder::~builder()
{
  switch(l->discr){
  case AP_TEXPR_CST:  ap_coeff_clear(&l->val.cst); break;
  case AP_TEXPR_DIM:  break;
  case AP_TEXPR_NODE: free(l->val.node); break;
  default: assert(false);
  }
  free(l);
}

inline bool texpr0::builder::is_zero() const
{
  return l->discr==AP_TEXPR_CST && ap_coeff_zero(&const_cast<ap_texpr0_t*>(l)->val.cst);
}


/* C-level compatibility */
/* ===================== */

inline ap_texpr0_t* texpr0::builder::get_ap_texpr0_t()
{
  return l;
}

inline const ap_texpr0_t* texpr0::builder::get_ap_texpr0_t() const
{
  return l;
}
