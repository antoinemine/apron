/*
 * fpp_transfer.c
 *
 * Assignment, substitution, guard transfer functions.
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

#include "fpp.h"
#include "fpp_internal.h"
#include "ap_generic.h"
#include "fpp_user.h"
#include "bt.h"
#include "fm.h"
#include "quasi_linear.h"
#include "fppol_domain.h"

#include "ap_linearize.h"
#include "ap_linearize_aux.h"
#include "num.h"
#include "bound.h"
#include "itv.h"
#include "itv_linexpr.h"
#include "itv_linearize.h"
#include "ap_interval.h"
/* ============================================================ */
/* Meet constraints and Join generators */
/* ============================================================ */

fpp_t* fpp_meet_lincons_array(ap_manager_t* man,
			      bool destructive, fpp_t* a,
			      ap_lincons0_array_t* array)
{
  fpp_t *a1,*a2;
  unsigned i,j,n,k;
  ap_coeff_t *coeff;
  numdbl_t* p;
  numdbl_t* q;
  numdbl_t val;
  bool flag;
  numdbl_t bnd1,bnd2;
  int *rvars;
  int nvars;
   ap_scalar_t *pscalar;
   int flag1,flag2;

  fprintf(stdout, "AP_FUNID_MEET_LINCONS_ARRAY\n");
  fflush(stdout);
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_MEET_LINCONS_ARRAY, 0);

  if(destructive==false)
	a1=fpp_copy_internal(pr, a);
  else	a1=a;
  if(a1->flag==EMPTY_POL || array->size==0)    return a1;

#if MEETITV
    /*-------- for interval linear constraints ---------*/
    itv_internal_t* intern;
    itv_t* env;
    itv_lincons_array_t tlincons;
    ap_lincons0_array_t array2;
    bool meet;
    ap_interval_t** tinterval;
    bool quasilinear;
    tinterval = fpp_to_box(man,a1);
    intern = itv_internal_alloc();
    itv_array_set_ap_interval_array(intern,&env,tinterval,a1->dim);
    ap_interval_array_free(tinterval,a1->dim);
    itv_lincons_array_init(&tlincons,array->size);
    itv_lincons_array_set_ap_lincons0_array(intern,&tlincons,array);
    quasilinear = itv_lincons_array_is_quasilinear(&tlincons);
    /* quasilinearize if needed */
    if (!quasilinear){
      meet=true;
#if defined ( NUM_DOUBLE )
      itv_quasilinearize_lincons_array_D(intern,&tlincons,env,meet);
      itv_linearize_lincons_array_D(intern,&tlincons,meet);
#endif
      array2 = ap_lincons0_array_make(tlincons.size);
      for (i=0;i<tlincons.size;i++){
         ap_lincons0_set_itv_lincons(intern,&array2.p[i],&tlincons.p[i]);
      }
      array=&array2;
    }
    itv_lincons_array_clear(&tlincons);
    itv_array_free(env,a1->dim);
    itv_internal_free(intern);
    /*--------------------  end   --------------------*/
#endif

  n=0;
  if(a1->flag==UNIVERSE_POL){
    checked_malloc(pr,a1->cons,numdbl_t,2*array->size*(a1->dim+1), return NULL;);
    p=a1->cons;
    for (i=0; i<array->size; i++){
       flag=false;
       if(i>0 && array->size >1 ){
	  for(j=0;j<i;j++){
            if( array->p[i].constyp==array->p[j].constyp &&
                ap_linexpr0_equal(array->p[i].linexpr0,array->p[j].linexpr0)==true ) {
	           flag=true;  /* for the problem of duplicated elments due to AP_LINEXPR_INTLINEAR*/
	           break;
            }
          }
       }
       if(flag==true) continue;
       /* ap_lincons0_t: expr(\sum ai*vi + c) >= 0  while vector c >= \sum ai*vi */
       q=fpp_vector_of_lincons0(array->p[i],a1->dim,a1->intdim,&k);
       if(k==0) continue;
       memcpy(p,q,k*(a1->dim+1)*sizeof(numdbl_t));
       checked_free(q);
       p=p+k*(a1->dim+1);
       n=n+k;
    }
    a1->cons=realloc(a1->cons,n*(a1->dim+1)*sizeof(numdbl_t));
    a1->ncons=n;
    a1->flag=GENERAL_POL;
    checked_malloc(pr,a1->bnds,numdbl_t,(2*a1->dim), return NULL;);
    fpp_init_bounds(a1->bnds,a1->dim);
    if(pr->newdims>0){  /* called from AP_FUNID_ASSIGN_TEXPR_ARRAY */
	/* just tighten the bounds of the new variables using bound propagation */
	for(i=a1->dim-pr->newdims;i<a1->dim;i++){
	  a1=bt_kthvar_by_bound_propagation(pr,true,a1,i);
	}
    }
    else {  /* called because of condition tests */
       rvars=fpp_vars_related_to_last_kthConstraints(a1,n, &nvars);
       a1=bt_byRLP(pr,true,a1,rvars,nvars);
       checked_free(rvars);
    }
    a2=a1;

  }
  else{
    a1->cons=realloc(a1->cons,(a1->ncons+2*array->size)*(a1->dim+1)*sizeof(numdbl_t));
    p=a1->cons+a1->ncons*(a1->dim+1);
    for (i=0; i<array->size; i++){
       /* ap_lincons0_t: expr(\sum ai*vi + c) >= 0  while vector c >= \sum ai*vi */
       if(ap_lincons0_is_unsat(&array->p[i])==true){
	  checked_free(a1->cons);
	  checked_free(a1->bnds);
	  a1->ncons=0;
	  a1->flag=EMPTY_POL;
          return a1;
       }
       flag=false;
       if(i>0 && array->size >1 ){
	  for(j=0;j<i;j++){
            if( array->p[i].constyp==array->p[j].constyp &&
                ap_linexpr0_equal(array->p[i].linexpr0,array->p[j].linexpr0)==true ) {
	           flag=true;  /* for the problem of duplicated elments due to AP_LINEXPR_INTLINEAR*/
	           break;
            }
          }
       }
       if(flag==true) continue;
       q=fpp_vector_of_lincons0(array->p[i],a1->dim,a1->intdim,&k);
       if(k==0) continue;
       memcpy(p,q,k*(a1->dim+1)*sizeof(numdbl_t));
       checked_free(q);
       p=p+k*(a1->dim+1);
       n=n+k;
    }
    a1->ncons=a1->ncons+n;
    a1->cons=realloc(a1->cons,a1->ncons*(a1->dim+1)*sizeof(numdbl_t));
    /* a2=bt_byRLP(pr,false,a1); */
    if(pr->newdims > 0){  /* called from AP_FUNID_ASSIGN_TEXPR_ARRAY */
        a2=bt_kVars_byRLP(pr,false,a1, a1->dim-pr->newdims,a1->dim-1);
    }
    else {  /* called because of condition tests */
       rvars=fpp_vars_related_to_last_kthConstraints(a1,n, &nvars);
#if LP_EXACT
       a2=bt_byExactLP(pr,false,a1,rvars,nvars);
#else
       a2=bt_byRLP(pr,false,a1,rvars,nvars);
#endif
       checked_free(rvars);

#if LAZY_RR
       a2=redundancy_removal_quasi_syntactic(pr,true,a2); /* lazy redundancy_removal*/
#else
       a2=redundancy_removal(pr,true,a2, true);
#endif
    }
    fpp_free_internal(pr, a1);
  }

#if MEETITV
   if (!quasilinear)  ap_lincons0_array_clear(&array2);  /*for interval linear constraints*/
#endif

   return a2;
}

fpp_t* fpp_meet_tcons_array(ap_manager_t* man,
			    bool destructive, fpp_t* a,
			    ap_tcons0_array_t* array)
{
  fprintf(stdout, "AP_FUNID_MEET_TCONS_ARRAY\n");
  fflush(stdout);

    /* NewPolka:AP_SCALAR_MPQ,AP_LINEXPR_LINEAR ;
       Ocatagon: AP_SCALAR_MPQ ||AP_SCALAR_DOUBLE, AP_LINEXPR_INTLINEAR */
    /* Fppol: {x=0;y=0;} AP_LINEXPR_LINEAR has problem after the 1st assignment */
    /*         AP_LINEXPR_INTLINEAR has problem for 1st assignment (duplicated) */
#if MEET_TCONS_GENERIC
  return ap_generic_meet_intlinearize_tcons_array(man,destructive,a,array,
			AP_SCALAR_DOUBLE,AP_LINEXPR_LINEAR,(void *)&fpp_meet_lincons_array);
#else
  itv_internal_t* intern;
  itv_t* env;
  itv_lincons_array_t tlincons;
  ap_lincons0_array_t array2;
  size_t i,j,k;
  bool meet;
  ap_interval_t** tinterval;
  itv_linexpr_t itvexp;
  itv_lincons_t itvcons;
  ap_dim_t dim;
  ap_dimension_t dimension;
  itv_ptr pitv;
  bool* peq;
  numdbl_t *newCons,*aCons,*l1,*itvIneq,*itvIneq1;
  unsigned n;
  int sat;
  int *rvars;
  int nvars;
  fpp_t* a1;
  bool flag;
  num_t num;

  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_MEET_TCONS_ARRAY,0);

  if(destructive==false)
 	a1=fpp_copy_internal(pr, a);
  else	a1=a;
  if (a1->flag==EMPTY_POL || array->size==0){
     return a1;
  }
  else {
    tinterval = fpp_to_box(man,a1);
    intern = itv_internal_alloc();
    itv_array_set_ap_interval_array(intern,&env,tinterval,a1->dim);
    ap_interval_array_free(tinterval,a1->dim);
    itv_lincons_array_init(&tlincons,array->size);
    itv_intlinearize_ap_tcons0_array(intern,&tlincons,array,env, a1->intdim);

#if defined ( NUM_DOUBLE )
    meet=true;
    itv_quasilinearize_lincons_array_D(intern,&tlincons,env,meet);
    itv_linearize_lincons_array_D(intern,&tlincons,meet);
#endif

	  array2 = ap_lincons0_array_make(tlincons.size);
	  for (i=0;i<tlincons.size;i++){
		ap_lincons0_set_itv_lincons(intern,&array2.p[i],&tlincons.p[i]);
	  }
	  itv_lincons_array_clear(&tlincons);
	  itv_array_free(env,a1->dim);
	  itv_internal_free(intern);
	  a1 = fpp_meet_lincons_array(man,true,a1,&array2);
	  ap_lincons0_array_clear(&array2);
	  return a1;
  }
#endif
}

fpp_t* fpp_add_ray_array(ap_manager_t* man,
			 bool destructive, fpp_t* a,
			 ap_generator0_array_t* array)
{
  fprintf(stdout, "AP_FUNID_ADD_RAY_ARRAY\n");
  fflush(stdout);

  fpp_internal_t* pr =
    fpp_init_from_manager(man,AP_FUNID_ADD_RAY_ARRAY,2*(a->dim+2));
    return a;
}


/* ============================================================ */
/* Assignement and Substitutions */
/* ============================================================ */
fpp_t* fpp_assign_linexpr(fpp_internal_t* pr,
			  bool destructive, fpp_t* a,
			  ap_dim_t d, ap_linexpr0_t* expr,
			  fpp_t* dest)
{
  return ap_generic_assign_linexpr_array(pr->man,destructive,a,&d,&expr,1/*size*/,dest);
}

fpp_t* fpp_substitute_linexpr(ap_manager_t* man,
			      bool destructive, fpp_t* a,
			      ap_dim_t d, ap_linexpr0_t* expr,
			      fpp_t* dest)
{
  return ap_generic_substitute_linexpr_array(man,destructive,a,&d,&expr,1/*size*/,dest);
}

fpp_t* fpp_assign_linexpr_array(ap_manager_t* man,
				bool destructive, fpp_t* a,
				ap_dim_t* tdim,
				ap_linexpr0_t** texpr,
				size_t size,
				fpp_t* dest)
{
  fprintf(stdout, "AP_FUNID_ASSIGN_LINEXPR_ARRAY\n");
  fflush(stdout);
  return ap_generic_assign_linexpr_array(man,destructive,a,tdim,texpr,size,dest);

}

fpp_t* fpp_substitute_linexpr_array(ap_manager_t* man,
				    bool destructive, fpp_t* a,
				    ap_dim_t* tdim,
				    ap_linexpr0_t** texpr,
				    size_t size,
				    fpp_t* dest)
{
  fprintf(stdout, "AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY\n");
  fflush(stdout);
  return ap_generic_substitute_linexpr_array(man,destructive,a,tdim,texpr,size,dest);
}

fpp_t* fpp_assign_texpr_array(ap_manager_t* man,
			      bool destructive, fpp_t* a,
			      ap_dim_t* tdim,
			      ap_texpr0_t** texpr,
			      size_t size,
			      fpp_t* dest)
{
  fprintf(stdout, "AP_FUNID_ASSIGN_TEXPR_ARRAY\n");
  fflush(stdout);
  fpp_internal_t* pr = (fpp_internal_t*) man->internal;
  pr->newdims=1;

  return ap_generic_assign_texpr_array(man,destructive,a,tdim,texpr,size,dest);
}
fpp_t* fpp_substitute_texpr_array(ap_manager_t* man,
				  bool destructive, fpp_t* a,
				  ap_dim_t* tdim,
				  ap_texpr0_t** texpr,
				  size_t size,
				  fpp_t* dest)
{
  fprintf(stdout, "AP_FUNID_SUBSTITUTE_TEXPR_ARRAY\n");
  fflush(stdout);

  return ap_generic_substitute_texpr_array(man,destructive,a,tdim,texpr,size,dest);
}
