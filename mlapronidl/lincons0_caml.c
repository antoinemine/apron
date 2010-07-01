/* File generated from lincons0.idl */

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
#include "ap_lincons0.h"
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

extern void camlidl_apron_linexpr0_ptr_ml2c(value, ap_linexpr0_ptr *);
#define camlidl_ml2c_linexpr0_ap_linexpr0_ptr(v,c,ctx) camlidl_apron_linexpr0_ptr_ml2c(v,c)

extern value camlidl_apron_linexpr0_ptr_c2ml(ap_linexpr0_ptr *);
#define camlidl_c2ml_linexpr0_ap_linexpr0_ptr(c,ctx) camlidl_apron_linexpr0_ptr_c2ml(c)



void camlidl_apron_lincons0_ml2c(value v,ap_lincons0_t* cons, camlidl_ctx _ctx)
{
value _v_linexpr0;
value _v_constyp;
value _v_scalar;

_v_linexpr0 = Field(v,0);
camlidl_apron_linexpr0_ptr_ml2c(_v_linexpr0,&cons->linexpr0);
_v_constyp = Field(v,1);
if (Is_long(_v_constyp)){
switch (Int_val(_v_constyp)){
    case 0:
case 1:
case 2:
cons->constyp = Int_val(_v_constyp);
break;
case 3:
cons->constyp = 4;
break;
default:
abort();
}
cons->scalar = NULL;
}
else {
switch (Tag_val(_v_constyp)){
case 0:
cons->constyp = AP_CONS_EQMOD;
_v_scalar = Field(_v_constyp,0);
cons->scalar = (ap_scalar_t *)camlidl_malloc(sizeof(ap_scalar_t),_ctx);
camlidl_apron_scalar_ml2c(_v_scalar,cons->scalar);
break;
default:
abort();
}
}
}
value camlidl_apron_lincons0_c2ml(ap_lincons0_t* cons)
{
value vres;
value _v[3];
_v[0] = _v[1] = _v[2] = 0;

Begin_roots_block(_v, 3)
_v[0] = camlidl_apron_linexpr0_ptr_c2ml(&cons->linexpr0);
switch(cons->constyp){
case AP_CONS_EQ:
case AP_CONS_SUPEQ:
case AP_CONS_SUP:
_v[1] = Val_int(cons->constyp);
break;
case AP_CONS_DISEQ:
_v[1] = Val_int(3);
break;
case AP_CONS_EQMOD:
assert(cons->scalar!=NULL);
_v[2] = camlidl_apron_scalar_c2ml(cons->scalar);
_v[1] = camlidl_alloc_small(1, 0);
Field(_v[1],0) = _v[2];
ap_scalar_free(cons->scalar); cons->scalar = NULL;
break;
}
vres = camlidl_alloc_small(2, 0);
Field(vres, 0) = _v[0];
Field(vres, 1) = _v[1];
End_roots()
return vres;
}

#define camlidl_ml2c_lincons0_ap_lincons0_t(v,c,ctx) camlidl_apron_lincons0_ml2c(v,c,ctx)

#define camlidl_c2ml_lincons0_ap_lincons0_t(c,ctx) camlidl_apron_lincons0_c2ml(c)

void camlidl_ml2c_lincons0_struct_ap_lincons0_array_t(value _v1, struct ap_lincons0_array_t * _c2, camlidl_ctx _ctx)
{
  mlsize_t _c3;
  mlsize_t _c4;
  value _v5;
  _c3 = Wosize_val(_v1);
  (*_c2).p = camlidl_malloc(_c3 * sizeof(ap_lincons0_t ), _ctx);
  for (_c4 = 0; _c4 < _c3; _c4++) {
    _v5 = Field(_v1, _c4);
    camlidl_ml2c_lincons0_ap_lincons0_t(_v5, &(*_c2).p[_c4], _ctx);
  }
  (*_c2).size = _c3;
}

value camlidl_c2ml_lincons0_struct_ap_lincons0_array_t(struct ap_lincons0_array_t * _c1, camlidl_ctx _ctx)
{
  value _v2;
  mlsize_t _c3;
  value _v4;
  _v2 = camlidl_alloc((*_c1).size, 0);
  Begin_root(_v2)
    for (_c3 = 0; _c3 < (*_c1).size; _c3++) {
      _v4 = camlidl_c2ml_lincons0_ap_lincons0_t(&(*_c1).p[_c3], _ctx);
      modify(&Field(_v2, _c3), _v4);
    }
  End_roots()
  return _v2;
}

