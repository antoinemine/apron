/*
 * apron_Octagon.c
 *
 * glue for Octagon.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include "japron.h"
#include "apron_Octagon.h"
#include "oct.h"

//////////////////////////////////////

/*
 * Class:     apron_Octagon
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Octagon_init
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,);
  ap_manager_t* m = oct_manager_alloc();
  if (!m) { out_of_memory("cannot create manager"); return; }
  japron_manager_setup(m);
  set_manager(o, m);
}

/*
 * Class:     apron_Octagon
 * Method:    class_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Octagon_class_1init
  (JNIEnv *env, jclass cls)
{
  japron_cache(env);
}

