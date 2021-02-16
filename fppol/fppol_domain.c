/*
 * fppol_domain.c
 *
 * Operations of polyhedra domain with floating point arithmetic.
 * A polyhedron is represented by linear constraints and bounds of variables.
 * The linear constraint has the form of Ax<=b while the bounds has the form of x0<=x<=x1.
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
#include <fenv.h> /* floating-point lib */
#include <glpk.h> /* glpk lib */

#include "fm.h"
#include "rlp.h"
#include "bt.h"
#include "numitv.h"
#include "quasi_linear.h"
#include "fppol_domain.h"
#include "fpp_user.h"
#include "bv.h"
#include "fpp_weakjoin.h"

/* ================================================== */
/* Bounded Polyhedra Domain: Operations */
/* ================================================== */

/* combining strong/weak joins strategies */
fpp_t *fppol_joins(fpp_internal_t* pr, fpp_t *fpp01, fpp_t *fpp02)
{
   fpp_t *fpp12;
   numdbl_t fact1,fact2;
   numdbl_t non0t1,non0t2;
   bool flag;


   if(fpp01->flag==UNIVERSE_POL)         return fpp_copy_internal(pr, fpp01);
   else if(fpp02->flag==UNIVERSE_POL)    return fpp_copy_internal(pr, fpp02);
   else if(fpp01->flag==EMPTY_POL )      return fpp_copy_internal(pr, fpp02);
   else if(fpp02->flag==EMPTY_POL )      return fpp_copy_internal(pr, fpp01);

   fact1=fppol_sparse_factor(fpp01);
   fact2=fppol_sparse_factor(fpp02);

   non0t1=fact1*fpp01->dim*fpp01->ncons;
   non0t2=fact2*fpp02->dim*fpp02->ncons;
   flag=1;
   if( fact1+fact2>0.5 && (non0t1>70 || non0t2>70) ){ /* Users may use other metrics to choose weak join*/
	flag=0;
   }
   if( flag==1 ){
   	fpp12=fppol_strong_join(pr,fpp01,fpp02);
    nstrongjoin++;
   }
   else{
	fpp12=fppol_weak_joins(pr, fpp01, fpp02);
    nweakjoin++;
   }

   return fpp12;
}

/* Polyhedron Join--Convex Hull */
/*
$$
P_{CH} = \left\{ {x\left| {\begin{array}{*{20}c}
   {x = y_1+y_2 \ \ \wedge \sigma _1  + \sigma _2  = 1 \ \wedge \sigma _1  \ge 0 \wedge }  \\
   {A_1 y_1  \le \sigma _1 b_1 \wedge A_2y_2  \le \sigma _2 b_2 \ \wedge \sigma _2  \ge 0}  \\
\end{array}} \right.} \right\}.
$$
that is
$$
P_{CH} = \left\{ {x\left| {\begin{array}{*{20}c}
{       0 \leq \sigma_1 \leq 1
\wedge  A_1*y_1 -b_1*\sigma_1 \leq 0
\wedge  A_2*x-A_2*y_1+b_2*\sigma_1 \leq b_2         }  \\
\end{array}} \right.} \right\}.
$$
*/
/* the intermediate repsentation of P_CH with (2*fpp1->dim+1) dimension */
/* c x y1 \sigma_1 */
fpp_t *fppol_strong_join(fpp_internal_t* pr, fpp_t *fpp01, fpp_t *fpp02)
{
   unsigned i,j,k,nchars,nshift,nrem;
   unsigned nenvs,nchbnds;
   numdbl_t *env, *penv;
   numdbl_t *chbnds,*p,*bd12,*bd1,*bd2;
   numdbl_t *res,*cons1;
   fpp_t *fpp1,*fpp2;
   fpp_t *fpp12; /* double dimension -> single dimension */
   char *bits,*bitVector;
   bit_vectors_t *bv;
   int superset;
   char *env1Index,*env2Index;
   char *env1bits,*nenv1bits,*env2bits,*nenv2bits,*sigmabits;
   int ni;
   numdbl_t *bnds1_12;
   numdbl_t *bnds12_bak;
   bool subset;
   bool flag;
   int r;

   if(fpp01->flag==UNIVERSE_POL)         return fpp_copy_internal(pr, fpp01);
   else if(fpp02->flag==UNIVERSE_POL)    return fpp_copy_internal(pr, fpp02);
   else if(fpp01->flag==EMPTY_POL )      return fpp_copy_internal(pr, fpp02);
   else if(fpp02->flag==EMPTY_POL )      return fpp_copy_internal(pr, fpp01);

#if LAZY_RR
	/* lazy redundancy removal */
   fpp01=redundancy_removal(pr,true,fpp01,true);
   fpp02=redundancy_removal(pr,true,fpp02,true);
#endif

#if  DEBUG_JOIN || PRINT_JWRES
   printf("fppol_strong_join:: \n");
   fpp_fdump(stdout, pr->man, fpp01);
   fpp_fdump(stdout, pr->man, fpp02);
   fflush(stdout);
#endif

	/* the envelope of fpp1 and fpp2 is certainly part of their convex hull */
    checked_malloc(pr,env1Index,char,fpp01->ncons, return NULL;);
    checked_malloc(pr,env2Index,char,fpp02->ncons, return NULL;);
    memset(env1Index,0,fpp01->ncons*sizeof(char));
    memset(env2Index,0,fpp02->ncons*sizeof(char));
    env=penv=fppol_envelope(fpp01,fpp02, &nenvs,&superset, env1Index, env2Index);
    if(superset==1){
		checked_free(env);
		checked_free(env1Index);
		checked_free(env2Index);

		return fpp_copy_internal(pr, fpp01);
    }
    else if(superset==2){
		checked_free(env);
		checked_free(env1Index);
		checked_free(env2Index);

		return fpp_copy_internal(pr, fpp02);
    }

   fpp1=fpp_copy_internal(pr, fpp01);
   fpp2=fpp_copy_internal(pr, fpp02);

   flag=fppol_factorization(pr,fpp01,fpp02,fpp1,fpp2);
   if(flag==true){
      if(fpp1->flag==UNIVERSE_POL ){
         checked_free(env);
		 checked_free(env1Index);
		 checked_free(env2Index);
         fpp_free_internal(pr, fpp2);
         return fpp1;
      }
      else if( fpp2->flag==UNIVERSE_POL  ){
         checked_free(env);
		 checked_free(env1Index);
		 checked_free(env2Index);
         fpp_free_internal(pr, fpp1);
         return fpp2;
      }
      else if(fpp1->flag==EMPTY_POL ){
         checked_free(env);
		 checked_free(env1Index);
		 checked_free(env2Index);
         fpp_free_internal(pr, fpp1);
         return fpp2;
      }
      else if(fpp2->flag==EMPTY_POL ){
         checked_free(env);
	     checked_free(env1Index);
	     checked_free(env2Index);
         fpp_free_internal(pr, fpp2);
         return fpp1;
      }
   }
   for(i=0;i<fpp1->ncons;i++){
	r=normalise_quasiInteger_inequality(fpp1->dim,fpp1->cons+i*(fpp1->dim+1),fpp1->bnds);
	if(flag==false && r!=UNCHANGED)  flag=true;
   }
   for(i=0;i<fpp2->ncons;i++){
	r=normalise_quasiInteger_inequality(fpp2->dim,fpp2->cons+i*(fpp2->dim+1),fpp2->bnds);
	if(flag==false && r!=UNCHANGED)  flag=true;
   }
   if(flag==true){
	/* recalculate envelope */
      checked_free(env);
      memset(env1Index,0,fpp1->ncons*sizeof(char));
      memset(env2Index,0,fpp2->ncons*sizeof(char));
      env=penv=fppol_envelope(fpp1,fpp2, &nenvs,&superset, env1Index, env2Index);
      if(superset==1){
		  checked_free(env);
		  checked_free(env1Index);
		  checked_free(env2Index);
          fpp_free_internal(pr, fpp2);
		  return fpp1;
      }
      else if(superset==2){
		  checked_free(env);
		  checked_free(env1Index);
		  checked_free(env2Index);
          fpp_free_internal(pr, fpp1);
		  return fpp2;
      }
   }
#if  DEBUG_JOIN
   printf("fppol_strong_join after pre-processing:: \n");
   fpp_fdump(stdout, pr->man, fpp1);
   fpp_fdump(stdout, pr->man, fpp2);
   fflush(stdout);
#endif
    fpp12=fppol_construct_convex_hull(pr,fpp1,fpp2,NULL/*env*/,0/*nenvs*/);
    /* bounds info for the first fpp1->dim vars */
    checked_malloc(pr,bnds12_bak,numdbl_t,2*fpp1->dim, return NULL;);
    memcpy(bnds12_bak,fpp12->bnds,2*fpp1->dim*sizeof(numdbl_t));
    checked_malloc(pr,chbnds,numdbl_t,2*fpp1->dim*(fpp1->dim+1), return NULL;);
    memset(chbnds,0,2*fpp1->dim*(fpp1->dim+1)*sizeof(numdbl_t));
    p=chbnds;
    nchbnds=0;
    for(i=0;i<fpp1->dim;i++){
        bd12=fpp12->bnds+2*i;
     	  	/* lower bound:   -V_i <= -\underline{V_i} */
        if(*bd12!=-MAX_VARBND){
    	   *p=-(*bd12);    /* -\underline{V_i} */
    	   *(p+i+1)=-1;    /* -V_i */
    	   p=p+fpp1->dim+1;
    	   nchbnds++;
		}
		  /* upper bound:   V_i <= \overline{V_i} */
		if(*(bd12+1)!=MAX_VARBND){
			   *p=*(bd12+1);    /* \overline{V_i} */
			   *(p+i+1)=1;    /* V_i */
			   p=p+fpp1->dim+1;
		   nchbnds++;
		}
    }

	/* Kohler's Rule: bits subset */
    /* Initialisation */
    checked_malloc(pr,bv,bit_vectors_t,1, return NULL;);
    bv->vsize=fpp12->ncons/*-nenvs*/;
    bv->nvecs=fpp12->ncons/*-nenvs*/;
    nchars=bv->vsize/8;
    if( (bv->vsize % 8)!=0 ) nchars++;
    bv->vchars=nchars;
    checked_malloc(pr,env1bits,char,bv->vchars, return NULL;);
    checked_malloc(pr,nenv1bits,char,bv->vchars, return NULL;);
    checked_malloc(pr,env2bits,char,bv->vchars, return NULL;);
    checked_malloc(pr,nenv2bits,char,bv->vchars, return NULL;);
    checked_malloc(pr,sigmabits,char,bv->vchars, return NULL;);
    fppol_initialize_bitVectors(pr,fpp1->ncons,fpp2->ncons,bv,env1bits,nenv1bits,
                                env2bits,nenv2bits,sigmabits,env1Index,env2Index);
    fpp12=fppol_reshape_steep_ineqs(pr,true,fpp12,bv);
#if  DEBUG_JOIN
    printf("fppol_strong_join before fm:: \n");
    fpp_fdump_withBV(stdout,fpp12,bv);
    fflush(stdout);
#endif
    /* project out y2, \sigma_1 */
	while(fpp12->dim > fpp1->dim){
			/* Sound Fourier-Motzkin Elimination */
		k=fm_select(fpp12,fpp1->dim+1,fpp12->dim);
		if(fpp12->dim>=2*fpp1->dim) fpp12=fm_elimination(pr, true, fpp12, k,bv,&subset);
		else fpp12=fm_elimination_lazy(pr, true, fpp12,k,bv,2*fpp1->dim+1-fpp12->dim,&subset);

		if(fpp12->ncons==0)	   break;
		   /* remove dimension */
		fpp12->dim--;
		if(fpp12->dim < fpp12->intdim) fpp12->intdim--;
		checked_malloc(pr,cons1,numdbl_t,fpp12->ncons*(fpp12->dim+1), return NULL;);
		for(j=0;j<fpp12->ncons;j++){
		  memcpy(cons1+j*(fpp12->dim+1),fpp12->cons+j*(fpp12->dim+2),k*sizeof(numdbl_t));
		  if(k<=fpp12->dim)
			memcpy(cons1+j*(fpp12->dim+1)+k,fpp12->cons+j*(fpp12->dim+2)+k+1,(fpp12->dim+1-k)*sizeof(numdbl_t));
		}
		checked_free(fpp12->cons);
		fpp12->cons=cons1;
		if(k<=fpp12->dim) memmove(fpp12->bnds+2*(k-1),fpp12->bnds+2*k,2*(fpp12->dim+1-k)*sizeof(numdbl_t));
		fpp12->bnds=realloc(fpp12->bnds,2*fpp12->dim*sizeof(numdbl_t));
			/* Redundancy Removal */
		if(fpp12->dim < 2*fpp1->dim){
			fpp12=redundancy_removal_quasi_syntactic_firstKs(pr,true,fpp12,fpp12->ncons/*-nenvs*/,bv);

			if(fpp12->ncons/*-nenvs*/ >= MINCONS4BDRR && fpp12->dim>fpp1->dim && fpp12->dim<2*fpp1->dim-1){
				/* using bounds info from fpp12->bnds */
				checked_malloc(pr,bnds1_12,numdbl_t,2*fpp12->dim, return NULL;);
				memcpy(bnds1_12,fpp1->bnds,2*fpp1->dim*sizeof(numdbl_t));
				memcpy(bnds1_12+2*fpp1->dim,fpp12->bnds+2*fpp1->dim,2*(fpp12->dim-fpp1->dim)*sizeof(numdbl_t));
				fpp12=redundancy_removal_firstKs_byBounds(pr,true,fpp12,bnds1_12,fpp12->ncons/*-nenvs*/,bv);

				memcpy(bnds1_12,fpp2->bnds,2*fpp2->dim*sizeof(numdbl_t));
				fpp12=redundancy_removal_firstKs_byBounds(pr,true,fpp12,bnds1_12,fpp12->ncons/*-nenvs*/,bv);
			    checked_free(bnds1_12);
		   }

		   if(fpp12->ncons/*-nenvs*/ >= MINCONS4LPRR && fpp12->dim>fpp1->dim){
				  /* fpp_fdump(stdout, pr->man, fpp12);fflush(stdout); */
				fpp12=fppol_reshape_ineqs(pr,true,fpp12,bv);
				fpp12=redundancy_removal_SLP_firstKs_withEnv(pr,true,fpp12,fpp12->ncons,env,nenvs,fpp1->dim,bv,true);
		   }
		}
    }
    if(fpp12->ncons>0){
       fpp12=redundancy_removal_firstKs_byBounds(pr,true,fpp12,fpp1->bnds,fpp12->ncons,bv);
       fpp12=redundancy_removal_firstKs_byBounds(pr,true,fpp12,fpp2->bnds,fpp12->ncons,bv);
    }
    fpp12->dim=fpp1->dim;
    fpp12->intdim=fpp1->intdim;
	/* tighten the convex hull using envelopes and bounds info */
    if(fpp12->ncons==0 && nenvs==0 && nchbnds==0 ){
        fpp12->flag=UNIVERSE_POL;
        checked_free(fpp12->cons);
        checked_free(fpp12->bnds);
        checked_free(env);
        checked_free(chbnds);
        checked_free(env1bits);
        checked_free(nenv1bits);
        checked_free(env2bits);
        checked_free(nenv2bits);
        checked_free(sigmabits);
        bv_free(bv);
        checked_free(env1Index);
        checked_free(env2Index);
        fpp_free_internal(pr, fpp1);
        fpp_free_internal(pr, fpp2);
        return fpp12;
    }
    if(fpp12->ncons==0){
    	checked_malloc(pr,fpp12->cons,numdbl_t,(nenvs+nchbnds)*(fpp12->dim+1), return NULL;);
        fpp12->bnds=bnds12_bak;
    }
    else{
        fpp12=fppol_reshape_ineqs(pr,true,fpp12,NULL);
        fpp12->cons=realloc(fpp12->cons,(fpp12->ncons+nenvs+nchbnds)*(fpp12->dim+1)*sizeof(numdbl_t));
        checked_free(bnds12_bak);
    }
    if(nenvs>0) memcpy(fpp12->cons+fpp12->ncons*(fpp12->dim+1),env,nenvs*(fpp12->dim+1)*sizeof(numdbl_t));
    if(nchbnds>0) memcpy(fpp12->cons+(fpp12->ncons+nenvs)*(fpp12->dim+1),chbnds,nchbnds*(fpp12->dim+1)*sizeof(numdbl_t));
    fpp12->ncons=fpp12->ncons+nenvs+nchbnds;
    fpp12->flag=GENERAL_POL;
       /* fpp12=redundancy_removal_firstKs(pr,true,fpp12,fpp12->ncons-nenvs-nchbnds,NULL,true); */
	   /* no need to quasi-syntactic redundancy removal */
       /* because no 0<= \sigma <= 1 in the constructed convex hull,thus no envelope */
    if(fpp12->ncons > nenvs+nchbnds)
        fpp12=redundancy_removal_SLP_firstKs(pr,true,fpp12,fpp12->ncons-nenvs-nchbnds,NULL,true);

	    /* remove the redundancy in the envelope,               */
        /* e.g., (0,0)|_| (1,1) results x=y;x>=0;y>=0;x<=1,y<=1 */
    fpp12=redundancy_removal_lastKs(pr,true,fpp12,nenvs+nchbnds,true);

#if DEBUG_JOIN || PRINT_JWRES
printf("fppol_strong_join result:: \n");
fpp_fdump(stdout, pr->man, fpp12);
fflush(stdout);
#endif
    checked_free(env);
    checked_free(chbnds);
    checked_free(env1bits);
    checked_free(nenv1bits);
    checked_free(env2bits);
    checked_free(nenv2bits);
    checked_free(sigmabits);
    bv_free(bv);
    checked_free(env1Index);
    checked_free(env2Index);
    fpp_free_internal(pr, fpp1);
    fpp_free_internal(pr, fpp2);
    return fpp12;
}


fpp_t *fppol_construct_convex_hull(fpp_internal_t* pr, fpp_t *fpp1, fpp_t *fpp2, numdbl_t *env, unsigned nenvs)
{
    fpp_t *fpp12; /* double dimension +1 */
    numdbl_t *p1,*p2,*p12;
    numdbl_t *bd1,*bd2,*q,*bd12;
    numdbl_t *res;
    numdbl_t *penv;
    unsigned i,j;
    int r;
    numdbl_t *c,*bSig;
    numdbl_t dk,maxadbk,dkInt,dkDif,mi;
    bool reshape;

    fpp12=fpp_alloc_urgent(pr,2*fpp1->dim+1,fpp1->intdim+fpp2->intdim,
			   fpp1->ncons+   /* A_1*y_1-b_1*\sigma_1 <= 0 */
               fpp2->ncons+/*+  A_2*x-A_2*y_1+b_2*\sigma_1 <= b_2 */
			   2);/*+  bounds for (\sigma_1) */
    /*--------------------------------------------------*/
    /* bounds (x y1 \sigma_1) for fpp12                  */
    /*--------------------------------------------------*/
    bd12=fpp12->bnds;
    bd1=fpp1->bnds;
    bd2=fpp2->bnds;
    /* bounds for (x y1 \sigma_1) */
    	/*-- bounds for x= y1+y2 =\sigma_1*x1+(1-\sigma_1)*x2 --*/
    	/*---- upper bound: x<= max{x1,x2}  ----*/
    	/*---- lower bound: x>= min{x1,x2}  ----*/


    for(i=0;i<fpp1->dim;i++){
	if( *(bd1+2*i)==-MAX_VARBND || *(bd2+2*i)==-MAX_VARBND )
		*(bd12+2*i)=-MAX_VARBND;
	else    *(bd12+2*i)  =fmin( *(bd1+2*i), *(bd2+2*i) );
	if( *(bd1+2*i+1)==MAX_VARBND || *(bd2+2*i+1)==MAX_VARBND )
		*(bd12+2*i+1)=MAX_VARBND;
	else *(bd12+2*i+1)=fmax( *(bd1+2*i+1), *(bd2+2*i+1) );
    }
    bd12=bd12+2*fpp1->dim;
    /*-- bounds for y1=(\sigma_1)*x1 --*/
    /*---- if \overline{x1}>0    \overline{y1}=\overline{x1}   ----*/
	/*---- if \overline{x1}<=0   \overline{y1}=0 ----*/
	/*---- if \underline{x1}>=0  \underline{y1}=0 ----*/
	/*---- if \underline{x1}<0   \underline{y1}=\underline{x1} ----*/
    for(i=0;i<fpp1->dim;i++){
	if(*bd1>=0)    *bd12=0;
	else          *bd12=*bd1;
	bd12++;
	bd1++;
	if(*bd1>0)     *bd12=*bd1;
	else          *bd12=0;
	bd12++;
	bd1++;
    }
    	/* bounds for \sigma_1: [0,1] */
    *(bd12)=0;
    *(bd12+1)=1;
    /*--------------------------------------------*/
    /* Constraints for fpp12                      */
    /*--------------------------------------------*/
    p12=fpp12->cons;
	/* Constraints from A_1: $A_1*y_1 -b_1*\sigma_1 \leq 0$ from fpp1 */
    p1=fpp1->cons;
    for(i=0;i<fpp1->ncons;i++){
	*p12=0;  /* c: 0 */
	p12++;
	memset(p12,0,fpp1->dim*sizeof(numdbl_t));    /* x: 0 */
	p12=p12+fpp1->dim;
	memcpy(p12,p1+1,fpp1->dim*sizeof(numdbl_t)); /* A_1*y_1 */
	p12=p12+fpp1->dim;
	*p12=-(*p1);  /* -b_1*\sigma_1 */
	p12++;
	p1=p1+fpp1->dim+1;
    }

	/* Constraints from A_2: $A_2*x - A_2*y1+b_1*\sigma_1 \leq b_2$ from fpp2 */
    p2=fpp2->cons;
    for(i=0;i<fpp2->ncons;i++){
	*p12=*p2; /* b_2 */
	*(p12+1+2*fpp2->dim)=*p2;  /* b_2 *\sigma_1 */
	p12++;
	p2++;
	memcpy(p12,p2,fpp2->dim*sizeof(numdbl_t));   /* A_2*x */
	p12=p12+fpp2->dim;
	for(j=0;j<fpp2->dim;j++){  /* -A_2*y_1 */
	   *p12=-(*p2);
	   p12++;
	   p2++;
	}
	p12++;  /* space for \sigma_1 (has already been set ) */
    }
    	/* Constraints from bounds */
    memset(p12,0,2*(fpp12->dim+1)*sizeof(numdbl_t));
    bd12=fpp12->bnds+2*2*fpp1->dim;
     	  /* lower bound:   -V_i <= -\underline{V_i} */
    *p12=-(*bd12);    /* -\underline{V_i} */
    *(p12+fpp12->dim)=-1;    /* -V_i */
    p12=p12+fpp12->dim+1;
	  /* upper bound:   V_i <= \overline{V_i} */
    *p12=*(bd12+1);    /* \overline{V_i} */
    *(p12+fpp12->dim)=1;      /* V_i */
    p12=p12+fpp12->dim+1;

    return fpp12;
}

bool fppol_factorization(fpp_internal_t* pr, fpp_t *fpp01, fpp_t *fpp02,  fpp_t *fpp1, fpp_t *fpp2)
{
   bool flag;
   unsigned i,j;
   numdbl_t *chbnds,*p,*bd1,*bd2;
   bool subset;

   flag=false;
	/* dimension reduction: just project out the vars without removing dimension */
   for(i=0;i<fpp1->dim;i++){
	bd1=fpp1->bnds+2*i;
	bd2=fpp2->bnds+2*i;
	if( *bd1==-MAX_VARBND &&  *(bd1+1)==MAX_VARBND &&
           (*bd2!=-MAX_VARBND || *(bd2+1)!=MAX_VARBND)  ){
   	    for(j=0;j<fpp1->ncons;j++){
		if(*(fpp1->cons+j*(fpp1->dim+1)+i+1)!=0){
		   break;
		}
	    }
	    if(j==fpp1->ncons){
	        /* project out i-th var in Pol2 */
			/* fpp2=fppol_project(pr,true,fpp2, i+1); */
		flag=true;
		fpp2=fm_elimination(pr,true,fpp2, i+1,NULL,&subset);
		if(fpp2->flag!=GENERAL_POL)  break;
		*bd2=-MAX_VARBND;
		*(bd2+1)=MAX_VARBND;
		if(subset!=true)  fpp2=redundancy_removal(pr,true,fpp2,true);
		if(fpp2->flag!=GENERAL_POL)  break;
	    }

	}
	else if( *bd2==-MAX_VARBND &&  *(bd2+1)==MAX_VARBND &&
            (*bd1!=-MAX_VARBND || *(bd1+1)!=MAX_VARBND)  ){
   	    for(j=0;j<fpp2->ncons;j++){
		if(*(fpp2->cons+j*(fpp2->dim+1)+i+1)!=0){
		   break;
		}
	    }
	    if(j==fpp2->ncons){
	        /* project out i-th var in Pol1 */
                /* fpp1=fppol_project(pr,true,fpp1, i+1);	*/
		flag=true;
                fpp1=fm_elimination(pr,true,fpp1, i+1,NULL,&subset);
		if(fpp1->flag!=GENERAL_POL)  break;
		*bd1=-MAX_VARBND;
		*(bd1+1)=MAX_VARBND;
		if(subset!=true)  fpp1=redundancy_removal(pr,true,fpp1,true);
		if(fpp1->flag!=GENERAL_POL)  break;
	    }
	}
   }
   return flag;
}

void fppol_initialize_bitVectors(fpp_internal_t* pr,unsigned ncons1,unsigned ncons2, bit_vectors_t *bv,
 char *env1bits, char *nenv1bits,char *env2bits, char *nenv2bits,char *sigmabits, char *env1Index,char *env2Index )
{

   unsigned i,j,nshift,nrem;
   char *bits,*bitVector;
   int superset;
   int ni;

	/* Kohler's Rule: bits subset */
    /* Initialisation */
    checked_malloc(pr,bv->bits,char,bv->nvecs*bv->vchars, return ;);
    memset(bv->bits,0,bv->nvecs*bv->vchars*sizeof(char));
    for(i=0;i<bv->nvecs;i++){
		bitVector=bv->bits+i*bv->vchars;
		nshift=i/8;
		nrem=i % 8;
		if(nrem==0) *(bitVector+nshift)=0x01;
		else *(bitVector+nshift)=0x01<<nrem;
    }
	  /*bit vector for envelope and non-envelope*/
    /* P1 */
    memset(env1bits,0,bv->vchars*sizeof(char));
    memset(nenv1bits,0,bv->vchars*sizeof(char));
    for(i=0;i<ncons1;i++){
		nshift=i/8;
		nrem=i % 8;
		if(*(env1Index+i)==1){ /* envelope */
		   if(nrem==0) *(env1bits+nshift)=0x01 | *(env1bits+nshift);
		   else *(env1bits+nshift)=0x01<<nrem  | *(env1bits+nshift);
		}
		else{   /* non- envelope */
		   if(nrem==0) *(nenv1bits+nshift)=0x01 | *(nenv1bits+nshift);
		   else *(nenv1bits+nshift)=0x01<<nrem  | *(nenv1bits+nshift);
		}
    }
    /* P2 */
    memset(env2bits,0,bv->vchars*sizeof(char));
    memset(nenv2bits,0,bv->vchars*sizeof(char));
    for(i=0;i<ncons2;i++){
	    nshift=(i+ncons1)/8;
        nrem=(i+ncons1) % 8;
        if(*(env2Index+i)==1){ /* envelope */
	       if(nrem==0) *(env2bits+nshift)=0x01 | *(env2bits+nshift);
           else *(env2bits+nshift)=0x01<<nrem  | *(env2bits+nshift);
	    }
        else{   /* non- envelope */
	       if(nrem==0) *(nenv2bits+nshift)=0x01| *(nenv2bits+nshift);
           else *(nenv2bits+nshift)=0x01<<nrem | *(nenv2bits+nshift);
	    }
    }
    /* sigma */
    memset(sigmabits,0,bv->vchars*sizeof(char));
    for(i=ncons1+ncons2;i<bv->nvecs;i++){
        nshift=i/8;
        nrem=i% 8;
        if(nrem==0) *(sigmabits+nshift)=0x01 | *(sigmabits+nshift);
        else *(sigmabits+nshift)=0x01<<nrem | *(sigmabits+nshift);
    }
    return;
}

fpp_t *fppol_reshape_ineqs(fpp_internal_t* pr, bool destructive, fpp_t *fpp, bit_vectors_t* bv)
{
    fpp_t *fpp1; /* double dimension +1 */
    unsigned i;
    int r;
    numdbl_t *c,*bSig;
    numdbl_t dk,maxadbk,dkInt,dkDif,mi;
    bool reshape;

   if(destructive==false)
	fpp1=fpp_copy_internal(pr, fpp);
   else	fpp1=fpp;

   reshape=false;
   for(i=0;i<fpp1->ncons;i++){
      r=normalise_steep_inequality(fpp1->dim,fpp1->cons+i*(fpp1->dim+1),fpp1->bnds);
      if(r==FAILED){
		 if(fpp1->ncons-i-1>0){
			   memmove(fpp1->cons+i*(fpp1->dim+1),fpp1->cons+(i+1)*(fpp1->dim+1),(fpp1->ncons-i-1)*(fpp1->dim+1)*sizeof(numdbl_t));
		 }
		 else
		   fpp1->cons=(numdbl_t*)realloc(fpp1->cons,(fpp1->ncons-1)*(fpp1->dim+1)*sizeof(numdbl_t) );
		 fpp1->ncons--;
		 if(bv!=NULL){
			if(bv->nvecs-i-1>0)
				memmove(bv->bits+i*bv->vchars,bv->bits+(i+1)*bv->vchars,(bv->nvecs-i-1)*bv->vchars*sizeof(char));
			else
				bv->bits=(char *)realloc(bv->bits,(bv->nvecs-1)*bv->vchars*sizeof(char));
			bv->nvecs=bv->nvecs-1;
		 }
      }
      else{
		 if(r!=UNCHANGED)  reshape=true;
			 r=normalise_quasiInteger_inequality(fpp1->dim,fpp1->cons+i*(fpp1->dim+1),fpp1->bnds);
		 if(r!=UNCHANGED)  reshape=true;
      }
   }
   if(reshape==true) fpp1=redundancy_removal_quasi_syntactic_firstKs(pr,true,fpp1,fpp1->ncons,bv);
#if  DEBUG_JOIN
    fprintf(stdout,"\n Pol after fppol_reshape_ineqs:  \n");
    fpp_fdump(stdout, pr->man, fpp1);
    fflush(stdout);
#endif
    return fpp1;
}


fpp_t *fppol_reshape_steep_ineqs(fpp_internal_t* pr, bool destructive, fpp_t *fpp, bit_vectors_t* bv)
{
    fpp_t *fpp12; /* double dimension +1 */
    unsigned i;
    int r;
    numdbl_t *c,*bSig;
    numdbl_t dk,maxadbk,dkInt,dkDif,mi;
    bool reshape;

   if(destructive==false)
	fpp12=fpp_copy_internal(pr, fpp);
   else	fpp12=fpp;

    /* the steep inequality is often caused by the tiny or huge coefficient of \Sigma, */
    /* that is, b1 or b2. */
    /* So we can reshape the ugly coefficient of \Sigma to integer(often is 0) */
	for(i=0;i<fpp12->ncons;i++){
			/* steep inequality normalisation (when b1 or b2 is very tiny or huge) */
		r=normalise_steep_inequality(fpp12->dim,fpp12->cons+i*(fpp12->dim+1),fpp12->bnds);
		if(r==FAILED){
			/*   fprintf(stdout,"\n normalise_steep_inequality: %d-th failed so removed \n",i);
		   fflush(stdout);  */
		   memmove(fpp12->cons+i*(fpp12->dim+1),fpp12->cons+(i+1)*(fpp12->dim+1),
					  (fpp12->ncons-i-1)*(fpp12->dim+1)*sizeof(numdbl_t));
		   fpp12->ncons--;
		   if(bv!=NULL){
			 memmove(bv->bits+i*bv->vchars,bv->bits+(i+1)*bv->vchars,(bv->nvecs-i-1)*bv->vchars*sizeof(char));
			 bv->nvecs=bv->nvecs-1;
		   }
		   i--;
		   continue;
		}
		else if(r==SUCCEED){
			  /* fprintf(stdout,"\n normalise_steep_inequality: %d-th succeeded \n",i); */
		}
			/* reshape the ugly coefficient of \Sigma to integer(often is 0) */
		bSig=fpp12->cons+i*(fpp12->dim+1)+fpp12->dim;
		dk=*bSig;
		dkInt=round(dk);
		reshape=false;
		if(dk!=dkInt){
		   dkDif=fabs(dk-dkInt);
		   if( (dkInt == 0) && (dkDif<=QSEPSILON) ){
			/* align to 0.0 */
			  dk=0.0;
			  reshape=true;
		   }
		   else{
			  mi=fmax(fabs(dk),fabs(dkInt));
			  if(dkDif/mi<=QSEPSILON){ /* align to integer */
				 dk=dkInt;
				 reshape=true;
			  }
		   }
		}
		if(reshape==true){
		   if (dk==0) dk=0.0;  /* ensure there is no -0.0 as a coefficient */
			/* compute ck */
		   maxadbk=fabs(dk-*bSig);
		   c=fpp12->cons+i*(fpp12->dim+1);
		   *c=*c+maxadbk*1; /* the upperbound for /Sigma is always 1 */
		   *bSig=dk;
		}
    }
#if DEBUG_JOIN
    fprintf(stdout,"\n Pch after normalise_steep_inequality:  \n");
    fpp_fdump(stdout, pr->man, fpp12);
    fflush(stdout);
#endif
    return fpp12;
}

/* ==================================== */
/* Affiliated Operations                */
/* ==================================== */

/* vector compare: equal returns 0, otherwise 1 */
int vector_cmp(numdbl_t *l1, numdbl_t *l2, int len)
{
   int i;
   numdbl_t a1,a2;
   for(i=0;i<len;i++){
     a1=*(l1+i);
     if(a1==0) a1=0.0;
     a2=*(l2+i);
     if(a2==0) a2=0.0;
     if( a1!=a2 ){
	return 1;
     }
   }
   return 0;
} /* vector_cmp */

/* Envelope of 2 Polyhedra                            */
/*      for Polyhedral Join(Convex Hull)              */
/* $$Env(P1,P2)={l \in P1 | P2\sqsubseteq l} \cup {l \in P2 | P1\sqsubseteq l}$$ */
/* result *superset: 2 ,if P1 \sqsubseteq P2        */
/*        *superset: 1 ,else if P2 \sqsubseteq P1   */
/*        *superset: 0 ,otherwise                   */
/* reverse fpp1 and fpp2 because it is often the case that fpp1 may be larger fpp2 */
numdbl_t *fppol_envelope(fpp_t *fpp2, fpp_t *fpp1, unsigned *NbEnvs, int *superset,char *env2Index,char *env1Index)
{
    unsigned i,j;
    numdbl_t c,ci,cb;
    numdbl_t *res;
    numdbl_t *env,*env1,*env2,*p1,*p2,*l1,*l2;
    unsigned nEnv,nEnv1,nEnv2,qsnEnv1,qsnEnvEQ2;
    glp_prob *lp;
    int lpres;
    numdbl_t q;
    unsigned qsenv1Index[200];
    int flag;
    bool nearly;

    env2=(numdbl_t *) malloc((fpp2->ncons)*(fpp2->dim+1)*sizeof(numdbl_t));
    p2=env2;
    env1=(numdbl_t *) malloc((fpp1->ncons)*(fpp1->dim+1)*sizeof(numdbl_t));
    p1=env1;
    nEnv1=0;
    nEnv2=0;
    qsnEnv1=0;
    qsnEnvEQ2=0;
    *superset=0;
    lp=rlp_create_matrix(fpp1);
    for(i=0;i<fpp2->ncons;i++){ /* {l \in P2 | P1\sqsubseteq l} */
        flag=0;
	l2=fpp2->cons+i*(fpp2->dim+1);
	/* Quasi-Syntactic check */
	for(j=0;j<fpp1->ncons;j++){ /* l1: e<=c1 ; l2: e<=c2 */
	   l1=fpp1->cons+j*(fpp1->dim+1);
	   if( vector_cmp(l1+1,l2+1,fpp2->dim)==0 ){
              if(*l1==0)  *l1=0.0;
	      if(*l2>*l1){ /* keep l2 in the envelope */
	         flag=1;
	         memcpy(p2,l2,(fpp2->dim+1)*sizeof(numdbl_t));
	         p2=p2+(fpp2->dim+1);
	         nEnv2++;
		 *(env2Index+i)=1;
	         break;
	      }
	      else{   /* keep l1 in the envelope */
		 if(*l1==*l2){
		     qsnEnvEQ2++;
		     *(env2Index+i)=1;
                 }
	         memcpy(p1,l1,(fpp1->dim+1)*sizeof(numdbl_t));
	         p1=p1+(fpp1->dim+1);
		 qsenv1Index[qsnEnv1]=j;
	         qsnEnv1++;
	         flag=1;
		 *(env1Index+j)=1;
	      }
	   }
	}
	if(flag==0){
	   if(lp==NULL)  lp=rlp_create_matrix(fpp1);
           rlp_set_objective(lp,GLP_MAX,l2+1);
    	   lp=rlp_solve(lp);
           ci=rlp_get_optimal_value(lp,fpp1->bnds,false,&nearly);
	   if(nearly==false){
          numitv_t itv1 = numitv_lexp_range(fpp1->dim,l2+1,fpp1->bnds);
          cb = itv1.sup;
	      if(cb<ci)  ci=cb;
	   }
	   c=*l2;
	   if(ci<=c){  /* P2\sqsubseteq l */
	      /* keep i-th linear constraint from fpp1 in the envelope */
	      memcpy(p2,l2,(fpp2->dim+1)*sizeof(numdbl_t));
	      p2=p2+(fpp2->dim+1);
	      nEnv2++;
	      *(env2Index+i)=1;
           }
		}
    }
    if(nEnv2+qsnEnvEQ2==fpp2->ncons){
		*superset=1;  /* 2 */
		lp_delete(lp);
		checked_free(env2);
		checked_free(env1);
		return NULL;
    }
    if(qsnEnv1==fpp1->ncons){
		*superset=2;   /* 1 */
		lp_delete(lp);
    	checked_free(env1);
    	checked_free(env2);
    	return NULL;
    }

    nEnv1=qsnEnv1;
    if(lp!=NULL) lp_delete(lp);
    lp= rlp_create_matrix(fpp2);
    for(i=0;i<fpp1->ncons;i++){ /* {l \in P2 | P1\sqsubseteq l} */
        flag=0;
        l1=fpp1->cons+i*(fpp1->dim+1);
		/* already in the previous Quasi-Syntactic check results? */
        if(qsnEnv1>0){
		   for(j=0;j<qsnEnv1;j++){ /* l1: e<=c1 ; l2: e<=c2 */
			if(i==qsenv1Index[j]){
				flag=1;
				break;
			}
		   }
        }
		if(flag==0){
		   if(lp==NULL)  lp=rlp_create_matrix(fpp2);
		   rlp_set_objective(lp,GLP_MAX,l1+1);
		   lp=rlp_solve(lp);
		   ci=rlp_get_optimal_value(lp,fpp2->bnds,false,&nearly);
		   if(nearly==false){
			  numitv_t itv1 = numitv_lexp_range(fpp2->dim,l1+1,fpp2->bnds);
			  cb = itv1.sup;
			  if(cb<ci)  ci=cb;
		   }
		   c=*l1;
		   if(ci<=c){  /* P1\sqsubseteq l */
			  memcpy(p1,l1,(fpp1->dim+1)*sizeof(numdbl_t));
			  p1=p1+(fpp1->dim+1);
		      nEnv1++;
		      *(env1Index+i)=1;
		   }
		}
    }
    if(nEnv1==fpp1->ncons){
		*superset=2;   /* 1 */
		lp_delete(lp);
			checked_free(env1);
		checked_free(env2);
		return NULL;
    }
    lp_delete(lp);
    nEnv=nEnv1+nEnv2;
    if(nEnv==0){
        *NbEnvs=0;
        checked_free(env1);
        checked_free(env2);
        return NULL;
    }
    env1=(numdbl_t *) realloc(env1,nEnv*(fpp1->dim+1)*sizeof(numdbl_t));
    env=env1;
    p1=env+nEnv1*(fpp1->dim+1);
    for(i=0;i<nEnv2;i++){
		l2=env2+i*(fpp2->dim+1);
			flag=0;
		/* Quasi-Syntactic check */
		for(j=0;j<nEnv1;j++){
		   l1=env1+j*(fpp1->dim+1);
		   if( vector_cmp(l1+1,l2+1,fpp2->dim)==0 ){
			flag=1;
			nEnv--;
				break;
		   }
		}
		if(flag==0){
		   memcpy(p1,l2,(fpp1->dim+1)*sizeof(numdbl_t));
		   p1=p1+(fpp1->dim+1);
		}
    }
    *NbEnvs=nEnv;
    checked_free(env2);
    env=(numdbl_t *) realloc(env,nEnv*(fpp1->dim+1)*sizeof(numdbl_t));
    return env;
}


/* Envelope from fpp1 with respect to fpp2   */
/*      for Polyhedral Widening              */
/* $${l \in P1 | P2\sqsubseteq l}$$ */
numdbl_t *fppol_envelope_half(fpp_t *fpp1, fpp_t *fpp2, unsigned *NbEnvs, unsigned *NbQuasi,
char *envIndex)
{
    unsigned quasiInd[1000],nquasi;
    unsigned i,j,flag;
    glp_prob *lp;
    int lpres;
    numdbl_t *p,*env,*l1,*l2;
    numdbl_t c,ci,cb;
    unsigned nEnv;
    bool nearly;

    env=p=(numdbl_t *) malloc( (fpp1->ncons)*(fpp1->dim+1)*sizeof(numdbl_t) );
    nEnv=0;
    nquasi=0;
    memset(quasiInd,0,(fpp2->ncons+1)*sizeof(unsigned));
	/* Quasi Envelope */
    for(i=0;i<fpp1->ncons;i++){ /* {l \in P1 | P2\sqsubseteq l} */
		l1=fpp1->cons+i*(fpp1->dim+1);
		for(j=0;j<fpp2->ncons;j++){ /* {l \in P1 | P2\sqsubseteq l} */
		   l2=fpp2->cons+j*(fpp2->dim+1);
		   if( vector_cmp(l1+1,l2+1,fpp2->dim)==0 ){
			if(*l1==0)	*l1=0.0;
			if(*l1 >= *l2 ){
			   memcpy(p,l1,(fpp1->dim+1)*sizeof(numdbl_t));
			   p=p+(fpp1->dim+1);
			   nEnv++;
			   nquasi++;
			   quasiInd[nquasi]=i;
			   if( envIndex!=NULL )  *(envIndex+i)=1;
			}
			break;
		   }
		}
		if(nquasi >= fpp2->ncons){
		   break;
		}
    }
    *NbQuasi=nquasi;

    lp= rlp_create_matrix(fpp2);
    for(i=0;i<fpp1->ncons;i++){ /* {l \in P1 | P2\sqsubseteq l} */
        flag=0;
		l1=fpp1->cons+i*(fpp1->dim+1);
		/* already in Quasi Envelope? */
		for(j=1;j<=nquasi;j++){
		   if(i==quasiInd[j]){
				flag=1;
				break;
		   }
		}
		if(flag==0){
		   if(lp==NULL)  lp=rlp_create_matrix(fpp2);
			   rlp_set_objective(lp,GLP_MAX,l1+1);
			   lp=rlp_solve(lp);
			   ci=rlp_get_optimal_value(lp,fpp2->bnds,false,&nearly);
		   if(nearly==false){
			  numitv_t itv1 = numitv_lexp_range(fpp2->dim,l1+1,fpp2->bnds);
			  cb = itv1.sup;
			  if(cb<ci)  ci=cb;
		   }
		   c=*l1;
		   if(ci<=c){  /* P2\sqsubseteq l */
			  memcpy(p,l1,(fpp1->dim+1)*sizeof(numdbl_t));
			  p=p+(fpp1->dim+1);
			  nEnv++;
			  if( envIndex!=NULL )  *(envIndex+i)=1;
		   }
		}
    }
    lp_delete(lp);
    *NbEnvs=nEnv;
    if(nEnv==0){
		checked_free(env);
		return NULL;
    }
    env=(numdbl_t *) realloc(env, nEnv*(fpp1->dim+1)*sizeof(numdbl_t) );
    return env;
}

/*  Incrementally infeasiblility test by Rigorous Linear Programming	*/
int fppol_infeasibility_byRLP(fpp_internal_t* pr, fpp_t *fpp)
{
    unsigned i,k;
    unsigned NbCons;
    fpp_t *fpp1;
    numdbl_t *aCons,*l1,*l2;
    numdbl_t t,c1,c2,a1k,a2k;
    numdbl_t a1k_0,a2k_0;
    numdbl_t c,cmin,cb;
    int res;
    int pos;
    glp_prob *lp;
    int lpres;
    bool nearly;

    fpp1=fpp_copy_internal(pr, fpp);
    NbCons=fpp->ncons;
    if(NbCons==1)    return 0;
	else if(NbCons==2) {  /* a pair of opposite parallel lines? */
		l1=fpp->cons;
		l2=fpp->cons+(fpp->dim+1);
		k=0;
		while( k<fpp->dim ){
		  k++;
		  if(*(l1+k)!=0) /* the first non-zero coeffecient of line 1 */
			break;
		  if(*(l2+k)!=0)  /* must equal to zero to match line 1 */
			return 0;
		}
		if( k == fpp->dim){ /* 0.0 <= b */
		  if(*l1<0 || *l2<0 )	return 1;
		  else return 0;
		}
		a1k=*(l1+k);
		a2k=*(l2+k);
		if(a1k*a2k>-0.0){ /* with the same sign */
		   return 0;
		}
		t=a1k/a2k;
		a1k_0=a1k;
		a2k_0=a2k;
		while(k<fpp->dim){
		   k=k+1;
		   a1k=*(l1+k);
		   a2k=*(l2+k);
		   if( (a1k!=t*a1k) || (a1k!=num_mul_downward(t,a1k)) ){
			  return 0;
		   }
		}
		c1=*l1;
		c2=*l2;
		if(a1k_0>0){
		   if(t*c2>c1)
			  return 0;
		   else return 1;
		}
		else{
		   if(t*c2>c1)
			  return 1;
		   else return 0;
		}
    }
    for(i=2;i<NbCons;i++){
		fpp1->ncons=i;
		aCons=fpp->cons+i*(fpp->dim+1);
			c=*aCons;
			aCons++;
			lp=rlp_create(fpp1,GLP_MIN,aCons);
			lp=rlp_solve(lp);
		if(lp==NULL)  return dntknow;
			lpres=glp_get_status(lp);
		if(lpres==GLP_OPT){
		   cmin=rlp_get_optimal_value(lp,fpp->bnds,false,&nearly);
		   if(nearly==false){
			  numitv_t itv1 = numitv_lexp_range(fpp->dim,aCons+1,fpp->bnds);
			  cb = itv1.inf;
			  if(cb>cmin)  cmin=cb;
		   }
		   if(cmin>c){
			  lp_delete(lp);
			  return 1;
		   }
		 }
		 lp_delete(lp);
    }
    fpp_free_internal(pr,fpp1);
    return dntknow;
}

/*   Equality Test by syntactical check     	*/
bool fppol_is_eq_bySyntactic(fpp_t *fpp1, fpp_t *fpp2)
{
    int flag;
    numdbl_t *l1,*l2;
    unsigned i,j;

    if( fpp1->flag!=fpp2->flag     || fpp1->dim!=fpp2->dim ||
        fpp1->intdim!=fpp2->intdim || fpp1->ncons!=fpp2->ncons )
	return false;
    for(i=0;i<fpp1->ncons;i++){
	flag=false;
	l1=fpp1->cons+i*(fpp1->dim+1);
	for(j=0;j<fpp2->ncons;j++){
	  l2=fpp2->cons+j*(fpp2->dim+1);
	  if( vector_cmp(l1,l2,fpp1->dim+1) ==0 ){
	     flag=true;  break;
      }
	}
	if(flag==false) return false;
    }
    return 1;
}

/*   Inclusion Test by syntactical check     	*/
bool fppol_is_leq_bySyntactic(fpp_t *fpp1, fpp_t *fpp2)
{
    int flag;
    numdbl_t *l1,*l2;
    unsigned i,j,n;
    n=0;
	for(i=0;i<fpp2->ncons;i++){
		l2=fpp2->cons+i*(fpp2->dim+1);
		for(j=0;j<fpp1->ncons;j++){
		  l1=fpp1->cons+j*(fpp1->dim+1);
		  if( vector_cmp(l1+1,l2+1,fpp1->dim) ==0 ){
			 if(*l1 > *l2)  return false;
			 else{
				n++;
				break;
			 }
		  }
		}
    }
    if(n==fpp2->ncons) return true;
    else return dntknow;
}

/* Inclusion Test (Entailment)  fpp1 \sqsubseteq  fpp2          	*/
/*  implemented by bound check             		*/
int fppol_inclusion_byBound(fpp_t *fpp1, fpp_t *fpp2)
{
    int flag;
    numdbl_t lbnd1,ubnd1,lbnd2,ubnd2;
    unsigned i;

    flag=dntknow; /* with the same bounds */
    for(i=0;i<fpp1->dim;i++){
        lbnd1=*(fpp1->bnds+2*i);
        lbnd2=*(fpp2->bnds+2*i);
        ubnd1=*(fpp1->bnds+2*i+1);
        ubnd2=*(fpp2->bnds+2*i+1);
        if(lbnd1 < lbnd2 || ubnd1 > ubnd2)
        	return false;
    }
    return flag;
}


/* Inclusion Test (Entailment)  fpp1 \sqsubseteq  fpp2          	*/
/*  implemented by rigorous linear programming             		*/
/*
$\forall \varphi\in P_2. P_1\models \varphi$.
For each inequality $\varphi$ in $P_2$, such as $\sum_i a_i V_i \leq b$,
posing the rigorous linear programming,
compute \textup{$\mu$= maximize $\sum_i a_i V_i$ subject to $P_1$}.
If $\mu\leq b$ then $P_1\models \varphi$ holds.
*/
int fppol_inclusion_byRLP(fpp_t *fpp1, fpp_t *fpp2)
{
    unsigned i,j;
    numdbl_t *aCons2,*ei,*l1;
    numdbl_t mu,b,cb;
    glp_prob *lp;
    int lpres;
    int r;
    bool nearly;

    if(fpp1->flag==EMPTY_POL || fpp2->flag==UNIVERSE_POL)  return true;
    else if(fpp2->flag==EMPTY_POL)    return false;
    r=fppol_inclusion_byBound(fpp1, fpp2);
    if(r==false) return r;
    r=fppol_is_leq_bySyntactic(fpp1, fpp2);
    if(r!=dntknow) return r;
    lp=rlp_create_matrix(fpp1);
    for(i=0;i<fpp2->ncons;i++){
		aCons2=fpp2->cons+i*(fpp2->dim+1);
		for(j=0;j<fpp1->ncons;j++){
		  l1=fpp1->cons+j*(fpp1->dim+1);
		  if( vector_cmp(l1+1,aCons2+1,fpp1->dim) ==0 )
			break;
		}
		if(j<fpp1->ncons) continue;
		b=*aCons2;
		ei=aCons2+1;
		if(lp==NULL) lp=rlp_create_matrix(fpp1);
		rlp_set_objective(lp,GLP_MAX,ei);
			lp=rlp_solve(lp);
			if(lp==NULL) return dntknow;
			lpres=glp_get_status(lp);
			if(lpres!=GLP_OPT){
				fprintf(stdout,"\n Warning:fpp_inclusion_byRLP(): Dual Problem glp_get_status(lp)!=GLP_OPT!");
				lp_delete(lp);
			return dntknow;
		}
		else{
		   mu=rlp_get_optimal_value(lp,fpp1->bnds,false,&nearly);
		   if(nearly==false){
			  numitv_t itv1 = numitv_lexp_range(fpp1->dim,aCons2+1,fpp1->bnds);
			  cb = itv1.sup;
			  if(cb<mu)  mu=cb;
		   }
		   if(mu>b){
			  lp_delete(lp);
			  return false;
		   }
		}
    }
    lp_delete(lp);
    return true;
}

/* Inclusion Test (Entailment)  fpp1 \sqsubseteq  fpp2          	*/
/*  implemented by epsilon linear programming             		*/
/*
$\forall \varphi\in P_2. P_1\models \varphi$.
For each inequality $\varphi$ in $P_2$, such as $\sum_i a_i V_i \leq b$,
posing the epsilon linear programming,
compute \textup{$\mu$= maximize $\sum_i a_i V_i$ subject to $P_1$}.
If $\mu\leq b$ then $P_1\models \varphi$ holds.
*/
int fppol_inclusion_byEpsilonLP(fpp_t *fpp1, fpp_t *fpp2)
{
    unsigned i,j;
    glp_prob *lp;
    numdbl_t *aCons2,*ei,*l1;
    numdbl_t mu,b;
    numdbl_t maxbmu,mu_b;
    int lpres;
    int r;

    if(fpp1->flag==EMPTY_POL || fpp2->flag==UNIVERSE_POL)  return true;
    else if(fpp2->flag==EMPTY_POL)    return false;
    else if(fpp1->flag==UNIVERSE_POL && fpp2->flag!=UNIVERSE_POL)    return false;
    r=fppol_inclusion_byBound(fpp1, fpp2);
    if(r==false) return r;
    r=fppol_is_leq_bySyntactic(fpp1, fpp2);
    if(r!=dntknow) return r;
    /* constraint inclusion */
    lp= slp_create_matrix(fpp1);
	for(i=0;i<fpp2->ncons;i++){
		aCons2=fpp2->cons+i*(fpp2->dim+1);
		for(j=0;j<fpp1->ncons;j++){
		  l1=fpp1->cons+j*(fpp1->dim+1);
		  if( vector_cmp(l1+1,aCons2+1,fpp1->dim) ==0 )
			break;
		}
		b=*aCons2;
		ei=aCons2+1;
		slp_set_objective(lp,GLP_MAX,ei);
		lp=slp_solve(lp,NULL);
		if(lp==NULL){
		  lp_delete(lp);
		  return dntknow;
		}
		lpres=glp_get_status(lp);
		if(lpres==GLP_OPT){
			mu=slp_get_optimal_value(lp);
			if(mu>b){
			  maxbmu=fmax(-b,mu);
			  mu_b=mu-b;
			  if( mu_b/maxbmu > EPSILON){
				lp_delete(lp);
			    return false;
			  }
			}
		}
		else{
			if(lpres==GLP_UNBND){
				lp_delete(lp);
				return false;
			}
		}
    }
    lp_delete(lp);
    return true;
}


/* Check whether a constraint ( c >= e ) is entailed by a polyhedron    */
/* posing Rigorous Linear Programming to get a sound result maxc=max(e) */
/* If maxc <= c, then entailment happens.                               */
bool fppol_entailment(fpp_internal_t* pr,fpp_t *fpp, numdbl_t *aCons, bool recalBnds)
{
  glp_prob *lp;
  numdbl_t c,cmax,cb;
  int lpres;
  bool nearly;
  numdbl_t *obnds=NULL;
  unsigned i;

  c=*aCons;
  lp= rlp_create(fpp,GLP_MAX,aCons+1);
  lp= rlp_solve(lp);
  if(lp!=NULL){
     cmax=rlp_get_optimal_value(lp,fpp->bnds,false,&nearly);
     if(nearly==false){
		 if(recalBnds==true){
			 checked_malloc(pr, obnds, numdbl_t, (2*(int)fpp->dim), return 0;);
			 memcpy(obnds,fpp->bnds,2*fpp->dim*sizeof(numdbl_t));
				/* recalculate bounds */
			 for(i=0;i<fpp->dim;i++){
				*(fpp->bnds+2*i)=-MAX_VARBND;
				*(fpp->bnds+2*i+1)=MAX_VARBND;
			 }
			 fpp=bt_byRLP(pr,true,fpp,NULL,0);
		 }
		 numitv_t itv1 = numitv_lexp_range(fpp->dim,aCons+1,fpp->bnds);
		 cb = itv1.sup;
		 if(cb<cmax)  cmax=cb;
		 if(recalBnds==true){
			 memcpy(fpp->bnds,obnds,2*fpp->dim*sizeof(numdbl_t));
			 checked_free(obnds);
		 }
     }
     lp_delete(lp);
     if(cmax<=c){
        return true;
     }
     else return false;
  }
  else return false;
}


/* Polyhedron Intersection         	 */
/*  implemented by redundancy removal	 */
fpp_t *fppol_intersection(fpp_internal_t* pr, bool destructive, fpp_t *fpp1, fpp_t *fpp2)
{
    unsigned i,res;
    fpp_t *fpp3;

    if(fpp1->flag==UNIVERSE_POL){
       if(destructive==true) fpp_free_internal(pr, fpp1);
       return fpp_copy_internal(pr, fpp2);
    }
    else if(fpp2->flag==UNIVERSE_POL){
       if(destructive==true) return fpp1;
       else return fpp_copy_internal(pr, fpp1);
    }
    else if(fpp1->flag==EMPTY_POL || fpp2->flag==EMPTY_POL){
       if(destructive==true) fpp_free_internal(pr, fpp1);
       return fpp_alloc_internal(pr,fpp2->dim,fpp2->intdim);
    }

    if(destructive==false) fpp3=fpp_copy_internal(pr, fpp1);
    else fpp3=fpp1;
	/*Constraints union */
    fpp3->cons=(numdbl_t *)realloc(fpp3->cons,(fpp3->ncons+fpp2->ncons)*(fpp3->dim+1)*sizeof(numdbl_t));
    memcpy(fpp3->cons+fpp3->ncons*(fpp3->dim+1), fpp2->cons,fpp2->ncons*(fpp3->dim+1)*sizeof(numdbl_t));
    fpp3->ncons=fpp1->ncons+fpp2->ncons;
	/*Bounds intersection */
    for(i=0;i<fpp3->dim;i++){
	*(fpp3->bnds+2*i)=fmax(*(fpp1->bnds+2*i),*(fpp2->bnds+2*i)); /* lower bound */
        *(fpp3->bnds+2*i+1)=fmin(*(fpp1->bnds+2*i+1),*(fpp2->bnds+2*i+1));  /* upper bound */
    }
        /* polyhedron feasibility */
         /* bound check */
    for(i=0;i<fpp3->dim;i++){
	if( *(fpp3->bnds+2*i) > *(fpp3->bnds+2*i+1) ){
	   fpp3->flag=EMPTY_POL;
	   checked_free(fpp3->cons);
	   checked_free(fpp3->bnds);
	   fpp3->ncons=0;
           return fpp3;
        }
    }

    res=fppol_infeasibility_byRLP(pr,fpp3);

    if(res==1){
	   fpp3->flag=EMPTY_POL;
	   checked_free(fpp3->cons);
	   checked_free(fpp3->bnds);
	   fpp3->ncons=0;
           return fpp3;
    }
#if LAZY_RR
    fpp3=redundancy_removal_quasi_syntactic(pr,true,fpp3); /* lazy redundancy_removal*/
#else
    fpp3=redundancy_removal(pr,true,fpp3, true);
#endif
    fpp3=bt_byRLP(pr,true, fpp3, NULL,0);

    return fpp3;
}

/* Forget Function (Polyhedron projection)                                              		*/
/*
$$
\tau \left[\kern-0.15em\left[V_j := ?(-\infty,
+\infty)\right]\kern-0.15em\right]^\# (X^\#)= \pi (X^\#, {V_j})
$$
*/
/*  parameter varIndex stores the index(start from 1) of the variable to be eliminated (projected out). */
/*  parameter newNbCons to store the number of the new linear constraints. 				*/
fpp_t *fppol_project(fpp_internal_t* pr, bool destructive, fpp_t *fpp, int varIndex)
{
    fpp_t *fpp1;
    bool subset;

    if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;

    fpp1=fm_elimination(pr,true,fpp1, varIndex,NULL,&subset);
    fpp1=fppol_reshape_ineqs(pr,true,fpp1,NULL);
#if LAZY_RR
    if(subset==true)  return fpp1;
    fpp1=redundancy_removal_quasi_syntactic(pr,true,fpp1);
#else
    fpp1=redundancy_removal(pr,true,fpp1,true);
#endif

    return fpp1;
}

/* Assignment Transfer Function by Substitution:  Vj := exp;		*/
/* exp: $ { b+\sum a_i V_i } $				*/
/* the index for Vj is started from 1			*/
/*
$$
\begin{array}{l}
    \tau \left[\kern-0.15em\left[V_j := \sum_i a_i V_i + b
    \right]\kern-0.15em\right]^\# (\chi^\#)
    \mathrel{\mathop=^{\rm
    def}}\\ \ \ \ \ \ \ \ \
    \left\{
    \begin{array}{ll}
    <\mathcal{M}, \mathcal{\overrightarrow C}> where\ V_j\ is\ replaced\
    with\ \frac{1}{{a_j }}(V_j - \sum\limits_{i\neq j} a_i V_i - b)
            &   , \textrm{if}\ a_j\neq 0\\
    \tau \left[\kern-0.15em\left[\sum_i a_i V_i - V_j + b = 0
    \right]\kern-0.15em\right]^\# \circ \tau \left[\kern-0.15em\left[V_j
    := ?(-\infty, +\infty)\right]\kern-0.15em\right]^\#(\chi^\#)
            &   , \textrm{if}\ a_j = 0. \\
    \end{array}
    \right.\\
\end{array}
$$
*/
fpp_t *fppol_assign_bySubstitution(fpp_internal_t* pr, bool destructive, fpp_t *fpp, unsigned Vj, numdbl_t *exp)
{
    fpp_t *fpp1;
    numdbl_t *itvIneq,*intvp;
    unsigned i,j;
    numdbl_t aj,a2j,a2i,b2,b,ai;
    numdbl_t *aCons,*p,*oldCons;
    numitv_t bIntv,a2iIntv,newVjBndIntv;
    int sat;

    if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;
    aj=*(exp+Vj);

    b=*exp;
    if( aj==0){  /* non-invertible assignments */
		fpp1=fppol_project(pr,true, fpp1, Vj);
		if(fpp1->flag==GENERAL_POL)
		  fpp1->cons=realloc(fpp1->cons,(fpp1->ncons+2)*(fpp1->dim+1)*sizeof(numdbl_t));
		else{
		  checked_malloc(pr,fpp1->cons,numdbl_t,2*(fpp1->dim+1), return NULL;);
		  checked_malloc(pr,fpp1->bnds,numdbl_t,2*(fpp1->dim), return NULL;);
		  fpp_init_bounds(fpp1->bnds,fpp1->dim);
		  fpp1->flag=GENERAL_POL;
		}
		p=fpp1->cons+fpp1->ncons*(fpp1->dim+1);
		fpp1->ncons=fpp1->ncons+2;
			/*add constraint $\sum a_i*V_i - Vj <= -b $*/
		aCons=(numdbl_t *)malloc((fpp1->dim+1) * sizeof(numdbl_t));
		memcpy(aCons,exp,(fpp1->dim+1) * sizeof(numdbl_t));
		if(b==0)   *aCons=0.0;
		else  *aCons=-b;
		*(aCons+Vj)=-1;
		memcpy(p,aCons,(fpp1->dim+1) * sizeof(numdbl_t));
		p=p+(fpp1->dim+1);
			/*add constraint $\sum (-a_i)*V_i + Vj <= b $*/
		memcpy(aCons,exp,(fpp1->dim+1) * sizeof(numdbl_t));
		if(b==0)   *aCons=0.0;
		else  *aCons=b;
		for(i=1;i<=fpp1->dim;i++){
		  if(*(aCons+i) != 0){
			*(aCons+i)=-*(aCons+i);
		  }
		}
		*(aCons+Vj)=1;
		memcpy(p,aCons,(fpp1->dim+1) * sizeof(numdbl_t));

			/* new bounds for V'_j */
		numitv_t itv1 = numitv_lexp_range(fpp1->dim, exp+1, fpp1->bnds);
		numitv_t itv2 = numitv_singleton(*exp);
		newVjBndIntv = numitv_add(itv1, itv2);
		p=fpp1->bnds+2*(Vj-1);
			memcpy(p,&newVjBndIntv,2*sizeof(numdbl_t));
		checked_free(aCons);
		return fpp1;
    }
	else{/*invertible assignments:for each constraint,$V_j$ is replaced with $(V_j - \sum a_i V_i - b)\a_j$*/
		oldCons=fpp1->cons;
			/* new bounds for V'_j */
		numitv_t itv1 = numitv_lexp_range(fpp1->dim, exp+1, fpp1->bnds);
		numitv_t itv2 = numitv_singleton(*exp);
		newVjBndIntv = numitv_add(itv1, itv2);

		p=fpp1->bnds+2*(Vj-1);
		memcpy(p,&newVjBndIntv,2*sizeof(numdbl_t));
			/* new constraints */
		for(j=0;j<fpp1->ncons;j++){
			   /* for each constraint: \sum a2i*Vi +a2j*Vj <=b2   */
		   a2j=*(oldCons+Vj);
		   if(a2j!=0){
			  itvIneq=intvp=(numdbl_t *)malloc((2*fpp1->dim+1) * sizeof(numdbl_t));
			  b2=*oldCons;
			/* b2+a2j*(b/aj) */
			  bIntv=numitv_add(numitv_singleton(b2),numitv_mul(numitv_singleton(a2j),numitv_div_number(b,aj)));
			  *intvp=bIntv.sup;
			  intvp++;
			  for(i=1;i<=fpp1->dim;i++){
				 a2i=*(oldCons+i);
				 if(i!=Vj){
					ai=*(exp+i);
					/* a2i-a2j*(ai/aj) */
					a2iIntv=numitv_sub(numitv_singleton(a2i),numitv_mul(numitv_singleton(a2j),numitv_div_number(ai,aj)));
					memcpy(intvp,&a2iIntv,2*sizeof(numdbl_t));
					intvp=intvp+2;
				 }
				 else{ /* i==Vj */
							a2iIntv=numitv_div_number(a2i,aj);
					memcpy(intvp,&a2iIntv,2*sizeof(numdbl_t));
					intvp=intvp+2;
				 }
			  }
			  aCons=quasi_linearisation(fpp1->dim,itvIneq,fpp1->bnds,&sat);
			  memcpy(oldCons,aCons,(fpp1->dim+1) * sizeof(numdbl_t));
			  checked_free(itvIneq);
			  checked_free(aCons);
		   }
		   oldCons=oldCons+(fpp1->dim+1);
		}
		return fpp1;
    }
}

/* Polyhedron Standard Widening: Pol1 widen Pol2					*/
/*  implemented by syntactic and rigororous linear programming		*/
/* Note standard widening requires: 1) no equlities; 2) in mininal form */
fpp_t* fppol_widen_standard(fpp_internal_t* pr, bool destructive, fpp_t *fpp1, fpp_t *fpp2)
{
    unsigned i,j,flag;
    fpp_t *fpp3,*wdfpp;
    glp_prob *lp;
    int lpres;
    numdbl_t *p2,*env1,*env2,*env,*l0,*l1,*l2,*cbnd,*pb;
    numdbl_t c,ci,bi,aj;
    unsigned nEnv1,nEnv2,nEnv,nCbnd,NbQuasi;
    int res;

    /* if P1 is empty polyhedron */
    if(fpp1->flag==EMPTY_POL || fpp2->flag==UNIVERSE_POL){
	if(destructive==true) fpp_free_internal(pr,fpp1);
		return fpp_copy_internal(pr,fpp2);
    }

#if LAZY_RR
	/* lazy redundancy removal */
    fpp1=redundancy_removal(pr,true,fpp1,true);
    fpp2=redundancy_removal(pr,true,fpp2,true);
#endif

#if  DEBUG_WIDEN || PRINT_JWRES
printf("fppol_widen_standard:: \n");
fpp_fdump(stdout, pr->man, fpp1);
fpp_fdump(stdout, pr->man, fpp2);
fflush(stdout);
#endif

    env1=fppol_envelope_half(fpp1,fpp2,&nEnv1,&NbQuasi,NULL);
    if(NbQuasi >= fpp2->ncons){ /* all constraints in Pol2 are already in env1 */
		checked_free(env1);
		if(destructive==true) fpp_free_internal(pr,fpp1);
		return fpp_copy_internal(pr,fpp2);
    }
    checked_malloc(pr,env2,numdbl_t,(fpp2->ncons)*(fpp2->dim+1), return NULL;);
    p2=env2;
    nEnv2=0;
    fpp3=fpp_copy_internal(pr,fpp1);
    /* for each inequality l2 in P2,
       check inequalities in P1 one by one to see if there exist a inequality l1 s.t.
      ((P1\setminus l1)\cup l2)\sqsubseteq l1 and P1\sq subseteq l2. */
    for(i=0;i<fpp2->ncons;i++){ /* for each l2 \in P2 */
        l2=fpp2->cons+i*(fpp2->dim+1);
		/* l2 already in env1? Syntactic equivalence */
		flag=0;
		if(nEnv1>0){
		   for(j=0;j<nEnv1;j++){ /* {l \in P1 | P2\sqsubseteq l} */
			  l1=env1+j*(fpp1->dim+1);
			  if( vector_cmp(l1,l2,fpp1->dim+1)==0 ){
				 flag=1;
				 break;
			  }
		   }
		}
		if(flag==0){
			   /* P1\sqsubseteq l2 */
		   res=fppol_entailment(pr,fpp1,l2,false);
		   if(res==true){
			  /* ((P1\setminus l1)\cup l2)\sqsubtPol->bndsseteq l1 */
			  for(j=0;j<fpp1->ncons;j++){
				 l0=fpp1->cons+j*(fpp1->dim+1);
				 l1=fpp3->cons+j*(fpp3->dim+1);
				 memcpy(l1,l2,(fpp3->dim+1)*sizeof(numdbl_t));
				 res=fppol_entailment(pr,fpp3,l0,true);
				 if(res==true){
				   /* put l2 in the widening results */
				   memcpy(p2,l2,(fpp3->dim+1)*sizeof(numdbl_t));
				   p2=p2+(fpp3->dim+1);
				   nEnv2++;
				   /* restore fpp3 */
				   memcpy(l1,l0,(fpp3->dim+1)*sizeof(numdbl_t));
				   break;
				 }
					 /* restore fpp3 */
				 memcpy(l1,l0,(fpp3->dim+1)*sizeof(numdbl_t));
			  }
		   }
		}
    }
    nCbnd=0;cbnd=NULL; /* without common bounds */
    nEnv=nEnv1+nEnv2+nCbnd;
    if(nEnv1>0){
    	env=env1=realloc(env1,nEnv*(fpp2->dim+1)*sizeof(numdbl_t));
        if(nEnv2>0){
        	memcpy(env+nEnv1*(fpp2->dim+1),env2,nEnv2*(fpp2->dim+1)*sizeof(numdbl_t));
        }
        checked_free(env2);
        if(nCbnd>0){
        	memcpy(env+(nEnv1+nEnv2)*(fpp2->dim+1),cbnd,nCbnd*(fpp2->dim+1)*sizeof(numdbl_t));
        }
    }
    else if(nEnv2>0){
		env=env2=realloc(env2,nEnv2*(fpp2->dim+1)*sizeof(numdbl_t));
		if(nCbnd>0){
		    memcpy(env+nEnv2*(fpp2->dim+1),cbnd,nCbnd*(fpp2->dim+1)*sizeof(numdbl_t));
		}
    }
    else if(nCbnd>0){
		env=(numdbl_t *)malloc(nCbnd*(fpp2->dim+1)*sizeof(numdbl_t));
		memcpy(env,cbnd,nCbnd*(fpp2->dim+1)*sizeof(numdbl_t));
		checked_free(env1);
		checked_free(env2);
    }
    else{
		checked_free(env1);
		checked_free(env2);
        fpp_free_internal(pr,fpp3);
        return fpp_alloc_top(pr, fpp1->dim, fpp1->intdim);
    }
    wdfpp=fpp_alloc_urgent( pr, fpp1->dim, fpp1->intdim,nEnv);
    memcpy(wdfpp->cons,env,nEnv*(fpp1->dim+1)*sizeof(numdbl_t));
    /* recalculate bounds */
    for(i=0;i<wdfpp->dim;i++){
    	*(wdfpp->bnds+2*i)=-MAX_VARBND;
        *(wdfpp->bnds+2*i+1)=MAX_VARBND;
    }
    for(i=0;i<nCbnd;i++){
		pb=cbnd+i*(wdfpp->dim+1);
		bi=*pb;
		for(j=1;j<=wdfpp->dim;j++){
		   aj=*(pb+j);
		   if(aj==1.0){ /* upper bound */
			  *(wdfpp->bnds+2*j-1)=bi;
			  break;
		   }
		   else if(aj==-1.0){ /* lower bound */
			  if(bi==0) *(wdfpp->bnds+2*j-2)=0.0;
			  else *(wdfpp->bnds+2*j-2)=-bi;
			  break;
		   }
		}
    }
    wdfpp=bt_byRLP(pr,true,wdfpp,NULL,0);
	 /* standard widening requires: 1) no equlities; 2) in mininal form */
    wdfpp=redundancy_removal(pr,true,wdfpp,true);
    checked_free(env);
    if(nCbnd>0)  checked_free(cbnd);
    if(destructive==true)  fpp_free_internal(pr, fpp1);
    fpp_free_internal(pr, fpp3);
#if  DEBUG_WIDEN || PRINT_JWRES
printf("fppol_widen_standard result:: \n");
fpp_fdump(stdout, pr->man, wdfpp);
fflush(stdout);
#endif
    return wdfpp;
}

/* Polyhedron Standard Widening: Pol1 widen Pol2					*/
/*  implemented by syntactic and rigororous linear programming		*/
/* Note standard widening requires: 1) no equlities; 2) in mininal form */
fpp_t* fppol_widen_standard_withBound(fpp_internal_t* pr, bool destructive, fpp_t *fpp1, fpp_t *fpp2)
{
    unsigned i,j,flag;
    fpp_t *fpp3,*wdfpp;
    glp_prob *lp;
    int lpres;
    numdbl_t *p2,*env1,*env2,*env,*l0,*l1,*l2,*cbnd,*pb;
    numdbl_t c,ci,bi,aj;
    unsigned nEnv1,nEnv2,nEnv,nCbnd,NbQuasi;
    int res;

    /* if P1 is empty polyhedron */
    if(fpp1->flag==EMPTY_POL || fpp2->flag==UNIVERSE_POL){
		if(destructive==true) fpp_free_internal(pr,fpp1);
		return fpp_copy_internal(pr,fpp2);
    }

#if LAZY_RR
	/* lazy redundancy removal */
    fpp1=redundancy_removal(pr,true,fpp1,true);
    fpp2=redundancy_removal(pr,true,fpp2,true);
#endif

    env1=fppol_envelope_half(fpp1,fpp2,&nEnv1,&NbQuasi,NULL);
	if(NbQuasi >= fpp2->ncons){ /* all constraints in Pol2 are already in env1 */
		checked_free(env1);
		if(destructive==true) fpp_free_internal(pr,fpp1);
		return fpp_copy_internal(pr,fpp2);
    }
    checked_malloc(pr,env2,numdbl_t,(fpp2->ncons)*(fpp2->dim+1), return NULL;);
    p2=env2;
    nEnv2=0;
    fpp3=fpp_copy_internal(pr,fpp1);
    /* for each inequality l2 in P2,
       check inequalities in P1 one by one to see if there exist a inequality l1 s.t.
      ((P1\setminus l1)\cup l2)\sqsubseteq l1 and P1\sq subseteq l2. */
	for(i=0;i<fpp2->ncons;i++){ /* for each l2 \in P2 */
		l2=fpp2->cons+i*(fpp2->dim+1);
		/* l2 already in env1? Syntactic equivalence */
		flag=0;
		if(nEnv1>0){
		   for(j=0;j<nEnv1;j++){ /* {l \in P1 | P2\sqsubseteq l} */
			  l1=env1+j*(fpp1->dim+1);
			  if( vector_cmp(l1,l2,fpp1->dim+1)==0 ){
				 flag=1;
				 break;
			  }
		   }
		}
		if(flag==0){
			   /* P1\sqsubseteq l2 */
		   res=fppol_entailment(pr,fpp1,l2,false);
		   if(res==true){
			  /* ((P1\setminus l1)\cup l2)\sqsubtPol->bndsseteq l1 */
			  for(j=0;j<fpp1->ncons;j++){
				 l0=fpp1->cons+j*(fpp1->dim+1);
				 l1=fpp3->cons+j*(fpp3->dim+1);
				 memcpy(l1,l2,(fpp3->dim+1)*sizeof(numdbl_t));
				 res=fppol_entailment(pr,fpp3,l0,true);
				 if(res==true){
			   /* put l2 in the widening results */
			   memcpy(p2,l2,(fpp3->dim+1)*sizeof(numdbl_t));
			   p2=p2+(fpp3->dim+1);
			   nEnv2++;
			   /* restore fpp3 */
			   memcpy(l1,l0,(fpp3->dim+1)*sizeof(numdbl_t));
			   break;
				 }
					 /* restore fpp3 */
				 memcpy(l1,l0,(fpp3->dim+1)*sizeof(numdbl_t));
			  }
		   }
		}
    }
    cbnd=fppol_common_bounds(fpp1,fpp2,&nCbnd);
    nEnv=nEnv1+nEnv2+nCbnd;
    if(nEnv1>0){
    	env=env1=realloc(env1,nEnv*(fpp2->dim+1)*sizeof(numdbl_t));
        if(nEnv2>0){
			memcpy(env+nEnv1*(fpp2->dim+1),env2,nEnv2*(fpp2->dim+1)*sizeof(numdbl_t));
			checked_free(env2);
        }
        if(nCbnd>0){
        	memcpy(env+(nEnv1+nEnv2)*(fpp2->dim+1),cbnd,nCbnd*(fpp2->dim+1)*sizeof(numdbl_t));
        }
    }
    else if(nEnv2>0){
    	env=env2=realloc(env2,nEnv2*(fpp2->dim+1)*sizeof(numdbl_t));
        if(nCbnd>0){
		  memcpy(env+nEnv2*(fpp2->dim+1),cbnd,nCbnd*(fpp2->dim+1)*sizeof(numdbl_t));
		}
    }
    else if(nCbnd>0){
		env=(numdbl_t *)malloc(nCbnd*(fpp2->dim+1)*sizeof(numdbl_t));
		memcpy(env,cbnd,nCbnd*(fpp2->dim+1)*sizeof(numdbl_t));
    }
    else{
		checked_free(env2);
		fpp_free_internal(pr,fpp3);
		return fpp_alloc_top(pr, fpp1->dim, fpp1->intdim);
    }
    wdfpp=fpp_alloc_urgent( pr, fpp1->dim, fpp1->intdim,nEnv);
    memcpy(wdfpp->cons,env,nEnv*(fpp1->dim+1)*sizeof(numdbl_t));
    /* recalculate bounds */
    for(i=0;i<wdfpp->dim;i++){
    	*(wdfpp->bnds+2*i)=-MAX_VARBND;
        *(wdfpp->bnds+2*i+1)=MAX_VARBND;
    }
    for(i=0;i<nCbnd;i++){
		pb=cbnd+i*(wdfpp->dim+1);
		bi=*pb;
		for(j=1;j<=wdfpp->dim;j++){
		   aj=*(pb+j);
		   if(aj==1.0){ /* upper bound */
				  *(wdfpp->bnds+2*j-1)=bi;
			  break;
		   }
		   else if(aj==-1.0){ /* lower bound */
				  if(bi==0) *(wdfpp->bnds+2*j-2)=0.0;
			  else *(wdfpp->bnds+2*j-2)=-bi;
			  break;
		   }
		}
    }
    wdfpp=bt_byRLP(pr,true,wdfpp,NULL,0);
	 /* standard widening requires: 1) no equlities; 2) in mininal form */
    wdfpp=redundancy_removal(pr,true,wdfpp,true);
    if(env!=NULL)  checked_free(env);
    if(nCbnd>0)  checked_free(cbnd);
    if(destructive==true)  fpp_free_internal(pr, fpp1);
    fpp_free_internal(pr, fpp3);
#if  DEBUG_WIDEN || PRINT_JWRES
printf("fppol_widen_standard_withBound result:: \n");
fpp_fdump(stdout, pr->man, wdfpp);
fflush(stdout);
#endif
    return wdfpp;
}

/* common bounds between fpp1 and fpp2   */
/* return bound constriants              */
numdbl_t* fppol_common_bounds(fpp_t *fpp1, fpp_t *fpp2, unsigned *NbCmbnds)
{
    numdbl_t *cbnds,*p;
    numdbl_t *bnd1,*bnd2;
    numdbl_t lbnd1,lbnd2,ubnd1,ubnd2;
    int n;
    int i;

    cbnds=p=(numdbl_t *) malloc(2*fpp1->dim*(fpp1->dim+1)*sizeof(numdbl_t) );
    memset(p,0,2*fpp1->dim*(fpp1->dim+1)*sizeof(numdbl_t));
    bnd1=fpp1->bnds;
    bnd2=fpp2->bnds;
    n=0;
    for(i=0;i<(int)fpp1->dim;i++){
        lbnd1=*(bnd1+2*i);
        lbnd2=*(bnd2+2*i);
	if( lbnd1==lbnd2 && lbnd1 != -MAX_VARBND ){
	   if(lbnd1==0) *p=0.0;
	   else *p=-lbnd1;
	   *(p+i+1)=-1.0;
	   p=p+(fpp1->dim+1);
	   n++;
        }
        ubnd1=*(bnd1+2*i+1);
        ubnd2=*(bnd2+2*i+1);
	if( ubnd1==ubnd2 && ubnd1 != MAX_VARBND ){
	   if(ubnd1==0) *p=0.0;
	   else *p=ubnd1;
	   *(p+i+1)=1.0;
	   p=p+(fpp1->dim+1);
	   n++;
        }
    }
    *NbCmbnds=n;
    cbnds=(numdbl_t *) realloc(cbnds, n*(fpp1->dim+1)*sizeof(numdbl_t) );
    return cbnds;
}


/***********************************************************/
/*                  Rational                               */
/***********************************************************/

/* Polyhedron Join--Convex Hull */
/*
$$
P_{CH} = \left\{ {x\left| {\begin{array}{*{20}c}
   {x = y_1+y_2 \ \ \wedge \sigma _1  + \sigma _2  = 1 \ \wedge \sigma _1  \ge 0 \wedge }  \\
   {A_1 y_1  \le \sigma _1 b_1 \wedge A_2y_2  \le \sigma _2 b_2 \ \wedge \sigma _2  \ge 0}  \\
\end{array}} \right.} \right\}.
$$
that is
$$
P_{CH} = \left\{ {x\left| {\begin{array}{*{20}c}
{       0 \leq \sigma_1 \leq 1
\wedge  A_1*y_1 -b_1*\sigma_1 \leq 0
\wedge  A_2*x-A_2*y_1+b_2*\sigma_1 \leq b_2         }  \\
\end{array}} \right.} \right\}.
$$
*/
/* the intermediate repsentation of P_CH with (2*fpp1->dim+1) dimension */
/* c x y1 \sigma_1 */
fpp_t *fppol_strong_join_rational(fpp_internal_t* pr, fpp_t *fpp1, fpp_t *fpp2)
{
   unsigned i,j,k,nchars,nshift,nrem;
   unsigned nenvs,nchbnds;
   numdbl_t *env, *penv;
   numdbl_t *chbnds,*p,*bd12;
   numdbl_t *res,*cons1;
   fpp_t *fpp12; /* double dimension -> single dimension */
   char *bits,*bitVector;
   bit_vectors_t *bv;
   int superset;
   char *env1Index,*env2Index;
   char *env1bits,*nenv1bits,*env2bits,*nenv2bits;
   int mm;
   bool subset;

#if  DEBUG_JOIN || PRINT_JWRES
printf("fppol_strong_join_rational:: \n");
fpp_fdump(stdout, pr->man, fpp1);
fpp_fdump(stdout, pr->man, fpp2);
fflush(stdout);
#endif

   if(fpp1->flag==UNIVERSE_POL ){
      return fpp_copy_internal(pr, fpp1);
   }
   else if( fpp2->flag==UNIVERSE_POL  ){
      return fpp_copy_internal(pr, fpp2);
   }
   else if(fpp1->flag==EMPTY_POL ){
      return fpp_copy_internal(pr, fpp2);
   }
   else if(fpp2->flag==EMPTY_POL ){
      return fpp_copy_internal(pr, fpp1);
   }
	/* the envelope of fpp1 and fpp2 is certainly part of their convex hull */
    checked_malloc(pr,env1Index,char,fpp1->ncons, return NULL;);
    checked_malloc(pr,env2Index,char,fpp2->ncons, return NULL;);
    memset(env1Index,0,fpp1->ncons*sizeof(char));
    memset(env2Index,0,fpp2->ncons*sizeof(char));
    env=penv=fppol_envelope(fpp1,fpp2, &nenvs,&superset, env1Index, env2Index);
    if(superset==1){
    	return fpp_copy_internal(pr, fpp1);
    }
    else if(superset==2){
    	return fpp_copy_internal(pr, fpp2);
    }

    fpp12=fppol_construct_convex_hull_rational(pr,fpp1,fpp2,NULL/*env*/,0/*nenvs*/);

	/* Kohler's Rule: bits subset */
	/* Initialisation */
    checked_malloc(pr,bv,bit_vectors_t,1, return NULL;);
    bv->vsize=fpp12->ncons;
    bv->nvecs=fpp12->ncons;
    nchars=bv->vsize/8;
    if( (bv->vsize % 8)!=0 ) nchars++;
    bv->vchars=nchars;
    checked_malloc(pr,bv->bits,char,bv->nvecs*bv->vchars, return NULL;);
    memset(bv->bits,0,bv->nvecs*bv->vchars*sizeof(char));
    for(i=0;i<bv->nvecs;i++){
        bitVector=bv->bits+i*bv->vchars;
        nshift=i/8;
        nrem=i % 8;
        if(nrem==0) *(bitVector+nshift)=0x01;
        else *(bitVector+nshift)=0x01<<nrem;
    }

	  /*bit vector for envelope and non-envelope*/
    checked_malloc(pr,env1bits,char,bv->vchars, return NULL;);
    memset(env1bits,0,bv->vchars*sizeof(char));
    checked_malloc(pr,nenv1bits,char,bv->vchars, return NULL;);
    memset(nenv1bits,0,bv->vchars*sizeof(char));
    for(i=0;i<fpp1->ncons;i++){
		nshift=i/8;
		nrem=i % 8;
		if(*(env1Index+i)==1){ /* envelope */
		   if(nrem==0) *(env1bits+nshift)=0x01 | *(env1bits+nshift);
		   else *(env1bits+nshift)=0x01<<nrem  | *(env1bits+nshift);
		}
		else{   /* non- envelope */
		   if(nrem==0) *(nenv1bits+nshift)=0x01 | *(nenv1bits+nshift);
		   else *(nenv1bits+nshift)=0x01<<nrem  | *(nenv1bits+nshift);
		}
    }

    checked_malloc(pr,env2bits,char,bv->vchars, return NULL;);
    memset(env2bits,0,bv->vchars*sizeof(char));
    checked_malloc(pr,nenv2bits,char,bv->vchars, return NULL;);
    memset(nenv2bits,0,bv->vchars*sizeof(char));
    for(i=0;i<fpp2->ncons;i++){
		nshift=(i+fpp1->ncons)/8;
		nrem=(i+fpp1->ncons) % 8;
		if(*(env2Index+i)==1){ /* envelope */
		   if(nrem==0) *(env2bits+nshift)=0x01 | *(env2bits+nshift);
		   else *(env2bits+nshift)=0x01<<nrem  | *(env2bits+nshift);
		}
		else{   /* non- envelope */
		   if(nrem==0) *(nenv2bits+nshift)=0x01| *(nenv2bits+nshift);
		   else *(nenv2bits+nshift)=0x01<<nrem | *(nenv2bits+nshift);
		}
    }

    /* project out y2, \sigma_1 */
	while(fpp12->dim > fpp1->dim){
			/* Sound Fourier-Motzkin Elimination */
		k=fm_select(fpp12,fpp1->dim+1,fpp12->dim);
		fpp12=fm_elimination_rational(pr, true, fpp12, k,bv,&subset);
		if(fpp12->ncons==0)	   break;
		   /* remove dimension */
		fpp12->dim--;
		checked_malloc(pr,cons1,numdbl_t,fpp12->ncons*(fpp12->dim+1), return NULL;);
			for(j=0;j<fpp12->ncons;j++){
		   memcpy(cons1+j*(fpp12->dim+1),fpp12->cons+j*(fpp12->dim+2),k*sizeof(numdbl_t));
			   if(k<=fpp12->dim)
			  memcpy(cons1+j*(fpp12->dim+1)+k,fpp12->cons+j*(fpp12->dim+2)+k+1,(fpp12->dim+1-k)*sizeof(numdbl_t));
			}
		checked_free(fpp12->cons);
		fpp12->cons=cons1;
		if(k<=fpp12->dim) memmove(fpp12->bnds+2*(k-1),fpp12->bnds+2*k,2*(fpp12->dim+1-k)*sizeof(numdbl_t));
		fpp12->bnds=realloc(fpp12->bnds,2*fpp12->dim*sizeof(numdbl_t));
			/* Redundancy Removal */
		/* Kohler's Rule */
		if(fpp12->dim < 2*fpp1->dim){
		   fpp12=bv_redundancy_removal_firstKs_byKohler(pr,true,fpp12,fpp12->ncons,bv);
			   fpp12=redundancy_removal_quasi_syntactic_firstKs(pr,true,fpp12,fpp12->ncons,bv);
		}
    }
    if(fpp12->ncons==0){
		fpp12->dim=fpp1->dim;
		fpp12->flag=UNIVERSE_POL;
		checked_free(fpp12->cons);
		checked_free(fpp12->bnds);
		checked_free(env);
		checked_free(env1Index);
		checked_free(env2Index);
		return fpp12;
    }
    fpp12=bv_redundancy_removal_firstKs_byEnvelope(pr,true,fpp12,fpp12->ncons,bv,nenv1bits,nenv2bits);
	fpp12=redundancy_removal_firstKs_byBounds(pr,true,fpp12,fpp1->bnds,fpp12->ncons,bv);
	fpp12=redundancy_removal_firstKs_byBounds(pr,true,fpp12,fpp2->bnds,fpp12->ncons,bv);
	/* add envelope */
    fpp12->cons=realloc(fpp12->cons,(fpp12->ncons+nenvs)*(fpp12->dim+1)*sizeof(numdbl_t));
    memcpy(fpp12->cons+fpp12->ncons*(fpp12->dim+1),env,nenvs*(fpp12->dim+1)*sizeof(numdbl_t));
    fpp12->ncons=fpp12->ncons+nenvs;
    fpp12=redundancy_removal_firstKs(pr,true,fpp12,fpp12->ncons-nenvs,NULL,true);
	/* remove the redundancy in the envelope,               */
        /* e.g., (0,0)|_| (1,1) results x=y;x>=0;y>=0;x<=1,y<=1 */
    fpp12=redundancy_removal_lastKs(pr,true,fpp12,nenvs,true);
#if DEBUG_JOIN || PRINT_JWRES
	printf("fppol_strong_join result:: \n");
	fpp_fdump(stdout, pr->man, fpp12);
	fflush(stdout);
#endif
    checked_free(env);
    checked_free(env1bits);
    checked_free(nenv1bits);
    checked_free(env2bits);
    checked_free(nenv2bits);
    bv_free(bv);
    checked_free(env1Index);
    checked_free(env2Index);
    return fpp12;
}

fpp_t *fppol_construct_convex_hull_rational(fpp_internal_t* pr, fpp_t *fpp1, fpp_t *fpp2, numdbl_t *env, unsigned nenvs)
{
    fpp_t *fpp12; /* double dimension +1 */
    numdbl_t *p1,*p2,*p12;
    numdbl_t *bd1,*bd2,*q,*bd12;
    numdbl_t *res;
    numdbl_t *penv;
    unsigned i,j;
    int r;
    numdbl_t *c,*bSig;
    numdbl_t dk,maxadbk,dkInt,dkDif,mi;
    bool reshape;

    fpp12=fpp_alloc_urgent(pr,2*fpp1->dim+1,fpp1->intdim+fpp2->intdim,
			   fpp1->ncons+
               fpp2->ncons);
    /*--------------------------------------------------*/
    /* bounds (x y1 \sigma_1) for fpp12                  */
    /*--------------------------------------------------*/
    bd12=fpp12->bnds;
    bd1=fpp1->bnds;
    bd2=fpp2->bnds;
    /* bounds for (x y1 \sigma_1) */
    	/*-- bounds for x= y1+y2 =\sigma_1*x1+(1-\sigma_1)*x2 --*/
    		/*---- upper bound: x<= max{x1,x2}  ----*/
    		/*---- lower bound: x>= min{x1,x2}  ----*/
    for(i=0;i<fpp1->dim;i++){
		*(bd12+2*i)  =fmin( *(bd1+2*i), *(bd2+2*i) );
		*(bd12+2*i+1)=fmax( *(bd1+2*i+1), *(bd2+2*i+1) );
    }
    bd12=bd12+2*fpp1->dim;
    /*-- bounds for y1=(\sigma_1)*x1 --*/
    	/*---- if \overline{x1}>0    \overline{y1}=\overline{x1}   ----*/
	/*---- if \overline{x1}<=0   \overline{y1}=0 ----*/
	/*---- if \underline{x1}>=0  \underline{y1}=0 ----*/
	/*---- if \underline{x1}<0   \underline{y1}=\underline{x1} ----*/
    for(i=0;i<fpp1->dim;i++){
		if(*bd1>=0)    *bd12=0;
		else          *bd12=*bd1;
		bd12++;
		bd1++;
		if(*bd1>0)     *bd12=*bd1;
		else          *bd12=0;
		bd12++;
		bd1++;
    }
    	/* bounds for \sigma_1: [0,1] */
    *(bd12)=0;
    *(bd12+1)=1;

    /*--------------------------------------------*/
    /* Constraints for fpp12                      */
    /*--------------------------------------------*/
    p12=fpp12->cons;
	/* Constraints from A_1: $A_1*y_1 -b_1*\sigma_1 \leq 0$ from fpp1 */
    p1=fpp1->cons;
    for(i=0;i<fpp1->ncons;i++){
		*p12=0;  /* c: 0 */
		p12++;
		memset(p12,0,fpp1->dim*sizeof(numdbl_t));    /* x: 0 */
		p12=p12+fpp1->dim;
		memcpy(p12,p1+1,fpp1->dim*sizeof(numdbl_t)); /* A_1*y_1 */
		p12=p12+fpp1->dim;
		*p12=-(*p1);  /* -b_1*\sigma_1 */
		p12++;
		p1=p1+fpp1->dim+1;
    }

	/* Constraints from A_2: $A_2*x - A_2*y1+b_1*\sigma_1 \leq b_2$ from fpp2 */
    p2=fpp2->cons;
    for(i=0;i<fpp2->ncons;i++){
		*p12=*p2; /* b_2 */
		*(p12+1+2*fpp2->dim)=*p2;  /* b_2 *\sigma_1 */
		p12++;
		p2++;
		memcpy(p12,p2,fpp2->dim*sizeof(numdbl_t));   /* A_2*x */
		p12=p12+fpp2->dim;
		for(j=0;j<fpp2->dim;j++){  /* -A_2*y_1 */
		   *p12=-(*p2);
		   p12++;
		   p2++;
		}
		p12++;  /* space for \sigma_1 (has already been set ) */
    }
    return fpp12;
}
