/*
 * gmp_Mpfr.c
 *
 * glue for Mpfr class
 *
 * APRON Library / Java GMP/MPFR binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include "jgmp.h"
#include "gmp_Mpfr.h"

/*
 * Class:     gmp_Mpfr
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_init__
  (JNIEnv *env, jobject o1)
{
  check_nonnull(o1,);
  mpfr_ptr ptr = jgmp_make_mpfr(env, o1);
  if (ptr) mpfr_init(ptr);
}

/*
 * Class:     gmp_Mpfr
 * Method:    init
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_init__I
  (JNIEnv *env, jobject o1, jint p)
{
  check_nonnull(o1,);
  check_positive(p,);
  mpfr_ptr ptr = jgmp_make_mpfr(env, o1);
  if (ptr) mpfr_init2(ptr, p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    init_set
 * Signature: (Lgmp/Mpfr;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_init_1set__Lgmp_Mpfr_2I
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  mpfr_ptr ptr = jgmp_make_mpfr(env, o1);
  if (ptr) mpfr_init_set(ptr, as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    init_set
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_init_1set__II
  (JNIEnv *env, jobject o1, jint i, jint p)
{
  check_nonnull(o1,);
  mpfr_ptr ptr = jgmp_make_mpfr(env, o1);
  if (ptr) mpfr_init_set_si(ptr, i, p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    init_set
 * Signature: (Lgmp/Mpz;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_init_1set__Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  mpfr_ptr ptr = jgmp_make_mpfr(env, o1);
  if (ptr) mpfr_init_set_z(ptr, as_mpz(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    init_set
 * Signature: (DI)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_init_1set__DI
  (JNIEnv *env, jobject o1, jdouble d, jint p)
{
  check_nonnull(o1,);
  mpfr_ptr ptr = jgmp_make_mpfr(env, o1);
  if (ptr) mpfr_init_set_d(ptr, d, p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    init_set
 * Signature: (Lgmp/Mpq;I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_init_1set__Lgmp_Mpq_2I
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  mpfr_ptr ptr = jgmp_make_mpfr(env, o1);
  if (ptr) mpfr_init_set_q(ptr, as_mpq(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    init_set
 * Signature: (Ljava/lang/String;II)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_init_1set__Ljava_lang_String_2II
  (JNIEnv *env, jobject o, jstring v, jint base, jint p)
{
  check_nonnull(o,);
  check_nonnull(v,);
  const char *str;
  int r;
  mpfr_ptr ptr = jgmp_make_mpfr(env, o);
  if (!ptr) return;
  str = (*env)->GetStringUTFChars(env, v, NULL);
  if (!str) return;
  r =  mpfr_init_set_str(ptr, str, base, p);
  (*env)->ReleaseStringUTFChars(env, v, str);
  if (r) illegal_argument("String does not represent a number");
}


/*
 * Class:     gmp_Mpfr
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_finalize
  (JNIEnv *env, jobject o)
{
  if (!o) return;
  mpfr_ptr ptr = as_mpfr(o);
  if (ptr) { mpfr_clear(ptr); free(ptr); }
}

/*
 * Class:     gmp_Mpfr
 * Method:    clean
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_clean
  (JNIEnv *env, jclass cls, jlong p)
{
  mpfr_ptr ptr = (mpfr_ptr)p;
  mpfr_clear(ptr); free(ptr);
}

/*
 * Class:     gmp_Mpfr
 * Method:    class_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_class_1init
  (JNIEnv *env, jclass cls)
{
  jgmp_cache(env);
}

/*
 * Class:     gmp_Mpfr
 * Method:    setDefaultPrec
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_setDefaultPrec
  (JNIEnv *env, jclass cls, jint p)
{
  check_positive(p,);
  mpfr_set_default_prec(p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    getDefaultPrec
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_getDefaultPrec
  (JNIEnv *env, jclass cls)
{
  return mpfr_get_default_prec();
}

/*
 * Class:     gmp_Mpfr
 * Method:    setPrec
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_setPrec
  (JNIEnv *env, jobject o, jint p)
{
  check_nonnull(o,);
  check_positive(p,);
  mpfr_set_prec(as_mpfr(o), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    getPrec
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_getPrec
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  return mpfr_get_prec(as_mpfr(o));
}

/*
 * Class:     gmp_Mpfr
 * Method:    set
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_set__Lgmp_Mpfr_2I
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_set(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    set
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_set__II
  (JNIEnv *env, jobject o1, jint i, jint p)
{
  check_nonnull(o1,0);
  return mpfr_set_si(as_mpfr(o1), i, p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    set
 * Signature: (III)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_set__III
  (JNIEnv *env, jobject o1, jint i, jint e, jint p)
{
  check_nonnull(o1,0);
  return mpfr_set_si_2exp(as_mpfr(o1), i, e, p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    set
 * Signature: (Lgmp/Mpz;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_set__Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_set_z(as_mpfr(o1), as_mpz(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    set
 * Signature: (DI)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_set__DI
  (JNIEnv *env, jobject o1, jdouble d, jint p)
{
  check_nonnull(o1,0);
  return mpfr_set_d(as_mpfr(o1), d, p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    set
 * Signature: (Lgmp/Mpq;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_set__Lgmp_Mpq_2I
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_set_q(as_mpfr(o1), as_mpq(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    set
 * Signature: (Ljava/lang/String;II)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_set__Ljava_lang_String_2II
  (JNIEnv *env, jobject o, jstring v, jint base, jint p)
{
  check_nonnull(o,);
  check_nonnull(v,);
  const char *str;
  int r;
  mpfr_ptr ptr = as_mpfr(o);
  if (!ptr) return;
  str = (*env)->GetStringUTFChars(env, v, NULL);
  if (!str) return;
  r =  mpfr_set_str(ptr, str, base, p);
  (*env)->ReleaseStringUTFChars(env, v, str);
  if (r) illegal_argument("String does not represent a number");
}

/*
 * Class:     gmp_Mpfr
 * Method:    setInf
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_setInf
  (JNIEnv *env, jobject o1, jint p)
{
  check_nonnull(o1,);
  mpfr_set_inf(as_mpfr(o1), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    setNaN
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_setNaN
  (JNIEnv *env, jobject o1)
{
  check_nonnull(o1,);
  mpfr_set_nan(as_mpfr(o1));
}

/*
 * Class:     gmp_Mpfr
 * Method:    intValue
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_intValue
  (JNIEnv *env, jobject o1, jint p)
{
  check_nonnull(o1,0);
  return mpfr_get_si(as_mpfr(o1), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    doubleValue
 * Signature: (I)D
 */
JNIEXPORT jdouble JNICALL Java_gmp_Mpfr_doubleValue
  (JNIEnv *env, jobject o1, jint p)
{
  check_nonnull(o1,0.);
  return mpfr_get_d(as_mpfr(o1), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    doubleExponentValue
 * Signature: ([II)D
 */
JNIEXPORT jdouble JNICALL Java_gmp_Mpfr_doubleExponentValue
  (JNIEnv *env, jobject o, jintArray ar, jint p)
{
  check_nonnull(o,0.);
  long exp;
  double r = mpfr_get_d_2exp(&exp, as_mpfr(o), p);
  jint e = exp;
  if (ar) (*env)->SetIntArrayRegion(env, ar, 0, 1, &e);
  return r;
}

/*
 * Class:     gmp_Mpfr
 * Method:    MpzValue
 * Signature: (I)Lgmp/Mpz;
 */
JNIEXPORT jobject JNICALL Java_gmp_Mpfr_MpzValue
  (JNIEnv *env, jobject o1, jint p)
{
  check_nonnull(o1,NULL);
  jobject r = jgmp_alloc_init_mpz(env);
  if (r) {
    mpfr_get_z(as_mpz(r), as_mpfr(o1), p);
  }
  return r;
}

/*
 * Class:     gmp_Mpfr
 * Method:    MpzExponentValue
 * Signature: (Lgmp/Mpz;)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_MpzExponentValue
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_get_z_exp(as_mpz(o2), as_mpfr(o1));
}

/*
 * Class:     gmp_Mpfr
 * Method:    toString
 * Signature: (III)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_gmp_Mpfr_toString
  (JNIEnv * env, jobject o, jint base, jint n, jint p)
{
  check_nonnull(o,NULL);
  jstring s;
  if (base==10) {
    /* nice formatting based on mpfr_asprintf */
    char *res;
    if (n==0) mpfr_asprintf(&res,"%R*e",(mp_rnd_t)p,as_mpfr(o));
    else mpfr_asprintf(&res,"%.*R*g",n,(mp_rnd_t)p,as_mpfr(o));
    if (!res) out_of_memory("failed to alloc string");
    s = (*env)->NewStringUTF(env, res);
    mpfr_free_str(res);
  }
  else {
    /* formatting based on mpfr_get_str */
    mp_exp_t exp;
    char* res = mpfr_get_str(NULL, &exp, base, n, as_mpfr(o), p);
    size_t len = strlen(res) + 16;
    char* res2 = malloc(len+1);
    /* glue mantissa and exponent */
    snprintf(res2, len, " %s%c%li", res, (base>10) ? '@' : 'e', (long)exp-1);
    res2[len] = 0;
    /* put . at the right position */
    res2[0] = res2[1];
    if (res2[1]=='-') { res2[1] = res2[2]; res2[2] = '.'; }
    else { res2[1] = '.'; }
    s = (*env)->NewStringUTF(env, res2);
    mpfr_free_str(res);
    free(res2);
  }
  return s;
}

/*
 * Class:     gmp_Mpfr
 * Method:    fitsInt
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_fitsInt
  (JNIEnv *env, jobject o1, jint p)
{
  check_nonnull(o1,0);
  return mpfr_fits_sint_p(as_mpfr(o1), p) != 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    add
 * Signature: (Lgmp/Mpfr;Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_add__Lgmp_Mpfr_2Lgmp_Mpfr_2I
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_add(as_mpfr(o1), as_mpfr(o2), as_mpfr(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    add
 * Signature: (Lgmp/Mpfr;II)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_add__Lgmp_Mpfr_2II
  (JNIEnv *env, jobject o1, jobject o2, jint i, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_add_si(as_mpfr(o1), as_mpfr(o2), i, p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    add
 * Signature: (Lgmp/Mpfr;DI)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_add__Lgmp_Mpfr_2DI
  (JNIEnv *env, jobject o1, jobject o2, jdouble d, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_add_d(as_mpfr(o1), as_mpfr(o2), d, p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    add
 * Signature: (Lgmp/Mpfr;Lgmp/Mpz;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_add__Lgmp_Mpfr_2Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_add_z(as_mpfr(o1), as_mpfr(o2), as_mpz(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    add
 * Signature: (Lgmp/Mpfr;Lgmp/Mpq;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_add__Lgmp_Mpfr_2Lgmp_Mpq_2I
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_add_q(as_mpfr(o1), as_mpfr(o2), as_mpq(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    sub
 * Signature: (Lgmp/Mpfr;Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_sub__Lgmp_Mpfr_2Lgmp_Mpfr_2I
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_sub(as_mpfr(o1), as_mpfr(o2), as_mpfr(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    sub
 * Signature: (Lgmp/Mpfr;II)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_sub__Lgmp_Mpfr_2II
  (JNIEnv *env, jobject o1, jobject o2, jint i, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_sub_si(as_mpfr(o1), as_mpfr(o2), i, p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    sub
 * Signature: (Lgmp/Mpfr;DI)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_sub__Lgmp_Mpfr_2DI
  (JNIEnv *env, jobject o1, jobject o2, jdouble d, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_sub_d(as_mpfr(o1), as_mpfr(o2), d, p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    sub
 * Signature: (Lgmp/Mpfr;Lgmp/Mpz;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_sub__Lgmp_Mpfr_2Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_sub_z(as_mpfr(o1), as_mpfr(o2), as_mpz(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    sub
 * Signature: (Lgmp/Mpfr;Lgmp/Mpq;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_sub__Lgmp_Mpfr_2Lgmp_Mpq_2I
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_sub_q(as_mpfr(o1), as_mpfr(o2), as_mpq(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    sub
 * Signature: (ILgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_sub__ILgmp_Mpfr_2I
  (JNIEnv *env, jobject o1, jint i, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_si_sub(as_mpfr(o1), i, as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    sub
 * Signature: (DLgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_sub__DLgmp_Mpfr_2I
  (JNIEnv *env, jobject o1, jdouble d, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_d_sub(as_mpfr(o1), d, as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    mul
 * Signature: (Lgmp/Mpfr;Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_mul__Lgmp_Mpfr_2Lgmp_Mpfr_2I
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_mul(as_mpfr(o1), as_mpfr(o2), as_mpfr(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    mul
 * Signature: (Lgmp/Mpfr;II)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_mul__Lgmp_Mpfr_2II
  (JNIEnv *env, jobject o1, jobject o2, jint i, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_mul_si(as_mpfr(o1), as_mpfr(o2), i, p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    mul
 * Signature: (Lgmp/Mpfr;DI)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_mul__Lgmp_Mpfr_2DI
  (JNIEnv *env, jobject o1, jobject o2, jdouble d, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_mul_d(as_mpfr(o1), as_mpfr(o2), d, p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    mul
 * Signature: (Lgmp/Mpfr;Lgmp/Mpz;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_mul__Lgmp_Mpfr_2Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_mul_z(as_mpfr(o1), as_mpfr(o2), as_mpz(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    mul
 * Signature: (Lgmp/Mpfr;Lgmp/Mpq;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_mul__Lgmp_Mpfr_2Lgmp_Mpq_2I
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_mul_q(as_mpfr(o1), as_mpfr(o2), as_mpq(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    div
 * Signature: (Lgmp/Mpfr;Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_div__Lgmp_Mpfr_2Lgmp_Mpfr_2I
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_div(as_mpfr(o1), as_mpfr(o2), as_mpfr(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    div
 * Signature: (Lgmp/Mpfr;II)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_div__Lgmp_Mpfr_2II
  (JNIEnv *env, jobject o1, jobject o2, jint i, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_div_si(as_mpfr(o1), as_mpfr(o2), i, p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    div
 * Signature: (Lgmp/Mpfr;DI)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_div__Lgmp_Mpfr_2DI
  (JNIEnv *env, jobject o1, jobject o2, jdouble d, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_div_d(as_mpfr(o1), as_mpfr(o2), d, p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    div
 * Signature: (Lgmp/Mpfr;Lgmp/Mpz;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_div__Lgmp_Mpfr_2Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
 return mpfr_div_z(as_mpfr(o1), as_mpfr(o2), as_mpz(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    div
 * Signature: (Lgmp/Mpfr;Lgmp/Mpq;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_div__Lgmp_Mpfr_2Lgmp_Mpq_2I
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_div_q(as_mpfr(o1), as_mpfr(o2), as_mpq(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    div
 * Signature: (ILgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_div__ILgmp_Mpfr_2I
  (JNIEnv *env, jobject o1, jint i, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_si_div(as_mpfr(o1), i, as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    div
 * Signature: (DLgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_div__DLgmp_Mpfr_2I
  (JNIEnv *env, jobject o1, jdouble d, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_d_div(as_mpfr(o1), d, as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    sqrt
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_sqrt__Lgmp_Mpfr_2I
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_sqrt(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    sqrt
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_sqrt__II
  (JNIEnv *env, jobject o1, jint i, jint p)
{
  check_nonnull(o1,0);
  check_positive(i,0);
  return mpfr_sqrt_ui(as_mpfr(o1), i, p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    recSqrt
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_recSqrt
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_rec_sqrt(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    cbrt
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_cbrt
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_cbrt(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    root
 * Signature: (Lgmp/Mpfr;II)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_root
  (JNIEnv *env, jobject o1, jobject o2, jint i, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_positive(i,0);
#if MPFR_VERSION_MAJOR >= 4
  return mpfr_rootn_ui(as_mpfr(o1), as_mpfr(o2), i, p);
#else
  return mpfr_root(as_mpfr(o1), as_mpfr(o2), i, p);
#endif
}

/*
 * Class:     gmp_Mpfr
 * Method:    pow
 * Signature: (Lgmp/Mpfr;Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_pow__Lgmp_Mpfr_2Lgmp_Mpfr_2I
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_pow(as_mpfr(o1), as_mpfr(o2), as_mpfr(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    pow
 * Signature: (Lgmp/Mpfr;II)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_pow__Lgmp_Mpfr_2II
  (JNIEnv *env, jobject o1, jobject o2, jint i, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_pow_si(as_mpfr(o1), as_mpfr(o2), i, p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    pow
 * Signature: (Lgmp/Mpfr;Lgmp/Mpz;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_pow__Lgmp_Mpfr_2Lgmp_Mpz_2I
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_pow_z(as_mpfr(o1), as_mpfr(o2), as_mpz(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    neg
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_neg
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_neg(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    abs
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_abs
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_abs(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    dim
 * Signature: (Lgmp/Mpfr;Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_dim
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o2,0);
  check_nonnull(o1,0);
  check_nonnull(o3,0);
  return mpfr_dim(as_mpfr(o1), as_mpfr(o2), as_mpfr(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    mul2exp
 * Signature: (Lgmp/Mpfr;II)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_mul2exp
  (JNIEnv *env, jobject o1, jobject o2, jint i, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_mul_2si(as_mpfr(o1), as_mpfr(o2), i, p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    div2exp
 * Signature: (Lgmp/Mpfr;II)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_div2exp
  (JNIEnv *env, jobject o1, jobject o2, jint i, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_div_2si(as_mpfr(o1), as_mpfr(o2), i, p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    cmp
 * Signature: (Lgmp/Mpfr;)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_cmp__Lgmp_Mpfr_2
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_cmp(as_mpfr(o1), as_mpfr(o2));
}

/*
 * Class:     gmp_Mpfr
 * Method:    cmp
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_cmp__I
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,0);
  return mpfr_cmp_si(as_mpfr(o1), i);
}

/*
 * Class:     gmp_Mpfr
 * Method:    cmp
 * Signature: (D)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_cmp__D
  (JNIEnv *env, jobject o1, jdouble d)
{
  check_nonnull(o1,0);
  return mpfr_cmp_d(as_mpfr(o1), d);
}

/*
 * Class:     gmp_Mpfr
 * Method:    cmp
 * Signature: (Lgmp/Mpz;)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_cmp__Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_cmp_z(as_mpfr(o1), as_mpz(o2));
}

/*
 * Class:     gmp_Mpfr
 * Method:    cmp
 * Signature: (Lgmp/Mpq;)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_cmp__Lgmp_Mpq_2
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_cmp_q(as_mpfr(o1), as_mpq(o2));
}

/*
 * Class:     gmp_Mpfr
 * Method:    cmp2exp
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_cmp2exp
  (JNIEnv *env, jobject o1, jint i, jint e)
{
  check_nonnull(o1,0);
  return mpfr_cmp_si_2exp(as_mpfr(o1), i, e);
}

/*
 * Class:     gmp_Mpfr
 * Method:    cmpAbs
 * Signature: (Lgmp/Mpfr;)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_cmpAbs
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_cmp_abs(as_mpfr(o1), as_mpfr(o2));
}

/*
 * Class:     gmp_Mpfr
 * Method:    nan
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_isNaN
  (JNIEnv *env, jobject o1)
{
  check_nonnull(o1,0);
  return mpfr_nan_p(as_mpfr(o1)) != 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    inf
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_isInf
  (JNIEnv *env, jobject o1)
{
  check_nonnull(o1,0);
  return mpfr_inf_p(as_mpfr(o1)) != 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    number
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_isNumber
  (JNIEnv *env, jobject o1)
{
  check_nonnull(o1,0);
  return mpfr_number_p(as_mpfr(o1)) != 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    zero
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_isZero
  (JNIEnv *env, jobject o1)
{
  check_nonnull(o1,0);
  return mpfr_zero_p(as_mpfr(o1)) != 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    sgn
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_sgn
  (JNIEnv *env, jobject o1)
{
  check_nonnull(o1,0);
  mpfr_ptr p = as_mpfr(o1);
  return mpfr_sgn(p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    greater
 * Signature: (Lgmp/Mpfr;)Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_isGreater
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_greater_p(as_mpfr(o1), as_mpfr(o2)) != 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    greaterEqual
 * Signature: (Lgmp/Mpfr;)Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_isGreaterEqual
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_greaterequal_p(as_mpfr(o1), as_mpfr(o2)) != 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    less
 * Signature: (Lgmp/Mpfr;)Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_isLess
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_less_p(as_mpfr(o1), as_mpfr(o2)) != 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    lessEqual
 * Signature: (Lgmp/Mpfr;)Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_isLessEqual
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_lessequal_p(as_mpfr(o1), as_mpfr(o2)) != 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    lessGreater
 * Signature: (Lgmp/Mpfr;)Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_isLessGreater
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_lessgreater_p(as_mpfr(o1), as_mpfr(o2)) != 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    equal
 * Signature: (Lgmp/Mpfr;)Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_isEqual
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_equal_p(as_mpfr(o1), as_mpfr(o2)) != 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    unordered
 * Signature: (Lgmp/Mpfr;)Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_isUnordered
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_unordered_p(as_mpfr(o1), as_mpfr(o2)) != 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    hashCode
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_hashCode
  (JNIEnv *env, jobject o1)
{
  check_nonnull(o1,0);
  mpz_t m;
  mpz_init(m);
  mp_exp_t e = mpfr_get_z_exp(m, as_mpfr(o1));
  jint hash = jgmp_mpz_hash(m);
  mpz_clear(m);
  return hash ^ e;
}

/*
 * Class:     gmp_Mpfr
 * Method:    log
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_log
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_log(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    log2
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_log2
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_log2(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    log10
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_log10
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_log10(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    exp
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_exp
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_exp(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    exp2
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_exp2
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_exp2(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    exp10
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_exp10
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_exp10(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    cos
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_cos
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_cos(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    sin
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_sin
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_sin(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    tan
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_tan
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_tan(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    sec
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_sec
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_sec(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    csc
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_csc
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_csc(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    cot
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_cot
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_cot(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    sinCos
 * Signature: (Lgmp/Mpfr;Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_sinCos
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_sin_cos(as_mpfr(o1), as_mpfr(o2), as_mpfr(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    acos
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_acos
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_acos(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    asin
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_asin
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_asin(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    atan
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_atan
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_atan(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    atan2
 * Signature: (Lgmp/Mpfr;Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_atan2
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_atan2(as_mpfr(o1), as_mpfr(o2), as_mpfr(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    cosh
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_cosh
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_cosh(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    sinh
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_sinh
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_sinh(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    sinhCosh
 * Signature: (Lgmp/Mpfr;Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_sinhCosh
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_sinh_cosh(as_mpfr(o1), as_mpfr(o2), as_mpfr(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    sech
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_sech
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_sech(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    csch
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_csch
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_csch(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    coth
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_coth
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_coth(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    acosh
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_acosh
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_acosh(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    asinh
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_asinh
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_asinh(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    atanh
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_atanh
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_atanh(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    fac
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_fac
  (JNIEnv *env, jobject o1, jint i, jint p)
{
  check_nonnull(o1,0);
  check_positive(i,0);
  return mpfr_fac_ui(as_mpfr(o1), i, p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    log1p
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_log1p
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_log1p(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    expm1
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_expm1
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_expm1(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    eint
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_eint
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_eint(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    li2
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_li2
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_li2(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    gamma
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_gamma
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_gamma(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    lngamma
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_lngamma
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_lngamma(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    lgamma
 * Signature: ([ILgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_lgamma
  (JNIEnv *env, jobject o1, jintArray ar, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  int sign;
  int r = mpfr_lgamma(as_mpfr(o1), &sign, as_mpfr(o2), p);
  jint s = sign;
  if (ar) (*env)->SetIntArrayRegion(env, ar, 0, 1, &s);
  return r;
}

/*
 * Class:     gmp_Mpfr
 * Method:    zeta
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_zeta__Lgmp_Mpfr_2I
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_zeta(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    zeta
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_zeta__II
  (JNIEnv *env, jobject o1, jint i, jint p)
{
  check_nonnull(o1,0);
  check_positive(i,0);
  return mpfr_zeta_ui(as_mpfr(o1), i, p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    erf
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_erf
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_erf(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    erfc
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_erfc
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_erfc(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    j0
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_j0
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_j0(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    j1
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_j1
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_j1(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    jn
 * Signature: (I;Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_jn
  (JNIEnv *env, jobject o1, jint i, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_jn(as_mpfr(o1), i, as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    y0
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_y0
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_y0(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    y1
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_y1
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_y1(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    yn
 * Signature: (I;Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_yn
(JNIEnv *env, jobject o1, jint i, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_yn(as_mpfr(o1), i, as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    fma
 * Signature: (Lgmp/Mpfr;Lgmp/Mpfr;Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_fma
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jobject o4, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  check_nonnull(o4,0);
  return mpfr_fma(as_mpfr(o1), as_mpfr(o2), as_mpfr(o3), as_mpfr(o4), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    fms
 * Signature: (Lgmp/Mpfr;Lgmp/Mpfr;Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_fms
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jobject o4, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  check_nonnull(o4,0);
  return mpfr_fms(as_mpfr(o1), as_mpfr(o2), as_mpfr(o3), as_mpfr(o4), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    agm
 * Signature: (Lgmp/Mpfr;Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_agm
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_agm(as_mpfr(o1), as_mpfr(o2), as_mpfr(o3), p);
}



/*
 * Class:     gmp_Mpfr
 * Method:    hypot
 * Signature: (Lgmp/Mpfr;Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_hypot
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_hypot(as_mpfr(o1), as_mpfr(o2), as_mpfr(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    constLog2
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_constLog2
  (JNIEnv *env, jobject o1, jint p)
{
  check_nonnull(o1,0);
  return mpfr_const_log2(as_mpfr(o1), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    constPi
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_constPi
  (JNIEnv *env, jobject o1, jint p)
{
  check_nonnull(o1,0);
  return mpfr_const_pi(as_mpfr(o1), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    constEuler
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_constEuler
  (JNIEnv *env, jobject o1, jint p)
{
  check_nonnull(o1,0);
  return mpfr_const_euler(as_mpfr(o1), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    constCatalan
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_constCatalan
  (JNIEnv *env, jobject o1, jint p)
{
  check_nonnull(o1,0);
  return mpfr_const_catalan(as_mpfr(o1), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    freeCache
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_freeCache
(JNIEnv *env, jclass cls)
{
  mpfr_free_cache();
}


/*
 * Class:     gmp_Mpfr
 * Method:    sum
 * Signature: ([Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_sum
  (JNIEnv *env, jobject o1, jobjectArray tab, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(tab,0);
  size_t i, nb = (*env)-> GetArrayLength(env, tab);
  mpfr_ptr* a = (mpfr_ptr*)malloc(sizeof(mpfr_ptr)*nb);
  if (!a) { out_of_memory("failed to alloc a mpfr_ptr array"); return 0; }
  for (i=0;i<nb;i++) {
    jobject b = (*env)->GetObjectArrayElement(env, tab, i);
    check_nonnull(b,0);
    a[i] = as_mpfr(b);
  }
  int r = mpfr_sum(as_mpfr(o1), a, nb, p);
  free(a);
  return r;
}

/*
 * Class:     gmp_Mpfr
 * Method:    rint
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_rint
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_rint(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    ceil
 * Signature: (Lgmp/Mpfr;)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_ceil
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_ceil(as_mpfr(o1), as_mpfr(o2));
}

/*
 * Class:     gmp_Mpfr
 * Method:    floor
 * Signature: (Lgmp/Mpfr;)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_floor
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_floor(as_mpfr(o1), as_mpfr(o2));
}

/*
 * Class:     gmp_Mpfr
 * Method:    round
 * Signature: (Lgmp/Mpfr;)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_round
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_round(as_mpfr(o1), as_mpfr(o2));
}

/*
 * Class:     gmp_Mpfr
 * Method:    trunc
 * Signature: (Lgmp/Mpfr;)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_trunc
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_trunc(as_mpfr(o1), as_mpfr(o2));
}

/*
 * Class:     gmp_Mpfr
 * Method:    rint_ceil
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_rint_1ceil
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_rint_ceil(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    rint_floor
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_rint_1floor
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_rint_floor(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    rint_round
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_rint_1round
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o2,0);
  return mpfr_rint_round(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    rint_trunc
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_rint_1trunc
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_rint_trunc(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    frac
 * Signature: (Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_frac
  (JNIEnv *env, jobject o1, jobject o2, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_frac(as_mpfr(o1), as_mpfr(o2), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    modf
 * Signature: (Lgmp/Mpfr;Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_modf
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_modf(as_mpfr(o1), as_mpfr(o2), as_mpfr(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    fmod
 * Signature: (Lgmp/Mpfr;Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_fmod
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_fmod(as_mpfr(o1), as_mpfr(o2), as_mpfr(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    remainder
 * Signature: (Lgmp/Mpfr;Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_remainder
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_remainder(as_mpfr(o1), as_mpfr(o2), as_mpfr(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    remquo
 * Signature: ([ILgmp/Mpfr;Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_remquo
  (JNIEnv *env, jobject o1, jintArray ar, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  long q;
  int r = mpfr_remquo(as_mpfr(o1), &q, as_mpfr(o2), as_mpfr(o3), p);
  jint qq = q;
  if (ar) (*env)->SetIntArrayRegion(env, ar, 0, 1, &qq);
  return r;
}

/*
 * Class:     gmp_Mpfr
 * Method:    integer
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_isInteger
  (JNIEnv *env, jobject o1)
{
  check_nonnull(o1,0);
  return mpfr_integer_p(as_mpfr(o1)) != 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    precRound
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_precRound
  (JNIEnv *env, jobject o1, jint i, jint p)
{
  check_nonnull(o1,0);
  check_positive(i,0);
  return mpfr_prec_round(as_mpfr(o1), i, p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    canRound
 * Signature: (IIII)Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_canRound
  (JNIEnv *env, jobject o1, jint i, jint j, jint k, jint l)
{
  check_nonnull(o1,0);
  check_positive(l,0);
  return mpfr_can_round(as_mpfr(o1), i, j, k, l) != 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    toStringRndMode
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_gmp_Mpfr_toStringRndMode
  (JNIEnv *env, jclass cls, jint i)
{  
  const char* res = mpfr_print_rnd_mode(i);
  return (*env)->NewStringUTF(env, res ? res : "");
}

/*
 * Class:     gmp_Mpfr
 * Method:    nextToward
 * Signature: (Lgmp/Mpfr;)V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_nextToward
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  return mpfr_nexttoward(as_mpfr(o1), as_mpfr(o2));
}

/*
 * Class:     gmp_Mpfr
 * Method:    nextAbove
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_nextAbove
  (JNIEnv *env, jobject o1)
{
  check_nonnull(o1,);
  return mpfr_nextabove(as_mpfr(o1));
}

/*
 * Class:     gmp_Mpfr
 * Method:    nextBelow
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_nextBelow
  (JNIEnv *env, jobject o1)
{
  check_nonnull(o1,);
  return mpfr_nextbelow(as_mpfr(o1));
}

/*
 * Class:     gmp_Mpfr
 * Method:    min
 * Signature: (Lgmp/Mpfr;Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_min
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_min(as_mpfr(o1), as_mpfr(o2), as_mpfr(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    max
 * Signature: (Lgmp/Mpfr;Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_max
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_max(as_mpfr(o1), as_mpfr(o2), as_mpfr(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    getExp
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_getExp
  (JNIEnv *env, jobject o1)
{
  check_nonnull(o1,0);
  return mpfr_get_exp(as_mpfr(o1));
}

/*
 * Class:     gmp_Mpfr
 * Method:    setExp
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_setExp
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,0);
  return mpfr_set_exp(as_mpfr(o1), i) == 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    signBit
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_signBit
  (JNIEnv *env, jobject o1)
{
  check_nonnull(o1,0);
  return mpfr_signbit(as_mpfr(o1)) != 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    setSign
 * Signature: (Lgmp/Mpfr;ZI)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_setSign
  (JNIEnv *env, jobject o1, jobject o2, jboolean s, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  return mpfr_setsign(as_mpfr(o1), as_mpfr(o2), s, p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    copySign
 * Signature: (Lgmp/Mpfr;Lgmp/Mpfr;I)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_copySign
  (JNIEnv *env, jobject o1, jobject o2, jobject o3, jint p)
{
  check_nonnull(o1,0);
  check_nonnull(o2,0);
  check_nonnull(o3,0);
  return mpfr_copysign(as_mpfr(o1), as_mpfr(o2), as_mpfr(o3), p);
}

/*
 * Class:     gmp_Mpfr
 * Method:    getVersion
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_gmp_Mpfr_getVersion
  (JNIEnv *env, jclass cls)
{
  return (*env)->NewStringUTF(env, mpfr_get_version());
}

/*
 * Class:     gmp_Mpfr
 * Method:    getPatches
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_gmp_Mpfr_getPatches
  (JNIEnv *env, jclass cls)
{
  return (*env)->NewStringUTF(env, mpfr_get_patches());
}

/*
 * Class:     gmp_Mpfr
 * Method:    getEmin
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_gmp_Mpfr_getEmin
  (JNIEnv *env, jclass cls)
{
  return mpfr_get_emin();
}

/*
 * Class:     gmp_Mpfr
 * Method:    getEmax
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_gmp_Mpfr_getEmax
  (JNIEnv *env, jclass cls)
{
  return mpfr_get_emax();
}

/*
 * Class:     gmp_Mpfr
 * Method:    setEmin
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_setEmin
  (JNIEnv *env, jclass cls, jlong i)
{
  return mpfr_set_emin(i) == 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    setEmax
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_setEmax
  (JNIEnv *env, jclass cls, jlong i)
{
  return mpfr_set_emax(i) == 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    getEminMin
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_gmp_Mpfr_getEminMin
  (JNIEnv *env, jclass cls)
{
  return mpfr_get_emin_min();
}

/*
 * Class:     gmp_Mpfr
 * Method:    getEminMax
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_gmp_Mpfr_getEminMax
  (JNIEnv *env, jclass cls)
{
  return mpfr_get_emin_max();
}

/*
 * Class:     gmp_Mpfr
 * Method:    getEmaxMin
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_gmp_Mpfr_getEmaxMin
  (JNIEnv *env, jclass cls)
{
  return mpfr_get_emax_min();
}

/*
 * Class:     gmp_Mpfr
 * Method:    getEmaxMax
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_gmp_Mpfr_getEmaxMax
  (JNIEnv *env, jclass cls)
{
  return mpfr_get_emax_max();
}

/*
 * Class:     gmp_Mpfr
 * Method:    checkRange
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_checkRange
  (JNIEnv *env, jobject o1, jint i, jint j)
{
  check_nonnull(o1,0);
  return mpfr_check_range(as_mpfr(o1), i, j);
}

/*
 * Class:     gmp_Mpfr
 * Method:    subNormalize
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_subNormalize
  (JNIEnv *env, jobject o1, jint i, jint j)
{
  check_nonnull(o1,0);
  return mpfr_subnormalize(as_mpfr(o1), i, j);
}

/*
 * Class:     gmp_Mpfr
 * Method:    clearUnderflow
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_clearUnderflow
  (JNIEnv *env, jclass cls)
{
  mpfr_clear_underflow();
}

/*
 * Class:     gmp_Mpfr
 * Method:    clearOverflow
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_clearOverflow
  (JNIEnv *env, jclass cls)
{
  mpfr_clear_overflow();
}

/*
 * Class:     gmp_Mpfr
 * Method:    clearNaNFlag
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_clearNaNFlag
  (JNIEnv *env, jclass cls)
{
  mpfr_clear_nanflag();
}

/*
 * Class:     gmp_Mpfr
 * Method:    clearInexFlag
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_clearInexFlag
  (JNIEnv *env, jclass cls)
{
  mpfr_clear_inexflag();
}

/*
 * Class:     gmp_Mpfr
 * Method:    clearERangeFlag
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_clearERangeFlag
  (JNIEnv *env, jclass cls)
{
  mpfr_clear_erangeflag();
}

/*
 * Class:     gmp_Mpfr
 * Method:    setUnderflow
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_setUnderflow
  (JNIEnv *env, jclass cls)
{
  mpfr_set_underflow();
}

/*
 * Class:     gmp_Mpfr
 * Method:    setOverflow
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_setOverflow
  (JNIEnv *env, jclass cls)
{
  mpfr_set_overflow();
}

/*
 * Class:     gmp_Mpfr
 * Method:    setNaNFlag
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_setNaNFlag
  (JNIEnv *env, jclass cls)
{
  mpfr_set_nanflag();
}

/*
 * Class:     gmp_Mpfr
 * Method:    setInexFlag
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_setInexFlag
  (JNIEnv *env, jclass cls)
{
  mpfr_set_inexflag();
}

/*
 * Class:     gmp_Mpfr
 * Method:    setERangeFlag
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_Mpfr_setERangeFlag
  (JNIEnv *env, jclass cls)
{
  mpfr_set_erangeflag();
}

/*
 * Class:     gmp_Mpfr
 * Method:    underflow
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_isUnderflow
  (JNIEnv *env, jclass cls)
{
  return mpfr_underflow_p() != 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    overflow
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_isOverflow
  (JNIEnv *env, jclass cls)
{
  return mpfr_overflow_p() != 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    nanFlag
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_isNaNFlag
  (JNIEnv *env, jclass cls)
{
  return mpfr_nanflag_p() != 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    inexFlag
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_isInexFlag
  (JNIEnv *env, jclass cls)
{
  return mpfr_inexflag_p() != 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    eRangeFlag
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_gmp_Mpfr_isERangeFlag
  (JNIEnv *env, jclass cls)
{
  return mpfr_erangeflag_p() != 0;
}

/*
 * Class:     gmp_Mpfr
 * Method:    random
 * Signature: (Lgmp/RandState;)I
 */
JNIEXPORT jint JNICALL Java_gmp_Mpfr_random
  (JNIEnv *env, jobject o, jobject r)
{
  check_nonnull(o,0);
  check_nonnull(r,0);
  return mpfr_urandomb(as_mpfr(o), as_randstate(r));
}
