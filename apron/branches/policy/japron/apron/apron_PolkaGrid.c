/*
 * apron_PolkaGrid.c
 *
 * glue for PolkaGrid.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include "japron.h"
#include "apron_PolkaGrid.h"
#include "pk.h"
#include "ap_ppl.h"
#include "ap_pkgrid.h"

//////////////////////////////////////

/*
 * Class:     apron_PolkaGrid
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_PolkaGrid_init
  (JNIEnv *env, jobject o, jboolean strict)
{
  check_nonnull(o,);
  ap_manager_t* m1 = pk_manager_alloc(strict);
  ap_manager_t* m2 = ap_ppl_grid_manager_alloc();
  ap_manager_t* m = ap_pkgrid_manager_alloc(m1, m2);
  japron_manager_setup(m1);
  japron_manager_setup(m2);
  ap_manager_free(m1);
  ap_manager_free(m2);
  if (!m) { out_of_memory("cannot create manager"); return; }
  japron_manager_setup(m);
  set_manager(o, m);
}

/*
 * Class:     apron_PolkaGrid
 * Method:    class_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_PolkaGrid_class_1init
  (JNIEnv *env, jclass cls)
{
  japron_cache(env);
}

