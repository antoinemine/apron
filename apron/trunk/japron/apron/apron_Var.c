/*
 * apron_Var.c
 *
 * Functions for ap_var_t to JNI call for implementation of compareTo, hashCode
 *
 * APRON Library / Java Apron binding
 *
 */
#include "japron.h"

//invoke v1.compareTo(v2) and return the result
static int jobject_var_compare(void *v1, void *v2) {
  jobject   j1 = (jobject) v1;
  jobject   j2 = (jobject) v2;
  JNIEnv    *env = get_env();
  jclass    cls = (*env)->GetObjectClass(env, j1);
  jmethodID methid = 
    (*env)->GetMethodID(env, cls, "compareTo", "(Lapron/Var;)I");

  return (*env)->CallIntMethod(env, j1, methid, j2);
}

//invoke v.hashCode() and return the result
static int jobject_var_hash(void *v) {
  jobject   j = (jobject) v; 
  JNIEnv    *env = get_env();
  jclass    cls = (*env)->GetObjectClass(env, j);
  jmethodID methid = (*env)->GetMethodID(env, cls, "hashCode", "()I");

  return (*env)->CallIntMethod(env, j, methid);
}

//invoke v.makeCopy(), get a global ref to result, return that ref
static void* jobject_var_copy(void *v) {
  jobject j = (jobject) v;
  JNIEnv  *env = get_env();
  jclass  cls = (*env)->GetObjectClass(env, j);
  jmethodID methid = 
    (*env)->GetMethodID(env, cls, "clone", "()Lapron/Var;");

  jobject tmp = (*env)->CallObjectMethod(env, j, methid);
  assert(tmp != NULL);

  //bump the ref on the new object so the caller doesn't lose it
  jobject res = (*env)->NewGlobalRef(env, tmp);
  assert(res != NULL);

  return res;
}

//decrement the reference on v
static void jobject_var_free(void *v) {
  jobject j = (jobject) v;
  JNIEnv  *env = get_env();

  (*env)->DeleteGlobalRef(env, j);
  return;
}

//invoke v.toString() and return the result
static char* jobject_var_to_string(void *v) {
  jobject   j = (jobject) v;
  JNIEnv    *env = get_env();
  jclass    cls = (*env)->GetObjectClass(env, j);
  jmethodID methid = 
    (*env)->GetMethodID(env, cls, "toString", "()Ljava/lang/String;");

  //call native toString
  jobject str = (*env)->CallObjectMethod(env, cls, methid);
  
  //convert jobect result to char * allocated with malloc so that the
  //caller can free() it
  const char  *cpch = (*env)->GetStringUTFChars(env, str, NULL);
  char        *pch = strdup(cpch);
  (*env)->ReleaseStringUTFChars(env, str, cpch);

  return pch;
}

//register these callbacks with ap_var_operations
void var_init(void)
{
  ap_var_operations->compare =    jobject_var_compare;
  ap_var_operations->hash =       jobject_var_hash;
  ap_var_operations->copy =       jobject_var_copy;
  ap_var_operations->free =       jobject_var_free;
  ap_var_operations->to_string =  jobject_var_to_string;

  return;
}
