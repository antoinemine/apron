/* File generated from dim.idl */

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
#include "ap_dimension.h"
#include "caml/mlvalues.h"
#include "apron_caml.h"

void camlidl_ml2c_dim_ap_dim_t(value _v1, ap_dim_t * _c2, camlidl_ctx _ctx)
{
  (*_c2) = Int_val(_v1);
}

value camlidl_c2ml_dim_ap_dim_t(ap_dim_t * _c2, camlidl_ctx _ctx)
{
value _v1;
  _v1 = Val_int((*_c2));
  return _v1;
}

extern void camlidl_ml2c_dim_struct_ap_dimchange_t(value, struct ap_dimchange_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_dim_struct_ap_dimchange_t(struct ap_dimchange_t *, camlidl_ctx _ctx);

#define camlidl_ml2c_dim_ap_dimchange_t(v,c,ctx) camlidl_apron_dimchange_ml2c(v,c)

#define camlidl_c2ml_dim_ap_dimchange_t(c,ctx) camlidl_apron_dimchange_c2ml(c)

void camlidl_ml2c_dim_struct_ap_dimchange2_t(value _v1, struct ap_dimchange2_t * _c2, camlidl_ctx _ctx)
{
  value _v3;
  value _v4;
  value _v5;
  value _v6;
  _v3 = Field(_v1, 0);
  if (_v3 == Val_int(0)) {
    (*_c2).add = NULL;
  } else {
    _v4 = Field(_v3, 0);
    (*_c2).add = (ap_dimchange_t  *) camlidl_malloc(sizeof(ap_dimchange_t ), _ctx);
    camlidl_ml2c_dim_ap_dimchange_t(_v4, &*(*_c2).add, _ctx);
  }
  _v5 = Field(_v1, 1);
  if (_v5 == Val_int(0)) {
    (*_c2).remove = NULL;
  } else {
    _v6 = Field(_v5, 0);
    (*_c2).remove = (ap_dimchange_t  *) camlidl_malloc(sizeof(ap_dimchange_t ), _ctx);
    camlidl_ml2c_dim_ap_dimchange_t(_v6, &*(*_c2).remove, _ctx);
  }
}

value camlidl_c2ml_dim_struct_ap_dimchange2_t(struct ap_dimchange2_t * _c1, camlidl_ctx _ctx)
{
  value _v2;
  value _v3[2];
  value _v4;
  value _v5;
  _v3[0] = _v3[1] = 0;
  Begin_roots_block(_v3, 2)
    if ((*_c1).add == NULL) {
      _v3[0] = Val_int(0);
    } else {
      _v4 = camlidl_c2ml_dim_ap_dimchange_t(&*(*_c1).add, _ctx);
      Begin_root(_v4)
        _v3[0] = camlidl_alloc_small(1, 0);
        Field(_v3[0], 0) = _v4;
      End_roots();
    }
    if ((*_c1).remove == NULL) {
      _v3[1] = Val_int(0);
    } else {
      _v5 = camlidl_c2ml_dim_ap_dimchange_t(&*(*_c1).remove, _ctx);
      Begin_root(_v5)
        _v3[1] = camlidl_alloc_small(1, 0);
        Field(_v3[1], 0) = _v5;
      End_roots();
    }
    _v2 = camlidl_alloc_small(2, 0);
    Field(_v2, 0) = _v3[0];
    Field(_v2, 1) = _v3[1];
  End_roots()
  return _v2;
}

void camlidl_ml2c_dim_struct_ap_dimperm_t(value _v1, struct ap_dimperm_t * _c2, camlidl_ctx _ctx)
{
  mlsize_t _c3;
  mlsize_t _c4;
  value _v5;
  _c3 = Wosize_val(_v1);
  (*_c2).dim = camlidl_malloc(_c3 * sizeof(unsigned int ), _ctx);
  for (_c4 = 0; _c4 < _c3; _c4++) {
    _v5 = Field(_v1, _c4);
    (*_c2).dim[_c4] = Int_val(_v5);
  }
  (*_c2).size = _c3;
}

value camlidl_c2ml_dim_struct_ap_dimperm_t(struct ap_dimperm_t * _c1, camlidl_ctx _ctx)
{
  value _v2;
  mlsize_t _c3;
  value _v4;
  _v2 = camlidl_alloc((*_c1).size, 0);
  for (_c3 = 0; _c3 < (*_c1).size; _c3++) {
    _v4 = Val_int((*_c1).dim[_c3]);
    modify(&Field(_v2, _c3), _v4);
  }
  return _v2;
}

void camlidl_ml2c_dim_struct_ap_dimension_t(value _v1, struct ap_dimension_t * _c2, camlidl_ctx _ctx)
{
  value _v3;
  value _v4;
  _v3 = Field(_v1, 0);
  (*_c2).intdim = Int_val(_v3);
  _v4 = Field(_v1, 1);
  (*_c2).realdim = Int_val(_v4);
}

value camlidl_c2ml_dim_struct_ap_dimension_t(struct ap_dimension_t * _c1, camlidl_ctx _ctx)
{
  value _v2;
  value _v3[2];
  _v3[0] = _v3[1] = 0;
  Begin_roots_block(_v3, 2)
    _v3[0] = Val_int((*_c1).intdim);
    _v3[1] = Val_int((*_c1).realdim);
    _v2 = camlidl_alloc_small(2, 0);
    Field(_v2, 0) = _v3[0];
    Field(_v2, 1) = _v3[1];
  End_roots()
  return _v2;
}

