/* File generated from interval.idl */

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
#include "ap_coeff.h"
#include "apron_caml.h"

extern void camlidl_apron_scalar_ml2c(value, ap_scalar_t *);
#define camlidl_ml2c_scalar_ap_scalar_t(v,c,ctx) camlidl_apron_scalar_ml2c(v,c)

extern value camlidl_apron_scalar_c2ml(ap_scalar_t *);
#define camlidl_c2ml_scalar_ap_scalar_t(c,ctx) camlidl_apron_scalar_c2ml(c)


extern void camlidl_ml2c_scalar_ap_scalar_ptr(value, ap_scalar_ptr *, camlidl_ctx _ctx);
extern value camlidl_c2ml_scalar_ap_scalar_ptr(ap_scalar_ptr *, camlidl_ctx _ctx);

extern void camlidl_ml2c_scalar_struct_ap_scalar_array_t(value, struct ap_scalar_array_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_scalar_struct_ap_scalar_array_t(struct ap_scalar_array_t *, camlidl_ctx _ctx);

void camlidl_ml2c_interval_struct_ap_interval_t(value _v1, struct ap_interval_t * _c2, camlidl_ctx _ctx)
{
  value _v3;
  value _v4;
  _v3 = Field(_v1, 0);
  (*_c2).inf = (ap_scalar_t  *) camlidl_malloc(sizeof(ap_scalar_t ), _ctx);
  camlidl_ml2c_scalar_ap_scalar_t(_v3, &*(*_c2).inf, _ctx);
  _v4 = Field(_v1, 1);
  (*_c2).sup = (ap_scalar_t  *) camlidl_malloc(sizeof(ap_scalar_t ), _ctx);
  camlidl_ml2c_scalar_ap_scalar_t(_v4, &*(*_c2).sup, _ctx);
}

value camlidl_c2ml_interval_struct_ap_interval_t(struct ap_interval_t * _c1, camlidl_ctx _ctx)
{
  value _v2;
  value _v3[2];
  _v3[0] = _v3[1] = 0;
  Begin_roots_block(_v3, 2)
    _v3[0] = camlidl_c2ml_scalar_ap_scalar_t(&*(*_c1).inf, _ctx);
    _v3[1] = camlidl_c2ml_scalar_ap_scalar_t(&*(*_c1).sup, _ctx);
    _v2 = camlidl_alloc_small(2, 0);
    Field(_v2, 0) = _v3[0];
    Field(_v2, 1) = _v3[1];
  End_roots()
  return _v2;
}

void camlidl_ml2c_interval_ap_interval_ptr(value _v1, ap_interval_ptr * _c2, camlidl_ctx _ctx)
{
  (*_c2) = (struct ap_interval_t  *) camlidl_malloc(sizeof(struct ap_interval_t ), _ctx);
  camlidl_ml2c_interval_struct_ap_interval_t(_v1, &*(*_c2), _ctx);
}

value camlidl_c2ml_interval_ap_interval_ptr(ap_interval_ptr * _c2, camlidl_ctx _ctx)
{
value _v1;
  _v1 = camlidl_c2ml_interval_struct_ap_interval_t(&*(*_c2), _ctx);
  return _v1;
}

void camlidl_ml2c_interval_struct_ap_interval_array_t(value _v1, struct ap_interval_array_t * _c2, camlidl_ctx _ctx)
{
  mlsize_t _c3;
  mlsize_t _c4;
  value _v5;
  _c3 = Wosize_val(_v1);
  (*_c2).p = camlidl_malloc(_c3 * sizeof(ap_interval_ptr ), _ctx);
  for (_c4 = 0; _c4 < _c3; _c4++) {
    _v5 = Field(_v1, _c4);
    camlidl_ml2c_interval_ap_interval_ptr(_v5, &(*_c2).p[_c4], _ctx);
  }
  (*_c2).size = _c3;
}

value camlidl_c2ml_interval_struct_ap_interval_array_t(struct ap_interval_array_t * _c1, camlidl_ctx _ctx)
{
  value _v2;
  mlsize_t _c3;
  value _v4;
  _v2 = camlidl_alloc((*_c1).size, 0);
  Begin_root(_v2)
    for (_c3 = 0; _c3 < (*_c1).size; _c3++) {
      _v4 = camlidl_c2ml_interval_ap_interval_ptr(&(*_c1).p[_c3], _ctx);
      modify(&Field(_v2, _c3), _v4);
    }
  End_roots()
  return _v2;
}

