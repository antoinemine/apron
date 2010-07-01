/* File generated from polka.idl */

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

#include "pk.h"
#include "pkeq.h"
#include "ap_manager.h"
#include "apron_caml.h"
typedef struct pk_internal_t* internal_ptr;
extern void camlidl_apron_manager_funid_ml2c(value, ap_funid_t *);
#define camlidl_ml2c_manager_ap_funid_t(v,c,ctx) camlidl_apron_manager_funid_ml2c(v,c)

extern value camlidl_apron_manager_funid_c2ml(ap_funid_t *);
#define camlidl_c2ml_manager_ap_funid_t(c,ctx) camlidl_apron_manager_funid_c2ml(c)


extern void camlidl_ml2c_manager_struct_ap_funopt_t(value, struct ap_funopt_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_manager_struct_ap_funopt_t(struct ap_funopt_t *, camlidl_ctx _ctx);

extern void camlidl_apron_manager_exc_ml2c(value, ap_exc_t *);
#define camlidl_ml2c_manager_ap_exc_t(v,c,ctx) camlidl_apron_manager_exc_ml2c(v,c)

extern value camlidl_apron_manager_exc_c2ml(ap_exc_t *);
#define camlidl_c2ml_manager_ap_exc_t(c,ctx) camlidl_apron_manager_exc_c2ml(c)


extern void camlidl_ml2c_manager_struct_ap_exclog_t(value, struct ap_exclog_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_manager_struct_ap_exclog_t(struct ap_exclog_t *, camlidl_ctx _ctx);

extern void camlidl_apron_manager_ptr_ml2c(value, ap_manager_ptr *);
#define camlidl_ml2c_manager_ap_manager_ptr(v,c,ctx) camlidl_apron_manager_ptr_ml2c(v,c)

extern value camlidl_apron_manager_ptr_c2ml(ap_manager_ptr *);
#define camlidl_c2ml_manager_ap_manager_ptr(c,ctx) camlidl_apron_manager_ptr_c2ml(c)


void camlidl_ml2c_polka_internal_ptr(value _v1, internal_ptr * _c2, camlidl_ctx _ctx)
{
  *_c2 = *((internal_ptr *) Bp_val(_v1));
}

value camlidl_c2ml_polka_internal_ptr(internal_ptr * _c2, camlidl_ctx _ctx)
{
value _v1;
  _v1 = camlidl_alloc((sizeof(internal_ptr) + sizeof(value) - 1) / sizeof(value), Abstract_tag);
  *((internal_ptr *) Bp_val(_v1)) = *_c2;
  return _v1;
}

value camlidl_polka_pk_manager_alloc_loose(value _unit)
{
  ap_manager_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  /* begin user-supplied calling sequence */

_res = pk_manager_alloc(false);
{ ap_exc_t i;
for (i=1; i<AP_EXC_SIZE; i++){
ap_manager_set_abort_if_exception(_res,i,false);
}}

  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_manager_ap_manager_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_polka_pk_manager_alloc_strict(value _unit)
{
  ap_manager_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  /* begin user-supplied calling sequence */
_res = pk_manager_alloc(true);
{ ap_exc_t i;
for (i=1; i<AP_EXC_SIZE; i++){
ap_manager_set_abort_if_exception(_res,i,false);
}}

  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_manager_ap_manager_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_polka_pk_manager_alloc_equalities(value _unit)
{
  ap_manager_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  /* begin user-supplied calling sequence */
_res = pkeq_manager_alloc();
{ ap_exc_t i;
for (i=1; i<AP_EXC_SIZE; i++){
ap_manager_set_abort_if_exception(_res,i,false);
}}

  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_manager_ap_manager_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_polka_manager_get_internal(
	value _v_man)
{
  ap_manager_ptr man; /*in*/
  internal_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  /* begin user-supplied calling sequence */
_res = (internal_ptr)man->internal;
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_polka_internal_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_polka_pk_set_max_coeff_size(
	value _v_pk,
	value _v_size)
{
  internal_ptr pk; /*in*/
  unsigned int size; /*in*/
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_polka_internal_ptr(_v_pk, &pk, _ctx);
  size = Int_val(_v_size);
  pk_set_max_coeff_size(pk, size);
  camlidl_free(_ctx);
  return Val_unit;
}

value camlidl_polka_pk_set_approximate_max_coeff_size(
	value _v_pk,
	value _v_size)
{
  internal_ptr pk; /*in*/
  unsigned int size; /*in*/
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_polka_internal_ptr(_v_pk, &pk, _ctx);
  size = Int_val(_v_size);
  pk_set_approximate_max_coeff_size(pk, size);
  camlidl_free(_ctx);
  return Val_unit;
}

value camlidl_polka_pk_get_max_coeff_size(
	value _v_pk)
{
  internal_ptr pk; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_polka_internal_ptr(_v_pk, &pk, _ctx);
  _res = pk_get_max_coeff_size(pk);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_polka_pk_get_approximate_max_coeff_size(
	value _v_pk)
{
  internal_ptr pk; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_polka_internal_ptr(_v_pk, &pk, _ctx);
  _res = pk_get_approximate_max_coeff_size(pk);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  return _vres;
}

