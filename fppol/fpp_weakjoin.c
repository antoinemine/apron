/*
 * join.c
 *
 * strong join & weak joins
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
#include <glpk.h> /* glpk lib */

#include "fpp_weakjoin.h"
#include "numitv.h"

#include "fm.h"
#include "rlp.h"
#include "bt.h"
#include "numitv.h"
#include "quasi_linear.h"
#include "fpp_user.h"
#include "bv.h"
#include "fppol_domain.h"

int nstrongjoin=0;
int nweakjoin=0;
int maxCons=0;
/* ============================================================ */
/* Construction, Initialization, Deconstrunction */
/* ============================================================ */

fpp_t *fppol_weak_join_template(fpp_internal_t* pr, fpp_t *fpp01, fpp_t *fpp02)
{
   unsigned nTmplt,nchbnds;
   int flag;
   unsigned i,j;
   numdbl_t c,ci,cb;
   fpp_t* fpp12;
   numdbl_t *p,*tmplt;
   numdbl_t *l1,*l2;
   numdbl_t *bd1,*bd2,*bd12;
   glp_prob *lp;
   bool nearly;


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
   printf("fppol_weak_join_template:: \n");
   fpp_fdump(stdout, pr->man, fpp01);
   fpp_fdump(stdout, pr->man, fpp02);
   fflush(stdout);
#endif

   nTmplt=0;
   if( fpp02->ncons >= fpp01->ncons ){
      /* using the constraints from fpp01 as the template */
      tmplt=p=(numdbl_t *) malloc((fpp01->ncons)*(fpp01->dim+1)*sizeof(numdbl_t));
      lp=rlp_create_matrix(fpp02);
      for(i=0;i<fpp01->ncons;i++){ /* { max l \in P1 subject to P2 } */
		l1=fpp01->cons+i*(fpp01->dim+1);
		/* Quasi-Syntactic check */
		flag=0;
		for(j=0;j<fpp02->ncons;j++){ /* l1: e<=c1 ; l2: e<=c2 */
		   l2=fpp02->cons+j*(fpp02->dim+1);
		   if( vector_cmp(l1+1,l2+1,fpp02->dim)==0 ){
				  if(*l1==0)  *l1=0.0;
			  if(*l2>*l1){ /* keep l2 in the result */
				 memcpy(p,l2,(fpp01->dim+1)*sizeof(numdbl_t));
			  }
			  else{   /* keep l1 in  the result */
				 memcpy(p,l1,(fpp01->dim+1)*sizeof(numdbl_t));
			  }
			  flag=1;
			  p=p+(fpp01->dim+1);
			  nTmplt++;
			  break;
		   }
		}
		if(flag==0){
		   if(lp==NULL)  lp=rlp_create_matrix(fpp02);
			   rlp_set_objective(lp,GLP_MAX,l1+1);
			   lp=rlp_solve(lp);
			   ci=rlp_get_optimal_value(lp,fpp02->bnds,false,&nearly);
		   if(nearly==false){
			  numitv_t itv1 = numitv_lexp_range(fpp02->dim,l1+1,fpp02->bnds);
			  cb = itv1.sup;
			  if(cb<ci)  ci=cb;
		   }
		   c=*l1;
		   if(ci<c){  /* keep e <= c in the result */
			  *p=c;
			  memcpy(p+1,l1+1,(fpp01->dim)*sizeof(numdbl_t));
			  p=p+(fpp01->dim+1);
			  nTmplt++;
		   }
		   else if (ci>=c && ci != MAX_VARBND && isfinite(ci) ){ /* keep e <= ci in the result */
			  *p=ci;
			  memcpy(p+1,l1+1,(fpp01->dim)*sizeof(numdbl_t));
			  p=p+(fpp01->dim+1);
			  nTmplt++;
		   }
		}
      }

   }
   else if( fpp02->ncons < fpp01->ncons){
      /* using the constraints from fpp02 as the template */
      tmplt=p=(numdbl_t *) malloc((fpp02->ncons)*(fpp02->dim+1)*sizeof(numdbl_t));
      lp=rlp_create_matrix(fpp01);
      for(i=0;i<fpp02->ncons;i++){ /* { max l2 \in P2 subject to P1 } */
			l2=fpp02->cons+i*(fpp02->dim+1);
		/* Quasi-Syntactic check */
		flag=0;
		for(j=0;j<fpp01->ncons;j++){ /* l1: e<=c1 ; l2: e<=c2 */
		   l1=fpp01->cons+j*(fpp01->dim+1);
		   if( vector_cmp(l1+1,l2+1,fpp02->dim)==0 ){
				  if(*l2==0)  *l2=0.0;
			  if(*l2>*l1){ /* keep l2 in the result */
				 memcpy(p,l2,(fpp02->dim+1)*sizeof(numdbl_t));
			  }
			  else{   /* keep l1 in  the result */
				 memcpy(p,l1,(fpp01->dim+1)*sizeof(numdbl_t));
			  }
			  flag=1;
			  p=p+(fpp01->dim+1);
			  nTmplt++;
			  break;
		   }
		}
		if(flag==0){
		   if(lp==NULL)  lp=rlp_create_matrix(fpp01);
			   rlp_set_objective(lp,GLP_MAX,l2+1);
			   lp=rlp_solve(lp);
			   ci=rlp_get_optimal_value(lp,fpp01->bnds,false,&nearly);
		   if(nearly==false){
			  numitv_t itv1 = numitv_lexp_range(fpp01->dim,l2+1,fpp01->bnds);
			  cb = itv1.sup;
			  if(cb<ci)  ci=cb;
		   }
		   c=*l2;
		   if(ci<c){  /* keep e <= c in the result */
			  *p=c;
			  memcpy(p+1,l2+1,(fpp02->dim)*sizeof(numdbl_t));
			  p=p+(fpp02->dim+1);
			  nTmplt++;
		   }
		   else if (ci>=c && ci != MAX_VARBND && isfinite(ci) ){ /* keep e <= ci in the result */
			  *p=ci;
			  memcpy(p+1,l2+1,(fpp02->dim)*sizeof(numdbl_t));
			  p=p+(fpp02->dim+1);
			  nTmplt++;
			   }
		}
      }
   }
   else{
      printf("fppol_weak_join_template(): does not support this stuation. So return NULL. \n");
      fflush(stdout);
      return NULL;
   }

   fpp12=fpp_alloc_urgent(pr, fpp01->dim, fpp01->intdim, nTmplt+2*fpp01->dim);
   memcpy(fpp12->cons,tmplt,nTmplt*(fpp01->dim+1)*sizeof(numdbl_t));
   /* bounds info for the vars in fpp12 */
    bd1=fpp01->bnds;
    bd2=fpp02->bnds;
    bd12=fpp12->bnds;
    for(i=0;i<fpp01->dim;i++){
	if( *(bd1+2*i)==-MAX_VARBND || *(bd2+2*i)==-MAX_VARBND || isinf(*(bd1+2*i))==-1 || isinf(*(bd2+2*i))==-1 )
		*(bd12+2*i)=-MAX_VARBND;
	else    *(bd12+2*i)  =fmin( *(bd1+2*i), *(bd2+2*i) );
	if( *(bd1+2*i+1)==MAX_VARBND || *(bd2+2*i+1)==MAX_VARBND || isinf(*(bd1+2*i+1))==1 || isinf(*(bd2+2*i+1))==1 )
		*(bd12+2*i+1)=MAX_VARBND;
	else *(bd12+2*i+1)=fmax( *(bd1+2*i+1), *(bd2+2*i+1) );
    }
    /* add bounds to the template */
    p=fpp12->cons+nTmplt*(fpp12->dim+1);
    memset(p,0,2*fpp01->dim*(fpp01->dim+1)*sizeof(numdbl_t));
    nchbnds=0;
    for(i=0;i<fpp12->dim;i++){
        bd12=fpp12->bnds+2*i;
     	  	/* lower bound:   -V_i <= -\underline{V_i} */
	if(*bd12!=-MAX_VARBND && isfinite(*bd12) ){
    	   *p=-(*bd12);    /* -\underline{V_i} */
    	   *(p+i+1)=-1;    /* -V_i */
    	   p=p+fpp12->dim+1;
	   nchbnds++;
	}
	  /* upper bound:   V_i <= \overline{V_i} */
	if(*(bd12+1)!=MAX_VARBND && isfinite(*(bd12+1)) ){
    	   *p=*(bd12+1);    /* \overline{V_i} */
    	   *(p+i+1)=1;    /* V_i */
    	   p=p+fpp12->dim+1;
	   nchbnds++;
	}
    }

    fpp12->ncons=nTmplt+nchbnds;
    fpp12->cons=realloc(fpp12->cons,fpp12->ncons*(fpp12->dim+1)*sizeof(numdbl_t));

#if DEBUG_JOIN
   printf("fppol_weak_join_template::before:redundancy_removal_quasi_syntactic_firstKs \n");
   fpp_fdump(stdout, pr->man, fpp12);
   fflush(stdout);
#endif

    fpp12=redundancy_removal_quasi_syntactic_firstKs(pr, true, fpp12, nTmplt, NULL);
    fpp12=redundancy_removal_SLP(pr,true,fpp12,true);

#if DEBUG_JOIN || PRINT_JWRES
printf("fppol_weak_join_template result:: \n");
fpp_fdump(stdout, pr->man, fpp12);
fflush(stdout);
#endif

    checked_free(tmplt);
    lp_delete(lp);
    return fpp12;
}

fpp_t *fppol_weak_joins(fpp_internal_t* pr, fpp_t *fpp01, fpp_t *fpp02)
{
    unsigned i,j,k;
    fpp_t *fpp1,*fpp2;
    fpp_t *fpp12; /* double dimension -> single dimension */
    numdbl_t *l1,*l2;
    numdbl_t a1, a2;
    unsigned flag;
    numdbl_t *commonCons,*p;
    unsigned nCommonCons;
    numdbl_t *bd1,*bd2,*bd;
    unsigned nBd1,nBd2;

#if  DEBUG_JOIN || PRINT_JWRES
   printf("fppol_weak_joins: \n");
   fpp_fdump(stdout, pr->man, fpp01);
   fpp_fdump(stdout, pr->man, fpp02);
   fflush(stdout);
#endif

    if( (fpp01->ncons> 5*fpp02->ncons) || (fpp02->ncons> 5*fpp01->ncons) )
    {
    	fpp12=fppol_weak_join_template(pr, fpp01, fpp02);
        return fpp12;
    }
    if( (fpp01->ncons<15 && fpp02->ncons>50) || (fpp01->ncons>50 && fpp02->ncons<15) )
    {
    	fpp12=fppol_weak_join_template(pr, fpp01, fpp02);
        return fpp12;
    }
    if( (fpp01->ncons<30 && fpp02->ncons>70) || (fpp01->ncons>70 && fpp02->ncons<30) )
    {
    	fpp12=fppol_weak_join_template(pr, fpp01, fpp02);
        return fpp12;
    }
    if( (fpp01->ncons<90 && fpp02->ncons>250) || (fpp01->ncons>250 && fpp02->ncons<90) )
	{
		fpp12=fppol_weak_join_template(pr, fpp01, fpp02);
        return fpp12;
    }
    fpp12=fppol_weak_join_each_overapprox(pr,fpp01,fpp02);

#if DEBUG_JOIN || PRINT_JWRES
printf("fppol_weak_joins result:: \n");
fpp_fdump(stdout, pr->man, fpp12);
fflush(stdout);
#endif

    return fpp12;
}


fpp_t *fppol_weak_join_each_overapprox(fpp_internal_t* pr, fpp_t *fpp01, fpp_t *fpp02)
{
    unsigned k;
    int i,j;
    numdbl_t *env=NULL;
    numdbl_t *p,*bd12,*bd1,*bd2,*bd,*chbnds;
    unsigned nBd1,nBd2,nchbnds;
    fpp_t *fpp1,*fpp2,*fpp3;
    fpp_t *fpp12; /* double dimension -> single dimension */
    int superset;
    int ni;
    bool subset;
    bool flag;
    int r;
    unsigned nEnv1, nEnv2,nenv, nQuasi1, nQuasi2, nNenv1, nNenv2, nEnv1New,nEnv2New,nEnvNew;
    numdbl_t *env1=NULL,*env2=NULL,*nenv1=NULL,*nenv2=NULL,*env1New=NULL,*env2New=NULL,*envNew=NULL;
    unsigned nNewCons;
    numdbl_t *newCons;
    char *envIndex1,*envIndex2;
    numdbl_t *l1,*l2;
    numdbl_t fact1,fact2;
    numdbl_t non0t1,non0t2;


    /* polyhedron: envelope + bnds for fpp01 */
    checked_malloc(pr,envIndex1,char,fpp01->ncons, return NULL;);
    memset(envIndex1,0,fpp01->ncons*sizeof(char));
    env1=fppol_envelope_half(fpp01,fpp02,&nEnv1, &nQuasi1,envIndex1);
    if(nEnv1==fpp01->ncons){
		checked_free(envIndex1);
		checked_free(env1);
		return fpp_copy_internal(pr, fpp01);
    }
    nNenv1=fpp01->ncons-nEnv1;
    checked_malloc(pr,envIndex2,char,fpp02->ncons, return NULL;);
    memset(envIndex2,0,fpp02->ncons*sizeof(char));
    env2=fppol_envelope_half(fpp02,fpp01,&nEnv2, &nQuasi2,envIndex2);
    nNenv2=fpp02->ncons-nEnv2;
    if(nEnv2==fpp02->ncons){
		checked_free(envIndex1);
		checked_free(env1);
		checked_free(envIndex2);
		checked_free(env2);
		return fpp_copy_internal(pr, fpp02);
    }
    /* join bounds */
    checked_malloc(pr,chbnds,numdbl_t,2*fpp01->dim*(fpp01->dim+1), return NULL;);
    memset(chbnds,0,2*fpp01->dim*(fpp01->dim+1)*sizeof(numdbl_t));
    p=chbnds;
    nchbnds=0;
    checked_malloc(pr,bd12,numdbl_t,2*fpp01->dim, return NULL;);
    memset(bd12,0,2*fpp01->dim*sizeof(numdbl_t));
    for(i=0;i<(int)fpp01->dim;i++){
        bd1=fpp01->bnds+2*i;
        bd2=fpp02->bnds+2*i;
     	  	/* lower bound:   -V_i <= -\underline{V_i} */
        if(*bd1!=-MAX_VARBND && *bd2!=-MAX_VARBND && isfinite(*bd1) && isfinite(*bd2) ){
           *(bd12+2*i)=fmin(*bd1,*bd2);
    	   *p=-(fmin(*bd1,*bd2));    /* -\underline{V_i} */
    	   *(p+i+1)=-1;    /* -V_i */
    	   p=p+fpp01->dim+1;
    	   nchbnds++;
        }
        else *(bd12+2*i)=-MAX_VARBND;
		  /* upper bound:   V_i <= \overline{V_i} */
		if(*(bd1+1)!=MAX_VARBND && *(bd2+1)!=MAX_VARBND && isfinite(*(bd1+1)) && isfinite(*(bd2+1)) ){
			   *(bd12+2*i+1)=fmax(*(bd1+1),*(bd2+1));
			   *p=fmax(*(bd1+1),*(bd2+1));    /* \overline{V_i} */
			   *(p+i+1)=1;    /* V_i */
			   p=p+fpp01->dim+1;
		   nchbnds++;
		}
        else *(bd12+2*i+1)=MAX_VARBND;
    }
	/* in general nQuasi1 not necessarily =nQuasi2 */
    fact1=fppol_sparse_factor(fpp01);
    fact2=fppol_sparse_factor(fpp02);
    non0t1=fact1*fpp01->dim*fpp01->ncons;
    non0t2=fact2*fpp02->dim*fpp02->ncons;
    if(non0t1>400 || non0t2>400 ){ /*too dense*/
	/* env+chbnds */
       fpp12=fpp_alloc_urgent(pr, fpp01->dim, fpp01->intdim,nEnv1+nEnv2+nchbnds);
       memcpy(fpp12->bnds,bd12, 2*fpp01->dim*sizeof(numdbl_t));
       if(nEnv1>0)
           memcpy(fpp12->cons, env1, nEnv1*(fpp01->dim+1)*sizeof(numdbl_t));
      if(nEnv2>0)
           memcpy(fpp12->cons+nEnv1*(fpp01->dim+1), env2, nEnv2*(fpp01->dim+1)*sizeof(numdbl_t));
      if(nchbnds>0) memcpy(fpp12->cons+(nEnv1+nEnv2)*(fpp01->dim+1),chbnds,nchbnds*(fpp01->dim+1)*sizeof(numdbl_t));
	  /* at most nQuasi2 common constraints in both nEnv1 + nEnv2 */
      fpp12=redundancy_removal_quasi_syntactic_lastKs(pr, true, fpp12, nEnv2+nchbnds);
      fpp12=redundancy_removal_SLP_lastKs(pr,true,fpp12, fpp12->ncons-(nEnv1+nEnv2-nQuasi2), true);
      checked_free(env1);
      checked_free(env2);
      checked_free(envIndex1);
      checked_free(envIndex2);
      checked_free(bd12);
      checked_free(chbnds);

#if DEBUG_JOIN || PRINT_JWRES
printf("fppol_weak_join_each_overapprox result:: \n");
fpp_fdump(stdout, pr->man, fpp12);
fflush(stdout);
#endif
      return fpp12;
   }

    	/* env + bnds + TVPI  */
    if( (nEnv1>15 && nNenv1 >15) || (nEnv2 >15 && nNenv2 >15 ) ){
        fpp12=fppol_weak_join_envTVPI(pr,fpp01,fpp02);
        return fpp12;
    }
    	/* env + bnds + TVPI  */
    if(fact1>0.4 && fact2>0.4 && fpp01->ncons>60 && fpp01->ncons>60 && fpp01->dim<10){
        fpp12=fppol_weak_join_envTVPI(pr,fpp01,fpp02);
        return fpp12;
    }

    bd1=NULL;
    bd2=NULL;
#if  DEBUG_JOIN || PRINT_JWRES
   printf("fppol_weak_join_each_overapprox: \n");
   fpp_fdump(stdout, pr->man, fpp01);
   fpp_fdump(stdout, pr->man, fpp02);
   fflush(stdout);
#endif

	/* overapprox. for fpp01  */
	if( nEnv1 > nNenv1 ){
		/* polyhedron: non-envelope + bnds for fpp01 */
			/* non-envelope for fpp01 */
		checked_malloc(pr,nenv1,numdbl_t,nNenv1*(fpp01->dim+1), return NULL;);
		memset(nenv1,0,nNenv1*(fpp01->dim+1)*sizeof(numdbl_t));
		p=nenv1;
		for(i=0;i<(int)fpp01->ncons;i++){
			if( *(envIndex1+i) == 0 ){
			memcpy(p, fpp01->cons+ i*(fpp01->dim+1), (fpp01->dim+1)*sizeof(numdbl_t) );
			p=p+(fpp01->dim+1);
			}
		}
			/* bnd for fpp01 */
		checked_malloc(pr,bd1,numdbl_t,2*fpp01->dim*(fpp01->dim+1), return NULL;);
		memset(bd1,0,2*fpp01->dim*(fpp01->dim+1)*sizeof(numdbl_t));
		p=bd1;
		nBd1=0;
		for(i=0;i<(int)fpp01->dim;i++){
			bd=fpp01->bnds+2*i;
				/* lower bound:   -V_i <= -\underline{V_i} */
			if(*bd!=-MAX_VARBND && isfinite(*bd)){
			*p=-(*bd);    /* -\underline{V_i} */
			*(p+i+1)=-1;    /* -V_i */
			p=p+fpp01->dim+1;
			nBd1++;
			}
			/* upper bound:   V_i <= \overline{V_i} */
			if(*(bd+1)!=MAX_VARBND && isfinite(*(bd+1)) ){
			*p=*(bd+1);    /* \overline{V_i} */
			*(p+i+1)=1;    /* V_i */
			p=p+fpp01->dim+1;
			nBd1++;
			}
		}

		fpp1=fpp_copy_internal(pr, fpp01); /* fpp1 has the same bounds as fpp01 */
		fpp1->cons=(numdbl_t *) realloc(fpp1->cons,(nBd1+nNenv1)*(fpp01->dim+1)*sizeof(numdbl_t));
		fpp1->ncons=nBd1+nNenv1;
		if(nBd1 > 0)
			memcpy(fpp1->cons,bd1, nBd1*(fpp01->dim+1)*sizeof(numdbl_t) );
		memcpy(fpp1->cons+nBd1*(fpp1->dim+1),nenv1, nNenv1*(fpp1->dim+1)*sizeof(numdbl_t) );
		if(nBd1 > 0)
			fpp1=redundancy_removal_quasi_syntactic_firstKs(pr, true, fpp1, nBd1, NULL);
		if(fpp1->ncons > nNenv1)
			fpp1=redundancy_removal_SLP_firstKs(pr,true,fpp1,fpp1->ncons-nNenv1, NULL, true);
		nEnv1New=0;
		if(nBd1 > 0) checked_free(bd1);
    }
    else{
	   /* polyhedron: envelope + bnds for fpp01 */
		checked_malloc(pr,bd1,numdbl_t,2*fpp01->dim*(fpp01->dim+1), return NULL;);
		memset(bd1,0,2*fpp01->dim*(fpp01->dim+1)*sizeof(numdbl_t));
		p=bd1;
		nBd1=0;
		for(i=0;i<(int)fpp01->dim;i++){
			bd=fpp01->bnds+2*i;
				/* lower bound:   -V_i <= -\underline{V_i} */
			if(*bd!=-MAX_VARBND && isfinite(*bd) ){
			*p=-(*bd);    /* -\underline{V_i} */
			*(p+i+1)=-1;    /* -V_i */
			p=p+fpp01->dim+1;
			nBd1++;
			}
			/* upper bound:   V_i <= \overline{V_i} */
			if(*(bd+1)!=MAX_VARBND  && isfinite(*(bd+1)) ){
			*p=*(bd+1);    /* \overline{V_i} */
			*(p+i+1)=1;    /* V_i */
			p=p+fpp01->dim+1;
			nBd1++;
			}
		}

		fpp1=fpp_copy_internal(pr, fpp01); /* fpp1 has the same bounds as fpp01 */
		fpp1->cons=(numdbl_t *) realloc(fpp1->cons,(nBd1+nEnv1)*(fpp01->dim+1)*sizeof(numdbl_t));
		fpp1->ncons=nBd1+nEnv1;
		if(nBd1 > 0)
			memcpy(fpp1->cons,bd1, nBd1*(fpp01->dim+1)*sizeof(numdbl_t) );
		memcpy(fpp1->cons+nBd1*(fpp01->dim+1),env1, nEnv1*(fpp01->dim+1)*sizeof(numdbl_t) );
		if(nBd1 > 0)
			fpp1=redundancy_removal_quasi_syntactic_firstKs(pr, true, fpp1, nBd1, NULL);
		if(fpp1->ncons > nEnv1)
			fpp1=redundancy_removal_SLP_firstKs(pr,true,fpp1,fpp1->ncons-nEnv1,NULL,true);
		nEnv1New=nEnv1;
		checked_malloc(pr,env1New,numdbl_t,nEnv1*(fpp01->dim+1), return NULL;);
		memcpy(env1New,env1,nEnv1*(fpp01->dim+1)*sizeof(numdbl_t));
		if(nBd1 > 0) checked_free(bd1);
    }
	/* overapprox. for fpp02  */
    if( nEnv2 > nNenv2 ){
	/* polyhedron: non-envelope + bnds for fpp02 */
			/* non-envelope for fpp02 */
		checked_malloc(pr,nenv2,numdbl_t,nNenv2*(fpp02->dim+1), return NULL;);
		memset(nenv2,0,nNenv2*(fpp02->dim+1)*sizeof(numdbl_t));
		p=nenv2;
		for(i=0;i<(int)fpp02->ncons;i++){
			if( *(envIndex2+i) == 0 ){
			memcpy(p, fpp02->cons+ i*(fpp02->dim+1), (fpp02->dim+1)*sizeof(numdbl_t) );
			p=p+(fpp02->dim+1);
			}
		}
			/* bnd for fpp02 */
		checked_malloc(pr,bd2,numdbl_t,2*fpp02->dim*(fpp02->dim+1), return NULL;);
		memset(bd2,0,2*fpp02->dim*(fpp02->dim+1)*sizeof(numdbl_t));
		p=bd2;
		nBd2=0;
		for(i=0;i<(int)fpp02->dim;i++){
			bd=fpp02->bnds+2*i;
				/* lower bound:   -V_i <= -\underline{V_i} */
			if(*bd!=-MAX_VARBND && isfinite(*bd)){
			*p=-(*bd);    /* -\underline{V_i} */
			*(p+i+1)=-1;    /* -V_i */
			p=p+fpp02->dim+1;
			nBd2++;
			}
			/* upper bound:   V_i <= \overline{V_i} */
			if(*(bd+1)!=MAX_VARBND && isfinite(*(bd+1)) ){
			*p=*(bd+1);    /* \overline{V_i} */
			*(p+i+1)=1;    /* V_i */
			p=p+fpp02->dim+1;
			nBd2++;
			}
		}
		fpp2=fpp_copy_internal(pr, fpp02); /* fpp2 has the same bounds as fpp02 */
		fpp2->cons=(numdbl_t *) realloc(fpp2->cons,(nBd2+nNenv2)*(fpp02->dim+1)*sizeof(numdbl_t));
		fpp2->ncons=nBd2+nNenv2;
		if(nBd2 > 0)
		   memcpy(fpp2->cons, bd2, nBd2*(fpp2->dim+1)*sizeof(numdbl_t) );
		memcpy(fpp2->cons+nBd2*(fpp2->dim+1),nenv2, nNenv2*(fpp2->dim+1)*sizeof(numdbl_t) );
		if(nBd2 > 0)
		   fpp2=redundancy_removal_quasi_syntactic_firstKs(pr, true, fpp2, nBd2, NULL);
		if(fpp2->ncons > nNenv1)
		   fpp2=redundancy_removal_SLP_firstKs(pr,true,fpp2,fpp2->ncons-nNenv2, NULL, true);
		nEnv2New=0;
		if(nBd2 > 0) checked_free(bd2);
    }
		else{
		/* polyhedron: envelope + bnds for fpp02 */
		checked_malloc(pr,bd2,numdbl_t,2*fpp02->dim*(fpp02->dim+1), return NULL;);
		memset(bd2,0,2*fpp02->dim*(fpp02->dim+1)*sizeof(numdbl_t));
		p=bd2;
		nBd2=0;
		for(i=0;i<(int)fpp02->dim;i++){
			bd=fpp02->bnds+2*i;
				/* lower bound:   -V_i <= -\underline{V_i} */
			if(*bd!=-MAX_VARBND &&  isfinite(*bd)){
			*p=-(*bd);    /* -\underline{V_i} */
			*(p+i+1)=-1;    /* -V_i */
			p=p+fpp02->dim+1;
			nBd2++;
			}
			/* upper bound:   V_i <= \overline{V_i} */
			if(*(bd+1)!=MAX_VARBND && isfinite(*(bd+1))){
			*p=*(bd+1);    /* \overline{V_i} */
			*(p+i+1)=1;    /* V_i */
			p=p+fpp02->dim+1;
			nBd2++;
			}
		}
		fpp2=fpp_copy_internal(pr, fpp02); /* fpp2 has the same bounds as fpp02 */
		fpp2->cons=(numdbl_t *) realloc(fpp2->cons,(nBd2+nEnv2)*(fpp02->dim+1)*sizeof(numdbl_t));
		fpp2->ncons=nBd2+nEnv2;
		if(nBd2 > 0)
		   memcpy(fpp2->cons, bd2, nBd2*(fpp2->dim+1)*sizeof(numdbl_t) );
		memcpy(fpp2->cons+nBd2*(fpp2->dim+1),env2, nEnv2*(fpp2->dim+1)*sizeof(numdbl_t) );
		if(nBd2 > 0)
		   fpp2=redundancy_removal_quasi_syntactic_firstKs(pr, true, fpp2, nBd2, NULL);
		if(fpp2->ncons > nEnv2)
		   fpp2=redundancy_removal_SLP_firstKs(pr,true,fpp2,fpp2->ncons-nEnv2,NULL,true);

		nEnv2New=nEnv2;
		checked_malloc(pr,env2New,numdbl_t,nEnv2*(fpp02->dim+1), return NULL;);
		memcpy(env2New,env2,nEnv2*(fpp02->dim+1)*sizeof(numdbl_t));
		if(nBd2 > 0) checked_free(bd2);
    }

   checked_malloc(pr,envNew,numdbl_t, (nEnv1New+nEnv2New)*(fpp01->dim+1), return NULL;);
   if(nEnv1New>0)
       memcpy(envNew, env1New, nEnv1New*(fpp01->dim+1)*sizeof(numdbl_t));
   if(nEnv2New>0)
       memcpy(envNew+nEnv1New*(fpp01->dim+1), env2New, nEnv2New*(fpp01->dim+1)*sizeof(numdbl_t));

#if  DEBUG_JOIN
   printf("fppol_convex_hull_new_cons:input: \n");
   fpp_fdump(stdout, pr->man, fpp1);
   fpp_fdump(stdout, pr->man, fpp2);
   fflush(stdout);
#endif

   newCons=fppol_convex_hull_new_cons(pr, fpp1, fpp2, envNew, nEnv1New+nEnv2New, chbnds, nchbnds,&nNewCons);
   if(nNewCons>0) {
      /* tighten the new relationship as templates */
      fppol_tighten_tmplts(pr, fpp01, fpp02, nNewCons, newCons);
   }

   fpp12=fpp_alloc_urgent(pr, fpp01->dim, fpp01->intdim,nEnv1+nEnv2+ nchbnds +nNewCons);
   memcpy(fpp12->bnds,bd12, 2*fpp01->dim*sizeof(numdbl_t));
   if(nNewCons>0)
     memcpy(fpp12->cons,newCons,nNewCons*(fpp01->dim+1)*sizeof(numdbl_t));
   if(nEnv1+nEnv2>0){
      checked_malloc(pr,env,numdbl_t, (nEnv1+nEnv2)*(fpp01->dim+1), return NULL;);
      if(nEnv1>0)
           memcpy(env, env1, nEnv1*(fpp01->dim+1)*sizeof(numdbl_t));
      if(nEnv2>0)
           memcpy(env+nEnv1*(fpp01->dim+1), env2, nEnv2*(fpp01->dim+1)*sizeof(numdbl_t));
      memcpy(fpp12->cons+nNewCons*(fpp01->dim+1),env,(nEnv1+nEnv2)*(fpp01->dim+1)*sizeof(numdbl_t));
   }
   if(nchbnds>0) memcpy(fpp12->cons+(nEnv1+nEnv2+nNewCons)*(fpp01->dim+1),chbnds,nchbnds*(fpp01->dim+1)*sizeof(numdbl_t));
	/* at most nQuasi2 common constraints in both nEnv1 + nEnv2 */
   fpp12=fppol_reshape_ineqs(pr,true,fpp12,NULL);
   fpp12=redundancy_removal_quasi_syntactic_lastKs(pr, true, fpp12, nEnv2+nchbnds);
   fpp12=redundancy_removal_SLP_lastKs(pr,true,fpp12, fpp12->ncons-(nNewCons+nEnv1+nEnv2-nQuasi2), true);
   if(nEnv1+nEnv2>0) checked_free(env);
   if(nEnv1>0) checked_free(env1);
   if(nEnv2>0) checked_free(env2);
   if(nEnv1New+nEnv2New>0)  checked_free(envNew);
   if(nEnv1New>0)  checked_free(env1New);
   if(nEnv2New>0)  checked_free(env2New);
   if(nNewCons>0)  checked_free(newCons);
   checked_free(envIndex1);
   checked_free(envIndex2);
   checked_free(bd12);
   checked_free(chbnds);
   fpp_free_internal(pr, fpp1);
   fpp_free_internal(pr, fpp2);

#if DEBUG_JOIN || PRINT_JWRES
printf("fppol_weak_join_each_overapprox result:: \n");
fpp_fdump(stdout, pr->man, fpp12);
fflush(stdout);
#endif
   return fpp12;
}

fpp_t *fppol_weak_join_envbnds(fpp_internal_t* pr, fpp_t *fpp01, fpp_t *fpp02)
{
   unsigned i,j,k,nchars,nshift,nrem;
   numdbl_t *env;
   numdbl_t *p,*bd12,*bd1,*bd2,*bd,*chbnds;
   unsigned nBd1,nBd2,nchbnds;
   fpp_t *fpp1,*fpp2;
   fpp_t *fpp12; /* double dimension -> single dimension */
   int superset;
   int ni;
   bool subset;
   bool flag;
   int r;
   unsigned nEnv1, nEnv2,nenv, nQuasi;
   numdbl_t *env1,*env2;
   unsigned nNewCons;
   numdbl_t *newCons;

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
   printf("fppol_weak_join_envbnds:: \n");
   fpp_fdump(stdout, pr->man, fpp01);
   fpp_fdump(stdout, pr->man, fpp02);
   fflush(stdout);
#endif

    /* polyhedron: envelope + bnds for fpp01 */
    env1=fppol_envelope_half(fpp01,fpp02,&nEnv1, &nQuasi,NULL);
    checked_malloc(pr,bd1,numdbl_t,2*fpp01->dim*(fpp01->dim+1), return NULL;);
    memset(bd1,0,2*fpp01->dim*(fpp01->dim+1)*sizeof(numdbl_t));
    p=bd1;
    nBd1=0;
    for(i=0;i<fpp01->dim;i++){
        bd=fpp01->bnds+2*i;
     	  	/* lower bound:   -V_i <= -\underline{V_i} */
		if(*bd!=-MAX_VARBND && isfinite(*bd) ){
			   *p=-(*bd);    /* -\underline{V_i} */
			   *(p+i+1)=-1;    /* -V_i */
			   p=p+fpp01->dim+1;
		   nBd1++;
		}
		  /* upper bound:   V_i <= \overline{V_i} */
		if(*(bd+1)!=MAX_VARBND  && isfinite(*(bd+1)) ){
			   *p=*(bd+1);    /* \overline{V_i} */
			   *(p+i+1)=1;    /* V_i */
			   p=p+fpp01->dim+1;
		   nBd1++;
		}
    }

   fpp1=fpp_copy_internal(pr, fpp01); /* fpp1 has the same bounds as fpp01 */
   fpp1->cons=(numdbl_t *) realloc(fpp1->cons,(nBd1+nEnv1)*(fpp01->dim+1)*sizeof(numdbl_t));
   fpp1->ncons=nBd1+nEnv1;
   if(nBd1 > 0)
       memcpy(fpp1->cons,bd1, nBd1*(fpp01->dim+1)*sizeof(numdbl_t) );
   memcpy(fpp1->cons+nBd1*(fpp01->dim+1),env1, nEnv1*(fpp01->dim+1)*sizeof(numdbl_t) );
   if(nBd1 > 0)
       fpp1=redundancy_removal_quasi_syntactic_firstKs(pr, true, fpp1, nBd1, NULL);
   if(fpp1->ncons > nEnv1)
       fpp1=redundancy_removal_SLP_firstKs(pr,true,fpp1,fpp1->ncons-nEnv1,NULL,true);

   /* polyhedron: envelope + bnds for fpp02 */
    env2=fppol_envelope_half(fpp02,fpp01,&nEnv2, &nQuasi,NULL);
    checked_malloc(pr,bd2,numdbl_t,2*fpp02->dim*(fpp02->dim+1), return NULL;);
    memset(bd2,0,2*fpp02->dim*(fpp02->dim+1)*sizeof(numdbl_t));
    p=bd2;
    nBd2=0;
    for(i=0;i<fpp02->dim;i++){
        bd=fpp02->bnds+2*i;
     	  	/* lower bound:   -V_i <= -\underline{V_i} */
		if(*bd!=-MAX_VARBND &&  isfinite(*bd)){
			   *p=-(*bd);    /* -\underline{V_i} */
			   *(p+i+1)=-1;    /* -V_i */
			   p=p+fpp02->dim+1;
		   nBd2++;
		}
		  /* upper bound:   V_i <= \overline{V_i} */
		if(*(bd+1)!=MAX_VARBND && isfinite(*(bd+1))){
			   *p=*(bd+1);    /* \overline{V_i} */
			   *(p+i+1)=1;    /* V_i */
			   p=p+fpp02->dim+1;
		   nBd2++;
		}
    }
   fpp2=fpp_copy_internal(pr, fpp02); /* fpp2 has the same bounds as fpp02 */
   fpp2->cons=(numdbl_t *) realloc(fpp2->cons,(nBd2+nEnv2)*(fpp02->dim+1)*sizeof(numdbl_t));
   fpp2->ncons=nBd2+nEnv2;
   if(nBd2 > 0)
       memcpy(fpp2->cons, bd2, nBd2*(fpp2->dim+1)*sizeof(numdbl_t) );
   memcpy(fpp2->cons+nBd2*(fpp2->dim+1),env2, nEnv2*(fpp2->dim+1)*sizeof(numdbl_t) );

#if  DEBUG_JOIN
   printf("fppol_convex_hull_new_cons:input: fpp2 \n");
   fpp_fdump(stdout, pr->man, fpp2);
   fflush(stdout);
#endif
   if(nBd2 > 0)
       fpp2=redundancy_removal_quasi_syntactic_firstKs(pr, true, fpp2, nBd2, NULL);
   if(fpp2->ncons > nEnv2)
       fpp2=redundancy_removal_SLP_firstKs(pr,true,fpp2,fpp2->ncons-nEnv2,NULL,true);

    /* join bounds */
    checked_malloc(pr,chbnds,numdbl_t,2*fpp1->dim*(fpp1->dim+1), return NULL;);
    memset(chbnds,0,2*fpp1->dim*(fpp1->dim+1)*sizeof(numdbl_t));
    p=chbnds;
    nchbnds=0;
    checked_malloc(pr,bd12,numdbl_t,2*fpp1->dim, return NULL;);
    memset(bd12,0,2*fpp1->dim*sizeof(numdbl_t));
    for(i=0;i<fpp1->dim;i++){
        bd1=fpp1->bnds+2*i;
		bd2=fpp2->bnds+2*i;
				/* lower bound:   -V_i <= -\underline{V_i} */
		if(*bd1!=-MAX_VARBND && *bd2!=-MAX_VARBND && isfinite(*bd1) && isfinite(*bd2) ){
			   *(bd12+2*i)=fmin(*bd1,*bd2);
			   *p=-(fmin(*bd1,*bd2));    /* -\underline{V_i} */
			   *(p+i+1)=-1;    /* -V_i */
			   p=p+fpp1->dim+1;
		   nchbnds++;
		}
			else *(bd12+2*i)=-MAX_VARBND;

		  /* upper bound:   V_i <= \overline{V_i} */
		if(*(bd1+1)!=MAX_VARBND && *(bd2+1)!=MAX_VARBND && isfinite(*(bd1+1)) && isfinite(*(bd2+1)) ){
			   *(bd12+2*i+1)=fmax(*(bd1+1),*(bd2+1));
			   *p=fmax(*(bd1+1),*(bd2+1));    /* \overline{V_i} */
			   *(p+i+1)=1;    /* V_i */
			   p=p+fpp1->dim+1;
		   nchbnds++;
		}
        else *(bd12+2*i+1)=MAX_VARBND;
    }

   checked_malloc(pr,env,numdbl_t, (nEnv1+nEnv2)*(fpp01->dim+1), return NULL;);
   if(nEnv1>0) memcpy(env, env1, nEnv1*(fpp01->dim+1)*sizeof(numdbl_t));
   if(nEnv2>0) memcpy(env+nEnv1*(fpp01->dim+1), env2, nEnv2*(fpp01->dim+1)*sizeof(numdbl_t));

#if  DEBUG_JOIN
   printf("fppol_convex_hull_new_cons:input: \n");
   fpp_fdump(stdout, pr->man, fpp1);
   fpp_fdump(stdout, pr->man, fpp2);
   fflush(stdout);
#endif


   newCons=fppol_convex_hull_new_cons(pr, fpp1, fpp2, env, nEnv1+nEnv2, chbnds, nchbnds,&nNewCons);

   if(nNewCons>0){
       /* tighten the new relationship as templates */
       fppol_tighten_tmplts(pr, fpp01, fpp02, nNewCons, newCons);
   }

   fpp12=fpp_alloc_urgent(pr, fpp01->dim, fpp01->intdim,nEnv1+nEnv2+ nchbnds +nNewCons);
   memcpy(fpp12->bnds,bd12, 2*fpp1->dim*sizeof(numdbl_t));
   if(nNewCons>0) memcpy(fpp12->cons,newCons,nNewCons*(fpp01->dim+1)*sizeof(numdbl_t));
   memcpy(fpp12->cons+nNewCons*(fpp01->dim+1),env,(nEnv1+nEnv2)*(fpp01->dim+1)*sizeof(numdbl_t));
   memcpy(fpp12->cons+(nEnv1+nEnv2+nNewCons)*(fpp01->dim+1),chbnds,nchbnds*(fpp01->dim+1)*sizeof(numdbl_t));

   fpp12=fppol_reshape_ineqs(pr,true,fpp12,NULL);
   fpp12=redundancy_removal_quasi_syntactic_lastKs(pr, true, fpp12, nchbnds+nEnv2);
   fpp12=redundancy_removal_SLP_lastKs(pr,true,fpp12, fpp12->ncons-(nEnv1+nEnv2+nNewCons-nQuasi), true);

   checked_free(env);
   checked_free(env1);
   checked_free(env2);
   checked_free(newCons);
   checked_free(bd12);
   checked_free(chbnds);

#if DEBUG_JOIN || PRINT_JWRES
printf("fppol_weak_join_envbnds result:: \n");
fpp_fdump(stdout, pr->man, fpp12);
fflush(stdout);
#endif

   fpp_free_internal(pr, fpp1);
   fpp_free_internal(pr, fpp2);
   return fpp12;
}


fpp_t *fppol_weak_join_nenvbnds(fpp_internal_t* pr, fpp_t *fpp01, fpp_t *fpp02)
{
   unsigned i,j,k,nchars,nshift,nrem;
   numdbl_t *env;
   numdbl_t *p,*bd12,*bd1,*bd2,*bd,*chbnds;
   unsigned nBd1,nBd2,nchbnds;
   fpp_t *fpp1,*fpp2;
   fpp_t *fpp12; /* double dimension -> single dimension */
   int superset;
   int ni;
   bool subset;
   bool flag;
   int r;
   unsigned nEnv1, nEnv2,nenv, nQuasi;
   unsigned nNenv1, nNenv2,nNenv;
   numdbl_t *env1,*env2,*nenv1,*nenv2;
   unsigned nNewCons;
   numdbl_t *newCons;
   char *envIndex1,*envIndex2;

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
   printf("fppol_weak_join_nenvbnds:: \n");
   fpp_fdump(stdout, pr->man, fpp01);
   fpp_fdump(stdout, pr->man, fpp02);
   fflush(stdout);
#endif

    checked_malloc(pr,envIndex1,char,fpp01->ncons, return NULL;);
    memset(envIndex1,0,fpp01->ncons*sizeof(char));
    /* polyhedron: non-envelope + bnds for fpp01 */
    env1=fppol_envelope_half(fpp01,fpp02,&nEnv1, &nQuasi,envIndex1);
	/* non-envelope for fpp01 */
    nNenv1=fpp01->ncons-nEnv1;
    checked_malloc(pr,nenv1,numdbl_t,nNenv1*(fpp01->dim+1), return NULL;);
    memset(nenv1,0,nNenv1*(fpp01->dim+1)*sizeof(numdbl_t));
    p=nenv1;
    for(i=0;i<fpp01->ncons;i++){
		if( *(envIndex1+i) == 0 ){
		   memcpy(p, fpp01->cons+ i*(fpp01->dim+1), (fpp01->dim+1)*sizeof(numdbl_t) );
		   p=p+(fpp01->dim+1);
		}
    }
	/* bnd for fpp01 */
    checked_malloc(pr,bd1,numdbl_t,2*fpp01->dim*(fpp01->dim+1), return NULL;);
    memset(bd1,0,2*fpp01->dim*(fpp01->dim+1)*sizeof(numdbl_t));
    p=bd1;
    nBd1=0;
    for(i=0;i<fpp01->dim;i++){
        bd=fpp01->bnds+2*i;
     	  	/* lower bound:   -V_i <= -\underline{V_i} */
		if(*bd!=-MAX_VARBND && isfinite(*bd)){
			   *p=-(*bd);    /* -\underline{V_i} */
			   *(p+i+1)=-1;    /* -V_i */
			   p=p+fpp01->dim+1;
		   nBd1++;
		}
		  /* upper bound:   V_i <= \overline{V_i} */
		if(*(bd+1)!=MAX_VARBND && isfinite(*(bd+1)) ){
			   *p=*(bd+1);    /* \overline{V_i} */
			   *(p+i+1)=1;    /* V_i */
			   p=p+fpp01->dim+1;
		   nBd1++;
		}
    }

   fpp1=fpp_copy_internal(pr, fpp01); /* fpp1 has the same bounds as fpp01 */
   fpp1->cons=(numdbl_t *) realloc(fpp1->cons,(nBd1+nNenv1)*(fpp01->dim+1)*sizeof(numdbl_t));
   fpp1->ncons=nBd1+nNenv1;
   if(nBd1 > 0)
       memcpy(fpp1->cons,bd1, nBd1*(fpp01->dim+1)*sizeof(numdbl_t) );
   memcpy(fpp1->cons+nBd1*(fpp1->dim+1),nenv1, nNenv1*(fpp1->dim+1)*sizeof(numdbl_t) );
   if(nBd1 > 0)
       fpp1=redundancy_removal_quasi_syntactic_firstKs(pr, true, fpp1, nBd1, NULL);
   if(fpp1->ncons > nNenv1)
       fpp1=redundancy_removal_SLP_firstKs(pr,true,fpp1,fpp1->ncons-nNenv1, NULL, true);

   /* polyhedron: non-envelope + bnds for fpp02 */
    checked_malloc(pr,envIndex2,char,fpp02->ncons, return NULL;);
    memset(envIndex2,0,fpp02->ncons*sizeof(char));
    env2=fppol_envelope_half(fpp02,fpp01,&nEnv2, &nQuasi,envIndex2);
	/* non-envelope for fpp01 */
    nNenv2=fpp02->ncons-nEnv2;
    checked_malloc(pr,nenv2,numdbl_t,nNenv2*(fpp02->dim+1), return NULL;);
    memset(nenv2,0,nNenv2*(fpp02->dim+1)*sizeof(numdbl_t));
    p=nenv2;
    for(i=0;i<fpp02->ncons;i++){
		if( *(envIndex2+i) == 0 ){
		   memcpy(p, fpp02->cons+ i*(fpp02->dim+1), (fpp02->dim+1)*sizeof(numdbl_t) );
		   p=p+(fpp02->dim+1);
		}
    }
	/* bnd for fpp02 */
    checked_malloc(pr,bd2,numdbl_t,2*fpp02->dim*(fpp02->dim+1), return NULL;);
    memset(bd2,0,2*fpp02->dim*(fpp02->dim+1)*sizeof(numdbl_t));
    p=bd2;
    nBd2=0;
    for(i=0;i<fpp02->dim;i++){
        bd=fpp02->bnds+2*i;
     	  	/* lower bound:   -V_i <= -\underline{V_i} */
		if(*bd!=-MAX_VARBND && isfinite(*bd)){
		   *p=-(*bd);    /* -\underline{V_i} */
		   *(p+i+1)=-1;    /* -V_i */
		   p=p+fpp02->dim+1;
		   nBd2++;
		}
		  /* upper bound:   V_i <= \overline{V_i} */
		if(*(bd+1)!=MAX_VARBND && isfinite(*(bd+1)) ){
			*p=*(bd+1);    /* \overline{V_i} */
			*(p+i+1)=1;    /* V_i */
			p=p+fpp02->dim+1;
		    nBd2++;
		}
    }
   fpp2=fpp_copy_internal(pr, fpp02); /* fpp2 has the same bounds as fpp02 */
   fpp2->cons=(numdbl_t *) realloc(fpp2->cons,(nBd2+nNenv2)*(fpp02->dim+1)*sizeof(numdbl_t));
   fpp2->ncons=nBd2+nNenv2;
   if(nBd2 > 0)
       memcpy(fpp2->cons, bd2, nBd2*(fpp2->dim+1)*sizeof(numdbl_t) );
   memcpy(fpp2->cons+nBd2*(fpp2->dim+1),nenv2, nNenv2*(fpp2->dim+1)*sizeof(numdbl_t) );

#if  DEBUG_JOIN
   printf("fppol_convex_hull_new_cons:input: fpp2 \n");
   fpp_fdump(stdout, pr->man, fpp2);
   fflush(stdout);
#endif
   if(nBd2 > 0)
       fpp2=redundancy_removal_quasi_syntactic_firstKs(pr, true, fpp2, nBd2, NULL);
   if(fpp2->ncons > nEnv2)
       fpp2=redundancy_removal_SLP_firstKs(pr,true,fpp2,fpp2->ncons-nEnv2,NULL,true);

    /* join bounds */
    checked_malloc(pr,chbnds,numdbl_t,2*fpp1->dim*(fpp1->dim+1), return NULL;);
    memset(chbnds,0,2*fpp1->dim*(fpp1->dim+1)*sizeof(numdbl_t));
    p=chbnds;
    nchbnds=0;
    checked_malloc(pr,bd12,numdbl_t,2*fpp1->dim, return NULL;);
    memset(bd12,0,2*fpp1->dim*sizeof(numdbl_t));
    for(i=0;i<fpp1->dim;i++){
        bd1=fpp1->bnds+2*i;
	bd2=fpp2->bnds+2*i;
     	  	/* lower bound:   -V_i <= -\underline{V_i} */
	if(*bd1!=-MAX_VARBND && *bd2!=-MAX_VARBND && isfinite(*(bd1)) && isfinite(*bd2) ){
           *(bd12+2*i)=fmin(*bd1,*bd2);
    	   *p=-(fmin(*bd1,*bd2));    /* -\underline{V_i} */
    	   *(p+i+1)=-1;    /* -V_i */
    	   p=p+fpp1->dim+1;
	   nchbnds++;
	}
        else *(bd12+2*i)=-MAX_VARBND;

	  /* upper bound:   V_i <= \overline{V_i} */
	if(*(bd1+1)!=MAX_VARBND && *(bd2+1)!=MAX_VARBND && isfinite(*(bd1+1)) && isfinite(*(bd2+1)) ){
           *(bd12+2*i+1)=fmax(*(bd1+1),*(bd2+1));
    	   *p=fmax(*(bd1+1),*(bd2+1));    /* \overline{V_i} */
    	   *(p+i+1)=1;    /* V_i */
    	   p=p+fpp1->dim+1;
	   nchbnds++;
	}
        else *(bd12+2*i+1)=MAX_VARBND;
    }

   checked_malloc(pr,env,numdbl_t, (nEnv1+nEnv2)*(fpp01->dim+1), return NULL;);
   if(nEnv1>0) memcpy(env, env1, nEnv1*(fpp01->dim+1)*sizeof(numdbl_t));
   if(nEnv2>0) memcpy(env+nEnv1*(fpp01->dim+1), env2, nEnv2*(fpp01->dim+1)*sizeof(numdbl_t));

#if  DEBUG_JOIN
   printf("fppol_convex_hull_new_cons:input: \n");
   fpp_fdump(stdout, pr->man, fpp1);
   fpp_fdump(stdout, pr->man, fpp2);
   fflush(stdout);
#endif


   newCons=fppol_convex_hull_new_cons(pr, fpp1, fpp2, NULL, 0, chbnds, nchbnds,&nNewCons);
   if(nNewCons>0){
	for(i=0;i<nNewCons;i++){
	   k=0;
	   for(j=1;j<=fpp1->dim+1;j++){
		if (*(newCons+i*(fpp1->dim+1)) != 0)
		k++;
		if(k>=2)  break;
	   }
	   if(k>=2){
		nNewCons--;
		if(nNewCons == i )
		newCons=(numdbl_t *) realloc(newCons, nNewCons*(fpp1->dim+1)*sizeof(numdbl_t) );
		else memcpy(newCons+i*(fpp1->dim+1), newCons+(i+1)*(fpp1->dim+1), (nNewCons-i)*(fpp1->dim+1)*sizeof(numdbl_t) );
	   }
	}

      /* tighten the new relationship as templates */
      fppol_tighten_tmplts(pr, fpp01, fpp02, nNewCons, newCons);
   }
   fpp12=fpp_alloc_urgent(pr, fpp01->dim, fpp01->intdim,nEnv1+nEnv2+ nchbnds +nNewCons);
   memcpy(fpp12->bnds,bd12, 2*fpp01->dim*sizeof(numdbl_t));
   if(nNewCons>0) memcpy(fpp12->cons,newCons,nNewCons*(fpp01->dim+1)*sizeof(numdbl_t));
   memcpy(fpp12->cons+nNewCons*(fpp01->dim+1),env,(nEnv1+nEnv2)*(fpp01->dim+1)*sizeof(numdbl_t));
   memcpy(fpp12->cons+(nEnv1+nEnv2+nNewCons)*(fpp01->dim+1),chbnds,nchbnds*(fpp01->dim+1)*sizeof(numdbl_t));
#if  DEBUG_JOIN
   printf("before redundancy_removal_quasi_syntactic_lastKs: \n");
   fpp_fdump(stdout, pr->man, fpp12);
   fflush(stdout);
#endif
   fpp12=fppol_reshape_ineqs(pr,true,fpp12,NULL);
   fpp12=redundancy_removal_quasi_syntactic_lastKs(pr, true, fpp12, nchbnds+nEnv2);
#if  DEBUG_JOIN
   printf("after redundancy_removal_quasi_syntactic_lastKs: \n");
   fpp_fdump(stdout, pr->man, fpp12);
   fflush(stdout);
#endif
   fpp12=redundancy_removal_SLP_lastKs(pr,true,fpp12, fpp12->ncons-(nEnv1+nEnv2+nNewCons-nQuasi), true);
#if  DEBUG_JOIN
   printf("after redundancy_removal_SLP_lastKs: \n");
   fpp_fdump(stdout, pr->man, fpp12);
   fflush(stdout);
#endif
   checked_free(env);
   checked_free(env1);
   checked_free(env2);
   checked_free(newCons);
   checked_free(bd12);
   checked_free(chbnds);

#if DEBUG_JOIN || PRINT_JWRES
printf("fppol_weak_join_nenvbnds result:: \n");
fpp_fdump(stdout, pr->man, fpp12);
fflush(stdout);
#endif

   fpp_free_internal(pr, fpp1);
   fpp_free_internal(pr, fpp2);
   return fpp12;
}

fpp_t *fppol_weak_join_envTVPI(fpp_internal_t* pr, fpp_t *fpp01, fpp_t *fpp02)
{
   unsigned i,j,k;
   numdbl_t a;
   numdbl_t *p,*bd12,*bd1,*bd2,*chbnds;
   unsigned nBd1,nBd2,nchbnds;
   fpp_t *fpp12; /* double dimension -> single dimension */
   int superset;
   int ni;
   bool subset;
   bool flag;
   int r;
   unsigned nEnv1, nEnv2,nenv, nQuasi;
   numdbl_t *env1,*env2;
   unsigned  nTVPIcons;
   numdbl_t *TVPIcons;
   numdbl_t B1l_i,B1l_j,B1u_i,B1u_j,B2l_i,B2l_j,B2u_i,B2u_j;

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
   printf("fppol_weak_join_envTVPI:: \n");
   fpp_fdump(stdout, pr->man, fpp01);
   fpp_fdump(stdout, pr->man, fpp02);
   fflush(stdout);
#endif

    /* TVPI constraints */
    checked_malloc(pr,TVPIcons,numdbl_t,2*fpp01->dim*(fpp01->dim-1)*(fpp01->dim+1), return NULL;);
    memset(TVPIcons,0,2*fpp01->dim*(fpp01->dim-1)*(fpp01->dim+1)*sizeof(numdbl_t));
    p=TVPIcons;
    nTVPIcons=0;
    for(i=0;i<fpp01->dim;i++){
        for(j=i+1;j<fpp01->dim;j++){
	   /* i-j plane */
		/*lower bound of xi xj in B1 */
	   B1l_i= *(fpp01->bnds+(i*2));
	   B1u_i= *(fpp01->bnds+(i*2+1));
	   B1l_j= *(fpp01->bnds+(j*2));
	   B1u_j= *(fpp01->bnds+(j*2+1));
		/*lower bound of xi xj in B2 */
	   B2l_i= *(fpp02->bnds+(i*2));
	   B2u_i= *(fpp02->bnds+(i*2+1));
	   B2l_j= *(fpp02->bnds+(j*2));
	   B2u_j= *(fpp02->bnds+(j*2+1));

	   /* 1. left-up */
	   if( (B1l_i != -MAX_VARBND && isfinite(B1l_i) ) && (B1l_i < B2l_i) && (B2u_j != MAX_VARBND && isfinite(B2u_j) ) && (B1u_j < B2u_j) ){
	      *p= num_sub_downward( B2l_i,B1l_i)*B1u_j - num_mul_downward( (B2u_j - B1u_j), B1l_i);
	      *(p+i+1)=num_sub_downward(B1u_j,B2u_j);
	      *(p+j+1)=num_sub_downward( B2l_i,B1l_i);
	      p=p+(fpp01->dim+1);
	      nTVPIcons++;
	   }
	   else if( (B2l_i != -MAX_VARBND && isfinite(B2l_i) ) && (B2l_i < B1l_i) && (B1u_j != MAX_VARBND && isfinite(B1u_j) ) && (B2u_j < B1u_j) ){
	      *p= num_sub_downward( B1l_i,B2l_i)*B2u_j - num_mul_downward( (B1u_j - B2u_j), B2l_i);
	      *(p+i+1)=num_sub_downward(B2u_j,B1u_j);
	      *(p+j+1)=num_sub_downward( B1l_i,B2l_i);
	      p=p+(fpp01->dim+1);
	      nTVPIcons++;
	   }

	   /* 2. left-down */
	   if( (B1l_i != -MAX_VARBND && isfinite(B1l_i) ) && (B1l_i < B2l_i) && (B2l_j != -MAX_VARBND  && isfinite(B2l_j) ) && (B2l_j < B1l_j) ){
	      *p= num_sub_downward( B1l_j,B2l_j) * B2l_i - num_mul_downward( B2l_j, (B2l_i-B1l_i) );
	      *(p+i+1)=B2l_j - B1l_j;
	      *(p+j+1)=num_sub_downward(B1l_i,B2l_i);
	      p=p+(fpp01->dim+1);
	      nTVPIcons++;
	   }
	   else if( (B2l_i != -MAX_VARBND && isfinite(B2l_i)) && (B2l_i < B1l_i) && (B1l_j != -MAX_VARBND && isfinite(B1l_j) ) && (B1l_j < B2l_j) ){
	      *p= num_sub_downward( B2l_j,B1l_j) * B1l_i - num_mul_downward( B1l_j, (B1l_i-B2l_i) );
	      *(p+i+1)=B1l_j - B2l_j;
	      *(p+j+1)=num_sub_downward(B2l_i,B1l_i);
	      p=p+(fpp01->dim+1);
	      nTVPIcons++;
	   }

	   /* 3. right-up */
	   if( (B1u_i != MAX_VARBND && isfinite(B1u_i) ) && (B2u_i < B1u_i) && (B2u_j != MAX_VARBND && isfinite(B2u_j) ) && (B1u_j < B2u_j) ){
	      *p= (B1u_i-B2u_i )*B1u_j + num_sub_downward( B2u_j,B1u_j) * B1u_i ;
	      *(p+i+1)=num_sub_downward( B2u_j,B1u_j);
	      *(p+j+1)=B1u_i-B2u_i;
	      p=p+(fpp01->dim+1);
	      nTVPIcons++;
	   }
	   else if( (B2u_i != MAX_VARBND && isfinite(B2u_i) ) && (B1u_i < B2u_i) && (B1u_j != MAX_VARBND && isfinite(B1u_j) ) && (B2u_j < B1u_j) ){
	      *p= (B2u_i-B1u_i )*B2u_j + num_sub_downward( B1u_j,B2u_j) * B2u_i ;
	      *(p+i+1)=num_sub_downward( B1u_j,B2u_j);
	      *(p+j+1)=B2u_i-B1u_i;
	      p=p+(fpp01->dim+1);
	      nTVPIcons++;
	   }

	   /* 4. right-down */
	   if( (B2l_j != -MAX_VARBND && isfinite(B2l_j) ) && (B2l_j < B1l_j) && (B1u_i != MAX_VARBND && isfinite(B1u_i) ) && (B2u_i < B1u_i) ){
	      *p= num_sub_downward( B1l_j,B2l_j) * B2u_i - num_mul_downward( B2l_j, (B1u_i-B2u_i));
	      *(p+i+1)=num_sub_downward(B1l_j, B2l_j);
	      *(p+j+1)=num_sub_downward(B2u_i, B1u_i);
	      p=p+(fpp01->dim+1);
	      nTVPIcons++;
	   }
	   else if( (B1l_j != -MAX_VARBND && isfinite(B1l_j) ) && (B1l_j < B2l_j) && (B2u_i != MAX_VARBND && isfinite(B2u_i) ) && (B1u_i < B2u_i) ){
	      *p= num_sub_downward( B2l_j,B1l_j) * B1u_i - num_mul_downward( B1l_j, (B2u_i-B1u_i));
	      *(p+i+1)=num_sub_downward(B2l_j, B1l_j);
	      *(p+j+1)=num_sub_downward(B1u_i, B2u_i);
	      p=p+(fpp01->dim+1);
	      nTVPIcons++;
	   }
        }
    }

   /* tighten the new relationship as templates */
   fppol_tighten_tmplts(pr, fpp01, fpp02, nTVPIcons, TVPIcons);

    /* envelope of fpp01 */
    env1=fppol_envelope_half(fpp01,fpp02,&nEnv1, &nQuasi,NULL);
    /* envelope of fpp02 */
    env2=fppol_envelope_half(fpp02,fpp01,&nEnv2, &nQuasi,NULL);

    /* join bounds */
    checked_malloc(pr,chbnds,numdbl_t,2*fpp01->dim*(fpp01->dim+1), return NULL;);
    memset(chbnds,0,2*fpp01->dim*(fpp01->dim+1)*sizeof(numdbl_t));
    p=chbnds;
    nchbnds=0;
    checked_malloc(pr,bd12,numdbl_t,2*fpp01->dim, return NULL;);
    memset(bd12,0,2*fpp01->dim*sizeof(numdbl_t));
    for(i=0;i<fpp01->dim;i++){
        bd1=fpp01->bnds+2*i;
	bd2=fpp02->bnds+2*i;
     	  	/* lower bound:   -V_i <= -\underline{V_i} */
	if( *bd1!=-MAX_VARBND && *bd2!=-MAX_VARBND && isfinite(*bd1) && isfinite(*bd2) ){
           *(bd12+2*i)=fmin(*bd1,*bd2);
    	   *p=-(fmin(*bd1,*bd2));    /* -\underline{V_i} */
    	   *(p+i+1)=-1;    /* -V_i */
    	   p=p+fpp01->dim+1;
	   nchbnds++;
	}
        else *(bd12+2*i)=-MAX_VARBND;

	  /* upper bound:   V_i <= \overline{V_i} */
	if(*(bd1+1)!=MAX_VARBND && *(bd2+1)!=MAX_VARBND && isfinite(*(bd1+1)) && isfinite(*(bd2+1))  ){
           *(bd12+2*i+1)=fmax(*(bd1+1),*(bd2+1));
    	   *p=fmax(*(bd1+1),*(bd2+1));    /* \overline{V_i} */
    	   *(p+i+1)=1;    /* V_i */
    	   p=p+fpp01->dim+1;
	   nchbnds++;
	}
        else *(bd12+2*i+1)=MAX_VARBND;
    }

   fpp12=fpp_alloc_urgent(pr, fpp01->dim, fpp01->intdim,nEnv1+nEnv2+ nchbnds + nTVPIcons);
   memcpy(fpp12->bnds,bd12, 2*fpp01->dim*sizeof(numdbl_t));
   if(nEnv1>0)
	memcpy(fpp12->cons,env1,nEnv1*(fpp01->dim+1)*sizeof(numdbl_t));
   if(nEnv2>0)
	memcpy(fpp12->cons+nEnv1*(fpp01->dim+1),env2,nEnv2*(fpp01->dim+1)*sizeof(numdbl_t));
   if(nchbnds>0)
	memcpy(fpp12->cons+(nEnv1+nEnv2)*(fpp01->dim+1),chbnds,nchbnds*(fpp01->dim+1)*sizeof(numdbl_t));
   if(nTVPIcons>0)
     memcpy(fpp12->cons+(nEnv1+nEnv2+nchbnds)*(fpp01->dim+1),TVPIcons,nTVPIcons*(fpp01->dim+1)*sizeof(numdbl_t));

#if  DEBUG_JOIN
   printf("before redundancy removal:: \n");
   fpp_fdump(stdout, pr->man, fpp12);
   fflush(stdout);
#endif
   fpp12=fppol_reshape_ineqs(pr,true,fpp12,NULL);
   fpp12=redundancy_removal_quasi_syntactic_lastKs(pr, true, fpp12, nEnv2+nchbnds+nTVPIcons);
   fpp12=redundancy_removal_SLP_lastKs(pr,true,fpp12, fpp12->ncons-(nEnv1+nEnv2-nQuasi), true);

#if  DEBUG_JOIN
   printf("after redundancy removal:: \n");
   fpp_fdump(stdout, pr->man, fpp12);
   fflush(stdout);
#endif

#if DEBUG_JOIN || PRINT_JWRES
printf("fppol_weak_join_envTVPI result:: \n");
fpp_fdump(stdout, pr->man, fpp12);
fflush(stdout);
#endif

   checked_free(env1);
   checked_free(env2);
   checked_free(TVPIcons);
   checked_free(bd12);
   checked_free(chbnds);

   return fpp12;
}

numdbl_t *fppol_convex_hull_new_cons(fpp_internal_t* pr, fpp_t *fpp01, fpp_t *fpp02, numdbl_t *env, unsigned nenv, numdbl_t *chbnds, unsigned nchbnds, unsigned *nNewCons)
{
   unsigned i,j,k;
   numdbl_t *p,*bd12,*bd1,*bd2;
   numdbl_t *bnds1_12;
   numdbl_t *cons1;
   numdbl_t *newCons;
   fpp_t *fpp12; /* double dimension -> single dimension */
   int superset;
   int ni;
   bool subset;
   bool flag;
   int r;
   bit_vectors_t *bv;
   unsigned nshift,nrem,nchars;
   char *bitVector;

   fpp12=fppol_construct_convex_hull(pr,fpp01,fpp02,NULL/*env*/,0/*nenv*/);
	/* Kohler's Rule: bits subset */
    /* Initialisation */
    checked_malloc(pr,bv,bit_vectors_t,1, return NULL;);
    bv->vsize=fpp12->ncons/*-nenvs*/;
    bv->nvecs=fpp12->ncons/*-nenvs*/;
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
    fpp12=fppol_reshape_steep_ineqs(pr,true,fpp12,bv);

    /* project out y2, \sigma_1 */
    while(fpp12->dim > fpp01->dim){
	    /* Sound Fourier-Motzkin Elimination */
        k=fm_select(fpp12,fpp01->dim+1,fpp12->dim);
#if DEBUG_JOIN
        printf("fm before remove fm_select k=%d fpp12->ncons=%d\n",k,fpp12->ncons);
        /* fpp_fdump(stdout,pr->man,fpp12); */
        fpp_fdump_withBV(stdout,fpp12,bv);
        fflush(stdout);
#endif
	if(fpp12->dim >= 2*fpp01->dim) fpp12=fm_elimination(pr, true, fpp12, k,bv,&subset);
        else{
            fpp12=fm_elimination_lazy(pr, true, fpp12,k,bv,2*fpp01->dim+1-fpp12->dim,&subset);
	 /*   fpp12=fm_elimination(pr, true, fpp12, k,bv,&subset);  */
        }

	if(fpp12->ncons==0)	   break;
#if DEBUG_JOIN
        if(fpp12->flag==GENERAL_POL) fpp_fdump(stdout,pr->man,fpp12);
        fflush(stdout);
#endif
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
	if(k<=fpp12->dim)
           memmove(fpp12->bnds+2*(k-1),fpp12->bnds+2*k,2*(fpp12->dim+1-k)*sizeof(numdbl_t));
	fpp12->bnds=realloc(fpp12->bnds,2*fpp12->dim*sizeof(numdbl_t));
	    /* Redundancy Removal */
	if(fpp12->dim < 2*fpp01->dim){
	   /* fpp12=bv_redundancy_removal_firstKs_byKohler(pr,true,fpp12,fpp12->ncons,bv);	 */
           fpp12=redundancy_removal_quasi_syntactic_firstKs(pr,true,fpp12,fpp12->ncons,bv);

           if(fpp12->ncons/*-nenv*/ >= MINCONS4BDRR && fpp12->dim>fpp01->dim && fpp12->dim<2*fpp01->dim-1){
			/* using bounds info from fpp12->bnds */
#if DEBUG_JOIN
             printf(" before redundancy_removal_firstKs_byBounds fpp12->ncons=%d\n",fpp12->ncons);
             fflush(stdout);
#endif
             checked_malloc(pr,bnds1_12,numdbl_t,2*fpp12->dim, return NULL;);
             memcpy(bnds1_12,fpp01->bnds,2*fpp01->dim*sizeof(numdbl_t));
             memcpy(bnds1_12+2*fpp01->dim,fpp12->bnds+2*fpp01->dim,2*(fpp12->dim-fpp01->dim)*sizeof(numdbl_t));
             fpp12=redundancy_removal_firstKs_byBounds(pr,true,fpp12,bnds1_12,fpp12->ncons/*-nenv*/,bv);
#if DEBUG_JOIN
             printf(" after redundancy_removal_firstKs_byBounds fpp12->ncons=%d  fpp1 \n",fpp12->ncons);
             fflush(stdout);
#endif
             memcpy(bnds1_12,fpp02->bnds,2*fpp02->dim*sizeof(numdbl_t));
             fpp12=redundancy_removal_firstKs_byBounds(pr,true,fpp12,bnds1_12,fpp12->ncons/*-nenv*/,bv);
	     checked_free(bnds1_12);
#if DEBUG_JOIN
             printf(" after redundancy_removal_firstKs_byBounds fpp12->ncons=%d  fpp2 \n",fpp12->ncons);
             fflush(stdout);
#endif
	   }
	   if(fpp12->ncons/*-nenv*/ >= MINCONS4LPRR && fpp12->dim>fpp01->dim){
              fpp12=fppol_reshape_ineqs(pr,true,fpp12,bv);
#if DEBUG_JOIN
              printf(" before redundancy_removal_SLP_firstKs_withSigma(withEnv) fpp12->ncons=%d\n",fpp12->ncons);
              fflush(stdout);
#endif
              fpp12=redundancy_removal_SLP_firstKs_withEnv(pr,true,fpp12,fpp12->ncons,env,nenv,fpp01->dim,bv,true);
#if DEBUG_JOIN
              printf(" after redundancy_removal_SLP_firstKs_withEnv fpp12->ncons=%d\n",fpp12->ncons);
              fflush(stdout);
#endif
	   }
	}
    }
    if(fpp12->ncons>0){
       fpp12=redundancy_removal_firstKs_byBounds(pr,true,fpp12,fpp01->bnds,fpp12->ncons,NULL);
#if DEBUG_JOIN
       printf("fppol_strong_join after redundancy_removal_firstKs_byBounds fpp1:: \n");
       fpp_fdump(stdout,pr->man,fpp12);
       fflush(stdout);
#endif
       fpp12=redundancy_removal_firstKs_byBounds(pr,true,fpp12,fpp02->bnds,fpp12->ncons,NULL);
#if DEBUG_JOIN
       printf("fppol_strong_join after redundancy_removal_firstKs_byBounds fpp2:: \n");
       fpp_fdump(stdout,pr->man,fpp12);
       fflush(stdout);
#endif
    }
    fpp12->dim=fpp01->dim;
    fpp12->intdim=fpp01->intdim;

	/* tighten the convex hull using envelopes and bounds info */
    if(fpp12->ncons==0 && nenv==0 && nchbnds==0 ){
        fpp12->flag=UNIVERSE_POL;
        checked_free(fpp12->cons);
        checked_free(fpp12->bnds);
        checked_free(env);
        checked_free(chbnds);
        fpp_free_internal(pr, fpp12);
        *nNewCons=0;
        bv_free(bv);
        return NULL;
    }
    if(fpp12->ncons==0){
	checked_malloc(pr,fpp12->cons,numdbl_t,(nenv+nchbnds)*(fpp12->dim+1), return NULL;);
    }
    else{
        fpp12=fppol_reshape_ineqs(pr,true,fpp12,NULL);
        fpp12->cons=realloc(fpp12->cons,(fpp12->ncons+nenv+nchbnds)*(fpp12->dim+1)*sizeof(numdbl_t));
    }
    if(nenv>0)
	memcpy(fpp12->cons+fpp12->ncons*(fpp12->dim+1),env,nenv*(fpp12->dim+1)*sizeof(numdbl_t));

    if(nchbnds>0)
	memcpy(fpp12->cons+(fpp12->ncons+nenv)*(fpp12->dim+1),chbnds,nchbnds*(fpp12->dim+1)*sizeof(numdbl_t));

    fpp12->ncons=fpp12->ncons+nenv+nchbnds;
    fpp12->flag=GENERAL_POL;
#if  DEBUG_JOIN
printf("fppol_strong_join after bound tightening:: \n");
 fpp_fdump(stdout, pr->man, fpp12);
  fflush(stdout);
#endif

              /* fpp12=redundancy_removal_firstKs(pr,true,fpp12,fpp12->ncons-nenv-nchbnds,NULL,true); */
	   /* no need to quasi-syntactic redundancy removal */
           /* because no 0<= \sigma <= 1 in the constructed convex hull,thus no envelope */
    if(fpp12->ncons > nenv+nchbnds)
        fpp12=redundancy_removal_SLP_firstKs(pr,true,fpp12,fpp12->ncons-nenv-nchbnds,NULL,true);

#if DEBUG_JOIN || PRINT_JWRES
printf("fppol_convex_hull_new_cons result:: \n");
printf("the first %d constraints are new.\n", fpp12->ncons-nenv-nchbnds);
fpp_fdump(stdout, pr->man, fpp12);
fflush(stdout);
#endif

    *nNewCons=fpp12->ncons-nenv-nchbnds;
    if(*nNewCons>0){
       checked_malloc(pr,newCons,numdbl_t,(fpp12->ncons-nenv-nchbnds)*(fpp12->dim+1), return NULL;);
       memcpy(newCons,fpp12->cons,*nNewCons * (fpp12->dim+1) * sizeof(numdbl_t));
    }
    else newCons=NULL;

    fpp_free_internal(pr, fpp12);
    bv_free(bv);
    return newCons;
}


/* tighten Templates */
void fppol_tighten_tmplts(fpp_internal_t* pr, fpp_t *fpp01,  fpp_t *fpp02, unsigned nTmplts, numdbl_t *tmplts)
{
   unsigned i,j;
   numdbl_t c,ci,ci1,ci2,cb;
   numdbl_t *l1;
   glp_prob *lp1=NULL;
   glp_prob *lp2=NULL;
   bool nearly;

   lp1=rlp_create_matrix(fpp01);
   if(fpp02!=NULL) lp2=rlp_create_matrix(fpp02);
   for(i=0;i<nTmplts;i++){ /* { max l \in templates subject to P1 } */
      l1=tmplts+i*(fpp01->dim+1);
      if(lp1==NULL)  lp1=rlp_create_matrix(fpp01);

      rlp_set_objective(lp1,GLP_MAX,l1+1);
      lp1=rlp_solve(lp1);
      ci1=rlp_get_optimal_value(lp1,fpp01->bnds,false,&nearly);
      if(nearly==false){
         numitv_t itv1 = numitv_lexp_range(fpp01->dim,l1+1,fpp01->bnds);
         cb = itv1.sup;
	     if(cb<ci1)  ci1=cb;
      }
      ci=ci1;
      if(fpp02!=NULL){
         if(lp2==NULL)  lp2=rlp_create_matrix(fpp02);
         rlp_set_objective(lp2,GLP_MAX,l1+1);
         lp2=rlp_solve(lp2);
         ci2=rlp_get_optimal_value(lp2,fpp02->bnds,false,&nearly);
         if(nearly==false){
            numitv_t itv1 = numitv_lexp_range(fpp02->dim,l1+1,fpp02->bnds);
            cb = itv1.sup;
	        if(cb<ci2)  ci2=cb;
         }
         ci=fmax(ci1,ci2);
      }
       c=*l1;
       if(ci<c){  /* keep e <= c in the result */
	  *l1=ci;
       }
    }
    return;
}

numdbl_t fppol_sparse_factor(fpp_t *fpp)
{
    unsigned i,j,k,n;
    numdbl_t *l;
    numdbl_t factor;

    k=0;
    for(i=0;i<fpp->ncons;i++){
	l=fpp->cons+i*(fpp->dim+1);
        for(j=1;j<fpp->dim+1;j++){
	   if( *(l+j)!=0 )
		k++;
	}
    }
    n=fpp->ncons*fpp->dim;
    factor=(numdbl_t)k/(numdbl_t)n;
    return factor;
}
