#ifndef _T1P_JOIN_ALT_H_
#define _T1P_JOIN_ALT_H_ 


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
  ap_dim_t*      	pdim;	/* index of the program variable, if t_Coeff=VA */
  t1p_nsym_t*          	pnsym;	/* index of the noise symbol, if t_Coeff=NS */
  itv_t 	       	coeff;	/* coeff, encoded as interval */
} ja_term_t;

/* equation of the form pdim = c + [value of the list or ja_terms]*/
typedef struct ja_eq_t {
 
  ap_dim_t*     pdim;	/* index of the program variable, xp */
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

/* ********************************************************************** */
/* 0. Constructors */
/* ********************************************************************** */
ja_eq_t* new_equation (void);
void free_equation (ja_eq_t* equation);
void print_equation (FILE* stream,ja_eq_t* equation);
void add_equation_term_ns (ja_eq_t* equation, itv_t c, t1p_nsym_t* pnsym);
void add_equation_term_va (ja_eq_t* equation, itv_t c, ap_dim_t* pdim);


ja_eq_set_t* new_equation_set (void);
void free_equation_set (ja_eq_set_t* eq_set);
void print_equation_set (FILE* stream,ja_eq_set_t* eqs);
void add_equation (ja_eq_set_t* eqs, ja_eq_t* eq);
itv_t* get_coeff_var (ja_eq_set_t* eqs, int eq_n, ap_dim_t v);
itv_t* get_coeff_nsym (ja_eq_set_t* eqs, int eq_n, t1p_nsym_t v);


/* ********************************************************************** */
/* 1. Rebuild an abstract value */
/* ********************************************************************** */

ja_eq_set_t* abstract_value_to_eq_set (t1p_internal_t* pr, t1p_t* a1,  t1p_t* a2);

void rebuild_abstract_value(ap_manager_t* man, bool destructive, t1p_t* a, ja_eq_set_t* eqs);


/* ********************************************************************** */
/* 2. Search for linear relations */
/* ********************************************************************** */



/* ********************************************************************** */
/* 3. Alternative to the join operator */
/* ********************************************************************** */
t1p_t* t1p_join_alt(ap_manager_t* man, bool destructive, t1p_t* a1, t1p_t* a2);


#endif
