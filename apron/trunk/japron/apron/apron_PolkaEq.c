/*
 * apron_PolkaEq.c
 *
 * glue for PolkaEq.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include "japron.h"
#include "apron_PolkaEq.h"
#include "pkeq.h"

//////////////////////////////////////

/*
 * Class:     apron_PolkaEq
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_PolkaEq_init
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,);
  ap_manager_t* m = pkeq_manager_alloc();
  if (!m) { out_of_memory("cannot create manager"); return; }
  japron_manager_setup(m);
  set_manager(o, m);
}

/*
 * Class:     apron_PolkaEq
 * Method:    class_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_PolkaEq_class_1init
  (JNIEnv *env, jclass cls)
{
  japron_cache(env);
}

