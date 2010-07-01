/* ********************************************************************** */
/* num_name.h: numerical type name and mangling */
/* ********************************************************************** */

/* Unlike num.h, this file can be included several times.
 */

#undef NUM_NAME
#undef NUM_SUFFIX
#undef NUMFUN3
#undef NUMFUN2
#undef NUMFUN

/* NUMFUN(xxx) expands to xxx_mpq, or xxx_dbl, ... */
#define NUMFUN3(NAME,SUFFIX) NAME ## _ ## SUFFIX
#define NUMFUN2(x,y) NUMFUN3(x,y)
#define NUMFUN(x) NUMFUN2(x,NUM_SUFFIX)

#if defined(NUM_LONGRAT)
#define NUM_NAME   "NUM_LONGRAT"
#define NUM_SUFFIX Rl

#elif defined(NUM_LONGLONGRAT)
#define NUM_NAME   "NUM_LONGLONGRAT"
#define NUM_SUFFIX Rll

#elif defined(NUM_MPQ)
#define NUM_NAME   "NUM_MPQ"
#define NUM_SUFFIX MPQ

#elif defined(NUM_LONGINT)
#define NUM_NAME   "NUM_LONGINT"
#define NUM_SUFFIX Il

#elif defined(NUM_LONGLONGINT)
#define NUM_NAME   "NUM_LONGLONGINT"
#define NUM_SUFFIX Ill

#elif defined(NUM_MPZ)
#define NUM_NAME   "NUM_MPZ"
#define NUM_SUFFIX MPZ

#elif defined(NUM_DOUBLE)
#define NUM_NAME   "NUM_DOUBLE"
#define NUM_SUFFIX D

#elif defined(NUM_LONGDOUBLE)
#define NUM_NAME   "NUM_LONGDOUBLE"
#define NUM_SUFFIX Dl

#elif defined(NUM_MPFR)
#define NUM_NAME   "NUM_MPFR"
#define NUM_SUFFIX MPFR

#else
#error "HERE"
#endif
