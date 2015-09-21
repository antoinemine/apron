/* ************************************************************************* */
/* ap_generator0.h: generators and arrays */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

/* normally included from ap_expr0.h */

#ifndef _AP_GENERATOR0_H_
#define _AP_GENERATOR0_H_

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "ap_coeff.h"
#include "ap_linexpr0.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* Datatype for type of generators */
typedef enum ap_gentyp_t {
  AP_GEN_LINE,
  AP_GEN_RAY,
  AP_GEN_VERTEX,
  AP_GEN_LINEMOD,
  AP_GEN_RAYMOD
} ap_gentyp_t;

/* Generator */
typedef struct ap_generator0_t {
  ap_linexpr0_t* linexpr0; /* constant coefficient ignored,
			   and assumed to be truly linear (without intervals) */
  ap_gentyp_t gentyp;
} ap_generator0_t;

/* Array of generators */
typedef struct ap_generator0_array_t {
  ap_generator0_t* p;
  size_t size;
} ap_generator0_array_t;

/* ********************************************************************** */
/* I. Generators */
/* ********************************************************************** */

static inline
ap_generator0_t ap_generator0_make(ap_gentyp_t gentyp, ap_linexpr0_t* linexpr);
  /* Create a generator of given type with the given expression.
     The expression is not duplicated, just pointed to */

static inline
ap_generator0_t ap_generator0_copy(ap_generator0_t* gen);
  /* Duplication */

static inline
void ap_generator0_clear(ap_generator0_t* gen);
  /* Free the linear expression of the generator and set pointer to NULL */

void ap_generator0_fprint(FILE* stream,
			  ap_generator0_t* gen, char** name_of_dim);
  /* Printing a generator */

/* ====================================================================== */
/* I.1 Change of dimensions and permutations */
/* ====================================================================== */

static inline
void ap_generator0_add_dimensions_with(ap_generator0_t* gen,
				       ap_dimchange_t* dimchange);
static inline
ap_generator0_t ap_generator0_add_dimensions(ap_generator0_t* gen,
					     ap_dimchange_t* dimchange);

static inline
void ap_generator0_permute_dimensions_with(ap_generator0_t* gen,
					   ap_dimperm_t* perm);
static inline
ap_generator0_t ap_generator0_permute_dimensions(ap_generator0_t* gen,
						 ap_dimperm_t* perm);

/* ********************************************************************** */
/* II. Array of Generators */
/* ********************************************************************** */

ap_generator0_array_t ap_generator0_array_make(size_t size);
  /* Allocate an array of size generators.
     The generators are initialized with NULL pointers. */

void ap_generator0_array_resize(ap_generator0_array_t* array, size_t size);
  /* Resize an array of size generators.
     New generators are initialized with NULL pointers,
     Removed generators with non-NULL pointers are deallocated */

void ap_generator0_array_clear(ap_generator0_array_t* ap_generator0_array);
  /* Clear the generators of the array, and then the array itself */

void ap_generator0_array_fprint(FILE* stream,
				ap_generator0_array_t* ap_generator0_array,
				char** name_of_dim);
/* Printing */

/* ====================================================================== */
/* II.1 Change of dimensions and permutations */
/* ====================================================================== */
void ap_generator0_array_add_dimensions_with(ap_generator0_array_t* array,
					     ap_dimchange_t* dimchange);
ap_generator0_array_t ap_generator0_array_add_dimensions(ap_generator0_array_t* array,
							 ap_dimchange_t* dimchange);

void ap_generator0_array_permute_dimensions_with(ap_generator0_array_t* array,
						 ap_dimperm_t* perm);
ap_generator0_array_t ap_generator0_array_permute_dimensions(ap_generator0_array_t* array,
							     ap_dimperm_t* perm);
/* ********************************************************************** */
/* III. Inline functions definitions */
/* ********************************************************************** */

static inline ap_generator0_t ap_generator0_make(ap_gentyp_t gentyp, ap_linexpr0_t* linexpr)
{
  ap_generator0_t gen;

  gen.gentyp = gentyp;
  gen.linexpr0 = linexpr;
  return gen;
}
static inline ap_generator0_t ap_generator0_copy(ap_generator0_t* gen)
{
  return ap_generator0_make(gen->gentyp, 
			    gen->linexpr0 ? ap_linexpr0_copy(gen->linexpr0) : NULL);
}
static inline void ap_generator0_clear(ap_generator0_t* generator)
{
  if (generator->linexpr0){
    ap_linexpr0_free(generator->linexpr0);
    generator->linexpr0 = NULL;
  }
}

static inline
void ap_generator0_add_dimensions_with(ap_generator0_t* gen,
				       ap_dimchange_t* dimchange)
{ ap_linexpr0_add_dimensions_with(gen->linexpr0,dimchange); }
static inline
ap_generator0_t ap_generator0_add_dimensions(ap_generator0_t* gen,
					     ap_dimchange_t* dimchange)
{
  return ap_generator0_make(gen->gentyp,
			    ap_linexpr0_add_dimensions(gen->linexpr0,dimchange));
}
static inline
void ap_generator0_permute_dimensions_with(ap_generator0_t* gen,
					   ap_dimperm_t* perm)
{ ap_linexpr0_permute_dimensions_with(gen->linexpr0,perm); }
static inline
ap_generator0_t ap_generator0_permute_dimensions(ap_generator0_t* gen,
						 ap_dimperm_t* perm)
{
  return ap_generator0_make(gen->gentyp,
			    ap_linexpr0_permute_dimensions(gen->linexpr0,perm));
}

#ifdef __cplusplus
}
#endif

#endif
