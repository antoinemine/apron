#line 8 "/udd/bjeannet/dev/commoninterface/expr1.nw"
/* ************************************************************************* */
/* expr1.h: datatypes for dimensions, expressions and constraints */
/* ************************************************************************* */

/* GENERATED FROM expr1.nw: DOT NOT MODIFY ! */


#ifndef _EXPR1_H_
#define _EXPR1_H_

#include <stdio.h>
#include "coeff.h"
#include "expr0.h"
#include "environment.h"

#line 151 "/udd/bjeannet/dev/commoninterface/expr1.nw"
/* ********************************************************************** */
/* I. Datatypes */
/* ********************************************************************** */

/* Linear expressions */
typedef struct linexpr1_t {
  linexpr0_t* linexpr0;
  environment_t* env;
} linexpr1_t;

/* Constraints */
typedef struct lincons1_t {
  lincons0_t lincons0;
  environment_t* env;
} lincons1_t;

/* Array of constraints */
typedef struct lincons1_array_t {
  lincons0_array_t lincons0_array;
  environment_t* env;
} lincons1_array_t;

/* Generators */
typedef struct generator1_t {
  generator0_t generator0;
  environment_t* env;
} generator1_t;

/* Array of generators */
typedef struct generator1_array_t {
  generator0_array_t generator0_array;
  environment_t* env;
} generator1_array_t;

/* Boxes */
typedef struct box1_t {
  interval_t** p;
  environment_t* env;
} box1_t;

#line 200 "/udd/bjeannet/dev/commoninterface/expr1.nw"
/* ********************************************************************** */
/* II. Memory and access functions */
/* ********************************************************************** */

#line 213 "/udd/bjeannet/dev/commoninterface/expr1.nw"
/* ********************************************************************** */
/* II.1 Linear and affine expressions  */
/* ********************************************************************** */

/* ------------------------------------------------------------ */
/* Memory management and printing */
/* ------------------------------------------------------------ */

linexpr1_t linexpr1_make(environment_t* env,
                         linexpr_discr_t lin_discr, size_t size);
  /* Build a linear expressions with by default coefficients
     of type SCALAR and DOUBLE.
     - If lin_discr selects a dense representation, the size of the expression is 
       the size of the environement.
     - Otherwise, the initial size is given by size and the expression may be resized 
       lazily.
  */

static inline
void linexpr1_minimize(linexpr1_t* e);
  /* In case of sparse representation, remove zero coefficients */

linexpr1_t linexpr1_copy(const linexpr1_t* e);
  /* Duplication */

void linexpr1_clear(linexpr1_t* linexpr);
  /* Clear the linear expression:
     - Free the linear expression of level 0
     - Dereference the environment 
     - Set fields to NULL 
  */

void linexpr1_fprint(FILE* stream, const linexpr1_t* a);
  /* Printing a linear expression */

/* ------------------------------------------------------------ */
/* Tests */
/* ------------------------------------------------------------ */

static inline 
bool linexpr1_is_integer(const linexpr1_t* e);
  /* Does the expression depends only on integer variables ? */
static inline 
bool linexpr1_is_real(const linexpr1_t* e);
  /* Does the expression depends only on real variables ? */

/* ------------------------------------------------------------ */
/* Access */
/* ------------------------------------------------------------ */

static inline 
coeff_t* linexpr1_cstref(linexpr1_t* expr);
  /* Get a reference to the constant */

coeff_t* linexpr1_coeffref(linexpr1_t* expr, var_t var);
  /* Get a reference to the coefficient associated to the variable.
     Do not free it.
     In case of sparse representation, 
     possibly induce the addition of a new linear term.
     Return NULL if var is unknown in the environment.
 */

/* Get the constant and assign it to coeff */
static inline 
void linexpr1_get_cst(coeff_t* coeff, const linexpr1_t* expr);

/* Get coefficient of variable var in the expression and assign it to coeff.
   return true if var is unkown in the environment */
bool linexpr1_get_coeff(coeff_t* coeff, const linexpr1_t* expr, var_t var);

/* Set the constant of the linear expression */
static inline
void linexpr1_set_cst(linexpr1_t* expr, const coeff_t* cst);
static inline
void linexpr1_set_cst_scalar(linexpr1_t* expr, scalar_t* scalar);
static inline
void linexpr1_set_cst_scalar_int(linexpr1_t* expr, int num);
static inline
void linexpr1_set_cst_scalar_frac(linexpr1_t* expr, int num, unsigned int den);
static inline
void linexpr1_set_cst_scalar_double(linexpr1_t* expr, double num);
static inline
void linexpr1_set_cst_interval(linexpr1_t* expr, interval_t* itv);
static inline
void linexpr1_set_cst_interval_infsup(linexpr1_t* expr, scalar_t* inf, scalar_t* sup);
static inline
void linexpr1_set_cst_interval_infsup_int(linexpr1_t* expr, int inf, int sup);
static inline
void linexpr1_set_cst_interval_infsup_frac(linexpr1_t* expr,
                                  int numinf, unsigned int deninf,
                                  int numsup, unsigned int densup);
static inline
void linexpr1_set_cst_interval_infsup_double(linexpr1_t* expr, double inf, double sup);

/* Set the coefficient of variable var in the expression.
   Return true if var is unknown in the environment */
static inline
bool linexpr1_set_coeff(linexpr1_t* expr, var_t var, const coeff_t* coeff);
static inline
bool linexpr1_set_coeff_scalar(linexpr1_t* expr, var_t var, const scalar_t* scalar);
static inline
bool linexpr1_set_coeff_scalar_int(linexpr1_t* expr, var_t var, int num);
static inline
bool linexpr1_set_coeff_scalar_frac(linexpr1_t* expr, var_t var, int num, unsigned int den);
static inline
bool linexpr1_set_coeff_scalar_double(linexpr1_t* expr, var_t var, double num);
static inline
bool linexpr1_set_coeff_interval(linexpr1_t* expr, var_t var, const interval_t* itv);
static inline
bool linexpr1_set_coeff_interval_infsup(linexpr1_t* expr, var_t var, const scalar_t* inf, const scalar_t* sup);
static inline
bool linexpr1_set_coeff_interval_infsup_int(linexpr1_t* expr, var_t var, int inf, int sup);
static inline
bool linexpr1_set_coeff_interval_infsup_frac(linexpr1_t* expr, var_t var,
                                  int numinf, unsigned int deninf,
                                  int numsup, unsigned int densup);
static inline
bool linexpr1_set_coeff_interval_infsup_double(linexpr1_t* expr, var_t var, double inf, double sup);

/* Iterator (Macro): use:
   linexpr1_ForeachLinterm(linexpr1_t* e, size_t i, var_t v, coeff_t* pcoeff){
     ..
   }
   where
   - e is the inspected expression,
   - i is the internal iterator
   - var is the variable of one linear term
   - pcoeff is a pointer to the corresponding coefficient
*/
#define linexpr1_ForeachLinterm1(e, i, v, pcoeff) \
  for ((i)=0; \
         (i)<(e)->size ? \
         ((e)->discr==LINEXPR_DENSE ? \
          ((v) = environment_var_of_dim((e)->env,i), (pcoeff) = &(e)->p.coeff[i], true) : \
          ((v) = environment_var_of_dim((e)->env,(e)->p.linterm[i].dim), (pcoeff) = &(e)->p.linterm[i].coeff, (d)!=DIM_MAX)) : \
         false; \
       i++)

/* ------------------------------------------------------------ */
/* Changing of environment */
/* ------------------------------------------------------------ */

/* Change current environment with a super-environment. */
bool linexpr1_extend_environment(linexpr1_t* nexpr,
                                 const linexpr1_t* expr,
                                 environment_t* nenv);
bool linexpr1_extend_environment_with(linexpr1_t* expr,
                                      environment_t* nenv);
linexpr1_t linexpr1_extend_environment_dimchange(const linexpr1_t* expr,
                                                 environment_t* env,
                                                 dimchange_t* dimchange);
void linexpr1_extend_environment_dimchange_with(linexpr1_t* expr,
                                           environment_t* env,
                                           dimchange_t* dimchange);


#line 377 "/udd/bjeannet/dev/commoninterface/expr1.nw"
/* ********************************************************************** */
/* II. Linear constraints */
/* ********************************************************************** */

static inline 
lincons1_t lincons1_make(constyp_t constyp, linexpr1_t* expr);
  /* Create a constraint of given type with the given expression.
     The expression is not duplicated. */

static inline 
lincons1_t lincons1_copy(const lincons1_t* cons);
  /* Duplication */

void lincons1_clear(lincons1_t* cons);
  /* Clear the constraint and set pointers to NULL */

void lincons1_fprint(FILE* stream, const lincons1_t* cons);
  /* Printing */

lincons1_t lincons1_make_unsat(environment_t* env);
  /* Create the constraint -1>=0 */

static inline
bool lincons1_is_unsat(const lincons1_t* cons);
  /* True if the constraint is b>=0 or [a,b]>=0 with b negative */

static inline 
constyp_t* lincons1_constypref(lincons1_t* cons);
  /* Get a reference to the type of constraint */

static inline 
coeff_t* lincons1_cstref(lincons1_t* cons);
  /* Get a reference to the constant */

bool lincons1_get_coeff(coeff_t* coeff, const lincons1_t* cons, var_t var);
  /* Set coeff to the value of the coefficient of variable var in
     the constraint.
     Return true if var is unknown in the environment */

bool lincons1_set_coeff(lincons1_t* cons, var_t var, const coeff_t* coeff);
  /* Set the coefficient of variable var in the constraint 
     to the value of coeff.
     Return true if var is unknown in the environment */

static inline 
const linexpr1_t lincons1_get_linexpr1(const lincons1_t* cons);
  /* Return the linear expression of the constraint.
     Nothing is duplicated.
     Modifying coefficients of the argument or of the result is equivalent.
     But it is not the case for change of environments */

/* Change current environment with a super-environment. */
lincons1_t lincons1_extend_environment_dimchange(const lincons1_t* cons,
                                       environment_t* nenv, dimchange_t* dimchange);
void lincons1_extend_environment_dimchange_with(lincons1_t* cons,
                                      environment_t* nenv, dimchange_t* dimchange);
bool lincons1_extend_environment(lincons1_t* ncons,
                                 const lincons1_t* cons,
                                 environment_t* nenv);
bool lincons1_extend_environment_with(lincons1_t* cons,
                                      environment_t* nenv);

#line 446 "/udd/bjeannet/dev/commoninterface/expr1.nw"
/* ********************************************************************** */
/* III. Generators */
/* ********************************************************************** */

static inline 
generator1_t generator1_make(gentyp_t gentyp, linexpr1_t* linexpr);
  /* Create a generator of given type with the given expression.
     The expression is not duplicated. */

static inline 
generator1_t generator1_copy(const generator1_t* gen);
  /* Duplication */

void generator1_clear(generator1_t* gen);
  /* Clear the generator and set pointers to NULL */

void generator1_fprint(FILE* stream, const generator1_t* gen);
  /* Printing */

static inline 
gentyp_t* generator1_gentypref(generator1_t* gen);
  /* Get a reference to the type of generator */

bool generator1_get_coeff(coeff_t* coeff, const generator1_t* expr, var_t var);
  /* Set coeff to the value of the coefficient of variable var in
     the generator.
     Return true if var is unknown in the environment */

bool generator1_set_coeff(generator1_t* expr, var_t var, const coeff_t* coeff);
  /* Set the coefficient of variable var in the generator 
     to the value of coeff.
     Return true if var is unknown in the environment */

linexpr1_t linexpr1_of_generator1(generator1_t* gen);
  /* Return the linear expression of the generator.
     Nothing is duplicated.
     Modifying coefficients of the argument or of the result is equivalent.
     But it is not the case for change of environments */

/* Change current environment with a super-environment. */
bool generator1_extend_environment(generator1_t* ngen,
                                   const generator1_t* gen,
                                   environment_t* nenv);
bool generator1_extend_environment_with(generator1_t* gen,
                                        environment_t* nenv);
generator1_t generator1_extend_environment_dimchange(const generator1_t* gen,
                                       environment_t* nenv, dimchange_t* dimchange);
void generator1_extend_environment_dimchange_with(generator1_t* gen,
                                      environment_t* nenv, dimchange_t* dimchange);

#line 504 "/udd/bjeannet/dev/commoninterface/expr1.nw"
/* ====================================================================== */
/* II.4 Array of linear constraints */
/* ====================================================================== */

lincons1_array_t lincons1_array_make(environment_t* env, size_t size);
  /* Allocate an array of size constraints. 
     The constraints are initialized with NULL pointers, 
     so that lincons1_array_free may be safely called */
void lincons1_array_clear(lincons1_array_t* array);
  /* Clear the constraints of the array, and then the array itself */
void lincons1_array_fprint(FILE* stream,
                           const lincons1_array_t* lincons1_array);
  /* Printing */

static inline 
size_t lincons1_array_size(lincons1_array_t* array);

static inline 
void lincons1_array_clear_index(lincons1_array_t* array, size_t index);
  /* Clear the constraint at index index. */ 
lincons1_t lincons1_array_get(const lincons1_array_t* array, 
                              size_t index);
  /* Return the linear constraint of the given index
     Nothing is duplicated, and the result should never be cleared.
     Modifying coefficients of the argument or of the result is equivalent.
     But it is not the case for change of environments */

bool lincons1_array_set(lincons1_array_t* array, 
                              size_t index, const lincons1_t* cons);
  /* Fill the index of the array with the constraint.
     Assumes array->env==cons->env.
     Nothing is duplicated. 
     The argument should never be cleared. (its environment is dereferenced).
     If a constraint was already stored, it is first cleared.
     Return true iff problem (index or array->env!=cons->env)
  */

/* Change current environment with a super-environment. */
void 
lincons1_array_extend_environment_dimchange_with(lincons1_array_t* array,
                                            environment_t* env, dimchange_t* dimchange);
lincons1_array_t 
lincons1_array_extend_environment_dimchange(const lincons1_array_t* array,
                                  environment_t* env, dimchange_t* dimchange);
bool
lincons1_array_extend_environment_with(lincons1_array_t* array,
                                            environment_t* env);
bool
lincons1_array_extend_environment(lincons1_array_t* narray,
                                  const lincons1_array_t* array,
                                  environment_t* env);

#line 564 "/udd/bjeannet/dev/commoninterface/expr1.nw"
/* ====================================================================== */
/* II.5 Array of generators */
/* ====================================================================== */

generator1_array_t generator1_array_make(environment_t* env, size_t size);
  /* Allocate an array of size generators. 
     The generators are initialized with NULL pointers, 
     so that lincons1_array_free may be safely called */
void generator1_array_clear(generator1_array_t* array);
  /* Clear the generators of the array, and then the array itself */
void generator1_array_fprint(FILE* stream,
                           const generator1_array_t* generator1_array);
  /* Printing */

static inline 
size_t generator1_array_size(generator1_array_t* array);

static inline 
void generator1_array_clear_index(generator1_array_t* array, size_t index);
  /* Clear the generator at index index. */ 
generator1_t generator1_array_get(const generator1_array_t* array, 
                                    size_t index);
  /* Return the linear generator of the given index
     Nothing is duplicated, and the result should never be cleared.
     Modifying coefficients of the argument or of the result is equivalent.
     But it is not the case for change of environments */

bool generator1_array_set(generator1_array_t* array, 
                              size_t index, const generator1_t* gen);
  /* Fill the index of the array with the generator.
     Assumes array->env==gen->env.
     Nothing is duplicated. 
     The argument should never be cleared. (its environment is dereferenced).
     If a generator was already stored, it is first cleared.
     Return true iff problem (index or array->env!=gen->env)
  */

/* Change current environment with a super-environment. */
void 
generator1_array_extend_environment_dimchange_with(generator1_array_t* array,
                                            environment_t* env, dimchange_t* dimchange);
generator1_array_t 
generator1_array_extend_environment_dimchange(const generator1_array_t* array,
                                  environment_t* env, dimchange_t* dimchange);
bool
generator1_array_extend_environment_with(generator1_array_t* array,
                                         environment_t* env);
bool
generator1_array_extend_environment(generator1_array_t* narray,
                                    const generator1_array_t* array,
                                    environment_t* env);

#line 624 "/udd/bjeannet/dev/commoninterface/expr1.nw"
/* ====================================================================== */
/* II.6 Boxes */
/* ====================================================================== */

void box1_fprint(FILE* stream,
                 const box1_t* box);
  /* Printing */
void box1_clear(box1_t* box);
  /* Clearing */

#line 641 "/udd/bjeannet/dev/commoninterface/expr1.nw"
/* ********************************************************************** */
/* II. Inline function definitions */
/* ********************************************************************** */

/* ====================================================================== */
/* II.1 Linear and affine expressions  */
/* ====================================================================== */

static inline
void linexpr1_minimize(linexpr1_t* e){
  linexpr0_minimize(e->linexpr0);
}
static inline 
bool linexpr1_is_integer(const linexpr1_t* e){
  return linexpr0_is_integer(e->linexpr0,e->env->intdim);
}

static inline 
bool linexpr1_is_real(const linexpr1_t* e){
  return linexpr0_is_real(e->linexpr0,e->env->intdim);
}

static inline 
coeff_t* linexpr1_cstref(linexpr1_t* expr){ 
  return &expr->linexpr0->cst; 
}
static inline 
void linexpr1_get_cst(coeff_t* cst, const linexpr1_t* expr){
 coeff_set(cst, &expr->linexpr0->cst); 
}

static inline
void linexpr1_set_cst(linexpr1_t* expr, const coeff_t* cst)
  { coeff_set(&expr->linexpr0->cst,cst); }
static inline
void linexpr1_set_cst_scalar(linexpr1_t* expr, scalar_t* scalar)
  { coeff_set_scalar(&expr->linexpr0->cst, scalar); }
static inline
void linexpr1_set_cst_scalar_int(linexpr1_t* expr, int num)
  { coeff_set_scalar_int(&expr->linexpr0->cst, num); }
static inline
void linexpr1_set_cst_scalar_frac(linexpr1_t* expr, int num, unsigned int den)
  { coeff_set_scalar_frac(&expr->linexpr0->cst, num, den); }
static inline
void linexpr1_set_cst_scalar_double(linexpr1_t* expr, double num)
  { coeff_set_scalar_double(&expr->linexpr0->cst, num); }
static inline
void linexpr1_set_cst_interval(linexpr1_t* expr, interval_t* itv)
  { coeff_set_interval(&expr->linexpr0->cst, itv); }
static inline
void linexpr1_set_cst_interval_infsup_int(linexpr1_t* expr, int inf, int sup)
  { coeff_set_interval_infsup_int(&expr->linexpr0->cst, inf,sup); }
static inline
void linexpr1_set_cst_interval_infsup(linexpr1_t* expr, scalar_t* inf, scalar_t* sup)
  { coeff_set_interval_infsup(&expr->linexpr0->cst, inf,sup); }
static inline
void linexpr1_set_cst_interval_infsup_frac(linexpr1_t* expr,
                                  int numinf, unsigned int deninf,
                                  int numsup, unsigned int densup)
  { coeff_set_interval_infsup_frac(&expr->linexpr0->cst, numinf,deninf, numsup,densup); }
static inline
void linexpr1_set_cst_interval_infsup_double(linexpr1_t* expr, double inf, double sup)
  { coeff_set_interval_infsup_double(&expr->linexpr0->cst, inf,sup); }

static inline
bool linexpr1_set_coeff(linexpr1_t* expr, var_t var, const coeff_t* coeff)
  { coeff_t* ecoeff = linexpr1_coeffref(expr,var); if (ecoeff){coeff_set(ecoeff,coeff); return false;} else return true; }
static inline
bool linexpr1_set_coeff_scalar(linexpr1_t* expr, var_t var, const scalar_t* scalar)
  { coeff_t* ecoeff = linexpr1_coeffref(expr,var); if (ecoeff){ coeff_set_scalar(ecoeff,scalar); return false; } else return true; }
static inline
bool linexpr1_set_coeff_scalar_int(linexpr1_t* expr, var_t var, int num)
  { coeff_t* ecoeff = linexpr1_coeffref(expr,var); if (ecoeff){ coeff_set_scalar_int(ecoeff,num); return false; } else return true; }
static inline
bool linexpr1_set_coeff_scalar_frac(linexpr1_t* expr, var_t var, int num, unsigned int den)
  { coeff_t* ecoeff = linexpr1_coeffref(expr,var); if (ecoeff){ coeff_set_scalar_frac(ecoeff,num, den); return false; } else return true; }
static inline
bool linexpr1_set_coeff_scalar_double(linexpr1_t* expr, var_t var, double num)
  { coeff_t* ecoeff = linexpr1_coeffref(expr,var); if (ecoeff){ coeff_set_scalar_double(ecoeff,num); return false; } else return true; }
static inline
bool linexpr1_set_coeff_interval(linexpr1_t* expr, var_t var, const interval_t* itv)
  { coeff_t* ecoeff = linexpr1_coeffref(expr,var); if (ecoeff){ coeff_set_interval(ecoeff,itv); return false; } else return true; }
static inline
bool linexpr1_set_coeff_interval_infsup_int(linexpr1_t* expr, var_t var, int inf, int sup)
  { coeff_t* ecoeff = linexpr1_coeffref(expr,var); if (ecoeff){ coeff_set_interval_infsup_int(ecoeff,inf,sup); return false; } else return true; }
static inline
bool linexpr1_set_coeff_interval_infsup(linexpr1_t* expr, var_t var, const scalar_t* inf, const scalar_t* sup)
  { coeff_t* ecoeff = linexpr1_coeffref(expr,var); if (ecoeff){ coeff_set_interval_infsup(ecoeff,inf,sup); return false; } else return true; }
static inline
bool linexpr1_set_coeff_interval_infsup_frac(linexpr1_t* expr, var_t var,
                                  int numinf, unsigned int deninf,
                                  int numsup, unsigned int densup)
  { coeff_t* ecoeff = linexpr1_coeffref(expr,var); if (ecoeff){ coeff_set_interval_infsup_frac(ecoeff,numinf,deninf, numsup,densup); return false; } else return true; }
static inline
bool linexpr1_set_coeff_interval_infsup_double(linexpr1_t* expr, var_t var, double inf, double sup)
  { coeff_t* ecoeff = linexpr1_coeffref(expr,var); if (ecoeff){ coeff_set_interval_infsup_double(ecoeff,inf,sup); return false; } else return true; }


/* ====================================================================== */
/* II.2 Constraints  */
/* ====================================================================== */

static inline 
lincons1_t lincons1_make(constyp_t constyp, linexpr1_t* expr)
{
  lincons1_t cons;
  cons.lincons0 = lincons0_make(constyp,expr->linexpr0);
  cons.env = expr->env;
  return cons;
}
static inline 
lincons1_t lincons1_copy(const lincons1_t* cons){
  lincons1_t ncons;
  ncons.lincons0 = lincons0_copy(&cons->lincons0);
  ncons.env = environment_copy(cons->env);
  return ncons;
}

bool lincons1_is_unsat(const lincons1_t* cons){
  return lincons0_is_unsat(&cons->lincons0);
}
static inline 
constyp_t* lincons1_constypref(lincons1_t* cons){
  return &cons->lincons0.constyp;
}
static inline 
coeff_t* lincons1_cstref(lincons1_t* cons){
  return &cons->lincons0.linexpr0->cst;
}
static inline 
const linexpr1_t lincons1_get_linexpr1(const lincons1_t* cons){
  linexpr1_t expr;
  expr.linexpr0 = cons->lincons0.linexpr0;
  expr.env = cons->env;
  return expr;
}
/* ====================================================================== */
/* II.3 Array of constraints  */
/* ====================================================================== */
static inline 
void lincons1_array_clear_index(lincons1_array_t* array, size_t index){
  lincons0_clear(&array->lincons0_array.p[index]);
}
static inline 
size_t lincons1_array_size(lincons1_array_t* array){
  return array->lincons0_array.size;
}

/* ====================================================================== */
/* II.4 Generators */
/* ====================================================================== */

static inline 
generator1_t generator1_make(gentyp_t gentyp, linexpr1_t* expr)
{
  generator1_t gen;
  gen.generator0 = generator0_make(gentyp,expr->linexpr0);
  gen.env = expr->env;
  return gen;
}
static inline 
generator1_t generator1_copy(const generator1_t* gen){
  generator1_t ngen;
  ngen.generator0 = generator0_copy(&gen->generator0);
  ngen.env = environment_copy(gen->env);
  return ngen;
}
static inline 
gentyp_t* generator1_gentypref(generator1_t* gen)
{
  return &gen->generator0.gentyp;
}
static inline 
const linexpr1_t generator1_get_linexpr1(const generator1_t* gen)
{
  linexpr1_t expr;
  expr.linexpr0 = gen->generator0.linexpr0;
  expr.env = gen->env;
  return expr;
}

/* ====================================================================== */
/* II.5 Array of generators  */
/* ====================================================================== */
static inline 
void generator1_array_clear_index(generator1_array_t* array, size_t index){
  generator0_clear(&array->generator0_array.p[index]);
}
static inline 
size_t generator1_array_size(generator1_array_t* array){
  return array->generator0_array.size;
}
#line 836 "/udd/bjeannet/dev/commoninterface/expr1.nw"
#endif
