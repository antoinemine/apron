/*
 * gmp_MpzRef.c
 *
 * glue for MpzRef class
 *
 * APRON Library / Java GMP/MPFR binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include "jgmp.h"
#include "gmp_Mpq.h"

/*
 * Class:     gmp_MpzRef
 * Method:    class_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_gmp_MpzRef_class_1init
  (JNIEnv *env, jclass cls)
{
  jgmp_cache(env);
}
