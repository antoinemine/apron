/* File generated from abstract0.idl */

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
#include "ap_expr0.h"
#include "ap_abstract0.h"
#include "caml/callback.h"
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


extern void camlidl_apron_lincons0_ml2c(value, ap_lincons0_t *, camlidl_ctx);
#define camlidl_ml2c_lincons0_ap_lincons0_t(v,c,ctx) camlidl_apron_lincons0_ml2c(v,c,ctx)

extern value camlidl_apron_lincons0_c2ml(ap_lincons0_t *);
#define camlidl_c2ml_lincons0_ap_lincons0_t(c,ctx) camlidl_apron_lincons0_c2ml(c)


extern void camlidl_ml2c_lincons0_struct_ap_lincons0_array_t(value, struct ap_lincons0_array_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_lincons0_struct_ap_lincons0_array_t(struct ap_lincons0_array_t *, camlidl_ctx _ctx);

extern int camlidl_ml2c_generator0_enum_gentyp(value);
extern value camlidl_c2ml_generator0_enum_gentyp(int);

extern int camlidl_transl_table_generator0_enum_gentyp[];

extern void camlidl_ml2c_generator0_struct_ap_generator0_t(value, struct ap_generator0_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_generator0_struct_ap_generator0_t(struct ap_generator0_t *, camlidl_ctx _ctx);

extern void camlidl_ml2c_generator0_struct_ap_generator0_array_t(value, struct ap_generator0_array_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_generator0_struct_ap_generator0_array_t(struct ap_generator0_array_t *, camlidl_ctx _ctx);

extern void camlidl_apron_texpr0_ptr_ml2c(value, ap_texpr0_ptr *);
#define camlidl_ml2c_texpr0_ap_texpr0_ptr(v,c,ctx) camlidl_apron_texpr0_ptr_ml2c(v,c)

extern value camlidl_apron_texpr0_ptr_c2ml(ap_texpr0_ptr *);
#define camlidl_c2ml_texpr0_ap_texpr0_ptr(c,ctx) camlidl_apron_texpr0_ptr_c2ml(c)


extern void camlidl_ml2c_texpr0_struct_ap_texpr_op_t(value, ap_texpr_op_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_texpr0_struct_ap_texpr_op_t(ap_texpr_op_t *, camlidl_ctx _ctx);

extern void camlidl_apron_texpr_unop_t_ml2c(value, ap_texpr_unop_t *);
#define camlidl_ml2c_texpr0_ap_texpr_unop_t(v,c,ctx) camlidl_apron_texpr_unop_t_ml2c(v,c)

extern value camlidl_apron_texpr_unop_t_c2ml(ap_texpr_unop_t *);
#define camlidl_c2ml_texpr0_ap_texpr_unop_t(c,ctx) camlidl_apron_texpr_unop_t_c2ml(c)


extern void camlidl_apron_texpr_binop_t_ml2c(value, ap_texpr_binop_t *);
#define camlidl_ml2c_texpr0_ap_texpr_binop_t(v,c,ctx) camlidl_apron_texpr_binop_t_ml2c(v,c)

extern value camlidl_apron_texpr_binop_t_c2ml(ap_texpr_binop_t *);
#define camlidl_c2ml_texpr0_ap_texpr_binop_t(c,ctx) camlidl_apron_texpr_binop_t_c2ml(c)


extern void camlidl_ml2c_texpr0_struct_ap_texpr_rtype_t(value, ap_texpr_rtype_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_texpr0_struct_ap_texpr_rtype_t(ap_texpr_rtype_t *, camlidl_ctx _ctx);

extern void camlidl_apron_texpr_rtype_t_ml2c(value, ap_texpr_rtype_t *);
#define camlidl_ml2c_texpr0_ap_texpr_rtype_t(v,c,ctx) camlidl_apron_texpr_rtype_t_ml2c(v,c)

extern value camlidl_apron_texpr_rtype_t_c2ml(ap_texpr_rtype_t *);
#define camlidl_c2ml_texpr0_ap_texpr_rtype_t(c,ctx) camlidl_apron_texpr_rtype_t_c2ml(c)


extern void camlidl_ml2c_texpr0_struct_ap_texpr_rdir_t(value, ap_texpr_rdir_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_texpr0_struct_ap_texpr_rdir_t(ap_texpr_rdir_t *, camlidl_ctx _ctx);

extern void camlidl_apron_texpr_rdir_t_ml2c(value, ap_texpr_rdir_t *);
#define camlidl_ml2c_texpr0_ap_texpr_rdir_t(v,c,ctx) camlidl_apron_texpr_rdir_t_ml2c(v,c)

extern value camlidl_apron_texpr_rdir_t_c2ml(ap_texpr_rdir_t *);
#define camlidl_c2ml_texpr0_ap_texpr_rdir_t(c,ctx) camlidl_apron_texpr_rdir_t_c2ml(c)


extern void camlidl_apron_tcons0_ml2c(value, ap_tcons0_t *, camlidl_ctx);
#define camlidl_ml2c_tcons0_ap_tcons0_t(v,c,ctx) camlidl_apron_tcons0_ml2c(v,c,ctx)

extern value camlidl_apron_tcons0_c2ml(ap_tcons0_t *);
#define camlidl_c2ml_tcons0_ap_tcons0_t(c,ctx) camlidl_apron_tcons0_c2ml(c)


extern void camlidl_ml2c_tcons0_struct_ap_tcons0_array_t(value, struct ap_tcons0_array_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_tcons0_struct_ap_tcons0_array_t(struct ap_tcons0_array_t *, camlidl_ctx _ctx);

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


#define camlidl_ml2c_abstract0_ap_abstract0_ptr(v,c,ctx) camlidl_apron_abstract0_ptr_ml2c(v,c)

#define camlidl_c2ml_abstract0_ap_abstract0_ptr(c,ctx) camlidl_apron_abstract0_ptr_c2ml(c)

value camlidl_abstract0_ap_abstract0_set_gc(
	value _v_size)
{
  int size; /*in*/
  size = Int_val(_v_size);
  /* begin user-supplied calling sequence */
camlidl_apron_heap = size;
  /* end user-supplied calling sequence */
  return Val_unit;
}

value camlidl_abstract0_ap_abstract0_copy(
	value _v_man,
	value _v_a)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_abstract0_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  _res = ap_abstract0_copy(man, a);
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_size(
	value _v_man,
	value _v_a)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  _res = ap_abstract0_size(man, a);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_minimize(
	value _v_man,
	value _v_a)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  ap_abstract0_minimize(man, a);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract0_ap_abstract0_canonicalize(
	value _v_man,
	value _v_a)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  ap_abstract0_canonicalize(man, a);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract0_ap_abstract0_hash(
	value _v_man,
	value _v_a)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  _res = ap_abstract0_hash(man, a);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_approximate(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  int v; /*in*/
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  v = Int_val(_v_v);
  ap_abstract0_approximate(man, a, v);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract0_ap_abstract0_fdump(
	value _v_man,
	value _v_a)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  /* begin user-supplied calling sequence */
ap_abstract0_fdump(stdout,man, a); fflush(stdout);
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract0_ap_abstract0_bottom(
	value _v_man,
	value _v_v1,
	value _v_v2)
{
  ap_manager_ptr man; /*in*/
  int v1; /*in*/
  int v2; /*in*/
  ap_abstract0_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  v1 = Int_val(_v_v1);
  v2 = Int_val(_v_v2);
  _res = ap_abstract0_bottom(man, v1, v2);
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_top(
	value _v_man,
	value _v_v1,
	value _v_v2)
{
  ap_manager_ptr man; /*in*/
  int v1; /*in*/
  int v2; /*in*/
  ap_abstract0_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  v1 = Int_val(_v_v1);
  v2 = Int_val(_v_v2);
  _res = ap_abstract0_top(man, v1, v2);
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_of_box(
	value _v_man,
	value _v_intdim,
	value _v_realdim,
	value _v_array)
{
  ap_manager_ptr man; /*in*/
  int intdim; /*in*/
  int realdim; /*in*/
  struct ap_interval_array_t array; /*in*/
  ap_abstract0_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  intdim = Int_val(_v_intdim);
  realdim = Int_val(_v_realdim);
  camlidl_ml2c_interval_struct_ap_interval_array_t(_v_array, &array, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_abstract0_of_box(man,intdim,realdim,(ap_interval_t**)array.p);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_dimension(
	value _v_man,
	value _v_a)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  struct ap_dimension_t _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  _res = ap_abstract0_dimension(man, a);
  _vres = camlidl_c2ml_dim_struct_ap_dimension_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_manager(
	value _v_a)
{
  ap_abstract0_ptr a; /*in*/
  ap_manager_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_manager_copy(a->man);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_manager_ap_manager_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_abstract0_ap_abstract0_is_bottom(
	value _v_man,
	value _v_a)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  _res = ap_abstract0_is_bottom(man, a);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_is_top(
	value _v_man,
	value _v_a)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  _res = ap_abstract0_is_top(man, a);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_is_leq(
	value _v_man,
	value _v_a1,
	value _v_a2)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a1; /*in*/
  ap_abstract0_ptr a2; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a1, &a1, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a2, &a2, _ctx);
  _res = ap_abstract0_is_leq(man, a1, a2);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_is_eq(
	value _v_man,
	value _v_a1,
	value _v_a2)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a1; /*in*/
  ap_abstract0_ptr a2; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a1, &a1, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a2, &a2, _ctx);
  _res = ap_abstract0_is_eq(man, a1, a2);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_sat_lincons(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_lincons0_t *v; /*in*/
  int _res;
  ap_lincons0_t _c1;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  v = &_c1;
  camlidl_ml2c_lincons0_ap_lincons0_t(_v_v, &_c1, _ctx);
  _res = ap_abstract0_sat_lincons(man, a, v);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_sat_tcons(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_tcons0_t *v; /*in*/
  int _res;
  ap_tcons0_t _c1;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  v = &_c1;
  camlidl_ml2c_tcons0_ap_tcons0_t(_v_v, &_c1, _ctx);
  _res = ap_abstract0_sat_tcons(man, a, v);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_sat_interval(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_dim_t v1; /*in*/
  struct ap_interval_t *v2; /*in*/
  int _res;
  struct ap_interval_t _c1;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  camlidl_ml2c_dim_ap_dim_t(_v_v1, &v1, _ctx);
  v2 = &_c1;
  camlidl_ml2c_interval_struct_ap_interval_t(_v_v2, &_c1, _ctx);
  _res = ap_abstract0_sat_interval(man, a, v1, v2);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_is_dimension_unconstrained(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_dim_t v; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  camlidl_ml2c_dim_ap_dim_t(_v_v, &v, _ctx);
  _res = ap_abstract0_is_dimension_unconstrained(man, a, v);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_abstract0_ap_abstract0_bound_dimension(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_dim_t v; /*in*/
  struct ap_interval_t *_res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  camlidl_ml2c_dim_ap_dim_t(_v_v, &v, _ctx);
  _res = ap_abstract0_bound_dimension(man, a, v);
  _vres = camlidl_c2ml_interval_struct_ap_interval_t(&*_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
ap_interval_free(_res); if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_bound_linexpr(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_linexpr0_ptr v; /*in*/
  struct ap_interval_t *_res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  camlidl_ml2c_linexpr0_ap_linexpr0_ptr(_v_v, &v, _ctx);
  _res = ap_abstract0_bound_linexpr(man, a, v);
  _vres = camlidl_c2ml_interval_struct_ap_interval_t(&*_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
ap_interval_free(_res); if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_bound_texpr(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_texpr0_ptr v; /*in*/
  struct ap_interval_t *_res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  camlidl_ml2c_texpr0_ap_texpr0_ptr(_v_v, &v, _ctx);
  _res = ap_abstract0_bound_texpr(man, a, v);
  _vres = camlidl_c2ml_interval_struct_ap_interval_t(&*_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
ap_interval_free(_res); if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_to_box(
	value _v_man,
	value _v_a)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  struct ap_interval_array_t _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  /* begin user-supplied calling sequence */
{
ap_dimension_t dim;
  _res.p = ap_abstract0_to_box(man,a);
dim = ap_abstract0_dimension(man,a);
_res.size = dim.intdim + dim.realdim;
}
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_interval_struct_ap_interval_array_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
ap_interval_array_free(_res.p, _res.size); if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_to_lincons_array(
	value _v_man,
	value _v_a)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  struct ap_lincons0_array_t _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  _res = ap_abstract0_to_lincons_array(man, a);
  _vres = camlidl_c2ml_lincons0_struct_ap_lincons0_array_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
free(_res.p); if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_to_tcons_array(
	value _v_man,
	value _v_a)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  struct ap_tcons0_array_t _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  _res = ap_abstract0_to_tcons_array(man, a);
  _vres = camlidl_c2ml_tcons0_struct_ap_tcons0_array_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
free(_res.p); if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_to_generator_array(
	value _v_man,
	value _v_a)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  struct ap_generator0_array_t _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  _res = ap_abstract0_to_generator_array(man, a);
  _vres = camlidl_c2ml_generator0_struct_ap_generator0_array_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
free(_res.p); if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_meet(
	value _v_man,
	value _v_a1,
	value _v_a2)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a1; /*in*/
  ap_abstract0_ptr a2; /*in*/
  ap_abstract0_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a1, &a1, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a2, &a2, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_abstract0_meet(man,false,a1,a2);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_meet_array(
	value _v_man,
	value _v_array)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr *array; /*in*/
  unsigned int size; /*in*/
  ap_abstract0_ptr _res;
  mlsize_t _c1;
  mlsize_t _c2;
  value _v3;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  _c1 = Wosize_val(_v_array);
  array = camlidl_malloc(_c1 * sizeof(ap_abstract0_ptr ), _ctx);
  for (_c2 = 0; _c2 < _c1; _c2++) {
    _v3 = Field(_v_array, _c2);
    camlidl_ml2c_abstract0_ap_abstract0_ptr(_v3, &array[_c2], _ctx);
  }
  size = _c1;
  _res = ap_abstract0_meet_array(man, array, size);
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_meet_lincons_array(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  struct ap_lincons0_array_t *v; /*in*/
  ap_abstract0_ptr _res;
  struct ap_lincons0_array_t _c1;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  v = &_c1;
  camlidl_ml2c_lincons0_struct_ap_lincons0_array_t(_v_v, &_c1, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_abstract0_meet_lincons_array(man,false,a,v);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_meet_tcons_array(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  struct ap_tcons0_array_t *v; /*in*/
  ap_abstract0_ptr _res;
  struct ap_tcons0_array_t _c1;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  v = &_c1;
  camlidl_ml2c_tcons0_struct_ap_tcons0_array_t(_v_v, &_c1, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_abstract0_meet_tcons_array(man,false,a,v);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_join(
	value _v_man,
	value _v_a1,
	value _v_a2)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a1; /*in*/
  ap_abstract0_ptr a2; /*in*/
  ap_abstract0_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a1, &a1, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a2, &a2, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_abstract0_join(man,false,a1,a2);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_join_array(
	value _v_man,
	value _v_array)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr *array; /*in*/
  unsigned int size; /*in*/
  ap_abstract0_ptr _res;
  mlsize_t _c1;
  mlsize_t _c2;
  value _v3;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  _c1 = Wosize_val(_v_array);
  array = camlidl_malloc(_c1 * sizeof(ap_abstract0_ptr ), _ctx);
  for (_c2 = 0; _c2 < _c1; _c2++) {
    _v3 = Field(_v_array, _c2);
    camlidl_ml2c_abstract0_ap_abstract0_ptr(_v3, &array[_c2], _ctx);
  }
  size = _c1;
  _res = ap_abstract0_join_array(man, array, size);
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_add_ray_array(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  struct ap_generator0_array_t *v; /*in*/
  ap_abstract0_ptr _res;
  struct ap_generator0_array_t _c1;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  v = &_c1;
  camlidl_ml2c_generator0_struct_ap_generator0_array_t(_v_v, &_c1, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_abstract0_add_ray_array(man,false,a,v);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_meet_with(
	value _v_man,
	value _v_a1,
	value _v_a2)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a1; /*in*/
  ap_abstract0_ptr a2; /*in*/
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a1, &a1, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a2, &a2, _ctx);
  /* begin user-supplied calling sequence */
{
ap_abstract0_t* res = ap_abstract0_meet(man,true,a1,a2);
*((ap_abstract0_ptr *) Data_custom_val(_v_a1)) = res;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract0_ap_abstract0_meet_lincons_array_with(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  struct ap_lincons0_array_t *v; /*in*/
  struct ap_lincons0_array_t _c1;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  v = &_c1;
  camlidl_ml2c_lincons0_struct_ap_lincons0_array_t(_v_v, &_c1, _ctx);
  /* begin user-supplied calling sequence */
{
ap_abstract0_t* res = ap_abstract0_meet_lincons_array(man,true,a,v);
*((ap_abstract0_ptr *) Data_custom_val(_v_a)) = res;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract0_ap_abstract0_meet_tcons_array_with(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  struct ap_tcons0_array_t *v; /*in*/
  struct ap_tcons0_array_t _c1;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  v = &_c1;
  camlidl_ml2c_tcons0_struct_ap_tcons0_array_t(_v_v, &_c1, _ctx);
  /* begin user-supplied calling sequence */
{
ap_abstract0_t* res = ap_abstract0_meet_tcons_array(man,true,a,v);
*((ap_abstract0_ptr *) Data_custom_val(_v_a)) = res;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract0_ap_abstract0_join_with(
	value _v_man,
	value _v_a1,
	value _v_a2)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a1; /*in*/
  ap_abstract0_ptr a2; /*in*/
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a1, &a1, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a2, &a2, _ctx);
  /* begin user-supplied calling sequence */
{
ap_abstract0_t* res = ap_abstract0_join(man,true,a1,a2);
*((ap_abstract0_ptr *) Data_custom_val(_v_a1)) = res;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract0_ap_abstract0_add_ray_array_with(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  struct ap_generator0_array_t *v; /*in*/
  struct ap_generator0_array_t _c1;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  v = &_c1;
  camlidl_ml2c_generator0_struct_ap_generator0_array_t(_v_v, &_c1, _ctx);
  /* begin user-supplied calling sequence */
{
ap_abstract0_t* res = ap_abstract0_add_ray_array(man,true,a,v);
*((ap_abstract0_ptr *) Data_custom_val(_v_a)) = res;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract0_ap_abstract0_assign_linexpr_array(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2,
	value _v_dest)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_dim_t *v1; /*in*/
  ap_linexpr0_ptr *v2; /*in*/
  int v3; /*in*/
  int v4; /*in*/
  ap_abstract0_ptr *dest; /*in*/
  ap_abstract0_ptr _res;
  mlsize_t _c1;
  mlsize_t _c2;
  value _v3;
  mlsize_t _c4;
  mlsize_t _c5;
  value _v6;
  value _v7;
  ap_abstract0_ptr _c8;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  _c1 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c1 * sizeof(ap_dim_t ), _ctx);
  for (_c2 = 0; _c2 < _c1; _c2++) {
    _v3 = Field(_v_v1, _c2);
    camlidl_ml2c_dim_ap_dim_t(_v3, &v1[_c2], _ctx);
  }
  v3 = _c1;
  _c4 = Wosize_val(_v_v2);
  v2 = camlidl_malloc(_c4 * sizeof(ap_linexpr0_ptr ), _ctx);
  for (_c5 = 0; _c5 < _c4; _c5++) {
    _v6 = Field(_v_v2, _c5);
    camlidl_ml2c_linexpr0_ap_linexpr0_ptr(_v6, &v2[_c5], _ctx);
  }
  v4 = _c4;
  if (_v_dest == Val_int(0)) {
    dest = NULL;
  } else {
    _v7 = Field(_v_dest, 0);
    dest = &_c8;
    camlidl_ml2c_abstract0_ap_abstract0_ptr(_v7, &_c8, _ctx);
  }
  /* begin user-supplied calling sequence */

if (v3!=v4) caml_failwith("Abstract0.assign_linexpr_array: arrays of different size");
_res = ap_abstract0_assign_linexpr_array(man,false,a,v1,v2,v3,dest==NULL ? NULL : *dest);

  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_substitute_linexpr_array(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2,
	value _v_dest)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_dim_t *v1; /*in*/
  ap_linexpr0_ptr *v2; /*in*/
  int v3; /*in*/
  int v4; /*in*/
  ap_abstract0_ptr *dest; /*in*/
  ap_abstract0_ptr _res;
  mlsize_t _c1;
  mlsize_t _c2;
  value _v3;
  mlsize_t _c4;
  mlsize_t _c5;
  value _v6;
  value _v7;
  ap_abstract0_ptr _c8;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  _c1 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c1 * sizeof(ap_dim_t ), _ctx);
  for (_c2 = 0; _c2 < _c1; _c2++) {
    _v3 = Field(_v_v1, _c2);
    camlidl_ml2c_dim_ap_dim_t(_v3, &v1[_c2], _ctx);
  }
  v3 = _c1;
  _c4 = Wosize_val(_v_v2);
  v2 = camlidl_malloc(_c4 * sizeof(ap_linexpr0_ptr ), _ctx);
  for (_c5 = 0; _c5 < _c4; _c5++) {
    _v6 = Field(_v_v2, _c5);
    camlidl_ml2c_linexpr0_ap_linexpr0_ptr(_v6, &v2[_c5], _ctx);
  }
  v4 = _c4;
  if (_v_dest == Val_int(0)) {
    dest = NULL;
  } else {
    _v7 = Field(_v_dest, 0);
    dest = &_c8;
    camlidl_ml2c_abstract0_ap_abstract0_ptr(_v7, &_c8, _ctx);
  }
  /* begin user-supplied calling sequence */

if (v3!=v4) caml_failwith("Abstract0.substitute_linexpr_array: arrays of different size");
_res = ap_abstract0_substitute_linexpr_array(man,false,a,v1,v2,v3,dest==NULL ? NULL : *dest);

  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_assign_texpr_array(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2,
	value _v_dest)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_dim_t *v1; /*in*/
  ap_texpr0_ptr *v2; /*in*/
  int v3; /*in*/
  int v4; /*in*/
  ap_abstract0_ptr *dest; /*in*/
  ap_abstract0_ptr _res;
  mlsize_t _c1;
  mlsize_t _c2;
  value _v3;
  mlsize_t _c4;
  mlsize_t _c5;
  value _v6;
  value _v7;
  ap_abstract0_ptr _c8;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  _c1 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c1 * sizeof(ap_dim_t ), _ctx);
  for (_c2 = 0; _c2 < _c1; _c2++) {
    _v3 = Field(_v_v1, _c2);
    camlidl_ml2c_dim_ap_dim_t(_v3, &v1[_c2], _ctx);
  }
  v3 = _c1;
  _c4 = Wosize_val(_v_v2);
  v2 = camlidl_malloc(_c4 * sizeof(ap_texpr0_ptr ), _ctx);
  for (_c5 = 0; _c5 < _c4; _c5++) {
    _v6 = Field(_v_v2, _c5);
    camlidl_ml2c_texpr0_ap_texpr0_ptr(_v6, &v2[_c5], _ctx);
  }
  v4 = _c4;
  if (_v_dest == Val_int(0)) {
    dest = NULL;
  } else {
    _v7 = Field(_v_dest, 0);
    dest = &_c8;
    camlidl_ml2c_abstract0_ap_abstract0_ptr(_v7, &_c8, _ctx);
  }
  /* begin user-supplied calling sequence */

if (v3!=v4) caml_failwith("Abstract0.assign_texpr_array: arrays of different size");
_res = ap_abstract0_assign_texpr_array(man,false,a,v1,v2,v3,dest==NULL ? NULL : *dest);

  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_substitute_texpr_array(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2,
	value _v_dest)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_dim_t *v1; /*in*/
  ap_texpr0_ptr *v2; /*in*/
  int v3; /*in*/
  int v4; /*in*/
  ap_abstract0_ptr *dest; /*in*/
  ap_abstract0_ptr _res;
  mlsize_t _c1;
  mlsize_t _c2;
  value _v3;
  mlsize_t _c4;
  mlsize_t _c5;
  value _v6;
  value _v7;
  ap_abstract0_ptr _c8;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  _c1 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c1 * sizeof(ap_dim_t ), _ctx);
  for (_c2 = 0; _c2 < _c1; _c2++) {
    _v3 = Field(_v_v1, _c2);
    camlidl_ml2c_dim_ap_dim_t(_v3, &v1[_c2], _ctx);
  }
  v3 = _c1;
  _c4 = Wosize_val(_v_v2);
  v2 = camlidl_malloc(_c4 * sizeof(ap_texpr0_ptr ), _ctx);
  for (_c5 = 0; _c5 < _c4; _c5++) {
    _v6 = Field(_v_v2, _c5);
    camlidl_ml2c_texpr0_ap_texpr0_ptr(_v6, &v2[_c5], _ctx);
  }
  v4 = _c4;
  if (_v_dest == Val_int(0)) {
    dest = NULL;
  } else {
    _v7 = Field(_v_dest, 0);
    dest = &_c8;
    camlidl_ml2c_abstract0_ap_abstract0_ptr(_v7, &_c8, _ctx);
  }
  /* begin user-supplied calling sequence */

if (v3!=v4) caml_failwith("Abstract0.substitute_texpr_array: arrays of different size");
_res = ap_abstract0_substitute_texpr_array(man,false,a,v1,v2,v3,dest==NULL ? NULL : *dest);

  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_assign_linexpr_array_with(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2,
	value _v_dest)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_dim_t *v1; /*in*/
  ap_linexpr0_ptr *v2; /*in*/
  int v3; /*in*/
  int v4; /*in*/
  ap_abstract0_ptr *dest; /*in*/
  mlsize_t _c1;
  mlsize_t _c2;
  value _v3;
  mlsize_t _c4;
  mlsize_t _c5;
  value _v6;
  value _v7;
  ap_abstract0_ptr _c8;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  _c1 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c1 * sizeof(ap_dim_t ), _ctx);
  for (_c2 = 0; _c2 < _c1; _c2++) {
    _v3 = Field(_v_v1, _c2);
    camlidl_ml2c_dim_ap_dim_t(_v3, &v1[_c2], _ctx);
  }
  v3 = _c1;
  _c4 = Wosize_val(_v_v2);
  v2 = camlidl_malloc(_c4 * sizeof(ap_linexpr0_ptr ), _ctx);
  for (_c5 = 0; _c5 < _c4; _c5++) {
    _v6 = Field(_v_v2, _c5);
    camlidl_ml2c_linexpr0_ap_linexpr0_ptr(_v6, &v2[_c5], _ctx);
  }
  v4 = _c4;
  if (_v_dest == Val_int(0)) {
    dest = NULL;
  } else {
    _v7 = Field(_v_dest, 0);
    dest = &_c8;
    camlidl_ml2c_abstract0_ap_abstract0_ptr(_v7, &_c8, _ctx);
  }
  /* begin user-supplied calling sequence */

if (v3!=v4) caml_failwith("Abstract0.assign_linexpr_array_with: arrays of different size");
{
ap_abstract0_t* res = ap_abstract0_assign_linexpr_array(man,true,a,v1,v2,v3,dest==NULL ? NULL : *dest);
*((ap_abstract0_ptr *) Data_custom_val(_v_a)) = res;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract0_ap_abstract0_substitute_linexpr_array_with(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2,
	value _v_dest)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_dim_t *v1; /*in*/
  ap_linexpr0_ptr *v2; /*in*/
  int v3; /*in*/
  int v4; /*in*/
  ap_abstract0_ptr *dest; /*in*/
  mlsize_t _c1;
  mlsize_t _c2;
  value _v3;
  mlsize_t _c4;
  mlsize_t _c5;
  value _v6;
  value _v7;
  ap_abstract0_ptr _c8;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  _c1 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c1 * sizeof(ap_dim_t ), _ctx);
  for (_c2 = 0; _c2 < _c1; _c2++) {
    _v3 = Field(_v_v1, _c2);
    camlidl_ml2c_dim_ap_dim_t(_v3, &v1[_c2], _ctx);
  }
  v3 = _c1;
  _c4 = Wosize_val(_v_v2);
  v2 = camlidl_malloc(_c4 * sizeof(ap_linexpr0_ptr ), _ctx);
  for (_c5 = 0; _c5 < _c4; _c5++) {
    _v6 = Field(_v_v2, _c5);
    camlidl_ml2c_linexpr0_ap_linexpr0_ptr(_v6, &v2[_c5], _ctx);
  }
  v4 = _c4;
  if (_v_dest == Val_int(0)) {
    dest = NULL;
  } else {
    _v7 = Field(_v_dest, 0);
    dest = &_c8;
    camlidl_ml2c_abstract0_ap_abstract0_ptr(_v7, &_c8, _ctx);
  }
  /* begin user-supplied calling sequence */

if (v3!=v4) caml_failwith("Abstract0.substitute_linexpr_array_with: arrays of different size");
{
ap_abstract0_t* res = ap_abstract0_substitute_linexpr_array(man,true,a,v1,v2,v3,dest==NULL ? NULL : *dest);
*((ap_abstract0_ptr *) Data_custom_val(_v_a)) = res;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract0_ap_abstract0_assign_texpr_array_with(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2,
	value _v_dest)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_dim_t *v1; /*in*/
  ap_texpr0_ptr *v2; /*in*/
  int v3; /*in*/
  int v4; /*in*/
  ap_abstract0_ptr *dest; /*in*/
  mlsize_t _c1;
  mlsize_t _c2;
  value _v3;
  mlsize_t _c4;
  mlsize_t _c5;
  value _v6;
  value _v7;
  ap_abstract0_ptr _c8;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  _c1 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c1 * sizeof(ap_dim_t ), _ctx);
  for (_c2 = 0; _c2 < _c1; _c2++) {
    _v3 = Field(_v_v1, _c2);
    camlidl_ml2c_dim_ap_dim_t(_v3, &v1[_c2], _ctx);
  }
  v3 = _c1;
  _c4 = Wosize_val(_v_v2);
  v2 = camlidl_malloc(_c4 * sizeof(ap_texpr0_ptr ), _ctx);
  for (_c5 = 0; _c5 < _c4; _c5++) {
    _v6 = Field(_v_v2, _c5);
    camlidl_ml2c_texpr0_ap_texpr0_ptr(_v6, &v2[_c5], _ctx);
  }
  v4 = _c4;
  if (_v_dest == Val_int(0)) {
    dest = NULL;
  } else {
    _v7 = Field(_v_dest, 0);
    dest = &_c8;
    camlidl_ml2c_abstract0_ap_abstract0_ptr(_v7, &_c8, _ctx);
  }
  /* begin user-supplied calling sequence */

if (v3!=v4) caml_failwith("Abstract0.assign_texpr_array_with: arrays of different size");
{
ap_abstract0_t* res = ap_abstract0_assign_texpr_array(man,true,a,v1,v2,v3,dest==NULL ? NULL : *dest);
*((ap_abstract0_ptr *) Data_custom_val(_v_a)) = res;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract0_ap_abstract0_substitute_texpr_array_with(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2,
	value _v_dest)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_dim_t *v1; /*in*/
  ap_texpr0_ptr *v2; /*in*/
  int v3; /*in*/
  int v4; /*in*/
  ap_abstract0_ptr *dest; /*in*/
  mlsize_t _c1;
  mlsize_t _c2;
  value _v3;
  mlsize_t _c4;
  mlsize_t _c5;
  value _v6;
  value _v7;
  ap_abstract0_ptr _c8;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  _c1 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c1 * sizeof(ap_dim_t ), _ctx);
  for (_c2 = 0; _c2 < _c1; _c2++) {
    _v3 = Field(_v_v1, _c2);
    camlidl_ml2c_dim_ap_dim_t(_v3, &v1[_c2], _ctx);
  }
  v3 = _c1;
  _c4 = Wosize_val(_v_v2);
  v2 = camlidl_malloc(_c4 * sizeof(ap_texpr0_ptr ), _ctx);
  for (_c5 = 0; _c5 < _c4; _c5++) {
    _v6 = Field(_v_v2, _c5);
    camlidl_ml2c_texpr0_ap_texpr0_ptr(_v6, &v2[_c5], _ctx);
  }
  v4 = _c4;
  if (_v_dest == Val_int(0)) {
    dest = NULL;
  } else {
    _v7 = Field(_v_dest, 0);
    dest = &_c8;
    camlidl_ml2c_abstract0_ap_abstract0_ptr(_v7, &_c8, _ctx);
  }
  /* begin user-supplied calling sequence */

if (v3!=v4) caml_failwith("Abstract0.substitute_texpr_array_with: arrays of different size");
{
ap_abstract0_t* res = ap_abstract0_substitute_texpr_array(man,true,a,v1,v2,v3,dest==NULL ? NULL : *dest);
*((ap_abstract0_ptr *) Data_custom_val(_v_a)) = res;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract0_ap_abstract0_forget_array(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v3)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_dim_t *v1; /*in*/
  int v2; /*in*/
  int v3; /*in*/
  ap_abstract0_ptr _res;
  mlsize_t _c1;
  mlsize_t _c2;
  value _v3;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  _c1 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c1 * sizeof(ap_dim_t ), _ctx);
  for (_c2 = 0; _c2 < _c1; _c2++) {
    _v3 = Field(_v_v1, _c2);
    camlidl_ml2c_dim_ap_dim_t(_v3, &v1[_c2], _ctx);
  }
  v2 = _c1;
  v3 = Int_val(_v_v3);
  /* begin user-supplied calling sequence */
_res = ap_abstract0_forget_array(man,false,a,v1,v2,v3);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_forget_array_with(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v3)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_dim_t *v1; /*in*/
  int v2; /*in*/
  int v3; /*in*/
  mlsize_t _c1;
  mlsize_t _c2;
  value _v3;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  _c1 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c1 * sizeof(ap_dim_t ), _ctx);
  for (_c2 = 0; _c2 < _c1; _c2++) {
    _v3 = Field(_v_v1, _c2);
    camlidl_ml2c_dim_ap_dim_t(_v3, &v1[_c2], _ctx);
  }
  v2 = _c1;
  v3 = Int_val(_v_v3);
  /* begin user-supplied calling sequence */
{
ap_abstract0_t* res = ap_abstract0_forget_array(man,true,a,v1,v2,v3);
 *((ap_abstract0_ptr *) Data_custom_val(_v_a)) = res;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract0_ap_abstract0_add_dimensions(
	value _v_man,
	value _v_a,
	value _v_dimchange,
	value _v_project)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_dimchange_t dimchange; /*in*/
  int project; /*in*/
  ap_abstract0_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  camlidl_ml2c_dim_ap_dimchange_t(_v_dimchange, &dimchange, _ctx);
  project = Int_val(_v_project);
  /* begin user-supplied calling sequence */
_res = ap_abstract0_add_dimensions(man,false,a,&dimchange, project);ap_dimchange_clear(&dimchange);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_remove_dimensions(
	value _v_man,
	value _v_a,
	value _v_dimchange)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_dimchange_t dimchange; /*in*/
  ap_abstract0_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  camlidl_ml2c_dim_ap_dimchange_t(_v_dimchange, &dimchange, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_abstract0_remove_dimensions(man,false,a,&dimchange);ap_dimchange_clear(&dimchange);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_apply_dimchange2(
	value _v_man,
	value _v_a,
	value _v_dimchange2,
	value _v_project)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  struct ap_dimchange2_t dimchange2; /*in*/
  int project; /*in*/
  ap_abstract0_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  camlidl_ml2c_dim_struct_ap_dimchange2_t(_v_dimchange2, &dimchange2, _ctx);
  project = Int_val(_v_project);
  /* begin user-supplied calling sequence */

_res = ap_abstract0_apply_dimchange2(man,false,a,&dimchange2,project);
if (dimchange2.add) ap_dimchange_clear(dimchange2.add);
if (dimchange2.remove) ap_dimchange_clear(dimchange2.remove);

  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_permute_dimensions(
	value _v_man,
	value _v_a,
	value _v_perm)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  struct ap_dimperm_t *perm; /*in*/
  ap_abstract0_ptr _res;
  struct ap_dimperm_t _c1;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  perm = &_c1;
  camlidl_ml2c_dim_struct_ap_dimperm_t(_v_perm, &_c1, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_abstract0_permute_dimensions(man,false,a,perm);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_add_dimensions_with(
	value _v_man,
	value _v_a,
	value _v_dimchange,
	value _v_v1)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_dimchange_t dimchange; /*in*/
  int v1; /*in*/
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  camlidl_ml2c_dim_ap_dimchange_t(_v_dimchange, &dimchange, _ctx);
  v1 = Int_val(_v_v1);
  /* begin user-supplied calling sequence */
{
ap_abstract0_t* res = ap_abstract0_add_dimensions(man,true,a,&dimchange,v1);
 ap_dimchange_clear(&dimchange);
*((ap_abstract0_ptr *) Data_custom_val(_v_a)) = res;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract0_ap_abstract0_remove_dimensions_with(
	value _v_man,
	value _v_a,
	value _v_dimchange)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_dimchange_t dimchange; /*in*/
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  camlidl_ml2c_dim_ap_dimchange_t(_v_dimchange, &dimchange, _ctx);
  /* begin user-supplied calling sequence */
{
ap_abstract0_t* res = ap_abstract0_remove_dimensions(man,true,a,&dimchange);
ap_dimchange_clear(&dimchange);
*((ap_abstract0_ptr *) Data_custom_val(_v_a)) = res;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract0_ap_abstract0_apply_dimchange2_with(
	value _v_man,
	value _v_a,
	value _v_dimchange2,
	value _v_project)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  struct ap_dimchange2_t dimchange2; /*in*/
  int project; /*in*/
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  camlidl_ml2c_dim_struct_ap_dimchange2_t(_v_dimchange2, &dimchange2, _ctx);
  project = Int_val(_v_project);
  /* begin user-supplied calling sequence */

ap_abstract0_t* res  = ap_abstract0_apply_dimchange2(man,true,a,&dimchange2,project);
ap_dimchange2_clear(&dimchange2);
*((ap_abstract0_ptr *) Data_custom_val(_v_a)) = res;

  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract0_ap_abstract0_permute_dimensions_with(
	value _v_man,
	value _v_a,
	value _v_perm)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  struct ap_dimperm_t *perm; /*in*/
  value _v1;
  struct ap_dimperm_t _c2;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  if (_v_perm == Val_int(0)) {
    perm = NULL;
  } else {
    _v1 = Field(_v_perm, 0);
    perm = &_c2;
    camlidl_ml2c_dim_struct_ap_dimperm_t(_v1, &_c2, _ctx);
  }
  /* begin user-supplied calling sequence */
{
ap_abstract0_t* res = ap_abstract0_permute_dimensions(man,true,a,perm);
*((ap_abstract0_ptr *) Data_custom_val(_v_a)) = res;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract0_ap_abstract0_expand(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_dim_t v1; /*in*/
  int v2; /*in*/
  ap_abstract0_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  camlidl_ml2c_dim_ap_dim_t(_v_v1, &v1, _ctx);
  v2 = Int_val(_v_v2);
  /* begin user-supplied calling sequence */
_res = ap_abstract0_expand(man,false,a,v1,v2);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_fold(
	value _v_man,
	value _v_a,
	value _v_v1)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_dim_t *v1; /*in*/
  int v2; /*in*/
  ap_abstract0_ptr _res;
  mlsize_t _c1;
  mlsize_t _c2;
  value _v3;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  _c1 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c1 * sizeof(ap_dim_t ), _ctx);
  for (_c2 = 0; _c2 < _c1; _c2++) {
    _v3 = Field(_v_v1, _c2);
    camlidl_ml2c_dim_ap_dim_t(_v3, &v1[_c2], _ctx);
  }
  v2 = _c1;
  /* begin user-supplied calling sequence */
_res = ap_abstract0_fold(man,false,a,v1,v2);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_expand_with(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_dim_t v1; /*in*/
  int v2; /*in*/
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  camlidl_ml2c_dim_ap_dim_t(_v_v1, &v1, _ctx);
  v2 = Int_val(_v_v2);
  /* begin user-supplied calling sequence */
{
ap_abstract0_t* res = ap_abstract0_expand(man,true,a,v1,v2);
  *((ap_abstract0_ptr *) Data_custom_val(_v_a)) = res;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract0_ap_abstract0_fold_with(
	value _v_man,
	value _v_a,
	value _v_v1)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_dim_t *v1; /*in*/
  int v2; /*in*/
  mlsize_t _c1;
  mlsize_t _c2;
  value _v3;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  _c1 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c1 * sizeof(ap_dim_t ), _ctx);
  for (_c2 = 0; _c2 < _c1; _c2++) {
    _v3 = Field(_v_v1, _c2);
    camlidl_ml2c_dim_ap_dim_t(_v3, &v1[_c2], _ctx);
  }
  v2 = _c1;
  /* begin user-supplied calling sequence */
{
ap_abstract0_t* res = ap_abstract0_fold(man,true,a,v1,v2);
  *((ap_abstract0_ptr *) Data_custom_val(_v_a)) = res;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract0_ap_abstract0_widening(
	value _v_man,
	value _v_a1,
	value _v_a2)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a1; /*in*/
  ap_abstract0_ptr a2; /*in*/
  ap_abstract0_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a1, &a1, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a2, &a2, _ctx);
  _res = ap_abstract0_widening(man, a1, a2);
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_widening_threshold(
	value _v_man,
	value _v_a1,
	value _v_a2,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a1; /*in*/
  ap_abstract0_ptr a2; /*in*/
  struct ap_lincons0_array_t *v; /*in*/
  ap_abstract0_ptr _res;
  struct ap_lincons0_array_t _c1;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a1, &a1, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a2, &a2, _ctx);
  v = &_c1;
  camlidl_ml2c_lincons0_struct_ap_lincons0_array_t(_v_v, &_c1, _ctx);
  _res = ap_abstract0_widening_threshold(man, a1, a2, v);
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_closure(
	value _v_man,
	value _v_a)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_abstract0_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_abstract0_closure(man,false,a);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract0_ap_abstract0_closure_with(
	value _v_man,
	value _v_a)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  /* begin user-supplied calling sequence */
{
ap_abstract0_t* res = ap_abstract0_closure(man,true,a);
  *((ap_abstract0_ptr *) Data_custom_val(_v_a)) = res;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

