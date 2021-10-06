/*
 * bt.c
 *
 * Tightening the bounds of variables soundly in floating point arithmetic:
 *	--1.rigorous linear programming
 *	--2.bound propagation
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
#include <math.h>

#include "rlp.h"
#include "bt.h"
#include "numitv.h"

/*  Bound tightening all variables by rigorous linear programming */
fpp_t* bt_byRLP(fpp_internal_t* pr, bool destructive, fpp_t *fpp, int *rvars, unsigned nvars)
{
    numdbl_t *objVector;
    numdbl_t maxi,mini,bd,m;
    glp_prob *lp;
    fpp_t *fpp1;
    unsigned j,n;
    int i;
    int lpres;
    bool nearly;
    numdbl_t mInt, diff;
    int k;

    if(destructive==false)
		      fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;
    if(rvars==NULL){
       n=fpp->dim;
    }
    else n=nvars;

	/* bound propagation for unbounded variables */
    for(j=0;j<n;j++){
    	if(rvars!=NULL) i=*(rvars+j);
    	else i=j;
    	bd=*(fpp1->bnds+2*i);
    	if(bd==-MAX_VARBND || nvars==1){
               m=bt_half_kthvar_by_bound_propagation(fpp1,i,false);
               if(m>bd) *(fpp1->bnds+2*i)=m;
    	}
    	bd=*(fpp1->bnds+2*i+1);
    	if(bd==MAX_VARBND || nvars==1){
               m=bt_half_kthvar_by_bound_propagation(fpp1,i,true);
               if(m<bd) *(fpp1->bnds+2*i+1)=m;
    	}
    }
    if(nvars==1 )  return fpp1;

	/* bound tightening by RLP */
    checked_malloc(pr,objVector,numdbl_t,fpp1->dim, return NULL;);
    lp= rlp_create_matrix(fpp1);
    for(j=0;j<n;j++){
      	if(rvars!=NULL) i=*(rvars+j);
      	else i=j;
      		/* tighten bounds of the i-th Variable */
      	memset(objVector,0,(fpp1->dim)*sizeof(numdbl_t));
      	*(objVector+i)=1;
	      /* upper bound */
        if(lp==NULL) lp= rlp_create_matrix(fpp1);
        rlp_set_objective(lp,GLP_MAX,objVector);
        lp= rlp_solve(lp);
        maxi=rlp_get_optimal_value(lp,fpp1->bnds,false,&nearly);

        mInt=round(maxi);
        diff=fabs(maxi-mInt);
      	if(diff!=0 && maxi!=0 && mInt!=0) diff=diff/fmax(fabs(maxi), fabs(mInt) );
      	if(nearly==false || (diff!=0 && diff< EPSILONINT) ){
            m=bt_half_kthvar_by_bound_propagation(fpp1,i,true);
      	    if(m<maxi) maxi=m;
        }
    	  if(maxi<*(fpp1->bnds+2*i+1))  *(fpp1->bnds+2*i+1)=maxi;

	      /* lower bound */
        if(lp==NULL)  lp= rlp_create_matrix(fpp1);
        rlp_set_objective(lp,GLP_MIN,objVector);
	      lp=rlp_solve(lp);
        mini=rlp_get_optimal_value(lp,fpp1->bnds,false,&nearly);
        mInt=round(mini);
        diff=fabs(mini-mInt);
      	if(mini!=0 && mInt!=0 && diff!=0) diff=diff/fmax(fabs(mini), fabs(mInt) );
      	if(nearly==false || (diff!=0 && diff< EPSILONINT) ){
            m=bt_half_kthvar_by_bound_propagation(fpp1,i,false);
	          if(m>mini) mini=m;
        }
        if(mini>*(fpp1->bnds+2*i))     *(fpp1->bnds+2*i)=mini;
    }
    checked_free(objVector);
    lp_delete(lp);
    return fpp1;
}

/*  Bound tightening the k0-th to k1-th (from 0-th ) variables by rigorous linear programming */
fpp_t* bt_kVars_byRLP(fpp_internal_t* pr, bool destructive, fpp_t *fpp,unsigned k0,unsigned k1)
{
    numdbl_t *objVector;
    numdbl_t maxi,mini,bd,m;
    glp_prob *lp;
    fpp_t *fpp1;
    unsigned i;
    int lpres;
    bool nearly;
    numdbl_t mInt, diff;

    if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;
	/* bound propagation for unbounded variables */
    for(i=k0;i<=k1;i++){
    	bd=*(fpp1->bnds+2*i);
    	if(bd==-MAX_VARBND){
               m=bt_half_kthvar_by_bound_propagation(fpp1,i,false);
               if(m>bd) *(fpp1->bnds+2*i)=m;
    	}
    	bd=*(fpp1->bnds+2*i+1);
    	if(bd==MAX_VARBND){
               m=bt_half_kthvar_by_bound_propagation(fpp1,i,true);
               if(m<bd) *(fpp1->bnds+2*i+1)=m;
    	}
    }
	  /* bound tightening by RLP */
    checked_malloc(pr,objVector,numdbl_t,fpp1->dim, return NULL;);
    lp= rlp_create_matrix(fpp1);
    for(i=k0;i<=k1;i++){
		    /* tighten bounds of the i-th Variable */
	      memset(objVector,0,(fpp1->dim)*sizeof(numdbl_t));
	      *(objVector+i)=1;
	      /* upper bound */
        if(lp==NULL) lp= rlp_create_matrix(fpp1);
        rlp_set_objective(lp,GLP_MAX,objVector);
        lp= rlp_solve(lp);
        maxi=rlp_get_optimal_value(lp,fpp1->bnds,false,&nearly);
        mInt=round(maxi);
        diff=fabs(maxi-mInt);
      	if(diff!=0 && maxi!=0 && mInt!=0) diff=diff/fmax(fabs(maxi), fabs(mInt) );
      	if(nearly==false || (diff!=0 && diff< EPSILONINT) ){
           m=bt_half_kthvar_by_bound_propagation(fpp1,i,true);
      	   if(m<maxi) maxi=m;
        }
    	  if(maxi<*(fpp1->bnds+2*i+1))  *(fpp1->bnds+2*i+1)=maxi;

	      /* lower bound */
        if(lp==NULL) lp= rlp_create_matrix(fpp1);
        rlp_set_objective(lp,GLP_MIN,objVector);
	      lp=rlp_solve(lp);
        mini=rlp_get_optimal_value(lp,fpp1->bnds,false,&nearly);
        mInt=round(mini);
        diff=fabs(mini-mInt);
      	if(mini!=0 && mInt!=0 && diff!=0) diff=diff/fmax(fabs(mini), fabs(mInt) );
      	if(nearly==false || (diff!=0 && diff< EPSILONINT) ){
           m=bt_half_kthvar_by_bound_propagation(fpp1,i,false);
      	   if(m>mini) mini=m;
        }
        if(mini>*(fpp1->bnds+2*i))     *(fpp1->bnds+2*i)=mini;
    }
    checked_free(objVector);
    lp_delete(lp);
    return fpp1;
}

/*  Bound tightening k-th(start from 0-th) variable by rigorous linear programming */
fpp_t *bt_kthvar_byRLP(fpp_internal_t* pr, bool destructive, fpp_t *fpp,int k)
{
    numdbl_t *objVector;
    glp_prob *lp;
    fpp_t *fpp1;
    numdbl_t mink,maxk,m;
    bool nearly;
    numdbl_t mInt, diff;

    if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;
    checked_malloc(pr,objVector,numdbl_t,fpp1->dim, return NULL;);
    memset(objVector,0,(fpp1->dim)*sizeof(numdbl_t));
    *(objVector+k)=1;
      /* upper bound */
    lp= rlp_create(fpp1,GLP_MAX,objVector);
    lp=rlp_solve(lp);
    maxk=rlp_get_optimal_value(lp,fpp1->bnds,false,&nearly);
    mInt=round(maxk);
    diff=fabs(maxk-mInt);
    if(diff!=0 && maxk!=0 && mInt!=0) diff=diff/fmax(fabs(maxk), fabs(mInt) );
    if(nearly==false || (diff!=0 && diff< EPSILONINT) ){
         m=bt_half_kthvar_by_bound_propagation(fpp1,k,true);
	       if(m<maxk) maxk=m;
    }
    if(maxk<*(fpp1->bnds+2*k+1))     *(fpp1->bnds+2*k+1)=maxk;

      /* lower bound */
    if(lp==NULL) lp= rlp_create_matrix(fpp1);
    rlp_set_objective(lp,GLP_MIN,objVector);
    lp=rlp_solve(lp);
    mink=rlp_get_optimal_value(lp,fpp1->bnds,false,&nearly);
    mInt=round(mink);
    diff=fabs(mink-mInt);
    if(mink!=0 && mInt!=0 && diff!=0) diff=diff/fmax(fabs(mink), fabs(mInt) );
    if(nearly==false || (diff!=0 && diff< EPSILONINT) ){
         m=bt_half_kthvar_by_bound_propagation(fpp1,k,false);
	       if(m>mink) mink=m;
    }
    if(mink>*(fpp1->bnds+2*k))    *(fpp1->bnds+2*k)=mink;

    checked_free(objVector);
    lp_delete(lp);
    return fpp1;
}

/*  Bound tightening k-th(start from 0-th) by bound propagation  */
/*
$$
	\begin{array}{l}
		\mathrm{rounddown};  \\
		t = \sum\limits_{j\neq i, j\in\mathcal{P}} a_j \underline{V_j}
		+\sum\limits_{j\neq i, j\in\mathcal{N}} a_j \overline{V_j}; \\
		\mathrm{roundup}; \\
		\nu = (b-t)/|a_i|; \\
		\mu = -\nu. \\
	\end{array}
$$
*/
fpp_t *bt_kthvar_by_bound_propagation(fpp_internal_t* pr, bool destructive, fpp_t *fpp,int k)
{
    unsigned i,j;
    numdbl_t aik,aij;
    numdbl_t t,mu,v;
    numdbl_t b;
    numdbl_t lowerBoundVj,upperBoundVj;
    fpp_t *fpp1;
    unsigned NbCons;
    bool flag;

    if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;
    NbCons=fpp1->ncons;
    i=0;
    while(i<NbCons){
    	aik=*(fpp1->cons+i*(fpp1->dim+1)+k+1);  /* aik */
    	flag=true;
    	if(aik>0.0 || aik<-0.0){
    		/* rounddown;  fesetround(FE_DOWNWARD); */
    	   t=0;
    	   b=*(fpp1->cons+i*(fpp1->dim+1));  /* b=ci */
    	   for(j=0;j<fpp1->dim;j++){
        		if((int)j==k) continue;
        		aij=*(fpp1->cons+i*(fpp1->dim+1)+j+1);  /* aij */
        		if(aij>0.0){
        		   lowerBoundVj=*(fpp1->bnds+2*j);  /* lower bound of Vj */
        		   if(lowerBoundVj==-MAX_VARBND){
        			flag=false;
        			break;
        		   }
        		   t=num_add_downward( t, num_mul_downward(aij,lowerBoundVj) ); /* t=t+aij*lowerBoundVj; */
        		}
        		else if(aij<-0.0){
        		   upperBoundVj=*(fpp1->bnds+2*j+1);  /* upper bound of Vj */
        		   if(upperBoundVj==MAX_VARBND){
        			flag=false;
        			break;
        		   }
        		   t=num_add_downward( t, num_mul_downward(aij,upperBoundVj) );  /* t=t+aij*upperBoundVj; */
        		}
    	   }
    		/* roundup;   fesetround(FE_UPWARD);  */
    	   if( flag==true && aik>0.0){  /* can be used to tighten upper bound */
        		v=(b-t)/aik;
        		if(v<*(fpp1->bnds+2*k+1)){  /* upper bound of Vk */
        		   *(fpp1->bnds+2*k+1)=v;
        		}
         }
         else if( flag==true && aik<-0.0){
        		v=(b-t)/(-aik);
        		mu=-v;
        		if(mu>*(fpp1->bnds+2*k)){  /* lower bound of Vk */
        		   *(fpp1->bnds+2*k)=mu;
        		}
    	   }
	    }
	    i++;
    }
    return fpp1;
}

/*  Bound tightening k-th(start from 0-th) by bound propagation  */
/*  upbd: true when upbound or false when lowerbound             */
numdbl_t bt_half_kthvar_by_bound_propagation(fpp_t *fpp,unsigned k,bool upbd)
{
    unsigned i,j;
    numdbl_t aik,aij;
    numdbl_t t,mu,v,mu0;
    numdbl_t b,aijb;
    numdbl_t lowerBoundVj,upperBoundVj;
    unsigned NbCons;
    bool flagINF;

    NbCons=fpp->ncons;
    if(upbd==true) mu=*(fpp->bnds+2*k+1);
    else mu=*(fpp->bnds+2*k);
    i=0;

    while(i<NbCons){
    	aik=*(fpp->cons+i*(fpp->dim+1)+k+1);  /* aik */
    	if(upbd==true && aik>0.0){ /* tighten upper bound */
    	   /* rounddown;  fesetround(FE_DOWNWARD); */
    	   t=0.0;
    	   b=*(fpp->cons+i*(fpp->dim+1));  /* b=ci */
    	   flagINF=false;
    	   for(j=0;j<fpp->dim;j++){
        		if(j!=k){
        		   aij=*(fpp->cons+i*(fpp->dim+1)+j+1);  /* aij */
        		   if(aij>0.0){
        	   	      lowerBoundVj=*(fpp->bnds+2*j);  /* lower bound of Vj */
          		      if(lowerBoundVj==-MAX_VARBND){
          	   		      flagINF=true;
                        break;
                    }
        		        aijb=num_mul_downward(aij,lowerBoundVj);
        		        t=num_add_downward( t, aijb); /* t=t+aij*lowerBoundVj; */
        		   }
        		   else if(aij<-0.0){
        		      upperBoundVj=*(fpp->bnds+2*j+1);  /* upper bound of Vj */
        		      if(upperBoundVj==MAX_VARBND){
          	   		   flagINF=true;
                     break;
                  }
        		      aijb=num_mul_downward(aij,upperBoundVj);
        		      t=num_add_downward( t, aijb);  /* t=t+aij*upperBoundVj; */
        		   }
        		}
    	   }
    	   if(flagINF==false){
    		    /* roundup;   fesetround(FE_UPWARD);  */
    	      v=(b-t)/aik;
    	      if(v<mu){  /* upper bound of Vk */
    		        mu=v;
    	      }
    	   }
    	}
    	else if	(upbd==false && aik<-0.0){ /* tighten lower bound */
    		/* rounddown;  fesetround(FE_DOWNWARD); */
    	   t=0;
    	   b=*(fpp->cons+i*(fpp->dim+1));  /* b=ci */
    	   flagINF=false;
    	   for(j=0;j<fpp->dim;j++){
        		if(j!=k){
        		   aij=*(fpp->cons+i*(fpp->dim+1)+j+1);  /* aij */
        		   if(aij>0.0){
        		      lowerBoundVj=*(fpp->bnds+2*j);  /* lower bound of Vj */
        		      if(lowerBoundVj==-MAX_VARBND){
          	   		   flagINF=true;
                     break;
                  }
        		      t=num_add_downward( t,num_mul_downward(aij,lowerBoundVj) ); /* t=t+aij*lowerBoundVj; */
        		   }
        		   else if(aij<-0.0){
        		      upperBoundVj=*(fpp->bnds+2*j+1);  /* upper bound of Vj */
        		      if(upperBoundVj==MAX_VARBND){
        	   		      flagINF=true;
                      break;
                  }
        		      t=num_add_downward( t,num_mul_downward(aij,upperBoundVj) );  /* t=t+aij*upperBoundVj; */
        		   }
        		}
    	   }
    	   if(flagINF==false){
    		     /* roundup;   fesetround(FE_UPWARD);  */
    	      v=(b-t)/(-aik);
    	      mu0=-v;
    	      if(mu0>mu){  /* lower bound of Vk */
    		        mu=mu0;
    	      }
    	   }
    	}
    	i++;
    }
    return (mu==-0.0)?0.0:mu;
}

/*  Bound tightening all variables only by rigorous linear programming */
fpp_t* bt_byPureRLP(fpp_internal_t* pr, bool destructive, fpp_t *fpp, int *rvars, unsigned nvars)
{
    numdbl_t *objVector;
    numdbl_t maxi,mini,bd,m;
    glp_prob *lp;
    fpp_t *fpp1;
    unsigned j,n;
    int i;
    int lpres;
    bool nearly;

    if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;

#if  DEBUG_BT
printf("bt_byPureRLP:: \n");
fpp_fdump(stdout, pr->man, fpp1);
if(rvars!=NULL) {
printf("bt_byPureRLP rvars:  [");
      for(j=0;j<nvars;j++){
	printf("%d,",*(rvars+j));
      }
printf("]\n");
}
fflush(stdout);
#endif

    if(rvars==NULL){
       n=fpp->dim;
    }
    else n=nvars;

	/* bound tightening by RLP */
    checked_malloc(pr,objVector,numdbl_t,fpp1->dim, return NULL;);
    lp= rlp_create_matrix(fpp1);
    for(j=0;j<n;j++){
      	if(rvars!=NULL) i=*(rvars+j);
      	else i=j;
      		/* tighten bounds of the i-th Variable */
      	memset(objVector,0,(fpp1->dim)*sizeof(numdbl_t));
      	*(objVector+i)=1;
	       /* upper bound */
        if(lp==NULL) lp= rlp_create_matrix(fpp1);
        rlp_set_objective(lp,GLP_MAX,objVector);
        lp= rlp_solve(lp);
        maxi=rlp_get_optimal_value(lp,fpp1->bnds,false,&nearly);
    	  if(maxi<*(fpp1->bnds+2*i+1))  *(fpp1->bnds+2*i+1)=maxi;

	      /* lower bound */
        if(lp==NULL) lp= rlp_create_matrix(fpp1);
        rlp_set_objective(lp,GLP_MIN,objVector);
	      lp=rlp_solve(lp);
        mini=rlp_get_optimal_value(lp,fpp1->bnds,false,&nearly);
        if(mini>*(fpp1->bnds+2*i))     *(fpp1->bnds+2*i)=mini;
    }
    checked_free(objVector);
    lp_delete(lp);
    return fpp1;
}

/*****************************************************************************/
/*   exact           */
/*****************************************************************************/
/*  Bound tightening all variables by rigorous linear programming */
fpp_t* bt_byExactLP(fpp_internal_t* pr, bool destructive, fpp_t *fpp, int *rvars, unsigned nvars)
{
    numdbl_t *objVector;
    numdbl_t maxi,mini,bd,m;
    glp_prob *lp;
    fpp_t *fpp1;
    unsigned j,n;
    int i;
    int lpres;
    bool nearly;
    numdbl_t mInt, diff;
    int k;

    if(destructive==false)
		      fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;

#if  DEBUG_BT
printf("bt_byExactLP:: \n");
fpp_fdump(stdout, pr->man, fpp1);
if(rvars!=NULL) {
printf("bt_byExactLP rvars:  [");
      for(j=0;j<nvars;j++){
	printf("%d,",*(rvars+j));
      }
printf("]\n");
}
fflush(stdout);
#endif

    if(rvars==NULL){
       n=fpp->dim;
    }
    else n=nvars;

	  /* bound tightening by RLP */
    checked_malloc(pr,objVector,numdbl_t,fpp1->dim, return NULL;);
    lp= slp_create_matrix(fpp1);
    for(j=0;j<n;j++){
      	if(rvars!=NULL) i=*(rvars+j);
      	else i=j;
      		/* tighten bounds of the i-th Variable */
      	memset(objVector,0,(fpp1->dim)*sizeof(numdbl_t));
      	*(objVector+i)=1;
  	    /* upper bound */
        if(lp==NULL) lp= slp_create_matrix(fpp1);
        maxi=slp_get_optimal_value_exact(lp,GLP_MAX,objVector);
      	if(maxi<*(fpp1->bnds+2*i+1))  *(fpp1->bnds+2*i+1)=maxi;
	      /* lower bound */
        if(lp==NULL)  lp= slp_create_matrix(fpp1);
        mini=slp_get_optimal_value_exact(lp,GLP_MIN,objVector);
        if(mini>*(fpp1->bnds+2*i))     *(fpp1->bnds+2*i)=mini;
    }
    checked_free(objVector);
    lp_delete(lp);
    return fpp1;
}

/*  Bound tightening the k0-th to k1-th (from 0-th ) variables by rigorous linear programming */
fpp_t* bt_kVars_byExactLP(fpp_internal_t* pr, bool destructive, fpp_t *fpp,unsigned k0,unsigned k1)
{
    numdbl_t *objVector;
    numdbl_t maxi,mini,bd,m;
    glp_prob *lp;
    fpp_t *fpp1;
    unsigned i;
    int lpres;
    bool nearly;
    numdbl_t mInt, diff;

    if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;
	/* bound tightening by RLP */
    checked_malloc(pr,objVector,numdbl_t,fpp1->dim, return NULL;);
    lp= slp_create_matrix(fpp1);
    for(i=k0;i<=k1;i++){
    		/* tighten bounds of the i-th Variable */
    	  memset(objVector,0,(fpp1->dim)*sizeof(numdbl_t));
      	*(objVector+i)=1;
    	  /* upper bound */
        if(lp==NULL) lp= slp_create_matrix(fpp1);
        maxi=slp_get_optimal_value_exact(lp,GLP_MAX,objVector);
        if(maxi<*(fpp1->bnds+2*i+1))  *(fpp1->bnds+2*i+1)=maxi;
    	  /* lower bound */
        mini=slp_get_optimal_value_exact(lp,GLP_MIN,objVector);
        if(mini>*(fpp1->bnds+2*i))     *(fpp1->bnds+2*i)=mini;
    }
    checked_free(objVector);
    lp_delete(lp);
    return fpp1;
}

/*  Bound tightening k-th(start from 0-th) variable by rigorous linear programming */
fpp_t *bt_kthvar_byExactLP(fpp_internal_t* pr, bool destructive, fpp_t *fpp,int k)
{
    numdbl_t *objVector;
    glp_prob *lp;
    fpp_t *fpp1;
    numdbl_t mink,maxk,m;
    bool nearly;
    numdbl_t mInt, diff;

    if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;
    checked_malloc(pr,objVector,numdbl_t,fpp1->dim, return NULL;);
    memset(objVector,0,(fpp1->dim)*sizeof(numdbl_t));
    *(objVector+k)=1;

      /* upper bound */
    lp= slp_create_matrix(fpp1);
    maxk=slp_get_optimal_value_exact(lp,GLP_MAX,objVector);
    if(maxk<*(fpp1->bnds+2*k+1))     *(fpp1->bnds+2*k+1)=maxk;

      /* lower bound */
    if(lp==NULL) lp= slp_create_matrix(fpp1);
    mink=slp_get_optimal_value_exact(lp,GLP_MIN,objVector);
    if(mink>*(fpp1->bnds+2*k))    *(fpp1->bnds+2*k)=mink;

    checked_free(objVector);
    lp_delete(lp);
    return fpp1;
}
