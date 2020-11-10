/*
 * ifm.c
 *
 * Interval Fourier-Motzkin Elimination in floating point arithemetic.
 *
 * APRON Library / itvpol Domain
 *
 * Copyright (C) Liqian CHEN' 2009
 *
 */
/* This file is part of the APRON Library, released under LGPL license.  
   Please read the COPYING file packaged in the distribution.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fenv.h> /* floating-point lib */

#include "ifm.h"
#include "ilp.h"
#include "numitv.h"
#include "itv_linear.h"
#include "itvp_user.h"

/*  Interval Fourier-Motzkin Elimination						            	*/
/*  2 parts:  	1)postive rows + negative rows              				    */
/*  		    2)zero-containted rows                                 	 	    */
/*  2 cases: for postive rows + negative rows             				        */
/*    1)multiplication: if a_1i *_{f,-\infity} (-a_2i)==a_1i *_{f,+\infity} (-a_2i)     */
/*    2)division:       otherwise      							                */
/*  parameter varIndex stores the index(start from 1) of the variable to be eliminated. */
/*  parameter newNbCons to store the number of the new linear constraints. 		*/
/*  return the new linear constraints of Pol. 						            */
itvp_t *ifm_elimination(itvp_internal_t* pr, bool destructive, itvp_t *itvp0, unsigned varIndex)
{
  unsigned *posIndex,posI;
  unsigned *negIndex,negI;
  unsigned *zeroIndex,zeroI;
  unsigned i,j,k;
  numitv_t *coeffi; /* the coefficient of varIndex in row i */
  numdbl_t *itvIneq,*p;
  numitv_t *a1i_itv,*a2i_itv,*a1k_itv,*a2k_itv;
  numdbl_t a3k_l,a3k_u;
  numdbl_t c,c1,c2;   
  numdbl_t *resCons,*resq;
  unsigned ncons1;
  itvp_t *itvp,*itvp1;    
  int flag;
  numdbl_t *l1;
  numitv_t itv1,itv2,itv3,itv4;

  if(itvp0->flag!=GENERAL_POL ){   
     if(destructive==true)  return itvp0;
     return 	itvp_copy_internal(pr, itvp0); 
  }
  itvp=itvp_copy_internal(pr, itvp0); 
  for(i=0;i<itvp->ncons;i++)
  {
	coeffi=(numitv_t *)(itvp->cons+i*(2*itvp->dim+1)+2*(varIndex-1)+1); /*(2*itvp->dim+1) columns per row*/
	if(coeffi->inf <= 0 && coeffi->sup >= 0 && (coeffi->inf != 0 || coeffi->sup != 0) ){  /* contain zero */
	   flag=itv_linearisation_kthVar(itvp->dim,itvp->cons+i*(2*itvp->dim+1),itvp->bnds,varIndex);
	   if(flag==0){  /* an universe constraint */
		if(i<itvp->ncons-1){
		   memmove(itvp->cons+i*(2*itvp->dim+1), itvp->cons+(i+1)*(2*itvp->dim+1),  (itvp->ncons-i-1)*(2*itvp->dim+1)*sizeof(numdbl_t));
		}
	    itvp->cons=(numdbl_t *)realloc(itvp->cons, (itvp->ncons-1)*(2*itvp->dim+1)*sizeof(numdbl_t));
		itvp->ncons--;
		i--;
	   }
        }
  }
	/* scan all the coefficients of varIndex in each inequality to collect the sign information */
        /*        positive; negative; zero(including -0.0 and +0.0)-contained */
  posI=0;
  negI=0;
  zeroI=0;  
  checked_malloc(pr,posIndex,unsigned,itvp->ncons,return NULL;);
  checked_malloc(pr,negIndex,unsigned,itvp->ncons,return NULL;);
  checked_malloc(pr,zeroIndex,unsigned,itvp->ncons,return NULL;);
  for(i=0;i<itvp->ncons;i++)
  {
	coeffi=(numitv_t *)(itvp->cons+i*(2*itvp->dim+1)+2*(varIndex-1)+1); /*(2*itvp->dim+1) columns per row*/
	if(coeffi->inf > 0){
		posIndex[posI]=i;
		posI++;
    }
	else if(coeffi->sup <0){
		negIndex[negI]=i;
		negI++;
    }
    else {
		zeroIndex[zeroI]=i;
		zeroI++;
    }
  }
  if(posI*negI+zeroI==0){
	if(destructive==false)
		itvp1=itvp_copy_internal(pr, itvp);
	else	itvp1=itvp;
	checked_free(itvp1->cons);
    checked_free(itvp1->bnds);
	checked_free(posIndex);
	checked_free(negIndex);
	checked_free(zeroIndex);
	itvp1->ncons=0;
	itvp1->flag=UNIVERSE_POL;
	return itvp1;
  }
  if(posI+negI==0){
    itvp1=itvp_copy_internal(pr,itvp);
	for(i=0;i<itvp->ncons;i++){
	  l1=itvp1->cons+i*(2*itvp1->dim+1);
	  *(l1+2*(varIndex-1)+1)=0.0;
	  *(l1+2*(varIndex-1)+1+1)=0.0;
	}	
	*(itvp1->bnds+2*(varIndex-1))=0.0;
	*(itvp1->bnds+2*(varIndex-1)+1)=0.0;
	checked_free(posIndex);
	checked_free(negIndex);
	checked_free(zeroIndex);
	if(destructive==true)	itvp_free_internal(pr, itvp0);
    itvp_free_internal(pr, itvp);
	return itvp1;
  }
  /* ------------------------------------------------------------------ */
  /* Sound Interval Fourier Elimination with floating point arithmetic  */
  /* ------------------------------------------------------------------ */
  checked_malloc(pr,resCons,numdbl_t,(posI*negI+zeroI)*(2*itvp->dim+1),return NULL;);
  resq=resCons;
  ncons1=0;
  if(posI>0 && negI>0){
     checked_malloc(pr,itvIneq,numdbl_t,2*itvp->dim+1,return NULL;);
     k=0; /* the index for the new generated constraint */
     for(i=0;i<posI;i++) /* positive row: posIndex[i] */
	 {
		a1i_itv=(numitv_t *)(itvp->cons+posIndex[i]*(2*itvp->dim+1)+2*(varIndex-1)+1); /* a_1i of row posIndex[i] */
		c1=*(itvp->cons+posIndex[i]*(2*itvp->dim+1)); /* 0-th(constant) of row posIndex[i] */

		/* ----------------------------------- */
		/* positive row + negative row       */
		/* ----------------------------------- */
		for(j=0;j<negI;j++) /* negative row: negIndex[j] */
		{
		   a2i_itv=(numitv_t *)(itvp->cons+negIndex[j]*(2*itvp->dim+1)+2*(varIndex-1)+1);/*a_2i of row negIndex[j] */
		   c2=*(itvp->cons+negIndex[j]*(2*itvp->dim+1));
			  /* positive row + negative row */
		   memset(itvIneq,0,(2*itvp->dim+1)*sizeof(numdbl_t));
		   p=itvIneq;
			  /* constant term */
		   itv1.inf=c1;
		   itv1.sup=c1;
		   itv2.inf=c2;
		   itv2.sup=c2;
		   itv3=numitv_div(itv1,*a1i_itv);
		   itv4=numitv_div(itv2,numitv_neg(*a2i_itv));
		   itv1=numitv_add(itv3,itv4);
		   *p=itv1.sup;
		   p++;
		   for(k=1; k<=itvp->dim; k++){ /* negative row: negIndex[j] */
				if(k!=varIndex){
					a1k_itv=(numitv_t *)(itvp->cons+posIndex[i]*(2*itvp->dim+1)+2*(k-1)+1);/*a_1k of row posIndex[i] */
					a2k_itv=(numitv_t *)(itvp->cons+negIndex[j]*(2*itvp->dim+1)+2*(k-1)+1);/*a_2k of row negIndex[j] */
					itv1=numitv_div(*a1k_itv,*a1i_itv);
					itv2=numitv_div(*a2k_itv,numitv_neg(*a2i_itv));
					itv3=numitv_add(itv1,itv2);
					*p=itv3.inf;
					*(p+1)=itv3.sup;
				}
				p=p+2;
			}
			memcpy(resq,itvIneq,(2*itvp->dim+1)*sizeof(numdbl_t));
			resq=resq+2*itvp->dim+1;
			ncons1++;
		}
     }
     checked_free(itvIneq);
  }

  /* ---------------------------------------------------------------- */
  /* copy constraints in which the coefficient for V_varIndex is zero */
  /* ---------------------------------------------------------------- */
  for(i=0;i<zeroI;i++) /* zero row: zeroIndex[i] */
  {
	memcpy(resq,itvp->cons+zeroIndex[i]*(2*itvp->dim+1),(2*itvp->dim+1)*sizeof(numdbl_t));
	resq=resq+2*itvp->dim+1;
	ncons1++;
  }

   checked_free(posIndex);
   checked_free(negIndex);
   checked_free(zeroIndex);

   checked_free(itvp->cons);
   checked_free(itvp->bnds);
   checked_free(itvp);
  /* --------------------------------------------------------------- */
  /* post-processing before return result constraints                */
  /* --------------------------------------------------------------- */

   if(destructive==false)
	itvp1=itvp_copy_internal(pr, itvp0);
   else	itvp1=itvp0;
   if(ncons1>0){
      resCons=(numdbl_t *)realloc(resCons,ncons1*(2*itvp1->dim+1)*sizeof(numdbl_t)); /* truncate */
      checked_free(itvp1->cons);
      itvp1->ncons=ncons1;
      itvp1->cons=resCons;
   }
   return itvp1;
}

itvp_t *itvp_redundancy_removal_quasi_syntactic(itvp_internal_t* pr,bool destructive, itvp_t *itvp)
{
    char *rrIndex;
    int i,j,m;
    itvp_t *itvp1;
    numdbl_t *cons1;
    numdbl_t *li,*lj;
    numdbl_t lim,ljm,dm,maxm;
    char *bits1;
    int n,k;

    if(destructive==false)
		itvp1=itvp_copy_internal(pr, itvp);
    else	itvp1=itvp;
    if(itvp1->flag!=GENERAL_POL || itvp1->ncons<=1 ) return itvp1;

    k=itvp1->ncons;
    checked_malloc(pr,rrIndex,char,k,return NULL;);
    memset(rrIndex,0,k*sizeof(char));
    i=0;
	while(i<k-1){
		if(rrIndex[i]==true){
			i++;
			continue;
		}
		li=itvp1->cons+i*(2*itvp1->dim+1);
			/* quasi-syntactic */
		for(j=i+1;j<(int)itvp1->ncons;j++){
		   if(rrIndex[j]==true)		continue;
		   lj=itvp1->cons+j*(2*itvp1->dim+1);
		   for(m=1;m<=2*(int)(int)(int)(int)itvp1->dim;m++){
			lim=*(li+m);
			ljm=*(lj+m);
			if(lim==ljm) continue;
			if( (lim>0.0 && ljm<-0.0) || (lim<-0.0 && ljm>0.0) )
				break;
			  /* with the same sign */
			if(lim >= ljm)   dm=lim-ljm;
			else	         dm=ljm-lim;
			maxm=fmax(fabs(lim),fabs(ljm));
			if( maxm==0 ){
			   if( dm>EPSILON ) break;
			}
			else if(dm/maxm >EPSILON ) break;
		   }
		   if(m> 2*(int)itvp1->dim){ /* ei == ej*/
				if((*li <= *lj) && (j<k)){  /* lj is redundant */
				rrIndex[j]=true;
				}
				else{ /* li is redundant */
				rrIndex[i]=true;
				break;
			}
		   }
		}
		i++;
    }	
    checked_malloc(pr,cons1,numdbl_t,itvp1->ncons*(2*itvp1->dim+1),return NULL;);
    n=0;
    for(i=0;i<k;i++){
		if(rrIndex[i]==true){
		   continue;
		}
		   /* i-th constraint is non-redundant */
		memcpy(cons1+n*(2*itvp1->dim+1),itvp1->cons+i*(2*itvp1->dim+1),(2*itvp1->dim+1)*sizeof(numdbl_t));
		n++;
    } 	
    cons1=realloc(cons1,n*(2*itvp1->dim+1)*sizeof(numdbl_t));
    checked_free(itvp1->cons);
    checked_free(rrIndex);
    itvp1->cons=cons1;
    itvp1->ncons=n;
    return itvp1;
}

itvp_t *redundancy_removal_ILP_backward(itvp_internal_t* pr,bool destructive, itvp_t *itvp, bool eps)
{
    int i,j;
    numdbl_t *objVector;
    numdbl_t c;
    numdbl_t ci;
    itvp_t *itvp1,*itvp11;
    numdbl_t cic,maxci;
    bool flag;
    numdbl_t epsValue;

    if(destructive==false)	itvp1=itvp_copy_internal(pr, itvp);
    else	itvp1=itvp;
    if(itvp1->flag!=GENERAL_POL || itvp1->ncons<=1 ) return itvp1;

#if INTPROGRAM 	
    epsValue=EPSILONINT;
#else
    epsValue=EPSILON;
#endif

    checked_malloc(pr,objVector,numdbl_t,2*itvp1->dim,return NULL;);
    i=itvp1->ncons-1;
    while(i>=0){
        flag=false;
		if(itvp1->ncons<=2){
			   checked_free(objVector);
		   return itvp1; /* less than 2 linear constraint */
		}
		c=*(itvp1->cons+i*(2*itvp1->dim+1));
		memcpy(objVector,itvp1->cons+i*(2*itvp1->dim+1)+1,2*itvp1->dim*sizeof(numdbl_t));
		itvp11=itvp_copy_internal(pr, itvp1);
		itvp11=itvp_remove_kthCons(itvp11,i);
		ci=ilp_std(itvp11, GLP_MAX, objVector);
		itvp_free_internal(pr, itvp11);
		if(ci<=c){  /* the i-th linear constraint is redundant */
			flag=true;
		}
		else if(eps==true){ /* ci > c*/
			cic=ci-c;
			maxci=fabs(c);/*maxci=fmax(-c,ci);*/
			if( (maxci==0.0 && cic<epsValue) || (maxci>0.0 && cic/maxci<epsValue) ){
				/*the i-th linear constraint is redundant*/
				flag=true;
			}
		}
		if(flag==true){
		   /* remove i-th linear constraint */
			   itvp1=itvp_remove_kthCons(itvp1,i);
		}
		i--;
    }
    checked_free(objVector);
    return itvp1;
}


itvp_t *redundancy_removal_backward(itvp_internal_t* pr, bool destructive,itvp_t *itvp, bool eps)
{
    itvp_t *itvp1;    
    if(destructive==false)
		itvp1=itvp_copy_internal(pr, itvp);
    else	itvp1=itvp;
    if(itvp1->flag!=GENERAL_POL || itvp1->ncons<=1) return itvp1;

    itvp1=itvp_redundancy_removal_quasi_syntactic(pr,true,itvp1);
    if(itvp1->flag!=GENERAL_POL || itvp1->ncons<=1) return itvp1;
    itvp1=redundancy_removal_ILP_backward(pr,true,itvp1,eps); 
    return itvp1;
}

/*  Select the optimal variable to eliminate		*/
/*  min(postive rows * negative rows)   		*/
/*  variable index from 1   				*/
unsigned ifm_select(itvp_t *itvp, unsigned startI, unsigned endI)
{
  unsigned posI[100];
  unsigned negI[100];
  unsigned i,j,minI=0,min;
  numdbl_t *con,ai_l,ai_u; /* the coefficient of varIndex in row i */

  if(startI==endI) return startI;

  for(i=startI;i<=endI;i++){
      posI[i]=0; 
      negI[i]=0;
  } 
  for(i=0;i<itvp->ncons;i++)
  {
      con=itvp->cons+i*(2*itvp->dim+1);
      for(j=startI;j<=endI;j++){
	 ai_l=*(con+2*(j-1)+1);
	 ai_u=*(con+2*j);
         if(ai_l>0.0)        posI[j]++; 
	 else if(ai_u<-0.0)  negI[j]++; 
      } 
  }
  min=100000000;
  for(i=endI;i>=startI;i--){
      j=posI[i]*negI[i];
      if(j<min){
		  minI=i;
		  min=j;
      }	
  } 
  return minI;
}

