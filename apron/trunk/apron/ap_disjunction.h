/* ************************************************************************* */
/* disjunction.h: generic disjunction of N domains */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#ifndef _AP_DISJUNCTION_H_
#define _AP_DISJUNCTION_H_

#include <stdlib.h>
#include <stdio.h>

#include "ap_global0.h"

#ifdef __cplusplus
extern "C" {
#endif

/* (internal) abstract value for disjunction */
typedef struct ap_disjunction_t {
  size_t size;  /* size of abstract value */
  void** p;   /* array of internal abstract values of size "size" */
} ap_disjunction_t;

/* internal fields of manager */
typedef struct ap_disjunction_internal_t {
  void (*merge)(ap_manager_t*, ap_disjunction_t*); /* merge function, may be
						      NULL */
  char* library;               /* (constructed) library name  */
  ap_manager_t* manager;   /* Manager of the base domain */
} ap_disjunction_internal_t;

/* ============================================================ */
/* IV. Allocating a manager */
/* ============================================================ */

ap_manager_t* ap_disjunction_manager_alloc
(
    ap_manager_t* underlying,  /* Array of managers */
    void (*merge)(ap_manager_t*, ap_disjunction_t*)
    /* merge function */
);

/* ============================================================ */
/* V. Extra functions */
/* ============================================================ */

ap_lincons0_array_t ap_disjunction_to_lincons0_set(ap_manager_t* manager,
						   ap_disjunction_t* a);


/* ap_abstract0_t */
void** ap_disjunction_decompose(ap_manager_t* manager, bool destructive,
				ap_disjunction_t* a, size_t* psize);
  /* Decompose a disjunctive abstract value into an array of abstract values
     of size *psize

     Be cautious: no type checking is performed.

     If destructive is true, then a is freed at the end of the call.

     Otherwise, abstract values are copied into the result, the argument and
     the result do not share any more information.
  */

ap_disjunction_t* ap_disjunction_compose(ap_manager_t* manager, bool destructive, void** tabs, size_t size);

  /* Create a disjunctive abstract value from an array of size "size".

     Be cautious: no type checking is performed.

     If destructive is true, the content of the array tabs cannot be used any
     more, but the array still requires a call to free() if dynamically
     allocated.

     Otherwise, abstract values are copied into the result, and the argument
     and the result do not share any more information.
  */

#ifdef __cplusplus
}
#endif

#endif
