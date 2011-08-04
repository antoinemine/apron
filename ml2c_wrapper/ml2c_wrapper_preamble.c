#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <caml/memory.h>
#include <caml/callback.h>
#include <caml/camlidlruntime.h>

#include "apron_caml.h"
#include "ml2c_wrapper.h"

static wrapper_t* wrapper_alloc(value v) 
{
  wrapper_t* x = (wrapper_t*) malloc(sizeof(wrapper_t));
  if (x == NULL) return NULL;
  x->val = v;
  caml_register_generational_global_root(&(x->val));
  return x;
}

static void wrapper_modify(wrapper_t* x, value v)
{
  if (x != NULL)		   
    caml_modify_generational_global_root(&(x->val), v);
}

void wrapper_free(ap_manager_t* man, wrapper_t* x) 
{
  if (x != NULL) {
    caml_remove_generational_global_root(&(x->val));
    free(x);
  }
}

