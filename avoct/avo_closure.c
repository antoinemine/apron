/*
 * avo_closure.c
 *
 * Closure algorithms
 *
 * APRON Library / Absolute Value Octagonal (AVO) Domain
 *
 * Copyright (C) Liqian Chen & Jiangchao Liu' 2014
 *
 */

/* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution.
*/

#include "avo.h"
#include "avo_internal.h"

/* All closures are in-place. */


/* ============================================================ */
/* Full Closure */
/* ============================================================ */
int nsc_min(bound_t a,bound_t b,int a1,int b1)
{
	if(bound_cmp(a,b)>0)
		return b1;
	else if(bound_cmp(a,b)<0)
		return a1;
	else
	{
		if(a1 < 0)
			return a1;

			return b1;
	}
}

int nsc_add(int a1,int b1)
{
	if(a1 < 0)
		return a1;
	return b1;
}
int nsc_max(bound_t a, bound_t b,int a1 ,int b1)
{
	if(bound_cmp(a,b)>0)
			return b1;
	else if(bound_cmp(a,b)<0)
		return b1;
	else
	{
		if(a1 > 0)
			return a1;
		else
			return b1;
	}
}
void nsc_3min(bound_t a , bound_t b, bound_t c )
{
	bound_t t;
	bound_min(t,b,c);
	bound_bmin(a,t);
	bound_clear(t);
}
int nsc_bmin(bound_t a,bound_t b,bound_t c,bound_t a1,bound_t b1,bound_t c1)
{
	bound_t temp;
	bound_init(temp);
	bound_min(temp,a,b);
	int r2;
	int r1 ;
	r1= nsc_min(a,b,bound_cmp_int(a1,1),bound_cmp_int(b1,1));
	r2 = nsc_min(temp,c,r1,bound_cmp_int(c1,1));
	bound_clear(temp);
	return r2;
}
void tighten_nsc(bound_t *m, bound_t *nsc, size_t dim)
{
     size_t j,i;
     size_t dim2 = dim * 2;
     size_t dim21 = dim2 + 1;
     size_t dim12 = dim2 - 1;
     int temp;
     for (i = 0 ; i < dim; i++)
     {
    		 if(bound_cmp_int(m[avo_matpos2(2*i+1,2*i)],0)<=0)
    		 {
			    bound_min(m[avo_matpos2(2*i+1,2*i)],m[avo_matpos2(2*i+1,2*i)],m[avo_matpos2(2*i+2*dim,2*i+2*dim+1)]);
    		 }
    		 if(bound_cmp_int(m[avo_matpos2(2*i,2*i+1)],0)<=0)
    		 {
    			 bound_min(m[avo_matpos2(2*i,2*i+1)],m[avo_matpos2(2*i,2*i+1)],m[avo_matpos2(2*i+2*dim,2*i+1+2*dim)]);
    		 }

     }
     for(i = 2 * dim; i < 4 * dim ; i ++)
     {
		for(j = 0 ; j < 2 * dim; j++)
		{
			if(i % 2)
			{
				temp = nsc_max(m[avo_matpos2(i - dim2,j)],m[avo_matpos2(i - dim21,j)],bound_cmp_int(nsc[avo_matpos2(i - dim2,j)],1),bound_cmp_int(nsc[avo_matpos2(i - dim21,j)],1));
				if(temp < 0)
				  bound_set_int(nsc[avo_matpos2(i,j)],0);
			    	else
				  bound_set_infty(nsc[avo_matpos2(i,j)],1);
				bound_max(m[avo_matpos2(i,j)],m[avo_matpos2(i - dim2,j)],m[avo_matpos2(i - dim21,j)]);
			}
			else
			{
				 temp = nsc_bmin(m[avo_matpos2(i,j)],m[avo_matpos2(i - dim2,j)],m[avo_matpos2(i - dim12,j)],nsc[avo_matpos2(i,j)],nsc[avo_matpos2(i - dim2,j)],nsc[avo_matpos2(i - dim12,j)]);
				 if(temp < 0)
					  bound_set_int(nsc[avo_matpos2(i,j)],0);
				 else
					  bound_set_infty(nsc[avo_matpos2(i,j)],1);

			    	 nsc_3min(m[avo_matpos2(i,j)],m[avo_matpos2(i - dim2,j)],m[avo_matpos2(i - dim12,j)]);
			}
		}
	}
	for(i = 2 * dim ; i < 4 *dim ; i++)
		for(j = 2 * dim ; j <= (i|1) ; j++)
		{
			if(i % 2)
			{
				temp = nsc_max(m[avo_matpos2(i - dim2,j)],m[avo_matpos2(i - dim21,j)],bound_cmp_int(nsc[avo_matpos2(i - dim2,j)],1),bound_cmp_int(nsc[avo_matpos2(i - dim21,j)],1));
				if(temp < 0)
					bound_set_int(nsc[avo_matpos2(i,j)],0);
				else
					bound_set_infty(nsc[avo_matpos2(i,j)],1);

				bound_max(m[avo_matpos2(i,j)],m[avo_matpos2(i - dim2,j)],m[avo_matpos2(i - dim21,j)]);

			}
			if(!(j % 2))
			{
			    temp = nsc_max(m[avo_matpos2(i ,j - dim2)],m[avo_matpos2(i ,j - dim12)],bound_cmp_int(nsc[avo_matpos2(i ,j - dim2)],1),bound_cmp_int(nsc[avo_matpos2(i ,j - dim12)],1));
			    if(temp < 0)
			    	bound_set_int(nsc[avo_matpos2(i,j)],0);
			    else
				bound_set_infty(nsc[avo_matpos2(i,j)],1);
			    bound_max(m[avo_matpos2(i,j)],m[avo_matpos2(i ,j - dim2)],m[avo_matpos2(i ,j - dim12)]);
			}
			if(!(i % 2) && j % 2)
			{
			    temp = nsc_bmin(m[avo_matpos2(i,j)],m[avo_matpos2(i - dim2,j)],m[avo_matpos2(i - dim12,j)],nsc[avo_matpos2(i,j)],nsc[avo_matpos2(i - dim2,j)],nsc[avo_matpos2(i - dim12,j)]);
			    if(temp < 0)
				  bound_set_int(nsc[avo_matpos2(i,j)],0);
			    else
				  bound_set_infty(nsc[avo_matpos2(i,j)],1);
			    nsc_3min(m[avo_matpos2(i,j)],m[avo_matpos2(i - dim2,j)],m[avo_matpos2(i - dim12,j)]);

			    temp = nsc_bmin(m[avo_matpos2(i,j)],m[avo_matpos2(i - dim2,j)],m[avo_matpos2(i ,j- dim21)],nsc[avo_matpos2(i,j)],nsc[avo_matpos2(i - dim2,j)],nsc[avo_matpos2(i ,j- dim21)]);
			    if(temp < 0)
				  bound_set_int(nsc[avo_matpos2(i,j)],0);
			    else
				  bound_set_infty(nsc[avo_matpos2(i,j)],1);
			    nsc_3min(m[avo_matpos2(i,j)],m[avo_matpos2(i ,j - dim2 )],m[avo_matpos2(i ,j- dim21)]);
			}
	}
}

void tighten(bound_t * m, size_t dim)
{
	size_t j,i;
	size_t dim2 = dim * 2;
	size_t dim21 = dim2 + 1;
	size_t dim12 = dim2 - 1;

	for(i = 2 * dim; i < 4 * dim ; i ++)
		for(j = 0 ; j < 2 * dim; j++)
		{
			if(i % 2)
			   bound_max(m[avo_matpos2(i,j)],m[avo_matpos2(i - dim2,j)],m[avo_matpos2(i - dim21,j)]);
			else
			   nsc_3min(m[avo_matpos2(i,j)],m[avo_matpos2(i - dim2,j)],m[avo_matpos2(i - dim12,j)]);

		}

	for(i = 2 * dim ; i < 4 *dim ; i++)
		for(j = 2 * dim ; j < 4 * dim ; j++)
		{

			if(i % 2)
				bound_max(m[avo_matpos2(i,j)],m[avo_matpos2(i - dim2,j)],m[avo_matpos2(i - dim21,j)]);
			if(j % 2 == 0 )
				bound_max(m[avo_matpos2(i,j)],m[avo_matpos2(i ,j - dim2)],m[avo_matpos2(i ,j - dim12)]);
			if(i % 2 == 0 && j % 2)
			{
				nsc_3min(m[avo_matpos2(i,j)],m[avo_matpos2(i - dim2,j)],m[avo_matpos2(i - dim12,j)]);
				nsc_3min(m[avo_matpos2(i,j)],m[avo_matpos2(i ,j - dim2 )],m[avo_matpos2(i ,j- dim21)]);
			}

		}

}
void tighten_2(bound_t * m, size_t dim,size_t ver, size_t hor)
{
	size_t j,i;
	size_t dim2 = dim * 2;
	size_t dim21 = dim2 + 1;
	size_t dim12 = dim2 - 1;

	for(i = 2 * dim + ver * 2; i <   2 * dim + ver * 2 + 2; i ++)
		for(j = hor * 2 ; j < hor * 2 + 2; j++)
		{
			if(i % 2)
			   bound_max(m[avo_matpos2(i,j)],m[avo_matpos2(i - dim2,j)],m[avo_matpos2(i - dim21,j)]);
			else
			   nsc_3min(m[avo_matpos2(i,j)],m[avo_matpos2(i - dim2,j)],m[avo_matpos2(i - dim12,j)]);
		}

	for(i = 2 * dim + ver * 2; i <   2 * dim + ver * 2 + 2; i ++)
		for(j = 2 * dim + hor * 2 ; j <2 * dim + hor * 2 + 2; j++)
		{

			if(i % 2)
				bound_max(m[avo_matpos2(i,j)],m[avo_matpos2(i - dim2,j)],m[avo_matpos2(i - dim21,j)]);
			if(!(j % 2))
				bound_max(m[avo_matpos2(i,j)],m[avo_matpos2(i ,j - dim2)],m[avo_matpos2(i ,j - dim12)]);
			if(!(i % 2) && j % 2)
			{
				nsc_3min(m[avo_matpos2(i,j)],m[avo_matpos2(i - dim2,j)],m[avo_matpos2(i - dim12,j)]);
				nsc_3min(m[avo_matpos2(i,j)],m[avo_matpos2(i ,j - dim2 )],m[avo_matpos2(i ,j- dim21)]);
			}
		}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*the most precise closure algorithm*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* unary constraint propagation */
bool avo_hmat_s_step(bound_t* m, size_t dim)
{

  tighten(m,dim);

  size_t i,j,k;
  bound_t *c,ik,ij;

  bound_init(ik); bound_init(ij);

  /* lone S step */
  c = m;
  for (i=0;i<4*dim;i++) {
    bound_div_2(ij,m[avo_matpos(i,i^1)]);
    for (j=0;j<=(i|1);j++,c++) {
      bound_div_2(ik,m[avo_matpos(j^1,j)]);
      bound_badd(ik,ij);
      bound_bmin(*c,ik);
    }
  }

  bound_clear(ik); bound_clear(ij);
  tighten(m,dim);
  /* emptyness checking */
  for (i=0;i<4*dim;i++) {
    c = m+avo_matpos(i,i);
    if (bound_sgn(*c)<0) return true;
    bound_set_int(*c,0);
  }
  tighten(m,dim);
  return false;
}

/* We use a variant of Floyd-Warshall shortest-path closure algorithm, with
   some extra constraint propagation step.

   Returns true if the resulting avo_matrix is empty, false otherwise
   (does not free the avo_matrix)

   Cubic time. Constant space.
 */

bool avo_hmat_close_1(bound_t* m, bound_t * nsc,size_t dim)
{
	tighten(m,dim);
  size_t i,j,k;
  bound_t *c,ik,ik2,ij;

  bound_init(ik); bound_init(ik2); bound_init(ij);

  /* Floyd-Warshall */
  for (k=0;k<4*dim;k++) {
    size_t k2 = k^1;
    c = m;
    for (i=0;i<2*dim;i++) {
      size_t i2 = i|1;
      size_t br = k<i2 ? k : i2;
      bound_set(ik,m[avo_matpos2(i,k)]);
      bound_set(ik2,m[avo_matpos2(i,k2)]);
      for (j=0;j<=br;j++,c++) {
		bound_add(ij,ik,m[avo_matpos(k,j)]);    /* ik+kj */
		bound_bmin(*c,ij);
		bound_add(ij,ik2,m[avo_matpos(k2,j)]);  /* ik2+k2j */
		bound_bmin(*c,ij);
      }
      for (;j<=i2;j++,c++) {
		bound_add(ij,ik,m[avo_matpos(j^1,k2)]); /* ik+kj */
		bound_bmin(*c,ij);
		bound_add(ij,ik2,m[avo_matpos(j^1,k)]); /* ik2+k2j */
		bound_bmin(*c,ij);
      }
    }

  }

  bound_clear(ik); bound_clear(ik2); bound_clear(ij);

  return avo_hmat_s_step(m,dim);
}


/* ============================================================ */
/* Incremental Closure */
/* ============================================================ */


void nsc_add_pro(bound_t des_m,bound_t des_nsc,bound_t m1,bound_t m2,bound_t nsc1,bound_t nsc2,bound_t temp1)
{
	int nsc_temp;
	bound_add(temp1,m1,m2);
	nsc_temp =nsc_add(bound_cmp_int(nsc1,1),bound_cmp_int(nsc2,1));
	nsc_temp =nsc_min(des_m,temp1,bound_cmp_int(des_nsc,1),nsc_temp);
	bound_bmin(des_m,temp1);
	if(nsc_temp > 0)
		bound_set_infty(des_nsc,1);
	else
		bound_set_int(des_nsc,0);
}



bool avo_new_close(bound_t* m , bound_t * nsc,size_t dim)
{
	tighten_nsc(m,nsc,dim);
	size_t i ,j , k,k2,k4;

	bound_t *c,ik,ik2,ij;
	bound_t *c1;
	bound_t nsc_ik,nsc_ik2;
	bound_init(ik); bound_init(ik2); bound_init(ij);
	  /* Floyd-Warshall */
	for (k=0;k<2*dim;k++) {
	    k2 = k^1;
	    c = m;
	    c1 = nsc;
	    for (i=0;i<2*dim;i++) {
	      size_t i2 = i|1;
	      size_t br = k<i2 ? k : i2;
	      bound_set(ik,m[avo_matpos2(i,k)]);
	      bound_set(nsc_ik,nsc[avo_matpos2(i,k)]);
	      bound_set(ik2,m[avo_matpos2(i,k2)]);
	      bound_set(nsc_ik2,nsc[avo_matpos2(i,k2)]);
	      for (j=0;j<=br;j++,c++,c1++) {
			nsc_add_pro(*c,*c1,ik,m[avo_matpos(k,j)],nsc_ik,nsc[avo_matpos(k,j)],ij);
			nsc_add_pro(*c,*c1,ik2,m[avo_matpos(k2,j)],nsc_ik2,nsc[avo_matpos(k2,j)],ij);
	      }
	      for (;j<=i2;j++,c++,c1++) {
			nsc_add_pro(*c,*c1,ik,m[avo_matpos(j^1,k2)],nsc_ik,nsc[avo_matpos(j^1,k2)],ij);
			nsc_add_pro(*c,*c1,ik2,m[avo_matpos(j^1,k)],nsc_ik2,nsc[avo_matpos(j^1,k)],ij);
	      }
	    }

	}

        bound_clear(ik); bound_clear(ik2); bound_clear(ij);
        bound_clear(nsc_ik);
        bound_clear(nsc_ik2);

	bound_t pos_sin_1,pos_sin_2,pos_dou_1,pos_dou_2;
	bound_t temp1;
	int nsc_pos_sin_1,nsc_pos_sin_2,nsc_pos_dou_1,nsc_pos_dou_2;
	bound_t neg_sin_1,neg_sin_2,neg_dou_1,neg_dou_2;
	int nsc_neg_sin_1,nsc_neg_sin_2,nsc_neg_dou_1,nsc_neg_dou_2;
	bound_t pos,neg,pos_sin,pos_dou,neg_sin,neg_dou;
	int nsc_pos,nsc_neg,nsc_pos_sin,nsc_pos_dou,nsc_neg_sin,nsc_neg_dou;
	bound_t temp;
	int nsc_temp;
	bound_t s1;
	bound_t s2;
  	bound_init(temp);
  	bound_init(temp1);
  	bound_init(s1);
  	bound_init(s2);
	bound_init(pos_sin_1); bound_init(pos_sin_2); bound_init(pos_dou_1);
	bound_init(neg_sin_1),bound_init(neg_sin_2),bound_init(neg_dou_1),bound_init(neg_dou_2);
	bound_init(pos),bound_init(neg),bound_init(pos_sin),bound_init(pos_dou),bound_init(neg_sin),bound_init(neg_dou);
	for(i = 0 ; i < 4 * dim ; i++)
	{
	  if(i >= 2*dim && i % 4 == 3)continue;

	  if(i > 2 * dim &&( bound_cmp_int(m[avo_matpos2( i - 2 * dim+ 1 , i - 2 * dim )],0)<= 0||bound_cmp_int(m[avo_matpos2( i - 2 * dim  , i - 2 * dim+ 1 )],0)<= 0))
			continue;

	  for(j = 0 ; j < 4 * dim; j++)
          {
		if(j >= 2 *dim && j % 4 == 2)continue;
		if(j > 2 * dim &&( bound_cmp_int(m[avo_matpos2( j - 2 * dim + 1 , j - 2 * dim )],0)<= 0||bound_cmp_int(m[avo_matpos2( j - 2 * dim , j - 2 * dim + 1)],0)<= 0))
				continue;
		for(k = 0 ; k < dim ; k++)
		{
		    if(( bound_cmp_int(m[avo_matpos2( 2 *k + 1,2 * k )],0)>= 0||bound_cmp_int(m[avo_matpos2( 2*k  , 2*k+ 1 )],0)<= 0))
		    {
			   bound_div_2(s1,m[avo_matpos2(i,i^1)]);
			   bound_div_2(s2,m[avo_matpos2(j^1,j)]);
			   nsc_temp =nsc_add(bound_cmp_int(nsc[avo_matpos2(i,i^1)],1),bound_cmp_int(nsc[avo_matpos2(j^1,j)],1));
			   bound_badd(s2,s1);
			   nsc_temp =nsc_min(m[avo_matpos2(i,j)],s2,bound_cmp_int(nsc[avo_matpos2(i,j)],1),nsc_temp);
			   bound_bmin(m[avo_matpos2(i,j)],s2);
			   if(nsc_temp > 0)
				   bound_set_infty(nsc[avo_matpos2(i,j)],1);
			   else
				   bound_set_int(nsc[avo_matpos2(i,j)],0);
			   if(i < 2 * dim && j< 2 * dim) continue;

			   bound_add(temp1,m[avo_matpos2(i,2*k)],m[avo_matpos2(2*k,j)]);
			   nsc_temp =nsc_add(bound_cmp_int(nsc[avo_matpos2(i,2*k)],1),bound_cmp_int(nsc[avo_matpos2(2*k,j)],1));
			   nsc_temp =nsc_min(m[avo_matpos2(i,j)],temp1,bound_cmp_int(nsc[avo_matpos2(i,j)],1),nsc_temp);
			   bound_bmin(m[avo_matpos2(i,j)],temp1);

			   if(nsc_temp > 0)
				   bound_set_infty(nsc[avo_matpos2(i,j)],1);
			   else
				   bound_set_int(nsc[avo_matpos2(i,j)],0);

			   bound_add(temp1,m[avo_matpos2(i,2*k+1)],m[avo_matpos2(2*k+1,j)]);
						   nsc_temp =nsc_add(bound_cmp_int(nsc[avo_matpos2(i,2*k+1)],1),bound_cmp_int(nsc[avo_matpos2(2*k+1,j)],1));
						   nsc_temp =nsc_min(m[avo_matpos2(i,j)],temp1,bound_cmp_int(nsc[avo_matpos2(i,j)],1),nsc_temp);
						   bound_bmin(m[avo_matpos2(i,j)],temp1);

			   if(nsc_temp > 0)
				   bound_set_infty(nsc[avo_matpos2(i,j)],1);
			   else
				   bound_set_int(nsc[avo_matpos2(i,j)],0);
			   continue;
		       }

		        k2 = 2 * k;
		        k4 = 2 * dim + k2;
		        bound_add(temp,m[avo_matpos2(i,k2)],m[avo_matpos2(i,k4 + 1)]);
		        nsc_temp = nsc_add(bound_cmp_int(nsc[avo_matpos2(i,k2)],1),bound_cmp_int(nsc[avo_matpos2(i,k4 + 1)],1));
		        bound_min(pos_sin_1,m[avo_matpos2(i,i^1)],temp);
		        nsc_pos_sin_1 = nsc_min(m[avo_matpos2(i,i^1)],temp,bound_cmp_int(nsc[avo_matpos2(i,i^1)],1),nsc_temp);
		        bound_add(temp,m[avo_matpos2(k2 + 1,j)],m[avo_matpos2(k4,j)]);
		        nsc_temp = nsc_add(bound_cmp_int(nsc[avo_matpos2(k2 + 1,j)],1),bound_cmp_int(nsc[avo_matpos2(k4,j)],1));
		        bound_min(pos_sin_2,m[avo_matpos2(j^1,j)],temp);
		        nsc_pos_sin_2 = nsc_min(m[avo_matpos2(j^1,j)],temp,bound_cmp_int(nsc[avo_matpos2(j^1,j)],1),nsc_temp);
		        bound_add(pos_sin,pos_sin_1,pos_sin_2);
		        nsc_pos_sin = nsc_add(nsc_pos_sin_1,nsc_pos_sin_2);
		        bound_div_2(pos_sin,pos_sin);
		        bound_min(temp,m[avo_matpos2(i,k2+1)],m[avo_matpos2(i,k4+1)]);
		        nsc_temp = nsc_min(m[avo_matpos2(i,k2+1)],m[avo_matpos2(i,k4+1)],bound_cmp_int(nsc[avo_matpos2(i,k2+1)],1),bound_cmp_int(nsc[avo_matpos2(i,k4+1)],1));
		        bound_add(pos_dou_1,temp,m[avo_matpos2(k2+1,j)]);
		        nsc_pos_dou_1= nsc_add(nsc_temp,bound_cmp_int(nsc[avo_matpos2(k2+1,j)],1));
			bound_min(temp,m[avo_matpos2(k2,j)],m[avo_matpos2(k4,j)]);
			nsc_temp = nsc_min(m[avo_matpos2(k2,j)],m[avo_matpos2(k4,j)],bound_cmp_int(nsc[avo_matpos2(k2,j)],1),bound_cmp_int(nsc[avo_matpos2(k4,j)],1));
			bound_add(pos_dou_2,temp,m[avo_matpos2(i,k2)]);
			nsc_pos_dou_2 = nsc_add(nsc_temp,bound_cmp_int(nsc[avo_matpos2(i,k2)],1));
			bound_min(pos_dou,pos_dou_2,pos_dou_1);
			nsc_pos_dou = nsc_min(pos_dou_2,pos_dou_1,nsc_pos_dou_2,nsc_pos_dou_1);
			bound_min(pos,pos_dou,pos_sin);
			nsc_pos = nsc_min(pos_dou,pos_sin,nsc_pos_dou,nsc_pos_sin);


			bound_add(temp,m[avo_matpos2(i,k2+1)],m[avo_matpos2(i,k4 + 1)]);
			nsc_temp = nsc_add(bound_cmp_int(nsc[avo_matpos2(i,k2+1)],1),bound_cmp_int(nsc[avo_matpos2(i,k4 + 1)],1));

			bound_min(neg_sin_1,m[avo_matpos2(i,i^1)],temp);
			nsc_neg_sin_1 = nsc_min(m[avo_matpos2(i,i^1)],temp,bound_cmp_int(nsc[avo_matpos2(i,i^1)],1),nsc_temp);

			bound_add(temp,m[avo_matpos2(k2,j)],m[avo_matpos2(k4,j)]);
			nsc_temp = nsc_add(bound_cmp_int(nsc[avo_matpos2(k2,j)],1),bound_cmp_int(nsc[avo_matpos2(k4,j)],1));

			bound_min(neg_sin_2,m[avo_matpos2(j^1,j)],temp);
			nsc_neg_sin_2 = nsc_min(m[avo_matpos2(j^1,j)],temp,bound_cmp_int(nsc[avo_matpos2(j^1,j)],1),nsc_temp);

			bound_add(neg_sin,neg_sin_1,neg_sin_2);
			nsc_neg_sin = nsc_add(nsc_neg_sin_1,nsc_neg_sin_2);

			bound_div_2(neg_sin,neg_sin);
			bound_min(temp,m[avo_matpos2(i,k2)],m[avo_matpos2(i,k4+1)]);
			nsc_temp = nsc_min(m[avo_matpos2(i,k2)],m[avo_matpos2(i,k4+1)],bound_cmp_int(nsc[avo_matpos2(i,k2)],1),bound_cmp_int(nsc[avo_matpos2(i,k4+1)],1));

			bound_add(neg_dou_1,temp,m[avo_matpos2(k2,j)]);
			nsc_neg_dou_1= nsc_add(nsc_temp,bound_cmp_int(nsc[avo_matpos2(k2,j)],1));

			bound_min(temp,m[avo_matpos2(k2+1,j)],m[avo_matpos2(k4,j)]);
			nsc_temp = nsc_min(m[avo_matpos2(k2+1,j)],m[avo_matpos2(k4,j)],bound_cmp_int(nsc[avo_matpos2(k2+1,j)],1),bound_cmp_int(nsc[avo_matpos2(k4,j)],1));

			bound_add(neg_dou_2,temp,m[avo_matpos2(i,k2+1)]);
			nsc_neg_dou_2 = nsc_add(nsc_temp,bound_cmp_int(nsc[avo_matpos2(i,k2+1)],1));

			bound_min(neg_dou,neg_dou_2,neg_dou_1);
			nsc_neg_dou = nsc_min(neg_dou_2,neg_dou_1,nsc_neg_dou_2,nsc_neg_dou_1);

			bound_min(neg,neg_dou,neg_sin);
			nsc_neg = nsc_min(neg_dou,neg_sin,nsc_neg_dou,nsc_neg_sin);

			bound_max(temp,pos,neg);
			nsc_temp = nsc_max(pos,neg,nsc_pos,nsc_neg);

			bound_min(m[avo_matpos2(i,j)],m[avo_matpos2(i,j)],temp);
			nsc_temp = nsc_min(m[avo_matpos2(i,j)],temp,bound_cmp_int(nsc[avo_matpos2(i,j)],1),nsc_temp);
			if(nsc_temp < 0)
				bound_set_int(nsc[avo_matpos2(i,j)],0);
			else
				bound_set_infty(nsc[avo_matpos2(i,j)],1);
		}
	  }
	}
	bound_clear(s2);bound_clear(s1);
	bound_clear(pos_sin_1); bound_clear(pos_sin_2); bound_clear(pos_dou_1);bound_clear(temp1);
	bound_clear(neg_sin_1),bound_clear(neg_sin_2),bound_clear(neg_dou_1),bound_clear(neg_dou_2);
	bound_clear(pos),bound_clear(neg),bound_clear(pos_sin),bound_clear(pos_dou),bound_clear(neg_sin),bound_clear(neg_dou);
        tighten(m,dim);
	for (i=0;i<4*dim;i++) {
	    c = m+avo_matpos2(i,i);
	    if (bound_sgn(*c)<0) return true;
	    bound_set_int(*c,0);
	}

	tighten_nsc(m,nsc,dim);
	return false;
}

bool avo_new_close_incremental(bound_t * m, bound_t * nsc,size_t dim, size_t v)
{
	size_t ljc,lll;
	tighten_nsc(m,nsc,dim);
	size_t i ,j , k,k2,k4;
	bound_t *c;
	bound_t *c1;
	bound_t ij,ik,ik2;
        bound_t nsc_ik,nsc_ik2;
	bound_init(ik); bound_init(ik2); bound_init(ij);
	  /* incremental Floyd-Warshall : v in end-point position */
	for (k=0;k<2*dim;k++) {
	    size_t kk = k^1;
	    size_t ii = 2*v+1;
	    size_t br  = k<ii ? k : ii;
	    for (i=2*v;i<2*v+2;i++) {
	      /* v in first end-point position */
	      c = m+avo_matpos(i,0);
	      c1 = nsc+avo_matpos(i,0);
	      bound_set(ik,m[avo_matpos2(i,k)]);
	      bound_set(nsc_ik,nsc[avo_matpos2(i,k)]);
	      bound_set(ik2,m[avo_matpos2(i,kk)]);
	      bound_set(nsc_ik2,nsc[avo_matpos2(i,kk)]);
	      for (j=0;j<=br;j++,c++,c1++) {
			nsc_add_pro(*c,*c1,ik,m[avo_matpos(k,j)],nsc_ik,nsc[avo_matpos(k,j)],ij);
			nsc_add_pro(*c,*c1,ik2,m[avo_matpos(kk,j)],nsc_ik2,nsc[avo_matpos(kk,j)],ij);
	      }
	      for (;j<=ii;j++,c++,c1++) {
			nsc_add_pro(*c,*c1,ik,m[avo_matpos(j^1,kk)],nsc_ik,nsc[avo_matpos(j^1,kk)],ij);
			nsc_add_pro(*c,*c1,ik2,m[avo_matpos(j^1,k)],nsc_ik2,nsc[avo_matpos(j^1,k)],ij);
	      }
	      /* v in second end-point position */
	      bound_set(ik,m[avo_matpos2(k,i)]);
	      bound_set(nsc_ik,nsc[avo_matpos2(k,i)]);
	      bound_set(ik2,m[avo_matpos2(kk,i)]);
	      bound_set(nsc_ik2,nsc[avo_matpos2(kk,i)]);
	      for (j=i;j<k;j++) {
			c = m+avo_matpos(j,i);
			c1 = nsc+avo_matpos(j,i);
			nsc_add_pro(*c,*c1,ik,m[avo_matpos(kk,j^1)],nsc_ik,nsc[avo_matpos(kk,j^1)],ij);
			nsc_add_pro(*c,*c1,ik2,m[avo_matpos(k,j^1)],nsc_ik2,nsc[avo_matpos(k,j^1)],ij);
	      }
	      for (;j<4*dim;j++) {
			c = m+avo_matpos(j,i);
			c1 = nsc+avo_matpos(j,i);
			nsc_add_pro(*c,*c1,ik,m[avo_matpos(j,k)],nsc_ik,nsc[avo_matpos(j,k)],ij);
			nsc_add_pro(*c,*c1,ik2,m[avo_matpos(j,kk)],nsc_ik2,nsc[avo_matpos(j,kk)],ij);
	      }
	    }
	  }


	  /* incremental Floyd-Warshall : v in pivot position */
	  for (k=2*v;k<2*v+2;k++) {
	    size_t kk = k^1;
	    c = m;
	    c1 = nsc;
	    for (i=0;i<4*dim;i++) {
	      size_t ii = i|1;
	      size_t br = k<ii ? k : ii;
	      bound_set(ik,m[avo_matpos2(i,k)]);
	      bound_set(nsc_ik,nsc[avo_matpos2(i,k)]);
	      bound_set(ik2,m[avo_matpos2(i,kk)]);
	      bound_set(nsc_ik2,nsc[avo_matpos2(i,kk)]);
	      for (j=0;j<=br;j++,c++,c1++) {
	    	  nsc_add_pro(*c,*c1,ik,m[avo_matpos(k,j)],nsc_ik,nsc[avo_matpos(k,j)],ij);
	    	  nsc_add_pro(*c,*c1,ik2,m[avo_matpos(kk,j)],nsc_ik2,nsc[avo_matpos(kk,j)],ij);
	      }
	      for (;j<=ii;j++,c++,c1++) {
	    	  nsc_add_pro(*c,*c1,ik,m[avo_matpos(j^1,kk)],nsc_ik,nsc[avo_matpos(j^1,kk)],ij);
	    	  nsc_add_pro(*c,*c1,ik2,m[avo_matpos(j^1,k)],nsc_ik2,nsc[avo_matpos(j^1,k)],ij);
	      }
	    }
	  }

	  bound_clear(ik); bound_clear(ik2); bound_clear(ij);
	  bound_t pos_sin_1,pos_sin_2,pos_dou_1,pos_dou_2;
	  int nsc_pos_sin_1,nsc_pos_sin_2,nsc_pos_dou_1,nsc_pos_dou_2;
	  bound_t neg_sin_1,neg_sin_2,neg_dou_1,neg_dou_2;
	  int nsc_neg_sin_1,nsc_neg_sin_2,nsc_neg_dou_1,nsc_neg_dou_2;
	  bound_t pos,neg,pos_sin,pos_dou,neg_sin,neg_dou;
	  int nsc_pos,nsc_neg,nsc_pos_sin,nsc_pos_dou,nsc_neg_sin,nsc_neg_dou;
	  bound_t temp,temp1;
	  int nsc_temp;
	  bound_t s1;
	  bound_t s2;
          k = v;
          size_t temp_i;
	  bound_init(temp);bound_init(temp1);bound_init(s1);bound_init(s2);
	  bound_init(pos_sin_1); bound_init(pos_sin_2); bound_init(pos_dou_1);
	  bound_init(neg_sin_1),bound_init(neg_sin_2),bound_init(neg_dou_1),bound_init(neg_dou_2);
	  bound_init(pos),bound_init(neg),bound_init(pos_sin),bound_init(pos_dou),bound_init(neg_sin),bound_init(neg_dou);
	  size_t times = 0;
	  for(i = 0 ; i < 4 * dim ; i++)
	  {

		if(i > 2 * dim && i % 4 == 3) continue;
		if(i > 2 * dim &&( bound_cmp_int(m[avo_matpos2( i - 2 * dim + 1, i - 2 * dim )],0)<= 0||bound_cmp_int(m[avo_matpos2( i - 2 * dim  , i - 2 * dim+ 1 )],0)<= 0))
			continue;
	        for(j = 0 ; j < 4 * dim; j++)
	        {
		  if(j > 2 * dim && j % 4 == 2) continue;
		  if(j > 2 * dim &&( bound_cmp_int(m[avo_matpos2( j - 2 * dim+ 1  , j - 2 * dim)],0)<= 0||bound_cmp_int(m[avo_matpos2( j - 2 * dim , j - 2 * dim+ 1 )],0)<= 0))
			   continue;

		  if(( bound_cmp_int(m[avo_matpos2( 2 *k + 1,2 * k )],0)<= 0||bound_cmp_int(m[avo_matpos2( 2*k, 2*k + 1)],0)<= 0))
		  {
			bound_div_2(s1,m[avo_matpos2(i,i^1)]);
			bound_div_2(s2,m[avo_matpos2(j^1,j)]);
			nsc_temp =nsc_add(bound_cmp_int(nsc[avo_matpos2(i,i^1)],1),bound_cmp_int(nsc[avo_matpos2(j^1,j)],1));
			bound_badd(s2,s1);
			nsc_temp =nsc_min(m[avo_matpos2(i,j)],s2,bound_cmp_int(nsc[avo_matpos2(i,j)],1),nsc_temp);
			bound_bmin(m[avo_matpos2(i,j)],s2);
			if(nsc_temp > 0)
				bound_set_infty(nsc[avo_matpos2(i,j)],1);
			else
				bound_set_int(nsc[avo_matpos2(i,j)],0);


			if(i < 2 * dim && j < 2 *dim) continue;
			bound_add(temp1,m[avo_matpos2(i,2*k)],m[avo_matpos2(2*k,j)]);
			nsc_temp =nsc_add(bound_cmp_int(nsc[avo_matpos2(i,2*k)],1),bound_cmp_int(nsc[avo_matpos2(2*k,j)],1));
			nsc_temp =nsc_min(m[avo_matpos2(i,j)],temp1,bound_cmp_int(nsc[avo_matpos2(i,j)],1),nsc_temp);
			bound_bmin(m[avo_matpos2(i,j)],temp1);

		    	if(nsc_temp > 0)
			   bound_set_infty(nsc[avo_matpos2(i,j)],1);
		    	else
			   bound_set_int(nsc[avo_matpos2(i,j)],0);


			bound_add(temp1,m[avo_matpos2(i,2*k+1)],m[avo_matpos2(2*k+1,j)]);
			nsc_temp =nsc_add(bound_cmp_int(nsc[avo_matpos2(i,2*k+1)],1),bound_cmp_int(nsc[avo_matpos2(2*k+1,j)],1));
			nsc_temp =nsc_min(m[avo_matpos2(i,j)],temp1,bound_cmp_int(nsc[avo_matpos2(i,j)],1),nsc_temp);
			bound_bmin(m[avo_matpos2(i,j)],temp1);

		    	if(nsc_temp > 0)
			   bound_set_infty(nsc[avo_matpos2(i,j)],1);
		    	else
			   bound_set_int(nsc[avo_matpos2(i,j)],0);
			bound_add(temp1,m[avo_matpos2(2*k,i)],m[avo_matpos2(i,j)]);
			nsc_temp =nsc_add(bound_cmp_int(nsc[avo_matpos2(2*k,i)],1),bound_cmp_int(nsc[avo_matpos2(i,j)],1));
			nsc_temp =nsc_min(m[avo_matpos2(2*k,j)],temp1,bound_cmp_int(nsc[avo_matpos2(2*k,j)],1),nsc_temp);
			bound_bmin(m[avo_matpos2(2*k,j)],temp1);
			if(nsc_temp > 0)
				bound_set_infty(nsc[avo_matpos2(2*k,j)],1);
			else
				bound_set_int(nsc[avo_matpos2(2*k,j)],0);

			bound_add(temp1,m[avo_matpos2(2*k+1,i)],m[avo_matpos2(i,j)]);
			nsc_temp =nsc_add(bound_cmp_int(nsc[avo_matpos2(2*k+1,i)],1),bound_cmp_int(nsc[avo_matpos2(i,j)],1));
			nsc_temp =nsc_min(m[avo_matpos2(2*k+1,j)],temp1,bound_cmp_int(nsc[avo_matpos2(2*k+1,j)],1),nsc_temp);
			bound_bmin(m[avo_matpos2(2*k+1,j)],temp1);
			if(nsc_temp > 0)
			   bound_set_infty(nsc[avo_matpos2(2*k+1,j)],1);
			else
			   bound_set_int(nsc[avo_matpos2(2*k+1,j)],0);
			continue;
		  }

		  k2 = 2 * k;
		  k4 = 2 * dim + k2;
		  bound_add(temp,m[avo_matpos2(i,k2)],m[avo_matpos2(i,k4 + 1)]);
		  nsc_temp = nsc_add(bound_cmp_int(nsc[avo_matpos2(i,k2)],1),bound_cmp_int(nsc[avo_matpos2(i,k4 + 1)],1));
		  bound_min(pos_sin_1,m[avo_matpos2(i,i^1)],temp);
		  nsc_pos_sin_1 = nsc_min(m[avo_matpos2(i,i^1)],temp,bound_cmp_int(nsc[avo_matpos2(i,i^1)],1),nsc_temp);
		  bound_add(temp,m[avo_matpos2(k2 + 1,j)],m[avo_matpos2(k4,j)]);
		  nsc_temp = nsc_add(bound_cmp_int(nsc[avo_matpos2(k2 + 1,j)],1),bound_cmp_int(nsc[avo_matpos2(k4,j)],1));
		  bound_min(pos_sin_2,m[avo_matpos2(j^1,j)],temp);
		  nsc_pos_sin_2 = nsc_min(m[avo_matpos2(j^1,j)],temp,bound_cmp_int(nsc[avo_matpos2(j^1,j)],1),nsc_temp);
		  bound_add(pos_sin,pos_sin_1,pos_sin_2);
		  nsc_pos_sin = nsc_add(nsc_pos_sin_1,nsc_pos_sin_2);
		  bound_div_2(pos_sin,pos_sin);
		  bound_min(temp,m[avo_matpos2(i,k2+1)],m[avo_matpos2(i,k4+1)]);
		  nsc_temp = nsc_min(m[avo_matpos2(i,k2+1)],m[avo_matpos2(i,k4+1)],bound_cmp_int(nsc[avo_matpos2(i,k2+1)],1),bound_cmp_int(nsc[avo_matpos2(i,k4+1)],1));
		  bound_add(pos_dou_1,temp,m[avo_matpos2(k2+1,j)]);
		  nsc_pos_dou_1= nsc_add(nsc_temp,bound_cmp_int(nsc[avo_matpos2(k2+1,j)],1));
		  bound_min(temp,m[avo_matpos2(k2,j)],m[avo_matpos2(k4,j)]);
		  nsc_temp = nsc_min(m[avo_matpos2(k2,j)],m[avo_matpos2(k4,j)],bound_cmp_int(nsc[avo_matpos2(k2,j)],1),bound_cmp_int(nsc[avo_matpos2(k4,j)],1));
		  bound_add(pos_dou_2,temp,m[avo_matpos2(i,k2)]);
		  nsc_pos_dou_2 = nsc_add(nsc_temp,bound_cmp_int(nsc[avo_matpos2(i,k2)],1));
		  bound_min(pos_dou,pos_dou_2,pos_dou_1);
		  nsc_pos_dou = nsc_min(pos_dou_2,pos_dou_1,nsc_pos_dou_2,nsc_pos_dou_1);
		  bound_min(pos,pos_dou,pos_sin);
		  nsc_pos = nsc_min(pos_dou,pos_sin,nsc_pos_dou,nsc_pos_sin);

		  bound_add(temp,m[avo_matpos2(i,k2+1)],m[avo_matpos2(i,k4 + 1)]);
		  nsc_temp = nsc_add(bound_cmp_int(nsc[avo_matpos2(i,k2+1)],1),bound_cmp_int(nsc[avo_matpos2(i,k4 + 1)],1));

		  bound_min(neg_sin_1,m[avo_matpos2(i,i^1)],temp);
		  nsc_neg_sin_1 = nsc_min(m[avo_matpos2(i,i^1)],temp,bound_cmp_int(nsc[avo_matpos2(i,i^1)],1),nsc_temp);

		  bound_add(temp,m[avo_matpos2(k2,j)],m[avo_matpos2(k4,j)]);
		  nsc_temp = nsc_add(bound_cmp_int(nsc[avo_matpos2(k2,j)],1),bound_cmp_int(nsc[avo_matpos2(k4,j)],1));

		  bound_min(neg_sin_2,m[avo_matpos2(j^1,j)],temp);
		  nsc_neg_sin_2 = nsc_min(m[avo_matpos2(j^1,j)],temp,bound_cmp_int(nsc[avo_matpos2(j^1,j)],1),nsc_temp);

		  bound_add(neg_sin,neg_sin_1,neg_sin_2);
		  nsc_neg_sin = nsc_add(nsc_neg_sin_1,nsc_neg_sin_2);
  
		  bound_div_2(neg_sin,neg_sin);
		  bound_min(temp,m[avo_matpos2(i,k2)],m[avo_matpos2(i,k4+1)]);
		  nsc_temp = nsc_min(m[avo_matpos2(i,k2)],m[avo_matpos2(i,k4+1)],bound_cmp_int(nsc[avo_matpos2(i,k2)],1),bound_cmp_int(nsc[avo_matpos2(i,k4+1)],1));

		  bound_add(neg_dou_1,temp,m[avo_matpos2(k2,j)]);
		  nsc_neg_dou_1= nsc_add(nsc_temp,bound_cmp_int(nsc[avo_matpos2(k2,j)],1));

		  bound_min(temp,m[avo_matpos2(k2+1,j)],m[avo_matpos2(k4,j)]);
		  nsc_temp = nsc_min(m[avo_matpos2(k2+1,j)],m[avo_matpos2(k4,j)],bound_cmp_int(nsc[avo_matpos2(k2+1,j)],1),bound_cmp_int(nsc[avo_matpos2(k4,j)],1));

		  bound_add(neg_dou_2,temp,m[avo_matpos2(i,k2+1)]);
		  nsc_neg_dou_2 = nsc_add(nsc_temp,bound_cmp_int(nsc[avo_matpos2(i,k2+1)],1));

		  bound_min(neg_dou,neg_dou_2,neg_dou_1);
		  nsc_neg_dou = nsc_min(neg_dou_2,neg_dou_1,nsc_neg_dou_2,nsc_neg_dou_1);

		  bound_min(neg,neg_dou,neg_sin);
		  nsc_neg = nsc_min(neg_dou,neg_sin,nsc_neg_dou,nsc_neg_sin);

		  bound_max(temp,pos,neg);
		  nsc_temp = nsc_max(pos,neg,nsc_pos,nsc_neg);

		  bound_min(m[avo_matpos2(i,j)],m[avo_matpos2(i,j)],temp);
		  nsc_temp = nsc_min(m[avo_matpos2(i,j)],temp,bound_cmp_int(nsc[avo_matpos2(i,j)],1),nsc_temp);
		  if(nsc_temp < 0)
			bound_set_int(nsc[avo_matpos2(i,j)],0);
		  else
			bound_set_infty(nsc[avo_matpos2(i,j)],1);
		}
	 }
	 bound_clear(pos_sin_1); bound_clear(pos_sin_2); bound_clear(pos_dou_1);bound_clear(temp1);
	 bound_clear(s1);bound_clear(s2);
	 bound_clear(neg_sin_1),bound_clear(neg_sin_2),bound_clear(neg_dou_1),bound_clear(neg_dou_2);
	 bound_clear(pos),bound_clear(neg),bound_clear(pos_sin),bound_clear(pos_dou),bound_clear(neg_sin),bound_clear(neg_dou);
	 tighten(m,dim);
	 for (i=0;i<4*dim;i++) {
	    c = m+avo_matpos2(i,i);
	    if (bound_sgn(*c)<0) return true;
	    bound_set_int(*c,0);
	 }
	 tighten_nsc(m,nsc,dim);

	 return false;
}

bool avo_hmat_close_incremental_1(bound_t* m, bound_t * nsc,size_t dim, size_t v)
{

  size_t i,j,k;
  bound_t *c;
  bound_t ij,ik,ik2;

  bound_init(ik); bound_init(ik2); bound_init(ij);

  /* incremental Floyd-Warshall : v in end-point position */
  for (k=0;k<2*dim;k++) {
    size_t kk = k^1;
    size_t ii = 2*v+1;
    size_t br  = k<ii ? k : ii;
    for (i=2*v;i<2*v+2;i++) {
      /* v in first end-point position */
      c = m+avo_matpos(i,0);
      bound_set(ik,m[avo_matpos2(i,k)]);
      bound_set(ik2,m[avo_matpos2(i,kk)]);
      for (j=0;j<=br;j++,c++) {
		bound_add(ij,ik,m[avo_matpos(k,j)]);    /* ik+kj */
		bound_bmin(*c,ij);
		bound_add(ij,ik2,m[avo_matpos(kk,j)]);  /* ik2+k2j */
		bound_bmin(*c,ij);
      }
      for (;j<=ii;j++,c++) {
		bound_add(ij,ik,m[avo_matpos(j^1,kk)]); /* ik+kj */
		bound_bmin(*c,ij);
		bound_add(ij,ik2,m[avo_matpos(j^1,k)]); /* ik2+k2j */
		bound_bmin(*c,ij);
      }
      /* v in second end-point position */
      bound_set(ik,m[avo_matpos2(k,i)]);
      bound_set(ik2,m[avo_matpos2(kk,i)]);
      for (j=i;j<k;j++) {
		c = m+avo_matpos(j,i);
		bound_add(ij,ik,m[avo_matpos(kk,j^1)]); /* ik+kj */
		bound_bmin(*c,ij);
		bound_add(ij,ik2,m[avo_matpos(k,j^1)]); /* ik2+k2j */
		bound_bmin(*c,ij);
      }
      for (;j<4*dim;j++) {
		c = m+avo_matpos(j,i);
		bound_add(ij,ik,m[avo_matpos(j,k)]);    /* ik+kj */
		bound_bmin(*c,ij);
		bound_add(ij,ik2,m[avo_matpos(j,kk)]);  /* ik2+k2j */
		bound_bmin(*c,ij);
      }
    }
  }

  /* incremental Floyd-Warshall : v in pivot position */
  for (k=2*v;k<2*v+2;k++) {
    size_t kk = k^1;
    c = m;
    for (i=0;i<4*dim;i++) {
      size_t ii = i|1;
      size_t br = k<ii ? k : ii;
      bound_set(ik,m[avo_matpos2(i,k)]);
      bound_set(ik2,m[avo_matpos2(i,kk)]);
      for (j=0;j<=br;j++,c++) {
		bound_add(ij,ik,m[avo_matpos(k,j)]);    /* ik+kj */
		bound_bmin(*c,ij);
		bound_add(ij,ik2,m[avo_matpos(kk,j)]);  /* ik2+k2j */
		bound_bmin(*c,ij);
      }
      for (;j<=ii;j++,c++) {
		bound_add(ij,ik,m[avo_matpos(j^1,kk)]); /* ik+kj */
		bound_bmin(*c,ij);
		bound_add(ij,ik2,m[avo_matpos(j^1,k)]); /* ik2+k2j */
		bound_bmin(*c,ij);
      }
    }
  }

  bound_clear(ik); bound_clear(ik2); bound_clear(ij);

  return avo_hmat_s_step(m,dim);
}

//////////////////////////////////////////

/*the most precise closure algorithm*/
int new_floy(bound_t * qua)
{
    size_t k , j ,i,jc,ic,kc;
    bound_t temp;
    for(k = 0 ; k < 6 ; k++)
    for(i = 0 ; i < 6 ; i++)
    for(j = 0 ; j < 6 ; j++)
    {
	   ic = i^1; jc = j^1; kc = k^1;
	   bound_add(temp,qua[avo_matpos2(k,j)],qua[avo_matpos2(i,k)] );
	   bound_bmin(qua[avo_matpos2(i,j)],temp);
	   bound_add(temp,qua[avo_matpos2(kc,j)],qua[avo_matpos2(i,kc)]);
	   bound_bmin(qua[avo_matpos2(i,j)],temp);
	   bound_add(temp,qua[avo_matpos2(kc,j)],qua[avo_matpos2(i,k)]);
	   bound_add(temp,temp,qua[avo_matpos2(k,kc)]);
	   bound_bmin(qua[avo_matpos2(i,j)],temp);
	   bound_add(temp,qua[avo_matpos2(k,j)],qua[avo_matpos2(i,kc)]);
	   bound_add(temp,temp,qua[avo_matpos2(kc,k)]);
	   bound_bmin(qua[avo_matpos2(i,j)],temp);
	   bound_add(temp,qua[avo_matpos2(i,ic)],qua[avo_matpos2(jc,j)]);
	   bound_div_2(temp,temp);
	   bound_bmin(qua[avo_matpos2(i,j)],temp);
	   bound_bmin(qua[avo_matpos2(i,j)],qua[avo_matpos2(jc,ic)]);
    }
    for(i = 0; i < 6 ; i++)
    	if(bound_sgn(qua[avo_matpos2(i,i)]) < 0)
    		return 0;
    return 1;
}

void tight_non_to_av(int time,bound_t * qua, bound_t * temp, bound_t *str)
{
    size_t i, j ;
    if(time == 1)
    {
           for (i = 0 ; i < 12 ; i++)
           for( j = 0 ; j < 12 ; j++)
           {
        	   bound_set_infty(str[avo_matpos2(i,j)],1);
        	   if( i < 6 && j < 6)bound_set(str[avo_matpos2(i,j)],qua[avo_matpos2(i,j)]);
           }
           tighten(str,3);
    }
    else
    {
    	for (i = 0 ; i < 12 ; i++)
	    for( j = 0 ; j < 12 ; j++)
	    {
		bound_set_infty(temp[avo_matpos2(i,j)],1);
		if( i < 6 && j < 6)bound_set(temp[avo_matpos2(i,j)],qua[avo_matpos2(i,j)]);
	    }

    	tighten(temp,3);

        for ( i = 0 ; i < 12 ; i++)
            for( j =0 ; j < 12 ; j++)
            	bound_max(str[avo_matpos2(i,j)],str[avo_matpos2(i,j)],temp[avo_matpos2(i,j)]);
    }

}

void tight_av_to_non(bound_t * qua, bound_t *m,int * flag, int * hash,size_t dim)
{
    size_t i,j;
    size_t t_i[6];
    size_t t_j[6];
    size_t g_i[6];
    size_t g_j[6];
    bound_t t;
    bound_set_int(t,0);

    for(i = 0 ; i < 6 ; i++)
    	g_i[i] = 2 * hash[i / 2]+ i % 2;
    for(j = 0 ; j < 6 ; j++)
    	g_j[j] = 2 * hash[j / 2]+ j % 2;
    for(i = 0 ; i < 6 ; i++)
    	if(flag[i/2]) t_i[i] = 2 * dim + 2 * hash[i/2] + i % 2;
    	else t_i[i] = 2 * dim + 2 * hash[i/2] + 1 - i % 2;
    for(j = 0 ; j < 6 ; j++)
    	if(flag[j/2]) t_j[j] = 2 * dim + 2 * hash[j/2] + j % 2;
    	else t_j[j] = 2 * dim + 2 * hash[j/2] + 1 - j % 2;
    for(i = 0 ; i < 6 ; i++)
    	for(j = 0 ; j < 6 ; j++)
    		bound_set_infty(qua[avo_matpos2(i,j)],1);

    for(i = 0 ; i < 6 ; i++)
    for(j = 0 ; j < 6 ; j++)
    {
    	bound_bmin(qua[avo_matpos2(i,j)],m[avo_matpos2(g_i[i],g_j[j])]);
    	bound_bmin(qua[avo_matpos2(i,j)],m[avo_matpos2(t_i[i],g_j[j])]);
    	bound_bmin(qua[avo_matpos2(i,j)],m[avo_matpos2(g_i[i],t_j[j])]);
    	bound_bmin(qua[avo_matpos2(i,j)],m[avo_matpos2(t_i[i],t_j[j])]);
    }

   for(i = 0 ; i < 3 ; i++)
	   if(flag[i])
		   bound_bmin(qua[avo_matpos2(2 * i,2 * i + 1)],t);
	   else
		   bound_bmin(qua[avo_matpos2(2 * i + 1,2 * i )],t);
}

int qua_tighten(size_t x , size_t y, size_t z, bound_t * qua, bound_t * str,bound_t * temp,bound_t * m,size_t dim)
{

    if( x == y || y == z || x == z )
	 return 1;
    size_t i , j;
    size_t ha_i[12];
    size_t ha_j[12];
    int flag[3];
    int hash[3];
    hash[0] = x;
    hash[1] = y;
    hash[2] = z;
    int ti_time = 1;
    size_t ljc,lll;

    for(i = 0 ; i < 12 ; i++)
    for(j = 0 ; j < 12 ; j++)
	bound_set_infty(str[avo_matpos2(i,j)],1);

    for(int r = 0 ; r < 8 ;r ++)
    {
	   flag[0] = r % 2;flag[1] = r / 4; flag[2] = (r/2) % 2;
	   tight_av_to_non(qua,m,flag,hash,dim);

	   if(new_floy(qua))
	   {

		 tight_non_to_av(ti_time,qua,temp,str);
		 ti_time++;
	   }
    }

    for(i = 0 ; i < 6 ;i++)
	ha_i[i] = hash[i/2] * 2 + i % 2;
    for(j = 0 ; j < 6 ;j++)
	ha_j[j] = hash[j/2] * 2 + j % 2;
    for(i = 6 ; i < 12 ;i++)
	ha_i[i] = hash[(i-6)/2] * 2 + 2 * dim + i % 2;
    for(j = 6 ; j < 12 ;j++)
	ha_j[j] = hash[(j-6)/2] * 2 + 2 * dim + j % 2;
    for(i = 0 ; i < 12 ;i++)
    for(j = 0 ; j < 12 ;j++)
	bound_bmin(m[avo_matpos2(ha_i[i],ha_j[j])],str[avo_matpos2(i,j)]);

    if(ti_time == 1)
	return 0;
    return 1;
}

void test(void)
{
	size_t dim = 4;
	bound_t *qua= malloc(sizeof(bound_t) * avo_matsize(6));
	bound_t *str= malloc(sizeof(bound_t) * avo_matsize(6));
	bound_t *temp=malloc(sizeof(bound_t) * avo_matsize(6));
	bound_t *m=malloc(sizeof(bound_t) * avo_matsize(dim));
	size_t i , j , k;
	size_t ljc,lll;
	for(i = 0 ; i < 16 ; i ++)
		for(j = 0 ; j < 16 ; j ++)
			bound_set_infty(m[avo_matpos2(i,j)],1);
	bound_set_int(m[avo_matpos2(3,6)],35);
	for(ljc = 0 ; ljc < 4 * dim ; ljc++)
	{
		  for(lll= 0 ; lll < ljc + 2 - ljc % 2 ;lll++)
		  {
		    bound_print(m[avo_matpos2(ljc,lll)]);
		    fprintf(stdout, "   "); fflush(stdout);
		  }
		  fprintf(stdout, "\n"); fflush(stdout);
	}

	for(k = 0 ; k < dim ; k++)
	 	for(i = k ; i < dim ; i++)
	 		for(j = i ; j < dim ; j++)
	 		{
	 			qua_tighten(i,j,k,qua,str,temp,m,dim);
	 		}



	for(ljc = 0 ; ljc < 4 * dim ; ljc++)
	{
		  for(lll= 0 ; lll < ljc + 2 - ljc % 2 ;lll++)
		  {
		     bound_print(m[avo_matpos2(ljc,lll)]);
		     fprintf(stdout, "   "); fflush(stdout);
		  }
		  fprintf(stdout, "\n"); fflush(stdout);
	}
	return ;
}


int new_floy_nsc(bound_t * qua,bound_t * qua_nsc)
{
    size_t k , j ,i,jc,ic,kc;
    bound_t temp;
    int  nsc_temp;
    for(k = 0 ; k < 6 ; k++)
      for(i = 0 ; i < 6 ; i++)
        for(j = 0 ; j < 6 ; j++)
	{
		   ic = i^1; jc = j^1; kc = k^1;
		   bound_add(temp,qua[avo_matpos2(k,j)],qua[avo_matpos2(i,k)] );
		   nsc_temp = nsc_add(bound_cmp_int(qua_nsc[avo_matpos2(k,j)],1),bound_cmp_int(qua_nsc[avo_matpos2(i,k)],1));
		   if(nsc_min(qua[avo_matpos2(i,j)],temp,bound_cmp_int(qua_nsc[avo_matpos2(i,j)],1),nsc_temp)>0)
			   bound_set_infty(qua_nsc[avo_matpos2(i,j)],1);
		   else
			   bound_set_int(qua_nsc[avo_matpos2(i,j)],0);
		   bound_bmin(qua[avo_matpos2(i,j)],temp);
		   bound_add(temp,qua[avo_matpos2(kc,j)],qua[avo_matpos2(i,kc)]);
		   nsc_temp = nsc_add(bound_cmp_int(qua_nsc[avo_matpos2(kc,j)],1),bound_cmp_int(qua_nsc[avo_matpos2(i,kc)],1));
		   if(nsc_min(qua[avo_matpos2(i,j)],temp,bound_cmp_int(qua_nsc[avo_matpos2(i,j)],1),nsc_temp)>0)
			   bound_set_infty(qua_nsc[avo_matpos2(i,j)],1);
	           else
			   bound_set_int(qua_nsc[avo_matpos2(i,j)],0);
		   bound_bmin(qua[avo_matpos2(i,j)],temp);
		   bound_add(temp,qua[avo_matpos2(kc,j)],qua[avo_matpos2(i,k)]);
		   nsc_temp = nsc_add(bound_cmp_int(qua_nsc[avo_matpos2(kc,j)],1),bound_cmp_int(qua_nsc[avo_matpos2(i,k)],1));
		   bound_add(temp,temp,qua[avo_matpos2(k,kc)]);
		   nsc_temp = nsc_add(nsc_temp,bound_cmp_int(qua_nsc[avo_matpos2(k,kc)],1));
		   if(nsc_min(qua[avo_matpos2(i,j)],temp,bound_cmp_int(qua_nsc[avo_matpos2(i,j)],1),nsc_temp)>0)
			  bound_set_infty(qua_nsc[avo_matpos2(i,j)],1);
		   else
			  bound_set_int(qua_nsc[avo_matpos2(i,j)],0);
		   bound_bmin(qua[avo_matpos2(i,j)],temp);
		   bound_add(temp,qua[avo_matpos2(k,j)],qua[avo_matpos2(i,kc)]);
		   nsc_temp = nsc_add(bound_cmp_int(qua_nsc[avo_matpos2(k,j)],1),bound_cmp_int(qua_nsc[avo_matpos2(i,kc)],1));
		   bound_add(temp,temp,qua[avo_matpos2(kc,k)]);
		   nsc_temp = nsc_add(nsc_temp,bound_cmp_int(qua_nsc[avo_matpos2(kc,k)],1));
		   if(nsc_min(qua[avo_matpos2(i,j)],temp,bound_cmp_int(qua_nsc[avo_matpos2(i,j)],1),nsc_temp)>0)
			  bound_set_infty(qua_nsc[avo_matpos2(i,j)],1);
		   else
			  bound_set_int(qua_nsc[avo_matpos2(i,j)],0);
		   bound_bmin(qua[avo_matpos2(i,j)],temp);
		   bound_add(temp,qua[avo_matpos2(i,ic)],qua[avo_matpos2(jc,j)]);
		   bound_div_2(temp,temp);
		   nsc_temp = nsc_add(bound_cmp_int(qua_nsc[avo_matpos2(i,ic)],1),bound_cmp_int(qua_nsc[avo_matpos2(jc,j)],1));
		   if(nsc_min(qua[avo_matpos2(i,j)],temp,bound_cmp_int(qua_nsc[avo_matpos2(i,j)],1),nsc_temp)>0)
			   bound_set_infty(qua_nsc[avo_matpos2(i,j)],1);
		   else
			   bound_set_int(qua_nsc[avo_matpos2(i,j)],0);
		   bound_bmin(qua[avo_matpos2(i,j)],temp);

		   if(nsc_min(qua[avo_matpos2(i,j)],qua[avo_matpos2(jc,ic)],bound_cmp_int(qua_nsc[avo_matpos2(i,j)],1),bound_cmp_int(qua_nsc[avo_matpos2(jc,ic)],1))>0)
			   bound_set_infty(qua_nsc[avo_matpos2(i,j)],1);
		   else
			   bound_set_int(qua_nsc[avo_matpos2(i,j)],0);
		   bound_bmin(qua[avo_matpos2(i,j)],qua[avo_matpos2(jc,ic)]);
        }
    for(i = 0; i < 6 ; i++)
		if(bound_sgn(qua[avo_matpos2(i,i)]) < 0)
			return 0;
    return 1;
}

void tight_non_to_av_nsc(int time,bound_t * qua,bound_t* qua_nsc, bound_t * temp,bound_t* temp_nsc, bound_t *str,bound_t *str_nsc)
{

    size_t i , j,ljc,lll ;
    int nsc_temp;
    if(time == 1)
    {
       for (i = 0 ; i < 12 ; i++)
           for( j = 0 ; j < 12 ; j++)
           {
        	   bound_set_infty(str[avo_matpos2(i,j)],1);
        	   bound_set_infty(str_nsc[avo_matpos2(i,j)],1);
        	   if( i < 6 && j < 6)
        	   {
        		   bound_set(str[avo_matpos2(i,j)],qua[avo_matpos2(i,j)]);
        		   bound_set(str_nsc[avo_matpos2(i,j)],qua_nsc[avo_matpos2(i,j)]);
        	   }
           }
       tighten_nsc(str,str_nsc,3);
    }
    else
    {
    	for (i = 0 ; i < 12 ; i++)
    	     for( j = 0 ; j < 12 ; j++)
    	     {
				   bound_set_infty(temp[avo_matpos2(i,j)],1);
				   bound_set_infty(temp_nsc[avo_matpos2(i,j)],1);
				   if( i < 6 && j < 6)
				   {
					   bound_set(temp[avo_matpos2(i,j)],qua[avo_matpos2(i,j)]);
					   bound_set(temp_nsc[avo_matpos2(i,j)],qua_nsc[avo_matpos2(i,j)]);
				   }
    	     }

    	tighten_nsc(temp,temp_nsc,3);


        for ( i = 0 ; i < 12 ; i++)
               for ( j = 0 ; j < 12 ; j++)
               {
            	   nsc_temp = nsc_max(str[avo_matpos2(i,j)],temp[avo_matpos2(i,j)],bound_cmp_int(str_nsc[avo_matpos2(i,j)],1),bound_cmp_int(temp_nsc[avo_matpos2(i,j)],1));

            	   if(nsc_temp > 0)
            	   {
            	      bound_set_infty(str_nsc[avo_matpos2(i,j)],1);
            	   }
            	   else
            	   {
            		  bound_set_int(str_nsc[avo_matpos2(i,j)],0);
            	   }
            	   bound_max(str[avo_matpos2(i,j)],str[avo_matpos2(i,j)],temp[avo_matpos2(i,j)]);

               }
    }
}

void tight_av_to_non_nsc(bound_t * qua,bound_t * qua_nsc, bound_t *m,bound_t * nsc,int * flag, int * hash,size_t dim)
{
    size_t i,j,ljc,lll;
    size_t t_i[6];
    size_t t_j[6];
    size_t g_i[6];
    size_t g_j[6];
    bound_t t;
    bound_set_int(t,0);

    for(i = 0 ; i < 6 ; i++)
    	g_i[i] = 2 * hash[i / 2]+ i % 2;
    for(j = 0 ; j < 6 ; j++)
    	g_j[j] = 2 * hash[j / 2]+ j % 2;
    for(i = 0 ; i < 6 ; i++)
    	if(flag[i/2]) t_i[i] = 2 * dim + 2 * hash[i/2] + i % 2;
    	else t_i[i] = 2 * dim + 2 * hash[i/2] + 1 - i % 2;
    for(j = 0 ; j < 6 ; j++)
    	if(flag[j/2]) t_j[j] = 2 * dim + 2 * hash[j/2] + j % 2;
    	else t_j[j] = 2 * dim + 2 * hash[j/2] + 1 - j % 2;
    for(i = 0 ; i < 6 ; i++)
    	for(j = 0 ; j < 6 ; j++)
    	{
		bound_set_infty(qua[avo_matpos2(i,j)],1);
		bound_set_infty(qua_nsc[avo_matpos2(i,j)],1);
    	}

    for(i = 0 ; i < 6 ; i++)
    	for(j = 0 ; j < 6 ; j++)
	{

	    if(nsc_min(qua[avo_matpos2(i,j)],m[avo_matpos2(g_i[i],g_j[j])],bound_cmp_int(qua_nsc[avo_matpos2(i,j)],1),bound_cmp_int(nsc[avo_matpos2(g_i[i],g_j[j])],1))<1)
		bound_set_int(qua_nsc[avo_matpos2(i,j)],0);
	    else
		bound_set_infty(qua_nsc[avo_matpos2(i,j)],1);
	    bound_bmin(qua[avo_matpos2(i,j)],m[avo_matpos2(g_i[i],g_j[j])]);

	    if(nsc_min(qua[avo_matpos2(i,j)],m[avo_matpos2(t_i[i],g_j[j])],bound_cmp_int(qua_nsc[avo_matpos2(i,j)],1),bound_cmp_int(nsc[avo_matpos2(t_i[i],g_j[j])],1))<1)
		bound_set_int(qua_nsc[avo_matpos2(i,j)],0);
	    else
	    	bound_set_infty(qua_nsc[avo_matpos2(i,j)],1);
	    bound_bmin(qua[avo_matpos2(i,j)],m[avo_matpos2(t_i[i],g_j[j])]);

	    if(nsc_min(qua[avo_matpos2(i,j)],m[avo_matpos2(g_i[i],t_j[j])],bound_cmp_int(qua_nsc[avo_matpos2(i,j)],1),bound_cmp_int(nsc[avo_matpos2(g_i[i],t_j[j])],1))<1)
		bound_set_int(qua_nsc[avo_matpos2(i,j)],0);
	    else
		bound_set_infty(qua_nsc[avo_matpos2(i,j)],1);
	    bound_bmin(qua[avo_matpos2(i,j)],m[avo_matpos2(g_i[i],t_j[j])]);

	    if(nsc_min(qua[avo_matpos2(i,j)],m[avo_matpos2(t_i[i],t_j[j])],bound_cmp_int(qua_nsc[avo_matpos2(i,j)],1),bound_cmp_int(nsc[avo_matpos2(t_i[i],t_j[j])],1))<1)
		bound_set_int(qua_nsc[avo_matpos2(i,j)],0);
	    else
		bound_set_infty(qua_nsc[avo_matpos2(i,j)],1);
	    bound_bmin(qua[avo_matpos2(i,j)],m[avo_matpos2(t_i[i],t_j[j])]);
	  }
       for(i = 0 ; i < 3 ; i++)
       {
    	   bound_set_infty(qua_nsc[avo_matpos2(2 * i,2 * i + 1)],1);
    	   bound_set_infty(qua_nsc[avo_matpos2(2 * i+1,2 * i )],1);
	   if(flag[i])
		   bound_bmin(qua[avo_matpos2(2 * i,2 * i + 1)],t);
	   else
		   bound_bmin(qua[avo_matpos2(2 * i + 1,2 * i )],t);
       }
}

int qua_tighten_nsc(size_t x , size_t y, size_t z, bound_t * qua, bound_t * str,bound_t * temp,bound_t * m,bound_t *nsc,size_t dim,bound_t* qua_nsc,bound_t * str_nsc,bound_t *temp_nsc )
{
	if( x == y || y == z || x == z )
		return 1;

	size_t i , j;
	int nsc_temp;
	size_t ha_i[12];
	size_t ha_j[12];
	int flag[3];
	int hash[3];
	hash[0] = x;
	hash[1] = y;
	hash[2] = z;
	int ti_time = 1;
	size_t ljc,lll;

	for(i = 0 ; i < 12 ; i++)for(j = 0 ; j < 12 ; j++)
	{
		bound_set_infty(str[avo_matpos2(i,j)],1);
		bound_set_infty(str_nsc[avo_matpos2(i,j)],1);
	}

	for(int r = 0 ; r < 8 ;r ++)
	{
	   flag[0] = r % 2;flag[1] = r / 4; flag[2] = (r/2) % 2;
	   tight_av_to_non_nsc(qua,qua_nsc,m,nsc,flag,hash,dim);
	   if(new_floy_nsc(qua,qua_nsc))
	   {
		 tight_non_to_av_nsc(ti_time,qua,qua_nsc,temp,temp_nsc,str,str_nsc);
		 ti_time++;
	   }
	}

	for(i = 0 ; i < 6 ;i++)
		ha_i[i] = hash[i/2] * 2 + i % 2;
	for(j = 0 ; j < 6 ;j++)
		ha_j[j] = hash[j/2] * 2 + j % 2;
	for(i = 6 ; i < 12 ;i++)
		ha_i[i] = hash[(i-6)/2] * 2 + 2 * dim + i % 2;
	for(j = 6 ; j < 12 ;j++)
		ha_j[j] = hash[(j-6)/2] * 2 + 2 * dim + j % 2;
	for(i = 0 ; i < 12 ;i++)
	  for(j = 0 ; j < 12 ;j++)
	  {
		 nsc_temp = nsc_min(m[avo_matpos2(ha_i[i],ha_j[j])],str[avo_matpos2(i,j)],bound_cmp_int(nsc[avo_matpos2(ha_i[i],ha_j[j])],1),bound_cmp_int(str_nsc[avo_matpos2(i,j)],1));
		 if(nsc_temp > 0)
			 bound_set_infty(nsc[avo_matpos2(ha_i[i],ha_j[j])],1);
		 else
			 bound_set_int(nsc[avo_matpos2(ha_i[i],ha_j[j])],0);
		 bound_bmin(m[avo_matpos2(ha_i[i],ha_j[j])],str[avo_matpos2(i,j)]);
	  }

	if(ti_time == 1)
		return 0;
	return 1;
}



bool avo_hmat_close_2(bound_t* m, bound_t * nsc,size_t dim)
{
   //test();
    bound_t *qua= malloc(sizeof(bound_t) * avo_matsize(6));
    bound_t *str= malloc(sizeof(bound_t) * avo_matsize(6));
    bound_t *temp=malloc(sizeof(bound_t) * avo_matsize(6));
    bound_t *qua_nsc= malloc(sizeof(bound_t) * avo_matsize(6));
    bound_t *str_nsc= malloc(sizeof(bound_t) * avo_matsize(6));
    bound_t *temp_nsc=malloc(sizeof(bound_t) * avo_matsize(6));
    size_t i , j , k;
    size_t ljc,lll;
    if(dim > 2 )
    {
	for(k = 0 ; k < dim ; k++)
	for(i = k ; i < dim ; i++)
	for(j = i ; j < dim ; j++)
	{
		qua_tighten_nsc(i,j,k,qua,str,temp,m,nsc,dim,qua_nsc,str_nsc,temp_nsc);
	}
   }
   else
   {
	tighten_nsc(m,nsc,dim);
   }
   bound_clear_array(qua,avo_matsize(6));
   bound_clear_array(str,avo_matsize(6));
   bound_clear_array(temp,avo_matsize(6));
   bound_clear_array(qua_nsc,avo_matsize(6));
   bound_clear_array(str_nsc,avo_matsize(6));
   bound_clear_array(temp_nsc,avo_matsize(6));
   return avo_hmat_s_step(m,dim);


}
bool avo_hmat_close_incremental_2(bound_t* m, bound_t* nsc,size_t dim, size_t v)
{
   bound_t *qua= malloc(sizeof(bound_t) * avo_matsize(6));
   bound_t *str= malloc(sizeof(bound_t) * avo_matsize(6));
   bound_t *temp=malloc(sizeof(bound_t) * avo_matsize(6));
   bound_t *qua_nsc= malloc(sizeof(bound_t) * avo_matsize(6));
   bound_t *str_nsc= malloc(sizeof(bound_t) * avo_matsize(6));
   bound_t *temp_nsc=malloc(sizeof(bound_t) * avo_matsize(6));
   size_t i , j , k,ljc,lll;
   if(dim > 2 )
   {
	for(i = 0 ; i < dim ; i++)
	for(j = i ; j < dim ; j++)
	{
		qua_tighten_nsc(i,j,v,qua,str,temp,m,nsc,dim,qua_nsc,str_nsc,temp_nsc);
	}
   }
   else
   {
   	tighten_nsc(m,nsc,dim);
   }
   bound_clear_array(qua,avo_matsize(6));
   bound_clear_array(str,avo_matsize(6));
   bound_clear_array(temp,avo_matsize(6));

   bound_clear_array(qua_nsc,avo_matsize(6));
   bound_clear_array(str_nsc,avo_matsize(6));
   bound_clear_array(temp_nsc,avo_matsize(6));
   return avo_hmat_s_step(m,dim);

}

/* now the strongest algorithm   */
int new_floy_3(bound_t * qua, size_t dim)
{
    size_t k , j ,i,jc,ic,kc;
    bound_t temp;
    for(k = 0 ; k < 2 * dim ; k++)
    for(i = 0 ; i < 2 * dim ; i++)
    for(j = 0 ; j < 2 * dim ; j++)
    {
       ic = i^1; jc = j^1; kc = k^1;
       bound_add(temp,qua[avo_matpos2(k,j)],qua[avo_matpos2(i,k)] );
       bound_bmin(qua[avo_matpos2(i,j)],temp);
       bound_add(temp,qua[avo_matpos2(kc,j)],qua[avo_matpos2(i,kc)]);
       bound_bmin(qua[avo_matpos2(i,j)],temp);
       bound_add(temp,qua[avo_matpos2(kc,j)],qua[avo_matpos2(i,k)]);
       bound_add(temp,temp,qua[avo_matpos2(k,kc)]);
       bound_bmin(qua[avo_matpos2(i,j)],temp);
       bound_add(temp,qua[avo_matpos2(k,j)],qua[avo_matpos2(i,kc)]);
       bound_add(temp,temp,qua[avo_matpos2(kc,k)]);
       bound_bmin(qua[avo_matpos2(i,j)],temp);
       bound_add(temp,qua[avo_matpos2(i,ic)],qua[avo_matpos2(jc,j)]);
       bound_div_2(temp,temp);
       bound_bmin(qua[avo_matpos2(i,j)],temp);
       bound_bmin(qua[avo_matpos2(i,j)],qua[avo_matpos2(jc,ic)]);
    }
    for(i = 0; i < 2 * dim ; i++)
    	if(bound_sgn(qua[avo_matpos2(i,i)]) < 0)
    		return 0;
    return 1;
}

void tight_non_to_av_3(int time,bound_t * qua, bound_t * temp, bound_t *str,size_t dim)
{

    size_t i , j ;
    if(time == 1)
    {
           for (i = 0 ; i < 4 * dim ; i++)
           for( j = 0 ; j < 4 * dim ; j++)
           {
        	   bound_set_infty(str[avo_matpos2(i,j)],1);
        	   if( i < 2 * dim && j < 2 * dim)
        		   bound_set(str[avo_matpos2(i,j)],qua[avo_matpos2(i,j)]);
           }
           tighten(str,dim);
    }
    else
    {
    	for (i = 0 ; i < 4 * dim ; i++)
    	for( j = 0 ; j < 4 * dim ; j++)
	{
	   bound_set_infty(temp[avo_matpos2(i,j)],1);
	   if( i < 2 * dim && j < 2 * dim)
		   bound_set(temp[avo_matpos2(i,j)],qua[avo_matpos2(i,j)]);
	}
	tighten(temp,dim);

        for ( i = 0 ; i < 4 * dim ; i++)
	    for( j =0 ; j < 4 * dim ; j++)
		   bound_max(str[avo_matpos2(i,j)],str[avo_matpos2(i,j)],temp[avo_matpos2(i,j)]);
    }
}

void tight_av_to_non_3(bound_t * qua, bound_t *m,int * flag,size_t dim)
{
    size_t i,j;
    size_t *t_i = malloc(sizeof(size_t) * 2 * dim);
    size_t *t_j = malloc(sizeof(size_t) * 2 * dim);
    bound_t t;
    bound_set_int(t,0);

    for(i = 0 ; i < 2 * dim ; i++)
    	if(flag[i/2]) t_i[i] = 2 * dim + i;
    	else t_i[i] = 2 * dim + (i^1);
    for(j = 0 ; j < 2 * dim ; j++)
    	if(flag[j/2]) t_j[j] = 2 * dim + j;
    	else t_j[j] = 2 * dim + (j^1);
    for(i = 0 ; i < 2 * dim ; i++)
    	for(j = 0 ; j < 2 * dim ; j++)
    		bound_set_infty(qua[avo_matpos2(i,j)],1);

    for(i = 0 ; i < 2 * dim ; i++)
    for(j = 0 ; j <  2 * dim ; j++)
    {
    	bound_bmin(qua[avo_matpos2(i,j)],m[avo_matpos2(i,j)]);
    	bound_bmin(qua[avo_matpos2(i,j)],m[avo_matpos2(t_i[i],j)]);
    	bound_bmin(qua[avo_matpos2(i,j)],m[avo_matpos2(i,t_j[j])]);
    	bound_bmin(qua[avo_matpos2(i,j)],m[avo_matpos2(t_i[i],t_j[j])]);
    }

   for(i = 0 ; i < dim ; i++)
	   if(flag[i])
		   bound_bmin(qua[avo_matpos2(2 * i,2 * i + 1)],t);
	   else
		   bound_bmin(qua[avo_matpos2(2 * i + 1,2 * i )],t);
}

size_t power(size_t po)
{
	size_t ret = 1 ;
	while(po)
	{
		po--;
		ret*=2;
	}
	return ret;
}
void assign_flag(int * flag, size_t r,size_t dim)
{
	size_t i;
	for(i = 0 ; i < dim;i++)
		flag[i] = 0;
	i = 0;
	while(r)
	{
		flag[i] = r % 2;
		i++;
		r = r / 2;
	}
}

int qua_tighten_3(bound_t * qua, bound_t * str,bound_t * temp,bound_t * m,size_t dim)
{
	size_t i , j;
	int *flag = malloc(sizeof(int) * dim);
	int ti_time = 1;
	for(i = 0 ; i < 4 * dim ; i++)
		for(j = 0 ; j < 4 * dim ; j++)
			bound_set_infty(str[avo_matpos2(i,j)],1);
        for(int r = 0 ; r < (int)power(dim) ;r ++)
	{
	    assign_flag(flag,r,dim);
	    tight_av_to_non_3(qua,m,flag,dim);
		if(new_floy_3(qua,dim))
		{
		  tight_non_to_av_3(ti_time,qua,temp,str,dim);
		  ti_time++;
		}
	}



	for(i = 0 ; i < 4 * dim ;i++)
	for(j = 0 ; j < 4 * dim ;j++)
		bound_bmin(m[avo_matpos2(i,j)],str[avo_matpos2(i,j)]);
	if(ti_time == 1)
		return 0;
	return 1;
}

bool avo_hmat_close_3(bound_t* m, bound_t * nsc,size_t dim)
{
   //test();
   bound_t *qua= malloc(sizeof(bound_t) * avo_matsize(dim));
   bound_t *str= malloc(sizeof(bound_t) * avo_matsize(dim));
   bound_t *temp=malloc(sizeof(bound_t) * avo_matsize(dim));

   size_t i , j , k;
   if(dim > 2 )
   {
	qua_tighten_3(qua, str,temp,m,dim);
   }
   else
	tighten(m,dim);
   bound_clear_array(qua,avo_matsize(dim));
   bound_clear_array(str,avo_matsize(dim));
   bound_clear_array(temp,avo_matsize(dim));
   return avo_hmat_s_step(m,dim);
}

bool avo_hmat_close_incremental_3(bound_t* m, bound_t * nsc,size_t dim, size_t v)
{
    bound_t *qua= malloc(sizeof(bound_t) * avo_matsize(dim));
    bound_t *str= malloc(sizeof(bound_t) * avo_matsize(dim));
    bound_t *temp=malloc(sizeof(bound_t) * avo_matsize(dim));
    size_t i , j , k;
    if(dim > 2 )
    {
	   qua_tighten_3(qua, str,temp,m,dim);
    }
    else
	   tighten(m,dim);
    bound_clear_array(qua,avo_matsize(dim));
    bound_clear_array(str,avo_matsize(dim));
    bound_clear_array(temp,avo_matsize(dim));
    return avo_hmat_s_step(m,dim);
}

//////////////////////////////////////////
bool avo_hmat_close(bound_t* m,bound_t * nsc, size_t dim)
{
	//return avo_hmat_close_3(m,nsc, dim); // the 8 orthants method
	return avo_new_close(m,nsc, dim);  //  the 2 orthants method
	//return avo_hmat_close_2(m,nsc, dim); // the 8 orthants method
}
bool avo_hmat_close_incremental(bound_t* m, bound_t * nsc,size_t dim, size_t v)
{
	//	return avo_hmat_close_incremental_3(m,nsc,dim,v); // the 8 orthants method
	return avo_new_close_incremental(m,nsc,dim,v); // the 2 orthants method
	//return avo_hmat_close_incremental_2(m,nsc,dim,v); // the 8 orthants method
}

/* ============================================================ */
/* Sanity Check */
/* ============================================================ */

bool avo_hmat_check_closed(bound_t* m, size_t dim)
{
  bool closed = true;
  size_t i,j,k;
  bound_t w;

  bound_init(w);

  for (i=0;i<4*dim;i++)
    for (j=0;j<4*dim;j++)
      for (k=0;k<4*dim;k++) {
		bound_add(w,m[avo_matpos2(i,k)],m[avo_matpos2(k,j)]);
		if (bound_cmp(m[avo_matpos2(i,j)],w)>0) closed = false;
      }

  for (i=0;i<4*dim;i++)
    for (j=0;j<4*dim;j++)
      if (bound_cmp(m[avo_matpos2(i,j)],m[avo_matpos2(j^1,i^1)])>0) closed = false;

  for (i=0;i<4*dim;i++)
    for (j=0;j<4*dim;j++) {
      bound_add(w,m[avo_matpos2(i,i^1)],m[avo_matpos2(j^1,j)]);
      bound_div_2(w,w);
      if (bound_cmp(m[avo_matpos2(i,j)],w)>0) closed = false;
    }

  bound_clear(w);

  return closed;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

















