/*
 * ilp.c
 *
 * exact interval linear programming
 *
 * APRON Library / itvPol Domain
 *
 * Copyright (C) Liqian CHEN' 2009
 *
 */
/* This file is part of the APRON Library, released under LGPL license.  
   Please read the COPYING file packaged in the distribution.
*/

/*
 *
 * Notes on GLPK: using exact LP
 *   Inside our library,we use glpk-4.25 which has the implementation of the simplex algorithm
 *   in floating point arithmetic, but glpk-4.25 only supports the double type for the API.
 *   So it is very important to use the double type when using glpk'API.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fenv.h> /* floating-point lib */
#include "ilp.h"
#include "numitv.h"
#include "itvp_config.h"

int nlpcreate_itvp=0;
int nlpdel_itvp=0;
int nlp_itvp=0;
int nrlp_itvp=0;
int nslp_itvp=0;
int nlpjoin_itvp=0;
double lptime_itvp=0;
/* ============================================================ */
/* Construction, Initialization, Deconstrunction */
/* ============================================================ */

void itvp_lp_delete( glp_prob *lp )
{
   nlpdel_itvp++;
   if(lp!=NULL){   
     glp_delete_prob(lp);       
     lp=NULL;
   }
}

/* Construction: Initialized with linear constraints from a polyhedron without objective info */
glp_prob *itvp_slp_create_matrix(fpp_t0 *fpp )
{ 
  glp_prob *lp;
  int *ia, *ja;
  double *ar;/*do not use float here because the glpk API uses double, 
                      otherwise may change the original problem */
  int rows,cols;
  numdbl_t *p,*q;
  int i,j,index;

  nlpcreate_itvp++;
  /*  A: m*n   */
  rows= fpp->ncons;
  cols = fpp->dim;
  ia=(int *)malloc((rows*cols+1)*sizeof(int));
  ja=(int *)malloc((rows*cols+1)*sizeof(int));
  ar=(double *)malloc((rows*cols+1)*sizeof(double));
  p=fpp->cons;
  lp = glp_create_prob();
  glp_add_rows(lp, rows);   
  glp_add_cols(lp, cols);
  /* set bounds of variables x; set coefficients for the objective function   */
  for(i=0;i<cols;i++)  {
      glp_set_col_bnds(lp, i+1, GLP_FR, 0, 0); 
  }  
  /* set b within A*x<=b */
  for(i=0;i<rows;i++)  {
      glp_set_row_bnds(lp, i+1, GLP_UP, 0, *p);
      p=p+(fpp->dim+1);
  }
  /* set A within A*x<=b: : don't import 0.0 term */
  index=1;
  p=fpp->cons;
  for(i=1;i<=rows;i++)  {
      p++;  /* space for constant */
      for(j=1;j<=cols;j++)  {
         if(*p!=0){
            ia[index] = i, ja[index] = j, ar[index] = *p; /* a[i,j] =  *p */
            index++;
	 }	
	 p++;
      }
  }
  glp_load_matrix(lp, index-1, ia, ja, ar);
  checked_free(ia);
  checked_free(ja);
  checked_free(ar);
  return lp;
} 

/*  set objective info */
void itvp_slp_set_objective(glp_prob *lp, unsigned objType, numdbl_t *objVector )
{ 
  int cols;
  int i;

  glp_set_obj_dir(lp, objType);   
  cols=glp_get_num_cols(lp);
  for(i=0;i<cols;i++)  {
      glp_set_obj_coef(lp, i+1, *(objVector+i)); 
  }
} 

numdbl_t itvp_slp_get_optimal_value(glp_prob *lp)
{
   numdbl_t mu;
   mu=glp_get_obj_val(lp);
   return mu==-0.0?0.0:mu;
}

numdbl_t itvp_slp_exact(fpp_t0 *fpp, unsigned objType, numdbl_t *objVector)
{
  glp_prob *lp;
  numdbl_t mu;
  int lpres;

  nlp_itvp++;
  nrlp_itvp++;

  lp= itvp_slp_create_matrix(fpp);
  itvp_slp_set_objective(lp,objType,objVector);

  glp_smcp parm;
  glp_init_smcp(&parm);
  parm.tm_lim= 1000; /* time limit, 1000ms */
  glp_exact(lp,&parm);
  lpres=glp_get_status(lp);

  if(lpres==GLP_OPT){	
     mu=itvp_slp_get_optimal_value(lp);
     itvp_lp_delete(lp);
     return mu==-0.0?0.0:mu;
  }
  else if(lpres==GLP_UNBND){	
     itvp_lp_delete(lp);
     if(objType==GLP_MIN) return -MAX_VARBND;
     else return MAX_VARBND;
  } 
  else if(lpres==GLP_INFEAS){	
     itvp_lp_delete(lp);
     if(objType==GLP_MIN) return MAX_VARBND;
     else return -MAX_VARBND;
  } 
  else if(lpres==GLP_NOFEAS){	
     itvp_lp_delete(lp);
     if(objType==GLP_MIN) return MAX_VARBND;
     else return -MAX_VARBND;
  } 
  else{	
     itvp_lp_delete(lp);
     fprintf(stdout, "err: slp_exact()lpres=%d: neither GLP_OPT nor GLP_UNBND nor GLP_INFEAS nor GLP_NOFEAS.\n",lpres);
     fprintf(stderr, "err: slp_exact()lpres=%d: neither GLP_OPT nor GLP_UNBND nor GLP_INFEAS nor GLP_NOFEAS.\n",lpres);
     if(objType==GLP_MIN) return -MAX_VARBND;
     else return MAX_VARBND;
  } 
}

/* Construction: Initialized with interval linear constraints from an interval polyhedron  */
numdbl_t ilp_rig(itvp_t *itvp, unsigned objType, numdbl_t *objVector)
{ 
  bool flag;
  int i,j,k;
  numdbl_t a_l,a_u;
  char  *split;
  numdbl_t *bnd;
  numdbl_t res;

#if LP_EXACT
  split= (char *) malloc( itvp->dim*sizeof(char));
  memset(split,0,itvp->dim*sizeof(char));
  for(i=0;i<itvp->dim;i++){
     bnd=itvp->bnds+2*i;
     if( (*(bnd)<0) && (*(bnd+1)>0) ){  /* unrestricted in sign (free) */
	/* check whether it is associated with interval coeff */
	flag=false;
	    /* constraint coefficient */
	for(j=0;j<itvp->ncons;j++){		
	    a_l=*(itvp->cons+j*(2*itvp->dim+1)+1+2*i);
	    a_u=*(itvp->cons+j*(2*itvp->dim+1)+1+2*i+1);
	    if(a_l != a_u){
	       flag=true;
	       break;
	    }	  
	}
	if(flag!=true){
	    	/* objective coefficient */
	    a_l=*(objVector+2*i);
	    a_u=*(objVector+2*i+1);
	    if(a_l != a_u)	flag=true;
	}
	if(flag==true){
	   *(split+i)=1;
        }
     }
   }
   res=ilp_orthants_enumerate_rig(itvp, objType, objVector,split,0);
   if(split!=NULL)  free(split);
   return res;

#else 
   return ilp_std(itvp,objType,objVector);
#endif
}

numdbl_t ilp_orthants_enumerate_rig(itvp_t *itvp, unsigned objType, numdbl_t *objVector, char *split, int index)
{
    char *split2;
    numdbl_t res1=0,res2=0,res=0;
    bool flag;
    int i;

    if(index>(int)itvp->dim){
       res=ilp_orthant_rig(itvp, objType, objVector,split);
       return res;
    }
    if(objType==GLP_MIN)       res=MAX_VARBND;
    else if(objType==GLP_MAX )  res=-MAX_VARBND;
    flag=false;
    for(i=index;i<(int)itvp->dim;i++){
	if(*(split+i)==1){
	    flag=true;
		res1=ilp_orthants_enumerate_rig(itvp,objType,objVector,split,i+1);  /* + orthant */
		if(objType==GLP_MIN && res1<res)       res=res1;
		else if(objType==GLP_MAX  && res1>res)       res=res1;
	    split2= (char *) malloc( itvp->dim*sizeof(char));
		memcpy(split2,split,itvp->dim*sizeof(char));
	    *(split2+i)=2;  /* - orthant */
		res2=ilp_orthants_enumerate_rig(itvp,objType,objVector,split2,i+1);  /* - orthant */
	    if(split2!=NULL)  free(split2);
		if(objType==GLP_MIN && res2<res)       res=res2;
		else if(objType==GLP_MAX  && res2>res)       res=res2;
	    break;
	 } 
   }
   if(flag==false)  res=ilp_orthant_rig(itvp, objType, objVector,split);
   return res;
}


/* orthant defined by split */
numdbl_t ilp_orthant_rig(itvp_t *itvp, unsigned objType, numdbl_t *objVector, char *split)
{
  bool flag;
  int i,j,k;
  fpp_t0 *fpp1;
  numdbl_t *objVec1;
  numdbl_t res;

  fpp1=(fpp_t0 *) malloc(sizeof(fpp_t0));
  fpp1->dim   = itvp->dim;
  fpp1->intdim= itvp->intdim;
  fpp1->ncons = itvp->ncons;
  fpp1->cons  = (numdbl_t*) malloc(itvp->ncons*(itvp->dim+1)*sizeof(numdbl_t));
  fpp1->bnds  = (numdbl_t*) malloc(2*itvp->dim*sizeof(numdbl_t));
  memcpy(fpp1->bnds,itvp->bnds,2*itvp->dim*sizeof(numdbl_t));
  fpp1->flag  = itvp->flag; 
  objVec1=(numdbl_t*) malloc(fpp1->dim*sizeof(numdbl_t));
	/* constant term */
  for(i=0;i<(int)fpp1->ncons;i++){
    *(fpp1->cons+i*(fpp1->dim+1))=*(itvp->cons+i*(2*itvp->dim+1));	
  } 

	/* constraint coefficients */
  for(i=0;i<(int)fpp1->dim;i++){
    switch( *(split+i) ){
       case 0:  /* sign-restricted or no interval coefficient */
	    if( *(fpp1->bnds+2*i) >= 0 ){    /* positive  \underline{a} */
				/* coefficient */
			for(j=0;j<(int)fpp1->ncons;j++){
					*(fpp1->cons+j*(fpp1->dim+1)+1+i)=*(itvp->cons+j*(2*itvp->dim+1)+1+2*i);
			}
					/* objective */
			if(objType==GLP_MAX)    *(objVec1+i)=*(objVector+2*i); /* underline{a} */
			else            *(objVec1+i)=*(objVector+2*i+1);   /* overline{a} */
	    }
	    else{   /* negative or no interval coefficient */
				/* coefficient */
			for(j=0;j<(int)fpp1->ncons;j++){  /* \overline{a} */
					*(fpp1->cons+j*(fpp1->dim+1)+1+i)=*(itvp->cons+j*(2*itvp->dim+1)+1+2*i+1);
			}
					/* objective */
			if(objType==GLP_MAX)   *(objVec1+i)=*(objVector+2*i+1); /* overline{a} */
			else            *(objVec1+i)=*(objVector+2*i);   /* underline{a} */
	    } 
	    break;
       case 1:  /* free variable with interval coefficient, + */
	    *(fpp1->bnds+2*i)=0;
		/* coefficient */
  	    for(j=0;j<(int)fpp1->ncons;j++){
    		*(fpp1->cons+j*(fpp1->dim+1)+1+i)=*(itvp->cons+j*(2*itvp->dim+1)+1+2*i);	
  	    }
 		/* objective */
	    if(objType==GLP_MAX)   *(objVec1+i)=*(objVector+2*i); /* underline{a} */
	    else            *(objVec1+i)=*(objVector+2*i+1);   /* overline{a} */ 
	    break;
       case 2:  /* free variable with interval coefficient, - */
	    *(fpp1->bnds+2*i+1)=0;
		/* coefficient */
  	    for(j=0;j<(int)fpp1->ncons;j++){  /* \overline{a} */
    		*(fpp1->cons+j*(fpp1->dim+1)+1+i)=*(itvp->cons+j*(2*itvp->dim+1)+1+2*i+1);	
  	    }
 		/* objective */
	    if(objType==GLP_MAX)   *(objVec1+i)=*(objVector+2*i+1); /* overline{a} */
	    else            *(objVec1+i)=*(objVector+2*i);   /* underline{a} */
	    break;
       default:
	    fprintf(stderr, "ilp_orthant_rig(): orthant spliting is wrong.\n");	
    }
  }	
  /* LP with convex polyhedron fpp1 and objective vector objVec1 */
  res=itvp_slp_exact(fpp1,objType, objVec1);

  if(objVec1!=NULL) free(objVec1);
  if(fpp1!=NULL){ 
     free(fpp1->bnds);
     free(fpp1->cons);
     free(fpp1);
  }
  return res;
} 


numdbl_t itvp_slp_std(fpp_t0 *fpp, unsigned objType, numdbl_t *objVector)
{
  glp_prob *lp;
  numdbl_t mu;
  int lpres;

  nlp_itvp++;
  nslp_itvp++;

  lp= itvp_slp_create_matrix(fpp);
  itvp_slp_set_objective(lp,objType,objVector);

  glp_smcp parm;
  glp_init_smcp(&parm);
  /* parm.presolve = GLP_ON;  parm.tol_bnd = 1e-6;   parm.tol_dj = 1e-6; */
  parm.meth = GLP_DUALP;
  parm.msg_lev = GLP_MSG_ERR;
  lpres = glp_simplex(lp,&parm);
  if (lpres != 0){
      fprintf(stdout, "\nThe LP simplex slover failed! \n");
      glp_delete_prob(lp);
      lp=NULL;
      if(objType==GLP_MIN) return -MAX_VARBND;
      else return MAX_VARBND;
  }

  lpres=glp_get_status(lp);

  if(lpres==GLP_OPT){	
     mu=itvp_slp_get_optimal_value(lp);
     itvp_lp_delete(lp);
     return mu==-0.0?0.0:mu;
  }
  else if(lpres==GLP_UNBND){	
     itvp_lp_delete(lp);
     if(objType==GLP_MIN) return -MAX_VARBND;
     else return MAX_VARBND;
  } 
  else if(lpres==GLP_INFEAS){	
     itvp_lp_delete(lp);
     if(objType==GLP_MIN) return MAX_VARBND;
     else return -MAX_VARBND;
  } 
  else if(lpres==GLP_NOFEAS){	
     itvp_lp_delete(lp);
     if(objType==GLP_MIN) return MAX_VARBND;
     else return -MAX_VARBND;
  } 
  else{	
     itvp_lp_delete(lp);
     fprintf(stdout, "err: slp_exact()lpres=%d: neither GLP_OPT nor GLP_UNBND nor GLP_INFEAS nor GLP_NOFEAS.\n",lpres);
     fprintf(stderr, "err: slp_exact()lpres=%d: neither GLP_OPT nor GLP_UNBND nor GLP_INFEAS nor GLP_NOFEAS.\n",lpres);
     if(objType==GLP_MIN) return -MAX_VARBND;
     else return MAX_VARBND;
  } 
}

/* Construction: Initialized with interval linear constraints from an interval polyhedron  */
numdbl_t ilp_std(itvp_t *itvp, unsigned objType, numdbl_t *objVector)
{ 
  bool flag;
  int i,j,k;
  numdbl_t a_l,a_u;
  char  *split;
  numdbl_t *bnd;
  numdbl_t res;

  split= (char *) malloc( itvp->dim*sizeof(char));
  memset(split,0,itvp->dim*sizeof(char));
  for(i=0;i<(int)itvp->dim;i++){
     bnd=itvp->bnds+2*i;
     if( (*(bnd)<0) && (*(bnd+1)>0 ) ){  /* unrestricted in sign (free) */
		/* check whether it is associated with interval coeff */
		flag=false;
			/* constraint coefficient */
		for(j=0;j<(int)itvp->ncons;j++){
			a_l=*(itvp->cons+j*(2*itvp->dim+1)+1+2*i);
			a_u=*(itvp->cons+j*(2*itvp->dim+1)+1+2*i+1);
			if(a_l != a_u){
			   flag=true;
			   break;
			}
		}
		if(flag!=true){
				/* objective coefficient */
			a_l=*(objVector+2*i);
			a_u=*(objVector+2*i+1);
			if(a_l != a_u)	flag=true;
		}
		if(flag==true){
		   *(split+i)=1;
		}
     }
   }
   res=ilp_orthants_enumerate_std(itvp, objType, objVector,split,0);
   if(split!=NULL)  free(split);
   return res;
}

numdbl_t ilp_orthants_enumerate_std(itvp_t *itvp, unsigned objType, numdbl_t *objVector, char *split, int index)
{
    char *split2;
    numdbl_t res1,res2,res=0;
    bool flag;
    int i;

    if(index>(int)itvp->dim){
       res=ilp_orthant_std(itvp, objType, objVector,split);
       return res;
    }
    if(objType==GLP_MIN)       res=MAX_VARBND;
    else if(objType==GLP_MAX )  res=-MAX_VARBND;
    flag=false;
    for(i=index;i<(int)itvp->dim;i++){
	if(*(split+i)==1){
	    flag=true;
		res1=ilp_orthants_enumerate_std(itvp,objType,objVector,split,i+1);  /* + orthant */
		if(objType==GLP_MIN && res1<res)       res=res1;
		else if(objType==GLP_MAX  && res1>res)       res=res1;
		split2= (char *) malloc( itvp->dim*sizeof(char));
        memcpy(split2,split,itvp->dim*sizeof(char));
	    *(split2+i)=2;  /* - orthant */
        res2=ilp_orthants_enumerate_std(itvp,objType,objVector,split2,i+1);  /* - orthant */
	    if(split2!=NULL)  free(split2);
		if(objType==GLP_MIN && res2<res)       res=res2;
		else if(objType==GLP_MAX  && res2>res)       res=res2;
	    break;
	 } 
   }
   if(flag==false)  res=ilp_orthant_std(itvp, objType, objVector,split);
   return res;
}


/* orthant defined by split */
numdbl_t ilp_orthant_std(itvp_t *itvp, unsigned objType, numdbl_t *objVector, char *split)
{
  bool flag;
  int i,j,k;
  fpp_t0 *fpp1;
  numdbl_t *objVec1;
  numdbl_t res;

  fpp1=(fpp_t0 *) malloc(sizeof(fpp_t0));
  fpp1->dim   = itvp->dim;
  fpp1->intdim= itvp->intdim;
  fpp1->ncons = itvp->ncons;
  fpp1->cons  = (numdbl_t*) malloc(itvp->ncons*(itvp->dim+1)*sizeof(numdbl_t));
  fpp1->bnds  = (numdbl_t*) malloc(2*itvp->dim*sizeof(numdbl_t));
  memcpy(fpp1->bnds,itvp->bnds,2*itvp->dim*sizeof(numdbl_t));
  fpp1->flag  = itvp->flag; 
  objVec1=(numdbl_t*) malloc(fpp1->dim*sizeof(numdbl_t));
	/* constant term */
  for(i=0;i<(int)fpp1->ncons;i++){
    *(fpp1->cons+i*(fpp1->dim+1))=*(itvp->cons+i*(2*itvp->dim+1));	
  } 

	/* constraint coefficients */
  for(i=0;i<(int)fpp1->dim;i++){
    switch( *(split+i) ){
       case 0:  /* sign-restricted or no interval coefficient */
	    if( *(fpp1->bnds+2*i) >= 0 ){    /* positive  \underline{a} */
				/* coefficient */
			for(j=0;j<(int)fpp1->ncons;j++){
					*(fpp1->cons+j*(fpp1->dim+1)+1+i)=*(itvp->cons+j*(2*itvp->dim+1)+1+2*i);
			}
					/* objective */
			if(objType==GLP_MAX)    *(objVec1+i)=*(objVector+2*i); /* underline{a} */
			else            *(objVec1+i)=*(objVector+2*i+1);   /* overline{a} */
	    }
	    else{   /* negative or no interval coefficient */
				/* coefficient */
			for(j=0;j<(int)fpp1->ncons;j++){  /* \overline{a} */
					*(fpp1->cons+j*(fpp1->dim+1)+1+i)=*(itvp->cons+j*(2*itvp->dim+1)+1+2*i+1);
			}
					/* objective */
			if(objType==GLP_MAX)   *(objVec1+i)=*(objVector+2*i+1); /* overline{a} */
			else            *(objVec1+i)=*(objVector+2*i);   /* underline{a} */
	    } 
	    break;
       case 1:  /* free variable with interval coefficient, + */
	    *(fpp1->bnds+2*i)=0;
		/* coefficient */
  	    for(j=0;j<(int)fpp1->ncons;j++){
    		*(fpp1->cons+j*(fpp1->dim+1)+1+i)=*(itvp->cons+j*(2*itvp->dim+1)+1+2*i);	
  	    }
 		/* objective */
	    if(objType==GLP_MAX)   *(objVec1+i)=*(objVector+2*i); /* underline{a} */
	    else            *(objVec1+i)=*(objVector+2*i+1);   /* overline{a} */ 
	    break;
       case 2:  /* free variable with interval coefficient, - */
	    *(fpp1->bnds+2*i+1)=0;
		/* coefficient */
  	    for(j=0;j<(int)fpp1->ncons;j++){  /* \overline{a} */
    		*(fpp1->cons+j*(fpp1->dim+1)+1+i)=*(itvp->cons+j*(2*itvp->dim+1)+1+2*i+1);	
  	    }
 		/* objective */
	    if(objType==GLP_MAX)   *(objVec1+i)=*(objVector+2*i+1); /* overline{a} */
	    else            *(objVec1+i)=*(objVector+2*i);   /* underline{a} */
	    break;
       default:
	    fprintf(stderr, "ilp_orthant_std(): orthant spliting is wrong.\n");	
    }
  }	
  /* LP with convex polyhedron fpp1 and objective vector objVec1 */
  res=itvp_slp_std(fpp1,objType, objVec1);
  if(objVec1!=NULL) free(objVec1);
  if(fpp1!=NULL){ 
     free(fpp1->bnds);
     free(fpp1->cons);
     free(fpp1);
  }
  return res;
} 
