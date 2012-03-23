#ifndef _T1P_JOINALT_H_
#define _T1P_JOINALT_H_ 


/*
#include "t1p_representation.h"
#include "ap_manager.h"
#include "ap_tcons0.h"
#include "ap_lincons0.h"
#include "ap_generator0.h"
#include "ap_generic.h"
*/



/* ********************************************************************** */
/* 2. Constructors */
/* ********************************************************************** */


/* ** Equations  ** */
ja_eq_t* new_equation (void);
void free_equation (ja_eq_t* equation);
void print_equation (FILE* stream,ja_eq_t* equation);
void add_equation_term_ns (ja_eq_t* equation, itv_t c, t1p_nsym_t* pnsym);
void add_equation_term_va (ja_eq_t* equation, itv_t c, ap_dim_t dim);


/* eqs may be of any type for the following functions */
ja_eq_set_t* new_equation_set (void);
/* NB: free_equation_set calls free_equation () on all its equations */
void free_equation_set (ja_eq_set_t* eq_set);
void print_equation_set (FILE* stream,ja_eq_set_t* eqs);
/* NB: DO NOT add an equation of type X in an equation set of type Y */
void add_equation (ja_eq_set_t* eqs, ja_eq_t* eq);

/* the two following functions return a pointer to the coefficient associated 
 * to var/nsym of index v in the equation (index eq_n) of eqs. 
 * Index eq_n ranges from 0 to eqs->nb_eq -1 */
itv_t* get_coeff_var (ja_eq_set_t* eqs, int eq_n, ap_dim_t v);
/* NB: eqs of type B have no nsym terms */
itv_t* get_coeff_nsym (ja_eq_set_t* eqs, int eq_n, size_t v);



/* **list of indexed noise symbols** */
typedef struct ja_nsym_list_elm {
  struct ja_nsym_list_elm*	n;	/* next element */
  int                   indice; /*  indice of the noise symbol */
  t1p_nsym_t*          	pnsym;	/* index of the noise symbol */
} ja_nsym_list_elm;

typedef struct ja_nsym_list_t {
  int	                nb;	/* size of the list */
  ja_nsym_list_elm*	first;	/* first cell */
  ja_nsym_list_elm*	last;	/* last cell */
} ja_nsym_list_t;

/* assumptions on those lists: each symbol is present at most once, all indice are different */

ja_nsym_list_t* new_nsym_list (void);
void free_nsym_list (ja_nsym_list_t* list);
void fprint_nsym_list (FILE* stream,ja_nsym_list_t* list);
/* add psym to the list (if it is not present) and returns its indice */
int add_nsym_to_list (ja_nsym_list_t* list, t1p_nsym_t* pnsym);
/* returns the indice of pnsym in list. returns -1 if not present */
int nsym_to_indice (ja_nsym_list_t* list, t1p_nsym_t* pnsym);
/* returns a pointer to the noise symbol in list. returns NULL if not present */
t1p_nsym_t* indice_to_nsym (ja_nsym_list_t* list, int indice);
/* get the indice of noise symbol eps_i */
int indice_to_nsym_index (ja_nsym_list_t* list, int indice);
/* permutes indice i and j in the list. Assert fails if i or j are not present */
void permute_indice (ja_nsym_list_t* list, int i, int j);



/* ********************************************************************** */
/* 1. Rebuild an abstract value */
/* ********************************************************************** */

/* eqs is of type A */
/* a should not be top or bottom, and the variables to be rebuild should be "t1p_aff_top" at the beginning */

void rebuild_abstract_value(ap_manager_t* man, t1p_t* a, ja_eq_set_t* eqs, bound_t* perturbation_ranges_array);


/* ********************************************************************** */
/* 2. Search for linear relations */
/* ********************************************************************** */


/* if one of the variable is "t1p_aff_top", the set of equation is empty */ 
/* result is of type B */
ja_eq_set_t* abstract_value_to_eq_set (t1p_internal_t* pr, ja_nsym_list_t* nsym_list, t1p_t* a);

/* result is of type B prime */
ja_eq_set_t* two_abstract_values_to_eq_set (t1p_internal_t* pr, ja_nsym_list_t* nsym_list, t1p_t* a1, t1p_t* a2);

/* list the noise symbols present in a1 or a2 */
ja_nsym_list_t* two_abstract_values_to_nsym_list  (t1p_t* a1, t1p_t* a2);

/* eqs is of type B, eqs_prime is of type B prime */
/* result is of type A */
ja_eq_set_t* eq_set_transformation (t1p_internal_t* pr, ja_nsym_list_t* nsym_list, ja_eq_set_t* eqs,  ja_eq_set_t* eqs_prime, int dimensions);


/* eqs is of type A  */
/* the rsult is an array of intervals (one for each equation of eqs) that represents the perturbation  */
bound_t* adapt_eq_set_to_abstract_values(t1p_internal_t *pr, t1p_t *a1,t1p_t *a2,ja_eq_set_t *eqs);




/* ********************************************************************** */
/* 3. Alternative to the join operator */
/* ********************************************************************** */

/* eqs is of type A. It represents the exact equation. if eqs==NULL,
   the function calls two_abstrac_valueto_eq_set and
   eq_set_transformation to find an equation set */

t1p_t* t1p_join_alt(ap_manager_t* man, bool destructive, t1p_t* a1, t1p_t* a2, ja_eq_set_t* eqs);


#endif
