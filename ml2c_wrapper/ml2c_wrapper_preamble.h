/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include <caml/mlvalues.h>
#include "ap_global0.h"

typedef struct wrapper_t {
  value val;
} wrapper_t;

ap_manager_t* manager_alloc(void);

void wrapper_free(ap_manager_t* man, wrapper_t* x);

void wrapper_fprint(FILE* stream,
			 ap_manager_t* man, wrapper_t* a, char** name_of_dim);

void wrapper_fprintdiff(FILE* stream,
			     ap_manager_t* man,
			     wrapper_t* a1, wrapper_t* a2,
			     char** name_of_dim);

void wrapper_fdump(FILE* stream, ap_manager_t* man, wrapper_t* a);

ap_membuf_t wrapper_serialize_raw(ap_manager_t* man, wrapper_t* a);

wrapper_t* wrapper_deserialize_raw(ap_manager_t* man, void* ptr, size_t* size);

