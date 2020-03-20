/*
   APRON Library / Taylor1+ Domain (beta version)
   Copyright (C) 2009-2011 Khalil Ghorbal

*/


#include "t1p_internal.h"
#include "t1p_representation.h"
#include "t1p_resize.h"

/* index of the newly inserted realdims should be greater than intdimensions of the abstract value*/
/*void check_dimchange(ap_dimchange_t* dimchange, t1p_t* a)
{
 if ((dimchange[dimchange->intdim - 1] <= a->intdim) && (dimchange->dim[dimchange->intdim] >= a->intdim)) return ;
 else {printf("check_dimchange\n"); abort();}
}
*/

/* Resize dimensions */
/*********************/
t1p_t* t1p_add_dimensions(ap_manager_t* man, bool destructive, t1p_t* a, ap_dimchange_t* dimchange, bool project)
{
    CALL();
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_ADD_DIMENSIONS);
#ifdef _T1P_DEBUG
    fprintf(stdout, "### ADD DiMENSIONS (destructive %d) (project %d)###\n",destructive, project);
    t1p_fprint(stdout, man, a, 0x0);
    ap_dimchange_fprint(stdout, dimchange);
    fprintf(stdout, "### ### ###\n");
#endif
    t1p_t* res = destructive ? a : t1p_copy(man, a);
    size_t intdim = a->intdim + dimchange->intdim;
    size_t dims = a->dims + (dimchange->intdim + dimchange->realdim);
    res->box = realloc(res->box,dims*sizeof(itv_t));
    res->paf = realloc(res->paf,dims*sizeof(t1p_aff_t*));
    size_t i = 0;
    int j = 0;
    for (i=0; i<dimchange->intdim + dimchange->realdim; i++) {
	if (res->dims == dimchange->dim[i]) {
	    /* add in the last place */
	    itv_init(res->box[res->dims]);
	} else {
	    /* increment */
	    for (j=(int)-1+res->dims;j>=(int)dimchange->dim[i];j--) {
		itv_init(res->box[j+1]);
		res->paf[j+1] = res->paf[j];
		itv_set(res->box[j+1],res->box[j]);
	    }
	}
	res->paf[dimchange->dim[i]] = project ? t1p_aff_alloc_init(pr) : pr->top;
	res->paf[dimchange->dim[i]]->pby++;
	if (project) itv_set_int(res->box[dimchange->dim[i]], 0);
	else itv_set_top(res->box[dimchange->dim[i]]);
	res->dims++;
    }
    res->intdim = intdim;
#ifdef _T1P_DEBUG
    fprintf(stdout, "### RESULT of ADD DiMENSIONS ###\n");
    t1p_fprint(stdout, man, res, 0x0);
    fprintf(stdout, "### ### ###\n");
#endif
    return res;
}

/* mimics box_remove_dimensions() */
/* fixing a bug reported by gavlegoat on Jun 11, 2019 */
t1p_t* t1p_remove_dimensions(ap_manager_t* man, bool destructive, t1p_t* a, ap_dimchange_t* dimchange)
{
    CALL();
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_REMOVE_DIMENSIONS);
#ifdef _T1P_DEBUG
    fprintf(stdout, "### REMOVE DiMENSIONS (destructive %d) ###\n",destructive);
    t1p_fprint(stdout, man, a, 0x0);
    ap_dimchange_fprint(stdout, dimchange);
    fprintf(stdout, "### ### ###\n");
#endif
    t1p_t* res;
    size_t size;
    size_t dimsup;
    size_t i,k;
    t1p_aff_t* tmp;
    man->result.flag_best = true;
    man->result.flag_exact = true;
    res = destructive ? a : t1p_copy(man, a);

    if (a->paf==NULL){
        goto t1p_remove_dimensions_exit;
    }
    size = res->dims;
    dimsup = dimchange->intdim+dimchange->realdim;
    k=0;
    for (i=0; i<size-dimsup; i++){
        while (k<dimsup && dimchange->dim[k]==i+k){
            k++;
        }
        tmp = res->paf[i]; 
        res->paf[i] = res->paf[i+k];
        res->paf[i+k] = tmp;
        itv_set(res->box[i],res->box[i+k]);
    }
    //??itv_set_int(res->p[size-dimsup],0);
    for (i=size-dimsup;i<size;i++){
        t1p_aff_check_free(pr,res->paf[i]);
        res->paf[i] = NULL;
    }
    res->box = realloc(res->box,(size-dimsup)*sizeof(itv_t));
    res->paf = realloc(res->paf,(size-dimsup)*sizeof(t1p_aff_t*));
t1p_remove_dimensions_exit:
    res->intdim = a->intdim - dimchange->intdim;
    res->dims = a->dims - (dimchange->intdim + dimchange->realdim);
#ifdef _T1P_DEBUG
    fprintf(stdout, "### RESULT of REMOVE DiMENSIONS ###\n");
    t1p_fprint(stdout, man, res, 0x0);
    fprintf(stdout, "### ### ###\n");
#endif
    return res;
}

t1p_t* t1p_permute_dimensions(ap_manager_t* man, bool destructive, t1p_t* a, ap_dimperm_t* permutation)
{
    CALL();
    t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_PERMUTE_DIMENSIONS);
#ifdef _T1P_DEBUG
    fprintf(stdout, "### PERMUTE DiMENSIONS (destructive %d) ###\n",destructive);
    t1p_fprint(stdout, man, a, 0x0);
    ap_dimperm_fprint(stdout, permutation);
    fprintf(stdout, "### ### ###\n");
#endif
    t1p_aff_t* tmp = NULL;
    t1p_t* res = destructive ? a : t1p_copy(man, a);
    size_t i = 0;
    for (i=0; i<permutation->size; i++) {
	tmp = res->paf[i];
	res->paf[i] = res->paf[permutation->dim[i]];
	res->paf[permutation->dim[i]] = tmp;
	itv_swap(res->box[i],res->box[permutation->dim[i]]);
    }
#ifdef _T1P_DEBUG
    fprintf(stdout, "### RESULT of PERMUTE DiMENSIONS ###\n");
    t1p_fprint(stdout, man, res, 0x0);
    fprintf(stdout, "### ### ###\n");
#endif
    return res;
}

