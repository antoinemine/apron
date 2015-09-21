/* ************************************************************************* */
/* reducedproduct.h: generic reduced product of N domains */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#ifndef _AP_REDUCEDPRODUCT_H_
#define _AP_REDUCEDPRODUCT_H_

#include <stdlib.h>
#include <stdio.h>

#include "ap_global0.h"

#ifdef __cplusplus
extern "C" {
#endif

/* (internal) abstract value for a product */
typedef struct ap_reducedproduct_t {
  bool reduced; /* is the product reduced ? */
  void* p[0];   /* array of internal abstract values, 
		   the size of which is specified in the associated 
		   ap_reducedproduct_internal_t */
} ap_reducedproduct_t;

/* internal field of manager */
typedef struct ap_reducedproduct_internal_t {
  void (*reduce)(ap_manager_t*, ap_reducedproduct_t*);             
    /* reduce function */
  void (*approximate)(ap_manager_t*, ap_reducedproduct_t*, int n); 
    /* approximate function */
  char* library;               /* (constructed) library name  */
  char* version;               /* (constructed) library version */
  size_t size;                 /* size of the product */
  ap_manager_t* tmanagers[0];  /* of size size */
} ap_reducedproduct_internal_t;

/* ============================================================ */
/* IV. Allocating a manager */
/* ============================================================ */

ap_manager_t* ap_reducedproduct_manager_alloc
(
 char* library, /* library name */
 ap_manager_t** tab,  /* Array of managers */
 size_t size,         /* size of array */
 void (*reduce)(ap_manager_t*, ap_reducedproduct_t*),            
   /* reduce function */
 void (*approximate)(ap_manager_t*, ap_reducedproduct_t*, int n) 
   /* approximate function */
);

/* ============================================================ */
/* V. Extra functions */
/* ============================================================ */

void** ap_reducedproduct_decompose(ap_manager_t* manager, bool destructive, ap_reducedproduct_t* a);
  /* Decompose an product abstract values into an array of abstract values,
     the size of which corresponds to the size of the (product) manager.

     Be cautious: no type checking is performed.

     If destructive is true, then a is freed at the end of the call.

     Otherwise, abstract values are copied into the result, and the argument
     and the result do not share any more information.
  */

ap_reducedproduct_t* ap_reducedproduct_compose(ap_manager_t* manager, bool destructive, void** tabs);
  /* Create a product abstract value from an array of size the size of the (product) manager.

     Be cautious: no type checking is performed.

     If destructive is true, the array tabs cannot be used any more, but may
     require a call to free() if dynamically allocated.

     Otherwise, abstract values are copied into the result, and the argument
     and the result do not share any more information.
  */

#ifdef __cplusplus
}
#endif

#endif
