#include <stdlib.h>
#include <stdio.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/callback.h>
#include <caml/camlidlruntime.h>

#include "apron_caml.h"

typedef struct wrapper_t {
  value val;
} wrapper_t;

static value camlidl_ap_dim_t_c2ml(ap_dim_t* dim)
{
  CAMLparam0();
  CAMLlocal1(v_dim);
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  v_dim = camlidl_c2ml_dim_ap_dim_t(dim, _ctx);
  camlidl_free(_ctx);
  CAMLreturn(v_dim);
} 

static void camlidl_ap_dim_t_ml2c(value v_dim, ap_dim_t* dim)
{
  CAMLparam1(v_dim);
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_dim_ap_dim_t(v_dim, dim, _ctx);
  camlidl_free(_ctx);
  CAMLreturn0;
}

static value camlidl_ap_dimension_t_c2ml(ap_dimension_t* dim)
{
  CAMLparam0();
  CAMLlocal1(v_dim);
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  v_dim = camlidl_c2ml_dim_struct_ap_dimension_t(dim, _ctx);
  camlidl_free(_ctx);
  CAMLreturn(v_dim);
}

static void camlidl_ap_dimension_t_ml2c(value v_dim, ap_dimension_t* dim)
{
  CAMLparam1(v_dim);
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_dim_struct_ap_dimension_t(v_dim, dim, _ctx);
  camlidl_free(_ctx);
  CAMLreturn0;
}

static value camlidl_ap_dimperm_t_c2ml(ap_dimperm_t* perm)
{
  CAMLparam0();
  CAMLlocal1(v_perm);
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  v_perm = camlidl_c2ml_dim_struct_ap_dimperm_t(perm, _ctx);
  camlidl_free(_ctx);
  CAMLreturn(v_perm);
}

static void camlidl_ap_dimperm_t_ml2c(value v_perm, ap_dimperm_t* perm)
{
  CAMLparam1(v_perm);
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_dim_struct_ap_dimperm_t(v_perm, perm, _ctx);
  camlidl_free(_ctx);
  CAMLreturn0;
}

static void camlidl_ap_tcons0_array_t_ml2c(value v_array, ap_tcons0_array_t* array)
{
  CAMLparam1(v_array);
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_apron_tcons0_array_t_ml2c(v_array, array, _ctx);
  camlidl_free(_ctx);
  CAMLreturn0;
}

/*
static value optional_wrapper_t_c2ml(wrapper_t *a) {
  CAMLparam0();
  CAMLlocal1(res);
  if (a == NULL) 
    res = Val_int(0);
  else {
    res = caml_alloc(1,0);
    Store_field(res, 0, a->val);
  }
  CAMLreturn(res);
}
*/

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
  caml_remove_generational_global_root(&(x->val));
  free(x);
}

