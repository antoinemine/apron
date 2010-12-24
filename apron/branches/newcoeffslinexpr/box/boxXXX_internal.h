/* ********************************************************************** */
/* boxXXX_internal.h: abstract lattice of intervals */
/* ********************************************************************** */

#ifndef _BOX_XXX_INTERNAL_H_
#define _BOX_XXX_INTERNAL_H_

#include <string.h>
#include <stdio.h>

#include "boxXXX.h"
#include "eitvXXX.h"

#ifdef __cplusplus
extern "C" {
#endif

ap_manager_t* boxXXX_manager_alloc(void);

typedef struct boxXXX_internal_t {
  num_internal_t num;
  eitvXXX_t bound_linexpr_internal_itv;
  eitvXXX_t bound_linexpr_internal_itv2;
  eitvXXX_t sat_interval_eitv;
  eitvXXX_t sat_lincons_itv;
  numXXX_t sat_lincons_num;
  eitvXXX_t bound_linexpr_itv;
  eitvXXX_t meet_lincons_internal_itv;
  eitvXXX_t meet_lincons_internal_itv2;
  eitvXXX_t meet_lincons_internal_itv3;
  boundXXX_t meet_lincons_internal_bound;
} boxXXX_internal_t;

void boxXXX_internal_init(boxXXX_internal_t* intern);
void boxXXX_internal_clear(boxXXX_internal_t* intern);

boxXXX_internal_t* boxXXX_internal_alloc(void);
void boxXXX_internal_free(boxXXX_internal_t* intern);

/* Initializes some fields of pk from manager */
static inline boxXXX_internal_t* boxXXX_init_from_manager(ap_manager_t* man, ap_funid_t funid)
{
  boxXXX_internal_t* itv = (boxXXX_internal_t*)man->internal;
  return itv;
}

/* Other internal functions */
boxXXX_t* boxXXX_alloc(size_t intdim, size_t realdim);
void boxXXX_init(boxXXX_t* a);
void boxXXX_set_bottom(boxXXX_t* a);
void boxXXX_set_top(boxXXX_t* a);
void boxXXX_set(boxXXX_t* a, boxXXX_t* b);

#ifdef __cplusplus
}
#endif

#endif
