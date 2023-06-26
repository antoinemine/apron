/*
 * ppl_user.cc
 *
 * APRON Library / PPL library wrapper
 *
 * Conversion between APRON user types and PPL classes.
 * These are shared by all PPL wrappers.
 *
 * Copyright (C) Antoine Mine' 2006
 *
 */

/*
 * This file is part of the APRON Library, released under GPL license.
 * Please read the COPYING file packaged in the distribution.
 */

#include <assert.h>
#include <stdexcept>
#include "ppl_user.hh"

extern "C" ppl_internal_t* ap_ppl_internal_alloc(bool strict)
{
  ppl_internal_t* intern = (ppl_internal_t*)malloc(sizeof(ppl_internal_t));
  intern->strict = strict;
  intern->itv = itv_internal_alloc();
  return intern;
}
extern "C" void ap_ppl_internal_free(void* internal)
{
  ppl_internal_t* intern = (ppl_internal_t*)internal;
  itv_internal_free(intern->itv);
  free(intern);
}
/* ********************************************************************** */
/* Conversions from PPL to APRON */
/* ********************************************************************** */

/* Constraint => ap_lincons0_t (exact) */
ap_lincons0_t ap_ppl_to_lincons(const Constraint& c)
{
  ap_constyp_t t;
  ap_linexpr0_t* e;
  int i, n = c.space_dimension();
  /* special, inconsistent case */
  if (c.is_inconsistent())
    return ap_lincons0_make_unsat();
  /* other cases */
  e = ap_linexpr0_alloc(AP_LINEXPR_DENSE,n);
  assert(e);
  /* variable coefficients */
  for (i=0;i<n;i++)
    ap_ppl_mpz_to_scalar(e->p.coeff[i].val.scalar,c.coefficient(Variable(i)));
  /* constant coefficient */
  ap_ppl_mpz_to_scalar(e->cst.val.scalar,c.inhomogeneous_term());
  /* constraint type */
  if (c.is_nonstrict_inequality()) t = AP_CONS_SUPEQ;
  else if (c.is_equality()) t = AP_CONS_EQ;
  else if (c.is_strict_inequality()) t = AP_CONS_SUP;
  else throw invalid_argument("Constraint type in ap_ppl_to_lincons");
  return ap_lincons0_make(t,e,NULL);
}

/* Constraint_System => ap_lincons0_array_t (exact) */
ap_lincons0_array_t ap_ppl_to_lincons_array(const Constraint_System& c)
{
  ap_lincons0_array_t a;
  Constraint_System::const_iterator i, end = c.end();
  int k;
  /* first, compute system size */
  for (i=c.begin(),k=0;i!=end;i++,k++) ;
  a = ap_lincons0_array_make(k);
  /* then, convert constraints */
  for (i=c.begin(),k=0;i!=end;i++,k++)
    a.p[k] = ap_ppl_to_lincons(*i);
  return a;
}

/* Congruence => ap_lincons0_t (exact) */
ap_lincons0_t ap_ppl_to_lincons(const Congruence& c)
{
  ap_constyp_t t;
  ap_linexpr0_t* e;
  int i, n = c.space_dimension();
  /* special, inconsistent case */
#ifndef PPL_0_9
  if (c.is_inconsistent())
#else
  if (c.is_trivial_false())
#endif
    return ap_lincons0_make_unsat();
  /* other cases */
  e = ap_linexpr0_alloc(AP_LINEXPR_DENSE,n);
  assert(e);
  /* variable coefficients */
  for (i=0;i<n;i++)
    ap_ppl_mpz_to_scalar(e->p.coeff[i].val.scalar,c.coefficient(Variable(i)));
  /* constant coefficient */
  ap_ppl_mpz_to_scalar(e->cst.val.scalar,c.inhomogeneous_term());
  /* constraint type */
  if (c.is_equality()) return ap_lincons0_make(AP_CONS_EQ,e,NULL);
  else {
    ap_scalar_t* mod = ap_scalar_alloc();
    ap_ppl_mpz_to_scalar(mod,c.modulus());
    return ap_lincons0_make(AP_CONS_EQMOD,e,mod);
  }
}

/* Congruence_System => ap_lincons0_array_t (exact) */
ap_lincons0_array_t ap_ppl_to_lincons_array(const Congruence_System& c)
{
  ap_lincons0_array_t a;
  Congruence_System::const_iterator i, end = c.end();
  int k;
  /* first, compute system size */
  for (i=c.begin(),k=0;i!=end;i++,k++) ;
  a = ap_lincons0_array_make(k);
  /* then, convert constraints */
  for (i=c.begin(),k=0;i!=end;i++,k++)
    a.p[k] = ap_ppl_to_lincons(*i);
  return a;
}


/* ====================================================================== */
/* Generators */
/* ====================================================================== */

/* Generator => ap_generator0_t */
ap_generator0_t ap_ppl_to_generator(const Generator& c, bool& exact)
{
  ap_gentyp_t t;
  ap_linexpr0_t* e;
  int i, n = c.space_dimension();
  e = ap_linexpr0_alloc(AP_LINEXPR_DENSE,n);
  assert(e);
  exact = true;
  ap_linexpr0_set_cst_scalar_int(e,0);
  if (c.is_ray() || c.is_line()) {
    /* ray or line: no divisor */
    for (i=0;i<n;i++)
      ap_ppl_mpz_to_scalar(e->p.coeff[i].val.scalar,c.coefficient(Variable(i)));
    return ap_generator0_make(c.is_ray() ? AP_GEN_RAY : AP_GEN_LINE,e);
  }
  else {
    /* point or closure point: has divisor */
    const mpz_class& div = c.divisor();
    if (c.is_closure_point()) exact = false;
    for (i=0;i<n;i++)
      ap_ppl_mpz2_to_scalar(e->p.coeff[i].val.scalar,c.coefficient(Variable(i)),div);
    return ap_generator0_make(AP_GEN_VERTEX,e);
  }
}

/* Generator_System => ap_generator0_array_t */
ap_generator0_array_t ap_ppl_to_generator_array(const Generator_System& c,
						bool& exact)
{
  ap_generator0_array_t a;
  Generator_System::const_iterator i, end = c.end();
  int k;
  bool exact2;
  exact = true;
  /* first, compute system size */
  for (i=c.begin(),k=0;i!=end;i++,k++) ;
  a = ap_generator0_array_make(k);
  /* then, convert generators */
  for (i=c.begin(),k=0;i!=end;i++,k++){
    a.p[k] = ap_ppl_to_generator(*i,exact2);
    exact = exact && exact2;
  }
  return a;
}

/* Grid_Generator => ap_generator0_t (exact) */
ap_generator0_t ap_ppl_to_generator(const Grid_Generator& c)
{
  ap_gentyp_t t;
  ap_linexpr0_t* e;
  int i, n = c.space_dimension();
  e = ap_linexpr0_alloc(AP_LINEXPR_DENSE,n);
  assert(e);
  ap_linexpr0_set_cst_scalar_int(e,0);
  if (c.is_line()) {
    /* line: no divisor */
    for (i=0;i<n;i++)
      ap_ppl_mpz_to_scalar(e->p.coeff[i].val.scalar,c.coefficient(Variable(i)));
    return ap_generator0_make(AP_GEN_LINE,e);
  }
  else {
    /* point or parameter: has divisor */
    const mpz_class& div = c.divisor();
    for (i=0;i<n;i++)
      ap_ppl_mpz2_to_scalar(e->p.coeff[i].val.scalar,c.coefficient(Variable(i)),div);
    return ap_generator0_make(c.is_point() ? AP_GEN_VERTEX : AP_GEN_LINEMOD,e);
  }
}

/* Grid_Generator_System => ap_generator0_array_t (exact) */
ap_generator0_array_t ap_ppl_to_generator_array(const Grid_Generator_System& c)
{
  ap_generator0_array_t a;
  Grid_Generator_System::const_iterator i, end = c.end();
  int k;
  /* first, compute system size */
  for (i=c.begin(),k=0;i!=end;i++,k++) ;
  a = ap_generator0_array_make(k);
  /* then, convert generators */
  for (i=c.begin(),k=0;i!=end;i++,k++) {
    a.p[k] = ap_ppl_to_generator(*i);
  }
  return a;
}

/* whole universe as a generator system */
ap_generator0_array_t ap_ppl_generator_universe(size_t dim)
{
  ap_generator0_array_t ar = ap_generator0_array_make(dim+1);
  /* origin vertex */
  ar.p[0] = ap_generator0_make(AP_GEN_VERTEX,ap_linexpr0_alloc(AP_LINEXPR_SPARSE,0));
  /* one line for each dimension */
  for (size_t i=0;i<dim;i++) {
    ap_linexpr0_t* e = ap_linexpr0_alloc(AP_LINEXPR_SPARSE,1);
    e->p.linterm[0].dim = i;
    ap_coeff_set_scalar_int(&e->p.linterm[0].coeff,1);
    ar.p[i+1] = ap_generator0_make(AP_GEN_LINE,e);
  }
  return ar;
}

/* ********************************************************************** */
/* Conversions from APRON to PPL */
/* ********************************************************************** */

/* ====================================================================== */
/* Boxes */
/* ====================================================================== */

/* whole universe as a box */
void ap_ppl_box_universe(ap_interval_t** i,size_t nb)
{
  for (size_t j=0;j<nb;j++)
    ap_interval_set_top(i[j]);
}

/* ap_interval_t box => Constraint_System (return exactness) */
bool ap_ppl_of_box(Constraint_System& r, ap_interval_t** a, size_t intdim, size_t realdim)
{
  bool exact = true;
  size_t i;
  mpq_class temp;
  size_t nb = intdim+realdim;
  mpq_ptr mpq = temp.get_mpq_t();

  r.clear();
  for (i=0;i<nb;i++) {
    /* inf */
    switch (ap_scalar_infty(a[i]->inf)) {
    case 0:
      exact =
	!ap_mpq_set_scalar(mpq,a[i]->inf,GMP_RNDD) && exact;
      if (i<intdim){
	mpz_fdiv_q(mpq_numref(mpq),mpq_numref(mpq),mpq_denref(mpq));
	mpz_set_ui(mpq_denref(mpq),1);
      }
      r.insert( Constraint( temp.get_den() * Variable(i) >= temp.get_num() ));
      break;
    case -1:
      break;
    case 1:
      r = Constraint_System::zero_dim_empty();
      return true;
    default:
      assert(0);
    }
    /* sup */
    switch (ap_scalar_infty(a[i]->sup)) {
    case 0:
      exact =
	!ap_mpq_set_scalar(mpq,a[i]->sup,GMP_RNDU) && exact;
      if (i<intdim){
	mpz_fdiv_q(mpq_numref(mpq),mpq_numref(mpq),mpq_denref(mpq));
	mpz_set_ui(mpq_denref(mpq),1);
      }
      r.insert( Constraint( temp.get_den() * Variable(i) <= temp.get_num() ));
      break;
    case 1:
      break;
    case -1:
      r = Constraint_System::zero_dim_empty();
      return true;
    default:
      assert(0);
    }
  }
  return exact;
}

/* ap_interval_t box => Congruence_System (return exactness) */
bool ap_ppl_of_box(Congruence_System& r, ap_interval_t** a, size_t intdim, size_t realdim)
{
  bool exact = true;
  size_t i;
  mpq_class temp;
  size_t nb = intdim+realdim;

  r.clear();
  for (i=0;i<nb;i++) {
    int inf = ap_scalar_infty(a[i]->inf);
    int sup = ap_scalar_infty(a[i]->sup);
    /* unsatisfiable */
    if (inf==1 || sup==-1) {
      r = Congruence_System::zero_dim_empty();
      return true;
    }
    /* no-singleton */
    if (inf || sup ||
	!ap_scalar_equal(a[i]->inf,a[i]->sup) ||
	ap_mpq_set_scalar(temp.get_mpq_t(),a[i]->inf,GMP_RNDD)) {
      exact = false;
    }
    /* singleton */
    else {
      /* integerness check */
      mpz_class temp2 = mpz_class(temp);
      if (temp==temp2) r.insert(Constraint(Variable(i)==temp2));
      else exact = false;
    }
  }
  return exact;
}

/* ====================================================================== */
/* Linear expressions */
/* ====================================================================== */

/* Assume a quasilinear expressions,
   with the selected bound of constant coefficient different from +oo
*/
bool ap_ppl_of_itv_linexpr(Linear_Expression& r,
			   mpz_class& den,
			   itv_linexpr_t* linexpr,
			   int mode)
{
  mpq_class temp;
  size_t i;
  ap_dim_t dim;
  itv_ptr pitv;
  bool* peq;

  mpz_set_ui(den.get_mpz_t(),1);
  if (mode>0){
    if (bound_infty(linexpr->cst->sup)){
      throw cannot_convert();
      abort();
    }
    if (bound_sgn(linexpr->cst->sup))
      mpz_set(den.get_mpz_t(),numrat_denref(bound_numref(linexpr->cst->sup)));
  }
  else if (mode < 0){
    if (bound_infty(linexpr->cst->inf)){
      throw cannot_convert();
      abort();
    }
    if (bound_sgn(linexpr->cst->inf))
      mpz_set(den.get_mpz_t(),numrat_denref(bound_numref(linexpr->cst->inf)));
  }
  else {
    assert(0);
  }
  r = Linear_Expression::zero();
  /* compute lcm of denominators in den */
  itv_linexpr_ForeachLinterm(linexpr,i,dim,pitv,peq){
    assert(*peq);
    mpz_lcm(den.get_mpz_t(),den.get_mpz_t(),numrat_denref(bound_numref(pitv->sup)));
  }
  /* add variable coefficients * den */
  itv_linexpr_ForeachLinterm(linexpr,i,dim,pitv,peq){
    mpq_set(temp.get_mpq_t(),bound_numref(pitv->sup));
    temp *= den;
    r += Variable(dim) * temp.get_num();
  }
  /* add constant coefficient * den */
  if (mode>0){
    mpq_set(temp.get_mpq_t(),bound_numref(linexpr->cst->sup));
  } else {
     mpq_neg(temp.get_mpq_t(),bound_numref(linexpr->cst->inf));
  }
  temp *= den;
  r += temp.get_num();
  return true;
}

/* ap_linexpr0_t => ppl */
/* linearize if titv!=NULL */
void ap_ppl_of_linexpr(itv_internal_t* intern,
		       Linear_Expression& r,
		       mpz_class& den,
		       ap_linexpr0_t* c,
		       int mode)
{
  itv_linexpr_t linexpr;

  itv_linexpr_init(&linexpr,0);
  itv_linexpr_set_ap_linexpr0(intern,&linexpr,c);
  ap_ppl_of_itv_linexpr(r,den,&linexpr,mode);
  itv_linexpr_clear(&linexpr);
}

/* ====================================================================== */
/* Constraints */
/* ====================================================================== */

bool ap_ppl_of_itv_lincons(Constraint& r,
			   mpz_class& den,
			   itv_lincons_t* lincons,
			   bool allow_strict)
{
  Linear_Expression l;
  ap_ppl_of_itv_linexpr(l,den,&lincons->linexpr,1);
  switch (lincons->constyp) {
  case AP_CONS_SUPEQ: r = ( l >= 0 ); return true;
  case AP_CONS_EQ:    r = ( l == 0 ); return true;
  case AP_CONS_SUP:
    if (allow_strict) { r = ( l > 0 ); return true; }
    else { r = (l >= 0); return false; }
  case AP_CONS_EQMOD:
    if (numrat_sgn(lincons->num)==0){
      r = ( l == 0 ); return true;
    }
    else {
     throw cannot_convert();
    }
  case AP_CONS_DISEQ:
    throw cannot_convert();
  default:
    throw invalid_argument("Constraint type in ap_ppl_of_lincons");
  }
}

/* ap_lincons0_t => Constraint (may raise cannot_convert, return exact) */
/* congruences are overapproximated as linear equalities */
bool ap_ppl_of_lincons(itv_internal_t* intern,
		       Constraint& r,ap_lincons0_t* c,bool allow_strict)
{
  itv_lincons_t lincons;
  mpz_class den;
  itv_lincons_init(&lincons);
  bool exact = itv_lincons_set_ap_lincons0(intern,&lincons,c);
  exact = ap_ppl_of_itv_lincons(r,den,&lincons,allow_strict) && exact;
  itv_lincons_clear(&lincons);
  return exact;
}

bool ap_ppl_of_itv_lincons(Congruence& r, mpz_class& den, itv_lincons_t* c)
{
  Linear_Expression l;
  if (!itv_linexpr_is_scalar(&c->linexpr)){
    throw cannot_convert();
  }
  ap_ppl_of_itv_linexpr(l,den,&c->linexpr,1);
  switch (c->constyp) {
  case AP_CONS_SUPEQ:
  case AP_CONS_SUP:
  case AP_CONS_DISEQ:
    throw cannot_convert();
  case AP_CONS_EQ:
    r = ( l %= 0 ) / 0;
    break;
  case AP_CONS_EQMOD:
    if (num_sgn(c->num)==0){
      r = ( l %= 0 ) / 0;
    }
    else {
      mpq_class mod;
      num_set(mod.get_mpq_t(),c->num);
      r = ( l * mod.get_den() %= 0) / mod.get_num();
    }
    break;
  default:
    throw invalid_argument("Constraint type in ap_ppl_of_lincons");
  }
  return true;
}

/* ap_lincons0_t => Congruence (may raise cannot_convert, return exact) */
bool ap_ppl_of_lincons(itv_internal_t* intern,
		       Congruence& r,ap_lincons0_t* c)
{
  itv_lincons_t lincons;
  mpz_class den;

  itv_lincons_init(&lincons);
  bool exact = itv_lincons_set_ap_lincons0(intern,&lincons,c);
  exact = ap_ppl_of_itv_lincons(r,den,&lincons) && exact;
  itv_lincons_clear(&lincons);
  return exact;
}

/* ap_lincons0_array_t => Constraint_System
   returns true if exact (some constraint was dropped or approximated) */
bool ap_ppl_of_itv_lincons_array(Constraint_System& r, mpz_class& den, itv_lincons_array_t* a, bool allow_strict)
{
  bool exact = true;
  size_t i;
  Constraint c = Constraint::zero_dim_positivity();
  r.clear();
  for (i=0;i<a->size;i++) {
    try {
      exact = ap_ppl_of_itv_lincons(c,den,&a->p[i],allow_strict) && exact;
      r.insert(c);
    }
    catch (cannot_convert& w) { exact = false; }
  }
  return exact;
}
/* ap_lincons0_array_t => Constraint_System
   returns true if exact (some constraint was dropped or approximated) */
bool ap_ppl_of_lincons_array(itv_internal_t* intern,
			     Constraint_System& r, ap_lincons0_array_t* a,bool allow_strict)
{
  itv_lincons_array_t array;
  mpz_class den;
  itv_lincons_array_init(&array,a->size);
  bool exact = itv_lincons_array_set_ap_lincons0_array(intern,&array,a);
  exact = ap_ppl_of_itv_lincons_array(r,den,&array,allow_strict) && exact;
  itv_lincons_array_clear(&array);
  return exact;
}

/* ap_lincons0_array_t => Constraint_System
   returns true if exact (some constraint was dropped or approximated) */
bool ap_ppl_of_itv_lincons_array(Congruence_System& r, mpz_class& den, itv_lincons_array_t* a)
{
  bool exact = true;
  size_t i;
  Congruence c = Congruence::zero_dim_false();
  r.clear();
  for (i=0;i<a->size;i++) {
    try {
      exact = ap_ppl_of_itv_lincons(c,den,&a->p[i]) && exact;
      r.insert(c);
    }
    catch (cannot_convert& w) { exact = false; }
  }
  return exact;
}
/* ap_lincons0_array_t => Congruence_System
   returns true if exact (some constraint was dropped or approximated) */
bool ap_ppl_of_lincons_array(itv_internal_t* intern,
			     Congruence_System& r,ap_lincons0_array_t* a)
{
  itv_lincons_array_t array;
  mpz_class den;
  itv_lincons_array_init(&array,a->size);
  bool exact = itv_lincons_array_set_ap_lincons0_array(intern,&array,a);
  exact = ap_ppl_of_itv_lincons_array(r,den,&array) && exact;
  itv_lincons_array_clear(&array);
  return exact;
}



/* ap_generator0_t => Generator (may raise cannot_convert, or return true) */
bool ap_ppl_of_generator(itv_internal_t* intern,
			 Generator& r, ap_generator0_t* c)
{
  Linear_Expression l;
  mpz_class den;

  if (!ap_linexpr0_is_linear(c->linexpr0)){
    throw cannot_convert();
  }
  ap_ppl_of_linexpr(intern,l,den,c->linexpr0,1);
  switch (c->gentyp) {
  case AP_GEN_VERTEX:  r = Generator::point(l,den); return true;
  case AP_GEN_RAY:     r = Generator::ray(l);       return true;
  case AP_GEN_LINE:    r = Generator::line(l);      return true;
  case AP_GEN_RAYMOD:  r = Generator::ray(l);       return false;
  case AP_GEN_LINEMOD: r = Generator::line(l);      return false;
  default:
    throw invalid_argument("Generator type in ap_ppl_of_generator");
  }
}

/* Test if the linear part of the generator is 0
   Needed because PPL refuses non-vertex generators with such expressions */

bool ap_ppl_ap_generator0_select(ap_generator0_t* g)
{
  if (g->gentyp==AP_GEN_VERTEX) {
    return true;
  }
  else {
    size_t i;
    ap_dim_t dim;
    ap_coeff_t* coeff;
    ap_linexpr0_t* e = g->linexpr0;

    bool res = false;
    ap_linexpr0_ForeachLinterm(e,i,dim,coeff){
      if (ap_coeff_zero(coeff)==false){
	res = true;
	break;
      }
    }
    return res;
  }
}

/* ap_generator0_array_t => Generator_System
   (may raise cannot_convert, or return false)
*/
bool ap_ppl_of_generator_array(itv_internal_t* intern,
			       Generator_System& r, ap_generator0_array_t* a)
{
  bool exact = true;
  size_t i;
  Generator c = Generator::zero_dim_point();
  r.clear();
  for (i=0;i<a->size;i++) {
    if (ap_ppl_ap_generator0_select(&a->p[i])){
      exact = ap_ppl_of_generator(intern,c,&a->p[i]) && exact;
      r.insert(c);
    }
  }
  return exact;
}

/* ap_generator0_t => Grid_Generator (may raise cannot_convert, or return true) */
bool ap_ppl_of_generator(itv_internal_t* intern,
			 Grid_Generator& r, ap_generator0_t* c)
{
  Linear_Expression l;
  mpz_class den;
  if (!ap_linexpr0_is_linear(c->linexpr0)){
    throw cannot_convert();
  }
  ap_ppl_of_linexpr(intern,l,den,c->linexpr0,1);
  switch (c->gentyp) {
#ifndef PPL_0_9
  case AP_GEN_VERTEX:  r = Grid_Generator::grid_point(l,den);     return true;
  case AP_GEN_RAY:     r = Grid_Generator::grid_line(l);          return false;
  case AP_GEN_LINE:    r = Grid_Generator::grid_line(l);          return true;
#else
  case AP_GEN_VERTEX:  r = Grid_Generator::point(l,den);     return true;
  case AP_GEN_RAY:     r = Grid_Generator::line(l);          return false;
  case AP_GEN_LINE:    r = Grid_Generator::line(l);          return true;
#endif
  case AP_GEN_RAYMOD:  r = Grid_Generator::parameter(l,den); return false;
  case AP_GEN_LINEMOD: r = Grid_Generator::parameter(l,den); return true;
  default:
    throw invalid_argument("Generator type in ap_ppl_of_generator");
  }
}

/* ap_generator0_array_t => Grid_Generator_System
   (may raise cannot_convert, or return true)
*/
bool ap_ppl_of_generator_array(itv_internal_t* intern,
			       Grid_Generator_System& r,ap_generator0_array_t* a)
{
  bool exact = true;
  size_t i;
#ifndef PPL_0_9
  Grid_Generator c = Grid_Generator::grid_point();
#else
  Grid_Generator c = Grid_Generator::point();
#endif
  r.clear();
  for (i=0;i<a->size;i++) {
    if (ap_ppl_ap_generator0_select(&a->p[i])){
      exact = ap_ppl_of_generator(intern,c,&a->p[i]) && exact;
      r.insert(c);
    }
  }
  return exact;
}
