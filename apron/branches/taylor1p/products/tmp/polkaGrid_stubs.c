/* File generated from polkaGrid.idl */

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

#include "ap_global0.h"
#include "apron_caml.h"
#include "ap_pkgrid.h"
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


extern void camlidl_apron_lincons0_ml2c(value, ap_lincons0_t *);
#define camlidl_ml2c_lincons0_ap_lincons0_t(v,c,ctx) camlidl_apron_lincons0_ml2c(v,c)

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


extern void camlidl_ml2c_texpr0_struct_ap_texpr_op_t(value, struct ap_texpr_op_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_texpr0_struct_ap_texpr_op_t(struct ap_texpr_op_t *, camlidl_ctx _ctx);

extern void camlidl_apron_texpr_unop_t_ml2c(value, ap_texpr_unop_t *);
#define camlidl_ml2c_texpr0_ap_texpr_unop_t(v,c,ctx) camlidl_apron_texpr_unop_t_ml2c(v,c)

extern value camlidl_apron_texpr_unop_t_c2ml(ap_texpr_unop_t *);
#define camlidl_c2ml_texpr0_ap_texpr_unop_t(c,ctx) camlidl_apron_texpr_unop_t_c2ml(c)


extern void camlidl_apron_texpr_binop_t_ml2c(value, ap_texpr_binop_t *);
#define camlidl_ml2c_texpr0_ap_texpr_binop_t(v,c,ctx) camlidl_apron_texpr_binop_t_ml2c(v,c)

extern value camlidl_apron_texpr_binop_t_c2ml(ap_texpr_binop_t *);
#define camlidl_c2ml_texpr0_ap_texpr_binop_t(c,ctx) camlidl_apron_texpr_binop_t_c2ml(c)


extern void camlidl_ml2c_texpr0_struct_ap_texpr_rtype_t(value, struct ap_texpr_rtype_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_texpr0_struct_ap_texpr_rtype_t(struct ap_texpr_rtype_t *, camlidl_ctx _ctx);

extern void camlidl_apron_texpr_rtype_t_ml2c(value, ap_texpr_rtype_t *);
#define camlidl_ml2c_texpr0_ap_texpr_rtype_t(v,c,ctx) camlidl_apron_texpr_rtype_t_ml2c(v,c)

extern value camlidl_apron_texpr_rtype_t_c2ml(ap_texpr_rtype_t *);
#define camlidl_c2ml_texpr0_ap_texpr_rtype_t(c,ctx) camlidl_apron_texpr_rtype_t_c2ml(c)


extern void camlidl_ml2c_texpr0_struct_ap_texpr_rdir_t(value, struct ap_texpr_rdir_t *, camlidl_ctx _ctx);
extern value camlidl_c2ml_texpr0_struct_ap_texpr_rdir_t(struct ap_texpr_rdir_t *, camlidl_ctx _ctx);

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

extern void camlidl_apron_abstract0_ptr_ml2c(value, ap_abstract0_ptr *);
#define camlidl_ml2c_abstract0_ap_abstract0_ptr(v,c,ctx) camlidl_apron_abstract0_ptr_ml2c(v,c)

extern value camlidl_apron_abstract0_ptr_c2ml(ap_abstract0_ptr *);
#define camlidl_c2ml_abstract0_ap_abstract0_ptr(c,ctx) camlidl_apron_abstract0_ptr_c2ml(c)


value camlidl_polkaGrid_ap_pkgrid_manager_alloc(
	value _v_manpk,
	value _v_manpplgrid)
{
  ap_manager_ptr manpk; /*in*/
  ap_manager_ptr manpplgrid; /*in*/
  ap_manager_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_manpk, &manpk, _ctx);
  camlidl_ml2c_manager_ap_manager_ptr(_v_manpplgrid, &manpplgrid, _ctx);
  /* begin user-supplied calling sequence */

_res = ap_pkgrid_manager_alloc(manpk,manpplgrid);
if (_res==NULL) caml_failwith("PolkaGrid.manager_alloc: the provided polka manager is most probably an equalities polka manager");
{ ap_exc_t i;
for (i=1; i<AP_EXC_SIZE; i++){
ap_manager_set_abort_if_exception(_res,i,false);
}}

  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_manager_ap_manager_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  return _vres;
}

value camlidl_polkaGrid_ap_pkgrid_manager_decompose(
	value _v_man)
{
  ap_manager_ptr man; /*in*/
  ap_manager_ptr manpk; /*out*/
  ap_manager_ptr manpplgrid; /*out*/
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  value _vresult;
  value _vres[2] = { 0, 0, };

  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  /* begin user-supplied calling sequence */

ap_reducedproduct_internal_t* intern = (ap_reducedproduct_internal_t*)man->internal;
manpk = ap_manager_copy(intern->tmanagers[0]);
manpplgrid = ap_manager_copy(intern->tmanagers[1]);

  /* end user-supplied calling sequence */
  Begin_roots_block(_vres, 2)
    _vres[0] = camlidl_c2ml_manager_ap_manager_ptr(&manpk, _ctx);
    _vres[1] = camlidl_c2ml_manager_ap_manager_ptr(&manpplgrid, _ctx);
    _vresult = camlidl_alloc_small(2, 0);
    Field(_vresult, 0) = _vres[0];
    Field(_vresult, 1) = _vres[1];
  End_roots()
  camlidl_free(_ctx);
  return _vresult;
}

value camlidl_polkaGrid_ap_pkgrid_decompose(
	value _v_abs)
{
  ap_abstract0_ptr abs; /*in*/
  ap_abstract0_ptr abspk; /*out*/
  ap_abstract0_ptr abspplgrid; /*out*/
  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  value _vresult;
  value _vres[2] = { 0, 0, };

  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_abs, &abs, _ctx);
  /* begin user-supplied calling sequence */

{
ap_reducedproduct_internal_t* intern = (ap_reducedproduct_internal_t*)abs->man->internal;
ap_manager_t* manpk = intern->tmanagers[0];
ap_manager_t* manpplgrid = intern->tmanagers[1];
void** res = ap_reducedproduct_decompose(abs->man,false,(ap_reducedproduct_t*)abs->value);
abspk = (ap_abstract0_t*)malloc(sizeof(ap_abstract0_t));
abspk->man = ap_manager_copy(manpk);
abspk->value = res[0];
abspplgrid = (ap_abstract0_t*)malloc(sizeof(ap_abstract0_t));
abspplgrid->man = ap_manager_copy(manpplgrid);
abspplgrid->value = res[1];
free(res);
}

  /* end user-supplied calling sequence */
  Begin_roots_block(_vres, 2)
    _vres[0] = camlidl_c2ml_abstract0_ap_abstract0_ptr(&abspk, _ctx);
    _vres[1] = camlidl_c2ml_abstract0_ap_abstract0_ptr(&abspplgrid, _ctx);
    _vresult = camlidl_alloc_small(2, 0);
    Field(_vresult, 0) = _vres[0];
    Field(_vresult, 1) = _vres[1];
  End_roots()
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (abs->man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(abs->man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vresult;
}

value camlidl_polkaGrid_ap_pkgrid_compose(
	value _v_man,
	value _v_abspk,
	value _v_abspplgrid)
{
  ap_manager_ptr man; /*in*/
  ap_abstract0_ptr abspk; /*in*/
  ap_abstract0_ptr abspplgrid; /*in*/
  ap_abstract0_ptr _res;
  value _vres;

  struct camlidl_ctx_struct _ctxs = { CAMLIDL_TRANSIENT, NULL };
  camlidl_ctx _ctx = &_ctxs;
  camlidl_ml2c_manager_ap_manager_ptr(_v_man, &man, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_abspk, &abspk, _ctx);
  camlidl_ml2c_abstract0_ap_abstract0_ptr(_v_abspplgrid, &abspplgrid, _ctx);
  /* begin user-supplied calling sequence */

{
ap_reducedproduct_internal_t* intern = (ap_reducedproduct_internal_t*)man->internal;
ap_manager_t* manpk = intern->tmanagers[0];
ap_manager_t* manpplgrid = intern->tmanagers[1];
void* tabs[2];
tabs[0] = abspk->value;
tabs[1] = abspplgrid->value;
ap_reducedproduct_t* prod = ap_reducedproduct_compose(man,false,tabs);
_res = (ap_abstract0_t*)malloc(sizeof(ap_abstract0_t));
_res->man = ap_manager_copy(man);
_res->value = prod;
}

  /* end user-supplied calling sequence */
  _vres = camlidl_c2ml_abstract0_ap_abstract0_ptr(&_res, _ctx);
  camlidl_free(_ctx);
  /* begin user-supplied deallocation sequence */
if (man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(man,_ctx);
  /* end user-supplied deallocation sequence */
  return _vres;
}

