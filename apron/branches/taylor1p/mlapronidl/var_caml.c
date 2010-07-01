/* File generated from var.idl */

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


#include "apron_caml.h"

#define camlidl_ml2c_var_ap_var_t(v,c,ctx) camlidl_apron_var_ptr_ml2c(v,c)

#define camlidl_c2ml_var_ap_var_t(c,ctx) camlidl_apron_var_ptr_c2ml(c)

value camlidl_var_ap_var_of_string(
	value _v_name)
{
  char *name; /*in*/
  ap_var_t _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  name = String_val(_v_name);
  _res = ap_var_of_string(name);
  _vres = camlidl_c2ml_var_ap_var_t(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_var_ap_var_compare(
	value _v_var1,
	value _v_var2)
{
  ap_var_t var1; /*in*/
  ap_var_t var2; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_var_ap_var_t(_v_var1, &var1, _ctx);
  camlidl_ml2c_var_ap_var_t(_v_var2, &var2, _ctx);
  _res = ap_var_compare(var1, var2);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_var_ap_var_to_string(
	value _v_var)
{
  ap_var_t var; /*in*/
  char *_res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_var_ap_var_t(_v_var, &var, _ctx);
  /* begin user-supplied calling sequence */
_res = ((apron_var_ptr)var)->name;
  /* end user-supplied calling sequence */
  _vres = copy_string(_res);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_var_ap_var_hash(
	value _v_var)
{
  ap_var_t var; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_var_ap_var_t(_v_var, &var, _ctx);
  _res = ap_var_hash(var);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  return _vres;
}

