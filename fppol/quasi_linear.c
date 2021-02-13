/*
 * quasi_linear.c
 *
 * Convert the interval linear form into the quasi-linearisation form.
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
 *
 * \begin{definition}[Quasi-Linearisation operator]
 * $$
 * \zeta(\sum \limits_{k = 1}^n {[a_k ,b_k ] \times V_k} \leq c)
 * \mathrel{\mathop=^{\rm def}}
 * \sum\limits_{k =1}^n {d_k \times V_k} \leq c \oplus_{f,+\infty}
 * \sum \limits_{k = 1}^n (\mathrm{max}\{b_k \ominus_{f,+\infty} d_k,\ d_k \ominus_{f,+\infty} a_k \} \otimes_{f,+\infty}|V_{k}|)
 * $$
 * where $d_k$ can be any floating point number inside the interval $[a_{k},b_{k}]$ and
 * $|V_{k}|= \mathrm{max}\{-\underline{V_{k}},\overline{V_{k}\}$
 * if $V_{k} \in \mathbf{V_{k}}=[\underline{V_{k}},\overline{V_{k}}]$.
 * \end{definition}
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fenv.h> /* floating-point lib */
#include <math.h>

#include "fpp_internal.h"
#include "fpp_config.h"
#include "quasi_linear.h"
#include "numitv.h"

/* ====================================================================== */
/* Internal functions */
/* ====================================================================== */
/* greatest common divisor between two integers, Euclid Algorithm */
long int gcd(long int x, long int y)
{
    long int t;
    /* if(x==1 || y==1 || x==-1 || y==-1) return 1; */
    while (y) {
        t = y;
        y = x % y;
        x = t;
    }
    return(x);
}

/* greatest common divisor among n integers, Euclid Algorithm */
long int ngcd(long int *a, int n)
{
 if (n == 1)
  return *a;

 return gcd(a[n-1], ngcd(a, n-1));
}

/*  if all the coefficients except constant are interger, then divide all coefficients by their gcd */
/*  parameter dim stores: n                                            */
/*  parameter c stores: coefficients                  			     */
int normalise_integer_inequality(int dim, numdbl_t *c)
{
   numdbl_t ci,ciInt;
   int flag;
   int i,j;
   long int gcd1,aInt;
   long int *cInt,*p;
   numdbl_t *cUp,*cDown;
   int n;

   cInt=p=(long int *)malloc(dim*sizeof(long int));
   n=0;
   for(i=1;i<=dim;i++){ /* check whether all coefficients are integers */
	ci=*(c+i);
	if(ci!=0){
		ciInt=trunc(ci);
		if(ciInt!=ci || ciInt==1.0 || ciInt==-1.0 ){
			checked_free(cInt);
			return UNCHANGED;
		}
		else{
		   aInt=labs((long int)ciInt);  /* absolute value */
		   for(j=0;j<n;j++){
		      if(*(cInt+j)==aInt) break;
		   }
		   if(j==n){
		      *p=aInt;
		      p++;
		      n++;
		   }
		}
	}
   }
   if(n==0){
	checked_free(cInt);
	return UNCHANGED;
   }
   gcd1=ngcd(cInt, n);
   checked_free(cInt);
   if(gcd1==1){
	return UNCHANGED;
   }
   else{ /* constant */
	cUp=(numdbl_t *)malloc((dim+1)*sizeof(numdbl_t));
	for(i=1;i<=dim;i++){
		ci=*(c+i);
		*(cUp+i)=ci/gcd1;
	}
	*cUp=*c/gcd1;
	memcpy(c,cUp,(dim+1)*sizeof(numdbl_t));
	checked_free(cUp);
	return SUCCEED;
   }
}

/*  destructive */
/*  if the inequality is too steep                                   */
/* (has a few very tiny or very large coeffs w.r.t. other coneffs)   */
/*  then remove the exceptional dimensions                           */
/*  by shifting the contribution to the constant term                */
/*  return 0 when nothing changed */
/*  return -1 when normalisation failed because of overflow */
/*  return 1 when normalisation succeed */
int normalise_steep_inequality(int dim, numdbl_t *con, numdbl_t *bnds)
{
   numitv_t aim,bndi,ci,ci2,ti;
   int i,res,n;
   int maxExp,minExp,exp,avExp,sumExp,difExp;
   int maxInd,minInd;
   numdbl_t *itvIneq,*intvp;
   numdbl_t *con1,aimin;
   int sat;

   maxExp=-1000;
   minExp=1000;
   maxInd=minInd=0;
   sumExp=0;
   n=0;
   for(i=1;i<=dim;i++){
	if(*(con+i)==0) continue;
        exp=0;
	frexp(*(con+i),&exp);
	if(exp<minExp){
	   minExp=exp;
	   minInd=i;
	}
        if(exp>maxExp){
	   maxExp=exp;
	   maxInd=i;
	}
	sumExp=sumExp+exp;
	n++;
   }
   if(n==0) return UNCHANGED;
   if(minExp>=MAX_MINEXP_COEF || (minExp>=MAX_MINEXP_COEF-2 && maxExp>=MAX_MINEXP_COEF+4 )){
        /* scale down large coefficents*/
	intvp=itvIneq=(numdbl_t *)malloc((2*dim+1)*sizeof(numdbl_t));
	memset(intvp,0,(2*dim+1)*sizeof(numdbl_t));
	aimin=fabs(*(con+minInd));
	*intvp=(*con)/aimin;
	intvp++;
   	for(i=1;i<=dim;i++){
	   *intvp=num_div_downward(*(con+i),aimin);
	   intvp++;
	   *intvp=(*(con+i))/(aimin);
	   intvp++;
	}
	con1=quasi_linearisation(dim,itvIneq,bnds,&sat);
	if(con1!=NULL){
	     memcpy(con,con1,(dim+1)*sizeof(numdbl_t));
	     checked_free(itvIneq);
	     checked_free(con1);
	     return SUCCEED;
	}
	checked_free(itvIneq);
        return FAILED; /* throw away */
   }
   difExp=maxExp-minExp;
   if(difExp < MAXEXP_DIFF){
       if(minExp>MIN_MINEXP_COEF)  return UNCHANGED;
          /* shift tiny coefficent to constant */
	aimin=*(con+minInd);
	bndi.inf=*(bnds+2*(minInd-1));
	bndi.sup=*(bnds+2*(minInd-1)+1);
	if((aimin<0 && bndi.sup==MAX_VARBND)||(aimin>0 && bndi.inf==-MAX_VARBND)){

	}
        aim=numitv_singleton(-*(con+minInd));
	bndi.inf=*(bnds+2*(minInd-1));
	bndi.sup=*(bnds+2*(minInd-1)+1);
	ci=numitv_singleton(*con);
	ti=numitv_mul(aim,bndi);
	ci2=numitv_add(ci,ti);
	if(ci2.sup >= MAX_VARBND )  return FAILED;
	*(con+minInd)=0.0;
	*con=ci2.sup;
   }
   else{
      avExp=sumExp/n;
      if(maxExp-avExp>= avExp-minExp){ /* normalise the maximum exponent */
        aim=numitv_singleton(-*(con+maxInd));
	bndi.inf=*(bnds+2*(maxInd-1));
	bndi.sup=*(bnds+2*(maxInd-1)+1);
	ci=numitv_singleton(*con);
	ti=numitv_mul(aim,bndi);
	ci2=numitv_add(ci,ti);
	if(ci2.sup >= MAX_VARBND )  return FAILED;
	*(con+maxInd)=0.0;
	*con=ci2.sup;
      }
      else { /* normalise the minimum exponent */
        aim=numitv_singleton(-*(con+minInd));
	bndi.inf=*(bnds+2*(minInd-1));
	bndi.sup=*(bnds+2*(minInd-1)+1);
	ci=numitv_singleton(*con);
	ti=numitv_mul(aim,bndi);
	ci2=numitv_add(ci,ti);
	if(ci2.sup >= MAX_VARBND )  return FAILED;
	*(con+minInd)=0.0;
	*con=ci2.sup;
     }
   }
   res=normalise_steep_inequality(dim,con,bnds);
   if(res==FAILED) return FAILED;
   else return SUCCEED;
}


/*  destructive */
/*  if the inequality has some coefficients which are nearly integers*/
/*  then reshape it to integer coefficients                          */
/*  by shifting the contribution to the constant term                */
int normalise_quasiInteger_inequality(int dim, numdbl_t *con, numdbl_t *bnds)
{
  numdbl_t c,ri;
  numdbl_t ak,dk,dak,bdk,maxadbk,dkInt,dkDif,mi;
  numdbl_t boundVk;
  numdbl_t *p;
  int k,i, num,r;
  bool reshape;
  bool changed;

  c=*con;
  p=con;
  p++;
  changed=false;
  /* --------------------------------------------------- */
  /* Quasi-linearisation with floating point arithmetic  */
  /* --------------------------------------------------- */
  for(k=1;k<=dim;k++){
     	/* compute dk */
     dk=ak=*(con+k);
     boundVk=fmax(-*(bnds+2*k-2), *(bnds+2*k-1) ); /* |Vk|=max{-\lowerline{Vk}, \overline{Vk}}*/
     reshape=false;
	/* integer ? */
     dkInt=round(dk);
     if(dk != dkInt && boundVk!=MAX_VARBND){
	dkDif=fabs(dk-dkInt);
	if(dkInt == 0 || dkDif<=QSEPSILON ){
	    dk=0.0;
	    reshape=true;
	}
	else{
	    mi=fmax(fabs(dk),fabs(dkInt));
	    if(dkDif/mi<=QSEPSILON){ /* align to integer */
	       dk=dkInt;
	       reshape=true;
	    }
	    else{  /* align to closest coefficient */
  	      for(i=1;i<k;i++){
		ri=*(con+i);
		if(ri==dk || ri==-dk ) break;
		if(ri==0)  continue; /* 0.0 is integer */
		if( (ri>-0.0 && dk<0.0) ||(ri<0.0 && dk>-0.0) ){ /*ri and dk have opposite sign */
		  ri=-ri;
		}
	        dkDif=fabs(dk-ri);
		mi=fmax(fabs(dk),fabs(ri));
		if(dkDif/mi<=QSEPSILON){
		      dk=ri;
		      reshape=true;
		      break;
		}
	      }
	    }
	}
     }
     if (dk==0) dk=0.0;  /* ensure there is no -0.0 as a coefficient */
     if(reshape==true){
	changed=true;
	*p=dk;
		/* compute ck */
	dak=dk-ak;
	maxadbk=fabs(dak);
	c=c+maxadbk*boundVk;
     }
     p++;
  }
  if(c==0) c=0.0; /* ensure there is no -0.0 as a coefficient */
  *con=c;   /* c d1 d2 d3 .. dn */
  r=normalise_integer_inequality(dim,con);
  if(r!=UNCHANGED) changed=true;
  return changed;
}
/* ====================================================================== */
/* interface functions */
/* ====================================================================== */

/*  quasi-linearisation for one interval linear inequality:                 */
/*               c >= \sum \limits_{k = 1}^n{ [ak,bk] * Vk }                */
/*  The midpoint result dk=(ak+_{f,+\infty}bk)/_{f,+\infty}2                */
/*  if the result linear constraint is trivial, return NULL                 */
/*  set *sat 1 when tautology, 0 when unsatisfiable    */
/*  parameter dim stores: n                                           */
/*  parameter coefficients stores: c a1 b1 a2 b2 ... an bn                  */
/*  parameter bounds stores the lower bound and upper bound of each var vk. */
numdbl_t *quasi_linearisation(int dim, numdbl_t *coeffs, numdbl_t *bounds, int *sat)
{
  numdbl_t c;
  numdbl_t ak,bk,ri;
  numdbl_t dk,dak,bdk,maxadbk,dkInt,dkDif,mi;
  numdbl_t boundVk;
  numdbl_t *res,*p;
  int k,i, num,r;
  bool flag0,reshape;

  res=p=(numdbl_t *)malloc((dim+1) * sizeof(numdbl_t));
  c=*coeffs;
  p++;
  flag0=true;
  /* --------------------------------------------------- */
  /* Quasi-linearisation with floating point arithmetic  */
  /* --------------------------------------------------- */
  for(k=1;k<=dim;k++){
     	/* compute dk */
     ak=*(coeffs+2*k-1);
     if(ak==-0.0) ak=0.0;
     bk=*(coeffs+2*k);
     if(bk==-0.0) bk=0.0;
     if(bounds==NULL) boundVk=MAX_VARBND;
     else boundVk=fmax( -*(bounds+2*k-2), *(bounds+2*k-1) ); /* |Vk|=max{-\lowerline{Vk}, \overline{Vk}}*/
     if(boundVk==MAX_VARBND && ak!=bk){ /*can not pose quasi_linearisation() */
    	 *sat=1;
        checked_free(res);
        return NULL;
     }
     if(ak==bk)	dk=ak;
     else	dk=(ak+bk)/2;
     reshape=false;
     if (dk==-0.0) dk=0.0;  /* ensure there is no -0.0 as a coefficient */
     if(dk!=0.0) flag0=false;
     *p=dk;
     p++;
     if(ak!=bk || reshape==true){
			/* compute ck */
		dak=fabs(dk-ak);
		bdk=fabs(bk-dk);
		maxadbk=fmax(dak,bdk);
		c=c+maxadbk*boundVk;
     }
  }
  if(c==-0.0) c=0.0; /* ensure there is no -0.0 as a coefficient */
  if(flag0==true ){ /* all coefficents are 0.0 */
     if(c>=0.0){
        *sat=1;
        checked_free(res);
        return NULL;
     }
     else{
        checked_free(res);
        *sat=0;
        return NULL;
     }
  }
  *res=c;
  return res;
}


numdbl_t *quasi_linearisation_bak(int dim, numdbl_t *coeffs, numdbl_t *bounds, int *sat)
{
  numdbl_t c;
  numdbl_t ak,bk,ri;
  numdbl_t dk,dak,bdk,maxadbk,dkInt,dkDif,mi;
  numdbl_t boundVk;
  numdbl_t *res,*p;
  int k,i, num,r;
  bool flag0,reshape;

  res=p=(numdbl_t *)malloc((dim+1) * sizeof(numdbl_t));
  c=*coeffs;
  p++;
  flag0=true;
  /* --------------------------------------------------- */
  /* Quasi-linearisation with floating point arithmetic  */
  /* --------------------------------------------------- */
  for(k=1;k<=dim;k++){
     	/* compute dk */
     ak=*(coeffs+2*k-1);
     bk=*(coeffs+2*k);
     boundVk=fmax( -*(bounds+2*k-2), *(bounds+2*k-1) ); /* |Vk|=max{-\lowerline{Vk}, \overline{Vk}}*/
     if(boundVk==MAX_VARBND && ak!=bk){ /*can not pose quasi_linearisation() */
    	 *sat=1;
        checked_free(res);
        return NULL;
     }
     if(ak==bk)	dk=ak;
     else	dk=(ak+bk)/2;
     reshape=false;
	/* integer ? */
     dkInt=round(dk);
     if(dk != dkInt && boundVk!=MAX_VARBND){
		dkDif=fabs(dk-dkInt);
		if( (dkInt == 0.0 || dkInt == -0.0)&&(dkDif<=QSEPSILON) ){
			dk=0.0;
			reshape=true;
		}
		else{
			mi=fmax(fabs(dk),fabs(dkInt));
			if(dkDif/mi<=QSEPSILON){ /* align to integer */
			   dk=dkInt;
			   reshape=true;
			}
			else{  /* align to closest coefficient */
			  for(i=1;i<k;i++){
				ri=*(res+i);
				if(ri==dk || ri==-dk ) break;
				if(ri==0.0)  continue; /* 0.0 is integer */
				if( (ri>-0.0 && dk<0.0) ||(ri<0.0 && dk>-0.0) ){ /*ri and dk have opposite sign */
				  ri=-ri;
				}
					dkDif=fabs(dk-ri);
				mi=fmax(fabs(dk),fabs(ri));
				if(dkDif/mi<=QSEPSILON){
					  dk=ri;
					  reshape=true;
					  break;
				}
			  }
			}
		}
     }
     if (dk==-0.0) dk=0.0;  /* ensure there is no -0.0 as a coefficient */
     if(dk!=0.0) flag0=false;
     *p=dk;
     p++;
     if(ak!=bk || reshape==true){
		/* compute ck */
		dak=fabs(dk-ak);
		bdk=fabs(bk-dk);
		maxadbk=fmax(dak,bdk);
		/*fprintf(stdout,"boundVk=%.53f\n",boundVk);*/
		c=c+maxadbk*boundVk;
     }
  }
  if(c==-0.0) c=0.0; /* ensure there is no -0.0 as a coefficient */
  if(flag0==true ){ /* all coefficents are 0.0 */
     if(c>=0.0){
        *sat=1;
        checked_free(res);
        return NULL;
     }
     else{
        checked_free(res);
        *sat=0;
        return NULL;
     }
  }
  *res=c;   /* c d1 d2 d3 .. dn */
  normalise_integer_inequality(dim,res);
  r=normalise_steep_inequality(dim, res, bounds);
  if(r==FAILED){
     *sat=1;
     checked_free(res);
     return NULL;
  }
  return res;
}


/*  if all the coefficients including constant are interger, */
/*   then divide all coefficients by their gcd               */
/*  parameter dim stores: n                                  */
/*  parameter c stores: coefficients          		     */
numdbl_t *normalise_integer_inequality_withConstant(int dim, numdbl_t *c, bool *sat)
{
   numdbl_t ci,ciInt;
   int flag;
   int i,j;
   long int gcd1,aInt;
   long int *cInt,*p;
   numdbl_t *cUp,*cDown;
   int n;
   bool zero;

   cInt=p=(long int *)malloc(dim*sizeof(long int));
   n=0;
   zero=true;
   for(i=0;i<=dim;i++){ /* check whether all coefficients are integers */
	ci=*(c+i);
	if(ci>0.0 || ci<-0.0 ){
   		if(i!=0) zero=false;
		ciInt=trunc(ci);
		if(ciInt!=ci || ciInt==1.0 || ciInt==-1.0 ){
			checked_free(cInt);
			return c;
		}
		else{
		   aInt=labs((long int)ciInt);  /* absolute value */
		   for(j=0;j<n;j++){
		      if(*(cInt+j)==aInt) break;
		   }
		   if(j==n){
		      *p=aInt;
		      p++;
		      n++;
		   }
		}
	}
   }
   if(n==0){
	checked_free(cInt);
	return c;
   }
   if( zero==true ){
	if(*c>=0) *sat=1;
        else *sat=0;
        checked_free(c);
	checked_free(cInt);
	return NULL;
   }
   gcd1=ngcd(cInt, n);
   checked_free(cInt);
   if(gcd1==1){
	return c;
   }
   else{ /* constant */
	cUp=(numdbl_t *)malloc((dim+1)*sizeof(numdbl_t));
	cDown=(numdbl_t *)malloc((dim+1)*sizeof(numdbl_t));
	for(i=0;i<=dim;i++){
		ci=*(c+i);
		*(cUp+i)=ci/gcd1;
		*(cDown+i)=num_div_downward(ci,gcd1);/*  *(cDown+i)=*(c+i)/gcd1; */
		if(*(cDown+i)!=*(cUp+i)){
			checked_free(cUp);
			checked_free(cDown);
			return c;
		}
	}
	*cUp=*c/gcd1;
	memcpy(c,cUp,(dim+1)*sizeof(numdbl_t));
	checked_free(cUp);
	checked_free(cDown);
	return c;
   }
}
