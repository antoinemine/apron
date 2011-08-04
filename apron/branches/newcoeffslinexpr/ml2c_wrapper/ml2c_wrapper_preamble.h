/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#include <caml/mlvalues.h>
#include "ap_global0.h"

typedef struct wrapper_t {
  value val;
} wrapper_t;

void wrapper_free(ap_manager_t* man, wrapper_t* x);
