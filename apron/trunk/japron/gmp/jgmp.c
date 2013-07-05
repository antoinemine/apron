/*
 * jgmp.c
 *
 * utilities
 *
 * APRON Library / Java GMP/MPFR binding
 *
 * Copyright (C) Antoine Mine' 2010
 */

#include "jgmp.h"

jclass jgmp_mpz;
jclass jgmp_mpzref;
jclass jgmp_mpq;
jclass jgmp_mpfr;
jclass jgmp_randstate;

jfieldID jgmp_mpz_ptr;
jfieldID jgmp_mpzref_parent;
jfieldID jgmp_mpq_ptr;
jfieldID jgmp_mpfr_ptr;
jfieldID jgmp_randstate_ptr;

jmethodID jgmp_mpz_init;
jmethodID jgmp_mpq_init;
jmethodID jgmp_mpfr_init;

static int jgmp_cached = 0;

void jgmp_cache(JNIEnv *env) 
{
  if (jgmp_cached) return;
  cache_class(jgmp_mpz,       "gmp/Mpz");
  cache_class(jgmp_mpzref,    "gmp/MpzRef");
  cache_class(jgmp_mpq,       "gmp/Mpq");
  cache_class(jgmp_mpfr,      "gmp/Mpfr");
  cache_class(jgmp_randstate, "gmp/RandState");
  cache_field(jgmp_mpz_ptr,        jgmp_mpz,       "ptr",    "J");
  cache_field(jgmp_mpzref_parent,  jgmp_mpzref,    "parent", "Ljava/lang/Object;");
  cache_field(jgmp_mpq_ptr,        jgmp_mpq,       "ptr",    "J");
  cache_field(jgmp_mpfr_ptr,       jgmp_mpfr,      "ptr",    "J");
  cache_field(jgmp_randstate_ptr,  jgmp_randstate, "ptr",    "J");
  cache_init(jgmp_mpz);
  cache_init(jgmp_mpq);
  cache_init(jgmp_mpfr);
  jgmp_cached = 1;
}


void jgmp_throw_by_name(JNIEnv *env, const char *name, const char *msg)
{
  jclass cls = (*env)->FindClass(env, name);
  if (cls) (*env)->ThrowNew(env, cls, msg);
  (*env)->DeleteLocalRef(env, cls);
}

int jgmp_mpz_invertible_p(mpz_t op1, mpz_t op2)
{
  mpz_t dummy;
  mpz_init(dummy);
  int r = mpz_invert(dummy, op1, op2);
  mpz_clear(dummy);
  return r;
}

mpz_ptr jgmp_make_mpz(JNIEnv *env, jobject o)
{
  mpz_ptr ptr = (mpz_ptr)malloc(sizeof(mpz_t));
  if (!ptr) out_of_memory("failed to alloc a mpz_t");
  set_mpz(o, ptr);
  return ptr;
}

jobject jgmp_alloc_init_mpz(JNIEnv *env)
{
  jobject o = (*env)->NewObject(env, jgmp_mpz, jgmp_mpz_init);
  if (!o) return NULL;
  return o;
}

mpq_ptr jgmp_make_mpq(JNIEnv *env, jobject o)
{
  mpq_ptr ptr = (mpq_ptr)malloc(sizeof(mpq_t));
  if (!ptr) out_of_memory("failed to alloc a mpq_t");
  set_mpq(o, ptr);
  return ptr;
}

jobject jgmp_alloc_init_mpq(JNIEnv *env)
{
  jobject o = (*env)->NewObject(env, jgmp_mpq, jgmp_mpq_init);
  if (!o) return NULL;
  return o;
}

mpfr_ptr jgmp_make_mpfr(JNIEnv *env, jobject o)
{
  mpfr_ptr ptr = (mpfr_ptr)malloc(sizeof(mpfr_t));
  if (!ptr) out_of_memory("failed to alloc a mpfr_t");
  set_mpfr(o, ptr);
  return ptr;
}

jobject jgmp_alloc_init_mpfr(JNIEnv *env)
{
  jobject o = (*env)->NewObject(env, jgmp_mpfr, jgmp_mpfr_init);
  if (!o) return NULL;
  return o;
}

gmp_randstate_ptr jgmp_make_randstate(JNIEnv *env, jobject o)
{
  gmp_randstate_ptr ptr = (gmp_randstate_ptr)malloc(sizeof(gmp_randstate_t));
  if (!ptr) out_of_memory("failed to alloc a gmp_randstate_t");
  set_randstate(o, ptr);
  return ptr;
}

jint jgmp_mpz_hash(mpz_t op)
{
  size_t i, sz;
  unsigned char* buf = mpz_export(NULL, &sz, 1, 1, 0, 0, op);
  jint hash = 0;
  for (i=0;i<sz;i++) 
    hash = (hash * 16777619) ^ buf[i];
  hash ^= mpz_sgn(op);
  free(buf);
  return hash;
}

