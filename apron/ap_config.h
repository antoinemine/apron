/* ************************************************************************* */
/* ap_config.h */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#ifndef _AP_CONFIG_H_
#define _AP_CONFIG_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef HAS_TBOOL
/* Boolean with a third value */
typedef enum tbool_t {
  tbool_false=0,
  tbool_true=1,
  tbool_top=2,   /* don't know */
} tbool_t;

static inline tbool_t tbool_of_int(int a);
static inline tbool_t tbool_of_bool(bool a);
static inline tbool_t tbool_of_or(tbool_t a, tbool_t b);
static inline tbool_t tbool_of_and(tbool_t a, tbool_t b);
static inline tbool_t tbool_of_not(tbool_t a);
#endif

static inline void* ap_malloc(size_t size);
static inline void* ap_realloc(void* ptr, size_t size);
static inline int ap_snprintf(char* restrict s, int n, const char* restrict format, ...);


#if !(defined __USE_SVID || defined __USE_BSD || defined __USE_XOPEN_EXTENDED || defined __APPLE__ || defined __CYGWIN__)

static inline char* strdup(const char* s){
  char* s2;

  s2 = malloc(strlen(s)+1);
  strcpy(s2,s);
  return s2;
}
#endif

#ifndef HAS_TBOOL
#define HAS_TBOOL

static inline tbool_t tbool_of_int(int a)
{ return ((a) ? tbool_true : tbool_false); }
static inline tbool_t tbool_of_bool(bool a)
{ return ((a) ? tbool_true : tbool_false); }
static inline tbool_t tbool_or(tbool_t a, tbool_t b)
{  return
    ( (a==tbool_true || b==tbool_true) ? tbool_true :
      ( (a==tbool_top || b==tbool_top) ? tbool_top :
	tbool_false ) ); }
static inline tbool_t tbool_and(tbool_t a, tbool_t b)
{
  return
    ( (a==tbool_false || b==tbool_false) ? tbool_false :
      ( (a==tbool_top || b==tbool_top) ? tbool_top :
	tbool_true ) );
}
static inline tbool_t tbool_not(tbool_t a)
{
  return
    a==tbool_false ? tbool_true :
    a==tbool_true  ? tbool_false : a;
}
#endif

static inline void* ap_malloc(size_t size)
{
  return size==0 ? NULL : malloc(size);
}
static inline void* ap_realloc(void* ptr, size_t size)
{
  if (size==0){ 
    free(ptr); return NULL;
  } else {
    return realloc(ptr,size);
  }
}
static inline int ap_snprintf(char* restrict s, int n, const char* restrict format, ...)
{
  int count;
  va_list ap;
  va_start(ap,format);
  if (n<=0){
    count = vsnprintf(s+n-1,1,format,ap);
  }
  else {
    count = vsnprintf(s+n,n,format,ap);
  }
  va_end(ap);
  return count;
}

static inline int ap_generic_asprint_of_fprint(
    char** str, void* obj, void (*fprint)(FILE*,void*), size_t size
)
{
  int res;
  char buf[size];
  FILE* stream = tmpfile();
  if (stream==NULL){ *str=NULL; return -1; }
  setvbuf(stream,buf,_IOFBF,size);
  fprint(stream,obj);
  long pos = ftell(stream);
  *str = malloc(pos+1);
  if (pos>0){
    rewind(stream);
    fread(str,1,pos,stream);
    str[pos] = NULL;
  }
  else {
    str[0]=NULL;
  }
  fclose(stream);
  res = pos;
  return res;
}


#ifdef __cplusplus
}
#endif

#endif
