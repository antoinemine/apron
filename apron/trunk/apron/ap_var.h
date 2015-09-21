/* ********************************************************************** */
/* ap_var.h: variables (strings or user-defined) */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#ifndef _AP_VAR_H_
#define _AP_VAR_H_

#ifdef __cplusplus
extern "C" {
#endif


/* ====================================================================== */
/* Datatype */
/* ====================================================================== */

/* The abstract type ap_var_t is
   equipped with a total ordering function, a copy function, 
   and a free function.

   The parametrization is done via a global variable pointing to an 
   ap_var_operations_t structure.
 */

typedef void* ap_var_t;

typedef struct ap_var_operations_t {
  int (*compare)(ap_var_t v1, ap_var_t v2); /* Total ordering function */
  int (*hash)(ap_var_t v);                  /* Hash function */
  ap_var_t (*copy)(ap_var_t var);           /* Duplication function */
  void (*free)(ap_var_t var);               /* Deallocation function */
  char* (*to_string)(ap_var_t var);         /* Conversion to a dynamically allocated string */
} ap_var_operations_t;

extern ap_var_operations_t ap_var_operations_default;
  /* default manager: ap_var_t are char* */

extern ap_var_operations_t* ap_var_operations;
/* points to manager in use, by default ap_var_operations_default */

#ifdef __cplusplus
}
#endif

#endif
