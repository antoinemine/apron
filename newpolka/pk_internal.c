/* ************************************************************************* */
/* internal.c: internal manager */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution */

#include "pk_config.h"
#include "pk_internal.h"
#include "pk_vector.h"
#include "pk_matrix.h"
#include "pk_satmat.h"

/* ********************************************************************** */
/* I. Constructor and destructor for internal */
/* ********************************************************************** */

/* Initialize pk with size maxdims */

void pk_internal_init(pk_internal_t* pk, size_t maxdims)
{
  size_t i;

  pk->exn = AP_EXC_NONE;

  pk->maxdims = maxdims;
  pk->maxcols = maxdims+3;
  
  pk->vector_numintp = vector_alloc(pk->maxcols);
  pk->vector_mpqp = malloc( (maxdims+3)*sizeof(mpq_t));
  for (i=0; i<maxdims+3; i++)
    mpq_init(pk->vector_mpqp[i]);

  pk->vector_tmp = vector_alloc(5);

  pk->matrix_dimp = malloc(pk->maxdims*sizeof(ap_dim_t));
  numint_init(pk->matrix_acc);
  numint_init(pk->matrix_prod);

  /* pk->cherni_bitstringp = bitstring_alloc(bitindex_size(pk->maxrows));*/
  pk->cherni_intp = (int*)malloc(pk->maxcols * sizeof(int));
  numint_init(pk->cherni_prod);

  pk->itv = itv_internal_alloc();
  bound_init(pk->poly_bound);
  itv_init(pk->poly_itv);
  itv_linexpr_init(&pk->poly_itv_linexpr,maxdims);
  itv_lincons_init(&pk->poly_itv_lincons);
  numrat_init(pk->poly_numrat);
  pk->poly_numintp = vector_alloc(pk->maxcols);
  pk->poly_numintp2 = vector_alloc(pk->maxcols);
  pk->poly_dimp = malloc(pk->maxdims*sizeof(ap_dim_t));
  pk->poly_dimp2 = malloc(pk->maxdims*sizeof(ap_dim_t));
  pk->poly_fold_dimp = malloc(pk->maxdims*sizeof(ap_dim_t));
  /* pk->poly_bitstringp = bitstring_alloc(bitindex_size(pk->maxrows)); */
  pk->poly_matspecial = pk_matrix_alloc(1,pk->maxcols,true);
  numint_init(pk->poly_prod);
}

/* Allocates pk and initializes it with a default size */
pk_internal_t* pk_internal_alloc(bool strict)
{
  pk_internal_t* pk = (pk_internal_t*)malloc(sizeof(pk_internal_t));

  pk->strict = strict;
  pk->dec = strict ? 3 : 2;
  pk->max_coeff_size = 0;
  pk->approximate_max_coeff_size = 2;

  pk_internal_init(pk,10);

  return pk;
}

/* Clear pk */
void pk_internal_clear(pk_internal_t* pk)
{
  size_t i;

  if (pk->vector_numintp) vector_free(pk->vector_numintp,pk->maxcols);
  pk->vector_numintp = 0;

  if (pk->vector_tmp) vector_free(pk->vector_tmp,5);
  pk->vector_tmp = 0;

  if (pk->vector_mpqp){ 
    for (i=0; i<pk->maxdims+3; i++)
      mpq_clear(pk->vector_mpqp[i]);
    free(pk->vector_mpqp);
  }
  pk->vector_mpqp = 0;

  if (pk->matrix_dimp) free(pk->matrix_dimp);
  pk->matrix_dimp = 0;

  numint_clear(pk->matrix_acc);
  numint_clear(pk->matrix_prod);

  /*  if (pk->cherni_bitstringp) free(pk->cherni_bitstringp);
  pk->cherni_bitstringp = 0;
  */
  if (pk->cherni_intp) free(pk->cherni_intp);
  pk->cherni_intp = 0;

  numint_clear(pk->cherni_prod);

  if (pk->itv) itv_internal_free(pk->itv);
  pk->itv = 0;
  bound_clear(pk->poly_bound);
  itv_clear(pk->poly_itv);
  itv_linexpr_clear(&pk->poly_itv_linexpr);
  itv_lincons_clear(&pk->poly_itv_lincons);
  numrat_clear(pk->poly_numrat);
  if (pk->poly_numintp) vector_free(pk->poly_numintp, pk->maxcols);
  pk->poly_numintp = 0; 

  if (pk->poly_numintp2) vector_free(pk->poly_numintp2, pk->maxcols);
  pk->poly_numintp2 = 0;

  if (pk->poly_dimp) free(pk->poly_dimp);
  pk->poly_dimp = 0;
  if (pk->poly_dimp2) free(pk->poly_dimp2);
  pk->poly_dimp2 = 0;
  if (pk->poly_fold_dimp) free(pk->poly_fold_dimp);
  pk->poly_fold_dimp = 0;

  /* if (pk->poly_bitstringp) bitstring_free(pk->poly_bitstringp);
  pk->poly_bitstringp = 0; 
  */
  if (pk->poly_matspecial) pk_matrix_free(pk->poly_matspecial);
  pk->poly_matspecial = 0;

  numint_clear(pk->poly_prod);

  pk->maxdims = 0;
  pk->maxrows = 0;
  pk->maxcols = 0;
}

/* Clear and free pk */
void pk_internal_free(pk_internal_t* pk)
{
  pk_internal_clear(pk);
  free(pk);
}

/* Reallocate pk */
void pk_internal_realloc(pk_internal_t* pk, size_t maxdims)
{
  if (pk->maxdims != maxdims){
    pk_internal_clear(pk);
    pk_internal_init(pk,maxdims);
  }
}

/* Reallocate pk only if a bigger dimension is required */
void pk_internal_realloc_lazy(pk_internal_t* pk, size_t maxdims)
{
  if (maxdims > pk->maxdims){
    pk_internal_clear(pk);
    pk_internal_init(pk,maxdims);
  }
}

/* ********************************************************************** */
/* II. Options */
/* ********************************************************************** */

pk_internal_t* pk_manager_get_internal(ap_manager_t* man){
  return man->internal;
}

void pk_set_max_coeff_size(pk_internal_t* pk, size_t size){
  pk->max_coeff_size = size;
}
void pk_set_approximate_max_coeff_size(pk_internal_t* pk, size_t size){  
  pk->approximate_max_coeff_size = size;
}
size_t pk_get_max_coeff_size(pk_internal_t* pk){
  return pk->max_coeff_size;
}
size_t pk_get_approximate_max_coeff_size(pk_internal_t* pk){
  return pk->approximate_max_coeff_size;
}

/* ********************************************************************** */
/* III. Initialization from manager */
/* ********************************************************************** */

ap_manager_t* pk_manager_alloc(bool strict)
{
  size_t i;
  pk_internal_t* pk;
  ap_manager_t* man;
  void** funptr;

  pk = pk_internal_alloc(strict);
  pk_set_approximate_max_coeff_size(pk, 1);
  man = ap_manager_alloc(strict ? "polka, strict mode" : "polka, loose mode",
#if defined(NUMINT_LONGINT)
		      "3.0 with NUMINT_LONGINT",
#elif defined(NUMINT_LONGLONGINT)
		      "3.0 with NUMINT_LONGLONGINT",
#elif defined(NUMINT_MPZ)
		      "3.0 with NUMINT_MPZ",
#else
#error "here"
#endif
		      pk, (void (*)(void*))pk_internal_free);
  funptr = man->funptr;
  
  funptr[AP_FUNID_COPY] = &pk_copy;
  funptr[AP_FUNID_FREE] = &pk_free;
  funptr[AP_FUNID_ASIZE] = &pk_size;
  funptr[AP_FUNID_MINIMIZE] = &pk_minimize;
  funptr[AP_FUNID_CANONICALIZE] = &pk_canonicalize;
  funptr[AP_FUNID_HASH] = &pk_hash;
  funptr[AP_FUNID_APPROXIMATE] = &pk_approximate;
  funptr[AP_FUNID_FPRINT] = &pk_fprint;
  funptr[AP_FUNID_FPRINTDIFF] = &pk_fprintdiff;
  funptr[AP_FUNID_FDUMP] = &pk_fdump;
  funptr[AP_FUNID_SERIALIZE_RAW] = &pk_serialize_raw;
  funptr[AP_FUNID_DESERIALIZE_RAW] = &pk_deserialize_raw;
  funptr[AP_FUNID_BOTTOM] = &pk_bottom;
  funptr[AP_FUNID_TOP] = &pk_top;
  funptr[AP_FUNID_OF_BOX] = &pk_of_box;
  funptr[AP_FUNID_DIMENSION] = &pk_dimension;
  funptr[AP_FUNID_IS_BOTTOM] = &pk_is_bottom;
  funptr[AP_FUNID_IS_TOP] = &pk_is_top;
  funptr[AP_FUNID_IS_LEQ] = &pk_is_leq;
  funptr[AP_FUNID_IS_EQ] = &pk_is_eq;
  funptr[AP_FUNID_IS_DIMENSION_UNCONSTRAINED] = &pk_is_dimension_unconstrained;
  funptr[AP_FUNID_SAT_INTERVAL] = &pk_sat_interval;
  funptr[AP_FUNID_SAT_LINCONS] = &pk_sat_lincons;
  funptr[AP_FUNID_SAT_TCONS] = &pk_sat_tcons;
  funptr[AP_FUNID_BOUND_DIMENSION] = &pk_bound_dimension;
  funptr[AP_FUNID_BOUND_LINEXPR] = &pk_bound_linexpr;
  funptr[AP_FUNID_BOUND_TEXPR] = &pk_bound_texpr;
  funptr[AP_FUNID_TO_BOX] = &pk_to_box;
  funptr[AP_FUNID_TO_LINCONS_ARRAY] = &pk_to_lincons_array;
  funptr[AP_FUNID_TO_TCONS_ARRAY] = &pk_to_tcons_array;
  funptr[AP_FUNID_TO_GENERATOR_ARRAY] = &pk_to_generator_array;
  funptr[AP_FUNID_MEET] = &pk_meet;
  funptr[AP_FUNID_MEET_ARRAY] = &pk_meet_array;
  funptr[AP_FUNID_MEET_LINCONS_ARRAY] = &pk_meet_lincons_array;
  funptr[AP_FUNID_MEET_TCONS_ARRAY] = &pk_meet_tcons_array;
  funptr[AP_FUNID_JOIN] = &pk_join;
  funptr[AP_FUNID_JOIN_ARRAY] = &pk_join_array;
  funptr[AP_FUNID_ADD_RAY_ARRAY] = &pk_add_ray_array;
  funptr[AP_FUNID_ASSIGN_LINEXPR_ARRAY] = &pk_assign_linexpr_array;
  funptr[AP_FUNID_SUBSTITUTE_LINEXPR_ARRAY] = &pk_substitute_linexpr_array;
  funptr[AP_FUNID_ASSIGN_TEXPR_ARRAY] = &pk_assign_texpr_array;
  funptr[AP_FUNID_SUBSTITUTE_TEXPR_ARRAY] = &pk_substitute_texpr_array;
  funptr[AP_FUNID_ADD_DIMENSIONS] = &pk_add_dimensions;
  funptr[AP_FUNID_REMOVE_DIMENSIONS] = &pk_remove_dimensions;
  funptr[AP_FUNID_PERMUTE_DIMENSIONS] = &pk_permute_dimensions;
  funptr[AP_FUNID_FORGET_ARRAY] = &pk_forget_array;
  funptr[AP_FUNID_EXPAND] = &pk_expand;
  funptr[AP_FUNID_FOLD] = &pk_fold;
  funptr[AP_FUNID_WIDENING] = &pk_widening;
  funptr[AP_FUNID_CLOSURE] = &pk_closure;

  ap_manager_set_abort_if_exception(man, AP_EXC_TIMEOUT, false);
  ap_manager_set_abort_if_exception(man, AP_EXC_OUT_OF_SPACE, false);
  ap_manager_set_abort_if_exception(man, AP_EXC_OVERFLOW, false);
  
  return man;
}

