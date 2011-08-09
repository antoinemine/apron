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
/* 0. Structures */
/* ********************************************************************** */
typedef enum ja_coeff_t {
    VA,		/* Variable of a program */
    NS,		/* Union noise symbol */
} ja_coeff_t;

typedef struct ja_term_t {
  struct ja_term_t*	n;	/* next element */
  ja_coeff_t            t_coeff; /*  type of coefficient */
  ap_dim_t     	        dim;	/* index of the program variable, if t_Coeff=VA */
  t1p_nsym_t*          	pnsym;	/* index of the noise symbol, if t_Coeff=NS */
  itv_t 	       	coeff;	/* coeff, encoded as interval */
} ja_term_t;

/* equation of the form dim = c + [value of the list or ja_terms]*/
typedef struct ja_eq_t {
 
  ap_dim_t     dim;	/* index of the program variable, xp */
  itv_t		c;	/* center */
  ja_term_t*	first_te;	/* first term */
  ja_term_t*	last_te;	/* quick jump to the term : to add a new term for instance */
} ja_eq_t;

/* list of equation */
typedef struct ja_eq_list_elm {
 struct ja_eq_list_elm* n;   /* next element of the list */
  struct ja_eq_t* content;   /* equation */
} ja_eq_list_elm;


/* assumption: the equation's order is the rebuild order: in other word the first equation should not include variables defined by the following equations. The second equation can only include the first vrariable and other variables not defined by equations, etc */
typedef struct ja_eq_set_t {
  int	                nb_eq;	        /* number_of_equations */
  ja_eq_list_elm*	first_eq;	/* first equation */
  ja_eq_list_elm*	last_eq;	/* last equation */
} ja_eq_set_t;


/*
 * IMPORTANT NOTE: the type ja_eq_set_t is used in four different contexts: 
 *
 * - context A: equations of the form 
 *      x_i = gamma+alpha_1.x_1+...+alpha_p+beta_0.eps_O+...+beta_n.eps_n
 *   in this contex, alpha_*, beta_* and gamma are constants
 *
 * - context Aprime: equations of the form 
 *      0 = gamma+alpha_1.x_1+...+alpha_p+beta_0.eps_O+...+beta_n.eps_n
 *   in this contex, alpha_*, beta_* and gamma are constants
 *
 * - context B: equations of the form 
 *      beta_i = 0+c_{i,1}.alpha_1+...+c_{i,p}.alpha_p
 *   in this context, alpha_* and beta_i are the unknown variables
 *
 * - context Bprime: equations of the form 
 *      0 = 0+c_{i,1}.alpha_1+...+c_{i,p}.alpha_p
 *   in this context, alpha_* are the unknown variables 


 * In this .h (and the .c), "eqs is of type X" means that eqs' context is X
 * DO NOT USE an equation set of type X in another context
 */



/* ********************************************************************** */
/* 0. Constructors */
/* ********************************************************************** */
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


/* ********************************************************************** */
/* 1. Rebuild an abstract value */
/* ********************************************************************** */

/* eqs is of type A */
/* a should not be top or bottom, and the variables to be rebuild should be "t1p_aff_top" at the beginning */

void rebuild_abstract_value(ap_manager_t* man, t1p_t* a, ja_eq_set_t* eqs);


/* ********************************************************************** */
/* 2. Search for linear relations */
/* ********************************************************************** */


/* if one of the variable is "t1p_aff_top", the set of equation is empty */ 
/* result is of type B */
ja_eq_set_t* abstract_value_to_eq_set (t1p_internal_t* pr, t1p_t* a);

/* result is of type B prime */
ja_eq_set_t* two_abstract_values_to_eq_set (t1p_internal_t* pr, t1p_t* a1, t1p_t* a2);


/* eqs is of type B, eqs_prime is of type B prime */
/* result is of type A */
ja_eq_set_t* eq_set_transformation (t1p_internal_t* pr, ja_eq_set_t* eqs,  ja_eq_set_t* eqs_prime, int dimensions);

/* ********************************************************************** */
/* 3. Alternative to the join operator */
/* ********************************************************************** */
t1p_t* t1p_join_alt(ap_manager_t* man, bool destructive, t1p_t* a1, t1p_t* a2);


#endif
