/* File generated from linexpr0.idl */

/* This file is part of the APRON Library, released under LGPL license.
  Please read the COPYING file packaged in the distribution  */

#include <stddef.h>
#include <string.h>
#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/fail.h>
#include <caml/callback.h>
#ifdef Custom_tag
#include <caml/custom.h>
#include <caml/bigarray.h>
#endif
#include <caml/camlidlruntime.h>


#include <limits.h>
#include "ap_linexpr0.h"
#include "apron_caml.h"

extern void camlidl_apron_scalar_ml2c(value, ap_scalar_t *);
#define camlidl_ml2c_scalar_ap_scalar_t(v,c,ctx) camlidl_apron_scalar_ml2c(v,c)

extern value camlidl_apron_scalar_c2ml(ap_scalar_t *);
#define camlidl_c2ml_scalar_ap_scalar_t(c,ctx) camlidl_apron_scalar_c2ml(c)


extern void camlidl_ml2c_scalar_ap_scalar_ptr(value, ap_scalar_ptr *, camlidl_ctx _ctx);
extern value camlidl_c2ml_scalar_ap_scalar_ptr(ap_scalar_ptr *, camlidl_ctx _ctx);

extern void camlidl_ml2c_scalar_struct_ap_scalar_array_t(value, struct ap_scalar_array_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_scalar_struct_ap_scalar_array_t(struct ap_scalar_array_t *, camlidl_ctx _ctx);

extern void camlidl_ml2c_interval_struct_ap_interval_t(value, struct ap_interval_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_interval_struct_ap_interval_t(struct ap_interval_t *, camlidl_ctx _ctx);

extern void camlidl_ml2c_interval_ap_interval_ptr(value, ap_interval_ptr *, camlidl_ctx _ctx);
extern value camlidl_c2ml_interval_ap_interval_ptr(ap_interval_ptr *, camlidl_ctx _ctx);

extern void camlidl_ml2c_interval_struct_ap_interval_array_t(value, struct ap_interval_array_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_interval_struct_ap_interval_array_t(struct ap_interval_array_t *, camlidl_ctx _ctx);

extern void camlidl_ml2c_coeff_struct_ap_coeff_t(value, struct ap_coeff_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_coeff_struct_ap_coeff_t(struct ap_coeff_t *, camlidl_ctx _ctx);

extern void camlidl_ml2c_dim_ap_dim_t(value, ap_dim_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_dim_ap_dim_t(ap_dim_t *, camlidl_ctx _ctx);

extern void camlidl_ml2c_dim_struct_ap_dimchange_t(value, struct ap_dimchange_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_dim_struct_ap_dimchange_t(struct ap_dimchange_t *, camlidl_ctx _ctx);

extern void camlidl_apron_dimchange_ml2c(value, ap_dimchange_t *);
#define camlidl_ml2c_dim_ap_dimchange_t(v,c,ctx) camlidl_apron_dimchange_ml2c(v,c)

extern value camlidl_apron_dimchange_c2ml(ap_dimchange_t *);
#define camlidl_c2ml_dim_ap_dimchange_t(c,ctx) camlidl_apron_dimchange_c2ml(c)


extern void camlidl_ml2c_dim_struct_ap_dimchange2_t(value, struct ap_dimchange2_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_dim_struct_ap_dimchange2_t(struct ap_dimchange2_t *, camlidl_ctx _ctx);

extern void camlidl_ml2c_dim_struct_ap_dimperm_t(value, struct ap_dimperm_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_dim_struct_ap_dimperm_t(struct ap_dimperm_t *, camlidl_ctx _ctx);

extern void camlidl_ml2c_dim_struct_ap_dimension_t(value, struct ap_dimension_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_dim_struct_ap_dimension_t(struct ap_dimension_t *, camlidl_ctx _ctx);

#define camlidl_ml2c_linexpr0_ap_linexpr0_ptr(v,c,ctx) camlidl_apron_linexpr0_ptr_ml2c(v,c)

#define camlidl_c2ml_linexpr0_ap_linexpr0_ptr(c,ctx) camlidl_apron_linexpr0_ptr_c2ml(c)

value camlidl_linexpr0_ap_linexpr0_make(
	value _v_size)
{
  int *size; /*in*/
  ap_linexpr0_ptr _res;
  value _v1;
  int _c2;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  if (_v_size == Val_int(0)) {
    size = NULL;
  } else {
    _v1 = Field(_v_size, 0);
    size = &_c2;
    _c2 = Int_val(_v1);
  }
  /* begin user-supplied calling sequence */

if (size && *size<0) caml_failwith("Linexpr0.make: negative optional size");
if (size){
_res = ap_linexpr0_alloc(AP_LINEXPR_DENSE, *size);
} else {
_res = ap_linexpr0_alloc(AP_LINEXPR_SPARSE, 0);
}

  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_linexpr0_ap_linexpr0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_linexpr0_ap_linexpr0_minimize(
	value _v_a)
{
  ap_linexpr0_ptr a; /*in*/
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_linexpr0_ap_linexpr0_ptr(_v_a, &a, _ctx);
  ap_linexpr0_minimize(a);
  camlidl_free(_ctx);
  return Val_unit;
}

value camlidl_linexpr0_ap_linexpr0_copy(
	value _v_a)
{
  ap_linexpr0_ptr a; /*in*/
  ap_linexpr0_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_linexpr0_ap_linexpr0_ptr(_v_a, &a, _ctx);
  _res = ap_linexpr0_copy(a);
  _vres = camlidl_c2ml_linexpr0_ap_linexpr0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_linexpr0_ap_linexpr0_compare(
	value _v_a,
	value _v_b)
{
  ap_linexpr0_ptr a; /*in*/
  ap_linexpr0_ptr b; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_linexpr0_ap_linexpr0_ptr(_v_a, &a, _ctx);
  camlidl_ml2c_linexpr0_ap_linexpr0_ptr(_v_b, &b, _ctx);
  _res = ap_linexpr0_compare(a, b);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_linexpr0_ap_linexpr0_hash(
	value _v_a)
{
  ap_linexpr0_ptr a; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_linexpr0_ap_linexpr0_ptr(_v_a, &a, _ctx);
  _res = ap_linexpr0_hash(a);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_linexpr0_ap_linexpr0_get_size(
	value _v_a)
{
  ap_linexpr0_ptr a; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_linexpr0_ap_linexpr0_ptr(_v_a, &a, _ctx);
  /* begin user-supplied calling sequence */
_res = a->size;
  /* end user-supplied calling sequence */
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_linexpr0_ap_linexpr0_get_cst(
	value _v_a)
{
  ap_linexpr0_ptr a; /*in*/
  struct ap_coeff_t *_res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_linexpr0_ap_linexpr0_ptr(_v_a, &a, _ctx);
  /* begin user-supplied calling sequence */
_res = &a->cst;
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_coeff_struct_ap_coeff_t(&*_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_linexpr0_ap_linexpr0_get_coeff(
	value _v_a,
	value _v_dim)
{
  ap_linexpr0_ptr a; /*in*/
  int dim; /*in*/
  struct ap_coeff_t _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_linexpr0_ap_linexpr0_ptr(_v_a, &a, _ctx);
  dim = Int_val(_v_dim);
  /* begin user-supplied calling sequence */

{
bool b;
ap_coeff_init(&_res,AP_COEFF_SCALAR);
if (dim<0) goto ap_linexpr0_get_coeff_exit;
b = ap_linexpr0_get_coeff(&_res,a,(ap_dim_t)dim);
   if (b){
ap_linexpr0_get_coeff_exit:
ap_coeff_clear(&_res);
caml_failwith("Linexpr0.get_coeff: out of bound dimension");
}
}
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_coeff_struct_ap_coeff_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
ap_coeff_clear(&_res);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_linexpr0_ap_linexpr0_set_cst(
	value _v_a,
	value _v_coeff)
{
  ap_linexpr0_ptr a; /*in*/
  struct ap_coeff_t *coeff; /*in*/
  struct ap_coeff_t _c1;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_linexpr0_ap_linexpr0_ptr(_v_a, &a, _ctx);
  coeff = &_c1;
  camlidl_ml2c_coeff_struct_ap_coeff_t(_v_coeff, &_c1, _ctx);
  /* begin user-supplied calling sequence */
ap_linexpr0_set_cst(a,coeff);
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  return Val_unit;
}

value camlidl_linexpr0_ap_linexpr0_set_coeff(
	value _v_a,
	value _v_dim,
	value _v_coeff)
{
  ap_linexpr0_ptr a; /*in*/
  int dim; /*in*/
  struct ap_coeff_t *coeff; /*in*/
  struct ap_coeff_t _c1;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_linexpr0_ap_linexpr0_ptr(_v_a, &a, _ctx);
  dim = Int_val(_v_dim);
  coeff = &_c1;
  camlidl_ml2c_coeff_struct_ap_coeff_t(_v_coeff, &_c1, _ctx);
  /* begin user-supplied calling sequence */

{
bool b;
if (dim<0) goto ap_linexpr0_set_coeff_exit;
b = ap_linexpr0_set_coeff(a,(ap_dim_t)dim,coeff);
 if (b){
ap_linexpr0_set_coeff_exit:
caml_failwith("Linexpr0.set_coeff: out of bound dimension");
}
}

  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  return Val_unit;
}


value camlidl_expr0_linexpr0_iter(value _v_closure, value _v_linexpr0)
{
CAMLparam2(_v_closure,_v_linexpr0);
CAMLlocal2(_v_dim,_v_coeff);
size_t i;
ap_dim_t dim;
ap_coeff_t* pcoeff;
ap_linexpr0_t* linexpr0;

camlidl_apron_linexpr0_ptr_ml2c(_v_linexpr0,&linexpr0);
ap_linexpr0_ForeachLinterm(linexpr0,i,dim,pcoeff){
_v_dim = Val_int(dim);
_v_coeff = camlidl_c2ml_coeff_struct_ap_coeff_t(pcoeff,NULL);
 callback2(_v_closure,_v_coeff,_v_dim);
}
CAMLreturn(Val_unit);
}

