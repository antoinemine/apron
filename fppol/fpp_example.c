/*
 * fpp_example.c
 *
 * Unit testing.
 *
 * APRON Library / Fppol Domain
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
#include <time.h>
#include <assert.h>

#include "fpp.h"
#include "fpp_fun.h"
#include "fpp_internal.h"
#include "numitv.h"
#include "fppol_domain.h"
#include "bt.h"
#include "rlp.h"
#include "fpp_user.h"
#include "fpp_weakjoin.h"
#include <fenv.h> /* floating-point lib */

ap_manager_t* mp; /* polyhedron */
fpp_internal_t* pr;


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

/* convert b-Ax>=0 to b>=Ax */
numdbl_t *Matrix_Convert_minusA(numdbl_t *cons,unsigned NbRows, unsigned NbColumns)
{
  int i,j;
  numdbl_t *minusA,a;

  minusA=(numdbl_t *)malloc(NbRows*NbColumns*sizeof(numdbl_t));
  memcpy(minusA,cons,NbRows*NbColumns*sizeof(numdbl_t));
	/* convert b-Ax>=0 to b>=Ax */
  for(i=0;i<(int)NbRows;i++){
      for(j=1;j<(int)NbColumns;j++){
        a=*(minusA+i*NbColumns+j);
		if(a==0.0){
		   *(minusA+i*NbColumns+j)=0.0;
		}
		else{
		   *(minusA+i*NbColumns+j)=-a;
		}
      }
  }
  return minusA;
}

/* Polyhedron H-representation */
fpp_t* fpp_read_H(void)
{
  char *c, s[128];
  numdbl_t *cons,*cons1,*bnds;
  char name[128];
  fpp_t *fpp1;
  unsigned NbRows, NbColumns;

  c = fgets(s, 128, stdin);
  while(c && (*c=='*'  || *c=='\n') ) c = fgets(s, 128, stdin);
  if (!c)
  {
     fprintf(stderr, "fpp_read_H: ? no H-representation\n");
     return NULL;
  }
  if ( strncmp(c,"H-representation",16)!=0 ){
     fprintf(stderr, "Syntax error: missing begin line 'H-representation'");
     return NULL;
  }
  if ( (scanf("%s",name) == EOF)|| (strcmp(name,"begin")!=0) ){
     fprintf(stderr, "Syntax error: missing begin line 'begin'");
     return NULL;
  }
       /* read the rows and columns number */
  c = fgets(s, 128, stdin);
  while ( (*s=='*' || *s=='\n') || (sscanf(s, " %d %d %s", &NbRows, &NbColumns,name)<3) ){
	  c = fgets(s, 128, stdin);
  }
  if( strcmp(name,"integer") != 0 && strcmp(name,"rational") != 0 && strcmp(name,"double") != 0) {
      fprintf(stderr, "\nSyntax error: Data type must be integer or rational or double,not \"%s\"\n", name);
      return NULL;
  }
  	/* read linear constraints */
  cons1 = Matrix_Read(NbRows, NbColumns);
	/* convert b-Ax>=0 to b>=Ax */
  cons=Matrix_Convert_minusA(cons1,NbRows,NbColumns);
  	/* read bounds */
  bnds = Matrix_Read(NbColumns-1,2);

  if ( (scanf("%s",name) == EOF)|| (strcmp(name,"end")!=0) ){
     fprintf(stderr, "Syntax error: missing end line 'end'");
     free(cons);
     free(bnds);
     return NULL;
  }
  fpp1=fpp_alloc_urgent(pr,NbColumns-1,0,NbRows);
  memcpy(fpp1->cons,cons,(int)(fpp1->ncons*(fpp1->dim+1))*sizeof(numdbl_t));
  memcpy(fpp1->bnds,bnds,(int)2*fpp1->dim*sizeof(numdbl_t));


  free(cons1);
  free(cons);
  free(bnds);
  return fpp1;
}/* end of fpp_read_H() */


/* ********************************* */
/*           main                    */
/* ********************************* */

void join_test(void)
{
  unsigned NbRows, NbColumns;
  char *c, s[128];
  numdbl_t *a;
  numdbl_t *bnds_a;
  fpp_t *fpp1,*fpp2,*fpp3;

  /* read the rows and columns number */
  c = fgets(s, 128, stdin);
  while ( (*s=='#' || *s=='\n') || (sscanf(s, " %d %d", &NbRows, &NbColumns)<2) )
	  c = fgets(s, 128, stdin);
  /* read linear constraints*/
  a = Matrix_Read(NbRows, NbColumns);
  /* read bounds*/
  bnds_a = Matrix_Read(NbColumns-1,2);
  fpp1=fpp_alloc_urgent(pr,NbColumns-1,0,NbRows);
  memcpy(fpp1->cons,a,(int)(fpp1->ncons*(fpp1->dim+1))*sizeof(numdbl_t));
  memcpy(fpp1->bnds,bnds_a,(int)2*fpp1->dim*sizeof(numdbl_t));

  free(a);
  free(bnds_a);
  /* read the rows and columns number */
  c = fgets(s, 128, stdin);
  while ( (*s=='#' || *s=='\n') || (sscanf(s, " %d %d", &NbRows, &NbColumns)<2) )
	  c = fgets(s, 128, stdin);
  /* read linear constraints*/
  a = Matrix_Read(NbRows, NbColumns);
  /* read bounds*/
  bnds_a = Matrix_Read(NbColumns-1,2);
  fpp2=fpp_alloc_urgent(pr,NbColumns-1,0,NbRows);
  memcpy(fpp2->cons,a,(int)(fpp2->ncons*(fpp1->dim+1))*sizeof(numdbl_t));
  memcpy(fpp2->bnds,bnds_a,(int)2*fpp2->dim*sizeof(numdbl_t));

  fpp1=bt_byExactLP(pr,true,fpp1, NULL, fpp1->dim);
  fpp2=bt_byExactLP(pr,true,fpp2, NULL, fpp2->dim);

  fprintf(stdout,"\n ====Strong join ====== \n");
  fprintf(stdout,"\n Input P1: \n");
  fpp_fdump(stdout, pr->man, fpp1);
  fprintf(stdout,"\n Input P2: \n");
  fpp_fdump(stdout, pr->man, fpp2);
  fpp3=fpp_join(pr->man,true,fpp1,fpp2);
  fprintf(stdout,"\n Result: \n");
  fpp_fdump(stdout, pr->man, fpp3);
}

void dimension_test(void)
{
  unsigned NbRows, NbColumns;
  char *c, s[128];
  numdbl_t *a;
  numdbl_t *bnds_a;
  fpp_t *fpp1,*fpp2;
  ap_dimchange_t* dimchange;
  ap_dimperm_t* dimperm;
  ap_dim_t* dim;
  ap_dim_t* dim2;

  /* read the rows and columns number */
  c = fgets(s, 128, stdin);
  while ( (*s=='#' || *s=='\n') || (sscanf(s, " %d %d", &NbRows, &NbColumns)<2) )
	  c = fgets(s, 128, stdin);
  /* read linear constraints*/
  a = Matrix_Read(NbRows, NbColumns);
  /* read bounds*/
  bnds_a = Matrix_Read(NbColumns-1,2);
  fpp1=fpp_alloc_urgent(pr,NbColumns-1,0,NbRows);
  memcpy(fpp1->cons,a,(int)(fpp1->ncons*(fpp1->dim+1))*sizeof(numdbl_t));
  memcpy(fpp1->bnds,bnds_a,(int)2*fpp1->dim*sizeof(numdbl_t));
  free(a);
  free(bnds_a);

  dimperm=ap_dimperm_alloc(4);
  dim=(ap_dim_t *)malloc(4*sizeof(ap_dim_t));
  *dim=3;  *(dim+1)=2; *(dim+2)=1; *(dim+3)=0;
  dimperm->dim=dim;
  fpp1=fpp_permute_dimensions(mp,true,fpp1,dimperm);
  fprintf(stdout,"\n Dimension permute: \n");
  fpp_fdump(stdout, pr->man, fpp1);

  dimchange=ap_dimchange_alloc(3,2);
  dim=(ap_dim_t *)malloc(5*sizeof(ap_dim_t));
  *dim=0;  *(dim+1)=1; *(dim+2)=2; *(dim+3)=2; *(dim+4)=4;
  dimchange->dim=dim;
  fpp1=fpp_add_dimensions(mp,true,fpp1,dimchange,true);
  fprintf(stdout,"\n Dimension add: \n");
  fpp_fdump(stdout, pr->man, fpp1);
  fflush(stdout);

  fpp2=fpp_alloc_top(pr,2,0);
  *dim=0;  *(dim+1)=0; *(dim+2)=0; *(dim+3)=0; *(dim+4)=0;
  dimchange->dim=dim;
  fpp2=fpp_add_dimensions(mp,true,fpp2,dimchange,true);
  fprintf(stdout,"\n Dimension add from top: \n");
  fpp_fdump(stdout, pr->man, fpp2);
  fflush(stdout);

  *dim=0;  *(dim+1)=2; *(dim+2)=4; *(dim+3)=5; *(dim+4)=8;
  dimchange->dim=dim;
  fpp1=fpp_remove_dimensions(mp,true,fpp1,dimchange);
  fprintf(stdout,"\n Dimension remove: \n");
  fpp_fdump(stdout, pr->man, fpp1);
  ap_dimchange_clear(dimchange);
  ap_dimchange_free(dimchange);
}

void initial_test(void)
{
  fpp_t *fpp1;
  ap_dimchange_t* dimchange;
  ap_dim_t* dim;

  fpp1=fpp_alloc_internal(pr,0,0);
  dimchange=ap_dimchange_alloc(0,1);
  dim=(ap_dim_t *)malloc(1*sizeof(ap_dim_t));
  *dim=0;
  dimchange->dim=dim;
  fpp1=fpp_add_dimensions(mp,true,fpp1,dimchange,true);
  fprintf(stdout,"\n Dimension add from bottom: \n");
  fpp_fdump(stdout, pr->man, fpp1);
  fflush(stdout);

  ap_linexpr0_t* expr=ap_linexpr0_alloc(AP_LINEXPR_DENSE,2);
  ap_linexpr0_set_coeff_scalar_double(expr,0,0.0);
  ap_linexpr0_set_cst_scalar_double(expr,1.0);
  /* fpp1=fpp_assign_linexpr(mp,true,fpp1,1,expr,NULL); */

  ap_linexpr0_t** texpr=malloc(1*sizeof(ap_linexpr0_t*));
  texpr[0]=expr;
  fpp1= fpp_assign_linexpr_array(mp,true,fpp1,dim, texpr,1,NULL) ;

  fpp_fdump(stdout, pr->man, fpp1);
  fflush(stdout);
}

void widen_test(void)
{
  fpp_t *fpp1,*fpp2,*fpp3;

  fpp1=fpp_read_H();
  fpp2=fpp_read_H();
  fpp3=fpp_widening(pr->man,fpp1,fpp2);
  fprintf(stdout,"\n Widening: \n");
  fpp_fdump(stdout, pr->man, fpp3);
}

void assign_test(void)
{
  fpp_t *fpp1;
  ap_dimchange_t* dimchange;
  ap_dim_t* dim;

  fpp1=fpp_alloc_internal(pr,0,0);
  dimchange=ap_dimchange_alloc(0,1);
  dim=(ap_dim_t *)malloc(1*sizeof(ap_dim_t));
  *dim=0;
  dimchange->dim=dim;
  fpp1=fpp_add_dimensions(mp,true,fpp1,dimchange,true);
  fprintf(stdout,"\n Dimension add from bottom: \n");
  fpp_fdump(stdout, pr->man, fpp1);
  fflush(stdout);

  ap_linexpr0_t* expr=ap_linexpr0_alloc(AP_LINEXPR_DENSE,2);
  ap_linexpr0_set_coeff_scalar_double(expr,0,0.0);
  ap_linexpr0_set_cst_scalar_double(expr,1.0);
  /* fpp1=fpp_assign_linexpr(mp,true,fpp1,1,expr,NULL); */

  ap_linexpr0_t** texpr=malloc(1*sizeof(ap_linexpr0_t*));
  texpr[0]=expr;
  fpp1= fpp_assign_linexpr_array(mp,true,fpp1,dim, texpr,1,NULL) ;

  fpp_fdump(stdout, pr->man, fpp1);
  fflush(stdout);
}

void join_bv_rational_test(void)
{
  unsigned NbRows, NbColumns;
  char *c, s[128];
  numdbl_t *a;
  numdbl_t *bnds_a;
  fpp_t *fpp1,*fpp2,*fpp3;
  int *subset;

  /* read the rows and columns number */
  c = fgets(s, 128, stdin);
  while ( (*s=='#' || *s=='\n') || (sscanf(s, " %d %d", &NbRows, &NbColumns)<2) )
	  c = fgets(s, 128, stdin);
  /* read linear constraints*/
  a = Matrix_Read(NbRows, NbColumns);
  /* read bounds*/
  bnds_a = Matrix_Read(NbColumns-1,2);
  fpp1=fpp_alloc_urgent(pr,NbColumns-1,0,NbRows);
  memcpy(fpp1->cons,a,(int)(fpp1->ncons*(fpp1->dim+1))*sizeof(numdbl_t));
  memcpy(fpp1->bnds,bnds_a,(int)2*fpp1->dim*sizeof(numdbl_t));

  free(a);
  free(bnds_a);
  /* read the rows and columns number */
  c = fgets(s, 128, stdin);
  while ( (*s=='#' || *s=='\n') || (sscanf(s, " %d %d", &NbRows, &NbColumns)<2) )
       c = fgets(s, 128, stdin);
  /* read linear constraints*/
  a = Matrix_Read(NbRows, NbColumns);
  /* read bounds*/
  bnds_a = Matrix_Read(NbColumns-1,2);
  fpp2=fpp_alloc_urgent(pr,NbColumns-1,0,NbRows);
  memcpy(fpp2->cons,a,(int)(fpp2->ncons*(fpp1->dim+1))*sizeof(numdbl_t));
  memcpy(fpp2->bnds,bnds_a,(int)2*fpp2->dim*sizeof(numdbl_t));
  fpp3=fppol_strong_join_rational(pr,fpp1,fpp2);
  fprintf(stdout,"\n Strong join Rational: \n");
  fpp_fdump(stdout, pr->man, fpp3);
  fflush(stdout);
}

void rlp_test(void)
{
  glp_prob *lp;
  unsigned NbRows, NbColumns;
  char *c, s[128];
  numdbl_t *a;
  numdbl_t *l1;
  numdbl_t *bnds_a;
  fpp_t *fpp1;
  numdbl_t ci;
  bool nearly;

  /* read the rows and columns number */
  c = fgets(s, 128, stdin);
  while ( (*s=='#' || *s=='\n') || (sscanf(s, " %d %d", &NbRows, &NbColumns)<2) )
       c = fgets(s, 128, stdin);
  /* read linear constraints*/
  a = Matrix_Read(NbRows, NbColumns);
  /* read bounds*/
  bnds_a = Matrix_Read(NbColumns-1,2);
  fpp1=fpp_alloc_urgent(pr,NbColumns-1,0,NbRows);
  memcpy(fpp1->cons,a,(int)(fpp1->ncons*(fpp1->dim+1))*sizeof(numdbl_t));
  memcpy(fpp1->bnds,bnds_a,(int)2*fpp1->dim*sizeof(numdbl_t));
  free(a);
  free(bnds_a);

  fprintf(stdout,"\n The polyhedron: \n");
  fpp_fdump(stdout, pr->man, fpp1);
  l1= (numdbl_t *)malloc(fpp1->dim*sizeof(numdbl_t));

  *l1=-5;
  *(l1+1)=-4;
  *(l1+2)=-6;

  lp=rlp_create_matrix(fpp1);
  rlp_set_objective(lp,GLP_MIN,l1);
  lp=rlp_solve(lp);
  ci=rlp_get_optimal_value(lp,fpp1->bnds,false,&nearly);

  fprintf(stdout,"\n Optimal value: %.10f\n", ci);
  fflush(stdout);
  fpp_free_internal(pr, fpp1);
  checked_free(l1);
  lp_delete(lp);
}

void bt_rlp_test(void)
{
  unsigned NbRows, NbColumns;
  char *c, s[128];
  numdbl_t *a;
  numdbl_t *bnds_a;
  fpp_t *fpp1;

  /* read the rows and columns number */
  c = fgets(s, 128, stdin);
  while ( (*s=='#' || *s=='\n') || (sscanf(s, " %d %d", &NbRows, &NbColumns)<2) )
       c = fgets(s, 128, stdin);
  /* read linear constraints*/
  a = Matrix_Read(NbRows, NbColumns);
  /* read bounds*/
  bnds_a = Matrix_Read(NbColumns-1,2);
  fpp1=fpp_alloc_urgent(pr,NbColumns-1,0,NbRows);
  memcpy(fpp1->cons,a,(int)(fpp1->ncons*(fpp1->dim+1))*sizeof(numdbl_t));
  memcpy(fpp1->bnds,bnds_a,(int)2*fpp1->dim*sizeof(numdbl_t));
  free(a);
  free(bnds_a);

  fpp_init_bounds(fpp1->bnds, fpp1->dim);
  /* *(fpp1->bnds)=3; */
  fprintf(stdout,"\n before Bt: \n");
  fpp_fdump(stdout, pr->man, fpp1);
  fflush(stdout);

  fpp1=bt_byPureRLP(pr,true,fpp1,NULL,0);

  fprintf(stdout,"\n after Bt using RLP: \n");
  fpp_fdump(stdout, pr->man, fpp1);
  fflush(stdout);
}

/* ********************************* */
/*           weak join tests              */
/* ********************************* */

void wjoin_test(void)
{
  unsigned NbRows, NbColumns;
  char *c, s[256];
  numdbl_t *a;
  numdbl_t *bnds_a;
  fpp_t *fpp1,*fpp2,*fpp3;

  /* read the rows and columns number */
  c = fgets(s, 256, stdin);
  while ( (*s=='#' || *s=='\n') || (sscanf(s, " %d %d", &NbRows, &NbColumns)<2) )
       c = fgets(s, 256, stdin);
  /* read linear constraints*/
  a = Matrix_Read(NbRows, NbColumns);
  /* read bounds*/
  bnds_a = Matrix_Read(NbColumns-1,2);
  fpp1=fpp_alloc_urgent(pr,NbColumns-1,0,NbRows);
  memcpy(fpp1->cons,a,(int)(fpp1->ncons*(fpp1->dim+1))*sizeof(numdbl_t));
  memcpy(fpp1->bnds,bnds_a,(int)2*fpp1->dim*sizeof(numdbl_t));

  free(a);
  free(bnds_a);
  /* read the rows and columns number */
  c = fgets(s, 256, stdin);
  while ( (*s=='#' || *s=='\n') || (sscanf(s, " %d %d", &NbRows, &NbColumns)<2) )
       c = fgets(s, 256, stdin);
  /* read linear constraints*/
  a = Matrix_Read(NbRows, NbColumns);
  /* read bounds*/
  bnds_a = Matrix_Read(NbColumns-1,2);
  fpp2=fpp_alloc_urgent(pr,NbColumns-1,0,NbRows);
  memcpy(fpp2->cons,a,(int)(fpp2->ncons*(fpp1->dim+1))*sizeof(numdbl_t));
  memcpy(fpp2->bnds,bnds_a,(int)2*fpp2->dim*sizeof(numdbl_t));
  gettimeofday(&tpstart,NULL); maxCons=0;
  fpp3=fppol_weak_join_envbnds(pr,fpp1,fpp2);
  gettimeofday(&tpend,NULL);
  timeuse=1000000*(tpend.tv_sec-tpstart.tv_sec)+tpend.tv_usec-tpstart.tv_usec;
  timeuse /= 1000000;
  fprintf(stdout,"\n --------------------------------- \n");
  fprintf(stdout," fppol_weak_join_envbnds: \n");
  printf("Time:%f(%d)\n",timeuse,maxCons);
  fpp_fdump(stdout, pr->man, fpp3);
  fflush(stdout);

  fpp_free_internal(pr, fpp1);
  fpp_free_internal(pr, fpp2);
  fpp_free_internal(pr, fpp3);
}

int main(int argc, const char** argv)
{
  long int seed;
  int i;

  /* init */
  mp = fpp_manager_alloc();

  for (i=0;i<AP_FUNID_SIZE;i++) {
    mp->option.funopt[i].flag_exact_wanted = true;
    mp->option.funopt[i].flag_best_wanted = true;
  }
  for (i=0;i<AP_EXC_SIZE;i++){
    mp->option.abort_if_exception[i] = true;
  }
  pr = fpp_init_from_manager(mp,0,0);
  rlp_test();
  /* join_test();      */
  /* dimension_test(); */
  /* widen_test();     */
  /* initial_test();   */
  /* assign_test();    */
  /* join_bv_rational_test(); */
  /* wjoin_test();     */
  ap_manager_free(mp);
  return 0;
}
