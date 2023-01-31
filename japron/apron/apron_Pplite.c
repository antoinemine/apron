/*
 * apron_Pplite.c
 *
 * glue for Pplite.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 * Copyright (C) Enea Zaffanella 2018-2023
 */

#include "japron.h"
#include "apron_Pplite.h"
#include "ap_pplite.h"

//////////////////////////////////////

/*
 * Class:     apron_Pplite
 * Method:    init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Pplite_init
  (JNIEnv *env, jobject o, jboolean strict)
{
  check_nonnull(o,);
  ap_manager_t* m = ap_pplite_manager_alloc(strict);
  if (!m) { out_of_memory("cannot create manager"); return; }
  japron_manager_setup(m);
  set_manager(o, m);
}

/*
 * Class:     apron_Pplite
 * Method:    class_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Pplite_class_1init
  (JNIEnv *env, jclass cls)
{
  japron_cache(env);
}

/*
 * Class:     apron_Pplite
 * Method:    setKind
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_apron_Pplite_setKind
  (JNIEnv *env, jobject m, jstring jname)
{
  check_nonnull(m,);
  const char* cname = (*env)->GetStringUTFChars(env, jname, 0);
  ap_pplite_manager_set_kind(as_manager(m), cname);
  (*env)->ReleaseStringUTFChars(env, jname, cname);
}

/*
 * Class:     apron_Pplite
 * Method:    getKind
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_apron_Pplite_getKind
(JNIEnv *env, jobject m)
{
  check_nonnull(m,NULL);
  const char* cres = ap_pplite_manager_get_kind(as_manager(m));
  return (*env)->NewStringUTF(env, cres);
}

/*
 * Class:     apron_Pplite
 * Method:    setWidenSpec
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_apron_Pplite_setWidenSpec
  (JNIEnv *env, jobject m, jstring jname)
{
  check_nonnull(m,);
  const char* cname = (*env)->GetStringUTFChars(env, jname, 0);
  ap_pplite_manager_set_widen_spec(as_manager(m), cname);
  (*env)->ReleaseStringUTFChars(env, jname, cname);
}

/*
 * Class:     apron_Pplite
 * Method:    getWidenSpec
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_apron_Pplite_getWidenSpec
  (JNIEnv *env, jobject m)
{
  check_nonnull(m,NULL);
  const char* cres = ap_pplite_manager_get_widen_spec(as_manager(m));
  return (*env)->NewStringUTF(env, cres);
}

/*
 * Class:     apron_Pplite
 * Method:    split
 * Signature: (Lapron/Abstract0;Lapron/Lincons0;ZZ)Lapron/Abstract0;
 */
JNIEXPORT jobject JNICALL Java_apron_Pplite_split
  (JNIEnv *env, jobject m, jobject a, jobject c,
   jboolean integral, jboolean strict)
{
  check_nonnull(m,NULL);
  check_nonnull(a,NULL);
  check_nonnull(c,NULL);
  ap_manager_t* man = as_manager(m);
  ap_lincons0_t con;
  if (!japron_lincons0_init_set(env, &con, c)) return NULL;
  ap_abstract0_t* r = ap_pplite_abstract0_split(man, as_abstract0(a),
                                                &con, integral, strict);
  check_exc( { if (r) { ap_abstract0_free(man, r); ap_lincons0_clear(&con); } return NULL; } );
  japron_lincons0_clear(&con);
  return japron_abstract0_get(env, man, r);
}

/*
 * Class:     apron_Pplite
 * Method:    isDisjunctive
 * Signature: (Lapron/Abstract0;)Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Pplite_isDisjunctive
  (JNIEnv *env, jobject m, jobject a);

/*
 * Class:     apron_Pplite
 * Method:    getNumDisjuncts
 * Signature: (Lapron/Abstract0;)I
 */
JNIEXPORT jint JNICALL Java_apron_Pplite_getNumDisjuncts
  (JNIEnv *env, jobject m, jobject a)
{
  check_nonnull(m,0);
  check_nonnull(a,0);
  ap_manager_t* man = as_manager(m);
  int r = ap_pplite_abstract0_num_disjuncts(man, as_abstract0(a));
  check_exc( { } );
  return r;
}

/*
 * Class:     apron_Pplite
 * Method:    toDisjunctLincons
 * Signature: (Lapron/Abstract0;I)[Lapron/Lincons0;
 */
JNIEXPORT jobjectArray JNICALL Java_apron_Pplite_toDisjunctLincons
  (JNIEnv *env, jobject m, jobject a, jint n)
{
  check_nonnull(m,NULL);
  check_nonnull(a,NULL);
  ap_manager_t* man = as_manager(m);
  ap_lincons0_array_t t
    = ap_pplite_abstract0_disj_to_lincons_array(man, as_abstract0(a), n);
  check_exc( { ap_lincons0_array_clear(&t); return NULL; } );
  jobjectArray rr = japron_lincons0_array_get(env, &t);
  ap_lincons0_array_clear(&t);
  return rr;
}

/*
 * Class:     apron_Pplite
 * Method:    toDisjunctTcons
 * Signature: (Lapron/Abstract0;I)[Lapron/Tcons0;
 */
JNIEXPORT jobjectArray JNICALL Java_apron_Pplite_toDisjunctTcons
  (JNIEnv *env, jobject m, jobject a, jint n)
{
  check_nonnull(m,NULL);
  check_nonnull(a,NULL);
  ap_manager_t* man = as_manager(m);
  ap_tcons0_array_t t
    = ap_pplite_abstract0_disj_to_tcons_array(man, as_abstract0(a), n);
  check_exc( { ap_tcons0_array_clear(&t); return NULL; } );
  jobjectArray rr = japron_tcons0_array_get(env, &t);
  ap_tcons0_array_clear(&t);
  return rr;
}

/*
 * Class:     apron_Pplite
 * Method:    collapse
 * Signature: (Lapron/Abstract0;I)V
 */
JNIEXPORT void JNICALL Java_apron_Pplite_collapse
  (JNIEnv *env, jobject m, jobject a, jint n)
{
  check_nonnull(m,);
  check_nonnull(a,);
  ap_manager_t* man = as_manager(m);
  ap_pplite_abstract0_collapse(man, as_abstract0(a), n);
  check_exc( { } );
}

/*
 * Class:     apron_Pplite
 * Method:    isGeomSubsetEq
 * Signature: (Lapron/Abstract0;Lapron/Abstract0;)Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Pplite_isGeomSubsetEq
  (JNIEnv *env, jobject m, jobject a1, jobject a2)
{
  check_nonnull(m,0);
  check_nonnull(a1,0);
  check_nonnull(a2,0);
  ap_manager_t* man = as_manager(m);
  ap_abstract0_t* abs1 = as_abstract0(a1);
  ap_abstract0_t* abs2 = as_abstract0(a2);
  bool r = ap_pplite_abstract0_geom_subseteq(man, abs1, abs2);
  check_exc( { } );
  return r;
}
