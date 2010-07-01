/* File generated from abstract1.idl */

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
#include "ap_environment.h"
#include "ap_expr1.h"
#include "ap_abstract1.h"
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

extern void camlidl_apron_abstract0_ptr_ml2c(value, ap_abstract0_ptr *);
#define camlidl_ml2c_abstract0_ap_abstract0_ptr(v,c,ctx) camlidl_apron_abstract0_ptr_ml2c(v,c)

extern value camlidl_apron_abstract0_ptr_c2ml(ap_abstract0_ptr *);
#define camlidl_c2ml_abstract0_ap_abstract0_ptr(c,ctx) camlidl_apron_abstract0_ptr_c2ml(c)


extern void camlidl_apron_var_ptr_ml2c(value, ap_var_t *);
#define camlidl_ml2c_var_ap_var_t(v,c,ctx) camlidl_apron_var_ptr_ml2c(v,c)

extern value camlidl_apron_var_ptr_c2ml(ap_var_t *);
#define camlidl_c2ml_var_ap_var_t(c,ctx) camlidl_apron_var_ptr_c2ml(c)


extern int camlidl_ml2c_environment_enum_typvar(value);
extern value camlidl_c2ml_environment_enum_typvar(int);

extern int camlidl_transl_table_environment_enum_typvar[];

extern void camlidl_apron_environment_ptr_ml2c(value, ap_environment_ptr *);
#define camlidl_ml2c_environment_ap_environment_ptr(v,c,ctx) camlidl_apron_environment_ptr_ml2c(v,c)

extern value camlidl_apron_environment_ptr_c2ml(ap_environment_ptr *);
#define camlidl_c2ml_environment_ap_environment_ptr(c,ctx) camlidl_apron_environment_ptr_c2ml(c)


extern void camlidl_ml2c_linexpr1_struct_ap_linexpr1_t(value, struct ap_linexpr1_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_linexpr1_struct_ap_linexpr1_t(struct ap_linexpr1_t *, camlidl_ctx _ctx);

extern void camlidl_ml2c_lincons1_struct_ap_lincons1_t(value, struct ap_lincons1_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_lincons1_struct_ap_lincons1_t(struct ap_lincons1_t *, camlidl_ctx _ctx);

extern void camlidl_ml2c_lincons1_struct_ap_lincons1_array_t(value, struct ap_lincons1_array_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_lincons1_struct_ap_lincons1_array_t(struct ap_lincons1_array_t *, camlidl_ctx _ctx);

extern void camlidl_ml2c_generator1_struct_ap_generator1_t(value, struct ap_generator1_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_generator1_struct_ap_generator1_t(struct ap_generator1_t *, camlidl_ctx _ctx);

extern void camlidl_ml2c_generator1_struct_ap_generator1_array_t(value, struct ap_generator1_array_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_generator1_struct_ap_generator1_array_t(struct ap_generator1_array_t *, camlidl_ctx _ctx);

extern void camlidl_ml2c_texpr1_struct_ap_texpr1_t(value, struct ap_texpr1_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_texpr1_struct_ap_texpr1_t(struct ap_texpr1_t *, camlidl_ctx _ctx);

extern void camlidl_ml2c_tcons1_struct_ap_tcons1_t(value, struct ap_tcons1_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_tcons1_struct_ap_tcons1_t(struct ap_tcons1_t *, camlidl_ctx _ctx);

extern void camlidl_ml2c_tcons1_struct_ap_tcons1_array_t(value, struct ap_tcons1_array_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_tcons1_struct_ap_tcons1_array_t(struct ap_tcons1_array_t *, camlidl_ctx _ctx);

void camlidl_ml2c_abstract1_struct_ap_abstract1_t(value _v1, struct ap_abstract1_t * _c2, camlidl_ctx _ctx)
{
  value _v3;
  value _v4;
  _v3 = Field(_v1, 0);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v3, &(*_c2).abstract0, _ctx);
  _v4 = Field(_v1, 1);
  camlidl_ml2c_environment_ap_environment_ptr(_v4, &(*_c2).env, _ctx);
}

value camlidl_c2ml_abstract1_struct_ap_abstract1_t(struct ap_abstract1_t * _c1, camlidl_ctx _ctx)
{
  value _v2;
  value _v3[2];
  _v3[0] = _v3[1] = 0;
  Begin_roots_block(_v3, 2)
    _v3[0] = camlidl_c2ml_abstract0_ap_abstract0_ptr(&(*_c1).abstract0, _ctx);
    _v3[1] = camlidl_c2ml_environment_ap_environment_ptr(&(*_c1).env, _ctx);
    _v2 = camlidl_alloc_small(2, 0);
    Field(_v2, 0) = _v3[0];
    Field(_v2, 1) = _v3[1];
  End_roots()
  return _v2;
}

value camlidl_abstract1_ap_abstract1_fdump(
	value _v_man,
	value _v_a)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  struct ap_abstract1_t _c1;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  /* begin user-supplied calling sequence */
ap_abstract1_fdump(stdout,man, a); fflush(stdout);
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract1_ap_abstract1_bottom(
	value _v_man,
	value _v_env)
{
  ap_manager_ptr man; /*in*/
  ap_environment_ptr env; /*in*/
  struct ap_abstract1_t _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_environment_ap_environment_ptr(_v_env, &env, _ctx);
  _res = ap_abstract1_bottom(man, env);
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_top(
	value _v_man,
	value _v_env)
{
  ap_manager_ptr man; /*in*/
  ap_environment_ptr env; /*in*/
  struct ap_abstract1_t _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_environment_ap_environment_ptr(_v_env, &env, _ctx);
  _res = ap_abstract1_top(man, env);
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_of_box(
	value _v_man,
	value _v_env,
	value _v_tvar,
	value _v_tinterval)
{
  ap_manager_ptr man; /*in*/
  ap_environment_ptr env; /*in*/
  ap_var_t *tvar; /*in*/
  ap_interval_ptr *tinterval; /*in*/
  unsigned int size; /*in*/
  unsigned int size2; /*in*/
  struct ap_abstract1_t _res;
  mlsize_t _c1;
  mlsize_t _c2;
  value _v3;
  mlsize_t _c4;
  mlsize_t _c5;
  value _v6;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_environment_ap_environment_ptr(_v_env, &env, _ctx);
  _c1 = Wosize_val(_v_tvar);
  tvar = camlidl_malloc(_c1 * sizeof(ap_var_t ), _ctx);
  for (_c2 = 0; _c2 < _c1; _c2++) {
    _v3 = Field(_v_tvar, _c2);
    camlidl_ml2c_var_ap_var_t(_v3, &tvar[_c2], _ctx);
  }
  size = _c1;
  _c4 = Wosize_val(_v_tinterval);
  tinterval = camlidl_malloc(_c4 * sizeof(ap_interval_ptr ), _ctx);
  for (_c5 = 0; _c5 < _c4; _c5++) {
    _v6 = Field(_v_tinterval, _c5);
    camlidl_ml2c_interval_ap_interval_ptr(_v6, &tinterval[_c5], _ctx);
  }
  size2 = _c4;
  /* begin user-supplied calling sequence */

if (size!=size2)
caml_failwith("Abstract1.of_box: the two arrays are of different sizes");
_res = ap_abstract1_of_box(man,env,tvar,tinterval,size);

  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_is_leq(
	value _v_man,
	value _v_a1,
	value _v_a2)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a1; /*in*/
  struct ap_abstract1_t *a2; /*in*/
  int _res;
  struct ap_abstract1_t _c1;
  struct ap_abstract1_t _c2;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a1 = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a1, &_c1, _ctx);
  a2 = &_c2;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a2, &_c2, _ctx);
  _res = ap_abstract1_is_leq(man, a1, a2);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_is_eq(
	value _v_man,
	value _v_a1,
	value _v_a2)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a1; /*in*/
  struct ap_abstract1_t *a2; /*in*/
  int _res;
  struct ap_abstract1_t _c1;
  struct ap_abstract1_t _c2;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a1 = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a1, &_c1, _ctx);
  a2 = &_c2;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a2, &_c2, _ctx);
  _res = ap_abstract1_is_eq(man, a1, a2);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_sat_lincons(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  struct ap_lincons1_t *v; /*in*/
  int _res;
  struct ap_abstract1_t _c1;
  struct ap_lincons1_t _c2;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  v = &_c2;
  camlidl_ml2c_lincons1_struct_ap_lincons1_t(_v_v, &_c2, _ctx);
  _res = ap_abstract1_sat_lincons(man, a, v);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_sat_tcons(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  struct ap_tcons1_t *v; /*in*/
  int _res;
  struct ap_abstract1_t _c1;
  struct ap_tcons1_t _c2;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  v = &_c2;
  camlidl_ml2c_tcons1_struct_ap_tcons1_t(_v_v, &_c2, _ctx);
  _res = ap_abstract1_sat_tcons(man, a, v);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_sat_interval(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  ap_var_t v1; /*in*/
  struct ap_interval_t *v2; /*in*/
  int _res;
  struct ap_abstract1_t _c1;
  struct ap_interval_t _c2;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  camlidl_ml2c_var_ap_var_t(_v_v1, &v1, _ctx);
  v2 = &_c2;
  camlidl_ml2c_interval_struct_ap_interval_t(_v_v2, &_c2, _ctx);
  _res = ap_abstract1_sat_interval(man, a, v1, v2);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_is_variable_unconstrained(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  ap_var_t v; /*in*/
  int _res;
  struct ap_abstract1_t _c1;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  camlidl_ml2c_var_ap_var_t(_v_v, &v, _ctx);
  _res = ap_abstract1_is_variable_unconstrained(man, a, v);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_bound_variable(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  ap_var_t v; /*in*/
  struct ap_interval_t *_res;
  struct ap_abstract1_t _c1;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  camlidl_ml2c_var_ap_var_t(_v_v, &v, _ctx);
  _res = ap_abstract1_bound_variable(man, a, v);
  _vres = camlidl_c2ml_interval_struct_ap_interval_t(&*_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
ap_interval_free(_res); if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_bound_linexpr(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  struct ap_linexpr1_t *v; /*in*/
  struct ap_interval_t *_res;
  struct ap_abstract1_t _c1;
  struct ap_linexpr1_t _c2;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  v = &_c2;
  camlidl_ml2c_linexpr1_struct_ap_linexpr1_t(_v_v, &_c2, _ctx);
  _res = ap_abstract1_bound_linexpr(man, a, v);
  _vres = camlidl_c2ml_interval_struct_ap_interval_t(&*_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
ap_interval_free(_res); if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_bound_texpr(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  struct ap_texpr1_t *v; /*in*/
  struct ap_interval_t *_res;
  struct ap_abstract1_t _c1;
  struct ap_texpr1_t _c2;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  v = &_c2;
  camlidl_ml2c_texpr1_struct_ap_texpr1_t(_v_v, &_c2, _ctx);
  _res = ap_abstract1_bound_texpr(man, a, v);
  _vres = camlidl_c2ml_interval_struct_ap_interval_t(&*_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
ap_interval_free(_res); if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_meet(
	value _v_man,
	value _v_a1,
	value _v_a2)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a1; /*in*/
  struct ap_abstract1_t *a2; /*in*/
  struct ap_abstract1_t _res;
  struct ap_abstract1_t _c1;
  struct ap_abstract1_t _c2;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a1 = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a1, &_c1, _ctx);
  a2 = &_c2;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a2, &_c2, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_abstract1_meet(man,false,a1,a2);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_meet_array(
	value _v_man,
	value _v_array)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *array; /*in*/
  unsigned int size; /*in*/
  struct ap_abstract1_t _res;
  mlsize_t _c1;
  mlsize_t _c2;
  value _v3;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  _c1 = Wosize_val(_v_array);
  array = camlidl_malloc(_c1 * sizeof(struct ap_abstract1_t ), _ctx);
  for (_c2 = 0; _c2 < _c1; _c2++) {
    _v3 = Field(_v_array, _c2);
    camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v3, &array[_c2], _ctx);
  }
  size = _c1;
  _res = ap_abstract1_meet_array(man, array, size);
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_meet_lincons_array(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  struct ap_lincons1_array_t *v; /*in*/
  struct ap_abstract1_t _res;
  struct ap_abstract1_t _c1;
  struct ap_lincons1_array_t _c2;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  v = &_c2;
  camlidl_ml2c_lincons1_struct_ap_lincons1_array_t(_v_v, &_c2, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_abstract1_meet_lincons_array(man,false,a,v);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_meet_tcons_array(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  struct ap_tcons1_array_t *v; /*in*/
  struct ap_abstract1_t _res;
  struct ap_abstract1_t _c1;
  struct ap_tcons1_array_t _c2;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  v = &_c2;
  camlidl_ml2c_tcons1_struct_ap_tcons1_array_t(_v_v, &_c2, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_abstract1_meet_tcons_array(man,false,a,v);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_join(
	value _v_man,
	value _v_a1,
	value _v_a2)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a1; /*in*/
  struct ap_abstract1_t *a2; /*in*/
  struct ap_abstract1_t _res;
  struct ap_abstract1_t _c1;
  struct ap_abstract1_t _c2;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a1 = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a1, &_c1, _ctx);
  a2 = &_c2;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a2, &_c2, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_abstract1_join(man,false,a1,a2);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_join_array(
	value _v_man,
	value _v_array)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *array; /*in*/
  unsigned int size; /*in*/
  struct ap_abstract1_t _res;
  mlsize_t _c1;
  mlsize_t _c2;
  value _v3;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  _c1 = Wosize_val(_v_array);
  array = camlidl_malloc(_c1 * sizeof(struct ap_abstract1_t ), _ctx);
  for (_c2 = 0; _c2 < _c1; _c2++) {
    _v3 = Field(_v_array, _c2);
    camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v3, &array[_c2], _ctx);
  }
  size = _c1;
  _res = ap_abstract1_join_array(man, array, size);
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_add_ray_array(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  struct ap_generator1_array_t *v; /*in*/
  struct ap_abstract1_t _res;
  struct ap_abstract1_t _c1;
  struct ap_generator1_array_t _c2;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  v = &_c2;
  camlidl_ml2c_generator1_struct_ap_generator1_array_t(_v_v, &_c2, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_abstract1_add_ray_array(man,false,a,v);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_meet_with(
	value _v_man,
	value _v_a1,
	value _v_a2)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a1; /*in*/
  struct ap_abstract1_t *a2; /*in*/
  struct ap_abstract1_t _c1;
  struct ap_abstract1_t _c2;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a1 = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a1, &_c1, _ctx);
  a2 = &_c2;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a2, &_c2, _ctx);
  /* begin user-supplied calling sequence */
{
ap_abstract1_t res = ap_abstract1_meet(man,true,a1,a2);
value v = Field(_v_a1,0);
*((ap_abstract0_ptr *) Data_custom_val(v)) = res.abstract0;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract1_ap_abstract1_meet_lincons_array_with(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  struct ap_lincons1_array_t *v; /*in*/
  struct ap_abstract1_t _c1;
  struct ap_lincons1_array_t _c2;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  v = &_c2;
  camlidl_ml2c_lincons1_struct_ap_lincons1_array_t(_v_v, &_c2, _ctx);
  /* begin user-supplied calling sequence */
{
ap_abstract1_t res = ap_abstract1_meet_lincons_array(man,true,a,v);
value v = Field(_v_a,0);
*((ap_abstract0_ptr *) Data_custom_val(v)) = res.abstract0;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract1_ap_abstract1_meet_tcons_array_with(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  struct ap_tcons1_array_t *v; /*in*/
  struct ap_abstract1_t _c1;
  struct ap_tcons1_array_t _c2;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  v = &_c2;
  camlidl_ml2c_tcons1_struct_ap_tcons1_array_t(_v_v, &_c2, _ctx);
  /* begin user-supplied calling sequence */
{
ap_abstract1_t res = ap_abstract1_meet_tcons_array(man,true,a,v);
value v = Field(_v_a,0);
*((ap_abstract0_ptr *) Data_custom_val(v)) = res.abstract0;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract1_ap_abstract1_join_with(
	value _v_man,
	value _v_a1,
	value _v_a2)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a1; /*in*/
  struct ap_abstract1_t *a2; /*in*/
  struct ap_abstract1_t _c1;
  struct ap_abstract1_t _c2;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a1 = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a1, &_c1, _ctx);
  a2 = &_c2;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a2, &_c2, _ctx);
  /* begin user-supplied calling sequence */
{
ap_abstract1_t res = ap_abstract1_join(man,true,a1,a2);
value v = Field(_v_a1,0);
*((ap_abstract0_ptr *) Data_custom_val(v)) = res.abstract0;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract1_ap_abstract1_add_ray_array_with(
	value _v_man,
	value _v_a,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  struct ap_generator1_array_t *v; /*in*/
  struct ap_abstract1_t _c1;
  struct ap_generator1_array_t _c2;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  v = &_c2;
  camlidl_ml2c_generator1_struct_ap_generator1_array_t(_v_v, &_c2, _ctx);
  /* begin user-supplied calling sequence */
{
ap_abstract1_t res = ap_abstract1_add_ray_array(man,true,a,v);
value v = Field(_v_a,0);
*((ap_abstract0_ptr *) Data_custom_val(v)) = res.abstract0;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract1_ap_abstract1_assign_linexpr_array(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2,
	value _v_dest)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  ap_var_t *v1; /*in*/
  struct ap_linexpr1_t *v2; /*in*/
  int v3; /*in*/
  int v4; /*in*/
  struct ap_abstract1_t *dest; /*in*/
  struct ap_abstract1_t _res;
  struct ap_abstract1_t _c1;
  mlsize_t _c2;
  mlsize_t _c3;
  value _v4;
  mlsize_t _c5;
  mlsize_t _c6;
  value _v7;
  value _v8;
  struct ap_abstract1_t _c9;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  _c2 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c2 * sizeof(ap_var_t ), _ctx);
  for (_c3 = 0; _c3 < _c2; _c3++) {
    _v4 = Field(_v_v1, _c3);
    camlidl_ml2c_var_ap_var_t(_v4, &v1[_c3], _ctx);
  }
  v3 = _c2;
  _c5 = Wosize_val(_v_v2);
  v2 = camlidl_malloc(_c5 * sizeof(struct ap_linexpr1_t ), _ctx);
  for (_c6 = 0; _c6 < _c5; _c6++) {
    _v7 = Field(_v_v2, _c6);
    camlidl_ml2c_linexpr1_struct_ap_linexpr1_t(_v7, &v2[_c6], _ctx);
  }
  v4 = _c5;
  if (_v_dest == Val_int(0)) {
    dest = NULL;
  } else {
    _v8 = Field(_v_dest, 0);
    dest = &_c9;
    camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v8, &_c9, _ctx);
  }
  /* begin user-supplied calling sequence */

if (v3!=v4) caml_failwith("Abstract1.assign_linexpr_array: arrays of different size");
_res = ap_abstract1_assign_linexpr_array(man,false,a,v1,v2,v3,dest);

  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_substitute_linexpr_array(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2,
	value _v_dest)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  ap_var_t *v1; /*in*/
  struct ap_linexpr1_t *v2; /*in*/
  int v3; /*in*/
  int v4; /*in*/
  struct ap_abstract1_t *dest; /*in*/
  struct ap_abstract1_t _res;
  struct ap_abstract1_t _c1;
  mlsize_t _c2;
  mlsize_t _c3;
  value _v4;
  mlsize_t _c5;
  mlsize_t _c6;
  value _v7;
  value _v8;
  struct ap_abstract1_t _c9;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  _c2 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c2 * sizeof(ap_var_t ), _ctx);
  for (_c3 = 0; _c3 < _c2; _c3++) {
    _v4 = Field(_v_v1, _c3);
    camlidl_ml2c_var_ap_var_t(_v4, &v1[_c3], _ctx);
  }
  v3 = _c2;
  _c5 = Wosize_val(_v_v2);
  v2 = camlidl_malloc(_c5 * sizeof(struct ap_linexpr1_t ), _ctx);
  for (_c6 = 0; _c6 < _c5; _c6++) {
    _v7 = Field(_v_v2, _c6);
    camlidl_ml2c_linexpr1_struct_ap_linexpr1_t(_v7, &v2[_c6], _ctx);
  }
  v4 = _c5;
  if (_v_dest == Val_int(0)) {
    dest = NULL;
  } else {
    _v8 = Field(_v_dest, 0);
    dest = &_c9;
    camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v8, &_c9, _ctx);
  }
  /* begin user-supplied calling sequence */

if (v3!=v4) caml_failwith("Abstract1.substitute_linexpr_array: arrays of different size");
_res = ap_abstract1_substitute_linexpr_array(man,false,a,v1,v2,v3,dest);

  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_assign_texpr_array(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2,
	value _v_dest)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  ap_var_t *v1; /*in*/
  struct ap_texpr1_t *v2; /*in*/
  int v3; /*in*/
  int v4; /*in*/
  struct ap_abstract1_t *dest; /*in*/
  struct ap_abstract1_t _res;
  struct ap_abstract1_t _c1;
  mlsize_t _c2;
  mlsize_t _c3;
  value _v4;
  mlsize_t _c5;
  mlsize_t _c6;
  value _v7;
  value _v8;
  struct ap_abstract1_t _c9;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  _c2 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c2 * sizeof(ap_var_t ), _ctx);
  for (_c3 = 0; _c3 < _c2; _c3++) {
    _v4 = Field(_v_v1, _c3);
    camlidl_ml2c_var_ap_var_t(_v4, &v1[_c3], _ctx);
  }
  v3 = _c2;
  _c5 = Wosize_val(_v_v2);
  v2 = camlidl_malloc(_c5 * sizeof(struct ap_texpr1_t ), _ctx);
  for (_c6 = 0; _c6 < _c5; _c6++) {
    _v7 = Field(_v_v2, _c6);
    camlidl_ml2c_texpr1_struct_ap_texpr1_t(_v7, &v2[_c6], _ctx);
  }
  v4 = _c5;
  if (_v_dest == Val_int(0)) {
    dest = NULL;
  } else {
    _v8 = Field(_v_dest, 0);
    dest = &_c9;
    camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v8, &_c9, _ctx);
  }
  /* begin user-supplied calling sequence */

if (v3!=v4) caml_failwith("Abstract1.assign_texpr_array: arrays of different size");
_res = ap_abstract1_assign_texpr_array(man,false,a,v1,v2,v3,dest);

  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_substitute_texpr_array(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2,
	value _v_dest)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  ap_var_t *v1; /*in*/
  struct ap_texpr1_t *v2; /*in*/
  int v3; /*in*/
  int v4; /*in*/
  struct ap_abstract1_t *dest; /*in*/
  struct ap_abstract1_t _res;
  struct ap_abstract1_t _c1;
  mlsize_t _c2;
  mlsize_t _c3;
  value _v4;
  mlsize_t _c5;
  mlsize_t _c6;
  value _v7;
  value _v8;
  struct ap_abstract1_t _c9;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  _c2 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c2 * sizeof(ap_var_t ), _ctx);
  for (_c3 = 0; _c3 < _c2; _c3++) {
    _v4 = Field(_v_v1, _c3);
    camlidl_ml2c_var_ap_var_t(_v4, &v1[_c3], _ctx);
  }
  v3 = _c2;
  _c5 = Wosize_val(_v_v2);
  v2 = camlidl_malloc(_c5 * sizeof(struct ap_texpr1_t ), _ctx);
  for (_c6 = 0; _c6 < _c5; _c6++) {
    _v7 = Field(_v_v2, _c6);
    camlidl_ml2c_texpr1_struct_ap_texpr1_t(_v7, &v2[_c6], _ctx);
  }
  v4 = _c5;
  if (_v_dest == Val_int(0)) {
    dest = NULL;
  } else {
    _v8 = Field(_v_dest, 0);
    dest = &_c9;
    camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v8, &_c9, _ctx);
  }
  /* begin user-supplied calling sequence */

if (v3!=v4) caml_failwith("Abstract1.substitute_texpr_array: arrays of different size");
_res = ap_abstract1_substitute_texpr_array(man,false,a,v1,v2,v3,dest);

  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_assign_linexpr_array_with(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2,
	value _v_dest)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  ap_var_t *v1; /*in*/
  struct ap_linexpr1_t *v2; /*in*/
  int v3; /*in*/
  int v4; /*in*/
  struct ap_abstract1_t *dest; /*in*/
  struct ap_abstract1_t _c1;
  mlsize_t _c2;
  mlsize_t _c3;
  value _v4;
  mlsize_t _c5;
  mlsize_t _c6;
  value _v7;
  value _v8;
  struct ap_abstract1_t _c9;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  _c2 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c2 * sizeof(ap_var_t ), _ctx);
  for (_c3 = 0; _c3 < _c2; _c3++) {
    _v4 = Field(_v_v1, _c3);
    camlidl_ml2c_var_ap_var_t(_v4, &v1[_c3], _ctx);
  }
  v3 = _c2;
  _c5 = Wosize_val(_v_v2);
  v2 = camlidl_malloc(_c5 * sizeof(struct ap_linexpr1_t ), _ctx);
  for (_c6 = 0; _c6 < _c5; _c6++) {
    _v7 = Field(_v_v2, _c6);
    camlidl_ml2c_linexpr1_struct_ap_linexpr1_t(_v7, &v2[_c6], _ctx);
  }
  v4 = _c5;
  if (_v_dest == Val_int(0)) {
    dest = NULL;
  } else {
    _v8 = Field(_v_dest, 0);
    dest = &_c9;
    camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v8, &_c9, _ctx);
  }
  /* begin user-supplied calling sequence */

if (v3!=v4) caml_failwith("Abstract1.assign_linexpr_array_with: arrays of different size");
{
ap_abstract1_t res = ap_abstract1_assign_linexpr_array(man,true,a,v1,v2,v3,dest);
value v = Field(_v_a,0);
*((ap_abstract0_ptr *) Data_custom_val(v)) = res.abstract0;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract1_ap_abstract1_substitute_linexpr_array_with(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2,
	value _v_dest)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  ap_var_t *v1; /*in*/
  struct ap_linexpr1_t *v2; /*in*/
  int v3; /*in*/
  int v4; /*in*/
  struct ap_abstract1_t *dest; /*in*/
  struct ap_abstract1_t _c1;
  mlsize_t _c2;
  mlsize_t _c3;
  value _v4;
  mlsize_t _c5;
  mlsize_t _c6;
  value _v7;
  value _v8;
  struct ap_abstract1_t _c9;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  _c2 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c2 * sizeof(ap_var_t ), _ctx);
  for (_c3 = 0; _c3 < _c2; _c3++) {
    _v4 = Field(_v_v1, _c3);
    camlidl_ml2c_var_ap_var_t(_v4, &v1[_c3], _ctx);
  }
  v3 = _c2;
  _c5 = Wosize_val(_v_v2);
  v2 = camlidl_malloc(_c5 * sizeof(struct ap_linexpr1_t ), _ctx);
  for (_c6 = 0; _c6 < _c5; _c6++) {
    _v7 = Field(_v_v2, _c6);
    camlidl_ml2c_linexpr1_struct_ap_linexpr1_t(_v7, &v2[_c6], _ctx);
  }
  v4 = _c5;
  if (_v_dest == Val_int(0)) {
    dest = NULL;
  } else {
    _v8 = Field(_v_dest, 0);
    dest = &_c9;
    camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v8, &_c9, _ctx);
  }
  /* begin user-supplied calling sequence */

if (v3!=v4) caml_failwith("Abstract1.substitute_linexpr_array_with: arrays of different size");
{
ap_abstract1_t res = ap_abstract1_substitute_linexpr_array(man,true,a,v1,v2,v3,dest);
value v = Field(_v_a,0);
*((ap_abstract0_ptr *) Data_custom_val(v)) = res.abstract0;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract1_ap_abstract1_assign_texpr_array_with(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2,
	value _v_dest)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  ap_var_t *v1; /*in*/
  struct ap_texpr1_t *v2; /*in*/
  int v3; /*in*/
  int v4; /*in*/
  struct ap_abstract1_t *dest; /*in*/
  struct ap_abstract1_t _c1;
  mlsize_t _c2;
  mlsize_t _c3;
  value _v4;
  mlsize_t _c5;
  mlsize_t _c6;
  value _v7;
  value _v8;
  struct ap_abstract1_t _c9;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  _c2 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c2 * sizeof(ap_var_t ), _ctx);
  for (_c3 = 0; _c3 < _c2; _c3++) {
    _v4 = Field(_v_v1, _c3);
    camlidl_ml2c_var_ap_var_t(_v4, &v1[_c3], _ctx);
  }
  v3 = _c2;
  _c5 = Wosize_val(_v_v2);
  v2 = camlidl_malloc(_c5 * sizeof(struct ap_texpr1_t ), _ctx);
  for (_c6 = 0; _c6 < _c5; _c6++) {
    _v7 = Field(_v_v2, _c6);
    camlidl_ml2c_texpr1_struct_ap_texpr1_t(_v7, &v2[_c6], _ctx);
  }
  v4 = _c5;
  if (_v_dest == Val_int(0)) {
    dest = NULL;
  } else {
    _v8 = Field(_v_dest, 0);
    dest = &_c9;
    camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v8, &_c9, _ctx);
  }
  /* begin user-supplied calling sequence */

if (v3!=v4) caml_failwith("Abstract1.assign_texpr_array_with: arrays of different size");
{
ap_abstract1_t res = ap_abstract1_assign_texpr_array(man,true,a,v1,v2,v3,dest);
value v = Field(_v_a,0);
*((ap_abstract0_ptr *) Data_custom_val(v)) = res.abstract0;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract1_ap_abstract1_substitute_texpr_array_with(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2,
	value _v_dest)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  ap_var_t *v1; /*in*/
  struct ap_texpr1_t *v2; /*in*/
  int v3; /*in*/
  int v4; /*in*/
  struct ap_abstract1_t *dest; /*in*/
  struct ap_abstract1_t _c1;
  mlsize_t _c2;
  mlsize_t _c3;
  value _v4;
  mlsize_t _c5;
  mlsize_t _c6;
  value _v7;
  value _v8;
  struct ap_abstract1_t _c9;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  _c2 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c2 * sizeof(ap_var_t ), _ctx);
  for (_c3 = 0; _c3 < _c2; _c3++) {
    _v4 = Field(_v_v1, _c3);
    camlidl_ml2c_var_ap_var_t(_v4, &v1[_c3], _ctx);
  }
  v3 = _c2;
  _c5 = Wosize_val(_v_v2);
  v2 = camlidl_malloc(_c5 * sizeof(struct ap_texpr1_t ), _ctx);
  for (_c6 = 0; _c6 < _c5; _c6++) {
    _v7 = Field(_v_v2, _c6);
    camlidl_ml2c_texpr1_struct_ap_texpr1_t(_v7, &v2[_c6], _ctx);
  }
  v4 = _c5;
  if (_v_dest == Val_int(0)) {
    dest = NULL;
  } else {
    _v8 = Field(_v_dest, 0);
    dest = &_c9;
    camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v8, &_c9, _ctx);
  }
  /* begin user-supplied calling sequence */

if (v3!=v4) caml_failwith("Abstract1.substitute_texpr_array_with: arrays of different size");
{
ap_abstract1_t res = ap_abstract1_substitute_texpr_array(man,true,a,v1,v2,v3,dest);
value v = Field(_v_a,0);
*((ap_abstract0_ptr *) Data_custom_val(v)) = res.abstract0;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract1_ap_abstract1_forget_array(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v3)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  ap_var_t *v1; /*in*/
  unsigned int v2; /*in*/
  int v3; /*in*/
  struct ap_abstract1_t _res;
  struct ap_abstract1_t _c1;
  mlsize_t _c2;
  mlsize_t _c3;
  value _v4;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  _c2 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c2 * sizeof(ap_var_t ), _ctx);
  for (_c3 = 0; _c3 < _c2; _c3++) {
    _v4 = Field(_v_v1, _c3);
    camlidl_ml2c_var_ap_var_t(_v4, &v1[_c3], _ctx);
  }
  v2 = _c2;
  v3 = Int_val(_v_v3);
  /* begin user-supplied calling sequence */
_res = ap_abstract1_forget_array(man,false,a,v1,v2,v3);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_forget_array_with(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v3)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  ap_var_t *v1; /*in*/
  int v2; /*in*/
  int v3; /*in*/
  struct ap_abstract1_t _c1;
  mlsize_t _c2;
  mlsize_t _c3;
  value _v4;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  _c2 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c2 * sizeof(ap_var_t ), _ctx);
  for (_c3 = 0; _c3 < _c2; _c3++) {
    _v4 = Field(_v_v1, _c3);
    camlidl_ml2c_var_ap_var_t(_v4, &v1[_c3], _ctx);
  }
  v2 = _c2;
  v3 = Int_val(_v_v3);
  /* begin user-supplied calling sequence */
{
ap_abstract1_t res = ap_abstract1_forget_array(man,true,a,v1,v2,v3);
  value v = Field(_v_a,0);
*((ap_abstract0_ptr *) Data_custom_val(v)) = res.abstract0;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract1_ap_abstract1_change_environment(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  ap_environment_ptr v1; /*in*/
  int v2; /*in*/
  struct ap_abstract1_t _res;
  struct ap_abstract1_t _c1;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  camlidl_ml2c_environment_ap_environment_ptr(_v_v1, &v1, _ctx);
  v2 = Int_val(_v_v2);
  /* begin user-supplied calling sequence */
_res = ap_abstract1_change_environment(man,false,a,v1,v2);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_minimize_environment(
	value _v_man,
	value _v_a)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  struct ap_abstract1_t _res;
  struct ap_abstract1_t _c1;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_abstract1_minimize_environment(man,false,a);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_rename_array(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  ap_var_t *v1; /*in*/
  ap_var_t *v2; /*in*/
  unsigned int v3; /*in*/
  unsigned int v4; /*in*/
  struct ap_abstract1_t _res;
  struct ap_abstract1_t _c1;
  mlsize_t _c2;
  mlsize_t _c3;
  value _v4;
  mlsize_t _c5;
  mlsize_t _c6;
  value _v7;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  _c2 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c2 * sizeof(ap_var_t ), _ctx);
  for (_c3 = 0; _c3 < _c2; _c3++) {
    _v4 = Field(_v_v1, _c3);
    camlidl_ml2c_var_ap_var_t(_v4, &v1[_c3], _ctx);
  }
  v3 = _c2;
  _c5 = Wosize_val(_v_v2);
  v2 = camlidl_malloc(_c5 * sizeof(ap_var_t ), _ctx);
  for (_c6 = 0; _c6 < _c5; _c6++) {
    _v7 = Field(_v_v2, _c6);
    camlidl_ml2c_var_ap_var_t(_v7, &v2[_c6], _ctx);
  }
  v4 = _c5;
  /* begin user-supplied calling sequence */

if (v3!=v4) caml_failwith("Abstract1.rename_array: arrays of different size");
_res = ap_abstract1_rename_array(man,false,a,v1,v2,v3);

  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_change_environment_with(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  ap_environment_ptr v1; /*in*/
  int v2; /*in*/
  struct ap_abstract1_t _c1;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  camlidl_ml2c_environment_ap_environment_ptr(_v_v1, &v1, _ctx);
  v2 = Int_val(_v_v2);
  /* begin user-supplied calling sequence */
{
ap_environment_t* oldenv = ap_environment_copy(a->env);
		ap_abstract1_t res = ap_abstract1_change_environment(man,true,a,v1,v2);
value v = Field(_v_a,0);
*((ap_abstract0_ptr *) Data_custom_val(v)) = res.abstract0;
if (res.env==oldenv){
ap_environment_free(oldenv);
}
else {
assert(res.env==v1);
Store_field(_v_a,1,_v_v1);
}
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract1_ap_abstract1_minimize_environment_with(
	value _v_man,
	value _v_a)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  struct ap_abstract1_t _c1;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  /* begin user-supplied calling sequence */
{
ap_environment_t* oldenv = ap_environment_copy(a->env);
		ap_abstract1_t res = ap_abstract1_minimize_environment(man,true,a);
value v = Field(_v_a,0);
*((ap_abstract0_ptr *) Data_custom_val(v)) = res.abstract0;
if (res.env==oldenv){
ap_environment_free(oldenv);
}
else {
v = camlidl_apron_environment_ptr_c2ml(&res.env); 
Store_field(_v_a,1,v); 
}
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract1_ap_abstract1_rename_array_with(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  ap_var_t *v1; /*in*/
  ap_var_t *v2; /*in*/
  unsigned int v3; /*in*/
  unsigned int v4; /*in*/
  struct ap_abstract1_t _c1;
  mlsize_t _c2;
  mlsize_t _c3;
  value _v4;
  mlsize_t _c5;
  mlsize_t _c6;
  value _v7;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  _c2 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c2 * sizeof(ap_var_t ), _ctx);
  for (_c3 = 0; _c3 < _c2; _c3++) {
    _v4 = Field(_v_v1, _c3);
    camlidl_ml2c_var_ap_var_t(_v4, &v1[_c3], _ctx);
  }
  v3 = _c2;
  _c5 = Wosize_val(_v_v2);
  v2 = camlidl_malloc(_c5 * sizeof(ap_var_t ), _ctx);
  for (_c6 = 0; _c6 < _c5; _c6++) {
    _v7 = Field(_v_v2, _c6);
    camlidl_ml2c_var_ap_var_t(_v7, &v2[_c6], _ctx);
  }
  v4 = _c5;
  /* begin user-supplied calling sequence */

if (v3!=v4) caml_failwith("Abstract1.rename_array_with: arrays of different size");
{
ap_environment_t* oldenv = ap_environment_copy(a->env);
		ap_abstract1_t res = ap_abstract1_rename_array(man,true,a,v1,v2,v3);
value v = Field(_v_a,0);
*((ap_abstract0_ptr *) Data_custom_val(v)) = res.abstract0;
if (res.env==oldenv){
ap_environment_free(oldenv);
}
else {
v = camlidl_apron_environment_ptr_c2ml(&res.env); 
Store_field(_v_a,1,v); 
}
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract1_ap_abstract1_expand(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  ap_var_t v1; /*in*/
  ap_var_t *v2; /*in*/
  int v3; /*in*/
  struct ap_abstract1_t _res;
  struct ap_abstract1_t _c1;
  mlsize_t _c2;
  mlsize_t _c3;
  value _v4;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  camlidl_ml2c_var_ap_var_t(_v_v1, &v1, _ctx);
  _c2 = Wosize_val(_v_v2);
  v2 = camlidl_malloc(_c2 * sizeof(ap_var_t ), _ctx);
  for (_c3 = 0; _c3 < _c2; _c3++) {
    _v4 = Field(_v_v2, _c3);
    camlidl_ml2c_var_ap_var_t(_v4, &v2[_c3], _ctx);
  }
  v3 = _c2;
  /* begin user-supplied calling sequence */
_res = ap_abstract1_expand(man,false,a,v1,v2,v3);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_fold(
	value _v_man,
	value _v_a,
	value _v_v1)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  ap_var_t *v1; /*in*/
  int v2; /*in*/
  struct ap_abstract1_t _res;
  struct ap_abstract1_t _c1;
  mlsize_t _c2;
  mlsize_t _c3;
  value _v4;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  _c2 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c2 * sizeof(ap_var_t ), _ctx);
  for (_c3 = 0; _c3 < _c2; _c3++) {
    _v4 = Field(_v_v1, _c3);
    camlidl_ml2c_var_ap_var_t(_v4, &v1[_c3], _ctx);
  }
  v2 = _c2;
  /* begin user-supplied calling sequence */
_res = ap_abstract1_fold(man,false,a,v1,v2);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_expand_with(
	value _v_man,
	value _v_a,
	value _v_v1,
	value _v_v2)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  ap_var_t v1; /*in*/
  ap_var_t *v2; /*in*/
  int v3; /*in*/
  struct ap_abstract1_t _c1;
  mlsize_t _c2;
  mlsize_t _c3;
  value _v4;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  camlidl_ml2c_var_ap_var_t(_v_v1, &v1, _ctx);
  _c2 = Wosize_val(_v_v2);
  v2 = camlidl_malloc(_c2 * sizeof(ap_var_t ), _ctx);
  for (_c3 = 0; _c3 < _c2; _c3++) {
    _v4 = Field(_v_v2, _c3);
    camlidl_ml2c_var_ap_var_t(_v4, &v2[_c3], _ctx);
  }
  v3 = _c2;
  /* begin user-supplied calling sequence */
{
ap_environment_t* oldenv = ap_environment_copy(a->env);
		ap_abstract1_t res = ap_abstract1_expand(man,true,a,v1,v2,v3);
  value v = Field(_v_a,0);
*((ap_abstract0_ptr *) Data_custom_val(v)) = res.abstract0;
if (res.env==oldenv){
ap_environment_free(oldenv);
}
else {
v = camlidl_apron_environment_ptr_c2ml(&res.env); 
Store_field(_v_a,1,v); 
}
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract1_ap_abstract1_fold_with(
	value _v_man,
	value _v_a,
	value _v_v1)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  ap_var_t *v1; /*in*/
  int v2; /*in*/
  struct ap_abstract1_t _c1;
  mlsize_t _c2;
  mlsize_t _c3;
  value _v4;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  _c2 = Wosize_val(_v_v1);
  v1 = camlidl_malloc(_c2 * sizeof(ap_var_t ), _ctx);
  for (_c3 = 0; _c3 < _c2; _c3++) {
    _v4 = Field(_v_v1, _c3);
    camlidl_ml2c_var_ap_var_t(_v4, &v1[_c3], _ctx);
  }
  v2 = _c2;
  /* begin user-supplied calling sequence */
{
ap_environment_t* oldenv = ap_environment_copy(a->env);
		ap_abstract1_t res = ap_abstract1_fold(man,true,a,v1,v2);
  value v = Field(_v_a,0);
*((ap_abstract0_ptr *) Data_custom_val(v)) = res.abstract0;
if (res.env==oldenv){
ap_environment_free(oldenv);
}
else {
v = camlidl_apron_environment_ptr_c2ml(&res.env); 
Store_field(_v_a,1,v); 
}
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract1_ap_abstract1_widening(
	value _v_man,
	value _v_a1,
	value _v_a2)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a1; /*in*/
  struct ap_abstract1_t *a2; /*in*/
  struct ap_abstract1_t _res;
  struct ap_abstract1_t _c1;
  struct ap_abstract1_t _c2;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a1 = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a1, &_c1, _ctx);
  a2 = &_c2;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a2, &_c2, _ctx);
  _res = ap_abstract1_widening(man, a1, a2);
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_widening_threshold(
	value _v_man,
	value _v_a1,
	value _v_a2,
	value _v_v)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a1; /*in*/
  struct ap_abstract1_t *a2; /*in*/
  struct ap_lincons1_array_t *v; /*in*/
  struct ap_abstract1_t _res;
  struct ap_abstract1_t _c1;
  struct ap_abstract1_t _c2;
  struct ap_lincons1_array_t _c3;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a1 = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a1, &_c1, _ctx);
  a2 = &_c2;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a2, &_c2, _ctx);
  v = &_c3;
  camlidl_ml2c_lincons1_struct_ap_lincons1_array_t(_v_v, &_c3, _ctx);
  _res = ap_abstract1_widening_threshold(man, a1, a2, v);
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_closure(
	value _v_man,
	value _v_a)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  struct ap_abstract1_t _res;
  struct ap_abstract1_t _c1;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_abstract1_closure(man,false,a);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_closure_with(
	value _v_man,
	value _v_a)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a; /*in*/
  struct ap_abstract1_t _c1;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a, &_c1, _ctx);
  /* begin user-supplied calling sequence */
{
ap_abstract1_t res = ap_abstract1_closure(man,true,a);
  value v = Field(_v_a,0);
*((ap_abstract0_ptr *) Data_custom_val(v)) = res.abstract0;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

value camlidl_abstract1_ap_abstract1_unify(
	value _v_man,
	value _v_a1,
	value _v_a2)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a1; /*in*/
  struct ap_abstract1_t *a2; /*in*/
  struct ap_abstract1_t _res;
  struct ap_abstract1_t _c1;
  struct ap_abstract1_t _c2;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a1 = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a1, &_c1, _ctx);
  a2 = &_c2;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a2, &_c2, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_abstract1_unify(man,false,a1,a2);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract1_struct_ap_abstract1_t(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_abstract1_ap_abstract1_unify_with(
	value _v_man,
	value _v_a1,
	value _v_a2)
{
  ap_manager_ptr man; /*in*/
  struct ap_abstract1_t *a1; /*in*/
  struct ap_abstract1_t *a2; /*in*/
  struct ap_abstract1_t _c1;
  struct ap_abstract1_t _c2;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  a1 = &_c1;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a1, &_c1, _ctx);
  a2 = &_c2;
  camlidl_ml2c_abstract1_struct_ap_abstract1_t(_v_a2, &_c2, _ctx);
  /* begin user-supplied calling sequence */
{
ap_abstract1_t res = ap_abstract1_unify(man,true,a1,a2);
value v = Field(_v_a1,0);
*((ap_abstract0_ptr *) Data_custom_val(v)) = res.abstract0;
v = Field(_v_a1,1);
*((ap_environment_ptr *) Data_custom_val(v)) = res.env;
}
  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return Val_unit;
}

