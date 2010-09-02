/* ********************************************************************** */
/* box_representation.c: general management */
/* ********************************************************************** */

#ifndef _BOX_REPRESENTATION_H_
#define _BOX_REPRESENTATION_H_

#include "box.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Internal functions */
box_t* box_alloc(size_t intdim, size_t realdim);
void box_init(box_t* a);
void box_set_bottom(box_t* a);
void box_set_top(box_t* a);
void box_set(box_t* a, box_t* b);

/* 1. Memory */
box_t* box_copy(ap_manager_t* man, box_t* a);
void box_free(ap_manager_t* man, box_t* a);
size_t box_size(ap_manager_t* man, box_t* a);

/* 2. Control of internal representation */
void box_minimize(ap_manager_t* man, box_t* a);
void box_canonicalize(ap_manager_t* man, box_t* a);
void box_approximate(ap_manager_t* man, box_t* a, int algorithm);

/* 3. Printing */
void box_print(FILE* stream,
	       ap_manager_t* man,box_t* a,char** name_of_dim);
void box_dump(FILE* stream, ap_manager_t* man, box_t* a);
void box_printdiff(FILE* stream,
		   ap_manager_t* man, box_t* a, box_t* b,
		   char** name_of_dim);

/* 4. Serialization */
ap_membuf_t box_serialize_raw(ap_manager_t* man, box_t* a);
box_t* box_deserialize_raw(ap_manager_t* man, void* ptr);

#ifdef __cplusplus
}
#endif

#endif
