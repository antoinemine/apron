/* ********************************************************************** */
/* apron_caml.c */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution  */

#include "apron_caml.h"

/* ********************************************************************** */
/* dimchange */
/* ********************************************************************** */

void camlidl_apron_dimchange_ml2c(const value v, ap_dimchange_t* dimchange)
{
  value vintdim,vrealdim,vdim,v2;
  size_t i,size;

  vintdim = Field(v,1);
  vrealdim = Field(v,2);
  ap_dimchange_init(dimchange,Int_val(vintdim),Int_val(vrealdim));
  vdim = Field(v,0);
  size = Wosize_val(vdim);
  if (dimchange->intdim+dimchange->realdim>size)
    caml_failwith("Invalid object of type Linexpr0.dimchange: size of the array smaller than intdim+realdim");
  for (i=0;i<size;i++){
    v2 = Field(vdim,i);
    dimchange->dim[i] = Int_val(v2);
  }
  return;
}

value camlidl_apron_dimchange_c2ml(ap_dimchange_t* dimchange)
{
  value vintdim,vrealdim,vdim,v;
  size_t i,size;

  size = dimchange->intdim+dimchange->realdim;
  vintdim = Val_int(dimchange->intdim);
  vrealdim = Val_int(dimchange->realdim);
  vdim = caml_alloc(size,0);
  Begin_root(vdim)
    for (i=0;i<size;i++){
      v = Val_int(dimchange->dim[i]);
      Store_field(vdim,i,v);
    }
  End_roots()
  v = caml_alloc_small(3,0);
  Field(v,1) = vintdim;
  Field(v,2) = vrealdim;
  Field(v,0) = vdim;
  return v;
}

/* ********************************************************************** */
/* linexpr0 */
/* ********************************************************************** */

static
void camlidl_apron_linexpr0_ptr_finalize(value v)
{
  ap_linexpr0_t* p = *(ap_linexpr0_ptr *) Data_custom_val(v);
  ap_linexpr0_free(p);
}

static
long camlidl_apron_linexpr0_ptr_hash(value v)
{
  ap_linexpr0_t* p = *(ap_linexpr0_ptr *) Data_custom_val(v);
  return ap_linexpr0_hash(p);
}

/* We no longer implement the polymorphic comparison function as
   we do not have a natural total order, which is assumed by
   its semantic. The previous implementation led to inconsistent
   behavior and was confusing.
 */

/*
static
int camlidl_apron_linexpr0_ptr_compare(value v1, value v2)
{
  ap_linexpr0_t* p1 = *(ap_linexpr0_ptr *) Data_custom_val(v1);
  ap_linexpr0_t* p2 = *(ap_linexpr0_ptr *) Data_custom_val(v2);
  return ap_linexpr0_compare(p1,p2);
}
*/

struct custom_operations camlidl_apron_custom_linexpr0_ptr = {
  "apl0",
  camlidl_apron_linexpr0_ptr_finalize,
  custom_compare_default /*camlidl_apron_linexpr0_ptr_compare*/,
  camlidl_apron_linexpr0_ptr_hash,
  custom_serialize_default,
  custom_deserialize_default,
#ifdef custom_compare_ext_default
  custom_compare_ext_default,
#endif
#ifdef custom_fixed_length_default
  custom_fixed_length_default,
#endif
};

/* ********************************************************************** */
/* lincons0 */
/* ********************************************************************** */

void camlidl_apron_lincons0_ml2c(value v, ap_lincons0_t* cons, struct camlidl_ctx_struct* _ctx)
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
  _v[0] = _v[1] = _v[2] = Val_unit;

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
      _v[1] = caml_alloc_small(1, 0);
      Field(_v[1],0) = _v[2];
      ap_scalar_free(cons->scalar); cons->scalar = NULL;
      break;
    }
    vres = caml_alloc_small(2, 0);
    Field(vres, 0) = _v[0];
    Field(vres, 1) = _v[1];
  End_roots()
  return vres;
}

/* ********************************************************************** */
/* texpr0 */
/* ********************************************************************** */

static
void camlidl_apron_texpr0_ptr_finalize(value v)
{
  ap_texpr0_t* p = *(ap_texpr0_ptr *) Data_custom_val(v);
  ap_texpr0_free(p);
}

static
long camlidl_apron_texpr0_ptr_hash(value v)
{
  ap_texpr0_t* p = *(ap_texpr0_ptr *) Data_custom_val(v);
  return ap_texpr0_hash(p);
}

/*
static
int camlidl_apron_texpr0_ptr_compare(value v1, value v2)
{
  ap_texpr0_t* p1 = *(ap_texpr0_ptr *) Data_custom_val(v1);
  ap_texpr0_t* p2 = *(ap_texpr0_ptr *) Data_custom_val(v2);
  return ap_texpr0_equal(p1,p2) ? 0 : (p1<p2 ? (-1) : 1);
}
*/

struct custom_operations camlidl_apron_custom_texpr0_ptr = {
  "apl0",
  camlidl_apron_texpr0_ptr_finalize,
  custom_compare_default /*camlidl_apron_texpr0_ptr_compare*/,
  camlidl_apron_texpr0_ptr_hash,
  custom_serialize_default,
  custom_deserialize_default,
#ifdef custom_compare_ext_default
  custom_compare_ext_default,
#endif
#ifdef custom_fixed_length_default
  custom_fixed_length_default,
#endif
};

/* ********************************************************************** */
/* manager */
/* ********************************************************************** */

static
void camlidl_apron_manager_ptr_finalize(value v)
{
  ap_manager_ptr p = *(ap_manager_ptr *) Data_custom_val(v);
  ap_manager_free(p);
}

/*
static
int camlidl_apron_manager_ptr_compare(value v1, value v2)
{
  CAMLparam2(v1,v2);
  int res;
  ap_manager_ptr p1 = *(ap_manager_ptr *) Data_custom_val(v1);
  ap_manager_ptr p2 = *(ap_manager_ptr *) Data_custom_val(v2);
  res = (p1==p2 || p1->library==p2->library) ? 0 : ((p1<p2) ? (-1) : 1);
  CAMLreturn(res);
}
*/

struct custom_operations camlidl_apron_custom_manager_ptr = {
  "apman",
  camlidl_apron_manager_ptr_finalize,
  custom_compare_default /*camlidl_apron_manager_ptr_compare*/,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default,
#ifdef custom_compare_ext_default
  custom_compare_ext_default,
#endif
#ifdef custom_fixed_length_default
  custom_fixed_length_default,
#endif
};

void camlidl_apron_manager_check_exception(struct ap_manager_t* man, void* _ctx)
{
  const value* p = caml_named_value("apron exception");
  if(p==NULL){
    fprintf(stderr,"mlapronidl: error: line <let _ = Callback.register ...> of manager.ml has not been executed: this is not normal\n");
    abort();
  }
  value v = camlidl_c2ml_manager_struct_ap_exclog_t(man->result.exclog,NULL);
  caml_raise_with_arg(*p,v);
}

/* ********************************************************************** */
/* abstract0 */
/* ********************************************************************** */

mlsize_t camlidl_apron_heap = 1 << 20;

static
void camlidl_apron_abstract0_ptr_finalize(value v)
{
  ap_abstract0_ptr* p = (ap_abstract0_ptr *) Data_custom_val(v);
  ap_abstract0_t* a = *p;
  ap_abstract0_free(a->man,a);
}
static
long camlidl_apron_abstract0_ptr_hash(value v)
{
  ap_abstract0_ptr* p = (ap_abstract0_ptr *) Data_custom_val(v);
  ap_abstract0_t* a = *p;
  long res = ap_abstract0_hash(a->man,a);
  if (a->man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(a->man,NULL);
  return res;
}

/*
static
int camlidl_apron_abstract0_ptr_compare(value v1, value v2)
{
  ap_abstract0_ptr* p1 = (ap_abstract0_ptr *) Data_custom_val(v1);
  ap_abstract0_ptr* p2 = (ap_abstract0_ptr *) Data_custom_val(v2);
  ap_abstract0_t* a1 = *p1;
  ap_abstract0_t* a2 = *p2;
  ap_dimension_t dim1,dim2;
  int res;
  if (v1==v2 || p1==p2 || a1==a2)
    res=0;
  else {
    dim1 = ap_abstract0_dimension(a1->man,a1);
    dim2 = ap_abstract0_dimension(a2->man,a2);
    res = dim1.intdim-dim2.intdim;
    if (!res){
      res = dim1.realdim-dim2.realdim;
      if (!res){
	if (ap_abstract0_is_eq(a1->man,a1,a2))
	  res=0;
	else
	  res = a1 > a2 ? 1 : (-1);
	if (a1->man->result.exn!=AP_EXC_NONE) camlidl_apron_manager_check_exception(a1->man,NULL);
      }
    }
  }
  return res;
}
*/

/* global manager used for deserialization */
static ap_manager_ptr deserialize_man = NULL;

void ap_manager_set_deserialize(ap_manager_ptr man)
{
  deserialize_man = man;
}

ap_manager_ptr ap_manager_get_deserialize(void)
{
  return deserialize_man;
}

static
void camlidl_apron_abstract0_serialize(value v, unsigned long * w32, unsigned long * w64)
{
  ap_abstract0_ptr* p = (ap_abstract0_ptr *) Data_custom_val(v);
  ap_abstract0_t* a = *p;
  ap_membuf_t buf = ap_abstract0_serialize_raw(a->man,a);
  caml_serialize_int_8(buf.size);
  caml_serialize_block_1(buf.ptr,buf.size);
  *w32 = 4;
  *w64 = 8;
}

static
unsigned long camlidl_apron_abstract0_deserialize(void * dst)
{
  if (deserialize_man) {
    size_t size = caml_deserialize_uint_8(), realsize;
    void* data;
    data = malloc(size);
    assert(data);
    caml_deserialize_block_1(data,size);
    *((ap_abstract0_ptr*)dst) =
      ap_abstract0_deserialize_raw(deserialize_man,data,&realsize);
    free(data);
  }
  else caml_failwith("you must call Apron.Manager.set_deserialize before deserializing abstract elements.");
  return sizeof(ap_abstract0_ptr);
}

struct custom_operations camlidl_apron_custom_abstract0_ptr = {
  "apa0",
  camlidl_apron_abstract0_ptr_finalize,
  custom_compare_default /*camlidl_apron_abstract0_ptr_compare*/,
  camlidl_apron_abstract0_ptr_hash,
  camlidl_apron_abstract0_serialize,
  camlidl_apron_abstract0_deserialize,
#ifdef custom_compare_ext_default
  custom_compare_ext_default,
#endif
#ifdef custom_fixed_length_default
  custom_fixed_length_default,
#endif
};

/* ********************************************************************** */
/* variable */
/* ********************************************************************** */
static
struct ap_var_operations_t
camlidl_apron_var_ptr_operations = {
  ap_var_compare,
  ap_var_hash,
  ap_var_copy,
  ap_var_free,
  ap_var_to_string
};
value camlidl_apron_set_var_operations(value v)
{
  CAMLparam1(v);
  ap_var_operations = &camlidl_apron_var_ptr_operations;
  CAMLreturn(Val_unit);
}

static
void camlidl_apron_var_ptr_finalize(value v)
{
  apron_var_ptr e = *(apron_var_ptr *) Data_custom_val(v);
  ap_var_free(e);
  return;
}

static
long camlidl_apron_var_ptr_hash(value v)
{
  CAMLparam1(v);
  apron_var_ptr e = *(apron_var_ptr *) Data_custom_val(v);
  int res = ap_var_hash(e);
  CAMLreturn(res);
}
static
int camlidl_apron_var_ptr_compare(value v1, value v2)
{
  CAMLparam2(v1,v2);
  int res;
  apron_var_ptr e1 = *(apron_var_ptr *) Data_custom_val(v1);
  apron_var_ptr e2 = *(apron_var_ptr *) Data_custom_val(v2);
  res = ap_var_compare(e1,e2);
  CAMLreturn(res);
}

struct custom_operations camlidl_apron_custom_var_ptr = {
  "ape",
  camlidl_apron_var_ptr_finalize,
  camlidl_apron_var_ptr_compare,
  camlidl_apron_var_ptr_hash,
  custom_serialize_default,
  custom_deserialize_default,
#ifdef custom_compare_ext_default
  custom_compare_ext_default,
#endif
#ifdef custom_fixed_length_default
  custom_fixed_length_default,
#endif
};

/* ********************************************************************** */
/* environment */
/* ********************************************************************** */

static
void camlidl_apron_environment_ptr_finalize(value v)
{
  ap_environment_t* e = *(ap_environment_ptr *) Data_custom_val(v);
  ap_environment_free(e);
  return;
}

static
long camlidl_apron_environment_ptr_hash(value v)
{
  CAMLparam1(v);
  ap_environment_t* e = *(ap_environment_ptr *) Data_custom_val(v);
  int res = ap_environment_hash(e);
  CAMLreturn(res);
}

/*
static
int camlidl_apron_environment_ptr_compare(value v1, value v2)
{
  CAMLparam2(v1,v2);
  int res;
  ap_environment_t* env1 = *(ap_environment_ptr *) Data_custom_val(v1);
  ap_environment_t* env2 = *(ap_environment_ptr *) Data_custom_val(v2);
  res = ap_environment_compare(env1,env2);
  CAMLreturn(res);
}
*/

struct custom_operations camlidl_apron_custom_environment_ptr = {
  "ape",
  camlidl_apron_environment_ptr_finalize,
  custom_compare_default /*camlidl_apron_environment_ptr_compare*/,
  camlidl_apron_environment_ptr_hash,
  custom_serialize_default,
  custom_deserialize_default,
#ifdef custom_compare_ext_default
  custom_compare_ext_default,
#endif
#ifdef custom_fixed_length_default
  custom_fixed_length_default,
#endif
};

value camlidl_apron_environment_ptr_c2ml(ap_environment_ptr* p)
{
  value v;

   v = caml_alloc_custom(&camlidl_apron_custom_environment_ptr, sizeof(ap_environment_ptr), 0,1);
  *((ap_environment_ptr *) Data_custom_val(v)) = *p;
  return v;
}

/* ********************************************************************** */
/* init */
/* ********************************************************************** */
value camlidl_apron_init(value dummy)
{
  caml_register_custom_operations(&camlidl_apron_custom_abstract0_ptr);
  return Val_unit;
}
/* ********************************************************************** */
/* ********************************************************************** */
/* Policy experimental extension */
/* ********************************************************************** */
/* ********************************************************************** */

value camlidl_apron_policy_optr_c2ml(ap_policy_optr* p)
{
  if (p==NULL){
    return Val_int(0);
  } else {
    value v,v2 = Val_unit;
    Begin_roots1(v2);
    v2 = camlidl_apron_policy_ptr_c2ml(p);
    v = caml_alloc_small(1,0);
    Field(v,0) = v2;
    End_roots();
    return v;
  }
}

static
void camlidl_apron_policy_manager_ptr_finalize(value v)
{
  ap_policy_manager_ptr p = *(ap_policy_manager_ptr *) Data_custom_val(v);
  ap_policy_manager_free(p);
}

struct custom_operations camlidl_apron_custom_policy_manager_ptr = {
  "appolicyman",
  camlidl_apron_policy_manager_ptr_finalize,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default,
#ifdef custom_compare_ext_default
  custom_compare_ext_default,
#endif
#ifdef custom_fixed_length_default
  custom_fixed_length_default,
#endif
};


static
void camlidl_apron_policy_ptr_finalize(value v)
{
  ap_policy_ptr* p = (ap_policy_ptr *) Data_custom_val(v);
  ap_policy_t* a = *p;
  ap_policy_free(a->pman,a);
}

/*
static
int camlidl_apron_policy_ptr_compare(value v1, value v2)
{
  ap_policy_ptr* p1 = (ap_policy_ptr *) Data_custom_val(v1);
  ap_policy_ptr* p2 = (ap_policy_ptr *) Data_custom_val(v2);
  ap_policy_t* a1 = *p1;
  ap_policy_t* a2 = *p2;
  int res;

  res = ap_policy_equal(a1->pman,a1,a2);
  if (res!=0)
    res = (int)(a1-a2);
  return res;
}
*/

static
long camlidl_apron_policy_ptr_hash(value v)
{
  ap_policy_ptr* p = (ap_policy_ptr *) Data_custom_val(v);
  ap_policy_t* a = *p;
  long res;

  res = ap_policy_hash(a->pman,a);
  return res;
}

struct custom_operations camlidl_apron_custom_policy_ptr = {
  "appolicy",
  camlidl_apron_policy_ptr_finalize,
  custom_compare_default /*camlidl_apron_policy_ptr_compare*/,
  camlidl_apron_policy_ptr_hash,
  custom_serialize_default,
  custom_deserialize_default,
#ifdef custom_compare_ext_default
  custom_compare_ext_default,
#endif
#ifdef custom_fixed_length_default
  custom_fixed_length_default,
#endif
};
