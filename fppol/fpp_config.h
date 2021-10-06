/*
 * fpp_config.h
 *
 * Global parameters.
 *
 * APRON Library / Fppol Domain
 *
 * Copyright (C) Liqian CHEN' 2008-
 *
 */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution.
*/

#ifndef _FPP_CONFIG_H_
#define _FPP_CONFIG_H_

#include <sys/time.h>

extern int nlpcreate;
extern int nlpdel;
extern int nlp;
extern int nlpjoin;
extern int nslp;
extern int nrlp;
extern int nstrongjoin;
extern int nweakjoin;
extern int maxCons;
extern int rump_max_iterations;
/*-----------------------------------------------*/
/*                 time                         */
/*-----------------------------------------------*/
extern double lptime;

/*-----------------------------------------------*/
/*                 types                         */
/*-----------------------------------------------*/
#ifdef __cplusplus
#define HAS_BOOL
extern "C" {
#endif

#ifndef HAS_BOOL
#define HAS_BOOL
typedef char bool;
static const bool false = 0;
static const bool true  = 1;
#endif

#define dntknow 2

typedef double numdbl_t;   /* floating point type: float or double (only double at present) */

/*-----------------------------------------------*/
/*                 MACROs                        */
/*-----------------------------------------------*/
#define EMPTY_POL    1
#define UNIVERSE_POL 2
#define GENERAL_POL  0

#if defined ( NUM_DOUBLE )
#define MAX_VARBND  NUMFLT_MAX
#else
#define MAX_VARBND  1.0/0.0
#endif

#define MAXEXP_DIFF 20 /*20*/
#define MAX_MULCOEF  1.0E6
#define MAX_MINEXP_COEF 18 /*18*/
#define MIN_MINEXP_COEF -16/*-16*/

#define MINCONS4BDRR 25
#define MINCONS4LPRR 30

#define UNCHANGED 0
#define FAILED -1
#define SUCCEED 1

#define EPSILON 1.0e-3
#define QSEPSILON 1.0e-4
#define EPSILONINT 1.0e-5

#define FPRT  "%f"
#define FPRTE "%e"
#define FPRTL  "%.50f"

#define FSCF  FSCFDOUBLE
#define FSCF2 FSCFDOUBLE2

#define FSCFFLOAT  "%f"
#define FSCFFLOAT2 " %f%n"

#define FSCFDOUBLE  "%lf"
#define FSCFDOUBLE2 " %lf%n"

#define LP_BASIS_RESTORE 0
#define DISABLE_LP_OUTPUT 1

#define DEBUG_RLP 1
#define DEBUG_RLP1 1
#define PRINT_RLP 1
#define PRINT_SLP_FAIL 1

#define DEBUG_JOIN 0
#define DEBUG_WIDEN 0
#define DEBUG_FPPOL 0
#define DEBUG_BT 1
#define PRINT_JWRES 0

#define TIME 0
#define LPTIME 0

#define MEETITV 0
#define MEET_TCONS_GENERIC 1

#define FM_SPLIT_SIZE 1000
/*-----------------------------------------------*/
/*                parameters                     */
/*-----------------------------------------------*/
/* scale glpk to avoid numerical instability*/
#define LP_SCALE 1
/* the program to be analyzed has integer coefficient */
#define INTPROGRAM 0
/* using standard widening with bounds rather than standard widening  */
#define WIDENBND 0

/* lazy redundancy_removal during fpp_meet_lincons_array and intersection */
/* but calling redundancy_removal before join and widening */
#define LAZY_RR 0

#define LP_EXACT 0

#define STRONG_JOIN 1

/*-----------------------------------------------*/
/*             join parameters                   */
/*-----------------------------------------------*/
/* using template join: minimum number of the complex polyhedron */
#define TEMPLATE_COMP_MIN 10
/* using template join: maximum number of the simple polyhedron */
#define TEMPLATE_SIMP_MAX 6

#ifdef __cplusplus
}
#endif

#endif
