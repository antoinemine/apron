/* ********************************************************************** */
/* pk.h: Interface of the polka linear relation library  */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#ifndef __PK_H__
#define __PK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ap_manager.h"

typedef enum pk_kind_t {
  PK_LOOSE,  /* loose inequalities only */
  PK_STRICT, /* enables also strict inequalities */
  PK_EQUALITIES      /* allows only equalities */
} pk_kind_t;

ap_manager_t* pk_manager_alloc(pk_kind_t kind, num_discr_t discr);
  /* Allocate a polka manager of kind kind, with internam number
     representation discr */

typedef struct pk_option_t {
  unsigned int max_coeff_size;
    /* Used for overflow exception (in vector_combine) */
  unsigned int approximate_max_coeff_size;
    /* Used in approximation */
  bool expand_lazy;
    /* Lazy or strict policy ? (lazy useful for several successive calls from
       a minimal form) */
  bool forget_lazy;
    /* Lazy useful in case of (successive) projection */
  bool remove_dimensions_lazy;
  bool op_lazy;
    /* Lazy or strict policy for minimization/conversion in other operations
       ? */
  bool strong_normalization;
  /* if true, normalize equalities, lines, and strict constraints, and uses
     it for testing equality */
  bool widening_affine;
  /* if true, affine (rather than linear) widening: the constant coefficient
     is not treatd as the other one */
} pk_option_t;

pk_option_t* pk_manager_option_ref(ap_manager_t* man);
  /* Get a reference to the structure storing options.
     Flags can then be modified in-place */

#ifdef __cplusplus
}
#endif

#endif
