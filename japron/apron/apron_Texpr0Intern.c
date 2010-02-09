/*
 * apron_Texpr0Intern.c
 *
 * glue for Texpr0Intern.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include "japron.h"
#include "apron_Texpr0Intern.h"

//////////////////////////////////////

/*
 * Class:     apron_Texpr0Intern
 * Method:    init
 * Signature: (Lapron/Texpr0Node;)V
 */
JNIEXPORT void JNICALL Java_apron_Texpr0Intern_init__Lapron_Texpr0Node_2
  (JNIEnv *env, jobject o, jobject c)
{
  check_nonnull(o,);
  check_nonnull(c,);
  ap_texpr0_t* e = japron_texpr0_alloc_set(env, c);
  if (!e) return;
  set_texpr0(o, e);
}

/*
 * Class:     apron_Texpr0Intern
 * Method:    init
 * Signature: (Lapron/Texpr0Intern;)V
 */
JNIEXPORT void JNICALL Java_apron_Texpr0Intern_init__Lapron_Texpr0Intern_2
  (JNIEnv *env, jobject o, jobject t)
{
  check_nonnull(o,);
  check_nonnull(t,);
  set_texpr0(o, ap_texpr0_copy(as_texpr0(t)));
}

/*
 * Class:     apron_Texpr0Intern
 * Method:    init
 * Signature: (Lapron/Linexpr0;)V
 */
JNIEXPORT void JNICALL Java_apron_Texpr0Intern_init__Lapron_Linexpr0_2
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  set_texpr0(o1, ap_texpr0_from_linexpr0(as_linexpr0(o2)));
}

/*
 * Class:     apron_Texpr0Intern
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Texpr0Intern_finalize
  (JNIEnv *env, jobject o)
{
  if (!o) return;
  ap_texpr0_t* p = as_texpr0(o);
  if (p) ap_texpr0_free(p);
}

/*
 * Class:     apron_Texpr0Intern
 * Method:    class_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Texpr0Intern_class_1init
  (JNIEnv *env, jclass cls)
{
  japron_cache(env);
}

/*
 * Class:     apron_Texpr0Intern
 * Method:    toTexpr0Node
 * Signature: ()Lapron/Texpr0Node;
 */
JNIEXPORT jobject JNICALL Java_apron_Texpr0Intern_toTexpr0Node
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,NULL);
  return japron_texpr0_get(env, as_texpr0(o));
}

/*
 * Class:     apron_Texpr0Intern
 * Method:    getDepth
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_apron_Texpr0Intern_getDepth
  (JNIEnv * env, jobject o1)
{
  check_nonnull(o1,0);
  return ap_texpr0_depth(as_texpr0(o1));
}

/*
 * Class:     apron_Texpr0Intern
 * Method:    getSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_apron_Texpr0Intern_getSize
  (JNIEnv * env, jobject o1)
{
  check_nonnull(o1,0);
  return ap_texpr0_size(as_texpr0(o1));
}

/*
 * Class:     apron_Texpr0Intern
 * Method:    maxDim
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_apron_Texpr0Intern_maxDim
  (JNIEnv * env, jobject o1)
{
  check_nonnull(o1,0);
  return ap_texpr0_max_dim(as_texpr0(o1));
}

/*
 * Class:     apron_Texpr0Intern
 * Method:    hasDim
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Texpr0Intern_hasDim
  (JNIEnv * env, jobject o1, jint d)
{
  check_nonnull(o1,0);
  return ap_texpr0_has_dim(as_texpr0(o1), d);
}

/*
 * Class:     apron_Texpr0Intern
 * Method:    getDims
 * Signature: ()[I
 */
JNIEXPORT jintArray JNICALL Java_apron_Texpr0Intern_getDims
  (JNIEnv * env, jobject o1)
{
  check_nonnull(o1,0);
  ap_dim_t* l = ap_texpr0_dimlist(as_texpr0(o1));
  int nb, i;
  for (nb=0; l[nb]!=AP_DIM_MAX; nb++);
  jintArray r = (*env)->NewIntArray(env, nb);
  if (!r) { free(l); return NULL; }
  jint* buf = (*env)->GetIntArrayElements(env, r, 0);
  for (i=0; i<nb; i++) buf[i] = l[i];
  (*env)->ReleaseIntArrayElements(env, r, buf, 0);
  free(l);
  return r;
}

/*
 * Class:     apron_Texpr0Intern
 * Method:    isIntervalCst
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Texpr0Intern_isIntervalCst
  (JNIEnv * env, jobject o1)
{
  check_nonnull(o1,0);
  return ap_texpr0_is_interval_cst(as_texpr0(o1));
}

/*
 * Class:     apron_Texpr0Intern
 * Method:    isIntervalLinear
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Texpr0Intern_isIntervalLinear
  (JNIEnv * env, jobject o1)
{
  check_nonnull(o1,0);
  return ap_texpr0_is_interval_linear(as_texpr0(o1));
}

/*
 * Class:     apron_Texpr0Intern
 * Method:    isIntervalPolynomial
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Texpr0Intern_isIntervalPolynomial
  (JNIEnv * env, jobject o1)
{
  check_nonnull(o1,0);
  return ap_texpr0_is_interval_polynomial(as_texpr0(o1));
}

/*
 * Class:     apron_Texpr0Intern
 * Method:    isIntervalPolyfrac
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Texpr0Intern_isIntervalPolyfrac
  (JNIEnv * env, jobject o1)
{
  check_nonnull(o1,0);
  return ap_texpr0_is_interval_polyfrac(as_texpr0(o1));
}

/*
 * Class:     apron_Texpr0Intern
 * Method:    isScalar
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Texpr0Intern_isScalar
  (JNIEnv * env, jobject o1)
{
  check_nonnull(o1,0);
  return ap_texpr0_is_scalar(as_texpr0(o1));
}

/*
 * Class:     apron_Texpr0Intern
 * Method:    substitute
 * Signature: (ILapron/Texpr0Intern;)V
 */
JNIEXPORT void JNICALL Java_apron_Texpr0Intern_substitute
  (JNIEnv * env, jobject o1, jint i, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  ap_texpr0_substitute_with(as_texpr0(o1), i, as_texpr0(o2));
}

/*
 * Class:     apron_Texpr0Intern
 * Method:    addDimensions
 * Signature: (Lapron/Dimchange;)V
 */
JNIEXPORT void JNICALL Java_apron_Texpr0Intern_addDimensions
  (JNIEnv * env, jobject o1, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  ap_texpr0_add_dimensions_with(as_texpr0(o1), as_dimchange(o2));
}

/*
 * Class:     apron_Texpr0Intern
 * Method:    removeDimensions
 * Signature: (Lapron/Dimchange;)V
 */
JNIEXPORT void JNICALL Java_apron_Texpr0Intern_removeDimensions
  (JNIEnv * env, jobject o1, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  ap_texpr0_remove_dimensions_with(as_texpr0(o1), as_dimchange(o2));
}

/*
 * Class:     apron_Texpr0Intern
 * Method:    permuteDimensions
 * Signature: (Lapron/Dimperm;)V
 */
JNIEXPORT void JNICALL Java_apron_Texpr0Intern_permuteDimensions
  (JNIEnv * env, jobject o1, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  ap_texpr0_t* t = as_texpr0(o1);
  ap_dimperm_t* p = as_dimperm(o2);
  if (p->size < ap_texpr0_max_dim(t))
    { illegal_argument("permutation is too short"); return; }
  ap_texpr0_permute_dimensions_with(t,  p);
}

/*
 * Class:     apron_Texpr0Intern
 * Method:    hashCode
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_apron_Texpr0Intern_hashCode
  (JNIEnv * env, jobject o1)
{
  check_nonnull(o1,0);
  return ap_texpr0_hash(as_texpr0(o1));
}

/*
 * Class:     apron_Texpr0Intern
 * Method:    isEqual
 * Signature: (Lapron/Texpr0Intern;)Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Texpr0Intern_isEqual
  (JNIEnv * env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return ap_texpr0_equal(as_texpr0(o1), as_texpr0(o2));
}
