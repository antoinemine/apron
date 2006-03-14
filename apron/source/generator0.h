/* ************************************************************************* */
/* generator0.h: generators and arrays */
/* ************************************************************************* */

/* normally included from expr0.h */

#ifndef _GENERATOR0_H_
#define _GENERATOR0_H_

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "coeff.h"
#include "linexpr0.h"

/* ====================================================================== */
/* Datatypes */
/* ====================================================================== */

/* Datatype for type of generators */
typedef enum gentyp_t {
  GEN_LINE,
  GEN_RAY,
  GEN_VERTEX
} gentyp_t;

/* Generator */
typedef struct generator0_t {
  linexpr0_t* linexpr0; /* constant coefficient ignored,
			   and assumed to be truly linear (without intervals) */
  gentyp_t gentyp;
} generator0_t;

/* Array of generators */
typedef struct generator0_array_t {
  generator0_t* p;
  size_t size;
} generator0_array_t;

/* ********************************************************************** */
/* I. Generators */
/* ********************************************************************** */

static inline
generator0_t generator0_make(gentyp_t gentyp, linexpr0_t* linexpr);
  /* Create a generator of given type with the given expression.
     The expression is not duplicated, just pointed to */

static inline
generator0_t generator0_copy(const generator0_t* gen);
  /* Duplication */

static inline
void generator0_clear(generator0_t* gen);
  /* Free the linear expression of the generator and set pointer to NULL */

void generator0_fprint(FILE* stream,
                       const generator0_t* gen, char** name_of_dim);
  /* Printing a generator */

/* ====================================================================== */
/* I.1 Change of dimensions and permutations */
/* ====================================================================== */

static inline
void generator0_add_dimensions_with(generator0_t* gen,
				    const dimchange_t* dimchange);
static inline
generator0_t generator0_add_dimensions(const generator0_t* gen,
				       const dimchange_t* dimchange);

static inline
void generator0_permute_dimensions_with(generator0_t* gen,
					const dim_t* perm);
static inline
generator0_t generator0_permute_dimensions(const generator0_t* gen,
					   const dim_t* perm);

/* ********************************************************************** */
/* II. Array of Generators */
/* ********************************************************************** */

generator0_array_t generator0_array_make(size_t size);
  /* Allocate an array of size generators.
     The generators are initialized with NULL pointers. */

void generator0_array_clear(generator0_array_t* generator0_array);
  /* Clear the generators of the array, and then the array itself */

void generator0_array_fprint(FILE* stream,
                           const generator0_array_t* generator0_array,
			     char** name_of_dim);
/* Printing */

/* ====================================================================== */
/* II.1 Change of dimensions and permutations */
/* ====================================================================== */
void generator0_array_add_dimensions_with(generator0_array_t* array,
					  const dimchange_t* dimchange);
generator0_array_t generator0_array_add_dimensions(const generator0_array_t* array,
						   const dimchange_t* dimchange);

void generator0_array_permute_dimensions_with(generator0_array_t* array,
					      const dim_t* perm);
generator0_array_t generator0_array_permute_dimensions(const generator0_array_t* array,
						       const dim_t* perm);
/* ********************************************************************** */
/* III. Inline functions definitions */
/* ********************************************************************** */

static inline generator0_t generator0_make(gentyp_t gentyp, linexpr0_t* linexpr)
{
  generator0_t gen;

  gen.gentyp = gentyp;
  gen.linexpr0 = linexpr;
  return gen;
}
static inline generator0_t generator0_copy(const generator0_t* gen)
{
  return generator0_make(gen->gentyp, linexpr0_copy(gen->linexpr0));
}
static inline void generator0_clear(generator0_t* generator)
{
  if (generator->linexpr0){
    linexpr0_free(generator->linexpr0);
    generator->linexpr0 = NULL;
  }
}

static inline
void generator0_add_dimensions_with(generator0_t* gen,
				  const dimchange_t* dimchange)
{ linexpr0_add_dimensions_with(gen->linexpr0,dimchange); }
static inline
generator0_t generator0_add_dimensions(const generator0_t* gen,
				   const dimchange_t* dimchange)
{
  return generator0_make(gen->gentyp,
			 linexpr0_add_dimensions(gen->linexpr0,dimchange));
}
static inline
void generator0_permute_dimensions_with(generator0_t* gen,
				      const dim_t* perm)
{ linexpr0_permute_dimensions_with(gen->linexpr0,perm); }
static inline
generator0_t generator0_permute_dimensions(const generator0_t* gen,
				       const dim_t* perm)
{
  return generator0_make(gen->gentyp,
			 linexpr0_permute_dimensions(gen->linexpr0,perm));
}

#endif
