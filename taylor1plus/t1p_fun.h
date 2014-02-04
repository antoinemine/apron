/*
   APRON Library / Taylor1+ Domain (beta version)
   Copyright (C) 2009-2011 Khalil Ghorbal

*/


#ifndef _T1P_FUN_H_
#define _T1P_FUN_H_

/*
#include "t1p_internal.h"
#include "ap_texpr0.h"
#include "t1p_representation.h"
*/

#ifdef __cplusplus
extern "C" {
#endif

t1p_aff_t* t1p_aff_eval_ap_texpr0(t1p_internal_t* pr, ap_texpr0_t* expr, t1p_t* env);

t1p_aff_t* t1p_aff_eval_node(t1p_internal_t* pr, ap_texpr0_node_t* node, t1p_t* env);

t1p_aff_t* t1p_aff_eval_node_unary (t1p_internal_t* pr, ap_texpr0_node_t* node, t1p_t* env);

t1p_aff_t* t1p_afexpr_eval_node_binary (t1p_internal_t* pr, ap_texpr0_node_t* node, t1p_t* env);

t1p_aff_t* t1p_aff_neg(t1p_internal_t* pr, t1p_aff_t* b);
void t1p_aff_neg_inplace(t1p_internal_t* pr, t1p_aff_t* b);

//t1p_aff_t* t1p_aff_add(t1p_internal_t* pr, t1p_aff_t* a, t1p_aff_t* b);
t1p_aff_t* t1p_aff_add(t1p_internal_t* pr, t1p_aff_t* exprA, t1p_aff_t* exprB, t1p_t* abs);

t1p_aff_t* t1p_aff_sub (t1p_internal_t* pr, t1p_aff_t* a, t1p_aff_t* b, t1p_t* abs);

/*
void t1p_aff_sub_pi(t1p_internal_t* pr,
		t1p_aff_t res,
		t1p_aff_t exprA,
		t1p_aff_t exprB);

void t1p_aff_sub_pi_bis(t1p_internal_t* pr,
		t1p_aff_t res,
		t1p_aff_t exprA,
		t1p_aff_t exprB);
*/

t1p_aff_t* t1p_aff_mul(t1p_internal_t* pr, t1p_aff_t* a, t1p_aff_t* b, t1p_t* env);
t1p_aff_t* t1p_aff_mul_non_constrained(t1p_internal_t* pr, t1p_aff_t* exprA, t1p_aff_t* exprB, t1p_t* env);
t1p_aff_t* t1p_aff_mul_constrained(t1p_internal_t* pr, t1p_aff_t* exprA, t1p_aff_t* exprB, t1p_t* env);
t1p_aff_t* t1p_aff_mul_itv(t1p_internal_t* pr, t1p_aff_t* src, itv_t lambda);
void t1p_aff_mul_itv_inplace(t1p_internal_t* pr, t1p_aff_t* exprA, itv_t lambda);

t1p_aff_t* t1p_aff_div(t1p_internal_t* pr, t1p_aff_t* a, t1p_aff_t* b, t1p_t* env);

t1p_aff_t* t1p_aff_sqrt(t1p_internal_t* pr, t1p_aff_t* a, t1p_t* env);

t1p_aff_t* t1p_aff_mod(t1p_internal_t* pr, t1p_aff_t* a, t1p_aff_t* b);

//void square_dep(itv_internal_t* itv, itv_t res, t1p_aaterm_t* p, t1p_aaterm_t* q);
void square_dep(t1p_internal_t* pr, itv_t res, t1p_aaterm_t* p, t1p_aaterm_t* q, int* hash, unsigned int dim, itv_t* gamma);
bool call_sdp(itv_internal_t* itv, itv_t res, t1p_aaterm_t* p, t1p_aaterm_t* q, size_t dim, int* hash, bool square);

void buildIntervalUpperTriangle(t1p_internal_t* pr, itv_t* array, t1p_aaterm_t* p, t1p_aaterm_t* q, size_t dim, int* hash);
double* buildDoubleUpperTriangle(t1p_internal_t* pr, itv_t* array, size_t dim);

#ifdef __cplusplus
}
#endif

#endif
