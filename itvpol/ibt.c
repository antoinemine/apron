/*
 * ibt.c
 *
 * Tightening the bounds of variables soundly in floating point arithmetic:
 *	--1.interval linear programming 
 *	--2.bound propagation
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
#include <math.h>

#include "ilp.h"
#include "ibt.h"
#include "numitv.h"

/*****************************************************************************/
/*   exact interval LP          */
/*****************************************************************************/
/*  Bound tightening all variables by exact interval linear programming */
itvp_t* bt_byExactILP(itvp_internal_t* pr, bool destructive, itvp_t *itvp, int *rvars, unsigned nvars)
{
    numdbl_t *objVector;
    numdbl_t maxi,mini,bd,m;
    itvp_t *itvp1;
    unsigned j,n;
    int i;
    int lpres;
    bool nearly;
    numdbl_t mInt, diff;
    int k;
    
    if(destructive==false)
		itvp1=itvp_copy_internal(pr, itvp);
    else	itvp1=itvp;

    if(rvars==NULL){
       n=itvp->dim;      
    }
    else n=nvars; 
 
	/* bound tightening by ILP */
    checked_malloc(pr,objVector,numdbl_t,2*itvp1->dim,return NULL;);
    for(j=0;j<n;j++){
		if(rvars!=NULL) i=*(rvars+j);
		else i=j;
			/* tighten bounds of the i-th Variable */
		memset(objVector,0,(2*itvp1->dim)*sizeof(numdbl_t));
		*(objVector+2*i)=1;
		*(objVector+2*i+1)=1;
	    /* upper bound */
        maxi=ilp_rig(itvp1,GLP_MAX,objVector);
    	if(maxi<*(itvp1->bnds+2*i+1))  *(itvp1->bnds+2*i+1)=maxi;

	    /* lower bound */
        mini=ilp_rig(itvp1,GLP_MIN,objVector);
        if(mini>*(itvp1->bnds+2*i))     *(itvp1->bnds+2*i)=mini;
    }
    checked_free(objVector);
    return itvp1;
}

/*  Bound tightening the k0-th to k1-th (from 0-th ) variables by rigorous linear programming */
itvp_t* bt_kVars_byExactILP(itvp_internal_t* pr, bool destructive, itvp_t *itvp,unsigned k0,unsigned k1)
{
    numdbl_t *objVector;
    numdbl_t maxi,mini;
    itvp_t *itvp1;
    unsigned i;

    if(destructive==false)
		itvp1=itvp_copy_internal(pr, itvp);
    else	itvp1=itvp;
	/* bound tightening by RLP */
    checked_malloc(pr,objVector,numdbl_t,2*itvp1->dim,return NULL;);
    for(i=k0;i<=k1;i++){
		/* tighten bounds of the i-th Variable */
	memset(objVector,0,(2*itvp1->dim)*sizeof(numdbl_t));
	*(objVector+2*i)=1;
	*(objVector+2*i+1)=1;
	    /* upper bound */
        maxi=ilp_rig(itvp1,GLP_MAX,objVector);
    	if(maxi<*(itvp1->bnds+2*i+1))  *(itvp1->bnds+2*i+1)=maxi;

	    /* lower bound */
        mini=ilp_rig(itvp1,GLP_MIN,objVector);
        if(mini>*(itvp1->bnds+2*i))     *(itvp1->bnds+2*i)=mini;
    }
    checked_free(objVector);
    return itvp1;
}

/*  Bound tightening k-th(start from 0-th) variable by rigorous linear programming */
itvp_t *bt_kthvar_byExactILP(itvp_internal_t* pr, bool destructive, itvp_t *itvp,int k)
{
    numdbl_t *objVector;
    glp_prob *lp;
    itvp_t *itvp1;
    numdbl_t mink,maxk;
    bool nearly;

    if(destructive==false)
		itvp1=itvp_copy_internal(pr, itvp);
    else	itvp1=itvp;
    checked_malloc(pr,objVector,numdbl_t,2*itvp1->dim,return NULL;);
    memset(objVector,0,(2*itvp1->dim)*sizeof(numdbl_t));
    *(objVector+2*k)=1;
    *(objVector+2*k+1)=1;
	    /* upper bound */
    maxk=ilp_rig(itvp1,GLP_MAX,objVector);
    if(maxk<*(itvp1->bnds+2*k+1))  *(itvp1->bnds+2*k+1)=maxk;
	    /* lower bound */
    mink=ilp_rig(itvp1,GLP_MIN,objVector);
    if(mink>*(itvp1->bnds+2*k))     *(itvp1->bnds+2*k)=mink;
    checked_free(objVector);
    return itvp1;
}
