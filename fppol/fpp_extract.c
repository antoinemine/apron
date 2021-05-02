/*
 * fpp_extract.c
 *
 * property extraction functions
 *
 * APRON Library / Fppol Domain
 *
 * Copyright (C) Liqian CHEN' 2008-
 *
 */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution.
*/

#include "fpp_user.h"
#include "fpp.h"
#include "ap_generic.h"
#include "bt.h"
#include "rlp.h"
#include "numitv.h"
#include <fenv.h> /* floating-point lib */
#include <math.h>

/* ====================================================================== */
/* Bounding the value of a dimension in a polyhedra */
/* ====================================================================== */

ap_interval_t* fpp_bound_dimension(ap_manager_t* man,fpp_t* po,ap_dim_t dim)
{
  ap_interval_t* interval;
  fpp_internal_t* fpp = fpp_init_from_manager(man,AP_FUNID_BOUND_DIMENSION,0);
  interval = ap_interval_alloc();
  ap_interval_reinit(interval,AP_SCALAR_DOUBLE);

  if(po->flag==EMPTY_POL){
    ap_interval_set_bottom(interval);
  }
  else if(po->flag==UNIVERSE_POL){
    ap_interval_set_top(interval);
  }
  else{
    if (fpp->funopt->algorithm >0 ){ /* most accurate */
       /* poly_tighten_kthvar(po,dim); */
    }
    else{  /* default or most efficient */
    }
    ap_interval_set_double(interval,*(po->bnds+2*dim),*(po->bnds+2*dim+1));
  }
  man->result.flag_exact = man->result.flag_best =
    dim<po->intdim ? false : true;

  return interval;
}

/* ====================================================================== */
/* Bounding the value of a linear expression in a polyhedra */
/* ====================================================================== */

ap_interval_t* fpp_bound_linexpr(ap_manager_t* man,
				fpp_t* a,
				ap_linexpr0_t* expr)
{
  bool exact;
  ap_interval_t* interval;
  numdbl_t sup,inf, cst_val;
  ap_coeff_t *coeff;
  ap_scalar_t *pscalar;
  numdbl_t val, *objVector;
  unsigned j;
  ap_dim_t dim;
  glp_prob *lp;
  bool nearly;

  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_BOUND_LINEXPR,0);

  interval = ap_interval_alloc();
  ap_interval_reinit(interval,AP_SCALAR_DOUBLE);

  if(a->flag==EMPTY_POL){
    ap_interval_set_bottom(interval);
  }
  else if(a->flag==UNIVERSE_POL){
    ap_interval_set_top(interval);
  }
  else{
	checked_malloc(pr,objVector,numdbl_t,a->dim, return NULL;);
	memset(objVector,0,(a->dim)*sizeof(numdbl_t));
    ap_linexpr0_ForeachLinterm(expr,j,dim,coeff){
	   if (! ap_coeff_zero(coeff)){
		 switch(coeff->discr){
			case AP_COEFF_SCALAR:
				pscalar = coeff->val.scalar;
				ap_double_set_scalar(&val,pscalar,FE_UPWARD);
				*(objVector+dim)=val;
				break;
		   case AP_COEFF_INTERVAL:
				fprintf(stdout,"Error: AP_COEFF_INTERVAL in fpp_vector_of_linexp0 \n");
				break;
			}
	   }
    }

	coeff=ap_linexpr0_cstref(expr);
	cst_val=coeff->val.scalar->val.dbl;
	  /* upper bound */
	lp= rlp_create(a,GLP_MAX,objVector);
	lp=rlp_solve(lp);
	sup=cst_val + rlp_get_optimal_value(lp,a->bnds,false,&nearly);
	lp_delete(lp);
	  /* lower bound */
	lp = rlp_create(a,GLP_MIN,objVector);
	lp = rlp_solve(lp);
	inf = num_add_downward(cst_val, rlp_get_optimal_value(lp,a->bnds,false,&nearly));
	lp_delete(lp);

	checked_free(objVector);
    ap_interval_set_double(interval,inf,sup);
  }

  man->result.flag_exact = man->result.flag_best =
    ( (pr->funopt->flag_exact_wanted || pr->funopt->flag_best_wanted) &&
      ap_linexpr0_is_real(expr,a->intdim) ) ?
    exact :
    false;
  return interval;
}

ap_interval_t* fpp_bound_texpr(ap_manager_t* man,
			      fpp_t* a,
			      ap_texpr0_t* expr)
{
  return ap_generic_bound_texpr(man,a,expr,
				AP_SCALAR_DOUBLE,
				false);
}


/* ====================================================================== */
/* Converting to a set of constraints */
/* ====================================================================== */

ap_lincons0_array_t fpp_to_lincons_array(ap_manager_t* man,
					fpp_t* a)
{
  ap_lincons0_array_t array;
  size_t i,k;

  fprintf(stdout, "AP_FUNID_TO_LINCONS_ARRAY\n");
  fflush(stdout);
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_TO_LINCONS_ARRAY,0);
  man->result.flag_exact = man->result.flag_best = true;

  if (a->flag==EMPTY_POL){ /* a is empty */
    array = ap_lincons0_array_make(1);
    array.p[0] = ap_lincons0_make_unsat();
    return array;
  }
  else if (a->flag==UNIVERSE_POL){ /* a is universe */
    array = ap_lincons0_array_make(1);
    ap_linexpr0_t* expr = ap_linexpr0_alloc(AP_LINEXPR_DENSE,0);
    ap_coeff_set_scalar_double(&expr->cst,1.0);
    array.p[0] = ap_lincons0_make(AP_CONS_SUPEQ,expr,NULL);
    return array;
  }

  array = ap_lincons0_array_make(a->ncons);
  for (i=0; i<a->ncons; i++){
    array.p[i] = fpp_lincons0_of_vector(a->cons+i*(a->dim+1), a->dim);
  }
#if DEBUG_FPPOL
ap_lincons0_array_fprint(stdout,&array,NULL);
fflush(stdout);
#endif
  return array;
}

ap_tcons0_array_t fpp_to_tcons_array(ap_manager_t* man,
				    fpp_t* po)
{
  fprintf(stdout, "AP_FUNID_TO_TCONS_ARRAY\n");
  fflush(stdout);
  return ap_generic_to_tcons_array(man,po);
}

/* ====================================================================== */
/* Converting to a box */
/* ====================================================================== */

ap_interval_t** fpp_to_box(ap_manager_t* man,
			  fpp_t* a)
{
  ap_interval_t** interval;
  size_t i,dim;
  numdbl_t *lbnd,*ubnd;

  fprintf(stdout, "AP_FUNID_TO_BOX\n");
#if DEBUG_FPPOL
  fpp_fdump(stdout, man, a);
#endif
  fflush(stdout);
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_TO_BOX,0);
  dim = a->dim;

  if(a->flag==EMPTY_POL){
    man->result.flag_exact = man->result.flag_best = false;
    interval = ap_interval_array_alloc(dim);
    for (i=0; i<dim; i++){
      ap_interval_set_bottom(interval[i]);
    }
    return interval;
  }
  else if(a->flag==UNIVERSE_POL){
    man->result.flag_exact = man->result.flag_best = false;
    interval = ap_interval_array_alloc(dim);
    for (i=0; i<dim; i++){
      ap_interval_set_top(interval[i]);
    }
    return interval;
  }
  else{
    if (pr->funopt->algorithm > 0){
	/* a=bt_byRLP(pr,true,a);  */
    }
    interval = ap_interval_array_alloc(dim);
    for (i=0; i<dim; i++){
      lbnd=a->bnds+2*i;
      ubnd=a->bnds+2*i+1;
#if num_export_double
    /* use double */
      if(*lbnd==-MAX_VARBND && *ubnd==MAX_VARBND) {
      	    ap_interval_set_top(interval[i]);
      }
      else if(*lbnd==-MAX_VARBND){
  	    ap_scalar_set_infty(interval[i]->inf,-1);
	    ap_scalar_set_double(interval[i]->sup,*ubnd);
      }
      else if(*ubnd==MAX_VARBND) {
  	    ap_scalar_set_double(interval[i]->inf,*lbnd);
  	    ap_scalar_set_infty(interval[i]->sup,+1);
      }
      else  ap_interval_set_double(interval[i],*lbnd,*ubnd);
#else
    /* use mpq */
      ap_interval_set_double(interval[i],*lbnd,*ubnd);
#endif
    }
    man->result.flag_exact = man->result.flag_best = true;
    return interval;
  }
}

/* ====================================================================== */
/* Converting to a set of generators */
/* ====================================================================== */

/* not implemented. */
ap_generator0_array_t fpp_to_generator_array(ap_manager_t* man,
					    fpp_t* a)
{
  ap_generator0_array_t array = ap_generator0_array_make(0);
   /*box_to_generator_array();*/
   return array;
}
