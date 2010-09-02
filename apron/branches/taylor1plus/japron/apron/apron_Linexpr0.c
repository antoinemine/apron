/*
 * apron_Linexpr0.c
 *
 * glue for Linexpr0.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include "japron.h"
#include "apron_Linexpr0.h"

//////////////////////////////////////

/*
 * Class:     apron_Linexpr0
 * Method:    init
 * Signature: (ZI)V
 */
JNIEXPORT void JNICALL Java_apron_Linexpr0_init__ZI
  (JNIEnv *env, jobject o, jboolean sparse, jint size)
{
  check_nonnull(o,);
  check_positive(size,);
  set_linexpr0(o, 
               ap_linexpr0_alloc(sparse ? AP_LINEXPR_SPARSE : AP_LINEXPR_DENSE,
                                 size));
}

/*
 * Class:     apron_Linexpr0
 * Method:    init
 * Signature: (Lapron/Linexpr0;)V
 */
JNIEXPORT void JNICALL Java_apron_Linexpr0_init__Lapron_Linexpr0_2
  (JNIEnv *env, jobject o, jobject oo)
{
  check_nonnull(o,);
  check_nonnull(oo,);
  set_linexpr0(o, ap_linexpr0_copy(as_linexpr0(oo)));
}

/*
 * Class:     apron_Linexpr0
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Linexpr0_finalize
  (JNIEnv *env, jobject o)
{
  if (!o) return;
  ap_linexpr0_t* ptr = as_linexpr0(o);
  if (ptr) ap_linexpr0_free(ptr);
}

/*
 * Class:     apron_Linexpr0
 * Method:    class_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Linexpr0_class_1init
  (JNIEnv *env, jclass cls)
{
  japron_cache(env);
}

/*
 * Class:     apron_Linexpr0
 * Method:    resize
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_apron_Linexpr0_resize
  (JNIEnv *env, jobject o, jint size)
{
  check_nonnull(o,);
  check_positive(size,);
  ap_linexpr0_realloc(as_linexpr0(o), size);
}

/*
 * Class:     apron_Linexpr0
 * Method:    minimize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Linexpr0_minimize
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,);
  ap_linexpr0_minimize(as_linexpr0(o));
}

/*
 * Class:     apron_Linexpr0
 * Method:    setCoeff
 * Signature: (ILapron/Coeff;)V
 */
JNIEXPORT void JNICALL Java_apron_Linexpr0_setCoeff
  (JNIEnv *env, jobject o, jint dim, jobject c)
{
  check_nonnull(o,);
  check_nonnull(c,);
  if (dim<0) { out_of_bounds("invalid dimension"); return; }
  ap_coeff_t* ptr = ap_linexpr0_coeffref(as_linexpr0(o), dim);
  if (ptr) japron_coeff_set(env, ptr, c);
  else out_of_bounds("invalid dimension");
}

/*
 * Class:     apron_Linexpr0
 * Method:    setCst
 * Signature: (Lapron/Coeff;)V
 */
JNIEXPORT void JNICALL Java_apron_Linexpr0_setCst
  (JNIEnv *env, jobject o, jobject c)
{
  check_nonnull(o,);
  check_nonnull(c,);
  japron_coeff_set(env, ap_linexpr0_cstref(as_linexpr0(o)), c);
}

/*
 * Class:     apron_Linexpr0
 * Method:    getCoeff
 * Signature: (I)Lapron/Coeff;
 */
JNIEXPORT jobject JNICALL Java_apron_Linexpr0_getCoeff
  (JNIEnv *env, jobject o, jint dim)
{
  check_nonnull(o,NULL);
  if (dim<0) { out_of_bounds("invalid dimension"); return NULL; }
  ap_coeff_t* ptr = ap_linexpr0_coeffref(as_linexpr0(o), dim);
  if (!ptr) { out_of_bounds("invalid dimension"); return NULL; }
  return japron_coeff_get(env, ptr);
}

/*
 * Class:     apron_Linexpr0
 * Method:    getLinterms
 * Signature: ()[Lapron/Linterm0;
 */
JNIEXPORT jobjectArray JNICALL Java_apron_Linexpr0_getLinterms
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,NULL);
  jobjectArray ar;
  size_t i,nb,pos;
  ap_dim_t dim;
  ap_coeff_t* coeff;
  ap_linexpr0_t* e = as_linexpr0(o);
  nb = 0;
  ap_linexpr0_ForeachLinterm(e,i,dim,coeff) { nb++; }
  ar = (*env)->NewObjectArray(env, nb, japron_linterm0, NULL);  
  if (!ar) return NULL;
  pos = 0;
  ap_linexpr0_ForeachLinterm(e,i,dim,coeff) {
    jobject t = (*env)->AllocObject(env, japron_linterm0);
    if (!t) return NULL;
    jobject r = japron_coeff_get(env, coeff);
    if (!r) return NULL;
    (*env)->SetIntField(env, t, japron_linterm0_dim, dim);
    (*env)->SetObjectField(env, t, japron_linterm0_coeff, r);
    (*env)->SetObjectArrayElement(env, ar, pos, t);
    pos++;
  }
  return ar;
}

/*
 * Class:     apron_Linexpr0
 * Method:    getCst
 * Signature: ()Lapron/Coeff;
 */
JNIEXPORT jobject JNICALL Java_apron_Linexpr0_getCst
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  return japron_coeff_get(env, ap_linexpr0_cstref(as_linexpr0(o)));
}

/*
 * Class:     apron_Linexpr0
 * Method:    size
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_apron_Linexpr0_getSize
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  return ap_linexpr0_size(as_linexpr0(o));
}

/*
 * Class:     apron_Linexpr0
 * Method:    maxDim
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_apron_Linexpr0_maxDim
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  return japron_linexpr0_max_dim(as_linexpr0(o));
}


/*
 * Class:     apron_Linexpr0
 * Method:    isSparse
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Linexpr0_isSparse
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  return as_linexpr0(o)->discr == AP_LINEXPR_SPARSE;
}

/*
 * Class:     apron_Linexpr0
 * Method:    hashCode
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_apron_Linexpr0_hashCode
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  return ap_linexpr0_hash(as_linexpr0(o));
}

/*
 * Class:     apron_Linexpr0
 * Method:    isInteger
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Linexpr0_isInteger
  (JNIEnv *env, jobject o, jint i)
{
  check_nonnull(o,0);
 return ap_linexpr0_is_integer(as_linexpr0(o), i);
}

/*
 * Class:     apron_Linexpr0
 * Method:    isReal
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Linexpr0_isReal
  (JNIEnv *env, jobject o, jint i)
{
  check_nonnull(o,0);
  return ap_linexpr0_is_real(as_linexpr0(o), i);
}

/*
 * Class:     apron_Linexpr0
 * Method:    isLinear
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Linexpr0_isLinear
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  return ap_linexpr0_is_linear(as_linexpr0(o));
}

/*
 * Class:     apron_Linexpr0
 * Method:    isQuasilinear
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Linexpr0_isQuasilinear
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  return ap_linexpr0_is_quasilinear(as_linexpr0(o));
}

/*
 * Class:     apron_Linexpr0
 * Method:    isEqual
 * Signature: (Lapron/Linexpr0;)Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Linexpr0_isEqual
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return ap_linexpr0_equal(as_linexpr0(o1), as_linexpr0(o2));
}

/*
 * Class:     apron_Linexpr0
 * Method:    cmp
 * Signature: (Lapron/Linexpr0;)I
 */
JNIEXPORT jint JNICALL Java_apron_Linexpr0_cmp
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return ap_linexpr0_compare(as_linexpr0(o1), as_linexpr0(o2));
}

/*
 * Class:     apron_Linexpr0
 * Method:    addDimensions
 * Signature: (Lapron/Dimchange;)V
 */
JNIEXPORT void JNICALL Java_apron_Linexpr0_addDimensions
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  ap_linexpr0_add_dimensions_with(as_linexpr0(o1), as_dimchange(o2));
}

/*
 * Class:     apron_Linexpr0
 * Method:    permuteDimensions
 * Signature: (Lapron/Dimperm;)V
 */
JNIEXPORT void JNICALL Java_apron_Linexpr0_permuteDimensions
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  ap_linexpr0_t* l = as_linexpr0(o1);
  ap_dimperm_t* p = as_dimperm(o2);
  if (p->size < japron_linexpr0_max_dim(l)) 
    { illegal_argument("permutation is too short"); return; }
  ap_linexpr0_permute_dimensions_with(l, p);
}
