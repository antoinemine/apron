/* File generated from coeff.idl */

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

#define Scalar AP_COEFF_SCALAR
#define Interval AP_COEFF_INTERVAL

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

void camlidl_ml2c_coeff_struct_ap_coeff_t(value _v1, struct ap_coeff_t * _c2, camlidl_ctx _ctx)
{
  value _v3;
  value _v4;
  switch (Tag_val(_v1)) {
  case 0: /* Scalar */
    (*_c2).discr = Scalar;
    _v3 = Field(_v1, 0);
    (*_c2).val.scalar = (ap_scalar_t  *) camlidl_malloc(sizeof(ap_scalar_t ), _ctx);
    camlidl_ml2c_scalar_ap_scalar_t(_v3, &*(*_c2).val.scalar, _ctx);
    break;
  case 1: /* Interval */
    (*_c2).discr = Interval;
    _v4 = Field(_v1, 0);
    (*_c2).val.interval = (struct ap_interval_t  *) camlidl_malloc(sizeof(struct ap_interval_t ), _ctx);
    camlidl_ml2c_interval_struct_ap_interval_t(_v4, &*(*_c2).val.interval, _ctx);
    break;
  }
}

value camlidl_c2ml_coeff_struct_ap_coeff_t(struct ap_coeff_t * _c1, camlidl_ctx _ctx)
{
  value _v2;
  value _v3;
  value _v4;
  switch ((*_c1).discr) {
  case Scalar:
    _v3 = camlidl_c2ml_scalar_ap_scalar_t(&*(*_c1).val.scalar, _ctx);
    Begin_root(_v3)
      _v2 = camlidl_alloc_small(1, 0);
      Field(_v2, 0) = _v3;
    End_roots()
    break;
  case Interval:
    _v4 = camlidl_c2ml_interval_struct_ap_interval_t(&*(*_c1).val.interval, _ctx);
    Begin_root(_v4)
      _v2 = camlidl_alloc_small(1, 1);
      Field(_v2, 0) = _v4;
    End_roots()
    break;
  default:
    invalid_argument("struct ap_coeff_t: bad discriminant for union ");
  }
  return _v2;
}

