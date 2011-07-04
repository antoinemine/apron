/* ********************************************************************** */
/* t1p_representation.c: general management */
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
ar_eq_t* new_equation (void)
{
  CALL();
  ar_eq_t* res =malloc(sizeof(ar_eq_t));
  res->pdim=NULL;
  itv_init(res->c);
  res->first_te=NULL;
  res->last_te=NULL;
  return res;
}

void free_equation (ar_eq_t* equation)
{
  CALL();
  ar_term_t* cell1= equation->first_te;
  ar_term_t* cell2;
  while (cell1 != NULL)
    {
      cell2=cell1;
      cell1=cell1->n;
      free(cell2);
    }
  free(equation);
}

void print_equation (FILE* stream, ar_eq_t* equation)
{
  CALL();
  fprintf(stream,"x%d=",(int) *equation->pdim);
  itv_fprint(stream, equation->c);
  ar_term_t* cell= equation->first_te;

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
	fprintf(stream,".x%d",(int) *cell->pdim);
      cell=cell->n;
    }
   fprintf(stream,"\n");
}


void add_equation_term_ns (ar_eq_t* equation, itv_t c, t1p_nsym_t* pnsym)
{
  CALL();
  ar_term_t* cell=malloc(sizeof(ar_term_t));
  cell->n=NULL;
  cell->t_coeff=NS;
  cell->pdim=NULL;
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

ar_eq_t* copy_equation (ar_eq_t* equation)
{
  CALL ();
  printf("copy equation not implemented");
  return NULL;
}

void add_equation_term_va (ar_eq_t* equation, itv_t c, ap_dim_t* pdim)
{
  CALL();
  ar_term_t* cell=malloc(sizeof(ar_term_t));
  cell->n=NULL;
  cell->t_coeff=VA;
  cell->pdim=pdim;
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
  


ar_set_eq_t* new_equation_set (void)
{
  CALL();
  ar_set_eq_t* res=malloc(sizeof(ar_set_eq_t));
  res->nb_eq= 0;
  res->first_eq=NULL;
  res->last_eq=NULL;
  return res;
}

void free_equation_set (ar_set_eq_t* set_eq)
{
  CALL();
  ar_eq_list_elm* cell1= set_eq->first_eq;
  ar_eq_list_elm* cell2;
  while (cell1 != NULL)
    {
      cell2=cell1;
      cell1=cell1->n;
      free(cell2);
    }
  free(set_eq);
}

void print_equation_set(FILE* stream, ar_set_eq_t* eqs)
{
  CALL();
  ar_eq_list_elm* cell= eqs->first_eq;
  while (cell != NULL)
    {
      print_equation(stream,cell->content);
      cell=cell->n;
    }
}


void add_equation (ar_set_eq_t* eqs, ar_eq_t* eq)
{
  CALL();
  /* creation of the cell of the list */
  ar_eq_list_elm* cell=malloc(sizeof(ar_eq_list_elm));
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
      ar_eq_list_elm* last = eqs->last_eq;
      last->n=cell;
      eqs->last_eq=cell;
    }
}


itv_t* get_coeff_var (ar_set_eq_t* eq_set, int eq_n, ap_dim_t v)
{
  CALL();
  assert (eq_n < eq_set->nb_eq); 
  /* find the right equation */
  ar_eq_list_elm* current_elm = eq_set->first_eq;
  for (int i=0;i<eq_n;i++)
    {
      current_elm = current_elm->n;
    }
  /* found */
  /* find the right coefficient */
  ar_term_t* current_term = (current_elm->content)->first_te;
  while (*current_term->pdim != v)
    {
      current_term=current_term->n;
    }
  /* found */
  return &(current_term->coeff);
}


itv_t* get_coeff_nsym (ar_set_eq_t* eq_set, int eq_n, t1p_nsym_t v)
{
  CALL();
   assert (eq_n < eq_set->nb_eq); 
  /* find the right equation */
  ar_eq_list_elm* current_elm = eq_set->first_eq;
  for (int i=0;i<eq_n;i++)
    {
      current_elm = current_elm->n;
    }
  /* found */
  /* find the right coefficient */
  ar_term_t* current_term = (current_elm->content)->first_te;
  while ((current_term->pnsym)->index != v.index)
    {
      current_term=current_term->n;
    }
  /* found */
  return &(current_term->coeff);
}



/* ********************************************************************** */
/* 1. From 2 abstract values to equations */
/* ********************************************************************** */

ar_set_eq_t* abstract_value_to_eq_set (t1p_internal_t* pr, t1p_t* a1,  t1p_t* a2)
{
  CALL();
  ar_set_eq_t* res = new_equation_set();
  size_t dims = a1->dims;
  assert (dims == a2->dims); // to be sure the abstract values have the same dims
  int i;
  int j;
  itv_t* pitv1=NULL;
  itv_t* pitv2=NULL;
  int nb_nsym=0; // number of noise symbols in a1 and a2: we look at the last symbol of each affine form
  for (i=0 ; i< (int) dims;i++)
    {
      if (a1->paf[i]->end != NULL)
	{
	  if (a1->paf[i]->end->pnsym->index > nb_nsym)
	    nb_nsym= a1->paf[i]->end->pnsym->index;
	}
      if (a2->paf[i]->end != NULL)
	{ 
	  if(a2->paf[i]->end->pnsym->index > nb_nsym)
	    nb_nsym= a2->paf[i]->end->pnsym->index;
	}
    }
 
  /* nb_nsym is the last index present, we add 1 to have the number of noise symbols*/
  nb_nsym++;

  //printf("nb_nsym=%zu \n",nb_nsym);

  ap_dim_t tdim[nb_nsym+1];
  for (i=0;i<nb_nsym+1;i++)
    {
      tdim[i]=(ap_dim_t) i-1; 
      /* the i+1 position of tdim contains the number (dimension) i
	 position 0 contains (size_t -1) with is the maximal dimension of apron 
	 may be a source of bugs if there are so many dimentions
       */
    }
  itv_t itv_buff;
  ar_eq_t* equation;
  

  /* special loop for the constant terms */
  equation=new_equation();
  equation->pdim=tdim; //pointer to the first position of tdim
  itv_init(itv_buff);

  for (j=0; j<dims ; j++)
    {
      /* printf("dim %d: itv1",j); */
      /* itv_print((a1->paf[j])->c); */
      /* printf("  itv2"); */
      /* itv_print((a2->paf[j])->c); */
      itv_sub(itv_buff,(a1->paf[j])->c,(a2->paf[j])->c);
      /* printf("\n itv1-itv2 ="); */
      /* itv_print(itv_buff); */
      /* printf("\n"); */
      add_equation_term_va (equation, itv_buff, tdim+j+1);
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
      equation->pdim=tdim+i+1;//pointer to the i+1th position of tdim
      itv_clear(itv_buff);

      for (j=0; j<dims ; j++)
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
		  /* printf("add "); */
		  /* itv_print(itv_buff); */
		  /* printf(" eps%d\n",j); */
		 
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
	      else
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

	      /* if itv_buff is not zero, add the term */
	      if (! itv_is_zero(itv_buff))
		{
		  add_equation_term_va (equation, itv_buff, tdim+j+1);
		}
	    }
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


/* matrix_swap - swap rows i and j of matrix m */

void matrix_swap(itv_t** m, int i, int j) {
    itv_t* temp;
    temp=m[i];
    m[i]=m[j];
    m[j]=temp;
}

/* m[i] <- m[i] + lambda * m[j], length is the number of columns */ 
void matrix_lin_aff(itv_internal_t* intern, int length, itv_t** m, int i, itv_t lambda, int j) {
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
/* NB: it assumes matrix m has nb_rows rows and nb_columns columns */

int matrix_jordan_reduction (itv_internal_t* intern, int nb_rows, int nb_columns, itv_t**m) {
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
    if (l>current_row)
      matrix_swap(m,current_row,l);
    // now, for al i>current_row, pivot(i) >= pivot(current_row)

    /* if j = nb_columns, it means that all rows are with zero coefficient */
    if (j== nb_columns) {
      return current_row-1;
      // stop the loop and returns the "rank"
    }
    else {
      itv_set(pivot,m[current_row][j]); // set the pivot
      for(i=current_row+1; i<nb_rows;i++) {
	itv_div(intern,lambda,m[i][j],pivot);
	itv_neg(lambda,lambda); // lambda = - m[i][j] / pivot
	/*  m[i] <- m[i] + lambda m[current_row]*/ 
	matrix_lin_aff(intern,nb_columns,m,i,lambda,current_row); 
	assert (itv_is_zero(m[i][j])); // check that the new coefficient is zero
	itv_clear(lambda);
      }
      itv_clear(pivot);
    }
  }

}

/* ********************************************************************** */
/* 1. Rebuild an abstract value */
/* ********************************************************************** */

void equation_to_aff_set (t1p_internal_t* pr, t1p_t* abstract_value, ar_eq_t* equation)
{
  CALL();
  /* initialization */
  t1p_aff_t* a=malloc(sizeof(t1p_aff_t));
  t1p_aff_t* buff1=malloc(sizeof(t1p_aff_t));
  t1p_aff_t* buff2=malloc(sizeof(t1p_aff_t));
  ar_term_t* current_term;

  /* print_equation(stdout,equation); */
  /* printf("*\n"); */

  t1p_aff_init(pr,a);
  /* we set the center*/ 
  itv_join (a->c,a->c,equation->c);
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
	  assert(current_term->pdim != NULL);
	  /* clear the buffers */
	  t1p_aff_clear(pr,buff1);
	  t1p_aff_clear(pr,buff2);
	  /* search for the affine form of equation->pdim and copy it in buff1 */
	  /* printf("dim of current_term: %d\n",*current_term->pdim ); */
	  /* printf("corresponding affine form:\n"); */
	  /* t1p_aff_fprint(pr,stdout,abstract_value->paf[*current_term->pdim]); */
	  /* printf("\n"); */
	  buff1 = t1p_aff_copy(pr,abstract_value->paf[*current_term->pdim]);
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
  /* printf("dim=%d\n",*(equation->pdim)); */
  /* printf("a:\n"); */
  /* t1p_aff_fprint(pr,stdout,a); */
  /* printf("\n"); */
  (abstract_value->paf)[*(equation->pdim)]=a;
  a->pby++;
  //printf("abstract value:\n");
  //t1p_aff_fprint(pr,stdout,(abstract_value->paf)[*(equation->pdim)]);
  //printf("\n");
}

void rebuild_abstract_value(ap_manager_t* man, bool destructive, t1p_t* a, ar_set_eq_t* eqs)
{
  CALL();
  t1p_internal_t* pr = man->internal;
  ar_eq_list_elm* cell=eqs->first_eq;
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
