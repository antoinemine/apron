/* ********************************************************************** */
/* boxXXX_internal.c */
/* ********************************************************************** */

#include <string.h>
#include <stdio.h>

#include "boxXXX_internal.h"
#include "boxXXX.h"
#include "eitvXXX.h"

void boxXXX_internal_init(boxXXX_internal_t* intern)
{
  num_internal_init(intern->num);
  eitvXXX_init(intern->bound_linexpr_internal_itv);
  eitvXXX_init(intern->bound_linexpr_internal_itv2);
  eitvXXX_init(intern->sat_interval_eitv);
  eitvXXX_init(intern->sat_lincons_itv);
  numXXX_init(intern->sat_lincons_num);
  eitvXXX_init(intern->bound_linexpr_itv);
  eitvXXX_init(intern->meet_lincons_internal_itv);
  eitvXXX_init(intern->meet_lincons_internal_itv2);
  eitvXXX_init(intern->meet_lincons_internal_itv3);
  boundXXX_init(intern->meet_lincons_internal_bound);
}
void boxXXX_internal_clear(boxXXX_internal_t* intern)
{
  num_internal_clear(intern->num);
  eitvXXX_clear(intern->bound_linexpr_internal_itv);
  eitvXXX_clear(intern->bound_linexpr_internal_itv2);
  eitvXXX_clear(intern->sat_interval_eitv);
  eitvXXX_clear(intern->sat_lincons_itv);
  numXXX_clear(intern->sat_lincons_num);
  eitvXXX_clear(intern->bound_linexpr_itv);
  eitvXXX_clear(intern->meet_lincons_internal_itv);
  eitvXXX_clear(intern->meet_lincons_internal_itv2);
  eitvXXX_clear(intern->meet_lincons_internal_itv3);
  boundXXX_clear(intern->meet_lincons_internal_bound);
}

boxXXX_internal_t* boxXXX_internal_alloc(void)
{
  boxXXX_internal_t* intern = malloc(sizeof(boxXXX_internal_t));
  boxXXX_internal_init(intern);
  return intern;
}
void boxXXX_internal_free(boxXXX_internal_t* intern)
{
  boxXXX_internal_clear(intern);
  free(intern);
}

ap_manager_t* boxXXX_manager_alloc(void)
{
  size_t i;
  boxXXX_internal_t* itv;
  ap_manager_t* man;
  void** funptr;

  if (!ap_fpu_init()) {
    fprintf(stderr,"boxXXX_manager_alloc cannot change the FPU rounding mode\n");
  }

  itv = boxXXX_internal_alloc();
  man = ap_manager_alloc("box" "XXX", "1.0",
			 itv, (void (*)(void*))boxXXX_internal_free);
  funptr = man->funptr;

  funptr[AP_FUNID_COPY] = &boxXXX_copy;
  funptr[AP_FUNID_FREE] = &boxXXX_free;
  funptr[AP_FUNID_ASIZE] = &boxXXX_size;
  funptr[AP_FUNID_MINIMIZE] = &boxXXX_minimize;
  funptr[AP_FUNID_CANONICALIZE] = &boxXXX_canonicalize;
  funptr[AP_FUNID_HASH] = &boxXXX_hash;
  funptr[AP_FUNID_APPROXIMATE] = &boxXXX_approximate;
  funptr[AP_FUNID_FPRINT] = &boxXXX_fprint;
  funptr[AP_FUNID_FPRINTDIFF] = &boxXXX_fprintdiff;
  funptr[AP_FUNID_FDUMP] = &boxXXX_fdump;
  funptr[AP_FUNID_SERIALIZE_RAW] = &boxXXX_serialize_raw;
  funptr[AP_FUNID_DESERIALIZE_RAW] = &boxXXX_deserialize_raw;
  funptr[AP_FUNID_BOTTOM] = &boxXXX_bottom;
  funptr[AP_FUNID_TOP] = &boxXXX_top;
  funptr[AP_FUNID_OF_BOX] = &boxXXX_of_box;
  funptr[AP_FUNID_DIMENSION] = &boxXXX_dimension;
  funptr[AP_FUNID_IS_BOTTOM] = &boxXXX_is_bottom;
  funptr[AP_FUNID_IS_TOP] = &boxXXX_is_top;
  funptr[AP_FUNID_IS_LEQ] = &boxXXX_is_leq;
  funptr[AP_FUNID_IS_EQ] = &boxXXX_is_eq;
  funptr[AP_FUNID_IS_DIMENSION_UNCONSTRAINED] = &boxXXX_is_dimension_unconstrained;
  funptr[AP_FUNID_SAT_INTERVAL] = &boxXXX_sat_interval;
  funptr[AP_FUNID_SAT_LINCONS] = &boxXXX_sat_lincons;
  funptr[AP_FUNID_SAT_TCONS] = &boxXXX_sat_tcons;
  funptr[AP_FUNID_BOUND_DIMENSION] = &boxXXX_bound_dimension;
  funptr[AP_FUNID_BOUND_LINEXPR] = &boxXXX_bound_linexpr;
  funptr[AP_FUNID_BOUND_TEXPR] = &boxXXX_bound_texpr;
  funptr[AP_FUNID_TO_BOX] = &boxXXX_to_box;
  funptr[AP_FUNID_TO_LINCONS_ARRAY] = &boxXXX_to_lincons_array;
  funptr[AP_FUNID_TO_TCONS_ARRAY] = &boxXXX_to_tcons_array;
  funptr[AP_FUNID_TO_LINGEN_ARRAY] = &boxXXX_to_lingen_array;
  funptr[AP_FUNID_MEET] = &boxXXX_meet;
  funptr[AP_FUNID_MEET_ARRAY] = &boxXXX_meet_array;
  funptr[AP_FUNID_MEET_LINCONS_ARRAY] = &boxXXX_meet_lincons_array;
  funptr[AP_FUNID_MEET_TCONS_ARRAY] = &boxXXX_meet_tcons_array;
  funptr[AP_FUNID_JOIN] = &boxXXX_join;
  funptr[AP_FUNID_JOIN_ARRAY] = &boxXXX_join_array;
  funptr[AP_FUNID_ADD_RAY_ARRAY] = &boxXXX_add_ray_array;
  funptr[AP_FUNID_ASSIGN_LINEXPR_ARRAY] = &boxXXX_assign_linexpr_array;
  funptr[AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY] = &boxXXX_substitute_linexpr_array;
  funptr[AP_FUNID_ASSIGN_TEXPR_ARRAY] = &boxXXX_assign_texpr_array;
  funptr[AP_FUNID_SUBSTITUTE_TEXPR_ARRAY] = &boxXXX_substitute_texpr_array;
  funptr[AP_FUNID_ADD_DIMENSIONS] = &boxXXX_add_dimensions;
  funptr[AP_FUNID_REMOVE_DIMENSIONS] = &boxXXX_remove_dimensions;
  funptr[AP_FUNID_PERMUTE_DIMENSIONS] = &boxXXX_permute_dimensions;
  funptr[AP_FUNID_FORGET_ARRAY] = &boxXXX_forget_array;
  funptr[AP_FUNID_EXPAND] = &boxXXX_expand;
  funptr[AP_FUNID_FOLD] = &boxXXX_fold;
  funptr[AP_FUNID_WIDENING] = &boxXXX_widening;
  funptr[AP_FUNID_CLOSURE] = &boxXXX_closure;

  for (i=0; i<AP_EXC_SIZE; i++){
    ap_manager_set_abort_if_exception(man, i, false);
  }
  return man;
}
 
