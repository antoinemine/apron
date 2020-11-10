/*
 * itvp_extract.c
 *
 * property extraction functions
 *
 * APRON Library / itvpol Domain
 *
 * Copyright (C) Liqian CHEN' 2009
 *
 */

/* This file is part of the APRON Library, released under LGPL license.  
   Please read the COPYING file packaged in the distribution.
*/
#include "itvp_user.h"
#include "itvp.h"
#include "ap_generic.h"
/* #include "bt.h" */
/* ====================================================================== */
/* Bounding the value of a dimension in a polyhedra */
/* ====================================================================== */

ap_interval_t* itvp_bound_dimension(ap_manager_t* man,itvp_t* po,ap_dim_t dim)
{
  ap_interval_t* interval;
  itvp_internal_t* itvp = itvp_init_from_manager(man,AP_FUNID_BOUND_DIMENSION,0);
  interval = ap_interval_alloc();
  ap_interval_reinit(interval,AP_SCALAR_DOUBLE);

  if(po->flag==EMPTY_POL){
    ap_interval_set_bottom(interval);
  }  
  else if(po->flag==UNIVERSE_POL){
    ap_interval_set_top(interval);
  }  
  else{
    ap_interval_set_double(interval,*(po->bnds+2*dim),*(po->bnds+2*dim+1));
  }
  man->result.flag_exact = man->result.flag_best = 
    dim<po->intdim ? false : true;

  return interval;
}

/* ====================================================================== */
/* Bounding the value of a linear expression in a polyhedra */
/* ====================================================================== */

ap_interval_t* itvp_bound_linexpr(ap_manager_t* man,
				itvp_t* a,
				ap_linexpr0_t* expr)
{
  itvp_internal_t* itvp = itvp_init_from_manager(man,AP_FUNID_BOUND_LINEXPR,0);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,itvp->funid,
			     "not implemented");
  return NULL;
}

ap_interval_t* itvp_bound_texpr(ap_manager_t* man,
			      itvp_t* a,
			      ap_texpr0_t* expr)
{
  return ap_generic_bound_texpr(man,a,expr,
				AP_SCALAR_DOUBLE,
				false);
}


/* ====================================================================== */
/* Converting to a set of constraints */
/* ====================================================================== */

ap_lincons0_array_t itvp_to_lincons_array(ap_manager_t* man,
					itvp_t* a)
{
  ap_lincons0_array_t array;
  size_t i,k;

  fprintf(stdout, "AP_FUNID_TO_LINCONS_ARRAY\n");
  fflush(stdout);
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_TO_LINCONS_ARRAY,0);
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
    array.p[i] = itvp_lincons0_of_vector(a->cons+i*(2*a->dim+1), a->dim);
  }

  return array;
}

ap_tcons0_array_t itvp_to_tcons_array(ap_manager_t* man,
				    itvp_t* po)
{
  fprintf(stdout, "AP_FUNID_TO_TCONS_ARRAY\n");
  fflush(stdout);
  return ap_generic_to_tcons_array(man,po);
}

/* ====================================================================== */
/* Converting to a box */
/* ====================================================================== */

ap_interval_t** itvp_to_box(ap_manager_t* man,
			  itvp_t* a)
{
  ap_interval_t** interval;
  size_t i,dim;
  numdbl_t *lbnd,*ubnd;

  fprintf(stdout, "AP_FUNID_TO_BOX\n");
  fflush(stdout);
  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_TO_BOX,0);
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
ap_generator0_array_t itvp_to_generator_array(ap_manager_t* man,
					    itvp_t* a)
{
   ap_generator0_array_t array;
   /*box_to_generator_array();*/
   return array;
}
