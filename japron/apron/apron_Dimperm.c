/*
 * apron_Dimperm.c
 *
 * glue for Dimperm.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include "japron.h"
#include "apron_Dimperm.h"

//////////////////////////////////////

/*
 * Class:     apron_Dimperm
 * Method:    init
 * Signature: ([I)V
 */
JNIEXPORT void JNICALL Java_apron_Dimperm_init___3I
  (JNIEnv *env, jobject o, jintArray ar)
{
  check_nonnull(o,);
  check_nonnull(ar,);
  size_t i, nb = (*env)-> GetArrayLength(env, ar);
  ap_dimperm_t* x = ap_dimperm_alloc(nb);
  jint* buf = (*env)->GetIntArrayElements(env, ar, 0);
  for (i=0; i<nb; i++) {
    if (buf[i]<0 || (size_t)buf[i]>=nb) {
      ap_dimperm_free(x);
      (*env)->ReleaseIntArrayElements(env, ar, buf, 0);
      illegal_argument("invalid dimension in array"); 
      return; 
    }
    x->dim[i] = buf[i];
  }
  (*env)->ReleaseIntArrayElements(env, ar, buf, 0);
  set_dimperm(o,x);
}

/*
 * Class:     apron_Dimperm
 * Method:    init
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_apron_Dimperm_init__I
  (JNIEnv *env, jobject o, jint nb)
{
  check_nonnull(o,);
  check_positive(nb,);
  ap_dimperm_t* x = ap_dimperm_alloc(nb);
  ap_dimperm_set_id(x);
  set_dimperm(o,x);
}

/*
 * Class:     apron_Dimperm
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Dimperm_finalize
  (JNIEnv *env, jobject o)
{
  if (!o) return;
  ap_dimperm_t* x = as_dimperm(o);
  if (x) ap_dimperm_free(x);
}

/*
 * Class:     apron_Dimperm
 * Method:    class_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Dimperm_class_1init
  (JNIEnv * env, jclass cls)
{
  japron_cache(env);
}

/*
 * Class:     apron_Dimperm
 * Method:    setId
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Dimperm_setId
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,);
  ap_dimperm_set_id(as_dimperm(o));
}


/*
 * Class:     apron_Dimperm
 * Method:    setElem
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_apron_Dimperm_setElem
  (JNIEnv *env, jobject o, jint i, jint dim)
{
  check_nonnull(o,);
  ap_dimperm_t* x = as_dimperm(o);
  if (i<0 || (size_t)i>=x->size) { out_of_bounds("invalid dimension"); return; }
  if (dim<0 || (size_t)dim>=x->size) { illegal_argument("invalid dimension"); return; }
  x->dim[i] = dim;
}

/*
 * Class:     apron_Dimperm
 * Method:    getElem
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_apron_Dimperm_getElem
  (JNIEnv *env, jobject o, jint i)
{
  check_nonnull(o,0);
  ap_dimperm_t* x = as_dimperm(o);
  if (i<0 || (size_t)i>=x->size) { out_of_bounds("invalid dimension"); return 0; }
  return x->dim[i];
}

/*
 * Class:     apron_Dimperm
 * Method:    getSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_apron_Dimperm_getSize
(JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  return (as_dimperm(o))->size;
}

/*
 * Class:     apron_Dimperm
 * Method:    getContents
 * Signature: ()[I
 */
JNIEXPORT jintArray JNICALL Java_apron_Dimperm_getContents
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,NULL);
  size_t i;
  ap_dimperm_t* x = as_dimperm(o);
  jintArray r = (*env)->NewIntArray(env, x->size);
  if (!r) return NULL;
  jint* buf = (*env)->GetIntArrayElements(env, r, 0);
  for (i=0;i<x->size;i++)
    buf[i] = x->dim[i];
  (*env)->ReleaseIntArrayElements(env, r, buf, 0);
  return r;
}

/*
  * Class:     apron_Dimperm
 * Method:    invert
 * Signature: ()Lapron/Dimperm;
 */
JNIEXPORT jobject JNICALL Java_apron_Dimperm_invert
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,NULL);
  ap_dimperm_t* x = as_dimperm(o);
  jobject oo = (*env)->NewObject(env, japron_dimperm, japron_dimperm_init);
  if (!oo) return NULL;
  ap_dimperm_t* y = ap_dimperm_alloc(x->size);
  ap_dimperm_invert(y, x);
  set_dimperm(oo,y);
  return oo;
}

/*
 * Class:     apron_Dimperm
 * Method:    compose
 * Signature: (Lapron/Dimperm;)Lapron/Dimperm;
 */
JNIEXPORT jobject JNICALL Java_apron_Dimperm_compose
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,NULL);
  check_nonnull(o2,NULL);
  ap_dimperm_t* x = as_dimperm(o1);
  ap_dimperm_t* y = as_dimperm(o2);
  if (x->size!=y->size) { illegal_argument("incompatible sizes"); return NULL; }
  jobject oo = (*env)->NewObject(env, japron_dimperm, japron_dimperm_init);
  if (!oo) return NULL;
  ap_dimperm_t* z = ap_dimperm_alloc(x->size);
  ap_dimperm_compose(z, x, y);
  set_dimperm(oo,z);
  return oo;
}
