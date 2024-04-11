/* ********************************************************************** */
/* pk_representation.h: General management of polyhedra  */
/* ********************************************************************** */

/* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.
   
   Please read the COPYING file packaged in the distribution */

#ifndef _PK_REPRESENTATION_H_
#define _PK_REPRESENTATION_H_

#include "pk.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************** */
/* I. Memory */
/* ********************************************************************** */

/* Allocates a polyedron and fills its structure with null values, which
   corresponds to a bottom polyhedron. */
pk_t* poly_alloc(size_t intdim, size_t realdim);

/* Free all the members of the polyhedron structure (GMP semantics) */
void poly_clear(pk_t* po);

/*  Assignement with GMP semantics */
void poly_set(pk_t* pa, pk_t* pb);

/* ********************************************************************** */
/* II. Control of internal representation */
/* ********************************************************************** */

/* Minimization function, in the sense of minimized dual representation This
   function minimizes if not already done the given polyhedron.
   Raise an exception, but still transmit it (pk->exn not reseted).
*/
void poly_chernikova(ap_manager_t* man, pk_t* poly, char* msg);

/* Same as poly_chernikova, but in addition ensure normalized epsilon
   constraints. */
void poly_chernikova2(ap_manager_t* man, pk_t* poly, char* msg);

/* Same as poly_chernikova2, but in addition normalize matrices by Gauss
   elimination and sorting */
void poly_chernikova3(ap_manager_t* man, pk_t* poly, char* msg);

/* Put the polyhedron with minimized constraints and frames.  If in addition
   the integer man->option->canonicalize.algorithm is strictly positive,
   normalize equalities and lines, and also strict constraints */
void poly_canonicalize(ap_manager_t* man, pk_t* poly);

/* Has the polyhedron normalized strict constraints ? */
static inline bool poly_is_conseps(pk_internal_t* pk, pk_t* po);

/* Is the polyhedron under (strong) normal form ? */
bool pk_is_canonical(ap_manager_t* man, pk_t* po);

/* Make available the matrix of constraints (resp. frames). The matrix will
   remain unavailable iff the polyhedron appears to be empty */
static inline void poly_obtain_C(ap_manager_t* man, pk_t* po, char* msg);
static inline void poly_obtain_F(ap_manager_t* man, pk_t* po, char* msg);

/* Assuming the the matrix of constraints (resp. frames) is available, sort it,
   and take care of the saturation matrices. */
void poly_obtain_sorted_C(pk_internal_t* pk, pk_t* poly);
void poly_obtain_sorted_F(pk_internal_t* pk, pk_t* poly);

/* Assuming one of the saturation matrix is available, make satC (resp. satF)
   available. */
static inline void poly_obtain_satC(pk_t* poly);
static inline void poly_obtain_satF(pk_t* poly);

/* Exchange C and F, sat C and satF, nbeq and nbline */
static inline void poly_dual(pk_t* po);

/* Like poly_chernikova, etc, but if usual is false, take into account the fact that
   poly_dual has been applied */ 
void poly_chernikova_dual(ap_manager_t* man,
			  pk_t* poly,
			  char* msg,
			  bool usual);
static inline void poly_obtain_C_dual(ap_manager_t* man,pk_t* po, char* msg, bool usual);
static inline void poly_obtain_F_dual(ap_manager_t* man,pk_t* po, char* msg, bool usual);

/* ********************************************************************** */
/* V. Checking */
/* ********************************************************************** */

/* Checks gauss elimination */
bool pk_matrix_check_gauss(pk_matrix_t* mat, size_t nbeq);

/* Perform rather detailed and costly checks on a polyhedron, to detect
   inconsistencies */
bool poly_check(pk_internal_t* pk, pk_t* po);
/* Same as previous function, but usual==false mean that poly_dual has been
   applied */
bool poly_check_dual(pk_internal_t* pk, pk_t* po, bool usual);

/* ********************************************************************** */
/* ********************************************************************** */
/* INLINE DEFINITIONS */
/* ********************************************************************** */

static inline bool poly_is_conseps(pk_internal_t* pk, pk_t* po)
{
  return (!pk->strict || 
	  po->realdim==0 || 
	  (po->status & pk_status_conseps));;
}
  static inline bool poly_is_minimaleps(pk_internal_t* pk, pk_t* po)
{
  return (!pk->strict ||
	  po->realdim==0 ||
	  (po->status & pk_status_minimaleps));
}

static inline void poly_obtain_C(ap_manager_t* man, pk_t* po, char* msg)
{
  if (!po->C) poly_chernikova(man,po,msg);
}
static inline void poly_obtain_F(ap_manager_t* man, pk_t* po, char* msg)
{
  if (!po->F) poly_chernikova(man,po,msg);
}
static inline void poly_obtain_satF(pk_t* poly)
{
  if (!poly->satF){
    assert(poly->C && poly->satC);
    poly->satF = satmat_transpose(poly->satC,poly->C->nbrows);
  }
}
static inline void poly_obtain_satC(pk_t* poly)
{
  if (!poly->satC){
    assert(poly->F && poly->satF);
    poly->satC = satmat_transpose(poly->satF,poly->F->nbrows);
  }
}

static inline void poly_dual(pk_t* po)
{
  void* ptr;
  size_t nb;
  ptr = po->C; po->C = po->F; po->F = ptr;
  ptr = po->satC; po->satC = po->satF; po->satF = ptr;
  nb = po->nbeq; po->nbeq = po->nbline; po->nbline = nb;
}

static inline void poly_obtain_C_dual(ap_manager_t* man, pk_t* po, char* msg, bool usual)
{
  if (!po->C) poly_chernikova_dual(man,po,msg,usual);
}
static inline void poly_obtain_F_dual(ap_manager_t* man, pk_t* po, char* msg, bool usual)
{
  if (!po->F) poly_chernikova_dual(man,po,msg,usual);
}


#ifdef __cplusplus
}
#endif

#endif
