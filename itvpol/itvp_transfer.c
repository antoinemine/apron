/*
 * itvp_transfer.c
 *
 * Assignment, substitution, guard transfer functions.
 *
 * APRON Library / itvpol Domain
 *
 * Copyright (C) Liqian CHEN' 2009
 *
 */

/* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution.
*/

#include "itvp.h"
#include "itvp_internal.h"
#include "ap_generic.h"
#include "itvp_user.h"
#include "ibt.h"
#include "ifm.h"
#include "itv_linear.h"
#include "itvp_domain.h"
#include "numitv.h"

#include "ap_linearize.h"
#include "ap_linearize_aux.h"
#include "num.h"
#include "bound.h"
#include "itv.h"
#include "itv_linexpr.h"
#include "itv_linearize.h"
#include "ap_interval.h"
#include "ap_scalar.h"

/* ============================================================ */
/* Meet constraints and Join generators */
/* ============================================================ */

itvp_t* itvp_meet_lincons_array(ap_manager_t* man,
			      bool destructive, itvp_t* a,
			      ap_lincons0_array_t* array)
{  
  itvp_t *a1,*a2;
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
/* gettimeofday(&tpstart,NULL); */

  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_MEET_LINCONS_ARRAY, 0);

   for(i=0;i<array->size;i++){
     coeff=ap_linexpr0_cstref(array->p[i].linexpr0); 
	 if (ap_coeff_zero(coeff)) continue;
	 flag1=0;
	 flag2=0;
	 switch (coeff->discr){
		case AP_COEFF_SCALAR:
		  flag1=ap_scalar_infty(coeff->val.scalar);
		  if(flag1){  /* infty */
			if(i==array->size-1){
			   ap_lincons0_clear(&array->p[array->size-1]);
			   array->p[array->size-1].linexpr0 = NULL;
			   array->p[array->size-1].scalar = NULL;
			   ap_lincons0_array_resize(array,array->size-1);
			}
			else{
			   memcpy(array->p+i,array->p+i+1,(array->size-i-1)*sizeof(ap_lincons0_t));
			   ap_lincons0_clear(&array->p[array->size-1]);
			   array->p[array->size-1].linexpr0 = NULL;
				   array->p[array->size-1].scalar = NULL;
			   array->p = (ap_lincons0_t*)realloc(array->p,(array->size-1)*sizeof(ap_lincons0_t));
			   array->size--;
			}
		  }
		  break;
	    case AP_COEFF_INTERVAL:
		  flag1=ap_scalar_infty(coeff->val.interval->inf);
		  flag2=ap_scalar_infty(coeff->val.interval->sup);
		  if( flag1 || flag2 ){  /* infty */
			if(i==array->size-1){
			   ap_lincons0_clear(&array->p[array->size-1]);
			   array->p[array->size-1].linexpr0 = NULL;
			   array->p[array->size-1].scalar = NULL;
			   ap_lincons0_array_resize(array,array->size-1);
			}
			else{
			   memcpy(array->p+i,array->p+i+1,(array->size-i-1)*sizeof(ap_lincons0_t));
			   ap_lincons0_clear(&array->p[array->size-1]);
			   array->p[array->size-1].linexpr0 = NULL;
			   array->p[array->size-1].scalar = NULL;
			   array->p = (ap_lincons0_t*)realloc(array->p,(array->size-1)*sizeof(ap_lincons0_t));
			   array->size--;
			}
		  }
		  break;
      }
   }

  if(destructive==false)
	a1=itvp_copy_internal(pr, a);
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
    tinterval = itvp_to_box(man,a1);
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
    checked_malloc(pr,a1->cons,numdbl_t,2*array->size*(2*a1->dim+1),return NULL;);
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
       q=itvp_vector_of_lincons0(array->p[i],a1->dim,a1->intdim,&k);
       if(k==0) continue;
       memcpy(p,q,k*(2*a1->dim+1)*sizeof(numdbl_t));	
       checked_free(q);
       p=p+k*(2*a1->dim+1);
       n=n+k;
    }	
    a1->cons=realloc(a1->cons,n*(2*a1->dim+1)*sizeof(numdbl_t));
    a1->ncons=n;
    a1->flag=GENERAL_POL;
    checked_malloc(pr,a1->bnds,numdbl_t,(2*a1->dim),return NULL;);
    itvp_init_bounds(a1->bnds,a1->dim);

    if(pr->newdims>0){  /* called from AP_FUNID_ASSIGN_TEXPR_ARRAY */
		/* just tighten the bounds of the new variables using bound propagation */
		for(i=a1->dim-pr->newdims;i<a1->dim;i++){
		  a1=bt_kthvar_byExactILP(pr,true,a1,i);
		}
    } 
    else {  /* called because of condition tests */
       rvars=itvp_vars_related_to_last_kthConstraints(a1,n, &nvars);
       a1=bt_byExactILP(pr,true,a1,rvars,nvars);
       checked_free(rvars);
    }
    a2=a1;
    a2=itvpol_reduction(pr,true,a2);
  }
  else{
    a1->cons=realloc(a1->cons,(a1->ncons+2*array->size)*(2*a1->dim+1)*sizeof(numdbl_t));
    p=a1->cons+a1->ncons*(2*a1->dim+1);
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
       q=itvp_vector_of_lincons0(array->p[i],a1->dim,a1->intdim,&k);
       if(k==0) continue;
       memcpy(p,q,k*(2*a1->dim+1)*sizeof(numdbl_t));	
       checked_free(q);
       p=p+k*(2*a1->dim+1);
       n=n+k;
    }
    a1->ncons=a1->ncons+n;
    a1->cons=realloc(a1->cons,a1->ncons*(2*a1->dim+1)*sizeof(numdbl_t));

    /* a2=bt_byRLP(pr,false,a1); */
    if(pr->newdims > 0){  /* called from AP_FUNID_ASSIGN_TEXPR_ARRAY */
        a2=bt_kVars_byExactILP(pr,false,a1, a1->dim-pr->newdims,a1->dim-1);
    } 
    else {  /* called because of condition tests */
       rvars=itvp_vars_related_to_last_kthConstraints(a1,n, &nvars);
       a2=bt_byExactILP(pr,false,a1,rvars,nvars);
       a2=itvpol_reduction(pr,true,a2);

       checked_free(rvars);

#if LAZY_RR
       a2=itvp_redundancy_removal_quasi_syntactic(pr,true,a2); /* lazy redundancy_removal*/
#else
       a2=redundancy_removal_backward(pr,true,a2, true); 
#endif
    } 
    itvp_free_internal(pr, a1);
  }

#if MEETITV
   if (!quasilinear)  ap_lincons0_array_clear(&array2);  /*for interval linear constraints*/
#endif

   return a2;
}

itvp_t* itvp_meet_tcons_array(ap_manager_t* man,
			    bool destructive, itvp_t* a,
			    ap_tcons0_array_t* array)
{
  fprintf(stdout, "AP_FUNID_MEET_TCONS_ARRAY\n");
  fflush(stdout);

#if MEET_TCONS_FLOAT==0
  return ap_generic_meet_intlinearize_tcons_array(man,destructive,a,array,
			AP_SCALAR_DOUBLE,AP_LINEXPR_LINEAR,(void *)&itvp_meet_lincons_array);
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
  itvp_t* a1;
  bool flag;
  num_t num;

  itvp_internal_t* pr = itvp_init_from_manager(man,AP_FUNID_MEET_TCONS_ARRAY,0);

  if(destructive==false)
 	a1=itvp_copy_internal(pr, a);
  else	a1=a;
  if (a1->flag==EMPTY_POL || array->size==0){
     return a1;
  }
  else {
    tinterval = itvp_to_box(man,a1);
    intern = itv_internal_alloc();
    itv_array_set_ap_interval_array(intern,&env,tinterval,a1->dim);
    ap_interval_array_free(tinterval,a1->dim);
    itv_lincons_array_init(&tlincons,array->size);
    itv_intlinearize_ap_tcons0_array(intern,&tlincons,array,env, a1->intdim);

#if defined ( NUM_DOUBLE )

#if MEET_TCONS_FLOAT==0
    meet=true;
    itv_quasilinearize_lincons_array_D(intern,&tlincons,env,meet);
    itv_linearize_lincons_array_D(intern,&tlincons,meet);
#endif

#endif

    array2 = ap_lincons0_array_make(tlincons.size);
    for (i=0;i<tlincons.size;i++){
      ap_lincons0_set_itv_lincons(intern,&array2.p[i],&tlincons.p[i]);
    }
    itv_lincons_array_clear(&tlincons);
    itv_array_free(env,a1->dim);
    itv_internal_free(intern);
    a1 = itvp_meet_lincons_array(man,true,a1,&array2);
    ap_lincons0_array_clear(&array2);

    return a1;
  }
#endif
}


itvp_t* itvp_add_ray_array(ap_manager_t* man,
			 bool destructive, itvp_t* a,
			 ap_generator0_array_t* array)
{
  fprintf(stdout, "AP_FUNID_ADD_RAY_ARRAY\n");
  fflush(stdout);

  itvp_internal_t* pr =
    itvp_init_from_manager(man,AP_FUNID_ADD_RAY_ARRAY,2*(a->dim+2));
    return a;
}


/* ============================================================ */
/* Assignement and Substitutions */
/* ============================================================ */
itvp_t* itvp_assign_linexpr(itvp_internal_t* pr,
			  bool destructive, itvp_t* a,
			  ap_dim_t d, ap_linexpr0_t* expr,
			  itvp_t* dest)
{
  return ap_generic_assign_linexpr_array(pr->man,destructive,a,&d,&expr,1/*size*/,dest);
}

itvp_t* itvp_substitute_linexpr(ap_manager_t* man,
			      bool destructive, itvp_t* a,
			      ap_dim_t d, ap_linexpr0_t* expr,
			      itvp_t* dest)
{
  return ap_generic_substitute_linexpr_array(man,destructive,a,&d,&expr,1/*size*/,dest);
}

itvp_t* itvp_assign_linexpr_array(ap_manager_t* man,
				bool destructive, itvp_t* a,
				ap_dim_t* tdim,
				ap_linexpr0_t** texpr,
				size_t size,
				itvp_t* dest)
{
  fprintf(stdout, "AP_FUNID_ASSIGN_LINEXPR_ARRAY\n");
  fflush(stdout);
  return ap_generic_assign_linexpr_array(man,destructive,a,tdim,texpr,size,dest);
}

itvp_t* itvp_substitute_linexpr_array(ap_manager_t* man,
				    bool destructive, itvp_t* a,
				    ap_dim_t* tdim,
				    ap_linexpr0_t** texpr,
				    size_t size,
				    itvp_t* dest)
{
  fprintf(stdout, "AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY\n");
  fflush(stdout);
  return ap_generic_substitute_linexpr_array(man,destructive,a,tdim,texpr,size,dest);
}

itvp_t* itvp_assign_texpr_array(ap_manager_t* man,
			      bool destructive, itvp_t* a,
			      ap_dim_t* tdim,
			      ap_texpr0_t** texpr,
			      size_t size,
			      itvp_t* dest)
{
  bool flag;
  unsigned i,j,k;
  numdbl_t *l;
  numdbl_t a_l,a_u;
  ap_linexpr0_t* lexpr0;
  bool pexact;
  itvp_t* a1;
  ap_coeff_t* coeff;
  numitv_t itv1,itv2,itv3,di_itv; 
  ap_abstract0_t* abstract0;

  fprintf(stdout, "AP_FUNID_ASSIGN_TEXPR_ARRAY\n");
  fflush(stdout);
  itvp_internal_t* pr = (itvp_internal_t*) man->internal;
  pr->newdims=1;

  if (size==1){
    flag=false;
      /* check whether there exists 0-contained interval coefficient */
      /* of tdim[0] in interval polyhedron a */ 
    if(a->flag==GENERAL_POL){
	  for(i=0;i<a->ncons;i++){
	     l=a->cons+i*(2*a->dim+1);
	     a_l=*(l+2*tdim[0]+1);
	     a_u=*(l+2*tdim[0]+1+1);
	     if(a_l==0 || a_u==0)  continue;
	     if(a_l<=0 && a_u>=0){
			flag=true;
			break;
	     }
	  }
    }
    if(flag==true){
          /* An assignment x'_i := \sum [d_j] x_j + [c] becomes */    
          /*  x_i = 1/[d_i] x_i -\sum_{j\neq i} [d_j]/d_i x_j - [c] */
          /* when \underline{d}_i==\overline{d}_i \neq 0 */
      if( ap_texpr0_is_interval_linear(texpr[0])==true){

#if defined ( NUM_DOUBLE )
	      pexact=false;
  	      abstract0 = malloc(sizeof(ap_abstract0_t));
  	      abstract0->value = a;
  	      abstract0->man = man;

          lexpr0=ap_intlinearize_texpr0_D( man, abstract0, texpr[0], &pexact, false);
	      flag=false;
	      coeff=ap_linexpr0_coeffref(lexpr0,tdim[0]);
          switch (coeff->discr){
           case AP_COEFF_SCALAR:
			flag=true;
			ap_double_set_scalar(&a_l,coeff->val.scalar,0);
			break;
		   case AP_COEFF_INTERVAL:
			ap_double_set_scalar(&a_l,coeff->val.interval->inf,0);
			ap_double_set_scalar(&a_u,coeff->val.interval->sup,0);
			if( a_l==a_u && (a_l==0 && a_u==0) ){
			   flag=true;
			}
			break;
  	      }
	      if(flag==false)
	    	  return ap_generic_assign_texpr_array(man,destructive,a,tdim,texpr,size,dest);
	      di_itv.inf=a_l;
	      di_itv.sup=a_l;
	      if(destructive==true)  a1=a;
	      else a1=itvp_copy_internal(pr, a);
	      for(k=0;k<a1->ncons;k++){
	         l=a1->cons+k*(2*a1->dim+1);
	         a_l=*(l+2*tdim[0]+1);
	         a_u=*(l+2*tdim[0]+1+1);
	         if(a_l==0 && a_u==0)  continue;
                    /* a constraint [ai]x_i + \sum [aj] x_j <= b becomes */    
                    /* ([ai]\di) x_i + \sum ([aj]-[dj]/di) x_j <= sup(b+[c]/di */ 
		     coeff=ap_linexpr0_cstref(lexpr0);
             switch (coeff->discr){
				case AP_COEFF_SCALAR:
					ap_double_set_scalar(&itv1.inf,coeff->val.scalar,0);
					itv1.sup=itv1.inf;
					break;
				case AP_COEFF_INTERVAL:
					ap_double_set_scalar(&itv1.inf,coeff->val.interval->inf,0);
					ap_double_set_scalar(&itv1.sup,coeff->val.interval->sup,0);
					break;
  	         } 
	         itv2=numitv_div(itv1,di_itv);
	         *l=*l+itv2.sup;
			 for(j=0;j<a1->dim;j++){
				 if(j==tdim[0]){
					itv1.inf=*(l+2*j+1);
					itv1.sup=*(l+2*j+1+1);
					itv2=numitv_div(itv1,di_itv);
					*(l+2*j+1)=itv2.inf;
					*(l+2*j+1+1)=itv2.sup;
				 }
				 else{
					itv1.inf=*(l+2*j+1);
					itv1.sup=*(l+2*j+1+1);  /*[a_j]*/
					coeff=ap_linexpr0_coeffref(lexpr0,j);
					switch (coeff->discr){
					   case AP_COEFF_SCALAR:
						flag=true;
						ap_double_set_scalar(&a_l,coeff->val.scalar,0);
						break;
					   case AP_COEFF_INTERVAL:
						ap_double_set_scalar(&a_l,coeff->val.interval->inf,0);
						ap_double_set_scalar(&a_u,coeff->val.interval->sup,0);
						break;
					}
					itv2.inf=a_l;
					itv2.sup=a_u;  /*[d_j]*/
					itv3=numitv_div(itv2,di_itv);
					itv2=numitv_sub(itv1,itv2);
					*(l+2*j+1)=itv2.inf;
					*(l+2*j+1+1)=itv2.sup;
				 }
			 }
	      }
          return a1;
#else
          return ap_generic_assign_texpr_array(man,destructive,a,tdim,texpr,size,dest);
#endif
	  }
    }
  }
  return ap_generic_assign_texpr_array(man,destructive,a,tdim,texpr,size,dest);
}

itvp_t* itvp_substitute_texpr_array(ap_manager_t* man,
				  bool destructive, itvp_t* a,
				  ap_dim_t* tdim,
				  ap_texpr0_t** texpr,
				  size_t size,
				  itvp_t* dest)
{
  fprintf(stdout, "AP_FUNID_SUBSTITUTE_TEXPR_ARRAY\n");
  fflush(stdout);

  return ap_generic_substitute_texpr_array(man,destructive,a,tdim,texpr,size,dest);
}
