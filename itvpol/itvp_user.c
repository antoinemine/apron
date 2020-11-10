/* ********************************************************************** */
/* itvp_user.c: conversions with interface datatypes */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include <fenv.h> /* floating-point lib */
#include "itvp_user.h"
#include "ap_lincons0.h"
#include "ap_scalar.h"

/* ********************************************************************** */
/* From itvpol to APRON */
/* ********************************************************************** */

/* ap_lincons0_t: expr(\sum [a_l,a_u]*vi + c) >= 0  while vector c >= \sum [-a_u,-a_l]*vi */
ap_lincons0_t itvp_lincons0_of_vector_dense( numdbl_t* q, size_t dim)
{
  ap_linexpr0_t* linexpr;
  numdbl_t v_l,v_u;
  size_t i;

  linexpr = ap_linexpr0_alloc(AP_LINEXPR_DENSE, dim);
  ap_linexpr0_set_cst_scalar_double(linexpr, *q); 
  for (i=0; i<dim; i++){ 
    v_l=*(q+2*i+1+1);
    if(v_l!=0.0) v_l=-v_l;
    v_u=*(q+2*i+1);
    if(v_u!=0.0) v_u=-v_u;
    ap_linexpr0_set_coeff_interval_double(linexpr,i,v_l,v_u);
  }
  return ap_lincons0_make(AP_CONS_SUPEQ,linexpr,NULL);
}

ap_lincons0_t itvp_lincons0_of_vector( numdbl_t* q, size_t dim)
{
  ap_linexpr0_t* linexpr;
  numdbl_t v_l,v_u;
  size_t i;

  linexpr = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, dim);
  ap_linexpr0_set_cst_scalar_double(linexpr, *q); 
  for (i=0; i<dim; i++){ 
    v_l=*(q+2*i+1+1);
    if(v_l!=0) v_l=-v_l;
    v_u=*(q+2*i+1);
    if(v_u!=0) v_u=-v_u;
    ap_linexpr0_set_coeff_interval_double(linexpr,i,v_l,v_u);
  }
  return ap_lincons0_make(AP_CONS_SUPEQ,linexpr,NULL);
}

numdbl_t* itvp_vector_of_lincons0(ap_lincons0_t p, unsigned dimension,unsigned intdim, unsigned* n)
{
   unsigned j;
   ap_coeff_t *coeff;
   ap_scalar_t *pscalar;
   ap_interval_t *pitv;
   ap_dim_t dim;
   numdbl_t val;
   numdbl_t a_l,a_u;
   numdbl_t *q;
   
   switch(p.constyp){ 
     case AP_CONS_EQ:
        /* ap_lincons0_t: expr(\sum ai*vi + c) == 0  while vector c >= \sum -ai*vi */		
       q=(numdbl_t *)malloc(2*(2*dimension+1)*sizeof(numdbl_t)); 
       memset(q,0,2*(2*dimension+1)*sizeof(numdbl_t));
       coeff=ap_linexpr0_cstref(p.linexpr0); 
       if (! ap_coeff_zero(coeff)){
      	 switch(coeff->discr){
           case AP_COEFF_SCALAR:
	        pscalar = coeff->val.scalar;
            ap_double_set_scalar(&val,pscalar,FE_UPWARD);
            *q=val;
			if(*q==0.0)  *(q+2*dimension+1)=0.0;
			else         *(q+2*dimension+1)=-*q;
       	    break;
           case AP_COEFF_INTERVAL:
	        pscalar = coeff->val.interval->sup;
			ap_double_set_scalar(&val,pscalar,FE_UPWARD);
			*q=val;
	        pscalar = coeff->val.interval->inf;
			ap_double_set_scalar(&val,pscalar,FE_UPWARD);
			if(val==0.0)  *(q+2*dimension+1)=0.0;
			else *(q+2*dimension+1)=-val;
			break;
		 }
		}
		else{
		   *q=0.0;
		   *(q+2*dimension+1)=0.0;
		}
        ap_linexpr0_ForeachLinterm(p.linexpr0,j,dim,coeff){
          if (! ap_coeff_zero(coeff)){
      	    switch(coeff->discr){
              case AP_COEFF_SCALAR:
            	pscalar = coeff->val.scalar;
                ap_double_set_scalar(&val,pscalar,FE_UPWARD);
                *(q+2*dim+1)=val;
                *(q+2*dim+1+1)=val;
          	    if(val==0.0){
					*(q+2*dim+1)=0.0;
					*(q+2*dim+1+1)=0.0;
					*(q+(2*dimension+1)+2*dim+1)=0.0;
					*(q+(2*dimension+1)+2*dim+1+1)=0.0;
	  	        }
          	    else{
					*(q+2*dim+1)=-val;
					*(q+2*dim+1+1)=-val;
					*(q+(2*dimension+1)+2*dim+1)=val;
					*(q+(2*dimension+1)+2*dim+1+1)=val;
	 	        }
       	        break;
              case AP_COEFF_INTERVAL:
				/* ap_lincons0_t: expr(\sum [ai]*vi + [c]) == 0  */
				/* ==>  \sum -[ai]*vi<= overline{c}              */
				/* ==>  \sum [ai]*vi<= -underline{c}             */
				pitv = coeff->val.interval;
				ap_double_set_scalar(&a_l,pitv->inf,FE_DOWNWARD);
				ap_double_set_scalar(&a_u,pitv->sup,FE_UPWARD);
				*(q+2*dim+1)=a_l;
				*(q+2*dim+1+1)=a_u;
				if(a_l==0){
					*(q+2*dim+1+1)=0.0;
					*(q+(2*dimension+1)+2*dim+1)=0.0;
				}
				else{
					*(q+2*dim+1+1)=-a_l;
					*(q+(2*dimension+1)+2*dim+1)=a_l;
				}
				if(a_u==0){
					*(q+2*dim+1)=0.0;
					*(q+(2*dimension+1)+2*dim+1+1)=0.0;
				}
				else{
					*(q+2*dim+1)=-a_u;
					*(q+(2*dimension+1)+2*dim+1+1)=a_u;
				}
       	        break;
            }
          }      
       }
       *n=2;
       return q;
     case AP_CONS_SUP:
		if(ap_linexpr0_is_integer(p.linexpr0,intdim)==true){
		   /* ap_lincons0_t: expr(\sum ai*vi + c) > 0  while vector [c-1] >= \sum ai*vi */
		   q=(numdbl_t *)malloc((2*dimension+1)*sizeof(numdbl_t));
		   memset(q,0,(2*dimension+1)*sizeof(numdbl_t));
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
					   if(val==0.0){
						 *(q+2*dim+1)=0.0;
						 *(q+2*dim+1+1)=0.0;
					   }
					   else{
						 *(q+2*dim+1)=-val;
						 *(q+2*dim+1+1)=-val;
					   }
					   break;
					case AP_COEFF_INTERVAL:
						/* ap_lincons0_t: \sum [ai]*vi + [c]) > 0    */
						/* ==>  \sum -[ai]*vi<= overline{c}          */
						pitv = coeff->val.interval;
						ap_double_set_scalar(&a_l,pitv->inf,FE_DOWNWARD);
						ap_double_set_scalar(&a_u,pitv->sup,FE_UPWARD);
						*(q+2*dim+1)=a_l;
						*(q+2*dim+1+1)=a_u;
					   if(a_l==0){
						   *(q+2*dim+1+1)=0.0;
					   }
					   else{
						   *(q+2*dim+1+1)=-a_l;
					   }
					   if(a_u==0){
						*(q+2*dim+1)=0.0;
					   }
					   else{
						   *(q+2*dim+1)=-a_u;
					   }
					   break;
				}
			 }
		   }
		   *n=1;
		   return q;
		}
	/*else overapproximation to AP_CONS_SUPEQ */	
     case AP_CONS_SUPEQ:
       /* ap_lincons0_t: expr(\sum ai*vi + c) >= 0  while vector c >= \sum ai*vi */	
       q=(numdbl_t *)malloc((2*dimension+1)*sizeof(numdbl_t)); 
       memset(q,0,(2*dimension+1)*sizeof(numdbl_t));
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
                if(val==0.0){
		             *(q+2*dim+1)=0.0;
    		         *(q+2*dim+1+1)=0.0;
		        }
                else{
					*(q+2*dim+1)=-val;
					*(q+2*dim+1+1)=-val;
				}
       	        break;
              case AP_COEFF_INTERVAL:
				/* ap_lincons0_t: \sum [ai]*vi + [c]) >= 0    */
				/* ==>  \sum -[ai]*vi<= overline{c}          */
	           pitv = coeff->val.interval;
                   ap_double_set_scalar(&a_l,pitv->inf,FE_DOWNWARD);
                   ap_double_set_scalar(&a_u,pitv->sup,FE_UPWARD);		
                   *(q+2*dim+1)=a_l;
                   *(q+2*dim+1+1)=a_u;
          	   if(a_l==0){
	     	       *(q+2*dim+1+1)=0.0;
          	   }
          	   else{
	    	       *(q+2*dim+1+1)=-a_l;
          	   }
          	   if(a_u==0){
          		   *(q+2*dim+1)=0.0;
	     	   }
          	   else{
	    	       *(q+2*dim+1)=-a_u;
          	   }
          	   break;
            }
          }      
       }
       *n=1;
       return q;
     default:
       fprintf(stdout,"Strict < inequality in itvp_vector_of_lincons0()\n");
       ap_lincons0_fprint(stdout,&p,NULL);
       fprintf(stdout,"\n");
       *n=0;
       return NULL;
   }
}

/* ap_lincons0_t: \sum ai*vi + c  while vector c + \sum ai*vi */
/* Note:   memset q to 0 before calling this function */
void itvp_vector_of_linexp0(numdbl_t* q, ap_linexpr0_t* p)
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
        	    fprintf(stdout,"Error: AP_COEFF_INTERVAL in itvp_vector_of_linexp0 \n");
                break;
            }
       }      
   }
}

/* bounds to constraints */
numdbl_t* itvp_bnds_to_cons(itvp_internal_t* pr, numdbl_t* bnds, size_t dim, size_t *ncons)
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

void itvp_init_bounds(numdbl_t* bnds, size_t dim)
{
   unsigned i;
   for(i=0;i<dim;i++){ 
      *(bnds+2*i)=-MAX_VARBND; 
      *(bnds+2*i+1)=MAX_VARBND; 
   }	
}

itvp_t* itvp_normalize(itvp_internal_t* pr,bool destructive,itvp_t* itvp)
{
  itvp_t* itvp1;
  numdbl_t* p;
  int i;
  size_t j;
  bool flag;

  if(destructive==false)
     	itvp1=itvp_copy_internal(pr, itvp); 
  else  itvp1=itvp;
  for (i=itvp1->ncons-1; i>=0; i--){
    p=itvp1->cons+i*(itvp1->dim+1);
    flag=false;
	for (j=0; j<itvp1->dim; j++){
		if( *(p+j+1)!=0 ){
		  flag=true;
		  break;
		}
    }
	if(flag==false){
		if(*p>=-0.0){
		  memmove(p, p+itvp1->dim+1, (itvp1->ncons+1-i)*(itvp1->dim+1)*sizeof(numdbl_t) );
		  itvp1->ncons--;
		}
		else{
		  checked_free(itvp1->cons);
		  checked_free(itvp1->bnds);
		  itvp1->ncons=0;
		  itvp1->flag=EMPTY_POL;
		  return itvp1;
		}
    }
  }
  if(itvp1->ncons==0){ 
    checked_free(itvp1->cons);
    checked_free(itvp1->bnds);  
    itvp1->flag=EMPTY_POL;
    return itvp1;     	
  }
  itvp1->cons=realloc(itvp1->cons,itvp1->ncons*(itvp1->dim+1)*sizeof(numdbl_t)); 
  return itvp1;
}

int* itvp_vars_related_to_last_kthConstraints(itvp_t* itvp1,int k, int* nvars)
{
   char consIndex[1000];
   int  rvarsIndex[100];
   int *r,*pr;
   int i,j,m,nc,nv;
   numdbl_t *l1;
   bool flag;

   if(itvp1->ncons-k > 0 ){
   	memset(consIndex,0,(itvp1->ncons-k)*sizeof(char));
   }
   memset(rvarsIndex,0,(itvp1->dim)*sizeof(int));
   nc=0;
   nv=0;
   for(j=0;j<(int)itvp1->dim;j++){
      flag=false; 
      for(i=(int)itvp1->ncons-k;i<(int)itvp1->ncons;i++){
      	 l1=itvp1->cons+i*(itvp1->dim+1);
		 if( *(l1+j+1)!=0 ){
			rvarsIndex[j]=1;
			nv++;
			flag=true;
            break;
         }
      }	
	  if(flag==true){
		 for(i=0;i<(int)itvp1->ncons-k;i++){
			if(consIndex[i]!=0) continue;
				l1=itvp1->cons+i*(itvp1->dim+1);
			if(*(l1+j+1)!=0){
				for(m=0;m<(int)itvp1->dim;m++){
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
      if( (nc==(int)itvp1->ncons-k && (int)itvp1->ncons-k >0) || nv==(int)itvp1->dim)  break;
   }
   r=pr=(int *)malloc(nv*sizeof(int));
   memset(r,0,nv*sizeof(int));
   for(j=0;j<(int)itvp1->dim;j++){
      if(rvarsIndex[j]==1){
         *pr=j;
         pr++;	
      } 
   } 
   *nvars=nv;
   return r;
}

/* k starts from 0 */
itvp_t* itvp_remove_kthCons(itvp_t* itvp,int k)
{
    if(itvp->ncons-k-1>0){
       memmove(itvp->cons+k*(2*itvp->dim+1),itvp->cons+(k+1)*(2*itvp->dim+1),(itvp->ncons-k-1)*(2*itvp->dim+1)*sizeof(numdbl_t));
       itvp->ncons--;
    }
    else if(itvp->ncons-1>0){
    	itvp->cons=(numdbl_t *) realloc(itvp->cons,(itvp->ncons-1)*(2*itvp->dim+1)*sizeof(numdbl_t));
        itvp->ncons--;
    }
    else{
    	checked_free(itvp->cons);
    	checked_free(itvp->bnds);
        itvp->ncons=0;
        itvp->flag=UNIVERSE_POL;
    }
    return itvp;
}
