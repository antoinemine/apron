/* File generated from texpr1.idl */

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
#include "ap_texpr1.h"
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


void camlidl_ml2c_texpr1_struct_ap_texpr1_t(value _v1, struct ap_texpr1_t * _c2, camlidl_ctx _ctx)
{
  value _v3;
  value _v4;
  _v3 = Field(_v1, 0);
  camlidl_ml2c_texpr0_ap_texpr0_ptr(_v3, &(*_c2).texpr0, _ctx);
  _v4 = Field(_v1, 1);
  camlidl_ml2c_environment_ap_environment_ptr(_v4, &(*_c2).env, _ctx);
}

value camlidl_c2ml_texpr1_struct_ap_texpr1_t(struct ap_texpr1_t * _c1, camlidl_ctx _ctx)
{
  value _v2;
  value _v3[2];
  _v3[0] = _v3[1] = 0;
  Begin_roots_block(_v3, 2)
    _v3[0] = camlidl_c2ml_texpr0_ap_texpr0_ptr(&(*_c1).texpr0, _ctx);
    _v3[1] = camlidl_c2ml_environment_ap_environment_ptr(&(*_c1).env, _ctx);
    _v2 = camlidl_alloc_small(2, 0);
    Field(_v2, 0) = _v3[0];
    Field(_v2, 1) = _v3[1];
  End_roots()
  return _v2;
}


value camlidl_texpr1_ap_texpr1_to_expr_c2ml(ap_environment_t* env, ap_texpr0_t* expr, camlidl_ctx _ctx)
{
value _v_res ;

switch(expr->discr){
case AP_TEXPR_CST:
{
value v0 = Val_unit;
Begin_root(v0);
v0 = camlidl_c2ml_coeff_struct_ap_coeff_t(&expr->val.cst, _ctx);
_v_res = caml_alloc_small(1,0);
Field(_v_res,0) = v0;
End_roots();
}
break;
case AP_TEXPR_DIM:
{
ap_var_t var;
value v0 = Val_unit;
var = ap_environment_var_of_dim(env,expr->val.dim);
assert(var!=NULL);
var = ap_var_copy(var);
Begin_root(v0);
v0 = camlidl_apron_var_ptr_c2ml(&var);
_v_res = caml_alloc_small(1,1);
Field(_v_res,0) = v0;
End_roots();
}
break;
case AP_TEXPR_NODE:
{
value v0,v1,v2,v3,v4;
v0 = v1 = v2 = v3 = v4 = Val_unit;
Begin_roots5(v0,v1,v2,v3,v4);
ap_texpr0_node_t* node = expr->val.node;
v1 = camlidl_texpr1_ap_texpr1_to_expr_c2ml(env,node->exprA,_ctx);
v3 = camlidl_apron_texpr_rtype_t_c2ml(&node->type);
v4 = camlidl_apron_texpr_rdir_t_c2ml(&node->dir);
if (ap_texpr_is_unop(node->op)){
v0 = camlidl_apron_texpr_unop_t_c2ml(&node->op);
_v_res = caml_alloc_small(4,2);
Field(_v_res,0) = v0;
Field(_v_res,1) = v1;
Field(_v_res,2) = v3;
Field(_v_res,3) = v4;
}
else {
v0 = camlidl_apron_texpr_binop_t_c2ml(&node->op);
v2 = camlidl_texpr1_ap_texpr1_to_expr_c2ml(env,node->exprB,_ctx);
_v_res = caml_alloc_small(5,3);
Field(_v_res,0) = v0;
Field(_v_res,1) = v1;
Field(_v_res,2) = v2;
Field(_v_res,3) = v3;
Field(_v_res,4) = v4;
}
End_roots();
}
break;
default:
abort();
}
return _v_res;
}
value camlidl_texpr1_ap_texpr1_to_expr(value _v_texpr)
{
CAMLparam1(_v_texpr);
CAMLlocal1(_v_res);
ap_environment_t* env;
ap_texpr1_t texpr1;

struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
camlidl_ctx _ctx = &_ctxs;

camlidl_ml2c_texpr1_struct_ap_texpr1_t(_v_texpr,&texpr1,_ctx);
_v_res = camlidl_texpr1_ap_texpr1_to_expr_c2ml(texpr1.env,texpr1.texpr0,_ctx);
camlidl_free(_ctx);

CAMLreturn(_v_res);
}

void camlidl_texpr1_ap_texpr0_of_expr_ml2c(value v, ap_environment_t* env, ap_texpr0_t** res, camlidl_ctx _ctx)
{
ap_texpr0_t* texpr;
ap_texpr0_node_t* node;
ap_coeff_t coeff;
ap_var_t var;
ap_dim_t dim;
ap_texpr0_t* exprA;
ap_texpr0_t* exprB;
value v0,v1,v2,v3,v4;

texpr = malloc(sizeof(ap_texpr0_t));
 
assert (Is_block(v));
switch (Tag_val(v)){
case 0: /* Constant */
assert(Wosize_val(v)==1);
v0 = Field(v,0);
texpr->discr = AP_TEXPR_CST;
camlidl_ml2c_coeff_struct_ap_coeff_t(v0,&coeff,_ctx);
ap_coeff_init_set(&texpr->val.cst,&coeff);
break;
case 1: /* Variable */
assert(Wosize_val(v)==1);
v0 = Field(v,0);
camlidl_apron_var_ptr_ml2c(v0,&var);
texpr->discr = AP_TEXPR_DIM;
texpr->val.dim = ap_environment_dim_of_var(env,var);
if (texpr->val.dim == AP_DIM_MAX){
free(texpr);
texpr = NULL;
}
break;
case 2: /* Unary node */
v0 = Field(v,0);
v1 = Field(v,1);
v2 = Field(v,2);
v3 = Field(v,3);

camlidl_texpr1_ap_texpr0_of_expr_ml2c(v1,env,&exprA,_ctx);
if (exprA==NULL){
free(texpr);
texpr = NULL;
}
else {
node = malloc(sizeof(ap_texpr0_node_t));
camlidl_apron_texpr_unop_t_ml2c      (v0,&node->op);
camlidl_apron_texpr_rtype_t_ml2c     (v2,&node->type);
camlidl_apron_texpr_rdir_t_ml2c      (v3,&node->dir);
node->exprA = exprA; 
node->exprB = NULL;
texpr->discr = AP_TEXPR_NODE;
texpr->val.node = node;
}
break;
case 3: /* Binary node */
v0 = Field(v,0);
v1 = Field(v,1);
v2 = Field(v,2);
v3 = Field(v,3);
v4 = Field(v,4);
camlidl_texpr1_ap_texpr0_of_expr_ml2c(v1,env,&exprA,_ctx);
if (exprA==NULL){
free(texpr);
texpr = NULL;
}
else {
camlidl_texpr1_ap_texpr0_of_expr_ml2c(v2,env,&exprB,_ctx);
if (exprB==NULL){
ap_texpr0_free(exprA);
free(texpr);
texpr = NULL;
}
else {
node = malloc(sizeof(ap_texpr0_node_t));
camlidl_apron_texpr_binop_t_ml2c     (v0,&node->op);
camlidl_apron_texpr_rtype_t_ml2c     (v3,&node->type);
camlidl_apron_texpr_rdir_t_ml2c      (v4,&node->dir);
node->exprA = exprA; 
node->exprB = exprB; 
texpr->discr = AP_TEXPR_NODE;
texpr->val.node = node;
}
}
break;
default:
assert(false);
}
*res = texpr;
}
value camlidl_texpr1_ap_texpr0_of_expr(value _v_env, value _v_expr)
{
CAMLparam2(_v_env,_v_expr);
CAMLlocal1(_v_res);
ap_texpr0_t* texpr0;
ap_environment_t* env;

struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
camlidl_ctx _ctx = &_ctxs;

camlidl_apron_environment_ptr_ml2c(_v_env,&env);
camlidl_texpr1_ap_texpr0_of_expr_ml2c(_v_expr,env,&texpr0,_ctx);
camlidl_free(_ctx);
if (texpr0==NULL){
caml_failwith("Texpr1.texpr0_of_expr: unknown variable in the environment");
}
_v_res = camlidl_apron_texpr0_ptr_c2ml(&texpr0);
CAMLreturn(_v_res);
}

value camlidl_texpr1_ap_texpr1_cst(
	value _v_env,
	value _v_coeff)
{
  ap_environment_ptr env; /*in*/
  struct ap_coeff_t *coeff; /*in*/
  struct ap_texpr1_t *_res;
  struct ap_coeff_t _c1;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_environment_ap_environment_ptr(_v_env, &env, _ctx);
  coeff = &_c1;
  camlidl_ml2c_coeff_struct_ap_coeff_t(_v_coeff, &_c1, _ctx);
  _res = ap_texpr1_cst(env, coeff);
  _vres = camlidl_c2ml_texpr1_struct_ap_texpr1_t(&*_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_texpr1_ap_texpr1_var(
	value _v_env,
	value _v_var)
{
  ap_environment_ptr env; /*in*/
  ap_var_t var; /*in*/
  struct ap_texpr1_t *_res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_environment_ap_environment_ptr(_v_env, &env, _ctx);
  camlidl_ml2c_var_ap_var_t(_v_var, &var, _ctx);
  _res = ap_texpr1_var(env, var);
  _vres = camlidl_c2ml_texpr1_struct_ap_texpr1_t(&*_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_texpr1_ap_texpr1_unop(
	value _v_op,
	value _v_expr,
	value _v_type,
	value _v_dir)
{
  ap_texpr_unop_t op; /*in*/
  struct ap_texpr1_t *expr; /*in*/
  ap_texpr_rtype_t type; /*in*/
  ap_texpr_rdir_t dir; /*in*/
  struct ap_texpr1_t *_res;
  struct ap_texpr1_t _c1;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_texpr0_ap_texpr_unop_t(_v_op, &op, _ctx);
  expr = &_c1;
  camlidl_ml2c_texpr1_struct_ap_texpr1_t(_v_expr, &_c1, _ctx);
  camlidl_ml2c_texpr0_ap_texpr_rtype_t(_v_type, &type, _ctx);
  camlidl_ml2c_texpr0_ap_texpr_rdir_t(_v_dir, &dir, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_texpr1_unop(op,ap_texpr1_copy(expr),type,dir);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_texpr1_struct_ap_texpr1_t(&*_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_texpr1_ap_texpr1_binop(
	value _v_op,
	value _v_exprA,
	value _v_exprB,
	value _v_type,
	value _v_dir)
{
  ap_texpr_binop_t op; /*in*/
  struct ap_texpr1_t *exprA; /*in*/
  struct ap_texpr1_t *exprB; /*in*/
  ap_texpr_rtype_t type; /*in*/
  ap_texpr_rdir_t dir; /*in*/
  struct ap_texpr1_t *_res;
  struct ap_texpr1_t _c1;
  struct ap_texpr1_t _c2;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_texpr0_ap_texpr_binop_t(_v_op, &op, _ctx);
  exprA = &_c1;
  camlidl_ml2c_texpr1_struct_ap_texpr1_t(_v_exprA, &_c1, _ctx);
  exprB = &_c2;
  camlidl_ml2c_texpr1_struct_ap_texpr1_t(_v_exprB, &_c2, _ctx);
  camlidl_ml2c_texpr0_ap_texpr_rtype_t(_v_type, &type, _ctx);
  camlidl_ml2c_texpr0_ap_texpr_rdir_t(_v_dir, &dir, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_texpr1_binop(op,ap_texpr1_copy(exprA),ap_texpr1_copy(exprB),type,dir);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_texpr1_struct_ap_texpr1_t(&*_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_texpr1_ap_texpr1_extend_environment(
	value _v_texpr,
	value _v_env)
{
  struct ap_texpr1_t *texpr; /*in*/
  ap_environment_ptr env; /*in*/
  struct ap_texpr1_t *_res;
  struct ap_texpr1_t _c1;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  texpr = &_c1;
  camlidl_ml2c_texpr1_struct_ap_texpr1_t(_v_texpr, &_c1, _ctx);
  camlidl_ml2c_environment_ap_environment_ptr(_v_env, &env, _ctx);
  /* begin user-supplied calling sequence */

{
bool b;
_res = ap_texpr1_extend_environment(texpr,env);
if (!_res) caml_failwith("Texpr1.extend_environment: new environment is not a superenvironment");
}

  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_texpr1_struct_ap_texpr1_t(&*_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_texpr1_ap_texpr1_extend_environment_with(
	value _v_texpr,
	value _v_env)
{
  struct ap_texpr1_t *texpr; /*in*/
  ap_environment_ptr env; /*in*/
  struct ap_texpr1_t _c1;
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  texpr = &_c1;
  camlidl_ml2c_texpr1_struct_ap_texpr1_t(_v_texpr, &_c1, _ctx);
  camlidl_ml2c_environment_ap_environment_ptr(_v_env, &env, _ctx);
  /* begin user-supplied calling sequence */

{
if (texpr->env!=env){ 
bool b;
ap_environment_copy(texpr->env); /* to protect it */ 
b = ap_texpr1_extend_environment_with(texpr,env);
if (b){ 
ap_environment_free(texpr->env); 
caml_failwith("Texpr1.extend_environment_with: new environment is not a superenvironment");
}
Store_field(_v_texpr,1,_v_env);
ap_environment_free(env);
}
}

  /* end user-supplied calling sequence */
  camlidl_free(_ctx);
  return Val_unit;
}

