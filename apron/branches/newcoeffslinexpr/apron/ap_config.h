/* ************************************************************************* */
/* ap_config.h */
/* ************************************************************************* */

/* This file is part of the APRON Library, released under LGPL license.  Please
   read the COPYING file packaged in the distribution */

#ifndef _AP_CONFIG_H_
#define _AP_CONFIG_H_

#include <stdbool.h>
#include <stdlib.h>
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

#ifdef __cplusplus
}
#endif

#endif
