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
#include "ap_manager.h"
#include "ap_lincons0.h"
#include "t1p_internal.h"
#include "t1p_representation.h"
#include "t1p_constructor.h"
#include "t1p_join_std.h"
#include "t1p_otherops.h"
#include "t1p_join_alt.h"



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
      //itv_clear(cell2->coeff);
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
  if (! itv_is_zero(c)) {
  ja_term_t* cell=malloc(sizeof(ja_term_t));
  cell->n=NULL;
  cell->t_coeff=NS;
  cell->dim=0;
  cell->pnsym=pnsym;
  itv_init(cell->coeff);
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
  if (!itv_is_zero(c)) 
    /* check that the coefficient is not null */
    {

  ja_term_t* cell=malloc(sizeof(ja_term_t));
  cell->n=NULL;
  cell->t_coeff=VA;
  cell->dim=dim;
  cell->pnsym=NULL;
  itv_init(cell->coeff);
  itv_set(cell->coeff,c);
  
  if (equation->last_te==NULL) // equation is empty
    {
      equation->first_te=cell;
      equation->last_te=cell;
    }
  else // equarion is not empty
    {
      assert (equation->last_te->n == NULL);
      equation->last_te->n=cell;
      equation->last_te=cell;
    }
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
  cell->n=NULL;
  cell->content=eq;

  if (eqs->nb_eq==0)
    {
      assert(eqs->first_eq == NULL);
      assert(eqs->last_eq == NULL);
      eqs->nb_eq=1;
      eqs->first_eq= cell;
      eqs->last_eq= cell;
    }
  else
    { assert(eqs->last_eq && eqs->last_eq->n == NULL); 
      eqs->nb_eq++;
      eqs->last_eq->n=cell;
      eqs->last_eq=cell;
    }
}

/* returns a pointer to the interval of variabble j in the affine form i. Returns null if there is not this variable in the affine form */
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
  while (current_term && (current_term->t_coeff==NS ||current_term->dim != v))
    {
      current_term=current_term->n;
    }
  if (current_term)
  /* found */
      return &(current_term->coeff);
  else
    /* not found */
    return NULL;
}

/* returns a pointer to the interval of noise_symbol j in the affine form i. Returns null if there is not this noise symbol in the affine form */
itv_t* get_coeff_nsym (ja_eq_set_t* eqs, int eq_n, size_t v)
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
  ja_term_t* current_term = current_elm->content->first_te;
  while (current_term &&(current_term->t_coeff==VA || current_term->pnsym->index != v))
    {
      current_term=current_term->n;
    }
  if (current_term)
    /* found */
    return &(current_term->coeff);
  else
    /*  not_found */
    return NULL;
}



/* ********************************************************************** */
/* 1. From 2 abstract values to equations */
/* ********************************************************************** */

/* result is of type B */
ja_eq_set_t* abstract_value_to_eq_set (t1p_internal_t* pr, t1p_t* a)
{
  CALL();
  ja_eq_set_t* res = new_equation_set();
  size_t dims = a->dims;
  int i;
  int j;
  itv_t* pitv=NULL;
  itv_t buff; // needed to negate all the coefficients
  ja_eq_t* equation=NULL;
  int nb_nsym; // number of noise symbols in [a]

  itv_init(buff);

  nb_nsym =-1;
  for (i=0 ; i< (int) dims;i++)
    {
      if (a->paf[i]->end != NULL)
	{
	  if ((int) a->paf[i]->end->pnsym->index > nb_nsym)
	    nb_nsym= a->paf[i]->end->pnsym->index;
	}
    }
 
  /* nb_nsym is the last index present, we add 1 to have the number of noise symbols*/
  nb_nsym++;

  /* special loop for the constant terms */
  equation=new_equation();
  equation->dim=(ap_dim_t) (-1);

  for (j=0; j<(int)dims ; j++)
    {
      itv_neg(buff,a->paf[j]->c);
      add_equation_term_va (equation,buff,(ap_dim_t) j);
    }
  add_equation(res,equation);

  /* 
     other loops
   */
  for (i=0;i<nb_nsym;i++)
    {
      equation=new_equation();
      equation->dim=(ap_dim_t)i;
   
      for (j=0; j<(int)dims ; j++)
	{
	  pitv=t1p_aff_get_coeff(pr,a->paf[j],i);
	  /* if itv_buff exists, add the term */
	  if (pitv!=NULL)
	    {
	      itv_neg(buff,*pitv);
	      add_equation_term_va (equation, buff,(ap_dim_t)j);
	    }
	}
      add_equation(res,equation);
    }
  itv_clear(buff);

  return res;
}


/* result is of type B prime */
ja_eq_set_t* two_abstract_values_to_eq_set (t1p_internal_t* pr, t1p_t* a1, t1p_t* a2)
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
  ja_eq_t* equation=NULL;
  int nb_nsym=0; // number of noise symbols in a1 and a2: we look at the last symbol of each affine form

  for (i=0 ; i< (int) dims;i++)
    {
      if (a1->paf[i]->end != NULL)
	{
	  if ((int) a1->paf[i]->end->pnsym->index > nb_nsym)
	    nb_nsym= a1->paf[i]->end->pnsym->index;
	}
      else {
	if (a2->paf[i]->end != NULL)
	  { 
	    if((int) a2->paf[i]->end->pnsym->index > nb_nsym)
	      nb_nsym= a2->paf[i]->end->pnsym->index;
	  }
	else 
/* no noise symbol is present, set to -1 to have the correct number of noise symbols after increment */
	  nb_nsym = -1;
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
#ifdef _T1P_DEBUG
      /* printf("dim %d: itv1",j); */
      /* itv_print((a1->paf[j])->c); */
      /* printf("  itv2"); */
      /* itv_print((a2->paf[j])->c); */
#endif
      itv_sub(itv_buff,(a1->paf[j])->c,(a2->paf[j])->c);
#ifdef _T1P_DEBUG
      /* printf("\n itv1-itv2 ="); */
      /* itv_print(itv_buff); */
      /* printf("\n"); */
#endif
      add_equation_term_va (equation, itv_buff,(ap_dim_t) j);
      //itv_clear(itv_buff);
    }
  add_equation(res,equation);

  /* 
     other loops
   */
  for (i=0;i<nb_nsym;i++)
    {
      equation=new_equation();
      /* printf("*\n"); */
      equation->dim=(ap_dim_t)i;
     
      //itv_clear(itv_buff);

      for (j=0; j<(int)dims ; j++)
	{
#ifdef _T1P_DEBUG
	  /* printf("j=%d\n",j); */
	  /* printf("a1[%d]",j); */
	  /* t1p_aff_fprint(pr,stdout,a1->paf[j]); */
	  /* printf("\n"); */
	  /* printf("a2[%d]",j); */
	  /* t1p_aff_fprint(pr,stdout,a2->paf[j]); */
	  /* printf("\n"); */
#endif
	  pitv1=t1p_aff_get_coeff(pr,a1->paf[j],i);
	  pitv2=t1p_aff_get_coeff(pr,a2->paf[j],i);

	  if (pitv1==NULL)
	    {
	      if (pitv2!=NULL)
		{
#ifdef _T1P_DEBUG
		  printf("pitv2 (eps %d):",i);
		  itv_print(*pitv2);
		  printf("\n");
#endif
		  itv_neg(itv_buff,*pitv2);
		 
		}
	      // else do nothing, ie set itv_buff to 0
	      else
		itv_set_int(itv_buff,0);
	    }
	  else
	    {
	      if (pitv2==NULL)
		{
#ifdef _T1P_DEBUG
		  /* printf("pitv1 (eps %d):",i); */
		  /* itv_print(*pitv1); */
		  /* printf("\n"); */
#endif
		  itv_set(itv_buff,*pitv1);
#ifdef _T1P_DEBUG
		  /* printf("add "); */
		  /* itv_print(itv_buff); */
		  /* printf(" eps%d\n",j); */
#endif
		}
	      else // both pointers are not null
		{
#ifdef _T1P_DEBUG
		  /* printf("pitv1 (eps %d):",i); */
		  /* itv_print(*pitv1); */
		  /* printf("\n"); */
		  /* printf("pitv2 (eps %d):",i); */
		  /* itv_print(*pitv2); */
		  /* printf("\n"); */
#endif
		  itv_sub(itv_buff,*pitv1,*pitv2);
#ifdef _T1P_DEBUG
		  /* printf("add "); */
		  /* itv_print(itv_buff); */
		  /* printf(" eps%d\n",j); */
#endif
		}
	    }
	  /* if itv_buff is not zero, add the term */
	  if (! itv_is_zero(itv_buff))
	    {
#ifdef _T1P_DEBUG
	      /* printf("add "); */
	      /* itv_print(itv_buff); */
	      /* printf(" eps%d\n",j); */
#endif
	      add_equation_term_va (equation, itv_buff,(ap_dim_t)j);
	    }
#ifdef _T1P_DEBUG
	  /* else */
	  /*   { */
	  /*     printf("coeff of eps%d is null\n ",j); */
	  /*   } */
	  
#endif
	  //itv_clear(itv_buff);
	}
      add_equation(res,equation);
    }
  
  /* printf("*\n"); */
  itv_clear(itv_buff);
  //free(itv_buff);
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

  /* initialisation of the matrix */  
  for (i=0;i<nb_rows;i++) {
    for (j=0; j<nb_columns; j++) {
      itv_init(m[i][j]);
    }
  }

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
/* matrix m must be allocated with eqs->nb_eq rows and dims+1+nb_nsym columns */

void eq_set_Aprime_to_matrix (ja_eq_set_t* eqs, int dims, int nb_nsym, itv_t** m) {
  int i,j;
  itv_t* pitv;
  int nb_rows= eqs->nb_eq;
  int nb_columns= dims + 1+ nb_nsym;
  ja_eq_list_elm* current_equation=eqs->first_eq;

  /* initialization of the matrix */
  for(i=0;i<nb_rows;i++){
    for(j=0;j<nb_columns;j++){
      itv_init(m[i][j]);
    }
  }

  for(i=0;i<nb_rows;i++){
    /* dims first columns */
    for (j=0;j<dims;j++){
      pitv=get_coeff_var (eqs, i, (ap_dim_t) j);
      if (pitv!=NULL){
	itv_set(m[i][j],*pitv);
      }
      else {
	itv_set_int(m[i][j],0);
      }
    }
    /* column dims */
    itv_set(m[i][dims],current_equation->content->c);

    /* other columns */
    for (j=0;j<nb_nsym;j++){
      pitv=get_coeff_nsym (eqs, i, (size_t) j);
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


/* matrix_swap - swap columns i and j of matrix m and register this change in tab_permut */

void matrix_swap_columns(itv_t** m, int* tab_permut, int i, int j, int nb_rows) {
  CALL();
  itv_t temp;
  int temp_v;
  int k;
  itv_init(temp);

  temp_v=tab_permut[i];
  tab_permut[i] = tab_permut[j];
  tab_permut[j]=temp_v;

  for (k=0;k<nb_rows;k++)
    {
      itv_set(temp,m[k][i]);
      itv_set(m[k][i],m[k][j]);
      itv_set(m[k][j],temp);
    }
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
  }
  itv_clear(temp);
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

    /* if j == nb_columns, it means that all rows are with zero coefficient */
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
	//itv_clear(lambda);
      }
      //itv_clear(pivot);
    }
  }
  itv_clear(lambda);
  //free(lambda);
  itv_clear(pivot);
  //free(pivot);
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
  int tab_permut [nb_columns]; // keep trace of column permutation
  itv_t buff_coeff, buff_sum; // buffers for interval
  ja_eq_t* equation = NULL;  // what will be added to the result
  ja_eq_list_elm* current_equation = NULL; // iterator on eqs
  ja_term_t* current_term = NULL; //sub iterator

  /* initialization */
  assert (rank <= nb_columns);
  for (i=0;i<nb_columns;i++) {
    itv_init(tab_alpha[i]);
    tab_permut[i]=i;
  }
  itv_init(buff_coeff);
  itv_init(buff_sum);
  if(eqs->nb_eq==0 && rank != 0){
    printf("ERROR: in function matrix_to_eq_set_Aprime, eqs must have at least one equation ");
    assert(false);
  }  

  /* permutation of the columns so that the matrix becomes  LU  with an identity diagonal */
  for (i=0;i<rank;i++) {
    j=matrix_choose_pivot(nb_columns,m,i);
    assert(i<=j);
    if (i<j) 
      {
	/* then do the permutation i<->j */
	matrix_swap_columns(m,tab_permut,i,j,nb_rows);
      }
    /* else do nothing */
  }
#ifdef _T1P_DEBUG
  printf("LU matrix\n");
  matrix_fdump(stdout,nb_rows,nb_columns,m);
  printf("tab_permut: [ ");
  for(i=0;i<nb_columns;i++)
    printf("%d ",tab_permut[i]);
  printf("]\n");
  printf("rank=%d\n",rank);
#endif
  /* main loop */
  for (k=rank;k<nb_columns;k++) {
    /* each value of k determine a value for the coefficients alpha_i (we take the canonical base for alpha_i with i>=rank), thus an independant equation. */


    /* (re)set tab_alpha  i>=rank */
    for (i=rank;i<nb_columns;i++) {
      if (i==k)
	itv_set_int(tab_alpha[tab_permut[i]],1);
      else
	itv_set_int(tab_alpha[tab_permut[i]],0);
    }
    

       /* (re)set tab_alpha  i<rank */
    for (i=rank-1;i>=0;i--) {
      /* computes the value of alpha[i] thanks to m */
      /* m[i] is a equation of the form sum(m[i][j] alpha[j]) = 0 */
      /* remember: m is LU */

      /* checks that m[i][i]=1 */
      itv_set_int(buff_sum,1);
      assert(itv_is_eq(buff_sum,m[i][i]));

      itv_set_int(buff_sum,0);
      for (j=i+1;j<nb_columns;j++) {

	itv_mul(pr->itv,buff_coeff,tab_alpha[tab_permut[j]],m[i][j]);
	itv_add(buff_sum,buff_sum,buff_coeff);
	//itv_clear(buff_coeff);
      }
      itv_neg(buff_sum,buff_sum);
      itv_set(tab_alpha[tab_permut[i]],buff_sum);
      //itv_clear(buff_sum);
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
      //itv_clear(buff_coeff);
      current_term = current_term->n;
    }
    /* set the center */
    //itv_neg(buff_sum,buff_sum);
    itv_set(equation->c,buff_sum);
    //itv_clear(buff_sum);
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
	//itv_clear(buff_coeff);
	current_term = current_term->n;
      }
      /* add the noise symbol eps_i */
      //itv_neg(buff_sum,buff_sum);
      add_equation_term_ns(equation,buff_sum,(pr->epsilon)[i]);
      //itv_clear(buff_sum);
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
  int i,j,k;
  itv_t buff; //to negate the coeeficient
  ja_eq_t* equation = NULL;  // what will be added to the result

  itv_init(buff);

  /* the equations are returned in the order of dependancy */
  for(i=nb_rows-1;i>=0;i--){
    equation=new_equation();
    k=matrix_choose_pivot(dims+nb_nsym+1,m,i); // k = first non null coefficient
    assert(k<dims);
    equation->dim=k;

    for (j=k+1;j<dims;j++){
      if (! itv_is_zero(m[i][j])) {
	  itv_neg(buff,m[i][j]);
	  add_equation_term_va(equation,buff,(ap_dim_t)j);
	}
    }
    itv_neg(equation->c,m[i][dims]);
    for (j=0;j<nb_nsym;j++){
      //itv_clear(buff);
      itv_neg(buff,m[i][j+dims+1]);
      add_equation_term_ns(equation,buff,(pr->epsilon)[j]);
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
ja_eq_set_t* eq_set_transformation (t1p_internal_t* pr, ja_eq_set_t* eqs,  ja_eq_set_t* eqs_prime, int dimensions) {
  CALL();
  int nb_rows = eqs_prime->nb_eq;
  int nb_columns= dimensions;
  int i,j,rank, rank_prime;

  itv_t** m = NULL;
  ja_eq_set_t* mid_eqs = NULL;
  ja_eq_set_t* res = NULL ;

#ifdef _T1P_DEBUG
  printf("eqs:\n");
  print_equation_set(stdout,eqs);
  printf("\n");
#endif

  /* 0 : create the matrix m  with N+1 rows and P columns */
  m=malloc(nb_rows * sizeof(itv_t*));
  for (i=0;i<nb_rows;i++) {
    m[i]=malloc(nb_columns*sizeof(itv_t));
  }
#ifdef _T1P_DEBUG
  printf("eqs_prime:\n");
  print_equation_set(stdout,eqs_prime);
  printf("\n");
#endif

  /* 1: call the function [eq_set_B_to_matrix] to initialise matrix m */
  eq_set_B_to_matrix(eqs_prime,nb_rows,nb_columns,m);
#ifdef _T1P_DEBUG
  printf("result of eq_set_B_to_matrix:\n");
  matrix_fdump(stdout,nb_rows,nb_columns,m);
  printf("******************************\n");
#endif

  /* 2: transform m with the function [matrix_jordan_reduction] ->(rank) */ 
  rank = matrix_jordan_reduction(pr->itv, nb_rows, nb_columns,m);
#ifdef _T1P_DEBUG
  printf("result of matrix_jordan_reduction has %d nonzero rows:\n",rank);
  matrix_fdump(stdout,nb_rows,nb_columns,m);
  printf("******************************\n");
#endif


  /* 3: call the function [matrix_to_eq_set_Aprime] on m and the input equation*/
  mid_eqs= matrix_to_eq_set_Aprime(pr,rank,nb_rows,nb_columns, m, eqs);
#ifdef _T1P_DEBUG
  printf("result of matrix_to_eq_set_Aprime :\n");
  print_equation_set(stdout,mid_eqs);
  printf("\n");
  printf("******************************\n");
#endif

   /* 4: re-create m with (P-rank) rows and N+P+1 columns */
  /* free matrix  m */
  for (i=0;i<nb_rows;i++) {
    for (j=0;j<nb_columns;j++) {
      itv_clear(m[i][j]);
      //free(m[i][j]);
    }
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

   /* 5: call the function [eq_set_Aprime_to_matrix] to re-initialise matrix m */
  eq_set_Aprime_to_matrix(mid_eqs,dimensions,nb_columns-dimensions-1,m);
#ifdef _T1P_DEBUG
  printf("result of eq_set_Aprime_to_matrix:\n");
  matrix_fdump(stdout,nb_rows,nb_columns,m);
  printf("******************************\n");
#endif

   /* 6: transform m with the function [matrix_jordan_reduction] ->(P-rank) */
  rank_prime = matrix_jordan_reduction(pr->itv, nb_rows, nb_columns,m);
  assert (rank_prime == dimensions-rank);
#ifdef _T1P_DEBUG
  printf("result of matrix_jordan_reduction has %d nonzero rows:\n",rank_prime);
  matrix_fdump(stdout,nb_rows,nb_columns,m);
  printf("******************************\n");
#endif

   /* 7: call the function [matrix_to_eq_set_A] on m */
  res = matrix_to_eq_set_A (pr, nb_rows, dimensions, nb_columns-dimensions-1, m);
#ifdef _T1P_DEBUG
  printf("result of matrix_to_eq_set_A :\n");
  print_equation_set(stdout,res);
  printf("\n");
  printf("******************************\n");
#endif

  /*  8: cleanup and return result */
  /* free mid_eqs */
  free_equation_set(mid_eqs);

  /*  free m */
  for (i=0;i<nb_rows;i++) {
    for (j=0;j<nb_columns;j++){
      itv_clear(m[i][j]);
      //free(m[i][j]);
    }
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

#ifdef _T1P_DEBUG
  print_equation(stdout,equation);
  printf("*\n");
#endif

  t1p_aff_init(pr,a);
  /* we set the center*/ 
  itv_set (a->c,equation->c);
#ifdef _T1P_DEBUG
  /* printf("[a] before first while loop:\n"); */
  /* t1p_aff_fprint(pr,stdout,a); */
  /* printf("\n"); */
#endif
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
#ifdef _T1P_DEBUG
  /* printf("[a] after first while loop:\n"); */
  /* t1p_aff_fprint(pr,stdout,a); */
  /* printf("\n"); */
#endif
  /* loop on the equation to create an affine form from terms in beta (noise symbols) */
  current_term= equation->first_te;

  t1p_aff_init(pr,buff1);
  t1p_aff_init(pr,buff2);
#ifdef _T1P_DEBUG
  /* printf("[a] after buffer init:\n"); */
  /* t1p_aff_fprint(pr,stdout,a); */
  /* printf("\n"); */
#endif
  while (current_term != NULL)
    {
#ifdef _T1P_DEBUG
      /* printf(".\n"); */
      /* t1p_aff_fprint(pr,stdout,a); */
      /* printf("\n"); */
#endif

      if (current_term->t_coeff == VA)
	{
	  /* if it is a program variable.*/
	  /* clear the buffers */
	  t1p_aff_free(pr,buff1);
	  t1p_aff_free(pr,buff2);
	  /* search for the affine form of equation->pdim and copy it in buff1 */
	  /* printf("dim of current_term: %d\n",current_term->dim ); */
	  /* printf("corresponding affine form:\n"); */
	  /* t1p_aff_fprint(pr,stdout,abstract_value->paf[current_term->dim]); */
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
	  t1p_aff_free(pr,a);
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
      //itv_clear(itv_buff);
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
  (abstract_value->paf)[equation->dim]->pby--;
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
/* 3. Alternative to the join operator */
/* ********************************************************************** */
t1p_t* t1p_join_alt(ap_manager_t* man, bool destructive, t1p_t* a1, t1p_t* a2)
{
  CALL();
  t1p_t *a1bis, *a2bis, *a_join, *res;
  ja_eq_set_t *eqs_a, *eqs_b, *eqs_b_prime;
  int nb_eq;
  int i; //iterator
  ja_eq_list_elm* current_equation; //idem

  assert(a1->dims==a2->dims && a1->intdim==a2->intdim);
  size_t realdim = a1->dims - a1->intdim;
  size_t intdim = a1->intdim;
#ifdef _T1P_DEBUG
  fprintf(stdout, "###JOIN OPERANDS (des:%d)(a1: %tx and a2: %tx) ###\n", destructive, (intptr_t)a1, (intptr_t)a2);
  t1p_fprint(stdout, man, a1, NULL);
  t1p_fprint(stdout, man, a2, NULL);
#endif

  if (t1p_is_eq(man, a1, a2)) {
	if (destructive) res = a1;
	else res = t1p_copy(man, a1);
    }
    else if (tbool_or(t1p_is_top(man, a1), t1p_is_top(man, a2)) == tbool_true) {
	if (destructive) {
	    t1p_free(man, a1);
	    res = a1 = t1p_top(man,intdim,realdim);
	} else {
	    res = t1p_top(man,intdim,realdim);
	}
    } else if (t1p_is_bottom(man, a1)) {
	if (destructive) {
	    t1p_free(man, a1);
	    res = a1 = t1p_copy(man,a2);
	} else {
	    res = t1p_copy(man,a2);
	}
    } else if (t1p_is_bottom(man, a2)) {
	if (destructive) res = a1;
	else res = t1p_copy(man,a1);
    } else {

      t1p_internal_t* pr = man->internal;
      size_t dimensions = a1->dims;

      assert (a2->dims == dimensions);

      /* check that both abstract values are finite */
      bool finite=true;
      i=0;
      while(finite&& (size_t)i<dimensions){
	assert(! t1p_aff_is_bottom(pr,a1->paf[i]));
	assert(! t1p_aff_is_bottom(pr,a2->paf[i]));
	finite= t1p_aff_is_bounded(pr,a1->paf[i]) &&   t1p_aff_is_bounded(pr,a2->paf[i]) && itv_is_bounded(a1->box[i]) && itv_is_bounded(a2->box[i]);
	i++;
      }
      /* printf("finite=%d\n",finite); */
      
      if (finite) {

      /* creation of the equations */
      eqs_b = abstract_value_to_eq_set (pr, a1);
      eqs_b_prime = two_abstract_values_to_eq_set (pr, a1, a2);

      eqs_a = eq_set_transformation(pr, eqs_b, eqs_b_prime, dimensions);
      current_equation =  eqs_a->first_eq;
      nb_eq = eqs_a->nb_eq;
      
      /* creation of the array of dimensions to forget */
      ap_dim_t tdim[nb_eq];
      for(i=0;i<nb_eq;i++) {
	tdim[i] = (ap_dim_t) current_equation->content->dim;
	/* print_equation(stdout,current_equation->content); */
	/* printf("\n erase variable %d \n", tdim[i]); */
	current_equation= current_equation->n;
      }
      
      /* forget and do the union */
      /* fprintf(stdout, "###PROJECTION OPERANDS (%tx and %tx) ###\n", (intptr_t)a1, (intptr_t)a2); */
      /* t1p_fprint(stdout, man, a1, NULL); */
      /* t1p_fprint(stdout, man, a2, NULL); */

      a1bis = t1p_forget_array(man, false, a1, tdim, (size_t) nb_eq, false);
      a2bis = t1p_forget_array(man, false, a2, tdim, (size_t) nb_eq, false);

     /* fprintf(stdout, "###PROJECTION RESULT (%tx and %tx) ###\n", (intptr_t)a1bis, (intptr_t)a2bis); */
     /*  t1p_fprint(stdout, man, a1bis, NULL); */
     /*  t1p_fprint(stdout, man, a2bis, NULL); */
      /* creation of the result */
      if (destructive) {
	t1p_free(man, a1);
	res = a1 = t1p_join_std(man, false, a1bis, a2bis); 
      }
      else
	res = t1p_join_std(man, false, a1bis, a2bis); 


      /* rebuild */
      rebuild_abstract_value(man, res, eqs_a);
#ifdef _T1P_DEBUG
      printf("Result of JOIN ALT:\n");
      t1p_fdump(stdout,man,res);
#endif
      /* cleanup */
      /* t1p_fdump(stdout,man,a1bis); */
      /* t1p_fdump(stdout,man,a2bis); */
      
      t1p_free(man,a1bis);
      t1p_free(man,a2bis);
 

      free_equation_set(eqs_a);
      free_equation_set(eqs_b);
      free_equation_set(eqs_b_prime);
      }



      else /* one of the two abstract value is infiniete -> classical join */
	res = t1p_join_std(man, destructive, a1, a2); 
    }
  return res;
}
