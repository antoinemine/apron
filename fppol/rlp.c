/*
 * rlp.c
 *
 * Rigorous linear programming via floating point arithmetic
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


/*
 * rigorous linear programming with floating point arithmetic.
 * --rigorous upper bound for maximum objective function ;
 * --rigorous lower bound for minimum objective fucntion.
 *
 * Given original(primal) linear program:
 *      min   c^T*x
 *      s.t.  Ax<=b
 * the dual of it is
 *      max   b^T*y
 *      s.t.  A^T*y = c
 *            y<=0
 *
 * For the rigorous upper bound of an original linear program:
 *      max   c^T*x
 *      s.t.  Ax<=b
 * we convert it into the negation of the rigorous lower bound of
 *      min   -c^T*x
 *      s.t.  Ax<=b
 *
 * Notes on bounds:
 *   The bounds of a polyhedron are redundant w.r.t the linear constraints of the polyhedron,
 *   that is, the bounds information is implied by the linear constraints.
 *   So constraints of the polyhedron contains all the information of this polyhedron,
 *   and all variables are unrestricted in the corresponding linear program,
 *   especially without any sign information. This also simplifies the duality.
 *
 * Notes on Soundness:
 *   The soundness of the implementation of rigorous linear programming depends on
 *   rounding towards +oo mode (using ap_fpu_init provided by APRON when creating a manager).
 *
 * Notes on GLPK:
 *   Inside our library,we use glpk-4.65 which has the implementation of the simplex algorithm
 *   in floating point arithmetic, but glpk-4.65 only supports the double type for the API.
 *   So it is very important to use the double type when using glpk'API.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fenv.h> /* floating-point lib */

#include "rlp.h"
#include "numitv.h"

int nlpcreate=0;
int nlpdel=0;
int nlp=0;
int nrlp=0;
int nslp=0;
int nlpjoin=0;
double lptime=0;

int rump_max_iterations=5;

/* ============================================================ */
/* Construction, Initialization, Deconstrunction */
/* ============================================================ */

/* Construction: Initialized with linear constraints from a polyhedron  */
glp_prob *rlp_create(fpp_t *fpp, unsigned objType, numdbl_t *objVector )
{
  glp_prob *lp;
  int *ia, *ja;
  double *ar;
  int rows,cols;
  numdbl_t *p,*q;
  int i,j,index;

  nlpcreate++;
  lp = glp_create_prob();
  /*  A: m*n  A^T:n*m  */
  rows= fpp->dim;
  cols = fpp->ncons;
  ia=(int *)malloc((rows*cols+1)*sizeof(int));
  ja=(int *)malloc((rows*cols+1)*sizeof(int));
  ar=(double *)malloc((rows*cols+1)*sizeof(double));
  p=fpp->cons;
  /* always GLP_MAX in the dual program (GLP_MIN in primal case); */
  /* primal GLP_MAX will also be converted into dual GLP_MAX */
  glp_set_obj_dir(lp, GLP_MAX);
  if(objType==GLP_MIN)
        glp_set_obj_name(lp, "MIN"); /* the type of objective function in the primal program */
  else  glp_set_obj_name(lp, "MAX");

  glp_add_rows(lp, rows);
  glp_add_cols(lp, cols);
  /* set bounds of variables y<=0 */
  for(i=1;i<=cols;i++)  {
      glp_set_col_bnds(lp, i, GLP_UP, 0.0, 0.0); /* y<=0 */
  }

  /* set c within A^T*y=c */
  q=objVector;
  for(i=1;i<=rows;i++)  {
      if(objType==GLP_MIN)
      	glp_set_row_bnds(lp, i, GLP_FX, *q, *q);
      else  glp_set_row_bnds(lp, i, GLP_FX, -*q, -*q); /* convert GLP_MAX into GLP_MIN */
      q++;
  }
  /* set A^T within A^T*y=c; that is, a[i,j]->a[j,i] */
  index=1;
  for(i=1;i<=cols;i++)  {
      /* The first column of the original constraints stores constant coefficient b; */
      /* And b becomes the coefficient vector of the objective function in the dual program. */
      glp_set_obj_coef(lp, i, *(p++));
      /* set A^T within A^T*y=c; that is, a[i,j]->a[j,i] */
      for(j=1;j<=rows;j++)  {
           if(*p!=0){
              ia[index] = j, ja[index] = i, ar[index] = *p; /* a[j,i] =  *p */
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

/* Construction: Initialized with linear constraints from a polyhedron  */
glp_prob *rlp_create_matrix(fpp_t *fpp)
{
  glp_prob *lp;
  int *ia, *ja;
  double *ar;
  int rows,cols;
  numdbl_t *p,*q;
  int i,j,index;

  nlpcreate++;
  lp = glp_create_prob();
  /*  A: m*n  A^T:n*m  */
  rows= fpp->dim;
  cols = fpp->ncons;
  ia=(int *)malloc((rows*cols+1)*sizeof(int));
  ja=(int *)malloc((rows*cols+1)*sizeof(int));
  ar=(double *)malloc((rows*cols+1)*sizeof(double));
  p=fpp->cons;
  /* always GLP_MAX in the dual program (GLP_MIN in primal case); */
  /* primal GLP_MAX will also be converted into dual GLP_MAX */
  glp_set_obj_dir(lp, GLP_MAX);

  glp_add_rows(lp, rows);
  glp_add_cols(lp, cols);
  /* set bounds of variables y<=0 */
  for(i=1;i<=cols;i++)  {
      glp_set_col_bnds(lp, i, GLP_UP, 0.0, 0.0); /* y<=0*/
  }

  /* set A^T within A^T*y=c; that is, a[i,j]->a[j,i] */
  index=1;
  for(i=1;i<=cols;i++)  {
      /* The first column of the original Constraints stores constant coefficient b; */
      /* And b becomes the coefficient vector of the objective function in the dual program. */
      glp_set_obj_coef(lp, i, *(p++));
      /* set A^T within A^T*y=c; that is, a[i,j]->a[j,i] */
      for(j=1;j<=rows;j++)  {
           if(*p!=0){
              ia[index] = j, ja[index] = i, ar[index] = *p; /* a[j,i] =  *p */
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
void rlp_set_objective(glp_prob *lp, unsigned objType, numdbl_t *objVector )
{
  int rows;
  int i;
  numdbl_t *q;

  if(objType==GLP_MIN)
        glp_set_obj_name(lp, "MIN"); /* the type of objective function in the primal program */
  else  glp_set_obj_name(lp, "MAX");

  rows=glp_get_num_rows(lp);
  /* set c within A^T*y=c */
  q=objVector;
  for(i=1;i<=rows;i++)  {
      if(objType==GLP_MIN)
      	    glp_set_row_bnds(lp, i, GLP_FX, *q, *q);
      else  glp_set_row_bnds(lp, i, GLP_FX, -*q, -*q); /* convert GLP_MAX into GLP_MIN */
      q++;
  }
}

/* The same with rlp_problem_create() except excluding k-th (start from 1-th) linear constraint in the polyhedron  */
glp_prob *rlp_create_without_KthCons(fpp_t *fpp, unsigned objType, numdbl_t *objVector, int k)
{
  glp_prob *lp;
  int *ia, *ja;
  double *ar;
  int rows,cols;
  numdbl_t *p,*q;
  int i,j,index;

  nlpcreate++;
  lp = glp_create_prob();
  /*  A: m*n  A^T:n*m  */
  rows= fpp->dim;
  cols = fpp->ncons;
  ia=(int *)malloc((rows*cols+1)*sizeof(int));
  ja=(int *)malloc((rows*cols+1)*sizeof(int));
  ar=(double *)malloc((rows*cols+1)*sizeof(double));
  p=fpp->cons;
  /* always GLP_MAX in the dual program (GLP_MIN in primal case); */
  /* primal GLP_MAX will also be converted into dual GLP_MAX */
  glp_set_obj_dir(lp, GLP_MAX);
  if(objType==GLP_MIN)
        glp_set_obj_name(lp, "MIN"); /* the type of objective function in the primal program */
  else  glp_set_obj_name(lp, "MAX");
  glp_add_rows(lp, rows);
  glp_add_cols(lp, cols);
  /* set bounds of variables y<=0 */
  for(i=1;i<=cols;i++)  {
      glp_set_col_bnds(lp, i, GLP_UP, 0.0, 0.0); /* y<=0*/
  }
  /* set c within A^T*y=c */
  q=objVector;
  for(i=1;i<=rows;i++)  {
      if(objType==GLP_MIN)
      	glp_set_row_bnds(lp, i, GLP_FX, *q, *q);
      else  glp_set_row_bnds(lp, i, GLP_FX, -*q, -*q); /* convert GLP_MAX into GLP_MIN */
      q++;
  }
  /* set A^T within A^T*y=c; that is, a[i,j]->a[j,i] */
  index=1;
  for(i=1;i<=cols;i++)  {
      /* The first column of the original Constraints stores constant coefficient b; */
      /* And b becomes the coefficient vector of the objective function in the dual program. */
      if(i==k){  /* set all related coefficient with 0 */
	    glp_set_obj_coef(lp, i,0);
        p=p+rows+1;
      }
      else{
      	glp_set_obj_coef(lp, i, *(p++));

      	/* set A^T within A^T*y=c; that is, a[i,j]->a[j,i] */
      	for(j=1;j<=rows;j++)  {
           if(*p!=0){
              ia[index] = j, ja[index] = i, ar[index] = *p; /* a[j,i] =  *p */
              index++;
      	   }
      	   p++;
      	}
      }
  }
  glp_load_matrix(lp, index-1, ia, ja, ar);
  checked_free(ia);
  checked_free(ja);
  checked_free(ar);
  return lp;
}

/* ============================================================ */
/* Solution */
/* ============================================================ */

glp_prob *rlp_solve(glp_prob *lp)
{
  int res;

  nlp++;
  nrlp++;
  glp_smcp parm;
  glp_init_smcp(&parm);
  /* parm.presolve = GLP_ON;  parm.tol_bnd = 1e-6;   parm.tol_dj = 1e-6; */
  parm.meth = GLP_DUALP;

  res=glp_simplex(lp, &parm);

  if (res != 0){
      fprintf(stdout, "\nThe RLP simplex slover failed! \n");
      glp_delete_prob(lp);
      lp=NULL;
      return NULL;
  }
  return lp;
}

glp_prob *slp_solve(glp_prob *lp,glp_smcp *p_parm)
{
  int res;

  nlp++;
  nslp++;
  res=glp_simplex(lp,p_parm);              /* restart */

  if (res != 0){
      fprintf(stdout, "\nThe SLP simplex slover failed! \n");
      glp_delete_prob(lp);
      lp=NULL;
      return NULL;
  }
  return lp;
}

void lp_delete( glp_prob *lp )
{
   nlpdel++;
   if(lp!=NULL){
     glp_delete_prob(lp);
     lp=NULL;
   }
}

/* Assume glp_get_status(lp) === GLP_OPT */
numdbl_t rlp_get_rigorous_optimal_value_dual(glp_prob *lp, numdbl_t *bounds)
{
  int *ind;
  numdbl_t *val;
  const char *objType;
  int i,j, num, rows, cols;
  numdbl_t t,mu,rTx,maxrTx_i;
  numdbl_t *x;
  numdbl_t *y,  *r_down, *r_up, *c;
  numdbl_t *py, *pr1,    *pr2,  *pc;
  numdbl_t sum,aij,ci,bi,yi,ri,xi,rdown_i,rup_i,xdown_i,xup_i;
  bool infFlag;
  numdbl_t tmp1,tmp2,tmpup;
  numdbl_t rawObj;
  glp_smcp parm;
  numdbl_t *eps;
  int iters;


  /*get optimal solution y */
  cols=glp_get_num_cols(lp);
  y=py=(numdbl_t *)malloc(cols * sizeof(numdbl_t));

  for(i=1;i<=cols;i++){
     *py = glp_get_col_prim(lp, i);
     if(*py>0.0) *py=0.0; /* assert py<=0 */
     py++;
  }

  rows=glp_get_num_rows(lp);
  r_down=pr1=(numdbl_t *)malloc(rows * sizeof(numdbl_t));
  r_up=pr2=(numdbl_t *)malloc(rows * sizeof(numdbl_t));
  c=pc=(numdbl_t *)malloc(rows * sizeof(numdbl_t));
  ind=(int *)malloc((cols+1)* sizeof(int));
  val=(numdbl_t *)malloc((cols+1)* sizeof(numdbl_t));
  for(i=1;i<=rows;i++){
     *pc=glp_get_row_lb(lp,i);
     pc++;
  } /*get c */

  /* ------------------------------------------------------------------------------- */
  /* Post-processing for rigorous linear programming with floating point arithmetic  */
  /* ------------------------------------------------------------------------------- */
     /* 1: rounddown; fesetround(FE_DOWNWARD); */
     /* 2: r_down=A^T*y-c */
  for(i=1;i<=rows;i++){
     num=glp_get_mat_row(lp,i,ind,val);
     sum=0.0;
     for(j=1;j<=num;j++){
       aij=*(y+(ind[j]-1));  /* ind[j] is the index of the column */
	     /* sum+val[j]*aij; val[j] is the value at the position of the column ind[j] */
       sum=num_add_downward(sum,num_mul_downward(val[j],aij));
     } /* calculate ai1*y1+ai2*y2+...+ain*yn */

     ci=*(c+i-1);
     *pr1= num_sub_downward(sum,ci);  /* *pr1= sum-ci;   */

     pr1++;
   }

     /* 3: t=y^T*b; */
   t=0.0;
   for(i=1;i<=cols;i++){
     yi=*(y+i-1);  /* ind[j] is the index of the column */
     bi=glp_get_obj_coef(lp,i);
     t=num_add_downward( t,num_mul_downward(yi,bi) );  /* t=t+yi*bi; */
   }

     /* 4: roundup;  fesetround(FE_UPWARD); */
     /* 5: r_up=A^T*y-c; */
  for(i=1;i<=rows;i++){
     num=glp_get_mat_row(lp,i,ind,val);
     sum=0.0;
     for(j=1;j<=num;j++){
       aij=*(y+(ind[j]-1));  /* ind[j] is the index of the column */
       sum=sum+val[j]*aij;   /* val[j] is the value at the position of the column ind[j] */
     } /* calculate ai1*y1+ai2*y2+...+ain*yn */
     ci=*(c+i-1);
     *pr2= sum-ci;
     pr2++;
   }

     /* 6: mu=max{\sum [r_down_i,r_up_i]*[x_down_i,x_up_i]}-t; */
   x=bounds;
   infFlag=false;
   rTx=0.0;
   for(i=1;i<=rows;i++){
     rdown_i=*(r_down+i-1);
     rup_i=*(r_up+i-1);
     xdown_i=*(x+2*(i-1));
     xup_i=*(x+2*(i-1)+1);
     maxrTx_i=-MAX_VARBND;

     if( rdown_i==0 && rup_i==0 )
        continue;
     if( xdown_i==-MAX_VARBND ){
	      if(rdown_i<0){
           rTx=MAX_VARBND;
           infFlag=true;
           break;
        }
     }
     else {
	      tmp1=rdown_i*xdown_i;
	      tmp2=rup_i*xdown_i;
	      if(tmp1<=tmp2) tmpup=tmp2;
              else  tmpup=tmp1;
	      if(tmpup>maxrTx_i)  maxrTx_i=tmpup;
     }

     if( xup_i==MAX_VARBND ){
	      if(rup_i>0){
           rTx=MAX_VARBND;
           infFlag=true;
           break;
        }
     }
     else {
    	  tmp1=rdown_i*xup_i;
    	  tmp2=rup_i*xup_i;
    	  if(tmp1<=tmp2) tmpup=tmp2;
        else  tmpup=tmp1;
      	if(tmpup>maxrTx_i)  maxrTx_i=tmpup;
     }
     rTx=rTx+maxrTx_i;
   }

   rawObj=glp_get_obj_val(lp);
   objType= glp_get_obj_name(lp); /* the type of objective function in the primal program */
   if(infFlag==false){
        /* mu=maxrTx-t */
        mu=rTx-t;

        /* 7: mu=-mu; */
        /* objType= (char *)malloc(10 * sizeof(char)); */
        if (strcmp(objType, "MIN")==0)   mu=(-1)*mu;   /* MIN: mu=-mu; MAX: mu=-(-mu);*/
   }
   else {
        /* initialize varepsilon */
        eps=(numdbl_t *)malloc(rows*sizeof(numdbl_t));
	      memset(eps,0,rows*sizeof(numdbl_t));
        for(i=1;i<=rows;i++){
           rdown_i=*(r_down+i-1);
           rup_i=*(r_up+i-1);
           xdown_i=*(x+2*(i-1));
           xup_i=*(x+2*(i-1)+1);

           if(xup_i>=MAX_VARBND && xdown_i<=-MAX_VARBND){
	              /* donot perturb when both bounds are infinite */
	         }
           else if((xup_i>=MAX_VARBND && rup_i>0)|| (xdown_i<=-MAX_VARBND && rdown_i<0)){
      	      num=glp_get_mat_row(lp,i,ind,val);
      	      sum=0.0;
      	      for(j=1;j<=num;j++){
      	         aij=*(y+(ind[j]-1));  /* ind[j] is the index of the column */
      	         sum=sum+fabs(val[j])*fabs(aij);  /* val[j] is the value at the position of the column ind[j] */
     	        } /* calculate |ai1|*|y1|+|ai2|*|y2|+...+|ain|*|yn| */
	            ci=*(c+i-1);
	            glp_init_smcp(&parm);
	            *(eps+i-1)=2*parm.tol_bnd*(sum+fabs(ci));
           }
        }

        iters=0;
      	mu=rlp_perturbed_dual(lp,bounds,y,r_down,r_up,c,eps,&iters);
      	checked_free(eps);

      	if(mu==-MAX_VARBND || mu==MAX_VARBND){
      	     checked_free(y);
         	 checked_free(r_down);
         	 checked_free(r_up);
         	 checked_free(c);
             checked_free(ind);
             checked_free(val);
         	 if(strcmp(objType, "MIN")==0) return -MAX_VARBND;
      	     else     return MAX_VARBND;
        }
   }

   /* ------------------------------------------------------------------------ */
   /*          End of post-processing for rigorous linear programming          */
   /* ------------------------------------------------------------------------ */


   checked_free(y);
   checked_free(r_down);
   checked_free(r_up);
   checked_free(c);
   checked_free(ind);
   checked_free(val);
   return (mu==-0.0?0.0:mu);
}

/* To deal with the case that varialbes have infinite bounds */
/* Note: */
/* 1) parameters r_down and r_up are used to compute eps' */
/* 2) r'_down = A^T*y'-c; where $c$ rather than $c_varepsilon$ is used */
numdbl_t rlp_perturbed_dual(glp_prob *lp, numdbl_t *bnds, numdbl_t *y,
			    numdbl_t *r_down, numdbl_t *r_up,
			    numdbl_t *c,
			    numdbl_t *eps, int *iters)
{
  int *ind;
  numdbl_t *val;
  const char *objType;
  int i,j, num, rows, cols;
  numdbl_t t,mu,rTx,maxrTx_i;
  numdbl_t *x;
  numdbl_t *py,*pr1,*pr2;
  numdbl_t sum,aij,ci,bi,yi,ri,xi,rdown_i,rup_i,xdown_i,xup_i;
  bool infFlag,due2unbndFlag;
  numdbl_t tmp1,tmp2,tmpup;
  numdbl_t c_eps;
  int *indices;

  objType= glp_get_obj_name(lp);
  rows=glp_get_num_rows(lp);

  x=bnds;

  due2unbndFlag=true;
  indices= (int *) malloc(rows*sizeof(int));
  memset(indices,0,rows*sizeof(int));
  j=0;
  for(i=1;i<=rows;i++){
     rdown_i=*(r_down+i-1);
     rup_i=*(r_up+i-1);
     xdown_i=*(x+2*(i-1));
     xup_i=*(x+2*(i-1)+1);
     if((xup_i>=MAX_VARBND && rup_i>0 && xdown_i>-MAX_VARBND)|| (xdown_i<=-MAX_VARBND && rdown_i<0 && xup_i<MAX_VARBND)){
         due2unbndFlag=false;
         if(xup_i>=MAX_VARBND && rup_i>0 && xdown_i>-MAX_VARBND){
       	    c_eps=*(c+i-1)-*(eps+i-1);  /* perturb the dual problem (c_varepsilon) */
      	 }
      	 else{
      	     c_eps=*(c+i-1)+*(eps+i-1);   /* perturb the dual problem (c_varepsilon) */
      	 }
      	 glp_set_row_bnds(lp, i, GLP_FX, c_eps, c_eps);
     }
     else if(xup_i>=MAX_VARBND && xdown_i<=-MAX_VARBND){ /*  && (rdown_i != 0 ||rup_i != 0)  */
      	    /* unbounded on both sides and cause side-effect */
      	 *(indices+j)=i;
      	 j++;
     }
  }

	/* Check whether it is only due to those variables that are unbounded on both sides */
  if(due2unbndFlag==true){ /* mu becomes infinite due to free variables */
	    mu=rlp_process_free_vars(lp,bnds,y,c,indices,j);
	    checked_free(indices);
      if(mu<MAX_VARBND && mu>-MAX_VARBND){ /* mu becomes finite */
         return mu;
      }
  }
  else{ /* mu becomes infinite partly due to variables that are bounded on one side */
      checked_free(indices);
  }

	/* solve the perturbed problem */
  lp=rlp_solve(lp);
  if(lp==NULL){
     if(strcmp(objType, "MIN")==0) return -MAX_VARBND;
     else     return MAX_VARBND;
  }

  /*get perturbed optimal solution y' */
  cols=glp_get_num_cols(lp);
  y=py=(numdbl_t *)malloc(cols * sizeof(numdbl_t));

  for(i=1;i<=cols;i++){
     *py = glp_get_col_prim(lp, i);
     if(*py>0.0) *py=0.0; /* assert py<=0 */
     py++;
  }

  ind=(int *)malloc((cols+1)* sizeof(int));
  val=(numdbl_t *)malloc((cols+1)* sizeof(numdbl_t));
  /* ------------------------------------------------------------------------------- */
  /* Post-processing for rigorous linear programming with floating point arithmetic  */
  /* ------------------------------------------------------------------------------- */
     /* 1: rounddown; fesetround(FE_DOWNWARD); */
     /* 2: r'_down=A^T*y'-c; */
  pr1=r_down;
  for(i=1;i<=rows;i++){
     num=glp_get_mat_row(lp,i,ind,val);
     sum=0.0;
     for(j=1;j<=num;j++){
       aij=*(y+(ind[j]-1));  /* ind[j] is the index of the column */
	     /* sum+val[j]*aij; val[j] is the value at the position of the column ind[j] */
       sum=num_add_downward(sum,num_mul_downward(val[j],aij));
     } /* calculate ai1*y1+ai2*y2+...+ain*yn */
     ci=*(c+i-1);
     *pr1= num_sub_downward(sum,ci);  /* *pr1= sum-ci;   */
     pr1++;
   }
     /* 3: t'=y'^T*b; */
   t=0.0;
   for(i=1;i<=cols;i++){
     yi=*(y+i-1);  /* ind[j] is the index of the column */
     bi=glp_get_obj_coef(lp,i);
     t=num_add_downward( t,num_mul_downward(yi,bi) );  /* t=t+yi*bi; */
   }

     /* 4: roundup;  fesetround(FE_UPWARD); */
     /* 5: r'_up=A^T*y'-c; */
  pr2=r_up;
  for(i=1;i<=rows;i++){
     num=glp_get_mat_row(lp,i,ind,val);
     sum=0.0;
     for(j=1;j<=num;j++){
       aij=*(y+(ind[j]-1));  /* ind[j] is the index of the column */
       sum=sum+val[j]*aij;   /* val[j] is the value at the position of the column ind[j] */
     } /* calculate ai1*y1+ai2*y2+...+ain*yn */
     /*printf(":::sum=%.40f :::",sum);*/
     ci=*(c+i-1);
     *pr2= sum-ci;
     pr2++;
   }

     /* 6: mu=max{\sum [r_down_i,r_up_i]*[x_down_i,x_up_i]}-t; */
   infFlag=false;
   rTx=0.0;
   for(i=1;i<=rows;i++){
     rdown_i=*(r_down+i-1);
     rup_i=*(r_up+i-1);
     xdown_i=*(x+2*(i-1));
     xup_i=*(x+2*(i-1)+1);
     maxrTx_i=-MAX_VARBND;

     if( rdown_i==0 && rup_i==0 )
    	 continue;
     if( xdown_i==-MAX_VARBND ){
	      if(rdown_i<0){
           rTx=MAX_VARBND;
           infFlag=true;
           break;
        }
     }
     else {
      	tmp1=rdown_i*xdown_i;
      	tmp2=rup_i*xdown_i;
      	if(tmp1<=tmp2) tmpup=tmp2;
        else  tmpup=tmp1;
      	if(tmpup>maxrTx_i)  maxrTx_i=tmpup;
     }

     if( xup_i==MAX_VARBND ){
      	if(rup_i>0){
           rTx=MAX_VARBND;
           infFlag=true;
           break;
        }
     }
     else {
      	tmp1=rdown_i*xup_i;
      	tmp2=rup_i*xup_i;
      	if(tmp1<=tmp2) tmpup=tmp2;
        else  tmpup=tmp1;
      	if(tmpup>maxrTx_i)  maxrTx_i=tmpup;
     }
     rTx=rTx+maxrTx_i;
   }

   if(infFlag==false){
      mu=rTx-t;
      /* 7: mu=-mu; */
      /* objType= (char *)malloc(10 * sizeof(char)); */
      if (strcmp(objType, "MIN")==0)   mu=(-1)*mu;   /* MIN: mu=-mu; MAX: mu=-(-mu);*/
   }
   else {
	 /* reach the threshold? */
       *iters=*iters+1;
       if(*iters> rump_max_iterations){
            checked_free(y);
            checked_free(ind);
            checked_free(val);

            if(strcmp(objType, "MIN")==0) return -MAX_VARBND;
            else     return MAX_VARBND;
       }

       /* update the perturbation parameter */
       for(i=1;i<=rows;i++){
         rdown_i=*(r_down+i-1);
         rup_i=*(r_up+i-1);
         xdown_i=*(x+2*(i-1));
         xup_i=*(x+2*(i-1)+1);
      	 if(xup_i>=MAX_VARBND && xdown_i<=-MAX_VARBND){
      	     /* unbounded on both sides */
      	 }
         else if((xup_i>=MAX_VARBND && rup_i>0)|| (xdown_i<=-MAX_VARBND && rdown_i<0)){
             *(eps+i-1)=*(eps+i-1)*2; /* enlarge perturbation */
         }
       }

       mu=rlp_perturbed_dual(lp,bnds,y,r_down,r_up,c,eps,iters);
   }

   /* LProblem_Delete(lp); */ /* delete the LP problem outside */
   checked_free(y);
   checked_free(ind);
   checked_free(val);
   return (mu==-0.0?0.0:mu);
}


/* Deal with free variables */
/* To compute an enclosure that constains solutions */
/*        satisfying correpsonding linear equations of m rows */
numdbl_t rlp_process_free_vars(glp_prob *lp, numdbl_t *bnds, numdbl_t *y,
			    numdbl_t *c, int *indices, int m)
{
  int *ind;
  numdbl_t *val;
  const char *objType;
  int i,j,k, num, rows, cols;
  numdbl_t t,mu,rTx,maxrTx_i;
  numdbl_t *x;
  numdbl_t *py,*pr1,*pr2;
  numdbl_t sum,aij,ci,bi,yi,ri,xi,rdown_i,rup_i,xdown_i,xup_i;
  bool indFlag,infFlag,bFlag;
  numdbl_t tmp1,tmp2,tmpup;
  numdbl_t c_eps;
  numdbl_t *A,*R,*pA;
  numdbl_t AA_ji;
  numdbl_t *y_itv,*r_itv;
  numdbl_t *y_itv_indices;
  numdbl_t *AA;
  numdbl_t *b;
  numitv_t sum_itv,itv1,itv2,itv3,t_itv,yi_itv,bi_itv,rTx_itv;

  cols=glp_get_num_cols(lp);
  if(m>cols)  return -MAX_VARBND;

  objType= glp_get_obj_name(lp);
  rows=glp_get_num_rows(lp);

	/* collect rows */
  AA=(numdbl_t *)malloc(m*cols*sizeof(numdbl_t));
  memset(AA,0,m*cols*sizeof(numdbl_t));
  b=(numdbl_t *)malloc(m*sizeof(numdbl_t));
  memset(b,0,m*sizeof(numdbl_t));

  ind=(int *)malloc((cols+1)* sizeof(int));
  val=(numdbl_t *)malloc((cols+1)* sizeof(numdbl_t));
  for(i=1;i<=m;i++){
     pA=AA+(i-1)*cols;
     j=*(indices+i-1);
     *(b+i-1)=*(c+j-1);
     num=glp_get_mat_row(lp,j,ind,val);
     for(k=1;k<=num;k++){
        *(pA+ind[k]-1)=val[k]; /* ind[j] is the index of the column */
                               /* val[j] is the value at the position of the column ind[j] */
     }
  }

	/* convert to square matrix of size mXm*/
  A=(numdbl_t *)malloc(m*m*sizeof(numdbl_t));
  memset(A,0,m*m*sizeof(numdbl_t));
  k=1; /* variables that are in A */

  bFlag=false;
  for(i=1;i<=cols;i++){
     	/* is variable x_i in indices? */
     indFlag=false;
     for(j=1;j<=m;j++){
	if(*(indices+j-1)==i)   indFlag=true;
	if(*(indices+j-1)>=i)   break;
     }
     if(indFlag==false){ /* x_i is not in indices */
      	   /* for each row */
      	yi=*(y+i-1);
      	for(j=1;j<=m;j++){
      	    /* b_j := b_j - AA_{ji} y_i */
      	   AA_ji=*(AA+(j-1)*cols+(i-1));
      	   *(b+j-1)=*(b+j-1)-AA_ji*yi;
      	}
     }
     else{               /* x_i is in indices */
  	   /* for each row: A_{jk} := AA_{ji}  */
       	for(j=1;j<=m;j++){
  	       *(A+(j-1)*m+(k-1))=*(AA+(j-1)*cols+(i-1));
        }
        k++;
     }
  }

  y_itv_indices=rump_method(A,b,m);
  if(y_itv_indices==NULL){
     checked_free(AA);
     checked_free(ind);
     checked_free(val);
     checked_free(A);
     checked_free(b);
     return -MAX_VARBND;
  }

   /* compute y' which is an interval vector */
  y_itv=malloc(2*cols*sizeof(numdbl_t));
  k=0;
  for(i=1;i<=cols;i++){
     	/* is variable x_i in indices? */
     indFlag=false;
     for(j=1;j<=m;j++){
      	if(*(indices+j-1)==i)   indFlag=true;
      	if(*(indices+j-1)>=i)   break;
     }

     if(indFlag==false){ /* x_i is not in indices */
      	*(y_itv+2*(i-1))=*(y+i-1); /* lower */
      	*(y_itv+2*(i-1)+1)=*(y+i-1); /* upper */
     }
     else{               /* x_i is in indices */
      	*(y_itv+2*(i-1))=*(y_itv_indices+2*k); /* lower */
      	*(y_itv+2*(i-1)+1)=*(y_itv_indices+2*k+1);  /* upper */
        k++;
     }
  }

     /* 2: compute r'  */
     /* (1) r'_i=(A_{:i})^T*y'-c_i for all i that are not in indices  */
     /* (2) r'_j=0 for all j that are in indices  */
  r_itv=malloc(2*rows*sizeof(numdbl_t));
  for(i=1;i<=rows;i++){
     	/* is row i in indices? */
     indFlag=false;
     for(j=1;j<=m;j++){
      	if(*(indices+j-1)==i)   indFlag=true;
      	if(*(indices+j-1)>=i)   break;
     }
     if(indFlag==false){ /* i-th row is not in indices */
 	      /* r'_i=(A_{:i})^T*y'-c_i */
        sum_itv.inf=0;
        sum_itv.sup=0;
        num=glp_get_mat_row(lp,i,ind,val);
        for(j=1;j<=num;j++){
      	  itv1.inf=val[j];  /* val[j] is the value at the position of the column ind[j] */
      	  itv1.sup=val[j];
      	  itv2.inf=*(y_itv+2*(ind[j]-1));
      	  itv2.sup=*(y_itv+2*(ind[j]-1)+1); /* ind[j] is the index of the column */
          itv3=numitv_mul(itv1,itv2);
          sum_itv=numitv_add(sum_itv,itv3);
        }
        *(r_itv+2*(i-1)) = num_sub_downward(sum_itv.inf,*(c+i-1));
        *(r_itv+2*(i-1)+1) = sum_itv.sup-*(c+i-1);
     }
     else{               /* i-th row is in indices */
       	*(r_itv+2*(i-1)) =0;
       	*(r_itv+2*(i-1)+1) =0;
     }
   }

     /* 3: t'=y'^T*b; */
   t_itv.inf=0.0;
   t_itv.sup=0.0;
   for(i=1;i<=cols;i++){
     yi_itv.inf=*(y_itv+2*(i-1));
     yi_itv.sup=*(y_itv+2*(i-1)+1);
     bi_itv.inf=glp_get_obj_coef(lp,i);
     bi_itv.sup=glp_get_obj_coef(lp,i);
     itv1=numitv_mul(yi_itv,bi_itv);
     t_itv.inf=num_add_downward(t_itv.inf,itv1.inf);  /* t'=t'+y'i*bi; */
     t_itv.sup=t_itv.sup+itv1.sup;  /* t'=t'+y'i*bi; */
   }


     /* 4: r'T*x  */
   infFlag=false;
   rTx_itv.inf=0.0;
   rTx_itv.sup=0.0;
   for(i=1;i<=rows;i++){
     itv1.inf=*(r_itv+2*(i-1));
     itv1.sup=*(r_itv+2*(i-1)+1);
     itv2.inf=*(bnds+2*(i-1));
     itv2.sup=*(bnds+2*(i-1)+1);

     itv3=numitv_mul(itv1,itv2);
     rTx_itv=numitv_add(rTx_itv,itv3);
   }

     /* 5: mu=inf(t'-r'T*x); */
   itv1=numitv_sub(t_itv,rTx_itv);

   checked_free(AA);
   checked_free(A);
   checked_free(b);
   checked_free(y_itv_indices);
   checked_free(y_itv);
   checked_free(r_itv);
   checked_free(ind);
   checked_free(val);
   return itv1.inf;
}

/* Rump's method */
/* To compute a verified enclosure for the solution of */
/*    the square system of linear equations Ax=b       */
/* RETURN: an interval vector */
numdbl_t* rump_method(numdbl_t *A, numdbl_t *b, int m)
{
  int i,j,k;
  numdbl_t *R,*x0; /* normal matrix/vector */
  numdbl_t *RA,*G,*Ax0,*g,*x,*y,*b_Ax0,*Gx,*Gy;  /* interval matrix/vector */
  numitv_t itv1,itv2,itv3;
  bool flag;
  numdbl_t Ax0i_l,Ax0i_u,x0i,bi;
  numdbl_t *res;
  numdbl_t *f;

  if(m==1){
    res=(numdbl_t *) malloc(2*m*sizeof(numdbl_t)); /* interval vector */
    *(res)=num_div_downward(*b,*A); /* lower */
    *(res+1)=*b/(*A); /* upper */
    return res;
  }

  R= (numdbl_t *) malloc(m*m*sizeof(numdbl_t));
  memcpy(R,A,m*m*sizeof(numdbl_t));
  flag=matrix_inverse(R, m);
  if(flag==false){
    checked_free(R);
    return NULL;
  }
  x0=matrix_X_vector(R,b,m);

      /* G = I-RA */
  RA=matrix_X_matrix_itv(R,A,false,false,m);
  G= (numdbl_t *) malloc(2*m*m*sizeof(numdbl_t));
  for(i=0;i<m;i++){
     for(j=0;j<m;j++){
      	if(j==i){ /* I- RA */
           *(G+2*(i*m+j))=num_sub_downward(1,*(RA+2*(i*m+j)+1)); /* lower */
           *(G+2*(i*m+j)+1)=1-*(RA+2*(i*m+j)); /* upper */
        }
      	else{ /* -RA */
           *(G+2*(i*m+j))=-*(RA+2*(i*m+j)+1);
           *(G+2*(i*m+j)+1)=-*(RA+2*(i*m+j));
      	}
     }
  }

      /* g = R(b-Ax0)*/
  Ax0=matrix_X_vector_itv(A,x0,false,false,m);
  b_Ax0 = (numdbl_t *) malloc(2*m*sizeof(numdbl_t));
  flag=true;
  for(i=0;i<m;i++){
     bi=*(b+i);
     Ax0i_l=*(Ax0+2*i);
     Ax0i_u=*(Ax0+2*i+1);
     *(b_Ax0+2*i)=num_sub_downward(bi,Ax0i_u); /* lower */
     *(b_Ax0+2*i+1)=bi-Ax0i_l; /* upper */
     if( *(b_Ax0+2*i)!=0 || *(b_Ax0+2*i+1)!=0 )  flag=false; /* x0 is not the exact solution */
  }

  if(flag==true){ /* x0 is indeed the exact solution */
     res=(numdbl_t *) malloc(2*m*sizeof(numdbl_t)); /* interval matrix */
	/* res := x0+y */
     for(i=0;i<m;i++){
	      x0i=*(x0+i);
        *(res+2*i)=*(x0+i); /* lower */
        *(res+2*i+1)=*(x0+i); /* upper */
     }
     checked_free(R);
     checked_free(RA);
     checked_free(G);
     checked_free(x0);
     checked_free(Ax0);
     checked_free(b_Ax0);
     return res;
  }

  g=matrix_X_vector_itv(R,b_Ax0,false,true,m);
  y=(numdbl_t *) malloc(2*m*sizeof(numdbl_t)); /* interval vector */
  memcpy(y,g,2*m*sizeof(numdbl_t));
	/* f := \diameterOf(Gy+g)[-eps,+eps]+[-eta,eta] */
  f=(numdbl_t *) malloc(2*m*sizeof(numdbl_t)); /* interval vector */
  Gy=matrix_X_vector_itv(G,y,true,true,m);
  for(i=0;i<m;i++){
  	itv1.inf=*(Gy+2*i);
  	itv1.sup=*(Gy+2*i+1);
  	itv2.inf=*(g+2*i);
  	itv2.sup=*(g+2*i+1);
    itv3=numitv_add(itv1,itv2);
  	itv1.inf=-0.5; /* we choose \eps=0.5 */
  	itv1.sup=0.5;
    itv2=numitv_mul(itv3,itv1);
  	itv1.inf=-1e-7; /* we choose \eta as the tolerance (tol_bnd) used by the LP solver */
  	itv1.sup=1e-7;
    itv3=numitv_add(itv2,itv1);
    *(f+2*i)=itv3.inf; /* lower */
    *(f+2*i+1)=itv3.sup; /* upper */
  }
  checked_free(Gy);
  k=0;
  x=(numdbl_t *) malloc(2*m*sizeof(numdbl_t)); /* interval vector */
  while(k<=rump_max_iterations){
  	/* x:= y + f   */
     for(i=0;i<m;i++){
        itv1.inf=*(f+2*i);
        itv1.sup=*(f+2*i+1);
      	itv2.inf=*(y+2*i);
      	itv2.sup=*(y+2*i+1);
        itv3=numitv_add(itv1,itv2);
        *(x+2*i)=itv3.inf; /* lower */
        *(x+2*i+1)=itv3.sup; /* upper */
     }
	    /* y := Gx+g */
     Gx=matrix_X_vector_itv(G,x,true,true,m);
     for(i=0;i<m;i++){
      	itv1.inf=*(Gx+2*i);
      	itv1.sup=*(Gx+2*i+1);
      	itv2.inf=*(g+2*i);
      	itv2.sup=*(g+2*i+1);
        itv3=numitv_add(itv1,itv2);
        *(y+2*i)=itv3.inf; /* lower */
        *(y+2*i+1)=itv3.sup; /* upper */
     }
     checked_free(Gx);
     /* y \subset x ? */
     flag=true;
     for(i=0;i<m;i++){
      	if( *(x+2*i)<*(y+2*i) && *(y+2*i+1)<*(x+2*i+1) ){
        }
      	else{
      	   flag=false;
      	   break;
        }
     }
     if(flag==true) break;
     k++;
  }
  if(k<=rump_max_iterations){ /* a verified enclosure */
        res=(numdbl_t *) malloc(2*m*sizeof(numdbl_t)); /* interval matrix */
	       /* res := x0+y */
        for(i=0;i<m;i++){
	         x0i=*(x0+i);
           *(res+2*i)=num_add_downward(x0i,*(y+2*i)); /* lower */
           *(res+2*i+1)=x0i+*(y+2*i+1); /* upper */
        }
  }
  else{
	   res=NULL;
  }

  checked_free(R);
  checked_free(x0);
  checked_free(RA);
  checked_free(G);
  checked_free(Ax0);
  checked_free(g);
  checked_free(x);
  checked_free(y);
  checked_free(b_Ax0);
  checked_free(f);
  return res;
}


/* approximate result of Ab using FP arithmetic */
numdbl_t* matrix_X_vector(numdbl_t *A, numdbl_t *b, int m)
{
   numdbl_t *c;
   numdbl_t sum,bj,aij;
   int i,j;

   c= (numdbl_t *) malloc(m*sizeof(numdbl_t)); /* normal vector */
   for(i=0;i<m;i++){
      sum=0;
      for(j=0;j<m;j++){
	       aij=*(A+i*m+j);
         bj=*(b+j);
	       sum=sum+aij*bj;
      }
      *(c+i)=sum;
   }
   return c;
}

/* Ab using interval arithmetic */
numdbl_t* matrix_X_vector_itv(numdbl_t *A, numdbl_t *b, bool Aitv, bool bitv, int m)
{
   int i,j;
   numitv_t sum,aij_itv,bj_itv,tmp;
   numdbl_t *c;

   c= (numdbl_t *) malloc(2*m*sizeof(numdbl_t)); /* interval vector */
   for(i=0;i<m;i++){
      sum.inf=0;
      sum.sup=0;
      for(j=0;j<m;j++){
      	if(Aitv==false){ /*normal matrix*/
      	   aij_itv.inf=*(A+i*m+j);
      	   aij_itv.sup=*(A+i*m+j);
        }
        else{
      	   aij_itv.inf=*(A+i*2*m+2*j); /* lower */
      	   aij_itv.sup=*(A+i*2*m+2*j+1); /* upper */
        }
        if(bitv==false){ /*normal vector*/
           bj_itv.inf=*(b+j);
           bj_itv.sup=*(b+j);
	      }
        else{
           bj_itv.inf=*(b+2*j);
           bj_itv.sup=*(b+2*j+1);
        }
        tmp=numitv_mul(aij_itv,bj_itv);
        sum=numitv_add(sum,tmp);
      }
      *(c+2*i)=sum.inf;
      *(c+2*i+1)=sum.sup;
   }
   return c;
}


/* square matrix multiplication AB using interval arithmetic */
numdbl_t* matrix_X_matrix_itv(numdbl_t *A, numdbl_t *B, bool Aitv, bool Bitv, int m)
{
   int i,j,k;
   numitv_t sum,Aik_itv,Bkj_itv,tmp;
   numdbl_t *C;

   C= (numdbl_t *) malloc(2*m*m*sizeof(numdbl_t)); /* interval vector */
   for(i=0;i<m;i++){
      for(j=0;j<m;j++){
         sum.inf=0;
         sum.sup=0;
         for(k=0;k<m;k++){
      	    if(Aitv==false){ /*normal matrix*/
        	   	Aik_itv.inf=*(A+i*m+k);
        	   	Aik_itv.sup=*(A+i*m+k);
            }
            else{
        	   	Aik_itv.inf=*(A+i*2*m+2*k); /* lower */
        	   	Aik_itv.sup=*(A+i*2*m+2*k+1); /* upper */
            }
      	    if(Bitv==false){ /*normal matrix*/
      	        Bkj_itv.inf=*(B+k*m+j);
      	        Bkj_itv.sup=*(B+k*m+j);
            }
            else{
      	        Bkj_itv.inf=*(B+k*2*m+2*j); /* lower */
      	        Bkj_itv.sup=*(B+k*2*m+2*j+1); /* upper */
            }
            tmp=numitv_mul(Aik_itv,Bkj_itv);
            sum=numitv_add(sum,tmp);
         }
         *(C+i*2*m+2*j)=sum.inf;
         *(C+i*2*m+2*j+1)=sum.sup;
      }
   }
   return C;
}


/* Matrix inverse A^{-1} based on Gauss-Jordan Elimination */
bool matrix_inverse(numdbl_t *A, int n)
{
    int *is,*js,i,j,k,l,u,v;
    numdbl_t d,p;
    is=malloc(n*sizeof(int));
    js=malloc(n*sizeof(int));
    for (k=0; k<=n-1; k++){
        d=0.0;
	      /* find pivot element */
        for (i=k; i<=n-1; i++){
          for (j=k; j<=n-1; j++){
             l=i*n+j; p=fabs(A[l]);
             if (p>d) { d=p; is[k]=i; js[k]=j;}
          }
        }
        if (d+1.0==1.0){  /* d==0 */
          free(is);
          free(js);
          return(0); /* singular matrix */
        }
	      /* exchange */
        if (is[k]!=k)
          for (j=0; j<=n-1; j++){
              u=k*n+j; v=is[k]*n+j;
              p=A[u]; A[u]=A[v]; A[v]=p;
          }
        if (js[k]!=k)
          for (i=0; i<=n-1; i++){
              u=i*n+k; v=i*n+js[k];
              p=A[u]; A[u]=A[v]; A[v]=p;
          }
        l=k*n+k;
        A[l]=1.0/A[l];
        for (j=0; j<=n-1; j++)
          if (j!=k) { u=k*n+j; A[u]=A[u]*A[l];}
        for (i=0; i<=n-1; i++)
          if (i!=k)
            for (j=0; j<=n-1; j++)
              if (j!=k){
	                 u=i*n+j;
                   A[u]=A[u]-A[i*n+k]*A[k*n+j];
              }
        for (i=0; i<=n-1; i++)
          if (i!=k) { u=i*n+k; A[u]=-A[u]*A[l];}
    }
    for (k=n-1; k>=0; k--){
        if (js[k]!=k)
          for (j=0; j<=n-1; j++){
	            u=k*n+j; v=js[k]*n+j;
              p=A[u]; A[u]=A[v]; A[v]=p;
          }
        if (is[k]!=k)
          for (i=0; i<=n-1; i++){
	            u=i*n+k; v=i*n+is[k];
              p=A[u]; A[u]=A[v]; A[v]=p;
          }
    }
    free(is); free(js);
    return 1;
}


/* if eps==true, we return the raw objective value */
/* return *nearly:  true when raw opt result is almost the same with rigorous result */
numdbl_t rlp_get_optimal_value(glp_prob *lp, numdbl_t *bounds, bool eps, bool *nearly)
{
  int objType;
  numdbl_t rawObj,rigObj,dif,a,b,mi;
  int lpres,lpsts1,lpsts2;
  glp_smcp parm;
  int prlgN;

  if (strcmp(glp_get_obj_name(lp), "MIN")==0)
       objType=GLP_MIN;
  else objType=GLP_MAX;

  *nearly=true;
  if(lp==NULL){
     if(objType==GLP_MIN)   return -MAX_VARBND;
     else   return MAX_VARBND;
  }
  lpsts1=glp_get_status(lp);
  /* fprintf(stdout, "\n glp_get_status(lp)=%d\n",lpres); */
  if(lpsts1!=GLP_OPT){
     if(eps==true){
         glp_init_smcp(&parm);
         parm.meth = GLP_DUALP;
         lpres=glp_simplex(lp,&parm);              /* restart */
         rawObj=glp_get_obj_val(lp);
         if(objType==GLP_MAX)   rawObj=-rawObj;  /* reverse sign for GLP_MAX */
         return rawObj==-0.0?0.0:rawObj;
     }
     else if(objType==GLP_MIN)   return -MAX_VARBND;
     else   return MAX_VARBND;
  }
  rawObj=glp_get_obj_val(lp);
  if(objType==GLP_MAX)   rawObj=-rawObj; /* reverse sign for GLP_MAX */
  if(eps==true)     return rawObj==-0.0?0.0:rawObj;
  rigObj=rlp_get_rigorous_optimal_value_dual(lp,bounds);
  if(rigObj==MAX_VARBND || rigObj==-MAX_VARBND )
      *nearly=false;
  else{
      dif=fabs(rawObj-rigObj);
#if INTPROGRAM
      if(dif!=0) *nearly=false;
#else
      a=fabs(rawObj);
      b=fabs(rigObj);
      mi=fmin(a,b);
      if( (mi==0.0 && dif>EPSILON) ||(mi>0.0 && dif/mi>EPSILON)){
	   *nearly=false;
      }
#endif
  }

  return rigObj;
}

/* ============================================================ */
/* Standard linear programming ( unsounded )  */
/* ============================================================ */

/* Construction: Initialized with linear constraints from a polyhedron  */
glp_prob *slp_create(fpp_t *fpp, unsigned objType, numdbl_t *objVector )
{
  glp_prob *lp;
  int *ia, *ja;
  double *ar;/*do not use float here because the glpk API uses double,
                      otherwise may change the original problem */
  int rows,cols;
  numdbl_t *p,*q;
  int i,j,index;

  nlpcreate++;

  /*  A: m*n   */
  rows= fpp->ncons;
  cols = fpp->dim;
  ia=(int *)malloc((rows*cols+1)*sizeof(int));
  ja=(int *)malloc((rows*cols+1)*sizeof(int));
  ar=(double *)malloc((rows*cols+1)*sizeof(double));

  p=fpp->cons;

  lp = glp_create_prob();

  glp_set_obj_dir(lp, objType);
  glp_add_rows(lp, rows);
  glp_add_cols(lp, cols);

  /* set bounds of variables x; set coefficients for the objective function   */
  for(i=0;i<cols;i++)  {
      glp_set_col_bnds(lp, i+1, GLP_FR, 0, 0);
      if(objVector!=NULL) glp_set_obj_coef(lp, i+1, *(objVector+i));
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

/* Construction: Initialized with linear constraints from a polyhedron without objective info */
glp_prob *slp_create_matrix(fpp_t *fpp )
{
  glp_prob *lp;
  int *ia, *ja;
  double *ar;/*do not use float here because the glpk API uses double,
                      otherwise may change the original problem */
  int rows,cols;
  numdbl_t *p,*q;
  int i,j,index;

  nlpcreate++;
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

/* Construction: Initialized with linear constraints from a polyhedron without objective info */
glp_prob *slp_create_matrix_withBnds(fpp_t *fpp, int ndimBnds)
{
  glp_prob *lp;
  int *ia, *ja;
  double *ar;/*do not use float here because the glpk API uses double,
                      otherwise may change the original problem */
  int rows,cols;
  numdbl_t *p,*q;
  int i,j,index;
  numdbl_t lbnd,ubnd;

  nlpcreate++;
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
  for(i=0;i<ndimBnds;i++)  {
      lbnd=*(fpp->bnds+2*i);
      ubnd=*(fpp->bnds+2*i+1);
      if(lbnd==-MAX_VARBND && ubnd==MAX_VARBND ){
         glp_set_col_bnds(lp, i+1, GLP_FR, 0, 0);
      }
      else if(lbnd==-MAX_VARBND && ubnd<MAX_VARBND ){
         glp_set_col_bnds(lp, i+1, GLP_UP,0, ubnd );
      }
      else if(lbnd>-MAX_VARBND && ubnd==MAX_VARBND ){
         glp_set_col_bnds(lp, i+1, GLP_LO,lbnd,0 );
      }
      else if(lbnd>-MAX_VARBND && ubnd<MAX_VARBND ){
         glp_set_col_bnds(lp, i+1, GLP_DB,lbnd,ubnd );
      }
  }
  for(i=ndimBnds;i<cols;i++)  {
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
void slp_set_objective(glp_prob *lp, unsigned objType, numdbl_t *objVector )
{
  int cols;
  int i;

  glp_set_obj_dir(lp, objType);
  cols=glp_get_num_cols(lp);
  for(i=0;i<cols;i++)  {
      glp_set_obj_coef(lp, i+1, *(objVector+i));
  }
}

/*  disable k-th(start from 1) row  */
void slp_disable_kthRow(glp_prob *lp,unsigned k, numdbl_t c)
{
/*   glp_set_row_bnds(lp,k,GLP_FR,0,0);  */
  numdbl_t ci;
  if(c==0.0 || c==-0.0)  ci=1; /*2*EPSILON; or use 1 */
  else    ci=ceil(c+2*EPSILON*fabs(c));
  glp_set_row_bnds(lp,k,GLP_UP,0,ci);
}

/*  enable k-th(start from 1) row  */
void slp_set_upbnd_kthRow(glp_prob *lp,unsigned k,numdbl_t c)
{
  glp_set_row_bnds(lp,k,GLP_UP,0,c);
}

/*  delete k-th(start from 1) row  */
void slp_del_kthRow(glp_prob *lp,unsigned k )
{
  int num[2];
  num[1]=k;
  glp_del_rows(lp,1,num);
}

/*  disable k-th(start from 1) row  */
void slp_filter_kthRow(glp_prob *lp,unsigned k, numdbl_t c)
{
   glp_set_mat_row(lp,k,0,NULL,NULL);
   glp_set_row_bnds(lp,k,GLP_FX,0,0);
}

/*  restore k-th(start from 1) row  */
void slp_restore_kthRow(glp_prob *lp,unsigned k,numdbl_t *con)
{
  int *ind;
  int i,cols;
  glp_set_row_bnds(lp,k,GLP_UP,0,*con);
  cols=glp_get_num_cols(lp);
  ind=(int *)malloc((cols+1)*sizeof(int));
  for(i=1;i<=cols;i++){
    ind[i]=i;
  }
  glp_set_mat_row(lp,k,cols,ind,con);
  free(ind);
}

/* Construction: Initialized with linear constraints from a polyhedron without k-th constraint */
glp_prob *slp_create_without_KthCons(fpp_t *fpp, unsigned objType, numdbl_t *objVector,int k)
{
  glp_prob *lp;
  int *ia, *ja;
  double *ar;/*do not use float here because the glpk API uses double,
                      otherwise may change the original problem */
  int rows,cols;
  numdbl_t *p,*q;
  int i,j,index;
  int num[2];

  nlpcreate++;

  /*  A: m*n   */
  rows= fpp->ncons;
  cols = fpp->dim;
  ia=(int *)malloc((rows*cols+1)*sizeof(int));
  ja=(int *)malloc((rows*cols+1)*sizeof(int));
  ar=(double *)malloc((rows*cols+1)*sizeof(double));
  p=fpp->cons;

  lp = glp_create_prob();

  glp_set_obj_dir(lp, objType);
  glp_add_rows(lp, rows);
  glp_add_cols(lp, cols);

  /* set bounds of variables x; set coefficients for the objective function   */
  for(i=0;i<cols;i++)  {
      glp_set_col_bnds(lp, i+1, GLP_FR, 0, 0);
      if(objVector!=NULL) glp_set_obj_coef(lp, i+1, *(objVector+i));
  }

  /* set b within A*x<=b */
  for(i=0;i<rows;i++)  {
      glp_set_row_bnds(lp, i+1, GLP_UP, 0, *p);
      p=p+(fpp->dim+1);
  }
  glp_set_row_bnds(lp, k , GLP_FX, 0, 0);  /* k-th */

  /* set A within A*x<=b: don't import 0.0 term */
  index=1;
  p=fpp->cons;
  for(i=1;i<=rows;i++)  {
      if(i==k){ /* don't import 0.0 term */
          /*for(j=1;j<=cols;j++)  {
             ia[index] = i, ja[index] = j, ar[index] = 0;
             index++;
          }*/
          p=p+cols+1;
      }
      else{
          p++;  /* space for constant */
          for(j=1;j<=cols;j++)  {
	     if(*p!=0){
                ia[index] = i, ja[index] = j, ar[index] = *p; /* a[i,j] =  *p */
                index++;
	     }
	     p++;
          }
      }
  }
  glp_load_matrix(lp, index-1, ia, ja, ar);
  num[1]=k;
  glp_del_rows(lp,1,num);
  checked_free(ia);
  checked_free(ja);
  checked_free(ar);
  return lp;
}

numdbl_t slp_get_optimal_value(glp_prob *lp)
{
   numdbl_t mu;
   mu=glp_get_obj_val(lp);
   return mu==-0.0?0.0:mu;
}

numdbl_t slp_exact(fpp_t *fpp, unsigned objType, numdbl_t *objVector)
{
  glp_prob *lp;
  numdbl_t mu;

  lp= slp_create_matrix(fpp);
  slp_set_objective(lp,objType,objVector);
  glp_smcp parm;
  glp_init_smcp(&parm);
  parm.tm_lim= 1000; /* time limit, 1000ms */
  glp_exact(lp,&parm);
  mu=slp_get_optimal_value(lp);
  lp_delete(lp);
  return mu==-0.0?0.0:mu;
}

numdbl_t slp_get_optimal_value_exact(glp_prob *lp, unsigned objType, numdbl_t *objVector)
{
  numdbl_t mu;
  slp_set_objective(lp,objType,objVector);
  glp_set_obj_dir(lp, objType);
  glp_smcp parm;
  glp_init_smcp(&parm);
  parm.tm_lim= 1000; /* time limit, 1000ms */
  glp_exact(lp,&parm);
  mu=slp_get_optimal_value(lp); 
  return mu==-0.0?0.0:mu;
}
