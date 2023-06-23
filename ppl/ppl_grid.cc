/*
 * ppl_grid.cc
 *
 * APRON Library / PPL library wrapper
 *
 * Wrappers specific to the Grid class.
 *
 * Copyright (C) Antoine Mine' 2006
 *
 */

/*
 * This file is part of the APRON Library, released under GPL license.
 * Please read the COPYING file packaged in the distribution.
 */

#include <assert.h>
#include "ap_generic.h"
#include "ap_ppl.h"
#include "ppl_user.hh"
#include "ppl_grid.hh"

#include <ppl.hh>
using namespace std;
using namespace Parma_Polyhedra_Library;

/* ********************************************************************** */
/* General stuff */
/* ********************************************************************** */

/* ====================================================================== */
/* Manager */
/* ====================================================================== */

static inline ppl_internal_t* get_internal(ap_manager_t* man)
{
  return (ppl_internal_t*)man->internal;
}

/* ====================================================================== */
/* PPL_Grid */
/* ====================================================================== */

/* wrapper: adds initdim info */
/* grids are always "reduced" wrt intdim: we keep mod 1 constraints for integer variables */

PPL_Grid::PPL_Grid(const PPL_Grid& x) : p(new Grid(*x.p)), intdim(x.intdim) {}

PPL_Grid::PPL_Grid(size_t intdim,size_t realdim,Degenerate_Element kind)
  : intdim(intdim)
{
  try {
    p = new Grid(intdim+realdim,kind);
    if (kind!=EMPTY) reduce();
  }
  catch (std::logic_error& e) { intdim = 0; p = new Grid(1,kind); }
}

/* enforce integer constraints */
void PPL_Grid::reduce()
{
  for (size_t i=0;i<intdim;i++)
    p->add_congruence((Variable(i) %=0) / 1);
}

void PPL_Grid::forget_dim(size_t dim)
{
#ifndef PPL_0_9
  if (intdim>dim) p->add_grid_generator(Grid_Generator::parameter(Variable(dim)));
  else p->add_grid_generator(Grid_Generator::grid_line(Variable(dim)));
#else
  if (intdim>dim) p->add_generator(Grid_Generator::parameter(Variable(dim)));
  else p->add_generator(Grid_Generator::line(Variable(dim)));
#endif
}

PPL_Grid::~PPL_Grid() { delete p; }

/* ====================================================================== */
/* Error Handlers */
/* ====================================================================== */

/* returns a grid, of specified size if possible */
#define CATCH_WITH_DIM(funid,intdim,realdim)				\
  catch (cannot_convert& w) {						\
    /* bailing out, not an error */					\
    man->result.flag_exact = man->result.flag_best = false;		\
    return new PPL_Grid(intdim,realdim,UNIVERSE);			\
  }									\
  catch (std::logic_error& e) {						\
    /* actual error */							\
    ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,funid,e.what()); \
    return new PPL_Grid(intdim,realdim,UNIVERSE);			\
  }

/* returns a grid, with size compatible with grid if possible */
#define CATCH_WITH_GRID(funid,grid)					\
  CATCH_WITH_DIM(funid,grid->intdim,grid->p->space_dimension()-grid->intdim)

/* returns v */
#define CATCH_WITH_VAL(funid,v)						\
  catch (cannot_convert& w) {						\
    /* bailing out, not an error */					\
    man->result.flag_exact = man->result.flag_best = false;		\
    return v;								\
  }									\
  catch (std::logic_error& e) {						\
    /* actual error */							\
    ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,funid,e.what()); \
    return v;								\
  }

/* returns nothing */
#define CATCH_WITH_VOID(funid) CATCH_WITH_VAL(funid,)

/* prints message */
#define CATCH_WITH_MSG(funid)						\
  catch (cannot_convert& w) {						\
    /* bailing out, not an error */					\
    fprintf(stream,"!exception!");					\
  }									\
  catch (std::logic_error& e) {						\
    /* actual error */							\
    ap_manager_raise_exception(man,AP_EXC_INVALID_ARGUMENT,funid,e.what()); \
    fprintf(stream,"!exception!");					\
  }


/* ====================================================================== */
/* utility shared by _bound_dimension, _to_box, & _sat_interval (exact) */
/* ====================================================================== */
static void ap_ppl_grid_bound_dim(ap_interval_t* r,PPL_Grid* a,int dim)
{
  Coefficient sup_n,sup_d,inf_n,inf_d;
  Linear_Expression l = Variable(dim);
  bool ok;
  /* sup bound */
  if (a->p->maximize(l,sup_n,sup_d,ok))
    ap_ppl_mpz2_to_scalar(r->sup,sup_n,sup_d);
  else ap_scalar_set_infty(r->sup,1);
  /* inf bound */
  if (a->p->minimize(l,inf_n,inf_d,ok))
    ap_ppl_mpz2_to_scalar(r->inf,inf_n,inf_d);
  else ap_scalar_set_infty(r->inf,-1);
}

static itv_t* ap_ppl_grid_to_itv_array(PPL_Grid* a)
{
  Coefficient sup_n,sup_d;
  Linear_Expression l;
  bool ok;
  size_t i,nb;

  nb = a->p->space_dimension();
  itv_t* env = itv_array_alloc(nb);
  for (i=0; i<nb; i++){
    l = Variable(i);
    /* sup bound */
    if (a->p->maximize(l,sup_n,sup_d,ok)){
      bound_set_int(env[i]->sup,0);
      numrat_set_numint2(env[i]->sup,sup_n.get_mpz_t(),sup_d.get_mpz_t());
    }
    else
      bound_set_infty(env[i]->sup,1);
    /* inf bound */
    if (a->p->minimize(l,sup_n,sup_d,ok)){
      bound_set_int(env[i]->inf,0);
      numrat_set_numint2(env[i]->inf,sup_n.get_mpz_t(),sup_d.get_mpz_t());
      numrat_neg(env[i]->inf,env[i]->inf);
    }
    else
      bound_set_infty(env[i]->inf,1);
  }
  return env;
}


/* ********************************************************************** */
/* I. General management */
/* ********************************************************************** */

/* ============================================================ */
/* I.1 Memory */
/* ============================================================ */

extern "C"
PPL_Grid* ap_ppl_grid_copy(ap_manager_t* man, PPL_Grid* a)
{
  man->result.flag_exact = man->result.flag_best = true;
  try {
    return new PPL_Grid(*a);
  }
  CATCH_WITH_GRID(AP_FUNID_COPY,a);
}

extern "C"
void ap_ppl_grid_free(ap_manager_t* man, PPL_Grid* a)
{
  man->result.flag_exact = man->result.flag_best = true;
  try {
    delete a;
  }
  CATCH_WITH_VOID(AP_FUNID_FREE);
}

extern "C"
size_t ap_ppl_grid_size(ap_manager_t* man, PPL_Grid* a)
{
  man->result.flag_exact = man->result.flag_best = true;
  try {
    return a->p->total_memory_in_bytes();
  }
  CATCH_WITH_VAL(AP_FUNID_ASIZE,0);
}

/* ============================================================ */
/* I.2 Control of internal representation */
/* ============================================================ */

extern "C"
void ap_ppl_grid_minimize(ap_manager_t* man, PPL_Grid* a)
{
  man->result.flag_exact = man->result.flag_best = true;
  try {
    /* re-add integer constraints (should not be needed except after a widening) */
    ((PPL_Grid*)a)->reduce();
    /* the calls force in-place minimisation */
    (void)a->p->minimized_congruences();
#ifndef PPL_0_9
    (void)a->p->minimized_grid_generators();
#else
    (void)a->p->minimized_generators();
#endif
  }
  CATCH_WITH_VOID(AP_FUNID_MINIMIZE);
}

extern "C"
void ap_ppl_grid_canonicalize(ap_manager_t* man, PPL_Grid* a)
{
  man->result.flag_exact = man->result.flag_best = true;
  try {
    /* re-add integer constraints (should not be needed except after a widening) */
    ((PPL_Grid*)a)->reduce();
    /* the calls force in-place minimisation */
    (void)a->p->minimized_congruences();
#ifndef PPL_0_9
    (void)a->p->minimized_grid_generators();
#else
    (void)a->p->minimized_generators();
#endif
  }
  CATCH_WITH_VOID(AP_FUNID_CANONICALIZE);
}
extern "C"
int ap_ppl_grid_hash(ap_manager_t* man, PPL_Grid* a)
{
  man->result.flag_exact = man->result.flag_best = true;
  try {
    ((PPL_Grid*)a)->reduce();
    /* the calls force in-place minimisation */
    (void)a->p->minimized_congruences();
#ifndef PPL_0_9
    (void)a->p->minimized_grid_generators();
#else
    (void)a->p->minimized_generators();
#endif
    return a->p->total_memory_in_bytes();
  }
  CATCH_WITH_VAL(AP_FUNID_HASH,0);
}

extern "C"
void ap_ppl_grid_approximate(ap_manager_t* man, PPL_Grid* a, int algorithm)
{
  man->result.flag_exact = man->result.flag_best = true;
  return;
}

/* ============================================================ */
/* I.3 Printing */
/* ============================================================ */

extern "C"
void ap_ppl_grid_fprint(FILE* stream,
			ap_manager_t* man,
			PPL_Grid* a,
			char** name_of_dim)
{
  man->result.flag_exact = man->result.flag_best = true;
  try {
    if (a->p->is_empty()){
      ap_dimension_t dim = ap_ppl_grid_dimension(man,a);
      fprintf(stream,"empty grid of dim (%lu,%lu)\n",
	      (unsigned long)dim.intdim,(unsigned long)dim.realdim);
    }
    else {
      ap_lincons0_array_t ar = ap_ppl_to_lincons_array(a->p->minimized_congruences());
      ap_lincons0_array_fprint(stream,&ar,name_of_dim);
      ap_lincons0_array_clear(&ar);
    }
  }
  CATCH_WITH_MSG(AP_FUNID_FPRINT);
}

/* NOT IMPLEMENTED! */
extern "C"
void ap_ppl_grid_fprintdiff(FILE* stream,
			     ap_manager_t* man,
			     PPL_Grid* a1, PPL_Grid* a2,
			     char** name_of_dim)
{
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_FPRINTDIFF,"not implemented");
}

/* fdump will print the unminimized contraint & generator systems */
extern "C"
void ap_ppl_grid_fdump(FILE* stream, ap_manager_t* man, PPL_Grid* a)
{
  man->result.flag_exact = man->result.flag_best = true;
  try {
    /* dump constraints */
    fprintf(stream,"constraints: ");
    ap_lincons0_array_t ar = ap_ppl_to_lincons_array(a->p->congruences());
    ap_lincons0_array_fprint(stream,&ar,NULL);
    ap_lincons0_array_clear(&ar);
    /* dump generators */
    fprintf(stream,"generators: ");
#ifndef PPL_0_9
    ap_generator0_array_t ar2 = ap_ppl_to_generator_array(a->p->grid_generators());
#else
    ap_generator0_array_t ar2 = ap_ppl_to_generator_array(a->p->generators());
#endif
    ap_generator0_array_fprint(stream,&ar2,NULL);
    ap_generator0_array_clear(&ar2);
  }
  CATCH_WITH_MSG(AP_FUNID_FPRINTDIFF);
}

/* ============================================================ */
/* I.4 Serialization */
/* ============================================================ */

/* NOT IMPLEMENTED! */
extern "C"
ap_membuf_t ap_ppl_grid_serialize_raw(ap_manager_t* man, PPL_Grid* a)
{
  ap_membuf_t membuf;
  membuf.ptr = NULL;
  membuf.size = 0;
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_SERIALIZE_RAW,"not implemented");
  return membuf;
}

/* NOT IMPLEMENTED! */
extern "C"
PPL_Grid* ap_ppl_grid_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size)
{
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,AP_FUNID_DESERIALIZE_RAW,"not implemented");
  return NULL;
}

/* ********************************************************************** */
/* II. Constructor, accessors, tests and property extraction */
/* ********************************************************************** */

extern "C"
PPL_Grid* ap_ppl_grid_bottom(ap_manager_t* man, size_t intdim, size_t realdim)
{
  man->result.flag_exact = man->result.flag_best = true;
  try {
    return new PPL_Grid(intdim,realdim,EMPTY);
  }
  CATCH_WITH_DIM(AP_FUNID_BOTTOM,intdim,realdim);
}

extern "C"
PPL_Grid* ap_ppl_grid_top(ap_manager_t* man, size_t intdim, size_t realdim)
{
  man->result.flag_exact = man->result.flag_best = true;
  try {
    return new PPL_Grid(intdim,realdim,UNIVERSE);
  }
  CATCH_WITH_DIM(AP_FUNID_TOP,intdim,realdim);
}

extern "C"
PPL_Grid* ap_ppl_grid_of_box(ap_manager_t* man,
			     size_t intdim, size_t realdim,
			     ap_interval_t** tinterval)
{
  man->result.flag_exact = false;
  man->result.flag_best = true;
  try {
    PPL_Grid* r = new PPL_Grid(intdim,realdim,UNIVERSE);
    Congruence_System c;
    if (!ap_ppl_of_box(c,tinterval,intdim,realdim))
      man->result.flag_exact = man->result.flag_best = false;
    r->p->add_recycled_congruences(c);
    return r;
  }
  CATCH_WITH_DIM(AP_FUNID_OF_BOX,intdim,realdim);
}

/* ============================================================ */
/* II.2 Accessors */
/* ============================================================ */

extern "C"
ap_dimension_t ap_ppl_grid_dimension(ap_manager_t* man, PPL_Grid* a)
{
  man->result.flag_exact = man->result.flag_best = true;
  ap_dimension_t d = { 0, 0 };
  try {
    d.intdim = a->intdim;
    d.realdim = a->p->space_dimension()-a->intdim;
    return d;
  }
  CATCH_WITH_VAL(AP_FUNID_DIMENSION,d);
}

/* ============================================================ */
/* II.3 Tests */
/* ============================================================ */

extern "C"
bool ap_ppl_grid_is_bottom(ap_manager_t* man, PPL_Grid* a)
{
  man->result.flag_exact = man->result.flag_best = true;
  try {
    return a->p->is_empty();
  }
  CATCH_WITH_VAL(AP_FUNID_IS_BOTTOM,false);
}

extern "C"
bool ap_ppl_grid_is_top(ap_manager_t* man, PPL_Grid* a)
{
  man->result.flag_exact = man->result.flag_best = true;
  try {
    return a->p->is_universe();
  }
  CATCH_WITH_VAL(AP_FUNID_IS_TOP,false);
}

extern "C"
bool ap_ppl_grid_is_leq(ap_manager_t* man, PPL_Grid* a1,
			PPL_Grid* a2)
{
  man->result.flag_exact = man->result.flag_best = true;
  try {
    return a2->p->contains(*a1->p);
  }
  CATCH_WITH_VAL(AP_FUNID_IS_LEQ,false);
}

extern "C"
bool ap_ppl_grid_is_eq(ap_manager_t* man, PPL_Grid* a1,
		       PPL_Grid* a2)
{
  man->result.flag_exact = man->result.flag_best = true;
  try {
    return *a1->p == *a2->p;
  }
  CATCH_WITH_VAL(AP_FUNID_IS_EQ,false);
}

extern "C"
bool ap_ppl_grid_sat_lincons(ap_manager_t* man, PPL_Grid* a,
			     ap_lincons0_t* lincons0)
{
  ppl_internal_t* intern = get_internal(man);
  man->result.flag_exact = man->result.flag_best = true;

  try {
    if (a->p->is_empty()) return true;
    else {
      itv_lincons_t lincons;
      Linear_Expression l;
      mpz_class den;
      Constraint r = Constraint::zero_dim_positivity();
      bool exact = true;

      switch (lincons0->constyp) {
      case AP_CONS_EQ:
      case AP_CONS_EQMOD:
	if (!ap_linexpr0_is_linear(lincons0->linexpr0)){
	  man->result.flag_exact = man->result.flag_best = false;
	  return false;
	}
        // fall through
      case AP_CONS_SUPEQ:
      case AP_CONS_SUP:
	itv_lincons_init(&lincons);
	exact = itv_lincons_set_ap_lincons0(intern->itv,&lincons,lincons0) && exact;
	if (itv_sat_lincons_is_false(intern->itv,&lincons)){
	  itv_lincons_clear(&lincons);
	  if (!exact)
	    man->result.flag_exact = man->result.flag_best = false;
	  return false;
	}
	if (!itv_lincons_is_quasilinear(&lincons)){
	  itv_t* env = ap_ppl_grid_to_itv_array(a);
	  exact = itv_quasilinearize_lincons(intern->itv,&lincons,env,false) && exact;
	  itv_array_free(env,a->p->space_dimension());
	}
	if (itv_sat_lincons_is_false(intern->itv,&lincons)){
	  itv_lincons_clear(&lincons);
	  man->result.flag_exact = man->result.flag_best = true;
	  return false;
	}
	assert (!bound_infty(lincons.linexpr.cst->inf));
	exact = ap_ppl_of_itv_linexpr(l,den,&lincons.linexpr,-1) && exact;
	itv_lincons_clear(&lincons);
	switch (lincons.constyp){
	case AP_CONS_SUPEQ:
	  r = (l>=0); 
          break;
	case AP_CONS_SUP:
          exact = false;
          r = (l>=0); 
          break;
	case AP_CONS_EQMOD:
	  exact = exact && (num_sgn(lincons.num)==0);
          // fall through
	case AP_CONS_EQ:
	  r = (l == 0);
	  break;
	default:
	  assert(0);
	}
	break;
      default:
	man->result.flag_exact = man->result.flag_best = false;
	return false;
      }
      if (!exact)
	man->result.flag_exact = man->result.flag_best = false;
      Poly_Con_Relation relation = a->p->relation_with(r);
      return relation.implies(Poly_Con_Relation::is_included());
    }
  }
  CATCH_WITH_VAL(AP_FUNID_SAT_LINCONS,false);
}

extern "C"
bool ap_ppl_grid_sat_tcons(ap_manager_t* man, PPL_Grid* a,
			   ap_tcons0_t* cons)
{
  return ap_generic_sat_tcons(man,a,cons,AP_SCALAR_MPQ,true);
}

extern "C"
bool ap_ppl_grid_sat_interval(ap_manager_t* man, PPL_Grid* a,
			      ap_dim_t dim, ap_interval_t* i)
{
  ap_interval_t* ig = ap_interval_alloc();
  man->result.flag_exact = man->result.flag_best = false;
  try {
    ap_ppl_grid_bound_dim(ig,a,dim);
    bool r = ap_interval_is_leq(ig,i);
    ap_interval_free(ig);
    return r || a->p->is_empty();
  }
  CATCH_WITH_VAL(AP_FUNID_SAT_INTERVAL,(ap_interval_free(ig),false));
}

extern "C"
bool ap_ppl_grid_is_dimension_unconstrained(ap_manager_t* man,
					    PPL_Grid* a,
					    ap_dim_t dim)
{
  man->result.flag_exact = man->result.flag_best = true;
  try {
#ifndef PPL_0_9
    Grid_Generator g = Grid_Generator::grid_line(Variable(dim));
#else
    Grid_Generator g = Grid_Generator::line(Variable(dim));
#endif
    return a->p->relation_with(g) == Poly_Gen_Relation::subsumes();
  }
  CATCH_WITH_VAL(AP_FUNID_IS_DIMENSION_UNCONSTRAINED,false);
}

/* ============================================================ */
/* II.4 Extraction of properties */
/* ============================================================ */

extern "C"
ap_interval_t* ap_ppl_grid_bound_linexpr(ap_manager_t* man,
					 PPL_Grid* a,
					 ap_linexpr0_t* expr)
{
  ppl_internal_t* intern = get_internal(man);
  man->result.flag_exact = man->result.flag_best = true;
  ap_interval_t* r = ap_interval_alloc();

  try {
    if (a->p->is_empty()) {
      /* empty */
      ap_interval_set_bottom(r);
    }
    else {
      /* not empty */
      itv_linexpr_t linexpr;
      Coefficient sup_n,sup_d;
      Linear_Expression l;
      mpz_class den;
      bool ok;
      bool exact = true;

      itv_linexpr_init(&linexpr,0);
      exact = itv_linexpr_set_ap_linexpr0(intern->itv,&linexpr,expr) && exact;
      if (!itv_linexpr_is_quasilinear(&linexpr)){
	itv_t* env = ap_ppl_grid_to_itv_array(a);
	exact = itv_quasilinearize_linexpr(intern->itv,&linexpr,env,false);
	itv_array_free(env,a->p->space_dimension());
      }
      if (linexpr.size==0){
	ap_interval_set_itv(intern->itv,r,linexpr.cst);
      }
      else {
	/* sup bound */
	if (bound_infty(linexpr.cst->sup)){
	  ap_scalar_set_infty(r->sup,1);
	}
	else {
	  ap_ppl_of_linexpr(intern->itv,l,den,expr,1);
	  if (a->p->maximize(l,sup_n,sup_d,ok)) {
	    sup_d *= den;
	    ap_ppl_mpz2_to_scalar(r->sup,sup_n,sup_d);
	  }
	  else ap_scalar_set_infty(r->sup,1);
	}
	/* inf bound */
	if (bound_infty(linexpr.cst->inf)){
	  ap_scalar_set_infty(r->inf,-1);
	}
	else {
	  if (!itv_linexpr_is_scalar(&linexpr)){
	    ap_ppl_of_itv_linexpr(l,den,&linexpr,-1);
	  }
	  if (a->p->minimize(l,sup_n,sup_d,ok)) {
	    sup_d *= den;
	    ap_ppl_mpz2_to_scalar(r->inf,sup_n,sup_d);
	  }
	  else {
	    ap_scalar_set_infty(r->inf,-1);
	  }
	}
      }
      if (!exact)
	man->result.flag_exact = man->result.flag_best = false;
      itv_linexpr_clear(&linexpr);
    }
  }
  CATCH_WITH_VAL(AP_FUNID_BOUND_LINEXPR,(ap_interval_set_top(r),r));
  return r;
}

extern "C"
ap_interval_t* ap_ppl_grid_bound_texpr(ap_manager_t* man,
				       PPL_Grid* a,
				       ap_texpr0_t* expr)
{
  return ap_generic_bound_texpr(man,a,expr,AP_SCALAR_MPQ,true);
}

extern "C"
ap_interval_t* ap_ppl_grid_bound_dimension(ap_manager_t* man,
					   PPL_Grid* a, ap_dim_t dim)
{
  man->result.flag_exact = man->result.flag_best = true;
  ap_interval_t* r = ap_interval_alloc();
  try {
    if (a->p->is_empty()) ap_interval_set_bottom(r); /* empty */
    else ap_ppl_grid_bound_dim(r,a,dim);         /* not empty */
    return r;
  }
  CATCH_WITH_VAL(AP_FUNID_BOUND_DIMENSION,(ap_interval_set_top(r),r));
}

extern "C"
ap_lincons0_array_t ap_ppl_grid_to_lincons_array(ap_manager_t* man,
						 PPL_Grid* a)
{
  man->result.flag_exact = man->result.flag_best = true;
  try {
    return ap_ppl_to_lincons_array(a->p->congruences());
  }
  CATCH_WITH_VAL(AP_FUNID_TO_LINCONS_ARRAY,ap_lincons0_array_make(0));
}

extern "C"
ap_tcons0_array_t ap_ppl_grid_to_tcons_array(ap_manager_t* man,
					     PPL_Grid* a)
{
  return ap_generic_to_tcons_array(man,a);
}

extern "C"
ap_interval_t** ap_ppl_grid_to_box(ap_manager_t* man, PPL_Grid* a)
{
  man->result.flag_exact = man->result.flag_best = true;
  size_t dim = a->p->space_dimension();
  ap_interval_t** in = ap_interval_array_alloc(dim);
  try {
    if (a->p->is_empty()){
      /* empty */
      for (size_t i=0;i<dim;i++){
	ap_interval_set_bottom(in[i]);
      }
    }
    else {
      /* not empty */
      for (size_t i=0;i<dim;i++){
	ap_ppl_grid_bound_dim(in[i],a,i);
      }
    }
    return in;
  }
  CATCH_WITH_VAL(AP_FUNID_TO_BOX,(ap_ppl_box_universe(in,dim),in));
}

extern "C"
ap_generator0_array_t ap_ppl_grid_to_generator_array(ap_manager_t* man,
						     PPL_Grid* a)
{
  man->result.flag_exact = man->result.flag_best = true;
  try {
#ifndef PPL_0_9
    return ap_ppl_to_generator_array(a->p->grid_generators());
#else
    return ap_ppl_to_generator_array(a->p->generators());
#endif
  }
  CATCH_WITH_VAL(AP_FUNID_TO_GENERATOR_ARRAY,ap_ppl_generator_universe(a->p->space_dimension()));
}

/* ********************************************************************** */
/* III. Operations */
/* ********************************************************************** */

/* ============================================================ */
/* III.1 Meet and Join */
/* ============================================================ */

extern "C"
PPL_Grid* ap_ppl_grid_meet(ap_manager_t* man, bool destructive,
			   PPL_Grid* a1, PPL_Grid* a2)
{
  man->result.flag_exact = man->result.flag_best = true;
  try {
    PPL_Grid* r = destructive ? a1 : new PPL_Grid(*a1);
    r->p->intersection_assign(*a2->p);
    return r;
  }
  CATCH_WITH_GRID(AP_FUNID_MEET,a1);
}

extern "C"
PPL_Grid* ap_ppl_grid_join(ap_manager_t* man, bool destructive,
			   PPL_Grid* a1, PPL_Grid* a2)
{
  man->result.flag_exact = false;
  man->result.flag_best = true;
  try {
    PPL_Grid* r = destructive ? a1 : new PPL_Grid(*a1);
    /* TODO: should we use join_assign_if_exact? */
#ifndef PPL_0_9
    r->p->upper_bound_assign(*a2->p);
#else
    r->p->join_assign(*a2->p);
#endif
    return r;
  }
  CATCH_WITH_GRID(AP_FUNID_JOIN,a1);
}

extern "C"
PPL_Grid* ap_ppl_grid_meet_array(ap_manager_t* man,
				 PPL_Grid** tab, size_t size)
{
  assert(size>=1);
  man->result.flag_exact = man->result.flag_best = true;
  try {
    PPL_Grid* r = new PPL_Grid(*tab[0]);
    for (size_t i=1;i<size;i++)
      r->p->intersection_assign(*tab[i]->p);
    return r;
  }
  CATCH_WITH_GRID(AP_FUNID_MEET_ARRAY,tab[0]);
}

extern "C"
PPL_Grid* ap_ppl_grid_join_array(ap_manager_t* man,
				 PPL_Grid** tab, size_t size)
{
  assert(size>=1);
  man->result.flag_exact = false;
  man->result.flag_best = true;
  try {
    PPL_Grid* r = new PPL_Grid(*tab[0]);
    for (size_t i=1;i<size;i++)
      /* TODO: should we use join_assign_if_exact? */
#ifndef PPL_0_9
      r->p->upper_bound_assign(*tab[i]->p);
#else
      r->p->join_assign(*tab[i]->p);
#endif
  return r;
  }
  CATCH_WITH_GRID(AP_FUNID_JOIN_ARRAY,tab[0]);
}

extern "C"
PPL_Grid* ap_ppl_grid_meet_lincons_array(ap_manager_t* man,
					 bool destructive,
					 PPL_Grid* a,
					 ap_lincons0_array_t* array)
{
  ppl_internal_t* intern = get_internal(man);
  man->result.flag_exact = man->result.flag_best = true;
  try {
    PPL_Grid* r = destructive ? a : new PPL_Grid(*a);
    Congruence_System c;
    if (!ap_ppl_of_lincons_array(intern->itv,c,array))
      man->result.flag_exact = man->result.flag_best = false;
    r->p->add_recycled_congruences(c);
    return r;
  }
  CATCH_WITH_GRID(AP_FUNID_MEET_LINCONS_ARRAY,a);
}

extern "C"
PPL_Grid* ap_ppl_grid_meet_tcons_array(ap_manager_t* man,
					 bool destructive,
					 PPL_Grid* a,
					 ap_tcons0_array_t* array)
{
  return (PPL_Grid*)ap_generic_meet_intlinearize_tcons_array(man,destructive, a,
							     array,
							     AP_SCALAR_MPQ, AP_LINEXPR_LINEAR,
							     (void* (*)(ap_manager_t*, bool, void*, ap_lincons0_array_t*))
							     (&ap_ppl_grid_meet_lincons_array));
}

extern "C"
PPL_Grid* ap_ppl_grid_add_ray_array(ap_manager_t* man,
				    bool destructive,
				    PPL_Grid* a,
				    ap_generator0_array_t* array)
{
  ppl_internal_t* intern = get_internal(man);
  man->result.flag_exact = man->result.flag_best = true;
  try {
    PPL_Grid* r = destructive ? a : new PPL_Grid(*a);
    if (!a->p->is_empty()){
      Grid_Generator_System c;
      if (!ap_ppl_of_generator_array(intern->itv,c,array))
	man->result.flag_exact = man->result.flag_best = false;
#ifndef PPL_0_9
      r->p->add_recycled_grid_generators(c);
#else
      r->p->add_recycled_generators(c);
#endif
      r->reduce();
    }
    return r;
  }
  CATCH_WITH_GRID(AP_FUNID_ADD_RAY_ARRAY,a);
}

/* ============================================================ */
/* III.2 Assignement and Substitutions */
/* ============================================================ */

extern "C"
PPL_Grid* ap_ppl_grid_assign_linexpr(ap_manager_t* man,
				     bool destructive,
				     PPL_Grid* org,
				     ap_dim_t dim, ap_linexpr0_t* expr,
				     PPL_Grid* dest)
{
  bool exact;
  ppl_internal_t* intern = get_internal(man);
  man->result.flag_exact = man->result.flag_best = true;
  try {
    PPL_Grid* r = destructive ? org : new PPL_Grid(*org);
    try {
      Linear_Expression e;
      mpz_class den;
      if (!ap_linexpr0_is_linear(expr)){
	throw cannot_convert();
      }
      ap_ppl_of_linexpr(intern->itv,e,den,expr,1);
      r->p->affine_image(Variable(dim),e,den);
      r->reduce();
    }
    catch (cannot_convert& x) {
      /* defaults to forget */
      r->forget_dim(dim);
      if (dim<r->intdim)
	r->reduce();
      man->result.flag_exact = man->result.flag_best = false;
    }
    if (dest) r->p->intersection_assign(*dest->p);
    return r;
  }
  CATCH_WITH_GRID(AP_FUNID_ASSIGN_LINEXPR_ARRAY,org);
}

extern "C"
PPL_Grid* ap_ppl_grid_substitute_linexpr(ap_manager_t* man,
					 bool destructive,
					 PPL_Grid* org,
					 ap_dim_t dim,
					 ap_linexpr0_t* expr,
					 PPL_Grid* dest)
{
  bool exact;
  ppl_internal_t* intern = get_internal(man);
  man->result.flag_exact = man->result.flag_best = true;
  try {
    PPL_Grid* r = destructive ? org : new PPL_Grid(*org);
    try {
      Linear_Expression e;
      mpz_class den;
      if (!ap_linexpr0_is_linear(expr)){
	throw cannot_convert();
      }
      ap_ppl_of_linexpr(intern->itv,e,den,expr,1);
      r->p->affine_preimage(Variable(dim),e,den);
      r->reduce();
    }
    catch (cannot_convert& x) {
      /* defaults to forget */
      r->forget_dim(dim);
      if (dim<r->intdim)
	r->reduce();
      man->result.flag_exact = man->result.flag_best = false;
    }
    if (dest) r->p->intersection_assign(*dest->p);
    return r;
  }
  CATCH_WITH_GRID(AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY,org);
}

extern "C"
PPL_Grid* ap_ppl_grid_assign_linexpr_array(ap_manager_t* man,
					   bool destructive,
					   PPL_Grid* org,
					   ap_dim_t* tdim,
					   ap_linexpr0_t** texpr,
					   size_t size,
					   PPL_Grid* dest)
{
  PPL_Grid* r;
  size_t i;
  bool exact = true;
  for (i=0;i<size;i++){
    if (!ap_linexpr0_is_linear(texpr[i])){
      exact = false;
      break;
    }
  }
  if (size==1)
    r = ap_ppl_grid_assign_linexpr(man,destructive,org,tdim[0],texpr[0],dest);
  else {
    r = (PPL_Grid*)ap_generic_assign_linexpr_array(man, destructive,
						   org,
						   tdim,texpr,size,
						   dest);
  }
  if (!exact || org->intdim)
    man->result.flag_exact = man->result.flag_best = false;
  return r;
}
extern "C"
PPL_Grid* ap_ppl_grid_substitute_linexpr_array(ap_manager_t* man,
					       bool destructive,
					       PPL_Grid* org,
					       ap_dim_t* tdim,
					       ap_linexpr0_t** texpr,
					       size_t size,
					       PPL_Grid* dest)
{
  PPL_Grid* r;
  size_t i;
  bool exact = true;
  for (i=0;i<size;i++){
    if (!ap_linexpr0_is_linear(texpr[i])){
      exact = false;
      break;
    }
  }
  if (size==1)
    r = ap_ppl_grid_substitute_linexpr(man,destructive,org,tdim[0],texpr[0],dest);
  else {
    r = (PPL_Grid*)ap_generic_substitute_linexpr_array(man, destructive,
						       org,
						       tdim,texpr,size,
						       dest);
  }
  if (!exact || org->intdim)
    man->result.flag_exact = man->result.flag_best = false;
  return r;
}

extern "C"
PPL_Grid* ap_ppl_grid_forget_array(ap_manager_t* man,
				   bool destructive,
				   PPL_Grid* a,
				   ap_dim_t* tdim, size_t size,
				   bool project)
{
  man->result.flag_exact = man->result.flag_best = true;
  try {
    PPL_Grid* r = destructive ? a : new PPL_Grid(*a);
    if (!a->p->is_empty()){
      for (size_t i=0;i<size;i++)
	r->forget_dim(tdim[i]);
      if (project) {
	for (size_t i=0;i<size;i++)
	  r->p->add_constraint(Variable(tdim[i])==0);
      }
      else {
	r->reduce();
      }
    }
    return r;
  }
  CATCH_WITH_GRID(AP_FUNID_FORGET_ARRAY,a);
}
extern "C"
PPL_Grid* ap_ppl_grid_assign_texpr_array(ap_manager_t* man,
					 bool destructive,
					 PPL_Grid* org,
					 ap_dim_t* tdim,
					 ap_texpr0_t** texpr,
					 size_t size,
					 PPL_Grid* dest)
{
  return (PPL_Grid*)ap_generic_assign_texpr_array(man, destructive,
						  org,
						  tdim,texpr,size,
						  dest);
}
extern "C"
PPL_Grid* ap_ppl_grid_substitute_texpr_array(ap_manager_t* man,
					     bool destructive,
					     PPL_Grid* org,
					     ap_dim_t* tdim,
					     ap_texpr0_t** texpr,
					     size_t size,
					     PPL_Grid* dest)
{
  return (PPL_Grid*)ap_generic_substitute_texpr_array(man, destructive,
						      org,
						      tdim,texpr,size,
						      dest);
}

/* partial functions used in map_space_dimensions */
class ap_ppl_map {
 protected:
  size_t dom,codom,*tab;
  char* def;
 public:
  ap_ppl_map(size_t dom,size_t codom) : dom(dom), codom(codom)
  {
    tab = new size_t [dom];
    def = new char [dom];
    memset(def,0,dom);
  }
  ~ap_ppl_map() { delete[] tab; delete[] def; }
  void set(dimension_type i,dimension_type j) { tab[i] = j; def[i] = 1; }
  bool has_empty_codomain() const { return codom==0; }
  dimension_type max_in_codomain() const { return codom-1; }
  bool maps(dimension_type i,dimension_type& j) const { j = tab[i]; return def[i]; }
  void do_map(PPL_Grid* r) const { r->p->map_space_dimensions(*this); }
};

extern "C"
PPL_Grid* ap_ppl_grid_add_dimensions(ap_manager_t* man,
				     bool destructive,
				     PPL_Grid* a,
				     ap_dimchange_t* dimchange,
				     bool project)
{
  man->result.flag_exact = man->result.flag_best = true;
  size_t adddim = dimchange->intdim+dimchange->realdim;
  size_t olddim = a->p->space_dimension();
  try {
    PPL_Grid* r = destructive ? a : new PPL_Grid(*a);
    /* add dimensions */
    if (project) r->p->add_space_dimensions_and_project(adddim);
    else r->p->add_space_dimensions_and_embed(adddim);
    /* reorder dimensions */
    ap_ppl_map map = ap_ppl_map(olddim+adddim,olddim+adddim);
    size_t i,j=0;
    for (i=0;i<adddim;i++) {
      for (;j<dimchange->dim[i];j++)
	map.set(j,i+j);
      map.set(olddim+i,i+j);
    }
    for (;j<olddim;j++)
      map.set(j,i+j);
    map.do_map(r);
    r->intdim += dimchange->intdim;
    r->reduce();
    return r;
  }
  CATCH_WITH_DIM(AP_FUNID_ADD_DIMENSIONS,a->intdim+dimchange->intdim,olddim+dimchange->realdim-a->intdim);
}

extern "C"
PPL_Grid* ap_ppl_grid_remove_dimensions(ap_manager_t* man,
					bool destructive,
					PPL_Grid* a,
					ap_dimchange_t* dimchange)
{
  man->result.flag_exact = man->result.flag_best = true;
  size_t deldim = dimchange->intdim+dimchange->realdim;
  size_t olddim = a->p->space_dimension();
  try {
    PPL_Grid* r = destructive ? a : new PPL_Grid(*a);
    /* reorder & drop dimensions */
    ap_ppl_map map = ap_ppl_map(olddim,olddim-deldim);
    for (size_t j=0,i=0;j<olddim;j++)
      if (i<deldim && j==dimchange->dim[i]) i++;
      else map.set(j,j-i);
    map.do_map(r);
    r->intdim -= dimchange->intdim;
    return r;
  }
  CATCH_WITH_DIM(AP_FUNID_REMOVE_DIMENSIONS,a->intdim-dimchange->intdim,olddim-dimchange->realdim-a->intdim);
}

extern "C"
PPL_Grid* ap_ppl_grid_permute_dimensions(ap_manager_t* man,
					 bool destructive,
					 PPL_Grid* a,
					 ap_dimperm_t* perm)
{
  man->result.flag_exact = false;
  man->result.flag_best = true;
  try {
    PPL_Grid* r = destructive ? a : new PPL_Grid(*a);
    ap_ppl_map map = ap_ppl_map(perm->size,perm->size);
    for (size_t i=0;i<perm->size;i++)
      map.set(i,perm->dim[i]);
    map.do_map(r);
    return r;
  }
  CATCH_WITH_GRID(AP_FUNID_PERMUTE_DIMENSIONS,a);
}

extern "C"
PPL_Grid* ap_ppl_grid_expand(ap_manager_t* man,
			     bool destructive,
			     PPL_Grid* a,
			     ap_dim_t dim,
			     size_t n)
{
  man->result.flag_exact = man->result.flag_best = true;
  try {
    size_t olddim = a->p->space_dimension();
    PPL_Grid* r = destructive ? a : new PPL_Grid(*a);
    r->p->expand_space_dimension(Variable(dim),n);
    if (dim<r->intdim) {
      /* expanded an integer dimension => needs some reordering */
      ap_ppl_map map = ap_ppl_map(olddim+n,olddim+n);
      size_t i;
      for (i=0;i<r->intdim;i++)
	map.set(i,i);
      for (i=0;i<n;i++)
	map.set(olddim+i,r->intdim+i);
      for (i=r->intdim;i<olddim;i++)
	map.set(i,i+n);
      map.do_map(r);
      r->intdim += n;
    }
    return r;
  }
  CATCH_WITH_DIM(AP_FUNID_EXPAND,a->intdim,a->p->space_dimension()-a->intdim+n);
}

extern "C"
PPL_Grid* ap_ppl_grid_fold(ap_manager_t* man,
			   bool destructive,
			   PPL_Grid* a,
			   ap_dim_t* tdim,
			   size_t size)
{
  man->result.flag_exact = false;
  man->result.flag_best = true;
  try {
    PPL_Grid* r = destructive ? a : new PPL_Grid(*a);
    Variables_Set s;
    assert(size>0);
    for (size_t i=1;i<size;i++)
      s.insert(Variable(tdim[i]));
    r->p->fold_space_dimensions(s,Variable(tdim[0]));
    if (tdim[0]<a->intdim) r->intdim -= size-1;
    r->reduce();
    return r;
  }
  CATCH_WITH_DIM(AP_FUNID_FOLD,a->intdim,a->p->space_dimension()-a->intdim-size);
}

extern "C"
PPL_Grid* ap_ppl_grid_widening(ap_manager_t* man,
			       PPL_Grid* a1,
			       PPL_Grid* a2)
{
  man->result.flag_exact = man->result.flag_best = false;
  try {
    PPL_Grid* r = new PPL_Grid(*a2);
    r->p->widening_assign(*a1->p);
    r->reduce();
    return r;
  }
  CATCH_WITH_GRID(AP_FUNID_WIDENING,a1);
}

extern "C"
PPL_Grid* ap_ppl_grid_widening_threshold(ap_manager_t* man,
					 PPL_Grid* a1,
					 PPL_Grid* a2,
					 ap_lincons0_array_t* array)
{
  ppl_internal_t* intern = get_internal(man);
  man->result.flag_exact = man->result.flag_best = false;
  try {
    Congruence_System c;
    ap_ppl_of_lincons_array(intern->itv,c,array);
    PPL_Grid* r = new PPL_Grid(*a2);
    r->p->limited_extrapolation_assign(*a1->p,c);
    r->reduce();
    return r;
  }
  CATCH_WITH_GRID(AP_FUNID_WIDENING,a1);
}

extern "C"
PPL_Grid* ap_ppl_grid_closure(ap_manager_t* man, bool destructive, PPL_Grid* a)
{
  man->result.flag_exact = man->result.flag_best = true;
  try {
    PPL_Grid* r = destructive ? a : new PPL_Grid(*a);
    r->p->topological_closure_assign();
    return r;
  }
  CATCH_WITH_GRID(AP_FUNID_CLOSURE,a);
}


/* ********************************************************************** */
/* Managers */
/* ********************************************************************** */

extern "C" ap_manager_t* ap_ppl_grid_manager_alloc(void)
{
  int i;
  ppl_internal_t* ppl;
  ap_manager_t* man;

  ppl = ap_ppl_internal_alloc(false);
  man = ap_manager_alloc("PPL::Grid",
			 PPL_VERSION,
			 ppl,
			 &ap_ppl_internal_free);
  assert(man);

  man->funptr[AP_FUNID_COPY] = (void*)ap_ppl_grid_copy;
  man->funptr[AP_FUNID_FREE] = (void*)ap_ppl_grid_free;
  man->funptr[AP_FUNID_ASIZE] = (void*)ap_ppl_grid_size;
  man->funptr[AP_FUNID_MINIMIZE] = (void*)ap_ppl_grid_minimize;
  man->funptr[AP_FUNID_CANONICALIZE] = (void*)ap_ppl_grid_canonicalize;
  man->funptr[AP_FUNID_HASH] = (void*)ap_ppl_grid_hash;
  man->funptr[AP_FUNID_APPROXIMATE] = (void*)ap_ppl_grid_approximate;
  man->funptr[AP_FUNID_FPRINT] = (void*)ap_ppl_grid_fprint;
  man->funptr[AP_FUNID_FPRINTDIFF] = (void*)ap_ppl_grid_fprintdiff;
  man->funptr[AP_FUNID_FDUMP] = (void*)ap_ppl_grid_fdump;
  man->funptr[AP_FUNID_SERIALIZE_RAW] = (void*)ap_ppl_grid_serialize_raw;
  man->funptr[AP_FUNID_DESERIALIZE_RAW] = (void*)ap_ppl_grid_deserialize_raw;
  man->funptr[AP_FUNID_BOTTOM] = (void*)ap_ppl_grid_bottom;
  man->funptr[AP_FUNID_TOP] = (void*)ap_ppl_grid_top;
  man->funptr[AP_FUNID_OF_BOX] = (void*)ap_ppl_grid_of_box;
  man->funptr[AP_FUNID_DIMENSION] = (void*)ap_ppl_grid_dimension;
  man->funptr[AP_FUNID_IS_BOTTOM] = (void*)ap_ppl_grid_is_bottom;
  man->funptr[AP_FUNID_IS_TOP] = (void*)ap_ppl_grid_is_top;
  man->funptr[AP_FUNID_IS_LEQ] = (void*)ap_ppl_grid_is_leq;
  man->funptr[AP_FUNID_IS_EQ] = (void*)ap_ppl_grid_is_eq;
  man->funptr[AP_FUNID_IS_DIMENSION_UNCONSTRAINED] = (void*)ap_ppl_grid_is_dimension_unconstrained;
  man->funptr[AP_FUNID_SAT_INTERVAL] = (void*)ap_ppl_grid_sat_interval;
  man->funptr[AP_FUNID_SAT_LINCONS] = (void*)ap_ppl_grid_sat_lincons;
  man->funptr[AP_FUNID_SAT_TCONS] = (void*)ap_ppl_grid_sat_tcons;
  man->funptr[AP_FUNID_BOUND_DIMENSION] = (void*)ap_ppl_grid_bound_dimension;
  man->funptr[AP_FUNID_BOUND_LINEXPR] = (void*)ap_ppl_grid_bound_linexpr;
  man->funptr[AP_FUNID_BOUND_TEXPR] = (void*)ap_ppl_grid_bound_texpr;
  man->funptr[AP_FUNID_TO_BOX] = (void*)ap_ppl_grid_to_box;
  man->funptr[AP_FUNID_TO_LINCONS_ARRAY] = (void*)ap_ppl_grid_to_lincons_array;
  man->funptr[AP_FUNID_TO_TCONS_ARRAY] = (void*)ap_ppl_grid_to_tcons_array;
  man->funptr[AP_FUNID_TO_GENERATOR_ARRAY] = (void*)ap_ppl_grid_to_generator_array;
  man->funptr[AP_FUNID_MEET] = (void*)ap_ppl_grid_meet;
  man->funptr[AP_FUNID_MEET_ARRAY] = (void*)ap_ppl_grid_meet_array;
  man->funptr[AP_FUNID_MEET_LINCONS_ARRAY] = (void*)ap_ppl_grid_meet_lincons_array;
  man->funptr[AP_FUNID_MEET_TCONS_ARRAY] = (void*)ap_ppl_grid_meet_tcons_array;
  man->funptr[AP_FUNID_JOIN] = (void*)ap_ppl_grid_join;
  man->funptr[AP_FUNID_JOIN_ARRAY] = (void*)ap_ppl_grid_join_array;
  man->funptr[AP_FUNID_ADD_RAY_ARRAY] = (void*)ap_ppl_grid_add_ray_array;
  man->funptr[AP_FUNID_ASSIGN_LINEXPR_ARRAY] = (void*)ap_ppl_grid_assign_linexpr_array;
  man->funptr[AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY] = (void*)ap_ppl_grid_substitute_linexpr_array;
  man->funptr[AP_FUNID_ASSIGN_TEXPR_ARRAY] = (void*)ap_ppl_grid_assign_texpr_array;
  man->funptr[AP_FUNID_SUBSTITUTE_TEXPR_ARRAY] = (void*)ap_ppl_grid_substitute_texpr_array;
  man->funptr[AP_FUNID_ADD_DIMENSIONS] = (void*)ap_ppl_grid_add_dimensions;
  man->funptr[AP_FUNID_REMOVE_DIMENSIONS] = (void*)ap_ppl_grid_remove_dimensions;
  man->funptr[AP_FUNID_PERMUTE_DIMENSIONS] = (void*)ap_ppl_grid_permute_dimensions;
  man->funptr[AP_FUNID_FORGET_ARRAY] = (void*)ap_ppl_grid_forget_array;
  man->funptr[AP_FUNID_EXPAND] = (void*)ap_ppl_grid_expand;
  man->funptr[AP_FUNID_FOLD] = (void*)ap_ppl_grid_fold;
  man->funptr[AP_FUNID_WIDENING] = (void*)ap_ppl_grid_widening;
  man->funptr[AP_FUNID_CLOSURE] = (void*)ap_ppl_grid_closure;

  for (i=0;i<AP_EXC_SIZE;i++)
    ap_manager_set_abort_if_exception(man,(ap_exc_t)i,false);

  return man;
}
