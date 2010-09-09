/* -*- mode: c -*- */

/* ************************************************************************* */
/* ap_linyyy1_array.h: array of ap_linyyy1_t */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

/* normally included from ap_expr0.h */

#ifndef _AP_linyyy1_ARRAY_H_
#define _AP_linyyy1_ARRAY_H_

#include "ap_linyyy0.h"
#include "ap_linyyy0_array.h"
#include "ap_linyyy1.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* Array of linear expressions */
typedef struct ap_linyyy1_array_struct {
  ap_linyyy0_array_t linyyy0_array;
  ap_environment_t* env;
} ap_linyyy1_array_struct;
typedef ap_linyyy1_array_struct* ap_linyyy1_array_ptr;
typedef ap_linyyy1_array_struct ap_linyyy1_array_t[1];

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_linyyy1_array_init(ap_linyyy1_array_t e, ap_scalar_discr_t discr, size_t size, ap_environment_t* env);
void ap_linyyy1_array_init_set(ap_linyyy1_array_t res, ap_linyyy1_array_t e);
void ap_linyyy1_array_init_set_linyyy0_array(ap_linyyy1_array_t res, ap_linyyy0_array_t e, ap_environment_t* env);
MACRO_MAINZ
void ap_linyyy1_array_init_set_linyyyZZZ_array(ap_linyyy1_array_t res, ap_linyyyZZZ_array_t e, ap_environment_t* env);
ENDMACRO
void ap_linyyy1_array_minimize(ap_linyyy1_array_t a);
void ap_linyyy1_array_clear(ap_linyyy1_array_t e);
void ap_linyyy1_array_fprint(FILE* stream, ap_linyyy1_array_t e);

/* ====================================================================== */
/* II. Access, possibly with conversions */
/* ====================================================================== */

static inline 
ap_scalar_discr_t ap_linyyy1_discr(ap_linyyy1_t expr);
static inline
ap_environment_t* ap_linyyy1_envref(ap_linyyy1_t expr);
  /* Get a reference to the environment. Do not free it. */
static inline 
ap_linyyy0_ptr ap_linyyy1_linyyy0ref(ap_linyyy1_t e);
  /* Get a reference to the underlying linear expression of level
     0. Do not free it. */


static inline ap_scalar_discr_t ap_linyyy1_array_discr(ap_linyyy1_array_t p);
size_t ap_linyyy1_array_size(ap_linyyy1_array_t p);

#if !defined(_AP_lingen1_ARRAY_H_)
ap_linexpr_type_t ap_linyyy1_array_type(ap_linyyy1_array_t array);
bool ap_linyyy1_array_is_quasilinear(ap_linyyy1_array_t array);
#endif
bool ap_linyyy1_array_is_linear(ap_linyyy1_array_t array);
  /* Are all the constraints involved linear (resp. quasilinear) */

bool ap_linyyy1_array_set(ap_linyyy1_array_t res, ap_linyyy1_array_t e, num_internal_t intern);
  bool ap_linyyy1_array_set_index(ap_linyyy1_array_t p, bool* perror, size_t i, ap_linyyy1_t a, num_internal_t intern);
bool ap_linyyy1_array_get_index(ap_linyyy1_t a, ap_linyyy1_array_t p, size_t i, num_internal_t intern);
void ap_linyyy1_array_ref_index(ap_linyyy1_t a, ap_linyyy1_array_t p, size_t i);

bool ap_linyyy1_array_set_linyyy0_array(ap_linyyy1_array_t res, ap_linyyy0_array_t e, ap_environment_t* env, num_internal_t intern);

MACRO_MAINZ
bool ap_linyyy1_array_set_linyyyZZZ_array(ap_linyyy1_array_t a, ap_linyyyZZZ_array_t b, ap_environment_t* env, num_internal_t intern);
bool ap_linyyyZZZ_array_set_linyyy1_array(ap_linyyyZZZ_array_t a, ap_linyyy1_array_t b, num_internal_t intern);

bool ap_linyyy1_array_set_index_linyyyZZZ(ap_linyyy1_array_t p, size_t i, ap_linyyyZZZ_t a, num_internal_t intern);
bool ap_linyyy1_array_get_index_linyyyZZZ(ap_linyyyZZZ_t a, ap_linyyy1_array_t p, size_t i, num_internal_t intern);
ENDMACRO

/* ====================================================================== */
/* III. Change of dimensions and permutations */
/* ====================================================================== */

void ap_linyyy1_array_extend_environment(ap_linyyy1_array_t nexpr,
				      bool* perror,
				      ap_linyyy1_array_t expr,
				      ap_environment_t* nenv);

static inline ap_scalar_discr_t ap_linyyy1_array_discr(ap_linyyy1_array_t p)
{ return p->linyyy0_array->discr; }
static inline
ap_environment_t* ap_linyyy1_array_envref(ap_linyyy1_array_t e)
{ return e->env; }
static inline
ap_linyyy0_array_ptr ap_linyyy1_array_linyyy0_arrayref(ap_linyyy1_array_t e)
{ return e->linyyy0_array; }

MACRO_MAINZ
static inline void ap_linyyy1_array_cons_ZZZ(ap_linyyy1_array_t res, ap_linyyyZZZ_array_t e, ap_environment_t* env)
{ 
  res->linyyy0_array->discr = AP_SCALAR_ZZZ; 
  res->linyyy0_array->linyyy_array.ZZZ = e; 
  res->env = env;
};
ENDMACRO

#ifdef __cplusplus
}
#endif

#endif
