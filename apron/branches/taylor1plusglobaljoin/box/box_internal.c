/* ********************************************************************** */
/* box_internal.c */
/* ********************************************************************** */

#include <string.h>
#include <stdio.h>

#include "box_internal.h"
#include "box.h"
#include "itv.h"

void box_internal_init(box_internal_t* intern)
{
  intern->itv = itv_internal_alloc();
  itv_init(intern->bound_linexpr_internal_itv);
  itv_init(intern->bound_linexpr_internal_itv2);
  intern->sat_interval_interval = ap_interval_alloc();
  itv_init(intern->sat_lincons_itv);
  num_init(intern->sat_lincons_num);
  itv_init(intern->bound_linexpr_itv);
  itv_init(intern->meet_lincons_internal_itv);
  itv_init(intern->meet_lincons_internal_itv2);
  itv_init(intern->meet_lincons_internal_itv3);
  bound_init(intern->meet_lincons_internal_bound);
}
void box_internal_clear(box_internal_t* intern)
{
  itv_internal_free(intern->itv); intern->itv=NULL;
  itv_clear(intern->bound_linexpr_internal_itv);
  itv_clear(intern->bound_linexpr_internal_itv2);
  ap_interval_free(intern->sat_interval_interval);
  intern->sat_interval_interval = NULL;
  itv_clear(intern->sat_lincons_itv);
  num_clear(intern->sat_lincons_num);
  itv_clear(intern->bound_linexpr_itv);
  itv_clear(intern->meet_lincons_internal_itv);
  itv_clear(intern->meet_lincons_internal_itv2);
  itv_clear(intern->meet_lincons_internal_itv3);
  bound_clear(intern->meet_lincons_internal_bound);
}

box_internal_t* box_internal_alloc(void)
{
  box_internal_t* intern = malloc(sizeof(box_internal_t));
  box_internal_init(intern);
  return intern;
}
void box_internal_free(box_internal_t* intern)
{
  box_internal_clear(intern);
  free(intern);
}

ap_manager_t* box_manager_alloc(void)
{
  size_t i;
  box_internal_t* itv;
  ap_manager_t* man;
  void** funptr;

  if (!ap_fpu_init()) {
    fprintf(stderr,"box_manager_alloc cannot change the FPU rounding mode\n");
  }

  itv = box_internal_alloc();
  man = ap_manager_alloc("box", "1.0 with " NUM_NAME,
			 itv, (void (*)(void*))box_internal_free);
  funptr = man->funptr;

  funptr[AP_FUNID_COPY] = &box_copy;
  funptr[AP_FUNID_FREE] = &box_free;
  funptr[AP_FUNID_ASIZE] = &box_size;
  funptr[AP_FUNID_MINIMIZE] = &box_minimize;
  funptr[AP_FUNID_CANONICALIZE] = &box_canonicalize;
  funptr[AP_FUNID_HASH] = &box_hash;
  funptr[AP_FUNID_APPROXIMATE] = &box_approximate;
  funptr[AP_FUNID_FPRINT] = &box_fprint;
  funptr[AP_FUNID_FPRINTDIFF] = &box_fprintdiff;
  funptr[AP_FUNID_FDUMP] = &box_fdump;
  funptr[AP_FUNID_SERIALIZE_RAW] = &box_serialize_raw;
  funptr[AP_FUNID_DESERIALIZE_RAW] = &box_deserialize_raw;
  funptr[AP_FUNID_BOTTOM] = &box_bottom;
  funptr[AP_FUNID_TOP] = &box_top;
  funptr[AP_FUNID_OF_BOX] = &box_of_box;
  funptr[AP_FUNID_DIMENSION] = &box_dimension;
  funptr[AP_FUNID_IS_BOTTOM] = &box_is_bottom;
  funptr[AP_FUNID_IS_TOP] = &box_is_top;
  funptr[AP_FUNID_IS_LEQ] = &box_is_leq;
  funptr[AP_FUNID_IS_EQ] = &box_is_eq;
  funptr[AP_FUNID_IS_DIMENSION_UNCONSTRAINED] = &box_is_dimension_unconstrained;
  funptr[AP_FUNID_SAT_INTERVAL] = &box_sat_interval;
  funptr[AP_FUNID_SAT_LINCONS] = &box_sat_lincons;
  funptr[AP_FUNID_SAT_TCONS] = &box_sat_tcons;
  funptr[AP_FUNID_BOUND_DIMENSION] = &box_bound_dimension;
  funptr[AP_FUNID_BOUND_LINEXPR] = &box_bound_linexpr;
  funptr[AP_FUNID_BOUND_TEXPR] = &box_bound_texpr;
  funptr[AP_FUNID_TO_BOX] = &box_to_box;
  funptr[AP_FUNID_TO_LINCONS_ARRAY] = &box_to_lincons_array;
  funptr[AP_FUNID_TO_TCONS_ARRAY] = &box_to_tcons_array;
  funptr[AP_FUNID_TO_GENERATOR_ARRAY] = &box_to_generator_array;
  funptr[AP_FUNID_MEET] = &box_meet;
  funptr[AP_FUNID_MEET_ARRAY] = &box_meet_array;
  funptr[AP_FUNID_MEET_LINCONS_ARRAY] = &box_meet_lincons_array;
  funptr[AP_FUNID_MEET_TCONS_ARRAY] = &box_meet_tcons_array;
  funptr[AP_FUNID_JOIN] = &box_join;
  funptr[AP_FUNID_JOIN_ARRAY] = &box_join_array;
  funptr[AP_FUNID_ADD_RAY_ARRAY] = &box_add_ray_array;
  funptr[AP_FUNID_ASSIGN_LINEXPR_ARRAY] = &box_assign_linexpr_array;
  funptr[AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY] = &box_substitute_linexpr_array;
  funptr[AP_FUNID_ASSIGN_TEXPR_ARRAY] = &box_assign_texpr_array;
  funptr[AP_FUNID_SUBSTITUTE_TEXPR_ARRAY] = &box_substitute_texpr_array;
  funptr[AP_FUNID_ADD_DIMENSIONS] = &box_add_dimensions;
  funptr[AP_FUNID_REMOVE_DIMENSIONS] = &box_remove_dimensions;
  funptr[AP_FUNID_PERMUTE_DIMENSIONS] = &box_permute_dimensions;
  funptr[AP_FUNID_FORGET_ARRAY] = &box_forget_array;
  funptr[AP_FUNID_EXPAND] = &box_expand;
  funptr[AP_FUNID_FOLD] = &box_fold;
  funptr[AP_FUNID_WIDENING] = &box_widening;
  funptr[AP_FUNID_CLOSURE] = &box_closure;

  for (i=0; i<AP_EXC_SIZE; i++){
    ap_manager_set_abort_if_exception(man, i, false);
  }
  return man;
}
