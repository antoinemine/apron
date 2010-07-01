/* File generated from texpr0.idl */

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
#include "ap_texpr0.h"
#include "apron_caml.h"

value camlidl_texpr0_ap_texpr0_to_expr_c2ml(ap_texpr0_t* expr, camlidl_ctx _ctx)
{
value _v_res;

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
_v_res = caml_alloc_small(1,1);
Field(_v_res,0) = Val_int(expr->val.dim);
break;
case AP_TEXPR_NODE:
{
value v0,v1,v2,v3,v4;
v0 = v1 = v2 = v3 = v4 = Val_unit;
Begin_roots5(v0,v1,v2,v3,v4);
ap_texpr0_node_t* node = expr->val.node;
v1 = camlidl_texpr0_ap_texpr0_to_expr_c2ml(node->exprA,_ctx);
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
v2 = camlidl_texpr0_ap_texpr0_to_expr_c2ml(node->exprB,_ctx);
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
value camlidl_texpr0_ap_texpr0_to_expr(value _v_texpr)
{
CAMLparam1(_v_texpr);
CAMLlocal1(_v_res);
ap_texpr0_t* expr;

struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
camlidl_ctx _ctx = &_ctxs;

camlidl_apron_texpr0_ptr_ml2c(_v_texpr,&expr);
_v_res = camlidl_texpr0_ap_texpr0_to_expr_c2ml(expr,_ctx);
camlidl_free(_ctx);

CAMLreturn(_v_res);
}

void camlidl_texpr0_ap_texpr0_of_expr_ml2c(value v, ap_texpr0_t** res, camlidl_ctx _ctx)
{
ap_texpr0_t* texpr;
ap_texpr0_node_t* node;
ap_coeff_t coeff;
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
case 1: /* Dimension */
assert(Wosize_val(v)==1);
v0 = Field(v,0);
texpr->discr = AP_TEXPR_DIM;
texpr->val.dim = Int_val(v0);
break;
case 2:  /* Unary node */
v0 = Field(v,0);
v1 = Field(v,1);
v2 = Field(v,2);
v3 = Field(v,3);

node = malloc(sizeof(ap_texpr0_node_t));
 camlidl_apron_texpr_unop_t_ml2c      (v0,&node->op);
camlidl_texpr0_ap_texpr0_of_expr_ml2c(v1,&node->exprA,_ctx);
 camlidl_apron_texpr_rtype_t_ml2c     (v2,&node->type);
camlidl_apron_texpr_rdir_t_ml2c      (v3,&node->dir);
node->exprB = NULL;
texpr->discr = AP_TEXPR_NODE;
texpr->val.node = node;
break;
case 3: /* Binary node */
v0 = Field(v,0);
v1 = Field(v,1);
v2 = Field(v,2);
v3 = Field(v,3);
v4 = Field(v,4);
node = malloc(sizeof(ap_texpr0_node_t));
camlidl_apron_texpr_binop_t_ml2c     (v0,&node->op);
camlidl_texpr0_ap_texpr0_of_expr_ml2c(v1,&node->exprA,_ctx);
 camlidl_texpr0_ap_texpr0_of_expr_ml2c(v2,&node->exprB,_ctx);
 camlidl_apron_texpr_rtype_t_ml2c     (v3,&node->type);
camlidl_apron_texpr_rdir_t_ml2c      (v4,&node->dir);
texpr->discr = AP_TEXPR_NODE;
texpr->val.node = node;
break;
default:
assert(false);
}
*res = texpr;
}
value camlidl_texpr0_ap_texpr0_of_expr(value _v_expr)
{
CAMLparam1(_v_expr);
CAMLlocal1(_v_res);
ap_texpr0_t* res;

struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
camlidl_ctx _ctx = &_ctxs;

camlidl_texpr0_ap_texpr0_of_expr_ml2c(_v_expr,&res,_ctx);
_v_res = camlidl_apron_texpr0_ptr_c2ml(&res);

camlidl_free(_ctx);
CAMLreturn(_v_res);
}

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


#define camlidl_ml2c_texpr0_ap_texpr0_ptr(v,c,ctx) camlidl_apron_texpr0_ptr_ml2c(v,c)

#define camlidl_c2ml_texpr0_ap_texpr0_ptr(c,ctx) camlidl_apron_texpr0_ptr_c2ml(c)

extern void camlidl_ml2c_texpr0_struct_ap_texpr_op_t(value, ap_texpr_op_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_texpr0_struct_ap_texpr_op_t(ap_texpr_op_t *, camlidl_ctx _ctx);

#define camlidl_ml2c_texpr0_ap_texpr_unop_t(v,c,ctx) camlidl_apron_texpr_unop_t_ml2c(v,c)

#define camlidl_c2ml_texpr0_ap_texpr_unop_t(c,ctx) camlidl_apron_texpr_unop_t_c2ml(c)

#define camlidl_ml2c_texpr0_ap_texpr_binop_t(v,c,ctx) camlidl_apron_texpr_binop_t_ml2c(v,c)

#define camlidl_c2ml_texpr0_ap_texpr_binop_t(c,ctx) camlidl_apron_texpr_binop_t_c2ml(c)

extern void camlidl_ml2c_texpr0_struct_ap_texpr_rtype_t(value, ap_texpr_rtype_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_texpr0_struct_ap_texpr_rtype_t(ap_texpr_rtype_t *, camlidl_ctx _ctx);

#define camlidl_ml2c_texpr0_ap_texpr_rtype_t(v,c,ctx) camlidl_apron_texpr_rtype_t_ml2c(v,c)

#define camlidl_c2ml_texpr0_ap_texpr_rtype_t(c,ctx) camlidl_apron_texpr_rtype_t_c2ml(c)

extern void camlidl_ml2c_texpr0_struct_ap_texpr_rdir_t(value, ap_texpr_rdir_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_texpr0_struct_ap_texpr_rdir_t(ap_texpr_rdir_t *, camlidl_ctx _ctx);

#define camlidl_ml2c_texpr0_ap_texpr_rdir_t(v,c,ctx) camlidl_apron_texpr_rdir_t_ml2c(v,c)

#define camlidl_c2ml_texpr0_ap_texpr_rdir_t(c,ctx) camlidl_apron_texpr_rdir_t_c2ml(c)

value camlidl_texpr0_ap_texpr0_copy(
	value _v_a)
{
  ap_texpr0_ptr a; /*in*/
  ap_texpr0_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_texpr0_ap_texpr0_ptr(_v_a, &a, _ctx);
  _res = ap_texpr0_copy(a);
  _vres = camlidl_c2ml_texpr0_ap_texpr0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_texpr0_ap_texpr0_of_linexpr(
	value _v_e)
{
  ap_linexpr0_ptr e; /*in*/
  ap_texpr0_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_linexpr0_ap_linexpr0_ptr(_v_e, &e, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_texpr0_from_linexpr0(e);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_texpr0_ap_texpr0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_texpr0_ap_texpr0_cst(
	value _v_coeff)
{
  struct ap_coeff_t *coeff; /*in*/
  ap_texpr0_ptr _res;
  struct ap_coeff_t _c1;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  coeff = &_c1;
  camlidl_ml2c_coeff_struct_ap_coeff_t(_v_coeff, &_c1, _ctx);
  _res = ap_texpr0_cst(coeff);
  _vres = camlidl_c2ml_texpr0_ap_texpr0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_texpr0_ap_texpr0_dim(
	value _v_dim)
{
  ap_dim_t dim; /*in*/
  ap_texpr0_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_dim_ap_dim_t(_v_dim, &dim, _ctx);
  _res = ap_texpr0_dim(dim);
  _vres = camlidl_c2ml_texpr0_ap_texpr0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_texpr0_ap_texpr0_unop(
	value _v_op,
	value _v_expr,
	value _v_type,
	value _v_dir)
{
  ap_texpr_unop_t op; /*in*/
  ap_texpr0_ptr expr; /*in*/
  ap_texpr_rtype_t type; /*in*/
  ap_texpr_rdir_t dir; /*in*/
  ap_texpr0_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_texpr0_ap_texpr_unop_t(_v_op, &op, _ctx);
  camlidl_ml2c_texpr0_ap_texpr0_ptr(_v_expr, &expr, _ctx);
  camlidl_ml2c_texpr0_ap_texpr_rtype_t(_v_type, &type, _ctx);
  camlidl_ml2c_texpr0_ap_texpr_rdir_t(_v_dir, &dir, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_texpr0_unop(op,ap_texpr0_copy(expr),type,dir);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_texpr0_ap_texpr0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_texpr0_ap_texpr0_binop(
	value _v_op,
	value _v_type,
	value _v_dir,
	value _v_exprA,
	value _v_exprB)
{
  ap_texpr_binop_t op; /*in*/
  ap_texpr_rtype_t type; /*in*/
  ap_texpr_rdir_t dir; /*in*/
  ap_texpr0_ptr exprA; /*in*/
  ap_texpr0_ptr exprB; /*in*/
  ap_texpr0_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_texpr0_ap_texpr_binop_t(_v_op, &op, _ctx);
  camlidl_ml2c_texpr0_ap_texpr_rtype_t(_v_type, &type, _ctx);
  camlidl_ml2c_texpr0_ap_texpr_rdir_t(_v_dir, &dir, _ctx);
  camlidl_ml2c_texpr0_ap_texpr0_ptr(_v_exprA, &exprA, _ctx);
  camlidl_ml2c_texpr0_ap_texpr0_ptr(_v_exprB, &exprB, _ctx);
  /* begin user-supplied calling sequence */
_res = ap_texpr0_binop(op,ap_texpr0_copy(exprA),ap_texpr0_copy(exprB),type,dir);
  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_texpr0_ap_texpr0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_texpr0_ap_texpr0_is_interval_cst(
	value _v_a)
{
  ap_texpr0_ptr a; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_texpr0_ap_texpr0_ptr(_v_a, &a, _ctx);
  _res = ap_texpr0_is_interval_cst(a);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_texpr0_ap_texpr0_is_interval_linear(
	value _v_a)
{
  ap_texpr0_ptr a; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_texpr0_ap_texpr0_ptr(_v_a, &a, _ctx);
  _res = ap_texpr0_is_interval_linear(a);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_texpr0_ap_texpr0_is_interval_polynomial(
	value _v_a)
{
  ap_texpr0_ptr a; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_texpr0_ap_texpr0_ptr(_v_a, &a, _ctx);
  _res = ap_texpr0_is_interval_polynomial(a);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_texpr0_ap_texpr0_is_interval_polyfrac(
	value _v_a)
{
  ap_texpr0_ptr a; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_texpr0_ap_texpr0_ptr(_v_a, &a, _ctx);
  _res = ap_texpr0_is_interval_polyfrac(a);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_texpr0_ap_texpr0_is_scalar(
	value _v_a)
{
  ap_texpr0_ptr a; /*in*/
  int _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_texpr0_ap_texpr0_ptr(_v_a, &a, _ctx);
  _res = ap_texpr0_is_scalar(a);
  _vres = Val_int(_res);
  camlidl_free(_ctx);
  return _vres;
}

