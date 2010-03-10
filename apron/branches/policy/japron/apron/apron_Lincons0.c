/*
 * apron_Lincons0.c
 *
 * glue for Lincons0.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include "japron.h"
#include "apron_Lincons0.h"

//////////////////////////////////////

JNIEXPORT jboolean JNICALL Java_apron_Lincons0_isUnsat
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  ap_lincons0_t l;
  if (!japron_lincons0_init_set(env, &l, o)) return 0;
  bool r = ap_lincons0_is_unsat(&l);
  japron_lincons0_clear(&l);
  return r;
}

