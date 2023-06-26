/*
 * fpp_user.c
 *
 * Conversions with interface datatypes.
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

#include <fenv.h> /* floating-point lib */
#include "fpp_user.h"
#include "ap_lincons0.h"
#include "ap_scalar.h"

/* ********************************************************************** */
/* From FPPOL to APRON */
/* ********************************************************************** */

/* ap_lincons0_t: expr(\sum ai*vi + c) >= 0  while vector c >= \sum ai*vi */
ap_lincons0_t fpp_lincons0_of_vector_dense( numdbl_t* q, size_t size)
{
  ap_linexpr0_t* linexpr;
  numdbl_t v;
  size_t i;
  linexpr = ap_linexpr0_alloc(AP_LINEXPR_DENSE, size);
  ap_linexpr0_set_cst_scalar_double(linexpr, *q);
  for (i=0; i<size; i++){
    v=-*(q+i+1);
    if(v!=0.0) v=-v;
    ap_linexpr0_set_coeff_scalar_double(linexpr,i,v);
  }
  return ap_lincons0_make(AP_CONS_SUPEQ,linexpr,NULL);
}

ap_lincons0_t fpp_lincons0_of_vector( numdbl_t* q, size_t size)
{
  ap_linexpr0_t* linexpr;
  numdbl_t v;
  size_t i;
  linexpr = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, size);
  ap_linexpr0_set_cst_scalar_double(linexpr, *q);
  for (i=0; i<size; i++){
    v=*(q+i+1);
    if(v!=0.0) v=-v;
    if(v!=0.0)    ap_linexpr0_set_coeff_scalar_double(linexpr,i,v);
  }
  return ap_lincons0_make(AP_CONS_SUPEQ,linexpr,NULL);
}

numdbl_t* fpp_vector_of_lincons0(ap_lincons0_t p, unsigned dimension,unsigned intdim, unsigned* n)
{
   unsigned j;
   ap_coeff_t *coeff;
   ap_scalar_t *pscalar;
   ap_dim_t dim;
   numdbl_t val;
   numdbl_t *q;

   switch(p.constyp){
     case AP_CONS_EQ:
        /* ap_lincons0_t: expr(\sum ai*vi + c) == 0  while vector c >= \sum ai*vi */
       q=(numdbl_t *)malloc(2*(dimension+1)*sizeof(numdbl_t));
       memset(q,0,2*(dimension+1)*sizeof(numdbl_t));
       coeff=ap_linexpr0_cstref(p.linexpr0);
       if (! ap_coeff_zero(coeff)){
      	    switch(coeff->discr){
              case AP_COEFF_SCALAR:
				pscalar = coeff->val.scalar;
                ap_double_set_scalar(&val,pscalar,FE_UPWARD);
                *q=val;
				if(*q==0.0)  *(q+dimension+1)=0.0;
				else         *(q+dimension+1)=-*q;
       	        break;
              case AP_COEFF_INTERVAL:
				pscalar = coeff->val.interval->sup;
                ap_double_set_scalar(&val,pscalar,FE_UPWARD);
                *q=val;
                pscalar = coeff->val.interval->inf;
                ap_double_set_scalar(&val,pscalar,FE_UPWARD);
				if(val==0.0)  *(q+dimension+1)=0.0;
				else *(q+dimension+1)=-val;
                break;
            }
       }
       else{
    	   *q=0.0;
           *(q+dimension+1)=0.0;
       }
       ap_linexpr0_ForeachLinterm(p.linexpr0,j,dim,coeff){
          if (! ap_coeff_zero(coeff)){
      	    switch(coeff->discr){
              case AP_COEFF_SCALAR:
            	pscalar = coeff->val.scalar;
                ap_double_set_scalar(&val,pscalar,FE_UPWARD);
                *(q+dim+1)=val;
				if(val==0.0){
					*(q+dim+1)=0.0;
					*(q+dim+1+dimension+1)=0.0;
				}
				else{
					*(q+dim+1)=-val;
					*(q+dim+1+dimension+1)=val;
			    }
       	        break;
              case AP_COEFF_INTERVAL:
	            fprintf(stdout,"Error: AP_COEFF_INTERVAL in fpp_vector_of_lincons0 \n");
	            fflush(stdout);
                break;
            }
          }
       }
       *n=2;
       return q;
     case AP_CONS_SUP:
		if(ap_linexpr0_is_integer(p.linexpr0,intdim)==true){
		   /* ap_lincons0_t: expr(\sum ai*vi + c) > 0  while vector [c-1] >= \sum ai*vi */
		   q=(numdbl_t *)malloc((dimension+1)*sizeof(numdbl_t));
		   memset(q,0,(dimension+1)*sizeof(numdbl_t));
		   coeff=ap_linexpr0_cstref(p.linexpr0);
		   if (! ap_coeff_zero(coeff)){
			 switch(coeff->discr){
				case AP_COEFF_SCALAR:
				   pscalar = coeff->val.scalar;
				   if(ap_scalar_infty(pscalar)==1){ /* +infty */
					checked_free(q);
					*n=0;
					return NULL;
				   }
				   ap_double_set_scalar(&val,pscalar,FE_UPWARD); /* AP_SCALAR_DOUBLE or AP_SCALAR_MPQ*/
				   *q=trunc(val-1);
				   break;
				case AP_COEFF_INTERVAL:
				   pscalar = coeff->val.interval->sup;
				   if(ap_scalar_infty(pscalar)==1){ /* +infty */
						checked_free(q);
					*n=0;
					return NULL;
				   }
				   ap_double_set_scalar(&val,pscalar,FE_UPWARD); /* AP_SCALAR_DOUBLE or AP_SCALAR_MPQ*/
				   *q=trunc(val-1);
				   break;
			}
		   }
		   else{
			*q=-1.0;
		   }
		   ap_linexpr0_ForeachLinterm(p.linexpr0,j,dim,coeff){
			 if (! ap_coeff_zero(coeff)){
				switch(coeff->discr){
				case AP_COEFF_SCALAR:
				   pscalar = coeff->val.scalar;
				   ap_double_set_scalar(&val,pscalar,FE_UPWARD); /* AP_SCALAR_DOUBLE or AP_SCALAR_MPQ*/
				   if(val==0.0)  *(q+dim+1)=0.0;
				   else *(q+dim+1)=-val;
				   break;
				case AP_COEFF_INTERVAL:
				   fprintf(stdout,"Error: AP_COEFF_INTERVAL in fpp_vector_of_lincons0 \n");
				   fflush(stdout);
				   break;
				}
			 }
		   }
		   *n=1;
		   return q;
		}
	/*else overapproximation to AP_CONS_SUPEQ */
        // fall through
     case AP_CONS_SUPEQ:
       /* ap_lincons0_t: expr(\sum ai*vi + c) >= 0  while vector c >= \sum ai*vi */
       q=(numdbl_t *)malloc((dimension+1)*sizeof(numdbl_t));
       memset(q,0,(dimension+1)*sizeof(numdbl_t));
       coeff=ap_linexpr0_cstref(p.linexpr0);
       if (! ap_coeff_zero(coeff)){
      	    switch(coeff->discr){
              case AP_COEFF_SCALAR:
				pscalar = coeff->val.scalar;
				if(ap_scalar_infty(pscalar)==1){ /* +infty */
				   checked_free(q);
				   *n=0;
				   return NULL;
				}
                ap_double_set_scalar(&val,pscalar,FE_UPWARD); /* AP_SCALAR_DOUBLE or AP_SCALAR_MPQ*/
                *q=val;
       	        break;
              case AP_COEFF_INTERVAL:
				pscalar = coeff->val.interval->sup;
				if(ap_scalar_infty(pscalar)==1){ /* +infty */
				   checked_free(q);
				   *n=0;
				   return NULL;
				}
                ap_double_set_scalar(&val,pscalar,FE_UPWARD); /* AP_SCALAR_DOUBLE or AP_SCALAR_MPQ*/
                *q=val;
                break;
            }
       }
       else{
	        *q=0.0;
       }
       ap_linexpr0_ForeachLinterm(p.linexpr0,j,dim,coeff){
          if (! ap_coeff_zero(coeff)){
      	    switch(coeff->discr){
              case AP_COEFF_SCALAR:
				pscalar = coeff->val.scalar;
				ap_double_set_scalar(&val,pscalar,FE_UPWARD); /* AP_SCALAR_DOUBLE or AP_SCALAR_MPQ*/
				if(val==0.0)  *(q+dim+1)=0.0;
				else *(q+dim+1)=-val;
				break;
			  case AP_COEFF_INTERVAL:
				fprintf(stdout,"Error: AP_COEFF_INTERVAL in fpp_vector_of_lincons0 \n");
				fflush(stdout);
				break;
            }
          }
       }
       *n=1;
       return q;
     default:
       fprintf(stdout,"Strict < inequality in fpp_vector_of_lincons0()\n");
       ap_lincons0_fprint(stdout,&p,NULL);
       fprintf(stdout,"\n");
       *n=0;
       return NULL;
   }
}

/* ap_lincons0_t: \sum ai*vi + c  while vector c + \sum ai*vi */
/* Note:   memset q to 0 before calling this function */
void fpp_vector_of_linexp0(numdbl_t* q, ap_linexpr0_t* p)
{
   unsigned j;
   ap_dim_t dim;
   ap_coeff_t *coeff;
   numdbl_t val;
   ap_scalar_t *pscalar;

   coeff=ap_linexpr0_cstref(p);
   *q=coeff->val.scalar->val.dbl;
   ap_linexpr0_ForeachLinterm(p,j,dim,coeff){
       if (! ap_coeff_zero(coeff)){
      	 switch(coeff->discr){
            case AP_COEFF_SCALAR:
            	pscalar = coeff->val.scalar;
                ap_double_set_scalar(&val,pscalar,FE_UPWARD);
                *(q+dim+1)=val;
       	        break;
           case AP_COEFF_INTERVAL:
        	    fprintf(stdout,"Error: AP_COEFF_INTERVAL in fpp_vector_of_linexp0 \n");
                break;
            }
       }
   }
}

/* bounds to constraints */
numdbl_t* fpp_bnds_to_cons(fpp_internal_t* pr, numdbl_t* bnds, size_t dim, size_t *ncons)
{
  numdbl_t *cons,*p;
  size_t i,j,n;
  numdbl_t lbnd,ubnd;

  checked_malloc(pr,cons,numdbl_t,2*dim*(dim+1),return NULL;);
  p=cons;
  n=0;
  for(i=0;i<dim;i++){
     /* lower bound:   -V_i <= -\underline{V_i} */
     lbnd= *(bnds+2*i);
     if(lbnd!=-MAX_VARBND){
		*p=-lbnd;
		*(p+i+1)=-1.0;
		p=p+dim+1;
		n++;
     }
     /* upper bound:   V_i <= \overline{V_i} */
     ubnd=*(bnds+2*i+1);
     if(ubnd!=MAX_VARBND){
		*p=ubnd;
		*(p+i+1)=1.0;
		p=p+dim+1;
		n++;
     }
  }
  cons=(numdbl_t *)realloc(cons,n*(dim+1)*sizeof(numdbl_t));
  *ncons=n;
  return cons;
}

void fpp_init_bounds(numdbl_t* bnds, size_t dim)
{
   unsigned i;
   for(i=0;i<dim;i++){
      *(bnds+2*i)=-MAX_VARBND;
      *(bnds+2*i+1)=MAX_VARBND;
   }
}

fpp_t* fpp_normalize(fpp_internal_t* pr,bool destructive,fpp_t* fpp)
{
  fpp_t* fpp1;
  numdbl_t* p;
  int i;
  size_t j;
  bool flag;

  if(destructive==false)
     	fpp1=fpp_copy_internal(pr, fpp);
  else  fpp1=fpp;
  for (i=fpp1->ncons-1; i>=0; i--){
    p=fpp1->cons+i*(fpp1->dim+1);
    flag=false;
    for (j=0; j<fpp1->dim; j++){
	if( *(p+j+1)!=0 ){
	  flag=true;
	  break;
	}
    }
    if(flag==false){
	if(*p>=-0.0){
	  memmove(p, p+fpp1->dim+1, (fpp1->ncons+1-i)*(fpp1->dim+1)*sizeof(numdbl_t) );
	  fpp1->ncons--;
	}
	else{
	  checked_free(fpp1->cons);
	  checked_free(fpp1->bnds);
	  fpp1->ncons=0;
	  fpp1->flag=EMPTY_POL;
	  return fpp1;
	}
    }
  }
  if(fpp1->ncons==0){
    checked_free(fpp1->cons);
    checked_free(fpp1->bnds);
    fpp1->flag=EMPTY_POL;
    return fpp1;
  }
  fpp1->cons=realloc(fpp1->cons,fpp1->ncons*(fpp1->dim+1)*sizeof(numdbl_t));
  return fpp1;
}

int* fpp_vars_related_to_last_kthConstraints(fpp_t* fpp1,int k, int* nvars)
{
   char consIndex[1000];
   int  rvarsIndex[100];
   int *r,*pr;
   int i,j,m,nc,nv;
   numdbl_t *l1;
   bool flag;

   if(fpp1->ncons-k > 0 ){
   	memset(consIndex,0,(fpp1->ncons-k)*sizeof(char));
   }
   memset(rvarsIndex,0,(fpp1->dim)*sizeof(int));
   nc=0;
   nv=0;
   for(j=0;j<(int)fpp1->dim;j++){
      flag=false;
      for(i=fpp1->ncons-k;i<(int)fpp1->ncons;i++){
      	 l1=fpp1->cons+i*(fpp1->dim+1);
         if( *(l1+j+1)!=0 ){
			rvarsIndex[j]=1;
			nv++;
			flag=true;
            break;
         }
      }
      if(flag==true){
		 for(i=0;i<(int)fpp1->ncons-k;i++){
			if(consIndex[i]!=0) continue;
				l1=fpp1->cons+i*(fpp1->dim+1);
			if(*(l1+j+1)!=0){
				for(m=0;m<(int)fpp1->dim;m++){
			   if(m==j || rvarsIndex[m]==1) continue;
			   if( *(l1+m+1)!=0 ){
				rvarsIndex[m]=1;
					nv++;
			   }
			}
			consIndex[i]=1;
			nc++;
				}
			 }
      }
      if( (nc==(int)fpp1->ncons-k && fpp1->ncons-k >0) || nv==(int)fpp1->dim)  break;
   }
   r=pr=(int *)malloc(nv*sizeof(int));
   memset(r,0,nv*sizeof(int));
   for(j=0;j<(int)fpp1->dim;j++){
      if(rvarsIndex[j]==1){
         *pr=j;
         pr++;
      }
   }
   *nvars=nv;
   return r;
}

/* k starts from 0 */
fpp_t* fpp_remove_kthCons(fpp_t* fpp,int k)
{
    if(fpp->ncons-k-1>0){
       memmove(fpp->cons+k*(fpp->dim+1),fpp->cons+(k+1)*(fpp->dim+1),(fpp->ncons-k-1)*(fpp->dim+1)*sizeof(numdbl_t));
       fpp->ncons--;
    }
    else if(fpp->ncons-1>0){
    	fpp->cons=(numdbl_t *) realloc(fpp->cons,(fpp->ncons-1)*(fpp->dim+1)*sizeof(numdbl_t));
        fpp->ncons--;
    }
    else{
		checked_free(fpp->cons);
		checked_free(fpp->bnds);
		fpp->ncons=0;
		fpp->flag=UNIVERSE_POL;
    }
    return fpp;
}
