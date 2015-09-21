/* ********************************************************************** */
/* pk_vector.h: operations on vectors */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

/* This header file defines operations on vectors. A vector is just an array of
   elements of type pkint_t, as a consequence functions need to be given also
   their size. */

#ifndef __PK_VECTOR_H__
#define __PK_VECTOR_H__

#include "pk_config.h"
#include "pk_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Basic Operations */
numint_t* vector_alloc(size_t size);
void     vector_realloc(numint_t** q, size_t size, size_t nsize);
void     vector_free(numint_t* q, size_t size);
void     vector_clear(numint_t* q, size_t size);
void     vector_copy(numint_t* q1, numint_t* q2, size_t size);
void     vector_print(numint_t* q, size_t size);

/* Normalization */
bool vector_normalize(pk_internal_t* pk,
		      numint_t* q, size_t size);
bool vector_normalize_expr(pk_internal_t* pk,
			   numint_t* q, size_t size);
bool vector_normalize_constraint(pk_internal_t* pk,
				 numint_t* q, 
				 size_t intdim, size_t realdim);
bool vector_normalize_constraint_int(pk_internal_t* pk,
				     numint_t* q, 
				     size_t intdim, size_t realdim);

/* Comparison */
int vector_compare(pk_internal_t* pk,
		   numint_t* q1, numint_t* q2, size_t size);

/* Combination and Algebraic Operations */
void vector_combine(pk_internal_t* pk,
		    numint_t* q1, numint_t* q2,
		    numint_t* q3, size_t k, size_t size);
void vector_product(pk_internal_t* pk,
		    numint_t prod,
		    numint_t* q1, numint_t* q2, size_t size);
void vector_product_strict(pk_internal_t* pk,
			   numint_t prod,
			   numint_t* r1, numint_t* r2, size_t size);

/* Predicates that can be useful for users */
bool vector_is_null(pk_internal_t* pk,
		    numint_t* q, size_t size);
bool vector_is_null_strict(pk_internal_t* pk,
			   numint_t* q, size_t size);
bool vector_is_positivity_constraint(pk_internal_t* pk,
				     numint_t* q, size_t size);
bool vector_is_dummy_constraint(pk_internal_t* pk,
				  numint_t* q, size_t size);
bool vector_is_dummy_or_strict_generator(pk_internal_t* pk,
					 numint_t* q, size_t size);
bool vector_is_integer(pk_internal_t* pk,
		       numint_t* vec,
		       size_t intdim, size_t realdim);
long vector_hash(pk_internal_t* pk,
		 numint_t* vec,size_t size);
/* Functions meant to be internal */
numint_t* _vector_alloc_int(size_t size);
void vector_gcd(pk_internal_t* pk,
		numint_t* q, size_t size, numint_t gcd);


#ifdef __cplusplus
}
#endif

#endif
