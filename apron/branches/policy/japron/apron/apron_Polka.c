/*
 * apron_Polka.c
 *
 * glue for Polka.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include "japron.h"
#include "apron_Polka.h"
#include "pk.h"

//////////////////////////////////////

/*
 * Class:     apron_Polka
 * Method:    init
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_apron_Polka_init
  (JNIEnv *env, jobject o, jboolean strict)
{
  check_nonnull(o,);
  ap_manager_t* m = pk_manager_alloc(strict);
  if (!m) { out_of_memory("cannot create manager"); return; }
  japron_manager_setup(m);
  set_manager(o, m);
}

/*
 * Class:     apron_Polka
 * Method:    class_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Polka_class_1init
  (JNIEnv *env, jclass cls)
{
  japron_cache(env);
}

