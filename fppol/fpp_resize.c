/*
 * fpp_resize.c
 *
 * Projection, changes of dimension, variable permutation.
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
#include "fpp_user.h"
#include "fppol_domain.h"

/* ********************************************************************** */
/*  I. Vectors */
/* ********************************************************************** */

/*
  Apply the  permutation permutation. The result is stored in newq.
*/
static
void fpp_vector_permute_dimensions(fpp_internal_t* pr,
			       numdbl_t* newq, numdbl_t* q, size_t size,
			       ap_dim_t* permut)
{
  size_t j,newj;

  /* Permutation itself */
  for (j=0; j<size; j++){
    newj = permut[j] ;
    newq[newj]=q[j];
  }
  return;

}

static
void fpp_vector_add_dimensions(fpp_internal_t* pr,
			   numdbl_t* newq,
			   numdbl_t* q, size_t size,
			   ap_dimchange_t* dimchange)
{
  int i;
  unsigned k,dimsup;

  dimsup = dimchange->intdim+dimchange->realdim;
  k = dimsup;
  for (i=(int)size; i>=0; i--){
    if(i<(int)size){
	*(newq+i+k)=*(q+i);
    }
    while (k>=1 && dimchange->dim[k-1]==(ap_dim_t)i){
      k--;
      *(newq+i+k)=0.0;
    }
  }
}


void fpp_vector_remove_dimensions(fpp_internal_t* pr,
			      numdbl_t* newq,
			      numdbl_t* q, size_t size,
			      ap_dimchange_t* dimchange)
{
  size_t i,k,dimsup;

  dimsup = dimchange->intdim+dimchange->realdim;
  k=0;
  for (i=0; i<size-dimsup; i++){
    while (k<dimsup && dimchange->dim[k]==i+k){
      k++;
    }
    *(newq+i)=*(q+i+k);
  }
}


/* ============================================================ */
/* Projections */
/* ============================================================ */
fpp_t* fpp_forget_array(ap_manager_t* man,
			bool destructive, fpp_t* a,
			ap_dim_t* tdim, size_t size,
			bool project)
{
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_FORGET_ARRAY,0);
  fprintf(stdout, "AP_FUNID_FORGET_ARRAY\n");
  fflush(stdout);
  return a;
}


/* ============================================================ */
/* Change and permutation of dimensions */
/* ============================================================ */
fpp_t* fpp_add_dimensions(ap_manager_t* man,
			  bool destructive, fpp_t* a,
			  ap_dimchange_t* dimchange,
			  bool project)
{
  numdbl_t *cons1=NULL,*bnds1=NULL,*p=NULL;
  fpp_t* a1;
  int i;
  size_t k,dimsup;

  fprintf(stdout, "AP_FUNID_ADD_DIMENSIONS\n");
  fflush(stdout);
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_ADD_DIMENSIONS,0);
  dimsup = dimchange->intdim+dimchange->realdim;
  if(pr->newdims>0){
      pr->newdims=dimsup;
  }
  if(a->flag==UNIVERSE_POL && project==false ){
    if(destructive==false){
	a1=fpp_alloc_top(pr,a->dim+dimsup, a->intdim+dimchange->intdim);
    }
    else{
	a1=a;
	a1->intdim=a->intdim+dimchange->intdim;
 	a1->dim=a->dim+dimsup;
    }
    return a1;
  }

  if(a->flag==GENERAL_POL){
       /*constraints*/
    checked_malloc(pr,cons1,numdbl_t,(a->ncons)*(a->dim+1+dimsup), return NULL;);
    for (i=0; i<(int)a->ncons; i++){
      *(cons1+i*(a->dim+1+dimsup))=*( a->cons+i*(a->dim+1) );
      fpp_vector_add_dimensions(pr,cons1+i*(a->dim+1+dimsup)+1,a->cons+i*(a->dim+1)+1, a->dim,dimchange);
    }
  }
     /*bounds*/
  checked_malloc(pr,bnds1,numdbl_t,2*(a->dim+dimsup), return NULL;);
  if(project==true){
         /* initialize the new dimensions to zero */
    if(a->flag==GENERAL_POL)
         cons1=realloc(cons1, (a->ncons+dimsup*2)*(a->dim+1+dimsup)*sizeof(numdbl_t));
    else checked_malloc(pr,cons1,numdbl_t,(dimsup*2)*(a->dim+1+dimsup), return NULL;);
    p=cons1+ a->ncons*(a->dim+1+dimsup);
    memset(p,0,2*dimsup*(a->dim+1+dimsup)*sizeof(numdbl_t));
  }
  k=dimsup;
  for (i=(int)a->dim; i>=0; i--){
     if(i<(int)a->dim){
	if(a->flag==GENERAL_POL){
          *(bnds1+2*(i+k))=*(a->bnds+2*i); /* bounds */
	  *(bnds1+2*(i+k)+1)=*(a->bnds+2*i+1); /* bounds */
	}
	else{
          *(bnds1+2*(i+k))=-MAX_VARBND; /* bounds */
	  *(bnds1+2*(i+k)+1)=MAX_VARBND; /* bounds */
	}
     }
     while (k>=1 && dimchange->dim[k-1]==(ap_dim_t)i){
        if(project==true){
          *(p+i+k)=1.0;  /* constraint: 0.0 >= x */
          p=p+(a->dim+1+dimsup);
          *(p+i+k)=-1.0;  /* constraint: 0.0 >= -x */
          p=p+(a->dim+1+dimsup);
          k--;
          *(bnds1+2*(i+k))=0.0; /* bounds */
          *(bnds1+2*(i+k)+1)=0.0; /* bounds */
        }
	else{
    	  k--;
	  *(bnds1+2*(i+k))=-MAX_VARBND; /* bounds */
	  *(bnds1+2*(i+k)+1)=MAX_VARBND; /* bounds */
        }
    }
  }
  if(destructive==true){
    if( a->flag==GENERAL_POL ){
       checked_free(a->cons);
       checked_free(a->bnds);
    }
    a->cons=cons1;
    a->bnds=bnds1;
    if(project==true)  a->ncons=a->ncons+dimsup*2;
    a->dim=a->dim+dimsup;
    a->intdim=a->intdim+dimchange->intdim;
    a->flag=GENERAL_POL;
    return a;
  }
  else{
    a1=fpp_alloc_internal(pr,a->dim+dimsup, a->intdim+dimchange->intdim);
    a1->flag=GENERAL_POL;
    a1->cons=cons1;
    a1->bnds=bnds1;
    if(project==true)  a1->ncons=a->ncons+dimsup*2;
    else a1->ncons=a->ncons;
    return a1;
  }
}

fpp_t* fpp_remove_dimensions(ap_manager_t* man,
			     bool destructive, fpp_t* a,
			     ap_dimchange_t* dimchange)
{
  numdbl_t *cons1,*bnds1,*p;
  fpp_t* a1;
  size_t i,k,dimsup;
  numdbl_t *lbnd1,*ubnd1,*lbnd2,*ubnd2;

  fprintf(stdout, "AP_FUNID_REMOVE_DIMENSIONS\n");
  fflush(stdout);

  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_REMOVE_DIMENSIONS,0);
  if(destructive==true)  a1=a;
  else  a1=fpp_copy_internal(pr,a);
  dimsup = dimchange->intdim+dimchange->realdim;
  pr->newdims=0;
  if( a1->flag!=GENERAL_POL ){
    a1->dim=a1->dim-dimsup;
    a1->intdim=a1->intdim-dimchange->intdim;
    return a1;
  }

  for(i=0;i<dimsup;i++){
     a1=fppol_project(pr,true,a1,dimchange->dim[i]+1);
  }

  if(a1->flag!=GENERAL_POL){
      a1->dim=a1->dim-dimsup;
      a1->intdim=a1->intdim-dimchange->intdim;
      return a1;
  }

     /* purely remove dimension */
     /*constraints */
  checked_malloc(pr,cons1,numdbl_t,(a1->ncons)*(a1->dim+1-dimsup), return NULL;);
  for (i=0; i<a1->ncons; i++){
      *(cons1+i*(a1->dim+1-dimsup))=*( a1->cons+i*(a1->dim+1) );
      fpp_vector_remove_dimensions(pr,cons1+i*(a1->dim+1-dimsup)+1,a1->cons+i*(a1->dim+1)+1, a1->dim,dimchange);
  }

     /*bounds*/
  checked_malloc(pr,bnds1,numdbl_t,2*(a1->dim-dimsup), return NULL;);
  k=0;
  for (i=0; i<a1->dim-dimsup; i++){
    while (k<dimsup && dimchange->dim[k]==i+k){
      k++;
    }
    lbnd2=a1->bnds+2*(i+k);
    lbnd1=bnds1+2*i;
    *lbnd1=*lbnd2;
    ubnd2=a1->bnds+2*(i+k)+1;
    *(bnds1+2*i+1)=*ubnd2;
  }
  checked_free(a1->cons);
  checked_free(a1->bnds);
  a1->cons=cons1;
  a1->bnds=bnds1;
  a1->dim=a1->dim-dimsup;
  a1->intdim=a1->intdim-dimchange->intdim;

  return a1;
}

fpp_t* fpp_permute_dimensions(ap_manager_t* man,
			      bool destructive, fpp_t* a,
			      ap_dimperm_t* permutation)
{
  numdbl_t *cons1,*bnds1,*p;
  fpp_t* a1;
  size_t i,j,newj;

  fprintf(stdout, "AP_FUNID_PERMUTE_DIMENSIONS\n");
  fflush(stdout);

  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_PERMUTE_DIMENSIONS,0);
  if(a->flag!=GENERAL_POL){
     if(destructive==true)  a1=a;
     else  a1=fpp_copy_internal(pr,a);
     return a1;
  }
  checked_malloc(pr,cons1,numdbl_t,(a->ncons)*(a->dim+1), return NULL;);
  for (i=0; i<a->ncons; i++){
      *(cons1+i*(a->dim+1))=*( a->cons+i*(a->dim+1) );
      fpp_vector_permute_dimensions(pr,cons1+i*(a->dim+1)+1,a->cons+i*(a->dim+1)+1, a->dim,permutation->dim);
  }

     /*bounds*/
  checked_malloc(pr,bnds1,numdbl_t,2*(a->dim), return NULL;);
  for (j=0; j<a->dim; j++){
    newj = permutation->dim[j] ;
    bnds1[2*newj]=a->bnds[2*j];
    bnds1[2*newj+1]=a->bnds[2*j+1];
  }
  if(destructive==true){
    checked_free(a->cons);
    checked_free(a->bnds);
    a->cons=cons1;
    a->bnds=bnds1;
    return a;
  }
  else{
    a1=fpp_alloc_internal(pr,a->dim, a->intdim);
    a1->flag=GENERAL_POL;
    a1->ncons=a->ncons;
    a1->cons=cons1;
    a1->bnds=bnds1;
    return a1;
  }
}


/* ============================================================ */
/* Expansion and folding of dimensions */
/* ============================================================ */

fpp_t* fpp_expand(ap_manager_t* man,
		  bool destructive, fpp_t* a,
		  ap_dim_t dim,
		  size_t n)
{
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_EXPAND,0);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,pr->funid,
			     "not implemented");
  return a;
}

fpp_t* fpp_fold(ap_manager_t* man,
		bool destructive, fpp_t* a,
		ap_dim_t* tdim,
		size_t size)
{
  fpp_internal_t* pr = fpp_init_from_manager(man,AP_FUNID_FOLD,a->dim);
  ap_manager_raise_exception(man,AP_EXC_NOT_IMPLEMENTED,pr->funid,
			     "not implemented");
  return a;
}
