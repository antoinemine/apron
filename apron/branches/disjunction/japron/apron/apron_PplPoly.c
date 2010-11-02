/*
 * apron_PplPoly.c
 *
 * glue for PplPoly.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include "japron.h"
#include "apron_PplPoly.h"
#include "ap_ppl.h"

//////////////////////////////////////

/*
 * Class:     apron_PplPoly
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_PplPoly_init
  (JNIEnv *env, jobject o, jboolean strict)
{
  check_nonnull(o,);
  ap_manager_t* m = ap_ppl_poly_manager_alloc(strict);
  if (!m) { out_of_memory("cannot create manager"); return; }
  japron_manager_setup(m);
  set_manager(o, m);
}

/*
 * Class:     apron_PplPoly
 * Method:    class_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_PplPoly_class_1init
  (JNIEnv *env, jclass cls)
{
  japron_cache(env);
}

