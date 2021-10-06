/*
 * bv.c
 *
 * Fixed-size Bit Vectors for Kohler's Rule
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
#include "bv.h"

/*  free bv */
void bv_free(bit_vectors_t *bv)
{
  if(bv!=NULL){
    checked_free(bv->bits);
    checked_free(bv);
  }
  return;
}

/*  copy bv */
bit_vectors_t* bv_copy(bit_vectors_t *bv)
{
  bit_vectors_t *bv1;

  bv1 = (bit_vectors_t *)malloc(sizeof(bit_vectors_t));

  bv1->vsize = bv->vsize;
  bv1->vchars= bv->vchars;
  bv1->nvecs= bv->nvecs;
  bv1->bits = (char *)malloc(bv1->nvecs*bv1->vchars*sizeof(char));
  memcpy(bv1->bits,bv->bits,bv1->nvecs*bv1->vchars*sizeof(char));
  return bv1;
}


/*  bit vector or operation */
char* bv_or(bit_vectors_t *bv, char *v1,char *v2)
{
  char *v12;
  unsigned i,j;

  v12 = (char *)malloc(bv->vchars*sizeof(char));
  memset(v12,0,bv->vchars*sizeof(char));
  for(i=0;i<bv->vchars;i++){
     *(v12+i)=*(v1+i) | *(v2+i);
  }

  return v12;
}

/*  set k-th(from 0-th) vector with bit vector v */
void bv_set(bit_vectors_t *bv, unsigned k,char *v)
{
  char *v12;
  unsigned i,j;
  v12 = bv->bits+k*bv->vchars;
  for(i=0;i<bv->vchars;i++){
     *(v12+i)=*(v+i);
  }
}

/*  subset test v1 \subseteq v2 */
bool bv_subset(bit_vectors_t *bv, char *v1,char *v2)
{
  unsigned i,j;
  char vand;

  for(i=0;i<bv->vchars;i++){
     vand=*(v1+i) & *(v2+i);
     if(vand != *(v1+i))  return false;
  }
  return true;
}

/*  bit vector and operation but return 0 or 1(not zero) */
int bv_andTest(bit_vectors_t *bv, char *v1,char *v2)
{
  char v12;
  unsigned i,j;

  for(i=0;i<bv->vchars;i++){
     v12=*(v1+i) & *(v2+i);
     if(v12!=0) return 1;
  }
  return 0;
}

#define NN 8
#define MM 1<<(NN-1)
void charprint(char c)
{
  int i;
  for (i=0;i<NN ;i++)
  {
     if(i%8==0) printf(" ");
     putchar(((c&MM)==0)?'0':'1');
     c<<=1;
  }
}

/*  bit vector and operation but return 0 or 1(not zero) */
unsigned bv_1bits(bit_vectors_t *bv, char *v)
{
  char v1;
  unsigned i,j,n;

  n=0;
  for(i=0;i<bv->vchars;i++){
     v1=*(v+i);
     if(v1!=0){
       for(j=0;j<NN;j++){
          if((v1&MM)!=0) n++;
          v1<<=1;
       }
     }
  }
  return n;
}

/*  Redundancy removal for the first Ks constriants by Kohler's Rule	*/
/*  parameter bv: bit vectors.                    */
/*  parameter NbCons to store the number of the new linear constraints. */
/*  return the linear constraints after redundancy removal. 		*/
fpp_t *bv_redundancy_removal_firstKs_byKohler(fpp_internal_t* pr,bool destructive, fpp_t *fpp, unsigned k,bit_vectors_t *bv)
{
    char *rrIndex;
    numdbl_t *cons1;
    fpp_t *fpp1;
    unsigned i,j,keepNb;
    char *v1,*v2;
    char *bits1;
    unsigned n;

    if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;
    if(fpp1->flag!=GENERAL_POL) return fpp1;

#if DEBUG_JOIN
printf("before bv_redundancy_removal_firstKs:: \n");
fpp_fdump_withBV(stdout, fpp1,bv);
fflush(stdout);
#endif

    keepNb=fpp1->ncons-k;
    checked_malloc(pr,rrIndex,char,k,return NULL;);
    memset(rrIndex,0,k*sizeof(char));
    for(i=0;i<bv->nvecs-1;i++){
    	 if(rrIndex[i]==true) continue;
    	 v1=bv->bits+i*bv->vchars;
    	 for(j=i+1;j<bv->nvecs;j++){
      		if(rrIndex[j]==true) continue;
      		v2=bv->bits+j*bv->vchars;
      		if( bv_subset(bv, v2, v1)==true ){
      			/* i-th constraint is redundant */
      			rrIndex[i]=true;
      			break;
      		}
      		else if( bv_subset(bv, v1, v2)==true ){
      			/* j-th constraint is redundant */
      			rrIndex[j]=true;
      		}
       }
    	 if(rrIndex[i]==true)  i--;
    }
    checked_malloc(pr,cons1,numdbl_t,fpp1->ncons*(fpp1->dim+1),return NULL;);
    checked_malloc(pr,bits1,char,bv->nvecs*bv->vchars, return NULL;);
    n=0;
    for(i=0;i<bv->nvecs;i++){
    	if(rrIndex[i]==true){

#if DEBUG_JOIN
	   printf(" bv_redundancy_removal_firstKs  %d removed \n",i);
#endif

    	   continue;
    	}
    	   /* i-th constraint is non-redundant */
    	memcpy(cons1+n*(fpp1->dim+1),fpp1->cons+i*(fpp1->dim+1),(fpp1->dim+1)*sizeof(numdbl_t));
    	memcpy(bits1+n*(bv->vchars),bv->bits+i*(bv->vchars),bv->vchars*sizeof(char));
    	n++;
    }

    if(keepNb>0){
	     memcpy(cons1+n*(fpp1->dim+1),fpp1->cons+k*(fpp1->dim+1),keepNb*(fpp1->dim+1)*sizeof(numdbl_t));
    }
    cons1=realloc(cons1,(n+keepNb)*(fpp1->dim+1)*sizeof(numdbl_t));
    checked_free(fpp1->cons);
    fpp1->cons=cons1;
    fpp1->ncons=n+keepNb;
    bits1=realloc(bits1,n*(bv->vchars)*sizeof(char));
    checked_free(bv->bits);
    bv->bits=bits1;
    bv->nvecs=n;

#if DEBUG_JOIN
printf("after bv_redundancy_removal_firstKs:: \n");
fpp_fdump_withBV(stdout, fpp1,bv);
fflush(stdout);
#endif

    checked_free(rrIndex);
    return fpp1;
}

/*  Redundancy removal for the first Ks constriants by Kohler's Rule	*/
/*  parameter bv: bit vectors.                    */
/*  parameter NbCons to store the number of the new linear constraints. */
/*  return the linear constraints after redundancy removal. 		*/
fm_pair_t *bv_redundancy_removal_byKohler_lazy(fpp_internal_t* pr,unsigned k,unsigned zeroI,unsigned nelimv, fm_pair_t *fmp, bit_vectors_t *bv)
{
    char *rrIndex;
    numdbl_t *cons1;
    fm_pair_t *fmp1;
    unsigned i,j,keepNb;
    char *v1,*v2;
    char *bits1;
    unsigned n;

    checked_malloc(pr,rrIndex,char,k, return NULL;);
    memset(rrIndex,0,k*sizeof(char));
    for(i=zeroI;i<bv->nvecs;i++){
    	 v1=bv->bits+i*bv->vchars;
       n=bv_1bits(bv,v1);
    	 if(n>nelimv+1){  /* Kohler's Rule */
    	     for(j=0;j<bv->nvecs;j++){
            		if(rrIndex[j]==true || j==i) continue;
            		v2=bv->bits+j*bv->vchars;
            		if( bv_subset(bv, v2, v1)==true ){
            			/* i-th constraint is redundant */
            			rrIndex[i]=true;
            			break;
            		}
            }
    	 }
    	 else{
    	     for(j=0;j<zeroI;j++){
          		v2=bv->bits+j*bv->vchars;
          		if( bv_subset(bv, v2, v1)==true ){
          			/* i-th constraint is redundant */
          			rrIndex[i]=true;
          			break;
          		}
            }
    	 }
    }
    checked_malloc(pr,fmp1,fm_pair_t,k, return NULL;);
    checked_malloc(pr,bits1,char,bv->nvecs*bv->vchars, return NULL;);

    memcpy(fmp1,fmp,zeroI*sizeof(fm_pair_t));
    memcpy(bits1,bv->bits,zeroI*bv->vchars*sizeof(char));
    n=zeroI;
    for(i=zeroI;i<bv->nvecs;i++){
    	if(rrIndex[i]==true){
#if DEBUG_JOIN
    	   printf(" bv_redundancy_removal_byKohler_lazy  %d removed \n",i);
#endif
    	   continue;
    	}
    	   /* i-th constraint is non-redundant */
    	memcpy(fmp1+n,fmp+i,sizeof(fm_pair_t));
    	memcpy(bits1+n*(bv->vchars),bv->bits+i*(bv->vchars),bv->vchars*sizeof(char));
    	n++;
    }
    bits1=realloc(bits1,n*(bv->vchars)*sizeof(char));
    checked_free(bv->bits);
    bv->bits=bits1;
    bv->nvecs=n;
    fmp1=realloc(fmp1,n*sizeof(fm_pair_t));
    checked_free(rrIndex);
    return fmp1;
}

/*  Redundancy removal for the first Ks constriants by Kohler's Rule	*/
/*  parameter bv: bit vectors.                    */
/*  parameter NbCons to store the number of the new linear constraints. */
/*  return the linear constraints after redundancy removal. 		*/
fm_pair_t *bv_redundancy_removal_byKohler_lazy_bak(fpp_internal_t* pr,unsigned k,unsigned zeroI,unsigned nelimv, fm_pair_t *fmp, bit_vectors_t *bv)
{
    char *rrIndex;
    numdbl_t *cons1;
    fm_pair_t *fmp1;
    unsigned i,j,keepNb;
    char *v1,*v2;
    char *bits1;
    unsigned n;

    checked_malloc(pr,rrIndex,char,k, return NULL;);
    memset(rrIndex,0,k*sizeof(char));
    for(i=0;i<bv->nvecs-1;i++){
    	 if(rrIndex[i]==true) continue;
    	 v1=bv->bits+i*bv->vchars;
    	 for(j=i+1;j<bv->nvecs;j++){
      		if(rrIndex[j]==true) continue;
      		v2=bv->bits+j*bv->vchars;
      		if( bv_subset(bv, v2, v1)==true ){
      			/* i-th constraint is redundant */
      			rrIndex[i]=true;
      			break;
      		}
      		else if( bv_subset(bv, v1, v2)==true ){
      			/* j-th constraint is redundant */
      			rrIndex[j]=true;
      		}
       }
    	 if(rrIndex[i]==true)  i--;
    }
    checked_malloc(pr,fmp1,fm_pair_t,k, return NULL;);
    checked_malloc(pr,bits1,char,bv->nvecs*bv->vchars, return NULL;);
    n=0;
    for(i=0;i<bv->nvecs;i++){
	     if(rrIndex[i]==true){
#if DEBUG_JOIN
	   printf(" bv_redundancy_removal_byKohler_lazy  %d removed \n",i);
#endif
	       continue;
	     }
    	   /* i-th constraint is non-redundant */
     	 memcpy(fmp1+n,fmp+i,sizeof(fm_pair_t));
    	 memcpy(bits1+n*(bv->vchars),bv->bits+i*(bv->vchars),bv->vchars*sizeof(char));
    	 n++;
    }
    bits1=realloc(bits1,n*(bv->vchars)*sizeof(char));
    checked_free(bv->bits);
    bv->bits=bits1;
    bv->nvecs=n;
    fmp1=realloc(fmp1,n*sizeof(fm_pair_t));
    checked_free(rrIndex);
    return fmp1;
}

/* constraints printing... */
void bv_fprint(FILE* stream, bit_vectors_t *bv)
{
   int i,j;
   char *v1;
   for (i=0;i<(int)bv->nvecs;i++) {
      fprintf(stream,"%d:  ", i+1);
         /* Bit Vector */
      fprintf(stream,"(");
      v1=bv->bits+i*bv->vchars;
      for(j=(int)bv->vchars-1;j>=0;j--){
    	  charprint(*(v1+j));
      }
      printf(")\n");
   }
}

/* constraints printing... */
void fpp_constraints_fprint_withBV(FILE* stream, fpp_t* a, bit_vectors_t *bv)
{
  int i,j;
  num_t *p;
  numdbl_t q,qInt;
  char *v1;

  fprintf(stream,"\n");
  for (i=0;i<(int)a->ncons;i++) {
      fprintf(stream,"%d:  ", i+1);
         /* Bit Vector */
      fprintf(stream,"(");
      if(i<(int)bv->nvecs){
         v1=bv->bits+i*bv->vchars;
      	 for(j=(int)bv->vchars-1;j>=0;j--){
      	 	charprint(*(v1+j));
      	 }
      	 printf(") ");
      }
      else fprintf(stream,"           ) ");

      p=(num_t *)(a->cons+i*(a->dim+1));
      double_set_num(&q,*p);
      qInt=trunc(q);
      if(qInt==q ){ /* integer */
	       fprintf(stream,"%ld", (long int)q); /* the first column is for constants */
      }
      else fprintf(stream,FPRTL, q); /* the first column is for constants */
      fprintf(stream," >=");
      for (j=0;j<(int)a->dim;j++) {
        double_set_num(&q,*(p+j+1));
        if((j!=0)&&(q>=0)) fprintf(stream," +");
        else fprintf(stream," ");
        if(q!=0) {
      	   qInt=trunc(q);
      	   if(qInt==q ){ /* integer */
	   	fprintf(stream,"%ld", (long int)q); /* the first column is for constants */
      	   }
           else fprintf(stream,FPRTL, q);
	   fprintf(stream,"*x%d", j);
        }
      }
      fprintf(stream,"\n");
   }
}

/* print the bounds, similar to box_fprint() */
void fpp_bounds_fprint_BV(FILE* stream, fpp_t* a, char** name_of_dim)
{
  size_t i;
  size_t nbdims = a->dim;
  numdbl_t p,pInt;

  fprintf(stream,"\n");
  for(i=0; i<nbdims; i++){
     if (name_of_dim){
	      fprintf(stream,"%8s in ", name_of_dim[i]);
     } else {
	      fprintf(stream,"x%ld in ", (long)i);
     }
     p=*(a->bnds+i*2);
     fprintf(stream,"[");
     if(p==-MAX_VARBND) fprintf(stream,FPRTE,p);
     else{
        pInt=trunc(p);
        if(pInt==p ){ /* integer */
	         fprintf(stream,"%ld", (long int)p); /* the first column is for constants */
        }
        else fprintf(stream,FPRTE,p);
     }
     fprintf(stream," , ");
     p=*(a->bnds+i*2+1);
     if(p==MAX_VARBND) fprintf(stream,FPRTE,p);
     else{
        pInt=trunc(p);
        if(pInt==p ){ /* integer */
	         fprintf(stream,"%ld", (long int)p); /* the first column is for constants */
        }
        else fprintf(stream,FPRTE,p);
     }
     fprintf(stream,"]\n");
  }
}

void fpp_fdump_withBV(FILE* stream, fpp_t* a,bit_vectors_t *bv)
{
  if (a->flag==EMPTY_POL) {
    fprintf(stream,"empty polyhedron of dim (%lu,%lu)\n",
	    (unsigned long)a->intdim,(unsigned long)(a->dim-a->intdim));
  }
  else if (a->flag==UNIVERSE_POL) {
    fprintf(stream,"universe polyhedron of dim (%lu,%lu)\n",
	    (unsigned long)a->intdim,(unsigned long)(a->dim-a->intdim));
  }
  else { /* GENERALPOL */
    fprintf(stream,"polyhedron of dim (%lu,%lu)\n",
	    (unsigned long)a->intdim,(unsigned long)(a->dim-a->intdim));
    fprintf(stream,"Constraints: %lu * %lu\n", (unsigned long)a->ncons, (unsigned long)(a->dim+1));
    fpp_constraints_fprint_withBV(stream,a,bv);
    fprintf(stream,"Bounds: ");
    fpp_bounds_fprint_BV(stream,a,NULL);
  }
}


/*  Redundancy removal for the first Ks constriants by Envelope's Rule	*/
/*  parameter bv: bit vectors.                    */
/*  parameter NbCons to store the number of the new linear constraints. */
/*  return the linear constraints after redundancy removal. 		*/
fpp_t *bv_redundancy_removal_firstKs_byEnvelope(fpp_internal_t* pr,bool destructive, fpp_t *fpp, unsigned k,bit_vectors_t *bv,
                                                char *nenv1bv,char *nenv2bv)
{
    numdbl_t *resCons;
    fpp_t *fpp1;
    unsigned i,j,keepNb;
    char *v1,*v2;
    bool flagi;
    /* keepNb=fpp->ncons-k; */
    if(destructive==false)
		fpp1=fpp_copy_internal(pr, fpp);
    else	fpp1=fpp;
    if(fpp1->flag!=GENERAL_POL) return fpp1;
    for(i=0;i<bv->nvecs;i++){
	     v1=bv->bits+i*bv->vchars;
       if(   bv_andTest(bv,v1,nenv1bv)
           &&  bv_andTest(bv,v1,nenv2bv) ){
	            continue;
    	 }
     	 /* i-th constraint is redundant */
    	 memmove(fpp1->cons+i*(fpp1->dim+1),fpp1->cons+(i+1)*(fpp1->dim+1),(fpp1->ncons-i-1)*(fpp1->dim+1)*sizeof(numdbl_t));
    	 fpp1->ncons=fpp1->ncons-1;
    	 memmove(bv->bits+i*bv->vchars,bv->bits+(i+1)*bv->vchars,(bv->nvecs-i-1)*bv->vchars*sizeof(char));
    	 bv->nvecs=bv->nvecs-1;
    	 i--;
    }
    return fpp1;
}
