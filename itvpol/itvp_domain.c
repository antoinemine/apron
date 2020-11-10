/*
 * itvpol_domain.c
 *
 * polyhedral operations of bounded polyhedra domain with floating point arithmetic.
 * The bounded polyhedra are represented by linear constraints and bounds of variables.
 * The linear constraint has the form of Ax<=b while the bounds has the form of x0<=x<=x1 
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
#include <fenv.h> /* floating-point lib */
#include <glpk.h> /* glpk lib */

#include "ifm.h"
#include "ilp.h"
#include "numitv.h"
#include "itv_linear.h"
#include "itvp_domain.h"
#include "itvp_user.h"
#include "ibt.h"

/* ================================================== */
/* Interval Polyhedra Domain: Operations */
/* ================================================== */
itvp_t *itvpol_weak_join(itvp_internal_t* pr, itvp_t *itvp01, itvp_t *itvp02)
{
   unsigned i,j,k;
   unsigned nenvs,nchbnds;
   numdbl_t *env, *penv;
   numdbl_t *res,*cons1;
   itvp_t *itvp1; 
   int superset;
   char *env1Index,*env2Index;
   unsigned nCons;
   bool subset;
   bool flag;
   int r;
   numdbl_t *bd1,*bd2,*bd12,*chbnds,*p;

   if(itvp01->flag==UNIVERSE_POL)         return itvp_copy_internal(pr, itvp01);
   else if(itvp02->flag==UNIVERSE_POL)    return itvp_copy_internal(pr, itvp02);
   else if(itvp01->flag==EMPTY_POL )      return itvp_copy_internal(pr, itvp02);
   else if(itvp02->flag==EMPTY_POL )      return itvp_copy_internal(pr, itvp01);

#if  DEBUG_JOIN || PRINT_JWRES
   printf("itvpol_weak_join:: \n");
   itvp_fdump(stdout, pr->man, itvp01);
   itvp_fdump(stdout, pr->man, itvp02);
   fflush(stdout);
#endif

	/* the envelope of itvp1 and itvp2 */	
    checked_malloc(pr,env1Index,char,itvp01->ncons,return NULL;);
    checked_malloc(pr,env2Index,char,itvp02->ncons,return NULL;);	
    memset(env1Index,0,itvp01->ncons*sizeof(char));
    memset(env2Index,0,itvp02->ncons*sizeof(char));
    env=penv=itvpol_envelope(itvp01,itvp02, &nenvs,&superset, env1Index, env2Index);
    if(superset==1){
		checked_free(env);
		checked_free(env1Index);
		checked_free(env2Index);
		return itvp_copy_internal(pr, itvp01);
    }
	else if(superset==2){
		checked_free(env);
		checked_free(env1Index);
		checked_free(env2Index);
		return itvp_copy_internal(pr, itvp02);
    }

    cons1=itvpol_nonenv_interval_combination(itvp01,itvp02, env1Index, env2Index,&nCons);
    itvp1=itvp_alloc_urgent(pr, itvp01->dim, itvp01->intdim, nCons+nenvs+2*itvp01->dim);
    memcpy(itvp1->cons,env,nenvs*(2*itvp01->dim+1)*sizeof(numdbl_t));
    memcpy(itvp1->cons+nenvs*(2*itvp01->dim+1),cons1,nCons*(2*itvp01->dim+1)*sizeof(numdbl_t));

    bd12=itvp1->bnds;
    bd1=itvp01->bnds;
    bd2=itvp02->bnds;

    /* bounds for (x y1 \sigma_1) */
    	/*-- bounds for x= y1+y2 =\sigma_1*x1+(1-\sigma_1)*x2 --*/
    		/*---- upper bound: x<= max{x1,x2}  ----*/
    		/*---- lower bound: x>= min{x1,x2}  ----*/
    for(i=0;i<itvp1->dim;i++){
		if( *(bd1+2*i)==-MAX_VARBND || *(bd2+2*i)==-MAX_VARBND )
			*(bd12+2*i)=-MAX_VARBND;
		else    *(bd12+2*i)  =fmin( *(bd1+2*i), *(bd2+2*i) );
		if( *(bd1+2*i+1)==MAX_VARBND || *(bd2+2*i+1)==MAX_VARBND )
			*(bd12+2*i+1)=MAX_VARBND;
		else *(bd12+2*i+1)=fmax( *(bd1+2*i+1), *(bd2+2*i+1) );
    } 

    checked_malloc(pr,chbnds,numdbl_t,2*itvp01->dim*(2*itvp01->dim+1),return NULL;);
    memset(chbnds,0,2*itvp01->dim*(2*itvp01->dim+1)*sizeof(numdbl_t));
    p=chbnds;
    nchbnds=0;
    for(i=0;i<itvp1->dim;i++){
        bd12=itvp1->bnds+2*i;
     	  	/* lower bound:   -V_i <= -\underline{V_i} */
		if(*bd12!=-MAX_VARBND){
		   *p=-(*bd12);    /* -\underline{V_i} */
		   *(p+2*i+1)=-1;    /* -V_i */
		   *(p+2*i+1+1)=-1;    /* -V_i */
		   p=p+2*itvp1->dim+1;
		   nchbnds++;
		}
		  /* upper bound:   V_i <= \overline{V_i} */
		if(*(bd12+1)!=MAX_VARBND){
		   *p=*(bd12+1);    /* \overline{V_i} */
		   *(p+2*i+1)=1;    /* V_i */
		   *(p+2*i+1+1)=1;    /* V_i */
		   p=p+2*itvp1->dim+1;
		   nchbnds++;
		}
    }

    memcpy(itvp1->cons+(nenvs+nCons)*(2*itvp01->dim+1),chbnds,nchbnds*(2*itvp01->dim+1)*sizeof(numdbl_t));
    itvp1->ncons=nenvs+nCons+nchbnds;
    itvp1->cons=(numdbl_t *)realloc(itvp1->cons,itvp1->ncons*(2*itvp01->dim+1)*sizeof(numdbl_t));

    itvp1=itvpol_reduction(pr,true,itvp1);
    itvp1=redundancy_removal_backward(pr,true,itvp1,true);
#if  DEBUG_JOIN || PRINT_JWRES
    printf("itvpol_weak_join res:: \n");
    itvp_fdump(stdout, pr->man, itvp1);
    fflush(stdout);
#endif
    
    checked_free(env);
    checked_free(env1Index);
    checked_free(env2Index);
    checked_free(chbnds);
    checked_free(cons1);
    return itvp1;
}


/* Envelope of 2 interval Polyhedra                            */
/* reverse itvp1 and itvp2 because it is often the case that itvp1 may be larger itvp2 */
numdbl_t *itvpol_envelope(itvp_t *itvp2, itvp_t *itvp1, unsigned *NbEnvs, int *superset,char *env2Index,char *env1Index)
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

    env2=(numdbl_t *) malloc((itvp2->ncons)*(2*itvp2->dim+1)*sizeof(numdbl_t));
    p2=env2;
    env1=(numdbl_t *) malloc((itvp1->ncons)*(2*itvp1->dim+1)*sizeof(numdbl_t));
    p1=env1;
    nEnv1=0;
    nEnv2=0;
    qsnEnv1=0;
    qsnEnvEQ2=0;
    *superset=0;
	for(i=0;i<itvp2->ncons;i++){ /* {l \in P2 | P1\sqsubseteq l} */
		flag=0;
		l2=itvp2->cons+i*(2*itvp2->dim+1);
		/* Quasi-Syntactic check */
		for(j=0;j<itvp1->ncons;j++){ /* l1: e<=c1 ; l2: e<=c2 */
		   l1=itvp1->cons+j*(2*itvp1->dim+1);
		   if( itvpol_vector_cmp(l1+1,l2+1,2*itvp2->dim)==0 ){
				  if(*l1==0)  *l1=0.0;
			  if(*l2>*l1){ /* keep l2 in the envelope */
				 flag=1;
				 memcpy(p2,l2,(2*itvp2->dim+1)*sizeof(numdbl_t));
				 p2=p2+(2*itvp2->dim+1);
				 nEnv2++;
			 *(env2Index+i)=1;
				 break;
			  }
			  else{   /* keep l1 in the envelope */
			 if(*l1==*l2){
				 qsnEnvEQ2++;
				 *(env2Index+i)=1;
					 }
				 memcpy(p1,l1,(2*itvp1->dim+1)*sizeof(numdbl_t));
				 p1=p1+(2*itvp1->dim+1);
			 qsenv1Index[qsnEnv1]=j;
				 qsnEnv1++;
				 flag=1;
			 *(env1Index+j)=1;
			  }
		   }
		}
		if(flag==0){
		   ci=ilp_rig(itvp1, GLP_MAX, l2+1);
		   c=*l2;
		   if(ci<=c){  /* P2\sqsubseteq l */
			  /* keep i-th linear constraint from itvp1 in the envelope */
			  memcpy(p2,l2,(2*itvp2->dim+1)*sizeof(numdbl_t));
			  p2=p2+(2*itvp2->dim+1);
			  nEnv2++;
			  *(env2Index+i)=1;
			}
		}
    }
    if(nEnv2+qsnEnvEQ2==itvp2->ncons){
    	*superset=1;  /* 2 */
    	checked_free(env2);
        checked_free(env1);
        return NULL;
    }
    if(qsnEnv1==itvp1->ncons){
    	*superset=2;   /* 1 */
    	checked_free(env1);
    	checked_free(env2);
    	return NULL;
    }

    nEnv1=qsnEnv1;
    for(i=0;i<itvp1->ncons;i++){ /* {l \in P2 | P1\sqsubseteq l} */
        flag=0;	
        l1=itvp1->cons+i*(2*itvp1->dim+1);
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
           ci=ilp_rig(itvp2, GLP_MAX, l1+1);
           c=*l1;
           if(ci<=c){  /* P1\sqsubseteq l */ 		   
              memcpy(p1,l1,(2*itvp1->dim+1)*sizeof(numdbl_t));	   
              p1=p1+(2*itvp1->dim+1);
			  nEnv1++;
			  *(env1Index+i)=1;
           }
        }
    }
    if(nEnv1==itvp1->ncons){
		*superset=2;   /* 1 */
		checked_free(env1);
		checked_free(env2);
		return NULL;
    }
    nEnv=nEnv1+nEnv2;
    if(nEnv==0){
        *NbEnvs=0; 
        checked_free(env1);
        checked_free(env2);
        return NULL;
    }
    env1=(numdbl_t *) realloc(env1,nEnv*(2*itvp1->dim+1)*sizeof(numdbl_t));
    env=env1;	
    p1=env+nEnv1*(2*itvp1->dim+1);
		for(i=0;i<nEnv2;i++){
		l2=env2+i*(2*itvp2->dim+1);
			flag=0;
		/* Quasi-Syntactic check */
		for(j=0;j<nEnv1;j++){
		   l1=env1+j*(2*itvp1->dim+1);
		   if( itvpol_vector_cmp(l1+1,l2+1,2*itvp2->dim)==0 ){
			flag=1;
			nEnv--;
				break;
		   }
		}
		if(flag==0){
		   memcpy(p1,l2,(2*itvp1->dim+1)*sizeof(numdbl_t));
		   p1=p1+(2*itvp1->dim+1);
		}
    }
    *NbEnvs=nEnv;
    checked_free(env2);
    env=(numdbl_t *) realloc(env,nEnv*(2*itvp1->dim+1)*sizeof(numdbl_t));
    return env;
}

/* Interval combination of 2 contraints  */
numdbl_t *interval_combination(numdbl_t *l1, numdbl_t *l2, int dim)
{
    int i,j;
    numdbl_t *res;

    res=(numdbl_t *)malloc((2*dim+1)*sizeof(numdbl_t));
    *res=fmax(*l1,*l2);
    for(i=1;i<=dim;i++){
		*(res+2*(i-1)+1)=fmin(*(l1+2*(i-1)+1),*(l2+2*(i-1)+1));
		*(res+2*(i-1)+1+1)=fmax(*(l1+2*(i-1)+1+1),*(l2+2*(i-1)+1+1));
    }
    if( is_universal_constraint(res,dim)==true ){
         free(res);
         return NULL;
    }
    else return res;
}

/* Interval combination of 2 contraints  */
numdbl_t *interval_combination_multi(numdbl_t *l1, numdbl_t *l2, int dim)
{
    int i,j;
    numdbl_t *res;
    numdbl_t a1_l=0,a1_u=0,a2_l=0,a2_u=0;
    numdbl_t a1_l0,a1_u0,a2_l0,a2_u0;
    numdbl_t multiper;
    numdbl_t multiper_sig;
    numdbl_t multiper_tmp;
    bool flag;

    res=(numdbl_t *)malloc((2*dim+1)*sizeof(numdbl_t));
    multiper=0;
    for(i=1;i<=(int)dim;i++){
		a1_l0=*(l1+2*(i-1)+1);
		a2_l0=*(l2+2*(i-1)+1);
		a1_u0=*(l1+2*(i-1)+1+1);
		a2_u0=*(l2+2*(i-1)+1+1);
		if(a1_l0==0 || a2_l0==0)  continue;
		multiper_tmp=a2_l0/a1_l0;
		if(multiper_tmp <= 0)  continue;
		if( (a1_u0*multiper_tmp== a2_u0) && (num_mul_downward(a1_u0,multiper_tmp)== a2_u0)  && (num_mul_downward(a1_l0,multiper_tmp)== a2_l0) ){
		   if(a1_l0==a1_u0){ /* prior to siglton */
			multiper=multiper_tmp;
			a1_l=a1_l0;
			a2_l=a2_l0;
			a1_u=a1_u0;
			a2_u=a2_u0;
			break;
		   }
		   else{
			if(multiper<=0){
				multiper=multiper_tmp;
				a1_l=a1_l0;
				a2_l=a2_l0;
				a1_u=a1_u0;
				a2_u=a2_u0;
			}
		   }
		}
    }
    if(multiper<=0){
       *res=fmax(*l1,*l2);
       for(i=1;i<=(int)dim;i++){
		   *(res+2*(i-1)+1)=fmin(*(l1+2*(i-1)+1),*(l2+2*(i-1)+1));
		   *(res+2*(i-1)+1+1)=fmax(*(l1+2*(i-1)+1+1),*(l2+2*(i-1)+1+1));
       }
       if( is_universal_constraint(res,dim)==true ){
            free(res);
            return NULL;
       }
       else return res;
    }
    else{
       flag=false;
	   if(multiper<1){
		  multiper=a1_l/a2_l;
		  if( (a2_u*multiper== a1_u) && (num_mul_downward(a2_u,multiper)== a1_u)  && (num_mul_downward(a2_l,multiper)== a1_l) ){
				  flag=true;
				  *res=fmax((*l2)*multiper,*l1);
				  for(i=1;i<=(int)dim;i++){
					  *(res+2*(i-1)+1)=fmin(*(l1+2*(i-1)+1),(*(l2+2*(i-1)+1))*multiper);
					  *(res+2*(i-1)+1+1)=fmax(*(l1+2*(i-1)+1+1),(*(l2+2*(i-1)+1+1))*multiper);
				  }
		  }
       }
       if(flag==false){
          *res=fmax((*l1)*multiper,*l2);
          for(i=1;i<=(int)dim;i++){
			  *(res+2*(i-1)+1)=fmin((*(l1+2*(i-1)+1))*multiper,*(l2+2*(i-1)+1));
			  *(res+2*(i-1)+1+1)=fmax((*(l1+2*(i-1)+1+1))*multiper,*(l2+2*(i-1)+1+1));
          }
       }
       if( is_universal_constraint(res,dim)==true ){
            free(res);
            return NULL;
       }
       else return res;
    }
}

/* Interval combination of non-envelope contraints from 2 interval Polyhedra           */
numdbl_t *itvpol_nonenv_interval_combination(itvp_t *itvp1, itvp_t *itvp2, char *env1Index,char *env2Index, unsigned *nCons)
{
    unsigned i,j;
    numdbl_t *res,*p;
    unsigned n;
    unsigned nNenv1,nNenv2;
    numdbl_t *l1,*l2,*l12;

    res=(numdbl_t *)malloc(itvp1->ncons*itvp2->ncons*(2*itvp1->dim+1)*sizeof(numdbl_t));
    p=res;
    n=0;
    for(i=0;i<itvp1->ncons;i++){
		if(*(env1Index+i)==0){  /* non-envelope 1 */
			l1=itvp1->cons+i*(2*itvp1->dim+1);
			for(j=0;j<itvp2->ncons;j++){
			   if(*(env2Index+j)==0){  /* non-envelope 2 */
				  l2=itvp2->cons+j*(2*itvp2->dim+1);
				  l12=interval_combination_multi(l1, l2, itvp1->dim);
				  if(l12!=NULL){
					  memcpy(p,l12,(2*itvp1->dim+1)*sizeof(numdbl_t));
					  free(l12);
					  p=p+(2*itvp1->dim+1);
					  n++;
				  }
			   }
			 }
		}
    }
    res=(numdbl_t *)realloc(res,n*(2*itvp1->dim+1)*sizeof(numdbl_t));
    *nCons=n;
    return res;
}

bool is_universal_constraint(numdbl_t *l, int dim)
{
  int i;
  numitv_t ai;

  if(*l==MAX_VARBND)  return true;
  if(*l<0)  return false;
  for(i=0;i<dim;i++){
     ai.inf=*(l+2*i+1);
     ai.sup=*(l+2*i+2);
     if(ai.inf>0 || ai.sup<0 )
       return false;
  }
  return true;
}

/* vector compare: equal returns 0, otherwise 1 */
int itvpol_vector_cmp(numdbl_t *l1, numdbl_t *l2, int len)
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
} /* itvp_vector_cmp */

/*  Incrementally infeasiblility test by Interval Linear Programming	*/
int itvpol_infeasibility_byILP(itvp_internal_t* pr, itvp_t *itvp)
{
    unsigned i,k;
    unsigned NbCons;
    itvp_t *itvp1;
    numdbl_t *aCons,*l1,*l2;
    numdbl_t t,c1,c2,a1k,a2k;
    numdbl_t c,cmin,cb;
    int res;
    int pos;
    glp_prob *lp;
    int lpres;
    bool nearly;

    itvp1=itvp_copy_internal(pr, itvp);
    NbCons=itvp->ncons;
    if(NbCons==1)    return false;
	for(i=1;i<NbCons;i++){
		itvp1->ncons=i;
		aCons=itvp->cons+i*(2*itvp->dim+1);
		c=*aCons;
		aCons++;
		cmin=ilp_rig(itvp1,GLP_MIN,aCons);
		if(cmin>c){
			return true;
		}
    }	 
    itvp_free_internal(pr,itvp1);
    return false;
}

/* Interval Polyhedra Intersection    	 */
/* implemented by redundancy removal	 */
itvp_t *itvpol_intersection(itvp_internal_t* pr, bool destructive, itvp_t *itvp1, itvp_t *itvp2)
{
    unsigned i,res;
    itvp_t *itvp3;
    
    if(itvp1->flag==UNIVERSE_POL){
       if(destructive==true) itvp_free_internal(pr, itvp1);       
       return itvp_copy_internal(pr, itvp2);
    }
    else if(itvp2->flag==UNIVERSE_POL){
       if(destructive==true) return itvp1;
       else return itvp_copy_internal(pr, itvp1);
    } 
    else if(itvp1->flag==EMPTY_POL || itvp2->flag==EMPTY_POL){
       if(destructive==true) itvp_free_internal(pr, itvp1);
       return itvp_alloc_internal(pr,itvp2->dim,itvp2->intdim);
    }

    if(destructive==false) itvp3=itvp_copy_internal(pr, itvp1);
    else itvp3=itvp1;
	/*Constraints union */    
    itvp3->cons=(numdbl_t *)realloc(itvp3->cons,(itvp3->ncons+itvp2->ncons)*(2*itvp3->dim+1)*sizeof(numdbl_t));
    memcpy(itvp3->cons+itvp3->ncons*(2*itvp3->dim+1), itvp2->cons,itvp2->ncons*(2*itvp3->dim+1)*sizeof(numdbl_t));
    itvp3->ncons=itvp1->ncons+itvp2->ncons;
	/*Bounds intersection */
    for(i=0;i<itvp3->dim;i++){
    	*(itvp3->bnds+2*i)=fmax(*(itvp1->bnds+2*i),*(itvp2->bnds+2*i)); /* lower bound */
        *(itvp3->bnds+2*i+1)=fmin(*(itvp1->bnds+2*i+1),*(itvp2->bnds+2*i+1));  /* upper bound */
    }
        /* interval polyhedron feasibility */
         /* bound check */
    for(i=0;i<itvp3->dim;i++){
		if( *(itvp3->bnds+2*i) > *(itvp3->bnds+2*i+1) ){
		   itvp3->flag=EMPTY_POL;
		   checked_free(itvp3->cons);
		   checked_free(itvp3->bnds);
		   itvp3->ncons=0;
           return itvp3;
        } 
    }
    res=itvpol_infeasibility_byILP(pr,itvp3);
    if(res==1){
	   itvp3->flag=EMPTY_POL;
	   checked_free(itvp3->cons);
	   checked_free(itvp3->bnds);
	   itvp3->ncons=0;
       return itvp3;
    }
#if LAZY_RR
    itvp3=redundancy_removal_quasi_syntactic(pr,true,itvp3); /* lazy redundancy_removal*/
#else
    itvp3=redundancy_removal_backward(pr,true,itvp3, true); 
#endif
    itvp3=bt_byExactILP(pr,true, itvp3, NULL,0);
    return itvp3;
}

/* Envelope from itvp1 with respect to itvp2   */
/*      for Polyhedral Widening              */
/* $${l \in P1 | P2\sqsubseteq l}$$ */
numdbl_t *itvpol_envelope_half(itvp_t *itvp1, itvp_t *itvp2, unsigned *NbEnvs, unsigned *NbQuasi,
char *envIndex)
{
    unsigned quasiInd[1000],nquasi;
    unsigned i,j,flag;
    int lpres;
    numdbl_t *p,*env,*l1,*l2;
    numdbl_t c,ci,cb;
    unsigned nEnv;
    bool nearly;

    env=p=(numdbl_t *) malloc( (itvp1->ncons)*(2*itvp1->dim+1)*sizeof(numdbl_t) );
    nEnv=0;
    nquasi=0;	
    memset(quasiInd,0,(itvp2->ncons+1)*sizeof(unsigned));
	/* Quasi Envelope */
    for(i=0;i<itvp1->ncons;i++){ /* {l \in P1 | P2\sqsubseteq l} */
		l1=itvp1->cons+i*(2*itvp1->dim+1);
		for(j=0;j<itvp2->ncons;j++){ /* {l \in P1 | P2\sqsubseteq l} */
		   l2=itvp2->cons+j*(2*itvp2->dim+1);
		   if( itvpol_vector_cmp(l1+1,l2+1,2*itvp2->dim)==0 ){
				if(*l1==0)	*l1=0.0;
				if(*l1 >= *l2 ){
				   memcpy(p,l1,(2*itvp1->dim+1)*sizeof(numdbl_t));
				   p=p+(2*itvp1->dim+1);
				   nEnv++;
			   nquasi++;
			   quasiInd[nquasi]=i;
			   if( envIndex!=NULL )  *(envIndex+i)=1;
			}
			break;
		   }
		}
		if(nquasi >= itvp2->ncons){
		   break;
		}
    }
    *NbQuasi=nquasi;

    // lp= rlp_create_matrix(itvp2);
    for(i=0;i<itvp1->ncons;i++){ /* {l \in P1 | P2\sqsubseteq l} */
        flag=0;	
		l1=itvp1->cons+i*(2*itvp1->dim+1);
		/* already in Quasi Envelope? */
		for(j=1;j<=nquasi;j++){
		   if(i==quasiInd[j]){
				flag=1;
				break;
		   }
		}
		if(flag==0){
		   // if(lp==NULL)  lp=rlp_create_matrix(itvp2);
			   ci=ilp_rig(itvp2,GLP_MAX,l1+1);
			   c=*l1;
			   if(ci<=c){  /* P2\sqsubseteq l */
			  memcpy(p,l1,(2*itvp1->dim+1)*sizeof(numdbl_t));
			  p=p+(2*itvp1->dim+1);
			  nEnv++;
			  if( envIndex!=NULL )  *(envIndex+i)=1;
			   }
		}
    }	
    *NbEnvs=nEnv;
    if(nEnv==0){
		checked_free(env);
		return NULL;
    }
    env=(numdbl_t *) realloc(env, nEnv*(2*itvp1->dim+1)*sizeof(numdbl_t) );
    return env; 
}

/* Check whether a constraint ( c >= e ) is entailed by an interval polyhedron    */
/* posing Interval Linear Programming to get a sound result maxc=max(e) */
/* If maxc <= c, then entailment happens.                               */
bool itvpol_entailment(itvp_internal_t* pr,itvp_t *itvp, numdbl_t *aCons, bool recalBnds)
{
  numdbl_t c,cmax,cb;
  int lpres;
  bool nearly;
  numdbl_t *obnds;
  unsigned i;

  c=*aCons;
  cmax=ilp_rig(itvp,GLP_MAX,aCons+1);
  if(cmax<=c){
        return true;
  }
  else return false;
}


/* Interval Polyhedron Standard Widening: Pol1 widen Pol2		*/
/*  implemented by syntactic and interval linear programming		*/
/* Note standard widening requires: 1) no equlities; 2) in mininal form */
itvp_t* itvpol_widen_standard(itvp_internal_t* pr, bool destructive, itvp_t *itvp1, itvp_t *itvp2)
{
    unsigned i,j,flag;
    itvp_t *itvp3,*wditvp;
    glp_prob *lp;
    int lpres;
    numdbl_t *p2,*env1,*env2,*env,*l0,*l1,*l2,*cbnd,*pb;
    numdbl_t c,ci,bi,aj;
    unsigned nEnv1,nEnv2,nEnv,nCbnd,NbQuasi;
    int res;

    /* if P1 is empty polyhedron */
    if(itvp1->flag==EMPTY_POL || itvp2->flag==UNIVERSE_POL){
		if(destructive==true) itvp_free_internal(pr,itvp1);
		return itvp_copy_internal(pr,itvp2);
    }

#if LAZY_RR
	/* lazy redundancy removal */
    itvp1=redundancy_removal_backward(pr,true,itvp1,true); 
    itvp2=redundancy_removal_backward(pr,true,itvp2,true); 
#endif

#if  DEBUG_WIDEN || PRINT_JWRES
printf("itvpol_widen_standard:: \n");
itvp_fdump(stdout, pr->man, itvp1);
itvp_fdump(stdout, pr->man, itvp2);
fflush(stdout);
#endif

    env1=itvpol_envelope_half(itvp1,itvp2,&nEnv1,&NbQuasi,NULL);
    if(NbQuasi >= itvp2->ncons){ /* all constraints in Pol2 are already in env1 */
		checked_free(env1);
		if(destructive==true) itvp_free_internal(pr,itvp1);
		return itvp_copy_internal(pr,itvp2);
    }
    checked_malloc(pr,env2,numdbl_t,(itvp2->ncons)*(2*itvp2->dim+1),return NULL;);
    p2=env2;
    nEnv2=0;    
    itvp3=itvp_copy_internal(pr,itvp1);
    /* for each inequality l2 in P2, 
       check inequalities in P1 one by one to see if there exist a inequality l1 s.t.
      ((P1\setminus l1)\cup l2)\sqsubseteq l1 and P1\sq subseteq l2. */
    for(i=0;i<itvp2->ncons;i++){ /* for each l2 \in P2 */
        l2=itvp2->cons+i*(2*itvp2->dim+1); 	
		/* l2 already in env1? Syntactic equivalence */
		flag=0;
		if(nEnv1>0){
		   for(j=0;j<nEnv1;j++){ /* {l \in P1 | P2\sqsubseteq l} */
			  l1=env1+j*(2*itvp1->dim+1);
			  if( itvpol_vector_cmp(l1,l2,2*itvp1->dim+1)==0 ){
				 flag=1;
				 break;
			  }
		   }
		}
		if(flag==0){
			   /* P1\sqsubseteq l2 */
		   res=itvpol_entailment(pr,itvp1,l2,false);
		   if(res==true){
			  /* ((P1\setminus l1)\cup l2)\sqsubtPol->bndsseteq l1 */
			  for(j=0;j<itvp1->ncons;j++){
				 l0=itvp1->cons+j*(2*itvp1->dim+1);
				 l1=itvp3->cons+j*(2*itvp3->dim+1);
				 memcpy(l1,l2,(2*itvp3->dim+1)*sizeof(numdbl_t));
				 res=itvpol_entailment(pr,itvp3,l0,true);
				 if(res==true){
				   /* put l2 in the widening results */
				   memcpy(p2,l2,(2*itvp3->dim+1)*sizeof(numdbl_t));
				   p2=p2+(2*itvp3->dim+1);
				   nEnv2++;
				   /* restore itvp3 */
				   memcpy(l1,l0,(2*itvp3->dim+1)*sizeof(numdbl_t));
				   break;
				 }
					 /* restore itvp3 */
				 memcpy(l1,l0,(2*itvp3->dim+1)*sizeof(numdbl_t));
			  }
		   }
		}
    }
    nCbnd=0;cbnd=NULL; /* without common bounds */
    nEnv=nEnv1+nEnv2+nCbnd;
    if(nEnv1>0){
    	env=env1=realloc(env1,nEnv*(2*itvp2->dim+1)*sizeof(numdbl_t));
        if(nEnv2>0){
        	memcpy(env+nEnv1*(2*itvp2->dim+1),env2,nEnv2*(2*itvp2->dim+1)*sizeof(numdbl_t));
        }	
        checked_free(env2);
        if(nCbnd>0){
        	memcpy(env+(nEnv1+nEnv2)*(2*itvp2->dim+1),cbnd,nCbnd*(2*itvp2->dim+1)*sizeof(numdbl_t));
        }	
    }
    else if(nEnv2>0){
    	env=env2=realloc(env2,nEnv2*(2*itvp2->dim+1)*sizeof(numdbl_t));
        if(nCbnd>0){
		  memcpy(env+nEnv2*(2*itvp2->dim+1),cbnd,nCbnd*(2*itvp2->dim+1)*sizeof(numdbl_t));
		}
    }
    else if(nCbnd>0){
		env=(numdbl_t *)malloc(nCbnd*(2*itvp2->dim+1)*sizeof(numdbl_t));
		memcpy(env,cbnd,nCbnd*(2*itvp2->dim+1)*sizeof(numdbl_t));
		checked_free(env1);
		checked_free(env2);
    }
    else{
		checked_free(env1);
		checked_free(env2);
		itvp_free_internal(pr,itvp3);
		return itvp_alloc_top(pr, itvp1->dim, itvp1->intdim);
    }
    wditvp=itvp_alloc_urgent( pr, itvp1->dim, itvp1->intdim,nEnv);
    memcpy(wditvp->cons,env,nEnv*(2*itvp1->dim+1)*sizeof(numdbl_t));
    /* recalculate bounds */ 
    for(i=0;i<wditvp->dim;i++){ 
    	*(wditvp->bnds+2*i)=-MAX_VARBND; 
        *(wditvp->bnds+2*i+1)=MAX_VARBND; 
    }	
    for(i=0;i<nCbnd;i++){ 
	   pb=cbnd+i*(wditvp->dim+1);
	   bi=*pb;
       for(j=1;j<=wditvp->dim;j++){
		   aj=*(pb+j);
		   if(aj==1.0){ /* upper bound */
			  *(wditvp->bnds+2*j-1)=bi;
			  break;
		   }
		   else if(aj==-1.0){ /* lower bound */
			  if(bi==0) *(wditvp->bnds+2*j-2)=0.0;
			  else *(wditvp->bnds+2*j-2)=-bi;
			  break;
		   }
    	}
    }	
    wditvp=bt_byExactILP(pr,true,wditvp,NULL,0);
	 /* standard widening requires: 1) no equlities; 2) in mininal form */	
    wditvp=redundancy_removal_backward(pr,true,wditvp,true);
    checked_free(env);
    if(nCbnd>0)  checked_free(cbnd);
    if(destructive==true)  itvp_free_internal(pr, itvp1);
    itvp_free_internal(pr, itvp3);    
#if  DEBUG_WIDEN || PRINT_JWRES
printf("itvpol_widen_standard result:: \n");
itvp_fdump(stdout, pr->man, wditvp);
fflush(stdout);
#endif
    return wditvp;
}

/* common bounds between itvp1 and itvp2   */
/* return bound constriants              */
numdbl_t* itvpol_common_bounds(itvp_t *itvp1, itvp_t *itvp2, unsigned *NbCmbnds)
{
    numdbl_t *cbnds,*p;	
    numdbl_t *bnd1,*bnd2;
    numdbl_t lbnd1,lbnd2,ubnd1,ubnd2;
    int n;
    int i;

    cbnds=p=(numdbl_t *) malloc(2*itvp1->dim*(2*itvp1->dim+1)*sizeof(numdbl_t) );
    memset(p,0,2*itvp1->dim*(2*itvp1->dim+1)*sizeof(numdbl_t));
    bnd1=itvp1->bnds; 
    bnd2=itvp2->bnds;
    n=0;
    for(i=0;i<(int)itvp1->dim;i++){
        lbnd1=*(bnd1+2*i);
        lbnd2=*(bnd2+2*i);
		if( lbnd1==lbnd2 && lbnd1 != -MAX_VARBND ){
		   if(lbnd1==0) *p=0.0;
		   else *p=-lbnd1;
		   *(p+2*i+1)=-1.0;
		   *(p+2*i+1+1)=-1.0;
		   p=p+(2*itvp1->dim+1);
		   n++;
		}
		ubnd1=*(bnd1+2*i+1);
		ubnd2=*(bnd2+2*i+1);
		if( ubnd1==ubnd2 && ubnd1 != MAX_VARBND ){
		   if(ubnd1==0) *p=0.0;
		   else *p=ubnd1;
		   *(p+2*i+1)=1.0;
		   *(p+2*i+1+1)=1.0;
		   p=p+(2*itvp1->dim+1);
		   n++;
		}
    }
    *NbCmbnds=n;
    cbnds=(numdbl_t *) realloc(cbnds, n*(2*itvp1->dim+1)*sizeof(numdbl_t) ); 
    return cbnds; 
}

/* Polyhedron Standard Widening: Pol1 widen Pol2					*/
/*  implemented by syntactic and rigororous linear programming		*/
/* Note standard widening requires: 1) no equlities; 2) in mininal form */
itvp_t* itvpol_widen_standard_withBound(itvp_internal_t* pr, bool destructive, itvp_t *itvp1, itvp_t *itvp2)
{
    unsigned i,j,flag;
    itvp_t *itvp3,*wditvp;
    glp_prob *lp;
    int lpres;
    numdbl_t *p2,*env1,*env2,*env,*l0,*l1,*l2,*cbnd,*pb;
    numdbl_t c,ci,bi,aj;
    unsigned nEnv1,nEnv2,nEnv,nCbnd,NbQuasi;
    int res;

    /* if P1 is empty polyhedron */
    if(itvp1->flag==EMPTY_POL || itvp2->flag==UNIVERSE_POL){
		if(destructive==true) itvp_free_internal(pr,itvp1);
		return itvp_copy_internal(pr,itvp2);
    }

#if LAZY_RR
	/* lazy redundancy removal */
    itvp1=redundancy_removal_backward(pr,true,itvp1,true); 
    itvp2=redundancy_removal_backward(pr,true,itvp2,true); 
#endif

#if  DEBUG_WIDEN || PRINT_JWRES
printf("itvpol_widen_standard_withBound:: \n");
itvp_fdump(stdout, pr->man, itvp1);
itvp_fdump(stdout, pr->man, itvp2);
fflush(stdout);
#endif

    env1=itvpol_envelope_half(itvp1,itvp2,&nEnv1,&NbQuasi,NULL);
    if(NbQuasi >= itvp2->ncons){ /* all constraints in Pol2 are already in env1 */
		checked_free(env1);
		if(destructive==true) itvp_free_internal(pr,itvp1);
		return itvp_copy_internal(pr,itvp2);
    }
    checked_malloc(pr,env2,numdbl_t,(itvp2->ncons)*(2*itvp2->dim+1),return NULL;);
    p2=env2;
    nEnv2=0;    
    itvp3=itvp_copy_internal(pr,itvp1);
    /* for each inequality l2 in P2, 
       check inequalities in P1 one by one to see if there exist a inequality l1 s.t.
      ((P1\setminus l1)\cup l2)\sqsubseteq l1 and P1\sq subseteq l2. */
    for(i=0;i<itvp2->ncons;i++){ /* for each l2 \in P2 */
		l2=itvp2->cons+i*(2*itvp2->dim+1);
		/* l2 already in env1? Syntactic equivalence */
		flag=0;
		if(nEnv1>0){
		   for(j=0;j<nEnv1;j++){ /* {l \in P1 | P2\sqsubseteq l} */
			  l1=env1+j*(2*itvp1->dim+1);
			  if( itvpol_vector_cmp(l1,l2,2*itvp1->dim+1)==0 ){
				 flag=1;
				 break;
			  }
		   }
		}
		if(flag==0){
			   /* P1\sqsubseteq l2 */
		   res=itvpol_entailment(pr,itvp1,l2,false);
		   if(res==true){
			  /* ((P1\setminus l1)\cup l2)\sqsubtPol->bndsseteq l1 */
			  for(j=0;j<itvp1->ncons;j++){
				 l0=itvp1->cons+j*(2*itvp1->dim+1);
				 l1=itvp3->cons+j*(2*itvp3->dim+1);
				 memcpy(l1,l2,(2*itvp3->dim+1)*sizeof(numdbl_t));
				 res=itvpol_entailment(pr,itvp3,l0,true);
				 if(res==true){
			   /* put l2 in the widening results */
			   memcpy(p2,l2,(2*itvp3->dim+1)*sizeof(numdbl_t));
			   p2=p2+(2*itvp3->dim+1);
			   nEnv2++;
			   /* restore itvp3 */
			   memcpy(l1,l0,(2*itvp3->dim+1)*sizeof(numdbl_t));
			   break;
				 }
					 /* restore itvp3 */
				 memcpy(l1,l0,(2*itvp3->dim+1)*sizeof(numdbl_t));
			  }
		   }
		}
    }
    cbnd=itvpol_common_bounds(itvp1,itvp2,&nCbnd);
    nEnv=nEnv1+nEnv2+nCbnd;
    if(nEnv1>0){
    	env=env1=realloc(env1,nEnv*(2*itvp2->dim+1)*sizeof(numdbl_t));
        if(nEnv2>0){
		  memcpy(env+nEnv1*(2*itvp2->dim+1),env2,nEnv2*(2*itvp2->dim+1)*sizeof(numdbl_t));
		  checked_free(env2);
        }	
        if(nCbnd>0){
        	memcpy(env+(nEnv1+nEnv2)*(2*itvp2->dim+1),cbnd,nCbnd*(2*itvp2->dim+1)*sizeof(numdbl_t));
        }	
    }
    else if(nEnv2>0){
    	env=env2=realloc(env2,nEnv2*(2*itvp2->dim+1)*sizeof(numdbl_t));
        if(nCbnd>0){
		  memcpy(env+nEnv2*(2*itvp2->dim+1),cbnd,nCbnd*(2*itvp2->dim+1)*sizeof(numdbl_t));
		}
    }
    else if(nCbnd>0){
		env=(numdbl_t *)malloc(nCbnd*(2*itvp2->dim+1)*sizeof(numdbl_t));
		memcpy(env,cbnd,nCbnd*(2*itvp2->dim+1)*sizeof(numdbl_t));
    }
    else{
    	checked_free(env2);
        itvp_free_internal(pr,itvp3);
        return itvp_alloc_top(pr, itvp1->dim, itvp1->intdim);
    }
    wditvp=itvp_alloc_urgent( pr, itvp1->dim, itvp1->intdim,nEnv);
    memcpy(wditvp->cons,env,nEnv*(2*itvp1->dim+1)*sizeof(numdbl_t));
    /* recalculate bounds */ 
    for(i=0;i<wditvp->dim;i++){ 
    	*(wditvp->bnds+2*i)=-MAX_VARBND; 
        *(wditvp->bnds+2*i+1)=MAX_VARBND; 
    }	
	for(i=0;i<nCbnd;i++){
		pb=cbnd+i*(wditvp->dim+1);
		bi=*pb;
		for(j=1;j<=wditvp->dim;j++){
		   aj=*(pb+j);
		   if(aj==1.0){ /* upper bound */
			  *(wditvp->bnds+2*j-1)=bi;
			  break;
		   }
		   else if(aj==-1.0){ /* lower bound */
			  if(bi==0) *(wditvp->bnds+2*j-2)=0.0;
			  else *(wditvp->bnds+2*j-2)=-bi;
			  break;
		   }
		}
    }	
    wditvp=bt_byExactILP(pr,true,wditvp,NULL,0);
	 /* standard widening requires: 1) no equlities; 2) in mininal form */	
    wditvp=redundancy_removal_backward(pr,true,wditvp,true);
    if(env!=NULL)  checked_free(env);
    if(nCbnd>0)  checked_free(cbnd);
    if(destructive==true)  itvp_free_internal(pr, itvp1);
    itvp_free_internal(pr, itvp3);    
#if  DEBUG_WIDEN || PRINT_JWRES
printf("itvpol_widen_standard_withBound result:: \n");
itvp_fdump(stdout, pr->man, wditvp);
fflush(stdout);
#endif
    return wditvp;
}

/* Inclusion Test (Entailment)  itvp1 \sqsubseteq  itvp2          	*/
/*  implemented by bound check             		*/
int itvpol_inclusion_byBound(itvp_t *itvp1, itvp_t *itvp2)
{
    int flag;
    numdbl_t lbnd1,ubnd1,lbnd2,ubnd2;
    unsigned i;

    flag=dntknow; /* with the same bounds */

    for(i=0;i<itvp1->dim;i++){
        lbnd1=*(itvp1->bnds+2*i);
        lbnd2=*(itvp2->bnds+2*i);
        ubnd1=*(itvp1->bnds+2*i+1);
        ubnd2=*(itvp2->bnds+2*i+1);
	if(lbnd1 < lbnd2 || ubnd1 > ubnd2)
	  return false;	
    }
    return flag; 
}


/*   Inclusion Test by syntactical check     	*/
bool itvpol_is_leq_bySyntactic(itvp_t *itvp1, itvp_t *itvp2)
{
    int flag;
    numdbl_t *l1,*l2;
    unsigned i,j,n;
    n=0;
    for(i=0;i<itvp2->ncons;i++){
		l2=itvp2->cons+i*(2*itvp2->dim+1);
		for(j=0;j<itvp1->ncons;j++){
		  l1=itvp1->cons+j*(2*itvp1->dim+1);
		  if( itvpol_vector_cmp(l1+1,l2+1,2*itvp1->dim) ==0 ){
			 if(*l1 > *l2)  return false;
			 else{
				n++;
				break;
			 }
		  }
		}
    }
    if(n==itvp2->ncons) return true;
    else return dntknow; 
}

/* Inclusion Test (Entailment)  itvp1 \sqsubseteq  itvp2          	*/
/*  implemented by interval linear programming             		*/
int itvpol_inclusion_byILP(itvp_t *itvp1, itvp_t *itvp2)
{
    unsigned i,j;
    numdbl_t *aCons2,*ei,*l1;    
    numdbl_t mu,b,cb;
    int r;
    bool nearly;

    if(itvp1->flag==EMPTY_POL || itvp2->flag==UNIVERSE_POL)  return true;
    else if(itvp2->flag==EMPTY_POL)    return false;
    else if(itvp1->flag==UNIVERSE_POL)    return false;
    r=itvpol_inclusion_byBound(itvp1, itvp2);
    if(r==false) return r;

    r=itvpol_is_leq_bySyntactic(itvp1, itvp2);
    if(r!=dntknow) return r;
	for(i=0;i<itvp2->ncons;i++){
		aCons2=itvp2->cons+i*(2*itvp2->dim+1);
		for(j=0;j<itvp1->ncons;j++){
		  l1=itvp1->cons+j*(2*itvp1->dim+1);
		  if( itvpol_vector_cmp(l1+1,aCons2+1,2*itvp1->dim) ==0 )
			break;
		}
		if(j<itvp1->ncons) continue;
		b=*aCons2;
		ei=aCons2+1;
		mu=ilp_rig(itvp1,GLP_MAX,ei);
		if(mu>b){
			  return false;
		}
    }
    return true;
}


/*   Equality Test by syntactical check     	*/
bool itvpol_is_eq_bySyntactic(itvp_t *itvp1, itvp_t *itvp2)
{
    int flag;
    numdbl_t *l1,*l2;
    unsigned i,j;

    if( itvp1->flag!=itvp2->flag     || itvp1->dim!=itvp2->dim || 
        itvp1->intdim!=itvp2->intdim || itvp1->ncons!=itvp2->ncons )
	return false;
	for(i=0;i<itvp1->ncons;i++){
		flag=false;
		l1=itvp1->cons+i*(2*itvp1->dim+1);
		for(j=0;j<itvp2->ncons;j++){
		  l2=itvp2->cons+j*(2*itvp2->dim+1);
		  if( itvpol_vector_cmp(l1,l2,2*itvp1->dim+1) ==0 ){
			 flag=true;  break;
		  }
		}
		if(flag==false) return false;
    }
    return true; 
}

/* Forget Function (Interval Polyhedron projection)       	*/
itvp_t *itvpol_project(itvp_internal_t* pr, bool destructive, itvp_t *itvp, int varIndex)
{
    itvp_t *itvp1;
    bool subset;

    if(destructive==false)
		itvp1=itvp_copy_internal(pr, itvp);
    else	itvp1=itvp;

    itvp1=ifm_elimination(pr,true,itvp1, varIndex);

#if LAZY_RR
    if(subset==true)  return itvp1;
    itvp1=redundancy_removal_quasi_syntactic(pr,true,itvp1); 
#else
    itvp1=redundancy_removal_backward(pr,true,itvp1,true);
#endif

    return itvp1;
}

/*  reduce orthants for sign-restricted variables       	*/
itvp_t *itvpol_reduction(itvp_internal_t* pr, bool destructive, itvp_t *itvp)
{
    itvp_t *itvp1;
    numdbl_t *bd;
    numdbl_t *l;
    unsigned i,j;

    if(destructive==false)
		itvp1=itvp_copy_internal(pr, itvp);
    else	itvp1=itvp;

    for( i=0;i<itvp1->dim;i++ ){
       bd=itvp1->bnds+2*i;
       if(*bd>=0){   /* positive */
		  for( j=0; j<itvp1->ncons; j++){
			l=itvp1->cons+j*(2*itvp1->dim+1);
			*(l+2*i+1+1)= *(l+2*i+1);  /* underline */
		  }
       }
       else if(*(bd+1)<=0){   /* negative */
		  for( j=0; j<itvp1->ncons; j++){
			l=itvp1->cons+j*(2*itvp1->dim+1);
			*(l+2*i+1)= *(l+2*i+1+1);  /* overline */
		  }
       }
    }
    return itvp1;
}


/* Polyhedral Convex Hull for interval polyhedra */
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
/* the intermediate repsentation of P_CH with (2*itvp1->dim+1) dimension */
/* c x y1 \sigma_1 */
itvp_t *itvpol_strong_join(itvp_internal_t* pr, itvp_t *itvp01, itvp_t *itvp02)
{
   unsigned i,j,k,nchars,nshift,nrem;
   unsigned nenvs,nchbnds;
   numdbl_t *env, *penv;
   numdbl_t *chbnds,*p,*bd12,*bd1,*bd2;
   numdbl_t *res,*cons1;
   itvp_t *itvp1,*itvp2;
   itvp_t *itvp12; /* double dimension -> single dimension */
   char *bits,*bitVector;
   int superset;
   char *env1Index,*env2Index;
   char *env1bits,*nenv1bits,*env2bits,*nenv2bits,*sigmabits;
   int ni;
   numdbl_t *bnds1_12;
   numdbl_t *bnds12_bak;
   bool subset;
   bool flag;
   int r;

   if(itvp01->flag==UNIVERSE_POL)         return itvp_copy_internal(pr, itvp01);
   else if(itvp02->flag==UNIVERSE_POL)    return itvp_copy_internal(pr, itvp02);
   else if(itvp01->flag==EMPTY_POL )      return itvp_copy_internal(pr, itvp02);
   else if(itvp02->flag==EMPTY_POL )      return itvp_copy_internal(pr, itvp01);

#if  DEBUG_JOIN || PRINT_JWRES
   printf("itvpol_strong_join:: \n");
   itvp_fdump(stdout, pr->man, itvp01);
   itvp_fdump(stdout, pr->man, itvp02);
   fflush(stdout);
#endif

	/* the envelope of itvp1 and itvp2 is certainly part of their convex hull */	
    checked_malloc(pr,env1Index,char,itvp01->ncons,return NULL;);
    checked_malloc(pr,env2Index,char,itvp02->ncons,return NULL;);	
    memset(env1Index,0,itvp01->ncons*sizeof(char));
    memset(env2Index,0,itvp02->ncons*sizeof(char));
    env=penv=itvpol_envelope(itvp01,itvp02, &nenvs,&superset, env1Index, env2Index);
    if(superset==1){
		checked_free(env);
		checked_free(env1Index);
		checked_free(env2Index);

		return itvp_copy_internal(pr, itvp01);
    }
    else if(superset==2){
		checked_free(env);
		checked_free(env1Index);
		checked_free(env2Index);

		return itvp_copy_internal(pr, itvp02);
    }

   itvp1=itvp_copy_internal(pr, itvp01);
   itvp2=itvp_copy_internal(pr, itvp02);

    itvp12=itvpol_construct_convex_hull(pr,itvp1,itvp2,NULL/*env*/,0/*nenvs*/);
    /* bounds info for the first itvp1->dim vars */	
    checked_malloc(pr,bnds12_bak,numdbl_t,2*itvp1->dim,return NULL;);
    memcpy(bnds12_bak,itvp12->bnds,2*itvp1->dim*sizeof(numdbl_t));
    checked_malloc(pr,chbnds,numdbl_t,2*itvp1->dim*(2*itvp1->dim+1),return NULL;);
    memset(chbnds,0,2*itvp1->dim*(2*itvp1->dim+1)*sizeof(numdbl_t));
    p=chbnds;
    nchbnds=0;
    for(i=0;i<itvp1->dim;i++){
        bd12=itvp12->bnds+2*i;
     	  	/* lower bound:   -V_i <= -\underline{V_i} */
		if(*bd12!=-MAX_VARBND){
		   *p=-(*bd12);    /* -\underline{V_i} */
		   *(p+2*i+1)=-1;    /* -V_i */
		   *(p+2*i+2)=-1;    /* -V_i */
		   p=p+2*itvp1->dim+1;
		   nchbnds++;
		}
		  /* upper bound:   V_i <= \overline{V_i} */
		if(*(bd12+1)!=MAX_VARBND){
		   *p=*(bd12+1);    /* \overline{V_i} */
		   *(p+2*i+1)=1;    /* V_i */
		   *(p+2*i+2)=1;    /* V_i */
		   p=p+2*itvp1->dim+1;
		   nchbnds++;
		}
    }

    /* project out y2, \sigma_1 */
	while(itvp12->dim > itvp1->dim){
			/* Sound Fourier-Motzkin Elimination */
		k=ifm_select(itvp12,itvp1->dim+1,itvp12->dim);
		itvp12=ifm_elimination(pr, true, itvp12, k);
		if(itvp12->ncons==0)	   break;
		   /* remove dimension */
		itvp12->dim--;
		if(itvp12->dim < itvp12->intdim) itvp12->intdim--;
		checked_malloc(pr,cons1,numdbl_t,itvp12->ncons*(2*itvp12->dim+1),return NULL;);
		for(j=0;j<itvp12->ncons;j++){
		  memcpy(cons1+j*(2*itvp12->dim+1),itvp12->cons+j*(2*(itvp12->dim+1)+1),(2*(k-1)+1)*sizeof(numdbl_t));
		  if(k<=itvp12->dim)
			memcpy(cons1+j*(2*itvp12->dim+1)+k,itvp12->cons+j*(2*(itvp12->dim+1)+1)+2*(k-1)+1,(2*itvp12->dim+1-(2*(k-1)+1))*sizeof(numdbl_t));
		}
		checked_free(itvp12->cons);
		itvp12->cons=cons1;
		if(k<=itvp12->dim) memmove(itvp12->bnds+2*(k-1),itvp12->bnds+2*k,2*(itvp12->dim+1-k)*sizeof(numdbl_t));
		itvp12->bnds=realloc(itvp12->bnds,2*itvp12->dim*sizeof(numdbl_t));
			/* Redundancy Removal */
		if(itvp12->dim < 2*itvp1->dim){
		   itvp12=itvpol_reduction(pr,true,itvp12);
		   itvp12=redundancy_removal_backward(pr,true,itvp1,true);
		}
    }
    itvp12->dim=itvp1->dim; 
    itvp12->intdim=itvp1->intdim; 
	/* tighten the convex hull using envelopes and bounds info */
    if(itvp12->ncons==0 && nenvs==0 && nchbnds==0 ){
        itvp12->flag=UNIVERSE_POL;
        checked_free(itvp12->cons);     
        checked_free(itvp12->bnds); 
        checked_free(env); 
        checked_free(chbnds); 
		checked_free(env1Index);
		checked_free(env2Index);
        itvp_free_internal(pr, itvp1);
        itvp_free_internal(pr, itvp2);
        return itvp12;
    } 
    if(itvp12->ncons==0){
    	checked_malloc(pr,itvp12->cons,numdbl_t,(nenvs+nchbnds)*(2*itvp12->dim+1),return NULL;);
        itvp12->bnds=bnds12_bak;
    }
    else{ 
        itvp12->cons=realloc(itvp12->cons,(itvp12->ncons+nenvs+nchbnds)*(2*itvp12->dim+1)*sizeof(numdbl_t));
        checked_free(bnds12_bak);
    }
    if(nenvs>0) memcpy(itvp12->cons+itvp12->ncons*(2*itvp12->dim+1),env,nenvs*(2*itvp12->dim+1)*sizeof(numdbl_t));
    if(nchbnds>0) memcpy(itvp12->cons+(itvp12->ncons+nenvs)*(2*itvp12->dim+1),chbnds,nchbnds*(2*itvp12->dim+1)*sizeof(numdbl_t));
    itvp12->ncons=itvp12->ncons+nenvs+nchbnds;
    itvp12->flag=GENERAL_POL;
    itvp12=redundancy_removal_backward(pr,true,itvp12,true);

#if DEBUG_JOIN || PRINT_JWRES
printf("itvpol_strong_join result:: \n");
itvp_fdump(stdout, pr->man, itvp12);
fflush(stdout);
#endif
    checked_free(env); 
    checked_free(chbnds); 
    checked_free(env1Index);
    checked_free(env2Index);
    itvp_free_internal(pr, itvp1);
    itvp_free_internal(pr, itvp2);
    return itvp12;
}


itvp_t *itvpol_construct_convex_hull(itvp_internal_t* pr, itvp_t *itvp1, itvp_t *itvp2, numdbl_t *env, unsigned nenvs)
{
    itvp_t *itvp12; /* double dimension +1 */
    numdbl_t *p1,*p2,*p12;  
    numdbl_t *bd1,*bd2,*q,*bd12;
    numdbl_t *res;
    numdbl_t *penv;
    unsigned i,j;
    int r;
    numdbl_t *c,*bSig;
    numdbl_t dk,maxadbk,dkInt,dkDif,mi;
    bool reshape;

    itvp12=itvp_alloc_urgent(pr,2*itvp1->dim+1,itvp1->intdim+itvp2->intdim,
			   itvp1->ncons+   /* A_1*y_1-b_1*\sigma_1 <= 0 */
               itvp2->ncons+/*+    A_2*x-A_2*y_1+b_2*\sigma_1 <= b_2 */
			   2);/*+    	bounds for (\sigma_1) */
    /*--------------------------------------------------*/
    /* bounds (x y1 \sigma_1) for itvp12                  */
    /*--------------------------------------------------*/
    bd12=itvp12->bnds;
    bd1=itvp1->bnds;
    bd2=itvp2->bnds;
    /* bounds for (x y1 \sigma_1) */
    	/*-- bounds for x= y1+y2 =\sigma_1*x1+(1-\sigma_1)*x2 --*/
    		/*---- upper bound: x<= max{x1,x2}  ----*/
    		/*---- lower bound: x>= min{x1,x2}  ----*/
    for(i=0;i<itvp1->dim;i++){
		if( *(bd1+2*i)==-MAX_VARBND || *(bd2+2*i)==-MAX_VARBND )
			*(bd12+2*i)=-MAX_VARBND;
		else    *(bd12+2*i)  =fmin( *(bd1+2*i), *(bd2+2*i) );
		if( *(bd1+2*i+1)==MAX_VARBND || *(bd2+2*i+1)==MAX_VARBND )
			*(bd12+2*i+1)=MAX_VARBND;
		else *(bd12+2*i+1)=fmax( *(bd1+2*i+1), *(bd2+2*i+1) );
    } 
    bd12=bd12+2*itvp1->dim;
    /*-- bounds for y1=(\sigma_1)*x1 --*/
    	/*---- if \overline{x1}>0    \overline{y1}=\overline{x1}   ----*/
	/*---- if \overline{x1}<=0   \overline{y1}=0 ----*/
	/*---- if \underline{x1}>=0  \underline{y1}=0 ----*/
	/*---- if \underline{x1}<0   \underline{y1}=\underline{x1} ----*/
    for(i=0;i<itvp1->dim;i++){
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
    /* Constraints for itvp12                      */
    /*--------------------------------------------*/
    p12=itvp12->cons;
	/* Constraints from A_1: $A_1*y_1 -b_1*\sigma_1 \leq 0$ from itvp1 */
    p1=itvp1->cons;    
    for(i=0;i<itvp1->ncons;i++){
		*p12=0;  /* c: 0 */
		p12++;
		memset(p12,0,itvp1->dim*2*sizeof(numdbl_t));    /* x: 0 */
		p12=p12+2*itvp1->dim;
		memcpy(p12,p1+1,itvp1->dim*2*sizeof(numdbl_t)); /* A_1*y_1 */
		p12=p12+2*itvp1->dim;
		*p12=-(*p1);  /* -b_1*\sigma_1 */
		*(p12+1)=-(*(p1+1));  /* -b_1*\sigma_1 */
		p12=p12+2;
		p1=p1+2*itvp1->dim+1;
    }

	/* Constraints from A_2: $A_2*x - A_2*y1+b_1*\sigma_1 \leq b_2$ from itvp2 */
    p2=itvp2->cons;    
    for(i=0;i<itvp2->ncons;i++){
		*p12=*p2; /* b_2 */
		*(p12+1+4*itvp2->dim)=*p2;  /* b_2 *\sigma_1 */
		*(p12+1+4*itvp2->dim+1)=*p2;  /* b_2 *\sigma_1 */
		p12++;
		p2++;
		memcpy(p12,p2,2*itvp2->dim*sizeof(numdbl_t));   /* A_2*x */
		p12=p12+2*itvp2->dim;
		for(j=0;j<itvp2->dim;j++){  /* -A_2*y_1 */
		   *p12=-(*(p2+1));
		   *(p12+1)=-(*p2);
		   p12=p12+2;
		   p2=p2+2;
		}
		p12=p12+2;  /* space for \sigma_1 (has already been set ) */
    }
    	/* Constraints from bounds */
    memset(p12,0,2*(2*itvp12->dim+1)*sizeof(numdbl_t));
    bd12=itvp12->bnds+2*2*itvp1->dim;
     	  /* lower bound:   -V_i <= -\underline{V_i} */
    *p12=-(*bd12);    /* -\underline{V_i} */
    *(p12+2*itvp12->dim-1)=-1;    /* -V_i */
    *(p12+2*itvp12->dim)=-1;    /* -V_i */
    p12=p12+2*itvp12->dim+1;
	  /* upper bound:   V_i <= \overline{V_i} */
    *p12=*(bd12+1);    /* \overline{V_i} */
    *(p12+2*itvp12->dim-1)=1;      /* V_i */
    *(p12+2*itvp12->dim)=1;      /* V_i */
    p12=p12+2*itvp12->dim+1;

    return itvp12;
}
