/* ********************************************************************** */
/* itv_types.h: all kind of intervals and general workspace */
/* ********************************************************************** */

#ifndef _ITV_TYPES_H_
#define _ITV_TYPES_H_

#include "itvIl_types.h"
#include "itvIll_types.h"
#include "itvMPZ_types.h"
#include "itvRl_types.h"
#include "itvRll_types.h"
#include "itvMPQ_types.h"
#include "itvD_types.h"
#include "itvDl_types.h"
#include "itvMPFR_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* Workspace */
/* ********************************************************************** */

/* Workspace to avoid temporary allocation and deallocation when numXXX_t and
   boundXXX_t are multiprecision numbers */

typedef struct __itv_internal_struct {
  num_internal_t num;
  itvIl_internal_t Il;
  itvIl_internal_t Ill;
  itvIl_internal_t MPZ;
  itvIl_internal_t Rl;
  itvIl_internal_t Rll;
  itvIl_internal_t MPQ;
  itvIl_internal_t D;
  itvIl_internal_t Dl;
  itvIl_internal_t MPFR;
}
typedef __itv_internal_struct* itv_internal_ptr;
typedef __itv_internal_struct itv_internal_t[1];

itv_internal_ptr itv_internal_alloc(void);
  /* Allocate and initialize internal workspace */
void itv_internal_free(itv_internal_ptr intern);
  /* Clear and free internal workspace */

void itv_internal_init(itv_internal_t intern);
void itv_internal_clear(itv_internal_t intern);
  /* GMP Semantics */

#ifdef __cplusplus
}
#endif

#endif
