/* File generated from environment.idl */

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
#include "ap_environment.h"
#include "caml/mlvalues.h"
#include "apron_caml.h"

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

extern void camlidl_apron_var_ptr_ml2c(value, ap_var_t *);
#define camlidl_ml2c_var_ap_var_t(v,c,ctx) camlidl_apron_var_ptr_ml2c(v,c)

extern value camlidl_apron_var_ptr_c2ml(ap_var_t *);
#define camlidl_c2ml_var_ap_var_t(c,ctx) camlidl_apron_var_ptr_c2ml(c)


enum typvar {
AP_INT,AP_REAL
};

int camlidl_transl_table_environment_enum_4[2] = {
  AP_INT,
  AP_REAL,
};

int camlidl_ml2c_environment_enum_typvar(value _v1)
{
  int _c2;
  _c2 = camlidl_transl_table_environment_enum_4[Int_val(_v1)];
  return _c2;
}

value camlidl_c2ml_environment_enum_typvar(int _c1)
{
  value _v2;
  switch(_c1) {
  case AP_INT: _v2 = Val_int(0); break;
  case AP_REAL: _v2 = Val_int(1); break;
  default: invalid_argument("enum typvar: bad enum typvar value");
  }
  return _v2;
}

#define camlidl_ml2c_environment_ap_environment_ptr(v,c,ctx) camlidl_apron_environment_ptr_ml2c(v,c)

#define camlidl_c2ml_environment_ap_environment_ptr(c,ctx) camlidl_apron_environment_ptr_c2ml(c)

value camlidl_environment_ap_environment_make(
	value _v_name_of_intdim,
	value _v_name_of_realdim)
{
  ap_var_t *name_of_intdim; /*in*/
  int intdim; /*in*/
  ap_var_t *name_of_realdim; /*in*/
  int realdim; /*in*/
  ap_environment_ptr _res;
  mlsize_t _c1;
  mlsize_t _c2;
  value _v3;
  mlsize_t _c4;
  mlsize_t _c5;
  value _v6;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  _c1 = Wosize_val(_v_name_of_intdim);
  name_of_intdim = camlidl_malloc(_c1 * sizeof(ap_var_t ), _ctx);
  for (_c2 = 0; _c2 < _c1; _c2++) {
    _v3 = Field(_v_name_of_intdim, _c2);
    camlidl_ml2c_var_ap_var_t(_v3, &name_of_intdim[_c2], _ctx);
  }
  intdim = _c1;
  _c4 = Wosize_val(_v_name_of_realdim);
  name_of_realdim = camlidl_malloc(_c4 * sizeof(ap_var_t ), _ctx);
  for (_c5 = 0; _c5 < _c4; _c5++) {
    _v6 = Field(_v_name_of_realdim, _c5);
    camlidl_ml2c_var_ap_var_t(_v6, &name_of_realdim[_c5], _ctx);
  }
  realdim = _c4;
  /* begin user-supplied calling sequence */

_res = ap_environment_alloc(name_of_intdim,intdim,name_of_realdim,realdim);
if (_res==NULL){
camlidl_free(_ctx);
caml_failwith("Environment.make: duplicated variable names");
}
assert(_res->count >= 1);

  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_environment_ap_environment_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_environment_ap_environment_add(
	value _v_e,
	value _v_name_of_intdim,
	value _v_name_of_realdim)
{
  ap_environment_ptr e; /*in*/
  ap_var_t *name_of_intdim; /*in*/
  int intdim; /*in*/
  ap_var_t *name_of_realdim; /*in*/
  int realdim; /*in*/
  ap_environment_ptr _res;
  mlsize_t _c1;
  mlsize_t _c2;
  value _v3;
  mlsize_t _c4;
  mlsize_t _c5;
  value _v6;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_environment_ap_environment_ptr(_v_e, &e, _ctx);
  _c1 = Wosize_val(_v_name_of_intdim);
  name_of_intdim = camlidl_malloc(_c1 * sizeof(ap_var_t ), _ctx);
  for (_c2 = 0; _c2 < _c1; _c2++) {
    _v3 = Field(_v_name_of_intdim, _c2);
    camlidl_ml2c_var_ap_var_t(_v3, &name_of_intdim[_c2], _ctx);
  }
  intdim = _c1;
  _c4 = Wosize_val(_v_name_of_realdim);
  name_of_realdim = camlidl_malloc(_c4 * sizeof(ap_var_t ), _ctx);
  for (_c5 = 0; _c5 < _c4; _c5++) {
    _v6 = Field(_v_name_of_realdim, _c5);
    camlidl_ml2c_var_ap_var_t(_v6, &name_of_realdim[_c5], _ctx);
  }
  realdim = _c4;
  /* begin user-supplied calling sequence */

_res = ap_environment_add(e,name_of_intdim,intdim,name_of_realdim,realdim);
if (_res==NULL){
camlidl_free(_ctx);
caml_failwith("Environment.add: duplicated variable names");
}
assert(_res->count >= 1);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_environment_ap_environment_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_environment_ap_environment_remove(
	value _v_e,
	value _v_tvar)
{
  ap_environment_ptr e; /*in*/
  ap_var_t *tvar; /*in*/
  int size; /*in*/
  ap_environment_ptr _res;
  mlsize_t _c1;
  mlsize_t _c2;
  value _v3;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_environment_ap_environment_ptr(_v_e, &e, _ctx);
  _c1 = Wosize_val(_v_tvar);
  tvar = camlidl_malloc(_c1 * sizeof(ap_var_t ), _ctx);
  for (_c2 = 0; _c2 < _c1; _c2++) {
    _v3 = Field(_v_tvar, _c2);
    camlidl_ml2c_var_ap_var_t(_v3, &tvar[_c2], _ctx);
  }
  size = _c1;
  /* begin user-supplied calling sequence */

_res = ap_environment_remove(e,tvar,size);
if (_res==NULL){
camlidl_free(_ctx);
caml_failwith("Environment.remove: unknown variable names");
}
assert(_res->count >= 1);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_environment_ap_environment_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_environment_ap_environment_rename(
	value _v_e,
	value _v_tvar1,
	value _v_tvar2)
{
  ap_environment_ptr e; /*in*/
  ap_var_t *tvar1; /*in*/
  ap_var_t *tvar2; /*in*/
  int size1; /*in*/
  int size2; /*in*/
  ap_environment_ptr _res;
  mlsize_t _c1;
  mlsize_t _c2;
  value _v3;
  mlsize_t _c4;
  mlsize_t _c5;
  value _v6;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_environment_ap_environment_ptr(_v_e, &e, _ctx);
  _c1 = Wosize_val(_v_tvar1);
  tvar1 = camlidl_malloc(_c1 * sizeof(ap_var_t ), _ctx);
  for (_c2 = 0; _c2 < _c1; _c2++) {
    _v3 = Field(_v_tvar1, _c2);
    camlidl_ml2c_var_ap_var_t(_v3, &tvar1[_c2], _ctx);
  }
  size1 = _c1;
  _c4 = Wosize_val(_v_tvar2);
  tvar2 = camlidl_malloc(_c4 * sizeof(ap_var_t ), _ctx);
  for (_c5 = 0; _c5 < _c4; _c5++) {
    _v6 = Field(_v_tvar2, _c5);
    camlidl_ml2c_var_ap_var_t(_v6, &tvar2[_c5], _ctx);
  }
  size2 = _c4;
  /* begin user-supplied calling sequence */

if (size1!=size2){
camlidl_free(_ctx);
caml_failwith("Environment.rename: arrays of different sizes");
}
else {
struct ap_dimperm_t perm;
_res = ap_environment_rename(e,tvar1,tvar2,size1,&perm);
ap_dimperm_clear(&perm);
if (_res==NULL){
camlidl_free(_ctx);
caml_failwith("Environment.rename: unknown variables or interference of new variables with unrenamed variables");
}
}
assert(_res->count >= 1);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_environment_ap_environment_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_environment_ap_environment_rename_perm(
	value _v_e,
	value _v_tvar1,
	value _v_tvar2)
{
  ap_environment_ptr e; /*in*/
  ap_var_t *tvar1; /*in*/
  ap_var_t *tvar2; /*in*/
  int size1; /*in*/
  int size2; /*in*/
  struct ap_dimperm_t perm; /*out*/
  ap_environment_ptr _res;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  mlsize_t _c1;
  mlsize_t _c2;
  value _v3;
  mlsize_t _c4;
  mlsize_t _c5;
  value _v6;
  value _vresult;
  value _vres[2] = { 0, 0, };

  camlidl_ml2c_environment_ap_environment_ptr(_v_e, &e, _ctx);
  _c1 = Wosize_val(_v_tvar1);
  tvar1 = camlidl_malloc(_c1 * sizeof(ap_var_t ), _ctx);
  for (_c2 = 0; _c2 < _c1; _c2++) {
    _v3 = Field(_v_tvar1, _c2);
    camlidl_ml2c_var_ap_var_t(_v3, &tvar1[_c2], _ctx);
  }
  size1 = _c1;
  _c4 = Wosize_val(_v_tvar2);
  tvar2 = camlidl_malloc(_c4 * sizeof(ap_var_t ), _ctx);
  for (_c5 = 0; _c5 < _c4; _c5++) {
    _v6 = Field(_v_tvar2, _c5);
    camlidl_ml2c_var_ap_var_t(_v6, &tvar2[_c5], _ctx);
  }
  size2 = _c4;
  /* begin user-supplied calling sequence */

if (size1!=size2){
camlidl_free(_ctx);
caml_failwith("Environment.rename: arrays of different sizes");
}
else {
_res = ap_environment_rename(e,tvar1,tvar2,size1,&perm);
if (_res==NULL){
camlidl_free(_ctx);
caml_failwith("Environment.rename_dimperm: unknown variables or interference of new variables with unrenamed variables");
}
}
assert(_res->count >= 1);
  /* end user-supplied calling sequence */
  Begin_roots_block(_vres, 2)
    _vres[0] = camlidl_c2ml_environment_ap_environment_ptr(&_res, _ctx);
    _vres[1] = camlidl_c2ml_dim_struct_ap_dimperm_t(&perm, _ctx);
    _vresult = camlidl_alloc_small(2, 0);
    Field(_vresult, 0) = _vres[0];
    Field(_vresult, 1) = _vres[1];
  End_roots()
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
ap_dimperm_clear(&perm);
  /* end user-supplied deallocation sequence */
  return _vresult;
}

value camlidl_environment_ap_environment_lce(
	value _v_e1,
	value _v_e2)
{
  ap_environment_ptr e1; /*in*/
  ap_environment_ptr e2; /*in*/
  ap_environment_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_environment_ap_environment_ptr(_v_e1, &e1, _ctx);
  camlidl_ml2c_environment_ap_environment_ptr(_v_e2, &e2, _ctx);
  /* begin user-supplied calling sequence */

{
ap_dimchange_t *c1;
ap_dimchange_t *c2;
_res = ap_environment_lce(e1,e2,&c1,&c2);
if (c1) ap_dimchange_free(c1);
if (c2) ap_dimchange_free(c2);
}
if (_res==NULL){
camlidl_free(_ctx);
caml_failwith("Environment.lce: variable with two different types");
}
assert(_res->count >= 1);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_environment_ap_environment_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_environment_ap_environment_lce_change(
	value _v_e1,
	value _v_e2)
{
  ap_environment_ptr e1; /*in*/
  ap_environment_ptr e2; /*in*/
  ap_dimchange_t *c1; /*out*/
  ap_dimchange_t *c2; /*out*/
  ap_environment_ptr _res;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  ap_dimchange_t _c1;
  ap_dimchange_t _c2;
  value _v3;
  value _v4;
  value _vresult;
  value _vres[3] = { 0, 0, 0, };

  camlidl_ml2c_environment_ap_environment_ptr(_v_e1, &e1, _ctx);
  camlidl_ml2c_environment_ap_environment_ptr(_v_e2, &e2, _ctx);
  c1 = &_c1;
  c2 = &_c2;
  /* begin user-supplied calling sequence */

{
_res = ap_environment_lce(e1,e2,&c1,&c2);
}
if (_res==NULL){
camlidl_free(_ctx);
caml_failwith("Environment.lce: variable with two different types");
}
assert(_res->count >= 1);
  /* end user-supplied calling sequence */
  Begin_roots_block(_vres, 3)
    _vres[0] = camlidl_c2ml_environment_ap_environment_ptr(&_res, _ctx);
    if (c1 == NULL) {
      _vres[1] = Val_int(0);
    } else {
      _v3 = camlidl_c2ml_dim_ap_dimchange_t(&*c1, _ctx);
      Begin_root(_v3)
        _vres[1] = camlidl_alloc_small(1, 0);
        Field(_vres[1], 0) = _v3;
      End_roots();
    }
    if (c2 == NULL) {
      _vres[2] = Val_int(0);
    } else {
      _v4 = camlidl_c2ml_dim_ap_dimchange_t(&*c2, _ctx);
      Begin_root(_v4)
        _vres[2] = camlidl_alloc_small(1, 0);
        Field(_vres[2], 0) = _v4;
      End_roots();
    }
    _vresult = camlidl_alloc_small(3, 0);
    Field(_vresult, 0) = _vres[0];
    Field(_vresult, 1) = _vres[1];
    Field(_vresult, 2) = _vres[2];
  End_roots()
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */

  if (c1) ap_dimchange_free(c1);
  if (c2) ap_dimchange_free(c2);

  /* end user-supplied deallocation sequence */
  return _vresult;
}

value camlidl_environment_ap_environment_dimchange(
	value _v_e1,
	value _v_e2)
{
  ap_environment_ptr e1; /*in*/
  ap_environment_ptr e2; /*in*/
  ap_dimchange_t *_res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_environment_ap_environment_ptr(_v_e1, &e1, _ctx);
  camlidl_ml2c_environment_ap_environment_ptr(_v_e2, &e2, _ctx);
  /* begin user-supplied calling sequence */

_res = ap_environment_dimchange(e1,e2);
if (_res==NULL){
camlidl_free(_ctx);
caml_failwith("Environment.dimchange: the second environment is not a superenvironment of the first one");
}

  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_dim_ap_dimchange_t(&*_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
ap_dimchange_free(_res);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_environment_ap_environment_dimchange2(
	value _v_e1,
	value _v_e2)
{
  ap_environment_ptr e1; /*in*/
  ap_environment_ptr e2; /*in*/
  struct ap_dimchange2_t *_res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_environment_ap_environment_ptr(_v_e1, &e1, _ctx);
  camlidl_ml2c_environment_ap_environment_ptr(_v_e2, &e2, _ctx);
  /* begin user-supplied calling sequence */

_res = ap_environment_dimchange2(e1,e2);
if (_res==NULL){
camlidl_free(_ctx);
caml_failwith("Environment.dimchange2: the two environments are not compatible");
}

  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_dim_struct_ap_dimchange2_t(&*_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
ap_dimchange2_free(_res);
  /* end user-supplied deallocation sequence */
  return _vres;
}

value camlidl_environment_ap_environment_equal(
	value _v_env1,
	value _v_env2)
{
  ap_environment_ptr env1; /*in*/
  ap_environment_ptr env2; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_environment_ap_environment_ptr(_v_env1, &env1, _ctx);
  camlidl_ml2c_environment_ap_environment_ptr(_v_env2, &env2, _ctx);
  /* begin user-supplied calling sequence */

_res = ap_environment_is_eq(env1,env2);

  /* end user-supplied calling sequence */
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_environment_ap_environment_compare(
	value _v_env1,
	value _v_env2)
{
  ap_environment_ptr env1; /*in*/
  ap_environment_ptr env2; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_environment_ap_environment_ptr(_v_env1, &env1, _ctx);
  camlidl_ml2c_environment_ap_environment_ptr(_v_env2, &env2, _ctx);
  _res = ap_environment_compare(env1, env2);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_environment_ap_environment_hash(
	value _v_env)
{
  ap_environment_ptr env; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_environment_ap_environment_ptr(_v_env, &env, _ctx);
  _res = ap_environment_hash(env);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_environment_ap_environment_dimension(
	value _v_e)
{
  ap_environment_ptr e; /*in*/
  struct ap_dimension_t _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_environment_ap_environment_ptr(_v_e, &e, _ctx);
  /* begin user-supplied calling sequence */
_res.intdim = e->intdim; _res.realdim = e->realdim;
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_dim_struct_ap_dimension_t(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_environment_ap_environment_size(
	value _v_e)
{
  ap_environment_ptr e; /*in*/
  unsigned int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_environment_ap_environment_ptr(_v_e, &e, _ctx);
  /* begin user-supplied calling sequence */
_res = e->intdim + e->realdim;
  /* end user-supplied calling sequence */
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_environment_ap_environment_mem_var(
	value _v_e,
	value _v_var)
{
  ap_environment_ptr e; /*in*/
  ap_var_t var; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_environment_ap_environment_ptr(_v_e, &e, _ctx);
  camlidl_ml2c_var_ap_var_t(_v_var, &var, _ctx);
  /* begin user-supplied calling sequence */

{
ap_dim_t dim;
dim = ap_environment_dim_of_var(e,var);
_res = (dim != AP_DIM_MAX);
}
  /* end user-supplied calling sequence */
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_environment_ap_environment_typ_of_var(
	value _v_e,
	value _v_var)
{
  ap_environment_ptr e; /*in*/
  ap_var_t var; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_environment_ap_environment_ptr(_v_e, &e, _ctx);
  camlidl_ml2c_var_ap_var_t(_v_var, &var, _ctx);
  /* begin user-supplied calling sequence */

{
ap_dim_t dim;
dim = ap_environment_dim_of_var(e,var);
if (dim==AP_DIM_MAX)
caml_failwith("Environment.dim_of_var: unknown variable in the environment");
_res =
dim=( dim<e->intdim ? AP_INT : AP_REAL);
}
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_environment_enum_typvar(_res);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_environment_ap_environment_vars(
	value _v_e)
{
  ap_environment_ptr e; /*in*/
  ap_var_t *name_of_intdim; /*out*/
  ap_var_t *name_of_realdim; /*out*/
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  mlsize_t _c1;
  value _v2;
  mlsize_t _c3;
  value _v4;
  value _vresult;
  value _vres[2] = { 0, 0, };

  camlidl_ml2c_environment_ap_environment_ptr(_v_e, &e, _ctx);
  name_of_intdim = camlidl_malloc((*e).intdim * sizeof(ap_var_t ), _ctx);
  name_of_realdim = camlidl_malloc((*e).realdim * sizeof(ap_var_t ), _ctx);
  /* begin user-supplied calling sequence */

{
size_t i;
for(i=0;i<e->intdim;i++) name_of_intdim[i] = ap_var_operations->copy(e->var_of_dim[i]);
 for(i=0;i<e->realdim;i++) name_of_realdim[i] = ap_var_operations->copy(e->var_of_dim[e->intdim+i]);
 }
  /* end user-supplied calling sequence */
  Begin_roots_block(_vres, 2)
    _vres[0] = camlidl_alloc((*e).intdim, 0);
    Begin_root(_vres[0])
      for (_c1 = 0; _c1 < (*e).intdim; _c1++) {
        _v2 = camlidl_c2ml_var_ap_var_t(&name_of_intdim[_c1], _ctx);
        modify(&Field(_vres[0], _c1), _v2);
      }
    End_roots()
    _vres[1] = camlidl_alloc((*e).realdim, 0);
    Begin_root(_vres[1])
      for (_c3 = 0; _c3 < (*e).realdim; _c3++) {
        _v4 = camlidl_c2ml_var_ap_var_t(&name_of_realdim[_c3], _ctx);
        modify(&Field(_vres[1], _c3), _v4);
      }
    End_roots()
    _vresult = camlidl_alloc_small(2, 0);
    Field(_vresult, 0) = _vres[0];
    Field(_vresult, 1) = _vres[1];
  End_roots()
  camlidl_free(_ctx);
  return _vresult;
}

value camlidl_environment_ap_environment_var_of_dim(
	value _v_e,
	value _v_dim)
{
  ap_environment_ptr e; /*in*/
  ap_dim_t dim; /*in*/
  ap_var_t _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_environment_ap_environment_ptr(_v_e, &e, _ctx);
  camlidl_ml2c_dim_ap_dim_t(_v_dim, &dim, _ctx);
  /* begin user-supplied calling sequence */

if (dim>=e->intdim+e->realdim){
caml_failwith("Environment.var_of_dim: dim out of range w.r.t. the environment");
}
_res = ap_var_operations->copy(e->var_of_dim[dim]);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_var_ap_var_t(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_environment_ap_environment_dim_of_var(
	value _v_e,
	value _v_var)
{
  ap_environment_ptr e; /*in*/
  ap_var_t var; /*in*/
  ap_dim_t _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_environment_ap_environment_ptr(_v_e, &e, _ctx);
  camlidl_ml2c_var_ap_var_t(_v_var, &var, _ctx);
  /* begin user-supplied calling sequence */

_res = ap_environment_dim_of_var(e,var);
if (_res==AP_DIM_MAX){
caml_failwith("Environment.dim_of_var: unknown variable in the environment");
}
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_dim_ap_dim_t(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

