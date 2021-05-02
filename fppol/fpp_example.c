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
struct timeval tpstart,tpend;
double timeuse;


/* *********************************** */
/*  Unit tests with hard-coded inputs  */
/* *********************************** */

void test_initial(void)
{
  fpp_t *fpp1;
  ap_dimchange_t* dimchange;
  ap_dim_t* dim;

  fprintf(stdout,"\n ----begin test_initial() ---- \n");

  fpp1=fpp_alloc_internal(pr,0,0);
  dimchange=ap_dimchange_alloc(0,1);
  dim=(ap_dim_t *)malloc(1*sizeof(ap_dim_t));
  *dim=0;
  dimchange->dim=dim;
  fpp1=fpp_add_dimensions(mp,true,fpp1,dimchange,true);
  fprintf(stdout,"\n Dimension add from bottom: \n");
  fpp_fdump(stdout, pr->man, fpp1);


  ap_linexpr0_t* expr=ap_linexpr0_alloc(AP_LINEXPR_DENSE,2);
  ap_linexpr0_set_coeff_scalar_double(expr,0,0.0);
  ap_linexpr0_set_cst_scalar_double(expr,1.0);

  ap_linexpr0_t** texpr=malloc(1*sizeof(ap_linexpr0_t*));
  texpr[0]=expr;
  fpp1= fpp_assign_linexpr_array(mp,true,fpp1,dim, texpr,1,NULL) ;

  fprintf(stdout,"\n After assignment x0 = 1 : \n");
  fpp_fdump(stdout, pr->man, fpp1);

  fprintf(stdout,"\n ----end test_initial() ---- \n");
}

void test_widen(void)
{
  unsigned NbRows, NbColumns;
  fpp_t *fpp1,*fpp2,*fpp3;

  fprintf(stdout,"\n ----begin test_widen() ---- \n");
  /* an example input polyhedron in H-representation */
  numdbl_t fpp1_data[32]={4, 3,  /* #Rows, #Columns */
		  	  	  	  	  0, -1, 0,  /* Constraints */
		  	  	  	  	  0, 1, 0,
		  	  	  	  	  0, 0, -1,
		  	  	  	  	  0, 0, 1,
		  	  	  	  	  0, 0,  /* Bounds */
		  	  	  	  	  0, 0};

  NbRows = (int) (fpp1_data[0]);  NbColumns= (int) (fpp1_data[1]);
  fpp1=fpp_alloc_urgent(pr,NbColumns-1,0,NbRows);
  memcpy(fpp1->cons,fpp1_data+2,(int)(fpp1->ncons*(fpp1->dim+1))*sizeof(numdbl_t));
  memcpy(fpp1->bnds,fpp1_data+2+(fpp1->ncons*(fpp1->dim+1)),(int)2*fpp1->dim*sizeof(numdbl_t));

  fprintf(stdout,"\n Polyhedron 1: \n");
  fpp_fdump(stdout, pr->man, fpp1);


  /* an example input polyhedron in H-representation */
  numdbl_t fpp2_data[32]={4, 3,  /* #Rows, #Columns */
						  0,  1, -1,  /* Constraints */
						  0, -1, 1,
						  1,  1, 0,
						  0, -1, 0,
						  0, 1,   /* Bounds */
						  0, 1};

  NbRows = (int) (fpp2_data[0]);  NbColumns= (int) (fpp2_data[1]);
  fpp2=fpp_alloc_urgent(pr,NbColumns-1,0,NbRows);
  memcpy(fpp2->cons,fpp2_data+2,(int)(fpp2->ncons*(fpp2->dim+1))*sizeof(numdbl_t));
  memcpy(fpp2->bnds,fpp2_data+2+(fpp2->ncons*(fpp2->dim+1)),(int)2*fpp2->dim*sizeof(numdbl_t));

  fprintf(stdout,"\n Polyhedron 2: \n");
  fpp_fdump(stdout, pr->man, fpp2);

  fpp3=fpp_widening(pr->man,fpp1,fpp2);
  fprintf(stdout,"\n Widening: \n");
  fpp_fdump(stdout, pr->man, fpp3);

  fprintf(stdout,"\n ----end test_widen() ---- \n");
}


void test_dimension(void)
{
  unsigned NbRows, NbColumns;
  fpp_t *fpp1,*fpp2;
  ap_dimchange_t* dimchange;
  ap_dimperm_t* dimperm;
  ap_dim_t* dim;

  fprintf(stdout,"\n ----begin test_dimension() ---- \n");
  /* an example input polyhedron in H-representation */
  numdbl_t fpp1_data[55]={9, 5,  /* #Rows, #Columns */
		  	  	  	  	  4.0, -1.0, -4.0, 2.1, 2.2,  /* Constraints */
		  	  	  	  	  20.0, 1.0, -4.0, 3.1, 3.2,
		  	  	  	  	  40.0, 3.0, -2.0, 4.1, 4.2,
		  	  	  	  	  14.0, 1.0, 0.0, 5.1, 5.2,
		  	  	  	  	  52.0, 3.0, 2.0, 6.1, 6.2,
		  	  	  	  	  36.0, 1.0, 3.0, 7.1, 7.2,
		  	  	  	  	  27.0, -1.0, 4.0, 8.1, 8.2,
		  	  	  	  	  3.0, -1.0, 1.0, 9.1, 9.2,
		  	  	  	  	  -6.0, -4.0, 1.0, 10.1, 10.2,
		  	  	  	  	  2.0, 14.0,  /* Bounds */
		  	  	  	  	  -3.0, 9.0,
		  	  	  	  	  3.1, 3.2,
		  	  	  	  	  4.1, 4.2};

  NbRows = (int) (fpp1_data[0]);  NbColumns= (int) (fpp1_data[1]);
  fpp1=fpp_alloc_urgent(pr,NbColumns-1,0,NbRows);
  memcpy(fpp1->cons,fpp1_data+2,(int)(fpp1->ncons*(fpp1->dim+1))*sizeof(numdbl_t));
  memcpy(fpp1->bnds,fpp1_data+2+(fpp1->ncons*(fpp1->dim+1)),(int)2*fpp1->dim*sizeof(numdbl_t));
  fprintf(stdout,"\n Input polyhedron: \n");
  fpp_fdump(stdout, pr->man, fpp1);

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

  *dim=0;  *(dim+1)=2; *(dim+2)=4; *(dim+3)=5; *(dim+4)=8;
  dimchange->dim=dim;
  fpp1=fpp_remove_dimensions(mp,true,fpp1,dimchange);
  fprintf(stdout,"\n Dimension remove: \n");
  fpp_fdump(stdout, pr->man, fpp1);

  fpp2=fpp_alloc_top(pr,2,0);
  *dim=0;  *(dim+1)=0; *(dim+2)=0; *(dim+3)=0; *(dim+4)=0;
  dimchange->dim=dim;
  fpp2=fpp_add_dimensions(mp,true,fpp2,dimchange,true);
  fprintf(stdout,"\n Dimension add from top: \n");
  fpp_fdump(stdout, pr->man, fpp2);

  ap_dimchange_clear(dimchange);
  ap_dimchange_free(dimchange);
  fprintf(stdout,"\n ----end test_dimension() ---- \n");
}

void test_join(void)
{
  unsigned NbRows, NbColumns;
  fpp_t *fpp1,*fpp2,*fpp3;

  fprintf(stdout,"\n ----begin test_join() ---- \n");
  /* an example input polyhedron in H-representation */
  numdbl_t fpp1_data[32]={3, 3,  /* #Rows, #Columns */
		  	  	  	 	  1.0, -1.0, 1.0,  /* Constraints */
		  	  	  	 	  2.0, 1.0, 0.0,
		  	  	  	 	  -1.0, 0.0, -1.0,
		  	  	  	 	  0.0, 2.0,   /* Bounds */
		  	  	  	 	  1.0, 3.0};

  NbRows = (int) (fpp1_data[0]);  NbColumns= (int) (fpp1_data[1]);
  fpp1=fpp_alloc_urgent(pr,NbColumns-1,0,NbRows);
  memcpy(fpp1->cons,fpp1_data+2,(int)(fpp1->ncons*(fpp1->dim+1))*sizeof(numdbl_t));
  memcpy(fpp1->bnds,fpp1_data+2+(fpp1->ncons*(fpp1->dim+1)),(int)2*fpp1->dim*sizeof(numdbl_t));
  fprintf(stdout,"\n Polyhedron 1: \n");
  fpp_fdump(stdout, pr->man, fpp1);

  /* an example input polyhedron in H-representation */
  numdbl_t fpp2_data[32]={4, 3,  /* #Rows, #Columns */
		  	  	  	      -1.0, -1.0, 0.0,  /* Constraints */
		  	  	  	      3.0, 1.0, 0.0,
		  	  	  	      -2.0, 0.0, -1.0,
		  	  	  	      4.0, 0.0, 1.0,
		  	  	  	      1.0, 3.0,   /* Bounds */
		  	  	  	      2.0, 4.0};
  NbRows = (int) (fpp2_data[0]);  NbColumns= (int) (fpp2_data[1]);
  fpp2=fpp_alloc_urgent(pr,NbColumns-1,0,NbRows);
  memcpy(fpp2->cons,fpp2_data+2,(int)(fpp2->ncons*(fpp2->dim+1))*sizeof(numdbl_t));
  memcpy(fpp2->bnds,fpp2_data+2+(fpp2->ncons*(fpp2->dim+1)),(int)2*fpp2->dim*sizeof(numdbl_t));
  fprintf(stdout,"\n Polyhedron 2: \n");
  fpp_fdump(stdout, pr->man, fpp2);

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

  fprintf(stdout,"\n ----end test_join() ---- \n");
}


void test_meet(void)
{
  unsigned NbRows, NbColumns;
  fpp_t *fpp1,*fpp2,*fpp3;

  fprintf(stdout,"\n ----begin test_meet() ---- \n");
  /* an example input polyhedron in H-representation */
  numdbl_t fpp1_data[32]={3, 3,  /* #Rows, #Columns */
		  	  	  	 	  1.0, -1.0, 1.0,  /* Constraints */
		  	  	  	 	  2.0, 1.0, 0.0,
		  	  	  	 	  -1.0, 0.0, -1.0,
		  	  	  	 	  0.0, 2.0,   /* Bounds */
		  	  	  	 	  1.0, 3.0};

  NbRows = (int) (fpp1_data[0]);  NbColumns= (int) (fpp1_data[1]);
  fpp1=fpp_alloc_urgent(pr,NbColumns-1,0,NbRows);
  memcpy(fpp1->cons,fpp1_data+2,(int)(fpp1->ncons*(fpp1->dim+1))*sizeof(numdbl_t));
  memcpy(fpp1->bnds,fpp1_data+2+(fpp1->ncons*(fpp1->dim+1)),(int)2*fpp1->dim*sizeof(numdbl_t));
  fprintf(stdout,"\n Polyhedron 1: \n");
  fpp_fdump(stdout, pr->man, fpp1);

  /* an example input polyhedron in H-representation */
  numdbl_t fpp2_data[32]={4, 3,  /* #Rows, #Columns */
		  	  	  	      -1.0, -1.0, 0.0,  /* Constraints */
		  	  	  	      3.0, 1.0, 0.0,
		  	  	  	      -2.0, 0.0, -1.0,
		  	  	  	      4.0, 0.0, 1.0,
		  	  	  	      1.0, 3.0,   /* Bounds */
		  	  	  	      2.0, 4.0};
  NbRows = (int) (fpp2_data[0]);  NbColumns= (int) (fpp2_data[1]);
  fpp2=fpp_alloc_urgent(pr,NbColumns-1,0,NbRows);
  memcpy(fpp2->cons,fpp2_data+2,(int)(fpp2->ncons*(fpp2->dim+1))*sizeof(numdbl_t));
  memcpy(fpp2->bnds,fpp2_data+2+(fpp2->ncons*(fpp2->dim+1)),(int)2*fpp2->dim*sizeof(numdbl_t));
  fprintf(stdout,"\n Polyhedron 2: \n");
  fpp_fdump(stdout, pr->man, fpp2);

  fpp1=bt_byExactLP(pr,true,fpp1, NULL, fpp1->dim);
  fpp2=bt_byExactLP(pr,true,fpp2, NULL, fpp2->dim);

  fprintf(stdout,"\n ==== Meet ====== \n");
  fprintf(stdout,"\n Input P1: \n");
  fpp_fdump(stdout, pr->man, fpp1);
  fprintf(stdout,"\n Input P2: \n");
  fpp_fdump(stdout, pr->man, fpp2);
  fpp3=fpp_meet(pr->man,true,fpp1,fpp2);
  fprintf(stdout,"\n Result: \n");
  fpp_fdump(stdout, pr->man, fpp3);

  fprintf(stdout,"\n ----end test_meet() ---- \n");
}

void test_assign(void)
{
  fpp_t *fpp1;
  ap_dimchange_t* dimchange;
  ap_dim_t* dim;

  fprintf(stdout,"\n ----begin test_assign() ---- \n");
  fpp1=fpp_alloc_internal(pr,0,0);
  dimchange=ap_dimchange_alloc(0,1);
  dim=(ap_dim_t *)malloc(1*sizeof(ap_dim_t));
  *dim=0;
  dimchange->dim=dim;
  fpp1=fpp_add_dimensions(mp,true,fpp1,dimchange,true);
  fprintf(stdout,"\n Dimension add from bottom: \n");
  fpp_fdump(stdout, pr->man, fpp1);

  ap_linexpr0_t* expr=ap_linexpr0_alloc(AP_LINEXPR_DENSE,2);
  ap_linexpr0_set_coeff_scalar_double(expr,0,0.0);
  ap_linexpr0_set_cst_scalar_double(expr,1.0);

  ap_linexpr0_t** texpr=malloc(1*sizeof(ap_linexpr0_t*));
  texpr[0]=expr;
  fpp1= fpp_assign_linexpr_array(mp,true,fpp1,dim, texpr,1,NULL) ;
  fpp_fdump(stdout, pr->man, fpp1);

  fprintf(stdout,"\n ----end test_assign() ---- \n");
}

void test_rlp(void)
{
  glp_prob *lp;
  unsigned NbRows, NbColumns;
  fpp_t *fpp1;
  numdbl_t ci;
  bool nearly;

  fprintf(stdout,"\n ----begin test_rlp() ---- \n");

  /* an example input polyhedron in H-representation */
  numdbl_t data[32]={6,4,  /* #Rows, #Columns */
		             100.0, 1.0, 1.0, 1.0,  /* Constraints */
		             600.0, 10.0, 4.0, 5.0,
		             300.0, 2.0, 2.0, 6.0,
		             0.0, -1.0, 0.0, 0.0,
		             0.0, 0.0, -1.0, 0.0,
		             0.0, 0.0, 0.0, -1.0,
		             0, 40.0,               /* Bounds */
					 0, 80.0,
					 0, 10.0};

  NbRows = (int) (data[0]);  NbColumns= (int) (data[1]);
  fpp1=fpp_alloc_urgent(pr,NbColumns-1,0,NbRows);
  memcpy(fpp1->cons,data+2,(int)(fpp1->ncons*(fpp1->dim+1))*sizeof(numdbl_t));
  memcpy(fpp1->bnds,data+2+(fpp1->ncons*(fpp1->dim+1)),(int)2*fpp1->dim*sizeof(numdbl_t));

  fprintf(stdout,"\n The polyhedron: \n");
  fpp_fdump(stdout, pr->man, fpp1);

  /* objective function: */
  numdbl_t obj[3]={	-5.0, -4.0, -6.0};
  fprintf(stdout,"\n Objective function: min %f x0 + %f x1 + %f x2\n", obj[0], obj[1], obj[2]);

  lp=rlp_create_matrix(fpp1);
  rlp_set_objective(lp,GLP_MIN,obj);
  lp=rlp_solve(lp);
  ci=rlp_get_optimal_value(lp,fpp1->bnds,false,&nearly);
  fprintf(stdout,"\n RLP optimal value: %.30f\n", ci);

  ci = slp_exact(fpp1,GLP_MIN,obj);
  fprintf(stdout,"\n GLPK-exact optimal value: %.30f\n", ci);

  fpp_free_internal(pr, fpp1);
  lp_delete(lp);
  fprintf(stdout,"\n ----end test_rlp() ---- \n");
}

/* *********************************** */
/*  Unit tests with inputs from pipe   */
/* *********************************** */

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

void test_join_via_file_input(void)
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


void test_dimension_via_file_input(void)
{
  unsigned NbRows, NbColumns;
  char *c, s[128];
  numdbl_t *a;
  numdbl_t *bnds_a;
  fpp_t *fpp1,*fpp2;
  ap_dimchange_t* dimchange;
  ap_dimperm_t* dimperm;
  ap_dim_t* dim;

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

  fpp2=fpp_alloc_top(pr,2,0);
  *dim=0;  *(dim+1)=0; *(dim+2)=0; *(dim+3)=0; *(dim+4)=0;
  dimchange->dim=dim;
  fpp2=fpp_add_dimensions(mp,true,fpp2,dimchange,true);
  fprintf(stdout,"\n Dimension add from top: \n");
  fpp_fdump(stdout, pr->man, fpp2);

  *dim=0;  *(dim+1)=2; *(dim+2)=4; *(dim+3)=5; *(dim+4)=8;
  dimchange->dim=dim;
  fpp1=fpp_remove_dimensions(mp,true,fpp1,dimchange);
  fprintf(stdout,"\n Dimension remove: \n");
  fpp_fdump(stdout, pr->man, fpp1);
  ap_dimchange_clear(dimchange);
  ap_dimchange_free(dimchange);
}

void test_widen_via_file_input(void)
{
  fpp_t *fpp1,*fpp2,*fpp3;

  fpp1=fpp_read_H();
  fpp2=fpp_read_H();
  fpp3=fpp_widening(pr->man,fpp1,fpp2);
  fprintf(stdout,"\n Widening: \n");
  fpp_fdump(stdout, pr->man, fpp3);
}


void test_join_bv_rational(void)
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
}



void test_rlp_via_file_input(void)
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
  fpp_free_internal(pr, fpp1);
  checked_free(l1);
  lp_delete(lp);
}

void test_bt_rlp(void)
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
  fprintf(stdout,"\n before Bt: \n");
  fpp_fdump(stdout, pr->man, fpp1);

  fpp1=bt_byPureRLP(pr,true,fpp1,NULL,0);

  fprintf(stdout,"\n after Bt using RLP: \n");
  fpp_fdump(stdout, pr->man, fpp1);
}

/* ********************************* */
/*           weak join tests              */
/* ********************************* */

void test_wjoin(void)
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

  fpp_free_internal(pr, fpp1);
  fpp_free_internal(pr, fpp2);
  fpp_free_internal(pr, fpp3);
}


/* ********************************* */
/*           main                    */
/* ********************************* */

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
  test_initial();
  test_rlp();
  test_assign();
  test_dimension();
  test_meet();
  test_join();
  test_widen();
  ap_manager_free(mp);
  return 0;
}
