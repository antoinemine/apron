/*
   APRON Library / Taylor1+ Domain (beta version)
   Copyright (C) 2009-2011 Khalil Ghorbal

*/


/* ********************************************************************** */
/* t1p_representation.c: general management */
/* ********************************************************************** */

#include "t1p_internal.h"
//#include "t1p_representation.h"
#include "t1p_constructor.h"
#include "ap_manager.h"
#include "ap_lincons0.h"

/* ********************************************************************** */
/* 1. Memory */
/* ********************************************************************** */

/* memory allocation of Taylor1+ structure */
t1p_t* t1p_alloc(ap_manager_t* man, size_t intdim, size_t realdim)
{
    CALL();
    t1p_internal_t * pr = t1p_init_from_manager(man, AP_FUNID_UNKNOWN);
    t1p_t* res;
    checked_malloc(res, t1p_t, 1, abort(););
    res->intdim = intdim;
    res->dims = intdim + realdim;
    res->size = 128;
    res->nsymcons = (ap_dim_t*)calloc(res->size, sizeof(ap_dim_t));
    res->gamma = (ap_interval_t**)calloc(res->size, sizeof(ap_interval_t*));
    res->abs = ap_abstract0_top(pr->manNS, 0, 0);
    res->hypercube = true;
    res->box = itv_array_alloc(intdim + realdim);
    res->gn = 0;
    res->g = NULL;
    checked_malloc(res->paf, t1p_aff_t*, res->dims, abort(););
#ifdef _T1P_DEBUG
//    fprintf(stdout, "********* alloc %tx*********\n",(intptr_t)res);
#endif
    return res;
}

/* Return a copy of an abstract value, on
 * which destructive update does not affect the initial value. */
t1p_t* t1p_copy(ap_manager_t* man, t1p_t* a)
{
    CALL();
    t1p_t* res;
    size_t i;
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_COPY);
    arg_assert(a, abort(););
    res = t1p_alloc(man, a->intdim, (a->dims - a->intdim));
    memcpy((void *)res->paf, (void *)a->paf, a->dims*sizeof(t1p_aff_t*));
    for(i=0; i<a->dims; i++) {
	itv_set(res->box[i], a->box[i]);
	res->paf[i]->pby++;
    }
    ap_abstract0_free(pr->manNS, res->abs);
    res->abs = ap_abstract0_copy(pr->manNS, a->abs);
    res->size = a->size;
    memcpy((void *)res->nsymcons, (void *)a->nsymcons, (res->size)*sizeof(ap_dim_t));
    //memcpy((void *)res->gamma, (void *)a->gamma, (res->size)*sizeof(ap_interval_t*));

    size_t nsymcons_size = t1p_nsymcons_get_dimension(pr, a);
    for (i=0; i<nsymcons_size; i++) {
	if (a->gamma[i]) {
	    if (a->gamma[i] != pr->ap_muu) res->gamma[i] = ap_interval_alloc_set(a->gamma[i]);
	    else res->gamma[i] = pr->ap_muu;
	} else {
	    fatal("t1p_copy, unconsistent gamma for T1+ abstract object\n");
	}
    }
    res->hypercube = a->hypercube;
    man->result.flag_best = tbool_true;
    man->result.flag_exact = tbool_true;
#ifdef _T1P_DEBUG
    fprintf(stdout, "********* %tx -> %tx *********\n",(intptr_t)a, (intptr_t)res);
#endif
    return res;
}

/* free memory used by abstract value */
void t1p_free(ap_manager_t* man, t1p_t* a)
{
    CALL();
#ifdef _T1P_DEBUG
  //  fprintf(stdout, "********* free %tx*********\n",(intptr_t)a);
#endif
    t1p_internal_t * pr = t1p_init_from_manager(man, AP_FUNID_FREE);
    arg_assert(a,abort(););
    size_t i = 0;
    for (i=0; i<a->dims; i++) {
	if (a->paf[i]) {
	    t1p_aff_check_free(pr, a->paf[i]);
	    a->paf[i] = NULL;
        }
	itv_clear(a->box[i]);
    }
    free(a->paf);
    free(a->box);
    a->paf = NULL;
    a->box = NULL;
    size_t nsymcons_size = t1p_nsymcons_get_dimension(pr, a);
    for (i=0; i<nsymcons_size; i++) {
	if (a->gamma[i]) {
	    if (a->gamma[i] != pr->ap_muu) ap_interval_free(a->gamma[i]);
	}
    }
    free(a->gamma);
    a->gamma = NULL;
    free(a->nsymcons);
    a->nsymcons = NULL;
    ap_abstract0_free(pr->manNS, a->abs);
    a->size = 0;
    a->dims = 0;
    a->intdim = 0;
    free(a);
    man->result.flag_best = tbool_true;
    man->result.flag_exact = tbool_true;
}

/* size of the abstract value
 * TODO: la taille de quoi exactement ?
 */
size_t t1p_size(ap_manager_t* man, t1p_t* a)
{
    CALL();
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_SIZE);
    //        not_implemented();
    size_t size = 0;
    size_t i = 0;
    if (t1p_is_bottom(man, a)) return ap_abstract0_size(pr->manNS, a->abs);
    else {
	for (i=0; i<a->dims; i++) {
	    if (a->paf[i]) size += size + 2*(a->paf[i]->l + 1);
	}
    }
    return size + ap_abstract0_size(pr->manNS, a->abs);
}

/* ********************************************************************** */
/* 2. Control of internal representation */
/* ********************************************************************** */
void t1p_minimize(ap_manager_t* man, t1p_t* a)
{
    CALL();
	t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_MINIMIZE);
	not_implemented();
}

/* Put the abstract value in canonical form. (supress zero coeff from the affine form) */
void t1p_canonicalize(ap_manager_t* man, t1p_t* a)
{
    CALL();
	t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_CANONICALIZE);
	not_implemented();
}

/* Return an hash code */
int t1p_hash(ap_manager_t* man, t1p_t* a)
{
    int i,dec,size,res;
    size = a->dims;
    res = size * 2999;

    if (a->box!=NULL){
	for (i=0; i<size; i += (size+4)/5){
	    res = 3*res + itv_hash(a->box[i]);
	}
    }
    man->result.flag_best = true;
    man->result.flag_exact = true;
    return res;
}
//{
    //CALL();
//	t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_HASH);
//	not_implemented();
//}

/* Perform some transformation on the abstract value, guided by the field algorithm.
 *
 * The transformation may lose information.  The argument "algorithm"
 * overrides the field algorithm of the structure of type ap_funopt_t
 * associated to ap_abstract0_approximate (commodity feature). */
void t1p_approximate(ap_manager_t* man, t1p_t* a, int algorithm)
{
    CALL();
	t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_APPROXIMATE);
	not_implemented();
}


/* ********************************************************************** */
/* 3. Printing */
/* ********************************************************************** */
/* Print the abstract value in a pretty way, using function name_of_dim to name dimensions */
/* TODO: LOGGING */
/* The analyzer print the abstract value when a fix point is reached at the end of the analysis.
 * We may use this to logg the abstract value at each control point of the program... still we don't have any information about the control points since this is totally hidden for the domain.
*/
void t1p_fprint(FILE* stream,
		ap_manager_t* man,
		t1p_t* a,
		char** name_of_dim)
{
    CALL();
    t1p_internal_t* pr = t1p_init_from_manager(man,AP_FUNID_FPRINT);
    size_t i = 0;
    fprintf(stream,"__________T1+ abstract__________\n");
    if (t1p_is_bottom(man, a)) fprintf(stream,"bottom\n");
    else if (t1p_is_top(man, a)) fprintf(stream,"top\n");
    else {
	for (i=0; i<a->dims; i++) {
	    if (a->paf[i]) {
#ifdef _T1P_DEBUG
    fprintf(stdout, "[[pby %u]]   ",a->paf[i]->pby);
#endif
	    if (name_of_dim) {
		fprintf(stream, "%s", name_of_dim[i]);
	    } else {
              fprintf(stream, "(%llu)", (unsigned long long)i);
	    }
	    fprintf(stream, " := ");
	    t1p_aff_fprint(pr, stream, a->paf[i]);
	    itv_fprint(stdout, a->box[i]);
	    fprintf(stream,"\n");
	} else {
	    fprintf(stream, "[[NULL]]\n");
	}
	}
    }
    size_t size = t1p_nsymcons_get_dimension(pr, a);
    char** name_of_ns = (char**)malloc(size*sizeof(char*));
    ap_dim_t dim = 0;
    for (i=0; i<size; i++) {
	name_of_ns[i] = (char*)malloc(10);
	pr->epsilon[a->nsymcons[i]]->type == IN ? snprintf(name_of_ns[i], 10, "eps%d", a->nsymcons[i]) : snprintf(name_of_ns[i], 10, "eta%d", a->nsymcons[i]);
    }
    ap_abstract0_fprint(stream, pr->manNS, a->abs, name_of_ns);
    for (i=0; i<size; i++) free(name_of_ns[i]);
    free(name_of_ns);
    name_of_ns = NULL;
    fprintf(stream,"__________\n");
    fflush(stream);
    man->result.flag_best = tbool_true;
    man->result.flag_exact = tbool_true;
}

void t1p_fprintdiff(FILE* stream,
		ap_manager_t* man,
		t1p_t* a1, t1p_t* a2,
		char** name_of_dim)
{
	t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_FPRINTDIFF);
	not_implemented();
}

void t1p_fdump(FILE* stream, ap_manager_t* man, t1p_t* a)
{
	CALL();
	t1p_internal_t* pr = t1p_init_from_manager(man,AP_FUNID_FDUMP);
	size_t i = 0;
#ifdef _T1P_DEBUG
	fprintf(stdout,"[[[___");
#endif
	for (i=0; i<a->dims; i++) {
          fprintf(stream, "%llu: ", (unsigned long long)i);
		fprintf(stream,"*** pby %u ***",a->paf[i]->pby);
		t1p_aff_fprint(pr, stream, a->paf[i]);
		fprintf(stream," ; \n");
	}
#ifdef _T1P_DEBUG
	fprintf(stdout,"___]]] \n");
#endif
	fflush(stream);
	man->result.flag_best = tbool_true;
	man->result.flag_exact = tbool_true;
}


/* ********************************************************************** */
/* 4. Serialisation */
/* ********************************************************************** */
void t1p_serialize_raw(ap_manager_t* man, t1p_t* a)
{
    CALL();
	t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_SERIALIZE_RAW);
	not_implemented();
}

void t1p_deserialize_raw(ap_manager_t* man, t1p_t* a)
{
    CALL();
	t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_DESERIALIZE_RAW);
	not_implemented();
}
