/* ********************************************************************** */
/* boxXXX_representation.c: general management */
/* ********************************************************************** */

#ifndef _BOX_XXX_REPRESENTATION_H_
#define _BOX_XXX_REPRESENTATION_H_

#include "boxXXX.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Internal functions */
boxXXX_t* boxXXX_alloc(size_t intdim, size_t realdim);
void boxXXX_init(boxXXX_t* a);
void boxXXX_set_bottom(boxXXX_t* a);
void boxXXX_set_top(boxXXX_t* a);
void boxXXX_set(boxXXX_t* a, boxXXX_t* b);

/* 1. Memory */
boxXXX_t* boxXXX_copy(ap_manager_t* man, boxXXX_t* a);
void boxXXX_free(ap_manager_t* man, boxXXX_t* a);
size_t boxXXX_size(ap_manager_t* man, boxXXX_t* a);

/* 2. Control of internal representation */
void boxXXX_minimize(ap_manager_t* man, boxXXX_t* a);
void boxXXX_canonicalize(ap_manager_t* man, boxXXX_t* a);
void boxXXX_approximate(ap_manager_t* man, boxXXX_t* a, int algorithm);

/* 3. Printing */
void boxXXX_print(FILE* stream,
	       ap_manager_t* man,boxXXX_t* a,char** name_of_dim);
void boxXXX_dump(FILE* stream, ap_manager_t* man, boxXXX_t* a);
void boxXXX_printdiff(FILE* stream,
		   ap_manager_t* man, boxXXX_t* a, boxXXX_t* b,
		   char** name_of_dim);

/* 4. Serialization */
ap_membuf_t boxXXX_serialize_raw(ap_manager_t* man, boxXXX_t* a);
boxXXX_t* boxXXX_deserialize_raw(ap_manager_t* man, void* ptr);

#ifdef __cplusplus
}
#endif

#endif
