/*
 * apron_Abstract0.c
 *
 * glue for Abstract0.java
 *
 * APRON Library / Java Apron binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include "japron.h"
#include "apron_Abstract0.h"

//////////////////////////////////////

/*
 * Class:     apron_Abstract0
 * Method:    init
 * Signature: (Lapron/Manager;Lapron/Abstract0;)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_init__Lapron_Manager_2Lapron_Abstract0_2
  (JNIEnv *env, jobject a, jobject m, jobject o)
{
  check_nonnull(a,);
  check_nonnull(m,);
  check_nonnull(o,);
  ap_manager_t* man = as_manager(m);
  ap_abstract0_t* r = ap_abstract0_copy(man, as_abstract0(o));
  check_exc( { if (r) ap_abstract0_free(man,r); return; } );
  set_abstract0(a, r);
}

/*
 * Class:     apron_Abstract0
 * Method:    finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_finalize
  (JNIEnv *env, jobject o)
{
  if (!o) return;
  ap_abstract0_t* a = as_abstract0(o);
  /* Note: we use the manager that was used at creation time. */
  if (a) ap_abstract0_free(a->man, a);
}

/*
 * Class:     apron_Abstract0
 * Method:    clean
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_clean
  (JNIEnv *env, jclass cls, jlong ptr)
{
  ap_abstract0_t* a = (ap_abstract0_t*)ptr;
  /* Note: we use the manager that was used at creation time. */
  ap_abstract0_free(a->man, a);
}

/*
 * Class:     apron_Abstract0
 * Method:    class_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_class_1init
  (JNIEnv *env, jclass cls)
{
  japron_cache(env);
}

/*
 * Class:     apron_Abstract0
 * Method:    init
 * Signature: (Lapron/Manager;IIZ)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_init__Lapron_Manager_2IIZ
  (JNIEnv *env, jobject a, jobject m, jint i, jint j, jboolean b)
{
  check_nonnull(a,);
  check_nonnull(m,);
  check_positive(i,);
  check_positive(j,);
  ap_manager_t* man = as_manager(m);
  ap_abstract0_t* r =
    b ? ap_abstract0_bottom(man, i, j) : ap_abstract0_top(man, i, j);
  check_exc( { if (r) ap_abstract0_free(man, r); return; } );
  set_abstract0(a, r);
}

/*
 * Class:     apron_Abstract0
 * Method:    init
 * Signature: (Lapron/Manager;II[Lapron/Interval;)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_init__Lapron_Manager_2II_3Lapron_Interval_2
  (JNIEnv *env, jobject a, jobject m, jint i, jint j, jobjectArray ar)
{
  check_nonnull(a,);
  check_nonnull(m,);
  check_positive(i,);
  check_positive(j,);
  check_nonnull(ar,);
  ap_manager_t* man = as_manager(m);
  size_t nb;
  ap_interval_t** t = japron_interval_array_alloc_set(env, &nb, ar);
  if (!t) return;
  if (nb!=(size_t)i+j) {
    ap_interval_array_free(t, nb);
    illegal_argument("invalid array size");
    return;
  }
  ap_abstract0_t* r = ap_abstract0_of_box(man, i, j, t);
  ap_interval_array_free(t, nb);
  check_exc( { if (r) ap_abstract0_free(man, r); return; } );
  set_abstract0(a, r);
}

/*
 * Class:     apron_Abstract0
 * Method:    getSize
 * Signature: (Lapron/Manager;)I
 */
JNIEXPORT jint JNICALL Java_apron_Abstract0_getSize
  (JNIEnv *env, jobject a, jobject m)
{
  check_nonnull(a,0);
  check_nonnull(m,0);
  ap_manager_t* man = as_manager(m);
  size_t r = ap_abstract0_size(man, as_abstract0(a));
  check_exc( { } );
  return r;
}


/*
 * Class:     apron_Abstract0
 * Method:    hashCode
 * Signature: (Lapron/Manager;)I
 */
JNIEXPORT jint JNICALL Java_apron_Abstract0_hashCode
  (JNIEnv *env, jobject a, jobject m)
{
  check_nonnull(a,0);
  check_nonnull(m,0);
  ap_manager_t* man = as_manager(m);
  int r = ap_abstract0_hash(man, as_abstract0(a));
  check_exc( { } );
  return r;
}

/*
 * Class:     apron_Abstract0
 * Method:    getDimension
 * Signature: (Lapron/Manager;)Lapron/Dimension;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_getDimension
  (JNIEnv *env, jobject a, jobject m)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  ap_manager_t* man = as_manager(m);
  ap_dimension_t d = ap_abstract0_dimension(man, as_abstract0(a));
  check_exc( { return NULL; } );
  return japron_dimension_get(env, d.intdim, d.realdim);
}

/*
 * Class:     apron_Abstract0
 * Method:    getCreationManager
 * Signature: ()Lapron/Manager;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_getCreationManager
  (JNIEnv *env, jobject a)
{
  check_nonnull(a,NULL);
  return japron_manager_get(env, as_abstract0(a)->man);
}

/*
 * Class:     apron_Abstract0
 * Method:    getBound
 * Signature: (Lapron/Manager;Lapron/Linexpr0;)Lapron/Interval;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_getBound__Lapron_Manager_2Lapron_Linexpr0_2
  (JNIEnv *env, jobject a, jobject m, jobject o)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  check_nonnull(o,NULL);
  ap_manager_t* man = as_manager(m);
  ap_interval_t* r =
    ap_abstract0_bound_linexpr(man, as_abstract0(a), as_linexpr0(o));
  check_exc( { if (r) ap_interval_free(r); return NULL; } );
  jobject rr = japron_interval_get(env, r);
  ap_interval_free(r);
  return rr;
}


/*
 * Class:     apron_Abstract0
 * Method:    getBound
 * Signature: (Lapron/Manager;Lapron/Texpr0Intern;)Lapron/Interval;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_getBound__Lapron_Manager_2Lapron_Texpr0Intern_2
  (JNIEnv *env, jobject a, jobject m, jobject o)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  check_nonnull(o,NULL);
  ap_manager_t* man = as_manager(m);
  ap_interval_t* r =
    ap_abstract0_bound_texpr(man, as_abstract0(a), as_texpr0(o));
  check_exc( { if (r) ap_interval_free(r); return NULL; } );
  jobject rr = japron_interval_get(env, r);
  ap_interval_free(r);
  return rr;
}

/*
 * Class:     apron_Abstract0
 * Method:    getBound
 * Signature: (Lapron/Manager;I)Lapron/Interval;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_getBound__Lapron_Manager_2I
  (JNIEnv *env, jobject a, jobject m, jint dim)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  check_positive(dim,NULL);
  ap_manager_t* man = as_manager(m);
  ap_interval_t* r =
    ap_abstract0_bound_dimension(man, as_abstract0(a), dim);
  check_exc( { if (r) ap_interval_free(r); return NULL; } );
  jobject rr = japron_interval_get(env, r);
  ap_interval_free(r);
  return rr;
}

/*
 * Class:     apron_Abstract0
 * Method:    toLincons
 * Signature: (Lapron/Manager;)[Lapron/Lincons0;
 */
JNIEXPORT jobjectArray JNICALL Java_apron_Abstract0_toLincons
  (JNIEnv *env, jobject a, jobject m)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  ap_manager_t* man = as_manager(m);
  ap_lincons0_array_t t = ap_abstract0_to_lincons_array(man, as_abstract0(a));
  check_exc( { ap_lincons0_array_clear(&t); return NULL; } );
  jobjectArray rr = japron_lincons0_array_get(env, &t);
  ap_lincons0_array_clear(&t);
  return rr;
}

/*
 * Class:     apron_Abstract0
 * Method:    toTcons
 * Signature: (Lapron/Manager;)[Lapron/Tcons0;
 */
JNIEXPORT jobjectArray JNICALL Java_apron_Abstract0_toTcons
  (JNIEnv *env, jobject a, jobject m)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  ap_manager_t* man = as_manager(m);
  ap_tcons0_array_t t = ap_abstract0_to_tcons_array(man, as_abstract0(a));
  check_exc( { ap_tcons0_array_clear(&t); return NULL; } );
  jobjectArray rr = japron_tcons0_array_get(env, &t);
  ap_tcons0_array_clear(&t);
  return rr;
}

/*
 * Class:     apron_Abstract0
 * Method:    toBox
 * Signature: (Lapron/Manager;)[Lapron/Interval;
 */
JNIEXPORT jobjectArray JNICALL Java_apron_Abstract0_toBox
  (JNIEnv *env, jobject a, jobject m)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  ap_manager_t* man = as_manager(m);
  ap_dimension_t d = ap_abstract0_dimension(man, as_abstract0(a));
  size_t nb = d.intdim + d.realdim;
  ap_interval_t** t = ap_abstract0_to_box(man, as_abstract0(a));
  check_exc( { if (t) ap_interval_array_free(t, nb); return NULL; } );
  jobjectArray rr = japron_interval_array_get(env, t, nb);
  ap_interval_array_free(t, nb); 
  return rr;
}

/*
 * Class:     apron_Abstract0
 * Method:    toGenerator
 * Signature: (Lapron/Manager;)[Lapron/Generator0;
 */
JNIEXPORT jobjectArray JNICALL Java_apron_Abstract0_toGenerator
  (JNIEnv *env, jobject a, jobject m)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  ap_manager_t* man = as_manager(m);
  ap_generator0_array_t t =
    ap_abstract0_to_generator_array(man, as_abstract0(a));
  check_exc( { ap_generator0_array_clear(&t); return NULL; } );
  jobjectArray rr = japron_generator0_array_get(env, &t);
  ap_generator0_array_clear(&t);
  return rr;
}

/*
 * Class:     apron_Abstract0
 * Method:    isBottom
 * Signature: (Lapron/Manager;)Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Abstract0_isBottom
  (JNIEnv *env, jobject a, jobject m)
{
  check_nonnull(a,0);
  check_nonnull(m,0);
  ap_manager_t* man = as_manager(m);
  bool r = ap_abstract0_is_bottom(man, as_abstract0(a));
  check_exc( { } );
  return r;
}

/*
 * Class:     apron_Abstract0
 * Method:    isTop
 * Signature: (Lapron/Manager;)Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Abstract0_isTop
  (JNIEnv *env, jobject a, jobject m)
{
  check_nonnull(a,0);
  check_nonnull(m,0);
  ap_manager_t* man = as_manager(m);
  bool r = ap_abstract0_is_top(man, as_abstract0(a));
  check_exc( { } );
  return r;
}

/*
 * Class:     apron_Abstract0
 * Method:    isIncluded
 * Signature: (Lapron/Manager;Lapron/Abstract0;)Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Abstract0_isIncluded
  (JNIEnv *env, jobject a, jobject m, jobject o)
{
  check_nonnull(a,0);
  check_nonnull(m,0);
  check_nonnull(o,0);
  ap_manager_t* man = as_manager(m);
  bool r = ap_abstract0_is_leq(man, as_abstract0(a), as_abstract0(o));
  check_exc( { } );
  return r;
}

/*
 * Class:     apron_Abstract0
 * Method:    isEqual
 * Signature: (Lapron/Manager;Lapron/Abstract0;)Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Abstract0_isEqual
  (JNIEnv *env, jobject a, jobject m, jobject o)
{
  check_nonnull(a,0);
  check_nonnull(m,0);
  check_nonnull(o,0);
  ap_manager_t* man = as_manager(m);
  bool r = ap_abstract0_is_eq(man, as_abstract0(a), as_abstract0(o));
  check_exc( { } );
  return r;
}

/*
 * Class:     apron_Abstract0
 * Method:    satisfy
 * Signature: (Lapron/Manager;Lapron/Lincons0;)Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Abstract0_satisfy__Lapron_Manager_2Lapron_Lincons0_2
  (JNIEnv *env, jobject a, jobject m, jobject o)
{
  check_nonnull(a,0);
  check_nonnull(m,0);
  check_nonnull(o,0);
  ap_manager_t* man = as_manager(m);
  ap_lincons0_t c;
  if (!japron_lincons0_init_set(env, &c, o)) return 0;
  bool r = ap_abstract0_sat_lincons(man, as_abstract0(a), &c);
  japron_lincons0_clear(&c);
  check_exc( { } );
  return r;
}

/*
 * Class:     apron_Abstract0
 * Method:    satisfy
 * Signature: (Lapron/Manager;Lapron/Tcons0;)Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Abstract0_satisfy__Lapron_Manager_2Lapron_Tcons0_2
  (JNIEnv *env, jobject a, jobject m, jobject o)
{
  check_nonnull(a,0);
  check_nonnull(m,0);
  check_nonnull(o,0);
  ap_manager_t* man = as_manager(m);
  ap_tcons0_t c;
  if (!japron_tcons0_init_set(env, &c, o)) return 0;
  bool r = ap_abstract0_sat_tcons(man, as_abstract0(a), &c);
  japron_tcons0_clear(&c);
  check_exc( { } );
  return r;
}

/*
 * Class:     apron_Abstract0
 * Method:    satisfy
 * Signature: (Lapron/Manager;ILapron/Interval;)Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Abstract0_satisfy__Lapron_Manager_2ILapron_Interval_2
  (JNIEnv *env, jobject a, jobject m, jint dim, jobject o)
{
  check_nonnull(a,0);
  check_nonnull(m,0);
  check_nonnull(o,0);
  check_positive(dim,0);
  ap_manager_t* man = as_manager(m);
  ap_interval_t* c = ap_interval_alloc();
  if (!japron_interval_set(env, c, o)) { ap_interval_free(c); return 0; }
  bool r = ap_abstract0_sat_interval(man, as_abstract0(a), dim, c);
  ap_interval_free(c);
  check_exc( { } );
  return r;
}

/*
 * Class:     apron_Abstract0
 * Method:    isDimensionUnconstrained
 * Signature: (Lapron/Manager;I)Z
 */
JNIEXPORT jboolean JNICALL Java_apron_Abstract0_isDimensionUnconstrained
  (JNIEnv *env, jobject a, jobject m, jint dim)
{
  check_nonnull(a,0);
  check_nonnull(m,0);
  check_positive(dim,0);
  ap_manager_t* man = as_manager(m);
  bool r = ap_abstract0_is_dimension_unconstrained(man, as_abstract0(a), dim);
  check_exc( { } );
  return r;
}

/*
 * Class:     apron_Abstract0
 * Method:    minimize
 * Signature: (Lapron/Manager;)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_minimize
  (JNIEnv *env, jobject a, jobject m)
{
  check_nonnull(a,);
  check_nonnull(m,);
  ap_manager_t* man = as_manager(m);
  ap_abstract0_minimize(man, as_abstract0(a));
  check_exc( { } );
}

/*
 * Class:     apron_Abstract0
 * Method:    canonicalize
 * Signature: (Lapron/Manager;)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_canonicalize
  (JNIEnv *env, jobject a, jobject m)
{
  check_nonnull(a,);
  check_nonnull(m,);
  ap_manager_t* man = as_manager(m);
  ap_abstract0_canonicalize(man, as_abstract0(a));
  check_exc( { } );
}

/*
 * Class:     apron_Abstract0
 * Method:    approximate
 * Signature: (Lapron/Manager;I)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_approximate
  (JNIEnv *env, jobject a, jobject m, jint algo)
{
  check_nonnull(a,);
  check_nonnull(m,);
  ap_manager_t* man = as_manager(m);
  ap_abstract0_approximate(man, as_abstract0(a), algo);
  check_exc( { } );
}

/*
 * Class:     apron_Abstract0
 * Method:    meetCopy
 * Signature: (Lapron/Manager;Lapron/Abstract0;)Lapron/Abstract0;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_meetCopy__Lapron_Manager_2Lapron_Abstract0_2
  (JNIEnv *env, jobject a, jobject m, jobject o)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  check_nonnull(o,NULL);
  ap_manager_t* man = as_manager(m);
  ap_abstract0_t* r = ap_abstract0_meet(man, false, as_abstract0(a), as_abstract0(o));
  check_exc( { if (r) ap_abstract0_free(man, r); return NULL; } );
  return japron_abstract0_get(env, man, r);
}

/*
 * Class:     apron_Abstract0
 * Method:    meet
 * Signature: (Lapron/Manager;[Lapron/Abstract0;)Lapron/Abstract0;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_meet__Lapron_Manager_2_3Lapron_Abstract0_2
  (JNIEnv *env, jclass cls, jobject m, jobjectArray ar)
{
  check_nonnull(m,NULL);
  check_nonnull(ar,NULL);
  ap_manager_t* man = as_manager(m);
  size_t nb;
  ap_abstract0_t** i = japron_abstract0_array_alloc_set(env, ar, &nb);
  if (!i) return NULL;
  ap_abstract0_t* r = ap_abstract0_meet_array(man, i, nb);
  free(i);
  check_exc( { if (r) ap_abstract0_free(man, r); return NULL; } );
  return japron_abstract0_get(env, man, r);
}

/*
 * Class:     apron_Abstract0
 * Method:    meet
 * Signature: (Lapron/Manager;Lapron/Abstract0;)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_meet__Lapron_Manager_2Lapron_Abstract0_2
  (JNIEnv *env, jobject a, jobject m, jobject o)
{
  check_nonnull(a,);
  check_nonnull(m,);
  check_nonnull(o,);
  ap_manager_t* man = as_manager(m);
  ap_abstract0_t* r = ap_abstract0_meet(man, true, as_abstract0(a), as_abstract0(o));
  set_abstract0(a, r);
  check_exc( { } );
}

/*
 * Class:     apron_Abstract0
 * Method:    joinCopy
 * Signature: (Lapron/Manager;Lapron/Abstract0;)Lapron/Abstract0;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_joinCopy
  (JNIEnv *env, jobject a, jobject m, jobject o)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  check_nonnull(o,NULL);
  ap_manager_t* man = as_manager(m);
  ap_abstract0_t* r = ap_abstract0_join(man, false, as_abstract0(a), as_abstract0(o));
  check_exc( { if (r) ap_abstract0_free(man, r); return NULL; } );
  return japron_abstract0_get(env, man, r);
}

/*
 * Class:     apron_Abstract0
 * Method:    join
 * Signature: (Lapron/Manager;[Lapron/Abstract0;)Lapron/Abstract0;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_join__Lapron_Manager_2_3Lapron_Abstract0_2
  (JNIEnv *env, jclass cls, jobject m, jobjectArray ar)
{
  check_nonnull(m,NULL);
  check_nonnull(ar,NULL);
  ap_manager_t* man = as_manager(m);
  size_t nb;
  ap_abstract0_t** i = japron_abstract0_array_alloc_set(env, ar, &nb);
  if (!i) return NULL;
  ap_abstract0_t* r = ap_abstract0_join_array(man, i, nb);
  free(i);
  check_exc( { if (r) ap_abstract0_free(man, r); return NULL; } );
  return japron_abstract0_get(env, man, r);
}

/*
 * Class:     apron_Abstract0
 * Method:    join
 * Signature: (Lapron/Manager;Lapron/Abstract0;)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_join__Lapron_Manager_2Lapron_Abstract0_2
  (JNIEnv *env, jobject a, jobject m, jobject o)
{
  check_nonnull(a,);
  check_nonnull(m,);
  check_nonnull(o,);
  ap_manager_t* man = as_manager(m);
  ap_abstract0_t* r = ap_abstract0_join(man, true, as_abstract0(a), as_abstract0(o));
  set_abstract0(a, r);
  check_exc( { } );
}

/*
 * Class:     apron_Abstract0
 * Method:    meetCopy
 * Signature: (Lapron/Manager;[Lapron/Lincons0;)Lapron/Abstract0;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_meetCopy__Lapron_Manager_2_3Lapron_Lincons0_2
  (JNIEnv *env, jobject a, jobject m, jobjectArray ar)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  check_nonnull(ar,NULL);
  ap_manager_t* man = as_manager(m);
  ap_lincons0_array_t t;
  if (!japron_lincons0_array_init_set(env, &t, ar)) return NULL;
  ap_abstract0_t* r = 
    ap_abstract0_meet_lincons_array(man, false, as_abstract0(a), &t);
  japron_lincons0_array_clear(&t);
  check_exc( { if (r) ap_abstract0_free(man, r); return NULL; } );
  return japron_abstract0_get(env, man, r);
}

/*
 * Class:     apron_Abstract0
 * Method:    meetCopy
 * Signature: (Lapron/Manager;[Lapron/Tcons0;)Lapron/Abstract0;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_meetCopy__Lapron_Manager_2_3Lapron_Tcons0_2
  (JNIEnv *env, jobject a, jobject m, jobjectArray ar)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  check_nonnull(ar,NULL);
  ap_manager_t* man = as_manager(m);
  ap_tcons0_array_t t;
  if (!japron_tcons0_array_init_set(env, &t, ar)) return NULL;
  ap_abstract0_t* r = 
    ap_abstract0_meet_tcons_array(man, false, as_abstract0(a), &t);
  japron_tcons0_array_clear(&t);
  check_exc( { if (r) ap_abstract0_free(man, r); return NULL; } );
  return japron_abstract0_get(env, man, r);
}

/*
 * Class:     apron_Abstract0
 * Method:    meet
 * Signature: (Lapron/Manager;[Lapron/Lincons0;)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_meet__Lapron_Manager_2_3Lapron_Lincons0_2
  (JNIEnv *env, jobject a, jobject m, jobjectArray ar)
{
  check_nonnull(a,);
  check_nonnull(m,);
  check_nonnull(ar,);
  ap_manager_t* man = as_manager(m);
  ap_lincons0_array_t t;
  if (!japron_lincons0_array_init_set(env, &t, ar)) return;
  ap_abstract0_t* r = 
    ap_abstract0_meet_lincons_array(man, true, as_abstract0(a), &t);
  japron_lincons0_array_clear(&t);
  set_abstract0(a, r);
  check_exc( { } );
}


/*
 * Class:     apron_Abstract0
 * Method:    meet
 * Signature: (Lapron/Manager;[Lapron/Tcons0;)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_meet__Lapron_Manager_2_3Lapron_Tcons0_2
  (JNIEnv *env, jobject a, jobject m, jobjectArray ar)
{
  check_nonnull(a,);
  check_nonnull(m,);
  check_nonnull(ar,);
  ap_manager_t* man = as_manager(m);
  ap_tcons0_array_t t;
  if (!japron_tcons0_array_init_set(env, &t, ar)) return;
  ap_abstract0_t* r = 
    ap_abstract0_meet_tcons_array(man, true, as_abstract0(a), &t);
  japron_tcons0_array_clear(&t);
  set_abstract0(a, r);
  check_exc( { } );
}

/*
 * Class:     apron_Abstract0
 * Method:    addRayCopy
 * Signature: (Lapron/Manager;[Lapron/Generator0;)Lapron/Abstract0;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_addRayCopy
  (JNIEnv *env, jobject a, jobject m, jobjectArray ar)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  check_nonnull(ar,NULL);
  ap_manager_t* man = as_manager(m);
  ap_generator0_array_t t;
  if (!japron_generator0_array_init_set(env, &t, ar)) return NULL;
  ap_abstract0_t* r = 
    ap_abstract0_add_ray_array(man, false, as_abstract0(a), &t);
  japron_generator0_array_clear(&t);
  check_exc( { if (r) ap_abstract0_free(man, r); return NULL; } );
  return japron_abstract0_get(env, man, r);
}

/*
 * Class:     apron_Abstract0
 * Method:    addRay
 * Signature: (Lapron/Manager;[Lapron/Generator0;)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_addRay
  (JNIEnv *env, jobject a, jobject m, jobjectArray ar)
{
  check_nonnull(a,);
  check_nonnull(m,);
  check_nonnull(ar,);
  ap_manager_t* man = as_manager(m);
  ap_generator0_array_t t;
  if (!japron_generator0_array_init_set(env, &t, ar)) return;
  ap_abstract0_t* r = 
    ap_abstract0_add_ray_array(man, true, as_abstract0(a), &t);
  japron_generator0_array_clear(&t);
  set_abstract0(a, r);
  check_exc( { } );
}

/*
 * Class:     apron_Abstract0
 * Method:    assign
 * Signature: (Lapron/Manager;[I[Lapron/Linexpr0;Lapron/Abstract0;)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_assign__Lapron_Manager_2_3I_3Lapron_Linexpr0_2Lapron_Abstract0_2
  (JNIEnv *env, jobject a, jobject m, jintArray ari, jobjectArray aro, jobject o)
{
  check_nonnull(a,);
  check_nonnull(m,);
  check_nonnull(ari,);
  check_nonnull(aro,);
  ap_manager_t* man = as_manager(m);
  size_t nb1, nb2;
  ap_linexpr0_t** to = japron_linexpr0_array_alloc_set(env, aro, &nb1);
  if (!to) return;
  ap_dim_t* ti = japron_dim_array_alloc_set(env, ari, &nb2);
  if (!ti) { free(to); return; }
  if (nb1!=nb2) {
    free(to); 
    free(ti); 
    illegal_argument("incompatible array dimensions");
    return;
  }
  ap_abstract0_t* r = 
    ap_abstract0_assign_linexpr_array(man, true, as_abstract0(a), ti, to, nb1,
                                      o ? as_abstract0(o) : NULL);
  free(to); 
  free(ti);
  set_abstract0(a, r);
  check_exc( { } );
}

/*
 * Class:     apron_Abstract0
 * Method:    assignCopy
 * Signature: (Lapron/Manager;[I[Lapron/Linexpr0;Lapron/Abstract0;)Lapron/Abstract0;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_assignCopy__Lapron_Manager_2_3I_3Lapron_Linexpr0_2Lapron_Abstract0_2
  (JNIEnv *env, jobject a, jobject m, jintArray ari, jobjectArray aro, jobject o)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  check_nonnull(ari,NULL);
  check_nonnull(aro,NULL);
  ap_manager_t* man = as_manager(m);
  size_t nb1, nb2;
  ap_linexpr0_t** to = japron_linexpr0_array_alloc_set(env, aro, &nb1);
  if (!to) return NULL;
  ap_dim_t* ti = japron_dim_array_alloc_set(env, ari, &nb2);
  if (!ti) { free(to); return NULL; }
  if (nb1!=nb2) {
    free(to); 
    free(ti); 
    illegal_argument("incompatible array dimensions");
    return NULL;
  }
  ap_abstract0_t* r = 
    ap_abstract0_assign_linexpr_array(man, false, as_abstract0(a), ti, to, nb1,
                                      o ? as_abstract0(o) : NULL);
  free(to); 
  free(ti);
  check_exc( { if (r) ap_abstract0_free(man, r); return NULL; } );
  return japron_abstract0_get(env, man, r);
}

/*
 * Class:     apron_Abstract0
 * Method:    assign
 * Signature: (Lapron/Manager;[I[Lapron/Texpr0Intern;Lapron/Abstract0;)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_assign__Lapron_Manager_2_3I_3Lapron_Texpr0Intern_2Lapron_Abstract0_2
  (JNIEnv *env, jobject a, jobject m, jintArray ari, jobjectArray aro, jobject o)
{
  check_nonnull(a,);
  check_nonnull(m,);
  check_nonnull(ari,);
  check_nonnull(aro,);
  ap_manager_t* man = as_manager(m);
  size_t nb1, nb2;
  ap_texpr0_t** to = japron_texpr0_array_alloc_set(env, aro, &nb1);
  if (!to) return;
  ap_dim_t* ti = japron_dim_array_alloc_set(env, ari, &nb2);
  if (!ti) { free(to); return; }
  if (nb1!=nb2) {
    free(to); 
    free(ti); 
    illegal_argument("incompatible array dimensions");
    return;
  }
  ap_abstract0_t* r = 
    ap_abstract0_assign_texpr_array(man, true, as_abstract0(a), ti, to, nb1,
                                      o ? as_abstract0(o) : NULL);
  free(to); free(ti);
  set_abstract0(a, r);
  check_exc( { } );
}

/*
 * Class:     apron_Abstract0
 * Method:    assignCopy
 * Signature: (Lapron/Manager;[I[Lapron/Texpr0Intern;Lapron/Abstract0;)Lapron/Abstract0;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_assignCopy__Lapron_Manager_2_3I_3Lapron_Texpr0Intern_2Lapron_Abstract0_2
  (JNIEnv *env, jobject a, jobject m, jintArray ari, jobjectArray aro, jobject o)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  check_nonnull(ari,NULL);
  check_nonnull(aro,NULL);
  ap_manager_t* man = as_manager(m);
  size_t nb1, nb2;
  ap_texpr0_t** to = japron_texpr0_array_alloc_set(env, aro, &nb1);
  if (!to) return NULL;
  ap_dim_t* ti = japron_dim_array_alloc_set(env, ari, &nb2);
  if (!ti) { free(to); return NULL; }
  if (nb1!=nb2) {
    free(to); 
    free(ti); 
    illegal_argument("incompatible array dimensions");
    return NULL;
  }
  ap_abstract0_t* r = 
    ap_abstract0_assign_texpr_array(man, false, as_abstract0(a), ti, to, nb1,
                                      o ? as_abstract0(o) : NULL);
  free(to); 
  free(ti);
  check_exc( { if (r) ap_abstract0_free(man, r); return NULL; } );
  return japron_abstract0_get(env, man, r);
}



/*
 * Class:     apron_Abstract0
 * Method:    substitute
 * Signature: (Lapron/Manager;[I[Lapron/Linexpr0;Lapron/Abstract0;)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_substitute__Lapron_Manager_2_3I_3Lapron_Linexpr0_2Lapron_Abstract0_2
  (JNIEnv *env, jobject a, jobject m, jintArray ari, jobjectArray aro, jobject o)
{
  check_nonnull(a,);
  check_nonnull(m,);
  check_nonnull(ari,);
  check_nonnull(aro,);
  ap_manager_t* man = as_manager(m);
  size_t nb1, nb2;
  ap_linexpr0_t** to = japron_linexpr0_array_alloc_set(env, aro, &nb1);
  if (!to) return;
  ap_dim_t* ti = japron_dim_array_alloc_set(env, ari, &nb2);
  if (!ti) { free(to); return; }
  if (nb1!=nb2) {
    free(to); 
    free(ti);
    illegal_argument("incompatible array dimensions");
    return;
  }
  ap_abstract0_t* r = 
    ap_abstract0_substitute_linexpr_array(man, true, as_abstract0(a), ti, to, nb1,
                                      o ? as_abstract0(o) : NULL);
  free(to); 
  free(ti);
  set_abstract0(a, r);
  check_exc( { } );
}

/*
 * Class:     apron_Abstract0
 * Method:    substituteCopy
 * Signature: (Lapron/Manager;[I[Lapron/Linexpr0;Lapron/Abstract0;)Lapron/Abstract0;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_substituteCopy__Lapron_Manager_2_3I_3Lapron_Linexpr0_2Lapron_Abstract0_2
  (JNIEnv *env, jobject a, jobject m, jintArray ari, jobjectArray aro, jobject o)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  check_nonnull(ari,NULL);
  check_nonnull(aro,NULL);
  ap_manager_t* man = as_manager(m);
  size_t nb1, nb2;
  ap_linexpr0_t** to = japron_linexpr0_array_alloc_set(env, aro, &nb1);
  if (!to) return NULL;
  ap_dim_t* ti = japron_dim_array_alloc_set(env, ari, &nb2);
  if (!ti) { free(to); return NULL; }
  if (nb1!=nb2) {
    free(to); 
    free(ti); 
    illegal_argument("incompatible array dimensions");
    return NULL;
  }
  ap_abstract0_t* r = 
    ap_abstract0_substitute_linexpr_array(man, false, as_abstract0(a), ti, to, nb1,
                                      o ? as_abstract0(o) : NULL);
  free(to); 
  free(ti);
  check_exc( { if (r) ap_abstract0_free(man, r); return NULL; } );
  return japron_abstract0_get(env, man, r);
}

/*
 * Class:     apron_Abstract0
 * Method:    substitute
 * Signature: (Lapron/Manager;[I[Lapron/Texpr0Intern;Lapron/Abstract0;)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_substitute__Lapron_Manager_2_3I_3Lapron_Texpr0Intern_2Lapron_Abstract0_2
  (JNIEnv *env, jobject a, jobject m, jintArray ari, jobjectArray aro, jobject o)
{
  check_nonnull(a,);
  check_nonnull(m,);
  check_nonnull(ari,);
  check_nonnull(aro,);
  ap_manager_t* man = as_manager(m);
  size_t nb1, nb2;
  ap_texpr0_t** to = japron_texpr0_array_alloc_set(env, aro, &nb1);
  if (!to) return;
  ap_dim_t* ti = japron_dim_array_alloc_set(env, ari, &nb2);
  if (!ti) { free(to); return; }
  if (nb1!=nb2) {
    free(to); 
    free(ti); 
    illegal_argument("incompatible array dimensions");
    return;
  }
  ap_abstract0_t* r = 
    ap_abstract0_substitute_texpr_array(man, true, as_abstract0(a), ti, to, nb1,
                                      o ? as_abstract0(o) : NULL);
  free(to); 
  free(ti);
  set_abstract0(a, r);
  check_exc( { } );
}

/*
 * Class:     apron_Abstract0
 * Method:    substituteCopy
 * Signature: (Lapron/Manager;[I[Lapron/Texpr0Intern;Lapron/Abstract0;)Lapron/Abstract0;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_substituteCopy__Lapron_Manager_2_3I_3Lapron_Texpr0Intern_2Lapron_Abstract0_2
  (JNIEnv *env, jobject a, jobject m, jintArray ari, jobjectArray aro, jobject o)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  check_nonnull(ari,NULL);
  check_nonnull(aro,NULL);
  ap_manager_t* man = as_manager(m);
  size_t nb1, nb2;
  ap_texpr0_t** to = japron_texpr0_array_alloc_set(env, aro, &nb1);
  if (!to) return NULL;
  ap_dim_t* ti = japron_dim_array_alloc_set(env, ari, &nb2);
  if (!ti) { free(to); return NULL; }
  if (nb1!=nb2) {
    free(to); 
    free(ti); 
    illegal_argument("incompatible array dimensions");
    return NULL;
  }
  ap_abstract0_t* r = 
    ap_abstract0_substitute_texpr_array(man, false, as_abstract0(a), ti, to, nb1,
                                      o ? as_abstract0(o) : NULL);
  free(to); 
  free(ti);
  check_exc( { if (r) ap_abstract0_free(man, r); return NULL; } );
  return japron_abstract0_get(env, man, r);
}



/*
 * Class:     apron_Abstract0
 * Method:    forgetCopy
 * Signature: (Lapron/Manager;[IZ)Lapron/Abstract0;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_forgetCopy
  (JNIEnv *env, jobject a, jobject m, jintArray ar, jboolean b)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  check_nonnull(ar,NULL);
  ap_manager_t* man = as_manager(m);
  size_t nb;
  ap_dim_t* t = japron_dim_array_alloc_set(env, ar, &nb);
  if (!t) return NULL;
  ap_abstract0_t* r = 
    ap_abstract0_forget_array(man, false, as_abstract0(a), t, nb, b);
  free(t);
  check_exc( { if (r) ap_abstract0_free(man, r); return NULL; } );
  return japron_abstract0_get(env, man, r);
}

/*
 * Class:     apron_Abstract0
 * Method:    forget
 * Signature: (Lapron/Manager;[IZ)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_forget
  (JNIEnv *env, jobject a, jobject m, jintArray ar, jboolean b)
{
  check_nonnull(a,);
  check_nonnull(m,);
  check_nonnull(ar,);
  ap_manager_t* man = as_manager(m);
  size_t nb;
  ap_dim_t* t = japron_dim_array_alloc_set(env, ar, &nb);
  if (!t) return;
  ap_abstract0_t* r = 
    ap_abstract0_forget_array(man, true, as_abstract0(a), t, nb, b);
  free(t);
  set_abstract0(a, r);  
  check_exc( { } );
}


/*
 * Class:     apron_Abstract0
 * Method:    addDimensionsCopy
 * Signature: (Lapron/Manager;Lapron/Dimchange;Z)Lapron/Abstract0;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_addDimensionsCopy
  (JNIEnv *env, jobject a, jobject m, jobject o, jboolean b)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  check_nonnull(o,NULL);
  ap_manager_t* man = as_manager(m);
  ap_abstract0_t* r = 
    ap_abstract0_add_dimensions(man, false, as_abstract0(a), 
                                as_dimchange(o), b);
  check_exc( { if (r) ap_abstract0_free(man, r); return NULL; } );
  return japron_abstract0_get(env, man, r);
}

/*
 * Class:     apron_Abstract0
 * Method:    addDimensions
 * Signature: (Lapron/Manager;Lapron/Dimchange;Z)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_addDimensions
  (JNIEnv *env, jobject a, jobject m, jobject o, jboolean b)
{
  check_nonnull(a,);
  check_nonnull(m,);
  check_nonnull(o,);
  ap_manager_t* man = as_manager(m);
  ap_abstract0_t* r = 
    ap_abstract0_add_dimensions(man, true, as_abstract0(a), 
                                as_dimchange(o), b);
  set_abstract0(a, r);  
  check_exc( { } );
}

/*
 * Class:     apron_Abstract0
 * Method:    removeDimensionsCopy
 * Signature: (Lapron/Manager;Lapron/Dimchange;)Lapron/Abstract0;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_removeDimensionsCopy
  (JNIEnv *env, jobject a, jobject m, jobject o)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  check_nonnull(o,NULL);
  ap_manager_t* man = as_manager(m);
  ap_abstract0_t* r = 
    ap_abstract0_remove_dimensions(man, false, as_abstract0(a), 
                                   as_dimchange(o));
  check_exc( { if (r) ap_abstract0_free(man, r); return NULL; } );
  return japron_abstract0_get(env, man, r);
}

/*
 * Class:     apron_Abstract0
 * Method:    removeDimensions
 * Signature: (Lapron/Manager;Lapron/Dimchange;)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_removeDimensions
  (JNIEnv *env, jobject a, jobject m, jobject o)
{
  check_nonnull(a,);
  check_nonnull(m,);
  check_nonnull(o,);
  ap_manager_t* man = as_manager(m);
  ap_abstract0_t* r = 
    ap_abstract0_remove_dimensions(man, true, as_abstract0(a), 
                                   as_dimchange(o));
  set_abstract0(a, r);  
  check_exc( { } );
}

/*
 * Class:     apron_Abstract0
 * Method:    permuteDimensionsCopy
 * Signature: (Lapron/Manager;Lapron/Dimperm;)Lapron/Abstract0;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_permuteDimensionsCopy
  (JNIEnv *env, jobject a, jobject m, jobject o)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  check_nonnull(o,NULL);
  ap_manager_t* man = as_manager(m);
  ap_abstract0_t* r = 
    ap_abstract0_permute_dimensions(man, false, as_abstract0(a), 
                                    as_dimperm(o));
  check_exc( { if (r) ap_abstract0_free(man, r); return NULL; } );
  return japron_abstract0_get(env, man, r);
}

/*
 * Class:     apron_Abstract0
 * Method:    permuteDimensions
 * Signature: (Lapron/Manager;Lapron/Dimperm;)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_permuteDimensions
  (JNIEnv *env, jobject a, jobject m, jobject o)
{
  check_nonnull(a,);
  check_nonnull(m,);
  check_nonnull(o,);
  ap_manager_t* man = as_manager(m);
  ap_abstract0_t* r = 
    ap_abstract0_permute_dimensions(man, true, as_abstract0(a), 
                                    as_dimperm(o));
  set_abstract0(a, r);  
  check_exc( { } );
}

/*
 * Class:     apron_Abstract0
 * Method:    expandCopy
 * Signature: (Lapron/Manager;II)Lapron/Abstract0;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_expandCopy
  (JNIEnv *env, jobject a, jobject m, jint i, jint j)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  check_positive(i,NULL);
  check_positive(j,NULL);
  ap_manager_t* man = as_manager(m);
  ap_abstract0_t* r = ap_abstract0_expand(man, false, as_abstract0(a), i, j);
  check_exc( { if (r) ap_abstract0_free(man, r); return NULL; } );
  return japron_abstract0_get(env, man, r);
}

/*
 * Class:     apron_Abstract0
 * Method:    expand
 * Signature: (Lapron/Manager;II)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_expand
  (JNIEnv *env, jobject a, jobject m, jint i, jint j)
{
  check_nonnull(a,);
  check_nonnull(m,);
  check_positive(i,);
  check_positive(j,);
  ap_manager_t* man = as_manager(m);
  ap_abstract0_t* r = ap_abstract0_expand(man, true, as_abstract0(a), i, j);
  set_abstract0(a, r);  
  check_exc( { } );
}

/*
 * Class:     apron_Abstract0
 * Method:    foldCopy
 * Signature: (Lapron/Manager;[I)Lapron/Abstract0;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_foldCopy
  (JNIEnv *env, jobject a, jobject m, jintArray ar)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  check_nonnull(ar,NULL);
  ap_manager_t* man = as_manager(m);
  size_t nb;
  ap_dim_t* t = japron_dim_array_alloc_set(env, ar, &nb);
  if (!t) return NULL;
  ap_abstract0_t* r = ap_abstract0_fold(man, false, as_abstract0(a), t, nb);
  free(t);
  check_exc( { if (r) ap_abstract0_free(man, r); return NULL; } );
  return japron_abstract0_get(env, man, r);
}

/*
 * Class:     apron_Abstract0
 * Method:    fold
 * Signature: (Lapron/Manager;[I)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_fold
  (JNIEnv *env, jobject a, jobject m, jintArray ar)
{
  check_nonnull(a,);
  check_nonnull(m,);
  check_nonnull(ar,);
  ap_manager_t* man = as_manager(m);
  size_t nb;
  ap_dim_t* t = japron_dim_array_alloc_set(env, ar, &nb);
  if (!t) return;
  ap_abstract0_t* r = ap_abstract0_fold(man, true, as_abstract0(a), t, nb);
  free(t);
  set_abstract0(a, r);  
  check_exc( { } );
}

/*
 * Class:     apron_Abstract0
 * Method:    widening
 * Signature: (Lapron/Manager;Lapron/Abstract0;)Lapron/Abstract0;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_widening
  (JNIEnv *env, jobject a, jobject m, jobject o)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  check_nonnull(o,NULL);
  ap_manager_t* man = as_manager(m);
  ap_abstract0_t* r = ap_abstract0_widening(man, as_abstract0(a), as_abstract0(o));
  check_exc( { if (r) ap_abstract0_free(man, r); return NULL; } );
  return japron_abstract0_get(env, man, r);
}

/*
 * Class:     apron_Abstract0
 * Method:    wideningThreshold
 * Signature: (Lapron/Manager;Lapron/Abstract0;[Lapron/Lincons0;)Lapron/Abstract0;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_wideningThreshold
  (JNIEnv *env, jobject a, jobject m, jobject o, jobjectArray ar)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  check_nonnull(o,NULL);
  check_nonnull(ar,NULL);
  ap_manager_t* man = as_manager(m);
  ap_lincons0_array_t t;
  if (!japron_lincons0_array_init_set(env, &t, ar)) return NULL;
  ap_abstract0_t* r = 
    ap_abstract0_widening_threshold(man, as_abstract0(a), as_abstract0(o), &t);
  japron_lincons0_array_clear(&t);
  check_exc( { if (r) ap_abstract0_free(man, r); return NULL; } );
  return japron_abstract0_get(env, man, r);
}

/*
 * Class:     apron_Abstract0
 * Method:    closureCopy
 * Signature: (Lapron/Manager;)Lapron/Abstract0;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_closureCopy
  (JNIEnv *env, jobject a, jobject m)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  ap_manager_t* man = as_manager(m);
  ap_abstract0_t* r = ap_abstract0_closure(man, false, as_abstract0(a));
  check_exc( { if (r) ap_abstract0_free(man, r); return NULL; } );
  return japron_abstract0_get(env, man, r);
}


/*
 * Class:     apron_Abstract0
 * Method:    closure
 * Signature: (Lapron/Manager;)V
 */
JNIEXPORT void JNICALL Java_apron_Abstract0_closure
  (JNIEnv *env, jobject a, jobject m)
{
  check_nonnull(a,);
  check_nonnull(m,);
  ap_manager_t* man = as_manager(m);
  ap_abstract0_t* r = ap_abstract0_closure(man, true, as_abstract0(a));
  set_abstract0(a, r);
  check_exc( { } );
}


/*
 * Class:     apron_Abstract0
 * Method:    serialize
 * Signature: (Lapron/Manager;)[B
 */
JNIEXPORT jbyteArray JNICALL Java_apron_Abstract0_serialize
  (JNIEnv *env, jobject a, jobject m)
{
  check_nonnull(a,NULL);
  check_nonnull(m,NULL);
  ap_manager_t* man = as_manager(m);
  ap_membuf_t r = ap_abstract0_serialize_raw(man, as_abstract0(a));
  check_exc( { if (r.ptr) free (r.ptr); return NULL; } );
  jbyteArray bytes = (*env)->NewByteArray(env, r.size);
  if (!bytes) { if (r.ptr) free(r.ptr); return NULL; }
  jbyte* buf = (*env)->GetByteArrayElements(env, bytes, 0);
  memcpy(buf, r.ptr, r.size);
  (*env)->ReleaseByteArrayElements(env, bytes, buf, 0);
  if (r.ptr) free(r.ptr); 
  return bytes;
}

/*
 * Class:     apron_Abstract0
 * Method:    deserialize
 * Signature: (Lapron/Manager;[B)Lapron/Abstract0;
 */
JNIEXPORT jobject JNICALL Java_apron_Abstract0_deserialize
  (JNIEnv *env, jclass cls, jobject m, jbyteArray ar)
{
  check_nonnull(m,NULL);
  check_nonnull(ar,NULL);
  ap_manager_t* man = as_manager(m);
  size_t nb = (*env)->GetArrayLength(env, ar);
  jbyte* buf = (*env)->GetByteArrayElements(env, ar, 0);
  ap_abstract0_t* r = ap_abstract0_deserialize_raw(man, buf, &nb);  
  (*env)->ReleaseByteArrayElements(env, ar, buf, 0);
  check_exc( { if (r) ap_abstract0_free(man, r); } );
  return japron_abstract0_get(env, man, r);
}
