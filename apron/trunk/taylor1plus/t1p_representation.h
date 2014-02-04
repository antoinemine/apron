/*
   APRON Library / Taylor1+ Domain (beta version)
   Copyright (C) 2009-2011 Khalil Ghorbal

*/


#ifndef _T1P_REPRESENTATION_H_
#define _T1P_REPRESENTATION_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Taylor1+ type */

//typedef struct _t1p_t {
//    t1p_aff_t** paf;              /* array of pointers to Taylor1+ expressions of size dims */
//    size_t intdim;                /* nb of integer variables */
//    size_t dims;                  /* intdim + realdim */
//    ap_dim_t* nsymcons;           /* array of index of constrained noise symbols */
//    ap_abstract0_t* abs;          /* nsym abstract object */
//} t1p_t;

t1p_t* t1p_alloc(ap_manager_t* man, size_t intdim, size_t realdim);

/* Return a copy of an abstract value, on
 * which destructive update does not affect the initial value. */
t1p_t* t1p_copy(ap_manager_t* man, t1p_t* a);

/* free all the memory used by abstract value */
void t1p_free(ap_manager_t* man, t1p_t* a);

size_t t1p_size(ap_manager_t* man, t1p_t* a);

/* ********************************************************************** */
/* 2. Control of internal representation */
/* ********************************************************************** */
void t1p_minimize(ap_manager_t* man, t1p_t* a);

void t1p_canonicalize(ap_manager_t* man, t1p_t* a);

int t1p_hash(ap_manager_t* man, t1p_t* a);

void t1p_approximate(ap_manager_t* man, t1p_t* a, int algorithm);

/* ********************************************************************** */
/* 3. Printing */
/* ********************************************************************** */
void t1p_fprint(FILE* stream,
		ap_manager_t* man,
		t1p_t* a,
		char** name_of_dim);

void t1p_fprintdiff(FILE* stream,
		ap_manager_t* man,
		t1p_t* a1, t1p_t* a2,
		char** name_of_dim);

void t1p_fdump(FILE* stream, ap_manager_t* man, t1p_t* a);

/* ********************************************************************** */
/* 4. Serialisation */
/* ********************************************************************** */
void t1p_serialize_raw(ap_manager_t* man, t1p_t* a);

void t1p_deserialize_raw(ap_manager_t* man, t1p_t* a);

#ifdef __cplusplus
}
#endif

#endif
