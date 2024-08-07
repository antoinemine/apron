/*
 * gmp_Mpq.c
 *
 * glue for Mpq class
 *
 * APRON Library / Java GMP/MPFR binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include "jgmp.h"
#include "gmp_Mpq.h"

/*
 * Class:     gmp_Mpq
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_init
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,);
  mpq_ptr ptr = jgmp_make_mpq(env, o);
  if (ptr) mpq_init(ptr);
}

/*
 * Class:     gmp_Mpq
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_finalize
  (JNIEnv *env, jobject o)
{
  if (!o) return;
  mpq_ptr ptr = as_mpq(o);
  if (ptr) { mpq_clear(ptr); free(ptr); }
}

/*
 * Class:     gmp_Mpq
 * Method:    clean
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_clean
  (JNIEnv *env, jclass cls, jlong p)
{
  mpq_ptr ptr = (mpq_ptr)p;
  mpq_clear(ptr); free(ptr);
}

/*
 * Class:     gmp_Mpq
 * Method:    class_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_class_1init
  (JNIEnv *env, jclass cls)
{
  jgmp_cache(env);
}

/*
 * Class:     gmp_Mpq
 * Method:    canonicalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_canonicalize
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,);
  mpq_canonicalize(as_mpq(o));
}

/*
 * Class:     gmp_Mpq
 * Method:    set
 * Signature: (Lgmp/Mpq;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_set__Lgmp_Mpq_2
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  mpq_set(as_mpq(o1), as_mpq(o2));
}

/*
 * Class:     gmp_Mpq
 * Method:    set
 * Signature: (Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_set__Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  mpq_set_z(as_mpq(o1), as_mpz(o2));
}

/*
 * Class:     gmp_Mpq
 * Method:    set
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_set__Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  mpz_set(mpq_numref(as_mpq(o1)), as_mpz(o2));
  mpz_set(mpq_denref(as_mpq(o1)), as_mpz(o3));
}

/*
 * Class:     gmp_Mpq
 * Method:    set
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_set__II
  (JNIEnv *env, jobject o1, jint i, jint j)
{
  check_nonnull(o1,);
  mpz_set_si(mpq_numref(as_mpq(o1)), i);
  mpz_set_si(mpq_denref(as_mpq(o1)), j);
}

/*
 * Class:     gmp_Mpq
 * Method:    set
 * Signature: (D)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_set__D
  (JNIEnv *env, jobject o1, jdouble d)
{
  check_nonnull(o1,);
  mpq_set_d(as_mpq(o1),d);
}

/*
 * Class:     gmp_Mpq
 * Method:    set
 * Signature: (Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_set__Ljava_lang_String_2I
  (JNIEnv *env, jobject o, jstring v, jint base)
{
  check_nonnull(o,);
  check_nonnull(v,);
  const char *str;
  int r;
  mpq_ptr ptr = as_mpq(o);
  if (!ptr) return;
  str = (*env)->GetStringUTFChars(env, v, NULL);
  if (!str) return;
  r =  mpq_set_str(ptr, str, base);
  (*env)->ReleaseStringUTFChars(env, v, str);
  if (r) illegal_argument("String does not represent a number");
}

/*
 * Class:     gmp_Mpq
 * Method:    setNum
 * Signature: (Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_setNum__Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  mpz_set(mpq_numref(as_mpq(o1)), as_mpz(o2));
}

/*
 * Class:     gmp_Mpq
 * Method:    setNum
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_setNum__I
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,);
  mpz_set_si(mpq_numref(as_mpq(o1)), i);
}

/*
 * Class:     gmp_Mpq
 * Method:    setDen
 * Signature: (Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_setDen__Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  mpz_set(mpq_denref(as_mpq(o1)), as_mpz(o2));
}

/*
 * Class:     gmp_Mpq
 * Method:    setDen
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_setDen__I
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,);
  mpz_set_si(mpq_denref(as_mpq(o1)), i);
}

/*
 * Class:     gmp_Mpq
 * Method:    doubleValue
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_gmp_Mpq_doubleValue
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0.);
  return mpq_get_d(as_mpq(o));
}

/*
 * Class:     gmp_Mpq
 * Method:    toString
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_gmp_Mpq_toString
  (JNIEnv *env, jobject o, jint base)
{
  check_nonnull(o,NULL);
  char* res = mpq_get_str(NULL, base, as_mpq(o));
  if (!res) out_of_memory("failed to alloc string");
  jstring s = (*env)->NewStringUTF(env, res);
  free(res);
  return s;
}

/*
 * Class:     gmp_Mpq
 * Method:    getNum
 * Signature: ()Lgmp/Mpz;
 */
JNIEXPORT jobject JNICALL Java_gmp_Mpq_getNum
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,NULL);
  jobject r = jgmp_alloc_init_mpz(env);
  if (r) mpz_set(as_mpz(r), mpq_numref(as_mpq(o)));
  return r;
}

/*
 * Class:     gmp_Mpq
 * Method:    getDen
 * Signature: ()Lgmp/Mpz;
 */
JNIEXPORT jobject JNICALL Java_gmp_Mpq_getDen
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,NULL);
  jobject r = jgmp_alloc_init_mpz(env);
  if (r) mpz_set(as_mpz(r), mpq_denref(as_mpq(o)));
  return r;
}

/*
 * Class:     gmp_Mpq
 * Method:    numRef
 * Signature: ()Lgmp/MpzRef;
 */
JNIEXPORT jobject JNICALL Java_gmp_Mpq_numRef
  (JNIEnv *env, jobject o1)
{
  check_nonnull(o1,NULL);
  jobject o = (*env)->AllocObject(env, jgmp_mpzref);
  if (!o) return NULL;
  (*env)->SetLongField(env, o, jgmp_mpz_ptr, (jlong)mpq_numref(as_mpq(o1)));
  (*env)->SetObjectField(env, o, jgmp_mpzref_parent, o1);
   return o;
}

/*
 * Class:     gmp_Mpq
 * Method:    denRef
 * Signature: ()Lgmp/MpzRef;
 */
JNIEXPORT jobject JNICALL Java_gmp_Mpq_denRef
  (JNIEnv *env, jobject o1)
{
  check_nonnull(o1,NULL);
  jobject o = (*env)->AllocObject(env, jgmp_mpzref);
  if (!o) return NULL;
  (*env)->SetLongField(env, o, jgmp_mpz_ptr, (jlong)mpq_denref(as_mpq(o1)));
  (*env)->SetObjectField(env, o, jgmp_mpzref_parent, o1);
   return o;
}

/*
 * Class:     gmp_Mpq
 * Method:    add
 * Signature: (Lgmp/Mpq;Lgmp/Mpq;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_add
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  mpq_add(as_mpq(o1), as_mpq(o2), as_mpq(o3));
}

/*
 * Class:     gmp_Mpq
 * Method:    sub
 * Signature: (Lgmp/Mpq;Lgmp/Mpq;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_sub
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  mpq_sub(as_mpq(o1), as_mpq(o2), as_mpq(o3));
}

/*
 * Class:     gmp_Mpq
 * Method:    mul
 * Signature: (Lgmp/Mpq;Lgmp/Mpq;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_mul
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  mpq_mul(as_mpq(o1), as_mpq(o2), as_mpq(o3));
}

/*
 * Class:     gmp_Mpq
 * Method:    mul2exp
 * Signature: (Lgmp/Mpq;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_mul2exp
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  if (i>=0) mpq_mul_2exp(as_mpq(o1), as_mpq(o2), i);
  else mpq_div_2exp(as_mpq(o1), as_mpq(o2), -i);
}

/*
 * Class:     gmp_Mpq
 * Method:    div
 * Signature: (Lgmp/Mpq;Lgmp/Mpq;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_div
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  mpq_div(as_mpq(o1), as_mpq(o2), as_mpq(o3));
}

/*
 * Class:     gmp_Mpq
 * Method:    div2exp
 * Signature: (Lgmp/Mpq;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_div2exp
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  if (i>=0) mpq_div_2exp(as_mpq(o1), as_mpq(o2), i);
  else mpq_mul_2exp(as_mpq(o1), as_mpq(o2), -i);
}

/*
 * Class:     gmp_Mpq
 * Method:    neg
 * Signature: (Lgmp/Mpq;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_neg
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  mpq_neg(as_mpq(o1), as_mpq(o2));
}

/*
 * Class:     gmp_Mpq
 * Method:    abs
 * Signature: (Lgmp/Mpq;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_abs
   (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  mpq_abs(as_mpq(o1), as_mpq(o2));
}

/*
 * Class:     gmp_Mpq
 * Method:    inv
 * Signature: (Lgmp/Mpq;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpq_inv
   (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  mpq_inv(as_mpq(o1), as_mpq(o2));
}

/*
 * Class:     gmp_Mpq
 * Method:    size
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpq_size
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  return mpz_size(mpq_numref(as_mpq(o))) + mpz_size(mpq_denref(as_mpq(o)));
}

/*
 * Class:     gmp_Mpq
 * Method:    cmp
 * Signature: (Lgmp/Mpq;)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpq_cmp__Lgmp_Mpq_2
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpq_cmp(as_mpq(o1), as_mpq(o2));
}

/*
 * Class:     gmp_Mpq
 * Method:    cmp
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpq_cmp__II
  (JNIEnv *env, jobject o1, jint i, jint j)
{
  check_nonnull(o1,0);
  mpq_ptr x = as_mpq(o1);
  return mpq_cmp_si(x, i, j);
}

/*
 * Class:     gmp_Mpq
 * Method:    sgn
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpq_sgn
  (JNIEnv *env, jobject o1)
{
  check_nonnull(o1,0);
  mpq_ptr x = as_mpq(o1);
  return mpq_sgn(x);
}

/*
 * Class:     gmp_Mpq
 * Method:    equal
 * Signature: (Lgmp/Mpq;)Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpq_isEqual
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpq_equal(as_mpq(o1), as_mpq(o2)) != 0;
}

/*
 * Class:     gmp_Mpq
 * Method:    hashCode
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpq_hashCode
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  return jgmp_mpz_hash(mpq_numref(as_mpq(o))) ^ 
         jgmp_mpz_hash(mpq_denref(as_mpq(o)));
}
