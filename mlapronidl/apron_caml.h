/* ********************************************************************** */
/* apron_caml.h */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution  */

#ifndef _APRON_CAML_H_
#define _APRON_CAML_H_

#include <string.h>
#include "gmp_caml.h"

#include "ap_manager.h"
#include "ap_coeff.h"
#include "ap_expr0.h"
#include "ap_environment.h"
#include "ap_abstract0.h"
#include "ap_expr1.h"
#include "ap_abstract1.h"

#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/fail.h>
#include <caml/callback.h>
#include <caml/custom.h>
#include <caml/bigarray.h>
#include <caml/intext.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ap_interval_array_t {
  struct ap_interval_t** p;
  size_t size;
};

struct ap_scalar_array_t {
  struct ap_scalar_t** p;
  size_t size;
};

typedef struct ap_interval_t* ap_interval_ptr;
typedef struct ap_scalar_t* ap_scalar_ptr;
typedef struct ap_linexpr0_t* ap_linexpr0_ptr;
typedef struct ap_texpr0_t* ap_texpr0_ptr;
typedef struct ap_manager_t* ap_manager_ptr;
typedef struct ap_manager_t* ap_manager_opt_ptr;

typedef union ap_lincons0_typ {
  ap_scalar_t* scalar;
} ap_lincons0_typ;
struct ap_lincons0_ptr {
  ap_linexpr0_t* linexpr0;
  ap_constyp_t constyp;
  union ap_lincons0_typ typ;
};

typedef struct apron_var_t {
  char* name;
  size_t count;
} apron_var_t;
typedef struct apron_var_t* apron_var_ptr;

typedef struct ap_environment_t* ap_environment_ptr;

typedef ap_abstract0_t* ap_abstract0_ptr;

typedef struct ap_abstract1_ptr {
  ap_abstract0_ptr ap_abstract0_ptr;
  ap_environment_t* env;
} ap_abstract1_ptr;

/* ********************************************************************** */
/* dimchange */
/* ********************************************************************** */

void camlidl_apron_dimchange_ml2c(value v, ap_dimchange_t* dimchange);
value camlidl_apron_dimchange_c2ml(ap_dimchange_t* dimchange);

/* ********************************************************************** */
/* scalar */
/* ********************************************************************** */

void camlidl_apron_scalar_ml2c(value v, struct ap_scalar_t* scalar);
value camlidl_apron_scalar_c2ml(struct ap_scalar_t* scalar);

/* ********************************************************************** */
/* coefficients */
/* ********************************************************************** */

struct camlidl_ctx_struct;
extern void * camlidl_malloc(size_t sz, struct camlidl_ctx_struct* ctx);

void camlidl_ml2c_coeff_struct_ap_coeff_t(value _v1, struct ap_coeff_t * _c2, struct camlidl_ctx_struct* _ctx);
value camlidl_c2ml_coeff_struct_ap_coeff_t(struct ap_coeff_t * _c1, struct camlidl_ctx_struct* _ctx);

/* ********************************************************************** */
/* linexpr0 */
/* ********************************************************************** */

extern struct custom_operations camlidl_apron_custom_linexpr0_ptr;

static inline
void camlidl_apron_linexpr0_ptr_ml2c(value v, ap_linexpr0_ptr* p)
{
  *p = *((ap_linexpr0_ptr *) Data_custom_val(v));
}

static inline
value camlidl_apron_linexpr0_ptr_c2ml(ap_linexpr0_ptr* p)
{
  value v;
  v = caml_alloc_custom(&camlidl_apron_custom_linexpr0_ptr, sizeof(ap_linexpr0_ptr), 0,1);
  *((ap_linexpr0_ptr *) Data_custom_val(v)) = *p;
  return v;
}
/* ********************************************************************** */
/* lincons0 */
/* ********************************************************************** */

void camlidl_apron_lincons0_ml2c(value v, ap_lincons0_t* cons, struct camlidl_ctx_struct* _ctx);
value camlidl_apron_lincons0_c2ml(ap_lincons0_t* cons);

/* ********************************************************************** */
/* texpr0 */
/* ********************************************************************** */

typedef ap_texpr_op_t ap_texpr_unop_t;
typedef ap_texpr_op_t ap_texpr_binop_t;

extern struct custom_operations camlidl_apron_custom_texpr0_ptr;

static inline
void camlidl_apron_texpr0_ptr_ml2c(value v, ap_texpr0_ptr* p)
{
  *p = *((ap_texpr0_ptr *) Data_custom_val(v));
}

static inline
value camlidl_apron_texpr0_ptr_c2ml(ap_texpr0_ptr* p)
{
  value v;
  v = caml_alloc_custom(&camlidl_apron_custom_texpr0_ptr, sizeof(ap_texpr0_ptr), 0,1);
  *((ap_texpr0_ptr *) Data_custom_val(v)) = *p;
  return v;
}

static inline
void camlidl_apron_texpr_unop_t_ml2c(value v, ap_texpr_op_t* op)
{
  *op = (ap_texpr_op_t)(AP_TEXPR_NEG + Int_val(v)); assert(*op>=AP_TEXPR_NEG && *op<=AP_TEXPR_SQRT);
}
static inline
value camlidl_apron_texpr_unop_t_c2ml(ap_texpr_op_t* op)
{
  assert(*op>=AP_TEXPR_NEG && *op<=AP_TEXPR_SQRT); return Val_int((*op)-AP_TEXPR_NEG);
}
static inline
void camlidl_apron_texpr_binop_t_ml2c(value v, ap_texpr_op_t* op)
{
  *op = (ap_texpr_op_t)(Int_val(v)); assert(*op<=AP_TEXPR_POW); }
static inline
value camlidl_apron_texpr_binop_t_c2ml(ap_texpr_op_t* op)
{
  assert(*op<=AP_TEXPR_POW); return Val_int((*op));
}
static inline
void camlidl_apron_texpr_rtype_t_ml2c(value v, ap_texpr_rtype_t* op)
{ *op = (ap_texpr_rtype_t)(Int_val(v)); assert(*op<=AP_RTYPE_QUAD); }
static inline
value camlidl_apron_texpr_rtype_t_c2ml(ap_texpr_rtype_t* op)
{ assert(*op<=AP_RTYPE_QUAD); return Val_int(*op); }
static inline
void camlidl_apron_texpr_rdir_t_ml2c(value v, ap_texpr_rdir_t* op)
{ *op = (ap_texpr_rdir_t)(Int_val(v)); assert(*op<=AP_RDIR_RND); }
static inline
value camlidl_apron_texpr_rdir_t_c2ml(ap_texpr_rdir_t* op)
{ assert(*op<=AP_RDIR_RND); return Val_int(*op); }

/* ********************************************************************** */
/* manager */
/* ********************************************************************** */

extern struct custom_operations camlidl_apron_custom_manager_ptr;

static inline
void camlidl_apron_manager_ptr_ml2c(value v, ap_manager_ptr* p)
{
  *p = *((ap_manager_ptr *) Data_custom_val(v));
}

static inline
value camlidl_apron_manager_ptr_c2ml(ap_manager_ptr* p)
{
  value v;
  v = caml_alloc_custom(&camlidl_apron_custom_manager_ptr, sizeof(ap_manager_ptr),
		   0,1);
  *((ap_manager_ptr *) Data_custom_val(v)) = *p;
  return v;
}

void ap_manager_set_deserialize(ap_manager_ptr man);
ap_manager_ptr ap_manager_get_deserialize(void);


extern value camlidl_c2ml_manager_struct_ap_exclog_t(struct ap_exclog_t*, struct camlidl_ctx_struct*);
extern void camlidl_apron_manager_check_exception(struct ap_manager_t* man, void* _ctx);

/* ********************************************************************** */
/* abstract0 */
/* ********************************************************************** */

extern struct custom_operations camlidl_apron_custom_abstract0_ptr;
extern mlsize_t camlidl_apron_heap;

static inline
void camlidl_apron_abstract0_ptr_ml2c(value v, ap_abstract0_ptr* p)
{
  *p = *((ap_abstract0_ptr *) Data_custom_val(v));
}

static inline
value camlidl_apron_abstract0_ptr_c2ml(ap_abstract0_ptr* p)
{
  value v;
  assert((*p)->man!=NULL);
  v = caml_alloc_custom(&camlidl_apron_custom_abstract0_ptr, sizeof(ap_abstract0_ptr),
		   ap_abstract0_size((*p)->man,(*p)),
		   camlidl_apron_heap);
  *((ap_abstract0_ptr *) Data_custom_val(v)) = *p;
  return v;
}

/* ********************************************************************** */
/* Variable */
/* ********************************************************************** */

static inline
apron_var_ptr ap_var_of_string(const char* name)
{
  apron_var_ptr p = (apron_var_ptr)malloc(sizeof(apron_var_t));
  p->name = (char*)malloc((1+strlen(name)));
  strcpy(p->name,name);
  p->count=1;
  return p;
}

static inline
int ap_var_compare(ap_var_t pp1, ap_var_t pp2)
{
  apron_var_ptr p1 = (apron_var_ptr)pp1;
  apron_var_ptr p2 = (apron_var_ptr)pp2;
  return (p1==p2) ? 0 : strcmp(p1->name,p2->name);
}
static inline
int ap_var_hash(ap_var_t pp)
{
  apron_var_ptr p = (apron_var_ptr)pp;
  unsigned char* c;
  int res = 0;
  for (c=(unsigned char*)p->name; (*c)!=0; c++){
    res = res * 11 + (*c);
  }
  return res;
}
static inline
ap_var_t ap_var_copy(ap_var_t pp){
  apron_var_ptr p = (apron_var_ptr)pp;
  p->count++;
  return (ap_var_t)p;
}
static inline
void ap_var_free(ap_var_t pp){
  apron_var_ptr p = (apron_var_ptr)pp;
  if (p->count<=1){
    free(p->name);
    free(p);
  }
  else
    p->count--;
}
static inline
char* ap_var_to_string(ap_var_t pp)
{
  apron_var_ptr p = (apron_var_ptr)pp;
  size_t l;
  char* res;

  l = strlen(p->name)+1;
  res = (char*)malloc(l*sizeof(char));
  strcpy(res,p->name);
  return res;
}
static inline
void camlidl_apron_var_ptr_ml2c(value v, ap_var_t* p){
  *p = *((ap_var_t *) Data_custom_val(v));
}
extern struct custom_operations camlidl_apron_custom_var_ptr;
static inline
value camlidl_apron_var_ptr_c2ml(ap_var_t* p)
{
  value v;

  v = caml_alloc_custom(&camlidl_apron_custom_var_ptr, sizeof(apron_var_ptr), 0,1);
  *((ap_var_t *) Data_custom_val(v)) = *p;
  return v;
}


/* ********************************************************************** */
/* environment */
/* ********************************************************************** */

extern struct custom_operations camlidl_apron_custom_environment_ptr;
static inline
void camlidl_apron_environment_ptr_ml2c(value v, ap_environment_ptr* p)
{
  *p = *((ap_environment_ptr *) Data_custom_val(v));
}

value camlidl_apron_environment_ptr_c2ml(ap_environment_ptr* p);

/* ********************************************************************** */
/* init */
/* ********************************************************************** */
value camlidl_apron_init(value dummy);

/* ********************************************************************** */
/* ********************************************************************** */
/* Policy experimental extension */
/* ********************************************************************** */
/* ********************************************************************** */
#include "ap_policy.h"

typedef struct ap_policy_manager_t* ap_policy_manager_ptr;
typedef struct ap_policy_t* ap_policy_ptr;
typedef struct ap_policy_t* ap_policy_optr;

extern struct custom_operations camlidl_apron_custom_policy_manager_ptr;
extern struct custom_operations camlidl_apron_custom_policy_ptr;

static inline
void camlidl_apron_policy_manager_ptr_ml2c(value v, ap_policy_manager_ptr* p)
{
  *p = *((ap_policy_manager_ptr *) Data_custom_val(v));
}

static inline
value camlidl_apron_policy_manager_ptr_c2ml(ap_policy_manager_ptr* p)
{
  value v;
  v = caml_alloc_custom(&camlidl_apron_custom_policy_manager_ptr, sizeof(ap_policy_manager_ptr),
		   0,1);
  *((ap_policy_manager_ptr *) Data_custom_val(v)) = *p;
  return v;
}
void ap_policy_manager_set_deserialize(ap_policy_manager_ptr man);
ap_policy_manager_ptr ap_policy_manager_get_deserialize(void);

static inline
void camlidl_apron_policy_ptr_ml2c(value v, ap_policy_ptr* p)
{
  *p = *((ap_policy_ptr *) Data_custom_val(v));
}
static inline
void camlidl_apron_policy_optr_ml2c(value v, ap_policy_optr* p)
{
  if (Is_long(v)){
    *p = NULL;
  } else {
    value v2 = Field(v,0);
    camlidl_apron_policy_ptr_ml2c(v2,p);
  }
}

static inline
value camlidl_apron_policy_ptr_c2ml(ap_policy_ptr* p)
{
  value v;
  assert((*p)->pman!=NULL);
  v = caml_alloc_custom(&camlidl_apron_custom_policy_ptr, sizeof(ap_policy_ptr),
		   0,1);
  *((ap_policy_ptr *) Data_custom_val(v)) = *p;
  return v;
}
value camlidl_apron_policy_optr_c2ml(ap_policy_optr* p);

#ifdef __cplusplus
}
#endif

#endif
