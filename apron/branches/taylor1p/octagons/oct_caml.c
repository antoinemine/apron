/* File generated from oct.idl */

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

/*
 This file is part of the APRON Library, released under LGPL license.
 Please read the COPYING file packaged in the distribution.
*/
#include "oct.h"
#include "apron_caml.h"
#define I0_CHECK_EXC(man) if (man->result.exn!=AP_EXC_NONE){ value v = camlidl_c2ml_manager_struct_ap_exclog_t(man->result.exclog,_ctx); caml_raise_with_arg(*caml_named_value("apron exception"),v); } 
typedef struct oct_internal_t* internal_ptr;
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


extern int camlidl_ml2c_generator0_enum_gentyp(value);
extern value camlidl_c2ml_generator0_enum_gentyp(int);

extern int camlidl_transl_table_generator0_enum_gentyp[];

extern void camlidl_ml2c_generator0_struct_ap_generator0_t(value, struct ap_generator0_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_generator0_struct_ap_generator0_t(struct ap_generator0_t *, camlidl_ctx _ctx);

extern void camlidl_ml2c_generator0_struct_ap_generator0_array_t(value, struct ap_generator0_array_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_generator0_struct_ap_generator0_array_t(struct ap_generator0_array_t *, camlidl_ctx _ctx);

extern void camlidl_apron_lincons0_ml2c(value, ap_lincons0_t *);
#define camlidl_ml2c_lincons0_ap_lincons0_t(v,c,ctx) camlidl_apron_lincons0_ml2c(v,c)

extern value camlidl_apron_lincons0_c2ml(ap_lincons0_t *);
#define camlidl_c2ml_lincons0_ap_lincons0_t(c,ctx) camlidl_apron_lincons0_c2ml(c)


extern void camlidl_ml2c_lincons0_struct_ap_lincons0_array_t(value, struct ap_lincons0_array_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_lincons0_struct_ap_lincons0_array_t(struct ap_lincons0_array_t *, camlidl_ctx _ctx);

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


extern void camlidl_apron_tcons0_ml2c(value, ap_tcons0_t *);
#define camlidl_ml2c_tcons0_ap_tcons0_t(v,c,ctx) camlidl_apron_tcons0_ml2c(v,c)

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


extern void camlidl_apron_abstract0_ptr_ml2c(value, ap_abstract0_ptr *);
#define camlidl_ml2c_abstract0_ap_abstract0_ptr(v,c,ctx) camlidl_apron_abstract0_ptr_ml2c(v,c)

extern value camlidl_apron_abstract0_ptr_c2ml(ap_abstract0_ptr *);
#define camlidl_c2ml_abstract0_ap_abstract0_ptr(c,ctx) camlidl_apron_abstract0_ptr_c2ml(c)


void camlidl_ml2c_oct_internal_ptr(value _v1, internal_ptr * _c2, camlidl_ctx _ctx)
{
  *_c2 = *((internal_ptr *) Bp_val(_v1));
}

value camlidl_c2ml_oct_internal_ptr(internal_ptr * _c2, camlidl_ctx _ctx)
{
value _v1;
  _v1 = camlidl_alloc((sizeof(internal_ptr) + sizeof(value) - 1) / sizeof(value), Abstract_tag);
  *((internal_ptr *) Bp_val(_v1)) = *_c2;
  return _v1;
}

value camlidl_oct_oct_manager_alloc(value _unit)
{
  ap_manager_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  _res = oct_manager_alloc();
  _vres = camlidl_c2ml_manager_ap_manager_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_oct_manager_get_internal(
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
  _vres = camlidl_c2ml_oct_internal_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_oct_ap_abstract0_oct_of_generator_array(
	value _v_man,
	value _v_v1,
	value _v_v2,
	value _v_v3)
{
  ap_manager_ptr man; /*in*/
  int v1; /*in*/
  int v2; /*in*/
  struct ap_generator0_array_t *v3; /*in*/
  ap_abstract0_ptr _res;
  struct ap_generator0_array_t _c1;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  v1 = Int_val(_v_v1);
  v2 = Int_val(_v_v2);
  v3 = &_c1;
  camlidl_ml2c_generator0_struct_ap_generator0_array_t(_v_v3, &_c1, _ctx);
  _res = ap_abstract0_oct_of_generator_array(man, v1, v2, v3);
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
I0_CHECK_EXC(man)
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_oct_ap_abstract0_oct_widening_thresholds(
	value _v_man,
	value _v_a1,
	value _v_a2,
	value _v_array)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a1; /*in*/
  ap_abstract0_ptr a2; /*in*/
  struct ap_scalar_array_t array; /*in*/
  ap_abstract0_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a1, &a1, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a2, &a2, _ctx);
  camlidl_ml2c_scalar_struct_ap_scalar_array_t(_v_array, &array, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_abstract0_oct_widening_thresholds(man,a1,a2,(const ap_scalar_t**)array.p,array.size);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
I0_CHECK_EXC(man)
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_oct_ap_abstract0_oct_narrowing(
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
  _res = ap_abstract0_oct_narrowing(man, a1, a2);
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
I0_CHECK_EXC(man)
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_oct_ap_abstract0_oct_add_epsilon(
	value _v_man,
	value _v_a,
	value _v_epsilon)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a; /*in*/
  ap_scalar_ptr epsilon; /*in*/
  ap_abstract0_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a, &a, _ctx);
  camlidl_ml2c_scalar_ap_scalar_ptr(_v_epsilon, &epsilon, _ctx);
  _res = ap_abstract0_oct_add_epsilon(man, a, epsilon);
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
I0_CHECK_EXC(man)
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_oct_ap_abstract0_oct_add_epsilon_bin(
	value _v_man,
	value _v_a1,
	value _v_a2,
	value _v_epsilon)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr a1; /*in*/
  ap_abstract0_ptr a2; /*in*/
  ap_scalar_ptr epsilon; /*in*/
  ap_abstract0_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a1, &a1, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_a2, &a2, _ctx);
  camlidl_ml2c_scalar_ap_scalar_ptr(_v_epsilon, &epsilon, _ctx);
  _res = ap_abstract0_oct_add_epsilon_bin(man, a1, a2, epsilon);
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
I0_CHECK_EXC(man)
  /* end user-supplied deallocation sequence */
  return _vres;
}

