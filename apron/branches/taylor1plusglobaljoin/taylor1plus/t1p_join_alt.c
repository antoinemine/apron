/* ********************************************************************** */
/* t1p_join_alt.c: general management */
/* ********************************************************************** */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>

#include "ap_generic.h"
#include "t1p_internal.h"
#include "t1p_join_alt.h"
#include "ap_manager.h"
#include "ap_lincons0.h"


/* ********************************************************************** */
/* 0. Equation manipulation */
/* ********************************************************************** */

/* constructors */
ja_eq_t* new_equation (void)
{
  CALL();
  ja_eq_t* res =malloc(sizeof(ja_eq_t));
  res->dim=0;
  itv_init(res->c);
  res->first_te=NULL;
  res->last_te=NULL;
  return res;
}

void free_equation (ja_eq_t* equation)
{
  CALL();
  ja_term_t* cell1= equation->first_te;
  ja_term_t* cell2;
  while (cell1 != NULL)
    {
      cell2=cell1;
      cell1=cell1->n;
      free(cell2);
    }
  free(equation);
}

void print_equation (FILE* stream, ja_eq_t* equation)
{
  CALL();
  fprintf(stream,"x%d=",(int) equation->dim);
  itv_fprint(stream, equation->c);
  ja_term_t* cell= equation->first_te;

   while (cell != NULL)
     {
     fprintf(stream,"+");
      itv_fprint(stream, cell->coeff);
      if (cell->t_coeff==NS)
	{
	  fprintf(stream,".");
	  t1p_nsym_fprint(stream,cell->pnsym);
	}
      else
	fprintf(stream,".x%d",(int) cell->dim);
      cell=cell->n;
    }
   fprintf(stream,"\n");
}


void add_equation_term_ns (ja_eq_t* equation, itv_t c, t1p_nsym_t* pnsym)
{
  CALL();
  ja_term_t* cell=malloc(sizeof(ja_term_t));
  cell->n=NULL;
  cell->t_coeff=NS;
  cell->dim=0;
  cell->pnsym=pnsym;
  itv_set(cell->coeff,c);
  
  if (equation->last_te==NULL)
    {
      equation->first_te=cell;
      equation->last_te=cell;
    }
  else
    {
      equation->last_te->n=cell;
      equation->last_te=cell;
    }
}

ja_eq_t* copy_equation (ja_eq_t* equation)
{
  CALL ();
  printf("copy equation not implemented");
  return NULL;
}

void add_equation_term_va (ja_eq_t* equation, itv_t c, ap_dim_t dim)
{
  CALL();
  ja_term_t* cell=malloc(sizeof(ja_term_t));
  cell->n=NULL;
  cell->t_coeff=VA;
  cell->dim=dim;
  cell->pnsym=NULL;
  itv_set(cell->coeff,c);
  
  if (equation->last_te==NULL)
    {
      equation->first_te=cell;
      equation->last_te=cell;
    }
  else
    {
      equation->last_te->n=cell;
      equation->last_te=cell;
    }
}
  


ja_eq_set_t* new_equation_set (void)
{
  CALL();
  ja_eq_set_t* res=malloc(sizeof(ja_eq_set_t));
  res->nb_eq= 0;
  res->first_eq=NULL;
  res->last_eq=NULL;
  return res;
}

void free_equation_set (ja_eq_set_t* eqs)
{
  CALL();
  ja_eq_list_elm* cell1= eqs->first_eq;
  ja_eq_list_elm* cell2;
  while (cell1 != NULL)
    {
      cell2=cell1;
      cell1=cell1->n;
      free_equation(cell2->content);
      free(cell2);
    }
  free(eqs);
}

void print_equation_set(FILE* stream, ja_eq_set_t* eqs)
{
  CALL();
  ja_eq_list_elm* cell= eqs->first_eq;
  while (cell != NULL)
    {
      print_equation(stream,cell->content);
      cell=cell->n;
    }
}


void add_equation (ja_eq_set_t* eqs, ja_eq_t* eq)
{
  CALL();
  /* creation of the cell of the list */
  ja_eq_list_elm* cell=malloc(sizeof(ja_eq_list_elm));
  assert (cell->n==NULL);
  cell->content=eq;

  if (eqs->nb_eq==0)
    {
      eqs->nb_eq=1;
      eqs->first_eq= cell;
      eqs->last_eq= cell;
    }
  else
    {
      eqs->nb_eq++;
      ja_eq_list_elm* last = eqs->last_eq;
      last->n=cell;
      eqs->last_eq=cell;
    }
}


itv_t* get_coeff_var (ja_eq_set_t* eqs, int eq_n, ap_dim_t v)
{
  CALL();
  assert (eq_n < eqs->nb_eq); 
  /* find the right equation */
  ja_eq_list_elm* current_elm = eqs->first_eq;
  for (int i=0;i<eq_n;i++)
    {
      current_elm = current_elm->n;
    }
  /* found */
  /* find the right coefficient */
  ja_term_t* current_term = (current_elm->content)->first_te;
  while (current_term->dim != v)
    {
      current_term=current_term->n;
    }
  /* found */
  return &(current_term->coeff);
}


itv_t* get_coeff_nsym (ja_eq_set_t* eqs, int eq_n, size_t v)
{
  CALL();
   assert (eq_n < eqs->nb_eq); 
  /* find the right equation */
  ja_eq_list_elm* current_elm = eqs->first_eq;
  printf("*\n");
  for (int i=0;i<eq_n;i++)
    {
      current_elm = current_elm->n;
    }
  print_equation(stdout,current_elm->content);
  printf("*\n");
  printf("index of nsym %zu \n",v);
  /* found */
  /* find the right coefficient */
  ja_term_t* current_term = current_elm->content->first_te;
  while (current_term->pnsym->index != v)
    {
      printf(".\n");
      current_term=current_term->n;
    }
  /* found */
  return &(current_term->coeff);
}



/* ********************************************************************** */
/* 1. From 2 abstract values to equations */
/* ********************************************************************** */

/* result is of type B */
ja_eq_set_t* abstract_value_to_eq_set (t1p_internal_t* pr, t1p_t* a1,  t1p_t* a2)
{
  CALL();
  ja_eq_set_t* res = new_equation_set();
  size_t dims = a1->dims;
  assert (dims == a2->dims); // to be sure the abstract values have the same dims
  int i;
  int j;
  itv_t* pitv1=NULL;
  itv_t* pitv2=NULL;
  itv_t itv_buff;
  ja_eq_t* equation;
  int nb_nsym=0; // number of noise symbols in a1 and a2: we look at the last symbol of each affine form
  for (i=0 ; i< (int) dims;i++)
    {
      if (a1->paf[i]->end != NULL)
	{
	  if ((int) a1->paf[i]->end->pnsym->index > nb_nsym)
	    nb_nsym= a1->paf[i]->end->pnsym->index;
	}
      if (a2->paf[i]->end != NULL)
	{ 
	  if((int) a2->paf[i]->end->pnsym->index > nb_nsym)
	    nb_nsym= a2->paf[i]->end->pnsym->index;
	}
    }
 
  /* nb_nsym is the last index present, we add 1 to have the number of noise symbols*/
  nb_nsym++;

  /* printf("nb_nsym=%zu \n",nb_nsym); */

  /* special loop for the constant terms */
  equation=new_equation();
  itv_init(itv_buff);
  equation->dim=(ap_dim_t) (-1);

  for (j=0; j<(int)dims ; j++)
    {
      /* printf("dim %d: itv1",j); */
      /* itv_print((a1->paf[j])->c); */
      /* printf("  itv2"); */
      /* itv_print((a2->paf[j])->c); */
      itv_sub(itv_buff,(a1->paf[j])->c,(a2->paf[j])->c);
      /* printf("\n itv1-itv2 ="); */
      /* itv_print(itv_buff); */
      /* printf("\n"); */
      add_equation_term_va (equation, itv_buff,(ap_dim_t) j);
      itv_clear(itv_buff);
    }
  /* printf("*\n"); */
  add_equation(res,equation);

  /* 
     other loops
   */
  for (i=0;i<nb_nsym;i++)
    {
      equation=new_equation();
      /* printf("*\n"); */
      equation->dim=(ap_dim_t)i;
     
      itv_clear(itv_buff);

      for (j=0; j<(int)dims ; j++)
	{
	  /* printf("j=%d\n",j); */
	  /* printf("a1[%d]",j); */
	  /* t1p_aff_fprint(pr,stdout,a1->paf[j]); */
	  /* printf("\n"); */
	  /* printf("a2[%d]",j); */
	  /* t1p_aff_fprint(pr,stdout,a2->paf[j]); */
	  /* printf("\n"); */
	  pitv1=t1p_aff_get_coeff(pr,a1->paf[j],i);
	  pitv2=t1p_aff_get_coeff(pr,a2->paf[j],i);

	  if (pitv1==NULL)
	    {
	      if (pitv2!=NULL)
		{
		  /* printf("pitv2 (eps %d):",i); */
		  /* itv_print(*pitv2); */
		  /* printf("\n"); */
		  itv_neg(itv_buff,*pitv2);
		 
		}
	      // else do nothing, ie set itv_buff to 0
	      else
		itv_set_int(itv_buff,0);
	      /* 	printf("both pointers are null for eps%d\n",i); */
	    }
	  else
	    {
	      if (pitv2==NULL)
		{
		  /* printf("pitv1 (eps %d):",i); */
		  /* itv_print(*pitv1); */
		  /* printf("\n"); */
		  itv_set(itv_buff,*pitv1);
		  /* printf("add "); */
		  /* itv_print(itv_buff); */
		  /* printf(" eps%d\n",j); */
		  
		}
	      else // both pointers are not null
		{
		  /* printf("pitv1 (eps %d):",i); */
		  /* itv_print(*pitv1); */
		  /* printf("\n"); */
		  /* printf("pitv2 (eps %d):",i); */
		  /* itv_print(*pitv2); */
		  /* printf("\n"); */
		  itv_sub(itv_buff,*pitv1,*pitv2);
		  /* printf("add "); */
		  /* itv_print(itv_buff); */
		  /* printf(" eps%d\n",j); */
		  
		}
	    }
	  /* if itv_buff is not zero, add the term */
	  if (! itv_is_zero(itv_buff))
	    {
	      /* printf("add "); */
	      /* itv_print(itv_buff); */
	      /* printf(" eps%d\n",j); */
	      add_equation_term_va (equation, itv_buff,(ap_dim_t)j);
	    }
	  /* else */
	  /*   { */
	  /*     printf("coeff of eps%d is null\n ",j); */
	  /*   } */
	  
	  /* printf(".\n"); */
	  itv_clear(itv_buff);
	}
      add_equation(res,equation);
    }
  /* printf("*\n"); */
  return res;
}



/* ********************************************************************** */
/* 2. Solving the equations */
/* ********************************************************************** */

/* the functions of this section MUST be called IN THIS WAY:
   
   INPUT: N+1 equations (result of the function  [abstract_value_to_eq_set])
   ...... over P program variables (dimensions in APRON)


   0: create the matrix m  with N+1 rows and P columns
   1: call the function [eq_set_B_to_matrix] to initialise matrix m
   2: transform m with the function [matrix_jordan_reduction] ->(rank)
   3: call the function [matrix_to_eq_set_Aprime] on m and the input equation
   4: re-create m with (P-rank) rows and N+P+1 columns
   5: call the function [eq_set_Aprime_to_matrix] to re-initialise matrix m
   6: transform m with the function [matrix_jordan_reduction] ->(P-rank)
   7: call the function [matrix_to_eq_set_A] on m

   the result is (P-rank) equations of the form X = c +
   alpha1 X1+ ... + alphap Xp + beta0 epsilon0 + ... + betaN epsilonN


*/




/* *******************/
/* Matrix dump debug */
/* *******************/

void matrix_fdump (FILE* stream, int nb_rows, int nb_columns, itv_t** m) {
  int i,j;
  for (i=0;i<nb_rows;i++) {
    for (j=0;j<nb_columns;j++) {
      fprintf(stream,"  ");
      itv_fprint(stream,m[i][j]);
    }
     fprintf(stream,"\n");
  }
}



/* *****************/
/* equation2matrix */
/* *****************/

/* change an equation set into a matrix */
/* equation [i] is of the form eps_(i-1) = 0 + c_0.dim_0 + ... + c_p.dim_p 
   when the program has p numerical variables */
/* matrix m must be allocated before calling this function */

/* eqs is of type B */
void eq_set_B_to_matrix (ja_eq_set_t* eqs, int nb_rows, int nb_columns, itv_t** m) {
  CALL();
  int i,j;
  ja_eq_list_elm* current_equation= eqs->first_eq ;
  ja_term_t* current_term;

  assert (eqs->nb_eq == nb_rows);

  for (i=0;i<nb_rows;i++) {
    assert(current_equation != NULL);

    current_term = current_equation->content->first_te;

    /* assume that the coefficients are sorted by increasing dimension */
    for (j=0; j<nb_columns; j++) {
      if (current_term == NULL)
	{
	  itv_set_int(m[i][j],0);
	}
      else
	{
	  if (j==(int)current_term->dim) {
	    assert(current_term->t_coeff == VA);
	    itv_set(m[i][j],current_term->coeff);
	    current_term=current_term->n;
	  }
	  else {
	    itv_set_int(m[i][j],0);
	  }
	}
    }
    current_equation = current_equation->n;
  }
}



/* eqs is of type Aprime */
/* at the end, in matrix m, the [dims] first columns correspond to the alpha_i, the columns [dims] is the constant and the other rows are coefficient beta_i*/

void eq_set_Aprime_to_matrix (ja_eq_set_t* eqs, int dims, int nb_nsym, itv_t** m) {
  int i,j;
  itv_t* pitv;
  int nb_rows= eqs->nb_eq;
  ja_eq_list_elm* current_equation=eqs->first_eq;

  for(i=0;i<nb_rows;i++){
    printf("*\n");
    /* dims first columns */
    for (j=0;j<dims;j++){
      printf(".\n");
      pitv=get_coeff_var (eqs, i, (ap_dim_t) j);
      if (pitv!=NULL){
	itv_set(m[i][j],*pitv);
      }
      else {
	itv_set_int(m[i][j],0);
      }
    }
    /* column dims */
    printf(";\n");
    itv_set(m[i][dims],current_equation->content->c);

    /* other columns */
    for (j=0;j<nb_nsym;j++){
      printf(".\n");
      pitv=get_coeff_nsym (eqs, i, (size_t) j);
      printf(",\n");
      if (pitv!=NULL){
	itv_set(m[i][j+dims+1],*pitv);
      }
      else {
	itv_set_int(m[i][j+dims+1],0);
      }
    }
    current_equation=current_equation->n;
  }
}





/* *****************/
/* Gauss reduction */
/* *****************/


/* matrix_swap - swap rows i and j of matrix m */

void matrix_swap(itv_t** m, int i, int j) {
  CALL();
  itv_t* temp;
  temp=m[i];
  m[i]=m[j];
  m[j]=temp;
}



/* m[i] <- m[i] / lambda, length is the number of columns */ 
void matrix_row_div (itv_internal_t* intern, int length, itv_t** m, int i, itv_t lambda) {
  CALL();
  int k; // loop counter
  for (k=0; k<length;k++) {
    itv_div(intern,m[i][k],m[i][k],lambda);
  }
}



/* m[i] <- m[i] + lambda * m[j], length is the number of columns */ 
void matrix_lin_aff(itv_internal_t* intern, int length, itv_t** m, int i, itv_t lambda, int j) {
  CALL();
  itv_t temp;
  int k; // loop counter
  itv_init(temp);
  //loop
  for (k=0; k<length;k++) {
    itv_mul(intern,temp,lambda,m[j][k]);
    itv_add(m[i][k],m[i][k],temp);
    itv_clear(temp);
  }
}



/* choose the first non zero value of m[i] and returns its index. Return length if all coefficients are zero */
int matrix_choose_pivot(int length, itv_t** m,  int i) {
  CALL();
  int k;
  int res = length;
  for (k=0;k<res;k++) {
    if ( ! itv_is_zero(m[i][k])) {
      res=k;
    }      
  }
  return res;
}



/* the next function reduces the equation M.X=0 and returns the "rank" (number of nonzero rows of the resulting matrix) */
/* NB: it assumes that matrix m has nb_rows rows and nb_columns columns 
*/

 int matrix_jordan_reduction (itv_internal_t* intern, int nb_rows, int nb_columns, itv_t**m) {

  CALL();
  int current_row;
  int i,j,k,l;
  itv_t pivot,lambda;
  itv_init(pivot);
  itv_init(lambda);

  for(current_row=0; current_row<nb_rows;current_row++) {
    // search the row with the smallest pivot
    j=matrix_choose_pivot(nb_columns,m,current_row); //smallest pivot value so far
    l=current_row; // row where it was found
    for(i=current_row+1; i<nb_rows;i++) {
      k=matrix_choose_pivot(nb_columns,m,i);
      if (k<j) {
	l=i;
	j=k;
      }
    }
    //swap if needed
    if (l>current_row) {
      matrix_swap(m,current_row,l);
    }
    // now, for al i>current_row, pivot(i) >= pivot(current_row)

    /* if j = nb_columns, it means that all rows are with zero coefficient */
    if (j== nb_columns) {
      return current_row;
      // stop the loop and return the "rank"
    }
    else {
      itv_set(pivot,m[current_row][j]); // set the pivot
      /*  m[i] <- m[i] / pivot */
      matrix_row_div(intern,nb_columns,m,current_row,pivot);

      /* check that the pivot m[current_row][j] is now [1,1] */
      itv_set_int(pivot,1); 
      assert(itv_is_eq(pivot,m[current_row][j]));

      for(i=current_row+1; i<nb_rows;i++) {
	itv_neg(lambda,m[i][j]); // lambda = - m[i][j]
	/*  m[i] <- m[i] + lambda *m[current_row] */ 
	matrix_lin_aff(intern,nb_columns,m,i,lambda,current_row); 
	assert (itv_is_zero(m[i][j])); // check that the new coefficient is zero
	itv_clear(lambda);
      }
      itv_clear(pivot);
    }
  }
  return nb_rows;
}






/* *****************/
/* matrix2equation */
/* *****************/

/* change an LU matrix and an equation set (of the form beta = c1
   alpha1 + ... ) into an equation set (of the form x = alpha1 x1+
   ... +beta1 epsilon1  +...  */
/* WARNING: this is NOT the inverse function of eq_set_B_to_matrix. */


/* the k first rows of the matrix are supposed to be LU, the diagonal
   must contain only [1,1] values, and the other rows should contain
   only zero values (in fact, they are ignored) */

/* returns [rank] equations (in this order):
   x_rank =  (something depending only on x_j with j>rank and noise symbols)
   x_rank-1 =  (something depending only on x_j with j>rank-1 and noise symbols)
   ...
   x_0 =  (something depending only on x_j with j>0 and noise symbols)
 */

/* eqs is of type B */
/* result is of type Aprime */
ja_eq_set_t* matrix_to_eq_set_Aprime (t1p_internal_t* pr, int rank, int nb_rows, int nb_columns, itv_t**m, ja_eq_set_t* eqs) {
  ja_eq_set_t* res = new_equation_set();
  int i,j,k;
  itv_t tab_alpha [nb_columns];
  itv_t buff_coeff, buff_sum; // buffers for interval
  ja_eq_t* equation = NULL;  // what will be added to the result
  ja_eq_list_elm* current_equation = NULL; // iterator on eqs
  ja_term_t* current_term = NULL; //sub iterator

  /* initialization */
  assert (rank <= nb_columns);
  for (i=0;i<nb_columns;i++) {
    itv_init(tab_alpha[i]);
  }
  itv_init(buff_coeff);
  itv_init(buff_sum);
  if(eqs->nb_eq==0){
    printf("ERROR: in function matrix_to_eq_set_Aprime, eqs must have at least one equation ");
    assert(false);
  }  

  /* main loop */
  for (k=rank;k<nb_columns;k++) {
    /* each value of k determine a value for the coefficients alpha_i (we take the canonical base for alha_i with i>=rank), thus an independant equation. */


    /* (re)set tab_alpha  i>=rank */
    for (i=rank;i<nb_columns;i++) {
      if (i==k)
	itv_set_int(tab_alpha[i],1);
      else
	itv_set_int(tab_alpha[i],0);
    }

       /* (re)set tab_alpha  i<rank */
    for (i=rank-1;i>=0;i--) {
      /* computes the value of alpha[i] thanks to m */
      /* m[i] is a equation of the form sum(m[i][j] alpha[j]) = 0 */
      /* remember: m is LU with diag(m) = Id */
      itv_set_int(buff_sum,0);
      for (j=i+1;j<nb_columns;j++) {
	/* */
	itv_mul(pr->itv,buff_coeff,tab_alpha[j],m[i][j]);
	itv_add(buff_sum,buff_sum,buff_coeff);
	itv_clear(buff_coeff);
      }
    itv_neg(buff_sum,buff_sum);
    itv_set(tab_alpha[i],buff_sum);
    itv_clear(buff_sum);
    }

    /* set the equation */
    equation = new_equation(); 
    equation->dim=(ap_dim_t)k;

   /* set the coefficient for the program variables */
    for (i=0;i<nb_columns;i++) {
      add_equation_term_va(equation,tab_alpha[i],i);
    }

    /* first equation of eqs determines the center of [equation] */
    current_equation = eqs->first_eq;
    current_term = current_equation->content->first_te; 
    itv_set_int(buff_sum,0);

    while (current_term != NULL) {
      assert(current_term->t_coeff==VA);
      itv_set(buff_coeff,current_term->coeff);
      j=(int) current_term->dim;
      itv_mul(pr->itv,buff_coeff,buff_coeff,tab_alpha[j]);
      itv_add(buff_sum,buff_sum,buff_coeff);
      itv_clear(buff_coeff);
      current_term = current_term->n;
    }
    /* set the center */
    itv_neg(buff_sum,buff_sum);
    itv_set(equation->c,buff_sum);
    itv_clear(buff_sum);
    current_equation = current_equation->n;

    /* other equations determine the coefficients beta i*/
    while (current_equation != NULL) {
      current_term = current_equation->content->first_te; 
      itv_set_int(buff_sum,0);
      i=(int)current_equation->content->dim;

      while (current_term != NULL) {
	assert(current_term->t_coeff==VA);
	itv_set(buff_coeff,current_term->coeff);
	j=(int) current_term->dim;
	itv_mul(pr->itv,buff_coeff,buff_coeff,tab_alpha[j]);
	itv_add(buff_sum,buff_sum,buff_coeff);
	itv_clear(buff_coeff);
	current_term = current_term->n;
      }
      /* add the noise symbol eps_i */
      itv_neg(buff_sum,buff_sum);
      add_equation_term_ns(equation,buff_sum,(pr->epsilon)[i]);
      itv_clear(buff_sum);
      current_equation = current_equation->n;
    }
    /* add equation to res */
    add_equation (res, equation);
  }
  return res;
}





/* result is is of type A */
ja_eq_set_t* matrix_to_eq_set_A (t1p_internal_t* pr, int nb_rows, int dims, int nb_nsym, itv_t** m) {
  ja_eq_set_t* res = new_equation_set();
  int i,j;
  ja_eq_t* equation = NULL;  // what will be added to the result

  /* the equations are returned in the order of dependancy */
  for(i=nb_rows-1;i>=0;i--){
    equation=new_equation();
    equation->dim=i;

    for (j=i+1;j<dims;j++){
      add_equation_term_va(equation,m[i][j],(ap_dim_t)j);
    }
    itv_set(equation->c,m[i][dims]);
    for (j=0;j<nb_nsym;j++){
      add_equation_term_ns(equation,m[i][j+dims+1],(pr->epsilon)[j]);
    }
    add_equation(res,equation);
  }
      
  return res;
}









/* ********************************************************************** */
/* 1. eq_set_transformation */
/* ********************************************************************** */


/* eqs is of type B */
/* result is of type A */
ja_eq_set_t* eq_set_transformation (t1p_internal_t* pr, ja_eq_set_t* eqs, int dimensions) {
  CALL();
  int nb_rows = eqs->nb_eq;
  int nb_columns= dimensions;
  int i,rank, rank_prime;

  itv_t** m = NULL;
  ja_eq_set_t* mid_eqs = NULL;
  ja_eq_set_t* res = NULL ;

  /* 0 : create the matrix m  with N+1 rows and P columns */
  m=malloc(nb_rows * sizeof(itv_t*));
  for (i=0;i<nb_rows;i++) {
    m[i]=malloc(nb_columns*sizeof(itv_t));
  }
  printf("m is allocated\n");

  printf("eqs:\n");
  print_equation_set(stdout,eqs);
  printf("\n");

  /* 1: call the function [eq_set_B_to_matrix] to initialise matrix m */
  eq_set_B_to_matrix(eqs,nb_rows,nb_columns,m);

  printf("result of eq_set_B_to_matrix:\n");
  matrix_fdump(stdout,nb_rows,nb_columns,m);
  printf("******************************\n");

  /* 2: transform m with the function [matrix_jordan_reduction] ->(rank) */ 
  rank = matrix_jordan_reduction(pr->itv, nb_rows, nb_columns,m);
  printf("result of matrix_jordan_reduction has %d nonzero rows:\n",rank);
  matrix_fdump(stdout,nb_rows,nb_columns,m);
  printf("******************************\n");

  /* 3: call the function [matrix_to_eq_set_Aprime] on m and the input equation*/
  mid_eqs= matrix_to_eq_set_Aprime(pr,rank,nb_rows,nb_columns, m, eqs);
  printf("result of matrix_to_eq_set_Aprime :\n");
  print_equation_set(stdout,mid_eqs);
  printf("\n");
  printf("******************************\n");

   /* 4: re-create m with (P-rank) rows and N+P+1 columns */
  /* free matrix  m */
  for (i=0;i<nb_rows;i++) {
    free(m[i]);
  }
  free(m);

  /*  change the numbers of rows and columns */
  assert(mid_eqs->nb_eq == dimensions - rank);
  nb_columns = nb_columns + nb_rows ; // P+ (N+1) columns
  nb_rows = mid_eqs->nb_eq; // P-rank 

  /* reallocate m */
  m=malloc(nb_rows * sizeof(itv_t*));
  for (i=0;i<nb_rows;i++) {
    m[i]=malloc(nb_columns*sizeof(itv_t));
  }
  printf("m is allocated again \n");

   /* 5: call the function [eq_set_Aprime_to_matrix] to re-initialise matrix m */
  eq_set_Aprime_to_matrix(mid_eqs,dimensions,nb_columns-dimensions-1,m);

  printf("result of eq_set_Aprime_to_matrix:\n");
  matrix_fdump(stdout,nb_rows,nb_columns,m);
  printf("******************************\n");


   /* 6: transform m with the function [matrix_jordan_reduction] ->(P-rank) */
  rank_prime = matrix_jordan_reduction(pr->itv, nb_rows, nb_columns,m);
  assert (rank_prime == dimensions-rank);

  printf("result of matrix_jordan_reduction has %d nonzero rows:\n",rank_prime);
  matrix_fdump(stdout,nb_rows,nb_columns,m);
  printf("******************************\n");

   /* 7: call the function [matrix_to_eq_set_A] on m */
  res = matrix_to_eq_set_A (pr, nb_rows, dimensions, nb_columns-dimensions-1, m);
  printf("result of matrix_to_eq_set_A :\n");
  print_equation_set(stdout,res);
  printf("\n");
  printf("******************************\n");

  /*  8: cleanup and return result */
  /* free mid_eqs */
  free_equation_set(mid_eqs);

  /*  free m */
  for (i=0;i<nb_rows;i++) {
    free(m[i]);
  }
  free(m);

  /* return */
  return res;

}


/* ********************************************************************** */
/* 1. Rebuild an abstract value */
/* ********************************************************************** */

void equation_to_aff_set (t1p_internal_t* pr, t1p_t* abstract_value, ja_eq_t* equation)
{
  CALL();
  /* initialization */
  t1p_aff_t* a=malloc(sizeof(t1p_aff_t));
  t1p_aff_t* buff1=malloc(sizeof(t1p_aff_t));
  t1p_aff_t* buff2=malloc(sizeof(t1p_aff_t));
  ja_term_t* current_term;

  /* print_equation(stdout,equation); */
  /* printf("*\n"); */

  t1p_aff_init(pr,a);
  /* we set the center*/ 
  itv_set (a->c,equation->c);
  /* printf("[a] before first while loop:\n"); */
  /* t1p_aff_fprint(pr,stdout,a); */
  /* printf("\n"); */
  /* loop on the equation to create an affine form from terms in beta (noise symbols) */
  current_term= equation->first_te;
  while (current_term != NULL)
    {
      if (current_term->t_coeff == NS)
	{
	  /* if it is a noise symbol, add it.*/
	  assert(current_term->pnsym != NULL);
	  t1p_aff_nsym_add(pr,a,current_term->coeff,current_term->pnsym);
	}
      /* if it is not a noise symbol, do nothing */
      current_term = current_term->n;
    }
  /* printf("[a] after first while loop:\n"); */
  /* t1p_aff_fprint(pr,stdout,a); */
  /* printf("\n"); */
  /* loop on the equation to create an affine form from terms in beta (noise symbols) */
  current_term= equation->first_te;

  t1p_aff_init(pr,buff1);
  t1p_aff_init(pr,buff2);
  /* printf("[a] after buffer init:\n"); */
  /* t1p_aff_fprint(pr,stdout,a); */
  /* printf("\n"); */
  while (current_term != NULL)
    {
      //printf(".\n");
      //t1p_aff_fprint(pr,stdout,a);
      //printf("\n");
      if (current_term->t_coeff == VA)
	{
	  /* if it is a program variable.*/
	  /* clear the buffers */
	  t1p_aff_clear(pr,buff1);
	  t1p_aff_clear(pr,buff2);
	  /* search for the affine form of equation->pdim and copy it in buff1 */
	  /* printf("dim of current_term: %d\n",*current_term->pdim ); */
	  /* printf("corresponding affine form:\n"); */
	  /* t1p_aff_fprint(pr,stdout,abstract_value->paf[*current_term->pdim]); */
	  /* printf("\n"); */
	  buff1 = t1p_aff_copy(pr,abstract_value->paf[current_term->dim]);
	  /* printf("buff1 after copy:\n"); */
	  /* t1p_aff_fprint(pr,stdout,buff1); */
	  /* printf("\n"); */
	  /* then multiply this affine form by the coefficient of the equation */
	  t1p_aff_mul_scalar(pr,buff1,current_term->coeff);
	  /* printf("mult:"); */
	  /* itv_fprint(stdout,current_term->coeff); */
	  /* printf("\n"); */
	  /* printf("buff1 after mult:\n"); */
	  /* t1p_aff_fprint(pr,stdout,buff1); */
	  /* printf("\n"); */
	  /* printf("a:\n"); */
	  /* t1p_aff_fprint(pr,stdout,a); */
	  /* printf("\n"); */
	  /* addition of the result with a, using buff2 */
	  t1p_aff_add_aff(pr,buff2,a,buff1);
	  /* printf("buff2 after add:\n"); */
	  /* t1p_aff_fprint(pr,stdout,buff2); */
	  /* printf("\n"); */
	  /* set a to the computed affine form */
	  t1p_aff_clear(pr,a);
	  a=t1p_aff_copy(pr,buff2);
	  /* printf("a:\n"); */
	  /* t1p_aff_fprint(pr,stdout,a); */
	  /* printf("\n"); */
	}
      /* if it is a noise symbol, do nothing */
      current_term = current_term->n;
    }
  //printf("*\n");
  /* free the buffers */
  t1p_aff_free(pr,buff1);
  t1p_aff_free(pr,buff2);

  itv_t new_itv; // updates the itv concretization of [a]
  itv_t itv_buff; 
  itv_init(itv_buff);
  itv_init(new_itv);
  itv_set(new_itv,a->c); // start with the center

  t1p_aaterm_t* current_aaterm = a->q;
  while (current_aaterm != NULL)
    {
      itv_clear(itv_buff);
      // get the concretization of current_aaterm->pnsym
      t1p_nsymcons_get_gamma(pr,itv_buff,current_aaterm->pnsym->index,abstract_value);
      //multiplication by the coefficient of this term
      itv_mul(pr->itv,itv_buff,itv_buff,current_aaterm->coeff);
      itv_add(new_itv,new_itv,itv_buff);
      current_aaterm=current_aaterm->n;
    }
  /* now, new_itv contains the best concretization of [a] */
  itv_set(a->itv,new_itv);

  /* adds [a] to the affine set */
  /* printf("dim=%d\n",equation->dim); */
  /* printf("a:\n"); */
  /* t1p_aff_fprint(pr,stdout,a); */
  /* printf("\n"); */
  (abstract_value->paf)[equation->dim]=a;
  a->pby++;
  //printf("abstract value:\n");
  //t1p_aff_fprint(pr,stdout,(abstract_value->paf)[equation->dim]);
  //printf("\n");
}

void rebuild_abstract_value(ap_manager_t* man, t1p_t* a, ja_eq_set_t* eqs)
{
  CALL();
  t1p_internal_t* pr = man->internal;
  ja_eq_list_elm* cell=eqs->first_eq;
  while (cell != NULL)
    {
      equation_to_aff_set(pr,a,cell->content);
      cell=cell->n;
    }

}

/* ********************************************************************** */
/* 2. Search for linear relations */
/* ********************************************************************** */



/* ********************************************************************** */
/* 3. Alternative to the join operator */
/* ********************************************************************** */
t1p_t* t1p_join_alt(ap_manager_t* man, bool destructive, t1p_t* a1, t1p_t* a2)
{
  /* TO DO */
  CALL();
  printf("TO BE DONE \n");
  return a1;
}
