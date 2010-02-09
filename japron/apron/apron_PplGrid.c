/*
 * apron_PplGrid.c
 *
 * glue for PplGrid.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include "japron.h"
#include "apron_PplGrid.h"
#include "ap_ppl.h"

//////////////////////////////////////

/*
 * Class:     apron_PplGrid
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_PplGrid_init
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,);
  ap_manager_t* m = ap_ppl_grid_manager_alloc();
  if (!m) { out_of_memory("cannot create manager"); return; }
  japron_manager_setup(m);
  set_manager(o, m);
}

/*
 * Class:     apron_PplGrid
 * Method:    class_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_PplGrid_class_1init
  (JNIEnv *env, jclass cls)
{
  japron_cache(env);
}

