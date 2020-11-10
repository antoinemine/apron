/*
 * itvp_example.c
 *
 * Unit testing.
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
#include <time.h>
#include <assert.h>

#include "itvp.h"
#include "itvp_fun.h"
#include "itvp_internal.h"
#include "numitv.h"
#include <fenv.h> /* floating-point lib */
#include "ilp.h"
#include "ifm.h"
#include "itvp_domain.h"

ap_manager_t* mp; /* polyhedron */
itvp_internal_t* pr;

/* a '#' in the first column is a comment line */
numdbl_t *Matrix_Read(unsigned NbRows,  unsigned NbColumns)
{
  numdbl_t *p,*q;
  unsigned i, j, n;
  char *c, s[512];

  q=p= (numdbl_t *)malloc(NbRows * NbColumns * sizeof(numdbl_t));
  for (i=0;i<NbRows;i++)
  { c = fgets(s, 512, stdin);
    while(c && (*c=='*'  || *c=='\n') ) c = fgets(s, 512, stdin);
    if (!c)
    {  fprintf(stderr, "Matrix_Read: ?not enough rows\n");
       break;
    }
    for (j=0;j<NbColumns;j++)
    { if (sscanf(c,FSCF2, p++, &n)==0) /*FSCF2=" %f%n"*/
      {  fprintf(stderr, "Matrix_Read: row(%d) ?not enough columns\n", i);
         break;
      }
      c += n;
    }
  }
  return q;
} /* Matrix_Read */

void ilp_test(void)
{
  unsigned NbRows, NbColumns;
  char *c, s[128];
  numdbl_t *a;
  numdbl_t *bnds_a;
  itvp_t *itvp1;
  numdbl_t res;
  unsigned objType;
  numdbl_t *objVector;

  /* read the rows and columns number */
  c=fgets(s, 128, stdin);
  while ( (*s=='#' || *s=='\n') || (sscanf(s, " %d %d", &NbRows, &NbColumns)<2) )
       c=fgets(s, 128, stdin);
  /* read linear constraints*/
  a = Matrix_Read(NbRows, NbColumns);
  /* read bounds*/
  bnds_a = Matrix_Read((NbColumns-1)/2,2);

  itvp1=(itvp_t *) malloc(sizeof(itvp_t));
  itvp1->dim   = (int) (NbColumns-1)/2;
  itvp1->intdim= 0;
  itvp1->ncons = NbRows;
  itvp1->flag  = GENERAL_POL; 
  itvp1->bnds  = (numdbl_t *) malloc(2* itvp1->dim *sizeof(numdbl_t) );
  memcpy(itvp1->bnds, bnds_a, 2* itvp1->dim *sizeof(numdbl_t));
  itvp1->cons  = (numdbl_t *) malloc(itvp1->ncons* (2*itvp1->dim+1) *sizeof(numdbl_t) );
  memcpy(itvp1->cons, a,itvp1->ncons* (2*itvp1->dim+1) *sizeof(numdbl_t));  
  free(a);
  free(bnds_a);

  itvp_fdump(stdout, mp, itvp1);
  fflush(stdout);

  objVector=(numdbl_t *) malloc(2*itvp1->dim*sizeof(numdbl_t));  
  *(objVector)=-1;
  *(objVector+1)=1;
  *(objVector+2)=-1;
  *(objVector+3)=1;
  objType=GLP_MIN;
  res=ilp_rig(itvp1,objType,objVector);
  printf("\n ilp_test(): res=%f\n",res);
  return;
}

void ifm_test(void)
{
  unsigned NbRows, NbColumns;
  char *c, s[128];
  numdbl_t *a;
  numdbl_t *bnds_a;
  itvp_t *itvp1;
  numdbl_t res;
  unsigned objType;
  numdbl_t *objVector;

  /* read the rows and columns number */
  c=fgets(s, 128, stdin);
  while ( (*s=='#' || *s=='\n') || (sscanf(s, " %d %d", &NbRows, &NbColumns)<2) )
       c=fgets(s, 128, stdin);
  /* read linear constraints*/
  a = Matrix_Read(NbRows, NbColumns);
  /* read bounds*/
  bnds_a = Matrix_Read((NbColumns-1)/2,2);

  itvp1=(itvp_t *) malloc(sizeof(itvp_t));
  itvp1->dim   = (int) (NbColumns-1)/2;
  itvp1->intdim= 0;
  itvp1->ncons = NbRows;
  itvp1->flag  = GENERAL_POL; 
  itvp1->bnds  = (numdbl_t *) malloc(2* itvp1->dim *sizeof(numdbl_t) );
  memcpy(itvp1->bnds, bnds_a, 2* itvp1->dim *sizeof(numdbl_t));
  itvp1->cons  = (numdbl_t *) malloc(itvp1->ncons* (2*itvp1->dim+1) *sizeof(numdbl_t) );
  memcpy(itvp1->cons, a,itvp1->ncons* (2*itvp1->dim+1) *sizeof(numdbl_t));  
  free(a);
  free(bnds_a);

  itvp_fdump(stdout, mp, itvp1);
  fflush(stdout);

  itvp1=ifm_elimination(pr,true, itvp1, 1);
  printf("\n ifm_elimination() result:\n");
  itvp_fdump(stdout, mp, itvp1);
  fflush(stdout);
  return;
}

void join_test(void)
{
  unsigned NbRows, NbColumns;
  char *c, s[128];
  numdbl_t *a;
  numdbl_t *bnds_a;
  itvp_t *itvp1,*itvp2,*itvp12;
  numdbl_t res;
  unsigned objType;
  numdbl_t *objVector;

  /* read the rows and columns number */
  c=fgets(s, 128, stdin);
  while ( (*s=='#' || *s=='\n') || (sscanf(s, " %d %d", &NbRows, &NbColumns)<2) )
       c=fgets(s, 128, stdin);
  /* read linear constraints*/
  a = Matrix_Read(NbRows, NbColumns);
  /* read bounds*/
  bnds_a = Matrix_Read((NbColumns-1)/2,2);

  itvp1=(itvp_t *) malloc(sizeof(itvp_t));
  itvp1->dim   = (int) (NbColumns-1)/2;
  itvp1->intdim= 0;
  itvp1->ncons = NbRows;
  itvp1->flag  = GENERAL_POL; 
  itvp1->bnds  = (numdbl_t *) malloc(2* itvp1->dim *sizeof(numdbl_t) );
  memcpy(itvp1->bnds, bnds_a, 2* itvp1->dim *sizeof(numdbl_t));
  itvp1->cons  = (numdbl_t *) malloc(itvp1->ncons* (2*itvp1->dim+1) *sizeof(numdbl_t) );
  memcpy(itvp1->cons, a,itvp1->ncons* (2*itvp1->dim+1) *sizeof(numdbl_t));  
  free(a);
  free(bnds_a);

  /* read the rows and columns number */
  c=fgets(s, 128, stdin);
  while ( (*s=='#' || *s=='\n') || (sscanf(s, " %d %d", &NbRows, &NbColumns)<2) )
       c=fgets(s, 128, stdin);
  /* read linear constraints*/
  a = Matrix_Read(NbRows, NbColumns);
  /* read bounds*/
  bnds_a = Matrix_Read((NbColumns-1)/2,2);

  itvp2=(itvp_t *) malloc(sizeof(itvp_t));
  itvp2->dim   = (int) (NbColumns-1)/2;
  itvp2->intdim= 0;
  itvp2->ncons = NbRows;
  itvp2->flag  = GENERAL_POL; 
  itvp2->bnds  = (numdbl_t *) malloc(2*itvp2->dim*sizeof(numdbl_t) );
  memcpy(itvp2->bnds, bnds_a, 2* itvp2->dim *sizeof(numdbl_t));
  itvp2->cons  = (numdbl_t *) malloc(itvp2->ncons* (2*itvp2->dim+1) *sizeof(numdbl_t) );
  memcpy(itvp2->cons, a,itvp2->ncons* (2*itvp2->dim+1) *sizeof(numdbl_t));  
  free(a);
  free(bnds_a);

  printf("join:: \n");
  itvp_fdump(stdout, mp, itvp1);
  fflush(stdout);
  itvp_fdump(stdout, mp, itvp2);
  fflush(stdout);

  itvp12=itvpol_weak_join(pr, itvp1, itvp2);
  printf("\n itvpol_weak_join() result:\n");
  itvp_fdump(stdout, mp, itvp12);
  fflush(stdout);
  return;
}

int main(int argc, const char** argv)
{
  int i;

  /* init */
  mp = itvp_manager_alloc();

  for (i=0;i<AP_FUNID_SIZE;i++) {
    mp->option.funopt[i].flag_exact_wanted = true;
    mp->option.funopt[i].flag_best_wanted = true;
  }
  for (i=0;i<AP_EXC_SIZE;i++){
    mp->option.abort_if_exception[i] = true;
  }
  pr = itvp_init_from_manager(mp,0,0);
  /* join_test(); */
  /* dimension_test(); */
  /* widen_test(); */
  /* initial_test();  */
  /* assign_test();  */
  /* join_bv_rational_test(); */
  /*  ilp_test(); */
  /* ifm_test(); */
  join_test();
 ap_manager_free(mp);
  return 0;
}
