/* ********************************************************************** */
/* itvp_predicate.c: predicates on interval polyhedra */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include "itvp.h"
#include "itvp_internal.h"
#include "ap_generic.h"
#include "itvp_domain.h"


/* ====================================================================== */
/* Emptiness test */
/* ====================================================================== */

bool itvp_is_bottom(ap_manager_t* man, itvp_t* a)
{
  fprintf(stdout, "AP_FUNID_IS_BOTTOM\n");
  fflush(stdout);
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_IS_BOTTOM,0);
  if (a->flag==EMPTY_POL){
    man->result.flag_exact = man->result.flag_best = true;
    return true;
  }
  return false;
}

/* ====================================================================== */
/* Universe test */
/* ====================================================================== */

bool itvp_is_top(ap_manager_t* man, itvp_t* a)
{
  fprintf(stdout, "AP_FUNID_IS_TOP\n");
  fflush(stdout);
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_IS_TOP,0);
  man->result.flag_exact = man->result.flag_best = false;

  if (a->flag==UNIVERSE_POL){
    return true;
  }
  return false;
}

/* ====================================================================== */
/* Inclusion test */
/* ====================================================================== */
bool itvp_is_leq(ap_manager_t* man, itvp_t* a1, itvp_t* a2)
{
  fprintf(stdout, "AP_FUNID_IS_LEQ\n");
  fflush(stdout);
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_IS_LEQ,0);

  man->result.flag_exact = man->result.flag_best = false;
  if (pr->funopt->algorithm >= 0){
  }
  else{
  }

  bool res=itvpol_inclusion_byILP(a1, a2);

  return res;
}

/* ====================================================================== */
/* Equality test */
/* ====================================================================== */

bool itvp_is_eq(ap_manager_t* man, itvp_t* a1, itvp_t* a2)
{
  fprintf(stdout, "AP_FUNID_IS_EQ\n");
  fflush(stdout);
  itvp_init_from_manager(man,AP_FUNID_IS_EQ,0);

  man->result.flag_exact = man->result.flag_best =
    (a1->intdim==0);

  return itvpol_is_eq_bySyntactic(a1,a2);
}

/* ====================================================================== */
/* Satisfiability of a linear constraint */
/* ====================================================================== */

bool itvp_sat_lincons(ap_manager_t* man, itvp_t* a, ap_lincons0_t* lincons0)
{
  bool exact;
  bool sat;
  size_t dim;
  itvp_internal_t* itvp = itvp_init_from_manager(man,AP_FUNID_SAT_LINCONS,0);
  return true;
}

bool itvp_sat_tcons(ap_manager_t* man, itvp_t* a, ap_tcons0_t* cons)
{
  return ap_generic_sat_tcons(man,a,cons,AP_SCALAR_DOUBLE,false);
}

/* ====================================================================== */
/* Inclusion of a dimension in an interval */
/* ====================================================================== */
bool itvp_sat_interval(ap_manager_t* man, itvp_t* a,
			ap_dim_t dim, ap_interval_t* interval)
{
  bool sat;
  ap_interval_t* bnd=NULL;
  int r;

  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_SAT_INTERVAL,0);
  man->result.flag_exact = man->result.flag_best = true;

  if(a->flag==EMPTY_POL){
    return true;
  }
  else if(a->flag==UNIVERSE_POL){
    sat=ap_interval_is_top(interval);
    return sat;
  }
  else{
    if (pr->funopt->algorithm >0 ){ /* most accurate */
       /* poly_tighten_kthvar(po,dim); */
    }
    else{  /* default or most efficient */
    }
    ap_interval_set_double(bnd,*(a->bnds+2*dim),*(a->bnds+2*dim+1));
    r=ap_interval_cmp(bnd,interval);
    if( r==0 || r==-1 )
	 return true;
    else return false;
  }
}

/* ====================================================================== */
/* Is a dimension unconstrained ? */
/* ====================================================================== */

bool itvp_is_dimension_unconstrained(ap_manager_t* man, itvp_t* a,
					ap_dim_t dim)
{
  size_t i,j;
  bool r;
  ap_interval_t *bnd=NULL,*topintv=NULL;

  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_SAT_INTERVAL,0);

  if(a->flag==EMPTY_POL){
    man->result.flag_exact = man->result.flag_best = true;
    return false;
  }
  else if(a->flag==UNIVERSE_POL){
    man->result.flag_exact = man->result.flag_best = true;
    return true;
  }
  else{
    if (pr->funopt->algorithm >0 ){ /* most accurate */
       /* poly_tighten_kthvar(po,dim);*/
    }
    else{  /* default or most efficient */
    }
    man->result.flag_exact = man->result.flag_best = false;
    ap_interval_set_double(bnd,*(a->bnds+2*dim),*(a->bnds+2*dim+1));
    topintv=ap_interval_alloc();
    ap_interval_set_top(topintv);
    r=ap_interval_cmp(bnd,topintv);
    if( r==0 )
	 return true;
    else return false;
  }
}
