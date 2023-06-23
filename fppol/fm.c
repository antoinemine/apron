/*
 * fm.c
 *
 * Sound Fourier-Motzkin elimination in floating point arithemetic.
 *
 * APRON Library / FPPol Domain
 *
 * Copyright (C) Liqian CHEN' 2008-
 *
 */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fenv.h> /* floating-point lib */

#include "fm.h"
#include "rlp.h"
#include "numitv.h"
#include "quasi_linear.h"

/*  Sound Fourier-Motzkin Elimination						        	*/
/*  2 parts: 1)postive rows + negative rows              				*/
/*  	     2)zero rows                                 				*/
/*  2 cases: for postive rows + negative rows             				*/
/*    1)multiplication: if a_1i *_{f,-\infity} (-a_2i)==a_1i *_{f,+\infity} (-a_2i)     */
/*    2)division:       otherwise      							                        */
/*  parameter varIndex stores the index(start from 1) of the variable to be eliminated. */
/*  parameter newNbCons to store the number of the new linear constraints. 		        */
/*  return the new linear constraints of Pol. 					                       	*/
fpp_t *fm_elimination(fpp_internal_t* pr, bool destructive, fpp_t *fpp, unsigned varIndex, bit_vectors_t* bv,bool *subset)
{
  unsigned *posIndex,posI;
  unsigned *negIndex,negI;
  unsigned *zeroIndex,zeroI;
  unsigned i,j,k;
  numdbl_t *coeffi; /* the coefficient of varIndex in row i */
  numdbl_t *itvIneq,*intvp;
  numdbl_t a1k,a1i,a2i,a2k,a3k,b3k;
  numdbl_t c,c1,c2;
  numdbl_t *resCons,*resq,*aCons;
  unsigned ncons1;
  numdbl_t a1x2idown,a1x2iup;
  numdbl_t t,tInt;
  unsigned m;
  numitv_t b3kIntv;
  bool dealFlag;
  fpp_t *fpp1;
  int sat;
  unsigned nenvs;
  bit_vectors_t* bv1;
  char *vec;
  numdbl_t *l1,*l2;

  *subset=true;
  if(fpp->flag!=GENERAL_POL ){
     if(destructive==true)  return fpp;
     return 	fpp_copy_internal(pr, fpp);
  }
	/* scan all the coefficients of varIndex in each inequality to collect the sign information */
    /*        positive; negative; zero(including -0.0 and +0.0) */
  posI=0;
  negI=0;
  zeroI=0;
  checked_malloc(pr,posIndex,unsigned,fpp->ncons, return NULL;);
  checked_malloc(pr,negIndex,unsigned,fpp->ncons, return NULL;);
  checked_malloc(pr,zeroIndex,unsigned,fpp->ncons, return NULL;);
  for(i=0;i<fpp->ncons;i++)
  {
  	coeffi=fpp->cons+i*(fpp->dim+1)+varIndex; /*(fpp->dim+1) columns per row*/
  	if(*coeffi>0.0){
  		posIndex[posI]=i;
  		posI++;
    }
  	else if(*coeffi<-0.0){
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
  		    fpp1=fpp_copy_internal(pr, fpp);
  	else	fpp1=fpp;
  	checked_free(fpp1->cons);
    checked_free(fpp1->bnds);
  	checked_free(posIndex);
  	checked_free(negIndex);
  	checked_free(zeroIndex);
  	fpp1->ncons=0;
  	fpp1->flag=UNIVERSE_POL;
          /* if(bv!=NULL)  bv_free(bv); */
  	return fpp1;
  }
  if(posI+negI==0){
    fpp1=fpp_copy_internal(pr,fpp);
  	for(i=0;i<fpp->ncons;i++){
  	  l1=fpp1->cons+i*(fpp1->dim+1);
  	  *(l1+varIndex)=0.0;
  	}
  	*(fpp1->bnds+2*(varIndex-1))=0.0;
  	*(fpp1->bnds+2*(varIndex-1)+1)=0.0;
  	checked_free(posIndex);
  	checked_free(negIndex);
  	checked_free(zeroIndex);
  	if(destructive==true)	fpp_free_internal(pr, fpp);
  	return fpp1;
  }
  /* --------------------------------------------------------- */
  /* Sound Fourier Elimination with floating point arithmetic  */
  /* --------------------------------------------------------- */
  checked_malloc(pr,resCons,numdbl_t,(posI*negI+zeroI)*(fpp->dim+1), return NULL;);
  resq=resCons;
  if(bv!=NULL){
     nenvs=fpp->ncons-bv->nvecs;
     bv1=bv_copy(bv);
     bv1->nvecs=bv1->nvecs+posI*negI+zeroI;
     bv1->bits = realloc(bv1->bits,bv1->nvecs*bv1->vchars*sizeof(char));
  }
  ncons1=0;
  if(posI>0 && negI>0){
     checked_malloc(pr,itvIneq,numdbl_t,2*fpp->dim+1, return NULL;);
     k=0; /* the index for the new generated constraint */
     for(i=0;i<posI;i++) /* positive row: posIndex[i] */
     {
      	a1i=*(fpp->cons+posIndex[i]*(fpp->dim+1)+varIndex); /* a_1i of row posIndex[i] */
      	c1=*(fpp->cons+posIndex[i]*(fpp->dim+1)); /* 0-th(constant) of row posIndex[i] */

      	/* ----------------------------------- */
      	/* positive row + negative row       */
      	/* ----------------------------------- */
      	for(j=0;j<negI;j++) /* negative row: negIndex[j] */
      	{
      		dealFlag=false;
      		a2i=*(fpp->cons+negIndex[j]*(fpp->dim+1)+varIndex); /* a_2i of row negIndex[j] */
      		   /* positive row + negative row */
            memset(itvIneq,0,(2*fpp->dim+1)*sizeof(numdbl_t));
      		intvp=itvIneq;
      		   /* see if t=a_1i/(-a_2i) is an integer, and a1i==t *_{f,+\infty} (-a_2i)==t *_{f,-\infty} (-a_2i) */
      		if(a1i == -a2i){
      			dealFlag=true;
        	 	c2=*(fpp->cons+negIndex[j]*(fpp->dim+1)); /* 0-th(constant) of row negIndex[j] */
      			c=c1+c2;
      			*intvp=c;
      			intvp++;
      			for(k=1; k<=fpp->dim; k++){ /* negative row: negIndex[j] */
      			   if(k!=varIndex){
      			       a1k=*(fpp->cons+posIndex[i]*(fpp->dim+1)+k); /* a_1k of row posIndex[i] */
      			       a2k=*(fpp->cons+negIndex[j]*(fpp->dim+1)+k); /* a_2k of row negIndex[j] */
      				     /* b3k=a1k+a2k;  */
      			       b3kIntv=numitv_add_number(a1k,a2k);
      			       memcpy(intvp,&b3kIntv,2*sizeof(numdbl_t));
      			   }
      			   intvp=intvp+2;
                }
      		}
      		else if(a1i > -a2i){
      		  t=a1i/(-a2i);  /* no matter which rounding mode */
      		  tInt=trunc(t);
      		  if(tInt==t){ /* t is an integer */
      		     if((t*(-a2i) == a1i) && (t*(-a2i)==num_mul_downward(t,-a2i))){/*row_posIndex[i]+t*row_negIndex[j]*/
            			dealFlag=true;
              	 	    c2=*(fpp->cons+negIndex[j]*(fpp->dim+1)); /* 0-th(constant) of row negIndex[j] */
            			c=c1+c2*t;
            			*intvp=c;
            			intvp++;
            			for(k=1; k<=fpp->dim; k++){ /* negative row: negIndex[j] */
            			   if(k!=varIndex){
            			       a1k=*(fpp->cons+posIndex[i]*(fpp->dim+1)+k); /* a_1k of row posIndex[i] */
            			       a2k=*(fpp->cons+negIndex[j]*(fpp->dim+1)+k); /* a_2k of row negIndex[j] */
            				     /* b3k=a1k+t*a2k;  */
            			       if(a1k==0.0 || a1k==-0.0 ){
                				    if(a2k==0.0 || a2k==-0.0 ){
                				       intvp=intvp+2;
                				       continue;
                				    }
                				    b3kIntv=numitv_mul_number(t,a2k);
            			       }
            			       else if(a2k==0.0 || a2k==-0.0 )
                                    b3kIntv=numitv_singleton(a1k);
            			       else b3kIntv=numitv_add( numitv_singleton(a1k),numitv_mul_number(t,a2k) );
            			       memcpy(intvp,&b3kIntv,2*sizeof(numdbl_t));
            			   }
            			   intvp=intvp+2;
                        }
      		     }
      		  }
      		}
      		else{ /* a1i < -a2i */
      		  t=(-a2i)/a1i;  /* no matter which rounding mode */
      		  tInt=trunc(t);
      		  if(tInt==t){ /* t is an integer */
      		     if((t*a1i == -a2i) && (t*a1i==num_mul_downward(t,a1i))){/* t*row_posIndex[i]+row_negIndex[j]*/
            			dealFlag=true;
            			c2=*(fpp->cons+negIndex[j]*(fpp->dim+1)); /* 0-th(constant) of row negIndex[j] */
            			c=c1*t+c2;
            			*intvp=c;
            			intvp++;
            			for(k=1; k<=fpp->dim; k++){ /* negative row: negIndex[j] */
            			   if(k!=varIndex){
            			       a1k=*(fpp->cons+posIndex[i]*(fpp->dim+1)+k); /* a_1k of row posIndex[i] */
            			       a2k=*(fpp->cons+negIndex[j]*(fpp->dim+1)+k); /* a_2k of row negIndex[j] */
            				     /* b3k=t*a1k+a2k;  */
            			       if(a1k==0.0 || a1k==-0.0 ){
                				    if(a2k==0.0 || a2k==-0.0 ){
                				       intvp=intvp+2;
                				       continue;
                				    }
                				    b3kIntv=numitv_singleton(a2k);
            			       }
            			       else if(a2k==0.0 || a2k==-0.0 )
            				          b3kIntv=numitv_mul_number(t,a1k);
            			       else b3kIntv=numitv_add( numitv_mul_number(t,a1k),numitv_singleton(a2k) );
            			       memcpy(intvp,&b3kIntv,2*sizeof(numdbl_t));
            			   }
            			   intvp=intvp+2;
                       }
      		     }
      		  }
      		}
      		if(dealFlag==false){
      		   /* see if a_1i *_{f,-\infity} (-a_2i)==a_1i *_{f,+\infity} (-a_2i) */
      		   a1x2idown=num_mul_downward(a1i,-a2i);
      		   a1x2iup=a1i*(-a2i);
      		   if(a1x2iup==a1x2idown &&a1x2iup<MAX_MULCOEF && a1i<MAX_MULCOEF && -a2i<MAX_MULCOEF){/* multiplication */
        		     /* roundup now; */
      		     c2=*(fpp->cons+negIndex[j]*(fpp->dim+1)); /* 0-th(constant) of row negIndex[j] */
      		     c=c1*(-a2i)+c2*a1i;
      		     *intvp=c;
      		     intvp++;
      		     for(k=1; k<=fpp->dim; k++){ /* negative row: negIndex[j] */
      		        if(k!=varIndex){
      		           a1k=*(fpp->cons+posIndex[i]*(fpp->dim+1)+k); /* a_1k of row posIndex[i] */
            		   a2k=*(fpp->cons+negIndex[j]*(fpp->dim+1)+k); /* a_2k of row negIndex[j] */
            		   if( (a1k==a1i && a2k==a2i) || (a1k==-a1i && a2k==-a2i) ){ /* crossing */
            		   }
            		   else{  /* b3k=a1k*(-a2i)+a2k*a1i;   */
            			      if(a1k==0.0 || a1k==-0.0 ){
                				    if(a2k==0.0 || a2k==-0.0 ){
                				       intvp=intvp+2;
                				       continue;
                				    }
                				    b3kIntv=numitv_mul_number(a2k,a1i);
            			      }
            			      else if(a2k==0.0 || a2k==-0.0 )
            				        b3kIntv=numitv_mul_number(a1k,-a2i);
            		          else b3kIntv=numitv_add( numitv_mul_number(a1k,-a2i),numitv_mul_number(a2k,a1i) );
            			            memcpy(intvp,&b3kIntv,2*sizeof(numdbl_t));
            		  }
        	 	    }
      		        intvp=intvp+2;
      		      }
      		    }
      		    else{/* division */
      		      c2=*(fpp->cons+negIndex[j]*(fpp->dim+1)); /* 0-th(constant) of row negIndex[j] */
      		      c=c1/a1i+c2/(-a2i);
      		      *intvp=c;
      		      intvp++;
      		      for(k=1; k<=fpp->dim; k++){ /* negative row: negIndex[j] */
      		        if(k!=varIndex){
            			   a1k=*(fpp->cons+posIndex[i]*(fpp->dim+1)+k); /* a_1k of row posIndex[i] */
            			   a2k=*(fpp->cons+negIndex[j]*(fpp->dim+1)+k); /* a_2k of row negIndex[j] */
            			     /* b3k=a1k/a1i+a2k/(-a2i);    */
            			   if(a1k==0.0 || a1k==-0.0 ){
            				    if(a2k==0.0 || a2k==-0.0 ){
            				       intvp=intvp+2;
            				       continue;
            				    }
            				    b3kIntv=numitv_div_number(a2k,-a2i);
            			   }
            			   else if(a2k==0.0 || a2k==-0.0 )
            				    b3kIntv=numitv_div_number(a1k,a1i);
            		       else b3kIntv=numitv_add( numitv_div_number(a1k,a1i),numitv_div_number(a2k,-a2i) );
            			        memcpy(intvp,&b3kIntv,2*sizeof(numdbl_t));
      		        }
      		        intvp=intvp+2;
      		      }
      		     }
      		    }
          		aCons=quasi_linearisation(fpp->dim,itvIneq,fpp->bnds,&sat);
          		if(aCons==NULL){
          		   if(sat==0){/* unsatifiable */
          		      checked_free(itvIneq);
             		  checked_free(resCons);
                      if(bv!=NULL) bv_free(bv1);
                      fpp1=fpp_alloc_internal(pr, fpp->dim,fpp->intdim);
                      if(destructive==true)  fpp_free_internal(pr, fpp);
             		  return fpp1;
                   }
                }
          		else{
          		   memcpy(resq,aCons,(fpp->dim+1)*sizeof(numdbl_t));
          		   checked_free(aCons);
          		   resq=resq+fpp->dim+1;
          		   if(bv!=NULL){
          			    /* Bit Vectors */
          		      vec=bv_or(bv,bv->bits+posIndex[i]*bv->vchars,bv->bits+negIndex[j]*bv->vchars);
          		      bv_set(bv1,ncons1,vec);
          		      checked_free(vec);
          		   }
          		   ncons1++;
          		}
      	}
     }
     checked_free(itvIneq);
  }

  /* ---------------------------------------------------------------- */
  /* copy constraints in which the coefficient for V_varIndex is zero */
  /* ---------------------------------------------------------------- */
  if(ncons1>0)   *subset=false;
  for(i=0;i<zeroI;i++) /* zero row: zeroIndex[i] */
  {
    	memcpy(resq,fpp->cons+zeroIndex[i]*(fpp->dim+1),(fpp->dim+1)*sizeof(numdbl_t));
    	resq=resq+fpp->dim+1;
    	if(bv!=NULL){
    		/* Bit Vectors */
    	  bv_set(bv1,ncons1,bv->bits+zeroIndex[i]*bv->vchars);
    	}
    	ncons1++;
   }

  /* --------------------------------------------------------------- */
  /* post-processing before return result constraints                */
  /* --------------------------------------------------------------- */
   if(destructive==false)
	    fpp1=fpp_copy_internal(pr, fpp);
   else	fpp1=fpp;

   if(ncons1>0){
      resCons=(numdbl_t *)realloc(resCons,ncons1*(fpp->dim+1)*sizeof(numdbl_t)); /* truncate */
      if(bv!=NULL){
	       /* Bit Vectors */
         bv1->nvecs=ncons1-nenvs;
         bv1->bits = realloc(bv1->bits,bv1->nvecs*bv1->vchars*sizeof(char));
         bv->nvecs=bv1->nvecs;
         checked_free(bv->bits);
         bv->bits=bv1->bits;
         checked_free(bv1);
      }
      checked_free(fpp1->cons);
      fpp1->ncons=ncons1;
      fpp1->cons=resCons;
   }
   else{
      if(bv!=NULL) bv_free(bv1);
      checked_free(resCons);
      checked_free(fpp1->cons);
      checked_free(fpp1->bnds);
      fpp1->ncons=0;
      fpp1->flag=UNIVERSE_POL;
   }
   checked_free(posIndex);
   checked_free(negIndex);
   checked_free(zeroIndex);

   return fpp1;
}

/*  Sound Lazy Fourier-Motzkin Elimination							    */
/*  2 parts: 1)postive rows + negative rows              				*/
/*  		 2)zero rows                                 				*/
/*  2 cases: for postive rows + negative rows             				*/
/*    1)multiplication: if a_1i *_{f,-\infity} (-a_2i)==a_1i *_{f,+\infity} (-a_2i)     */
/*    2)division:       otherwise      						                        	*/
/*  parameter varIndex stores the index(start from 1) of the variable to be eliminated. */
/*  parameter newNbCons to store the number of the new linear constraints. 		        */
/*  return the new linear constraints of Pol. 						                    */
fpp_t *fm_elimination_lazy(fpp_internal_t* pr, bool destructive, fpp_t *fpp, unsigned varIndex, bit_vectors_t* bv,unsigned nelimv,bool *subset)
{
  unsigned *posIndex,posI;
  unsigned *negIndex,negI;
  unsigned *zeroIndex,zeroI;
  unsigned i,j,k;
  numdbl_t *coeffi; /* the coefficient of varIndex in row i */
  numdbl_t *itvIneq,*intvp;
  numdbl_t *l1,*l2;
  numdbl_t *sigcons,*sigconsp;
  numdbl_t *itvcons,*itvconsp;
  unsigned nitvcons,nsigcons;
  numdbl_t a1k,a1i,a2i,a2k,a3k,b3k;
  numdbl_t c,c1,c2;
  numdbl_t *resCons,*resq,*aCons;
  unsigned ncons1;
  numdbl_t a1x2idown,a1x2iup;
  numdbl_t t,tInt;
  unsigned m;
  numitv_t b3kIntv;
  bool dealFlag;
  fpp_t *fpp1;
  int sat;
  bit_vectors_t* bv1;
  char *vec;
  fm_pair_t *fmp,*fmp_p;
  int res;

  *subset=true;
  if(fpp->flag!=GENERAL_POL ){
     if(destructive==true)  return fpp;
     return 	fpp_copy_internal(pr, fpp);
  }
	/* scan all the coefficients of varIndex in each inequality to collect the sign information */
  /*        positive; negative; zero(including -0.0 and +0.0) */
  posI=0;
  negI=0;
  zeroI=0;
  checked_malloc(pr,posIndex,unsigned,fpp->ncons, return NULL;);
  checked_malloc(pr,negIndex,unsigned,fpp->ncons, return NULL;);
  checked_malloc(pr,zeroIndex,unsigned,fpp->ncons, return NULL;);
  for(i=0;i<fpp->ncons;i++)
  {
	coeffi=fpp->cons+i*(fpp->dim+1)+varIndex; /*(fpp->dim+1) columns per row*/
	if(*coeffi>0.0){
		posIndex[posI]=i;
		posI++;
    }
	else if(*coeffi<-0.0){
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
		    fpp1=fpp_copy_internal(pr, fpp);
	else	fpp1=fpp;
	checked_free(fpp1->cons);
    checked_free(fpp1->bnds);
	checked_free(posIndex);
	checked_free(negIndex);
	checked_free(zeroIndex);
	fpp1->ncons=0;
	fpp1->flag=UNIVERSE_POL;
        /* if(bv!=NULL)  bv_free(bv); */
	return fpp1;
  }
  /* --------------------------------------------------------- */
  /* Sound Fourier Elimination with floating point arithmetic  */
  /* --------------------------------------------------------- */
  assert(bv!=NULL);
  bv1=bv_copy(bv);
  bv1->nvecs=posI*negI+zeroI;
  bv1->bits = realloc(bv1->bits,bv1->nvecs*bv1->vchars*sizeof(char));
  ncons1=0;
  if(posI*negI==0){
     checked_malloc(pr,resCons,numdbl_t,(zeroI)*(fpp->dim+1), return NULL;);
     resq=resCons;
     for(i=0;i<zeroI;i++) /* zero row: zeroIndex[i] */
     {
	    memcpy(resq,fpp->cons+zeroIndex[i]*(fpp->dim+1),(fpp->dim+1)*sizeof(numdbl_t));
	    resq=resq+fpp->dim+1;
        bv_set(bv1,i,bv->bits+zeroIndex[i]*bv->vchars);
     }
     if(destructive==false)	fpp1=fpp_copy_internal(pr, fpp);
     else	                fpp1=fpp;
     checked_free(fpp1->cons);
     fpp1->cons=resCons;
     fpp1->ncons=zeroI;

     bv->nvecs=bv1->nvecs;
     checked_free(bv->bits);
     bv->bits=bv1->bits;
     checked_free(bv1);
     checked_free(posIndex);
     checked_free(negIndex);
     checked_free(zeroIndex);
     return fpp1;
  }

  /* posI>0 && negI>0 */
  checked_malloc(pr,fmp,fm_pair_t,(posI*negI+zeroI), return NULL;);
  fmp_p=fmp;
  for(i=0;i<zeroI;i++){ /* zero row: zeroIndex[i] */
	bv_set(bv1,ncons1,bv->bits+zeroIndex[i]*bv->vchars);
  	fmp_p->posI=zeroIndex[i];
	fmp_p->negI=zeroIndex[i];
	fmp_p++;
	ncons1++;
  }
  for(i=0;i<posI;i++){ /* positive row: posIndex[i] */
	for(j=0;j<negI;j++){ /* negative row: negIndex[j] */
	    vec=bv_or(bv,bv->bits+posIndex[i]*bv->vchars,bv->bits+negIndex[j]*bv->vchars);
	    bv_set(bv1,ncons1,vec);
	    checked_free(vec);
  	    fmp_p->posI=posIndex[i];
	    fmp_p->negI=negIndex[j];
	    fmp_p++;
	    ncons1++;
	}
   }
   fmp_p=bv_redundancy_removal_byKohler_lazy(pr,ncons1,zeroI,nelimv,fmp,bv1);
   checked_free(fmp);
   fmp=fmp_p;
   ncons1=bv1->nvecs;
   if(zeroI>0) checked_malloc(pr,sigcons,numdbl_t,(zeroI*(fpp->dim+1)), return NULL;);
   else sigcons=NULL;
   checked_malloc(pr,itvcons,numdbl_t,(posI*negI*(2*fpp->dim+1)), return NULL;);
   nitvcons=0;
   nsigcons=0;
   fm_elimination_combination(pr,fpp,ncons1,fmp,varIndex,&nitvcons,&nsigcons,itvcons,sigcons);
   checked_free(fmp);
	/* quasi-linearisation */
   if(nsigcons>0) sigcons=realloc(sigcons,(nsigcons+nitvcons)*(fpp->dim+1)*sizeof(numdbl_t));
   else checked_malloc(pr,sigcons,numdbl_t,nitvcons*(fpp->dim+1), return NULL;);
   resq=sigcons+nsigcons*(fpp->dim+1);
   for(i=0;i<nitvcons;i++){
	itvIneq=itvcons+i*(2*fpp->dim+1);
	aCons=quasi_linearisation(fpp->dim,itvIneq,fpp->bnds,&sat);
	if(aCons==NULL){
	   if(sat==0){/* unsatifiable */
	   	checked_free(sigcons);
		checked_free(itvcons);
        bv_free(bv1);
		checked_free(posIndex);
		checked_free(negIndex);
		checked_free(zeroIndex);
        fpp1=fpp_alloc_internal(pr, fpp->dim,fpp->intdim);
        if(destructive==true)  fpp_free_internal(pr, fpp);
   		return fpp1;
       }
	   else{
	      j=bv1->nvecs-(nitvcons-i);
	      if(bv1->nvecs-j-1>0)
               memmove(bv1->bits+j*bv1->vchars,bv1->bits+(j+1)*bv1->vchars,(bv1->nvecs-j-1)*bv1->vchars*sizeof(char));
	      else bv->bits=(char *)realloc(bv->bits,(bv->nvecs-1)*bv->vchars*sizeof(char));
	      bv1->nvecs--;
	   }
    }
	else{
	   memcpy(resq,aCons,(fpp->dim+1)*sizeof(numdbl_t));
	   checked_free(aCons);
	   resq=resq+fpp->dim+1;
	}
   }
   ncons1=bv1->nvecs;
   if(ncons1-nsigcons>0)   *subset=false;

   /* --------------------------------------------------------------- */
   /* post-processing before return result constraints                */
   /* --------------------------------------------------------------- */
   if(destructive==false)
	    fpp1=fpp_copy_internal(pr, fpp);
   else	fpp1=fpp;
   if(ncons1>0){
      sigcons=(numdbl_t *)realloc(sigcons,ncons1*(fpp->dim+1)*sizeof(numdbl_t)); /* truncate */
      checked_free(fpp1->cons);
      checked_free(itvcons);
      fpp1->ncons=ncons1;
      fpp1->cons=sigcons;

      bv1->bits = realloc(bv1->bits,bv1->nvecs*bv1->vchars*sizeof(char));
      bv->nvecs=bv1->nvecs;
      checked_free(bv->bits);
      bv->bits=bv1->bits;
      checked_free(bv1);
   }
   else{
      bv_free(bv1);
      checked_free(sigcons);
      checked_free(itvcons);
      checked_free(fpp1->cons);
      checked_free(fpp1->bnds);
      fpp1->ncons=0;
      fpp1->flag=UNIVERSE_POL;
   }
   checked_free(posIndex);
   checked_free(negIndex);
   checked_free(zeroIndex);

   if(maxCons<(int)fpp1->ncons){
	   maxCons=fpp1->ncons;
	   if(maxCons>1000) printf("maxCons=%d\n",maxCons);
   }

   return fpp1;
}


/* just do the combination to generate interval inequalities and  sigton inequalities */
void fm_elimination_combination(fpp_internal_t* pr, fpp_t *fpp,unsigned ncons1,fm_pair_t *fmp,unsigned varIndex,unsigned *nitvcons,unsigned *nsigcons,numdbl_t *itvcons,numdbl_t *sigcons)
{
     unsigned i,j,k;
     numdbl_t *itvIneq,*intvp;
     numdbl_t *sigp,*itvp;
     numdbl_t a1k,a1i,a2i,a2k,a3k,b3k;
     numdbl_t c,c1,c2;
     numdbl_t a1x2idown,a1x2iup;
     numdbl_t t,tInt;
     unsigned m;
     numitv_t b3kIntv;
     bool dealFlag;
     fm_pair_t *fmp_p;
     unsigned nitv,nsig;

     sigp=sigcons;
     itvp=itvcons;
     nitv=0;
     nsig=0;
     checked_malloc(pr,itvIneq,numdbl_t,2*fpp->dim+1, return ;);
     fmp_p=fmp;
	 for(i=0;i<ncons1;i++){
		if(fmp_p->posI==fmp_p->negI){ /*zero row*/
		   memcpy(sigp,fpp->cons+fmp_p->posI*(fpp->dim+1),(fpp->dim+1)*sizeof(numdbl_t));
		   sigp=sigp+fpp->dim+1;
			   nsig++;
			}
		else{  /* positive row + negative row */
		   a1i=*(fpp->cons+fmp_p->posI*(fpp->dim+1)+varIndex); /* a_1i of row posIndex[i] */
		   c1=*(fpp->cons+fmp_p->posI*(fpp->dim+1)); /* 0-th(constant) of row posIndex[i] */
		   a2i=*(fpp->cons+fmp_p->negI*(fpp->dim+1)+varIndex); /* a_2i of row negIndex[j] */
			   memset(itvIneq,0,(2*fpp->dim+1)*sizeof(numdbl_t));
		   intvp=itvIneq;
		   dealFlag=false;
		   /* see if t=a_1i/(-a_2i) is an integer, and a1i==t *_{f,+\infty} (-a_2i)==t *_{f,-\infty} (-a_2i) */
		   if(a1i == -a2i){
			dealFlag=true;
			c2=*(fpp->cons+fmp_p->negI*(fpp->dim+1)); /* 0-th(constant) of row negIndex[j] */
			c=c1+c2;
			*intvp=c;
			intvp++;
			for(k=1; k<=fpp->dim; k++){ /* negative row: negIndex[j] */
			   if(k!=varIndex){
				   a1k=*(fpp->cons+fmp_p->posI*(fpp->dim+1)+k); /* a_1k of row posIndex[i] */
				   a2k=*(fpp->cons+fmp_p->negI*(fpp->dim+1)+k); /* a_2k of row negIndex[j] */
				  /* b3k=a1k+a2k;  */
				   b3kIntv=numitv_add_number(a1k,a2k);
				   memcpy(intvp,&b3kIntv,2*sizeof(numdbl_t));
			   }
			   intvp=intvp+2;
			}
		   }
		   else if(a1i > -a2i){
			t=a1i/(-a2i);  /* no matter which rounding mode */
			tInt=trunc(t);
			if(tInt==t){ /* t is an integer */
				 if((t*(-a2i) == a1i) && (t*(-a2i)==num_mul_downward(t,-a2i))){/*row_posIndex[i]+t*row_negIndex[j]*/
				dealFlag=true;
				c2=*(fpp->cons+fmp_p->negI*(fpp->dim+1)); /* 0-th(constant) of row negIndex[j] */
				c=c1+c2*t;
				*intvp=c;
				intvp++;
				for(k=1; k<=fpp->dim; k++){ /* negative row: negIndex[j] */
				   if(k!=varIndex){
					   a1k=*(fpp->cons+fmp_p->posI*(fpp->dim+1)+k); /* a_1k of row posIndex[i] */
					   a2k=*(fpp->cons+fmp_p->negI*(fpp->dim+1)+k); /* a_2k of row negIndex[j] */
					  /* b3k=a1k+t*a2k;  */
					   if(a1k==0.0 || a1k==-0.0 ){
						if(a2k==0.0 || a2k==-0.0 ){
						   intvp=intvp+2;
						   continue;
						}
						b3kIntv=numitv_mul_number(t,a2k);
					   }
					   else if(a2k==0.0 || a2k==-0.0 ){
					  b3kIntv=numitv_singleton(a1k);
					   }
					   else b3kIntv=numitv_add( numitv_singleton(a1k),numitv_mul_number(t,a2k) );
					   memcpy(intvp,&b3kIntv,2*sizeof(numdbl_t));
				  }
				  intvp=intvp+2;
						}
				 }
			}
                   }
                   else{ /* a1i < -a2i */
			  t=(-a2i)/a1i;  /* no matter which rounding mode */
			  tInt=trunc(t);
			  if(tInt==t){ /* t is an integer */
				 if((t*a1i == -a2i) && (t*a1i==num_mul_downward(t,a1i))){/* t*row_posIndex[i]+row_negIndex[j]*/
				dealFlag=true;
				c2=*(fpp->cons+fmp_p->negI*(fpp->dim+1)); /* 0-th(constant) of row negIndex[j] */
				c=c1*t+c2;
				*intvp=c;
				intvp++;
				for(k=1; k<=fpp->dim; k++){ /* negative row: negIndex[j] */
				   if(k!=varIndex){
					   a1k=*(fpp->cons+fmp_p->posI*(fpp->dim+1)+k); /* a_1k of row posIndex[i] */
					   a2k=*(fpp->cons+fmp_p->negI*(fpp->dim+1)+k); /* a_2k of row negIndex[j] */
					  /* b3k=t*a1k+a2k;  */
					   if(a1k==0.0 || a1k==-0.0 ){
						if(a2k==0.0 || a2k==-0.0 ){
						   intvp=intvp+2;
						   continue;
						}
						b3kIntv=numitv_singleton(a2k);
					   }
					   else if(a2k==0.0 || a2k==-0.0)
						b3kIntv=numitv_mul_number(t,a1k);
					   else b3kIntv=numitv_add( numitv_mul_number(t,a1k),numitv_singleton(a2k) );
					   memcpy(intvp,&b3kIntv,2*sizeof(numdbl_t));
				  }
				  intvp=intvp+2;
						}
				 }
			  }
			}
			if(dealFlag==false){
			   /* see if a_1i *_{f,-\infity} (-a_2i)==a_1i *_{f,+\infity} (-a_2i) */
			   a1x2idown=num_mul_downward(a1i,-a2i);
			   a1x2iup=a1i*(-a2i);
			   if(a1x2iup==a1x2idown &&a1x2iup<MAX_MULCOEF && a1i<MAX_MULCOEF && -a2i<MAX_MULCOEF){/* multiplication */
				 /* roundup now; */
				 c2=*(fpp->cons+fmp_p->negI*(fpp->dim+1)); /* 0-th(constant) of row negIndex[j] */
				 c=c1*(-a2i)+c2*a1i;
				 *intvp=c;
				 intvp++;
				 for(k=1; k<=fpp->dim; k++){ /* negative row: negIndex[j] */
					if(k!=varIndex){
					   a1k=*(fpp->cons+fmp_p->posI*(fpp->dim+1)+k); /* a_1k of row posIndex[i] */
				   a2k=*(fpp->cons+fmp_p->negI*(fpp->dim+1)+k); /* a_2k of row negIndex[j] */
				   if( (a1k==a1i && a2k==a2i) || (a1k==-a1i && a2k==-a2i) ){ /* crossing */
				   }
				   else{  /* b3k=a1k*(-a2i)+a2k*a1i;   */
					  if(a1k==0.0 || a1k==-0.0 ){
					if(a2k==0.0 || a2k==-0.0 ){
						intvp=intvp+2;
						continue;
					}
						 b3kIntv=numitv_mul_number(a2k,a1i);
								  }
					  else if(a2k==0.0 || a2k==-0.0 )
						 b3kIntv=numitv_mul_number(a1k,-a2i);
						  else b3kIntv=numitv_add( numitv_mul_number(a1k,-a2i),numitv_mul_number(a2k,a1i) );
					  memcpy(intvp,&b3kIntv,2*sizeof(numdbl_t));
				   }
					}
					intvp=intvp+2;
				 }
			   }
			   else{/* division */
				  c2=*(fpp->cons+fmp_p->negI*(fpp->dim+1)); /* 0-th(constant) of row negIndex[j] */
				  c=c1/a1i+c2/(-a2i);
				  *intvp=c;
				  intvp++;
				  for(k=1; k<=fpp->dim; k++){ /* negative row: negIndex[j] */
					if(k!=varIndex){
				   a1k=*(fpp->cons+fmp_p->posI*(fpp->dim+1)+k); /* a_1k of row posIndex[i] */
				   a2k=*(fpp->cons+fmp_p->negI*(fpp->dim+1)+k); /* a_2k of row negIndex[j] */
					 /* b3k=a1k/a1i+a2k/(-a2i);    */
				   if(a1k==0.0 || a1k==-0.0 ){
					if(a2k==0.0 || a2k==-0.0 ){
						intvp=intvp+2;
						continue;
					}
						b3kIntv=numitv_div_number(a2k,-a2i);
				   }
				   else if(a2k==0.0 || a2k==-0.0 )
						 b3kIntv=numitv_div_number(a1k,a1i);
					   else  b3kIntv=numitv_add( numitv_div_number(a1k,a1i),numitv_div_number(a2k,-a2i) );
				   memcpy(intvp,&b3kIntv,2*sizeof(numdbl_t));
					}
					intvp=intvp+2;
				  }
			   }
			}
			memcpy(itvp,itvIneq,(2*fpp->dim+1)*sizeof(numdbl_t));
			itvp=itvp+2*fpp->dim+1;
			nitv++;
		}
		fmp_p++;
     }
     checked_free(itvIneq);
     *nitvcons=nitv;
     *nsigcons=nsig;
     return;
}

/* compare 2 inequalities syntacticly                   */
/* itv:   true (l1,l2 are itv inequality),                 */
/*        false (l1:itv inequality; l2:sigton inequality)   */
/* result:0: uncomparable;          			*/
/*        1: l2 is redundant w.r.p l1; 		        */
/*        2: l1 is redundant w.r.p l2; 			*/
int quasi_syntactic_cmp(numdbl_t *l1,numdbl_t *l2,unsigned dim, bool itv)
{
    numdbl_t c1,c2,a1,a2,a11;
    numdbl_t dm,maxm;
    unsigned i;
    if(itv==true){ /*itv <> itv */
    	for(i=0;i<2*dim;i++){
    	   a1=*(l1+i+1);
    	   if(a1==-0.0){
              *(l1+i+1)=0.0;
    	      a1=0.0;
           }
    	   a2=*(l2+i+1);
    	   if(a2==-0.0){
              *(l2+i+1)=0.0;
    	      a2=0.0;
           }
    	   if(a1==a2) continue;
    	   dm=fabs(a2-a1);
    	   maxm=fmax(fabs(a1),fabs(a2));
    	   if( maxm==0.0 && dm>QSEPSILON ) break;
    	   else if(dm/maxm >QSEPSILON ) break;
      }
      if(i==2*dim){
    	   c1=*l1;
    	   c2=*l2;
     	   if(c1<=c2) return 1;
           else return 2;
      }
      else return 0;
    }
    else{  /*itv <> sigton */
    	for(i=0;i<dim;i++){
    	   a1=*(l1+2*i+1);
    	   if(a1==-0.0){
              *(l1+2*i+1)=0.0;
    	      a1=0.0;
           }
    	   a11=*(l1+2*i+2);
    	   if(a11==-0.0){
              *(l1+2*i+2)=0.0;
    	      a11=0.0;
           }
    	   if(a11!=a1){
    	      dm=fabs(a11-a1);
    	      maxm=fmax(fabs(a11),fabs(a1));
    	      if( maxm==0.0 && dm>QSEPSILON ) break;
    	      else if(dm/maxm >QSEPSILON ) break;
    	   }

    	   a2=*(l2+i+1);
    	   if(a2==-0.0){
              *(l2+i+1)=0.0;
    	      a2=0.0;
           }
    	   if(a2!=a1){
    	      dm=fabs(a2-a1);
    	      maxm=fmax(fabs(a2),fabs(a1));
    	      if( maxm==0.0 && dm>QSEPSILON ) break;
    	      else if(dm/maxm >QSEPSILON ) break;
           }
      }
      if(i==dim){
    	   c1=*l1;
    	   c2=*l2;
     	   if(c1<=c2) return 1;
           else return 2;
      }
      else return 0;
    }
}

/* return the number of nsigcons after redundancy removal */
unsigned itv_quasi_syntactic_redundancy_removal(fpp_internal_t* pr,bit_vectors_t* bv1,unsigned dim,unsigned nitv,unsigned nsig,numdbl_t *itvcons,numdbl_t *sigcons)
{
   unsigned i,j;
   numdbl_t *l1,*l2;
   numdbl_t a1;
   unsigned nitvcons,nsigcons;
   int res;

   nsigcons=nsig;
   nitvcons=nitv;
    /* zero coefficient only for interval inequalities */
   for(i=0;i<nitvcons;i++){
        l1=itvcons+i*(2*dim+1);
        for(j=0;j<2*dim;j++){
      	   a1=*(l1+j+1);
      	   if(a1==-0.0){
              *(l1+j+1)=0.0;
      	      a1=0.0;
           }
      	   if(a1!=0.0) break;
      	}
      	if(j==2*dim){
      	   if(*l1>=-0.0){
      	      memmove(l1,l1+(2*dim+1),(nitvcons-i-1)*(2*dim+1)*sizeof(numdbl_t));
      	      memmove(bv1->bits+(nsigcons+i)*bv1->vchars,bv1->bits+(nsigcons+i+1)*bv1->vchars,(bv1->nvecs-i-nsigcons-1)*bv1->vchars*sizeof(char));
      	      nitvcons--;
      	      bv1->nvecs--;
      	   }
      	}
   }
    /* compare */
   for(i=0;i<nitvcons;i++){
        l1=itvcons+i*(2*dim+1);
        for(j=0;j<nsigcons;j++){
      	   l2=sigcons+j*(dim+1);
           res=quasi_syntactic_cmp(l1,l2,dim,false);
      	   if(res==1){
          		memmove(l2,l2+(dim+1),(nsigcons-j-1)*(dim+1)*sizeof(numdbl_t));
          		memmove(bv1->bits+j*bv1->vchars,bv1->bits+(j+1)*bv1->vchars,(bv1->nvecs-j-1)*bv1->vchars*sizeof(char));
          		nsigcons--;
          		bv1->nvecs--;
           }
           else if(res==2){
          		memmove(l1,l1+(2*dim+1),(nitvcons-i-1)*(2*dim+1)*sizeof(numdbl_t));
          		memmove(bv1->bits+(nsigcons+i)*bv1->vchars,bv1->bits+(nsigcons+i+1)*bv1->vchars,(bv1->nvecs-i-nsigcons-1)*bv1->vchars*sizeof(char));
          		nitvcons--;
          		bv1->nvecs--;
          		i--;
          		break;
           }
        }
      	if(j>=nsigcons){ /* l1 is nonredundant at present */
      	   for(j=i+1;j<nitvcons;j++){
      	      l2=itvcons+j*(2*dim+1);
              res=quasi_syntactic_cmp(l1,l2,dim,true);
      	      if(res==1){
          		  memmove(l2,l2+(2*dim+1),(nitvcons-j-1)*(2*dim+1)*sizeof(numdbl_t));
          		  memmove(bv1->bits+(nsigcons+j)*bv1->vchars,bv1->bits+(nsigcons+j+1)*bv1->vchars,(bv1->nvecs-j-nsigcons-1)*bv1->vchars*sizeof(char));
          		  nitvcons--;
          		  bv1->nvecs--;
              }
              else if(res==2){
          		  memmove(l1,l1+(2*dim+1),(nitvcons-i-1)*(2*dim+1)*sizeof(numdbl_t));
          		  memmove(bv1->bits+(nsigcons+i)*bv1->vchars,bv1->bits+(nsigcons+i+1)*bv1->vchars,(bv1->nvecs-i-nsigcons-1)*bv1->vchars*sizeof(char));
          		  nitvcons--;
          		  bv1->nvecs--;
          		  i--;
          		  break;
              }
           }
        }
   }
   return nsigcons;
}

/*  Sound Fourier-Motzkin Elimination using non-envelope info			*/
/*  2 parts:  1)postive rows + negative rows              				*/
/*  		  2)zero rows                                 				*/
/*  2 cases: for postive rows + negative rows             				*/
/*    1)multiplication: if a_1i *_{f,-\infity} (-a_2i)==a_1i *_{f,+\infity} (-a_2i)     */
/*    2)division:       otherwise      							                        */
/*  parameter varIndex stores the index(start from 1) of the variable to be eliminated. */
/*  parameter newNbCons to store the number of the new linear constraints. 		        */
/*  return the new linear constraints of Pol. 						                    */
fpp_t *fm_elimination_withNenv(fpp_internal_t* pr, bool destructive, fpp_t *fpp, unsigned varIndex, bit_vectors_t* bv,char *nenv1bv,char *nenv2bv,char *sigmabv,bool *subset)
{
  unsigned posIndex[5000],posI;
  unsigned negIndex[5000],negI;
  unsigned zeroIndex[5000],zeroI;
  unsigned i,j,k;
  numdbl_t *coeffi; /* the coefficient of varIndex in row i */
  numdbl_t *itvIneq,*intvp;
  numdbl_t a1k,a1i,a2i,a2k,a3k,b3k;
  numdbl_t c,c1,c2;
  numdbl_t *resCons,*resq,*aCons;
  unsigned ncons1;
  numdbl_t a1x2idown,a1x2iup;
  numdbl_t t,tInt;
  unsigned m;
  numitv_t b3kIntv;
  bool dealFlag;
  fpp_t *fpp1;
  int sat;
  unsigned nenvs;
  bit_vectors_t* bv1=NULL;
  char *vec;
  char *v1,*v2,*v12;

  *subset=true;
  if(fpp->dim==1){
     fpp1=fpp_alloc_internal(pr,fpp->dim,fpp->intdim); /* EMPTY_POL */
     if(destructive==true)	fpp_free_internal(pr, fpp);
     return fpp1;
  }
	/* scan all the coefficients of varIndex in each inequality to collect the sign information */
    /*        positive; negative; zero(including -0.0 and +0.0) */
  posI=0;
  negI=0;
  zeroI=0;
  for(i=0;i<fpp->ncons;i++)
  {
  	coeffi=fpp->cons+i*(fpp->dim+1)+varIndex; /*(fpp->dim+1) columns per row*/
  	if(*coeffi>0.0){
  		posIndex[posI]=i;
  		posI++;
    }
  	else if(*coeffi<-0.0){
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
		    fpp1=fpp_copy_internal(pr, fpp);
	else	fpp1=fpp;
	checked_free(fpp1->cons);
    checked_free(fpp1->bnds);
	fpp1->ncons=0;
	fpp1->flag=UNIVERSE_POL;
        /* if(bv!=NULL)  bv_free(bv); */
	return fpp1;
  }
  /* --------------------------------------------------------- */
  /* Sound Fourier Elimination with floating point arithmetic  */
  /* --------------------------------------------------------- */
  checked_malloc(pr,resCons,numdbl_t,(posI*negI+zeroI)*(fpp->dim+1), return NULL;);
  resq=resCons;
  if(bv!=NULL){
     nenvs=fpp->ncons-bv->nvecs;
     bv1=bv_copy(bv);
     bv1->nvecs=bv1->nvecs+posI*negI+zeroI;
     bv1->bits = realloc(bv1->bits,bv1->nvecs*bv1->vchars*sizeof(char));
  }
  ncons1=0;
  if(posI>0 && negI>0){
     k=0; /* the index for the new generated constraint */
     for(i=0;i<posI;i++) /* positive row: posIndex[i] */
     {
      	a1i=*(fpp->cons+posIndex[i]*(fpp->dim+1)+varIndex); /* a_1i of row posIndex[i] */
      	c1=*(fpp->cons+posIndex[i]*(fpp->dim+1)); /* 0-th(constant) of row posIndex[i] */

      	/* ----------------------------------- */
      	/* positive row + negative row       */
      	/* ----------------------------------- */
      	for(j=0;j<negI;j++) /* negative row: negIndex[j] */
      	{
      		dealFlag=false;
      		a2i=*(fpp->cons+negIndex[j]*(fpp->dim+1)+varIndex); /* a_2i of row negIndex[j] */
      		/* Non-Envelope info */
      		v1=bv->bits+posIndex[i]*bv->vchars;
      		v2=bv->bits+negIndex[j]*bv->vchars;
      		v12=bv_or(bv,v1,v2);
            if( (bv_andTest(bv,v12,nenv1bv)==false /* && bv_andTest(bv,v12,sigmabv)==false*/ )
                ||(bv_andTest(bv,v12,nenv2bv)==false /* && bv_andTest(bv,v12,sigmabv)==false*/) ){
      		       checked_free(v12);
      		}
      	    checked_free(v12);

      		   /* positive row + negative row */
            checked_malloc(pr,itvIneq,numdbl_t,2*fpp->dim+1, return NULL;);
      		intvp=itvIneq;
      		   /* see if t=a_1i/(-a_2i) is an integer, and a1i==t *_{f,+\infty} (-a_2i)==t *_{f,-\infty} (-a_2i) */
      		if(a1i == -a2i){
      			dealFlag=true;
        	 	c2=*(fpp->cons+negIndex[j]*(fpp->dim+1)); /* 0-th(constant) of row negIndex[j] */
      			c=c1+c2;
      			*intvp=c;
      			intvp++;
      			for(k=1; k<=fpp->dim; k++){ /* negative row: negIndex[j] */
      			   if(k!=varIndex){
      			       a1k=*(fpp->cons+posIndex[i]*(fpp->dim+1)+k); /* a_1k of row posIndex[i] */
      			       a2k=*(fpp->cons+negIndex[j]*(fpp->dim+1)+k); /* a_2k of row negIndex[j] */

      			       b3kIntv=numitv_add_number(a1k,a2k);
      			       memcpy(intvp,&b3kIntv,2*sizeof(numdbl_t));
      			       intvp=intvp+2;
      			  }
      			  else{
      				*intvp=0;  /* ) for V_varIndex */
      				intvp++;
      				*intvp=0;  /* ) for V_varIndex */
      				intvp++;
      			   }
              	}
      		}
      		else if(a1i > -a2i){
      		  t=a1i/(-a2i);  /* no matter which rounding mode */
      		  tInt=trunc(t);
      		  if(tInt==t){ /* t is an integer */
      		     if((t*(-a2i) == a1i) && (t*(-a2i)==num_mul_downward(t,-a2i))){/*row_posIndex[i]+t*row_negIndex[j]*/
      			    dealFlag=true;
        	 		c2=*(fpp->cons+negIndex[j]*(fpp->dim+1)); /* 0-th(constant) of row negIndex[j] */
      				c=c1+c2*t;
      				*intvp=c;
      				intvp++;
      				for(k=1; k<=fpp->dim; k++){ /* negative row: negIndex[j] */
						if(k!=varIndex){
						   a1k=*(fpp->cons+posIndex[i]*(fpp->dim+1)+k); /* a_1k of row posIndex[i] */
						   a2k=*(fpp->cons+negIndex[j]*(fpp->dim+1)+k); /* a_2k of row negIndex[j] */
						  /* b3k=a1k+t*a2k;  */
						   b3kIntv=numitv_add( numitv_singleton(a1k),numitv_mul_number(t,a2k) );
						   memcpy(intvp,&b3kIntv,2*sizeof(numdbl_t));
						   intvp=intvp+2;
					   }
					   else{
						  *intvp=0;  /* ) for V_varIndex */
						  intvp++;
						  *intvp=0;  /* ) for V_varIndex */
						  intvp++;
					   }
              	    }
      		     }
      		  }
      		}
      		else{ /* a1i < -a2i */
      		  t=(-a2i)/a1i;  /* no matter which rounding mode */
      		  tInt=trunc(t);
      		  if(tInt==t){ /* t is an integer */
      		     if((t*a1i == -a2i) && (t*a1i==num_mul_downward(t,a1i))){/* t*row_posIndex[i]+row_negIndex[j]*/
					dealFlag=true;
					c2=*(fpp->cons+negIndex[j]*(fpp->dim+1)); /* 0-th(constant) of row negIndex[j] */
					c=c1*t+c2;
					*intvp=c;
					intvp++;
					for(k=1; k<=fpp->dim; k++){ /* negative row: negIndex[j] */
					   if(k!=varIndex){
						   a1k=*(fpp->cons+posIndex[i]*(fpp->dim+1)+k); /* a_1k of row posIndex[i] */
						   a2k=*(fpp->cons+negIndex[j]*(fpp->dim+1)+k); /* a_2k of row negIndex[j] */
						  /* b3k=t*a1k+a2k;  */
						   b3kIntv=numitv_add( numitv_mul_number(t,a1k),numitv_singleton(a2k) );
						   memcpy(intvp,&b3kIntv,2*sizeof(numdbl_t));
						   intvp=intvp+2;
					  }
					  else{
						*intvp=0;  /* ) for V_varIndex */
						intvp++;
						*intvp=0;  /* ) for V_varIndex */
						intvp++;
					  }
              	    }
      		     }
      		  }
      		}
      		if(dealFlag==false){
      		   /* see if a_1i *_{f,-\infity} (-a_2i)==a_1i *_{f,+\infity} (-a_2i) */
      		   a1x2idown=num_mul_downward(a1i,-a2i);
      		   a1x2iup=a1i*(-a2i);
      		   if(a1x2iup==a1x2idown &&a1x2iup<MAX_MULCOEF && a1i<MAX_MULCOEF && -a2i<MAX_MULCOEF){/* multiplication */
        		     /* roundup now; */
      		     c2=*(fpp->cons+negIndex[j]*(fpp->dim+1)); /* 0-th(constant) of row negIndex[j] */
      		     c=c1*(-a2i)+c2*a1i;
      		     *intvp=c;
      		     intvp++;
      		     for(k=1; k<=fpp->dim; k++){ /* negative row: negIndex[j] */
      		        if(k!=varIndex){
      		           a1k=*(fpp->cons+posIndex[i]*(fpp->dim+1)+k); /* a_1k of row posIndex[i] */
					   a2k=*(fpp->cons+negIndex[j]*(fpp->dim+1)+k); /* a_2k of row negIndex[j] */
					   if( (a1k==a1i && a2k==a2i) || (a1k==-a1i && a2k==-a2i) ){ /* crossing */
						  *intvp=0;  intvp++;
						  *intvp=0;  intvp++;
					   }
					   else{  /* b3k=a1k*(-a2i)+a2k*a1i;   */
						  b3kIntv=numitv_add( numitv_mul_number(a1k,-a2i),numitv_mul_number(a2k,a1i) );
						  memcpy(intvp,&b3kIntv,2*sizeof(numdbl_t));
					      intvp=intvp+2;
					   }
        	 	    }
      		        else{
      			      *intvp=0;  intvp++;
      			      *intvp=0;  intvp++;
      		        }
      		     }

      		   }
      		   else{/* division */
      		      c2=*(fpp->cons+negIndex[j]*(fpp->dim+1)); /* 0-th(constant) of row negIndex[j] */
      		      c=c1/a1i+c2/(-a2i);
      		      *intvp=c;
      		      intvp++;
      		      for(k=1; k<=fpp->dim; k++){ /* negative row: negIndex[j] */
      		        if(k!=varIndex){
					   a1k=*(fpp->cons+posIndex[i]*(fpp->dim+1)+k); /* a_1k of row posIndex[i] */
					   a2k=*(fpp->cons+negIndex[j]*(fpp->dim+1)+k); /* a_2k of row negIndex[j] */
						 /* b3k=a1k/a1i+a2k/(-a2i);    */
					   b3kIntv=numitv_add( numitv_div_number(a1k,a1i),numitv_div_number(a2k,-a2i) );
					   memcpy(intvp,&b3kIntv,2*sizeof(numdbl_t));
      		           intvp=intvp+2;
      		        }
      		        else{
					   *intvp=0;  /* ) for V_varIndex */
					   intvp++;
					   *intvp=0;  /* ) for V_varIndex */
					   intvp++;
      		        }
      		      }
      		   }
      		}
      		aCons=quasi_linearisation(fpp->dim,itvIneq,fpp->bnds,&sat);
      		if(aCons==NULL){
      		   if(sat==0){/* unsatifiable */
      		      checked_free(itvIneq);
         		  checked_free(resCons);
                  if(bv!=NULL) bv_free(bv1);
                  fpp1=fpp_alloc_internal(pr, fpp->dim,fpp->intdim);
                  if(destructive==true)  fpp_free_internal(pr, fpp);
         		  return fpp1;
               }
            }
      		else{
      		   memcpy(resq,aCons,(fpp->dim+1)*sizeof(numdbl_t));
      		   checked_free(aCons);
      		   resq=resq+fpp->dim+1;
      		   if(bv!=NULL){
      			/* Bit Vectors */
      		      vec=bv_or(bv,bv->bits+posIndex[i]*bv->vchars,bv->bits+negIndex[j]*bv->vchars);
      		      bv_set(bv1,ncons1,vec);
      		      checked_free(vec);
      		   }
      		   ncons1++;
      		}
      		checked_free(itvIneq);
      	}
     }
  }


  /* ---------------------------------------------------------------- */
  /* copy constraints in which the coefficient for V_varIndex is zero */
  /* ---------------------------------------------------------------- */
  if(ncons1>0)   *subset=false;
  for(i=0;i<zeroI;i++) /* zero row: zeroIndex[i] */
  {
	memcpy(resq,fpp->cons+zeroIndex[i]*(fpp->dim+1),(fpp->dim+1)*sizeof(numdbl_t));
	resq=resq+fpp->dim+1;
	if(bv!=NULL){
		/* Bit Vectors */
	  bv_set(bv1,ncons1,bv->bits+zeroIndex[i]*bv->vchars);
	}
	ncons1++;
  }

  /* --------------------------------------------------------------- */
  /* post-processing before return result constraints                */
  /* --------------------------------------------------------------- */
   if(destructive==false)
	    fpp1=fpp_copy_internal(pr, fpp);
   else	fpp1=fpp;

   if(ncons1>0){
      resCons=(numdbl_t *)realloc(resCons,ncons1*(fpp->dim+1)*sizeof(numdbl_t)); /* truncate */
      if(bv!=NULL){
	   /* Bit Vectors */
         bv1->nvecs=ncons1-nenvs;
         bv1->bits = realloc(bv1->bits,bv1->nvecs*bv1->vchars*sizeof(char));
         bv->nvecs=bv1->nvecs;
         checked_free(bv->bits);
         bv->bits=bv1->bits;
         checked_free(bv1);
      }
      checked_free(fpp1->cons);
      fpp1->ncons=ncons1;
      fpp1->cons=resCons;
   }
   else{
      if(bv!=NULL) bv_free(bv1);
      checked_free(resCons);
      checked_free(fpp1->cons);
      checked_free(fpp1->bnds);
      fpp1->ncons=0;
      fpp1->flag=UNIVERSE_POL;
   }
   return fpp1;
}


/*  Redundancy removal:  2 steps         				*/
/*  	1)quasi-syntactic  						*/
/*  	2)rigorous linear programming.					*/
/*  parameter eps: epsilon redundancy removal or not.                    */
/*  parameter NbCons to store the number of the new linear constraints. */
/*  return the linear constraints after redundancy removal. 		*/
fpp_t *redundancy_removal(fpp_internal_t* pr, bool destructive,fpp_t *fpp, bool eps)
{
    fpp_t *fpp1;
    if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;
    if(fpp1->flag!=GENERAL_POL || fpp1->ncons<=1) return fpp1;
    fpp1=redundancy_removal_quasi_syntactic(pr,true,fpp1);
    if(fpp1->flag!=GENERAL_POL || fpp1->ncons<=1) return fpp1;
    fpp1=redundancy_removal_SLP_firstKs(pr,true,fpp1,fpp1->ncons,NULL,eps);
    return fpp1;
}

/*  Redundancy removal for the first Ks constriants:  2 steps		*/
/*  	1)quasi-syntactic  						*/
/*  	2)rigorous linear programming.					*/
/*  parameter eps: epsilon redundant removal or not.                    */
/*  parameter NbCons to store the number of the new linear constraints. */
/*  return the linear constraints after redundancy removal. 		*/
fpp_t *redundancy_removal_firstKs(fpp_internal_t* pr,bool destructive, fpp_t *fpp, unsigned k,bit_vectors_t *bv,bool eps)
{
    numdbl_t *resCons;
    fpp_t *fpp1;
    unsigned keepNb;

    keepNb=fpp->ncons-k;
    if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;
    if(fpp1->flag!=GENERAL_POL || fpp1->ncons<=1 ) return fpp1;
    fpp1=redundancy_removal_quasi_syntactic_firstKs(pr,true,fpp1,fpp1->ncons-keepNb,bv);
    fpp1=redundancy_removal_SLP_firstKs(pr,true,fpp1,fpp1->ncons-keepNb,bv,eps);
    return fpp1;
}


/*  Redundancy removal for the Last Ks constriants:  2 steps		*/
/*  	1)quasi-syntactic  						*/
/*  	2)rigorous linear programming.					*/
/*  parameter eps: epsilon redundant removal or not.                    */
/*  parameter NbCons to store the number of the new linear constraints. */
/*  return the linear constraints after redundancy removal. 		*/
fpp_t *redundancy_removal_lastKs(fpp_internal_t* pr,bool destructive, fpp_t *fpp,unsigned k,bool eps)
{
    numdbl_t *resCons;
    fpp_t *fpp1;
    unsigned keepNb;

    keepNb=fpp->ncons-k;
    if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;
    if(fpp1->flag!=GENERAL_POL || fpp1->ncons<=1 ) return fpp1;

    fpp1=redundancy_removal_quasi_syntactic_lastKs(pr,true,fpp1,fpp1->ncons-keepNb);
    fpp1=redundancy_removal_SLP_lastKs(pr,true,fpp1,fpp1->ncons-keepNb,eps);

    return fpp1;
}

/*  Redundancy removal by rigorous linear programming:           */
/*  	for each inequality l: e<=c in P, posing mu=max(e,P)     */
/*  	if mu<c then remove l from P.				 */
/*  parameter eps: epsilon redundant removal or not.                    */
/*  parameter NbCons to store the number of the new linear constraints. */
/*  return the linear constraints after redundancy removal. */
fpp_t *redundancy_removal_RLP(fpp_internal_t* pr,bool destructive, fpp_t *fpp,bool eps)
{
    unsigned i,j;
    numdbl_t *objVector;
    numdbl_t c,ci;
    glp_prob *lp;
    fpp_t *fpp1;
    numdbl_t *resCons;
    int lpres;
    numdbl_t cic,maxci;
    bool nearly;

    if(destructive==false)
		    fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;
    if(fpp1->flag!=GENERAL_POL || fpp1->ncons<=1 ) return fpp1;
    i=0;
	while(i<fpp1->ncons){
		if(fpp1->ncons==1){ /* only one linear constraint */
		   for(j=0;j<fpp1->dim;j++){
			if(*(fpp1->cons+j+1)!=0){ /* some coefficients are not zero, so non-redundant */
				return fpp1;
			}
		   }
		   /* all coefficients are zero */
		   if(*(fpp1->cons)>=0){ /* whole space: universe polyhedron */
			fpp1->ncons=0;
			fpp1->flag=UNIVERSE_POL;
			checked_free(fpp1->cons);
			checked_free(fpp1->bnds);
			return fpp1;
		   }
		   else{ /* infeasible: empty polyhedron */
			fpp1->ncons=0;
			fpp1->flag=EMPTY_POL;
			checked_free(fpp1->cons);
			checked_free(fpp1->bnds);
			return fpp1;
		   }
		}
		c=*(fpp1->cons+i*(fpp1->dim+1));
		checked_malloc(pr,objVector,numdbl_t,fpp1->dim, return NULL;);
		memcpy(objVector,fpp1->cons+i*(fpp1->dim+1)+1,fpp1->dim*sizeof(numdbl_t));
		for(j=0;j<fpp1->dim;j++){
			if(*(objVector+j)>0.0 || *(objVector+j)<-0.0){ /* some coefficients are non-zero,so need RLP */
				break;
			}
		}
		if(j==fpp1->dim){ /* all coefficients are zero */
			if(c>=-0.0){ /* the i-th constraint is redundant */
				checked_free(objVector);
				if(fpp1->ncons-i-1>0)
				memmove(fpp1->cons+i*(fpp1->dim+1),fpp1->cons+(i+1)*(fpp1->dim+1),(fpp1->ncons-i-1)*(fpp1->dim+1)*sizeof(numdbl_t));
				else  fpp1->cons=(numdbl_t *)realloc(fpp1->cons,(fpp1->ncons-1)*(fpp1->dim+1)*sizeof(numdbl_t));
				fpp1->ncons=fpp1->ncons-1;
			}
			else{ /* infeasible: empty polyhedron */
				checked_free(objVector);
				fpp1->ncons=0;
				fpp1->flag=EMPTY_POL;
						checked_free(fpp1->cons);
				checked_free(fpp1->bnds);
				return fpp1;
			}
		}
		else{
			lp= rlp_create_without_KthCons(fpp1,GLP_MAX,objVector,i+1);
			lp=rlp_solve(lp);
			if(lp==NULL){
				checked_free(objVector);
				i++;
			}
			else{
				lpres=glp_get_status(lp);
				if((lpres!=GLP_OPT) && ((lpres!=GLP_FEAS))){	/* solution is neither optimal nor feasible*/
					if(lpres==GLP_NOFEAS){   /* problem has no feasible solution */
						fprintf(stdout, "\n Dual problem has no feasible solution, thus no rigorous linear programming for primal problem!");
						/* The primal problem may be either infeasible or unbounded, so we keep this constraint. */
						fprintf(stdout, "\n%d is not removed because the dual problem has no feasible solution \n",i);
					}
					else{
						fprintf(stdout, "\n Dual problem: glp_get_status()=%d \n",lpres);
						/* The primal problem may be either infeasible or unbounded, so we keep this constraint. */
						fprintf(stdout, "\n%d is not removed because the dual problem has no feasible solution \n",i);
					}
					lp_delete(lp);
					checked_free(objVector);
					i++;
				}
				else{
					ci=rlp_get_optimal_value(lp,fpp1->bnds,false,&nearly);
					lp_delete(lp);
					checked_free(objVector);
					if(eps==false){
						if(ci<=c){  /* the i-th linear constraint is redundant */
							/* remove i-th linear constraint */
							if(fpp1->ncons-i-1>0)
								memmove(fpp1->cons+i*(fpp1->dim+1),fpp1->cons+(i+1)*(fpp1->dim+1),(fpp1->ncons-i-1)*(fpp1->dim+1)*sizeof(numdbl_t));
							else  fpp1->cons=(numdbl_t *)realloc(fpp1->cons,(fpp1->ncons-1)*(fpp1->dim+1)*sizeof(numdbl_t));
							fpp1->ncons=fpp1->ncons-1;
						}
						else{/* the i-th linear constraint is non-redundant */
							i++;
						}
					}
					else{
						if(ci<=c){  /* the i-th linear constraint is redundant */
							/* remove i-th linear constraint */
							if(fpp1->ncons-i-1>0)
								  memmove(fpp1->cons+i*(fpp1->dim+1),fpp1->cons+(i+1)*(fpp1->dim+1),(fpp1->ncons-i-1)*(fpp1->dim+1)*sizeof(numdbl_t));
							else  fpp1->cons=(numdbl_t *)realloc(fpp1->cons,(fpp1->ncons-1)*(fpp1->dim+1)*sizeof(numdbl_t));
							fpp1->ncons=fpp1->ncons-1;
						}
						else{ /* ci > c*/
						   cic=	ci-c;
						   maxci=fmax(-c,ci);
						   if( cic/maxci <= EPSILON){ /*the i-th linear constraint is redundant*/
							/* remove i-th linear constraint */
							if(fpp1->ncons-i-1>0)
								  memmove(fpp1->cons+i*(fpp1->dim+1),fpp1->cons+(i+1)*(fpp1->dim+1),(fpp1->ncons-i-1)*(fpp1->dim+1)*sizeof(numdbl_t));
							else  fpp1->cons=(numdbl_t *)realloc(fpp1->cons,(fpp1->ncons-1)*(fpp1->dim+1)*sizeof(numdbl_t));
							fpp1->ncons=fpp1->ncons-1;
						   }
						   else{/* the i-th linear constraint is non-redundant */
							i++;
						   }
						}
					}
				}
			}
		}
    }
    return fpp1;
}


/*  Redundancy removal for the first ks inequalities by rigorous linear programming: */
/*  parameter eps: epsilon redundant removal or not.                                 */
/*  return the linear constraints after redundancy removal.                          */
fpp_t *redundancy_removal_RLP_firstKs(fpp_internal_t* pr,bool destructive, fpp_t *fpp,unsigned k,bool eps)
{
    unsigned i,j,t;
    numdbl_t *objVector;
    numdbl_t c,ci;
    glp_prob *lp;
    fpp_t *fpp1;
    numdbl_t *resCons;
    int lpres;
    numdbl_t cic,maxci;
    bool nearly;

    if(destructive==false)
		  fpp1=fpp_copy_internal(pr, fpp);
    else  fpp1=fpp;
    if(fpp1->flag!=GENERAL_POL || fpp1->ncons<=1 ) return fpp1;
    i=0;
    t=0;
	while(i<fpp1->ncons){
		if(t>=k)         break;      /* Redundancy removal for the first ks inequalities */
		if(fpp1->ncons==1){ /* only one linear constraint */
		   for(j=0;j<fpp1->dim;j++){
			if(*(fpp1->cons+j+1)>0.0 || *(fpp1->cons+j+1)<-0.0){
				/* some coefficients are not zero, so non-redundant */
				return fpp1;
			}
		   }
		   /* all coefficients are zero */
		   if(*(fpp1->cons)>=-0.0){ /* whole space: universe polyhedron */
			fpp1->ncons=0;
			fpp1->flag=UNIVERSE_POL;
					checked_free(fpp1->cons);
			checked_free(fpp1->bnds);
			return fpp1;
		   }
		   else{ /* infeasible: empty polyhedron */
			fpp1->ncons=0;
			fpp1->flag=EMPTY_POL;
					checked_free(fpp1->cons);
			checked_free(fpp1->bnds);
			return fpp1;
		   }
		}
		checked_malloc(pr,objVector,numdbl_t,fpp1->dim, return NULL;);
		memcpy(objVector,fpp1->cons+i*(fpp1->dim+1)+1,fpp1->dim*sizeof(numdbl_t));
		lp= rlp_create_without_KthCons(fpp1,GLP_MAX,objVector,i+1);
		lp=rlp_solve(lp);
		if(lp==NULL){
			checked_free(objVector);
			i++;
		}
		else{
		  lpres=glp_get_status(lp);
		  if((lpres!=GLP_OPT) && ((lpres!=GLP_FEAS))){	/* solution is neither optimal nor feasible*/
			if(lpres==GLP_NOFEAS){   /* problem has no feasible solution */
			fprintf(stdout, "\n Dual problem has no feasible solution, thus no rigorous linear programming for primal problem!");
			/* The primal problem may be either infeasible or unbounded, so we keep this constraint. */
			fprintf(stdout, "\n%d is not removed because the dual problem has no feasible solution \n",i);
		  }
		  else{
			fprintf(stdout, "\n Dual problem: glp_get_status()=%d \n",lpres);
			/* The primal problem may be either infeasible or unbounded, so we keep this constraint. */
			fprintf(stdout, "\n%d is not removed because the dual problem has no feasible solution \n",i);
		  }
		  lp_delete(lp);
		  checked_free(objVector);
		  i++;
		}
		else{
			ci=rlp_get_optimal_value(lp,fpp1->bnds,false,&nearly);
			lp_delete(lp);
			checked_free(objVector);
			c=*(fpp1->cons+i*(fpp1->dim+1));
			if(eps==false){
				if(ci<=c){  /* the i-th linear constraint is redundant */
					/* remove i-th linear constraint */
					if(fpp1->ncons-i-1>0)
						  memmove(fpp1->cons+i*(fpp1->dim+1),fpp1->cons+(i+1)*(fpp1->dim+1),(fpp1->ncons-i-1)*(fpp1->dim+1)*sizeof(numdbl_t));
					else  fpp1->cons=(numdbl_t *)realloc(fpp1->cons,(fpp1->ncons-1)*(fpp1->dim+1)*sizeof(numdbl_t));
					fpp1->ncons=fpp1->ncons-1;
				}
				else{/* the i-th linear constraint is non-redundant */
					i++;
				}
			}
			else{
				if(ci<=c){  /* the i-th linear constraint is redundant */
					/* remove i-th linear constraint */
					if(fpp1->ncons-i-1>0)
						  memmove(fpp1->cons+i*(fpp1->dim+1),fpp1->cons+(i+1)*(fpp1->dim+1),(fpp1->ncons-i-1)*(fpp1->dim+1)*sizeof(numdbl_t));
					else  fpp1->cons=(numdbl_t *)realloc(fpp1->cons,(fpp1->ncons-1)*(fpp1->dim+1)*sizeof(numdbl_t));
					fpp1->ncons=fpp1->ncons-1;
				}
				else{ /* ci > c*/
				   cic=ci-c;
				   maxci=fmax(-c,ci);
				   if( cic/maxci <= EPSILON){ /*the i-th linear constraint is redundant*/
					/* remove i-th linear constraint */
					if(fpp1->ncons-i-1>0)
						 memmove(fpp1->cons+i*(fpp1->dim+1),fpp1->cons+(i+1)*(fpp1->dim+1),(fpp1->ncons-i-1)*(fpp1->dim+1)*sizeof(numdbl_t));
					else  fpp1->cons=(numdbl_t *)realloc(fpp1->cons,(fpp1->ncons-1)*(fpp1->dim+1)*sizeof(numdbl_t));
					fpp1->ncons=fpp1->ncons-1;
				   }
				   else{/* the i-th linear constraint is non-redundant */
					i++;
				   }
				}
			}
		}
	  }
      t++;
    }
    return fpp1;
}

/*  Redundancy removal for the last ks inequalities by rigorous linear programming: */
/*  parameter eps: epsilon redundant removal or not.                                 */
/*  return the linear constraints after redundancy removal.                          */
fpp_t *redundancy_removal_RLP_lastKs(fpp_internal_t* pr,bool destructive, fpp_t *fpp,unsigned k,bool eps)
{
    unsigned i,j;
    numdbl_t *objVector;
    numdbl_t c,ci;
    glp_prob *lp;
    fpp_t *fpp1;
    numdbl_t *resCons;
    int lpres;
    numdbl_t cic,maxci;
    bool nearly;

    if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;
    if(fpp1->flag!=GENERAL_POL || fpp1->ncons<=1 ) return fpp1;
    i=fpp1->ncons-k; /* Redundancy removal for the last ks inequalities */
    while(i<fpp1->ncons){
	 if(fpp1->ncons==1){ /* only one linear constraint */
	   for(j=0;j<fpp1->dim;j++){
		if(*(fpp1->cons+j+1)>0.0 || *(fpp1->cons+j+1)<-0.0){
			/* some coefficients are not zero, so non-redundant */
		    return fpp1;
		}
	   }
	   /* all coefficients are zero */
	   if(*(fpp1->cons)>=-0.0){ /* whole space: universe polyhedron */
		fpp1->ncons=0;
		fpp1->flag=UNIVERSE_POL;
                checked_free(fpp1->cons);
		checked_free(fpp1->bnds);
		return fpp1;
	   }
	   else{ /* infeasible: empty polyhedron */
		fpp1->ncons=0;
		fpp1->flag=EMPTY_POL;
                checked_free(fpp1->cons);
		checked_free(fpp1->bnds);
		return fpp1;
	   }
	}
    checked_malloc(pr,objVector,numdbl_t,fpp1->dim, return NULL;);
	memcpy(objVector,fpp1->cons+i*(fpp1->dim+1)+1,fpp1->dim*sizeof(numdbl_t));
    lp= rlp_create_without_KthCons(fpp1,GLP_MAX,objVector,i+1);
    lp=rlp_solve(lp);
    if(lp==NULL){
        checked_free(objVector);
	    i++;
    }
  	else{
  	  lpres=glp_get_status(lp);
  	  if((lpres!=GLP_OPT) && ((lpres!=GLP_FEAS))){	/* solution is neither optimal nor feasible*/
	    if(lpres==GLP_NOFEAS){   /* problem has no feasible solution */
			fprintf(stdout, "\n Dual problem has no feasible solution, thus no rigorous linear programming for primal problem!");
			/* The primal problem may be either infeasible or unbounded, so we keep this constraint. */
			fprintf(stdout, "\n%d is not removed because the dual problem has no feasible solution \n",i);
	    }
	    else{
			fprintf(stdout, "\n Dual problem: glp_get_status()=%d \n",lpres);
			/* The primal problem may be either infeasible or unbounded, so we keep this constraint. */
			fprintf(stdout, "\n%d is not removed because the dual problem has no feasible solution \n",i);
	    }
	    lp_delete(lp);
	    checked_free(objVector);
	    i++;
	  }
	  else{
	    ci=rlp_get_optimal_value(lp,fpp1->bnds,false,&nearly);
	    lp_delete(lp);
	    checked_free(objVector);
	    c=*(fpp1->cons+i*(fpp1->dim+1));
	    if(eps==false){
			if(ci<=c){  /* the i-th linear constraint is redundant */
				/* remove i-th linear constraint */
				if(fpp1->ncons-i-1>0)
					  memmove(fpp1->cons+i*(fpp1->dim+1),fpp1->cons+(i+1)*(fpp1->dim+1),(fpp1->ncons-i-1)*(fpp1->dim+1)*sizeof(numdbl_t));
				else  fpp1->cons=(numdbl_t *)realloc(fpp1->cons,(fpp1->ncons-1)*(fpp1->dim+1)*sizeof(numdbl_t));
				fpp1->ncons=fpp1->ncons-1;
			}
			else{/* the i-th linear constraint is non-redundant */
				i++;
			}
	    }
	    else{
		 if(ci<=c){  /* the i-th linear constraint is redundant */
			/* remove i-th linear constraint */
			if(fpp1->ncons-i-1>0)
			     memmove(fpp1->cons+i*(fpp1->dim+1),fpp1->cons+(i+1)*(fpp1->dim+1),(fpp1->ncons-i-1)*(fpp1->dim+1)*sizeof(numdbl_t));
			else fpp1->cons=(numdbl_t *)realloc(fpp1->cons,(fpp1->ncons-1)*(fpp1->dim+1)*sizeof(numdbl_t));
			fpp1->ncons=fpp1->ncons-1;
		 }
		 else{ /* ci > c*/
		   cic=ci-c;
		   maxci=fmax(-c,ci);
		   if( cic/maxci <= EPSILON){ /*the i-th linear constraint is redundant*/
			/* remove i-th linear constraint */
			if(fpp1->ncons-i-1>0)
			     memmove(fpp1->cons+i*(fpp1->dim+1),fpp1->cons+(i+1)*(fpp1->dim+1),(fpp1->ncons-i-1)*(fpp1->dim+1)*sizeof(numdbl_t));
			else fpp1->cons=(numdbl_t *)realloc(fpp1->cons,(fpp1->ncons-1)*(fpp1->dim+1)*sizeof(numdbl_t));
			fpp1->ncons=fpp1->ncons-1;
		   }
		   else{/* the i-th linear constraint is non-redundant */
			i++;
		   }
		 }
	   }
	  }
     }
    }
    return fpp1;
}

/*  Quasi-Syntactic Redundancy Removal:                                 */
/*  	for each inequality l_i: e_i<=c_i in P,                         */
/*  	check if there exists l_j: e_j<=c_j where j>i s.t. e_i=e_j 	*/
/*  	if c_i<=c_j then l_i is redundant otherwise l_j is redundant 	*/
/*  parameter NbCons to store the number of the new linear constraints. */
/*  return the linear constraints after redundancy removal. */
fpp_t *redundancy_removal_quasi_syntactic(fpp_internal_t* pr,bool destructive, fpp_t *fpp)
{
    char *rrIndex;
    int i,j,m;
    fpp_t *fpp1;
    numdbl_t *cons1;
    numdbl_t *li,*lj;
    numdbl_t lim,ljm,dm,maxm;
    char *bits1=NULL;
    int n,k;

    if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;
    if(fpp1->flag!=GENERAL_POL || fpp1->ncons<=1 ) return fpp1;
    k=fpp1->ncons;
    checked_malloc(pr,rrIndex,char,k, return NULL;);
    memset(rrIndex,0,k*sizeof(char));
    i=0;
	while(i<k-1){
		if(rrIndex[i]==true){
			i++;
			continue;
		}
		li=fpp1->cons+i*(fpp1->dim+1);
			/* for trivial inequalities with all coefficients are 0 */
		for(j=1;j<=(int)fpp1->dim;j++){
			if(*(li+j)>0.0 || *(li+j)<-0.0){ /*some coefficients are not zero,so need LP */
				break;
			}
		}
		if(j>(int)fpp1->dim){ /* all coefficients are zero */
			if(*li>=-0.0){ /* the i-th constraint is redundant */
				rrIndex[i]=true;
				i++;
				continue;
			}
			else{ /* infeasible: empty polyhedron */
				fpp1->ncons=0;
				fpp1->flag=EMPTY_POL;
				checked_free(fpp1->cons);
				checked_free(fpp1->bnds);
				checked_free(rrIndex);
				return fpp1;
			}
		}
			/* quasi-syntactic */
		for(j=i+1;j<(int)fpp1->ncons;j++){
		   if(rrIndex[j]==true)		continue;
		   lj=fpp1->cons+j*(fpp1->dim+1);
		   for(m=1;m<=(int)fpp1->dim;m++){
			lim=*(li+m);
			ljm=*(lj+m);
			if(lim==ljm) continue;
			if( (lim>0.0 && ljm<-0.0) || (lim<-0.0 && ljm>0.0) )
				break;
			  /* with the same sign */
			if(lim >= ljm)   dm=lim-ljm;
			else	         dm=ljm-lim;
			maxm=fmax(fabs(lim),fabs(ljm));
			if( maxm==0.0 ){
			   if( dm>EPSILON ) break;
			}
			else if(dm/maxm >EPSILON ) break;
		   }
		   if(m> (int)fpp1->dim){ /* ei == ej*/
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
    checked_malloc(pr,cons1,numdbl_t,fpp1->ncons*(fpp1->dim+1), return NULL;);
    n=0;
    for(i=0;i<k;i++){
		if(rrIndex[i]==true){
		   continue;
		}
		   /* i-th constraint is non-redundant */
		memcpy(cons1+n*(fpp1->dim+1),fpp1->cons+i*(fpp1->dim+1),(fpp1->dim+1)*sizeof(numdbl_t));
		n++;
    }
    cons1=realloc(cons1,n*(fpp1->dim+1)*sizeof(numdbl_t));
    checked_free(fpp1->cons);
    checked_free(rrIndex);
    fpp1->cons=cons1;
    fpp1->ncons=n;
    return fpp1;
}

/*  Quasi-Syntactic Redundancy Removal for the first ks constraints:    */
/*  	for each inequality l_i: e_i<=c_i in P,                         */
/*  	check if there exists l_j: e_j<=c_j where j>i s.t. e_i=e_j    	*/
/*  	if c_i<=c_j then l_i is redundant otherwise l_j is redundant 	*/
/*  parameter NbCons to store the number of the new linear constraints. */
/*  return the linear constraints after redundancy removal.             */
fpp_t *redundancy_removal_quasi_syntactic_firstKs(fpp_internal_t* pr,bool destructive, fpp_t *fpp,unsigned k,bit_vectors_t *bv)
{
    char *rrIndex;
    unsigned i,j,m;
    fpp_t *fpp1;
    numdbl_t *resCons,*cons1;
    numdbl_t *li,*lj;
    numdbl_t lim,ljm,dm,maxm;
    unsigned keepNb;
    char *bits1=NULL;
    int n;

    if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;
    if(fpp1->flag!=GENERAL_POL || fpp1->ncons<=1 ) return fpp1;
    keepNb=fpp1->ncons-k;
    checked_malloc(pr,rrIndex,char,k, return NULL;);
    memset(rrIndex,0,k*sizeof(char));
    i=0;
	while(i<k){
		if(rrIndex[i]==true){
			i++;
			continue;
		}
		li=fpp1->cons+i*(fpp1->dim+1);
			/* for trivial inequalities with all coefficients are 0 */
		for(j=1;j<=fpp1->dim;j++){
			if(*(li+j)>0.0 || *(li+j)<-0.0){ /*some coefficients are not zero,so need LP */
				break;
			}
		}
		if(j>fpp1->dim){ /* all coefficients are zero */
			if(*li>=-0.0){ /* the i-th constraint is redundant */
				rrIndex[i]=true;
				i++;
				continue;
			}
			else{ /* infeasible: empty polyhedron */
				fpp1->ncons=0;
				fpp1->flag=EMPTY_POL;
				checked_free(fpp1->cons);
				checked_free(fpp1->bnds);
				checked_free(rrIndex);
				return fpp1;
			}
		}
			/* quasi-syntactic */
		for(j=i+1;j<fpp1->ncons;j++){
		   if(rrIndex[j]==true)		continue;
		   lj=fpp1->cons+j*(fpp1->dim+1);
		   for(m=1;m<=fpp1->dim;m++){
			lim=*(li+m);
			ljm=*(lj+m);
			if(lim==ljm) continue;
			if( (lim>0.0 && ljm<-0.0) || (lim<-0.0 && ljm>0.0) )
				break;
			  /* with the same sign */
			if(lim >= ljm)   dm=lim-ljm;
			else	         dm=ljm-lim;
			maxm=fmax(fabs(lim),fabs(ljm));
			if( maxm==0.0 ){
			   if( dm>EPSILON ) break;
			}
			else if(dm/maxm >EPSILON ) break;
		   }
		   if(m> fpp1->dim){ /* ei == ej*/
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
    checked_malloc(pr,cons1,numdbl_t,fpp1->ncons*(fpp1->dim+1), return NULL;);
    if(bv!=NULL) checked_malloc(pr,bits1,char,bv->nvecs*bv->vchars, return NULL;);
    n=0;
    for(i=0;i<k;i++){
		if(rrIndex[i]==true){
		   continue;
		}
		   /* i-th constraint is non-redundant */
		memcpy(cons1+n*(fpp1->dim+1),fpp1->cons+i*(fpp1->dim+1),(fpp1->dim+1)*sizeof(numdbl_t));
		if(bv!=NULL) memcpy(bits1+n*(bv->vchars),bv->bits+i*(bv->vchars),bv->vchars*sizeof(char));
		n++;
    }
    if(keepNb>0){
    	memcpy(cons1+n*(fpp1->dim+1),fpp1->cons+k*(fpp1->dim+1),keepNb*(fpp1->dim+1)*sizeof(numdbl_t));
    }
    cons1=realloc(cons1,(n+keepNb)*(fpp1->dim+1)*sizeof(numdbl_t));
    checked_free(fpp1->cons);
    fpp1->cons=cons1;
    fpp1->ncons=n+keepNb;
    checked_free(rrIndex);
    if(bv!=NULL) {
    	bits1=realloc(bits1,n*(bv->vchars)*sizeof(char));
    	checked_free(bv->bits);
    	bv->bits=bits1;
    	bv->nvecs=n;
    }
    return fpp1;
}

/*  Quasi-Syntactic Redundancy Removal for the last ks constraints:    */
/*  	for each inequality l_i: e_i<=c_i in P,                         */
/*  	check if there exists l_j: e_j<=c_j where j>i s.t. e_i=e_j 	*/
/*  	if c_i<=c_j then l_i is redundant otherwise l_j is redundant 	*/
/*  parameter NbCons to store the number of the new linear constraints. */
/*  return the linear constraints after redundancy removal. */
fpp_t *redundancy_removal_quasi_syntactic_lastKs(fpp_internal_t* pr,bool destructive, fpp_t *fpp, unsigned k)
{
    unsigned i,j,m;
    fpp_t *fpp1;
    numdbl_t *resCons;
    numdbl_t *li,*lj;
    numdbl_t lim,ljm;
    int liRed;

    if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;
    if(fpp1->flag!=GENERAL_POL || fpp1->ncons<=1 ) return fpp1;
    i=fpp1->ncons-k;  /* Redundancy removal for the last ks inequalities */
    while(i<fpp1->ncons){
	li=fpp1->cons+i*(fpp1->dim+1);
	liRed=0;
	for(j=0;j<fpp1->ncons;j++){
	   if(j==i) continue;
	   lj=fpp1->cons+j*(fpp1->dim+1);
	   for(m=1;m<=fpp1->dim;m++){
		lim=*(li+m);
		ljm=*(lj+m);
		if(lim!=ljm){
		   if(!( lim==0 && ljm ==0) ){
			break;
		   }
		}
	   }
	   if(m> fpp1->dim){ /* ei == ej*/
			/* li is redundant */
		if(fpp1->ncons-i-1>0)
		   memmove(fpp1->cons+i*(fpp1->dim+1),fpp1->cons+(i+1)*(fpp1->dim+1),(fpp1->ncons-i-1)*(fpp1->dim+1)*sizeof(numdbl_t));
		else fpp1->cons=(numdbl_t *) realloc(fpp1->cons, (fpp1->ncons-1)*(fpp1->dim+1)*sizeof(numdbl_t));
		fpp1->ncons=fpp1->ncons-1;
		liRed=1;
		break;
	   }
	}
	if(liRed!=1)   i++;
    }
    return fpp1;
}

/*  Redundancy removal by standard linear programming:           */
/*  	for each inequality l: e<=c in P, posing mu=max(e,P)     */
/*  	if mu<c then remove l from P.				 */
/*  parameter eps: epsilon redundant removal or not.                       */
/*  parameter newNbCons to store the number of the new linear constraints. */
/*  return the linear constraints after redundancy removal. */
fpp_t *redundancy_removal_SLP(fpp_internal_t* pr,bool destructive, fpp_t *fpp,bool eps)
{
    int rowStat[1000],colStat[100];
    int ret;
    bool store;
    int rows,cols;
    unsigned i,j;
    numdbl_t *objVector;
    numdbl_t c,ci,cInt;
    glp_prob *lp;
    fpp_t *fpp1;
    numdbl_t *resCons;
    int lpres;
    numdbl_t cic,maxci;
    bool rrFlag;
    numdbl_t epsValue;
    glp_smcp parm;

    if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;
    if(fpp1->flag!=GENERAL_POL || fpp1->ncons<=1 ) return fpp1;
    i=0;
#if INTPROGRAM
    epsValue=EPSILONINT;
#else
    epsValue=EPSILON;
#endif
    lp= slp_create_matrix(fpp1);
    glp_init_smcp(&parm);
    parm.tol_bnd = 1.5e-5;
    parm.tol_dj = 1.5e-5;
    parm.tol_piv = 1e-8;
    parm.tm_lim=50; /*parm.tm_lim=50;*/
    parm.meth = GLP_DUALP;
    store=false;
    checked_malloc(pr,objVector,numdbl_t,fpp1->dim, return NULL;);
	while(i<fpp1->ncons){
		if(fpp1->ncons<=2){ /* only 2 linear constraint */
			checked_free(objVector);
			lp_delete(lp);
			return fpp1;
		}
		c=*(fpp1->cons+i*(fpp1->dim+1));
		memcpy(objVector,fpp1->cons+i*(fpp1->dim+1)+1,fpp1->dim*sizeof(numdbl_t));
		slp_set_objective(lp,GLP_MAX,objVector);
		slp_disable_kthRow(lp,i+1,c);

		lp=slp_solve(lp, &parm);
		if(lp!=NULL){
			rrFlag=false;
			lpres=glp_get_status(lp);
			if( lpres==GLP_OPT ){
				ci=slp_get_optimal_value(lp);
				if(ci<=c){  /* the i-th linear constraint is redundant */
					rrFlag=true;
				}
				else{  /* ci > c*/
						if(eps==true){
						cic=ci-c;
						maxci=fabs(c); /*maxci=fmax(-c,ci);*/
						if( (maxci==0.0 && cic<epsValue) || (maxci>0.0 && cic/maxci<epsValue)){
						   rrFlag=true;
						}
						else{/* the i-th linear constraint is non-redundant */
						}
					}
				}
			}
			if(rrFlag==true){  /* remove i-th linear constraint */
			  if(fpp1->ncons-i-1>0)
			        memmove(fpp1->cons+i*(fpp1->dim+1),fpp1->cons+(i+1)*(fpp1->dim+1),(fpp1->ncons-i-1)*(fpp1->dim+1)*sizeof(numdbl_t));
			  else  fpp1->cons=(numdbl_t *)realloc(fpp1->cons,(fpp1->ncons-1)*(fpp1->dim+1)*sizeof(numdbl_t));
			  fpp1->ncons=fpp1->ncons-1;
			  slp_del_kthRow(lp,i+1);
#if LP_BASIS_RESTORE
					/* store current basis */
				rows=glp_get_num_rows(lp);
				cols=glp_get_num_cols(lp);
				for(j=1;j<=rows;j++){
				   rowStat[j]=glp_get_row_stat(lp,j);
				}
				for(j=1;j<=cols;j++){
				   colStat[j]=glp_get_col_stat(lp,j);
				}
				store=true;
#endif
			}
			else{
			  slp_set_upbnd_kthRow(lp,i+1,c); /* restore */
#if LP_BASIS_RESTORE
		      if(store==true){
					/* restore previous basis */
				for(j=1;j<=rows;j++){
				   glp_set_row_stat(lp,j,rowStat[j]);
				}
				for(j=1;j<=cols;j++){
				   glp_set_col_stat(lp,j,colStat[j]);
				}
				ret=lpx_warm_up(lp);
				if (ret==LPX_E_BADB) /* the basis is invalid; build some valid basis */
						   lpx_adv_basis(lp);
			 }
#endif
			 i++;
			}
		}
		else{
			lp= slp_create_matrix(fpp1);
			store=false;
			i++;
		}
    }
    checked_free(objVector);
    lp_delete(lp);
    return fpp1;
}



/*  Redundancy removal the first ks constraints by standard linear programming:   */
/*  	for each inequality l: e<=c in P, posing mu=max(e,P)               */
/*  	if mu<c then remove l from P.			                           */
/*  parameter eps: epsilon redundant removal or not.                       */
/*  parameter newNbCons to store the number of the new linear constraints. */
/*  return the linear constraints after redundancy removal. */
fpp_t *redundancy_removal_SLP_firstKs(fpp_internal_t* pr,bool destructive, fpp_t *fpp,unsigned k,bit_vectors_t *bv, bool eps)
{
    int rowStat[1000],colStat[100];
    int ret;
    bool store;
    unsigned i,j;
    numdbl_t *objVector;
    numdbl_t c,ci;
    glp_prob *lp;
    fpp_t *fpp1;
    fpp_t *fpp01,*fpp02;
    numdbl_t *resCons;
    int lpres;
    unsigned t;
    numdbl_t cic,maxci;
    bool flag;
    int rows,cols;
    numdbl_t epsValue;
    glp_smcp parm;
    int ncons01;
    bit_vectors_t *bv01, *bv02;
    int k2;

    if(fpp->ncons > FM_SPLIT_SIZE){
        ncons01=(int)(fpp->ncons/2);
        fpp01=fpp_alloc_urgent(pr,fpp->dim,fpp->intdim,ncons01);
        memcpy(fpp01->cons,fpp->cons, fpp01->ncons*(fpp01->dim+1)*sizeof(numdbl_t) );
	    memcpy(fpp01->bnds,fpp->bnds, 2*fpp01->dim*sizeof(numdbl_t) );
        bv01=NULL;
        bv02=NULL;
			if(bv!=NULL){
			bv01 = (bit_vectors_t *)malloc(sizeof(bit_vectors_t));
			bv01->vsize = bv->vsize;
			bv01->vchars= bv->vchars;
			bv01->nvecs= ncons01;
			bv01->bits = (char *)malloc(bv01->nvecs*bv01->vchars*sizeof(char));
			memcpy(bv01->bits,bv->bits,bv01->nvecs*bv01->vchars*sizeof(char));
        }
        fpp02=fpp_alloc_urgent(pr,fpp->dim,fpp->intdim,fpp->ncons-ncons01);
        memcpy(fpp02->cons,fpp->cons+fpp01->ncons*(fpp01->dim+1), (fpp->ncons-fpp01->ncons)*(fpp01->dim+1)*sizeof(numdbl_t) );
		memcpy(fpp02->bnds,fpp->bnds, 2*fpp02->dim*sizeof(numdbl_t) );
		if(bv!=NULL){
			bv02 = (bit_vectors_t *)malloc(sizeof(bit_vectors_t));
			bv02->vsize = bv->vsize;
			bv02->vchars= bv->vchars;
			bv02->nvecs= bv->nvecs-bv01->nvecs;
			bv02->bits = (char *)malloc(bv02->nvecs*bv02->vchars*sizeof(char));
			memcpy(bv02->bits,bv->bits+bv01->nvecs*bv01->vchars,bv02->nvecs*bv02->vchars*sizeof(char));
		}
		if(fpp01->ncons > k){
           fpp01=redundancy_removal_SLP_firstKs(pr,true,fpp01,k,bv01,eps);
        }
        else{
	       k2=k-fpp01->ncons;
           fpp01=redundancy_removal_SLP_firstKs(pr,true,fpp01,fpp01->ncons, bv01,eps);
           fpp02=redundancy_removal_SLP_firstKs(pr,true,fpp02,k2,bv02,eps);
        }
		k=k-(fpp->ncons-fpp01->ncons-fpp02->ncons);
		fpp1=fpp_copy_internal(pr, fpp01);
		fpp1->ncons=fpp01->ncons+fpp02->ncons;
		fpp1->cons=(numdbl_t *)realloc(fpp1->cons, fpp1->ncons*(fpp1->dim+1)*sizeof(numdbl_t));
		memcpy(fpp1->cons+fpp01->ncons*(fpp1->dim+1),fpp02->cons, fpp02->ncons*(fpp1->dim+1)*sizeof(numdbl_t) );
		if(bv!=NULL){
			bv->nvecs= bv01->nvecs+bv02->nvecs;
			bv->bits = (char *)realloc(bv->bits,bv->nvecs*bv->vchars*sizeof(char));
			memcpy(bv->bits,bv01->bits,bv01->nvecs*bv01->vchars*sizeof(char));
			memcpy(bv->bits+bv01->nvecs*bv01->vchars,bv02->bits,bv02->nvecs*bv02->vchars*sizeof(char));
			bv_free(bv01);
		}
        bv_free(bv02);
	    fpp_free_internal(pr, fpp01);
	    fpp_free_internal(pr, fpp02);
        if(destructive==true)
		   fpp_free_internal(pr, fpp);
    }
    else{
        if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
        else	fpp1=fpp;
    }
    if(fpp1->flag!=GENERAL_POL || fpp1->ncons<=1 ) return fpp1;

    i=0;
    t=0;
#if INTPROGRAM
    epsValue=EPSILONINT;
#else
    epsValue=EPSILON;
#endif
    lp= slp_create_matrix(fpp1);
    glp_init_smcp(&parm);
  /*  parm.presolve = GLP_ON;  */
    parm.tol_bnd = 1.5e-5;
    parm.tol_dj = 1.5e-5;
    parm.tol_piv = 1e-8;
    parm.tm_lim=50; /*parm.tm_lim=50;*/
    parm.meth = GLP_DUALP;

    store=false;
    checked_malloc(pr,objVector,numdbl_t,fpp1->dim, return NULL;);
    while(i<fpp1->ncons){
	if(t>=k)            break;      /* Redundancy removal for the first ks inequalities */
	if(fpp1->ncons<=2){
           checked_free(objVector);
           lp_delete(lp);
	       return fpp1; /* less than 2 linear constraint */
	}
	c=*(fpp1->cons+i*(fpp1->dim+1));
	memcpy(objVector,fpp1->cons+i*(fpp1->dim+1)+1,fpp1->dim*sizeof(numdbl_t));
	if(lp==NULL){  lp= slp_create_matrix(fpp1);  store=false; }
    slp_set_objective(lp,GLP_MAX,objVector);
    slp_disable_kthRow(lp,i+1,c);
    if(store==false){
#if LP_SCALE
	   glp_scale_prob(lp,GLP_SF_AUTO); /* Numerical Instability Problem */
       glp_adv_basis(lp,0); /* Numerical Instability Problem */
#endif
    }
	lp=slp_solve(lp,&parm);
    if(lp==NULL){
          i++;
    }
    else{
	    flag=false;
	    lpres=glp_get_status(lp);
	    if(lpres==GLP_OPT){	/* no optimal, then keep the constraints */
			ci=slp_get_optimal_value(lp);
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
	   }
	   if(flag==true){
		/* remove i-th linear constraint */
		if(fpp1->ncons-i-1>0)
			memmove(fpp1->cons+i*(fpp1->dim+1),fpp1->cons+(i+1)*(fpp1->dim+1),(fpp1->ncons-i-1)*(fpp1->dim+1)*sizeof(numdbl_t));
		else  fpp1->cons=(numdbl_t *)realloc(fpp1->cons,(fpp1->ncons-1)*(fpp1->dim+1)*sizeof(numdbl_t));
		fpp1->ncons=fpp1->ncons-1;
		if(bv!=NULL){
		   if(bv->nvecs-i-1)
			memmove(bv->bits+i*bv->vchars,bv->bits+(i+1)*bv->vchars,(bv->nvecs-i-1)*bv->vchars*sizeof(char));
		   else bv->bits=(char *)realloc(bv->bits,(bv->nvecs-1)*bv->vchars*sizeof(char));
		   bv->nvecs=bv->nvecs-1;
		}
		slp_del_kthRow(lp,i+1);
#if LP_BASIS_RESTORE
			/* store current basis */
		rows=glp_get_num_rows(lp);
		cols=glp_get_num_cols(lp);
		for(j=1;j<=rows;j++){
		   rowStat[j]=glp_get_row_stat(lp,j);
		}
		for(j=1;j<=cols;j++){
		   colStat[j]=glp_get_col_stat(lp,j);
		}
		store=true;
#endif
	   }
	   else{
	      slp_set_upbnd_kthRow(lp,i+1,c); /* restore */
#if LP_BASIS_RESTORE
	      if(store==true){
			/* restore previous basis */
			for(j=1;j<=rows;j++){
			   glp_set_row_stat(lp,j,rowStat[j]);
			}
			for(j=1;j<=cols;j++){
			   glp_set_col_stat(lp,j,colStat[j]);
			}
		    ret=lpx_warm_up(lp);
	        if (ret==LPX_E_BADB) /* the basis is invalid; build some valid basis */
                   lpx_adv_basis(lp);
	      }
#endif
	      i++;
	    }
	  }
	  t++;
    }
    checked_free(objVector);
    lp_delete(lp);
    return fpp1;
}

/*  Redundancy removal the first ks constraints by standard linear programming:           */
/*  	for each inequality l: e<=c in P, posing mu=max(e,P)     */
/*  	if mu<c then remove l from P.				 */
/*  considering envelope info                                                             */
/*  parameter eps: epsilon redundant removal or not.                       */
/*  parameter newNbCons to store the number of the new linear constraints. */
/*  return the linear constraints after redundancy removal. */
fpp_t *redundancy_removal_SLP_firstKs_withEnv(fpp_internal_t* pr,bool destructive, fpp_t *fpp,unsigned k,numdbl_t *env,unsigned nenvs,unsigned envdim, bit_vectors_t *bv, bool eps)
{
    int rowStat[1000],colStat[100];
    unsigned i,j;
    numdbl_t *objVector;
    numdbl_t c,ci;
    glp_prob *lp;
    fpp_t *fpp1;
    fpp_t *fpp01,*fpp02;
    numdbl_t *resCons;
    int lpres;
    unsigned t;
    numdbl_t cic,maxci;
    bool flag;
    int rows,cols;
    bool store;
    numdbl_t epsValue;
    int ret;
    glp_smcp parm;
    int ncons01;
    bit_vectors_t *bv01,*bv02;
    int k2;

    if(fpp->ncons > FM_SPLIT_SIZE){
        ncons01=(int)(fpp->ncons/2);
        fpp01=fpp_alloc_urgent(pr,fpp->dim,fpp->intdim,ncons01);
        memcpy(fpp01->cons,fpp->cons, fpp01->ncons*(fpp01->dim+1)*sizeof(numdbl_t) );
	    memcpy(fpp01->bnds,fpp->bnds, 2*fpp01->dim*sizeof(numdbl_t) );
        bv01 = (bit_vectors_t *)malloc(sizeof(bit_vectors_t));
        bv01->vsize = bv->vsize;
        bv01->vchars= bv->vchars;
        bv01->nvecs= ncons01;
        bv01->bits = (char *)malloc(bv01->nvecs*bv01->vchars*sizeof(char));
        memcpy(bv01->bits,bv->bits,bv01->nvecs*bv01->vchars*sizeof(char));

        fpp02=fpp_alloc_urgent(pr,fpp->dim,fpp->intdim,fpp->ncons-ncons01);
        memcpy(fpp02->cons,fpp->cons+fpp01->ncons*(fpp01->dim+1), (fpp->ncons-fpp01->ncons)*(fpp01->dim+1)*sizeof(numdbl_t) );
	    memcpy(fpp02->bnds,fpp->bnds, 2*fpp02->dim*sizeof(numdbl_t) );
        bv02 = (bit_vectors_t *)malloc(sizeof(bit_vectors_t));
        bv02->vsize = bv->vsize;
        bv02->vchars= bv->vchars;
        bv02->nvecs= bv->nvecs-bv01->nvecs;
        bv02->bits = (char *)malloc(bv02->nvecs*bv02->vchars*sizeof(char));
        memcpy(bv02->bits,bv->bits+bv01->nvecs*bv01->vchars,bv02->nvecs*bv02->vchars*sizeof(char));
  	    if(fpp01->ncons > k){
           fpp01=redundancy_removal_SLP_firstKs_withEnv(pr,true,fpp01,k,env,nenvs,envdim, bv01,eps);
        }
        else{
	       k2=k-fpp01->ncons;
           fpp01=redundancy_removal_SLP_firstKs_withEnv(pr,true,fpp01,fpp01->ncons,env,nenvs,envdim, bv01,eps);
           fpp02=redundancy_removal_SLP_firstKs_withEnv(pr,true,fpp02,k2,env,nenvs,envdim, bv02,eps);
	    }
	    k=k-(fpp->ncons-fpp01->ncons-fpp02->ncons);
    	fpp1=fpp_copy_internal(pr, fpp01);
        fpp1->ncons=fpp01->ncons+fpp02->ncons;
        fpp1->cons=(numdbl_t *)realloc(fpp1->cons, fpp1->ncons*(fpp1->dim+1)*sizeof(numdbl_t));
	    memcpy(fpp1->cons+fpp01->ncons*(fpp1->dim+1),fpp02->cons, fpp02->ncons*(fpp1->dim+1)*sizeof(numdbl_t) );
        bv->nvecs= bv01->nvecs+bv02->nvecs;
        bv->bits = (char *)realloc(bv->bits,bv->nvecs*bv->vchars*sizeof(char));
        memcpy(bv->bits,bv01->bits,bv01->nvecs*bv01->vchars*sizeof(char));
        memcpy(bv->bits+bv01->nvecs*bv01->vchars,bv02->bits,bv02->nvecs*bv02->vchars*sizeof(char));
        bv_free(bv01);
        bv_free(bv02);
	    fpp_free_internal(pr, fpp01);
	    fpp_free_internal(pr, fpp02);
        if(destructive==true)
		   fpp_free_internal(pr, fpp);
    }
    else{
        if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
        else	fpp1=fpp;
    }
    if(fpp1->flag!=GENERAL_POL || fpp1->ncons<=1 ) return fpp1;

    fpp1->cons=realloc(fpp1->cons,(fpp1->ncons+nenvs)*(fpp1->dim+1)*sizeof(numdbl_t));
    memset(fpp1->cons+fpp1->ncons*(fpp1->dim+1),0,nenvs*(fpp1->dim+1)*sizeof(numdbl_t));
    for(i=0;i<nenvs;i++){
	   memcpy(fpp1->cons+(fpp1->ncons+i)*(fpp1->dim+1),env+i*(envdim+1),(envdim+1)*sizeof(numdbl_t));
    }
    fpp1->ncons=fpp1->ncons+nenvs;

#if INTPROGRAM
    epsValue=EPSILONINT;
#else
    epsValue=EPSILON;
#endif
    i=0;
    t=0;
    lp= slp_create_matrix(fpp1);
    glp_init_smcp(&parm);
    parm.tol_bnd = 1.5e-5;
    parm.tol_dj = 1.5e-5;
    parm.tol_piv = 1e-8;
    parm.tm_lim=50;
    parm.meth = GLP_DUALP;
    store=false;
    checked_malloc(pr,objVector,numdbl_t,fpp1->dim, return NULL;);
	while(i<fpp1->ncons){
		if(t>=k)            break;      /* Redundancy removal for the first ks inequalities */
		if(fpp1->ncons<=2){
		   checked_free(objVector);
		   lp_delete(lp);
		   return fpp1; /* less than 2 linear constraint */
		}
		c=*(fpp1->cons+i*(fpp1->dim+1));
		memcpy(objVector,fpp1->cons+i*(fpp1->dim+1)+1,fpp1->dim*sizeof(numdbl_t));
		if(lp==NULL){  lp= slp_create_matrix(fpp1);  store=false; }
			slp_set_objective(lp,GLP_MAX,objVector);
			slp_disable_kthRow(lp,i+1,c);
			if(store==false){
#if LP_SCALE
			   glp_scale_prob(lp,GLP_SF_AUTO); /* Numerical Instability Problem */
			   glp_adv_basis(lp,0); /* Numerical Instability Problem */
#endif
			}

			if(eps==true){
			   if(c==0) parm.obj_ul=epsValue;
			   else parm.obj_ul=c+fabs(c)*epsValue;
			}
			else parm.obj_ul=c;

			lp=slp_solve(lp,&parm);
			flag=false;
			if(lp==NULL){
			   i++;
			}
			else{
			  lpres=glp_get_status(lp);
			  if(lpres==GLP_OPT){
				ci=slp_get_optimal_value(lp);
				if(ci<=c){  /* the i-th linear constraint is redundant */
					flag=true;
				}
				else if(eps==true){ /* ci > c*/
					cic=ci-c;
					maxci=fabs(c);/*maxci=fmax(-c,ci);*/
					if( (maxci==0.0 && cic<epsValue) || (maxci>0.0 && cic/maxci<epsValue) ){
						flag=true;
					}
				}
			  }
			  if(flag==true){
			     /* remove i-th linear constraint */
				if(fpp1->ncons-i-1>0)
					memmove(fpp1->cons+i*(fpp1->dim+1),fpp1->cons+(i+1)*(fpp1->dim+1),(fpp1->ncons-i-1)*(fpp1->dim+1)*sizeof(numdbl_t));
				else  fpp1->cons=(numdbl_t *)realloc(fpp1->cons,(fpp1->ncons-1)*(fpp1->dim+1)*sizeof(numdbl_t));
				fpp1->ncons=fpp1->ncons-1;
				if(bv!=NULL){
				   if((bv->nvecs-i-1)>0)
					 memmove(bv->bits+i*bv->vchars,bv->bits+(i+1)*bv->vchars,(bv->nvecs-i-1)*bv->vchars*sizeof(char));
				   else bv->bits=(char *)realloc(bv->bits,(bv->nvecs-1)*bv->vchars*sizeof(char));
				   bv->nvecs=bv->nvecs-1;
				}
				slp_del_kthRow(lp,i+1);
#if LP_BASIS_RESTORE
					/* store current basis */
				rows=glp_get_num_rows(lp);
				cols=glp_get_num_cols(lp);
				for(j=1;j<=rows;j++){
				   rowStat[j]=glp_get_row_stat(lp,j);
				}
				for(j=1;j<=cols;j++){
				   colStat[j]=glp_get_col_stat(lp,j);
				}
				store=true;
#endif
			  }
			  else{
				 slp_set_upbnd_kthRow(lp,i+1,c); /* restore */
					  /* slp_restore_kthRow(lp,i+1,fpp1->cons+i*(fpp1->dim+1));   */
#if LP_BASIS_RESTORE
			    if(store==true){
					/* restore previous basis */
				for(j=1;j<=rows;j++){
				   glp_set_row_stat(lp,j,rowStat[j]);
				}
				for(j=1;j<=cols;j++){
				   glp_set_col_stat(lp,j,colStat[j]);
				}
				ret=lpx_warm_up(lp);
					if (ret==LPX_E_BADB) /* the basis is invalid; build some valid basis */
						   lpx_adv_basis(lp);
			    }
#endif
				i++;
			 }
		}
		t++;
    }
    checked_free(objVector);
    lp_delete(lp);
	/* remove envelope */
    fpp1->cons=realloc(fpp1->cons,(fpp1->ncons-nenvs)*(fpp1->dim+1)*sizeof(numdbl_t));
    fpp1->ncons=fpp1->ncons-nenvs;
    return fpp1;
}


/*  Redundancy removal the first ks constraints by standard linear programming:        */
/*  	for each inequality l: e<=c in P, posing mu=max(e,P)                           */
/*  	if mu<c then remove l from P.		                          		           */
/*  considering sigma info  0<=\sigma<=1                                               */
/*  parameter eps: epsilon redundant removal or not.                                   */
/*  parameter newNbCons to store the number of the new linear constraints.             */
/*  return the linear constraints after redundancy removal.                            */
fpp_t *redundancy_removal_SLP_firstKs_withSigma(fpp_internal_t* pr,bool destructive, fpp_t *fpp,unsigned k,bit_vectors_t *bv, bool eps)
{
    int rowStat[1000],colStat[100];
    unsigned i,j;
    numdbl_t *objVector;
    numdbl_t c,ci;
    glp_prob *lp;
    fpp_t *fpp1;
    numdbl_t *resCons;
    int lpres;
    unsigned t;
    numdbl_t cic,maxci;
    bool flag;
    int rows,cols;
    bool store;
    numdbl_t epsValue;
    int ret;
    glp_smcp parm;

    if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;
    if(fpp1->flag!=GENERAL_POL || fpp1->ncons<=1 ) return fpp1;

    fpp1->cons=realloc(fpp1->cons,(fpp1->ncons+2)*(fpp1->dim+1)*sizeof(numdbl_t));
    memset(fpp1->cons+fpp1->ncons*(fpp1->dim+1),0,2*(fpp1->dim+1)*sizeof(numdbl_t));
	/*  1 >= \sigma  */
    *(fpp1->cons+fpp1->ncons*(fpp1->dim+1))=1;
    *(fpp1->cons+fpp1->ncons*(fpp1->dim+1)+fpp1->dim)=1;
	/* 0 >= -\sigma  */
    *(fpp1->cons+(fpp1->ncons+1)*(fpp1->dim+1))=0;
    *(fpp1->cons+(fpp1->ncons+1)*(fpp1->dim+1)+fpp1->dim)=-1;

    fpp1->ncons=fpp1->ncons+2;
#if INTPROGRAM
    epsValue=EPSILONINT;
#else
    epsValue=EPSILON;
#endif
    i=0;
    t=0;
    lp= slp_create_matrix(fpp1);
    glp_init_smcp(&parm);
/*  parm.presolve = GLP_ON;
    parm.tol_bnd = 1e-6;
    parm.tol_dj = 1e-6; */
    parm.meth = GLP_DUALP;
    store=false;
    checked_malloc(pr,objVector,numdbl_t,fpp1->dim, return NULL;);
    while(i<fpp1->ncons){
	if(t>=k)            break;      /* Redundancy removal for the first ks inequalities */
	if(fpp1->ncons<=2){
           lp_delete(lp);
	   return fpp1; /* less than 2 linear constraint */
	}
	c=*(fpp1->cons+i*(fpp1->dim+1));
	memcpy(objVector,fpp1->cons+i*(fpp1->dim+1)+1,fpp1->dim*sizeof(numdbl_t));
	if(lp==NULL){  lp= slp_create_matrix(fpp1);  store=false; }

    slp_set_objective(lp,GLP_MAX,objVector);
    slp_disable_kthRow(lp,i+1,c);
    if(store==false){
#if LP_SCALE
	   glp_scale_prob(lp,GLP_SF_AUTO); /* Numerical Instability Problem */
       glp_adv_basis(lp,0); /* Numerical Instability Problem */
#endif
    }
	lp=slp_solve(lp,&parm);
    if(lp==NULL){
            i++;
    }
    else{
	    flag=false;
	    lpres=glp_get_status(lp);
	    if(lpres==GLP_OPT){
			ci=slp_get_optimal_value(lp);
			if(ci<=c){  /* the i-th linear constraint is redundant */
	 			flag=true;
			}
			else if(eps==true){ /* ci > c*/
				cic=ci-c;
				maxci=fabs(c);/*maxci=fmax(-c,ci);*/
				if( (maxci==0.0 && cic<epsValue) || (maxci>0.0 && cic/maxci<epsValue) ){
					flag=true;
				}
			}
	   }
	   if(flag==true){
		/* remove i-th linear constraint */
		if(fpp1->ncons-i-1>0)
			memmove(fpp1->cons+i*(fpp1->dim+1),fpp1->cons+(i+1)*(fpp1->dim+1),(fpp1->ncons-i-1)*(fpp1->dim+1)*sizeof(numdbl_t));
		else fpp1->cons=(numdbl_t *)realloc(fpp1->cons,(fpp1->ncons-1)*(fpp1->dim+1)*sizeof(numdbl_t));
		fpp1->ncons=fpp1->ncons-1;
		if(bv!=NULL){
		   if(bv->nvecs-i-1>0)
			memmove(bv->bits+i*bv->vchars,bv->bits+(i+1)*bv->vchars,(bv->nvecs-i-1)*bv->vchars*sizeof(char));
		   else bv->bits=(char *)realloc(bv->bits,(bv->nvecs-1)*bv->vchars*sizeof(char));
		   bv->nvecs=bv->nvecs-1;
		}
		slp_del_kthRow(lp,i+1);
#if LP_BASIS_RESTORE
			/* store current basis */
		rows=glp_get_num_rows(lp);
		cols=glp_get_num_cols(lp);
		for(j=1;j<=rows;j++){
		   rowStat[j]=glp_get_row_stat(lp,j);
		}
		for(j=1;j<=cols;j++){
		   colStat[j]=glp_get_col_stat(lp,j);
		}
		store=true;
#endif
	   }
	   else{
	      slp_set_upbnd_kthRow(lp,i+1,c); /* restore */
#if LP_BASIS_RESTORE
	      if(store==true){
				/* restore previous basis */
			for(j=1;j<=rows;j++){
			   glp_set_row_stat(lp,j,rowStat[j]);
			}
			for(j=1;j<=cols;j++){
			   glp_set_col_stat(lp,j,colStat[j]);
			}
			ret=lpx_warm_up(lp);
			if (ret==LPX_E_BADB) /* the basis is invalid; build some valid basis */
			   lpx_adv_basis(lp);
	      }
#endif
	      i++;
	   }
	 }
     t++;
    }
    checked_free(objVector);
    lp_delete(lp);
	/* remove envelope */
    fpp1->cons=realloc(fpp1->cons,(fpp1->ncons-2)*(fpp1->dim+1)*sizeof(numdbl_t));
    fpp1->ncons=fpp1->ncons-2;
    return fpp1;
}

/*  Redundancy removal the last ks constraints by standard linear programming:  */
/*  	for each inequality l: e<=c in P, posing mu=max(e,P)                    */
/*  	if mu<c then remove l from P.				                            */
/*  parameter eps: epsilon redundant removal or not.                            */
/*  parameter newNbCons to store the number of the new linear constraints.      */
/*  return the linear constraints after redundancy removal. */
fpp_t *redundancy_removal_SLP_lastKs(fpp_internal_t* pr,bool destructive, fpp_t *fpp,unsigned k, bool eps)
{
    int rowStat[1000],colStat[100];
    int ret;
    bool store;
    unsigned i,j,m;
    numdbl_t *objVector;
    numdbl_t c,ci;
    glp_prob *lp;
    fpp_t *fpp1;
    numdbl_t *resCons;
    int lpres;
    int rows,cols;
    numdbl_t cic,maxci;
    bool rrFlag;
    numdbl_t epsValue;
    glp_smcp parm;

    if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;
    if(fpp1->flag!=GENERAL_POL || fpp1->ncons<=1 ) return fpp1;
#if INTPROGRAM
    epsValue=EPSILONINT;
#else
    epsValue=EPSILON;
#endif
    i=fpp1->ncons-k; /* Redundancy removal for the last ks inequalities */
    lp= slp_create_matrix(fpp1);
    glp_init_smcp(&parm);
    parm.meth = GLP_DUALP;
    store=false;
    checked_malloc(pr,objVector,numdbl_t,fpp1->dim, return NULL;);
    while(i<fpp1->ncons){
	if(fpp1->ncons<=2){ /* only one linear constraint */
    	     checked_free(objVector);
             lp_delete(lp);
	     return fpp1;
	}
	c=*(fpp1->cons+i*(fpp1->dim+1));
	memcpy(objVector,fpp1->cons+i*(fpp1->dim+1)+1,fpp1->dim*sizeof(numdbl_t));
    slp_set_objective(lp,GLP_MAX,objVector);
    slp_disable_kthRow(lp,i+1,c);
	lp=slp_solve(lp,&parm);
    if(lp!=NULL){
		rrFlag=false;
		lpres=glp_get_status(lp);
		if(lpres==GLP_OPT){
			ci=slp_get_optimal_value(lp);
			if(ci<=c){  /* the i-th linear constraint is redundant */
	 			rrFlag=true;
			}
			else if(eps==true){ /* ci > c*/
				cic=ci-c;
				maxci=fabs(c);/*maxci=fmax(-c,ci);*/
				if( (maxci==0.0 && cic<epsValue) || (maxci>0.0 && cic/maxci<epsValue) ){
					rrFlag=true;
				}
			}
		}
		if(rrFlag==true){
			/* remove i-th linear constraint */
		    if(fpp1->ncons-i-1>0)
				memmove(fpp1->cons+i*(fpp1->dim+1),fpp1->cons+(i+1)*(fpp1->dim+1),(fpp1->ncons-i-1)*(fpp1->dim+1)*sizeof(numdbl_t));
			else
				fpp1->cons=(numdbl_t *) realloc(fpp->cons,(fpp1->ncons-1)*(fpp1->dim+1)*sizeof(numdbl_t));
			fpp1->ncons=fpp1->ncons-1;
			slp_del_kthRow(lp,i+1);
#if LP_BASIS_RESTORE
				/* store current basis */
			rows=glp_get_num_rows(lp);
			cols=glp_get_num_cols(lp);
			for(j=1;j<=rows;j++){
		   	   rowStat[j]=glp_get_row_stat(lp,j);
			}
			for(j=1;j<=cols;j++){
		   	    colStat[j]=glp_get_col_stat(lp,j);
			}
			store=true;
#endif
		}
		else{
            slp_set_upbnd_kthRow(lp,i+1,c); /* restore */
#if LP_BASIS_RESTORE
	      	if(store==true){
				/* restore previous basis */
			   for(j=1;j<=rows;j++){
		   		glp_set_row_stat(lp,j,rowStat[j]);
			   }
			   for(j=1;j<=cols;j++){
		   		glp_set_col_stat(lp,j,colStat[j]);
			   }
			   ret=lpx_warm_up(lp);
	        	   if (ret==LPX_E_BADB) /* the basis is invalid; build some valid basis */
                   		lpx_adv_basis(lp);
	      	}
#endif
			i++;
		}
    }
  	else{
		lp= slp_create_matrix(fpp1);
		store=false;
		i++;
    }
   }
   checked_free(objVector);
   lp_delete(lp);
   return fpp1;
}

/*  Select the optimal variable to eliminate    */
/*  min(postive rows * negative rows)   		*/
/*  variable index from 1   				    */
unsigned fm_select(fpp_t *fpp, unsigned startI, unsigned endI)
{
  unsigned posI[100];
  unsigned negI[100];
  unsigned i,j,minI=0,min;
  numdbl_t *con,ai; /* the coefficient of varIndex in row i */

  if(startI==endI) return startI;

  for(i=startI;i<=endI;i++){
      posI[i]=0;
      negI[i]=0;
  }
  for(i=0;i<fpp->ncons;i++)
  {
     con=fpp->cons+i*(fpp->dim+1);
     for(j=startI;j<=endI;j++){
    	 ai=*(con+j);
         if(ai>0.0)        posI[j]++;
         else if(ai<-0.0)  negI[j]++;
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

/*  Redundancy removal the first ks constraints by variable bounds info */
fpp_t *redundancy_removal_firstKs_byBounds(fpp_internal_t* pr,bool destructive, fpp_t *fpp, numdbl_t *bounds,unsigned k,bit_vectors_t *bv)
{
    char *rrIndex;
    unsigned i,n;
    numdbl_t c,ci;
    fpp_t *fpp1;
    numdbl_t *li,*cons1;
    numdbl_t expUp;
    char *bits1=NULL;
    int keepNb;

    if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;
    if(fpp1->flag!=GENERAL_POL || fpp1->ncons<=1 ) return fpp1;

    keepNb=fpp1->ncons-k;
    i=0;
    checked_malloc(pr,rrIndex,char,k, return NULL;);
    memset(rrIndex,0,k*sizeof(char));
    while(i<k){
		li=fpp1->cons+i*(fpp1->dim+1);  /* aik */
		ci=*li;
		numitv_t itv1=numitv_lexp_range(fpp1->dim,li+1,bounds);
		expUp=itv1.sup;
		if(expUp < ci ){ /* i is redundant */
			 rrIndex[i]=true;
		}
		i++;
    }

    checked_malloc(pr,cons1,numdbl_t,fpp1->ncons*(fpp1->dim+1), return NULL;);
    if(bv!=NULL) checked_malloc(pr,bits1,char,bv->nvecs*bv->vchars, return NULL;);
    n=0;
    for(i=0;i<k;i++){
		if(rrIndex[i]==true){
		   continue;
		}
		   /* i-th constraint is non-redundant */
		memcpy(cons1+n*(fpp1->dim+1),fpp1->cons+i*(fpp1->dim+1),(fpp1->dim+1)*sizeof(numdbl_t));
		if(bv!=NULL) memcpy(bits1+n*(bv->vchars),bv->bits+i*(bv->vchars),bv->vchars*sizeof(char));
		n++;
    }

    if(keepNb>0){
    	memcpy(cons1+n*(fpp1->dim+1),fpp1->cons+k*(fpp1->dim+1),keepNb*(fpp1->dim+1)*sizeof(numdbl_t));
    }
    cons1=realloc(cons1,(n+keepNb)*(fpp1->dim+1)*sizeof(numdbl_t));
    checked_free(fpp1->cons);
    fpp1->cons=cons1;
    fpp1->ncons=n+keepNb;
    if(bv!=NULL) {
    	bits1=realloc(bits1,n*(bv->vchars)*sizeof(char));
    	checked_free(bv->bits);
    	bv->bits=bits1;
    	bv->nvecs=n;
    }
    checked_free(rrIndex);
    return fpp1;
}

/***********************************************************/
/*                  Rational                               */
/***********************************************************/

/*  Standard Fourier-Motzkin Elimination in rational:*/
/*  		postive rows + negative rows             */
/*  		zero rows                                */
/*  parameter varIndex stores the index(start from 1) of the variable to be eliminated. */
/*  parameter newNbCons to store the number of the new linear constraints.               */
/*  return the newly generated linear constraints.   */
fpp_t *fm_elimination_rational(fpp_internal_t* pr, bool destructive, fpp_t *fpp, unsigned varIndex, bit_vectors_t* bv,  bool *subset)
{
  unsigned posIndex[5000],posI;
  unsigned negIndex[5000],negI;
  unsigned zeroIndex[5000],zeroI;
  unsigned i,j,k;
  numdbl_t *coeffi; /* the coefficient of varIndex in row i */
  numdbl_t a1k,a1i,a2i,a2k,a3k,b3k;
  numdbl_t c,c1,c2;
  numdbl_t *resCons,*resq,*aCons,*p;
  unsigned ncons1;
  unsigned counter=0;
  numdbl_t a1x2idown,a1x2iup;
  unsigned m;
  bool dealFlag;
  fpp_t *fpp1;
  bool sat;
  unsigned nenvs;
  bit_vectors_t* bv1;
  char *vec;

  *subset=true;
  if(fpp->dim==1){
     fpp1=fpp_alloc_internal(pr,fpp->dim,fpp->intdim); /* EMPTY_POL */
     if(destructive==true)	fpp_free_internal(pr, fpp);
     return fpp1;
  }
	/* scan all the coefficients of varIndex in each inequality to collect the sign information */
    /*        positive; negative; zero(including -0.0 and +0.0) */
  posI=0;
  negI=0;
  zeroI=0;
  for(i=0;i<fpp->ncons;i++)
  {
	coeffi=fpp->cons+i*(fpp->dim+1)+varIndex; /*(fpp->dim+1) columns per row*/
	if(*coeffi>0.0){
		posIndex[posI]=i;
		posI++;
        }
	else if(*coeffi<-0.0){
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
		fpp1=fpp_copy_internal(pr, fpp);
	else	fpp1=fpp;
	checked_free(fpp1->cons);
        checked_free(fpp1->bnds);
	fpp1->ncons=0;
	fpp1->flag=UNIVERSE_POL;
        /* if(bv!=NULL)  bv_free(bv); */
	return fpp1;
  }
  /* --------------------------------------------------------- */
  /* Sound Fourier Elimination with floating point arithmetic  */
  /* --------------------------------------------------------- */
  checked_malloc(pr,resCons,numdbl_t,(posI*negI+zeroI)*(fpp->dim+1), return NULL;);
  resq=resCons;
  if(bv!=NULL){
     nenvs=fpp->ncons-bv->nvecs;
     bv1=bv_copy(bv);
     bv1->nvecs=bv1->nvecs+posI*negI+zeroI;
     bv1->bits = realloc(bv1->bits,bv1->nvecs*bv1->vchars*sizeof(char));
  }
  ncons1=0;
  k=0; /* the index for the new generated constraint */
  if(posI>0 && negI>0){
    for(i=0;i<posI;i++) /* positive row: posIndex[i] */
	{
		a1i=*(fpp->cons+posIndex[i]*(fpp->dim+1)+varIndex); /* a_1i of row posIndex[i] */
		c1=*(fpp->cons+posIndex[i]*(fpp->dim+1)); /* 0-th(constant) of row posIndex[i] */

		/* ----------------------------------- */
		/* positive row + negative row       */
		/* ----------------------------------- */
		for(j=0;j<negI;j++) /* negative row: negIndex[j] */
		{
			dealFlag=false;
			a2i=*(fpp->cons+negIndex[j]*(fpp->dim+1)+varIndex); /* a_2i of row negIndex[j] */
			/* positive row + negative row */
			c2=*(fpp->cons+negIndex[j]*(fpp->dim+1)); /* 0-th(constant) of row negIndex[j] */
			c=c1*(-a2i)+c2*a1i;
			aCons=p=(numdbl_t *)malloc((fpp->dim+1) * sizeof(numdbl_t));
			*p=c;
			p++;
			for(k=1; k<=fpp->dim; k++) /* negative row: negIndex[j] */
			{
				if(k!=varIndex){
					a1k=*(fpp->cons+posIndex[i]*(fpp->dim+1)+k); /* a_1k of row posIndex[i] */
					a2k=*(fpp->cons+negIndex[j]*(fpp->dim+1)+k); /* a_2k of row negIndex[j] */
					b3k=a1k*(-a2i)+a2k*a1i;
					*p=b3k;  /* coefficient for Vk */
					p++;
				}
				else{
					*p=0;  /* ) for V_varIndex */
					p++;
				}
			}
			aCons=normalise_integer_inequality_withConstant(fpp->dim, aCons,&sat);
			if(aCons==NULL){
			   if(sat==false){/* unsatifiable */
				  checked_free(resCons);
				  if(bv!=NULL) bv_free(bv1);
				  fpp1=fpp_alloc_internal(pr, fpp->dim,fpp->intdim);
				  if(destructive==true)  fpp_free_internal(pr, fpp);
				  return fpp1;
				}
			}
			else{
			   memcpy(resq,aCons,(fpp->dim+1)*sizeof(numdbl_t));
			   checked_free(aCons);
			   resq=resq+fpp->dim+1;
			   if(bv!=NULL){
				  /* Bit Vectors */
				  vec=bv_or(bv,bv->bits+posIndex[i]*bv->vchars,bv->bits+negIndex[j]*bv->vchars);
				  bv_set(bv1,ncons1,vec);
				  checked_free(vec);
			   }
			   ncons1++;
			}
		}
     }
  }


  /* ---------------------------------------------------------------- */
  /* copy constraints in which the coefficient for V_varIndex is zero */
  /* ---------------------------------------------------------------- */
  if(ncons1>0)   *subset=false;
  for(i=0;i<zeroI;i++) /* zero row: zeroIndex[i] */
  {
	if(counter>1023){
		resCons=(numdbl_t *)realloc(resCons, (ncons1+1024) *(fpp->dim+1)* sizeof(numdbl_t));
		resq=resCons+ncons1*(fpp->dim+1);
		counter=0;
	}
	memcpy(resq,fpp->cons+zeroIndex[i]*(fpp->dim+1),(fpp->dim+1)*sizeof(numdbl_t));
	resq=resq+fpp->dim+1;
	counter++;
	if(bv!=NULL){
		/* Bit Vectors */
	  bv_set(bv1,ncons1,bv->bits+zeroIndex[i]*bv->vchars);
	}
	ncons1++;
   }

  /* --------------------------------------------------------------- */
  /* post-processing before return result constraints                */
  /* --------------------------------------------------------------- */
   if(destructive==false)
	    fpp1=fpp_copy_internal(pr, fpp);
   else	fpp1=fpp;

   if(ncons1>0){
      resCons=(numdbl_t *)realloc(resCons,ncons1*(fpp->dim+1)*sizeof(numdbl_t)); /* truncate */
      if(bv!=NULL){
	   /* Bit Vectors */
         bv1->nvecs=ncons1-nenvs;
         bv1->bits = realloc(bv1->bits,bv1->nvecs*bv1->vchars*sizeof(char));
         bv->nvecs=bv1->nvecs;
         checked_free(bv->bits);
         bv->bits=bv1->bits;
         checked_free(bv1);
      }
      checked_free(fpp1->cons);
      fpp1->ncons=ncons1;
      fpp1->cons=resCons;
   }
   else{
      if(bv!=NULL) bv_free(bv1);
      checked_free(resCons);
      checked_free(fpp1->cons);
      checked_free(fpp1->bnds);
      fpp1->ncons=0;
      fpp1->flag=UNIVERSE_POL;
   }
   return fpp1;
}
