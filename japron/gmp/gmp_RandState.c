/*
 * gmp_RandState.c
 *
 * glue for RandState class
 *
 * APRON Library / Java GMP/MPFR binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include "jgmp.h"
#include "gmp_RandState.h"

/*
 * Class:     gmp_RandState
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_RandState_init__
  (JNIEnv *env, jobject o1)
{
  check_nonnull(o1,);
  gmp_randstate_ptr ptr = jgmp_make_randstate(env, o1);
  if (ptr) gmp_randinit_default(ptr);
}

/*
 * Class:     gmp_RandState
 * Method:    init_mt
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_RandState_init_1mt
  (JNIEnv *env, jobject o1)
{
  check_nonnull(o1,);
  gmp_randstate_ptr ptr = jgmp_make_randstate(env, o1);
  if (ptr) gmp_randinit_mt(ptr);
}

/*
 * Class:     gmp_RandState
 * Method:    init_lc_2exp
 * Signature: (Lgmp/Mpz;II)V
 */
JNIEXPORT void JNICALL Java_gmp_RandState_init_1lc_12exp
  (JNIEnv *env, jobject o1, jobject o2, jint i, jint j)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  gmp_randstate_ptr ptr = jgmp_make_randstate(env, o1);
  if (ptr) gmp_randinit_lc_2exp(ptr, as_mpz(o2), i, j);
}

/*
 * Class:     gmp_RandState
 * Method:    init_lc_2exp_size
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_gmp_RandState_init_1lc_12exp_1size
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,);
  gmp_randstate_ptr ptr = jgmp_make_randstate(env, o1);
  if (ptr) gmp_randinit_lc_2exp_size(ptr, i);
}

/*
 * Class:     gmp_RandState
 * Method:    init
 * Signature: (Lgmp/RandState;)V
 */
JNIEXPORT void JNICALL Java_gmp_RandState_init__Lgmp_RandState_2
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  gmp_randstate_ptr ptr = jgmp_make_randstate(env, o1);
  if (ptr) gmp_randinit_set(ptr, as_randstate(o2));
}

/*
 * Class:     gmp_RandState
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_RandState_finalize
  (JNIEnv *env, jobject o1)
{
  if (!o1) return;
  gmp_randstate_ptr ptr = as_randstate(o1);
  if (ptr) { gmp_randclear(ptr); free(ptr); }
}

/*
 * Class:     gmp_RandState
 * Method:    clean
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_gmp_RandState_clean
  (JNIEnv *env, jclass cls, jlong p)
{
  gmp_randstate_ptr ptr = (gmp_randstate_ptr)p;
  gmp_randclear(ptr); free(ptr);
}

/*
 * Class:     gmp_RandState
 * Method:    class_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_RandState_class_1init
  (JNIEnv *env, jclass cls)
{
  jgmp_cache(env);
}

/*
 * Class:     gmp_RandState
 * Method:    Seed
 * Signature: (Lgmp/Mpz;)V
 */
JNIEXPORT void JNICALL Java_gmp_RandState_Seed__Lgmp_Mpz_2
  (JNIEnv *env, jobject o1, jobject o2)
{
  check_nonnull(o1,);
  check_nonnull(o2,);
  gmp_randseed(as_randstate(o1), as_mpz(o2));
}

/*
 * Class:     gmp_RandState
 * Method:    Seed
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_gmp_RandState_Seed__I
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,);
  gmp_randseed_ui(as_randstate(o1), i);
}

/*
 * Class:     gmp_RandState
 * Method:    randomBits
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_gmp_RandState_randomBits
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,0);
  check_positive(i,0);
  return gmp_urandomb_ui(as_randstate(o1), i);
}

/*
 * Class:     gmp_RandState
 * Method:    randomInt
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_gmp_RandState_randomInt
  (JNIEnv *env, jobject o1, jint i)
{
  check_nonnull(o1,0);
  check_positive(i,0);
  return gmp_urandomm_ui(as_randstate(o1), i);
}

