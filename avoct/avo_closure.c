/*
 * avo_closure.c
 *
 * Closure algorithms
 *
 * APRON Library / Absolute Value Octagonal (AVO) Domain
 *
 * Copyright (C) Liqian Chen & Jiangchao Liu' 2014-
 *
 */

/* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution.
*/

#include "avo.h"
#include "avo_internal.h"

/* All closures are in-place. */
int avo_closure_alg=1;

/* *
 * 0: WeakCloVia1Sign(i.e., enumerating the signs of 1 variables each time)
 * 1(default): WeakCloVia3Sign(i.e., enumerating the signs of 3 variables each time)
 * 2: StrongCloViaAllSigns(i.e., enumerating the signs of all variables each time)
 * */

/* ============================================================ */
/* Basic functions */
/* ============================================================ */

/* 2^{po} */
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

/* encoding (falg) of one orthant (r) in (dim) dimension */
void orthant_encoding_nd(size_t * flag, size_t r,size_t dim)
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

/* ============================================================ */
/* Tighten between Vk and |Vk| */
/* ============================================================ */

/* use oct part to tighten av part, and vice versa
 * considering the strictness of inequalities
 * */
void tighten_nsc(bound_t* m, bound_t* nsc, size_t dim)
{
     size_t j,i;
     size_t dim2 = dim * 2;
     size_t dim21 = dim2 + 1;
     size_t dim12 = dim2 - 1;
     bound_t b_zero, temp, temp_nsc;
     bound_init(b_zero); bound_set_int(b_zero,0); bound_init(temp); bound_init(temp_nsc);

     for (i = 0 ; i < dim; i++)
     {
		 if(bound_cmp_int(m[avo_matpos2((2*i)^1,2*i)],0)<=0)
		 {   // Vi<=0 =>  Vi==-|Vi|  and -Vi==|Vi|
			 bound_bmin_nsc(m+avo_matpos2((2*i)^1,2*i),nsc+avo_matpos2((2*i)^1,2*i),m[avo_matpos2(2*i+2*dim,(2*i+2*dim)^1)],nsc[avo_matpos2(2*i+2*dim,(2*i+2*dim)^1)]);
			 bound_bmin_nsc(m+avo_matpos2(2*i+2*dim,(2*i+2*dim)^1),nsc+avo_matpos2(2*i+2*dim,(2*i+2*dim)^1),m[avo_matpos2((2*i)^1,2*i)],nsc[avo_matpos2((2*i)^1,2*i)]);

			 bound_bmin_nsc(m+avo_matpos2(2*i,(2*i)^1),nsc+avo_matpos2(2*i,(2*i)^1),m[avo_matpos2((2*i+2*dim)^1,2*i+2*dim)],nsc[avo_matpos2((2*i+2*dim)^1,2*i+2*dim)]);
			 bound_bmin_nsc(m+avo_matpos2((2*i+2*dim)^1,2*i+2*dim),nsc+avo_matpos2((2*i+2*dim)^1,2*i+2*dim),m[avo_matpos2(2*i,(2*i)^1)],nsc[avo_matpos2(2*i,(2*i)^1)]);
		 }
		 if(bound_cmp_int(m[avo_matpos2(2*i,(2*i)^1)],0)<=0)
		 {   // -Vi<=0 =>  Vi==|Vi| and -Vi==-|Vi|
			 bound_bmin_nsc(m+avo_matpos2(2*i,(2*i)^1),nsc+avo_matpos2(2*i,(2*i)^1),m[avo_matpos2(2*i+2*dim,(2*i+2*dim)^1)],nsc[avo_matpos2(2*i+2*dim,(2*i+2*dim)^1)]);
			 bound_bmin_nsc(m+avo_matpos2(2*i+2*dim,(2*i+2*dim)^1),nsc+avo_matpos2(2*i+2*dim,(2*i+2*dim)^1),m[avo_matpos2(2*i,(2*i)^1)],nsc[avo_matpos2(2*i,(2*i)^1)]);

			 bound_bmin_nsc(m+avo_matpos2((2*i)^1,2*i),nsc+avo_matpos2((2*i)^1,2*i),m[avo_matpos2((2*i+2*dim)^1,2*i+2*dim)],nsc[avo_matpos2((2*i+2*dim)^1,2*i+2*dim)]);
			 bound_bmin_nsc(m+avo_matpos2((2*i+2*dim)^1,2*i+2*dim),nsc+avo_matpos2((2*i+2*dim)^1,2*i+2*dim),m[avo_matpos2((2*i)^1,2*i)],nsc[avo_matpos2((2*i)^1,2*i)]);
		 }
     }

     for(i = 2 * dim; i < 4 * dim ; i ++)
     {
		for(j = 0 ; j < 2 * dim; j++)
		{
			if(i % 2==1)
			{  // Vj + |Vi| <= (Vj + Vi or Vj - Vi)
				bound_set(temp, m[avo_matpos2(i - dim2,j)]); bound_set(temp_nsc, nsc[avo_matpos2(i - dim2,j)]);
				bound_bmax_nsc(&temp, &temp_nsc, m[avo_matpos2(i - dim21,j)],nsc[avo_matpos2(i - dim21,j)]);
				bound_bmin_nsc(m+avo_matpos2(i,j),nsc+avo_matpos2(i,j),temp, temp_nsc);
			}
			else
			{  // Vj - |Vi| <= (Vj - Vi and Vj + Vi)
				bound_bmin_nsc(m+avo_matpos2(i,j),nsc+avo_matpos2(i,j),m[avo_matpos2(i - dim2,j)],nsc[avo_matpos2(i - dim2,j)]);
				bound_bmin_nsc(m+avo_matpos2(i,j),nsc+avo_matpos2(i,j),m[avo_matpos2(i - dim12,j)],nsc[avo_matpos2(i - dim12,j)]);
			}
		}
	}

	for(i = 2 * dim ; i < 4 *dim ; i++)
		for(j = 2 * dim ; j <= (i|1) ; j++)
		{
			if(i % 2 ==1 )  /*  *+|Vi|<=  */
			{
				   //  *+Vi<=c1  ;  *-Vi<= c2 => *+|Vi|<= max(c1,c2)
				bound_set(temp, m[avo_matpos2(i - dim2,j)]);
				bound_set(temp_nsc, nsc[avo_matpos2(i - dim2,j)]);
				bound_bmax_nsc(&temp, &temp_nsc, m[avo_matpos2(i - dim21,j)],nsc[avo_matpos2(i - dim21,j)]);
				bound_bmin_nsc(m+avo_matpos2(i,j),nsc+avo_matpos2(i,j),temp, temp_nsc);
			}
			if(j % 2 == 0)    //  |Vj|-*<=
			{
				bound_set(temp,m[avo_matpos2(i ,j - dim2)]); bound_set(temp_nsc,nsc[avo_matpos2(i, j - dim2)]);
				bound_bmax_nsc(&temp, &temp_nsc,m[avo_matpos2(i ,j - dim12)],nsc[avo_matpos2(i ,j - dim12)]);
				bound_bmin_nsc(m+avo_matpos2(i,j),nsc+avo_matpos2(i,j),temp, temp_nsc);
			}
			if(i % 2 ==0 && j % 2 == 1 )  //  -|Vj|-|Vi|<= *
			{
				if(j==(i^1)){ // -|Vi|-|Vi|<= *
					// -|Vi|-|Vi|<= -Vi-Vi
					bound_bmin_nsc(m+avo_matpos2(i,j),nsc+avo_matpos2(i,j),m[avo_matpos2(i-2*dim,(i-2*dim)^1)],nsc[avo_matpos2(i-2*dim,(i-2*dim)^1)]);
					// -|Vi|-|Vi|<= Vi+Vi
					bound_bmin_nsc(m+avo_matpos2(i,j),nsc+avo_matpos2(i,j),m[avo_matpos2((i-2*dim)^1,i-2*dim)],nsc[avo_matpos2((i-2*dim)^1,i-2*dim)]);
				}
				  //  -|Vj|-|Vi|<= -|Vj|<= *
				bound_div_2(temp, m[avo_matpos2(j^1,j)]);
				bound_bmin_nsc(m+avo_matpos2(i,j),nsc+avo_matpos2(i,j),temp,nsc[avo_matpos2(j^1,j)]);

				  //  -|Vj|-|Vi|<= -|Vi|<= *
				bound_div_2(temp, m[avo_matpos2(i,i^1)]);
				bound_bmin_nsc(m+avo_matpos2(i,j),nsc+avo_matpos2(i,j),temp,nsc[avo_matpos2(i,i^1)]);

				//  -|Vj|-|Vi|<= 0, -|Vi|<= 0
				if(bound_cmp(b_zero,m[avo_matpos2(i,j)]) < 0){
					bound_set(m[avo_matpos2(i,j)],b_zero);
					bound_set_infty(nsc[avo_matpos2(i,j)],1);
				}
			}

	}
    bound_clear(b_zero); bound_clear(temp);bound_clear(temp_nsc);
}

/* Tighten M_{ii,jj} (ii and jj range in [0,4dim]
 * when variable k (range in [0,dim]) is either definitely non-positive or definitely non-negative.
 * */
void tighten_nsc_IJviaK_fixedSignK(bound_t* m , bound_t* nsc, size_t dim, size_t ii, size_t jj, size_t k)
{
   bound_t s1, s2;
   bound_t temp, nsc_temp;
   bound_init(temp); bound_init(nsc_temp);
   bound_init(s1);   bound_init(s2);

   /* i -> j, e.g.,  (2Vi' + (-2Vj'))/2 */
   bound_div_2(s1,m[avo_matpos2(ii,ii^1)]);
   bound_div_2(s2,m[avo_matpos2(jj^1,jj)]);
   bound_min(nsc_temp,nsc[avo_matpos2(ii,ii^1)],nsc[avo_matpos2(jj^1,jj)]);
   bound_badd(s2,s1);
   bound_bmin_nsc(m+avo_matpos2(ii,jj),nsc+avo_matpos2(ii,jj),s2,nsc_temp);

   if(ii < 2 * dim && jj< 2 * dim) return;

   /* i -> 2k -> j, i.e.,  Vi' -> Vk -> Vj' */
   bound_add(temp,m[avo_matpos2(ii,2*k)],m[avo_matpos2(2*k,jj)]);
   bound_min(nsc_temp,nsc[avo_matpos2(ii,2*k)],nsc[avo_matpos2(2*k,jj)]);
   bound_bmin_nsc(m+avo_matpos2(ii,jj),nsc+avo_matpos2(ii,jj),temp,nsc_temp);

   /* i -> 2k+1 -> j, i.e.,  Vi' -> -Vk -> Vj' */
   bound_add(temp,m[avo_matpos2(ii,(2*k)^1)],m[avo_matpos2((2*k)^1,jj)]);
   bound_min(nsc_temp,nsc[avo_matpos2(ii,(2*k)^1)],nsc[avo_matpos2((2*k)^1,jj)]);
   bound_bmin_nsc(m+avo_matpos2(ii,jj),nsc+avo_matpos2(ii,jj),temp,nsc_temp);
}

/* Tighten M_{ii,jj} (ii and jj range in [0,4dim]
 * assumign variable k (range in [0,dim]) is either positive or negative.
 * */
void tighten_nsc_IJviaK_signK(bound_t* pos, bound_t* nsc_pos, bound_t* m , bound_t* nsc, size_t dim, size_t ii, size_t jj, size_t k, bool positiveK)
{
	size_t k2, k4;
	bound_t temp, pos_sin, pos_sin_1,pos_sin_2,pos_dou, pos_dou_1,pos_dou_2;
	bound_t nsc_temp, nsc_pos_sin, nsc_pos_sin_1,nsc_pos_sin_2,nsc_pos_dou, nsc_pos_dou_1,nsc_pos_dou_2;

  	bound_init(temp); bound_init(pos_sin); bound_init(pos_sin_1); bound_init(pos_sin_2);
  	bound_init(pos_dou); bound_init(pos_dou_1); bound_init(pos_dou_2);
  	bound_init(nsc_temp); bound_init(nsc_pos_sin); bound_init(nsc_pos_sin_1); bound_init(nsc_pos_sin_2);
  	bound_init(nsc_pos_dou); bound_init(nsc_pos_dou_1); bound_init(nsc_pos_dou_2);

	if(positiveK == true)  k2 = 2*k;     /* Vk */
	else 				   k2 = (2*k)^1; 	 /* -Vk */
    k4 = 2 * dim + k2;  /* |Vk|*/

    //////////////////////
    bound_add(temp,m[avo_matpos2(ii,k2)],m[avo_matpos2(ii,k4^1)]);   /* (Vk-Vi) + (|Vk|-Vi)*/
    bound_min(nsc_temp,nsc[avo_matpos2(ii,k2)],nsc[avo_matpos2(ii,k4^1)]);
    bound_set(pos_sin_1,m[avo_matpos2(ii,ii^1)]);
    bound_set(nsc_pos_sin_1,nsc[avo_matpos2(ii,ii^1)]);
    bound_bmin_nsc(&pos_sin_1,&nsc_pos_sin_1,temp,nsc_temp);

	bound_add(temp,m[avo_matpos2(k2^1,jj)],m[avo_matpos2(k4,jj)]);  /* (Vjj+Vk) + (Vjj-|Vk|)  or (Vjj-Vk) + (Vjj+|Vk|)*/
	bound_min(nsc_temp,nsc[avo_matpos2(k2^1,jj)],nsc[avo_matpos2(k4,jj)]);
    bound_set(pos_sin_2,m[avo_matpos2(jj^1,jj)]);
    bound_set(nsc_pos_sin_2,nsc[avo_matpos2(jj^1,jj)]);
    bound_bmin_nsc(&pos_sin_2,&nsc_pos_sin_2,temp,nsc_temp);

    bound_add(pos_sin,pos_sin_1,pos_sin_2); /* 2*(Vj-Vi)*/
    bound_min(nsc_pos_sin,nsc_pos_sin_1,nsc_pos_sin_2);
    bound_div_2(pos_sin,pos_sin);

    //////////////////////
    bound_set(temp,m[avo_matpos2(ii,k2^1)]);
    bound_set(nsc_temp,nsc[avo_matpos2(ii,k2^1)]);
    bound_bmin_nsc(&temp,&nsc_temp,m[avo_matpos2(ii,k4^1)],nsc[avo_matpos2(ii,k4^1)]);
    bound_set_infty(pos_dou_1,1); bound_set_infty(nsc_pos_dou_1,1);
    	/* (Vk-Vii)+(Vjj-Vk)  or  (|Vk|-Vii)+((Vjj-|Vk|)) */
	bound_add_nsc(&pos_dou_1,&nsc_pos_dou_1,temp,m[avo_matpos2(k2^1,jj)],nsc_temp,nsc[avo_matpos2(k2^1,jj)]);

    bound_set(temp,m[avo_matpos2(k2,jj)]);
    bound_set(nsc_temp,nsc[avo_matpos2(k2,jj)]);
    bound_bmin_nsc(&temp,&nsc_temp,m[avo_matpos2(k4,jj)],nsc[avo_matpos2(k4,jj)]);
    bound_set_infty(pos_dou_2,1); bound_set_infty(nsc_pos_dou_2,1);
	bound_add_nsc(&pos_dou_2,&nsc_pos_dou_2,temp,m[avo_matpos2(ii,k2)],nsc_temp,nsc[avo_matpos2(ii,k2)]);

    bound_set(pos_dou,pos_dou_1);
    bound_set(nsc_pos_dou,nsc_pos_dou_1);
	bound_bmin_nsc(&pos_dou,&nsc_pos_dou,pos_dou_2,nsc_pos_dou_2);

    //////////////////////
    bound_set(*pos,pos_dou);
    bound_set(*nsc_pos,nsc_pos_dou);
	bound_bmin_nsc(pos,nsc_pos,pos_sin,nsc_pos_sin);

  	bound_clear(temp); bound_clear(pos_sin); bound_clear(pos_sin_1); bound_clear(pos_sin_2);
  	bound_clear(pos_dou); bound_clear(pos_dou_1); bound_clear(pos_dou_2);
  	bound_clear(nsc_temp); bound_clear(nsc_pos_sin); bound_clear(nsc_pos_sin_1); bound_clear(nsc_pos_sin_2);
  	bound_clear(nsc_pos_dou); bound_clear(nsc_pos_dou_1); bound_clear(nsc_pos_dou_2);
}



/* =================================================================== */
/* Conversions between avo and oct  */
/* =================================================================== */

/* convert oct: octnd, octnd_nsc
 * to      avo: avond, avond_nsc
 * */
void oct_to_avo_nsc_nvars(int time,bound_t* octnd,bound_t* octnd_nsc, bound_t* avond, bound_t* avond_nsc, size_t dim)
{
	bound_t* temp;
    size_t i , j ;
    if(time == 1)
    {
        for (i = 0 ; i < 4 * dim ; i++)
        for( j = 0 ; j <= (i|1) ; j++)
        {
           if( i < 2 * dim && j < 2 * dim){ /* the oct part without abs() */
        	   bound_set(avond[avo_matpos2(i,j)],octnd[avo_matpos2(i,j)]);
        	   bound_set(avond_nsc[avo_matpos2(i,j)],octnd_nsc[avo_matpos2(i,j)]);
           }
           else{  /* the av part with abs() */
               bound_set_infty(avond[avo_matpos2(i,j)],1);
			   bound_set_infty(avond_nsc[avo_matpos2(i,j)],1);
           }
        }
        tighten_nsc(avond,avond_nsc,dim); /* use oct part to tighten av part */
    }
    else
    {
        bound_t *temp=malloc(sizeof(bound_t) * avo_matsize(dim));
        bound_t *temp_nsc=malloc(sizeof(bound_t) * avo_matsize(dim));
        bound_init_array(temp,avo_matsize(dim));
        bound_init_array(temp_nsc,avo_matsize(dim));

    	for (i = 0 ; i < 4 * dim ; i++)
    	for( j = 0 ; j <= (i|1) ; j++)
		{
		   if( i < 2 * dim && j < 2 * dim){
			   bound_set(temp[avo_matpos2(i,j)],octnd[avo_matpos2(i,j)]);
			   bound_set(temp_nsc[avo_matpos2(i,j)],octnd_nsc[avo_matpos2(i,j)]);
		   }
		   else{
			   bound_set_infty(temp[avo_matpos2(i,j)],1);
			   bound_set_infty(temp_nsc[avo_matpos2(i,j)],1);
		   }
		}

    	tighten_nsc(temp,temp_nsc,dim);

        for( i = 0 ; i < 4 * dim ; i++)
	    for( j =0 ; j <= (i|1) ; j++){
			bound_bmax_nsc(avond+avo_matpos2(i,j),avond_nsc+avo_matpos2(i,j),temp[avo_matpos2(i,j)],temp_nsc[avo_matpos2(i,j)]);
	    }

        bound_clear_array(temp,avo_matsize(dim));
        bound_clear_array(temp_nsc,avo_matsize(dim));
        free(temp); free(temp_nsc);
    }
}


/* convert avo: m, nsc of size dim
 ** to     oct: octnd, octnd_nsc  of size n (n vars)
 ** in orthant defined via flag (0: negative; 1: positive)
 ** over n vars defined in hash
 ** */
void avo_to_oct_nsc_nvars(bound_t* octnd,bound_t* octnd_nsc,bound_t* m,bound_t* nsc,size_t dim, size_t * hash, size_t * flag, size_t n)
{
    size_t i,j;
    size_t *t_i, *t_j, *g_i, *g_j;
    bound_t b_zero;
    bound_init(b_zero);    bound_set_int(b_zero,0);

    g_i = malloc(sizeof(size_t) * 2 * n);
    g_j = malloc(sizeof(size_t) * 2 * n);
    for(i = 0 ; i < 2 * n ; i++)
    	g_i[i] = 2 * hash[i / 2]+ i % 2;
    for(j = 0 ; j < 2 * n ; j++)
    	g_j[j] = 2 * hash[j / 2]+ j % 2;

    t_i = malloc(sizeof(size_t) * 2 * n);
    t_j = malloc(sizeof(size_t) * 2 * n);
    for(i = 0 ; i < 2 * n ; i++)
    	if(flag[i/2]) t_i[i] = 2 * dim +2 * hash[i/2] + i % 2;
    	else          t_i[i] = 2 * dim +2 * hash[i/2] + 1 - i % 2;
    for(j = 0 ; j < 2 * n ; j++)
    	if(flag[j/2]) t_j[j] = 2 * dim +2 * hash[j/2] + j % 2;
    	else          t_j[j] = 2 * dim +2 * hash[j/2] + 1 - j % 2;

    for(i = 0 ; i < 2 * n ; i++)
    for(j = 0 ; j <= (i|1) ; j++){
    	bound_set_infty(octnd[avo_matpos2(i,j)],1);
    	bound_set_infty(octnd_nsc[avo_matpos2(i,j)],1);
    }

    for(i = 0 ; i < 2*n ; i++)
    for(j = 0 ; j <= (i|1) ; j++)
    {
        bound_bmin_nsc(octnd+avo_matpos2(i,j),octnd_nsc+avo_matpos2(i,j),m[avo_matpos2(g_i[i],g_j[j])],nsc[avo_matpos2(g_i[i],g_j[j])]);
       	bound_bmin_nsc(octnd+avo_matpos2(i,j),octnd_nsc+avo_matpos2(i,j),m[avo_matpos2(t_i[i],g_j[j])],nsc[avo_matpos2(t_i[i],g_j[j])]);
       	bound_bmin_nsc(octnd+avo_matpos2(i,j),octnd_nsc+avo_matpos2(i,j),m[avo_matpos2(g_i[i],t_j[j])],nsc[avo_matpos2(g_i[i],t_j[j])]);
       	bound_bmin_nsc(octnd+avo_matpos2(i,j),octnd_nsc+avo_matpos2(i,j),m[avo_matpos2(t_i[i],t_j[j])],nsc[avo_matpos2(t_i[i],t_j[j])]);
    }

   for(i = 0 ; i < n ; i++){
	   if(flag[i]){
		   if(bound_cmp(b_zero,octnd[avo_matpos2(2*i,(2*i)^1)])<0){
			   bound_set(octnd[avo_matpos2(2*i,(2*i)^1)],b_zero);
			   bound_set_infty(octnd_nsc[avo_matpos2(2*i,(2*i)^1)],1);
		   }
	   }
	   else {
		   if(bound_cmp(b_zero,octnd[avo_matpos2((2*i)^1,2*i)])<0){
			   bound_set(octnd[avo_matpos2((2*i)^1,2*i)],b_zero);
			   bound_set_infty(octnd_nsc[avo_matpos2((2*i)^1,2*i)],1);
		   }
	   }
   }

   free(t_i); free(t_j); free(g_i); free(g_j);
   bound_clear(b_zero);
}

/* ============================================================ */
/* Closures */
/* ============================================================ */


/* unary constraint propagation */
/*
 * return
 *  true: empty avo
 *  false: non-empty avo
 * */
bool oct_hmat_s_step(bound_t* m, bound_t* nsc, size_t dim)
{
  size_t i,j,k;
  bound_t *c, *c1, ik,ij;
  bound_t *c_nsc,ik_nsc,ij_nsc;

  bound_init(ik); bound_init(ij);
  bound_init(ik_nsc); bound_init(ij_nsc);

  /* lone S step */
	/* M_{ij} := min(M_{ij} , (M_{i\bar{i}}+M_{\bar{j}j)/2) */
  c = m;
  c1 = nsc;
  for (i=0;i<2*dim;i++) {
    bound_div_2(ij,m[avo_matpos2(i,i^1)]);
    bound_set(ij_nsc,nsc[avo_matpos2(i,i^1)]);
    for (j=0;j<=(i|1);j++,c++,c1++) {
      bound_div_2(ik,m[avo_matpos2(j^1,j)]);
      bound_set(ik_nsc,nsc[avo_matpos2(j^1,j)]);
      bound_badd(ik,ij);
      bound_bmin_nsc(c,c1,ik,ik_nsc);
    }
  }

  bound_clear(ik); bound_clear(ij);
  bound_clear(ik_nsc); bound_clear(ij_nsc);

  /* emptyness checking */
  for (i=0;i<2*dim;i++) {
    c = m+avo_matpos2(i,i);
    if (bound_sgn(*c)<0) return true;
    else if (bound_sgn(*c)==0) { /* <0 */
    	c1 = nsc+avo_matpos2(i,i);
    	if (bound_sgn(*c1)<0) return true;
    }
    bound_set_int(*c,0);
  }

  return false;
}

/* unary constraint propagation
 * return
 *   true: empty avo
 *   false: non-empty avo
 * note
 *   normally tighten_nsc*() is needed before avo_hmat_s_step()
 * */
bool avo_hmat_s_step(bound_t* m, bound_t* nsc, size_t dim)
{
  size_t i,j,k;
  bound_t *c, *c1;
  bound_t ij,ij2,ii,ii2,jj2;
  bound_t ij_nsc,ij2_nsc,ii_nsc,ii2_nsc,jj2_nsc;

  bound_init(ij); bound_init(ij2); bound_init(ii);  bound_init(ii2); bound_init(jj2);
  bound_init(ij_nsc); bound_init(ij2_nsc); bound_init(ii_nsc); bound_init(ii2_nsc); bound_init(jj2_nsc);

  // lone S step
	// M_{ij} := min(M_{ij} , (M_{i\bar{i}}+M_{\bar{j}j)/2) when i!=\bar{j} and j!=\bar{i}
  for (i=0;i<4*dim;i++) {
    bound_div_2(ii,m[avo_matpos2(i,i^1)]);
    bound_set(ii_nsc,nsc[avo_matpos2(i,i^1)]);
    for (j=0;j<=(i|1);j++) {
      if(j==(i^1) || i==(j^1)) continue;
      bound_div_2(jj2,m[avo_matpos2(j^1,j)]);
      bound_set(jj2_nsc,nsc[avo_matpos2(j^1,j)]);
      bound_add(ij,ii,jj2);
      bound_min(ij_nsc,ii_nsc,jj2_nsc);
      bound_bmin_nsc(m+avo_matpos2(i,j),nsc+avo_matpos2(i,j),ij,ij_nsc);
    }
  }

  /* M_{i\bar{i}} := min(M_{i\bar{i}} , 2*(M_{ij}+M_{j\bar{j}/2)) when j!=i and j!=\bar{i}
   * M_{i\bar{i}} := min(M_{i\bar{i}} , M_{ij}+M_{i\bar{j}) when j!=i and j!=\bar{i}
   * M_{i\bar{i}} := min(M_{i\bar{i}} , M_{j\bar{i}+M_{\bar{j}\bar{i}) when j!=i and j!=\bar{i}
   * */
  for (i=0;i<4*dim;i++) {
    bound_set(ii,m[avo_matpos2(i,i^1)]);
    bound_set(ii_nsc,nsc[avo_matpos2(i,i^1)]);
    for (j=0;j<4*dim;j++) {
      if(j==i || j==(i^1)) continue;

      // M_{i\bar{i}} := min(M_{i\bar{i}} ,  2*(M_{ij}+M_{j\bar{j}/2)) when j!=i and j!=\bar{i}
      bound_set(ij,m[avo_matpos2(i,j)]);
      bound_set(ij_nsc,nsc[avo_matpos2(i,j)]);
      bound_div_2(jj2,m[avo_matpos2(j,j^1)]);
      bound_set(jj2_nsc,nsc[avo_matpos2(j,j^1)]);
      bound_add(ii2, ij, jj2);
	  bound_mul_2(ii2, ii2);
	  bound_min(ii2_nsc,ij_nsc,jj2_nsc);
	  bound_bmin_nsc(m+avo_matpos2(i,i^1),nsc+avo_matpos2(i,i^1),ii2,ii2_nsc);

      // M_{i\bar{i}} := min(M_{i\bar{i}} , M_{ij}+M_{i\bar{j}) when j!=i and j!=\bar{i}
	  bound_set(ij2,m[avo_matpos2(i,j^1)]);
	  bound_set(ij2_nsc,nsc[avo_matpos2(i,j^1)]);
	  bound_add(ii2, ij, ij2);
	  bound_min(ii2_nsc,ij_nsc,ij2_nsc);
	  bound_bmin_nsc(m+avo_matpos2(i,i^1),nsc+avo_matpos2(i,i^1),ii2,ii2_nsc);

      // M_{i\bar{i}} := min(M_{i\bar{i}} , M_{j\bar{i}+M_{\bar{j}\bar{i}) when j!=i and j!=\bar{i}
	  bound_set(ij,m[avo_matpos2(j,i^1)]);
	  bound_set(ij_nsc,nsc[avo_matpos2(j,i^1)]);
	  bound_set(ij2,m[avo_matpos2(j^1,i^1)]);
	  bound_set(ij2_nsc,nsc[avo_matpos2(j^1,i^1)]);
	  bound_add(ii2, ij, ij2);
	  bound_min(ii2_nsc,ij_nsc,ij2_nsc);
	  bound_bmin_nsc(m+avo_matpos2(i,i^1),nsc+avo_matpos2(i,i^1),ii2,ii2_nsc);
    }
  }

  // emptiness checking
  for (i=0;i<4*dim;i++) {
	 c = m+avo_matpos2(i,i);
	 c1 = nsc+avo_matpos2(i,i);
	 if (bound_sgn(*c)<0 || (bound_sgn(*c)==0 && bound_sgn(*c1)<0 ) ){
		bound_clear(ij); bound_clear(ij2); bound_clear(ii); bound_clear(ii2); bound_clear(jj2);
		bound_clear(ii_nsc); bound_clear(ij2_nsc);  bound_clear(ij_nsc); bound_clear(ii2_nsc); bound_clear(jj2_nsc);
		return true;
	 }
	 bound_set_int(*c,0);
  }

  tighten_nsc(m,nsc,dim);

  for (i=0;i<4*dim;i++)
  for (j=0;j<=(i|1);j++)
  for (k=0;k<4*dim;k++) {
		bound_add(ij,m[avo_matpos2(i,k)],m[avo_matpos2(k,j)]);
		bound_min(ij_nsc,nsc[avo_matpos2(i,k)],nsc[avo_matpos2(k,j)]);
		bound_bmin_nsc(m+avo_matpos2(i,j), nsc+avo_matpos2(i,j), ij, ij_nsc);
  }

  bound_clear(ij); bound_clear(ij2); bound_clear(ii); bound_clear(ii2); bound_clear(jj2);
  bound_clear(ii_nsc); bound_clear(ij2_nsc);  bound_clear(ij_nsc); bound_clear(ii2_nsc); bound_clear(jj2_nsc);

  // emptiness checking again
  for (i=0;i<4*dim;i++) {
	 c = m+avo_matpos2(i,i);
	 c1 = nsc+avo_matpos2(i,i);
	 if (bound_sgn(*c)<0 || (bound_sgn(*c)==0 && bound_sgn(*c1)<0 ) ){
	    return true;
	 }
	 bound_set_int(*c,0);
  }
  return false;
}

/* closure over the octagonal part, i.e., M[0:2dim][0:2dim] */
/* making use of +Vk and -Vk (without +|Vk| and -|Vk|)      */
bool avo_hmat_close_oct_part(bound_t* m, bound_t* nsc, size_t dim)
{
	size_t i, j, k, k2;
	bound_t *c, *c1;

	  // Floyd-Warshall without orthant partitioning
	for (k=0;k<2*dim;k++) {
	    k2 = k^1;
	    c = m;
	    c1 = nsc;
	    for (i=0;i<2*dim;i++) {
	      size_t i2 = i|1;
	      size_t br = k<i2 ? k : i2;
	      for (j=0;j<=br;j++,c++,c1++) {
	    	bound_add_nsc(c,c1,m[avo_matpos2(i,k)],m[avo_matpos(k,j)],nsc[avo_matpos2(i,k)],nsc[avo_matpos(k,j)]);  /* ik+kj */
	    	bound_add_nsc(c,c1,m[avo_matpos2(i,k2)],m[avo_matpos(k2,j)],nsc[avo_matpos2(i,k2)],nsc[avo_matpos(k2,j)]);  /* ik2+k2j */
	      }
	      for (;j<=i2;j++,c++,c1++) {
	    	bound_add_nsc(c,c1,m[avo_matpos2(i,k)],m[avo_matpos(j^1,k2)],nsc[avo_matpos2(i,k)],nsc[avo_matpos(j^1,k2)]);  /* ik+kj */
	    	bound_add_nsc(c,c1,m[avo_matpos2(i,k2)],m[avo_matpos(j^1,k)],nsc[avo_matpos2(i,k2)],nsc[avo_matpos(j^1,k)]);  /* ik2+k2j */
	      }
	    }
	}
	return oct_hmat_s_step(m,nsc,dim);
}

/* tighten avo(m,nsc) via orthant-partitioning 2 vars (x,y)
 * return
 *  true: empty avo
 *  false: non-empty avo
 * */
bool tighten_nsc_via_nvars(bound_t* m, bound_t* nsc, size_t dim, size_t *hash, size_t n)
{
	size_t i, j, *ha_i, *ha_j, *flag;
	int ti_time = 1;
    bound_t *octnd= malloc(sizeof(bound_t) * avo_matsize(n));
    bound_t *octnd_nsc= malloc(sizeof(bound_t) * avo_matsize(n));
    bound_t *avond= malloc(sizeof(bound_t) * avo_matsize(n));
    bound_t *avond_nsc= malloc(sizeof(bound_t) * avo_matsize(n));
	bound_init_array(octnd,avo_matsize(n));
	bound_init_array(octnd_nsc,avo_matsize(n));
	bound_init_array(avond,avo_matsize(n));
	bound_init_array(avond_nsc,avo_matsize(n));

	flag = malloc(sizeof(size_t) * n);
    for(int r = 0 ; r < (int)power(n) ;r ++)  // 2^{dim} orthants
	{
    	// for each orthant of n dimension
		orthant_encoding_nd(flag,r,n);   // flag[i]==0 denotes negative orthant
		avo_to_oct_nsc_nvars(octnd,octnd_nsc,m,nsc,dim,hash,flag,n);  // deriving octnd, for each orthant encoded by flag
 		if(avo_hmat_close_oct_part(octnd,octnd_nsc,n)==false)  // check emptiness of octnd
		{
		   oct_to_avo_nsc_nvars(ti_time,octnd,octnd_nsc,avond,avond_nsc,n); // constructing avond from octnd
		   ti_time++;
		}
 	}
	if(ti_time > 1) { // feasible
		ha_i = malloc(sizeof(size_t) * 4 * n);
		ha_j = malloc(sizeof(size_t) * 4 * n);
		for(i = 0 ; i < 2*n ;i++)
			ha_i[i] = hash[i/2] * 2 + i % 2;
		for(j = 0 ; j < 2*n ;j++)
			ha_j[j] = hash[j/2] * 2 + j % 2;
		for(i = 2*n ; i < 4*n ;i++)
			ha_i[i] = hash[(i-2*n)/2] * 2 + 2 * dim + i % 2;
		for(j = 2*n ; j < 4*n ;j++)
			ha_j[j] = hash[(j-2*n)/2] * 2 + 2 * dim + j % 2;
		for(i = 0 ; i < 4*n ;i++)
		for(j = 0 ; j <= (i|1) ;j++)
		{    //using avond to tighten avo(m,nsc)
			bound_bmin_nsc(m+avo_matpos2(ha_i[i],ha_j[j]),nsc+avo_matpos2(ha_i[i],ha_j[j]),avond[avo_matpos2(i,j)],avond_nsc[avo_matpos2(i,j)]);
		}
		free(ha_i);  free(ha_j);
	}
 	bound_clear_array(octnd,avo_matsize(n));
	bound_clear_array(octnd_nsc,avo_matsize(n));
	bound_clear_array(avond,avo_matsize(n));
	bound_clear_array(avond_nsc,avo_matsize(n));
	free(octnd); free(octnd_nsc); free(avond); free(avond_nsc);
	free(flag);
	return false;
}

bool avo_hmat_close_weak_1sign(bound_t* m, bound_t* nsc, size_t dim)
{
	size_t i, j, k;
	bound_t pos,neg,temp;
	bound_t nsc_pos,nsc_neg,nsc_temp;

    if(dim <= 2){
    	size_t hash[3];
		for(i=0;i<dim;i++) hash[i] = i;
		bool res=tighten_nsc_via_nvars(m,nsc,dim,hash,dim);
		if(res==true) return true; //infeasiable
		else          return avo_hmat_s_step(m,nsc,dim);
    }

  	tighten_nsc(m,nsc,dim);
	  // closure over the octagonal part, i.e., M[0:2dim][0:2dim]
	  // making use of +Vk and -Vk (without +|Vk| and -|Vk|)
	if(avo_hmat_close_oct_part(m, nsc, dim)==true)
		return true; // emptiness

 	bound_init(temp);	   bound_init(pos);     bound_init(neg);
 	bound_init(nsc_temp);  bound_init(nsc_pos); bound_init(nsc_neg);

	  //TightenIJviaK()
	for(i = 0 ; i < 4 * dim ; i++)
	{
	  if(i >= 2*dim && i % 2 == 1)
		    continue;  // no need to tighten redundant constraints ( *+|Vi|<= c )

	  for(j = 0 ; j<=(i|1); j++)
      {
		if(j >= 2*dim && j % 2 == 0)
			   continue; // no need to tighten redundant constraints ( |Vj|-*<= c )

		for(k = 0 ; k < dim ; k++)
		{
 		    if((  bound_cmp_int(m[avo_matpos2((2*k)^1, 2*k)],0)<= 0
		        ||bound_cmp_int(m[avo_matpos2(2*k, (2*k)^1)],0)<= 0))
		    {  // Vk<=0 \/ -Vk<=0: use only Vk and -Vk
			   tighten_nsc_IJviaK_fixedSignK(m,nsc,dim,i,j,k);
			   continue;
		    }
		    tighten_nsc_IJviaK_signK(&pos, &nsc_pos, m , nsc, dim, i, j, k, true);
		    tighten_nsc_IJviaK_signK(&neg, &nsc_neg, m , nsc, dim, i, j, k, false);
			bound_set(temp,pos);
			bound_set(nsc_temp,nsc_pos);
			bound_bmax_nsc(&temp,&nsc_temp,neg,nsc_neg);

			bound_bmin_nsc(m+avo_matpos2(i,j),nsc+avo_matpos2(i,j),temp,nsc_temp);
		}
	  }
	}

    for( i = 0 ; i < 4 * dim ; i++)
      for( j = 0 ; j <=(i|1) ; j++){
	    bound_add(temp,m[avo_matpos2(i,i^1)],m[avo_matpos2(j^1,j)]);
	    bound_min(nsc_temp,nsc[avo_matpos2(i,i^1)],nsc[avo_matpos2(j^1,j)]);
	    bound_div_2(temp,temp);
	    bound_bmin_nsc(m+avo_matpos2(i,j),nsc+avo_matpos2(i,j),temp,nsc_temp);
    }

	tighten_nsc(m,nsc,dim);

        for (i=0;i<4*dim;i++)
          for (j=0;j<=(i|1);j++)
            for (k=0;k<4*dim;k++) {
		bound_add(temp,m[avo_matpos2(i,k)],m[avo_matpos2(k,j)]);
		bound_min(nsc_temp,nsc[avo_matpos2(i,k)],nsc[avo_matpos2(k,j)]);
		bound_bmin_nsc(m+avo_matpos2(i,j),nsc+avo_matpos2(i,j),temp,nsc_temp);
	}

	for (i=0;i<4*dim;i++) {
	    bound_t *c = m+avo_matpos2(i,i);
	    if (bound_sgn(*c)<0) return true;
	    bound_set_int(*c,0);
	}

	bound_clear(temp); bound_clear(pos); bound_clear(neg);
	bound_clear(nsc_temp); bound_clear(nsc_pos); bound_clear(nsc_neg);
	return avo_hmat_s_step(m,nsc,dim);;
}

/*
 * return
 *  true: empty avo
 *  false: non-empty avo
 * */
bool avo_hmat_close_weak_3signs(bound_t* m, bound_t* nsc,size_t dim)
{
   size_t i , j , k;
   size_t hash[3];

   if(dim <= 2){
	   for(i=0;i<dim;i++) hash[i] = i;
		bool res=tighten_nsc_via_nvars(m,nsc,dim,hash,dim);
		if(res==true) return true; // infeasiable
   }
   else{
     for(k = 0 ; k < dim ; k++)
       for(i = 0 ; i < dim ; i++)
         for(j = 0 ; j < dim ; j++)
	   {
			if(i==j || i==k || j==k) continue;
			hash[0] = i;	hash[1] = j;	hash[2] = k;
			bool res=tighten_nsc_via_nvars(m,nsc,dim,hash,3);
			if(res==true) return true; // infeasiable
	   }
   }
   return avo_hmat_s_step(m,nsc,dim);
}

/* tighten avo(m,nsc) via orthant-partitioning dim variables */
bool avo_hmat_close_strong_all_signs(bound_t* m, bound_t* nsc,size_t dim)
{
	size_t i , j;
	int ti_time = 1;

    if(dim <= 2){
    	size_t hash[3];
		for(i=0;i<dim;i++) hash[i] = i;
		bool res=tighten_nsc_via_nvars(m,nsc,dim,hash,dim);
		if(res==true) return true; // infeasiable
		else 		  return avo_hmat_s_step(m,nsc,dim);
    }

    bound_t *octnd= malloc(sizeof(bound_t) * avo_matsize(dim));
    bound_t *octnd_nsc= malloc(sizeof(bound_t) * avo_matsize(dim));
    bound_t *avond= malloc(sizeof(bound_t) * avo_matsize(dim));
    bound_t *avond_nsc= malloc(sizeof(bound_t) * avo_matsize(dim));
    bound_init_array(octnd,avo_matsize(dim));
    bound_init_array(avond,avo_matsize(dim));
    bound_init_array(octnd_nsc,avo_matsize(dim));
    bound_init_array(avond_nsc,avo_matsize(dim));

	size_t *flag = malloc(sizeof(size_t) * dim);
	size_t *hash = malloc(sizeof(size_t) * dim);
	for(i = 0 ; i < dim; i++)		hash[i] = i;

    for(int r = 0 ; r < (int)power(dim) ;r ++)  // 2^{dim} orthants
	{
    	// for each orthant of n dimension
		orthant_encoding_nd(flag,r,dim);   // flag[i]==0 denotes negative orthant
		avo_to_oct_nsc_nvars(octnd,octnd_nsc,m,nsc,dim,hash,flag,dim);  // deriving octnd, for each orthant encoded by flag
		if(avo_hmat_close_oct_part(octnd,octnd_nsc,dim)==false)  // check emptiness of octnd
		{
		   oct_to_avo_nsc_nvars(ti_time,octnd,octnd_nsc,avond,avond_nsc,dim); // constructing avond from octnd
		   ti_time++;
		}
	}

    if(ti_time>1){
      for(i = 0 ; i < 4 * dim ;i++)
		for(j = 0 ; j <= (i|1) ;j++){
			 // using avond to tighten avo(m,nsc)
			bound_bmin_nsc(m+avo_matpos2(i,j),nsc+avo_matpos2(i,j),avond[avo_matpos2(i,j)],avond_nsc[avo_matpos2(i,j)]);
		}
    }

    bound_clear_array(octnd,avo_matsize(dim));
    bound_clear_array(avond,avo_matsize(dim));
    bound_clear_array(octnd_nsc,avo_matsize(dim));
    bound_clear_array(avond_nsc,avo_matsize(dim));
    free(octnd);      free(avond);    free(octnd_nsc);  free(avond_nsc);
    free(flag);       free(hash);
    if(ti_time==1) return true;
    else return avo_hmat_s_step(m,nsc,dim);
}

/*
 * return
 *  true: empty avo
 *  false: non-empty avo
 * */
bool avo_hmat_close(bound_t* m,bound_t* nsc, size_t dim)
{
    if (avo_closure_alg == 2)
    	return avo_hmat_close_strong_all_signs(m,nsc, dim); // enumerating the signs of all variables each time
    else if (avo_closure_alg == 1) //by default
		return avo_hmat_close_weak_3signs(m,nsc, dim);      // enumerating the sign of 3 variables each time
    else
    	return avo_hmat_close_weak_1sign(m,nsc, dim);       // enumerating the signs of 1 variable each time
}

/* ============================================================ */
/* Incremental Closure */
/* ============================================================ */

/*
 * return
 *  true: empty avo
 *  false: non-empty avo
 * */
bool avo_hmat_close_incremental_weak_3signs(bound_t* m, bound_t* nsc,size_t dim, size_t v)
{
   size_t i , j;
   size_t hash[3];

   if(dim <= 2){
		for(i=0;i<dim;i++) hash[i] = i;
		bool res=tighten_nsc_via_nvars(m,nsc,dim,hash,dim);
		if(res==true) return true; // infeasiable
   }
   else{
     for(i = 0 ; i < dim ; i++)
		for(j = i ; j < dim ; j++)
		{
			if(i==j || i==v || j==v) continue;
			hash[0] = i;	hash[1] = j;	hash[2] = v;
			bool res=tighten_nsc_via_nvars(m,nsc,dim,hash,3);
			if(res==true) return true; // infeasible
		}
   }
   return avo_hmat_s_step(m,nsc,dim);
}

bool avo_hmat_close_incremental_weak_1sign(bound_t* m, bound_t* nsc,size_t dim, size_t v)
{
	size_t i, j, k, k2;
	bound_t *c;
	bound_t *c1;
	bound_t pos,neg,temp;
	bound_t nsc_pos,nsc_neg, nsc_temp;
  	bound_init(temp);	bound_init(pos); bound_init(neg); bound_init(nsc_pos); bound_init(nsc_neg);  bound_init(nsc_temp);

	tighten_nsc(m,nsc,dim);

	// incremental Floyd-Warshall : v in end-point position
	for (k=0;k<2*dim;k++) {
	    size_t kk = k^1;
	    size_t ii = (2*v)^1;
	    size_t br = k<ii ? k : ii;
	    for (i=2*v;i<2*v+2;i++) {
	      // v in first end-point position
	      c = m+avo_matpos(i,0);
	      c1 = nsc+avo_matpos(i,0);
	      for (j=0;j<=br;j++,c++,c1++) {
	    	  // ik+kj
	    	bound_add_nsc(c,c1,m[avo_matpos2(i,k)],m[avo_matpos(k,j)],nsc[avo_matpos2(i,k)],nsc[avo_matpos(k,j)]);
			  // ik2+k2j
	    	bound_add_nsc(c,c1,m[avo_matpos2(i,kk)],m[avo_matpos(kk,j)],nsc[avo_matpos2(i,kk)],nsc[avo_matpos(kk,j)]);
	      }
	      for (;j<=ii;j++,c++,c1++) {
	    	  // ik+kj
	    	bound_add_nsc(c,c1,m[avo_matpos2(i,k)],m[avo_matpos(j^1,kk)],nsc[avo_matpos2(i,k)],nsc[avo_matpos(j^1,kk)]);
			  // ik2+k2j
	    	bound_add_nsc(c,c1,m[avo_matpos2(i,kk)],m[avo_matpos(j^1,k)],nsc[avo_matpos2(i,kk)],nsc[avo_matpos(j^1,k)]);
	      }

	      // v in second end-point position
	      for (j=i;j<k;j++) {
			c = m+avo_matpos(j,i);
			c1 = nsc+avo_matpos(j,i);
			  // ik+kj
			bound_add_nsc(c,c1,m[avo_matpos2(k,i)],m[avo_matpos(kk,j^1)],nsc[avo_matpos2(k,i)],nsc[avo_matpos(kk,j^1)]);
			  // ik2+k2j
			bound_add_nsc(c,c1,m[avo_matpos2(kk,i)],m[avo_matpos(k,j^1)],nsc[avo_matpos2(kk,i)],nsc[avo_matpos(k,j^1)]);
	      }
	      for (;j<4*dim;j++) {
			c = m+avo_matpos(j,i);
			c1 = nsc+avo_matpos(j,i);
			  // ik+kj
			bound_add_nsc(c,c1,m[avo_matpos2(k,i)],m[avo_matpos(j,k)],nsc[avo_matpos2(k,i)],nsc[avo_matpos(j,k)]);
			  // ik2+k2j
			bound_add_nsc(c,c1,m[avo_matpos2(kk,i)],m[avo_matpos(j,kk)],nsc[avo_matpos2(kk,i)],nsc[avo_matpos(j,kk)]);
	      }
	   }
	}

	// incremental Floyd-Warshall : v in pivot position
	for (k=2*v;k<2*v+2;k++) {
	    size_t kk = k^1;
	    c = m;
	    c1 = nsc;
	    for (i=0;i<4*dim;i++) {
	      size_t ii = (i|1);
	      size_t br = k<ii ? k : ii;
	      for (j=0;j<=br;j++,c++,c1++) {
	    	    // ik+kj
	    	  bound_add_nsc(c,c1,m[avo_matpos2(i,k)],m[avo_matpos(k,j)],nsc[avo_matpos2(i,k)],nsc[avo_matpos(k,j)]);
	    	    // ik2+k2j
	    	  bound_add_nsc(c,c1,m[avo_matpos2(i,kk)],m[avo_matpos(kk,j)],nsc[avo_matpos2(i,kk)],nsc[avo_matpos(kk,j)]);
	      }
	      for (;j<=ii;j++,c++,c1++) {
	    	   // ik+kj
	    	  bound_add_nsc(c,c1,m[avo_matpos2(i,k)],m[avo_matpos(j^1,kk)],nsc[avo_matpos2(i,k)],nsc[avo_matpos(j^1,kk)]);
	    	   // ik2+k2j
	    	  bound_add_nsc(c,c1,m[avo_matpos2(i,kk)],m[avo_matpos(j^1,k)],nsc[avo_matpos2(i,kk)],nsc[avo_matpos(j^1,k)]);
	      }
	    }
	}

	// considering |v| in pivot position
    k = v;
	  //TightenIJviaK()
	for(i = 0 ; i < 4 * dim ; i++)
	{
	  if(i >= 2*dim && i % 2 == 1)   continue;  // no need to tighten redundant constraints ( *+|Vi|<= c )

	  if(i > 2*dim &&
		( bound_cmp_int(m[avo_matpos2(i-2*dim, (i-2*dim)^1)],0)<= 0
		||bound_cmp_int(m[avo_matpos2((i-2*dim)^1, i-2*dim)],0)<=0 ))
			continue; // Vi<=0 \/ -Vi<=0: no need to tighten when Vi

	  for(j = 0 ; j <= (i|1); j++)
      {
		if(j >= 2*dim && j % 2 == 0)   continue; //no need to tighten redundant constraints ( |Vj|-*<= c )
		if(j > 2*dim &&
		  ( bound_cmp_int(m[avo_matpos2(j-2*dim, (j-2*dim)^1)],0)<= 0
		  ||bound_cmp_int(m[avo_matpos2((j-2*dim)^1, j-2*dim)],0)<= 0))
				continue; // Vj<=0 \/-Vj<=0 : no need to tighten when Vj

		if(   bound_cmp_int(m[avo_matpos2(2*k, (2*k)^1)],0)<= 0
			||bound_cmp_int(m[avo_matpos2((2*k)^1, 2*k)],0)<= 0)
		{  // Vk<=0 \/ -Vk<=0: use only Vk or -Vk
		   tighten_nsc_IJviaK_fixedSignK(m,nsc,dim,i,j,k);
		   continue;
		}

		tighten_nsc_IJviaK_signK(&pos, &nsc_pos, m, nsc, dim, i, j, k, true);
		tighten_nsc_IJviaK_signK(&neg, &nsc_neg, m, nsc, dim, i, j, k, false);
		bound_set(temp,pos);
		bound_set(nsc_temp,nsc_pos);
		bound_bmax_nsc(&temp,&nsc_temp,neg,nsc_neg);
		bound_bmin_nsc(m+avo_matpos2(i,j),nsc+avo_matpos2(i,j),temp,nsc_temp);
	  }
	}

	bound_clear(temp); bound_clear(pos); bound_clear(neg);
	bound_clear(nsc_temp); bound_clear(nsc_pos); bound_clear(nsc_neg);
	return avo_hmat_s_step(m,nsc,dim);
}


/*
 * m must equal to a closed matrix, except for constraints involving
 * variable v
 **/
bool avo_hmat_close_incremental(bound_t* m, bound_t* nsc,size_t dim, size_t v)
{
    if (avo_closure_alg >= 1)
    	return avo_hmat_close_incremental_weak_3signs(m,nsc,dim,v); // the 8 orthants method
    else
    	return avo_hmat_close_incremental_weak_1sign(m,nsc,dim,v); // the 2 orthants method
}

/* ============================================================ */
/* Sanity Check */
/* ============================================================ */

bool avo_hmat_check_closed(bound_t* m, bound_t* nsc, size_t dim)
{
  bool closed = true;
  size_t i,j,k;
  bound_t w,nsc_w;

  bound_init(w); bound_init(nsc_w);

  for (i=0;i<4*dim;i++)
    for (j=0;j<=(i|1);j++)
      for (k=0;k<4*dim;k++) {
		bound_add(w,m[avo_matpos2(i,k)],m[avo_matpos2(k,j)]);
		bound_min(nsc_w,nsc[avo_matpos2(i,k)],nsc[avo_matpos2(k,j)]);
		int order=bound_cmp(m[avo_matpos2(i,j)],w);
		if (order>0 || (order==0 && bound_cmp(nsc[avo_matpos2(i,j)],nsc_w)>0) ){
			closed = false;
		}
      }

  for (i=0;i<4*dim;i++)
    for (j=0;j<=(i|1);j++){
    	int order=bound_cmp(m[avo_matpos2(i,j)],m[avo_matpos2(j^1,i^1)]);
    	if ( order>0 ||
    		(order==0 && bound_cmp(nsc[avo_matpos2(i,j)],nsc[avo_matpos2(j^1,i^1)])>0) ){
            closed = false;
    	}

    }

  for (i=0;i<4*dim;i++)
    for (j=0;j<=(i|1);j++) {
      bound_add(w,m[avo_matpos2(i,i^1)],m[avo_matpos2(j^1,j)]);
      bound_min(nsc_w,nsc[avo_matpos2(i,i^1)],nsc[avo_matpos2(j^1,j)]);
      bound_div_2(w,w);
      int order=bound_cmp(m[avo_matpos2(i,j)],w);
	  if ( order>0 ||
		  (order==0 && bound_cmp(nsc[avo_matpos2(i,j)], nsc_w)>0) ){
		   closed = false;
	  }
    }

  bound_clear(w); bound_clear(nsc_w);

  return closed;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
