/* ************************************************************************* */
/* disjunction.h: generic disjunction of N domains */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

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
  void* p[0];   /* array of internal abstract values of size "size" */
} ap_disjunction_t;

/* internal fields of manager */
typedef struct ap_disjunction_internal_t {
	 /* merge function */
  void (*merge)(ap_manager_t*, ap_disjunction_t*);
     /* widening function */
  void (*widening)(ap_manager_t*, ap_disjunction_t*, ap_disjunction_t*);
     /* approximate function */
  void (*approximate)(ap_manager_t*, ap_disjunction_t*, int n);
  char* library;               /* (constructed) library name  */
  char* version;               /* (constructed) library version */
  ap_manager_t* manager;   /* Manager of the base domain */
} ap_disjunction_internal_t;

/* ============================================================ */
/* IV. Allocating a manager */
/* ============================================================ */

ap_manager_t* ap_disjunction_manager_alloc
(
 char* library, /* library name */
 ap_manager_t** tab,  /* Array of managers */
 size_t size,         /* size of array */
 void (*merge)(ap_manager_t*, ap_disjunction_t*),
   /* merge function */

 void (*widening)(ap_manager_t*, ap_disjunction_t*, ap_disjunction_t*),
   /* widening function */

 void (*approximate)(ap_manager_t*, ap_disjunction_t*, int n)
   /* approximate function */
);

/* ============================================================ */
/* V. Extra functions */
/* ============================================================ */

/* ap_abstract0_t */ void** ap_disjunction_decompose(ap_manager_t* manager, bool destructive,
									ap_disjunction_t* a, size_t* psize);
/* Decompose a disjunctive abstract value into an array of abstract values of size *psize

     Be cautious: no type checking is performed.

     If destructive is true, then a is freed at the end of the call.

     Otherwise, abstract values are copied into the result, the argument
     and the result do not share any more information.
  */
ap_disjunction_t* ap_disjunction_compose(ap_manager_t* manager, bool destructive, void** tabs
							/*ap_abstract0_t** tabs, size_t size */);
  /* Create a disjunctive abstract value from an array of size "size".

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
