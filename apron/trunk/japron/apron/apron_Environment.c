/*
 * apron_Environment.c
 *
 * glue for Environment.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include "japron.h"
#include "apron_Environment.h"

//////////////////////////////////////

/*
 * Class:     apron_Environment
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Environment_init__
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,);
  set_environment(o, ap_environment_alloc_empty());
}

/*
 * Class:     apron_Environment
 * Method:    init
 * Signature: ([Lapron/Var;[Lapron/Var;)V
 */
JNIEXPORT void JNICALL Java_apron_Environment_init___3Lapron_Var_2_3Lapron_Var_2
  (JNIEnv *env, jobject o, jobjectArray ar1, jobjectArray ar2)
{
  check_nonnull(o,);
  check_nonnull(ar1,);
  check_nonnull(ar2,);

  size_t nb1, nb2;
  void** x1 = japron_object_array_alloc_set(env, ar1, &nb1);
  if (!x1) return;
  void** x2 = japron_object_array_alloc_set(env, ar2, &nb2);
  if (!x2) { japron_object_array_free(x1, nb1); return; }
  ap_environment_t* r = ap_environment_alloc(x1, nb1, x2, nb2);
  japron_object_array_free(x1, nb1);
  japron_object_array_free(x2, nb2);
  if (!r) { illegal_argument("environment has duplicate names"); return; }
  set_environment(o, r);
}

/*
 * Class:     apron_Environment
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Environment_finalize
  (JNIEnv *env, jobject o)
{
  if (!o) return;
  ap_environment_t* e = as_environment(o);
  if (e) ap_environment_free(e);
}

/*
 * Class:     apron_Environment
 * Method:    class_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Environment_class_1init
  (JNIEnv *env, jclass cls)
{
  japron_cache(env);
}

/*
 * Class:     apron_Environment
 * Method:    add
 * Signature: ([Lapron/Var;[Lapron/Var;)Lapron/Environment;
 */
JNIEXPORT jobject JNICALL Java_apron_Environment_add
  (JNIEnv *env, jobject o, jobjectArray ar1, jobjectArray ar2)
{
  check_nonnull(o,NULL);
  jobject rr = (*env)->NewObject(env, japron_environment, japron_environment_init);
  if (!rr) return NULL;
  size_t nb1 = 0, nb2 = 0;
  void** x1 = NULL, **x2 = NULL;
  if (ar1) {
    x1 = japron_object_array_alloc_set(env, ar1, &nb1);
    if (!x1) return NULL;
  }
  if (ar2) {
    x2 = japron_object_array_alloc_set(env, ar2, &nb2);
    if (!x2) { if (x1) japron_object_array_free(x1, nb1); return NULL; }
  }
  ap_environment_t* r = 
    ap_environment_add(as_environment(o), x1, nb1, x2, nb2);
  if (x1) japron_object_array_free(x1, nb1);
  if (x2) japron_object_array_free(x2, nb2);
  if (!r) { illegal_argument("environment has duplicate names"); return NULL; }
  ap_environment_free(as_environment(rr));
  set_environment(rr, r);
  return rr;
}

/*
 * Class:     apron_Environment
 * Method:    addPerm
 * Signature: ([Lapron/Var;[Lapron/Var;[Lapron/Dimperm;)Lapron/Environment;
 */
JNIEXPORT jobject JNICALL Java_apron_Environment_addPerm
  (JNIEnv *env, jobject o, jobjectArray ar1, jobjectArray ar2, jobjectArray pr)
{
  check_nonnull(o,NULL);
  jobject rr = (*env)->NewObject(env, japron_environment, japron_environment_init);
  if (!rr) return NULL;
  size_t nb1 = 0, nb2 = 0;
  void** x1 = NULL, **x2 = NULL;
  if (ar1) {
    x1 = japron_object_array_alloc_set(env, ar1, &nb1);
    if (!x1) return NULL;
  }
  if (ar2) {
    x2 = japron_object_array_alloc_set(env, ar2, &nb2);
    if (!x2) { if (x1) japron_object_array_free(x1, nb1); return NULL; }
  }
  ap_dimperm_t p = { NULL, 0 };
  ap_environment_t* r = 
    ap_environment_add_perm(as_environment(o), x1, nb1, x2, nb2, &p);
  if (x1) japron_object_array_free(x1, nb1);
  if (x2) japron_object_array_free(x2, nb2);
  if (!r) {
    if (p.dim) free(p.dim); 
    illegal_argument("environment has duplicate names"); 
    return NULL; 
  }
  jobject pp = (*env)->NewObject(env, japron_dimperm, japron_dimperm_init);
  if (!pp) { ap_environment_free(r); if (p.dim) free(p.dim); return NULL; }
  ap_dimperm_t* p2 = (ap_dimperm_t*)malloc(sizeof(ap_dimperm_t));
  *p2 = p;
  set_dimperm(pp,p2);
  (*env)->SetObjectArrayElement(env, pr, 0, pp);
  ap_environment_free(as_environment(rr));
  set_environment(rr, r);
  return rr;
}

/*
 * Class:     apron_Environment
 * Method:    remove
 * Signature: ([Lapron/Var;)Lapron/Environment;
 */
JNIEXPORT jobject JNICALL Java_apron_Environment_remove
  (JNIEnv *env, jobject o, jobjectArray ar)
{
  check_nonnull(o,NULL);
  check_nonnull(ar,NULL);
  jobject rr = (*env)->NewObject(env, japron_environment, japron_environment_init);
  if (!rr) return NULL;
  size_t nb;
  void** x = japron_object_array_alloc_set(env, ar, &nb);
  if (!x) return NULL;
  ap_environment_t* r = ap_environment_remove(as_environment(o), x, nb);
  japron_object_array_free(x, nb);
  if (!r) { illegal_argument("removing nonexistent names"); return NULL; }
  ap_environment_free(as_environment(rr));
  set_environment(rr, r);
  return rr;
}

/*
 * Class:     apron_Environment
 * Method:    lce
 * Signature: (Lapron/Environment;)Lapron/Environment;
 */
JNIEXPORT jobject JNICALL Java_apron_Environment_lce__Lapron_Environment_2
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,NULL);
  check_nonnull(o2,NULL);
  jobject rr = (*env)->NewObject(env, japron_environment, japron_environment_init);
  if (!rr) return NULL;
  ap_dimchange_t *p1 = NULL, *p2 = NULL;
  ap_environment_t* r = 
    ap_environment_lce(as_environment(o1), as_environment(o2), &p1, &p2);
  if (p1) ap_dimchange_free(p1);
  if (p2) ap_dimchange_free(p2);
  if (!r) { illegal_argument("incompatible environments"); return NULL; }
  ap_environment_free(as_environment(rr));
  set_environment(rr, r);
  return rr;
}

/*
 * Class:     apron_Environment
 * Method:    lce
 * Signature: ([Lapron/Environment;)Lapron/Environment;
 */
JNIEXPORT jobject JNICALL Java_apron_Environment_lce___3Lapron_Environment_2
  (JNIEnv *env, jclass cls, jobjectArray ar)
{
  check_nonnull(ar,NULL);
  jobject rr = (*env)->NewObject(env, japron_environment, japron_environment_init);
  if (!rr) return NULL;
  size_t i, nb = (*env)->GetArrayLength(env, ar);
  ap_environment_t** e = 
    (ap_environment_t**)malloc(nb*sizeof(ap_environment_t*));
  for (i=0;i<nb;i++) {
    jobject o = (*env)->GetObjectArrayElement(env, ar, i);
    if (!o) {
      free(e); 
      null_pointer_exception("environment is null");  
      return NULL;
    }
    e[i] = as_environment(o);
  }
  ap_dimchange_t** x = NULL;
  ap_environment_t* r = ap_environment_lce_array(e, nb, &x);
  free(e);
  if (x) {
    for (i=0; i<nb; i++)
      if (x[i]) ap_dimchange_free(x[i]);
    free(x);
  }
  if (!r) { 
    illegal_argument("incompatible environments"); 
    return NULL; 
  }
  ap_environment_free(as_environment(rr));
  set_environment(rr, r);
  return rr;
}

/*
 * Class:     apron_Environment
 * Method:    rename
 * Signature: ([Lapron/Var;[Lapron/Var;[Lapron/Dimperm;)Lapron/Environment;
 */
JNIEXPORT jobject JNICALL Java_apron_Environment_rename
  (JNIEnv *env, jobject o, jobjectArray ar1, jobjectArray ar2, jobjectArray pr)
{
  check_nonnull(o,NULL);
  check_nonnull(ar1,NULL);
  check_nonnull(ar2,NULL);
  jobject rr = (*env)->NewObject(env, japron_environment, japron_environment_init);
  if (!rr) return NULL;
  size_t nb1, nb2;
  void** x1, **x2;
  x1 = japron_object_array_alloc_set(env, ar1, &nb1);
  if (!x1) return NULL;
  x2 = japron_object_array_alloc_set(env, ar2, &nb2);
  if (!x2) { 
    japron_object_array_free(x1, nb1); 
    return NULL;
  }
  if (nb1!=nb2) { 
    illegal_argument("incompatible array sizes");
    japron_object_array_free(x1, nb1);
    japron_object_array_free(x2, nb2);
    return NULL;
  }
  ap_dimperm_t p = { NULL, 0 };
  ap_environment_t* r = 
    ap_environment_rename(as_environment(o), x1, x2, nb1, &p);
  japron_object_array_free(x1, nb1);
  japron_object_array_free(x2, nb2);
  if (!r) { illegal_argument("environment has duplicate names"); return NULL; }
  if (pr) {
    jobject pp = (*env)->NewObject(env, japron_dimperm, japron_dimperm_init);
    if (!pp) { ap_environment_free(r); if (p.dim) free(p.dim); return NULL; }
    ap_dimperm_t* p2 = (ap_dimperm_t*)malloc(sizeof(ap_dimperm_t));
    *p2 = p;
    set_dimperm(pp,p2);
    (*env)->SetObjectArrayElement(env, pr, 0, pp);
  }
  else
    if (p.dim) free(p.dim);
  ap_environment_free(as_environment(rr));
  set_environment(rr, r);
  return rr;
}

/*
 * Class:     apron_Environment
 * Method:    getDimension
 * Signature: ()Lapron/Dimension;
 */
JNIEXPORT jobject JNICALL Java_apron_Environment_getDimension
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,NULL);
  ap_environment_t* d = as_environment(o);
  return japron_dimension_get(env, d->intdim, d->realdim);
}

/*
 * Class:     apron_Environment
 * Method:    hasVar
 * Signature: (Lapron/Var;)Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Environment_hasVar
  (JNIEnv *env, jobject o, jobject v)
{
  check_nonnull(o,0);
  check_nonnull(v,0);
  bool r = ap_environment_mem_var(as_environment(o), v);
  return r;
}

/*
 * Class:     apron_Environment
 * Method:    dimOfVar
 * Signature: (Lapron/Var;)I
 */
JNIEXPORT jint JNICALL Java_apron_Environment_dimOfVar
  (JNIEnv *env, jobject o, jobject v)
{
  check_nonnull(o,0);
  check_nonnull(v,0);
  ap_dim_t d = ap_environment_dim_of_var(as_environment(o), v);
  if (d==AP_DIM_MAX) {
    illegal_argument("no such variable"); 
    return 0;
  }
  return d;
}

/*
 * Class:     apron_Environment
 * Method:    varOfDim
 * Signature: (I)Lapron/Var;
 */
JNIEXPORT jobject JNICALL Java_apron_Environment_varOfDim
  (JNIEnv *env, jobject o, jint d)
{
  check_nonnull(o,NULL);
  ap_environment_t* e = as_environment(o);
  if (d<0 || (size_t)d>=e->intdim+e->realdim) {
    out_of_bounds("no such dimension");
    return NULL;
  }
  return (*env)->NewGlobalRef(env, e->var_of_dim[d]);
}

/*
 * Class:     apron_Environment
 * Method:    getIntVars
 * Signature: ()[Lapron/Var;
 */
JNIEXPORT jobjectArray JNICALL Java_apron_Environment_getIntVars
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,NULL);
  ap_environment_t* e = as_environment(o);
  return japron_var_array_get(env, e->var_of_dim, e->intdim);
}

/*
 * Class:     apron_Environment
 * Method:    getRealVars
 * Signature: ()[Lapron/Var;
 */
JNIEXPORT jobjectArray JNICALL Java_apron_Environment_getRealVars
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,NULL);
  ap_environment_t* e = as_environment(o);
  return japron_var_array_get(env, (e->var_of_dim)+e->intdim, e->realdim);
}

/*
 * Class:     apron_Environment
 * Method:    getVars
 * Signature: ()[Lapron/Var;
 */
JNIEXPORT jobjectArray JNICALL Java_apron_Environment_getVars
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,NULL);
  ap_environment_t* e = as_environment(o);
  return japron_var_array_get(env, e->var_of_dim, e->intdim + e->realdim);
}

/*
 * Class:     apron_Environment
 * Method:    dimchange
 * Signature: (Lapron/Environment;)Lapron/Dimchange;
 */
JNIEXPORT jobject JNICALL Java_apron_Environment_dimchange
  (JNIEnv *env, jobject o, jobject e)
{
  check_nonnull(o,NULL);
  check_nonnull(e,NULL);
  ap_dimchange_t* x = 
    ap_environment_dimchange(as_environment(o),as_environment(e));
  if (!x) {
    illegal_argument("not a super-environment");
    return NULL;
  }
  jobject r = (*env)->NewObject(env, japron_dimchange, japron_dimchange_init);
  if (!r) { ap_dimchange_free(x); return NULL; }
  set_dimchange(r, x);
  return r;
}

/*
 * Class:     apron_Environment
 * Method:    dimchange2
 * Signature: (Lapron/Environment;)[Lapron/Dimchange;
 */
JNIEXPORT jobjectArray JNICALL Java_apron_Environment_dimchange2
  (JNIEnv *env, jobject o, jobject e)
{
  check_nonnull(o,NULL);
  check_nonnull(e,NULL);
  ap_dimchange2_t* x = 
    ap_environment_dimchange2(as_environment(o),as_environment(e));
  if (!x) {
    illegal_argument("incompatible environments");
    return NULL;
  }
  jobjectArray r = (*env)->NewObjectArray(env, 2, japron_dimchange, NULL);
  jobject d1 = (*env)->NewObject(env, japron_dimchange, japron_dimchange_init);
  jobject d2 = (*env)->NewObject(env, japron_dimchange, japron_dimchange_init);
  if (!r) { ap_dimchange2_free(x); return NULL; }
  if (!d1) { ap_dimchange2_free(x); return NULL; }
  if (!d2) { ap_dimchange2_free(x); return NULL; }
  if (!x->add)    x->add = ap_dimchange_alloc(0,0);
  if (!x->remove) x->remove = ap_dimchange_alloc(0,0);
  set_dimchange(d1, x->add);
  set_dimchange(d2, x->remove);
  (*env)->SetObjectArrayElement(env, r, 0, d1);
  (*env)->SetObjectArrayElement(env, r, 1, d2);
  free(x);
  return r;
}

/*
 * Class:     apron_Environment
 * Method:    isEqual
 * Signature: (Lapron/Environment;)Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Environment_isEqual
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return ap_environment_is_eq(as_environment(o1), as_environment(o2));
}

/*
 * Class:     apron_Environment
 * Method:    isIncluded
 * Signature: (Lapron/Environment;)Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Environment_isIncluded
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return ap_environment_is_leq(as_environment(o1), as_environment(o2));
}

/*
 * Class:     apron_Environment
 * Method:    cmp
 * Signature: (Lapron/Environment;)I
 */
JNIEXPORT jint JNICALL Java_apron_Environment_cmp
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return ap_environment_compare(as_environment(o1), as_environment(o2));
}

/*
 * Class:     apron_Environment
 * Method:    hashCode
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_apron_Environment_hashCode
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  return ap_environment_hash(as_environment(o));
}
