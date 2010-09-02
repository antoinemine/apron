/*
 * apron_Manager.c
 *
 * glue for Manager.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include "japron.h"
#include "apron_Manager.h"

//////////////////////////////////////

JNIEXPORT void JNICALL Java_apron_Manager_finalize
  (JNIEnv *env, jobject o)
{
  if (!o) return;
  ap_manager_t* m = as_manager(o);
  if (m) ap_manager_free(m);
}

/*
 * Class:     apron_Manager
 * Method:    class_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Manager_class_1init
  (JNIEnv *env, jclass cls)
{
  japron_cache(env);
}

/*
 * Class:     apron_Manager
 * Method:    getLibrary
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_apron_Manager_getLibrary
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,NULL);
  return (*env)->NewStringUTF(env, ap_manager_get_library(as_manager(o)));
}

/*
 * Class:     apron_Manager
 * Method:    getVersion
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_apron_Manager_getVersion
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,NULL);
  return (*env)->NewStringUTF(env, ap_manager_get_version(as_manager(o)));
}

/*
 * Class:     apron_Manager
 * Method:    getAlgorithm
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_apron_Manager_getAlgorithm
  (JNIEnv *env, jobject o, jint id)
{
  check_nonnull(o,0);
  if (id<AP_FUNID_COPY || id>=AP_FUNID_SIZE)
    { illegal_argument("unknown operation identifier"); return 0; }
  return as_manager(o)->option.funopt[id].algorithm;
}

/*
 * Class:     apron_Manager
 * Method:    getTimeout
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_apron_Manager_getTimeout
  (JNIEnv *env, jobject o, jint id)
{
  check_nonnull(o,0);
  if (id<AP_FUNID_COPY || id>=AP_FUNID_SIZE)
    { illegal_argument("unknown operation identifier"); return 0; }
  return as_manager(o)->option.funopt[id].timeout;
}

/*
 * Class:     apron_Manager
 * Method:    getMaxObjectSize
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_apron_Manager_getMaxObjectSize
  (JNIEnv *env, jobject o, jint id)
{
  check_nonnull(o,0);
  if (id<AP_FUNID_COPY || id>=AP_FUNID_SIZE)
    { illegal_argument("unknown operation identifier"); return 0; }
  return as_manager(o)->option.funopt[id].max_object_size;
}

/*
 * Class:     apron_Manager
 * Method:    getFlagExactWanted
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Manager_getFlagExactWanted
  (JNIEnv *env, jobject o, jint id)
{
  check_nonnull(o,0);
  if (id<AP_FUNID_COPY || id>=AP_FUNID_SIZE)
    { illegal_argument("unknown operation identifier"); return 0; }
  return as_manager(o)->option.funopt[id].flag_exact_wanted != 0;
}

/*
 * Class:     apron_Manager
 * Method:    getFlagBestWanted
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Manager_getFlagBestWanted
  (JNIEnv *env, jobject o, jint id)
{
  check_nonnull(o,0);
  if (id<AP_FUNID_COPY || id>=AP_FUNID_SIZE)
    { illegal_argument("unknown operation identifier"); return 0; }
  return as_manager(o)->option.funopt[id].flag_best_wanted != 0;
}

/*
 * Class:     apron_Manager
 * Method:    setAlgorithm
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_apron_Manager_setAlgorithm
  (JNIEnv *env, jobject o, jint id, jint v)
{
  check_nonnull(o,);
  if (id<AP_FUNID_COPY || id>=AP_FUNID_SIZE)
    { illegal_argument("unknown operation identifier"); return; }
  as_manager(o)->option.funopt[id].algorithm = v;
}

/*
 * Class:     apron_Manager
 * Method:    setTimeout
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_apron_Manager_setTimeout
  (JNIEnv *env, jobject o, jint id, jint v)
{
  check_nonnull(o,);
  if (id<AP_FUNID_COPY || id>=AP_FUNID_SIZE)
    { illegal_argument("unknown operation identifier"); return; }
  as_manager(o)->option.funopt[id].timeout = v;
}

/*
 * Class:     apron_Manager
 * Method:    setMaxObjectSize
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_apron_Manager_setMaxObjectSize
  (JNIEnv *env, jobject o, jint id, jint v)
{
  check_nonnull(o,);
  if (id<AP_FUNID_COPY || id>=AP_FUNID_SIZE)
    { illegal_argument("unknown operation identifier"); return; }
  as_manager(o)->option.funopt[id].max_object_size = v;
}

/*
 * Class:     apron_Manager
 * Method:    setFlagExactWanted
 * Signature: (IZ)V
 */
JNIEXPORT void JNICALL Java_apron_Manager_setFlagExactWanted
  (JNIEnv *env, jobject o, jint id, jboolean v)
{
  check_nonnull(o,);
  if (id<AP_FUNID_COPY || id>=AP_FUNID_SIZE)
    { illegal_argument("unknown operation identifier"); return; }
  as_manager(o)->option.funopt[id].flag_exact_wanted = v;
}

/*
 * Class:     apron_Manager
 * Method:    setFlagBestWanted
 * Signature: (IZ)V
 */
JNIEXPORT void JNICALL Java_apron_Manager_setFlagBestWanted
  (JNIEnv *env, jobject o, jint id, jboolean v)
{
  check_nonnull(o,);
  if (id<AP_FUNID_COPY || id>=AP_FUNID_SIZE)
    { illegal_argument("unknown operation identifier"); return; }
  as_manager(o)->option.funopt[id].flag_best_wanted = v;
}

/*
 * Class:     apron_Manager
 * Method:    wasExact
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Manager_wasExact
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  return ap_manager_get_flag_exact(as_manager(o)) != 0;
}

/*
 * Class:     apron_Manager
 * Method:    wasBest
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Manager_wasBest
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  return ap_manager_get_flag_best(as_manager(o)) != 0;
}

/*
 * Class:     apron_Manager
 * Method:    getPreferedScalarType
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_apron_Manager_getPreferedScalarType
  (JNIEnv *env, jobject o)
{
  check_nonnull(o,0);
  return as_manager(o)->option.scalar_discr;
}

/*
 * Class:     apron_Manager
 * Method:    setPreferedScalarType
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_apron_Manager_setPreferedScalarType
  (JNIEnv *env, jobject o, jint v)
{
  check_nonnull(o,);
  as_manager(o)->option.scalar_discr = v;
}
