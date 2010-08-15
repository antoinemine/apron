/* -*- mode: c -*- */

/* ************************************************************************* */
/* ap_yyy1_array.h: array of ap_yyy1_t */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

/* normally included from ap_expr0.h */

#ifndef _AP_yyy1_ARRAY_H_
#define _AP_yyy1_ARRAY_H_

#include "ap_yyy0.h"
#include "ap_yyy0_array.h"
#include "ap_yyy1.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* Array of linear expressions */
typedef struct ap_yyy1_array_struct {
  ap_yyy0_array_t yyy0_array;
  ap_environment_t* env;
} ap_yyy1_array_struct;
typedef ap_yyy1_array_struct* ap_yyy1_array_ptr;
typedef ap_yyy1_array_struct ap_yyy1_array_t[1];

/* ====================================================================== */
/* I. Memory management and printing */
/* ====================================================================== */

void ap_yyy1_array_init(ap_yyy1_array_t e, ap_scalar_discr_t discr, size_t size, ap_environment_t* env);
void ap_yyy1_array_init_set(ap_yyy1_array_t res, ap_yyy1_array_t e);
void ap_yyy1_array_init_set_yyy0_array(ap_yyy1_array_t res, ap_yyy0_array_t e, ap_environment_t* env);
MACRO_MAINZ
void ap_yyy1_array_init_set_yyyZZZ_array(ap_yyy1_array_t res, ap_yyyZZZ_array_t e, ap_environment_t* env);
ENDMACRO
void ap_yyy1_array_minimize(ap_yyy1_array_t a);
void ap_yyy1_array_clear(ap_yyy1_array_t e);
void ap_yyy1_array_fprint(FILE* stream, ap_yyy1_array_t e);

/* ====================================================================== */
/* II. Access, possibly with conversions */
/* ====================================================================== */

static inline 
ap_scalar_discr_t ap_yyy1_discr(ap_yyy1_t expr);
static inline
ap_environment_t* ap_yyy1_envref(ap_yyy1_t expr);
  /* Get a reference to the environment. Do not free it. */
static inline 
ap_yyy0_ptr ap_yyy1_yyy0ref(ap_yyy1_t e);
  /* Get a reference to the underlying linear expression of level
     0. Do not free it. */


static inline ap_scalar_discr_t ap_yyy1_array_discr(ap_yyy1_array_t p);
size_t ap_yyy1_array_size(ap_yyy1_array_t p);
ap_linexpr_type_t ap_yyy1_array_type(ap_yyy1_array_t array);
bool ap_yyy1_array_is_linear(ap_yyy1_array_t array);
bool ap_yyy1_array_is_quasilinear(ap_yyy1_array_t array);
  /* Are all the constraints involved linear (resp. quasilinear) */

bool ap_yyy1_array_set(ap_yyy1_array_t res, ap_yyy1_array_t e, num_internal_t intern);
  bool ap_yyy1_array_set_index(ap_yyy1_array_t p, bool* perror, size_t i, ap_yyy1_t a, num_internal_t intern);
bool ap_yyy1_array_get_index(ap_yyy1_t a, ap_yyy1_array_t p, size_t i, num_internal_t intern);
void ap_yyy1_array_ref_index(ap_yyy1_t a, ap_yyy1_array_t p, size_t i);

bool ap_yyy1_array_set_yyy0_array(ap_yyy1_array_t res, ap_yyy0_array_t e, ap_environment_t* env, num_internal_t intern);

MACRO_MAINZ
bool ap_yyy1_array_set_yyyZZZ_array(ap_yyy1_array_t a, ap_yyyZZZ_array_t b, ap_environment_t* env, num_internal_t intern);
bool ap_yyyZZZ_array_set_yyy1_array(ap_yyyZZZ_array_t a, ap_yyy1_array_t b, num_internal_t intern);

bool ap_yyy1_array_set_index_yyyZZZ(ap_yyy1_array_t p, size_t i, ap_yyyZZZ_t a, num_internal_t intern);
bool ap_yyy1_array_get_index_yyyZZZ(ap_yyyZZZ_t a, ap_yyy1_array_t p, size_t i, num_internal_t intern);
ENDMACRO

/* ====================================================================== */
/* III. Change of dimensions and permutations */
/* ====================================================================== */

void ap_yyy1_array_extend_environment(ap_yyy1_array_t nexpr,
				      bool* perror,
				      ap_yyy1_array_t expr,
				      ap_environment_t* nenv);

static inline ap_scalar_discr_t ap_yyy1_array_discr(ap_yyy1_array_t p)
{ return p->yyy0_array->discr; }
static inline
ap_environment_t* ap_yyy1_array_envref(ap_yyy1_array_t e)
{ return e->env; }
static inline
ap_yyy0_array_ptr ap_yyy1_array_yyy0_arrayref(ap_yyy1_array_t e)
{ return e->yyy0_array; }

MACRO_MAINZ
static inline void ap_yyy1_array_cons_ZZZ(ap_yyy1_array_t res, ap_yyyZZZ_array_t e, ap_environment_t* env)
{ 
  res->yyy0_array->discr = AP_SCALAR_ZZZ; 
  res->yyy0_array->yyy_array.ZZZ = e; 
  res->env = env;
};
ENDMACRO

#ifdef __cplusplus
}
#endif

#endif
