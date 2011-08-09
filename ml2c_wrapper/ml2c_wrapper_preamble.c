#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <caml/memory.h>
#include <caml/callback.h>
#include <caml/camlidlruntime.h>

#include "apron_caml.h"
#include "ml2c_wrapper.h"

static void ap_coeff_ptr_nonfinalize(value v)
{
  switch(Tag_val(v)){
  case AP_SCALAR_D:
    Custom_ops_val(Field(v,0)) = &camlidl_custom_eitvD_nonfinal;
    break;
  case AP_SCALAR_MPQ:
    Custom_ops_val(Field(v,0)) = &camlidl_custom_eitvMPQ_nonfinal;
    break;
  case AP_SCALAR_MPFR:
    Custom_ops_val(Field(v,0)) = &camlidl_custom_eitvMPFR_nonfinal;
    break;
  default:
    abort();
  }
}

static void ap_linexpr0_ptr_nonfinalize(value v)
{
  switch(Tag_val(v)){
  case AP_SCALAR_D:
    Custom_ops_val(Field(v,0)) = &camlidl_custom_ap_linexprD_nonfinal;
    break;
  case AP_SCALAR_MPQ:
    Custom_ops_val(Field(v,0)) = &camlidl_custom_ap_linexprMPQ_nonfinal;
    break;
  case AP_SCALAR_MPFR:
    Custom_ops_val(Field(v,0)) = &camlidl_custom_ap_linexprMPFR_nonfinal;
    break;
  default:
    abort();
  }
}

static void ap_linexpr0_array_ptr_nonfinalize(value v)
{
  switch(Tag_val(v)){
  case AP_SCALAR_D:
    Custom_ops_val(Field(v,0)) = &camlidl_custom_ap_linexprD_array_nonfinal;
    break;
  case AP_SCALAR_MPQ:
    Custom_ops_val(Field(v,0)) = &camlidl_custom_ap_linexprMPQ_array_nonfinal;
    break;
  case AP_SCALAR_MPFR:
    Custom_ops_val(Field(v,0)) = &camlidl_custom_ap_linexprMPFR_array_nonfinal;
    break;
  default:
    abort();
  }
}

static void ap_lincons0_ptr_nonfinalize(value v)
{
  switch(Tag_val(v)){
  case AP_SCALAR_D:
    Custom_ops_val(Field(v,0)) = &camlidl_custom_ap_linconsD_nonfinal;
    break;
  case AP_SCALAR_MPQ:
    Custom_ops_val(Field(v,0)) = &camlidl_custom_ap_linconsMPQ_nonfinal;
    break;
  case AP_SCALAR_MPFR:
    Custom_ops_val(Field(v,0)) = &camlidl_custom_ap_linconsMPFR_nonfinal;
    break;
  default:
    abort();
  }
}

static void ap_lincons0_array_ptr_nonfinalize(value v)
{
  switch(Tag_val(v)){
  case AP_SCALAR_D:
    Custom_ops_val(Field(v,0)) = &camlidl_custom_ap_linconsD_array_nonfinal;
    break;
  case AP_SCALAR_MPQ:
    Custom_ops_val(Field(v,0)) = &camlidl_custom_ap_linconsMPQ_array_nonfinal;
    break;
  case AP_SCALAR_MPFR:
    Custom_ops_val(Field(v,0)) = &camlidl_custom_ap_linconsMPFR_array_nonfinal;
    break;
  default:
    abort();
  }
}

static void ap_lingen0_ptr_nonfinalize(value v)
{
  switch(Tag_val(v)){
  case AP_SCALAR_D:
    Custom_ops_val(Field(v,0)) = &camlidl_custom_ap_lingenD_nonfinal;
    break;
  case AP_SCALAR_MPQ:
    Custom_ops_val(Field(v,0)) = &camlidl_custom_ap_lingenMPQ_nonfinal;
    break;
  case AP_SCALAR_MPFR:
    Custom_ops_val(Field(v,0)) = &camlidl_custom_ap_lingenMPFR_nonfinal;
    break;
  default:
    abort();
  }
}

static void ap_lingen0_array_ptr_nonfinalize(value v)
{
  switch(Tag_val(v)){
  case AP_SCALAR_D:
    Custom_ops_val(Field(v,0)) = &camlidl_custom_ap_lingenD_array_nonfinal;
    break;
  case AP_SCALAR_MPQ:
    Custom_ops_val(Field(v,0)) = &camlidl_custom_ap_lingenMPQ_array_nonfinal;
    break;
  case AP_SCALAR_MPFR:
    Custom_ops_val(Field(v,0)) = &camlidl_custom_ap_lingenMPFR_array_nonfinal;
    break;
  default:
    abort();
  }
}

static void ap_texpr0_ptr_nonfinalize(value v)
{
  Custom_ops_val(v) = &camlidl_apron_custom_texpr0_ptr_nonfinal;
}

static void ap_texpr0_array_t_nonfinalize(value v)
{
  mlsize_t i;
  mlsize_t size = Wosize_val(v);
  for (i = 0; i < size; i++) 
    ap_texpr0_ptr_nonfinalize(Field(v,i));
}

static void ap_tcons0_t_nonfinalize(value v)
{
  ap_texpr0_ptr_nonfinalize(Field(v,0));
  Custom_ops_val(Field(v,2)) = &camlidl_custom_mpq_nonfinal;
}

static void ap_tcons0_array_t_nonfinalize(value v)
{
  mlsize_t i;
  mlsize_t size = Wosize_val(v);
  for (i = 0; i < size; i++) 
    ap_tcons0_t_nonfinalize(Field(v,i));
}

static wrapper_t* wrapper_alloc(value v) 
{
  wrapper_t* x = (wrapper_t*) malloc(sizeof(wrapper_t));
  if (x == NULL) return NULL;
  x->val = v;
  caml_register_generational_global_root(&(x->val));
  return x;
}

static void wrapper_modify(wrapper_t* x, value v)
{
  if (x != NULL)		   
    caml_modify_generational_global_root(&(x->val), v);
}

void wrapper_free(ap_manager_t* man, wrapper_t* x) 
{
  if (x != NULL) {
    caml_remove_generational_global_root(&(x->val));
    free(x);
  }
}
