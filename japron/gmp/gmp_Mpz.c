/*
 * gmp_Mpz.c
 *
 * glue for Mpz class
 *
 * APRON Library / Java GMP/MPFR binding
 *
 * Copyright (C) Antoine Mine' 2010
 */
#include "jgmp.h"
#include "gmp_Mpz.h"

/*
 * Class:     gmp_Mpz
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_init
  (JNIEnv *env, jobject o)
{ 
  check_nonnull(o,);
  mpz_ptr ptr = jgmp_make_mpz(env, o);
  if (ptr) mpz_init(ptr);
}

/*
 * Class:     gmp_Mpz
 * Method:    init_set
 * Signature: (Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_init_1set__Lgmp_Mpz_2
  (JNIEnv *env, jobject o, jobject v)
{
  check_nonnull(o,);
  check_nonnull(v,);
  mpz_ptr ptr = jgmp_make_mpz(env, o);
  if (ptr) mpz_init_set(ptr, as_mpz(v));
}

/*
 * Class:     gmp_Mpz
 * Method:    init_set
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_init_1set__I
  (JNIEnv *env, jobject o, jint v)
{
  check_nonnull(o,);
  mpz_ptr ptr = jgmp_make_mpz(env, o);
  if (ptr) mpz_init_set_si(ptr, v);
}

/*
 * Class:     gmp_Mpz
 * Method:    init_set
 * Signature: (D)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_init_1set__D
  (JNIEnv *env, jobject o, jdouble v)
{
  check_nonnull(o,);
  mpz_ptr ptr = jgmp_make_mpz(env, o);
  if (ptr) mpz_init_set_d(ptr, v);
}

/*
 * Class:     gmp_Mpz
 * Method:    init_set
 * Signature: (Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_init_1set__Ljava_lang_String_2I
  (JNIEnv *env, jobject o, jstring v, jint base)
{
  check_nonnull(o,);
  check_nonnull(v,);
  const char *str;
  int r;
  mpz_ptr ptr = jgmp_make_mpz(env, o);
  if (!ptr) return;
  str = (*env)->GetStringUTFChars(env, v, NULL);
  if (!str) return;
  r =  mpz_init_set_str(ptr, str, base);
  (*env)->ReleaseStringUTFChars(env, v, str);
  if (r) illegal_argument("String does not represent a number");
}

/*
 * Class:     gmp_Mpz
 * Method:    set
 * Signature: (I[B)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_set__I_3B
  (JNIEnv *env, jobject o, jint sign, jbyteArray bytes)
{
  check_nonnull(o,);
  check_nonnull(bytes,);
  mpz_ptr ptr = as_mpz(o);
  jbyte* buf = (*env)->GetByteArrayElements(env, bytes, 0);
  if (!buf) return;
  mpz_import(ptr, (*env)->GetArrayLength(env, bytes), 1, 1, 0, 0, buf);
  if (sign<0) mpz_neg(ptr, ptr);
  (*env)->ReleaseByteArrayElements(env, bytes, buf, 0);
}

/*
 * Class:     gmp_Mpz
 * Method:    byteArrayValue
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_gmp_Mpz_byteArrayValue
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,NULL);
  mpz_ptr ptr = as_mpz(o);
  int size = (mpz_sizeinbase( ptr, 2) + 7) / 8;
  jbyteArray bytes = (*env)->NewByteArray(env, size);
  if (!bytes) return NULL;
  jbyte* buf = (*env)->GetByteArrayElements(env, bytes, 0);
  mpz_export(buf, NULL, 1, 1, 0, 0, ptr);
  (*env)->ReleaseByteArrayElements(env, bytes, buf, 0);
  return bytes;
}

/*
 * Class:     gmp_Mpz
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_finalize
  (JNIEnv *env, jobject o)
{
  if (!o) return;
  mpz_ptr ptr = as_mpz(o);
  if (ptr) { mpz_clear(ptr); free(ptr); }
}

/*
 * Class:     gmp_Mpz
 * Method:    clean
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_clean
  (JNIEnv *env, jclass cls, jlong p)
{
  mpz_ptr ptr = (mpz_ptr)p;
  mpz_clear(ptr); free(ptr);
}

/*
 * Class:     gmp_Mpz
 * Method:    class_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_class_1init
  (JNIEnv *env, jclass cls)
{
  jgmp_cache(env);
}

/*
 * Class:     gmp_Mpz
 * Method:    set
 * Signature: (Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_set__Lgmp_Mpz_2
  (JNIEnv *env, jobject o, jobject v)
{
  check_nonnull(o,);
  check_nonnull(v,);
  mpz_set(as_mpz(o), as_mpz(v));
}

/*
 * Class:     gmp_Mpz
 * Method:    set
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_set__I
  (JNIEnv *env, jobject o, jint v)
{
  check_nonnull(o,);
  mpz_set_si(as_mpz(o), v);
}

/*
 * Class:     gmp_Mpz
 * Method:    set
 * Signature: (D)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_set__D
  (JNIEnv *env, jobject o, jdouble v)
{
  check_nonnull(o,);
  mpz_set_d(as_mpz(o),v);
}

/*
 * Class:     gmp_Mpz
 * Method:    set
 * Signature: (Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_set__Ljava_lang_String_2I
  (JNIEnv *env, jobject o, jstring v, jint base)
{
  check_nonnull(o,);
  check_nonnull(v,);
  const char *str;
  int r;
  mpz_ptr ptr = as_mpz(o);
  if (!ptr) return;
  str = (*env)->GetStringUTFChars(env, v, NULL);
  if (!str) return;
  r =  mpz_set_str(ptr, str, base);
  (*env)->ReleaseStringUTFChars(env, v, str);
  if (r) illegal_argument("String does not represent a number");
}

/*
 * Class:     gmp_Mpz
 * Method:    intValue
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_intValue
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  return mpz_get_si(as_mpz(o));
}

/*
 * Class:     gmp_Mpz
 * Method:    doubleValue
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_gmp_Mpz_doubleValue
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0.);
  return mpz_get_d(as_mpz(o));
}

/*
 * Class:     gmp_Mpz
 * Method:    doubleExponentValue
 * Signature: ([I)D
 */
JNIEXPORT jdouble JNICALL Java_gmp_Mpz_doubleExponentValue
  (JNIEnv * env, jobject o, jintArray ar)
{
  check_nonnull(o,0.);
  long exp;
  double r = mpz_get_d_2exp(&exp, as_mpz(o));
  jint e = exp;
  if (ar) (*env)->SetIntArrayRegion(env, ar, 0, 1, &e);
  return r;
}

/*
 * Class:     gmp_Mpz
 * Method:    toString
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_gmp_Mpz_toString
  (JNIEnv *env, jobject o, jint base)
{
  check_nonnull(o,NULL);
  char* res = mpz_get_str(NULL,base,as_mpz(o));
  if (!res) out_of_memory("failed to alloc string");
  jstring s = (*env)->NewStringUTF(env, res);
  free(res);
  return s;
}


/*
 * Class:     gmp_Mpz
 * Method:    add
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_add__Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  mpz_add(as_mpz(o1),as_mpz(o2),as_mpz(o3));
}

/*
 * Class:     gmp_Mpz
 * Method:    add
 * Signature: (Lgmp/Mpz;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_add__Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  if (i>=0) mpz_add_ui(as_mpz(o1),as_mpz(o2),i);
  else mpz_sub_ui(as_mpz(o1),as_mpz(o2),-i);
}

/*
 * Class:     gmp_Mpz
 * Method:    sub
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_sub__Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  mpz_sub(as_mpz(o1),as_mpz(o2),as_mpz(o3));
}

/*
 * Class:     gmp_Mpz
 * Method:    sub
 * Signature: (Lgmp/Mpz;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_sub__Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  if (i>=0) mpz_sub_ui(as_mpz(o1),as_mpz(o2),i);
  else mpz_add_ui(as_mpz(o1),as_mpz(o2),-i);
}

/*
 * Class:     gmp_Mpz
 * Method:    sub
 * Signature: (ILgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_sub__ILgmp_Mpz_2
  (JNIEnv *env, jobject o1, jint i, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  if (i>=0) mpz_ui_sub(as_mpz(o1),i,as_mpz(o2));
  else { mpz_add_ui(as_mpz(o1),as_mpz(o2),-i); mpz_neg(as_mpz(o1),as_mpz(o1)); }
}

/*
 * Class:     gmp_Mpz
 * Method:    mul
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_mul__Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  mpz_mul(as_mpz(o1),as_mpz(o2),as_mpz(o3));
}

/*
 * Class:     gmp_Mpz
 * Method:    mul
 * Signature: (Lgmp/Mpz;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_mul__Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  mpz_mul_si(as_mpz(o1),as_mpz(o2),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    addMul
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_addMul__Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  mpz_addmul(as_mpz(o1),as_mpz(o2),as_mpz(o3));
}

/*
 * Class:     gmp_Mpz
 * Method:    addMul
 * Signature: (Lgmp/Mpz;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_addMul__Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  if (i>=0) mpz_addmul_ui(as_mpz(o1),as_mpz(o2),i);
  else mpz_submul_ui(as_mpz(o1),as_mpz(o2),-i);
}

/*
 * Class:     gmp_Mpz
 * Method:    subMul
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_subMul__Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  mpz_submul(as_mpz(o1),as_mpz(o2),as_mpz(o3));
}

/*
 * Class:     gmp_Mpz
 * Method:    subMul
 * Signature: (Lgmp/Mpz;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_subMul__Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  if (i>=0) mpz_submul_ui(as_mpz(o1),as_mpz(o2),i);
  else mpz_addmul_ui(as_mpz(o1),as_mpz(o2),-i);
}

/*
 * Class:     gmp_Mpz
 * Method:    mul2exp
 * Signature: (Lgmp/Mpz;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_mul2exp
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_positive(i,);
  mpz_mul_2exp(as_mpz(o1),as_mpz(o2),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    neg
 * Signature: (Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_neg
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  mpz_neg(as_mpz(o1),as_mpz(o2));
}

/*
 * Class:     gmp_Mpz
 * Method:    abs
 * Signature: (Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_abs
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  mpz_abs(as_mpz(o1),as_mpz(o2));
}


/*
 * Class:     gmp_Mpz
 * Method:    cdivQ
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_cdivQ__Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  check_nonzero_mpz(as_mpz(o3),);
  mpz_cdiv_q(as_mpz(o1),as_mpz(o2),as_mpz(o3));
}

/*
 * Class:     gmp_Mpz
 * Method:    cdivR
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_cdivR__Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  check_nonzero_mpz(as_mpz(o3),);
  mpz_cdiv_r(as_mpz(o1),as_mpz(o2),as_mpz(o3));
}

/*
 * Class:     gmp_Mpz
 * Method:    cdivQR
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_cdivQR__Lgmp_Mpz_2Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jobject o4)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  check_nonnull(o4,);
  check_nonzero_mpz(as_mpz(o4),);
  mpz_cdiv_qr(as_mpz(o1),as_mpz(o2),as_mpz(o3),as_mpz(o4));
}

/*
 * Class:     gmp_Mpz
 * Method:    cdivQ
 * Signature: (Lgmp/Mpz;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_cdivQ__Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonzero(i,0);
  check_positive(i,0);
  return mpz_cdiv_q_ui(as_mpz(o1),as_mpz(o2),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    cdivR
 * Signature: (Lgmp/Mpz;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_cdivR__Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonzero(i,0);
  check_positive(i,0);
  return mpz_cdiv_r_ui(as_mpz(o1),as_mpz(o2),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    cdivQR
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_cdivQR__Lgmp_Mpz_2Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint i)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  check_nonzero(i,0);
  check_positive(i,0);
  return mpz_cdiv_qr_ui(as_mpz(o1),as_mpz(o2),as_mpz(o3),i);
}


/*
 * Class:     gmp_Mpz
 * Method:    cdivR
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_cdivR
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,0);
  check_nonzero(i,0);
  check_positive(i,0);
  return mpz_cdiv_ui(as_mpz(o1),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    cdivQ2exp
 * Signature: (Lgmp/Mpz;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_cdivQ2exp
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_positive(i,);
  mpz_cdiv_q_2exp(as_mpz(o1),as_mpz(o2),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    cdivR2exp
 * Signature: (Lgmp/Mpz;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_cdivR2exp
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_positive(i,);
  mpz_cdiv_r_2exp(as_mpz(o1),as_mpz(o2),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    fdivQ
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_fdivQ__Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  check_nonzero_mpz(as_mpz(o3),);
  mpz_fdiv_q(as_mpz(o1),as_mpz(o2),as_mpz(o3));
}

/*
 * Class:     gmp_Mpz
 * Method:    fdivR
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_fdivR__Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  check_nonzero_mpz(as_mpz(o3),);
  mpz_fdiv_r(as_mpz(o1),as_mpz(o2),as_mpz(o3));
}

/*
 * Class:     gmp_Mpz
 * Method:    fdivQR
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_fdivQR__Lgmp_Mpz_2Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jobject o4)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  check_nonnull(o4,);
  check_nonzero_mpz(as_mpz(o4),);
  mpz_fdiv_qr(as_mpz(o1),as_mpz(o2),as_mpz(o3),as_mpz(o4));
}

/*
 * Class:     gmp_Mpz
 * Method:    fdivQ
 * Signature: (Lgmp/Mpz;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_fdivQ__Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonzero(i,0);
  check_positive(i,0);
  return mpz_fdiv_q_ui(as_mpz(o1),as_mpz(o2),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    fdivR
 * Signature: (Lgmp/Mpz;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_fdivR__Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonzero(i,0);
  check_positive(i,0);
  return mpz_fdiv_r_ui(as_mpz(o1),as_mpz(o2),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    fdivQR
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_fdivQR__Lgmp_Mpz_2Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint i)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  check_nonzero(i,0);
  check_positive(i,0);
  return mpz_fdiv_qr_ui(as_mpz(o1),as_mpz(o2),as_mpz(o3),i);
}


/*
 * Class:     gmp_Mpz
 * Method:    fdivR
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_fdivR
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,0);
  check_nonzero(i,0);
  check_positive(i,0);
  return mpz_fdiv_ui(as_mpz(o1),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    fdivQ2exp
 * Signature: (Lgmp/Mpz;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_fdivQ2exp
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_positive(i,);
  mpz_fdiv_q_2exp(as_mpz(o1),as_mpz(o2),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    fdivR2exp
 * Signature: (Lgmp/Mpz;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_fdivR2exp
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_positive(i,);
  mpz_fdiv_r_2exp(as_mpz(o1),as_mpz(o2),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    tdivQ
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_tdivQ__Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  check_nonzero_mpz(as_mpz(o3),);
  mpz_tdiv_q(as_mpz(o1),as_mpz(o2),as_mpz(o3));
}

/*
 * Class:     gmp_Mpz
 * Method:    tdivR
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_tdivR__Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  check_nonzero_mpz(as_mpz(o3),);
  mpz_tdiv_r(as_mpz(o1),as_mpz(o2),as_mpz(o3));
}

/*
 * Class:     gmp_Mpz
 * Method:    tdivQR
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_tdivQR__Lgmp_Mpz_2Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jobject o4)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  check_nonnull(o4,);
  check_nonzero_mpz(as_mpz(o4),);
  mpz_tdiv_qr(as_mpz(o1),as_mpz(o2),as_mpz(o3),as_mpz(o4));
}

/*
 * Class:     gmp_Mpz
 * Method:    tdivQ
 * Signature: (Lgmp/Mpz;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_tdivQ__Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonzero(i,0);
  check_positive(i,0);
  return mpz_tdiv_q_ui(as_mpz(o1),as_mpz(o2),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    tdivR
 * Signature: (Lgmp/Mpz;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_tdivR__Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonzero(i,0);
  check_positive(i,0);
  return mpz_tdiv_r_ui(as_mpz(o1),as_mpz(o2),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    tdivQR
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_tdivQR__Lgmp_Mpz_2Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint i)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  check_nonzero(i,0);
  check_positive(i,0);
  return mpz_tdiv_qr_ui(as_mpz(o1),as_mpz(o2),as_mpz(o3),i);
}


/*
 * Class:     gmp_Mpz
 * Method:    tdivR
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_tdivR
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,0);
  check_nonzero(i,0);
  check_positive(i,0);
  return mpz_tdiv_ui(as_mpz(o1),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    tdivQ2exp
 * Signature: (Lgmp/Mpz;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_tdivQ2exp
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_positive(i,);
  mpz_tdiv_q_2exp(as_mpz(o1),as_mpz(o2),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    tdivR2exp
 * Signature: (Lgmp/Mpz;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_tdivR2exp
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_positive(i,);
  mpz_tdiv_r_2exp(as_mpz(o1),as_mpz(o2),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    mod
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_mod__Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  check_nonzero_mpz(as_mpz(o3),);
  mpz_mod(as_mpz(o1),as_mpz(o2),as_mpz(o3));
}

/*
 * Class:     gmp_Mpz
 * Method:    mod
 * Signature: (Lgmp/Mpz;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_mod__Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonzero(i,0);
  return mpz_mod_ui(as_mpz(o1),as_mpz(o2),labs(i));
}

/*
 * Class:     gmp_Mpz
 * Method:    divExact
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_divExact__Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  check_nonzero_mpz(as_mpz(o3),);
  mpz_divexact(as_mpz(o1),as_mpz(o2),as_mpz(o3));
}

/*
 * Class:     gmp_Mpz
 * Method:    divExact
 * Signature: (Lgmp/Mpz;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_divExact__Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonzero(i,);
  check_positive(i,);
  mpz_divexact_ui(as_mpz(o1),as_mpz(o2),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    divisible
 * Signature: (Lgmp/Mpz;)Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpz_isDivisible__Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpz_divisible_p(as_mpz(o1),as_mpz(o2)) != 0;
}

/*
 * Class:     gmp_Mpz
 * Method:    divisible
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpz_isDivisible__I
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,0);
  check_positive(i,0);
  return mpz_divisible_ui_p(as_mpz(o1),i) != 0;
}

/*
 * Class:     gmp_Mpz
 * Method:    divisible2exp
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpz_isDivisible2exp
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,0);
  check_positive(i,0);
  return mpz_divisible_2exp_p(as_mpz(o1),i) != 0;
}

/*
 * Class:     gmp_Mpz
 * Method:    congruent
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpz_isCongruent__Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpz_congruent_p(as_mpz(o1),as_mpz(o2),as_mpz(o3)) != 0;
}

/*
 * Class:     gmp_Mpz
 * Method:    congruent
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpz_isCongruent__II
  (JNIEnv *env, jobject o1, jint i, jint j)
{
  check_nonnull(o1,0);
  check_positive(i,0);
  check_positive(j,0);
  return mpz_congruent_ui_p(as_mpz(o1),i,j) != 0;
}

/*
 * Class:     gmp_Mpz
 * Method:    congruent2exp
 * Signature: (Lgmp/Mpz;I)Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpz_isCongruent2exp
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_positive(i,0);
  return mpz_congruent_2exp_p(as_mpz(o1),as_mpz(o2),i) != 0;
}

/*
 * Class:     gmp_Mpz
 * Method:    pow
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_pow__Lgmp_Mpz_2Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jobject o4)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  check_nonnull(o4,);
  if (mpz_sgn(as_mpz(o3))<0 && !jgmp_mpz_invertible_p(as_mpz(o2),as_mpz(o4))) 
    arithmetic_exception("non-invertible base");
  else
    mpz_powm(as_mpz(o1),as_mpz(o2),as_mpz(o3),as_mpz(o4));
}

/*
 * Class:     gmp_Mpz
 * Method:    pow
 * Signature: (Lgmp/Mpz;ILgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_pow__Lgmp_Mpz_2ILgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jint i, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_positive(i,);
  mpz_powm_ui(as_mpz(o1),as_mpz(o2),i,as_mpz(o3));
}

/*
 * Class:     gmp_Mpz
 * Method:    pow
 * Signature: (Lgmp/Mpz;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_pow__Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_positive(i,);
  mpz_pow_ui(as_mpz(o1),as_mpz(o2),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    pow
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_pow__II
  (JNIEnv *env, jobject o1, jint i, jint j)
{
  check_nonnull(o1,);
  check_positive(i,);
  check_positive(j,);
  mpz_ui_pow_ui(as_mpz(o1),i,j);
}

/*
 * Class:     gmp_Mpz
 * Method:    root
 * Signature: (Lgmp/Mpz;I)Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpz_root__Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_positive(i,0);
  return mpz_root(as_mpz(o1),as_mpz(o2),i) != 0;
}

/*
 * Class:     gmp_Mpz
 * Method:    root
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_root__Lgmp_Mpz_2Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint i)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  check_positive(i,);
  mpz_rootrem(as_mpz(o1),as_mpz(o2),as_mpz(o3),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    sqrt
 * Signature: (Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_sqrt
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  mpz_sqrt(as_mpz(o1),as_mpz(o2));
}

/*
 * Class:     gmp_Mpz
 * Method:    sqrt
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_sqrt__Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  mpz_sqrtrem(as_mpz(o1),as_mpz(o2),as_mpz(o3));
}

/*
 * Class:     gmp_Mpz
 * Method:    perfectPower
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpz_isPerfectPower
  (JNIEnv *env, jobject o1)
{
  check_nonnull(o1,0);
  return mpz_perfect_power_p(as_mpz(o1)) != 0;
}

/*
 * Class:     gmp_Mpz
 * Method:    perfectSquare
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpz_isPerfectSquare
  (JNIEnv *env, jobject o1)
{
  check_nonnull(o1,0);
  return mpz_perfect_square_p(as_mpz(o1)) != 0;
}

/*
 * Class:     gmp_Mpz
 * Method:    probabPrime
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_isProbabPrime
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,0);
  check_positive(i,0);
  return mpz_probab_prime_p(as_mpz(o1),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    nextPrime
 * Signature: (Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_nextPrime
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  mpz_nextprime(as_mpz(o1),as_mpz(o2));
}

/*
 * Class:     gmp_Mpz
 * Method:    gcd
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_gcd__Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  mpz_gcd(as_mpz(o1),as_mpz(o2),as_mpz(o3));
}

/*
 * Class:     gmp_Mpz
 * Method:    gcd
 * Signature: (Lgmp/Mpz;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_gcd__Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_positive(i,0);
  return mpz_gcd_ui(as_mpz(o1),as_mpz(o2),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    gcd
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_gcd__Lgmp_Mpz_2Lgmp_Mpz_2Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jobject o4, jobject o5)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  check_nonnull(o4,);
  check_nonnull(o5,);
  mpz_gcdext(as_mpz(o1),as_mpz(o2),as_mpz(o3),as_mpz(o4),as_mpz(o5));
}

/*
 * Class:     gmp_Mpz
 * Method:    lcm
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_lcm__Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  mpz_lcm(as_mpz(o1),as_mpz(o2),as_mpz(o3));
}

/*
 * Class:     gmp_Mpz
 * Method:    lcm
 * Signature: (Lgmp/Mpz;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_lcm__Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_positive(i,);
  mpz_lcm_ui(as_mpz(o1),as_mpz(o2),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    invert
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpz_invert
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpz_invert(as_mpz(o1),as_mpz(o2),as_mpz(o3)) != 0;
}

/*
 * Class:     gmp_Mpz
 * Method:    jacobi
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_jacobi
  (JNIEnv *env, jclass cls, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpz_jacobi(as_mpz(o1),as_mpz(o2));
}

/*
 * Class:     gmp_Mpz
 * Method:    legendre
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_legendre
  (JNIEnv *env, jclass cls, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpz_legendre(as_mpz(o1),as_mpz(o2));
}

/*
 * Class:     gmp_Mpz
 * Method:    kronecker
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_kronecker__Lgmp_Mpz_2Lgmp_Mpz_2
  (JNIEnv *env, jclass cls, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpz_kronecker(as_mpz(o1),as_mpz(o2));
}

/*
 * Class:     gmp_Mpz
 * Method:    kronecker
 * Signature: (Lgmp/Mpz;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_kronecker__Lgmp_Mpz_2I
  (JNIEnv *env, jclass cls, jobject o1, jint i)
{
  check_nonnull(o1,0);
  return mpz_kronecker_si(as_mpz(o1),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    kronecker
 * Signature: (ILgmp/Mpz;)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_kronecker__ILgmp_Mpz_2
  (JNIEnv *env, jclass cls, jint i, jobject o1)
{
  check_nonnull(o1,0);
  return mpz_si_kronecker(i,as_mpz(o1));
}

/*
 * Class:     gmp_Mpz
 * Method:    remove
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_remove
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpz_remove(as_mpz(o1),as_mpz(o2),as_mpz(o3));
}

/*
 * Class:     gmp_Mpz
 * Method:    fac
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_fac
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,);
  check_positive(i,);
  mpz_fac_ui(as_mpz(o1),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    bin
 * Signature: (Lgmp/Mpz;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_bin__Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_positive(i,);
  mpz_bin_ui(as_mpz(o1),as_mpz(o2),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    bin
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_bin__II
  (JNIEnv *env, jobject o1, jint i, jint j)
{
  check_nonnull(o1,);
  check_positive(i,);
  check_positive(j,);
  mpz_bin_uiui(as_mpz(o1),i,j);
}

/*
 * Class:     gmp_Mpz
 * Method:    fib
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_fib__I
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,);
  check_positive(i,);
  mpz_fib_ui(as_mpz(o1),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    fib
 * Signature: (Lgmp/Mpz;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_fib__Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_positive(i,);
  mpz_fib2_ui(as_mpz(o1),as_mpz(o2),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    lucnum
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_lucnum__I
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,);
  check_positive(i,);
  mpz_lucnum_ui(as_mpz(o1),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    lucnum
 * Signature: (Lgmp/Mpz;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_lucnum__Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jint i)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_positive(i,);
  mpz_lucnum2_ui(as_mpz(o1),as_mpz(o2),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    cmp
 * Signature: (Lgmp/Mpz;)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_cmp__Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpz_cmp(as_mpz(o1),as_mpz(o2));
}

/*
 * Class:     gmp_Mpz
 * Method:    cmp
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_cmp__I
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,0);
  mpz_ptr p = as_mpz(o1);
  return mpz_cmp_si(p,i);
}

/*
 * Class:     gmp_Mpz
 * Method:    cmp
 * Signature: (D)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_cmp__D
  (JNIEnv *env, jobject o1, jdouble d)
{
  check_nonnull(o1,0);
  return mpz_cmp_d(as_mpz(o1),d);
}

/*
 * Class:     gmp_Mpz
 * Method:    cmpAbs
 * Signature: (Lgmp/Mpz;)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_cmpAbs__Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpz_cmpabs(as_mpz(o1),as_mpz(o2));
}

/*
 * Class:     gmp_Mpz
 * Method:    cmpAbs
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_cmpAbs__I
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,0);
  return mpz_cmpabs_ui(as_mpz(o1),labs(i));
}

/*
 * Class:     gmp_Mpz
 * Method:    cmpAbs
 * Signature: (D)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_cmpAbs__D
  (JNIEnv *env, jobject o1, jdouble d)
{
  check_nonnull(o1,0);
  return mpz_cmpabs_d(as_mpz(o1),d);
}

/*
 * Class:     gmp_Mpz
 * Method:    sgn
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_sgn
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  mpz_ptr p = as_mpz(o);
  return mpz_sgn(p);
}

/*
 * Class:     gmp_Mpz
 * Method:    hashCode
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_hashCode
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  return jgmp_mpz_hash(as_mpz(o));
}

/*
 * Class:     gmp_Mpz
 * Method:    and
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_and
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  mpz_and(as_mpz(o1),as_mpz(o2),as_mpz(o3));
}

/*
 * Class:     gmp_Mpz
 * Method:    or
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_or
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  mpz_ior(as_mpz(o1),as_mpz(o2),as_mpz(o3));
}

/*
 * Class:     gmp_Mpz
 * Method:    xor
 * Signature: (Lgmp/Mpz;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_xor
  (JNIEnv *env, jobject o1, jobject o2, jobject o3)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  check_nonnull(o3,);
  mpz_xor(as_mpz(o1),as_mpz(o2),as_mpz(o3));
}

/*
 * Class:     gmp_Mpz
 * Method:    not
 * Signature: (Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_not
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  mpz_com(as_mpz(o1),as_mpz(o2));
}

/*
 * Class:     gmp_Mpz
 * Method:    popCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_popCount
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  unsigned long r = mpz_popcount(as_mpz(o));
  return (r==ULONG_MAX) ? gmp_Mpz_infinite : (jint)r;
}

/*
 * Class:     gmp_Mpz
 * Method:    hamDist
 * Signature: (Lgmp/Mpz;)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_hamDist
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  unsigned long r = mpz_hamdist(as_mpz(o1),as_mpz(o2));
  return (r==ULONG_MAX) ? gmp_Mpz_infinite : (jint)r;
}

/*
 * Class:     gmp_Mpz
 * Method:    scan0
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_scan0
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,0);
  check_positive(i,gmp_Mpz_infinite);
  unsigned long r = mpz_scan0(as_mpz(o1),i);
  return (r==ULONG_MAX) ? gmp_Mpz_infinite : (jint)r;
}

/*
 * Class:     gmp_Mpz
 * Method:    scan1
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_scan1
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,0);
  check_positive(i,gmp_Mpz_infinite);
  unsigned long r = mpz_scan1(as_mpz(o1),i);
  return (r==ULONG_MAX) ? gmp_Mpz_infinite : (jint)r;
}

/*
 * Class:     gmp_Mpz
 * Method:    setBit
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_setBit
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,);
  check_positive(i,);
  mpz_setbit(as_mpz(o1),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    clrBit
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_clrBit
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,);
  check_positive(i,);
  mpz_clrbit(as_mpz(o1),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    notBit
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_notBit
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,);
  check_positive(i,);
  mpz_combit(as_mpz(o1),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    tstBit
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpz_tstBit
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,0);
  check_positive(i,0);
  return mpz_tstbit(as_mpz(o1),i);
}

/*
 * Class:     gmp_Mpz
 * Method:    fitsInt
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpz_fitsInt
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  return mpz_fits_sint_p(as_mpz(o)) != 0;
}

/*
 * Class:     gmp_Mpz
 * Method:    odd
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpz_isOdd
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  return mpz_odd_p(as_mpz(o)) != 0;
}

/*
 * Class:     gmp_Mpz
 * Method:    even
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpz_isEven
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  mpz_ptr p = as_mpz(o);
  return mpz_even_p(p) != 0;
}

/*
 * Class:     gmp_Mpz
 * Method:    sizeInBase
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_sizeInBase
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,0);
  mpz_ptr p = as_mpz(o1);
  return mpz_sizeinbase(p,i);
}

/*
 * Class:     gmp_Mpz
 * Method:    size
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpz_size
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  return mpz_size(as_mpz(o));
}

/*
 * Class:     gmp_Mpz
 * Method:    randomBits
 * Signature: (Lgmp/RandState;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_randomBits
  (JNIEnv *env, jobject o, jobject r, jint i)
{
  check_nonnull(o,);
  check_nonnull(r,);
  check_positive(i,);
  mpz_urandomb(as_mpz(o), as_randstate(r), i);
}

/*
 * Class:     gmp_Mpz
 * Method:    random
 * Signature: (Lgmp/RandState;Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_random
  (JNIEnv *env, jobject o, jobject r, jobject o2)
{
  check_nonnull(o,);
  check_nonnull(r,);
  check_nonnull(o2,);
  mpz_urandomm(as_mpz(o), as_randstate(r), as_mpz(o2));
}

/*
 * Class:     gmp_Mpz
 * Method:    rRandomBits
 * Signature: (Lgmp/RandState;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpz_rRandomBits
  (JNIEnv *env, jobject o, jobject r, jint i)
{
  check_nonnull(o,);
  check_nonnull(r,);
  check_positive(i,);
  mpz_rrandomb(as_mpz(o), as_randstate(r), i);
}
