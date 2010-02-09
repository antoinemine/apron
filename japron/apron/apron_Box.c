/*
 * apron_Box.c
 *
 * glue for Box.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include "japron.h"
#include "apron_Box.h"
#include "box.h"

//////////////////////////////////////

/*
 * Class:     apron_Box
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Box_init
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,);
  ap_manager_t* m = box_manager_alloc();
  if (!m) { out_of_memory("cannot create manager"); return; }
  japron_manager_setup(m);
  set_manager(o, m);
}

/*
 * Class:     apron_Box
 * Method:    class_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Box_class_1init
  (JNIEnv *env, jclass cls)
{
  japron_cache(env);
}

