/*
 * japron.c
 *
 * utilities
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include "japron.h"


/* Method, field & class cache */
/* =========================== */

jclass japron_string;
jclass japron_object;
jclass japron_var;
jclass japron_mpqscalar;
jclass japron_mpfrscalar;
jclass japron_doublescalar;
jclass japron_interval;
jclass japron_coeff;
jclass japron_dimchange;
jclass japron_dimperm;
jclass japron_linterm0;
jclass japron_linexpr0;
jclass japron_lincons0;
jclass japron_generator0;
jclass japron_texpr0bin;
jclass japron_texpr0un;
jclass japron_texpr0cst;
jclass japron_texpr0dim;
jclass japron_texpr0intern;
jclass japron_tcons0;
jclass japron_manager;
jclass japron_abstract0;
jclass japron_environment;
jclass japron_dimension;

jfieldID japron_mpqscalar_val;
jfieldID japron_mpfrscalar_val;
jfieldID japron_doublescalar_val;
jfieldID japron_interval_inf;
jfieldID japron_interval_sup;
jfieldID japron_dimchange_ptr;
jfieldID japron_dimperm_ptr;
jfieldID japron_linterm0_dim;
jfieldID japron_linterm0_coeff;
jfieldID japron_linexpr0_ptr;
jfieldID japron_lincons0_expr;
jfieldID japron_lincons0_kind;
jfieldID japron_lincons0_scalar;
jfieldID japron_generator0_coord;
jfieldID japron_generator0_kind;
jfieldID japron_texpr0bin_op;
jfieldID japron_texpr0bin_rtype;
jfieldID japron_texpr0bin_rdir;
jfieldID japron_texpr0bin_larg;
jfieldID japron_texpr0bin_rarg;
jfieldID japron_texpr0un_op;
jfieldID japron_texpr0un_rtype;
jfieldID japron_texpr0un_rdir;
jfieldID japron_texpr0un_arg;
jfieldID japron_texpr0cst_cst;
jfieldID japron_texpr0dim_dim;
jfieldID japron_texpr0intern_ptr;
jfieldID japron_tcons0_expr;
jfieldID japron_tcons0_kind;
jfieldID japron_tcons0_scalar;
jfieldID japron_manager_ptr;
jfieldID japron_abstract0_ptr;
jfieldID japron_environment_ptr;
jfieldID japron_dimension_intdim;
jfieldID japron_dimension_realdim;
 
jmethodID japron_abstract0_init;
jmethodID japron_dimchange_init;
jmethodID japron_dimperm_init;
jmethodID japron_environment_init;
jmethodID japron_linexpr0_init;
jmethodID japron_manager_init;
jmethodID japron_texpr0intern_init;

JavaVM          *g_vm;
__thread JNIEnv *t_env;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *r) 
{
  assert(jvm != NULL);

  g_vm = jvm;
  var_init();

  return JNI_VERSION_1_6;
}

//check if t_env is null, if null, call through g_vm to get
//JNIEnv, cache in t_env, return to caller. if not null, return to caller
JNIEnv *get_env(void) 
{
  if(t_env) return t_env;

  assert(g_vm != NULL);

  JNIEnv  *env = NULL;
  jint res = (*g_vm)->GetEnv(g_vm, (void **)&env, JNI_VERSION_1_6);
  assert(env != NULL && res != 0);

  t_env = env;

  return env;
}

static int japron_cached = 0;

void japron_cache(JNIEnv *env) 
{
  if (japron_cached) return;
  jgmp_cache(env);
  cache_class(japron_string,       "java/lang/String");
  cache_class(japron_object,       "java/lang/Object");
  cache_class(japron_var,          "apron/Var");
  cache_class(japron_mpqscalar,    "apron/MpqScalar");
  cache_class(japron_mpfrscalar,   "apron/MpfrScalar");
  cache_class(japron_doublescalar, "apron/DoubleScalar");
  cache_class(japron_interval,     "apron/Interval");
  cache_class(japron_coeff,        "apron/Coeff");
  cache_class(japron_dimchange,    "apron/Dimchange");
  cache_class(japron_dimperm,      "apron/Dimperm");
  cache_class(japron_linterm0,     "apron/Linterm0");
  cache_class(japron_linexpr0,     "apron/Linexpr0");
  cache_class(japron_lincons0,     "apron/Lincons0");
  cache_class(japron_generator0,   "apron/Generator0");
  cache_class(japron_texpr0bin,    "apron/Texpr0BinNode");
  cache_class(japron_texpr0un,     "apron/Texpr0UnNode");
  cache_class(japron_texpr0dim,    "apron/Texpr0DimNode");
  cache_class(japron_texpr0cst,    "apron/Texpr0CstNode");
  cache_class(japron_texpr0intern, "apron/Texpr0Intern");
  cache_class(japron_tcons0,       "apron/Tcons0");
  cache_class(japron_manager,      "apron/Manager");
  cache_class(japron_abstract0,    "apron/Abstract0");
  cache_class(japron_environment,  "apron/Environment");
  cache_class(japron_dimension,    "apron/Dimension");
  cache_field(japron_mpqscalar_val,     japron_mpqscalar,    "val",     "Lgmp/Mpq;");
  cache_field(japron_mpfrscalar_val,    japron_mpfrscalar,   "val",     "Lgmp/Mpfr;");
  cache_field(japron_doublescalar_val,  japron_doublescalar, "val",     "D");
  cache_field(japron_interval_inf,      japron_interval,     "inf",     "Lapron/Scalar;");
  cache_field(japron_interval_sup,      japron_interval,     "sup",     "Lapron/Scalar;");
  cache_field(japron_dimchange_ptr,     japron_dimchange,    "ptr",     "J");
  cache_field(japron_dimperm_ptr,       japron_dimperm,      "ptr",     "J");
  cache_field(japron_linterm0_dim,      japron_linterm0,     "dim",     "I");
  cache_field(japron_linterm0_coeff,    japron_linterm0,     "coeff",   "Lapron/Coeff;");
  cache_field(japron_linexpr0_ptr,      japron_linexpr0,     "ptr",     "J");
  cache_field(japron_lincons0_expr,     japron_lincons0,     "expr",    "Lapron/Linexpr0;");
  cache_field(japron_lincons0_kind,     japron_lincons0,     "kind",    "I");
  cache_field(japron_lincons0_scalar,   japron_lincons0,     "scalar",  "Lapron/Scalar;");
  cache_field(japron_generator0_coord,  japron_generator0,   "coord",   "Lapron/Linexpr0;");
  cache_field(japron_generator0_kind,   japron_generator0,   "kind",    "I");
  cache_field(japron_texpr0bin_op,      japron_texpr0bin,    "op",      "I");
  cache_field(japron_texpr0bin_rtype,   japron_texpr0bin,    "rtype",   "I");
  cache_field(japron_texpr0bin_rdir,    japron_texpr0bin,    "rdir",    "I");
  cache_field(japron_texpr0bin_larg,    japron_texpr0bin,    "lArg",    "Lapron/Texpr0Node;");
  cache_field(japron_texpr0bin_rarg,    japron_texpr0bin,    "rArg",    "Lapron/Texpr0Node;");
  cache_field(japron_texpr0un_op,       japron_texpr0un,     "op",      "I");
  cache_field(japron_texpr0un_rtype,    japron_texpr0un,     "rtype",   "I");
  cache_field(japron_texpr0un_rdir,     japron_texpr0un,     "rdir",    "I");
  cache_field(japron_texpr0un_arg,      japron_texpr0un,     "arg",     "Lapron/Texpr0Node;");
  cache_field(japron_texpr0cst_cst,     japron_texpr0cst,    "cst",     "Lapron/Coeff;");
  cache_field(japron_texpr0dim_dim,     japron_texpr0dim,    "dim",     "I");
  cache_field(japron_texpr0intern_ptr,  japron_texpr0intern, "ptr",     "J");
  cache_field(japron_tcons0_expr,       japron_tcons0,       "expr",    "Lapron/Texpr0Intern;");
  cache_field(japron_tcons0_kind,       japron_tcons0,       "kind",    "I");
  cache_field(japron_tcons0_scalar,     japron_tcons0,       "scalar",  "Lapron/Scalar;");
  cache_field(japron_manager_ptr,       japron_manager,      "ptr",     "J");
  cache_field(japron_abstract0_ptr,     japron_abstract0,    "ptr",     "J");
  cache_field(japron_environment_ptr,   japron_environment,  "ptr",     "J");
  cache_field(japron_dimension_intdim,  japron_dimension,    "intDim",  "I");
  cache_field(japron_dimension_realdim, japron_dimension,    "realDim", "I");
  cache_init(japron_abstract0);
  cache_init(japron_dimchange);
  cache_init(japron_dimperm);
  cache_init(japron_environment);
  cache_init(japron_linexpr0);
  cache_init(japron_manager);
  cache_init(japron_texpr0intern);
  japron_cached = 1;
}



/* Java <-> Apron objects conversion */
/* ================================= */

/*
  The following functions convert from Java to Apron (set) and
  from Apron to Java (get).

  - Scalar, Coeff, Interval are copied (including Mpq, Mpfr data)
  - Linexpr0 are referenced
  - array set functions store the array size in *pnb.

  Memory management:
  - after a get, call the ap_free/clean function to free the argument
  if needed
  - result of set function must be initialized before the call,
  and ap_free when no longer needed 
  - result of a alloc_set function need to be freed with ap_free (or free)
  when no longer needed (except if there was an error)
  - result of a init_set function need to be freed with japron_clear
  when no longer needed (except if there was an error)

  In case of error:
  - 0 or NULL is returned
  - a Java exception is set

 */



/* Scalar */
/* ----- -*/

/* Scalar o -> ap_scalar_t *c */
int japron_scalar_set(JNIEnv *env, ap_scalar_t* c, jobject o)
{
  check_nonnull(o,0);
  if ((*env)->IsInstanceOf(env, o, japron_mpqscalar)) {
    jobject r = (*env)->GetObjectField(env, o, japron_mpqscalar_val);
    check_nonnull(r,0);
    ap_scalar_set_mpq(c, as_mpq(r));
  }
  else if ((*env)->IsInstanceOf(env, o, japron_mpfrscalar)) {
    jobject r = (*env)->GetObjectField(env, o, japron_mpfrscalar_val);
    ap_scalar_set_mpfr(c, as_mpfr(r));
  }
  else if ((*env)->IsInstanceOf(env, o, japron_doublescalar))
    ap_scalar_set_double(c, (*env)->GetDoubleField(env, o, japron_doublescalar_val));
  else { 
    illegal_argument("unknown Scalar subtype "); 
    return 0; 
  }
  return 1;
}

/* ap_scalar_t c -> Scalar */
jobject japron_scalar_get(JNIEnv *env, ap_scalar_t* c)
{
  check_nonnull(c,NULL);
  jobject p, o;
  switch (c->discr) {

  case AP_SCALAR_DOUBLE:
    o = (*env)->AllocObject(env, japron_doublescalar);
    if (!o) return NULL;
    (*env)->SetDoubleField(env, o, japron_doublescalar_val, c->val.dbl);
    return o;

  case AP_SCALAR_MPQ:
    p = jgmp_alloc_init_mpq(env);
    if (!p) return NULL;
    o = (*env)->AllocObject(env, japron_mpqscalar);
    if (!o) return NULL;
    mpq_set(as_mpq(p), c->val.mpq);
    (*env)->SetObjectField(env, o, japron_mpqscalar_val, p);
    return o;

  case AP_SCALAR_MPFR:
    p = jgmp_alloc_init_mpfr(env);
    if (!p) return NULL;
    o = (*env)->AllocObject(env, japron_mpfrscalar);
    if (!o) return NULL;
    mpfr_set_prec(as_mpfr(p), mpfr_get_prec(c->val.mpfr));
    mpfr_set(as_mpfr(p), c->val.mpfr, GMP_RNDN);
    (*env)->SetObjectField(env, o, japron_mpfrscalar_val, p);
    return o;

  default:
    illegal_argument("unknown ap_scalar_t discriminant");
    return NULL;
  }

}


/* Interval */
/* -------- */

/* Interval o -> ap_interval_t */
int japron_interval_set(JNIEnv *env, ap_interval_t* c, jobject o)
{
  check_nonnull(o,1);
  jobject inf = (*env)->GetObjectField(env, o, japron_interval_inf);
  jobject sup = (*env)->GetObjectField(env, o, japron_interval_sup);
  if (!japron_scalar_set(env, c->inf, inf)) return 0;
  if (!japron_scalar_set(env, c->sup, sup)) return 0;
  return 1;
}

/* ap_interval_t c -> Interval */
jobject japron_interval_get(JNIEnv *env, ap_interval_t* c)
{
  check_nonnull(c,NULL);
  jobject o = (*env)->AllocObject(env, japron_interval);
  if (!o) return NULL;
  jobject inf = japron_scalar_get(env, c->inf);
  if (!inf) return NULL;
  jobject sup = japron_scalar_get(env, c->sup);
  if (!sup) return NULL;
  (*env)->SetObjectField(env, o, japron_interval_inf, inf);
  (*env)->SetObjectField(env, o, japron_interval_sup, sup);
  return o;
}


/* Interval[] o -> ap_interval_t** */
ap_interval_t** japron_interval_array_alloc_set(JNIEnv *env, size_t* pnb, jobjectArray o)
{
  check_nonnull(o,NULL);
  size_t i, nb = (*env)->GetArrayLength(env, o);
  ap_interval_t** t = ap_interval_array_alloc(nb);
  *pnb = nb;
  for (i=0; i<nb; i++) {
    if (!japron_interval_set(env, t[i], 
                             (*env)->GetObjectArrayElement(env, o, i))) {
      ap_interval_array_free(t, nb);
      return NULL;
    }
  }    
  return t;
}

/* ap_interval_t** -> Interval[] */
jobjectArray japron_interval_array_get(JNIEnv *env, ap_interval_t **t, size_t nb)
{
  check_nonnull(t,NULL);
  jobjectArray o = (*env)->NewObjectArray(env, nb, japron_interval, NULL);
  if (!o) return NULL;
  size_t i;
  for (i=0;i<nb;i++) {
    jobject c = japron_interval_get(env, t[i]);
    if (!c) return NULL;
    (*env)->SetObjectArrayElement(env, o, i, c);
  }
  return o;
}


/* Coeff */
/* ----- */

/* Coeff -> ap_coeff_t */
int japron_coeff_set(JNIEnv *env, ap_coeff_t* c, jobject o)
{
  if ((*env)->IsInstanceOf(env, o, japron_interval)) {
    ap_coeff_reinit(c, AP_COEFF_INTERVAL, AP_SCALAR_DOUBLE);
    return japron_interval_set(env, c->val.interval, o);
  }
  else {
    ap_coeff_reinit(c, AP_COEFF_SCALAR, AP_SCALAR_DOUBLE);
    return japron_scalar_set(env, c->val.scalar, o);
  }
}

/* ap_coeff_t -> Coeff */
jobject japron_coeff_get(JNIEnv *env, ap_coeff_t* c)
{
  check_nonnull(c,NULL);
  switch (c->discr) {
  case AP_COEFF_SCALAR:   return japron_scalar_get(env, c->val.scalar);
  case AP_COEFF_INTERVAL: return japron_interval_get(env, c->val.interval);
  default: 
    illegal_argument("unknown ap_coeff_t discriminant");
    return NULL;
  }
}


/* Lincons0 */
/* -------- */

void japron_lincons0_clear(ap_lincons0_t* t)
{
  if (t->scalar) {
    ap_scalar_free(t->scalar);
    t->scalar = NULL;
  }
}

/* Lincons0 -> ap_lincons0_t */
int japron_lincons0_init_set(JNIEnv *env, ap_lincons0_t* t, jobject c)
{
  t->linexpr0 = NULL;
  t->scalar = NULL;
  check_nonnull(c,0);
  t->constyp = (*env)->GetIntField(env, c, japron_lincons0_kind);
  jobject e = (*env)->GetObjectField(env, c, japron_lincons0_expr);
  check_nonnull(e,0);
  t->linexpr0 = as_linexpr0(e);
  jobject s = (*env)->GetObjectField(env, c, japron_lincons0_scalar);
  if (s) {
    t->scalar = ap_scalar_alloc();
    if (!japron_scalar_set(env, t->scalar, s)) { 
      ap_scalar_free(t->scalar);
      t->scalar = NULL;
      return 0;
    }
  }
  return 1;
}

/* ap_lincons0_t -> Lincons0 */
jobject japron_lincons0_get(JNIEnv *env, ap_lincons0_t* t)
{
  check_nonnull(t,NULL);
  jobject c = (*env)->AllocObject(env, japron_lincons0);
  if (!c) return NULL;
  (*env)->SetIntField(env, c, japron_lincons0_kind, t->constyp);
  jobject s = NULL;
  if (t->scalar) {
    s = japron_scalar_get(env, t->scalar);
    if (!s) return NULL;
  }
  (*env)->SetObjectField(env, c, japron_lincons0_scalar, s);
  jobject e = (*env)->NewObject(env, japron_linexpr0, japron_linexpr0_init);
  if (!e) return NULL;
  ap_linexpr0_free(as_linexpr0(e));
  set_linexpr0(e, t->linexpr0);
  t->linexpr0 = NULL;
  (*env)->SetObjectField(env, c, japron_lincons0_expr, e);
  return c;
}


void japron_lincons0_array_clear(ap_lincons0_array_t* t)
{
  size_t i;
  for (i=0;i<t->size;i++)
    if (t->p[i].scalar) ap_scalar_free(t->p[i].scalar);
  if (t->p) {
    free(t->p);
    t->p = NULL;
  }
}

/* Lincons0[] -> ap_lincons0_array_t */
int japron_lincons0_array_init_set(JNIEnv *env, ap_lincons0_array_t* t, jobjectArray o)
{
  t->size = 0;
  t->p = NULL;
  check_nonnull(o,0);
  size_t i, nb = (*env)->GetArrayLength(env, o);
  *t = ap_lincons0_array_make(nb);
  for (i=0; i<nb; i++) {
    jobject c = (*env)->GetObjectArrayElement(env, o, i);
    if (!japron_lincons0_init_set(env, &t->p[i], c)) {
      japron_lincons0_array_clear(t); 
      return 0; 
    }
  }
  return 1;
}

/* ap_lincons0_array_t -> Lincons0[] */
jobjectArray japron_lincons0_array_get(JNIEnv *env, ap_lincons0_array_t* t)
{
  check_nonnull(t,NULL);
  jobjectArray o = (*env)->NewObjectArray(env, t->size, japron_lincons0, NULL);
  if (!o) return NULL;
  size_t i;
  for (i=0; i<t->size; i++) {
    jobject c = japron_lincons0_get(env, &t->p[i]);
    if (!c) return NULL;
    (*env)->SetObjectArrayElement(env, o, i, c);
  }
  return o;
}


/* Generator0 */
/* ---------- */

void japron_generator0_array_clear(ap_generator0_array_t* t)
{
  if (t->p) {
    free(t->p);
    t->p = NULL;
  }
}

/* Generator0[] -> ap_generator0_array_t */
int japron_generator0_array_init_set(JNIEnv *env, ap_generator0_array_t* t, jobjectArray o)
{
  t->size = 0;
  t->p = NULL;
  check_nonnull(o,0);
  size_t i, nb = (*env)->GetArrayLength(env, o);
  *t = ap_generator0_array_make(nb);
  for (i=0; i<nb; i++) {
    jobject c = (*env)->GetObjectArrayElement(env, o, i);
    if (!c) {
      null_pointer_exception("generator is null");
      japron_generator0_array_clear(t);
      return 0;
    }
    t->p[i].gentyp = (*env)->GetIntField(env, c, japron_generator0_kind);
    jobject e = (*env)->GetObjectField(env, c, japron_generator0_coord);
    if (!e) {
      null_pointer_exception("expression is null"); 
      japron_generator0_array_clear(t);
      return 0;
    }
    t->p[i].linexpr0 = as_linexpr0(e);
  }
  return 1;
}

/* ap_generator0_array_t -> Generator0[] */
jobjectArray japron_generator0_array_get(JNIEnv *env, ap_generator0_array_t* t)
{
  check_nonnull(t,NULL);
  jobjectArray o = (*env)->NewObjectArray(env, t->size, japron_generator0, NULL);
  if (!o) return NULL;
  size_t i;
  for (i=0; i<t->size; i++) {
    jobject c = (*env)->AllocObject(env, japron_generator0);
    if (!c) return NULL;
    (*env)->SetIntField(env, c, japron_generator0_kind, t->p[i].gentyp);
    jobject e = (*env)->NewObject(env, japron_linexpr0, japron_linexpr0_init);
    ap_linexpr0_free(as_linexpr0(e));
    set_linexpr0(e, t->p[i].linexpr0);
    t->p[i].linexpr0 = NULL;
    (*env)->SetObjectField(env, c, japron_generator0_coord, e);
    (*env)->SetObjectArrayElement(env, o, i, c);
  }
  return o;
}


/* Tcons0 */
/* ------ */

void japron_tcons0_clear(ap_tcons0_t* t)
{
  if (t->scalar) {
    ap_scalar_free(t->scalar);
    t->scalar = NULL;
  }
}

/* Tcons0 -> ap_tcons0_t */
int japron_tcons0_init_set(JNIEnv *env, ap_tcons0_t* t, jobject c)
{
  t->texpr0 = NULL;
  t->scalar = NULL;
  check_nonnull(c,0);
  t->constyp = (*env)->GetIntField(env, c, japron_tcons0_kind);
  jobject e = (*env)->GetObjectField(env, c, japron_tcons0_expr);
  check_nonnull(e,0);
  t->texpr0 = as_texpr0(e);
  jobject s = (*env)->GetObjectField(env, c, japron_tcons0_scalar);
  if (s) {
    t->scalar = ap_scalar_alloc();
    if (!japron_scalar_set(env, t->scalar, s)) { 
      ap_scalar_free(t->scalar);
      t->scalar = NULL;
      return 0;
    }
  }
  return 1;
}

/* ap_tcons0_t -> Tcons0 */
jobject japron_tcons0_get(JNIEnv *env, ap_tcons0_t* t)
{
  check_nonnull(t,NULL);
  jobject c = (*env)->AllocObject(env, japron_tcons0);
  if (!c) return NULL;
  (*env)->SetIntField(env, c, japron_tcons0_kind, t->constyp);
  jobject s = NULL;
  if (t->scalar) {
    s = japron_scalar_get(env, t->scalar);
    if (!s) return NULL;
  }
  (*env)->SetObjectField(env, c, japron_tcons0_scalar, s);
  jobject e = (*env)->NewObject(env, japron_texpr0intern, japron_texpr0intern_init);
  set_texpr0(e, t->texpr0);
  t->texpr0 = NULL;
  (*env)->SetObjectField(env, c, japron_tcons0_expr, e);
  return c;
}


void japron_tcons0_array_clear(ap_tcons0_array_t* t)
{
  size_t i;
  for (i=0;i<t->size;i++) {
    if (t->p[i].scalar) ap_scalar_free(t->p[i].scalar);
    t->p[i].scalar = NULL;
  }
  if (t->p) {
    free(t->p);
    t->p = NULL;
  }
}

/* Tcons0[] -> ap_tcons0_array_t */
int japron_tcons0_array_init_set(JNIEnv *env, ap_tcons0_array_t* t, jobjectArray o)
{
  check_nonnull(o,0);
  size_t i, nb = (*env)->GetArrayLength(env, o);
  *t = ap_tcons0_array_make(nb);
  for (i=0; i<nb; i++) {
    jobject c = (*env)->GetObjectArrayElement(env, o, i);
    if (!japron_tcons0_init_set(env, &t->p[i], c)) { 
      japron_tcons0_array_clear(t); 
      return 0; 
    }
  }
  return 1;
}

/* ap_tcons0_array_t -> Tcons0[] */
jobjectArray japron_tcons0_array_get(JNIEnv *env, ap_tcons0_array_t* t)
{
  check_nonnull(t,NULL);
  jobjectArray o = (*env)->NewObjectArray(env, t->size, japron_tcons0, NULL);
  if (!o) return NULL;
  size_t i;
  for (i=0; i<t->size; i++) {
    jobject c = japron_tcons0_get(env, &t->p[i]);
    if (!c) return NULL;
    (*env)->SetObjectArrayElement(env, o, i, c);
  }
  return o;
}



/* Texpr0Intern */
/* ------------ */

/* Texpr0Node -> ap_texpr0_t */
ap_texpr0_t* japron_texpr0_alloc_set(JNIEnv *env, jobject o)
{
  check_nonnull(o,NULL);
  if ((*env)->IsInstanceOf(env, o, japron_texpr0bin)) {
    /* binary */
    jobject larg = (*env)->GetObjectField(env, o, japron_texpr0bin_larg);
    check_nonnull(larg,NULL);
    jobject rarg = (*env)->GetObjectField(env, o, japron_texpr0bin_rarg);
    check_nonnull(rarg,NULL);
    jint op = (*env)->GetIntField(env, o, japron_texpr0bin_op);
    jint rtype = (*env)->GetIntField(env, o, japron_texpr0bin_rtype);
    jint rdir = (*env)->GetIntField(env, o, japron_texpr0bin_rdir);
    if (!ap_texpr_is_binop(op)) {
      illegal_argument("unknown binary operator"); 
      return NULL; 
    }
    ap_texpr0_t* l = japron_texpr0_alloc_set(env, larg);
    if (!l) return NULL;
    ap_texpr0_t* r = japron_texpr0_alloc_set(env, rarg);
    if (!r) { ap_texpr0_free(l); return NULL; }
    return ap_texpr0_binop(op, l, r, rtype, rdir);
  }
  else if ((*env)->IsInstanceOf(env, o, japron_texpr0un)) {
    /* unary */
    jobject arg = (*env)->GetObjectField(env, o, japron_texpr0un_arg);
    check_nonnull(arg,NULL);
    jint op = (*env)->GetIntField(env, o, japron_texpr0un_op);
    jint rtype = (*env)->GetIntField(env, o, japron_texpr0un_rtype);
    jint rdir = (*env)->GetIntField(env, o, japron_texpr0un_rdir);
    if (!ap_texpr_is_unop(op)) {
      illegal_argument("unknown unary operator"); 
      return NULL; 
    }
    ap_texpr0_t* a = japron_texpr0_alloc_set(env, arg);
    if (!a) return NULL;
    return ap_texpr0_unop(op, a, rtype, rdir);
  }
  else if ((*env)->IsInstanceOf(env, o, japron_texpr0dim)) {
    /* dimension */
    jint dim = (*env)->GetIntField(env, o, japron_texpr0dim_dim);
    if (dim<0) {
      illegal_argument("invalid dimension"); 
      return NULL; 
    }
    return ap_texpr0_dim(dim);
  }
  else if ((*env)->IsInstanceOf(env, o, japron_texpr0cst)) {
    /* constant */
    jobject cst = (*env)->GetObjectField(env, o, japron_texpr0cst_cst);
    check_nonnull(cst,NULL);
    ap_texpr0_t* x = ap_texpr0_cst_scalar_double(0);
    if (!japron_coeff_set(env, &x->val.cst, cst)) {
      ap_texpr0_free(x);
      return NULL; 
    }
    return x;
  }
  else { 
    illegal_argument("unknown Texpr0Node subtype"); 
    return NULL; 
  }
}

/* ap_texpr0_t -> Texpr0Node */
jobject japron_texpr0_get(JNIEnv *env, ap_texpr0_t* t)
{
  check_nonnull(t,NULL);
  switch (t->discr) {
  case AP_TEXPR_CST:
    {
      /* constant */
      jobject o = (*env)->AllocObject(env, japron_texpr0cst);
      if (!o) return NULL;
      jobject c = japron_coeff_get(env, &t->val.cst);
      if (!c) return NULL;
      (*env)->SetObjectField(env, o, japron_texpr0cst_cst, c);
      return o;
    }
  case AP_TEXPR_DIM:
    {
      /* dimension */
      jobject o = (*env)->AllocObject(env, japron_texpr0dim);
      if (!o) return NULL;
      (*env)->SetIntField(env, o, japron_texpr0dim_dim, t->val.dim);
      return o;
    }
  case AP_TEXPR_NODE:
    if (ap_texpr_is_binop(t->val.node->op)) {
      /* binary */
      jobject o = (*env)->AllocObject(env, japron_texpr0bin);
      if (!o) return NULL;
      jobject l = japron_texpr0_get(env, t->val.node->exprA);
      if (!l) return NULL;
      jobject r = japron_texpr0_get(env, t->val.node->exprB);
      if (!r) return NULL;
      (*env)->SetIntField(env, o, japron_texpr0bin_op,    t->val.node->op);
      (*env)->SetIntField(env, o, japron_texpr0bin_rdir,  t->val.node->dir);
      (*env)->SetIntField(env, o, japron_texpr0bin_rtype, t->val.node->type);
      (*env)->SetObjectField(env, o, japron_texpr0bin_larg, l);
      (*env)->SetObjectField(env, o, japron_texpr0bin_rarg, r);
      return o;
    }
    else {
      /* unary */
      jobject o = (*env)->AllocObject(env, japron_texpr0un);
      if (!o) return NULL;
      jobject a = japron_texpr0_get(env, t->val.node->exprA);
      if (!a) return NULL;
      (*env)->SetIntField(env, o, japron_texpr0un_op,    t->val.node->op);
      (*env)->SetIntField(env, o, japron_texpr0un_rdir,  t->val.node->dir);
      (*env)->SetIntField(env, o, japron_texpr0un_rtype, t->val.node->type);
      (*env)->SetObjectField(env, o, japron_texpr0un_arg, a);
      return o;
    }
  default:
    illegal_argument("unknown ap_texpr0_t discriminant"); 
    return NULL; 
  }
}



/* Linexpr0 */
/* -------- */

/* Linexpr0[] -> ap_linexpr0_t* */
ap_linexpr0_t** japron_linexpr0_array_alloc_set(JNIEnv *env, jobjectArray ar, size_t* pnb)
{
  check_nonnull(ar,NULL);
  size_t i, nb = (*env)->GetArrayLength(env, ar);
  *pnb = nb;
  ap_linexpr0_t** r = (ap_linexpr0_t**) malloc(nb*sizeof(ap_linexpr0_t*));
  for (i=0; i<nb; i++) {
    jobject o = (*env)->GetObjectArrayElement(env, ar, i);
    if (!o) { free(r); return NULL; }
    r[i] = as_linexpr0(o);
  }
  return r;
}


/* Texpr0 */
/* ------ */

/* Texpr0Intern[] -> ap_texpr0_t* */
ap_texpr0_t** japron_texpr0_array_alloc_set(JNIEnv *env, jobjectArray ar, size_t* pnb)
{
  check_nonnull(ar,NULL);
  size_t i, nb = (*env)->GetArrayLength(env, ar);
  *pnb = nb;
  ap_texpr0_t** r = (ap_texpr0_t**) malloc(nb*sizeof(ap_texpr0_t*));
  for (i=0; i<nb; i++) {
    jobject o = (*env)->GetObjectArrayElement(env, ar, i);
    if (!o) { free(r); return NULL; }
    r[i] = as_texpr0(o);
  }
  return r;
}


/* Manager */
/* ------- */

/* ap_manager_t -> Manager */
jobject japron_manager_get(JNIEnv *env, ap_manager_t* m)
{
  check_nonnull(m,NULL);
  jobject mm = (*env)->NewObject(env, japron_manager, japron_manager_init);
  if (!mm) return NULL;
  set_manager(mm, ap_manager_copy(m));
  return mm;
}


/* Dimension */
/* --------- */
 
jobject japron_dimension_get(JNIEnv *env, int intdim, int realdim)
{
  jobject o = (*env)->AllocObject(env, japron_dimension);
  if (!o) return NULL;
  (*env)->SetIntField(env, o, japron_dimension_intdim, intdim);
  (*env)->SetIntField(env, o, japron_dimension_realdim, realdim);
  return o;
}


/* Abstract0 */
/* --------- */

/* ap_abstract0_t -> Abstract0 */
jobject japron_abstract0_get(JNIEnv *env, ap_manager_t* man, ap_abstract0_t* a)
{
  check_nonnull(a,NULL);
  jobject o = (*env)->NewObject(env, japron_abstract0, japron_abstract0_init);
  if (!o) { ap_abstract0_free(man, a); return NULL; }
  set_abstract0(o, a);
  return o;
}

/* Abstract0[] -> ap_abstract_t */
ap_abstract0_t** japron_abstract0_array_alloc_set(JNIEnv *env, jobjectArray ar, size_t *pnb)
{
  check_nonnull(ar,NULL);
  size_t i, nb = (*env)->GetArrayLength(env, ar);
  *pnb = nb;
  ap_abstract0_t** r = (ap_abstract0_t**) malloc(nb*sizeof(ap_abstract0_t*));
  for (i=0; i<nb; i++) {
    jobject o = (*env)->GetObjectArrayElement(env, ar, i);
    if (!o) { free(r); return NULL; }
    r[i] = as_abstract0(o);
  }
  return r;
}


/* Dim */
/* --- */

/* int[] -> ap_dim_t* */
ap_dim_t* japron_dim_array_alloc_set(JNIEnv *env, jintArray ar, size_t* pnb)
{
  check_nonnull(ar,NULL);
  size_t i, nb = (*env)->GetArrayLength(env, ar);
  *pnb = nb;
  ap_dim_t* r = (ap_dim_t*) malloc(nb*sizeof(ap_dim_t));
  jint* buf = (*env)->GetIntArrayElements(env, ar, 0);
  for (i=0; i<nb; i++) {
    if (buf[i]<0) { 
      free(r); 
      (*env)->ReleaseIntArrayElements(env, ar, buf, 0);
      illegal_argument("invalid dimension");
      return NULL; 
    }
    r[i] = buf[i];
  }
  (*env)->ReleaseIntArrayElements(env, ar, buf, 0);
  return r;
}


/* String */
/* ------ */


/* frees all nb strings, and r itself */
void japron_string_array_free(char** r, size_t nb)
{
  size_t i;
  for (i=0;i<nb;i++)
    if (r[i]) free(r[i]);
  free(r);
}

void japron_object_array_free(void** r, size_t nb)
{
  for(size_t i = 0; i < nb; i++) {
    if(r[i]) {
      JNIEnv  *env = get_env();
      jobject j = (jobject) r[i];
      (*env)->DeleteGlobalRef(env, j);
    }
  }

  free(r);
}

/* String[] -> char** */
char** japron_string_array_alloc_set(JNIEnv *env, jintArray ar, size_t* pnb)
{
  check_nonnull(ar,NULL);
  size_t i, nb = (*env)->GetArrayLength(env, ar);
  *pnb = nb;
  char** r = (char**) malloc(nb*sizeof(char*));
  for (i=0; i<nb; i++) r[i] = NULL;
  for (i=0; i<nb; i++) {
    jobject o = (*env)->GetObjectArrayElement(env, ar, i);
    if (!o) { 
      null_pointer_exception("String is null"); 
      japron_string_array_free(r, nb); 
      return NULL; 
    }
    const char* str = (*env)->GetStringUTFChars(env, o, NULL);
    r[i] = strdup(str);
    (*env)->ReleaseStringUTFChars(env, o, str);
  }  
  return r;
}

/* char** -> String[] */   
jobjectArray japron_string_array_get(JNIEnv *env, char** x, size_t nb)
{
  check_nonnull(x,NULL);
  jobjectArray o = (*env)->NewObjectArray(env, nb, japron_string, NULL);
  if (!o) return NULL;
  size_t i;
  for (i=0;i<nb;i++) {
    if (!x[i]) return NULL;
    jstring s = (*env)->NewStringUTF(env, x[i]);
    (*env)->SetObjectArrayElement(env, o, i, s);
  }
  return o;
}

/* Object[] -> void ** */
void** japron_object_array_alloc_set(JNIEnv *env, jobjectArray ar, size_t *pnb)
{
  check_nonnull(ar,NULL);
  size_t nb = (*env)->GetArrayLength(env, ar);
  *pnb = nb;
  
  void**  r = (void**) malloc(nb*sizeof(void*));
  assert(r != NULL);
  memset(r, 0, nb*sizeof(void*));
  for(size_t i = 0; i < nb; i++) {
    jobject o = (*env)->GetObjectArrayElement(env, ar, i);
    if(!o) {
      null_pointer_exception("element of array is null");

      return NULL;
    }

    r[i] = (*env)->NewGlobalRef(env, o);
  }

  return r;
}

/* void ** -> Object[] */
jobjectArray japron_object_array_get(JNIEnv *env, void **x, size_t nb)
{
  check_nonnull(x, NULL);

  jobjectArray o = (*env)->NewObjectArray(env, nb, japron_object, NULL);
  if(!o) return NULL;
  
  for(size_t i = 0; i < nb; i++) {
    jobject ref = (*env)->NewGlobalRef(env, x[i]);
    (*env)->SetObjectArrayElement(env, o, i, ref);
  }

  return (*env)->NewGlobalRef(env, o);
}

/* void ** -> Var[] */
jobjectArray japron_var_array_get(JNIEnv *env, void **x, size_t nb)
{
  check_nonnull(x, NULL);

  jobjectArray o = (*env)->NewObjectArray(env, nb, japron_var, NULL);
  if(!o) return NULL;
  
  for(size_t i = 0; i < nb; i++) {
    jobject ref = (*env)->NewGlobalRef(env, x[i]);
    (*env)->SetObjectArrayElement(env, o, i, ref);
  }

  return (*env)->NewGlobalRef(env, o);
}

/* Apron utilities */
/* =============== */

/* max variable dimension + 1 (or 0 if empty) */ 
ap_dim_t japron_linexpr0_max_dim(ap_linexpr0_t* e)
{
  size_t i;
  if (e->discr==AP_LINEXPR_DENSE) return e->size;
  for (i=e->size-1;i>0;i--) {
    ap_dim_t x = e->p.linterm[i].dim;
    if (x!=AP_DIM_MAX) return x+1;
  }
  ap_dim_t x = e->p.linterm[0].dim;
  return (x==AP_DIM_MAX) ? 0 : x+1;
}

/* disables abort-on-exception for all functions */
void japron_manager_setup(ap_manager_t* m)
{
  ap_exc_t exn;
  for (exn=0; exn<AP_EXC_SIZE; exn++)
    m->option.abort_if_exception[exn] = false;
}

/* throws an exception in from the manager's exclog;
   also, frees a 
 */
void japron_exc(JNIEnv *env, ap_manager_t* m)
{
  /* Construct message */
  check_nonnull(m->result.exclog,);
  ap_funid_t funid = m->result.exclog->funid;
  ap_exc_t exn = m->result.exclog->exn;
  char msg[1024];
  snprintf(msg, sizeof(msg)-1, "in %s: %s",
           ap_name_of_funid[funid], m->result.exclog->msg);
  ap_manager_clear_exclog(m);

  /* Special case; IllegalArgumentException */
  if (exn==AP_EXC_INVALID_ARGUMENT) {
    jclass cls = (*env)->FindClass(env, "java/lang/IllegalArgumentException");
    if (!cls) return;
    (*env)->ThrowNew(env, cls, msg);
    return;
  }

  /* ApronException (or subclass) case */
  jclass cls;
  switch (exn) {
  case AP_EXC_TIMEOUT:
    cls = (*env)->FindClass(env, "apron/TimeoutException");
    break;
  case AP_EXC_OUT_OF_SPACE:
    cls = (*env)->FindClass(env, "apron/OutOfSpaceException");
    break;
  case AP_EXC_OVERFLOW:
    cls = (*env)->FindClass(env, "apron/OverflowException");
    break;
  case AP_EXC_NOT_IMPLEMENTED:
    cls = (*env)->FindClass(env, "apron/NotImplementedException");
    break;
  default:
    cls = (*env)->FindClass(env, "apron/ApronException");
    break;
  }

  /* Constructs exception */
  if (!cls) return;
  jmethodID meth = 
    (*env)->GetMethodID(env, cls, "<init>",
                        "(Ljava/lang/String;Lapron/Manager;I)V");
  if (!meth) return;

  jstring omsg = (*env)->NewStringUTF(env, msg);
  if (!omsg) return;

  jobject om = japron_manager_get(env, m);
  if (!om) return;
  
  jobject e = (*env)->NewObject(env, cls, meth, omsg, om, funid);
  if (!e) return;

  /* Throws exception */
  (*env)->Throw(env, e);
}
