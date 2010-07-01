/* File generated from scalar.idl */

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


#define HAS_MPFR 1
#include <limits.h>
#include "ap_coeff.h"
#include "gmp_caml.h"
#include "apron_caml.h"


void camlidl_apron_scalar_ml2c(value v, struct ap_scalar_t* scalar)
{
value v2 = Field(v,0);
scalar->discr = Tag_val(v);
switch (scalar->discr) {
case 0: /* DOUBLE */
scalar->val.dbl = Double_val(v2);
break;
case 1: /* MPQ */
scalar->val.mpq = (mpq_ptr)(Data_custom_val(v2));
break;
case 2: /* MPFR */
scalar->val.mpfr = (mpfr_ptr)(Data_custom_val(v2));
break;
default:
caml_failwith("unknown scalar discriminant in camlidl_apron_scalar_ml2c");
}
return;
}
value camlidl_apron_scalar_c2ml(struct ap_scalar_t* scalar)
{
value v,v2;
v2 = Val_unit;
Begin_root(v2);
switch(scalar->discr){
case AP_SCALAR_DOUBLE:
v2 = caml_copy_double(scalar->val.dbl);
break;
case AP_SCALAR_MPQ:
{
mpq_t mpq;
mpq_ptr mpq_ptr = mpq;
mpq_init(mpq);
mpq_set(mpq,scalar->val.mpq);
v2 = camlidl_mpq_ptr_c2ml(&mpq_ptr);
}
break;
case AP_SCALAR_MPFR:
{
mpfr_t mpfr;
mpfr_ptr mpfr_ptr = mpfr;
mpfr_init2(mpfr,mpfr_get_prec(scalar->val.mpfr));
mpfr_set(mpfr,scalar->val.mpfr,GMP_RNDU /* exact */);
v2 = camlidl_mpfr_ptr_c2ml(&mpfr_ptr);
}
break;
default:
caml_failwith("unknown scalar discriminant in camlidl_apron_scalar_c2ml");
}
v = alloc_small(1,scalar->discr);
Field(v,0) = v2;
End_roots();
return v;
}

#define camlidl_ml2c_scalar_ap_scalar_t(v,c,ctx) camlidl_apron_scalar_ml2c(v,c)

#define camlidl_c2ml_scalar_ap_scalar_t(c,ctx) camlidl_apron_scalar_c2ml(c)

void camlidl_ml2c_scalar_ap_scalar_ptr(value _v1, ap_scalar_ptr * _c2, camlidl_ctx _ctx)
{
  (*_c2) = (ap_scalar_t  *) camlidl_malloc(sizeof(ap_scalar_t ), _ctx);
  camlidl_ml2c_scalar_ap_scalar_t(_v1, &*(*_c2), _ctx);
}

value camlidl_c2ml_scalar_ap_scalar_ptr(ap_scalar_ptr * _c2, camlidl_ctx _ctx)
{
value _v1;
  _v1 = camlidl_c2ml_scalar_ap_scalar_t(&*(*_c2), _ctx);
  return _v1;
}

void camlidl_ml2c_scalar_struct_ap_scalar_array_t(value _v1, struct ap_scalar_array_t * _c2, camlidl_ctx _ctx)
{
  mlsize_t _c3;
  mlsize_t _c4;
  value _v5;
  _c3 = Wosize_val(_v1);
  (*_c2).p = camlidl_malloc(_c3 * sizeof(ap_scalar_ptr ), _ctx);
  for (_c4 = 0; _c4 < _c3; _c4++) {
    _v5 = Field(_v1, _c4);
    camlidl_ml2c_scalar_ap_scalar_ptr(_v5, &(*_c2).p[_c4], _ctx);
  }
  (*_c2).size = _c3;
}

value camlidl_c2ml_scalar_struct_ap_scalar_array_t(struct ap_scalar_array_t * _c1, camlidl_ctx _ctx)
{
  value _v2;
  mlsize_t _c3;
  value _v4;
  _v2 = camlidl_alloc((*_c1).size, 0);
  Begin_root(_v2)
    for (_c3 = 0; _c3 < (*_c1).size; _c3++) {
      _v4 = camlidl_c2ml_scalar_ap_scalar_ptr(&(*_c1).p[_c3], _ctx);
      modify(&Field(_v2, _c3), _v4);
    }
  End_roots()
  return _v2;
}

