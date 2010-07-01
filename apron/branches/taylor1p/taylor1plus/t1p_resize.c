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
/*
{
	t1p_internal_t* pr = t1p_init_from_manager(man, AP_FUNID_ADD_DIMENSIONS);
	t1p_t* res;
	size_t i,j;
	size_t intdim, realdim, nbdims;

	intdim = a->intdim + dimchange->intdim;
	realdim = (a->dims -  a->intdim) + dimchange->realdim;
	res = t1p_copy(man, a);
	nbdims = intdim + realdim;
	arg_assert(!(((dimchange->intdim + dimchange->realdim) != 1) && (dimchange->dim[0] > a->dims)),abort(););
	for (i=0; i<(dimchange->intdim + dimchange->realdim); i++) {
		arg_assert(!((dimchange->dim[i] > dimchange->dim[i+1]) || (dimchange->dim[i] > a->dims)), abort(););
	}
	size_t* index = (size_t*)malloc((a->dims)*sizeof(size_t));
	for (i=0; i<a->dims; i++) {
		index[i] = i;
	}
	for (i=0; i<(dimchange->intdim + dimchange->realdim); i++) {
		dimchange->dim[i+1] ++;
		for(j=0; j<a->dims; j++) {
			if (index[j] >= dimchange->dim[i]) index[j]++ ;
		}
	}
	for (j=0; j<a->dims; j++) {
		t1p_aaexpr_set(&res->aa[index[j]], &a->aa[j]);
		itv_set(res->itv[index[j]], a->itv[j]);
	}
	free(index);
	arg_assert(project, abort(););
	for (i=0; i<(dimchange->intdim + dimchange->realdim); i++) {
		itv_set_top(res->itv[dimchange->dim[i]]);
		t1p_aaexpr_set_top(&res->aa[dimchange->dim[i]]);
	}
	res->intdim = intdim ;
	res->dims = nbdims ;

	man->result.flag_best = tbool_true;
	man->result.flag_exact = tbool_true;
	return res;
}
*/

bool is_in(size_t i, ap_dimchange_t* dimchange) 
{
	size_t j = 0;
	while (j<(dimchange->intdim + dimchange->realdim)) {
		if (dimchange->dim[j] != i) j++; 
		else return true;
	}
	return false;
}
 
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
    t1p_t* res = destructive ? a : t1p_copy(man, a);
    size_t i = 0;
    size_t j = 0;
    for (i=0; i<dimchange->intdim + dimchange->realdim; i++) {
	t1p_aff_check_free(pr,res->paf[dimchange->dim[i]]);
	res->paf[dimchange->dim[i]] = NULL;
	for (j=dimchange->dim[i];j<-1+res->dims;j++) {
	    res->paf[j] = res->paf[j+1];
	    itv_set(res->box[j],res->box[j+1]);
	}
    }
    res->intdim = a->intdim - dimchange->intdim;
    res->dims = a->dims - (dimchange->intdim + dimchange->realdim);
    res->box = realloc(res->box,res->dims*sizeof(itv_t));
    res->paf = realloc(res->paf,res->dims*sizeof(t1p_aff_t*));
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

