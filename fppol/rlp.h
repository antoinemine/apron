/*
 * rlp.h
 *
 * Functions for rigorous linear programming.
 *
 * APRON Library / FPPol Domain
 *
 * Copyright (C) Liqian CHEN' 2008-
 *
 */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution.
 */

#ifndef _RLP_H_
#define _RLP_H_

#include <glpk.h> /* linear programming lib */
#include "fpp.h"
#include "fpp_internal.h"

#ifdef __cplusplus
extern "C" {
#endif


void lp_delete( glp_prob *lp );

/* rigorous linear programming */
glp_prob *rlp_create(fpp_t *fpp, unsigned objType, numdbl_t *objVector);
glp_prob *rlp_create_without_KthCons(fpp_t *fpp, unsigned objType, numdbl_t *objVector, int k);
glp_prob *rlp_solve(glp_prob *lp);
numdbl_t rlp_get_rigorous_optimal_value_dual(glp_prob *lp, numdbl_t *bounds);
numdbl_t rlp_get_optimal_value(glp_prob *lp, numdbl_t *bounds, bool eps, bool *nearly);
void rlp_set_objective(glp_prob *lp, unsigned objType, numdbl_t *objVector );
glp_prob *rlp_create_matrix(fpp_t *fpp);

/* ---Christian Jansson's rigorous LP ----- */
numdbl_t rlp_perturbed_dual(glp_prob *lp, numdbl_t *bnds, numdbl_t *y, numdbl_t *r_down, numdbl_t *r_up, numdbl_t *c, numdbl_t *eps, int *iters);
numdbl_t rlp_process_free_vars(glp_prob *lp, numdbl_t *bnds, numdbl_t *y, numdbl_t *c, int *indices, int m);
numdbl_t* rump_method(numdbl_t *A, numdbl_t *b, int m);
bool matrix_inverse(numdbl_t *A, int n);
numdbl_t* matrix_X_vector(numdbl_t *A, numdbl_t *b, int m);
numdbl_t* matrix_X_vector_itv(numdbl_t *A, numdbl_t *b, bool Aitv, bool bitv, int m);
numdbl_t* matrix_X_matrix_itv(numdbl_t *A, numdbl_t *B, bool Aitv, bool Bitv, int m);
/* -------- */

/* standard linear programming */
glp_prob *slp_create(fpp_t *fpp, unsigned objType, numdbl_t *objVector );
glp_prob *slp_create_without_KthCons(fpp_t *fpp, unsigned objType, numdbl_t *objVector,int k);
numdbl_t slp_get_optimal_value(glp_prob *lp);
glp_prob *slp_solve(glp_prob *lp,glp_smcp *parm);
glp_prob *slp_create_matrix(fpp_t *fpp );
void slp_set_objective(glp_prob *lp, unsigned objType, numdbl_t *objVector );
void slp_disable_kthRow(glp_prob *lp,unsigned k, numdbl_t c);
void slp_set_upbnd_kthRow(glp_prob *lp,unsigned k,numdbl_t c);
void slp_del_kthRow(glp_prob *lp,unsigned k );

glp_prob *slp_create_matrix_withBnds(fpp_t *fpp, int ndimBnds);

void slp_restore_kthRow(glp_prob *lp,unsigned k,numdbl_t *con);
void slp_filter_kthRow(glp_prob *lp,unsigned k, numdbl_t c);

numdbl_t slp_exact(fpp_t *fpp, unsigned objType, numdbl_t *objVector);
numdbl_t slp_get_optimal_value_exact(glp_prob *lp, unsigned objType, numdbl_t *objVector);

#ifdef __cplusplus
}
#endif

#endif
