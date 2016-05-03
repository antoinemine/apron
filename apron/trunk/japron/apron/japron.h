/*
 * japron.h
 *
 * utilities
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include "jgmp.h"
#include <ap_global1.h>

/* Method, field & class cache */
/* --------------------------- */

extern jclass japron_string;
extern jclass japron_mpqscalar;
extern jclass japron_mpfrscalar;
extern jclass japron_doublescalar;
extern jclass japron_interval;
extern jclass japron_coeff;
extern jclass japron_dimchange;
extern jclass japron_dimperm;
extern jclass japron_linterm0;
extern jclass japron_linexpr0;
extern jclass japron_lincons0;
extern jclass japron_generator0;
extern jclass japron_texpr0bin;
extern jclass japron_texpr0un;
extern jclass japron_texpr0cst;
extern jclass japron_texpr0dim;
extern jclass japron_texpr0intern;
extern jclass japron_tcons0;
extern jclass japron_manager;
extern jclass japron_abstract0;
extern jclass japron_environment;
extern jclass japron_dimension;

extern jfieldID japron_mpqscalar_val;
extern jfieldID japron_mpfrscalar_val;
extern jfieldID japron_doublescalar_val;
extern jfieldID japron_interval_inf;
extern jfieldID japron_interval_sup;
extern jfieldID japron_dimchange_ptr;
extern jfieldID japron_dimperm_ptr;
extern jfieldID japron_linterm0_dim;
extern jfieldID japron_linterm0_coeff;
extern jfieldID japron_linexpr0_ptr;
extern jfieldID japron_lincons0_expr;
extern jfieldID japron_lincons0_kind;
extern jfieldID japron_lincons0_scalar;
extern jfieldID japron_generator0_coord;
extern jfieldID japron_generator0_kind;
extern jfieldID japron_texpr0bin_op;
extern jfieldID japron_texpr0bin_rtype;
extern jfieldID japron_texpr0bin_rdir;
extern jfieldID japron_texpr0bin_larg;
extern jfieldID japron_texpr0bin_rarg;
extern jfieldID japron_texpr0un_op;
extern jfieldID japron_texpr0un_rtype;
extern jfieldID japron_texpr0un_rdir;
extern jfieldID japron_texpr0un_arg;
extern jfieldID japron_texpr0cst_cst;
extern jfieldID japron_texpr0dim_dim;
extern jfieldID japron_texpr0intern_ptr;
extern jfieldID japron_tcons0_expr;
extern jfieldID japron_tcons0_kind;
extern jfieldID japron_tcons0_scalar;
extern jfieldID japron_manager_ptr;
extern jfieldID japron_abstract0_ptr;
extern jfieldID japron_environment_ptr;
extern jfieldID japron_dimension_intdim;
extern jfieldID japron_dimension_realdim;

extern jmethodID japron_mpqscalar_init;
extern jmethodID japron_mpfrscalar_init;
extern jmethodID japron_doublescalar_init;
extern jmethodID japron_interval_init;
extern jmethodID japron_scalar_init;
extern jmethodID japron_coeff_init;
extern jmethodID japron_abstract0_init;
extern jmethodID japron_dimchange_init;
extern jmethodID japron_dimperm_init;
extern jmethodID japron_environment_init;
extern jmethodID japron_linexpr0_init;
extern jmethodID japron_manager_init;
extern jmethodID japron_texpr0intern_init;

/* fills cache, once */
void japron_cache(JNIEnv *env);

JNIEnv *get_env(void);
void var_init(void);

/* Field access */
/* ------------ */

#define as_dimchange(obj) \
  ((ap_dimchange_t*)((*env)->GetLongField(env, (obj), japron_dimchange_ptr)))

#define set_dimchange(obj,ptr) \
  ((*env)->SetLongField(env, (obj), japron_dimchange_ptr, (jlong)ptr))


#define as_dimperm(obj) \
  ((ap_dimperm_t*)((*env)->GetLongField(env, (obj), japron_dimperm_ptr)))

#define set_dimperm(obj,ptr) \
  ((*env)->SetLongField(env, (obj), japron_dimperm_ptr, (jlong)ptr))


#define as_linexpr0(obj) \
  ((ap_linexpr0_t*)((*env)->GetLongField(env, (obj), japron_linexpr0_ptr)))

#define set_linexpr0(obj,ptr) \
  ((*env)->SetLongField(env, (obj), japron_linexpr0_ptr, (jlong)ptr))


#define as_texpr0(obj) \
  ((ap_texpr0_t*)((*env)->GetLongField(env, (obj), japron_texpr0intern_ptr)))

#define set_texpr0(obj,ptr) \
  ((*env)->SetLongField(env, (obj), japron_texpr0intern_ptr, (jlong)ptr))


#define as_manager(obj) \
  ((ap_manager_t*)((*env)->GetLongField(env, (obj), japron_manager_ptr)))

#define set_manager(obj,ptr) \
  ((*env)->SetLongField(env, (obj), japron_manager_ptr, (jlong)ptr))


#define as_abstract0(obj) \
  ((ap_abstract0_t*)((*env)->GetLongField(env, (obj), japron_abstract0_ptr)))

#define set_abstract0(obj,ptr) \
  ((*env)->SetLongField(env, (obj), japron_abstract0_ptr, (jlong)ptr))


#define as_environment(obj) \
  ((ap_environment_t*)((*env)->GetLongField(env, (obj), japron_environment_ptr)))

#define set_environment(obj,ptr) \
  ((*env)->SetLongField(env, (obj), japron_environment_ptr, (jlong)ptr))


#define check_exc(cont) \
  if (man->result.exclog) { japron_exc(env, man); cont }


/* Java <-> Apron objects conversion */
/* --------------------------------- */

int     japron_scalar_set(JNIEnv *env, ap_scalar_t* c, jobject o);
jobject japron_scalar_get(JNIEnv *env, ap_scalar_t* c);

int     japron_interval_set(JNIEnv *env, ap_interval_t* c, jobject o);
jobject japron_interval_get(JNIEnv *env, ap_interval_t* c);

ap_interval_t** japron_interval_array_alloc_set(JNIEnv *env, size_t* pnb, jobjectArray o);
jobjectArray    japron_interval_array_get      (JNIEnv *env, ap_interval_t **t, size_t nb);

int     japron_coeff_set(JNIEnv *env, ap_coeff_t* c, jobject o);
jobject japron_coeff_get(JNIEnv *env, ap_coeff_t* c);

void    japron_lincons0_clear   (ap_lincons0_t* t);
int     japron_lincons0_init_set(JNIEnv *env, ap_lincons0_t* t, jobject c);
jobject japron_lincons0_get     (JNIEnv *env, ap_lincons0_t* t);

void         japron_lincons0_array_clear   (ap_lincons0_array_t* t);
int          japron_lincons0_array_init_set(JNIEnv *env, ap_lincons0_array_t* t, jobjectArray o);
jobjectArray japron_lincons0_array_get     (JNIEnv *env, ap_lincons0_array_t* t);

void         japron_generator0_array_clear   (ap_generator0_array_t* t);
int          japron_generator0_array_init_set(JNIEnv *env, ap_generator0_array_t* t, jobjectArray o);
jobjectArray japron_generator0_array_get     (JNIEnv *env, ap_generator0_array_t* t);

void    japron_tcons0_clear   (ap_tcons0_t* t);
int     japron_tcons0_init_set(JNIEnv *env, ap_tcons0_t* t, jobject c);
jobject japron_tcons0_get     (JNIEnv *env, ap_tcons0_t* t);

void         japron_tcons0_array_clear    (ap_tcons0_array_t* t);
int          japron_tcons0_array_init_set(JNIEnv *env, ap_tcons0_array_t* t, jobjectArray o);
jobjectArray japron_tcons0_array_get     (JNIEnv *env, ap_tcons0_array_t* t);

ap_texpr0_t* japron_texpr0_alloc_set(JNIEnv *env, jobject o);
jobject      japron_texpr0_get      (JNIEnv *env, ap_texpr0_t* t);

ap_linexpr0_t** japron_linexpr0_array_alloc_set(JNIEnv *env, jobjectArray ar, size_t* pnb);
ap_texpr0_t**   japron_texpr0_array_alloc_set  (JNIEnv *env, jobjectArray ar, size_t* pnb);

jobject japron_manager_get  (JNIEnv *env, ap_manager_t* m);
jobject japron_dimension_get(JNIEnv *env, int intdim, int realdim);

jobject japron_abstract0_get(JNIEnv *env, ap_manager_t* man, ap_abstract0_t* a);
ap_abstract0_t** japron_abstract0_array_alloc_set(JNIEnv *env, jobjectArray ar, size_t *pnb);
ap_dim_t* japron_dim_array_alloc_set(JNIEnv *env, jintArray ar, size_t* pnb);

void         japron_string_array_free(char** r, size_t nb);
char**       japron_string_array_alloc_set(JNIEnv *env, jintArray ar, size_t* pnb);
jobjectArray japron_string_array_get(JNIEnv *env, char** x, size_t nb);

void japron_object_array_free(void** r, size_t nb);
void** japron_object_array_alloc_set(JNIEnv *env, jobjectArray ar, size_t *pnb);
jobjectArray japron_object_array_get(JNIEnv *env, void **x, size_t nb);
jobjectArray japron_var_array_get(JNIEnv *env, void **x, size_t nb);


/* Apron utilities */
/* --------------- */

ap_dim_t japron_linexpr0_max_dim(ap_linexpr0_t* e);

void japron_manager_setup(ap_manager_t* m);

void japron_exc(JNIEnv *env, ap_manager_t* m);

