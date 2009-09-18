/* ********************************************************************** */
/* box_internal.h: abstract lattice of intervals */
/* ********************************************************************** */

#ifndef _BOX_INTERNAL_H_
#define _BOX_INTERNAL_H_

#include <string.h>
#include <stdio.h>

#include "box_config.h"
#include "itv.h"

#ifdef __cplusplus
extern "C" {
#endif

struct box_t {
  itv_t* p;
  size_t intdim;
  size_t realdim;
};

ap_manager_t* box_manager_alloc(void);

typedef struct box_internal_t {
  itv_internal_t* itv;
  itv_t bound_linexpr_internal_itv;
  itv_t bound_linexpr_internal_itv2;
  ap_interval_t* sat_interval_interval;
  itv_t sat_lincons_itv;
  num_t sat_lincons_num;
  itv_t bound_linexpr_itv;
  itv_t meet_lincons_internal_itv;
  itv_t meet_lincons_internal_itv2;
  itv_t meet_lincons_internal_itv3;
  bound_t meet_lincons_internal_bound;
} box_internal_t;

void box_internal_init(box_internal_t* intern);
void box_internal_clear(box_internal_t* intern);

box_internal_t* box_internal_alloc(void);
void box_internal_free(box_internal_t* intern);

/* Initializes some fields of pk from manager */
static inline box_internal_t* box_init_from_manager(ap_manager_t* man, ap_funid_t funid)
{
  box_internal_t* itv = (box_internal_t*)man->internal;
  return itv;
}

#ifdef __cplusplus
}
#endif

#endif
