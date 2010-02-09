/*
 * apron_Dimchange.c
 *
 * glue for Dimchange.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include "japron.h"
#include "apron_Dimchange.h"

//////////////////////////////////////

/*
 * Class:     apron_Dimchange
 * Method:    init
 * Signature: (II[I)V
 */
JNIEXPORT void JNICALL Java_apron_Dimchange_init
  (JNIEnv *env, jobject o, jint i, jint r, jintArray ar)
{
  check_nonnull(o,);
  check_nonnull(ar,);
  check_positive(i,);
  check_positive(r,);
  size_t k, nb = (*env)-> GetArrayLength(env, ar);
  if ((size_t)i+r!=nb) { illegal_argument("invalid array size"); return; }
  ap_dimchange_t* x = ap_dimchange_alloc(i,r);
  jint* buf = (*env)->GetIntArrayElements(env, ar, 0);
  for (k=0; k<nb; k++) {
    if (buf[k]<0)  {
      ap_dimchange_free(x);
      (*env)->ReleaseIntArrayElements(env, ar, buf, 0);
     illegal_argument("invalid dimension"); 
      return; 
    }
    x->dim[k] = buf[k];
  }
  (*env)->ReleaseIntArrayElements(env, ar, buf, 0);
  set_dimchange(o, x);
}

/*
 * Class:     apron_Dimchange
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Dimchange_finalize
  (JNIEnv *env, jobject o)
{
  if (!o) return;
  ap_dimchange_t* d = as_dimchange(o);
  if (d) ap_dimchange_free(d);
}

/*
 * Class:     apron_Dimchange
 * Method:    class_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Dimchange_class_1init
  (JNIEnv *env, jclass cls)
{
  japron_cache(env);
}

/*
 * Class:     apron_Dimchange
 * Method:    getDimension
 * Signature: ()Lapron/Dimension;
 */
JNIEXPORT jobject JNICALL Java_apron_Dimchange_getDimension
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,NULL);
  ap_dimchange_t* d = as_dimchange(o);
  return japron_dimension_get(env, d->intdim, d->realdim);
}

/*
 * Class:     apron_Dimchange
 * Method:    getContents
 * Signature: ()[I
 */
JNIEXPORT jintArray JNICALL Java_apron_Dimchange_getContents
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,NULL);
  ap_dimchange_t* x = as_dimchange(o);
  size_t i, nb = x->intdim + x->realdim;
  jintArray r = (*env)->NewIntArray(env, nb);
  if (!r) return NULL;
  jint* buf = (*env)->GetIntArrayElements(env, r, 0);
  for (i=0;i<nb;i++)
    buf[i] = x->dim[i];
  (*env)->ReleaseIntArrayElements(env, r, buf, 0);
  return r;
}

/*
 * Class:     apron_Dimchange
 * Method:    afterAdd
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_apron_Dimchange_afterAdd
  (JNIEnv *env, jobject o, jint i)
{
  check_nonnull(o,0);
  check_positive(i,0);
  ap_dimchange_t* d = as_dimchange(o);
  size_t k, dimsup = d->intdim+d->realdim;
  for (k=0; k<dimsup && (size_t)i>=d->dim[k]; k++);
  return i+k;

}

/*
 * Class:     apron_Dimchange
 * Method:    afterRemove
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_apron_Dimchange_afterRemove
  (JNIEnv *env, jobject o, jint i)
{
  check_nonnull(o,0);
  check_positive(i,0);
  ap_dimchange_t* d = as_dimchange(o);
  size_t k, dimrem = d->intdim+d->realdim;
  for (k=0;k<dimrem && (size_t)i>d->dim[k];k++);
  if (k<dimrem && (size_t)i==d->dim[k]) return -1;
  else return i-k;
}


/*
 * Class:     apron_Dimchange
 * Method:    addInvert
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Dimchange_addInvert
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,);
  ap_dimchange_add_invert(as_dimchange(o));
}

