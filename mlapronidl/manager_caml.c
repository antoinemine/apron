/* File generated from manager.idl */

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
#include "ap_manager.h"
#include "apron_caml.h"

void camlidl_apron_manager_funid_ml2c(value _v, enum ap_funid_t* res)
{
int v = Int_val(_v);
if (v+1>=AP_FUNID_CHANGE_ENVIRONMENT)
*res = v+1;
else
*res = v;
}
value camlidl_apron_manager_funid_c2ml(enum ap_funid_t* arg)
{
int n = *arg;
if (n>=0 && n<AP_FUNID_SIZE2 && n!=AP_FUNID_SIZE){
if (n>AP_FUNID_SIZE) n--;
}
else {
invalid_argument("enum ap_funid_t: bad enum value");
}
return Val_int(n);
}
void camlidl_apron_manager_exc_ml2c(value _v, enum ap_exc_t* res)
{
*res = Int_val(_v);
}
value camlidl_apron_manager_exc_c2ml(enum ap_exc_t* arg)
{
int n = *arg;
if (! (n>=AP_EXC_NONE && n<AP_EXC_SIZE)){
invalid_argument("enum ap_exc_t: bad enum value");
}
return Val_int(n);
}

#define camlidl_ml2c_manager_ap_funid_t(v,c,ctx) camlidl_apron_manager_funid_ml2c(v,c)

#define camlidl_c2ml_manager_ap_funid_t(c,ctx) camlidl_apron_manager_funid_c2ml(c)

void camlidl_ml2c_manager_struct_ap_funopt_t(value _v1, struct ap_funopt_t * _c2, camlidl_ctx _ctx)
{
  value _v3;
  value _v4;
  value _v5;
  value _v6;
  value _v7;
  _v3 = Field(_v1, 0);
  (*_c2).algorithm = Int_val(_v3);
  _v4 = Field(_v1, 1);
  (*_c2).timeout = Int_val(_v4);
  _v5 = Field(_v1, 2);
  (*_c2).max_object_size = Int_val(_v5);
  _v6 = Field(_v1, 3);
  (*_c2).flag_exact_wanted = Int_val(_v6);
  _v7 = Field(_v1, 4);
  (*_c2).flag_best_wanted = Int_val(_v7);
}

value camlidl_c2ml_manager_struct_ap_funopt_t(struct ap_funopt_t * _c1, camlidl_ctx _ctx)
{
  value _v2;
  value _v3[5];
  memset(_v3, 0, 5 * sizeof(value));
  Begin_roots_block(_v3, 5)
    _v3[0] = Val_int((*_c1).algorithm);
    _v3[1] = Val_int((*_c1).timeout);
    _v3[2] = Val_int((*_c1).max_object_size);
    _v3[3] = Val_int((*_c1).flag_exact_wanted);
    _v3[4] = Val_int((*_c1).flag_best_wanted);
    _v2 = camlidl_alloc_small(5, 0);
    { mlsize_t _c4;
      for (_c4 = 0; _c4 < 5; _c4++) Field(_v2, _c4) = _v3[_c4];
    }
  End_roots()
  return _v2;
}

#define camlidl_ml2c_manager_ap_exc_t(v,c,ctx) camlidl_apron_manager_exc_ml2c(v,c)

#define camlidl_c2ml_manager_ap_exc_t(c,ctx) camlidl_apron_manager_exc_c2ml(c)

void camlidl_ml2c_manager_struct_ap_exclog_t(value _v1, struct ap_exclog_t * _c2, camlidl_ctx _ctx)
{
  value _v3;
  value _v4;
  value _v5;
  _v3 = Field(_v1, 0);
  camlidl_ml2c_manager_ap_exc_t(_v3, &(*_c2).exn, _ctx);
  _v4 = Field(_v1, 1);
  camlidl_ml2c_manager_ap_funid_t(_v4, &(*_c2).funid, _ctx);
  _v5 = Field(_v1, 2);
  (*_c2).msg = camlidl_malloc_string(_v5, _ctx);
}

value camlidl_c2ml_manager_struct_ap_exclog_t(struct ap_exclog_t * _c1, camlidl_ctx _ctx)
{
  value _v2;
  value _v3[3];
  _v3[0] = _v3[1] = _v3[2] = 0;
  Begin_roots_block(_v3, 3)
    _v3[0] = camlidl_c2ml_manager_ap_exc_t(&(*_c1).exn, _ctx);
    _v3[1] = camlidl_c2ml_manager_ap_funid_t(&(*_c1).funid, _ctx);
    _v3[2] = copy_string((*_c1).msg);
    _v2 = camlidl_alloc_small(3, 0);
    Field(_v2, 0) = _v3[0];
    Field(_v2, 1) = _v3[1];
    Field(_v2, 2) = _v3[2];
  End_roots()
  return _v2;
}

#define camlidl_ml2c_manager_ap_manager_ptr(v,c,ctx) camlidl_apron_manager_ptr_ml2c(v,c)

#define camlidl_c2ml_manager_ap_manager_ptr(c,ctx) camlidl_apron_manager_ptr_c2ml(c)

value camlidl_manager_ap_manager_get_library(
	value _v_man)
{
  ap_manager_ptr man; /*in*/
  char const *_res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  _res = ap_manager_get_library(man);
  _vres = copy_string(_res);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_manager_ap_manager_get_version(
	value _v_man)
{
  ap_manager_ptr man; /*in*/
  char const *_res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  _res = ap_manager_get_version(man);
  _vres = copy_string(_res);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_manager_ap_funopt_make(value _unit)
{
  struct ap_funopt_t *x; /*out*/
  struct ap_funopt_t _c1;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  x = &_c1;
  /* begin user-supplied calling sequence */
ap_funopt_init(x);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_manager_struct_ap_funopt_t(&*x, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_manager_ap_manager_get_funopt(
	value _v_man,
	value _v_funid)
{
  ap_manager_ptr man; /*in*/
  ap_funid_t funid; /*in*/
  struct ap_funopt_t _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_manager_ap_funid_t(_v_funid, &funid, _ctx);
  /* begin user-supplied calling sequence */
if (funid >= AP_FUNID_SIZE) invalid_argument("Manager.get.funopt <funid>: <funid> should be different from Funid_change_environment and Funid_rename_array (no option for remaining identifiers)");
_res = ap_manager_get_funopt(man,funid);

  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_manager_struct_ap_funopt_t(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_manager_ap_manager_set_funopt(
	value _v_man,
	value _v_funid,
	value _v_funopt)
{
  ap_manager_ptr man; /*in*/
  ap_funid_t funid; /*in*/
  struct ap_funopt_t *funopt; /*in*/
  struct ap_funopt_t _c1;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_manager_ap_funid_t(_v_funid, &funid, _ctx);
  funopt = &_c1;
  camlidl_ml2c_manager_struct_ap_funopt_t(_v_funopt, &_c1, _ctx);
  /* begin user-supplied calling sequence */
if (funid >= AP_FUNID_SIZE) invalid_argument("Manager.set.funopt <funid> ...: <funid> should be different from Funid_change_environment and Funid_rename_array (no option for remaining identifiers)");
ap_manager_set_funopt(man,funid,funopt);

  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  return Val_unit;
}

value camlidl_manager_ap_manager_get_flag_exact(
	value _v_man)
{
  ap_manager_ptr man; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  _res = ap_manager_get_flag_exact(man);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_manager_ap_manager_get_flag_best(
	value _v_man)
{
  ap_manager_ptr man; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  _res = ap_manager_get_flag_best(man);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_manager_ap_manager_set_deserialize(
	value _v_man)
{
  ap_manager_ptr man; /*in*/
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  ap_manager_set_deserialize(man);
  camlidl_free(_ctx);
  return Val_unit;
}

value camlidl_manager_ap_manager_get_deserialize(value _unit)
{
  ap_manager_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  _res = ap_manager_get_deserialize();
  _vres = camlidl_c2ml_manager_ap_manager_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

