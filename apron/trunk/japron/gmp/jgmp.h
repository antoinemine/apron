/*
 * jgmp.h
 *
 * utilities
 *
 * APRON Library / Java GMP/MPFR binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <gmp.h>
#include <mpfr.h>
#include <jni.h>


/* Extra GMP / MPFR functions */
/* -------------------------- */

typedef __gmp_randstate_struct * gmp_randstate_ptr;

int jgmp_mpz_invertible_p(mpz_t op1, mpz_t op2);

jint jgmp_mpz_hash(mpz_t op);


/* Method, field & class cache */
/* --------------------------- */


extern jclass jgmp_mpz;
extern jclass jgmp_mpzref;
extern jclass jgmp_mpq;
extern jclass jgmp_mpfr;
extern jclass jgmp_randstate;

extern jfieldID jgmp_mpz_ptr;
extern jfieldID jgmp_mpzref_parent;
extern jfieldID jgmp_mpq_ptr;
extern jfieldID jgmp_mpfr_ptr;
extern jfieldID jgmp_randstate_ptr;

extern jmethodID jgmp_mpz_init;
extern jmethodID jgmp_mpq_init;
extern jmethodID jgmp_mpfr_init;

#define cache_class(r,name)                     \
  r = (*env)->FindClass(env, name);             \
  if (r) r =(*env)->NewGlobalRef(env, r);       \
  if (!r) return;                               \
  
#define cache_field(r,cls,nm,typ)            \
  r = (*env)->GetFieldID(env, cls, nm, typ); \
  if (!r) return;

#define cache_init(cls)                                         \
  cls##_init = (*env)->GetMethodID(env, cls, "<init>", "()V");

/* fills cache, once */
void jgmp_cache(JNIEnv *env);


/* Field access */
/* ------------ */

#define as_mpz(obj) \
  ((mpz_ptr)((*env)->GetLongField(env, (obj), jgmp_mpz_ptr)))

#define as_mpq(obj) \
  ((mpq_ptr)((*env)->GetLongField(env, (obj), jgmp_mpq_ptr)))

#define as_mpfr(obj) \
  ((mpfr_ptr)((*env)->GetLongField(env, (obj), jgmp_mpfr_ptr)))

#define as_randstate(obj) \
  ((gmp_randstate_ptr)((*env)->GetLongField(env, (obj), jgmp_randstate_ptr)))


#define set_mpz(obj,ptr) \
  ((*env)->SetLongField(env, (obj), jgmp_mpz_ptr, (jlong)ptr))

#define set_mpq(obj,ptr) \
  ((*env)->SetLongField(env, (obj),jgmp_mpq_ptr, (jlong)ptr))

#define set_mpfr(obj,ptr) \
  ((*env)->SetLongField(env, (obj), jgmp_mpfr_ptr, (jlong)ptr))

#define set_randstate(obj,ptr) \
  ((*env)->SetLongField(env, (obj), jgmp_randstate_ptr, (jlong)ptr))



/* Exceptions */
/* ---------- */


/* throws exceptio name, with a message */
void jgmp_throw_by_name(JNIEnv *env, const char *name, const char *msg);


/* a few standard exceptions to throw */

#define out_of_memory(msg) \
  jgmp_throw_by_name(env,"java/lang/OutOfMemoryError",msg)

#define out_of_bounds(msg) \
  jgmp_throw_by_name(env,"java/lang/IndexOutOfBoundsException",msg)

#define illegal_argument(msg) \
  jgmp_throw_by_name(env,"java/lang/IllegalArgumentException",msg)

#define arithmetic_exception(msg)                               \
  jgmp_throw_by_name(env,"java/lang/ArithmeticException",msg)

#define null_pointer_exception(msg)                             \
  jgmp_throw_by_name(env,"java/lang/NullPointerException",msg)


/* test & throw */

#define check_positive(v,r)                                             \
  if (v<0) { illegal_argument("integer argument must be positive: " #v); return r; }

#define check_nonzero(v,r)                                              \
  if (!v) { arithmetic_exception("division by zero: " #v); return r; }

#define check_nonzero_mpz(v,r)                                           \
  if (!mpz_sgn(v)) { arithmetic_exception("division by zero: " #v); return r; }

#define check_nonnull(v,r)                                              \
  if ((v)==NULL) { null_pointer_exception("argument is null: " #v); return r; }


/* Object creation */
/* --------------- */


mpz_ptr jgmp_make_mpz(JNIEnv *env, jobject o);
mpq_ptr jgmp_make_mpq(JNIEnv *env, jobject o);
mpfr_ptr jgmp_make_mpfr(JNIEnv *env, jobject o);
gmp_randstate_ptr jgmp_make_randstate(JNIEnv *env, jobject o);

jobject jgmp_alloc_init_mpz(JNIEnv *env);
jobject jgmp_alloc_init_mpq(JNIEnv *env);
jobject jgmp_alloc_init_mpfr(JNIEnv *env);
